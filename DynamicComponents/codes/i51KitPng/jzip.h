#ifndef _ZIP_H_
#define _ZIP_H_

typedef long (*FileSizeFunction)(void* state);
typedef long (*FileReadFunction)(void* state, unsigned char* buffer,
                                long numberOfChars);


typedef int (*FileSeekFunction)(void* state, long offset, int whence);


typedef int (*FileReadCharFunction)(void* state);


typedef struct _FileObj {
    void* state;          
    FileSizeFunction size; 
    FileReadFunction read; 
    FileSeekFunction seek; 
    FileReadCharFunction readChar; 
} FileObj;





typedef void* (*AllocFunction)(void* state, int n);


typedef void (*FreeFunction)(void* state, void* handle);


typedef void* (*AddrFromHandleFunction)(void* state, void* handle);


typedef struct _HeapManObj {
    void* state;         
    AllocFunction alloc; 
    FreeFunction free;   
    AddrFromHandleFunction addrFromHandle;
                         
} HeapManObj;



typedef struct _JarInfo {
    int status; 
    unsigned long locOffset; 
    unsigned long cenOffset; 
} JarInfo;


typedef struct _JarEntryInfo {
    int status; 
    unsigned long decompLen; 
    unsigned long compLen; 
    unsigned long method; 
    unsigned long expectedCRC; 
    int encrypted; 
    unsigned long offset; 
    unsigned long nameLen; 
    unsigned long nameOffset; 
    unsigned long nextCenEntryOffset; 
} JarEntryInfo;






JarInfo getJarInfo(FileObj* fileObj);


JarEntryInfo findJarEntryInfo(FileObj* pFileObj, JarInfo* jarInfo,
    unsigned char *name, unsigned int nameLen,
    unsigned char* compBuffer);


JarEntryInfo getFirstJarEntryInfo(FileObj* pFileObj, JarInfo* pJarInfo);


JarEntryInfo getNextJarEntryInfo(FileObj* fileObj, JarInfo* jarInfo,
                                 JarEntryInfo* current);

int getJarEntryName(FileObj* pFileObj, JarEntryInfo* pEntry,
    unsigned char *nameBuffer);


int inflateJarEntry(FileObj* fileObj, HeapManObj* heapManObj,
    JarEntryInfo* entry, unsigned char *decompBuffer, int bufferIsAHandle);


int zip_inflateData(FileObj* fileObj, HeapManObj* heapManObj, int compLen,
                unsigned char* decompBuffer, int decompLen,
                int bufferIsAHandle);


#define INFLATE_LEVEL_ERROR (-1)
#define OUT_OF_MEMORY_ERROR                (INFLATE_LEVEL_ERROR - 1)
#define INFLATE_INVALID_BTYPE              (INFLATE_LEVEL_ERROR - 2)
#define INFLATE_INPUT_BIT_ERROR            (INFLATE_LEVEL_ERROR - 3)
#define INFLATE_OUTPUT_BIT_ERROR           (INFLATE_LEVEL_ERROR - 4)
#define INFLATE_BAD_LENGTH_FIELD           (INFLATE_LEVEL_ERROR - 5)
#define INFLATE_INPUT_OVERFLOW             (INFLATE_LEVEL_ERROR - 6)
#define INFLATE_OUTPUT_OVERFLOW            (INFLATE_LEVEL_ERROR - 7)
#define INFLATE_EARLY_END_OF_INPUT         (INFLATE_LEVEL_ERROR - 8)
#define INFLATE_HUFFMAN_ENTRY_ERROR        (INFLATE_LEVEL_ERROR - 9)
#define INFLATE_INVALID_LITERAL_OR_LENGTH  (INFLATE_LEVEL_ERROR - 10)
#define INFLATE_BAD_DISTANCE_CODE          (INFLATE_LEVEL_ERROR - 11)
#define INFLATE_COPY_UNDERFLOW             (INFLATE_LEVEL_ERROR - 12)
#define INFLATE_CODE_TABLE_LENGTH_ERROR    (INFLATE_LEVEL_ERROR - 13)
#define INFLATE_EARLY_END_OF_CCTABLE_INPUT (INFLATE_LEVEL_ERROR - 14)
#define INFLATE_BAD_REPEAT_CODE            (INFLATE_LEVEL_ERROR - 15)
#define INFLATE_BAD_CODELENGTH_CODE        (INFLATE_LEVEL_ERROR - 16)
#define INFLATE_CODE_TABLE_EMPTY           (INFLATE_LEVEL_ERROR - 17)



#define JAR_LEVEL_ERROR 100

#define JAR_CORRUPT                  (JAR_LEVEL_ERROR + 1)
#define JAR_ENTRY_NOT_FOUND          (JAR_LEVEL_ERROR + 2)
#define JAR_ENCRYPTION_NOT_SUPPORTED (JAR_LEVEL_ERROR + 3)
#define JAR_EARLY_END_OF_INPUT       (JAR_LEVEL_ERROR + 4)
#define JAR_ENTRY_SIZE_MISMATCH      (JAR_LEVEL_ERROR + 5)
#define JAR_UNKNOWN_COMP_METHOD      (JAR_LEVEL_ERROR + 6)
#define JAR_ENTRY_CORRUPT            (JAR_LEVEL_ERROR + 7)













typedef long (*GetBytesFunctionType)(void* state, unsigned char* buffer,
                                long numberOfChars);


typedef void* (*MallocBytesFunctionType)(void* state, int size);


typedef void (*FreeBytesFunctionType)(void* state, void* handle);


typedef void* (*AddrFromHandleFunctionType)(void* state, void* handle);


typedef struct HuffmanCodeTableHeader {
    unsigned short quickBits;   
    unsigned short maxCodeLen;  
} HuffmanCodeTableHeader;


#define HUFFINFO_LONG_MASK 0x8000 

#define MAX_QUICK_CXD  6
#define MAX_QUICK_LXL  9

#if !defined(ASSERT) 
#    if 0
#        define ASSERT(x) (void)0
#    else
#        define ASSERT(x) (void)0
#    endif
#endif



#define BTYPE_NO_COMPRESSION 0x00  
#define BTYPE_FIXED_HUFFMAN  0x01  
#define BTYPE_DYNA_HUFFMAN   0x02  
#define BTYPE_INVALID        0x03  

#define MAX_BITS 15   

#define LITXLEN_BASE 257

#define INFLATEBUFFERSIZE 256


typedef struct _HuffmanCodeTable {
    struct HuffmanCodeTableHeader h;
    
    unsigned short entries[512];
} HuffmanCodeTable;


typedef struct shortHuffmanCodeTable {
    struct HuffmanCodeTableHeader h;
    unsigned short entries[32];
} shortHuffmanCodeTable;

typedef struct _InflaterState {
    
    void* fileState;               
    GetBytesFunctionType getBytes;

    void* heapState;
    MallocBytesFunctionType mallocBytes;
    FreeBytesFunctionType freeBytes;
    AddrFromHandleFunction addrFromHandle;

    int inRemaining;            
    unsigned int inDataSize;    
    unsigned long inData;       
                                
    
    unsigned char* outBuffer;
    unsigned long outOffset;
    unsigned long outLength;
    int outBufferIsAHandle; 

    int inflateBufferIndex;
    int inflateBufferCount;
    unsigned char inflateBuffer[INFLATEBUFFERSIZE];
} InflaterState;



#define NEXTBYTE ((state->inflateBufferCount)-- > 0 ? (unsigned long)(state->inflateBuffer[state->inflateBufferIndex++]) : \
    ((state->inflateBufferCount = getBytes(fileState, state->inflateBuffer, INFLATEBUFFERSIZE)) > 0 ? \
    (state->inflateBufferIndex = 1, state->inflateBufferCount--, (unsigned long)(state->inflateBuffer[0])) : 0xff))


#define NEEDBITS(j) {                                         \
      while (inDataSize < (j)) {                              \
           inData |= ((unsigned long)NEXTBYTE) << inDataSize; \
           inRemaining--; inDataSize += 8;                    \
      }                                                       \
      ASSERT(inDataSize <= 32);                               \
}


#define NEXTBITS(j) \
       (ASSERT((j) <= inDataSize), inData & ((1 << (j)) - 1))


#define DUMPBITS(j) {                                         \
       ASSERT((j) <= inDataSize);                             \
       inData >>= (j);                                        \
       inDataSize -= (j);                                     \
    }  


#define GET_HUFFMAN_ENTRY(table, quickBits, result) {  \
    unsigned int huff = table->entries[NEXTBITS(quickBits)];       \
    if (huff & HUFFINFO_LONG_MASK) {                               \
        long delta = (huff & ~HUFFINFO_LONG_MASK);                 \
        unsigned short *table2 = (unsigned short *)((char *)table + delta); \
        huff = table2[NEXTBITS(table->h.maxCodeLen) >> quickBits]; \
    }                                                              \
    if (huff == 0) {                                               \
        error = INFLATE_HUFFMAN_ENTRY_ERROR;                       \
        break;                                                     \
    }                                                              \
    DUMPBITS(huff & 0xF);                                          \
    result = huff >> 4;                                            \
}

#define NEXTBYTE ((state->inflateBufferCount)-- > 0 ? (unsigned long)(state->inflateBuffer[state->inflateBufferIndex++]) : \
    ((state->inflateBufferCount = getBytes(fileState, state->inflateBuffer, INFLATEBUFFERSIZE)) > 0 ? \
    (state->inflateBufferIndex = 1, state->inflateBufferCount--, (unsigned long)(state->inflateBuffer[0])) : 0xff))

#define DECLARE_IN_VARIABLES                         \
    register void* fileState = state->fileState;     \
    GetBytesFunctionType getBytes = state->getBytes; \
    register unsigned long inData;                   \
    register unsigned int inDataSize;                \
    register long inRemaining;


#define LOAD_IN                       \
    inData = state->inData;           \
    inDataSize = state->inDataSize;   \
    inRemaining = state->inRemaining; \


#define STORE_IN                      \
    state->inData = inData;           \
    state->inDataSize = inDataSize;   \
    state->inRemaining = inRemaining; 

#define DECLARE_OUT_VARIABLES                              \
    register unsigned char *outBuffer = state->outBuffer; \
    register unsigned long outLength = state->outLength;   \
    register unsigned long outOffset;

#define LOAD_OUT outOffset = state->outOffset;

#define STORE_OUT state->outOffset = outOffset;











static unsigned char ll_extra_bits[] = {
       0,   
       0,   
       0,   
       0,   
       0,   
       0,   
       0,   
       0,   
       1,   
       1,   

       1,   
       1,   
       2,   
       2,   
       2,   
       2,   
       3,   
       3,   
       3,   
       3,   

       4,   
       4,   
       4,   
       4,   
       5,   
       5,   
       5,   
       5,   
       0,   
};

#define MAX_ZIP_EXTRA_LENGTH_BITS 5

static unsigned short ll_length_base[] = {
       3,
       4,
       5,
       6,
       7,
       8,
       9,
       10,
       11,   
       13,   

       15,   
       17,   
       19,   
       23,   
       27,   
       31,   
       35,   
       43,   
       51,   
       59,   

       67,   
       83,   
       99,   
       115,  
       131,  
       163,  
       195,  
       227,  
       258
};


static unsigned char dist_extra_bits[] = {
       0,    
       0,    
       0,    
       0,    
       1,    
       1,    
       2,    
       2,    
       3,    
       3,    

       4,    
       4,    
       5,    
       5,    
       6,    
       6,    
       7,    
       7,    
       8,    
       8,    

       9,    
       9,    
       10,   
       10,   
       11,   
       11,   
       12,   
       12,   
       13,   
       13,   
};

#define MAX_ZIP_EXTRA_DISTANCE_BITS 13
#define MAX_ZIP_DISTANCE_CODE 29

static unsigned int dist_base[] = {
       1,
       2,
       3,
       4,
       5,      
       7,      
       9,      
       13,     
       17,     
       25,     

       33,     
       49,     
       65,     
       97,     
       129,    
       193,    
       257,    
       385,    
       513,    
       769,    
       1025,   
       1537,   
       2049,   
       3073,   
       4097,   
       6145,   
       8193,   
       12289,  
       16385,  
       24577,  
};




static char ccode_idx[] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15,
};



static unsigned char reverse5[] = {
    0x0, 0x10, 0x8, 0x18, 0x4, 0x14, 0xc, 0x1c,
    0x2, 0x12, 0xa, 0x1a, 0x6, 0x16, 0xe, 0x1e,
    0x1, 0x11, 0x9, 0x19, 0x5, 0x15, 0xd, 0x1d,
    0x3, 0x13, 0xb, 0x1b, 0x7, 0x17, 0xf, 0x1f
};

#define REVERSE_5BITS(code) reverse5[code]
#define REVERSE_9BITS(code) \
      ((reverse5[((code) & 0x1F)] << 4) | reverse5[(code) >> 4])
#define REVERSE_15BITS(code) \
      (  (reverse5[code & 0x1F] << 10) | (reverse5[((code) >> 5) & 0x1F] << 5) \
       | (reverse5[code >> 10]) )


#define HUFFMAN_ENTRY(char, bits) ((char << 4) + bits)


#ifdef USE_FIXED_HUFFMAN_CODE_TABLES



static struct HuffmanCodeTable fixedHuffmanCodeTable = { 
    { 
        7,            
        9,            
    }, 
    { 
        HUFFMAN_ENTRY(0x100, 7), 
        HUFFMAN_ENTRY('P',   8), 
        HUFFMAN_ENTRY(0x010, 8), 
        HUFFMAN_ENTRY(0x118, 8), 
        HUFFMAN_ENTRY(0x110, 7), 
        HUFFMAN_ENTRY('p',   8), 
        HUFFMAN_ENTRY('0',   8), 
        HUFFMAN_ENTRY(0x0c0, 9), 
        HUFFMAN_ENTRY(0x108, 7), 
        HUFFMAN_ENTRY('`',   8), 
        HUFFMAN_ENTRY(' ',   8), 
        HUFFMAN_ENTRY(0x0a0, 9), 
        HUFFMAN_ENTRY(0x000, 8), 
        HUFFMAN_ENTRY(0x080, 8), 
        HUFFMAN_ENTRY('@',   8), 
        HUFFMAN_ENTRY(0x0e0, 9), 
        HUFFMAN_ENTRY(0x104, 7), 
        HUFFMAN_ENTRY('X',   8), 
        HUFFMAN_ENTRY(0x018, 8), 
        HUFFMAN_ENTRY(0x090, 9), 
        HUFFMAN_ENTRY(0x114, 7), 
        HUFFMAN_ENTRY('x',   8), 
        HUFFMAN_ENTRY('8',   8), 
        HUFFMAN_ENTRY(0x0d0, 9), 
        HUFFMAN_ENTRY(0x10c, 7), 
        HUFFMAN_ENTRY('h',   8), 
        HUFFMAN_ENTRY('(',   8), 
        HUFFMAN_ENTRY(0x0b0, 9), 
        HUFFMAN_ENTRY(0x008, 8), 
        HUFFMAN_ENTRY(0x088, 8), 
        HUFFMAN_ENTRY('H',   8), 
        HUFFMAN_ENTRY(0x0f0, 9), 
        HUFFMAN_ENTRY(0x102, 7), 
        HUFFMAN_ENTRY('T',   8), 
        HUFFMAN_ENTRY(0x014, 8), 
        HUFFMAN_ENTRY(0x11c, 8), 
        HUFFMAN_ENTRY(0x112, 7), 
        HUFFMAN_ENTRY('t',   8), 
        HUFFMAN_ENTRY('4',   8), 
        HUFFMAN_ENTRY(0x0c8, 9), 
        HUFFMAN_ENTRY(0x10a, 7), 
        HUFFMAN_ENTRY('d',   8), 
        HUFFMAN_ENTRY('$',   8), 
        HUFFMAN_ENTRY(0x0a8, 9), 
        HUFFMAN_ENTRY(0x004, 8), 
        HUFFMAN_ENTRY(0x084, 8), 
        HUFFMAN_ENTRY('D',   8), 
        HUFFMAN_ENTRY(0x0e8, 9), 
        HUFFMAN_ENTRY(0x106, 7), 
        HUFFMAN_ENTRY('\\',  8), 
        HUFFMAN_ENTRY(0x01c, 8), 
        HUFFMAN_ENTRY(0x098, 9), 
        HUFFMAN_ENTRY(0x116, 7), 
        HUFFMAN_ENTRY('|',   8), 
        HUFFMAN_ENTRY('<',   8), 
        HUFFMAN_ENTRY(0x0d8, 9), 
        HUFFMAN_ENTRY(0x10e, 7), 
        HUFFMAN_ENTRY('l',   8), 
        HUFFMAN_ENTRY(',',   8), 
        HUFFMAN_ENTRY(0x0b8, 9), 
        HUFFMAN_ENTRY(0x00c, 8), 
        HUFFMAN_ENTRY(0x08c, 8), 
        HUFFMAN_ENTRY('L',   8), 
        HUFFMAN_ENTRY(0x0f8, 9), 
        HUFFMAN_ENTRY(0x101, 7), 
        HUFFMAN_ENTRY('R',   8), 
        HUFFMAN_ENTRY(0x012, 8), 
        HUFFMAN_ENTRY(0x11a, 8), 
        HUFFMAN_ENTRY(0x111, 7), 
        HUFFMAN_ENTRY('r',   8), 
        HUFFMAN_ENTRY('2',   8), 
        HUFFMAN_ENTRY(0x0c4, 9), 
        HUFFMAN_ENTRY(0x109, 7), 
        HUFFMAN_ENTRY('b',   8), 
        HUFFMAN_ENTRY('"',   8), 
        HUFFMAN_ENTRY(0x0a4, 9), 
        HUFFMAN_ENTRY(0x002, 8), 
        HUFFMAN_ENTRY(0x082, 8), 
        HUFFMAN_ENTRY('B',   8), 
        HUFFMAN_ENTRY(0x0e4, 9), 
        HUFFMAN_ENTRY(0x105, 7), 
        HUFFMAN_ENTRY('Z',   8), 
        HUFFMAN_ENTRY(0x01a, 8), 
        HUFFMAN_ENTRY(0x094, 9), 
        HUFFMAN_ENTRY(0x115, 7), 
        HUFFMAN_ENTRY('z',   8), 
        HUFFMAN_ENTRY(':',   8), 
        HUFFMAN_ENTRY(0x0d4, 9), 
        HUFFMAN_ENTRY(0x10d, 7), 
        HUFFMAN_ENTRY('j',   8), 
        HUFFMAN_ENTRY('*',   8), 
        HUFFMAN_ENTRY(0x0b4, 9), 
        HUFFMAN_ENTRY(0x00a, 8), 
        HUFFMAN_ENTRY(0x08a, 8), 
        HUFFMAN_ENTRY('J',   8), 
        HUFFMAN_ENTRY(0x0f4, 9), 
        HUFFMAN_ENTRY(0x103, 7), 
        HUFFMAN_ENTRY('V',   8), 
        HUFFMAN_ENTRY(0x016, 8), 
        HUFFMAN_ENTRY(0x11e, 8), 
        HUFFMAN_ENTRY(0x113, 7), 
        HUFFMAN_ENTRY('v',   8), 
        HUFFMAN_ENTRY('6',   8), 
        HUFFMAN_ENTRY(0x0cc, 9), 
        HUFFMAN_ENTRY(0x10b, 7), 
        HUFFMAN_ENTRY('f',   8), 
        HUFFMAN_ENTRY('&',   8), 
        HUFFMAN_ENTRY(0x0ac, 9), 
        HUFFMAN_ENTRY(0x006, 8), 
        HUFFMAN_ENTRY(0x086, 8), 
        HUFFMAN_ENTRY('F',   8), 
        HUFFMAN_ENTRY(0x0ec, 9), 
        HUFFMAN_ENTRY(0x107, 7), 
        HUFFMAN_ENTRY('^',   8), 
        HUFFMAN_ENTRY(0x01e, 8), 
        HUFFMAN_ENTRY(0x09c, 9), 
        HUFFMAN_ENTRY(0x117, 7), 
        HUFFMAN_ENTRY('~',   8), 
        HUFFMAN_ENTRY('>',   8), 
        HUFFMAN_ENTRY(0x0dc, 9), 
        HUFFMAN_ENTRY(0x10f, 7), 
        HUFFMAN_ENTRY('n',   8), 
        HUFFMAN_ENTRY('.',   8), 
        HUFFMAN_ENTRY(0x0bc, 9), 
        HUFFMAN_ENTRY(0x00e, 8), 
        HUFFMAN_ENTRY(0x08e, 8), 
        HUFFMAN_ENTRY('N',   8), 
        HUFFMAN_ENTRY(0x0fc, 9), 
        HUFFMAN_ENTRY(0x100, 7), 
        HUFFMAN_ENTRY('Q',   8), 
        HUFFMAN_ENTRY(0x011, 8), 
        HUFFMAN_ENTRY(0x119, 8), 
        HUFFMAN_ENTRY(0x110, 7), 
        HUFFMAN_ENTRY('q',   8), 
        HUFFMAN_ENTRY('1',   8), 
        HUFFMAN_ENTRY(0x0c2, 9), 
        HUFFMAN_ENTRY(0x108, 7), 
        HUFFMAN_ENTRY('a',   8), 
        HUFFMAN_ENTRY('!',   8), 
        HUFFMAN_ENTRY(0x0a2, 9), 
        HUFFMAN_ENTRY(0x001, 8), 
        HUFFMAN_ENTRY(0x081, 8), 
        HUFFMAN_ENTRY('A',   8), 
        HUFFMAN_ENTRY(0x0e2, 9), 
        HUFFMAN_ENTRY(0x104, 7), 
        HUFFMAN_ENTRY('Y',   8), 
        HUFFMAN_ENTRY(0x019, 8), 
        HUFFMAN_ENTRY(0x092, 9), 
        HUFFMAN_ENTRY(0x114, 7), 
        HUFFMAN_ENTRY('y',   8), 
        HUFFMAN_ENTRY('9',   8), 
        HUFFMAN_ENTRY(0x0d2, 9), 
        HUFFMAN_ENTRY(0x10c, 7), 
        HUFFMAN_ENTRY('i',   8), 
        HUFFMAN_ENTRY(')',   8), 
        HUFFMAN_ENTRY(0x0b2, 9), 
        HUFFMAN_ENTRY(0x009, 8), 
        HUFFMAN_ENTRY(0x089, 8), 
        HUFFMAN_ENTRY('I',   8), 
        HUFFMAN_ENTRY(0x0f2, 9), 
        HUFFMAN_ENTRY(0x102, 7), 
        HUFFMAN_ENTRY('U',   8), 
        HUFFMAN_ENTRY(0x015, 8), 
        HUFFMAN_ENTRY(0x11d, 8), 
        HUFFMAN_ENTRY(0x112, 7), 
        HUFFMAN_ENTRY('u',   8), 
        HUFFMAN_ENTRY('5',   8), 
        HUFFMAN_ENTRY(0x0ca, 9), 
        HUFFMAN_ENTRY(0x10a, 7), 
        HUFFMAN_ENTRY('e',   8), 
        HUFFMAN_ENTRY('%',   8), 
        HUFFMAN_ENTRY(0x0aa, 9), 
        HUFFMAN_ENTRY(0x005, 8), 
        HUFFMAN_ENTRY(0x085, 8), 
        HUFFMAN_ENTRY('E',   8), 
        HUFFMAN_ENTRY(0x0ea, 9), 
        HUFFMAN_ENTRY(0x106, 7), 
        HUFFMAN_ENTRY(']',   8), 
        HUFFMAN_ENTRY(0x01d, 8), 
        HUFFMAN_ENTRY(0x09a, 9), 
        HUFFMAN_ENTRY(0x116, 7), 
        HUFFMAN_ENTRY('}',   8), 
        HUFFMAN_ENTRY('=',   8), 
        HUFFMAN_ENTRY(0x0da, 9), 
        HUFFMAN_ENTRY(0x10e, 7), 
        HUFFMAN_ENTRY('m',   8), 
        HUFFMAN_ENTRY('-',   8), 
        HUFFMAN_ENTRY(0x0ba, 9), 
        HUFFMAN_ENTRY(0x00d, 8), 
        HUFFMAN_ENTRY(0x08d, 8), 
        HUFFMAN_ENTRY('M',   8), 
        HUFFMAN_ENTRY(0x0fa, 9), 
        HUFFMAN_ENTRY(0x101, 7), 
        HUFFMAN_ENTRY('S',   8), 
        HUFFMAN_ENTRY(0x013, 8), 
        HUFFMAN_ENTRY(0x11b, 8), 
        HUFFMAN_ENTRY(0x111, 7), 
        HUFFMAN_ENTRY('s',   8), 
        HUFFMAN_ENTRY('3',   8), 
        HUFFMAN_ENTRY(0x0c6, 9), 
        HUFFMAN_ENTRY(0x109, 7), 
        HUFFMAN_ENTRY('c',   8), 
        HUFFMAN_ENTRY('#',   8), 
        HUFFMAN_ENTRY(0x0a6, 9), 
        HUFFMAN_ENTRY(0x003, 8), 
        HUFFMAN_ENTRY(0x083, 8), 
        HUFFMAN_ENTRY('C',   8), 
        HUFFMAN_ENTRY(0x0e6, 9), 
        HUFFMAN_ENTRY(0x105, 7), 
        HUFFMAN_ENTRY('[',   8), 
        HUFFMAN_ENTRY(0x01b, 8), 
        HUFFMAN_ENTRY(0x096, 9), 
        HUFFMAN_ENTRY(0x115, 7), 
        HUFFMAN_ENTRY('{',   8), 
        HUFFMAN_ENTRY(';',   8), 
        HUFFMAN_ENTRY(0x0d6, 9), 
        HUFFMAN_ENTRY(0x10d, 7), 
        HUFFMAN_ENTRY('k',   8), 
        HUFFMAN_ENTRY('+',   8), 
        HUFFMAN_ENTRY(0x0b6, 9), 
        HUFFMAN_ENTRY(0x00b, 8), 
        HUFFMAN_ENTRY(0x08b, 8), 
        HUFFMAN_ENTRY('K',   8), 
        HUFFMAN_ENTRY(0x0f6, 9), 
        HUFFMAN_ENTRY(0x103, 7), 
        HUFFMAN_ENTRY('W',   8), 
        HUFFMAN_ENTRY(0x017, 8), 
        HUFFMAN_ENTRY(0x11f, 8), 
        HUFFMAN_ENTRY(0x113, 7), 
        HUFFMAN_ENTRY('w',   8), 
        HUFFMAN_ENTRY('7',   8), 
        HUFFMAN_ENTRY(0x0ce, 9), 
        HUFFMAN_ENTRY(0x10b, 7), 
        HUFFMAN_ENTRY('g',   8), 
        HUFFMAN_ENTRY('\'',  8), 
        HUFFMAN_ENTRY(0x0ae, 9), 
        HUFFMAN_ENTRY(0x007, 8), 
        HUFFMAN_ENTRY(0x087, 8), 
        HUFFMAN_ENTRY('G',   8), 
        HUFFMAN_ENTRY(0x0ee, 9), 
        HUFFMAN_ENTRY(0x107, 7), 
        HUFFMAN_ENTRY('_',   8), 
        HUFFMAN_ENTRY(0x01f, 8), 
        HUFFMAN_ENTRY(0x09e, 9), 
        HUFFMAN_ENTRY(0x117, 7), 
        HUFFMAN_ENTRY(0x07f, 8), 
        HUFFMAN_ENTRY('?',   8), 
        HUFFMAN_ENTRY(0x0de, 9), 
        HUFFMAN_ENTRY(0x10f, 7), 
        HUFFMAN_ENTRY('o',   8), 
        HUFFMAN_ENTRY('/',   8), 
        HUFFMAN_ENTRY(0x0be, 9), 
        HUFFMAN_ENTRY(0x00f, 8), 
        HUFFMAN_ENTRY(0x08f, 8), 
        HUFFMAN_ENTRY('O',   8), 
        HUFFMAN_ENTRY(0x0fe, 9), 
        HUFFMAN_ENTRY(0x100, 7), 
        HUFFMAN_ENTRY('P',   8), 
        HUFFMAN_ENTRY(0x010, 8), 
        HUFFMAN_ENTRY(0x118, 8), 
        HUFFMAN_ENTRY(0x110, 7), 
        HUFFMAN_ENTRY('p',   8), 
        HUFFMAN_ENTRY('0',   8), 
        HUFFMAN_ENTRY(0x0c1, 9), 
        HUFFMAN_ENTRY(0x108, 7), 
        HUFFMAN_ENTRY('`',   8), 
        HUFFMAN_ENTRY(' ',   8), 
        HUFFMAN_ENTRY(0x0a1, 9), 
        HUFFMAN_ENTRY(0x000, 8), 
        HUFFMAN_ENTRY(0x080, 8), 
        HUFFMAN_ENTRY('@',   8), 
        HUFFMAN_ENTRY(0x0e1, 9), 
        HUFFMAN_ENTRY(0x104, 7), 
        HUFFMAN_ENTRY('X',   8), 
        HUFFMAN_ENTRY(0x018, 8), 
        HUFFMAN_ENTRY(0x091, 9), 
        HUFFMAN_ENTRY(0x114, 7), 
        HUFFMAN_ENTRY('x',   8), 
        HUFFMAN_ENTRY('8',   8), 
        HUFFMAN_ENTRY(0x0d1, 9), 
        HUFFMAN_ENTRY(0x10c, 7), 
        HUFFMAN_ENTRY('h',   8), 
        HUFFMAN_ENTRY('(',   8), 
        HUFFMAN_ENTRY(0x0b1, 9), 
        HUFFMAN_ENTRY(0x008, 8), 
        HUFFMAN_ENTRY(0x088, 8), 
        HUFFMAN_ENTRY('H',   8), 
        HUFFMAN_ENTRY(0x0f1, 9), 
        HUFFMAN_ENTRY(0x102, 7), 
        HUFFMAN_ENTRY('T',   8), 
        HUFFMAN_ENTRY(0x014, 8), 
        HUFFMAN_ENTRY(0x11c, 8), 
        HUFFMAN_ENTRY(0x112, 7), 
        HUFFMAN_ENTRY('t',   8), 
        HUFFMAN_ENTRY('4',   8), 
        HUFFMAN_ENTRY(0x0c9, 9), 
        HUFFMAN_ENTRY(0x10a, 7), 
        HUFFMAN_ENTRY('d',   8), 
        HUFFMAN_ENTRY('$',   8), 
        HUFFMAN_ENTRY(0x0a9, 9), 
        HUFFMAN_ENTRY(0x004, 8), 
        HUFFMAN_ENTRY(0x084, 8), 
        HUFFMAN_ENTRY('D',   8), 
        HUFFMAN_ENTRY(0x0e9, 9), 
        HUFFMAN_ENTRY(0x106, 7), 
        HUFFMAN_ENTRY('\\',  8), 
        HUFFMAN_ENTRY(0x01c, 8), 
        HUFFMAN_ENTRY(0x099, 9), 
        HUFFMAN_ENTRY(0x116, 7), 
        HUFFMAN_ENTRY('|',   8), 
        HUFFMAN_ENTRY('<',   8), 
        HUFFMAN_ENTRY(0x0d9, 9), 
        HUFFMAN_ENTRY(0x10e, 7), 
        HUFFMAN_ENTRY('l',   8), 
        HUFFMAN_ENTRY(',',   8), 
        HUFFMAN_ENTRY(0x0b9, 9), 
        HUFFMAN_ENTRY(0x00c, 8), 
        HUFFMAN_ENTRY(0x08c, 8), 
        HUFFMAN_ENTRY('L',   8), 
        HUFFMAN_ENTRY(0x0f9, 9), 
        HUFFMAN_ENTRY(0x101, 7), 
        HUFFMAN_ENTRY('R',   8), 
        HUFFMAN_ENTRY(0x012, 8), 
        HUFFMAN_ENTRY(0x11a, 8), 
        HUFFMAN_ENTRY(0x111, 7), 
        HUFFMAN_ENTRY('r',   8), 
        HUFFMAN_ENTRY('2',   8), 
        HUFFMAN_ENTRY(0x0c5, 9), 
        HUFFMAN_ENTRY(0x109, 7), 
        HUFFMAN_ENTRY('b',   8), 
        HUFFMAN_ENTRY('"',   8), 
        HUFFMAN_ENTRY(0x0a5, 9), 
        HUFFMAN_ENTRY(0x002, 8), 
        HUFFMAN_ENTRY(0x082, 8), 
        HUFFMAN_ENTRY('B',   8), 
        HUFFMAN_ENTRY(0x0e5, 9), 
        HUFFMAN_ENTRY(0x105, 7), 
        HUFFMAN_ENTRY('Z',   8), 
        HUFFMAN_ENTRY(0x01a, 8), 
        HUFFMAN_ENTRY(0x095, 9), 
        HUFFMAN_ENTRY(0x115, 7), 
        HUFFMAN_ENTRY('z',   8), 
        HUFFMAN_ENTRY(':',   8), 
        HUFFMAN_ENTRY(0x0d5, 9), 
        HUFFMAN_ENTRY(0x10d, 7), 
        HUFFMAN_ENTRY('j',   8), 
        HUFFMAN_ENTRY('*',   8), 
        HUFFMAN_ENTRY(0x0b5, 9), 
        HUFFMAN_ENTRY(0x00a, 8), 
        HUFFMAN_ENTRY(0x08a, 8), 
        HUFFMAN_ENTRY('J',   8), 
        HUFFMAN_ENTRY(0x0f5, 9), 
        HUFFMAN_ENTRY(0x103, 7), 
        HUFFMAN_ENTRY('V',   8), 
        HUFFMAN_ENTRY(0x016, 8), 
        HUFFMAN_ENTRY(0x11e, 8), 
        HUFFMAN_ENTRY(0x113, 7), 
        HUFFMAN_ENTRY('v',   8), 
        HUFFMAN_ENTRY('6',   8), 
        HUFFMAN_ENTRY(0x0cd, 9), 
        HUFFMAN_ENTRY(0x10b, 7), 
        HUFFMAN_ENTRY('f',   8), 
        HUFFMAN_ENTRY('&',   8), 
        HUFFMAN_ENTRY(0x0ad, 9), 
        HUFFMAN_ENTRY(0x006, 8), 
        HUFFMAN_ENTRY(0x086, 8), 
        HUFFMAN_ENTRY('F',   8), 
        HUFFMAN_ENTRY(0x0ed, 9), 
        HUFFMAN_ENTRY(0x107, 7), 
        HUFFMAN_ENTRY('^',   8), 
        HUFFMAN_ENTRY(0x01e, 8), 
        HUFFMAN_ENTRY(0x09d, 9), 
        HUFFMAN_ENTRY(0x117, 7), 
        HUFFMAN_ENTRY('~',   8), 
        HUFFMAN_ENTRY('>',   8), 
        HUFFMAN_ENTRY(0x0dd, 9), 
        HUFFMAN_ENTRY(0x10f, 7), 
        HUFFMAN_ENTRY('n',   8), 
        HUFFMAN_ENTRY('.',   8), 
        HUFFMAN_ENTRY(0x0bd, 9), 
        HUFFMAN_ENTRY(0x00e, 8), 
        HUFFMAN_ENTRY(0x08e, 8), 
        HUFFMAN_ENTRY('N',   8), 
        HUFFMAN_ENTRY(0x0fd, 9), 
        HUFFMAN_ENTRY(0x100, 7), 
        HUFFMAN_ENTRY('Q',   8), 
        HUFFMAN_ENTRY(0x011, 8), 
        HUFFMAN_ENTRY(0x119, 8), 
        HUFFMAN_ENTRY(0x110, 7), 
        HUFFMAN_ENTRY('q',   8), 
        HUFFMAN_ENTRY('1',   8), 
        HUFFMAN_ENTRY(0x0c3, 9), 
        HUFFMAN_ENTRY(0x108, 7), 
        HUFFMAN_ENTRY('a',   8), 
        HUFFMAN_ENTRY('!',   8), 
        HUFFMAN_ENTRY(0x0a3, 9), 
        HUFFMAN_ENTRY(0x001, 8), 
        HUFFMAN_ENTRY(0x081, 8), 
        HUFFMAN_ENTRY('A',   8), 
        HUFFMAN_ENTRY(0x0e3, 9), 
        HUFFMAN_ENTRY(0x104, 7), 
        HUFFMAN_ENTRY('Y',   8), 
        HUFFMAN_ENTRY(0x019, 8), 
        HUFFMAN_ENTRY(0x093, 9), 
        HUFFMAN_ENTRY(0x114, 7), 
        HUFFMAN_ENTRY('y',   8), 
        HUFFMAN_ENTRY('9',   8), 
        HUFFMAN_ENTRY(0x0d3, 9), 
        HUFFMAN_ENTRY(0x10c, 7), 
        HUFFMAN_ENTRY('i',   8), 
        HUFFMAN_ENTRY(')',   8), 
        HUFFMAN_ENTRY(0x0b3, 9), 
        HUFFMAN_ENTRY(0x009, 8), 
        HUFFMAN_ENTRY(0x089, 8), 
        HUFFMAN_ENTRY('I',   8), 
        HUFFMAN_ENTRY(0x0f3, 9), 
        HUFFMAN_ENTRY(0x102, 7), 
        HUFFMAN_ENTRY('U',   8), 
        HUFFMAN_ENTRY(0x015, 8), 
        HUFFMAN_ENTRY(0x11d, 8), 
        HUFFMAN_ENTRY(0x112, 7), 
        HUFFMAN_ENTRY('u',   8), 
        HUFFMAN_ENTRY('5',   8), 
        HUFFMAN_ENTRY(0x0cb, 9), 
        HUFFMAN_ENTRY(0x10a, 7), 
        HUFFMAN_ENTRY('e',   8), 
        HUFFMAN_ENTRY('%',   8), 
        HUFFMAN_ENTRY(0x0ab, 9), 
        HUFFMAN_ENTRY(0x005, 8), 
        HUFFMAN_ENTRY(0x085, 8), 
        HUFFMAN_ENTRY('E',   8), 
        HUFFMAN_ENTRY(0x0eb, 9), 
        HUFFMAN_ENTRY(0x106, 7), 
        HUFFMAN_ENTRY(']',   8), 
        HUFFMAN_ENTRY(0x01d, 8), 
        HUFFMAN_ENTRY(0x09b, 9), 
        HUFFMAN_ENTRY(0x116, 7), 
        HUFFMAN_ENTRY('}',   8), 
        HUFFMAN_ENTRY('=',   8), 
        HUFFMAN_ENTRY(0x0db, 9), 
        HUFFMAN_ENTRY(0x10e, 7), 
        HUFFMAN_ENTRY('m',   8), 
        HUFFMAN_ENTRY('-',   8), 
        HUFFMAN_ENTRY(0x0bb, 9), 
        HUFFMAN_ENTRY(0x00d, 8), 
        HUFFMAN_ENTRY(0x08d, 8), 
        HUFFMAN_ENTRY('M',   8), 
        HUFFMAN_ENTRY(0x0fb, 9), 
        HUFFMAN_ENTRY(0x101, 7), 
        HUFFMAN_ENTRY('S',   8), 
        HUFFMAN_ENTRY(0x013, 8), 
        HUFFMAN_ENTRY(0x11b, 8), 
        HUFFMAN_ENTRY(0x111, 7), 
        HUFFMAN_ENTRY('s',   8), 
        HUFFMAN_ENTRY('3',   8), 
        HUFFMAN_ENTRY(0x0c7, 9), 
        HUFFMAN_ENTRY(0x109, 7), 
        HUFFMAN_ENTRY('c',   8), 
        HUFFMAN_ENTRY('#',   8), 
        HUFFMAN_ENTRY(0x0a7, 9), 
        HUFFMAN_ENTRY(0x003, 8), 
        HUFFMAN_ENTRY(0x083, 8), 
        HUFFMAN_ENTRY('C',   8), 
        HUFFMAN_ENTRY(0x0e7, 9), 
        HUFFMAN_ENTRY(0x105, 7), 
        HUFFMAN_ENTRY('[',   8), 
        HUFFMAN_ENTRY(0x01b, 8), 
        HUFFMAN_ENTRY(0x097, 9), 
        HUFFMAN_ENTRY(0x115, 7), 
        HUFFMAN_ENTRY('{',   8), 
        HUFFMAN_ENTRY(';',   8), 
        HUFFMAN_ENTRY(0x0d7, 9), 
        HUFFMAN_ENTRY(0x10d, 7), 
        HUFFMAN_ENTRY('k',   8), 
        HUFFMAN_ENTRY('+',   8), 
        HUFFMAN_ENTRY(0x0b7, 9), 
        HUFFMAN_ENTRY(0x00b, 8), 
        HUFFMAN_ENTRY(0x08b, 8), 
        HUFFMAN_ENTRY('K',   8), 
        HUFFMAN_ENTRY(0x0f7, 9), 
        HUFFMAN_ENTRY(0x103, 7), 
        HUFFMAN_ENTRY('W',   8), 
        HUFFMAN_ENTRY(0x017, 8), 
        HUFFMAN_ENTRY(0x11f, 8), 
        HUFFMAN_ENTRY(0x113, 7), 
        HUFFMAN_ENTRY('w',   8), 
        HUFFMAN_ENTRY('7',   8), 
        HUFFMAN_ENTRY(0x0cf, 9), 
        HUFFMAN_ENTRY(0x10b, 7), 
        HUFFMAN_ENTRY('g',   8), 
        HUFFMAN_ENTRY('\'',  8), 
        HUFFMAN_ENTRY(0x0af, 9), 
        HUFFMAN_ENTRY(0x007, 8), 
        HUFFMAN_ENTRY(0x087, 8), 
        HUFFMAN_ENTRY('G',   8), 
        HUFFMAN_ENTRY(0x0ef, 9), 
        HUFFMAN_ENTRY(0x107, 7), 
        HUFFMAN_ENTRY('_',   8), 
        HUFFMAN_ENTRY(0x01f, 8), 
        HUFFMAN_ENTRY(0x09f, 9), 
        HUFFMAN_ENTRY(0x117, 7), 
        HUFFMAN_ENTRY(0x07f, 8), 
        HUFFMAN_ENTRY('?',   8), 
        HUFFMAN_ENTRY(0x0df, 9), 
        HUFFMAN_ENTRY(0x10f, 7), 
        HUFFMAN_ENTRY('o',   8), 
        HUFFMAN_ENTRY('/',   8), 
        HUFFMAN_ENTRY(0x0bf, 9), 
        HUFFMAN_ENTRY(0x00f, 8), 
        HUFFMAN_ENTRY(0x08f, 8), 
        HUFFMAN_ENTRY('O',   8), 
        HUFFMAN_ENTRY(0x0ff, 9), 
    }
};

static struct shortHuffmanCodeTable fixedHuffmanDistanceTable = { 
    { 
        5,            
        5,            
    }, 
    {
        HUFFMAN_ENTRY(0x00, 5), 
        HUFFMAN_ENTRY(0x10, 5), 
        HUFFMAN_ENTRY(0x08, 5), 
        HUFFMAN_ENTRY(0x18, 5), 
        HUFFMAN_ENTRY(0x04, 5), 
        HUFFMAN_ENTRY(0x14, 5), 
        HUFFMAN_ENTRY(0x0c, 5), 
        HUFFMAN_ENTRY(0x1c, 5), 
        HUFFMAN_ENTRY(0x02, 5), 
        HUFFMAN_ENTRY(0x12, 5), 
        HUFFMAN_ENTRY(0x0a, 5), 
        HUFFMAN_ENTRY(0x1a, 5), 
        HUFFMAN_ENTRY(0x06, 5), 
        HUFFMAN_ENTRY(0x16, 5), 
        HUFFMAN_ENTRY(0x0e, 5), 
        HUFFMAN_ENTRY(0x1e, 5), 
        HUFFMAN_ENTRY(0x01, 5), 
        HUFFMAN_ENTRY(0x11, 5), 
        HUFFMAN_ENTRY(0x09, 5), 
        HUFFMAN_ENTRY(0x19, 5), 
        HUFFMAN_ENTRY(0x05, 5), 
        HUFFMAN_ENTRY(0x15, 5), 
        HUFFMAN_ENTRY(0x0d, 5), 
        HUFFMAN_ENTRY(0x1d, 5), 
        HUFFMAN_ENTRY(0x03, 5), 
        HUFFMAN_ENTRY(0x13, 5), 
        HUFFMAN_ENTRY(0x0b, 5), 
        HUFFMAN_ENTRY(0x1b, 5), 
        HUFFMAN_ENTRY(0x07, 5), 
        HUFFMAN_ENTRY(0x17, 5), 
        HUFFMAN_ENTRY(0x0f, 5), 
        HUFFMAN_ENTRY(0x1f, 5), 
    }
};

#endif 












#endif 
