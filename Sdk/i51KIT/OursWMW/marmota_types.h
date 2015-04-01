#if !(defined __MARMOTA_TYPES_H__)
#define __MARMOTA_TYPES_H__

#include "marmota_backend_defines.h"
#include "marmota_sysdefines.h"


typedef unsigned char		mmU8;
typedef unsigned char * 	mmPU8;
typedef unsigned short		mmU16;
typedef unsigned short *	mmPU16;
typedef unsigned int		mmU32;
typedef unsigned int *		mmPU32;
typedef signed char			mmS8;
typedef signed char *		mmPS8;
typedef signed short		mmS16;
typedef signed short *		mmPS16;
typedef signed int			mmS32;
typedef signed int *		mmPS32;
typedef float 				mmFloat;
typedef int					mmBool;

#ifndef WCHAR
typedef unsigned short WCHAR;
#endif

#define mmFalse				0
#define mmTrue				1
//enum mmBoolResult{mmFalse, mmTrue};

//typedef void (*mmCallBackFunc)();
//typedef void (*mmHardwareEvenCallBack)(mmHardwareEventEnum eventType, mmU16 wParam, mmU16 mParam);

#define MM_GET_ARRAY_SIZE(x)	(sizeof(x) / sizeof(*(x + 0)))

typedef struct {mmS32 x; mmS32 y;}mmPoint2D;
#define mmPoint mmPoint2D
typedef struct {mmS32 x; mmS32 y; mmU32 width; mmU32 height;}mmRectangle;
typedef struct {mmFloat x; mmFloat y;}mmVector2D;


typedef struct mmImageRawData{
	int x, y, w, h;
}mmImageData;

typedef enum {mmGraphicsTypes_MTK, mmGraphicsTypes_MSTAR, mmGraphicsTypes_ST}mmGraphicsTypes;

typedef struct {
	int w, h;
///	mmBool isFullScreen;
}mmDisplayMode;

typedef struct mmDisplayDevice{
	mmGraphicsTypes graphicsType;
	mmDisplayMode displayMode;
	//mmU8 (*getBufferFromRawImageData)(mmU8 *imagedata, mmU32 w, mmU32 h);
}mmDisplayDevice;

typedef mmS32 mmHandle;

typedef struct mmAudioData{
	///void *system_handle;
	/*const*/ unsigned char *data;
	mmU32 len; 
	mmU32 format;
}mmAudioData;

#endif

