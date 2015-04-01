#ifndef __I51ADEMSTAR_H__
#define __I51ADEMSTAR_H__

#include "i51.h"

typedef enum
{
	I51_RECORDER_ACTION_NONE = 0,
	I51_RECORDER_ACTION_START_PCM,
	I51_RECORDER_ACTION_STOP_PCM,
	I51_RECORDER_ACTION_START_AMR,
	I51_RECORDER_ACTION_STOP_AMR
}I51_RECORDER_ACTION_FLAG;
extern I51_RECORDER_ACTION_FLAG g_i51RecorderActionFlag;


typedef enum {
	i51_REC_AMR = 0 ,
	i51_REC_PCM
} ;


typedef    struct{
	
	iBOOL bRecord;
    iU32 mediaType;
	void *obj;    
    iU32 len;
	iU16 fileName[128];

	void (*appcb ) ( void* stream , iU32 length );
	void (*finishCb) ( iBOOL success ) ;
	void (*stopCb) ( iBOOL success ) ;
	
}i51RecoderStruct;

extern i51RecoderStruct  i51Recoder;

#endif