
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by osca.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/
#include "i51AdeCnf.h"
#include "i51AdeMmi.h"
#include "i51Script.h"
#include "i51AdeMStar.h"


#include "ven_std.h"
#include "ven_sdk.h"
#include "ven_handle.h"
#include "ven_graphics.h"
#include "ven_font.h"     
#include "ven_stc.h"
#include "ven_ts.h"
#include "ven_os.h"
#include "ven_kpd.h"
#include "ven_setting.h"
#include "ven_clipboard.h"
#include "ven_dbg.h"
#include "ven_drv.h"
#include "ven_codec.h"
#include "ven_socket.h"
#include "ven_util.h"
#include "ven_ui.h"
#include "ven_wap.h"
#include "ven_cam.h"
#include "ven_editor.h"
#include "ven_msg.h"
#include "ven_time.h"

//#define i51ADAPTER_DEBUG

#define R_OF_565(p)      (((p)&0xf800) >> 8)
#define G_OF_565(p)      (((p)&0x7e0) >> 3)
#define B_OF_565(p)      (((p)&0x1f) << 3)

extern iU32 g_i51PkigHandle;

extern void* i51AdeOsMalloc ( iU32 length , const iCHAR* file , iU32 line );
extern void i51AdeStdMemset16(void* dest, iU16 value, iU32 length);
extern iU16 *i51S_AdeFileFixPath(const iU16 *oldpath); //外部引用，非标准接口

typedef void (*i51PlayerCB)(iHANDLE handle);    
typedef void (*i51ADEPLAYRECORDCB)( iBOOL results );   

extern ven_GraphicsCanvas *g_i51PkigCanvas ;
extern iS32 g_i51MediaHandle ;
extern ven_FontAttr *g_i51PkigFontNormal ;

i51RecoderStruct  i51Recoder = { 0 } ;
I51_RECORDER_ACTION_FLAG g_i51RecorderActionFlag = I51_RECORDER_ACTION_NONE;


static i51ADECALLBACK i51AdeCallback = 0 ;
static i51ADECALLBACK i51RecorderStop = 0 ;
static i51ADEPLAYRECORDCB i51PlayRecordCb = 0 ; 
static iBOOL i51AdeMmiMediaPlayerLock = iFALSE ;
static iBOOL i51AdeMmiMediaPlaying = iFALSE ;
static iU32 g_i51LastVolume = VEN_MEDIA_VOLUME_0;
/*========================================================
	Media Player -- start
=========================================================*/
static void _i51AdeMmiMediaPlayerMessageProcessing( iU32 timerid );
static iBOOL _i51AdeMediaPlayerDeviceStopPlayImmediately();
static iBOOL _i51AdeMmiMediaPlayerIsWorking();

extern iS32 g_i51MediaHandle;
extern iU32 g_i51PkigHandle;


#define I51_MEDIA_PLAYER_OBJECT_NUM			(8)
#define I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE	(32)
#define I51_MEIDA_PLAYER_INVALIDATE_ID		(0xFF)
#define I51_WAV_HEAD_LEN					(44)
#define I51_MEDIA_PLAYER_SECTOR_NUM			(3)
#define I51_MEDIA_PLAYER_SECTOR_SIZE		(1024 * 50)

typedef enum
{
	i51_PLAYMODE_MIX = 0,
	i51_PLAYMODE_SINGLE,
	i51_PLAYMODE_INVALIDATE
}i51PLAYMODE;

typedef enum
{
	i51_PLAYSTATUS_PLAYING = 0,
	i51_PLAYSTATUS_PAUSE,
	i51_PLAYSTATUS_STOP
}i51PLAYSTATUS;

typedef struct{
	char magic[4];		//0x52, 0x49, 0x46, 0x46 "RIFF"标志 
	int  len;			//文件长度 - 8 
	char wav[4];		//0x57, 0x41, 0x56, 0x45   "WAVE"标志 
	char fmt[4];		//0x66, 0x6D, 0x74, 0x20, // "fmt"标志 
	int  ignore;		//0x10,0x00,0x00,0x00 
	u16  format; 		//0x01,0x00 格式类别（10H为PCM形式的声音数据) 
	u16  channel;		//0x01  通道数
	int  sample; 		 //0x40, 0x1F,0x00,0x00   采样率
	int  nAvgBytesPerSec; //0x80,0x3E,0x00,0x00 波形音频数据传送速率，其值为通道数×每秒数据位数×每   样本的数据位数／8。播放软件利用此值可以估计缓冲区的大小。 
	u16  align; 		//0x02  数据块的调整数（按字节算的），其值为通道数×每样本的数据位值／8
	u16  elemt; 		//0x10 每样本的数据位数，表示每个声道中各个样本的数据位数。
	char data[4];		//0x64, 0x61, 0x74, 0x61,数据标记符＂data＂ 
	int  pcmlen; 		//pcm len 语音数据的长度
}wavheader_t;

typedef enum
{
	i51_WAVSAMPLE_8_KHZ = 0,
	i51_WAVSAMPLE_11_025_KHZ,
	i51_WAVSAMPLE_12_KHZ,
	i51_WAVSAMPLE_16_KHZ,
	i51_WAVSAMPLE_22_KHZ,
	i51_WAVSAMPLE_24_KHZ,
	i51_WAVSAMPLE_32_KHZ,
	i51_WAVSAMPLE_44_1_KHZ,
	i51_WAVSAMPLE_48_KHZ,
	i51_WAVSAMPLE_64_KHZ,
	i51_WAVSAMPLE_96_KHZ,
	i51_WAVSAMPLE_MAX
}i51WAVSAMPLE;

typedef struct
{
	i51WAVSAMPLE eSampleType;
	iU8 nBit;
	iS32 sampleValue;
}i51WavSampleStruct;

static const i51WavSampleStruct stWavSample[] = 
{
	{i51_WAVSAMPLE_8_KHZ,  8 , 8000},
	{i51_WAVSAMPLE_16_KHZ, 16, 16000},
	{i51_WAVSAMPLE_24_KHZ, 24, 24000},
	{i51_WAVSAMPLE_32_KHZ, 32, 32000},
	{i51_WAVSAMPLE_48_KHZ, 48, 48000},
	{i51_WAVSAMPLE_64_KHZ, 64, 64000},
	{i51_WAVSAMPLE_96_KHZ, 96, 96000},
};

typedef struct
{
	iBOOL	bUsed;				//是否被使用
	void*	handle;				//句柄
	iU32	priority;			//优先级
	iU32	length;				//数据长度
	iU32	codec;				//数据类型
	iU32	dec;				//数据来源
	iBOOL	loop;				//是否循环播放
	iFILE	iFileHandle;		//文件句柄
	i51PLAYSTATUS status;		//播放状态
	iU32	pos;				//播放指针
	void	(*pfnStopCallback)(iHANDLE handle);	//停止播放后的回调函数
	
}i51MediaPlayerObjStruct;

typedef struct
{
	iU8* id;
	iU32 priority;
	void* handle;
	iU32 length;
	iU32 codec;
	iU32 dec;
	iBOOL loop;
	void (*STOP)(iHANDLE handle);
}i51MediaPlayerParamDataStruct;


typedef enum
{
	i51_MEDIAOPERATOR_PLAY = 0,
	i51_MEDIAOPERATOR_PAUSE,
	i51_MEDIAOPERATOR_RESUME,
	i51_MEDIAOPERATOR_STOP,
	i51_MEDIAOPERATOR_RELEASE
}i51MEDIAOPERATOR;

typedef struct
{
	iBOOL bEnable;
	void* handle;
	i51MEDIAOPERATOR eOpreator;
	i51MediaPlayerParamDataStruct stDataParam;
}i51MediaPlayerMessageStruct;

typedef struct
{
	iU8*	pStreamBuf;			//播放Buffer
	iU32	nStreamLen;			//播放长度
	iU32	nPlayLen;			//已播放长度
}i51MediaPlayerStreamStruct;

typedef struct
{
	iBOOL	bCreated;						//播放器是否被创建
	i51MEDIACALLBACK pfnCreateCb;			//创建播放器后的CallBack
	iU32	proccessTimerId;				//播放事件处理Timer
	
	iBOOL	bPlaying;						//是否正在播放
	iBOOL	bBusyPlaying;					//设备繁忙，等待播放消息
	iBOOL	bBusyStopping;					//设备繁忙，等待停止消息
	i51PLAYMODE	eMode;						//播放模式(单音频/混音)
	i51WavSampleStruct*	pSample;			//混音采样率(当前只支持相同采样率的WAV混音)
	i51MediaPlayerStreamStruct	streamSector[I51_MEDIA_PLAYER_SECTOR_NUM];	//播放器缓存
	iU32		sectorId;					//当前播放的Sector
}i51MediaPlayerMgrStruct;

static i51MediaPlayerObjStruct i51MediaPlayerAudioPlayList[I51_MEDIA_PLAYER_OBJECT_NUM];
i51MediaPlayerMessageStruct i51MediaPlayerMessageQueue[I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE];
static i51MediaPlayerMgrStruct i51MediaPlayerManager;

extern iU8 gi51IsRunning;
/*========================================================
	Media Player -- end
=========================================================*/


static void i51S_AdeMmiRecoderPlayCB (u32 UserData, u32 RspData)
{

 	//	author : osca
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	接口"i51AdeMmiMediaPlay" 的Callback
    ven_codec_ret_t retCode = RspData;
    iHANDLE stream = (iHANDLE)UserData;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51S_AdeMmiRecoderPlayCB , S , retCode = %d ,stream=0x%x", retCode,stream);
#endif
    if (retCode == VEN_CODEC_PLAY_FINISHED)
    {
        if(i51PlayRecordCb)
        {
            i51AdeOsLog(i51_LOG_SC, "MMI : i51S_AdeMmiRecoderPlayCB , Recoder-Play Callback S");
            i51PlayRecordCb(1);
            i51AdeOsLog(i51_LOG_SC, "MMI : i51S_AdeMmiRecoderPlayCB , Recoder-Play Callback E");
        }
    }
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51S_AdeMmiRecoderPlayCB , E ");
#endif		
 	
}

static void i51S_AdeMmiCreateCB ( iU32 timerid )
{

    if(i51AdeCallback)
    {
        i51AdeCallback(1);
    }
    i51AdeOsTimerStop ( timerid ) ;
    
}

iBOOL i51AdeMmiGetScreenScale ( iU16* width , iU16* height )
{

 	//	author : osca
	//	since : 2011.4.20
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 成功返回> 0 ，失败0
	
	ven_GraphicsDisplayInfo_t DispInfo;	
	iU32 ret = 0;
	
#ifdef i51ADAPTER_DEBUG
	//i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetScreenScale S , %x , %x", width, height);
#endif

	if(VEN_GRAPHICS_RET_SUCCESS != ven_graphics_getDisplayInfo( &DispInfo ))
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetScreenScale, ERR, Get Scale");
		return 0 ; 
	}

	*width = DispInfo.width ;
	*height = DispInfo.height ;

#ifdef i51ADAPTER_DEBUG
	//i51AdeOsLog(i51_LOG_SC, "ADE - MMI : i51AdeMmiGetScreenScale E");
#endif
	return 1 ; 
}

void i51AdeMmiUpdateScreen ()
{

 	//	author : osca
	//	since : 2011.4.20
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 成功返回> 0 ，失败0
    ven_GraphicsDisplayInfo_t DispInfo;
    ven_GraphicsRect_t rect={0,0,0,0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiUpdateScreen , S");
#endif
 
    if (!g_i51PkigCanvas)
    {
        return;  
    }
    
	#ifdef i51PROFILINGS
		i51KernelDrawProfiles () ;
	#endif
	
	ven_graphics_getDisplayInfo( &DispInfo );
    rect.w = DispInfo.width;
    rect.h = DispInfo.height;
    ven_graphics_setDirtyArea(g_i51PkigCanvas, &rect);
    ven_graphics_setClipArea(g_i51PkigCanvas, &rect);
    ven_graphics_screenUpdate( g_i51PkigCanvas, VEN_GRA_UPDATE_NORMAL);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiUpdateScreen , E");
#endif

}


void i51AdeMmiUpdateScreenEx ( iRECT* irect , iU32 length )
{

 	//	author : jelo
	//	since : 2012.3.31
	// 	(C) 2011 , PKIG Tech. Co., Ltd.


	//	notes : 成功返回> 0 ，失败0

	iU32 counter = 0 ;
	ven_GraphicsRect_t rect={0,0,0,0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiUpdateScreenEx , S , %x,%d" , irect, length);
#endif

	if (!g_i51PkigCanvas)
	{
		return;  
	}
	
	#ifdef i51PROFILINGS
		i51KernelDrawProfiles () ;
	#endif
	
	for ( counter = 0 ; counter < length ; counter ++ ) {

		rect.x = irect[counter].X ;
		rect.y = irect[counter].Y ;
		rect.w = irect[counter].Width ;
		rect.h = irect[counter].Height ;
		
	    ven_graphics_setDirtyArea(g_i51PkigCanvas, &rect);
	    ven_graphics_setClipArea(g_i51PkigCanvas, &rect);
	    ven_graphics_screenUpdate( g_i51PkigCanvas, VEN_GRA_UPDATE_NORMAL);
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiUpdateScreenEx , E");
#endif	

}

iU32* i51AdeMmiGetPanel ()
{

 	//	author : osca
	//	since : 2011.4.20
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 返回层数组，其中存放了层的buffer 地址、宽、高
	//	array[0] 存放了层个数，数组长度= 层的个数* 3 + 1
	//	array[1...n] 存放了层的buffer以及宽高
	//	如：array[1] 第0层的屏幕指针、array[2] 第0层的宽、array[3]第0层的高
	//	array[4] 为第1层地屏幕指针、array[5]、array[6] 同上。第0层为物理显存、其余为虚拟层。
	//	MTK 的实现可以直接用系统的层资源。
	
	static iU32 array [4] = { 0 } ;	
	ven_GraphicDIB_t *CanvasDIB = NULL;
	
#ifdef i51ADAPTER_DEBUG
//	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetPanel , S");
#endif

    if (!g_i51PkigCanvas)
    {
		i51AdeOsLog(i51_LOG_SC, "ADE - MMI : Get Panel BE");
        return NULL;
    }

	CanvasDIB = ven_graphics_getCanvasDIB(g_i51PkigCanvas);
    if (CanvasDIB)
    {
        array[0] = 1 ; 
        array[1] = (iU32 )CanvasDIB->pData;
        array[2] = CanvasDIB->width;
        array[3] = CanvasDIB->height;
    }
	
#ifdef i51ADAPTER_DEBUG
//	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetPanel , E , %x", array);
#endif

	return array ;

}

void i51AdeMmiSetPanelUsed ( void* panel ) 
{


 	//	author : osca
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	return ;

}

void i51AdeMmiRecorderGetDataCb(void* pData, u32 nDataLen)
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderGetDataCb , S");
#endif	

	if(i51Recoder.bRecord == iFALSE)
	{
		return;
	}

	if(i51Recoder.appcb && gi51IsRunning)
	{
		i51Recoder.appcb(pData , nDataLen);
	}
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderGetDataCb , E");
#endif	
}

void i51AdeMmiRecorderStopCb(iBOOL bResult)
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStopCb , S");
#endif

	if(i51Recoder.stopCb && gi51IsRunning)
	{
		i51Recoder.stopCb(bResult);
	}

	memset(&i51Recoder, 0, sizeof(i51Recoder));
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStopCb , E");
#endif	
}

void i51AdeMmiRecorderStatusCb(iBOOL bResult)
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStatusCb , S");
#endif

	if(i51Recoder.bRecord == iFALSE)
	{
		return;
	}

	if(i51Recoder.finishCb && gi51IsRunning)
	{
		i51Recoder.finishCb(bResult);
	}

	memset(&i51Recoder, 0, sizeof(i51Recoder));

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStatusCb ,  E");
#endif
	
}



//非扩展接口，默认录制PCM
iS32 i51AdeMmiRecorderStart ( void* buffer , iU32 bufflen , void (*callback ) ( void* stream , iU32 length ) ) 
{  

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStart , S , %x , %d , %x ", buffer, bufflen, callback);
#endif

    if(_i51AdeMmiMediaPlayerIsWorking())
    {
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStart , F, MediaPlayer working..");
		return iFALSE;
	}

	if(i51Recoder.bRecord == iTRUE)
	{
		i51AdeOsLog(i51_LOG_SC, "ADE - MMI : i51AdeMmiRecorderStart , F, Recorder working..");
		return iFALSE;
	}
	
   	memset(&i51Recoder,0,sizeof(i51RecoderStruct));
	i51Recoder.bRecord = iTRUE;
	i51Recoder.mediaType = i51_REC_PCM;
	i51Recoder.appcb = callback;
	i51Recoder.obj = buffer;
    i51Recoder.len = bufflen;
	
	g_i51RecorderActionFlag = I51_RECORDER_ACTION_START_PCM;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStart , E");
#endif
	
    return iTRUE;
	
}


iBOOL i51AdeMmiRecorderStartEx ( iU16* filename , void (*callback ) ( iBOOL finished ) ) 
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStartEx , S");
#endif

	if(_i51AdeMmiMediaPlayerIsWorking())
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStartEx , MediaPlayer working..");
		return iFALSE;
	}
	
	if(i51Recoder.bRecord == iTRUE)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStartEx , Recorder is working");
		return iFALSE;
	}
		
	memset(&i51Recoder,0,sizeof(i51RecoderStruct));
	i51Recoder.bRecord = iTRUE;
	i51Recoder.mediaType = i51_REC_AMR;
	i51Recoder.finishCb = callback;
	i51Recoder.obj = (void*)filename;
    i51Recoder.len = 0;
	ven_std_wstrcpy ( (iU16*)&i51Recoder.fileName[0], (iU16*)L"fs:") ;	
	ven_std_wstrcat ((iU16*)&i51Recoder.fileName[0],i51S_AdeFileFixPath(filename) );

	g_i51RecorderActionFlag = I51_RECORDER_ACTION_START_AMR;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStartEx , E");
#endif

	return iTRUE;
	
}


iBOOL i51AdeMmiRecorderStop ( void (*callback) ( iBOOL results )) 
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , S");
#endif

	if(i51Recoder.bRecord == iFALSE)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , ERR , already stop");
		return iFALSE;
	}

	i51Recoder.stopCb = callback;

	i51Recoder.bRecord = iFALSE;

	if(i51Recoder.mediaType == i51_REC_PCM)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , PCM");
		g_i51RecorderActionFlag = I51_RECORDER_ACTION_STOP_PCM;
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , AMR");
		g_i51RecorderActionFlag = I51_RECORDER_ACTION_STOP_AMR;
		
	}

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , E");
#endif

	return iTRUE;
}

iBOOL i51AdeMmiRecoderPlay ( void* stream , iU32 length , void (*callback) ( iBOOL results ) ) 
{
 	iU32 ret = 0 ,codec= 0xff;
	ven_req_data_t ptVenReq;
	ven_codec_mediaFormat_t eFormat;

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecoderPlay , S , %x,%d,%x", stream, length, callback);
#endif

    if(_i51AdeMmiMediaPlayerIsWorking())
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecoderPlay , ERR, MediaPlayer working..");
		return iFALSE;
	}

    i51PlayRecordCb = callback;

    ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iFALSE;
	ptVenReq.pfnCb = i51S_AdeMmiRecoderPlayCB;
	ptVenReq.userData = (iU32)stream;

	ret = ven_codec_playAudioRaw(g_i51MediaHandle, VEN_MEDIA_FORMAT_WAVE, (iU8*)stream, length, g_i51LastVolume, 0, &ptVenReq);

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecoderPlay , S , %d" , ret);
#endif

    return (ret==VEN_CODEC_SUCCESS); 
    
}

iBOOL i51AdeMmiRecorderPlayStop (void)
{
    ven_req_data_t ReqData = {0};
    iU32 ret=0;

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderPlayStop , S");
#endif
	
    ReqData.Handle = g_i51PkigHandle;
	ReqData.bSync = iTRUE;

    if ( 0 == i51PlayRecordCb ) {
        i51AdeOsLog ( "MMI : i51AdeMmiRecorderPlayStop , ERR , Recoder-Play No Need Stop" ) ;
        return iTRUE ;
    }

	ret = ven_codec_stopAudio(g_i51MediaHandle, &ReqData);

	i51PlayRecordCb = 0 ;

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderPlayStop , E , %d" , ret);
#endif
	
    return (ret == 0); 

}

void i51AdeMmiRecorderFormat ( iU32* samples , iU32* bitscale , iU32* chanels )
{

    *samples = 8*1024;
    *bitscale = 16;
    *chanels = 1;

}

void i51AdeMmiGetTextAttr ( iU16* text , iU16* textwidth , iU16* textheight ) 
{
    //	author : osca
    //	since : 20111228
    //	PKIG Tech. Co., Ltd.
    //	文字
	iU32 width=0,height=0;
    iU16 pAscent,pDescent,pLeading;
    
#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetTextAttr , S , %x , %x, %x", text,textwidth,textheight);
#endif

    if (!g_i51PkigFontNormal)
    {
        i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetTextAttr , Get Text-Attr F");
        return;  
    }
    ven_font_getTextWidth(g_i51PkigFontNormal, text, &width);
    ven_font_getFontInfo(g_i51PkigFontNormal, &height, &pAscent, &pDescent, &pLeading);

	*textwidth = width;
	*textheight = height;
	
#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetTextAttr ,  E");
#endif

}

void i51AdeMmiDrawText ( iU8* panel , iU16 width , iU16 height , const iU16* text , iS32 desx , iS32 desy , iCOLOR color ) 
{

    //	author : jone
    //	since : 20111010
    //	PKIG Tech. Co., Ltd.
    //	文字

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiDrawText , S , %x,%d,%d,%x,%d,%d,%d", panel,width,height, text, desx, desy, color);
#endif

    if (!g_i51PkigCanvas ||!text||!panel)
    {
        i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiDrawText , Draw Text F");
        return;  
    }
    ven_graphics_setColor( g_i51PkigCanvas, VEN_CLR_TEXT,  VEN_MAKE_RGB888( R_OF_565(color),G_OF_565(color),B_OF_565(color) )   );  
    ven_graphics_drawText(g_i51PkigCanvas, g_i51PkigFontNormal, text , desx, desy);
#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiDrawText , E");
#endif
}

iS32 i51AdeMmiMediaPlayerFormat (void)
{
	//	author : jone
	//	since : 2012428
	//	PKIG Tech. Co., Ltd.

	iS32 format;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI :  i51AdeMmiMediaPlayerFormat ,  S ");
#endif

	format = i51_MEDIA_TYPE_MIDI|i51_MEDIA_TYPE_WAV|i51_MEDIA_TYPE_MP3|i51_MEDIA_TYPE_AMR;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerFormat , E , %d", format);
#endif

    return format;
}

iBOOL i51AdeMmiCreate ( i51ADECALLBACK callback ) 
{

 	//	author : osca
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	ven_GraphicsDisplayInfo_t DispInfo;
	ven_req_data_t ReqData = {0};
	iU32 timerid;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiCreate , S , %x", callback);
#endif

	i51AdeCallback = callback ;
	timerid = i51AdeOsTimerCreate ( 0 , 0) ;
	i51AdeOsTimerStart ( timerid, i51S_AdeMmiCreateCB, 20);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiCreate ,  E");
#endif
	return ( 1 ) ;		
}

iBOOL i51AdeMmiRelease ( void ) 
{

 	//	author : osca
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	i51AdeCallback = 0;
		
	i51MediaPlayerManager.bCreated = iFALSE;
	return (1) ;	
		
}


/*========================================================
	Media Player -- start
=========================================================*/
static iBOOL _i51AdeMmiMediaPlayerIsWorking()
{
	if(!i51MediaPlayerManager.bCreated)
	{
		return iFALSE;
	}
	if(i51MediaPlayerManager.bPlaying || i51MediaPlayerManager.bBusyPlaying || i51MediaPlayerManager.bBusyStopping)
	{
		return iTRUE;
	}
	return iFALSE;
}


//播放器构造
static iBOOL _i51AdeMmiMediaPlayerManagerDataInit(void (*pfnCreateCb) ( iBOOL results ))
{
	iU8 i;
	
	memset(&i51MediaPlayerAudioPlayList[0], 0, sizeof(i51MediaPlayerAudioPlayList));
	memset(&i51MediaPlayerMessageQueue[0], 0, sizeof(i51MediaPlayerMessageQueue));
	memset(&i51MediaPlayerManager, 0, sizeof(i51MediaPlayerMgrStruct));

	
	i51MediaPlayerManager.bCreated = iTRUE;
	i51MediaPlayerManager.pfnCreateCb = pfnCreateCb;
	i51MediaPlayerManager.eMode = i51_PLAYMODE_INVALIDATE;

	for( i = 0; i <  I51_MEDIA_PLAYER_SECTOR_NUM; i++ )
	{
		i51MediaPlayerManager.streamSector[i].nPlayLen = 0;
		i51MediaPlayerManager.streamSector[i].nStreamLen = 0;
#if 0		
		i51MediaPlayerManager.streamSector[i].pStreamBuf = (iU8*)i51AdeOsMalloc(I51_MEDIA_PLAYER_SECTOR_SIZE, __FILE__, __LINE__);
		if(i51MediaPlayerManager.streamSector[i].pStreamBuf == NULL)
		{
			return iFALSE;
		}
		memset(i51MediaPlayerManager.streamSector[i].pStreamBuf, 0, I51_MEDIA_PLAYER_SECTOR_SIZE);
#endif		
	}

	i51MediaPlayerManager.sectorId = 0;

	return iTRUE;
}

//播放器析构
static void _i51AdeMmiMediaPlayerManagerDataRelease()
{
	iU8 i;

#if 0			
	for( i = 0; i <  I51_MEDIA_PLAYER_SECTOR_NUM; i++ )
	{
		if(i51MediaPlayerManager.streamSector[i].pStreamBuf)
		{
			i51AdeOsFree(i51MediaPlayerManager.streamSector[i].pStreamBuf);
		}
	}
#endif
	memset(&i51MediaPlayerAudioPlayList[0], 0, sizeof(i51MediaPlayerAudioPlayList));
	memset(&i51MediaPlayerMessageQueue[0], 0, sizeof(i51MediaPlayerMessageQueue));
	memset(&i51MediaPlayerManager, 0, sizeof(i51MediaPlayerMgrStruct));

	i51MediaPlayerManager.bCreated = iFALSE;
}

//通过播放数据handle获取对象Id
static iU8 _i51AdeMmiMediaPlayerManagerGetRestNum()
{
	iU8 i, num = 0;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(!i51MediaPlayerAudioPlayList[i].bUsed)
		{
			num ++;
		}
	}

	return num;
}


//通过播放数据handle获取对象Id
static iU8 _i51AdeMmiMediaPlayerManagerGetIdByHandle(void* handle)
{
	iU8 i;
	iU8 nId = I51_MEIDA_PLAYER_INVALIDATE_ID;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].handle == handle)
		{
			nId = i;
			break;
		}
	}

	return nId;
}

//创建新的播放对象添加到播放队列
static iBOOL _i51AdeMmiMediaPlayerManagerAddNewObject(iU32 priority , void* handle , iU32 length , 
		iU32 codec , iU32 dec  , iBOOL loop , void (*STOP)(iHANDLE handle)  )

{
	iU8 i;
	iBOOL bAddSuccess = iFALSE;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(!i51MediaPlayerAudioPlayList[i].bUsed)
		{
			i51MediaPlayerAudioPlayList[i].handle = handle;
			i51MediaPlayerAudioPlayList[i].priority = priority;
			i51MediaPlayerAudioPlayList[i].length = length;
			i51MediaPlayerAudioPlayList[i].codec = codec;
			i51MediaPlayerAudioPlayList[i].dec = dec;
			i51MediaPlayerAudioPlayList[i].loop = loop;
			i51MediaPlayerAudioPlayList[i].pfnStopCallback = STOP;
			i51MediaPlayerAudioPlayList[i].bUsed = iTRUE;
			i51MediaPlayerAudioPlayList[i].status = i51_PLAYSTATUS_PLAYING;
			i51MediaPlayerAudioPlayList[i].pos = 0;
			bAddSuccess = TRUE;
			break;
		}
	}

	return bAddSuccess;
}

//从播放队列中移除某播放对象(句柄)
static void _i51AdeMmiMediaPlayerManagerRemoveObjectByHandle(void* handle)
{
	iU8 i;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].handle == handle)
		{
			memset(&i51MediaPlayerAudioPlayList[i], 0, sizeof(i51MediaPlayerObjStruct));
			break;
		}
	}
}

//从播放队列中移除某播放对象(对象Id)
static void _i51AdeMmiMediaPlayerManagerRemoveObjectById(iU8 id)
{
	
	if(id < I51_MEDIA_PLAYER_OBJECT_NUM)
	{
		memset(&i51MediaPlayerAudioPlayList[id], 0, sizeof(i51MediaPlayerObjStruct));
	}
}

//移除正在播放的对象
static void _i51AdeMmiMediaPlayerManagerRemovePlayingObject()
{
	iU8 i;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].status == i51_PLAYSTATUS_PLAYING)
		{
			memset(&i51MediaPlayerAudioPlayList[i], 0, sizeof(i51MediaPlayerObjStruct));
		}
	}
}


//移除全部播放对象
static void _i51AdeMmiMediaPlayerManagerRemoveAllObject()
{
	iU8 i;
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		memset(&i51MediaPlayerAudioPlayList[i], 0, sizeof(i51MediaPlayerObjStruct));
	}
}

//调用所有正在播放对象的STOP回调函数
static void _i51AdeMmiMediaPlayerManagerCallAllStopCb()
{
	iU8 i;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].status == i51_PLAYSTATUS_PLAYING)
		{
			if(i51MediaPlayerAudioPlayList[i].pfnStopCallback)
				i51MediaPlayerAudioPlayList[i].pfnStopCallback(i51MediaPlayerAudioPlayList[i].handle);
		}
	}
}


//从播放队列中获取播放对象(句柄)
static i51MediaPlayerObjStruct* _i51AdeMediaPlayerManagerGetObjectByHandle(void* handle)
{
	iU8 i;
	i51MediaPlayerObjStruct* pObj = NULL;

		
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].handle == handle)
		{
			pObj = &i51MediaPlayerAudioPlayList[i];
			break;
		}
	}

	return pObj;
}

//从播放队列中获取播放对象(对象ID)
static i51MediaPlayerObjStruct* _i51AdeMediaPlayerManagerGetObjectById(iU8 id)
{
	i51MediaPlayerObjStruct* pObj = NULL;

	
	if(id < I51_MEDIA_PLAYER_OBJECT_NUM)
	{
		pObj = &i51MediaPlayerAudioPlayList[id];
	}

	return pObj;
}

static iU8 _i51AdeMediaPlayerManagerGetObjectNumber()
{
	iU8 i, num = 0;

	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed)
		{
			num++;
		}
	}

	return num;

}

//判断某音源是否已在播放状态
static iBOOL _i51AdeMediaPlayerManagerAudioIsPlaying(void* handle)
{
	i51MediaPlayerObjStruct* pObj = NULL;


	if(i51MediaPlayerManager.bPlaying)
	{
		pObj = _i51AdeMediaPlayerManagerGetObjectByHandle(handle);
		if(pObj != NULL)
		{
			if(pObj->status == i51_PLAYSTATUS_PLAYING || pObj->status == i51_PLAYSTATUS_PAUSE)
			{
				return iTRUE;
			}
		}
	}

	return iFALSE;
}

//发送消息
static iBOOL _i51AdeMediaPlayerManagerSendMessage(void* handle, i51MEDIAOPERATOR eOpreator)
{
	iU8 i;
	iBOOL ret = iFALSE;
	
	for(i = 0; i < I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE; i++)
	{
		if(i51MediaPlayerMessageQueue[i].bEnable == iFALSE)
		{
			i51MediaPlayerMessageQueue[i].handle = handle;
			i51MediaPlayerMessageQueue[i].eOpreator = eOpreator;
			i51MediaPlayerMessageQueue[i].bEnable = iTRUE;
			ret = iTRUE;
			break;
		}
	}
	return ret;
}

//发送消息
static iBOOL _i51AdeMediaPlayerManagerSendMessageWithParam(void* handle, i51MEDIAOPERATOR eOpreator, i51MediaPlayerParamDataStruct* pDataParam)
{
	iU8 i;
	iBOOL ret = iFALSE;
	
	for(i = 0; i < I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE; i++)
	{
		if(i51MediaPlayerMessageQueue[i].bEnable == iFALSE)
		{
			i51MediaPlayerMessageQueue[i].handle = handle;
			i51MediaPlayerMessageQueue[i].eOpreator = eOpreator;
			i51MediaPlayerMessageQueue[i].bEnable = iTRUE;
			memcpy(&i51MediaPlayerMessageQueue[i].stDataParam, pDataParam, sizeof(i51MediaPlayerParamDataStruct));
			ret = iTRUE;
			break;
		}
	}
	return ret;
}


//获取消息
static i51MediaPlayerMessageStruct* _i51AdeMediaPlayerManagerGetMessage()
{
	if(i51MediaPlayerMessageQueue[0].bEnable)
	{
		return &i51MediaPlayerMessageQueue[0];
	}
	else
	{
		return NULL;	
	}
}

//移除消息
static iBOOL _i51AdeMediaPlayerManagerRemoveMessage()
{
	iU8 i;

	if(!i51MediaPlayerMessageQueue[0].bEnable)
	{
		return iFALSE;
	}
	
	for( i = 1; i < I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE; i ++)
	{
		if(i51MediaPlayerMessageQueue[i].bEnable)
			ven_std_memcpy(&i51MediaPlayerMessageQueue[i - 1], &i51MediaPlayerMessageQueue[i], sizeof(i51MediaPlayerMessageStruct));
		else
			break;
	}

	i51MediaPlayerMessageQueue[i - 1].bEnable = FALSE;

	return iTRUE;
}


//从文件中读取一段数据
static iBOOL _i51AdeMediaPlayerGetFileData(const iU16* path, void* data, iU32 pos, iU32 len )
{
	iFILE hFile = 0 ;
	iU32 readByte = 0;
	iBOOL ret = FALSE;
	
	hFile = i51AdeOsFileOpen(path, i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__, __LINE__);
	if(hFile)
	{
		do
		{
			ret = i51AdeOsFileSetSeek(hFile, pos, VEN_FILE_SEEK_SET);
			if(!ret) 
				break;

			ret = i51AdeOsFileRead(hFile, data, len, &readByte);
			if(!ret)
				break;

			if(readByte != len)
			{
				ret = iFALSE;
				break;
			}
		}while(0);

		i51AdeOsFileClose(hFile);
	}

	return ret;
}


//判断音频数据是否支持混音--目前只支持相同采样率的WAV格式文件进行混音，且采样率必须是8的整倍数(只取单字节，简化算法)
static i51WavSampleStruct* _i51AdeMediaPlayerMixFormatSupport(void* handle , iU32 length , iU32 codec, iU32 dec)
{
	iS8 data[I51_WAV_HEAD_LEN];
	wavheader_t* pWavHead = NULL;
	iU8 i;
	i51WavSampleStruct* pSample = NULL;
	
	if(codec == VEN_MEDIA_FORMAT_WAVE)
	{
		if(dec == i51_MMI_DEC_FILE_BASED)
		{
			ven_std_memset(&data[0], 0, I51_WAV_HEAD_LEN);
			if(_i51AdeMediaPlayerGetFileData((const iU16*)handle, &data[0], 0, I51_WAV_HEAD_LEN))
			{
				pWavHead = (wavheader_t*)&data[0];
			}
		}
		else
		{
			pWavHead = (wavheader_t*)handle;
		}

		if(pWavHead)
		{
			for(i =0 ; i < sizeof(stWavSample) / sizeof(i51WavSampleStruct); i++)
			{
				if(pWavHead->sample == stWavSample[i].sampleValue)
				{
					pSample = &stWavSample[i];
					break;
				}
			}
		}
	}
	
	return pSample;
}

//判断当前是否可以播放混音音频(如果当前没有其他音频在播放，则支持; 如果当前在播放的模式是混音播放且采样率相同，则支持)
static iBOOL _i51AdeMediaPlayerMixCanPlay(i51WavSampleStruct* pWavSample)
{
	if(!i51MediaPlayerManager.bPlaying)
	{
		return iTRUE;
	}

	if(i51MediaPlayerManager.eMode == i51_PLAYMODE_MIX && i51MediaPlayerManager.pSample->eSampleType == pWavSample->eSampleType)
	{
		return iTRUE;
	}
	else
	{
		return iFALSE;
	}
}

static void _i51AdeMediaPlayerLoadMixData()
{

}

static iBOOL _i51AdeMediaPlayerJudgePriority(iU32 priority)
{
	iU8 i;
	iBOOL bHigh = iTRUE;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerJudgePriority new priority = %d,  bUsed = %d, priority = %d, i = %d", 
			priority, i51MediaPlayerAudioPlayList[i].bUsed, i51MediaPlayerAudioPlayList[i].priority, i);
		
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].priority < priority
			&& i51MediaPlayerAudioPlayList[i].status != i51_PLAYSTATUS_STOP)
		{
			bHigh = FALSE;
			break;
		}
	}
	return bHigh;
}

static void _i51AdeMediaPlayerResetManager()
{
	iU8 i;
	
	i51MediaPlayerManager.bPlaying = iFALSE;
	i51MediaPlayerManager.bBusyPlaying = iFALSE;
	i51MediaPlayerManager.bBusyStopping = iFALSE;
	i51MediaPlayerManager.eMode = i51_PLAYMODE_INVALIDATE;
	i51MediaPlayerManager.pSample = NULL;
	i51MediaPlayerManager.sectorId = 0;
	for( i = 0; i <  I51_MEDIA_PLAYER_SECTOR_NUM; i++ )
	{
		i51MediaPlayerManager.streamSector[i].nPlayLen = 0;
		i51MediaPlayerManager.streamSector[i].nStreamLen = 0;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
//Device Control
////////////////////////////////////////////////////////////////////////////////////////////

static void _i51AdeMediaPlayerDeviceStartSinglePlay_Cb(u32 UserData, u32 RspData)
{
	ven_codec_ret_t retCode = RspData;
	void* handle = (void*)UserData;
	i51MediaPlayerObjStruct* pAudioObj = _i51AdeMediaPlayerManagerGetObjectByHandle(handle);

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay_Cb. retCode = %d, handle = 0x%x", retCode, handle);

	if(!i51MediaPlayerManager.bCreated && retCode != VEN_CODEC_PLAY_FINISHED)
	{
		//设备繁忙时被释放，此处停止播放
		_i51AdeMediaPlayerDeviceStopPlayImmediately();
		return;
	}

	if(!pAudioObj)		//Debug: 兼容MStar平台异常情况，当Stop已经成功调用后，Play Callback仍会通知Finish消息
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay_Cb. NULL Object, return");
		return;
	}

	if ( retCode == VEN_CODEC_PLAY_FINISHED )
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay_Cb. Play finish");
		if(pAudioObj->pfnStopCallback)
				pAudioObj->pfnStopCallback(handle);

		_i51AdeMmiMediaPlayerManagerRemoveAllObject();

		_i51AdeMediaPlayerResetManager();
	}
	else if( retCode == VEN_CODEC_SUCCESS )
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay_Cb. Codec Success");
	}

	i51MediaPlayerManager.bBusyPlaying = iFALSE;

}

static void _i51AdeMediaPlayerDeviceStartSinglePlay(i51MediaPlayerParamDataStruct* pDataParam)
{
    iU32 ret = 0;
	ven_req_data_t ptVenReq;
	iU16 filename[128];

	if(pDataParam->dec== i51_MMI_DEC_FILE_BASED) 
	{
        ptVenReq.Handle = g_i51PkigHandle;
        ptVenReq.bSync = iFALSE ;
        ptVenReq.pfnCb = _i51AdeMediaPlayerDeviceStartSinglePlay_Cb;
        ptVenReq.userData = (iU32)pDataParam->handle ;
            	                     
        ven_std_wstrcpy ( filename , i51S_AdeFileFixPath((iU16*)pDataParam->handle) ) ;
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay. Name Len = %d, loop = %d", ven_std_wstrlen(filename), pDataParam->loop);
    
        ret = ven_codec_playAudio (g_i51MediaHandle , 
                filename, 
                ven_std_wstrlen(filename) , 
                0 , 
                g_i51LastVolume,
                pDataParam->loop, 
                &ptVenReq ) ;

		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay. Play File   ret = %d", ret);

		if(ret == VEN_CODEC_SUCCESS)
		{
			i51MediaPlayerManager.bPlaying = iTRUE;
			i51MediaPlayerManager.bBusyPlaying = iTRUE;
		}

	}
	else if(pDataParam->dec == i51_MMI_DEC_MEM_BASED) 
	{
        	
		ptVenReq.Handle = g_i51PkigHandle;
		ptVenReq.bSync = iFALSE ;
		ptVenReq.pfnCb = _i51AdeMediaPlayerDeviceStartSinglePlay_Cb;
		ptVenReq.userData = (iU32)pDataParam->handle;

		ret = ven_codec_playAudioRaw (
		    g_i51MediaHandle , 
		    pDataParam->codec , 
		    (iU8*)pDataParam->handle , 
		    pDataParam->length , 
		    g_i51LastVolume , 
		    pDataParam->loop , 
		    &ptVenReq ) ; 

		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay    Play Memory   ret = %d", ret);
		
		if(ret == VEN_CODEC_SUCCESS)
		{
			i51MediaPlayerManager.bPlaying = iTRUE;
			i51MediaPlayerManager.bBusyPlaying = iTRUE;
		}
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay   Error dec");

		i51MediaPlayerManager.bBusyPlaying = iFALSE;
	}

}


static void _i51AdeMediaPlayerDeviceStartMixPlay(i51MediaPlayerParamDataStruct* pDataParam)
{
	//ven_codec_playAudioStreaming
}


static void _i51AdeMmiMediaPlayerDeviceStopCallback ( iU32 UserData , iU32 RspData )
{
	ven_codec_ret_t retCode = RspData;
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerDeviceStopCallback. ret = %d", retCode);	

	i51MediaPlayerManager.bBusyStopping = iFALSE;
	
	if (VEN_CODEC_SUCCESS == retCode)
    {
        
    }
    else if (VEN_CODEC_NOT_PLAYING == retCode || VEN_CODEC_PLAY_FINISHED == retCode)
    {
        
    }
    else
    {
        _i51AdeMediaPlayerDeviceStopPlayImmediately();		//异步STOP失败直接同步STOP
    }
}

static iBOOL _i51AdeMediaPlayerDeviceStopPlay()
{
	ven_req_data_t ptVenReq = {0};
	iU32 ret;

	if(!g_i51PkigHandle)
	{
		return iFALSE;
	}
		
	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iFALSE ;
	ptVenReq.userData = 0 ;
	ptVenReq.pfnCb = _i51AdeMmiMediaPlayerDeviceStopCallback;
	ret = ven_codec_stopAudio(g_i51MediaHandle, &ptVenReq);
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStopPlay    ret = %d", ret);	

	if(ret == VEN_CODEC_SUCCESS)
	{
		i51MediaPlayerManager.bBusyStopping = iTRUE;
		i51MediaPlayerManager.bPlaying = iFALSE;
		return iTRUE;
	}
	else if(ret == VEN_CODEC_PLAY_FINISHED || ret == VEN_CODEC_NOT_PLAYING)	//lynn edit, 20120716, 播放异常处理
	{
		i51MediaPlayerManager.bBusyStopping = iFALSE;
		i51MediaPlayerManager.bPlaying = iFALSE;
		return iTRUE;
	}

	return iFALSE;
}

static iBOOL _i51AdeMediaPlayerDeviceStopPlayImmediately()
{
	ven_req_data_t ptVenReq = {0};
	iU32 ret;

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStopPlayImmediately E");	

	if(!g_i51PkigHandle || !g_i51MediaHandle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStopPlayImmediately Error 11111111111");	
		return iFALSE;
	}
		
	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE ;
	ptVenReq.userData = 0 ;
	ptVenReq.pfnCb = 0;
	ret = ven_codec_stopAudio(g_i51MediaHandle, &ptVenReq);

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStopPlayImmediately    ret = %d", ret);	

	return ret == VEN_CODEC_SUCCESS ? iTRUE : iFALSE ;
}

////////////////////////////////////////////////////////////////////////////////////////////
//Message Process
////////////////////////////////////////////////////////////////////////////////////////////
static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Play(i51MediaPlayerMessageStruct* pMsg)
{
	i51WavSampleStruct*	pSample = NULL;
	i51MediaPlayerParamDataStruct* pDataParam = &pMsg->stDataParam;
	iBOOL bJoinMix = iFALSE;
	iBOOL bHandled = iTRUE;

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play handle = 0x%x", pDataParam->handle);
	
#if 0
	//是否支持混音
	if(pDataParam->codec == VEN_MEDIA_FORMAT_WAVE)			
	{
		pSample = _i51AdeMediaPlayerMixFormatSupport(pDataParam->handle, pDataParam->length, pDataParam->codec, pDataParam->dec);
		if(pSample)
		{
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play --- Mix Supprot, sample = %d", pSample->sampleValue);
			if(i51MediaPlayerManager.bPlaying == iFALSE)
			{
				bJoinMix = iTRUE;
			}
			else if(i51MediaPlayerManager.bPlaying == iTRUE && i51MediaPlayerManager.eMode == i51_PLAYMODE_MIX
				&& i51MediaPlayerManager.pSample == pSample)
			{
				bJoinMix = iTRUE;
			}
		}
	}
#endif
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. bJoinMix = %d", bJoinMix);

	if(!i51MediaPlayerManager.bPlaying)
	{//停止状态
		
		if(bJoinMix)
		{//混音模式
			i51MediaPlayerManager.eMode = i51_PLAYMODE_MIX;
			i51MediaPlayerManager.pSample = pSample;
			i51MediaPlayerManager.bPlaying = iTRUE;
			i51MediaPlayerManager.bBusyPlaying = iTRUE;
			
			_i51AdeMediaPlayerLoadMixData();
			_i51AdeMediaPlayerDeviceStartMixPlay(pDataParam);
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Start Mix Play");
		}
		else
		{//单音模式
			i51MediaPlayerManager.eMode = i51_PLAYMODE_SINGLE;
			i51MediaPlayerManager.pSample = 0;
		
			_i51AdeMmiMediaPlayerManagerAddNewObject(
					pDataParam->priority, 
					pDataParam->handle, 
					pDataParam->length,
					pDataParam->codec,
					pDataParam->dec,
					pDataParam->loop,
					pDataParam->STOP);
			_i51AdeMediaPlayerDeviceStartSinglePlay(pDataParam);
			
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Start Single Play");
		}
		
	}
	else													
	{//已在播放
		if(bJoinMix)		//可加入混音，播放器缓存装载数据,继续播放
		{
			_i51AdeMediaPlayerLoadMixData();

			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Continue Mix Play");
		}
		else				//不可加入混音，打断之前播放，重新播放新音源
		{
			if(_i51AdeMediaPlayerJudgePriority(pDataParam->priority))
			{
				//优先级较高或相同，先停止之前的播放，再继续播放
				i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Single Play New Audio");
				
				_i51AdeMediaPlayerDeviceStopPlay();

				_i51AdeMmiMediaPlayerManagerCallAllStopCb();

				_i51AdeMmiMediaPlayerManagerRemovePlayingObject();

				i51MediaPlayerManager.pSample = 0;
				i51MediaPlayerManager.eMode = i51_PLAYMODE_SINGLE;

				bHandled = iFALSE;
			}
			else
			{//优先级较低，不处理播放请求，直接通知用户播放完毕

				i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Single Play Ignore");
				
				if(pDataParam->STOP)
					pDataParam->STOP(pDataParam->handle);
			}
		}
	}
	
	return bHandled;
}

static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Pause(i51MediaPlayerMessageStruct* pMsg)
{
	ven_req_data_t ptVenReq;
	iU32 ret;
	
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Pause. handle = 0x%x", pMsg->handle);

	if( iTRUE != i51MediaPlayerManager.bPlaying )
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Pause. No audio playing");
       	 	return iTRUE ;
	}

	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE;
	ret = ven_codec_pauseAudio(g_i51MediaHandle, &ptVenReq);

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Pause ret = %d", ret);

	return iTRUE;
}

static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Resume(i51MediaPlayerMessageStruct* pMsg)
{
	ven_req_data_t ptVenReq;
	iU32 ret;

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Resume. handle = 0x%x", pMsg->handle);

	if( iTRUE != i51MediaPlayerManager.bPlaying )
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Resume. No audio playing ");
		 return iTRUE ;
	}

	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE;
	ret = ven_codec_resumeAudio(g_i51MediaHandle, &ptVenReq);

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Resume ret = %d", ret);

	return iTRUE;
}

static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Stop(i51MediaPlayerMessageStruct* pMsg)
{
	i51MediaPlayerObjStruct* pAudioObj = NULL;
	iBOOL bHandled = iTRUE;

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. handle = 0x%x", pMsg->handle);

	pAudioObj = _i51AdeMediaPlayerManagerGetObjectByHandle(pMsg->handle);
	if(!pAudioObj)
	{
		//已停止，不在播放队列中
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Object is null");
		return bHandled;
	}

	if(i51MediaPlayerManager.bPlaying == iFALSE)
	{
		//未播放
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Already stopped");
		_i51AdeMmiMediaPlayerManagerRemovePlayingObject();
		return bHandled;
	}

	if(i51MediaPlayerManager.eMode == i51_PLAYMODE_MIX)	
	{
		//混音播放模式，只停止需要停止音频数据的Merage
		if(_i51AdeMmiMediaPlayerManagerGetIdByHandle(pAudioObj->handle) != I51_MEIDA_PLAYER_INVALIDATE_ID)
		{
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Mix...Remove Object");

			if(pAudioObj->pfnStopCallback != 0)
				pAudioObj->pfnStopCallback(pAudioObj->handle);
			
			_i51AdeMmiMediaPlayerManagerRemoveObjectByHandle(pAudioObj->handle);
		}

		//只有当需要混音的对象个数为0时才停止设备
		if(_i51AdeMediaPlayerManagerGetObjectNumber() == 0)
		{
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Mix...Stop Device");

			_i51AdeMediaPlayerDeviceStopPlay();

			_i51AdeMmiMediaPlayerManagerRemovePlayingObject();
		}
		else
		{
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop.  Mix...Stop Merage Only");
		}
	}
	else if(i51MediaPlayerManager.eMode == i51_PLAYMODE_SINGLE)	
	{
		//非混音模式，需要停止设备
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Single..Stop Device");
		
		_i51AdeMediaPlayerDeviceStopPlay();

		_i51AdeMmiMediaPlayerManagerRemovePlayingObject();

		i51MediaPlayerManager.bPlaying = iFALSE;

	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Error play mode");
	}

	return bHandled;
}

static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Release(i51MediaPlayerMessageStruct* pMsg)
{
	i51AdeOsTimerStop(i51MediaPlayerManager.proccessTimerId);

	if(i51MediaPlayerManager.bPlaying)
	{
		_i51AdeMediaPlayerDeviceStopPlayImmediately();
	}	
	
	_i51AdeMmiMediaPlayerManagerDataRelease();		//Lynn add 20120606	
}

//播放器消息处理
static void _i51AdeMmiMediaPlayerMessageProcessing( iU32 timerid )		
{
	i51MediaPlayerMessageStruct *pMsg = NULL;
	iBOOL bHandled = iFALSE;

	if(!g_i51MediaHandle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Null meida handle");
		return;
	}

	if(!i51MediaPlayerManager.bCreated)
	{
		i51AdeOsTimerStop(i51MediaPlayerManager.proccessTimerId);		//保护机制，防止Timer无法停止
		return;
	}

	if(i51MediaPlayerManager.bBusyPlaying || i51MediaPlayerManager.bBusyStopping)		//设备繁忙暂不响应消息处理
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Busy Playing = %d, Busy Stopping = %d..... ",
		i51MediaPlayerManager.bBusyPlaying , i51MediaPlayerManager.bBusyStopping );
		return;
	}

	pMsg = _i51AdeMediaPlayerManagerGetMessage();
	if(!pMsg)								//没有需要处理的消息
	{
		//i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Idle ...no message ");
		return;
	}
	if(!pMsg->handle || !pMsg->bEnable)		//无效消息
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Error message ");
		_i51AdeMediaPlayerManagerRemoveMessage();	
		return;
	}

	

	switch(pMsg->eOpreator)					//消息分发处理
	{
		case i51_MEDIAOPERATOR_PLAY:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing PLAY ================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Play(pMsg);
			break;
			
		case i51_MEDIAOPERATOR_PAUSE:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing PAUSE================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Pause(pMsg);
			break;

		case i51_MEDIAOPERATOR_RESUME:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing RESUME================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Resume(pMsg);
			break;

		case i51_MEDIAOPERATOR_STOP:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing STOP================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Stop(pMsg);
			break;

		case i51_MEDIAOPERATOR_RELEASE:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing STOP================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Release(pMsg);
			break;

		default:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Cannot dispose the message  handle = 0x%x", pMsg->handle);
			bHandled = iTRUE;
			break;
	}

	if(bHandled)
		_i51AdeMediaPlayerManagerRemoveMessage();	//消息处理完毕，从队列中移除消息

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing End.....");
	
}



//播放器创建成功，开始消息处理
static void _i51S_AdeMmiMediaPlayerCreateCB( iU32 timerid )
{	
	iBOOL bCreateSuccess = iTRUE;

	i51AdeOsLog(i51_LOG_SC, "MMI : _i51S_AdeMmiMediaPlayerCreateCB Entry");

	i51AdeOsTimerStop(timerid);
	
	if(!gi51IsRunning)
	{
		return;
	}

	do
	{
		i51MediaPlayerManager.proccessTimerId = i51AdeOsTimerCreate(1, 1);
		if(i51MediaPlayerManager.proccessTimerId == 0)
		{
			bCreateSuccess = iFALSE;
			break;
		}

		if(iFALSE == i51AdeOsTimerStart(i51MediaPlayerManager.proccessTimerId, _i51AdeMmiMediaPlayerMessageProcessing, 1))
		{
			bCreateSuccess = iFALSE;
			break;
		}
			
		if( 0 != i51MediaPlayerManager.pfnCreateCb )
		{
			i51MediaPlayerManager.pfnCreateCb ( bCreateSuccess ) ;
		}
		
	}while(0);

	i51AdeOsLog(i51_LOG_SC, "MMI : _i51S_AdeMmiMediaPlayerCreateCB -- Process Timer Id = %d", i51MediaPlayerManager.proccessTimerId);
}



////////////////////////////////////////////////////////////////////////////////////////////
// Adapter Interface
////////////////////////////////////////////////////////////////////////////////////////////

//创建播放器
iBOOL i51AdeMmiMediaPlayerCreate ( void (*callback) ( iBOOL results ) )
{
	iU32 timerid;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , S ");
#endif
		
    if (i51MediaPlayerManager.bCreated == iTRUE)		//已创建
    {
    	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , ERR 1");
        return iFALSE;
    }
    if (!g_i51MediaHandle )
    {
    	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , ERR 2");
        return iFALSE;
    }

	if(!_i51AdeMmiMediaPlayerManagerDataInit(callback))
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , ERR 3");
		return iFALSE;
	}
	
    timerid = i51AdeOsTimerCreate(0, 0);
    i51AdeOsTimerStart(timerid, _i51S_AdeMmiMediaPlayerCreateCB, 50);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , E ");
#endif

	return iTRUE;

}

//关闭播放器
iBOOL i51AdeMmiMediaPlayerRelease () 
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerRelease , S");
#endif
	
	if(!i51MediaPlayerManager.bCreated)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerRelease , ERR 1");
		return iFALSE;
	}

	//_i51AdeMediaPlayerManagerSendMessage(1, i51_MEDIAOPERATOR_RELEASE);
	i51AdeOsTimerStop(i51MediaPlayerManager.proccessTimerId);

	if(i51MediaPlayerManager.bPlaying && !i51MediaPlayerManager.bBusyPlaying)		//正在播放并且设备空闲,停止播放
	{
		_i51AdeMediaPlayerDeviceStopPlayImmediately();
	}	
	
	_i51AdeMmiMediaPlayerManagerDataRelease();		//Lynn add 20120606	

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerRelease , E");
#endif

	return iTRUE;
}

////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

iBOOL i51AdeMmiMediaPlayerPlay ( iU32 priority , void* handle , iU32 length , iU32 type , iU32 dec  , iBOOL loop , void (*STOP)(iHANDLE handle)  ) 
{
	iU32 codec= 0xff;
	iU8 id;
	i51MediaPlayerParamDataStruct stParam = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , S , priority = %d, handle = 0x%x, length = %d, type = %d, dec = %d, loop = %d",
							priority , handle , length , type , dec, loop );
#endif
	
	if(!g_i51MediaHandle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 1");
		return 0;
	}

	/*	参数校验	*/
    if(!handle ||dec >i51_MMI_DEC_MEM_BASED || i51MediaPlayerManager.bCreated== 0)
    {
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 2");
        return 0;
    }
    switch ( type )
    {
	    case i51_MEDIA_TYPE_MIDI :
	        codec = VEN_MEDIA_FORMAT_MID ;
	        break ;
	    case i51_MEDIA_TYPE_WAV :
	        codec = VEN_MEDIA_FORMAT_WAVE ;
	        break ;
		case i51_MEDIA_TYPE_AMR :
			codec = VEN_MEDIA_FORMAT_AMR ;
			break ;
		case i51_MEDIA_TYPE_MP3 :
			codec = VEN_MEDIA_FORMAT_MP3 ;
			break ;
	    default:
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 3");
	        return 0;
    }
	if(dec == i51_MMI_DEC_FILE_BASED)
	{
		if(!i51AdeOsFileIsExist((const u16*)handle))
		{
			i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 4");
			return 0;
		}
	}

	/*	可同时播放的对象已满	*/
	if(!_i51AdeMmiMediaPlayerManagerGetRestNum())
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 5");
		return 0;
	}
	
	/*	发送消息	*/
	stParam.handle = handle;
	stParam.priority = priority;
	stParam.length = length;
	stParam.codec= codec;
	stParam.dec = dec;
	stParam.loop = loop;
	stParam.STOP = STOP;
	
	if(!_i51AdeMediaPlayerManagerSendMessageWithParam(handle, i51_MEDIAOPERATOR_PLAY, &stParam))
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 6");
		return 0;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , E");
#endif

	return 1;

}

iBOOL i51AdeMmiMediaPlayerPause ( void* handle  )
{
	i51MediaPlayerObjStruct* pObj = NULL;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPause , S , 0x%x", handle);
#endif
	
	if(!g_i51MediaHandle)
	{
		return 0;
	}
	if(!handle)
	{
        return 0;
	}

	//获取对象
	pObj =  _i51AdeMediaPlayerManagerGetObjectByHandle(handle);
	if(!pObj)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerResume , ERR ,  Null Obj");
		return 0;
	}

	//发送消息
	if(!_i51AdeMediaPlayerManagerSendMessage(handle, i51_MEDIAOPERATOR_PAUSE))
	{
		return 0;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPause , E");
#endif
	
	return 1;
	
}

iBOOL i51AdeMmiMediaPlayerResume ( void* handle  )
{
	i51MediaPlayerObjStruct* pObj = NULL;

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerResume , S , 0x%x", handle);
#endif	

    if(!g_i51MediaHandle)
    {
        return 0;
    }

	if(!handle)
	{
        return 0;
	}

	//获取对象
	pObj =  _i51AdeMediaPlayerManagerGetObjectByHandle(handle);
	if(!pObj)
	{
		return 0;
	}

	//发送消息
	if(!_i51AdeMediaPlayerManagerSendMessage(handle, i51_MEDIAOPERATOR_RESUME))
	{
		return 0;
	}

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerResume , E");
#endif	
	
	return 1;
	
}

iBOOL i51AdeMmiMediaPlayerStop ( void* handle  )
{
	i51MediaPlayerObjStruct* pObj = NULL;

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , S , 0x%x", handle);
#endif
	
	if(!g_i51MediaHandle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , ERR 1");
		return 0;
	}
	if(!i51MediaPlayerManager.bCreated)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , ERR 2");
		return 0;
	}

	if(!handle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , ERR 3");
		return 0;
	}

	//发送消息
	if(!_i51AdeMediaPlayerManagerSendMessage(handle, i51_MEDIAOPERATOR_STOP))
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , ERR 4");
		return 0;
	}

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , E");
#endif

	return 1;
	
}

iU32 i51AdeMmiMediaPlayerGetVolume ()
{

 	//	author : osca
	//	since : 2011.6.14
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 调整音量，返回1= volume <=7.

	iU8 volume = 0 ;
	float rate = 0 ;
	ven_req_data_t ptVenReq={0};
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerGetVolume , S ");
#endif

	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE;
	
	ven_codec_getAudioVolume(g_i51MediaHandle, &volume, &ptVenReq);
    if(volume == VEN_MEDIA_VOLUME_MAX)
    {
        volume = 7;
    }
    else if(volume == VEN_MEDIA_VOLUME_MIN)
    {
        volume = 1;
    }
    else if(volume == VEN_MEDIA_VOLUME_1)
    {
        volume = 2;
    }

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerGetVolume , E , %d", volume);
#endif
	return volume ;	
	
}

iBOOL i51AdeMmiMediaPlayerSetVolume ( iU32 volume )
{

 	//	author : osca
	//	since : 2011.6.14
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 调整音量，0= volume <=8.
	//	成功返回1 ， 失败返回0

	iU32 ret = 0 ;
	float rate = 0 ;
	ven_req_data_t ptVenReq={0};
		
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerSetVolume , S  , %d",  volume);
#endif

    if(volume >7||volume <1)
    {
        i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerSetVolume , ERR , Media-Play Set Volume");
        return 0;
    }
    
    if(volume == 7)
    {
        volume = VEN_MEDIA_VOLUME_8;
    }
    else if(volume == 1)
    {
        volume = VEN_MEDIA_VOLUME_0;
    }
    else if(volume == 2)
    {
        volume = VEN_MEDIA_VOLUME_1;
    }

	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE;
	ret = ven_codec_setAudioVolume(g_i51MediaHandle, volume , &ptVenReq);

	g_i51LastVolume = volume;
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerSetVolume , E , %d", ret);
#endif

	return ( ret == VEN_CODEC_SUCCESS ) ;
}

/*========================================================
	Media Player -- end
=========================================================*/


