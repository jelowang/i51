#include "i51.h"
#include "i51KitAde.h"
#include "jzip.h"

#define PIXEL_FORMAT_0565

#define lp_assert(_cond) do { \
	if(!(_cond)) { \
	iLog("assert faild:%s, halt\n",#_cond); \
	while(1); \
	} \
}while(0) \


#define LP_SUCCESS 0
#define LP_FAILED -1


#ifndef NULL
#define NULL (void*)0
#endif

#define KB (1024)
#define MB (KB * KB)
#define MAX_LOG_LEN (1024)
#define VMTASK_STACK_SIZE  (16 * KB)
#define MAXPARMLENGTH 20
#define MAXIMUM_TEMPORARY_ROOTS 50
#define MAX_FILENAME_LENGTH 256
#define MAXIMUM_GLOBAL_ROOTS 20
#define DUMMY(x) void x(){iLog("DUMMY FUNC: %s %d\n",__FILE__, __LINE__);}
#define SMS_STATUS_POOL_MAX 10
#define SMS_MAX_RECV_COUNT 20
#define MAX_ERROR_LEN 159
#define XASSERT(_cond) do {if(!(_cond)){iLog("XASSERT: %s %d %s\n",__FILE__,__LINE__,#_cond);goto _error;}} while(0)

#define VERIFIERTYPE unsigned short
#define CLASSKEY unsigned short
#define METHODTYPEKEY unsigned short
#define METHODNAMEKEY unsigned short


#if defined(PIXEL_FORMAT_ARGB)
typedef unsigned int lp_pixel_type;
#elif defined(PIXEL_FORMAT_0565)
typedef unsigned short lp_pixel_type;
#else
#error "not supported PIXLE FORMAT!"
#endif

typedef unsigned short lp_unicode;

typedef struct
{
	unsigned int width;
	unsigned int height;
	char *pixelData;
	char *alphaData;// tou ming se
	int hasPal;
	lp_pixel_type cmap[256]; //tiao se ban
	int hasTrans;
	char tmap[256]; //tou ming ban
	int mutable;
}lp_pixmap;

#define LP_RGB2PIXEL(r, g, b) ( b +(g << 5)+ (r << 11) )
#define LP_RGB24TORGB16(x) \
	(((( x ) & 0x00F80000) >> 8) + \
	((( x ) & 0x0000FC00) >> 5) + \
	((( x ) & 0x000000F8) >> 3) )







#define LP_RGB16TORGB24(x) (((( x ) << 8) & 0x00F80000) | \
	((( x ) << 5) & 0x0000FC00) | \
	((( x ) << 3) & 0x000000F8) )


#define LP_XAAA8888_FROM_ARGB8888(src) \
	(unsigned int)(((src >> 24) & 0x000000FF) | \
	((src >> 16) & 0x0000FF00) | \
	((src >> 8 ) & 0x00FF0000) )





typedef enum {
	LP_IMAGE_FORMAT_UNSUPPORTED,
	LP_IMAGE_FORMAT_PNG,
	LP_IMAGE_FORMAT_JPEG,
	LP_IMAGE_FORMAT_GIF,
	LP_IMAGE_FORMAT_RAW
} lp_image_format;


extern unsigned char lp_png_header[16];
extern unsigned char lp_jpeg_header[4];
extern unsigned char lp_raw_header[4];
extern unsigned char lp_gif_header[6];



typedef struct {
	unsigned char header[4];  
	unsigned int width;       
	unsigned int height;      
	unsigned int hasAlpha;    
	unsigned char data[1];    
} lp_image_buffer_raw;



typedef enum {
	LP_ERROR_NONE = 0,
	LP_ERROR_OUT_MEM,
	LP_ERROR_ILLEGAL_ARGUMENT,
	LP_ERROR_UNSUPPORTED,
	LP_ERROR_OUT_OF_RESOURCE,
	LP_ERROR_PERMISSION_DENIED,
	LP_ERROR_FOREIGN = LP_ERROR_OUT_MEM,
	LP_ERROR_AMS_SUITE_NOT_FOUND = 1000,
	LP_ERROR_AMS_SUITE_CORRUPTED,
	LP_ERROR_AMS_MIDLET_NOT_FOUND,
	LP_ERROR_PUSH_CONNECTION_IN_USE = 2000,
	LP_ERROR_IMAGE_CORRUPTED = 3000
} LP_ERROR;





typedef enum {
	LP_NATIVE_IMAGE_NO_ERROR            = 0, 
	LP_NATIVE_IMAGE_OUT_OF_MEMORY_ERROR = 1, 
	LP_NATIVE_IMAGE_DECODING_ERROR      = 2, 
	LP_NATIVE_IMAGE_UNSUPPORTED_FORMAT_ERROR = 3,  
	LP_NATIVE_IMAGE_RESOURCE_LIMIT = 4 
}  lp_native_image_error_codes;


typedef unsigned char uchar;


#ifndef TRUE
#define TRUE (-1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef bool
typedef int bool;
#endif

typedef struct _ids {
	void   *ptr;                  
	void   (*reset)(struct _ids *self);
	long   (*getpos)(struct _ids *self);
	void   (*seek)(struct _ids *self, long pos);
	int    (*getByte)(struct _ids *self);
	int    (*getBytes)(struct _ids *self, uchar *buf, int bufsize);
	void   (*skip)(struct _ids *self, int numBytes);
	void   (*done)(struct _ids *self);
} imageSrcData, *imageSrcPtr;

typedef struct _idd {
	int    depth;
	void   *ptr;                  
	void   (*setColormap)(struct _idd *self, long *map, int length);
	bool   (*setSize)(struct _idd *self, int width, int height, int colorType);
	void   (*sendPixels)(struct _idd *self, int y, uchar *scanline, int rgb);
	void   (*setTransMap)(struct _idd *self, unsigned char *map, int length, int palLength);
} imageDstData, *imageDstPtr;

typedef struct _imgDst {
	imageDstData   super;
	int hasPal;//解码图片是否有调色板
	lp_pixel_type cmap[256]; //调色板
	int hasTrans;//解码图片是否有透明板
	int srcStartX, srcStartY;//需要解码区域在原图的起始点
	lp_pixel_type *dstMapPtr;//解压到的目标图片
	int dstMapW, dstMapH;//解压到的目标图片的宽高
	int dstStartX, dstStartY;//需要解码区域在目标Map的左上角坐标
	int needW, needH;//需要解码区域的宽高
	int	dstMapPtrType; //dstMapPtr类型 0==文件句柄， 1==指针
	int	decodeType; //0==解码，1==解码并绘制
	unsigned int transColor;
} _imageDstData, *_imageDstPtr;

//********************png 解码器***********************************************/
#define CT_PALETTE  0x01
#define CT_COLOR    0x02
#define CT_ALPHA    0x04

#define IHDR_CHUNK 0x49484452
#define PLTE_CHUNK 0x504C5445
#define IDAT_CHUNK 0x49444154
#define IEND_CHUNK 0x49454E44

#define tRNS_CHUNK 0x74524E53


#define freeBytes(p) lp_free((p))

typedef struct _pngData
{
	signed int width;
	signed int height;


	int y;
	unsigned int scanlength;
	unsigned short bytesPerPixel;


	long *palette;
	unsigned short paletteLength;

	unsigned char *trans;
	unsigned short transLength;

	unsigned char pass;

	unsigned char depth;
	unsigned char colorType;
	unsigned char compress;
	unsigned char filter;
	unsigned char interlace;
	unsigned int lineBytes[7];
	unsigned int passSize[7];
} pngData;

static unsigned long readHeader (imageSrcPtr, long, pngData *, unsigned long);
static unsigned long readPalette (imageSrcPtr, long, pngData *,
								  long *, unsigned long);
static unsigned long readTransPal (imageSrcPtr, long, pngData *,
								   unsigned char *, unsigned long);
static bool handleImageData (unsigned char *, int, imageDstPtr, pngData *);
static unsigned long getInt (imageSrcPtr);
static unsigned long png_skip (imageSrcPtr, int, unsigned long);
static bool getChunk (imageSrcPtr, unsigned long *, long *);
static bool signatureOK (imageSrcPtr);
static bool headerOK (pngData *);
static unsigned long init_CRC (unsigned long);
static bool check_CRC (imageSrcPtr, unsigned long);
static unsigned long update_crc (unsigned long, unsigned char *, int);
static int findPassWidth (int, pngData *);

typedef struct _iid
{
	imageSrcPtr src;
	long clen;
} idatInflateData;

static int
PNGdecodeImage_getByte (void *p)
{
	idatInflateData *d = (idatInflateData *) p;
	imageSrcPtr src = d->src;
	unsigned long chunkType;
	unsigned int byte;


	while (d->clen == 0)
	{
		(void) getInt (src);

		getChunk (src, &chunkType, &(d->clen));

		if (chunkType != IDAT_CHUNK)
		{
			return -1;
		}
	}

	--(d->clen);
	byte = src->getByte (src);
	return byte;
}

long PNGdecodeImage_getBytes (void *p, unsigned char *buffer, long count)
{
	idatInflateData *d = (idatInflateData *) p;
	imageSrcPtr src = d->src;
	unsigned long chunkType;


	while (d->clen == 0)
	{
		(void) getInt (src);

		getChunk (src, &chunkType, &(d->clen));

		if (chunkType != IDAT_CHUNK)
		{
			return 0;
		}
	}

	if (d->clen <= (int) count)
	{
		count = d->clen;
	}

	count = (long) (src->getBytes (src, buffer, (int) count));
	d->clen -= (int) count;
	return count;
}

static long PNGdecodeImage_size (void *p)
{
	(void) p;
	return 0;
}

static int PNGdecodeImage_seek (void *p, long offset, int whence)
{
	(void) p;
	(void) offset;
	(void) whence;

	return -1;
}


static void * allocFunction (void *state, int n)
{
	(void) state;
	return i51AdeOsMalloc(n, __FILE__, __LINE__);
}


static void freeFunction (void *state, void *handle)
{
	(void) state;
	i51AdeOsFree(handle);
}


static void *addrFromHandleFunction (void *state, void *handle)
{
	(void) state;
	return handle;
}


static bool PNGdecodeImage_real (imageSrcPtr src, imageDstPtr dst, long *paletteData, unsigned char *transData)
{
	bool OK = TRUE;
	bool saw_IDAT = FALSE, saw_PLTE = FALSE, saw_tRNS = FALSE;
	static pngData data;

	static unsigned long chunkType;
	static long chunkLength;
	static unsigned long CRC;

	if (!signatureOK (src))
	{
		goto formaterror;
	}

	memset (&data, 0, sizeof (data));

	while (getChunk (src, &chunkType, &chunkLength))
	{
		CRC = init_CRC (chunkType);

		if (chunkType == IHDR_CHUNK)
		{

			if ((chunkLength < 13) || headerOK (&data))
			{
				goto formaterror;
			}

			CRC = readHeader (src, chunkLength, &data, CRC);

			if (!headerOK (&data))
			{
				goto formaterror;
			}

			if(!dst->setSize (dst, data.width, data.height, data.colorType)) goto formaterror;

			if ((data.colorType & CT_COLOR) == 0)
			{
				int n = 0;
				static long _p[4];
				switch (data.depth)
				{
				case 1:
					_p[0] = 0;
					_p[1] = 0xffffff;
					n = 2;
					dst->setColormap (dst, _p, n);
					break;

				case 2:
					_p[0] = 0;
					_p[1] = 0x555555;
					_p[2] = 0xaaaaaa;
					_p[3] = 0xffffff;
					n = 4;
					dst->setColormap (dst, _p, n);
					break;

				case 4:
					{
						static long p[16];

						for (n = 0; n < 16; ++n)
						{
							p[n] = n * 0x111111;
						}

						dst->setColormap (dst, p, n);
					}
					break;

				default:
					{
						static long p[256];

						for (n = 0; n < 256; ++n)
						{
							p[n] = (n << 16) | (n << 8) | n;
						}

						dst->setColormap (dst, p, n);
					}
					break;
				}
			}
		}
		else if (chunkType == PLTE_CHUNK)
		{

			if ((chunkLength > 768) || ((chunkLength % 3) != 0))
			{
				goto formaterror;
			}

			if ((data.paletteLength > 0) || saw_IDAT || saw_tRNS)
			{
				goto formaterror;
			}

			CRC = readPalette (src, chunkLength, &data, paletteData, CRC);
			if (data.palette == NULL)
			{
				goto formaterror;
			}

			if (data.colorType & CT_PALETTE)
			{
				dst->setColormap (dst, data.palette, data.paletteLength);
			}

			saw_PLTE = TRUE;
		}
		else if (chunkType == tRNS_CHUNK)
		{

			if (saw_IDAT || data.colorType == 4 || data.colorType == 6)
			{
				goto formaterror;
			}

			CRC = readTransPal (src, chunkLength, &data, transData, CRC);
			if (data.trans == NULL)
			{
				goto formaterror;
			}

			dst->setTransMap (dst, data.trans, data.transLength,
				saw_PLTE ? data.paletteLength : -1);

			saw_tRNS = TRUE;
		}
		else if (chunkType == IDAT_CHUNK)
		{
			static idatInflateData _data;
			static FileObj fileObj;
			static HeapManObj heapManObj;

			int compLen = 0;
			int decompLen = data.scanlength * data.height;
			unsigned char *decompBuf;
			long startPos = 0, lastGoodPos = 0;

			saw_IDAT = TRUE;

			if (data.interlace)
			{
				int i;

				decompLen = 0;
				for (i = 0; i < 7; ++i)
				{
					int off = 7 >> (i / 2);
					int shift = 3 - (((i - 1) / 2));
					int height = ((data.height + off) >> shift);
					int width = findPassWidth (i, &data);

					data.lineBytes[i] = width;
					data.passSize[i] = width * height;

					decompLen += data.passSize[i];
				}
			}
			else
			{
				data.lineBytes[6] = data.scanlength;
				data.passSize[6] = decompLen;
			}

			if ((data.colorType & CT_PALETTE) && (data.palette == NULL))
			{
				goto formaterror;
			}

			_data.src = src;
			_data.clen = chunkLength;

			lastGoodPos = startPos = src->getpos (src);
			do
			{

				if (chunkType != IDAT_CHUNK)
					break;

				CRC = init_CRC (chunkType);
				compLen += chunkLength;
				CRC = png_skip (src, chunkLength, CRC);
				lastGoodPos = src->getpos (src);

				if (!check_CRC (src, CRC))
				{
					goto formaterror;
				}
			}
			while (getChunk (src, &chunkType, &chunkLength));

			src->seek (src, startPos);

			decompBuf = (unsigned char *)i51AdeOsMalloc(decompLen, __FILE__, __LINE__);
			if (decompBuf == iNULL)
			{
				OK = FALSE;
				goto done;
			}

			compLen -= 2;
			PNGdecodeImage_getByte (&_data);
			PNGdecodeImage_getByte (&_data);

			fileObj.state = &_data;
			fileObj.size = PNGdecodeImage_size;
			fileObj.read = PNGdecodeImage_getBytes;
			fileObj.seek = PNGdecodeImage_seek;
			fileObj.readChar = PNGdecodeImage_getByte;

			heapManObj.state = NULL;
			heapManObj.alloc = allocFunction;
			heapManObj.free = freeFunction;
			heapManObj.addrFromHandle = addrFromHandleFunction;

			if ( zip_inflateData (&fileObj, &heapManObj, compLen - 4, decompBuf, decompLen, 0) != 0)
			{
				i51AdeOsFree((void*)decompBuf);
				
				goto formaterror;
			}

			OK = handleImageData (decompBuf, decompLen, dst, &data);

			i51AdeOsFree((void*)decompBuf);

			src->seek (src, lastGoodPos);
		}
		else if (chunkType == IEND_CHUNK)
		{

		}
		else
		{

			CRC = png_skip (src, chunkLength, CRC);
		}

		if (!check_CRC (src, CRC))
		{
			iLog ("PNG data corrupted (CRC mismatch)");

			goto formaterror;
		}
	}

done:

	return OK;

formaterror:
	OK = FALSE;
	goto done;

}

static  char pStartX[8] = {0, 4, 0, 2, 0, 1, 0, 0};
static  char pIncX[8]   = {8, 8, 4, 4, 2, 2, 1, 1};

static int findPassWidth (int pass, pngData * data)
{
	int ix = ((pass < 0) || (pass > 7)) ? 7 : pass;
	int pixels = (data->width - pStartX[ix] + pIncX[ix] - 1) / pIncX[ix];
	int channels = (data->colorType & CT_COLOR) ? 3 : 1;

	if (data->colorType & CT_ALPHA)
		++channels;
	if (data->colorType & CT_PALETTE)
		channels = 1;

	if (pixels <= 0)
	{
		return 0;
	}

	return 1 + ((pixels * data->depth * channels + 7) >> 3);
}

static unsigned long readHeader (imageSrcPtr src, long length, pngData * data, unsigned long CRC)
{
	unsigned char buf[13];

	src->getBytes (src, buf, sizeof (buf));

	data->width = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	data->height = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];

	data->depth = buf[8];
	data->colorType = buf[9];
	data->compress = buf[10];
	data->filter = buf[11];
	data->interlace = buf[12];



	if (data->interlace)
	{
		data->pass = 0;
	}
	else
	{
		data->pass = 7;
	}

	switch (data->colorType)
	{
	case 0:
	case 3:
		data->bytesPerPixel = (data->depth + 7) / 8;
		break;

	case 2:
		data->bytesPerPixel = 3 * data->depth / 8;
		break;

	case 4:
		data->bytesPerPixel = data->depth / 4;
		break;

	case 6:
		data->bytesPerPixel = data->depth / 2;
		break;
	}

	data->scanlength = findPassWidth (6, data);

	CRC = update_crc (CRC, buf, sizeof (buf));
	length -= sizeof (buf);

	while (length > 0)
	{
		int n = (length < (long) sizeof (buf)) ? length : (long) sizeof (buf);
		src->getBytes (src, buf, n);
		CRC = update_crc (CRC, buf, n);
	}

	return CRC;
}

static unsigned long readPalette (imageSrcPtr src, long length, pngData * data,
			 long *buffer, unsigned long CRC)
{
	int i;

	data->paletteLength =(unsigned short) length / 3;
	data->palette = buffer;

	for (i = 0; i < data->paletteLength; ++i)
	{
		unsigned char buf[3];
		src->getBytes (src, buf, 3);
		CRC = update_crc (CRC, buf, 3);

		data->palette[i] = (buf[0] << 16) | (buf[1] << 8) | buf[2];
	}

	return CRC;
}

static unsigned long readTransPal (imageSrcPtr src, long length, pngData * data,
			  unsigned char *buffer, unsigned long CRC)
{
	int i;

	data->transLength = (unsigned short)length;
	data->trans = buffer;

	if (data->colorType & CT_PALETTE)
	{
		for (i = 0; i < data->transLength; i++)
		{
			unsigned char buf[1];
			buf[0] = src->getByte (src);
			CRC = update_crc (CRC, buf, 1);
			data->trans[i] = buf[0];
		}
	}
	else if ((data->colorType & CT_COLOR) == 0)
	{
		unsigned char buf[2];
		src->getBytes (src, buf, 2);
		CRC = update_crc (CRC, buf, 2);

		data->trans[0] = buf[0];
		data->trans[1] = buf[1];
		data->transLength = 2;
	}
	else if (data->colorType & CT_COLOR)
	{

		unsigned char buf[6];
		src->getBytes (src, buf, 6);
		CRC = update_crc (CRC, buf, 6);

		data->trans[0] = buf[0];
		data->trans[1] = buf[1];
		data->trans[2] = buf[2];
		data->trans[3] = buf[3];
		data->trans[4] = buf[4];
		data->trans[5] = buf[5];
		data->transLength = 6;
	}

	return CRC;
}

static void applyFilter (int filterType, unsigned char *buf, int n,
			 unsigned char *prev, int bpp)
{
	int x;

	if (filterType == 0)
	{
		return;
	}
	else if (prev == NULL)
	{
		if (filterType == 4)
		{
			filterType = 1;
		}
		else if (filterType == 3)
		{
			for (x = bpp; x < n; ++x)
			{
				buf[x] += buf[x - bpp] >> 1;
			}
			return;
		}
		else if (filterType == 2)
		{
			return;
		}
	}

	switch (filterType)
	{
	case 1:

		for (x = bpp; x < n; ++x)
		{
			buf[x] += buf[x - bpp];
		}
		break;

	case 2:
		for (x = 0; x < n; ++x)
		{
			buf[x] += prev[x];
		}
		break;

	case 3:


		for (x = 0; x < bpp; ++x)
		{
			buf[x] += prev[x] >> 1;
		}


		for (x = bpp; x < n; ++x)
		{
			buf[x] += (prev[x] + (buf[x - bpp])) >> 1;
		}
		break;

	case 4:

		for (x = 0; x < bpp; ++x)
		{
			buf[x] += prev[x];
		}


		for (x = bpp; x < n; ++x)
		{
			int a, b, c, p, pa, pb, pc;
			a = buf[x - bpp];
			b = prev[x];
			c = prev[x - bpp];
			p = a + b - c;
			pa = p > a ? p - a : a - p;
			pb = p > b ? p - b : b - p;
			pc = p > c ? p - c : c - p;
			buf[x] += ((pa <= pb) && (pa <= pc)) ? a : ((pb <= pc) ? b : c);
		}
		break;

	default:
		iLog ("Illegal filter value %d\n", filterType);
		break;
	}
}

static void filterAllRows (unsigned char *pixels, pngData * data)
{

	if (data->interlace)
	{
		int pass = 0;
		while (pass <= 6)
		{
			int n = data->lineBytes[pass];
			unsigned char *end = pixels + data->passSize[pass];
			unsigned char *rowBuf = pixels;
			unsigned char *prevRow = NULL;

			while (rowBuf < end)
			{
				applyFilter (rowBuf[0], rowBuf + 1, n - 1,
					prevRow, data->bytesPerPixel);

				prevRow = rowBuf + 1;
				rowBuf += n;
			}

			pixels = end;
			++pass;
		}
	}
	else
	{
		unsigned char *rowBuf = pixels;
		unsigned char *prevRow = NULL;
		int n = data->lineBytes[6];
		int y;

		for (y = 0; y < data->height; ++y)
		{
			applyFilter (rowBuf[0], rowBuf + 1, n - 1,
				prevRow, data->bytesPerPixel);

			prevRow = rowBuf + 1;
			rowBuf += n;
		}
	}
}


#define INIT_TRANS_PACK_RGB(b, x, a)        \
	(x) = ((b) > 8) ? 1 : 0;                \
	(a) = ((b) > 8) ? 6 : 3

#define TRANS_PACK_RGB(d, s, x, a, tmap)       \
	*(d)++ = (s)[0];                           \
	*(d)++ = (s)[1 << x];                      \
	*(d)++ = (s)[2 << x];                      \
	if(x) {                                    \
	*(d)++ = ( memcmp(s,tmap,a)) ? 0xFF: 0x00;  \
	} else {                                   \
	*(d)++ = ((s[0] == tmap[1]) &&             \
	(s[1] == tmap[3]) &&             \
	(s[2] == tmap[5]))               \
	? 0x00: 0xFF;                     \
	}                                          \
	(s) += (a)

#define INIT_TRANS_PACK_GS(b, x, a) \
	(x) = ((b) > 8) ? 1 : 0;        \
	(a) = ((b) > 8) ? 2 : 1

#define TRANS_PACK_GS(d, s, x, a, tmap) \
	*(d)++ = (s)[0];                   \
	if(x) {                            \
	*(d)++ = ((s[0] == tmap[0]) &&   \
	(s[1] == tmap[1]) )    \
	? 0x00: 0xFF;           \
	} else {                           \
	*(d)++ = ((s[1] == tmap[1]))    \
	? 0x00: 0xFF;          \
	}                                  \
	(s) += (a)

#define INIT_TRANS_UNPACK_GS(b, x, m) \
	(x) = (8 - (b));                  \
	(m) = 0xff ^ (0xff >> (x))

#define TRANS_UNPACK_GS(d, s, x, m, b, x0, m0, tmap) \
	*(d)++ = ((*s) & (m)) >> (x);                    \
	*(d) = (*(d-1) == tmap[1])                       \
	? 0x00 : 0xFF;                          \
	(d)++;                                           \
	if (x) {                                         \
	(m) >>= (b);                        \
	(x) -= (b);                         \
	} else {                                \
	(x) = (x0);                         \
	(m) = (m0);                         \
	++(s);                              \
	}


#define INIT_UNPACK(b, x, m)          \
	(x) = (8 - (b));                  \
	(m) = 0xff ^ (0xff >> (x))

#define UNPACK(d, s, x, m, b, x0, m0) \
	*(d)++ = ((*s) & (m)) >> (x);     \
	if (x) {                          \
	(m) >>= (b);                  \
	(x) -= (b);                   \
	} else {                          \
	(x) = (x0);                   \
	(m) = (m0);                   \
	++(s);                        \
	}

#define INIT_PACK3(b, t, x, a)      \
	(x) = ((b) > 8) ? 1 : 0;        \
	(a) = ((t) ? 4 : 3) << (x)

#define PACK3(d, s, x, a, cta)      \
	*(d)++ = (s)[0];                \
	*(d)++ = (s)[1 << x];           \
	*(d)++ = (s)[2 << x];           \
	if (cta) *(d)++ = (s)[3 << x];  \
	(s) += (a)

#define INIT_PACK1(b, t, x, a)      \
	(x) = ((b) > 8) ? 1 : 0;        \
	(a) = ((t) ? 2 : 1) << (x)

#define PACK1(d, s, x, a, cta)      \
	*(d)++ = (s)[0];                \
	if (cta) *(d)++ = (s)[1 << x];  \
	(s) += (a)

static void unpack1 (unsigned char *dst, unsigned char *src, pngData * data)
{
	int i;
	int shift, inc, x0;
	unsigned char mask, m0;

	if (data->trans != NULL && data->colorType != 3)
	{



		if (data->colorType == 2)
		{


			INIT_TRANS_PACK_RGB (data->depth, shift, inc);

			for (i = 0; i < data->width; ++i)
			{
				TRANS_PACK_RGB (dst, src, shift, inc, data->trans);
			}

		}
		else if (data->colorType == 0)
		{


			if (data->depth < 8)
			{


				INIT_TRANS_UNPACK_GS (data->depth, shift, mask);
				x0 = shift;
				m0 = mask;

				for (i = 0; i < data->width; ++i)
				{
					TRANS_UNPACK_GS (dst, src, shift, mask,
						data->depth, x0, m0, data->trans);
				}

			}
			else
			{
				INIT_TRANS_PACK_GS (data->depth, shift, inc);
				for (i = 0; i < data->width; ++i)
				{
					TRANS_PACK_GS (dst, src, shift, inc, data->trans);
				}
			}

		}

	}
	else if (data->depth < 8)
	{

		INIT_UNPACK (data->depth, shift, mask);
		x0 = shift;
		m0 = mask;

		for (i = 0; i < data->width; ++i)
		{
			UNPACK (dst, src, shift, mask, data->depth, x0, m0);
		}
	}
	else if ((data->colorType & 3) == 2)
	{
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift, inc);

		for (i = 0; i < data->width; ++i)
		{
			PACK3 (dst, src, shift, inc, data->colorType & CT_ALPHA);
		}
	}
	else
	{
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift, inc);

		for (i = 0; i < data->width; ++i)
		{
			PACK1 (dst, src, shift, inc, data->colorType & CT_ALPHA);
		}
	}
}

static void unpack2 (unsigned char *dst,
		 unsigned char *src0, unsigned char *src1, pngData * data)
{
	int i;
	int shift0, inc0, x0;
	int shift1, inc1;
	unsigned char mask0, m0;
	unsigned char mask1;

	if (data->trans != NULL && data->colorType != 3)
	{

		if (data->colorType == 2)
		{

			INIT_TRANS_PACK_RGB (data->depth, shift0, inc0);
			INIT_TRANS_PACK_RGB (data->depth, shift1, inc1);

			for (i = 0; i < data->width; ++i)
			{
				TRANS_PACK_RGB (dst, src0, shift0, inc0, data->trans);

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src1, shift1, inc1, data->trans);
				}
			}

		}
		else if (data->colorType == 0)
		{

			if (data->depth < 8)
			{

				INIT_TRANS_UNPACK_GS (data->depth, shift0, mask0);
				INIT_TRANS_UNPACK_GS (data->depth, shift1, mask1);

				x0 = shift0;
				m0 = mask0;

				for (i = 0; i < data->width; ++i)
				{
					TRANS_UNPACK_GS (dst, src0, shift0, mask0,
						data->depth, x0, m0, data->trans);

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src1, shift1, mask1,
							data->depth, x0, m0, data->trans);
					}
				}

			}
			else
			{

				INIT_TRANS_PACK_GS (data->depth, shift0, inc0);
				INIT_TRANS_PACK_GS (data->depth, shift1, inc1);

				for (i = 0; i < data->width; ++i)
				{

					TRANS_PACK_GS (dst, src0, shift0, inc0, data->trans);

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src1, shift1, inc1, data->trans);
					}

				}
			}
		}

	}
	else if (data->depth < 8)
	{

		INIT_UNPACK (data->depth, shift0, mask0);
		INIT_UNPACK (data->depth, shift1, mask1);

		x0 = shift0;
		m0 = mask0;

		for (i = 0; i < data->width; ++i)
		{
			UNPACK (dst, src0, shift0, mask0, data->depth, x0, m0);

			if ((++i) < data->width)
			{
				UNPACK (dst, src1, shift1, mask1, data->depth, x0, m0);
			}
		}
	}
	else if ((data->colorType & 3) == 2)
	{
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift0, inc0);
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift1, inc1);

		for (i = 0; i < data->width; ++i)
		{
			PACK3 (dst, src0, shift0, inc0, data->colorType & CT_ALPHA);

			if ((++i) < data->width)
			{
				PACK3 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}
		}
	}
	else
	{
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift0, inc0);
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift1, inc1);

		for (i = 0; i < data->width; ++i)
		{
			PACK1 (dst, src0, shift0, inc0, data->colorType & CT_ALPHA);

			if ((++i) < data->width)
			{
				PACK1 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}
		}
	}
}

static void unpack3 (unsigned char *dst,
		 unsigned char *src0, unsigned char *src1, unsigned char *src2,
		 pngData * data)
{
	int i;
	int shift0, inc0, x0;
	int shift1, inc1;
	int shift2, inc2;
	unsigned char mask0, m0;
	unsigned char mask1;
	unsigned char mask2;



	if (data->trans != NULL && data->colorType != 3)
	{

		if (data->colorType == 2)
		{

			INIT_TRANS_PACK_RGB (data->depth, shift0, inc0);
			INIT_TRANS_PACK_RGB (data->depth, shift1, inc1);
			INIT_TRANS_PACK_RGB (data->depth, shift2, inc2);

			for (i = 0; i < data->width; ++i)
			{
				TRANS_PACK_RGB (dst, src0, shift0, inc0, data->trans);

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src1, shift1, inc1, data->trans);
				}

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src2, shift2, inc2, data->trans);
				}

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src1, shift1, inc1, data->trans);
				}

			}

		}
		else if (data->colorType == 0)
		{

			if (data->depth < 8)
			{

				INIT_TRANS_UNPACK_GS (data->depth, shift0, mask0);
				INIT_TRANS_UNPACK_GS (data->depth, shift1, mask1);
				INIT_TRANS_UNPACK_GS (data->depth, shift2, mask2);

				x0 = shift0;
				m0 = mask0;

				for (i = 0; i < data->width; ++i)
				{
					TRANS_UNPACK_GS (dst, src0, shift0, mask0,
						data->depth, x0, m0, data->trans);

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src1, shift1, mask1,
							data->depth, x0, m0, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src2, shift2, mask2,
							data->depth, x0, m0, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src1, shift1, mask1,
							data->depth, x0, m0, data->trans);
					}

				}

			}
			else
			{
				INIT_TRANS_PACK_GS (data->depth, shift0, inc0);
				INIT_TRANS_PACK_GS (data->depth, shift1, inc1);
				INIT_TRANS_PACK_GS (data->depth, shift2, inc2);

				for (i = 0; i < data->width; ++i)
				{

					TRANS_PACK_GS (dst, src0, shift0, inc0, data->trans);

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src1, shift1, inc1, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src2, shift2, inc2, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src1, shift1, inc1, data->trans);
					}
				}
			}

		}

	}
	else if (data->depth < 8)
	{
		INIT_UNPACK (data->depth, shift0, mask0);
		INIT_UNPACK (data->depth, shift1, mask1);
		INIT_UNPACK (data->depth, shift2, mask2);

		x0 = shift0;
		m0 = mask0;

		for (i = 0; i < data->width; ++i)
		{
			UNPACK (dst, src0, shift0, mask0, data->depth, x0, m0);

			if ((++i) < data->width)
			{
				UNPACK (dst, src1, shift1, mask1, data->depth, x0, m0);
			}

			if ((++i) < data->width)
			{
				UNPACK (dst, src2, shift2, mask2, data->depth, x0, m0);
			}

			if ((++i) < data->width)
			{
				UNPACK (dst, src1, shift1, mask1, data->depth, x0, m0);
			}
		}
	}
	else if ((data->colorType & 3) == 2)
	{
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift0, inc0);
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift1, inc1);
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift2, inc2);

		for (i = 0; i < data->width; ++i)
		{
			PACK3 (dst, src0, shift0, inc0, data->colorType & CT_ALPHA);

			if ((++i) < data->width)
			{
				PACK3 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK3 (dst, src2, shift2, inc2, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK3 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}
		}
	}
	else
	{
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift0, inc0);
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift1, inc1);
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift2, inc2);

		for (i = 0; i < data->width; ++i)
		{
			PACK1 (dst, src0, shift0, inc0, data->colorType & CT_ALPHA);

			if ((++i) < data->width)
			{
				PACK1 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK1 (dst, src2, shift2, inc2, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK1 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}
		}
	}
}

static void unpack4 (unsigned char *dst,
		 unsigned char *src0, unsigned char *src1,
		 unsigned char *src2, unsigned char *src3, pngData * data)
{
	int i;
	int shift0, inc0, x0;
	int shift1, inc1;
	int shift2, inc2;
	int shift3, inc3;
	unsigned char mask0, m0;
	unsigned char mask1;
	unsigned char mask2;
	unsigned char mask3;



	if (data->trans != NULL && data->colorType != 3)
	{

		if (data->colorType == 2)
		{

			INIT_TRANS_PACK_RGB (data->depth, shift0, inc0);
			INIT_TRANS_PACK_RGB (data->depth, shift1, inc1);
			INIT_TRANS_PACK_RGB (data->depth, shift2, inc2);
			INIT_TRANS_PACK_RGB (data->depth, shift3, inc3);

			for (i = 0; i < data->width; ++i)
			{
				TRANS_PACK_RGB (dst, src0, shift0, inc0, data->trans);

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src1, shift1, inc1, data->trans);
				}

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src2, shift2, inc2, data->trans);
				}

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src1, shift1, inc1, data->trans);
				}

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src3, shift3, inc3, data->trans);
				}

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src2, shift2, inc2, data->trans);
				}

				if ((++i) < data->width)
				{
					TRANS_PACK_RGB (dst, src2, shift2, inc2, data->trans);
				}

			}

		}
		else if (data->colorType == 0)
		{

			if (data->depth < 8)
			{

				INIT_TRANS_UNPACK_GS (data->depth, shift0, mask0);
				INIT_TRANS_UNPACK_GS (data->depth, shift1, mask1);
				INIT_TRANS_UNPACK_GS (data->depth, shift2, mask2);
				INIT_TRANS_UNPACK_GS (data->depth, shift3, mask3);

				x0 = shift0;
				m0 = mask0;

				for (i = 0; i < data->width; ++i)
				{
					TRANS_UNPACK_GS (dst, src0, shift0, mask0,
						data->depth, x0, m0, data->trans);

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src1, shift1, mask1,
							data->depth, x0, m0, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src2, shift2, mask2,
							data->depth, x0, m0, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src1, shift1, mask1,
							data->depth, x0, m0, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src3, shift3, mask3,
							data->depth, x0, m0, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src2, shift2, mask2,
							data->depth, x0, m0, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_UNPACK_GS (dst, src1, shift1, mask1,
							data->depth, x0, m0, data->trans);
					}

				}

			}
			else
			{
				INIT_TRANS_PACK_GS (data->depth, shift0, inc0);
				INIT_TRANS_PACK_GS (data->depth, shift1, inc1);
				INIT_TRANS_PACK_GS (data->depth, shift2, inc2);
				INIT_TRANS_PACK_GS (data->depth, shift3, inc3);

				for (i = 0; i < data->width; ++i)
				{

					TRANS_PACK_GS (dst, src0, shift0, inc0, data->trans);

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src1, shift1, inc1, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src2, shift2, inc2, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src1, shift1, inc1, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src3, shift3, inc3, data->trans);
					}

					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src2, shift2, inc2, data->trans);
					}
					if ((++i) < data->width)
					{
						TRANS_PACK_GS (dst, src1, shift1, inc1, data->trans);
					}

				}
			}

		}

	}
	else if (data->depth < 8)
	{
		INIT_UNPACK (data->depth, shift0, mask0);
		INIT_UNPACK (data->depth, shift1, mask1);
		INIT_UNPACK (data->depth, shift2, mask2);
		INIT_UNPACK (data->depth, shift3, mask3);

		x0 = shift0;
		m0 = mask0;

		for (i = 0; i < data->width; ++i)
		{
			UNPACK (dst, src0, shift0, mask0, data->depth, x0, m0);

			if ((++i) < data->width)
			{
				UNPACK (dst, src1, shift1, mask1, data->depth, x0, m0);
			}

			if ((++i) < data->width)
			{
				UNPACK (dst, src2, shift2, mask2, data->depth, x0, m0);
			}

			if ((++i) < data->width)
			{
				UNPACK (dst, src1, shift1, mask1, data->depth, x0, m0);
			}

			if ((++i) < data->width)
			{
				UNPACK (dst, src3, shift3, mask3, data->depth, x0, m0);
			}

			if ((++i) < data->width)
			{
				UNPACK (dst, src1, shift1, mask1, data->depth, x0, m0);
			}

			if ((++i) < data->width)
			{
				UNPACK (dst, src2, shift2, mask2, data->depth, x0, m0);
			}

			if ((++i) < data->width)
			{
				UNPACK (dst, src1, shift1, mask1, data->depth, x0, m0);
			}
		}
	}
	else if ((data->colorType & 3) == 2)
	{
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift0, inc0);
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift1, inc1);
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift2, inc2);
		INIT_PACK3 (data->depth, data->colorType & CT_ALPHA, shift3, inc3);

		for (i = 0; i < data->width; ++i)
		{
			PACK3 (dst, src0, shift0, inc0, data->colorType & CT_ALPHA);

			if ((++i) < data->width)
			{
				PACK3 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK3 (dst, src2, shift2, inc2, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK3 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK3 (dst, src3, shift3, inc3, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK3 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK3 (dst, src2, shift2, inc2, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK3 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}
		}
	}
	else
	{
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift0, inc0);
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift1, inc1);
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift2, inc2);
		INIT_PACK1 (data->depth, data->colorType & CT_ALPHA, shift3, inc3);

		for (i = 0; i < data->width; ++i)
		{
			PACK1 (dst, src0, shift0, inc0, data->colorType & CT_ALPHA);

			if ((++i) < data->width)
			{
				PACK1 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK1 (dst, src2, shift2, inc2, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK1 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK1 (dst, src3, shift3, inc3, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK1 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK1 (dst, src2, shift2, inc2, data->colorType & CT_ALPHA);
			}

			if ((++i) < data->width)
			{
				PACK1 (dst, src1, shift1, inc1, data->colorType & CT_ALPHA);
			}
		}
	}
}

static bool handleImageData (unsigned char *pixels, int pixelsLength,
				 imageDstPtr dst, pngData * data)
{
	int pixelSize = ((data->colorType & (CT_PALETTE | CT_COLOR)) ? 3 : 1) +
		(((data->colorType & CT_ALPHA) || (data->trans != NULL)) ? 1 : 0);
	int rgba = data->colorType;

	int sendDirect = FALSE;

	unsigned char *scanline = NULL;
	unsigned char *passes[7];
	int y;

	(void) pixelsLength;

	filterAllRows (pixels, data);


	if (data->interlace)
	{
		scanline = (unsigned char *)i51AdeOsMalloc((data->width * pixelSize), __FILE__, __LINE__);
		if (scanline == NULL)
		{
			return FALSE;
		}

		passes[0] = pixels;
		for (y = 1; y < 7; ++y)
		{
			passes[y] = passes[y - 1] + data->passSize[y - 1];
		}
	}
	else
	{
		passes[6] = pixels;
	}

	if ((data->depth == 8) &&
		((data->colorType & CT_PALETTE) || (data->trans == NULL)))
	{

		sendDirect = TRUE;
	}
	else if (scanline == NULL)
	{
		scanline = (unsigned char *)i51AdeOsMalloc((data->width * pixelSize), __FILE__, __LINE__);
		
		if (scanline == NULL)
		{
			return FALSE;
		}
	}

	for (y = 0; y < data->height; ++y)
	{
		if ((y & 1) || !data->interlace)
		{
			if (sendDirect)
			{
				dst->sendPixels (dst, y, passes[6] + 1, rgba);
			}
			else
			{
				unpack1 (scanline, passes[6] + 1, data);
				dst->sendPixels (dst, y, scanline, rgba);
			}

			passes[6] += data->lineBytes[6];
		}
		else
		{
			switch (y & 6)
			{
			case 2:
			case 6:
				unpack2 (scanline, passes[4] + 1, passes[5] + 1, data);
				passes[4] += data->lineBytes[4];
				passes[5] += data->lineBytes[5];
				break;

			case 4:
				unpack3 (scanline,
					passes[2] + 1, passes[5] + 1, passes[3] + 1, data);
				passes[2] += data->lineBytes[2];
				passes[5] += data->lineBytes[5];
				passes[3] += data->lineBytes[3];
				break;

			case 0:
				unpack4 (scanline,
					passes[0] + 1, passes[5] + 1,
					passes[3] + 1, passes[1] + 1, data);
				passes[0] += data->lineBytes[0];
				passes[5] += data->lineBytes[5];
				passes[3] += data->lineBytes[3];
				passes[1] += data->lineBytes[1];
				break;
			}

			dst->sendPixels (dst, y, scanline, rgba);
		}
	}

	if (scanline != NULL){
		i51AdeOsFree((void*)scanline);
	}

	return TRUE;
}


static unsigned long getInt (imageSrcPtr src)
{
	unsigned long n;

	n = ((unsigned long) src->getByte (src) << 24);
	n |= ((unsigned long) src->getByte (src) << 16);
	n |= ((unsigned long) src->getByte (src) << 8);
	n |= ((unsigned long) src->getByte (src) << 0);

	return n;
}

static bool getChunk (imageSrcPtr src, unsigned long *chunkType, long *chunkLength)
{
	*chunkLength = getInt (src);
	*chunkType = getInt (src);

	return (*chunkType != IEND_CHUNK) ? TRUE : FALSE;
}

static unsigned long png_skip (imageSrcPtr src, int nBytes, unsigned long CRC)
{
	static unsigned char buf[64];
	while (nBytes > 0)
	{
		int n = (nBytes > (long) sizeof (buf)) ? (long) sizeof (buf) : nBytes;
		src->getBytes (src, buf, n);
		CRC = update_crc (CRC, buf, n);
		nBytes -= n;
	}

	return CRC;
}


static bool signatureOK (imageSrcPtr src)
{
	unsigned char buf[8];
	static unsigned char png_header[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

	src->getBytes (src, buf, sizeof (buf));

	return ( memcmp (png_header, buf, sizeof (png_header)) == 0) ? TRUE : FALSE;
}

static bool headerOK (pngData * data)
{

	if (((data->width <= 0) || (data->height <= 0))
		|| (data->compress != 0)
		|| (data->filter != 0)
		|| ((data->interlace != 0) && (data->interlace != 1))
		|| ((data->depth > 16) || (data->depth & (data->depth - 1))))
	{

		return FALSE;
	}



	switch (data->colorType)
	{
	case 0:
		return TRUE;

	case 2:
	case 4:
	case 6:
		return (data->depth >= 8) ? TRUE : FALSE;

	case 3:
		return (data->depth <= 8) ? TRUE : FALSE;

	default:
		return FALSE;

	}
}

static unsigned long init_CRC (unsigned long chunkType)
{
	unsigned char buf[4];

	buf[0] = (unsigned char)( (chunkType >> 24) & 0xff);
	buf[1] = (unsigned char)( (chunkType >> 16) & 0xff);
	buf[2] = (unsigned char)( (chunkType >> 8) & 0xff);
	buf[3] = (unsigned char)( (chunkType >> 0) & 0xff);

	return update_crc (0xffffffffL, buf, 4);
}

static bool check_CRC (imageSrcPtr src, unsigned long CRC)
{
	unsigned long transmitted_CRC = getInt (src);

	CRC = (~CRC) & 0xffffffffl;



	return (transmitted_CRC == CRC) ? TRUE : FALSE;
}



static unsigned long crc_table[256];


static int crc_table_computed = 0;


static void make_crc_table (void)
{
	unsigned long c;
	int n, k;

	for (n = 0; n < 256; n++)
	{
		c = (unsigned long) n;
		for (k = 0; k < 8; k++)
		{
			if (c & 1)
			{
				c = 0xedb88320L ^ (c >> 1);
			}
			else
			{
				c = c >> 1;
			}
		}
		crc_table[n] = c;
	}
	crc_table_computed = 1;
}


static unsigned long update_crc (unsigned long crc, unsigned char *buf, int len)
{
	unsigned long c = crc;
	int n;

	if (!crc_table_computed)
	{
		make_crc_table ();
	}

	for (n = 0; n < len; n++)
	{
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}

	return c;
}



/**********************************************************/
/********************解码器接口封装*************************/
unsigned char lp_png_header[16] = { 0x89, 'P', 'N', 'G',
0x0d, 0x0a, 0x1a, 0x0a,
0x00, 0x00, 0x00, 0x0d,
0x49, 0x48, 0x44, 0x52
};

#ifndef offsetof
#define   offsetof(s,m)     ((int)&(((s*)0)->m))
#endif

#define testimg 0
#define preloadPix 0

#define CLIP_X1 0
#define CLIP_Y1 1
#define CLIP_X2 2
#define CLIP_Y2 3

typedef struct _imgSrc
{
	imageSrcData super;
	uchar *buf;
	int offset;
	int len, remaining;
} _imageSrcData, *_imageSrcPtr;

static int getByte (imageSrcPtr self)
{
	_imageSrcPtr r = (_imageSrcPtr) self->ptr;

	if (r->remaining < 0)
		return 0;

	--(r->remaining);

	return (int)((r->buf)[r->offset++]);
}

static int getBytes (imageSrcPtr self, uchar * buf, int len)
{
	_imageSrcPtr r = (_imageSrcPtr) self->ptr;

	if (r->remaining < 0)
		return 0;

	if (len > r->remaining)
		len = r->remaining;

	if (buf != NULL)
	{
		memcpy (buf, &((r->buf)[r->offset]), len);
	}

	r->remaining -= len;
	r->offset += len;

	return len;
}


static void done (imageSrcPtr self)
{
	(void) self;
}

static void skip (imageSrcPtr self, int numBytes)
{
	getBytes (self, NULL, numBytes);
}

static void reset (imageSrcPtr self)
{
	_imageSrcPtr p = (_imageSrcPtr) self->ptr;

	p->offset = 0;
	p->remaining = p->len;
}
static long getPos (imageSrcPtr self)
{
	_imageSrcPtr p = (_imageSrcPtr) self->ptr;

	return p->offset;
}

static void seek (imageSrcPtr self, long pos)
{
	_imageSrcPtr p = (_imageSrcPtr) self->ptr;

	p->offset = pos;
	p->remaining = p->len - pos;
}

static void setImageColormap (imageDstPtr self, long *map, int length)
{
	_imageDstPtr p = (_imageDstPtr) self;
	int i = 0;
	p->hasPal = 1;

	for(i = 0; i < length && i < 256; i++) {
		p->cmap[i] = (lp_pixel_type)( LP_RGB24TORGB16(map[i]) );
	}
}

static void setImageTransparencyMap (imageDstPtr self, unsigned char *map,
						 int length, int palLength)
{
	_imageDstPtr p = (_imageDstPtr) self;
	p->hasTrans = 1;
}


static bool setImageSize (imageDstPtr self, int width, int height, int colorType)
{
	_imageDstPtr p = (_imageDstPtr) self;

	if ((colorType == CT_COLOR) || (colorType == (CT_COLOR | CT_ALPHA))) {
		p->hasPal = 0;
		p->hasTrans = 0;
	} else if (colorType & CT_PALETTE) {
		p->hasPal = 1;
		p->hasTrans = 0;
	}
	return TRUE;
}

//写内存
static void sendPixelsColor (imageDstPtr self, int y, uchar * pixels, int pixelType)
{
	_imageDstPtr p = (_imageDstPtr) self;
	int x, offx, offy;
	lp_pixel_type *pixelPtr;
	int src_step;
	unsigned short TransColor = 0;

	if( y< p->srcStartY || y>= p->srcStartY + p->needH || NULL == self) return;

	if ((pixelType == CT_COLOR) || (pixelType == (CT_COLOR | CT_ALPHA)))
	{
		src_step = 3;
		if (pixelType & CT_ALPHA)
		{
			src_step++;
		} else if (p->hasTrans) {
			src_step++;
		}
	} else {
		src_step =1;
		if ((pixelType & (CT_ALPHA | CT_COLOR)) == CT_ALPHA) {
			src_step++;
		} else if (p->hasTrans) {
			if ((pixelType & CT_COLOR) == 0) {
				src_step++;
			} 
		}
	}

	pixels += p->srcStartX * src_step;
	offx = p->dstStartX - p->srcStartX;
	offy = p->dstStartY - p->srcStartY;
	pixelPtr = &(p->dstMapPtr[(y+offy) * p->dstMapW + p->srcStartX+offx]);
	TransColor = (unsigned short)(p->transColor);
	if ((pixelType == CT_COLOR) || (pixelType == (CT_COLOR | CT_ALPHA))){
		if ( 0 == p->decodeType ){
			for (x = 0; x <p->needW; ++x){
				int r = pixels[0] >> 3;
				int g = pixels[1] >> 2;
				int b = pixels[2] >> 3;
				*pixelPtr = LP_RGB2PIXEL (r, g, b);
				pixels += src_step;
				pixelPtr++;
			}
		}else{
			for (x = 0; x <p->needW; ++x){
				int r = pixels[0] >> 3;
				int g = pixels[1] >> 2;
				int b = pixels[2] >> 3;
				unsigned short color = LP_RGB2PIXEL (r, g, b);
				if ( TransColor != color){
					*pixelPtr = color;
				}

				pixels += src_step;
				pixelPtr++;
			}
		}
	} else {
		if (0 == p->decodeType){
			for (x = 0; x <p->needW; ++x) {
				*pixelPtr = p->cmap[*pixels];
				pixels += src_step;
				pixelPtr++;
			}
		}else{
			for (x = 0; x <p->needW; ++x) {
				if ( TransColor != p->cmap[*pixels]){
					*pixelPtr = p->cmap[*pixels];
				}
				pixels += src_step;
				pixelPtr++;
			}
		}
	}
}

//写文件
static void sendPixelsColorEx(imageDstPtr self, int y, uchar * pixels, int pixelType)
{
	_imageDstPtr p = (_imageDstPtr) self;
	int x = 0, src_step=0;
	iFILE SrcFile = iNULL;
	iU16 *RowPixs = iNULL;
	iU32 WriteSize = 0;

	if( y< p->srcStartY || y>= p->srcStartY + p->needH || NULL == self) return;

	SrcFile = (iFILE)(p->dstMapPtr);
	if ( 0 == SrcFile ) return ;

	if ((pixelType == CT_COLOR) || (pixelType == (CT_COLOR | CT_ALPHA)))
	{
		src_step = 3;
		if (pixelType & CT_ALPHA)
		{
			src_step++;
		} else if (p->hasTrans) {
			src_step++;
		}
	} else {
		src_step =1;
		if ((pixelType & (CT_ALPHA | CT_COLOR)) == CT_ALPHA) {
			src_step++;
		} else if (p->hasTrans) {
			if ((pixelType & CT_COLOR) == 0) {
				src_step++;
			} 
		}
	}

	pixels += p->srcStartX * src_step;
	RowPixs = (iU16*)i51AdeOsMalloc((p->needW)*sizeof(iU16), __FILE__, __LINE__);
	if ( iNULL == RowPixs ){
		return ;
	}

	if ((pixelType == CT_COLOR) || (pixelType == (CT_COLOR | CT_ALPHA)))
	{
		for (x = 0; x <p->needW; ++x){
			int r = pixels[0] >> 3;
			int g = pixels[1] >> 2;
			int b = pixels[2] >> 3;
			RowPixs[x] = LP_RGB2PIXEL (r, g, b); 
			pixels += src_step;
		}
	} else {
		for (x = 0; x <p->needW; ++x) {
			RowPixs[x] = p->cmap[*pixels];
			pixels += src_step;
		}
	}

	if ( iFALSE == i51AdeOsFileWrite(SrcFile, RowPixs, (p->needW)*sizeof(iU16), &WriteSize)){
		iLog("sendPixelsColorEx write file err SrcFile is %x", SrcFile);
	}

	if ( iNULL !=RowPixs ){
		i51AdeOsFree((void*)RowPixs);
	}
}

//////////////////////////////////////////////////////////////////////////
//新增函数
static bool decode_png_image (imageSrcPtr src, imageDstPtr dst)
{
	bool retval;
	long *paletteDataPtr;
	unsigned char *transDataPtr;

	paletteDataPtr = (long*)i51AdeOsMalloc(sizeof(long)*256, __FILE__, __LINE__);
	if( !paletteDataPtr ) return FALSE;

	transDataPtr = (unsigned char *)i51AdeOsMalloc(256, __FILE__, __LINE__);
	if( !transDataPtr ){
		i51AdeOsFree((void*)paletteDataPtr);
		return FALSE;
	}

	retval = PNGdecodeImage_real (src, dst, paletteDataPtr, transDataPtr);

	i51AdeOsFree((void*)paletteDataPtr);
	i51AdeOsFree((void*)transDataPtr);

	return retval;
}

static iBOOL png_create_ImageSrc(_imageSrcPtr _src,  unsigned char *SrcData, int SrcDataLen)
{
	if ( iNULL == _src || iNULL == SrcData) 
		return iFALSE;

	_src->super.ptr = _src;
	_src->super.done = done;
	_src->super.reset = reset;
	_src->super.getByte = getByte;
	_src->super.getBytes = getBytes;
	_src->super.skip = skip;
	_src->super.seek = seek;
	_src->super.getpos = getPos;

	_src->buf = SrcData;
	_src->len = SrcDataLen;
	_src->super.reset ((imageSrcPtr) _src);

	return iTRUE;
}

static iBOOL png_create_ImageDst(iS32 type, iU32 TransColor, _imageDstPtr _dst,  iRECT* SrcRect, iRECT* DesRect, iHANDLE DesHandle, iU8 DesHandleType)
{
	int clipx1, clipy1, clipx2, clipy2;
	int dstStartX, dstStartY, dstEndX, dstEndY;
	iU16 width = 0, height = 0;

	if ( iNULL == _dst) return iFALSE;

	memset( _dst, 0, sizeof(_imageDstData) );

	_dst->super.ptr = _dst;
	_dst->super.depth = 8;
	_dst->super.setColormap = setImageColormap;
	_dst->super.setTransMap = setImageTransparencyMap;
	_dst->super.setSize = setImageSize;
	_dst->dstMapPtr = (lp_pixel_type *)DesHandle;
	_dst->dstMapPtrType = (int)DesHandleType;

	if ( 0 == DesHandleType ){
		_dst->super.sendPixels = sendPixelsColorEx;
	}else{
		_dst->super.sendPixels = sendPixelsColor;
	}

	//确定解码后，须更新区域
	clipx1 = DesRect->X;
	clipx2 = DesRect->X + DesRect->Width - 1;
	clipy1 = DesRect->Y;
	clipy2 = DesRect->Y + DesRect->Height - 1;

	dstStartX = DesRect->X;
	dstEndX = DesRect->X + SrcRect->Width - 1;
	dstStartY = DesRect->Y;
	dstEndY = DesRect->Y + SrcRect->Height - 1;

	dstStartX = dstStartX < 0 ? 0 : dstStartX;  
	dstEndX = dstEndX > clipx2 ? clipx2 : dstEndX;
	dstStartY = dstStartY < 0 ? 0 : dstStartY;  
	dstEndY = dstEndY > clipy2 ? clipy2 : dstEndY;

	_dst->srcStartX = SrcRect->X;
	_dst->srcStartY = SrcRect->Y;
	_dst->needW = dstEndX - dstStartX + 1;
	_dst->needH = dstEndY - dstStartY  + 1;
	_dst->dstStartX = dstStartX;
	_dst->dstStartY = dstStartY;

	_dst->decodeType = type;
	if ( 0 == type ){
		_dst->dstMapW = _dst->needW;
		_dst->dstMapH = _dst->needH;
	}else{
		//dst 宽高
		i51AdeMmiGetScreenScale ( &width , &height );
		_dst->dstMapW = (int)width;
		_dst->dstMapH = (int)height;
	}

	_dst->transColor = TransColor;
	if (TransColor > 0xFFFF){
		_dst->decodeType = 0;
	}

	return iTRUE;
}

iBOOL png_get_CheckFileHead(iHANDLE ImageHandle, iU8 ImageHandleType)
{
//  author : yepan
//  since  : 2012-7-25
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			ImageHandle 数据源指针
//			ImageHandleType	0==数据源是文件句柄,1==数据源是内存指针
//  return :
//			iTRUE==是， iFALSE==不是
//  note   :
//			检查是否为PNG格式

	iU32 length = 0;
	iU8 buffer[8] = {0};
	iU8 *BufPtr = iNULL;

	if ( iNULL == ImageHandle ) return iFALSE;

	//文件
	if ( 0 ==ImageHandleType ){

		if ( iFALSE == i51AdeOsFileSetSeek((iFILE)ImageHandle, 0, i51_OS_FL_SEEK_HEAD)){
			iLog("i51KitPngDecodeImage file setseek err");
			return iFALSE;
		}

		if ( iFALSE == i51AdeOsFileRead((iFILE)ImageHandle, (void*)buffer, 8, &length) ){
			iLog("i51KitPngGetAttribute read file err");
			return iFALSE;
		}

		if ( iFALSE == i51AdeOsFileSetSeek((iFILE)ImageHandle, 0, i51_OS_FL_SEEK_HEAD)){
			iLog("i51KitPngDecodeImage file setseek err");
			return iFALSE;
		}
	}else if ( 1 ==ImageHandleType ){
		i51AdeStdMemcpy16(buffer, ImageHandle, 8);
	}

	BufPtr = buffer;
	if ( 0 == memcmp(BufPtr, "\x89PNG\x0d\x0a\x1a\x0a", 8) ){
		return iTRUE;
	}
	
	return iFALSE;
}

iBOOL png_get_attribute(iHANDLE ImageHandle, iU8 ImageHandleType,  iU32 * ImageWidth, iU32 * ImageHeight)
{
//  author : yepan
//  since  : 2012-7-20
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			ImageHandle 数据源指针
//			ImageHandleType	0==数据源是文件句柄,1==数据源是内存指针
//			ImageWidth	输出的图片宽度
//			ImageHeight 输出的图片高度
//
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			获取图片信息

	iU32 length = 0;
	iU8 buffer[24] = {0};
	iU8 *BufPtr = iNULL;

	if ( iNULL == ImageHandle ) return iFALSE;

	//文件
	if ( 0 ==ImageHandleType ){

		if ( iFALSE == i51AdeOsFileSetSeek((iFILE)ImageHandle, 0, i51_OS_FL_SEEK_HEAD)){
			iLog("i51KitPngDecodeImage file setseek err");
			return iFALSE;
		}

		if ( iFALSE == i51AdeOsFileRead((iFILE)ImageHandle, (void*)buffer, 24, &length) ){
			iLog("i51KitPngGetAttribute read file err");
			return iFALSE;
		}

		if ( iFALSE == i51AdeOsFileSetSeek((iFILE)ImageHandle, 0, i51_OS_FL_SEEK_HEAD)){
			iLog("i51KitPngDecodeImage file setseek err");
			return iFALSE;
		}
	}else if ( 1 ==ImageHandleType ){
		i51AdeStdMemcpy16(buffer, ImageHandle, 24);
	}


	BufPtr = buffer;
	if ( 0 == memcmp(BufPtr, "\x89PNG\x0d\x0a\x1a\x0a", 8) ){
		if( *( (iU32*)(BufPtr + 8) ) == (13 << 24) ){ //小端
			if ( 0 == memcmp(BufPtr + 12, "IHDR", 4) ){
				BufPtr += 16;
				*ImageWidth = (BufPtr[0] << 24) | (BufPtr[1] << 16) | (BufPtr[2] << 8) | (BufPtr[3]);
				BufPtr += 4;
				*ImageHeight = (BufPtr[0] << 24) | (BufPtr[1] << 16) | (BufPtr[2] << 8) | (BufPtr[3]);
			}
		}
	}

	return iTRUE;
}

iBOOL png_decode(iS32 type, iU32 TransColor, iHANDLE ImageHandle, iU8 ImageHandleType, iU32 ImageFileSize, iRECT* SrcRect, iHANDLE DesHandle, iU8 DesHandleType, iRECT* DesRect)
{
//  author : yepan
//  since  : 2012-7-20
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			type				0==解码， 1==解码并绘制
//			TransColor			透明色
//			ImageHandle			数据源指针
//			ImageHandleType		0==数据源是文件句柄,1==数据源是内存指针
//			ImageFileSize		大小, 如果数据源为文件句柄此项赋0
//			SrcRect				数据源区域
//			DesHandle			目的对象指针
//			DesHandleType		0==目的对象是文件句柄,1==目的对象是内存指针
//			DesRect				目标区域
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			解码png图片。

	iBOOL flag = iTRUE;
	iS32 FileSize = ImageFileSize;
	iU32 ReadSize = 0;
	iU8 *FileBuf = iNULL;
	iS16 TempX = 0, TempY=0;
	iS16 SrcStartX=0, SrcEndX=0, SrcStartY=0, SrcEndY=0;
	iU32 ImageWidth=0, ImageHeight=0;
	iRECT DRect={0}, SRect={0};

	imageSrcPtr  src = NULL;
	_imageSrcPtr _src = NULL;
	imageDstPtr  dst = NULL;
	_imageDstPtr _dst = NULL;

	if ( iNULL == ImageHandle || iNULL == DesHandle ){
		iLog("i51KitPngDecodeImage parmater is err ImageHandle is %x ,DesHandle is %x", ImageHandle, DesHandle);
		return iFALSE;
	} 

	//检验PNG头
	if ( iFALSE == png_get_CheckFileHead(ImageHandle, ImageHandleType)){
		iLog("png_decode file is not PNG");
		return iFALSE;
	}

	//将png文件读到内存
	if ( 0 == ImageHandleType ){

		if ( iFALSE == i51AdeOsFileGetSize((iFILE)ImageHandle, &FileSize)){
			iLog("i51KitPngDecodeImage get flie size err");
			flag = iFALSE;
			goto FREE_MEMORY;
		}

		FileBuf = (iU8*)i51AdeOsMalloc(FileSize, __FILE__, __LINE__);

		if ( iNULL == FileBuf ){
			iLog("i51KitPngDecodeImage malloc err");
			flag = iFALSE;
			goto FREE_MEMORY;
		}
	
		if ( iFALSE == i51AdeOsFileSetSeek((iFILE)ImageHandle, 0, i51_OS_FL_SEEK_HEAD)){
			iLog("png_decode file setseek err");
			return iFALSE;
		}

		if ( iFALSE == i51AdeOsFileRead((iFILE)ImageHandle, (void*)FileBuf, FileSize, &ReadSize) ){
			iLog("i51KitPngGetAttribute read file err");
			flag = iFALSE;
			goto FREE_MEMORY;
		}

	}else if ( 1 == ImageHandleType ){

		FileBuf = (iU8*)ImageHandle;

	}else{
		return iFALSE;
	}
	
	//获取图片大小
	if ( iFALSE == png_get_attribute((iHANDLE)FileBuf, 1,  &ImageWidth, &ImageHeight) ){
		flag = iFALSE;
		goto FREE_MEMORY;
	}

	//图片裁剪
	SrcStartX = 0;
	SrcEndX = ImageWidth - 1;
	SrcStartY = 0;
	SrcEndY = ImageHeight - 1;
	SRect.X = SrcRect->X < SrcStartX ? SrcStartX : SrcRect->X;
	SRect.Y = SrcRect->Y < SrcStartY ? SrcStartY : SrcRect->Y;
	TempX = SrcRect->X + SrcRect->Width;
	TempY = SrcRect->Y + SrcRect->Height;
	TempX = TempX < SrcEndX ? TempX : SrcEndX;
	TempY = TempY < SrcEndY ? TempY : SrcEndY;
	SRect.Width = TempX - SRect.X + 1;
	SRect.Height = TempY - SRect.Y + 1;

	if ( 0 == DesHandleType ){
		DRect.X = 0;
		DRect.Y = 0;
		DRect.Width = SRect.Width;
		DRect.Height = SRect.Height;
	}else{
		i51AdeStdMemcpy16(&DRect, DesRect, sizeof(iRECT));
	}

	_src = (_imageSrcPtr)i51AdeOsMalloc(sizeof(_imageSrcData), __FILE__, __LINE__);
	if ( iNULL == _src ){
		iLog("i51KitPngDecodeImage malloc _imageSrcData err");
		flag = iFALSE;
		goto FREE_MEMORY;
	}

	_dst = (_imageDstPtr)i51AdeOsMalloc(sizeof(_imageDstData), __FILE__, __LINE__);
	if ( iNULL == _src ){
		iLog("i51KitPngDecodeImage malloc _imageDstData err");
		flag = iFALSE;
		goto FREE_MEMORY;
	}

	png_create_ImageDst(type, TransColor, _dst,  &SRect, &DRect, DesHandle, DesHandleType);
	png_create_ImageSrc(_src, FileBuf, (int)FileSize);
	
	src = (imageSrcPtr)_src;
	dst = (imageDstPtr)_dst;

	//解码
	if ( 0 == decode_png_image (src, dst) ){
		flag = iFALSE;
	}
	
FREE_MEMORY:
	if ( 0 == ImageHandleType && iNULL != FileBuf ){
		i51AdeOsFree((void*)FileBuf);
		FileBuf = iNULL;
	}

	if ( iNULL != _src){
		i51AdeOsFree((void*)_src);
		_src = iNULL;
	}

	if ( iNULL != _dst){
		i51AdeOsFree((void*)_dst);
		_dst = iNULL;
	}

	return flag;
}