#include "i51.h"
#include "i51KitAde.h"
#include "jzip.h"

#ifndef NULL
#define NULL               0
#endif

static int decodeDynamicHuffmanTables (InflaterState * state,
									   void **lcodesPtr, void **dcodesPtr);

static int makeCodeTable (InflaterState * state,
						  unsigned char *codelen,
						  unsigned numElems,
						  unsigned maxQuickBits, void **result);

static int inflateHuffman (InflaterState * state, int fixedHuffman);
static int inflateStored (InflaterState * state);

#define INFLATER_EXTRA_BYTES 4

int zip_inflateData (FileObj * fileObj, HeapManObj * heapManObj, int compLen,
					 unsigned char *decompBuffer, int decompLen,
					 int bufferIsAHandle)
{

	static InflaterState stateStruct;
	InflaterState *state = &stateStruct;
	int result = 0;

	state->outBuffer = decompBuffer;
	state->outOffset = 0;
	state->outLength = decompLen;
	state->outBufferIsAHandle = bufferIsAHandle;

	state->fileState = fileObj->state;
	state->getBytes = fileObj->read;

	state->heapState = heapManObj->state;
	state->mallocBytes = heapManObj->alloc;
	state->freeBytes = heapManObj->free;
	state->addrFromHandle = heapManObj->addrFromHandle;

	state->inData = 0;
	state->inDataSize = 0;
	state->inRemaining = compLen + INFLATER_EXTRA_BYTES;

	state->inflateBufferIndex = 0;
	state->inflateBufferCount = 0;

	for (;;)
	{
		int type;
		DECLARE_IN_VARIABLES LOAD_IN;
		NEEDBITS (3);
		type = NEXTBITS (3);
		DUMPBITS (3);
		STORE_IN;

		switch (type >> 1)
		{
		default:
		case BTYPE_INVALID:
			result = INFLATE_INVALID_BTYPE;
			break;

		case BTYPE_NO_COMPRESSION:
			result = inflateStored (state);
			break;

		case BTYPE_FIXED_HUFFMAN:
			result = inflateHuffman (state, 1);
			break;

		case BTYPE_DYNA_HUFFMAN:
			result = inflateHuffman (state, 0);
			break;
		}

		if (result != 0)
		{
			break;
		}

		if (type & 1)
		{
			if (state->inRemaining + (state->inDataSize >> 3) !=
				INFLATER_EXTRA_BYTES)
			{
				result = INFLATE_INPUT_BIT_ERROR;
				break;
			}

			if (state->outOffset != state->outLength)
			{
				result = INFLATE_OUTPUT_BIT_ERROR;
				break;
			}
			break;
		}
	}

	return result;
}

static int inflateStored (InflaterState * state)
{
	DECLARE_IN_VARIABLES DECLARE_OUT_VARIABLES long len, nlen;

	LOAD_IN;
	LOAD_OUT;

	DUMPBITS (inDataSize & 7);
	NEEDBITS (32) len = NEXTBITS (16);
	DUMPBITS (16);
	nlen = NEXTBITS (16);
	DUMPBITS (16);

	ASSERT (inDataSize == 0);

	if (len + nlen != 0xFFFF)
	{
		return INFLATE_BAD_LENGTH_FIELD;
	}
	else if (inRemaining < len)
	{
		return INFLATE_INPUT_OVERFLOW;
	}
	else if (outOffset + len > outLength)
	{
		return INFLATE_OUTPUT_OVERFLOW;
	}
	else
	{
		int count;

		if (state->outBufferIsAHandle)
		{

			outBuffer = state->addrFromHandle (state->heapState,
				state->outBuffer);
		}

		while (len > 0)
		{
			if (state->inflateBufferCount > 0)
			{

				memcpy (&outBuffer[outOffset],
					&(state->inflateBuffer[state->inflateBufferIndex]),
					(count = (state->inflateBufferCount <= len ?
					state->inflateBufferCount : len)));
				len -= count;
				(state->inflateBufferCount) -= count;
				(state->inflateBufferIndex) += count;
				outOffset += count;
				inRemaining -= count;
			}

			if (len > 0)
			{

				outBuffer[outOffset++] = NEXTBYTE;
				len--;
				inRemaining--;
			}
		}
	}

	STORE_IN;
	STORE_OUT;
	return 0;
}

static int inflateHuffman (InflaterState * state, int fixedHuffman)
{
	int error = 0;
	DECLARE_IN_VARIABLES DECLARE_OUT_VARIABLES unsigned int quickDataSize = 0;
	unsigned int quickDistanceSize = 0;
	static unsigned int code;
	static unsigned int litxlen;
	static void *lcodesMemHandle;
	static void *dcodesMemHandle;
	HuffmanCodeTable *lcodes = NULL;
	HuffmanCodeTable *dcodes = NULL;

	if (!fixedHuffman)
	{
		error = decodeDynamicHuffmanTables (state, &lcodesMemHandle,
			&dcodesMemHandle);
		if (error != 0)
		{
			return error;
		}


		lcodes = state->addrFromHandle (state->heapState, lcodesMemHandle);
		dcodes = state->addrFromHandle (state->heapState, dcodesMemHandle);

		quickDataSize = lcodes->h.quickBits;
		quickDistanceSize = dcodes->h.quickBits;
	}

	LOAD_IN;
	LOAD_OUT;

	if (state->outBufferIsAHandle)
	{

		outBuffer = state->addrFromHandle (state->heapState, state->outBuffer);
	}

	for (;;)
	{
		if (inRemaining < 0)
		{
			error = INFLATE_EARLY_END_OF_INPUT;
			break;
		}

		NEEDBITS (MAX_BITS + MAX_ZIP_EXTRA_LENGTH_BITS);

		if (fixedHuffman)
		{



			code = NEXTBITS (9);
			code = REVERSE_9BITS (code);

			if (code < 0x060)
			{

				DUMPBITS (7);
				litxlen = 0x100 + (code >> 2);
			}
			else if (code < 0x190)
			{
				DUMPBITS (8);
				litxlen = (code >> 1) + ((code < 0x180) ? (0x000 - 0x030)
					: (0x118 - 0x0c0));
			}
			else
			{
				DUMPBITS (9);
				litxlen = 0x90 + code - 0x190;
			}
		}
		else
		{
			GET_HUFFMAN_ENTRY (lcodes, quickDataSize, litxlen);
		}

		if (litxlen <= 255)
		{
			if (outOffset < outLength)
			{
				outBuffer[outOffset] = litxlen;
				outOffset++;
			}
			else
			{

				break;
			}
		}
		else if (litxlen == 256)
		{

			break;
		}
		else if (litxlen > 285)
		{
			error = INFLATE_INVALID_LITERAL_OR_LENGTH;
			break;
		}
		else
		{
			unsigned int n = litxlen - LITXLEN_BASE;
			unsigned int length = ll_length_base[n];
			unsigned int moreBits = ll_extra_bits[n];
			static unsigned int d0, distance;


			length += NEXTBITS (moreBits);
			DUMPBITS (moreBits);

			NEEDBITS (MAX_BITS);
			if (fixedHuffman)
			{
				d0 = REVERSE_5BITS (NEXTBITS (5));
				DUMPBITS (5);
			}
			else
			{
				GET_HUFFMAN_ENTRY (dcodes, quickDistanceSize, d0);
			}

			if (d0 > MAX_ZIP_DISTANCE_CODE)
			{
				error = INFLATE_BAD_DISTANCE_CODE;
				break;
			}

			NEEDBITS (MAX_ZIP_EXTRA_DISTANCE_BITS) distance = dist_base[d0];
			moreBits = dist_extra_bits[d0];
			distance += NEXTBITS (moreBits);
			DUMPBITS (moreBits);

			if (outOffset < distance)
			{
				error = INFLATE_COPY_UNDERFLOW;
				break;
			}
			else if (outOffset + length > outLength)
			{
				error = INFLATE_OUTPUT_OVERFLOW;
				break;
			}
			else
			{
				unsigned long prev = outOffset - distance;
				unsigned long end = outOffset + length;
				unsigned char value;
				while (outOffset != end)
				{
					value = outBuffer[prev];
					outBuffer[outOffset] = value;
					outOffset++;
					prev++;
				}
			}
		}
	}

	STORE_IN;
	STORE_OUT;

	if (!fixedHuffman)
	{
		state->freeBytes (state->heapState, lcodesMemHandle);
		state->freeBytes (state->heapState, dcodesMemHandle);
	}

	return error;
}


static int decodeDynamicHuffmanTables (InflaterState * state,
							void **lcodesMemHandle, void **dcodesMemHandle)
{
	DECLARE_IN_VARIABLES void *ccodesMemHandle;
	HuffmanCodeTable *ccodes = NULL;

	static int hlit, hdist, hclen;
	static int i;
	static unsigned int quickBits;
	static unsigned char codelen[286 + 32];
	static unsigned char *codePtr, *endCodePtr;
	int error = 0;
	LOAD_IN;


	NEEDBITS (14);
	hlit = 257 + NEXTBITS (5);
	DUMPBITS (5);
	hdist = 1 + NEXTBITS (5);
	DUMPBITS (5);
	hclen = 4 + NEXTBITS (4);
	DUMPBITS (4);


	memset (codelen, 0x0, 19);
	for (i = 0; i < hclen; i++)
	{
		NEEDBITS (3);
		if (inRemaining < 0)
		{
			return INFLATE_CODE_TABLE_LENGTH_ERROR;
		}

		codelen[(int) ccode_idx[i]] = NEXTBITS (3);
		DUMPBITS (3);
	}

	error = makeCodeTable (state, codelen, 19, MAX_QUICK_CXD, &ccodesMemHandle);



	if (error != 0)
	{
		return error;
	}


	ccodes = state->addrFromHandle (state->heapState, ccodesMemHandle);

	quickBits = ccodes->h.quickBits;



	memset (codelen, 0x0, sizeof (codelen));
	for (codePtr = codelen, endCodePtr = codePtr + hlit + hdist;
		codePtr < endCodePtr;)
	{

		int val;

		if (inRemaining < 0)
		{
			error = INFLATE_EARLY_END_OF_CCTABLE_INPUT;
			break;
		}

		NEEDBITS (MAX_BITS + 7);
		GET_HUFFMAN_ENTRY (ccodes, quickBits, val);


		if (val <= 15)
		{
			*codePtr++ = val;
		}
		else if (val <= 18)
		{
			unsigned repeat = (val == 18) ? 11 : 3;
			unsigned bits = (val == 18) ? 7 : (val - 14);

			repeat += NEXTBITS (bits);
			DUMPBITS (bits);

			if (codePtr + repeat > endCodePtr)
			{
				error = INFLATE_BAD_REPEAT_CODE;
				break;
			}

			if (val == 16)
			{
				if (codePtr == codelen)
				{
					error = INFLATE_BAD_REPEAT_CODE;
					break;
				}
				memset (codePtr, codePtr[-1], repeat);
			}
			else
			{

			}
			codePtr += repeat;
		}
		else
		{
			error = INFLATE_BAD_CODELENGTH_CODE;
			break;
		}
	}


	state->freeBytes (state->heapState, ccodesMemHandle);

	while (error == 0)
	{

		error = makeCodeTable (state, codelen, hlit, MAX_QUICK_LXL,
			lcodesMemHandle);
		if (error != 0)
		{
			break;
		}

		error = makeCodeTable (state, codelen + hlit, hdist, MAX_QUICK_CXD,
			dcodesMemHandle);
		if (error != 0)
		{
			break;
		}

		STORE_IN;


		return 0;
	}


	state->freeBytes (state->heapState, *dcodesMemHandle);
	*dcodesMemHandle = NULL;
	state->freeBytes (state->heapState, *lcodesMemHandle);
	*lcodesMemHandle = NULL;

	return error;
}


static int makeCodeTable (InflaterState * state,
			   unsigned char *codelen,
			   unsigned numElems, unsigned maxQuickBits, void **result)
{
	static unsigned int bitLengthCount[MAX_BITS + 1];
	static unsigned int codes[MAX_BITS + 1];
	unsigned bits, minCodeLen = 0, maxCodeLen = 0;
	unsigned char *endCodeLen = codelen + numElems;
	static unsigned int code, quickMask;
	static unsigned char *p;
	static void *tableMemHandle;
	static HuffmanCodeTable *table;
	static int mainTableLength, longTableLength, numLongTables;
	static int tableSize;
	static int j;
	static unsigned short *nextLongTable;

	*result = NULL;


	memset (bitLengthCount, 0, sizeof (bitLengthCount));
	for (p = codelen; p < endCodeLen; p++)
	{
		bitLengthCount[*p]++;
	}

	if (bitLengthCount[0] == numElems)
	{
		// Empty code table is allowed!
	}


	code = minCodeLen = maxCodeLen = 0;
	for (bits = 1; bits <= MAX_BITS; bits++)
	{
		codes[bits] = code;
		if (bitLengthCount[bits] != 0)
		{
			if (minCodeLen == 0)
			{
				minCodeLen = bits;
			}
			maxCodeLen = bits;
			code += bitLengthCount[bits] << (MAX_BITS - bits);
		}
	}


	if (maxCodeLen <= maxQuickBits)
	{

		maxQuickBits = maxCodeLen;
		mainTableLength = (1 << maxCodeLen);
		numLongTables = longTableLength = 0;
	}
	else
	{
		mainTableLength = (1 << maxQuickBits);
		numLongTables = (1 << MAX_BITS) - codes[maxQuickBits + 1];
		numLongTables = numLongTables >> (MAX_BITS - maxQuickBits);
		longTableLength = 1 << (maxCodeLen - maxQuickBits);
	}

	ASSERT (mainTableLength == 1 << maxQuickBits);
	tableSize = sizeof (HuffmanCodeTableHeader)
		+ (mainTableLength + numLongTables * longTableLength)
		* sizeof (table->entries[0]);

	tableMemHandle = state->mallocBytes (state->heapState, tableSize);

	if (tableMemHandle == NULL)
	{
		return OUT_OF_MEMORY_ERROR;
	}


	table = state->addrFromHandle (state->heapState, tableMemHandle);

	nextLongTable = &table->entries[mainTableLength];

	memset (table, 0, tableSize);

	table->h.maxCodeLen = maxCodeLen;
	table->h.quickBits = maxQuickBits;

	quickMask = (1 << maxQuickBits) - 1;

	for (p = codelen; p < endCodeLen; p++)
	{
		unsigned short huff;
		bits = *p;
		if (bits == 0)
		{
			continue;
		}


		code = codes[bits];
		codes[bits] += 1 << (MAX_BITS - bits);
		code = REVERSE_15BITS (code);
		huff = ((p - codelen) << 4) + bits;

		if (bits <= maxQuickBits)
		{
			unsigned stride = 1 << bits;
			for (j = code; j < mainTableLength; j += stride)
			{
				table->entries[j] = huff;
			}
		}
		else
		{
			unsigned short *thisLongTable;
			unsigned stride = 1 << (bits - maxQuickBits);
			unsigned int prefixCode = code & quickMask;
			unsigned int suffixCode = code >> maxQuickBits;

			if (table->entries[prefixCode] == 0)
			{

				long delta = (char *) nextLongTable - (char *) table;
				table->entries[prefixCode] =
					(unsigned short) (HUFFINFO_LONG_MASK | delta);
				thisLongTable = nextLongTable;
				nextLongTable += longTableLength;
			}
			else
			{
				long delta = table->entries[prefixCode] & ~HUFFINFO_LONG_MASK;
				thisLongTable = (unsigned short *) ((char *) table + delta);
			}

			for (j = suffixCode; j < longTableLength; j += stride)
			{
				thisLongTable[j] = huff;
			}
		}
	}

	ASSERT (nextLongTable ==
		&table->entries[mainTableLength + numLongTables * longTableLength]);

	*result = tableMemHandle;
	return 0;
}
