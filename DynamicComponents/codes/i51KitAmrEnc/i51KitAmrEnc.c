
#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitAmrEnc.h"
#include "interf_enc.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);

#define i51KITAMRENC_BETA		1
#define i51KITAMRENC_DUBUG		0	//调试0==非调试状态 1==调试状态
#define i51KITAMRENC_DEMO		0	//测试

#define PCM_FREAM_SIZE					160	//8000*0.02
#define MAX_AMR_FRAME_SIZE				32	//AMR帧大小
#define AMR_FRAME_COUNT_PER_SECOND		50
#define AMR_FRAME_TYPE					7	//编码类型
#define AMR_MAGIC_NUMBER				"#!AMR\n"
#define SAMPLES_PERSCE8000				8000	//采样频率8Khz
#define MAX_READFRAME_COUNT				100
#define MAX_ENCODE_TIME					500

//CHUCK HEADER
typedef struct 
{
	iCHAR id[4];
	iU32  size;
}i51CHUNK_HEADER;

//RIFF
typedef struct 
{
	iCHAR			RiffID[4];		// 'R','I','F','F'
	iU32			RiffSize;			// 大小: wav文件大小 - sizeof(szRiffID) - sizeof(lRiffSize)
	iCHAR			RiffFormat[4];	// 'W','A','V','E'
}i51RIFF_CHUNK;

//FORMAT
typedef struct 
{
	iCHAR			FmtId[4];		// 'f','m','t',' '
	iU32			FmtSize;		//数值为16或18，18则最后有附加信息

	iU16			FormatTag;	    //编码方式，一般为0x0001
	iU16			Channels;		//声道数目，1--单声道；2--双声道
	iU32			SamplesPerSce;  //采样频率
	iU32			AvgBytesPerSec; //每秒所需字节数
	iU16			BlockAlign;	    //数据块对齐单位(每个采样需要的字节数)
	iU16			BitsPerSample;  //每个采样需要的bit数
}i51FORMAT_CHUNK;

typedef struct 
{
	i51RIFF_CHUNK		tRiff;
	i51FORMAT_CHUNK		tFormat;
	i51CHUNK_HEADER		tFact;
	i51CHUNK_HEADER		tData;
}i51WAVE_HEADER;

typedef struct  
{
	iHANDLE				EncoderState;	//编码器状态变量
	iCHAR				*EncoderMemory;	//编码器需要的内存

	iU32				Type;			//0==文件， 1==内存
	iU32				WaveHandle;		//文件句柄或数据地址
	iS32				Offset;			//数据偏移大小
	iU32				PCMDataLength;	//PCM数据总长(字节)
	iU32				EncodeLength;	//已编码的长度(字节)

	iCHAR				*AmrPtr;		
	iU32				AmrRemainSize;
	iCHAR				*ReadBuf;		//Wave读文件缓存
	iU32				ReadBufSize;	//读文件缓存数据大小
	iU32				PCMFrameSize;

	i51WAVE_HEADER	    tWaveHeader;	//wave文件信息

	void				(*ProcessCB)(iU32, iU32);
	void				(*ResultCB)(iU32);
}AMR_ENCODEER_CONTEXT;

AMR_ENCODEER_CONTEXT AmrEncoderContext = {0};

#if i51KITAMRENC_DEMO
	iFILE	DemoWaveFile = iNULL;
	iFILE	DemoAmrFile = iNULL;
	iCHAR	*DemoWaveBuffer = iNULL;
	iU8		*DemoAmrBuffer = iNULL;
	iU32	Demolenght = 0, DemoWriteSize=0, DemoReadSize = 0, DemoWavelenght=0;

	iS32	lFinishFlag = 0;
#endif


//读取wave文件头
static iBOOL i51KitAmrEncReadWaveHeader(iFILE inFile, i51WAVE_HEADER *outWaveHeader)
{
	iU32				size = 0;
	iCHAR				id[5] = {0};
	i51CHUNK_HEADER		tHeader = {0};

	if ( iNULL == inFile || iNULL == outWaveHeader)
	{
#if i51KITAMRENC_DUBUG
		iLog("i51KitAmrEncReadWaveHeader parameter is error!");
#endif
		return iFALSE;
	}

	if ( iFALSE == i51AdeOsFileSetSeek(inFile, 0, i51_OS_FL_SEEK_HEAD) )
	{
		iLog("i51KitAmrEncReadWaveHeader seek file error!");
		return iFALSE;
	}

	//读RIIF块
	if ( iFALSE  == i51AdeOsFileRead(inFile, &(outWaveHeader->tRiff), sizeof(i51RIFF_CHUNK), &size) )
	{
		iLog("i51KitAmrEncReadWaveHeader read RIFF error!");
		return iFALSE;
	}

	//读FORMAT块
	if ( iFALSE  == i51AdeOsFileRead(inFile, &(outWaveHeader->tFormat), sizeof(i51FORMAT_CHUNK), &size) )
	{
		iLog("i51KitAmrEncReadWaveHeader read Format error!");
		return iFALSE;
	}

	//判断FORMAT是否有附加信息
	if ( 18 == outWaveHeader->tFormat.FmtSize )
	{
		if ( iFALSE == i51AdeOsFileSetSeek(inFile, 2, i51_OS_FL_SEEK_CURRENT) )
		{
#if i51KITAMRENC_DUBUG
			iLog("i51KitAmrEncReadWaveHeader seek file error!");
#endif
			return iFALSE;
		}
	}

	//读取fact或者data头信息
	if ( iFALSE  == i51AdeOsFileRead(inFile, &tHeader, sizeof(i51CHUNK_HEADER), &size) )
	{
#if i51KITAMRENC_DUBUG
		iLog("i51KitAmrEncReadWaveHeader read file error!");
#endif
		return iFALSE;
	}

	i51AdeStdMemcpy16(id, tHeader.id, 4);
	id[4] = '\0';

	if ( 0 == strcmp(id, "fact") )
	{
		//有fact块
		i51AdeStdMemcpy16(&(outWaveHeader->tFact), &tHeader, sizeof(i51CHUNK_HEADER));

		if ( iFALSE == i51AdeOsFileSetSeek(inFile, tHeader.size, i51_OS_FL_SEEK_CURRENT) )
		{
#if i51KITAMRENC_DUBUG
			iLog("i51KitAmrEncReadWaveHeader seek file error!");
#endif
			return iFALSE;
		}

		//读取data块
		i51AdeStdMemset16(&tHeader, 0, sizeof(i51CHUNK_HEADER));
		if ( iFALSE  == i51AdeOsFileRead(inFile, &tHeader, sizeof(i51CHUNK_HEADER), &size) )
		{
#if i51KITAMRENC_DUBUG
			iLog("i51KitAmrEncReadWaveHeader read Data error!");
#endif
			return iFALSE;
		}

		i51AdeStdMemcpy16(&(outWaveHeader->tData), &tHeader, sizeof(i51CHUNK_HEADER));
	}
	else if (0 == strcmp(id, "data"))
	{
		i51AdeStdMemcpy16(&(outWaveHeader->tData), &tHeader, sizeof(i51CHUNK_HEADER));
	}
	else
	{
		return iFALSE;
	}

	return iTRUE;
}

//读取wave文件头
static iBOOL i51KitAmrEncReadWaveHeaderEx(iU8 *inBuffer, iU32 inBuffLen, i51WAVE_HEADER *outWaveHeader)
{
	iU32				size = 0;
	iCHAR				id[5] = {0};
	i51CHUNK_HEADER		tHeader = {0};
	iU8					*pBuffer = inBuffer;

	if ( iNULL == inBuffer || inBuffLen < sizeof(i51WAVE_HEADER) )
	{
#if i51KITAMRENC_DUBUG
		iLog("i51KitAmrEncReadWaveHeaderEx parameter is error!");
#endif
		return iFALSE;
	}

	//读RIIF块
	i51AdeStdMemcpy16(&(outWaveHeader->tRiff), pBuffer, sizeof(i51RIFF_CHUNK));
	pBuffer += sizeof(i51RIFF_CHUNK);

	//读FORMAT块
	i51AdeStdMemcpy16(&(outWaveHeader->tFormat), pBuffer, sizeof(i51FORMAT_CHUNK));
	pBuffer += sizeof(i51FORMAT_CHUNK);
	size = sizeof(i51RIFF_CHUNK) + sizeof(i51FORMAT_CHUNK);

	//判断FORMAT是否有附加信息
	if ( 18 == outWaveHeader->tFormat.FmtSize )
	{
		pBuffer += 2;
		size += 2;
	}

	//读取fact或者data头信息
	i51AdeStdMemcpy16(&tHeader, pBuffer, sizeof(i51CHUNK_HEADER));
	i51AdeStdMemcpy16(id, tHeader.id, 4);
	id[4] = '\0';

	if ( 0 == strcmp(id, "fact") )
	{
		//有fact块
		i51AdeStdMemcpy16(&(outWaveHeader->tFact), &tHeader, sizeof(i51CHUNK_HEADER));
		if ( tHeader.size + size > inBuffLen)
		{
#if i51KITAMRENC_DUBUG
			iLog("i51KitAmrEncReadWaveHeaderEx file length is error!");
#endif
			return iFALSE;
		}

		pBuffer += tHeader.size;

		//读取data块
		i51AdeStdMemset16(&tHeader, 0, sizeof(i51CHUNK_HEADER));
		i51AdeStdMemcpy16(&tHeader, pBuffer, sizeof(i51CHUNK_HEADER));
		pBuffer += sizeof(i51CHUNK_HEADER);

		i51AdeStdMemcpy16(&(outWaveHeader->tData), &tHeader, sizeof(i51CHUNK_HEADER));
	}
	else if (0 == strcmp(id, "data"))
	{
		i51AdeStdMemcpy16(&(outWaveHeader->tData), &tHeader, sizeof(i51CHUNK_HEADER));
	}
	else
	{
		return iFALSE;
	}

	return iTRUE;
}

static iU32 i51KitAmrEncGetPCMFrameSize(iU32 inChannel, iU32 inBitsPerSample)
{
	iU32	size = PCM_FREAM_SIZE*2;

	if ( 8 == inBitsPerSample && 1 == inChannel )
	{
		size = PCM_FREAM_SIZE;
	}
	else if (8 == inBitsPerSample && 2 == inChannel)
	{
		size = PCM_FREAM_SIZE*2;
	}
	else if (16 == inBitsPerSample && 1 == inChannel)
	{
		size = PCM_FREAM_SIZE*2;
	}
	else if (16 == inBitsPerSample && 2 == inChannel)
	{
		size = PCM_FREAM_SIZE*4;
	}

	return size;
}

//从wave文件中读一PCM帧
static iBOOL i51KitAmrEncReadPCMFrame(iU16 *outPCMFrame, iU8 *inData, iU32 inChannel, iU32 inBitsPerSample)
{
	iS32		i = 0;
	iU16		simple = 0;
	iU16		*pSimple = iNULL;
	iU8			frame[PCM_FREAM_SIZE*4] = {0};

	if ( iNULL == outPCMFrame || iNULL == inData)
	{
#if i51KITAMRENC_DUBUG
		iLog("i51KitAmrEncReadPCMFrame parameter is error!");
#endif
		return iFALSE;	
	}

	if ( 8 == inBitsPerSample && 1 == inChannel )
	{
		i51AdeStdMemcpy16(&frame, inData, PCM_FREAM_SIZE);

		for (i=0; i<PCM_FREAM_SIZE; i++)
		{
			simple = (short)(frame[i]);
			outPCMFrame[i] = simple<<7;
		}
	}
	else if (8 == inBitsPerSample && 2 == inChannel)
	{
		i51AdeStdMemcpy16(&frame, inData, PCM_FREAM_SIZE*2);

		for (i=0; i<PCM_FREAM_SIZE; i++)
		{
			simple = (short)(frame[i*2]);
			outPCMFrame[i] = simple<<7;
		}
	}
	else if (16 == inBitsPerSample && 1 == inChannel)
	{
		i51AdeStdMemcpy16(&frame, inData, PCM_FREAM_SIZE*2);
	}
	else if (16 == inBitsPerSample && 2 == inChannel)
	{
		i51AdeStdMemcpy16(&frame, inData, PCM_FREAM_SIZE*4);

		pSimple = (iU16*)frame;

		for (i=0; i<PCM_FREAM_SIZE; i++)
		{
			//取左声道
			simple = pSimple[i*2];
			outPCMFrame[i] = simple;
		}
	}
	else
	{
		return iFALSE;
	}

	return iTRUE;
}

//初始化编码库
static iBOOL i51KitAmrEncInit()
{
	if ( iNULL == AmrEncoderContext.EncoderMemory || iNULL == AmrEncoderContext.EncoderState)
	{
		iU32 memorySize = (iU32) Encoder_Interface_GetMemorySize();

#if i51KITAMRENC_BETA
		iLog( "i51KitAmrEncInit start");
#endif

		AmrEncoderContext.EncoderMemory = (iCHAR *)i51AdeOsMalloc(memorySize, __FILE__ , __LINE__);
		if ( iNULL == AmrEncoderContext.EncoderMemory )
		{
#if i51KITAMRENC_BETA
			iLog("i51KitAmrEncInit malloc memory fail, memory size is %d", memorySize);
#endif
			return iFALSE;
		}

		AmrEncoderContext.EncoderState = Encoder_Interface_init(0, AmrEncoderContext.EncoderMemory, (int)memorySize);
		if ( iNULL == AmrEncoderContext.EncoderState )
		{
#if i51KITAMRENC_DUBUG
			iLog("i51KitAmrEncInit Init fail!");
#endif
			i51AdeOsFree(AmrEncoderContext.EncoderMemory);
			AmrEncoderContext.EncoderMemory = iNULL;
			return iFALSE;
		}
#if i51KITAMRENC_BETA
		iLog( "i51KitAmrEncInit end");
#endif
	}

	return iTRUE;
}

//退出编码库
static iBOOL i51KitAmrEncExit()
{
	if ( iNULL != AmrEncoderContext.EncoderMemory || iNULL != AmrEncoderContext.EncoderState)
	{
		Encoder_Interface_exit(AmrEncoderContext.EncoderState);
		i51AdeOsFree(AmrEncoderContext.EncoderMemory);
		AmrEncoderContext.EncoderMemory = iNULL;
		AmrEncoderContext.EncoderState = iNULL;
	}

	return iTRUE;
}


//获取编码后amr文件大小
iBOOL i51KitAmrGetLength( iFILE inFile, iU32 *outLength )
{
	i51WAVE_HEADER		tWaveHeader = {0};
	iU32				PCMDataLength = 0;
	iU32				Offset = 0;

	if ( iNULL == inFile || iNULL == outLength )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrGetLength parameter is error!");
#endif
		return iFALSE;
	}

	if ( iFALSE == i51KitAmrEncReadWaveHeader(inFile, &tWaveHeader) )
	{
#if i51KITAMRENC_DUBUG
		iLog("i51KitAmrEncReadWaveHeader read wave header error!");
#endif
		return iFALSE;
	}

	if ( SAMPLES_PERSCE8000 != tWaveHeader.tFormat.SamplesPerSce )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrGetLength error, wave sample is not support!");
#endif
		return iFALSE;
	}

	//求文件数据部分长度
	if ( iFALSE == i51AdeOsFileSetSeek(inFile, 0, i51_OS_FL_SEEK_TAIL) )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrGetLength i51AdeOsFileSetSeek file error, inFile is %d", inFile);
#endif
		return iFALSE;
	}

	if ( iFALSE == i51AdeOsFileGetSeek(inFile, (iS32*)&PCMDataLength, i51_OS_FL_SEEK_HEAD) )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrGetLength i51AdeOsFileGetSeek error, inFile is %d", inFile);
#endif
		return iFALSE;
	}

	if ( iFALSE == i51AdeOsFileSetSeek(inFile, 0, i51_OS_FL_SEEK_HEAD) )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrGetLength i51AdeOsFileSetSeek file error, inFile is %d", inFile);
#endif
		return iFALSE;
	}

	if ( 18 == tWaveHeader.tFormat.FmtSize)
	{
		Offset	+= 2;
	}
	Offset += (sizeof(i51RIFF_CHUNK) + sizeof(i51FORMAT_CHUNK));

	if ( 0 != tWaveHeader.tFact.id[0])
	{
		Offset	+= ( sizeof(i51CHUNK_HEADER) + tWaveHeader.tFact.size );
	}

	Offset += sizeof(i51CHUNK_HEADER);

	PCMDataLength = PCMDataLength - Offset;

	PCMDataLength = PCMDataLength > tWaveHeader.tData.size ? tWaveHeader.tData.size : PCMDataLength;

	if ( 8 == tWaveHeader.tFormat.BitsPerSample && 1 == tWaveHeader.tFormat.Channels )
	{
		*outLength =  (iU32)( 32 * PCMDataLength / (tWaveHeader.tFormat.SamplesPerSce * 0.02) + 0.5 );
	}
	else if (8 == tWaveHeader.tFormat.BitsPerSample && 2 == tWaveHeader.tFormat.Channels)
	{
		*outLength =  (iU32)( 32 * PCMDataLength / (tWaveHeader.tFormat.SamplesPerSce * 2 * 0.02) + 0.5 );
	}
	else if (16 == tWaveHeader.tFormat.BitsPerSample && 1 == tWaveHeader.tFormat.Channels)
	{
		*outLength =  (iU32)( 32 * PCMDataLength / (tWaveHeader.tFormat.SamplesPerSce * 2 * 0.02) + 0.5 );
	}
	else if (16 == tWaveHeader.tFormat.BitsPerSample && 2 == tWaveHeader.tFormat.Channels)
	{
		*outLength =  (iU32)( 32 * PCMDataLength / (tWaveHeader.tFormat.SamplesPerSce * 4 * 0.02) + 0.5 );
	}
	else
	{
		return iFALSE;
	}

	*outLength += 6;

	return iTRUE;
}

iBOOL i51KitAmrGetLengthEx(iU8 *inBuffer, iU32 inBuffLen, iU8 *outLength)
{
	i51WAVE_HEADER		tWaveHeader = {0};
	iU32				PCMDataLength = inBuffLen;
	iU32				Offset = 0;

	if ( iNULL == inBuffer || inBuffLen < sizeof(tWaveHeader) || iNULL == outLength )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrGetLengthEx parameter is error!");
#endif
		return iFALSE;
	}

	if ( iFALSE == i51KitAmrEncReadWaveHeaderEx(inBuffer, inBuffLen, &tWaveHeader) ) 
	{
#if i51KITAMRENC_DUBUG
		iLog("i51KitAmrEncReadWaveHeaderEx read wave header error!");
#endif
		return iFALSE;
	}

	if ( SAMPLES_PERSCE8000 != tWaveHeader.tFormat.SamplesPerSce )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrGetLengthEx error, wave sample is not support!");
#endif
		return iFALSE;
	}

	if ( 18 == tWaveHeader.tFormat.FmtSize)
	{
		Offset	+= 2;
	}
	Offset += (sizeof(i51RIFF_CHUNK) + sizeof(i51FORMAT_CHUNK));

	if ( 0 != tWaveHeader.tFact.id[0])
	{
		Offset	+= ( sizeof(i51CHUNK_HEADER) + tWaveHeader.tFact.size );
	}

	Offset += sizeof(i51CHUNK_HEADER);

	PCMDataLength = PCMDataLength - Offset;

	PCMDataLength = PCMDataLength > tWaveHeader.tData.size ? tWaveHeader.tData.size : PCMDataLength;

	if ( 8 == tWaveHeader.tFormat.BitsPerSample && 1 == tWaveHeader.tFormat.Channels )
	{
		*outLength =  (iU32)( 32 * PCMDataLength / (tWaveHeader.tFormat.SamplesPerSce * 0.02) + 0.5 );
	}
	else if (8 == tWaveHeader.tFormat.BitsPerSample && 2 == tWaveHeader.tFormat.Channels)
	{
		*outLength =  (iU32)( 32 * PCMDataLength / (tWaveHeader.tFormat.SamplesPerSce * 2 * 0.02) + 0.5 );
	}
	else if (16 == tWaveHeader.tFormat.BitsPerSample && 1 == tWaveHeader.tFormat.Channels)
	{
		*outLength =  (iU32)( 32 * PCMDataLength / (tWaveHeader.tFormat.SamplesPerSce * 2 * 0.02) + 0.5 );
	}
	else if (16 == tWaveHeader.tFormat.BitsPerSample && 2 == tWaveHeader.tFormat.Channels)
	{
		*outLength =  (iU32)( 32 * PCMDataLength / (tWaveHeader.tFormat.SamplesPerSce * 4 * 0.02) + 0.5 );
	}
	else
	{
		return iFALSE;
	}

	*outLength += 6;

	return iTRUE;
}

static	void i51KitAmrEncTimeCB( iU32 id )
{
	iS32					i=0, Counts=0;
	iS32					ByteCounter = 0, Length = 0;
	AMR_ENCODEER_CONTEXT	*pContext = &AmrEncoderContext;
	iCHAR					*DataPtr = AmrEncoderContext.ReadBuf;
	iFILE					WaveFlie = iNULL;

	iU8						AmrFrame[MAX_AMR_FRAME_SIZE] = {0};
	iU16					PCMFrame[PCM_FREAM_SIZE] = {0};
	iS32					iIsLastEncode = 0;//0==不是， 1==是

#if i51KITAMRENC_DUBUG
	static	iS32	EncodeTime = 0;
#endif

	Length = pContext->PCMDataLength - pContext->EncodeLength;
	if (Length <= (iS32)pContext->ReadBufSize)
	{
		i51AdeOsTimerStop(id);
		iIsLastEncode = 1;
#if i51KITAMRENC_DUBUG
		iLog("stop the encode Timer");
#endif	
	}
	Length = Length > (iS32)pContext->ReadBufSize ? (iS32)pContext->ReadBufSize : Length;

#if i51KITAMRENC_DUBUG
	EncodeTime = i51AdeOsGetTick();
#endif	

	//文件
	if ( 0 == AmrEncoderContext.Type )
	{
		WaveFlie = (iFILE)(pContext->WaveHandle);
		if ( iFALSE == i51AdeOsFileSetSeek(WaveFlie, AmrEncoderContext.Offset, i51_OS_FL_SEEK_HEAD) )
		{
#if i51KITAMRENC_DUBUG
			iLog("i51KitAmrEncTimeCB seek file error!");
#endif
			i51AdeOsTimerStop(id);
			if ( 0 == AmrEncoderContext.Type )
			{
				i51AdeOsFree(pContext->ReadBuf);
				pContext->ReadBuf = iNULL;
			}
			pContext->ResultCB(0);
			return ;
		}

		if ( iFALSE  == i51AdeOsFileRead(WaveFlie, (void*)(pContext->ReadBuf), (iU32)Length, (iU32*)&Counts) )
		{
#if i51KITAMRENC_DUBUG
			iLog("i51KitAmrEncTimeCB read file error!");
#endif
			i51AdeOsTimerStop(id);
			if ( 0 == AmrEncoderContext.Type )
			{
				i51AdeOsFree(pContext->ReadBuf);
				pContext->ReadBuf = iNULL;
			}
			pContext->ResultCB(0);
			return ;
		}
	}
	else//内存
	{
		AmrEncoderContext.ReadBuf += Length;
	}
#if i51KITAMRENC_DUBUG
	EncodeTime = i51AdeOsGetTick() - EncodeTime;
	iLog("read wave data use time %d", EncodeTime);
	EncodeTime = i51AdeOsGetTick();
#endif	

	AmrEncoderContext.Offset += Length;
	Counts = Length / pContext->PCMFrameSize;
	for (i=0; i<Counts; i++)
	{
		if ( iFALSE == i51KitAmrEncReadPCMFrame(PCMFrame, (iU8 *)DataPtr, pContext->tWaveHeader.tFormat.Channels, 
			pContext->tWaveHeader.tFormat.BitsPerSample ) )
		{
#if i51KITAMRENC_DUBUG
			iLog("i51KitAmrEncReadPCMFrame read PCM Frame error!");
#endif
			i51AdeOsTimerStop(id);
			if ( 0 == AmrEncoderContext.Type )
			{
				i51AdeOsFree(pContext->ReadBuf);
				pContext->ReadBuf = iNULL;
			}
			pContext->ResultCB(0);
			return ;
		}
		DataPtr += pContext->PCMFrameSize;

		//编码出来的帧已经包含头了
		ByteCounter = (iU32)Encoder_Interface_Encode(pContext->EncoderState, AMR_FRAME_TYPE, (iS16*)PCMFrame, AmrFrame, 0);
		if ( pContext->AmrRemainSize > MAX_AMR_FRAME_SIZE)
		{
			i51AdeStdMemcpy16(pContext->AmrPtr, AmrFrame, ByteCounter);
			pContext->AmrPtr += ByteCounter;
			pContext->EncodeLength += pContext->PCMFrameSize;
			pContext->AmrRemainSize -= ByteCounter;
		}
		else
		{
			i51AdeOsTimerStop(id);
			if ( 0 == AmrEncoderContext.Type )
			{
				i51AdeOsFree(pContext->ReadBuf);
				pContext->ReadBuf = iNULL;
			}
			pContext->ResultCB(0);

			return ;
		}
	}

#if i51KITAMRENC_DUBUG
	EncodeTime = i51AdeOsGetTick() - EncodeTime;
	iLog("encode 100 frame use time %d", EncodeTime);
#endif	

	if (iIsLastEncode)
	{
		pContext->ProcessCB(pContext->EncodeLength , pContext->PCMDataLength);

		if ( 0 == AmrEncoderContext.Type )
		{
			i51AdeOsFree(pContext->ReadBuf);
			pContext->ReadBuf = iNULL;
		}

		pContext->ResultCB(1);
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder end");
#endif
	}
	else
	{
		pContext->ProcessCB(pContext->EncodeLength , pContext->PCMDataLength);
	}
}

iBOOL i51KitAmrEncoder(iFILE inFile, iU8 *outBuffer, iU32 outBufSize, void (*process)(iU32 , iU32), void (*result)(iU32))
{
	iCHAR					Flag[8] = {0};
	iU32					TimeId = 0;
	iS32					length=0;
	iS32					PCMDataLength = 0;
	i51WAVE_HEADER			*pWaveHeader = &(AmrEncoderContext.tWaveHeader);

#if i51KITAMRENC_BETA
	iLog("i51KitAmrEncoder start, inFile is %d, outBuffer is %d, process is %d, result is %d", inFile, outBuffer, process, result);
#endif

	if ( iNULL == inFile || iNULL == outBuffer || iNULL == process || iNULL == result)
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder parameter is error!");
#endif
		return iFALSE;
	}

	AmrEncoderContext.WaveHandle = (iU32)inFile;
	AmrEncoderContext.AmrPtr = (iCHAR*)outBuffer;
	AmrEncoderContext.AmrRemainSize = outBufSize;
	AmrEncoderContext.ProcessCB = process;
	AmrEncoderContext.ResultCB = result;
	AmrEncoderContext.Type = 0;

	if ( iFALSE == i51KitAmrEncReadWaveHeader(inFile, pWaveHeader) )
	{
#if i51KITAMRENC_DUBUG
		iLog("i51KitAmrEncoder read wave header error!");
#endif
		AmrEncoderContext.ResultCB(0);
		return iFALSE;
	}

	if ( SAMPLES_PERSCE8000 != pWaveHeader->tFormat.SamplesPerSce )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder not support the sample!");
#endif
		AmrEncoderContext.ResultCB(0);
		return iFALSE;
	}

	//移动文件句柄到数据块
	if ( 18 == pWaveHeader->tFormat.FmtSize)
	{
		AmrEncoderContext.Offset	+= 2;
	}
	AmrEncoderContext.Offset += (sizeof(i51RIFF_CHUNK) + sizeof(i51FORMAT_CHUNK));

	if ( 0 != pWaveHeader->tFact.id[0])
	{
		AmrEncoderContext.Offset	+= ( sizeof(i51CHUNK_HEADER) + pWaveHeader->tFact.size );
	}

	//判断数据头“data”
	i51AdeStdMemcpy16(Flag, pWaveHeader->tData.id, sizeof(pWaveHeader->tData.id));
	Flag[4] = '\0';
	if ( 0 != strcmp(Flag, "data") )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder wave header error!");
#endif
		AmrEncoderContext.ResultCB(0);
		return iFALSE;
	}

	AmrEncoderContext.Offset += sizeof(i51CHUNK_HEADER);

	//求文件数据部分长度
	if ( iFALSE == i51AdeOsFileSetSeek(inFile, 0, i51_OS_FL_SEEK_TAIL) )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder seek file error!");
#endif
		AmrEncoderContext.ResultCB(0);
		return iFALSE;
	}

	if ( iFALSE == i51AdeOsFileGetSeek(inFile, &PCMDataLength, i51_OS_FL_SEEK_HEAD) )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder i51AdeOsFileGetSeek error!");
#endif
		AmrEncoderContext.ResultCB(0);
		return iFALSE;
	}

	PCMDataLength -= AmrEncoderContext.Offset;
	PCMDataLength = PCMDataLength > (iS32)pWaveHeader->tData.size ? (iS32)pWaveHeader->tData.size : PCMDataLength;

#if i51KITAMRENC_DUBUG
	iLog("i51KitAmrEncoder Get Wave file length success, PCMDataLength is %d", PCMDataLength);
#endif

	AmrEncoderContext.PCMDataLength = PCMDataLength;

	AmrEncoderContext.PCMFrameSize = i51KitAmrEncGetPCMFrameSize(pWaveHeader->tFormat.Channels, pWaveHeader->tFormat.BitsPerSample);

	//分配读数据缓存
	length = MAX_READFRAME_COUNT * AmrEncoderContext.PCMFrameSize;
	length = length > (iS32)AmrEncoderContext.PCMDataLength ? (iS32)AmrEncoderContext.PCMDataLength : length;
	AmrEncoderContext.ReadBuf = (iCHAR*)i51AdeOsMalloc(length, __FILE__ , __LINE__);
	AmrEncoderContext.ReadBufSize = length;
	if ( iNULL == AmrEncoderContext.ReadBuf )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder i51AdeOsMalloc ReadBuf error!");
#endif
		AmrEncoderContext.ResultCB(0);
		return iFALSE;		
	}

	//写amr文件头
	if ( AmrEncoderContext.AmrRemainSize > 6)
	{
		i51AdeStdMemcpy16(AmrEncoderContext.AmrPtr, AMR_MAGIC_NUMBER, 6);
		AmrEncoderContext.AmrPtr += 6;
		AmrEncoderContext.AmrRemainSize -= 6;
	}
	else
	{
		i51AdeOsFree(AmrEncoderContext.ReadBuf);
		return iFALSE;
	}


#if i51KITAMRENC_DEMO
	{
		iRECT tRect = {0};
		tRect.X = 10;
		tRect.Y = 300;
		tRect.Width = 220;
		tRect.Height = 20;

		i51KitG2CleanScreen(0x3222, &tRect);

		i51KitG2Update();
	}
#endif


	TimeId = i51AdeOsTimerCreate(1, 1);
	if ( 0 == TimeId )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder i51AdeOsTimerCreate fail!");
#endif
		i51AdeOsFree(AmrEncoderContext.ReadBuf);
		return iFALSE;
	}

	i51AdeOsTimerStart(TimeId, i51KitAmrEncTimeCB, MAX_ENCODE_TIME);

	return iTRUE;
}

iBOOL i51KitAmrEncoderEx(iU8 *inBuffer, iU32 inBuffLen, iU8 *outBuffer, iU32 outBufSize, void (*process)(iU32 , iU32), void (*result)(iU32))
{
	iU32					TimeId = 0;
	iS32					PCMDataLength = 0, Length=0;
	iCHAR					Flag[5] = {0};
	i51WAVE_HEADER			*pWaveHeader = &(AmrEncoderContext.tWaveHeader);

#if i51KITAMRENC_BETA
	iLog("i51KitAmrEncoderEx start, inBuffer is %d, outBuffer is %d, process is %d, result is %d", inBuffer, outBuffer, process, result);
#endif

	if ( iNULL == inBuffer || inBuffLen<sizeof(i51WAVE_HEADER) || iNULL == outBuffer ||
		iNULL == process || iNULL == result)
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder parameter is error!");
#endif
		return iFALSE;
	}

	AmrEncoderContext.WaveHandle = (iU32)inBuffer;
	AmrEncoderContext.AmrPtr = (iCHAR*)outBuffer;
	AmrEncoderContext.AmrRemainSize = outBufSize;
	AmrEncoderContext.ProcessCB = process;
	AmrEncoderContext.ResultCB = result;
	AmrEncoderContext.Type = 1;

	if ( iFALSE == i51KitAmrEncReadWaveHeaderEx(inBuffer, inBuffLen, pWaveHeader) )
	{
#if i51KITAMRENC_DUBUG
		iLog("i51KitAmrEncoder read wave header error!");
#endif
		return iFALSE;
	}

	if ( SAMPLES_PERSCE8000 != pWaveHeader->tFormat.SamplesPerSce )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder wave sample is not support!");
#endif
		return iFALSE;
	}

	//移动文件句柄到数据块
	if ( 18 == pWaveHeader->tFormat.FmtSize)
	{
		AmrEncoderContext.Offset += 2;
	}
	AmrEncoderContext.Offset += (sizeof(i51RIFF_CHUNK) + sizeof(i51FORMAT_CHUNK));

	if ( 0 != pWaveHeader->tFact.id[0])
	{
		AmrEncoderContext.Offset += ( sizeof(i51CHUNK_HEADER) + pWaveHeader->tFact.size );
	}

	//判断数据头“data”
	i51AdeStdMemcpy16(Flag, pWaveHeader->tData.id, sizeof(pWaveHeader->tData.id));
	Flag[4] = '\0';
	if ( 0 != strcmp(Flag, "data") )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoder wave header error!");
#endif
		return iFALSE;
	}

	AmrEncoderContext.Offset += sizeof(i51CHUNK_HEADER);
	
	PCMDataLength = inBuffLen - AmrEncoderContext.Offset;
	PCMDataLength = PCMDataLength > (iS32)pWaveHeader->tData.size ? (iS32)pWaveHeader->tData.size : PCMDataLength;

#if i51KITAMRENC_DUBUG
	iLog("i51KitAmrEncoderEx Get Wave file length success, PCMDataLength is %d", PCMDataLength);
#endif

	AmrEncoderContext.PCMDataLength = PCMDataLength;
	AmrEncoderContext.PCMFrameSize = i51KitAmrEncGetPCMFrameSize(pWaveHeader->tFormat.Channels, pWaveHeader->tFormat.BitsPerSample);

	Length = MAX_READFRAME_COUNT * AmrEncoderContext.PCMFrameSize;
	Length = Length > (iS32)AmrEncoderContext.PCMDataLength ? (iS32)AmrEncoderContext.PCMDataLength : Length;
	AmrEncoderContext.ReadBuf = (iCHAR*)inBuffer + AmrEncoderContext.Offset;
	AmrEncoderContext.ReadBufSize = Length;

	//写amr文件头
	if ( AmrEncoderContext.AmrRemainSize > 6)
	{
		i51AdeStdMemcpy16(AmrEncoderContext.AmrPtr, AMR_MAGIC_NUMBER, 6);
		AmrEncoderContext.AmrPtr += 6;
		AmrEncoderContext.AmrRemainSize -= 6;
	}
	else
	{
		return iFALSE;
	}

	TimeId = i51AdeOsTimerCreate(1, 1);
	if ( 0 == TimeId )
	{
#if i51KITAMRENC_BETA
		iLog("i51KitAmrEncoderEx i51AdeOsTimerCreate fail!");
#endif
		return iFALSE;
	}

	i51AdeOsTimerStart(TimeId, i51KitAmrEncTimeCB, MAX_ENCODE_TIME);

 	return iTRUE;
 }

#if i51KITAMRENC_DEMO
void i51Process(iU32 already , iU32 Totall)
{
	iRECT tRect = {0};
	tRect.X = 10;
	tRect.Y = 300;
	tRect.Width = 0;
	tRect.Height = 20;

	tRect.Width = (iS16)((float)already / (float)Totall * 220);

	i51KitG2CleanScreen(0x3411, &tRect);

	i51KitG2Update();
}

void i51Result( iU32 id)
{
	i51AdeOsFileClose(DemoWaveFile);

	DemoAmrFile = i51AdeOsFileOpen(L"i51KitAmrEnc\\music\\test.amr", i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN, __FILE__, __LINE__);
	if ( iNULL == DemoAmrFile )
	{
#if i51KITAMRENC_DUBUG
		iLog("create amr file fail!\n");
#endif		
	}

	i51AdeOsFileWrite(DemoAmrFile, DemoAmrBuffer, Demolenght, &DemoWriteSize);

	i51AdeOsFileClose(DemoAmrFile);
	i51AdeOsFree(DemoAmrBuffer);
	DemoAmrBuffer = NULL;

	if (DemoWaveBuffer)
	{
		i51AdeOsFree(DemoWaveBuffer);
		DemoWaveBuffer = NULL;
	}

	iLog("encode finish");
}
#endif

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{

	case i51_MSG_INIT :
		{
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

			i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) )
			{
				return 0;
			}

			i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) )
			{
				return 0;
			}
#if i51KITAMRENC_BETA
			iLog("Encode success load i51KitStd.kit and i51KitG2.kit");
#endif
			if ( iFALSE == i51KitAmrEncInit() )
			{
#if i51KITAMRENC_BETA
				iLog("i51KitAmrEncInit fail");
#endif
				return 0;
			}
		}break ;
	case i51_MSG_RUN :
		{
			i51KitAmrEncInit();
	
#if i51KITAMRENC_DEMO

#if 0
			{
				DemoWaveFile = i51AdeOsFileOpen(L"i51KitAmrEnc\\music\\test.wav", i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__, __LINE__);
				if ( iNULL == DemoWaveFile )
				{
#if i51KITAMRENC_DUBUG
					iLog("open wave file fail!\n");
#endif
					return iFALSE;
				}

				if ( iFALSE == i51KitAmrGetLength(DemoWaveFile, &Demolenght))
				{
#if i51KITAMRENC_DUBUG
					iLog("i51KitAmrGetLength fail!\n");
#endif
					return iFALSE;
				}

				DemoAmrBuffer = i51AdeOsMalloc(Demolenght, __FILE__, __LINE__);
				if ( iNULL == DemoAmrBuffer )
				{
#if i51KITAMRENC_DUBUG
					iLog("i51AdeOsMalloc fail!\n");
#endif
					return iFALSE;
				}

				if ( iFALSE == i51KitAmrEncoder(DemoWaveFile, DemoAmrBuffer, Demolenght, i51Process, i51Result) )
				{
#if i51KITAMRENC_DUBUG
					iLog("i51KitAmrEncoder fail!\n");
#endif			
					return iFALSE;
				}
			}
#endif


#if 1
			{
				iS32  ReadSize = 0;
				iCHAR *temp = NULL;
				DemoWaveFile = i51AdeOsFileOpen(L"i51KitAmrEnc\\music\\test.wav", i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__, __LINE__);
				if ( iNULL == DemoWaveFile )
				{
#if i51KITAMRENC_DUBUG
					iLog("open wave file fail!\n");
#endif
					return iFALSE;
				}

				//求文件数据部分长度
				if ( iFALSE == i51AdeOsFileSetSeek(DemoWaveFile, 0, i51_OS_FL_SEEK_TAIL) )
				{
#if i51KITAMRENC_DUBUG
					iLog("i51KitAmrGetLength seek file error!");
#endif
					return iFALSE;
				}

				if ( iFALSE == i51AdeOsFileGetSeek(DemoWaveFile, (iS32*)&DemoWavelenght, i51_OS_FL_SEEK_HEAD) )
				{
#if i51KITAMRENC_DUBUG
					iLog("i51KitAmrGetLength i51AdeOsFileGetSeek error!");
#endif
					return iFALSE;
				}

				//重置文件句柄
				if ( iFALSE == i51AdeOsFileSetSeek(DemoWaveFile, 0, i51_OS_FL_SEEK_HEAD) )
				{
#if i51KITAMRENC_DUBUG
					iLog("i51KitAmrGetLength seek file error!");
#endif
					return iFALSE;
				}

				DemoWaveBuffer = i51AdeOsMalloc(DemoWavelenght, __FILE__, __LINE__);
				if ( iNULL == DemoWaveBuffer )
				{
#if i51KITAMRENC_DUBUG
					iLog("i51AdeOsMalloc DemoWaveBuffer fail!\n");
#endif
					return iFALSE;
				}

				iLog("i51AdeOsMalloc success DemoWaveBuffer is %d", DemoWaveBuffer);
 
				Demolenght = DemoWavelenght;
				temp = DemoWaveBuffer;
				while (1)
				{
					ReadSize = Demolenght > 131072 ? 131072 : Demolenght;

					if ( iFALSE  == i51AdeOsFileRead(DemoWaveFile, temp, ReadSize, &DemoReadSize) )
					{
#if i51KITAMRENC_DUBUG
						iLog("i51KitAmrEncReadWaveHeader read Format error!");
#endif
						return iFALSE;
					}

					Demolenght -=  ReadSize;
					temp += ReadSize;

					if ( Demolenght <= 0)
					{
						break;
					}
				}

				Demolenght = 0;
				if ( iFALSE == i51KitAmrGetLength(DemoWaveFile, &Demolenght))
				{
#if i51KITAMRENC_DUBUG
					iLog("i51KitAmrGetLength fail!\n");
#endif
					return iFALSE;
				}

				DemoAmrBuffer = i51AdeOsMalloc(Demolenght, __FILE__, __LINE__);
				if ( iNULL == DemoAmrBuffer )
				{
#if i51KITAMRENC_DUBUG
					iLog("i51AdeOsMalloc fail!\n");
#endif
					return iFALSE;
				}

				iLog("i51AdeOsMalloc success DemoAmrBuffer is %d", DemoAmrBuffer);

				if ( iFALSE == i51KitAmrEncoderEx((iU8*)DemoWaveBuffer, DemoWavelenght, (iU8*)DemoAmrBuffer, Demolenght, i51Process, i51Result) )
				{
#if i51KITAMRENC_DUBUG
					iLog("i51KitAmrEncoder fail!\n");
#endif			
					return iFALSE;
				}

			}
#endif

#endif	

		}
	case i51_MSG_RESUME :
		{
			i51KitAmrEncInit();
		}break ;

	case i51_MSG_PAUSE :
	case i51_MSG_EXIT :
		{
			i51KitAmrEncExit();
		}break ;
	}

	return iTRUE ;
}

iKIT iU32* i51KitPort ()
{
	//	TODO : Initialize API Port for external layer

	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitAmrEnc)

	i51KIT_DYNAMIC_KIT_MAPING(i51KitAmrGetLength)
	i51KIT_DYNAMIC_KIT_MAPING(i51KitAmrGetLengthEx)
	i51KIT_DYNAMIC_KIT_MAPING(i51KitAmrEncoder)
	i51KIT_DYNAMIC_KIT_MAPING(i51KitAmrEncoderEx)

	i51KIT_DYNAMIC_KIT_MAPING_END 

	return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitAmrEnc) ;
}
