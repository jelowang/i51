 
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#include "i51AdeOs.h"
#include "i51AdeMmi.h"
#include "i51_define.h"

#include "image_proc.h"
#include "block_mem.h"
#include "os_api.h"
#include "std_header.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "lcd_cfg.h"
#include "dal_img.h"                                            

#include "aud_gen.h"
#include "audio_api.h"
#include "audio_prod.h"
#include "sig_code.h"
#if defined (SPRD_6600) || defined (SPRD_6620) || defined (SPRD_6800)
#include "mmi_subwintab.h"
#endif
#include "audio_api.h"
#include "guistring.h"
#include "mmiaudio_ctrl.h"
#include "mmieng_position.h"
#include "AdpcmRecord_adp.h"
#include "Dsp_codec_adp.h "

static GUI_FONT_T i51FontType = SONG_FONT_20;
static iU8 i51MmicreateTimer = 0;
static iU32 i51MmicreateTimerID = 0;

//amr record file, sprd_6600 sprd_6620 in i51AdeMmiRecorderStartEx() used only
static iFILE i51MmiAmrRecordFile = 0;
static iCHAR *i51MmiAmrRecordFileBuff = iNULL;

static i51ADECALLBACK i51AdeCallback = 0 ;
static iU32 i51AdeCreateTimer = 0 ;

//	是否第一次录音
static iBOOL isFirstRecord = iFALSE ;
//
const iU8 QUIET_WAV[] ={
	0x52, 0x49, 0x46, 0x46, 0x38, 0x00, 0x00, 0x00,
	0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20,
	0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x40, 0x1f, 0x00, 0x00, 0x40, 0x1f, 0x00, 0x00,
	0x01, 0x00, 0x08, 0x00, 0x64, 0x61, 0x74, 0x61,
	0x65, 0x03, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80,
	0x7f, 0x81, 0x80, 0x80, 0x7f, 0x81, 0x81, 0x7e,
	0x80, 0x81, 0x80, 0x81, 0x80, 0x7f, 0x80, 0x80
};

iBOOL i51AdeMmiMediaPlayerStop ( void* handle );
static void i51MediaPlayerTimerCallBack(iU32 id);
typedef void (*MEDIAPLAYERTIMERCALLBACK)(iBOOL result);
MEDIAPLAYERTIMERCALLBACK i51playcallback = NULL;
MEDIAPLAYERTIMERCALLBACK i51Mmicreatecallback = NULL;

#if defined(SPRD_6800) || defined(SPRD_6530)
extern iBOOL i51InvalidateLcdRect(LCD_ID_E  lcd_id, GUI_RECT_T  rect) ;
#endif

#ifdef ASYNC_SUPPORT
#define __MEDIA_ASY__
#endif
//#define _AUDIO_MEDIA__

#ifdef __MEDIA_ASY__
#define i51MediaCreateFileHandle(hAudioCodec , pusCodecName , hDevice , pusDevName , psFileName , puiTrackBuffer , uiTrackBufferSize , pNotifyCallbackFunc)	AUDIO_ASYCreateFileHandle(( HAUDIOCODEC) hAudioCodec,(const uint16*) pusCodecName,(HAUDIODEV) hDevice,(const uint16*) pusDevName,(const int16*) psFileName,(uint32*) puiTrackBuffer,(uint32) uiTrackBufferSize,pNotifyCallbackFunc)
#define i51MediaCreateBufferHandle(hAudioCodec , pusCodecName , hDevice , pusDevName  , pucData , uiDataLength , puiTrackBuffer , uiTrackBufferSize , pNotifyCallbackFunc )	AUDIO_ASYCreateBufferHandle(( HAUDIOCODEC) hAudioCodec,(const uint16*) pusCodecName,(HAUDIODEV) hDevice,(const uint16*) pusDevName, (const uint8*) pucData,(uint32) uiDataLength, (uint32*) puiTrackBuffer,(uint32) uiTrackBufferSize,pNotifyCallbackFunc )
#define i51MediaPlay(hAudioHandle , uiOffset )	AUDIO_ASYPlay( (HAUDIO) hAudioHandle,  (uint32) uiOffset ) 
#define i51MediaCloseHandle(hAudioHandle)	AUDIO_ASYCloseHandle( (HAUDIO) hAudioHandle)
#define i51MediaPause(hAudioHandle)	AUDIO_ASYPause( (HAUDIO) hAudioHandle)
#define i51MediaResume(hAudioHandle)	AUDIO_ASYResume( (HAUDIO) hAudioHandle)
#define i51MediaStop(hAudioHandle)	AUDIO_ASYStop( (HAUDIO) hAudioHandle)
#else
#define i51MediaCreateFileHandle(hAudioCodec , pusCodecName , hDevice , pusDevName , psFileName , puiTrackBuffer , uiTrackBufferSize , pNotifyCallbackFunc)	AUDIO_CreateFileHandle(( HAUDIOCODEC) hAudioCodec,(const uint16*) pusCodecName,(HAUDIODEV) hDevice,(const uint16*) pusDevName,(const int16*) psFileName,(uint32*) puiTrackBuffer,(uint32) uiTrackBufferSize,pNotifyCallbackFunc)
#define i51MediaCreateBufferHandle(hAudioCodec , pusCodecName , hDevice , pusDevName  , pucData , uiDataLength , puiTrackBuffer , uiTrackBufferSize , pNotifyCallbackFunc )	AUDIO_CreateBufferHandle(( HAUDIOCODEC) hAudioCodec,(const uint16*) pusCodecName,(HAUDIODEV) hDevice,(const uint16*) pusDevName, (const uint8*) pucData,(uint32) uiDataLength, (uint32*) puiTrackBuffer,(uint32) uiTrackBufferSize,pNotifyCallbackFunc )
#define i51MediaPlay(hAudioHandle , uiOffset )	AUDIO_Play( (HAUDIO) hAudioHandle,  (uint32) uiOffset )
#define i51MediaCloseHandle(hAudioHandle)	AUDIO_CloseHandle( (HAUDIO) hAudioHandle)
#define i51MediaPause(hAudioHandle)	AUDIO_Pause( (HAUDIO) hAudioHandle)
#define i51MediaResume(hAudioHandle)	AUDIO_Resume( (HAUDIO) hAudioHandle)
#define i51MediaStop(hAudioHandle)	AUDIO_Stop( (HAUDIO) hAudioHandle)
#endif


#ifndef _AUDIO_MEDIA__

#ifdef SPRD_6530

typedef struct {

	void *handle ;
	iU32 length ;
	iU32 type ;
	iU32 dec ;
	iU32 priority ;
	iBOOL loop ;
	iBOOL playing ;
	iBOOL iscreate ;
	iU32 playeTimer ;
	iU32 CreateMediaTimer ;
	iU32 AudioHandle ;
	//	声音播放正常  停止记录，声音的句柄
	iU32 RecordAudioHandle ;
	iU32 *Recordhandle ;
	iU32 *Audiobuffer ;
	iU32 asystimer ;
	void (*STOP)(void* handle) ;
}i51MMIPLAYER ;

i51MMIPLAYER i51MmiPlayer = {0} ;

iU32 i51AdemmiGetHandle()
{
	i51AdeOsLog(0, "MMI : i51AdemmiGetHandle , i51MmiPlayer.AudioHandle , %d", i51MmiPlayer.AudioHandle);
	return i51MmiPlayer.AudioHandle; 
}
static void  i51AdeMmiGetFilePath ( iU16 *FilePath , const iU16 *path )
{
	//	author : william
	//	since : 20110829
	//	PKIG Tech. Co., Ltd.
	
	iU16 *GetCurPath  = iNULL ;
	//相对路径的处理 
  	GetCurPath =  i51KernelGetApplicationPath () ;
	i51AdeStdWStrcpy ((iU16*)FilePath , (iCHAR*)GetCurPath ) ;
 	i51AdeStdWStrcat ( (iU16*)FilePath , (iCHAR*)path ) ;
	
}

static iBOOL i51MediaPlayerOnePlayEnd ()
{
	if ( i51MmiPlayer.loop)
	{
             i51AdeOsLog(0, "MMI : i51MediaPlayerOnePlayEnd , i51MediaPlayerNotifyCB play end need loop");
		MMISRVAUD_Stop(i51MmiPlayer.AudioHandle);
		MMISRVAUD_Play(i51MmiPlayer.AudioHandle, 0) ;
	}
	else
	{
	      i51AdeOsLog(0, "MMI : i51MediaPlayerOnePlayEnd , i51MediaPlayerNotifyCB play end needn't loop");	
		MMISRVAUD_Stop(i51MmiPlayer.AudioHandle);
		MMISRVMGR_Free(i51MmiPlayer.AudioHandle);
		i51MmiPlayer.playing = iFALSE ;
		i51MmiPlayer.AudioHandle = 0 ;
		if(i51MmiPlayer.STOP)
			i51MmiPlayer.STOP(i51MmiPlayer.handle);
	}
}

static iBOOL i51MediaPlayerNotifyCB(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
	MMISRVAUD_REPORT_T *report_ptr = PNULL;    
	if(param != PNULL && handle > 0)
	{
		report_ptr = (MMISRVAUD_REPORT_T *)param->data;
		if(report_ptr != PNULL)
		{            
			switch(report_ptr->report)
			{
				case MMISRVAUD_REPORT_END:
					i51MediaPlayerOnePlayEnd () ;
					break;
				case MMISRVAUD_REPORT_PLAY:
				case MMISRVAUD_REPORT_STOP:
				case MMISRVAUD_REPORT_RESUME:
				case MMISRVAUD_REPORT_PAUSE:
				default:
					break;
			}
			return TRUE;
		}
	}
	return FALSE;
}
iBOOL i51AdeMmiMediaPlayerPlay ( iU32 priority , void* handle , iU32 length , iU32 type , iU32 dec , iBOOL loop , void (*STOP)(void* handle)  ) 
{
	MMISRVMGR_SERVICE_REQ_T req = {0};
	MMISRVAUD_TYPE_T audio_srv = {0};
	MMISRV_HANDLE_T audiohandle = 0;
	MMISRVAUD_RET_E res = MMISRVAUD_RET_ERROR;
	iU16 FilePath[64] = {0} ;
	iU8 cur_volume = 7;
      i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , S , priority:%d,handle:%x,length:%d,type:%d,dec:%d,loop:%d,STOP:0x%d", priority, handle,  length,  type,  dec,  loop, STOP);

	if( i51MmiPlayer.playing )
       {
            if( i51MmiPlayer.priority < priority )
            {
 		   i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , other playing and priority above than this , F");
                return iFALSE;
            }

             i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , priority block");
	 	MMISRVAUD_Stop ( i51MmiPlayer.AudioHandle) ;
		MMISRVMGR_Free (i51MmiPlayer.AudioHandle ) ;

	     //	需要通知上层，已经stop
	     //	此处最好用异步，否则可能发生递归情况
	     if ( i51MmiPlayer.STOP)
	     i51MmiPlayer.STOP ( i51MmiPlayer.handle) ;
       }	
	
	req.notify = i51MediaPlayerNotifyCB;
	req.pri = MMISRVAUD_PRI_NORMAL;
	req.client_name = "i51_audio_client";

       cur_volume = i51PortGetCurVolumeValue();
	audio_srv.volume = cur_volume;
	audio_srv.duation = 0;
	audio_srv.eq_mode = 0;
	audio_srv.is_mixing_enable = FALSE;
	audio_srv.play_times = 1;
	audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
	audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;

	if ( i51_MMI_DEC_FILE_BASED == dec ) {

		i51AdeMmiGetFilePath ( FilePath , (iWCHAR*)handle) ;
		audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE ;
		audio_srv.info.ring_file.name = FilePath ;
		audio_srv.info.ring_file.name_len = i51AdeStdWStrlen(FilePath) ;
		
	} else if ( i51_MMI_DEC_MEM_BASED == dec ) {

		audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
		audio_srv.info.ring_buf.data = (uint8 *)handle;
		audio_srv.info.ring_buf.data_len = length ;

	} else {
             i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , dec error , F");
		return iFALSE ;
	}
	
	if( i51_MEDIA_TYPE_MIDI == type ){
		
		audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MIDI;
	}
	else if( i51_MEDIA_TYPE_WAV == type){
		
		audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_WAVE;
	}
	else if( i51_MEDIA_TYPE_AMR == type){
		
		audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MP3;
	}
	else if( i51_MEDIA_TYPE_MP3 == type ){
		
		audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MP3;
	} else {
             i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , type error , F");
		return iFALSE ; ;
	}
	audiohandle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

	if ( audiohandle <= 0 ) {
             i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , Create audiohandle , F");
		return iFALSE ;
	}
       
	res = MMISRVAUD_Play(audiohandle, 0);
//	MMISRVAUD_SetVolume(audiohandle, cur_volume);

	if ( MMISRVAUD_RET_OK != res ) {

		i51AdeOsLog ( 0 , "MMI : i51AdeMmiMediaPlayerPlay , play , F" ) ;
		return iFALSE ;
	}
	
	i51MmiPlayer.handle = handle ;
	i51MmiPlayer.AudioHandle = audiohandle ;
	i51MmiPlayer.loop = loop ;
	i51MmiPlayer.STOP = STOP ;
	i51MmiPlayer.playing = iTRUE ;	
	
	return iTRUE ;
	
}

iBOOL i51AdeMmiMediaPlayerPause ( void* handle ) 
{
	iBOOL ret = 0;

       #ifdef i51ADAPTER_DEBUG
	   i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPause , S , 0x%x", handle);
       #endif
       
       if(!i51MmiPlayer.playing)
       {
	      i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPause , F");
            return iFALSE;
       }
	ret = MMISRVAUD_Pause ( i51MmiPlayer.AudioHandle ) ;	

       #ifdef i51ADAPTER_DEBUG
  	     i51AdeOsLog ( 0 , "MMI : i51AdeMmiMediaPlayerPause , E , %d",ret) ;
       #endif
       
	return  ret  ;
}
iBOOL i51AdeMmiMediaPlayerResume ( void* handle ) 
{
 
	iBOOL ret = 0;

       #ifdef i51ADAPTER_DEBUG
	   i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , S , handle:0x%x", handle);
       #endif
       
       if(!i51MmiPlayer.playing)
       {
 		i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , F");
            return iFALSE;
       }
	ret = MMISRVAUD_Resume ( i51MmiPlayer.AudioHandle ) ;	

       #ifdef i51ADAPTER_DEBUG
	   i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , E , %d", ret);
       #endif
       
	return  ret  ;
	
}

iBOOL i51AdeMmiMediaPlayerStop ( void* handle ) 
{
	AUDIO_RESULT_E ret = 0;
    #ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , S , handle:0x%x i51MmiPlayer.handle:0x%x i51MmiPlayer.asystimer:%d", handle, i51MmiPlayer.handle, i51MmiPlayer.asystimer);
    #endif

	if ( handle != i51MmiPlayer.handle ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , F(1) , %x ,%x" ,handle , i51MmiPlayer.handle);
		return iFALSE ;
	}
       
       if(!i51MmiPlayer.playing)
       {
            i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , F(2)");
            return iFALSE;
       }

	MMISRVAUD_Stop ( i51MmiPlayer.AudioHandle) ;
	MMISRVMGR_Free (i51MmiPlayer.AudioHandle ) ;
	
	 i51MmiPlayer.playing = iFALSE ;
	  i51MmiPlayer.AudioHandle = 0 ;
	 
       #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0 , "MMI : i51AdeMmiMediaPlayerStop , E") ;
       #endif
   
	return iTRUE ;
}
#else
typedef struct {

	void *handle ;
	iU32 length ;
	iU32 type ;
	iU32 dec ;
	iU32 priority ;
	iBOOL loop ;
	iBOOL playing ;
	iBOOL iscreate ;
	iU32 playeTimer ;
	iU32 CreateMediaTimer ;
	iU32 AudioHandle ;
	//	声音播放正常  停止记录，声音的句柄
	iU32 RecordAudioHandle ;
	iU32 *Recordhandle ;
	iU32 *Audiobuffer ;
	iU32 asystimer ;
	void (*STOP)(void* handle) ;
}i51MMIPLAYER ;

#define I51_AUDIOBUFFER_LEN	(150*80*4)
i51MMIPLAYER i51MmiPlayer = {0} ;

static void  i51AdeMmiGetFilePath ( iU16 *FilePath , const iU16 *path )
{


	//	author : william
	//	since : 20110829
	//	PKIG Tech. Co., Ltd.
	
	iU16 *GetCurPath  = iNULL ;
	//相对路径的处理 
  	GetCurPath =  i51KernelGetApplicationPath () ;
	i51AdeStdWStrcpy ((iU16*)FilePath , (iCHAR*)GetCurPath ) ;
 	i51AdeStdWStrcat ( (iU16*)FilePath , (iCHAR*)path ) ;
	
}

iU32 i51AdemmiGetHandle()
{
	return i51MmiPlayer.AudioHandle; 
}
static void i51MediaPlayerContinueCB(iU32 timerID)
{
      //	author : steven
     //	since : 2011.12.21
     //	notes : 

      AUDIO_RESULT_E result = 0;
      i51AdeOsTimerStop(i51MmiPlayer.asystimer);
      result = i51MediaPlay( i51MmiPlayer.AudioHandle , 0 ) ;
      i51MmiPlayer.asystimer =  0;
      i51AdeOsLog(0, "MMI : i51MediaPlayerContinueCB , %d", result);
}

static void i51MediaPlayerStopCB(iU32 timerID)
{
      //	author : steven
     //	since : 2011.12.21
     //	notes :

      AUDIO_RESULT_E result = 0;
      i51AdeOsTimerStop(i51MmiPlayer.asystimer);
      i51MmiPlayer.asystimer =  0;

	if ( (i51MmiPlayer.RecordAudioHandle == i51MmiPlayer.AudioHandle) && (i51MmiPlayer.Recordhandle == i51MmiPlayer.handle ) )
    {

		//	有可能在停止回调，还没有过来的时候，优先级高的声音，将它关闭
	      i51MediaStop ( i51MmiPlayer.AudioHandle) ;
	      i51MediaCloseHandle ( i51MmiPlayer.AudioHandle ) ;
	      i51MmiPlayer.playing = 0 ;
	      i51MmiPlayer.AudioHandle = 0 ;
          i51AdeOsLog(0, "MMI : i51MediaPlayerStopCB , send app");
		  if ( i51MmiPlayer.STOP )
				i51MmiPlayer.STOP ( i51MmiPlayer.handle ) ;
	}
    else
        i51AdeOsLog(0, "MMI : i51MediaPlayerStopCB , already stop");

}


static void i51MediaPlayerNotifyCB(iU32 iHandle,iU32 Info1,iU32 Info2)
{

     //	notes :播放媒体数据的回调函数

     AUDIO_RESULT_E result = 0;
     
     #ifdef i51ADAPTER_DEBUG
     i51AdeOsLog(0, "MMI : i51MediaPlayerNotifyCB , S , iHandle:%d,Info1:%d,Info2:%d,%d", iHandle, Info1, Info2, AUDIO_PLAYEND_IND);
     #endif
     
     if( iHandle != i51MmiPlayer.AudioHandle ) return;
     if( AUDIO_PLAYEND_IND == Info1 )
     {
          if( i51MmiPlayer.loop ) {
              i51AdeOsLog(0, "MMI : i51MediaPlayerNotifyCB , play end need loop");
    		i51MmiPlayer.asystimer = i51AdeOsTimerCreate (0,1) ;
	    	i51AdeOsTimerStart(i51MmiPlayer.asystimer,i51MediaPlayerContinueCB,1);
          }
          else
          {
              i51AdeOsLog(0, "MMI : i51MediaPlayerNotifyCB , play end needn't loop");
    		i51MmiPlayer.asystimer = i51AdeOsTimerCreate (0,1) ;
	    	//	讲声音句柄保留，防止出现多处关闭的，或者错误关闭情况
		    i51MmiPlayer.RecordAudioHandle = i51MmiPlayer.AudioHandle ;
    		i51MmiPlayer.Recordhandle = i51MmiPlayer.handle ;
	    	i51AdeOsTimerStart(i51MmiPlayer.asystimer,i51MediaPlayerStopCB,1);
          }
     }
     #ifdef i51ADAPTER_DEBUG
     i51AdeOsLog(0, "MMI : i51MediaPlayerNotifyCB , E");
     #endif
}

iBOOL i51AdeMmiMediaPlayerPlay ( iU32 priority , void* handle , iU32 length , iU32 type , iU32 dec , iBOOL loop , void (*STOP)(void* handle)  ) 
{


	 iU32 i51hAudioCodec = 0;
       iU32 i51hDevice = 0;
	 iS8 SysAudioType = 0 ;
       iU16 FilePath[64] = {0};
	 iBOOL result=  iFALSE ;

     #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , S , priority:%d,handle:%x,length:%d,type:%d,dec:%d,loop:%d", priority, handle, length, type, dec, loop);
     #endif

	 #if defined (SPRD_6620) || defined (SPRD_6600)
	 if ( !loop && i51_MEDIA_TYPE_AMR != type ) {
         i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , sprd 66系列 don't loop and don't i51_MEDIA_TYPE_AMR(for vxin only)");
		 return iTRUE;
	}
	 #endif
       if( i51MmiPlayer.playing )
       {
            if( i51MmiPlayer.priority < priority )
            {
                i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , F");
                return iFALSE;
            }

            i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , priority block");
            i51MediaStop ( i51MmiPlayer.AudioHandle) ;
            i51MediaCloseHandle (i51MmiPlayer.AudioHandle ) ;
	     //	需要通知上层，已经stop
	     //	此处最好用异步，否则可能发生递归情况
	     if ( i51MmiPlayer.STOP)
	     i51MmiPlayer.STOP ( i51MmiPlayer.handle) ;
       }

       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , i51MmiPlayer.playing = %d", i51MmiPlayer.playing);

	if ( type ==  i51_MEDIA_TYPE_MIDI ) {

		i51hAudioCodec = hMIDICodec ;
	}
	else if ( type ==  i51_MEDIA_TYPE_WAV )  {

		i51hAudioCodec = hWAVCodec ;
	}
	else if ( type ==  i51_MEDIA_TYPE_AMR )  {

		i51hAudioCodec = hAMRCodec ;
	}	
	else if ( type ==  i51_MEDIA_TYPE_MP3 )  {

//		i51hAudioCodec = hMP3Codec ;
		i51hAudioCodec = hMP3DSPCodec ;
	}	
	else {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , Err AudioCodec , F");
		return iFALSE ;
	}

	#if (defined(SPRD_6600) || defined(SPRD_6620) )
	        if ( type ==  i51_MEDIA_TYPE_AMR )
 	               i51hDevice = hMP4VB;	
		else
	               i51hDevice = hARMVB;	
	#else
	       i51hDevice = hARMVB;	
	#endif
	switch(dec)
	{
		case i51_MMI_DEC_FILE_BASED:
		{
            i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , in i51_MMI_DEC_FILE_BASED");
			     i51AdeMmiGetFilePath ( FilePath , (iWCHAR*)handle) ;
			     i51MmiPlayer.AudioHandle  = i51MediaCreateFileHandle (
				 	i51hAudioCodec ,
				 	NULL ,
				 	i51hDevice ,
				 	NULL ,
				 	(iU16*)FilePath ,
				 	i51MmiPlayer.Audiobuffer ,
				 	I51_AUDIOBUFFER_LEN/4 ,
				 	i51MediaPlayerNotifyCB
				 ) ;
              }
		break; 
		case i51_MMI_DEC_MEM_BASED:
		{
                 i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , in i51_MMI_DEC_MEM_BASED");
			     i51MmiPlayer.AudioHandle  =  i51MediaCreateBufferHandle (
				 	i51hAudioCodec ,
				 	NULL ,
				 	i51hDevice ,
				 	NULL ,
				 	handle ,
				 	length ,
				 	i51MmiPlayer.Audiobuffer ,
				 	I51_AUDIOBUFFER_LEN/4 ,
				 	i51MediaPlayerNotifyCB				 	
				);
          }
		break;
		default:
         {
             i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , Err DEC , F");
			return iFALSE;
         }
	}
      
	if ( i51MmiPlayer.AudioHandle <= 0 ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , Create Handle fail , %d", i51MmiPlayer.AudioHandle);
		return iFALSE ;
	}

#if defined (SPRD_6530)
	//AUDIO_DEVICE_MODE_TVOUT
	AUDIO_SetDevMode (AUDIO_DEVICE_MODE_HANDFREE) ;
	AUDIO_SetVolume ( i51PortGetCurVolumeValue ()) ;
#endif	

    result = i51MediaPlay ( i51MmiPlayer.AudioHandle , 0 ) ;
    if(result != AUDIO_NO_ERROR)
    {
  		i51MediaCloseHandle (i51MmiPlayer.AudioHandle ) ;
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , play fail , %d", result);
        return iFALSE;
     }

	i51MmiPlayer.priority = priority ;
	i51MmiPlayer.handle = handle ;
	i51MmiPlayer.length = length ;
	i51MmiPlayer.type = type ;
	i51MmiPlayer.loop = loop ;
	i51MmiPlayer.dec = dec ;
	i51MmiPlayer.STOP = STOP ;
	i51MmiPlayer.playing = iTRUE ;

    #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , E");
    #endif
    
	return iTRUE;	
}
 
 iBOOL i51AdeMmiMediaPlayerPause ( void* handle ) 
 {

 	//	author : steven
	//	since : 2011.12.21
	//	notes : 暂停播放
	//	成功返回1 ， 失败返回0


	iBOOL ret = 0;

    #ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPause , S , %x", handle);
    #endif
       
       if(!i51MmiPlayer.playing)
       {
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPause , F");
            return iFALSE;
       }
	ret = i51MediaPause ( i51MmiPlayer.AudioHandle ) ;	

       #ifdef i51ADAPTER_DEBUG
          i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPause , E , %d", ret);
       #endif
       
	return  ret  ;
	
}
 
iBOOL i51AdeMmiMediaPlayerResume ( void* handle ) 
{

 	//	author : steven
	//	since : 2011.12.21

	//	notes : 继续播放
	//	成功返回1 ， 失败返回0

	iBOOL ret = 0 ;

       #ifdef i51ADAPTER_DEBUG
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , S , 0x%x", handle);
       #endif
       
       if(!i51MmiPlayer.playing)
       {
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , F");
            return iFALSE;
       }
	ret = i51MediaResume ( i51MmiPlayer.AudioHandle ) ;	


       #ifdef i51ADAPTER_DEBUG
         i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , E , %d", ret);
       #endif
       
	return ret ;
	 
	
}
 
iBOOL i51AdeMmiMediaPlayerStop ( void* handle ) 
{
       //	author : steven
	//	notes : 停止播放
	//	成功返回1 ， 失败返回0


	AUDIO_RESULT_E ret = 0;

       #ifdef i51ADAPTER_DEBUG
          i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , S , %x,i51CurrentHand:%x,timer:%d",handle, i51MmiPlayer.handle, i51MmiPlayer.asystimer);
       #endif

	if ( handle != i51MmiPlayer.handle ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , F(1) , %x ,%x" ,handle , i51MmiPlayer.handle);
		return iFALSE ;
	}
       
       if(!i51MmiPlayer.playing)
       {
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , F(2)");
           return iFALSE;
       }

	if (i51MmiPlayer.asystimer) {

		i51AdeOsTimerStop(i51MmiPlayer.asystimer);
		i51MmiPlayer.asystimer = 0 ;
	}
	i51MediaStop ( i51MmiPlayer.AudioHandle) ;
	i51MediaCloseHandle (i51MmiPlayer.AudioHandle ) ;
	i51MmiPlayer.AudioHandle = 0 ;
	 i51MmiPlayer.playing = iFALSE ;
	 
       #ifdef i51ADAPTER_DEBUG
            i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , E , %d", ret);
       #endif
   
	return iTRUE ;
	
}
#endif
#else

typedef struct {

	void *handle ;
	iU32 length ;
	iU32 type ;
	iU32 dec ;
	iU32 priority ;
	iBOOL loop ;
	iBOOL playing ;
	iU32 playeTimer ;
	iU32 CreateMediaTimer ;
	void (*STOP)(void* handle) ;
}i51MMIPLAYER ;

i51MMIPLAYER i51MmiPlayer = {0} ;

static iS32 i51AdeMmiConvetPlayFormat ( iU32 format)
{

	//	author : william
	//	since : 20110801
	//	PKIG Tech. Co., Ltd.
	
	switch ( format  ) {

		case i51_MEDIA_TYPE_MIDI :
			return MMIAUDIO_MIDI ;
		case i51_MEDIA_TYPE_WAV :
			return MMIAUDIO_WAVE ;
		case i51_MEDIA_TYPE_AMR :
			return MMIAUDIO_AMR ;
	}
} 

static void  i51AdeMmiGetFilePath ( iU16 *FilePath , const iU16 *path )
{

	//	author : william
	//	since : 20110829
	//	PKIG Tech. Co., Ltd.
	
	iU16 *GetCurPath  = iNULL ;
	//相对路径的处理 
  	GetCurPath =  i51KernelGetApplicationPath () ;
	i51AdeStdWStrcpy ((iU16*)FilePath , (iCHAR*)GetCurPath ) ;
 	i51AdeStdWStrcat ( (iU16*)FilePath , (iCHAR*)path ) ;
	
}

static void i51AdeMmiPlayEnd ( MMIAUDIO_RESULT result, DPARAM param )
{
	iBOOL ret = iFALSE ;
    i51AdeOsLog(0, "MMI : i51AdeMmiPlayEnd , %d,%x" , result , param);
	i51MmiPlayer.playeTimer -- ;
	if (i51MmiPlayer.playeTimer) {

//		ret = MMIAUDIO_AudioPlay ( MMIAUDIO_APP , 0 , 1 , i51AdeMmiPlayEnd  ) ;
        i51AdeOsLog(0, "MMI : i51AdeMmiPlayEnd , loop continue play , %d , %d" , i51MmiPlayer.playeTimer , result);
		return ;
	} 

	if (!i51MmiPlayer.loop) {

            MMIAUDIO_AudioStop ( MMIAUDIO_APP) ;
            MMIAUDIO_CloseAudioHandle (MMIAUDIO_APP ) ;		
	     i51MmiPlayer.playing = iFALSE ;
         i51AdeOsLog(0, "MMI : i51AdeMmiPlayEnd , send app");
		if ( i51MmiPlayer.STOP)
			i51MmiPlayer.STOP (i51MmiPlayer.handle) ;
	}
	
}
iBOOL i51AdeMmiMediaPlayerPlay ( iU32 priority , void* handle , iU32 length , iU32 type , iU32 dec , iBOOL loop , void (*STOP)(void* handle)  ) 
{
  
       iU16 FilePath[64] = {0};
	 iBOOL result=  iFALSE ;

       #ifdef i51ADAPTER_DEBUG
         i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , S , priority:%d,handle:%x,length:%d,type:%d,dec:%d,loop:%d", priority, handle, length, type, dec, loop);
       #endif

       if(i51MmiPlayer.playing)
       {
            if(i51MmiPlayer.priority<priority)
            {
                i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , F(1)");
                return iFALSE;
            }
			
            MMIAUDIO_AudioStop ( MMIAUDIO_APP) ;
            MMIAUDIO_CloseAudioHandle (MMIAUDIO_APP ) ;
       }

       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , i51MmiPlayer.playing = %d" , i51MmiPlayer.playing);
	switch(dec)
	{
		case i51_MMI_DEC_FILE_BASED:
		{
            i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , in i51_MMI_DEC_FILE_BASED");
			     i51AdeMmiGetFilePath ( FilePath , (iWCHAR*)handle) ;
                        result = MMIAUDIO_CreateAudioFileHandle(
                        iFALSE, 
                        MMIAUDIO_APP,
                        i51AdeMmiConvetPlayFormat (type) ,
                        (iWCHAR*)FilePath
                        );
              }
		break; 
		case i51_MMI_DEC_MEM_BASED:
		{
            i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , in i51_MMI_DEC_MEM_BASED");
                        result = MMIAUDIO_CreateAudioBufHandle(
                        iFALSE, 
                        MMIAUDIO_APP,
                        i51AdeMmiConvetPlayFormat (type), 
           		     (iU8*)handle ,
           		     length
                        );
              }
		break;
		default:
                {
                    i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , Err dec , F");
			        return iFALSE;
                }
	}
      
	if ( !result ) {

		iS8 filename[128] = {0} ;
		i51AdeStdUnicode2Ascii ( FilePath , filename ) ;
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , Create Handle fail , %s,%d" , filename , result);
		return iFALSE ;
	}


	if ( loop ) 
		i51MmiPlayer.playeTimer= 256 ;
	else
		i51MmiPlayer.playeTimer = 1 ;
	result = MMIAUDIO_AudioPlay ( MMIAUDIO_APP , 0 , i51MmiPlayer.playeTimer , i51AdeMmiPlayEnd  ) ;
	
	if(!result)
       {
  		MMIAUDIO_CloseAudioHandle (MMIAUDIO_APP ) ;
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , play fail , %d" , result);
             return iFALSE;
       }  

	i51MmiPlayer.priority = priority ;
	i51MmiPlayer.handle = handle ;
	i51MmiPlayer.length = length ;
	i51MmiPlayer.type = type ;
	i51MmiPlayer.loop = loop ;
	i51MmiPlayer.dec = dec ;
	i51MmiPlayer.STOP = STOP ;
	i51MmiPlayer.playing = iTRUE ;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , E");
    #endif
       
	return iTRUE;	
}
 
 iBOOL i51AdeMmiMediaPlayerPause ( void* handle ) 
 {

 	//	author : steven
	//	since : 2011.12.21
	//	notes : 暂停播放
	//	成功返回1 ， 失败返回0

	iBOOL ret = 0;

       #ifdef i51ADAPTER_DEBUG
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPause , S , %x",handle);
       #endif
       
       if(!i51MmiPlayer.playing)
       {
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPause , F");
            return iFALSE;
       }
	ret = MMIAUDIO_AudioPause ( MMIAUDIO_APP ) ;	

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPause , E , %d", ret);
    #endif
       
	return  ret  ;
	
}
 
iBOOL i51AdeMmiMediaPlayerResume ( void* handle ) 
{

 	//	author : steven
	//	since : 2011.12.21

	//	notes : 继续播放
	//	成功返回1 ， 失败返回0

	iBOOL ret = 0 ;

       #ifdef i51ADAPTER_DEBUG
          i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , S , %x",handle);
       #endif
       
       if(!i51MmiPlayer.playing)
       {
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , F");
            return iFALSE;
       }
	ret = MMIAUDIO_AudioResume ( MMIAUDIO_APP ) ;	


       #ifdef i51ADAPTER_DEBUG
          i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerResume , E , %d", ret);
       #endif
       
	return ret ;
	
}
 
iBOOL i51AdeMmiMediaPlayerStop ( void* handle ) 
{
       //	author : steven
	//	notes : 停止播放
	//	成功返回1 ， 失败返回0

	AUDIO_RESULT_E ret = 0;

       #ifdef i51ADAPTER_DEBUG
         i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , S , %x,i51CurrentHand:%x",handle,i51MmiPlayer.handle);
       #endif

	if ( handle != i51MmiPlayer.handle ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , F(1) , %x ,%x" ,handle , i51MmiPlayer.handle);
		return iFALSE ;
	}
       
       if(!i51MmiPlayer.playing)
       {
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , F(2)");
            return iFALSE;
       }
	 MMIAUDIO_AudioStop ( MMIAUDIO_APP ) ;	
	 MMIAUDIO_CloseAudioHandle (MMIAUDIO_APP ) ;
	 i51MmiPlayer.playing = iFALSE ;
	 i51MmiPlayer.AudioHandle = 0 ;

     #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , E , %d", ret);
     #endif
       
	return iTRUE ;
	
}
#endif

iBOOL i51AdeMmiMediaPlayerSetVolume ( iU32 volume )   
{

       //	author : steven
	//	since : 2011.12.21

	//	notes : 调整音量，返回1= volume <=7.
	//	成功返回1 ， 失败返回0

	iU32 ret = 0 ;
	float rate = 0 ;

    #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerSetVolume , S , %d", volume);
    #endif
       
	if ( 1 > volume ) volume = 1 ;
	else if ( volume > 7 ) volume = 7 ;

	ret = AUDIO_SetVolume ( volume ) ;

    #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerSetVolume , E , %d", ret);
    #endif
       
	return ( ret == AUDIO_NO_ERROR ) ;
}
 
iU32 i51AdeMmiMediaPlayerGetVolume ( ) 
{

 	//	author : steven
	//	since : 2011.12.21
	//	notes : 成功返回> 0 ，失败0

	iU32 volume = 0 ;

       #ifdef i51ADAPTER_DEBUG
            i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerGetVolume , S");
       #endif
       
	AUDIO_GetVolume ( &volume ) ;

       #ifdef i51ADAPTER_DEBUG
           i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerGetVolume , E , %d", volume);
      #endif
       
	return volume ;	
	
}


void i51AdeMmiUpdateScreen (void) 
{

        //	author : steven
	//	since : 2011.12.21

	iS32 ret = 0 ;
    
     GUI_RECT_T rect = {0};

    #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "MMI : i51AdeMmiUpdateScreen , S");
    #endif


	#ifdef i51PROFILINGS
		i51KernelDrawProfiles () ;
	#endif
	
	rect.right = LCD_WIDTH - 1;
	rect.bottom = LCD_HEIGHT - 1;	

   #if defined ( SPRD_6800 ) || defined ( SPRD_6530 )
  // ret = GUILCD_Invalidate ( GUI_MAIN_LCD_ID , 0 ) ;
	ret = i51InvalidateLcdRect ( GUI_MAIN_LCD_ID ,  rect ) ;
   #else
    ret = GUILCD_InvalidateLCDRect(
		GUI_MAIN_LCD_ID, 
		rect.left,
		rect.top, 
        rect.right, 
        rect.bottom,
        0);  
   #endif
   #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "MMI : i51AdeMmiUpdateScreen , E");
    #endif
}

void i51AdeMmiUpdateScreenEx ( iRECT* irect , iU32 length )
{

	iU16 i = 0 ;
	GUI_RECT_T rect = {0};
	for ( i = 0 ; i < length ; i++ ) {

		memset ( &rect , 0 , sizeof(rect) ) ;
		rect.left = irect[i].X ;
		rect.top = irect[i].Y ;
		rect.right = irect[i].X + irect[i].Width - 1 ;
		rect.bottom= irect[i].Y + irect[i].Height -1  ; 
		#if defined ( SPRD_6800 ) || defined ( SPRD_6530 )
		i51InvalidateLcdRect ( GUI_MAIN_LCD_ID ,  rect ) ;
		#else
		GUILCD_InvalidateLCDRect(
		GUI_MAIN_LCD_ID, 
		rect.left,
		rect.top, 
		rect.right, 
		rect.bottom,
		0);  
		#endif
	}
}


static void i51MediaPlayerTimerCallback(iU32 timerID)
{
     //	author : steven
     //	since : 2011.12.21
     //	notes :i51AdeMmiMediaPlayerCreate 函数的timer回调函数
     
    i51AdeOsLog(0, "MMI : i51MediaPlayerTimerCallback , S");
     i51AdeOsTimerStop( i51MmiPlayer.CreateMediaTimer);
     i51MmiPlayer.CreateMediaTimer = 0;
     if(NULL!=i51playcallback)
      i51playcallback(1);
    i51AdeOsLog(0, "MMI : i51MediaPlayerTimerCallback , E");

}

#ifndef _AUDIO_MEDIA__
iBOOL i51AdeMmiMediaPlayerCreate ( void (*callback) ( iBOOL results ) ) 
{

	 iU32 volume = 0;
	 iS32 mode = 0 ;
     i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerCreate , S");

	if ( i51MmiPlayer.iscreate) {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerCreate , already create , F");
		return iFALSE ;
	}
	#if defined (SPRD_6800) || defined (SPRD_6600) || defined (SPRD_6620)
       MMIAUDIO_SetMute(FALSE);
       MMIAPISET_GetDeviceMode(&mode);
       AUDIO_GetVolume ( &volume ) ;
       MMIAUDIO_SetVolume(mode, (uint32)volume);
	 #endif
	 MMIAPISET_StopAllVibrator(); 
       MMIAPISET_StopAllRing(TRUE);
       

	 //	屏蔽按键音，触屏音
	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iFALSE);
	#elif defined ( SPRD_6530 )
	MMIDEFAULT_EnableKeyRing( "i51", iFALSE);
	MMIDEFAULT_EnableTpRing ( "i51" , iFALSE) ;
	#else
       MMIDEFAULT_EnableKeyRing( iFALSE );
	MMIDEFAULT_EnableTpRing ( iFALSE) ;
	#endif

	memset ( &i51MmiPlayer , 0 , sizeof(i51MmiPlayer) ) ;
	#ifndef SPRD_6530
	i51MmiPlayer.Audiobuffer = (iU32*)i51AdeOsMalloc ( I51_AUDIOBUFFER_LEN , __FILE__, __LINE__ ) ;
	if ( !i51MmiPlayer.Audiobuffer ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerCreate , F");
		return iFALSE ;
	}
	#endif
       i51MmiPlayer.CreateMediaTimer = i51AdeOsTimerCreate (0,1) ;
       i51AdeOsTimerStart( i51MmiPlayer.CreateMediaTimer , i51MediaPlayerTimerCallback,1);
       i51playcallback = callback;
       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerCreate , E");
	 
        return iTRUE;
}


iBOOL i51AdeMmiMediaPlayerRelease () 
{

	//	恢复按键音，触屏音
    i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerRelease , S");
	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iTRUE);
	#elif defined ( SPRD_6530 )
	MMIDEFAULT_EnableKeyRing( "i51", iTRUE);
	MMIDEFAULT_EnableTpRing ( "i51" , iTRUE) ;
	#else
	MMIDEFAULT_EnableKeyRing(iTRUE);
	MMIDEFAULT_EnableTpRing (iTRUE) ;
	#endif

#ifndef SPRD_6530
	 if ( i51MmiPlayer.Audiobuffer ) {

		i51AdeOsFree (i51MmiPlayer.Audiobuffer) ;
	 }
#endif
	 if ( i51MmiPlayer.CreateMediaTimer)
	 	 i51AdeOsTimerStop(i51MmiPlayer.CreateMediaTimer);
	  i51MmiPlayer.CreateMediaTimer = 0 ;
	  memset (&i51MmiPlayer , 0 , sizeof(i51MmiPlayer)) ;

	 i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerRelease , E");
	 return iTRUE;	 
}
#else
iBOOL i51AdeMmiMediaPlayerCreate ( void (*callback) ( iBOOL results ) ) 
{
	 iU32 volume = 0;
	 iS32 mode = 0 ;
       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerCreate , S");


       MMIAUDIO_SetMute(FALSE);
       MMIAPISET_GetDeviceMode(&mode);
       AUDIO_GetVolume ( &volume ) ;
       MMIAUDIO_SetVolume(mode, (uint32)volume);
	 MMIAPISET_StopAllVibrator(); 
       MMIAPISET_StopAllRing(TRUE);
 
	 //	屏蔽按键音，触屏音
	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iFALSE);
	#else
       MMIDEFAULT_EnableKeyRing(iFALSE);
	MMIDEFAULT_EnableTpRing (iFALSE) ;
	#endif


	

 
       i51MmiPlayer.CreateMediaTimer = i51AdeOsTimerCreate (0,1) ;
       i51AdeOsTimerStart( i51MmiPlayer.CreateMediaTimer , i51MediaPlayerTimerCallback,1);
       i51playcallback = callback;
       i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerCreate , E");
        return iTRUE;
}


iBOOL i51AdeMmiMediaPlayerRelease () 
{

	//	恢复按键音，触屏音
	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iTRUE);
	#else
       MMIDEFAULT_EnableKeyRing(iTRUE);
	MMIDEFAULT_EnableTpRing (iTRUE) ;
	#endif

    
	
     if ( i51MmiPlayer.CreateMediaTimer)
	 	 i51AdeOsTimerStop(i51MmiPlayer.CreateMediaTimer);
      i51MmiPlayer.CreateMediaTimer = 0 ;
     memset (&i51MmiPlayer , 0 , sizeof(i51MmiPlayer)) ;
     return iTRUE;
	 }
#endif

iS32 i51AdeMmiMediaPlayerFormat ()
{
	//	返回可以可以播放的格式
	iS32 format;

	#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerFormat , S");
	#endif

	format = i51_MEDIA_TYPE_MIDI|i51_MEDIA_TYPE_WAV|i51_MEDIA_TYPE_MP3|i51_MEDIA_TYPE_AMR;

	#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerFormat , E , format = %d", format);
	#endif

	return format;

}


typedef struct {

	void *buffer ;
	iU32 bufflen ;
	iU32 RecordHandle ;
	iBOOL recodering ;
	iU32 *RecordeNeedbuffer;
    iU8 IsPcm;
    iU16 name[64] ;
    iU32 ReadPcmLen;
    iU8 *RecordBuffPoint;
    iU32 SendPcmTimer;
	void (*callback ) ( void* stream , iU32 length ) ;

}I51MMIRECORDER ;


I51MMIRECORDER i51MmiRecoder = {0} ;



static void i51AdeMmiRecorderNotifyCallback(HAUDIO hAudio, iU32 notify_info, iU32 affix_info)
{
    i51AdeOsLog(0, "MMI : i51AdeMmiRecorderNotifyCallback , AudioHandle , %d,%d,%d,%d" ,hAudio , notify_info , affix_info , AUDIO_PLAYEND_IND);
}

#if 1
#else
static iU32 RecordLen = 0;
static void i51AdeMmiRecorderDateDone ( iU32 id )
{
	iS32 SEEK = 0 ;	
    iCHAR *BackData = NULL;
    iBOOL ReadResult = 0;
    AUDIO_RESULT_E audioFLag = 0;
    iFILE file = 0;
    
    iS32 len = 0;
    iS32 SetSeek = 0;
    iCHAR *point = (iCHAR *)i51MmiRecoder.buffer;
    

    if(NULL==i51MmiRecoder.buffer)
    {
        return;
    }
           
    i51AdeOsFileGetSeek(i51MmiRecoder.FileHandle, &SEEK,i51_OS_FL_SEEK_HEAD);
    if(SEEK==0)
    {
        return;
    }

    len = SEEK - RecordLen - 60;
    if(len>i51MmiRecoder.bufflen)
    {
        len = i51MmiRecoder.bufflen;
    }

    SetSeek = -(SEEK-RecordLen-60);

    i51AdeOsFileSetSeek (i51MmiRecoder.FileHandle,SetSeek,i51_OS_FL_SEEK_TAIL);
    
    ReadResult = i51AdeOsFileRead (i51MmiRecoder.FileHandle,point,len ,0);
    
    RecordLen = RecordLen + len;
    
    if(NULL!=i51MmiRecoder.callback)
        i51MmiRecoder.callback(point ,len);
}
#endif

static void i51AUD_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length )
{
    DSP_CODEC_EXT_OPE_VOICE_DATA_T ptPara;
    uint32 temp;
    ptPara.source_buf_ptr = destination_buf_ptr;
    ptPara.data_length = data_length;
    
    AUDIO_ExeCodecExtOpe(i51MmiRecoder.RecordHandle, 
                            ANSI2UINT16("READ_VOICE_DATA"), 
                            DSP_CODEC_EXT_OPE_VOICE_DATA_PARA_COUNT, 
                            &ptPara,
                            &temp); 

}

static MEDIAPLAYERTIMERCALLBACK RecordBack = NULL;
static iBOOL RecordStopResult = 0;
static AUDIO_RESULT_E i51AUD_EnableVoiceRecord(iBOOL is_enable,AUD_REC_FORMAT_E recorder_format,RECORD_CALLBACK callback_fun );

static void timerCallback(iU32 id)
{
    if(i51MmiRecoder.ReadPcmLen==0)
    {
        return;
    }
    if(NULL!=i51MmiRecoder.callback)
        i51MmiRecoder.callback(i51MmiRecoder.buffer,i51MmiRecoder.ReadPcmLen);

    i51MmiRecoder.ReadPcmLen = 0;
    i51MmiRecoder.RecordBuffPoint = i51MmiRecoder.buffer;
}

static void i51RecordCallBack(iU16 i_len)
{
    //	author : steven
	//	since : 2012.07.21
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

    //录音回调，返回录音数据的长度
    
    iU16 ReadLen = i_len;
    iU8 *point = NULL;
    iU16 i = 0;
    iU8 data = 0;

    if(i51MmiRecoder.ReadPcmLen>=i51MmiRecoder.bufflen)
    {
        return;
    }
    
    if((i51MmiRecoder.ReadPcmLen+i_len*2)>i51MmiRecoder.bufflen)
    {
        ReadLen = (i51MmiRecoder.bufflen - i51MmiRecoder.ReadPcmLen)/2;
    }
    else
        ReadLen = i_len;
    
    i51AUD_ReadVoiceData((uint16 *)i51MmiRecoder.RecordBuffPoint, ReadLen);
    
    ReadLen = ReadLen*2;
    
    i51MmiRecoder.ReadPcmLen = i51MmiRecoder.ReadPcmLen + ReadLen;
    
    #ifndef SPRD_6530
    point = (iU8 *)i51MmiRecoder.RecordBuffPoint;
    while(i<ReadLen)
    {
        data = point[i];
        point[i] = point[i+1];
        point[i+1] = data;
        i = i+2;
    }
    #endif
    i51MmiRecoder.RecordBuffPoint = i51MmiRecoder.RecordBuffPoint + ReadLen;
}

static AUDIO_RESULT_E i51AUD_EnableVoiceRecord(iBOOL is_enable,AUD_REC_FORMAT_E recorder_format,RECORD_CALLBACK callback_fun )
{
    //	author : steven
	//	since : 2012.07.21
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes :启动/停止录音， is_enable为1启动录音，为0停止录音；成功返回 AUDIO_NO_ERROR ，失败AUDIO_ERROR

    DSP_CODEC_EXT_OPE_START_RECORD_T ptPara = {0}; 
    uint32 temp = 0; 
    AUDIO_RESULT_E status = 0;
    if (is_enable)
    {        
        ptPara.recorder_format = recorder_format;
        ptPara.callback_fun = callback_fun;
        ptPara.hAudioHandle = i51MmiRecoder.RecordHandle;
        status = AUDIO_ExeCodecExtOpe(i51MmiRecoder.RecordHandle, 
                        ANSI2UINT16("EXT_START_RECORD"), 
                        DSP_CODEC_EXT_OPE_START_RECORD_PARA_COUNT, 
                        &ptPara,
                        &temp);  
    }
    else
    {
        status = AUDIO_ExeCodecExtOpe(i51MmiRecoder.RecordHandle, 
                        ANSI2UINT16("EXT_STOP_RECORD"), 
                        1, 
                        (void *)i51MmiRecoder.RecordHandle,
                        &temp); 
    }
        
    if(AUDIO_NO_ERROR == status)
    {
        return AUDIO_NO_ERROR;      
    }
    else
    {
        return AUDIO_ERROR;  
    }          
}


static void i51AdeMmiPlayQuietStop ( void * handle ){}
static void i51AdeMmiPlayQuiet ()
{
	if ( isFirstRecord ) {

		return ;
	}
    i51AdeOsLog(0, "MMI : i51AdeMmiPlayQuiet , play quite");
	i51AdeMmiMediaPlayerCreate (0) ;
	if (!i51AdeMmiMediaPlayerPlay (5 , QUIET_WAV , sizeof(QUIET_WAV), i51_MEDIA_TYPE_WAV , i51_MMI_DEC_MEM_BASED , 0 , i51AdeMmiPlayQuietStop) )
	{
		return ;
	}
	i51AdeMmiMediaPlayerStop (QUIET_WAV) ;
	i51AdeMmiMediaPlayerRelease () ;
	isFirstRecord = iTRUE ;
	
}
iS32 i51AdeMmiRecorderStart ( void* buffer , iU32 bufflen , void (*callback ) ( void* stream , iU32 length ) ) 
{


	//	return : -1 失败, 0 arm ，1 pcm
	iU32 ret = 0 ;
	iU32 mode , volume ;
	iU32 timeid = 0;

	HAUDIOCODEC hAudioCodec = 0;
	HAUDIODEV hDevice = 0;

	hAudioCodec = hDSPCodec;
	hDevice = hMP4RECORDVB;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStart , S , %x,%d,%x",buffer, bufflen, callback);
	#endif	


	i51AdeMmiPlayQuiet () ;
	#ifndef SPRD_6530
	MMIAPISET_GetDeviceMode(&mode);
	AUDIO_GetVolume ( &volume ) ;
	MMIAPISET_StopAllVibrator(); 
	MMIAPISET_StopAllRing(TRUE);
	#endif

	//	屏蔽按键音，触屏音
	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iFALSE);
	#elif defined SPRD_6530
	{
	MMIDEFAULT_EnableKeyRing("i51",iFALSE);
	MMIDEFAULT_EnableTpRing ("i51",iFALSE) ;
	}
	#else
	MMIDEFAULT_EnableKeyRing(iFALSE);
	MMIDEFAULT_EnableTpRing (iFALSE) ;
	#endif

	MMIAPISET_StopAllRing (iFALSE) ;
	MMIAPISET_InitAudioDevice(7);



	if ( i51MmiRecoder.recodering ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStart , already recorder , F");
	    return -1 ;
	}

	memset ( &i51MmiRecoder , 0 , sizeof(i51MmiRecoder) ) ;

	i51MmiRecoder.RecordHandle = AUDIO_CreateNormalHandle(hAudioCodec,iNULL, hDevice,iNULL, i51AdeMmiRecorderNotifyCallback);
	if ( i51MmiRecoder.RecordHandle == 0 ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStart , create recorder handle fail , %d" , i51MmiRecoder.RecordHandle);
	return -1 ;
	}

	i51MmiRecoder.buffer = buffer ;
	i51MmiRecoder.bufflen = bufflen ;
	i51MmiRecoder.callback = callback ;
	i51MmiRecoder.recodering = iTRUE ;
	i51MmiRecoder.IsPcm = 1;

	i51MmiRecoder.ReadPcmLen = 0;

	i51MmiRecoder.SendPcmTimer = i51AdeOsTimerCreate(1,1);
	if(i51MmiRecoder.SendPcmTimer>0)
	{
	i51AdeOsTimerStart(i51MmiRecoder.SendPcmTimer,timerCallback,10);
	}

	i51MmiRecoder.RecordBuffPoint = i51MmiRecoder.buffer;

	if(AUDIO_NO_ERROR!=i51AUD_EnableVoiceRecord(iTRUE,AUD_REC_PCM,i51RecordCallBack))
	{
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStart , F");
	    return -1 ;
	}

	#ifdef i51ADAPTER_DEBUG
	    i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStart , E");
	#endif

	return 1 ;
}

static void timerCallback_AmrFileWrite(iU32 id)
{
    if (0 < i51MmiRecoder.ReadPcmLen)
    {
	i51AdeOsFileWrite(i51MmiAmrRecordFile, i51MmiRecoder.buffer, i51MmiRecoder.ReadPcmLen, iNULL);

	i51MmiRecoder.ReadPcmLen = 0;
       i51MmiRecoder.RecordBuffPoint = i51MmiRecoder.buffer;
    }

     return;
}

static void i51RecordCallback_AmrFile(iU16 i_len)
{
    //	author : steven
	//	since : 2012.07.21
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

    //AMR文件录音回调，传入录音数据的长度i_len
    
    iU16 ReadLen = i_len;
    iU16 i = 0;
    uint16 samr_temp[40] = {0};

    if(i51MmiRecoder.ReadPcmLen >= i51MmiRecoder.bufflen)
    {
        return;
    }
    
    if((i51MmiRecoder.ReadPcmLen+i_len*2)>i51MmiRecoder.bufflen)
    {
        ReadLen = (i51MmiRecoder.bufflen - i51MmiRecoder.ReadPcmLen)/2;
    }
    else
        ReadLen = i_len;
    
    i51AUD_ReadVoiceData((uint16 *)samr_temp, ReadLen);
    
    i51MmiRecoder.ReadPcmLen = i51MmiRecoder.ReadPcmLen + ReadLen;
    
     for (i=0; i<i_len; i++)
    {
        *(i51MmiRecoder.RecordBuffPoint+i) = (uint8)(samr_temp[i]);
    }
    i51MmiRecoder.RecordBuffPoint = i51MmiRecoder.RecordBuffPoint + ReadLen;
}

//iS32 i51AdeMmiMediaRecorderStartEx(void *buff,iU32 bufflen,iU32 media_tpe,iBOOL needHeader,void (*callback)(void *stream,iU32 length))
iBOOL i51AdeMmiRecorderStartEx(iU16 *filename,void (*callback)( iBOOL finished ))
{
  //	return : iFALSE 失败,  iTRUE 成功
#if defined(SPRD_6800) || defined(SPRD_6530)
	#define I51_RECODER_NEEDBUFFER_SIZE       (80*150*4)
    #define i51_AMR_FRAM_LEN 32
    
	HAUDIOCODEC hAudioCodec = 0;
	const uint16* pusCodecName = PNULL;
	//RECORD_PARAM_T record_param = {AUD_REC_ADPCM, RECORD_OUTPUT_SAVE_TO_FILE, 0, ADPCM_FRAME_LENGTH, 0,0,0};
    RECORD_PARAM_T record_param = {AUD_REC_ADPCM, RECORD_OUTPUT_SAVE_TO_FILE, 0, 0, 0,0,0};
    RECORD_IF_FROM_NET_T record_from_net = {0};
	uint32          pram_result = 0;
	HAUDIODEV hDevice = 0;
	const uint16* pusDevName = PNULL;
	iU32 ret = 0 ;
	iU16 path[64] = {0} ;   
    AUDIO_RESULT_E  aud_result  = AUDIO_ERROR;

    i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , S");
    if ( i51MmiRecoder.recodering ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , already recorder , F");
		return iFALSE ;
	}

    hAudioCodec = hRecordCodec;
   	hDevice = hRECORDVB;

#if 1
   {
	   iU32 mode , volume ;
       #if defined(SPRD_6800)
       MMIAUDIO_SetMute(FALSE);
       #endif
       MMIAPISET_GetDeviceMode(&mode);
       AUDIO_GetVolume ( &volume ) ;
       #if defined(SPRD_6800)
       MMIAUDIO_SetVolume(mode, (uint32)volume);
       #endif
	 MMIAPISET_StopAllVibrator(); 
       MMIAPISET_StopAllRing(TRUE);
	}

	 //	屏蔽按键音，触屏音
	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iFALSE);
    #elif defined ( SPRD_6530 )
	MMIDEFAULT_EnableKeyRing( "i51", iFALSE);
	MMIDEFAULT_EnableTpRing ( "i51" , iFALSE) ;
	#else
       MMIDEFAULT_EnableKeyRing(iFALSE);
	MMIDEFAULT_EnableTpRing (iFALSE) ;
	#endif
#endif
    memset ( &i51MmiRecoder , 0 , sizeof(i51MmiRecoder) ) ;

    i51MmiRecoder.RecordeNeedbuffer = (iU32*)i51AdeOsMalloc(I51_RECODER_NEEDBUFFER_SIZE,
													__FILE__, __LINE__) ;

    if( i51MmiRecoder.RecordeNeedbuffer == 0 ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , F");
		return iFALSE ;
	}

        record_param.format = AUD_REC_AMR;
        record_param.frame_length = i51_AMR_FRAM_LEN;

    record_from_net.b_if_from_net = FALSE;

    #if 1
    i51AdeStdWStrcpy (i51MmiRecoder.name , filename) ;
	i51AdeMmiGetFilePath ( path , i51MmiRecoder.name) ;
    i51MmiRecoder.RecordHandle = AUDIO_CreateRecordFileHandle(
                                hAudioCodec,
                                pusCodecName,
                                hDevice,
                                pusDevName,
                                path,
                                i51MmiRecoder.RecordeNeedbuffer,
                                I51_RECODER_NEEDBUFFER_SIZE,
                                i51AdeMmiRecorderNotifyCallback
                                );
    #else
    
    i51AdeStdWStrcpy (i51MmiRecoder.name , L"recoder.dat") ;
	i51AdeMmiGetFilePath ( path , i51MmiRecoder.name  ) ;
    
    i51MmiRecoder.FileHandle = i51AdeOsFileOpen (i51MmiRecoder.name,i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE, __FILE__,__LINE__) ;
    if ( i51MmiRecoder.FileHandle == 0 ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , create file fail , F");
		return -1 ;
	}
    
    i51MmiRecoder.RecordHandle =  AUDIO_CreateRecordFileHandle_FileCreated(
                                hAudioCodec,
                                pusCodecName,
                                hDevice,
                                pusDevName,
                                path,
                                 i51MmiRecoder.FileHandle ,
                                 i51MmiRecoder.RecordeNeedbuffer,
                                I51_RECODER_NEEDBUFFER_SIZE,
                                i51AdeMmiRecorderNotifyCallback
                                );	

    #endif
    if (INVALID_HANDLE != i51MmiRecoder.RecordHandle)
    {  
    #if 1
        AUDIO_ExeCodecExtOpe(
            i51MmiRecoder.RecordHandle,
            ANSI2UINT16("SET_RECORDPARAM"),
            RECORD_PARAM_SET_COUNT,
            &record_param,
            &pram_result
            );
    

        AUDIO_ExeCodecExtOpe(
            i51MmiRecoder.RecordHandle,
            ANSI2UINT16("SET_RECORDIFNET"),
            RECORD_SET_IF_FROM_NET_COUNT,
            &record_from_net,
            &pram_result
            );
    #endif
        aud_result = AUDIO_Play(i51MmiRecoder.RecordHandle, 0);
        
        if(AUDIO_NO_ERROR != aud_result)
        {
            AUDIO_CloseHandle(i51MmiRecoder.RecordHandle);
            i51MmiRecoder.RecordHandle = PNULL;
            i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , AUDIO_Play fail , F");
            return iFALSE ;
        }
    }
	//i51MmiRecoder.buffer = buff ;
	//i51MmiRecoder.bufflen = bufflen ;
	i51MmiRecoder.callback = callback ;
	i51MmiRecoder.recodering = iTRUE ;

    i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , E");
	
	return iTRUE ;

#else // defined(SPRD_6600) || defined(SPRD_6620)
    iU32 ret = 0 ;
    iU32 mode , volume ;
    iU32 timeid = 0;
    
    HAUDIOCODEC hAudioCodec = 0;
    HAUDIODEV hDevice = 0;
    const uint8 AMR_HEADER[6] = {'#', '!', 'A', 'M', 'R', '\n'};
    iU32 bufflen = 1024*8; //i51MmiAmrRecordFileBuff length, for amr record file only

    hAudioCodec = hDSPCodec;
    hDevice = hMP4RECORDVB;
    
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , S , %x,%x", filename , callback);
	#endif	

    #ifndef SPRD_6530
	
       MMIAPISET_GetDeviceMode(&mode);
       AUDIO_GetVolume ( &volume ) ;
	MMIAPISET_StopAllVibrator(); 
       MMIAPISET_StopAllRing(TRUE);
    #endif

	//	屏蔽按键音，触屏音
	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iFALSE);
	#elif defined SPRD_6530
	{
		MMIDEFAULT_EnableKeyRing("i51",iFALSE);
		MMIDEFAULT_EnableTpRing ("i51",iFALSE) ;
	}
	#else
       MMIDEFAULT_EnableKeyRing(iFALSE);
	MMIDEFAULT_EnableTpRing (iFALSE) ;
	#endif
    MMIAPISET_InitAudioDevice(7);
    
    if ( i51MmiRecoder.recodering )
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , already recorder , F");
		return iFALSE;
    }
    if (iNULL != i51MmiAmrRecordFileBuff)
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , i51MmiAmrRecordFileBuff already used , F");
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , i51MmiAmrRecordFileBuff free , result: %d", i51AdeOsFree(i51MmiAmrRecordFileBuff)) ;
        i51MmiAmrRecordFileBuff = iNULL;
	    return iFALSE;
    }
    if (0 != i51MmiAmrRecordFile)
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , i51MmiAmrRecordFile already used , F");
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , i51MmiAmrRecordFile close , result: %d", i51AdeOsFileClose(i51MmiAmrRecordFile));
        i51MmiAmrRecordFile = 0;
    	return iFALSE;
    }

    //record buffer
    i51MmiAmrRecordFileBuff = (iCHAR *)i51AdeOsMalloc ( bufflen, __FILE__ , __LINE__ ) ;
    //record file
    i51MmiAmrRecordFile = i51AdeOsFileOpen (filename, i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
    i51AdeOsFileWrite(i51MmiAmrRecordFile, AMR_HEADER,  6, iNULL); //add amr file head "#!AMR\n"

    memset ( &i51MmiRecoder , 0 , sizeof(i51MmiRecoder) ) ;
    
    i51MmiRecoder.RecordHandle = AUDIO_CreateNormalHandle(hAudioCodec,iNULL, hDevice,iNULL, i51AdeMmiRecorderNotifyCallback);
    if ( i51MmiRecoder.RecordHandle == 0 ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , create recorder handle fail , %d", i51MmiRecoder.RecordHandle);
		return iFALSE;
	}
    
	i51MmiRecoder.buffer = i51MmiAmrRecordFileBuff ;
	i51MmiRecoder.bufflen = bufflen ;
	i51MmiRecoder.callback = callback ;
	i51MmiRecoder.recodering = iTRUE ;
    i51MmiRecoder.IsPcm = 1;

    i51MmiRecoder.ReadPcmLen = 0;

    i51MmiRecoder.SendPcmTimer = i51AdeOsTimerCreate(1,1);
    if(i51MmiRecoder.SendPcmTimer>0)
    {
        i51AdeOsTimerStart(i51MmiRecoder.SendPcmTimer,timerCallback_AmrFileWrite,10);
    }

    i51MmiRecoder.RecordBuffPoint = i51MmiRecoder.buffer;
    
    if(AUDIO_NO_ERROR!=i51AUD_EnableVoiceRecord(iTRUE,AUD_REC_AMR,i51RecordCallback_AmrFile))
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , F");
		return iFALSE;
    }

	#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStartEx , E");
	#endif
	
	return iTRUE;

#endif
	
}

#if 1
static void RecordStopCallback(iU32 id)
{
    if(RecordBack!=NULL)
        RecordBack(RecordStopResult);
    RecordBack = NULL;
    i51AdeOsTimerStop(id);
}

iBOOL i51AdeMmiRecorderStop ( void (*callback) ( iBOOL results ) ) 
{
    iU32 ret = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , S , %x", callback);
	#endif	

	if (! i51MmiRecoder.recodering ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , haven't Recoder , F");
		 return iFALSE ;
	}
    
    if(i51MmiRecoder.IsPcm)
    {
        {
            ret=i51AUD_EnableVoiceRecord(iFALSE,AUD_REC_PCM,iNULL);
        }
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , %d", ret);
    }
    else
    {
        ret = AUDIO_Stop ( i51MmiRecoder.RecordHandle) ;
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , stop , %d", ret);
        
        ret = AUDIO_CloseHandle(i51MmiRecoder.RecordHandle);
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , colse , %d", ret);
    }
    if(i51MmiRecoder.SendPcmTimer!=0)
    {
        i51AdeOsTimerStop(i51MmiRecoder.SendPcmTimer);
        i51MmiRecoder.SendPcmTimer = 0;
    }
    
    if (  i51MmiRecoder.RecordeNeedbuffer ) {

		i51AdeOsFree( i51MmiRecoder.RecordeNeedbuffer) ;
		 i51MmiRecoder.RecordeNeedbuffer = 0 ;
	}
	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iTRUE);
    #elif defined ( SPRD_6530 )
	MMIDEFAULT_EnableKeyRing( "i51", iFALSE);
	MMIDEFAULT_EnableTpRing ( "i51" , iFALSE) ;
	#else
    MMIDEFAULT_EnableKeyRing(iTRUE);
	MMIDEFAULT_EnableTpRing (iTRUE) ;
	#endif
    
    RecordBack = callback;
    
	 if ( ret==AUDIO_NO_ERROR ) {
        RecordStopResult = 1;
	 } else {
	    RecordStopResult = 0;
	 }
	i51MmiRecoder.recodering = iFALSE ;
    i51AdeOsTimerStart(i51AdeOsTimerCreate(0,1),RecordStopCallback,5);
    
    if (0 != i51MmiAmrRecordFile)
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , i51MmiAmrRecordFile close , result: %d", i51AdeOsFileClose(i51MmiAmrRecordFile));
        i51MmiAmrRecordFile = 0;
    }

    if (iNULL != i51MmiAmrRecordFileBuff)
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , i51MmiAmrRecordFileBuff free , result: %d", i51AdeOsFree(i51MmiAmrRecordFileBuff));
        i51MmiAmrRecordFileBuff = iNULL;
    }

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , E");
	#endif	
	 
      return (ret==AUDIO_NO_ERROR) ;
	
}
#else
iBOOL i51AdeMmiRecorderStop ( void (*callback) ( iBOOL results ) ) 
{


	iU32 ret = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , S , %x", callback);
	#endif	
	
	if (! i51MmiRecoder.recodering ) {
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , haven't Recoder , F");
		 return iFALSE ;
	}

	if ( i51MmiRecoder.FileHandle ) {

		i51AdeOsFileClose(i51MmiRecoder.FileHandle) ;
		i51MmiRecoder.FileHandle = 0 ;
	}

	if ( i51MmiRecoder.timerid ) {

		i51AdeOsTimerStop(i51MmiRecoder.timerid) ;
		i51MmiRecoder.timerid = 0 ;
	}

	if (  i51MmiRecoder.RecordeNeedbuffer ) {

		i51AdeOsFree( i51MmiRecoder.RecordeNeedbuffer) ;
		 i51MmiRecoder.RecordeNeedbuffer = 0 ;
	}

//	MMIAUDIO_StopRecord () ;
//	return ;


	
       ret = AUDIO_Stop ( i51MmiRecoder.RecordHandle) ;
       i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , stop , %d", ret);
	 i51MmiRecoder.recodering = iFALSE ;
       ret = AUDIO_CloseHandle(i51MmiRecoder.RecordHandle);
       i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , colse , %d", ret);

	#ifdef SPRD_6800
	MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, 0, iTRUE);
	#elif defined (SPRD_6530)
	MMIDEFAULT_EnableKeyRing("I51",iTRUE);
	MMIDEFAULT_EnableTpRing ("I51",iTRUE) ;
	#else
       MMIDEFAULT_EnableKeyRing(iTRUE);
	MMIDEFAULT_EnableTpRing (iTRUE) ;
	#endif
	 
	 if ( ret==AUDIO_NO_ERROR ) {

		if (callback)
			callback (iTRUE) ;
	 } else {
		if (callback)
			callback (iFALSE) ;
	 }
	
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiRecorderStop , E");
	#endif	

      RecordLen = 0;
	return (ret==AUDIO_NO_ERROR) ;
	
}
#endif

void i51AdeMmiRecorderFormat ( iU32* samples , iU32* bitscale , iU32* chanels )
{
	*samples = 8*1024 ;
	*bitscale = 16 ;
	*chanels = 1 ;
		
}

typedef void (*AUDIOCREATECALLBACK)(void *callback);
typedef struct RECORDPLAYINFO
{
    //add by steven
    AUDIOCREATECALLBACK RecordPlayCallback;
    iU8 IsRecordPlay;
}PlayRecord;
static PlayRecord i51RecordPlay = {0};
static void *ReocrdHand;

static void i51AdeMmiRecordPlayCallback(void* handle)
{
    if(NULL!=i51RecordPlay.RecordPlayCallback)
    {
        i51RecordPlay.RecordPlayCallback(1);
        i51RecordPlay.RecordPlayCallback = NULL;
    }
}

iBOOL i51AdeMmiRecoderPlay ( void* stream , iU32 length , void (*callback) ( iBOOL results ) ) 
{
    i51RecordPlay.RecordPlayCallback = callback;
    ReocrdHand = stream;
    if(i51AdeMmiMediaPlayerPlay (0 ,stream ,length , i51_MEDIA_TYPE_WAV ,i51_MMI_DEC_MEM_BASED ,0 ,i51AdeMmiRecordPlayCallback))
    {
        return 1;
    }
    return 0;
}

iBOOL i51AdeMmiRecorderPlayStop (void) 
{
       //	author : steven
	//	since : 2011.12.21
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes :停止播放录音流， 成功返回> 0 ，失败0
	return i51AdeMmiMediaPlayerStop(ReocrdHand);
}

iBOOL i51AdeMmiGetScreenScale ( iU16* width , iU16* height ) 
{

 	//	author : steven
	//	since : 2011.12.21
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 成功返回> 0 ，失败0

      #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiGetScreenScale , S");
      #endif

	*width = LCD_WIDTH ;
	*height = LCD_HEIGHT ;

     #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiGetScreenScale , E , width:%d,height:%d",width,height);
     #endif
      
	return 1 ; 
	
}


iU32* i51AdeMmiGetPanel (void) 
{

    //	author : Jelo
    //	since : 2011.4.20
    // 	(C) 2011 , PKIG Tech. Co., Ltd.

    //	notes : 返回层数组，其中存放了层的buffer 地址、宽、高
    //	array[0] 存放了层个数，数组长度= 层的个数* 3 + 1
    //	array[1...n] 存放了层的buffer以及宽高
    //	如：array[1] 第0层的屏幕指针、array[2] 第0层的宽、array[3]第0层的高
    //	array[4] 为第1层地屏幕指针、array[5]、array[6] 同上。第0层为物理显存、其余为虚拟层。
    //	MTK 的实现可以直接用系统的层资源。

    static iU32 array [4] = { 0} ;	
    GUI_LCD_DEV_INFO lcd = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN} ;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiGetPanel , S");
    #endif
    
    array[0] = 1 ; 
    #if defined (SPRD_6800) || defined(SPRD_6530)
    array[1] = (iU32 )GUIBLOCK_GetBuf((GUI_LCD_DEV_INFO *)&lcd) ;
    #else
    array[1] = (iU32 )GUIBLOCK_GetBlockBuffer((GUI_LCD_DEV_INFO *)&lcd) ;
    #endif
    array[2] = LCD_WIDTH ;
    array[3] = LCD_HEIGHT;
    //MMK_PARSEWIN_CODE_E

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiGetPanel , E , array[0]:%d,array[1]:%d,array[2]:%d,array[3]:%d",array[0],array[1],array[2],array[3]);
    #endif	
    
    return (iU32* ) array ;
}

void i51AdeMmiGetTextAttr ( iU16* text , iU16* textwidth , iU16* textheight ) 
{
       //	author : steven
	//	since : 2011.12.28
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes :获取字符串的宽高
	
        iU16 width = 0;
        iU16 high = 0;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiGetTextAttr , S , text:%x",text);
	#endif
    
        width = GUI_GetStringWidth(i51FontType, text,i51AdeStdWStrlen(text));
        high = GUI_GetStringHeight(i51FontType, text,i51AdeStdWStrlen(text));
        *textwidth = width;
        *textheight = high;
     

    #ifdef i51ADAPTER_DEBUG1

        i51AdeOsLog(0, "MMI : i51AdeMmiGetTextAttr , E");
	#endif
}

void i51AdeMmiDrawText ( iU8* panel , iU16 width , iU16 height , const iU16* text , iS32 desx , iS32 desy , iCOLOR color ) 
{
       //	author : steven
	//	since : 2011.12.28
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes :绘制文字

#if defined (SPRD_6620) || defined (SPRD_6600)
	 GUI_LCD_DEV_INFO lcd = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN} ;
	 GUI_POINT_T startpoint = {0} ;
	 iU32 len = i51AdeStdWStrlen(text);
	 startpoint.x = desx ; 
	 startpoint.y = desy ;
	LCD_DisplayString ( &lcd , startpoint , text, (iU16)len, 1 , color , i51FontType  , 2 ) ;
#elif defined (SPRD_6800) || defined (SPRD_6530)
        GUI_RECT_T rect= {0};
        GUISTR_BUF_T des_buf_data = {0};
        GUISTR_STYLE_T str_style = {0};
        MMI_STRING_T   string = {0};
        GUI_RECT_T  box = {0};
        iU32 *lcdtt = (iU32 *)panel;
        iU32 len = i51AdeStdWStrlen(text);
	 GUI_LCD_DEV_INFO lcd = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN} ;
	 GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;

    #ifdef i51ADAPTER_DEBUG
     i51AdeOsLog(0, "MMI : i51AdeMmiDrawText , S , anel:%s,width:%d,heght:%d,text:%x,desx:%d,desy:%d,color:%d",panel ,width,height,text,desx,desy,color);
	#endif
    
        rect.bottom =(iS16)desy+height -1  ;
        rect.right = (iS16)desx+width - 1;
        rect.left = (iS16)desx;
        rect.top = (iS16)desy;

        str_style.font       = i51FontType;
        str_style.font_color = color;
        str_style.char_space = 0;
        str_style.line_space = 0;

        string.wstr_ptr = text;
        string.wstr_len = len;

        des_buf_data.buffer = (GUI_COLOR_T *)lcdtt[1];
        des_buf_data.width = (iU16)lcdtt[2];
        des_buf_data.height = (iU16)lcdtt[3];

//        box = GUISTR_DrawTextInRect(&des_buf_data,&rect, &rect, &string,&str_style, 0, GUISTR_TEXT_DIR_AUTO);


	GUISTR_DrawTextToLCDInRect(&lcd , 
		&rect , 
		&rect , 
		&string ,
		&str_style, 
		str_state ,
		GUISTR_TEXT_DIR_AUTO) ; 

	
        //i51AdeMmiUpdateScreen();

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "MMI : i51AdeMmiDrawText , E");
	#endif
#endif
}


void i51AdeMmiSetPanelUsed ( void* panel )
{
    
}
static void i51AdeCreateCB ( iU32 id )
{
	i51AdeOsTimerStop ( i51AdeCreateTimer ) ;
	i51AdeCreateTimer = 0 ;
	if ( i51AdeCallback ) {
		
		i51AdeCallback (1) ;
	}
}

iBOOL i51AdeMmiCreate ( i51ADECALLBACK callback ) 
{	
	//callback ( 1 ) ;
	#ifdef i51ADAPTER_DEBUG
     i51AdeOsLog(0, "MMI : i51AdeMmiCreate , S");
    #endif

	i51AdeCallback = callback ;
       i51AdeCreateTimer = i51AdeOsTimerCreate (0,1) ;
	if ( i51AdeCreateTimer == 0 ) {

		return iFALSE ;
	}
       i51AdeOsTimerStart(i51AdeCreateTimer,i51AdeCreateCB,10);

	   
	#ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "MMI : i51AdeMmiCreate , E");
    #endif

      return iTRUE;
}
iBOOL i51AdeMmiRelease ( void )
{
     return iTRUE;
}




