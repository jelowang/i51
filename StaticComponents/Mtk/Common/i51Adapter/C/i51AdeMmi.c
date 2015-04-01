  
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Api.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#include "i51Scripts.h"
#include "i51.h"
#include "i51AdeCnf.h"
#include "i51AdeMmi.h"
#include "i51AdeOs.h"
#include "mmi_include.h"
#include "kal_release.h"  //basic data type: mutex, tick
#include "mdi_datatype.h"
#include "gdi_include.h"
#include "mdi_audio.h"
#include "l1audio.h"
#include "gui_data_types.h"

//	文件长度
#define I51_ADE_MMI_FILENAME_LEN	64

static gdi_handle NewLayer ;
static gdi_handle BaseLayer ;
static U8 *NewLayerBuffer = iNULL ;
static U8 *BaseLayerBuffer = iNULL ;

//画板信息
static iU32 PanelInfo[8] ;

static iBOOL BaseLayerUsed ;
static iBOOL NewLayerUsed ;

static iU32 l_volumeValue = 4;
static i51RECCALLBACK i51AppRecordCB ;
static i51ADECALLBACK i51AppRecordFileCB;
static void (*i51AppRecorderStopCB) ( iBOOL results );
static void (*i51AppRecoderPlayCB) ( iBOOL results );
static BOOL  IsRecording;
static i51ADECALLBACK i51AdaCreateCB ;

extern iS32 UI_device_height ;
extern iS32 UI_device_width ;

typedef enum
{
    AUDIO_STOP,
    AUDIO_PLAYING,
    AUDIO_PAUSE
}AUDIO_STATE_ENUM;

typedef struct
{
    iBOOL using;
    iS32  aud_hd;
    iU32  priority;  
    void* buffer; 
    void (*STOP)(iHANDLE handle);
    iU32  start_time;
    AUDIO_STATE_ENUM state;
    iBOOL   loop_flag;
}I51AUDIOCHANNEL;

#define MIDI_CHL_START 0
#define MIDI_CHL_END   3
#define WAV_CHL_START  4
#define WAV_CHL_END    5
#define AUDIO_CHL_MAX  6

static I51AUDIOCHANNEL  l_AudChls[AUDIO_CHL_MAX];
static iS32  l_QuietWavHd;
static iBOOL l_MediaPlayerCreated;
static void (*i51AdeMmiMediaCreatCBFnPtr) ( iBOOL results );

__align(4) const U8 QUIET_WAV[] = {
0X52,0X49,0X46,0X46,0X18,0X02,0X00,0X00,0X57,0X41,0X56,0X45,0X66,0X6D,0X74,0X20,
0X10,0X00,0X00,0X00,0X01,0X00,0X01,0X00,0XE8,0X03,0X00,0X00,0XE8,0X03,0X00,0X00,
0X01,0X00,0X08,0X00,0X64,0X61,0X74,0X61,0XF4,0X01,0X00,0X00,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,
0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80,0X80
};

static iU8 i51AudioConvetFormat ( i51METYPE format)
{

	//	author : william
	//	since : 20110801
	//	PKIG Tech. Co., Ltd.
	
	switch ( format  ) {

		case i51_MEDIA_TYPE_MIDI :
			return MDI_FORMAT_SMF ;
		case i51_MEDIA_TYPE_WAV :
			return MDI_FORMAT_WAV ;
        case i51_MEDIA_TYPE_AMR :
            return MDI_FORMAT_AMR ;
        case i51_MEDIA_TYPE_MP3 :
            return MDI_FORMAT_DAF ;
	
	}
	
} 

static iU8 i51AudioConvetLoop ( iBOOL loop)
{

	//	author : william
	//	since : 20110801
	//	PKIG Tech. Co., Ltd.
	
	switch ( loop  ) {

		case 0 :
			return 1 ;
		case 1 :
			//循环播放
			return 0 ;
	}
} 

static iU8 i51AudioConvetLoop_SinglePlay(iBOOL loop)
{
    
  	switch ( loop  ) {

		case 0 :
			return DEVICE_AUDIO_PLAY_ONCE ;
		case 1 :
			//循环播放
			return DEVICE_AUDIO_PLAY_INFINITE ;
	}  

}

static void i51AdeMmi_i51ADECALLBACK_TIMERCB ( iU32 id )
{

	i51AdeOsTimerStop(id) ;
	i51AdaCreateCB (iTRUE) ;
}
static iBOOL i51AdeMmi_ASYN_i51ADECALLBACK ()
{

	//	i51SlDlCALLBACK 回调，异步通知

	iU32 timerid = 0 ;
	timerid = i51AdeOsTimerCreate ( 0 , 1 ) ;
	if ( 0 == timerid )
	{
		i51AdeOsLog ( 0 , "MMI : i51AdeMmi_ASYN_i51ADECALLBACK , TC " ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart (timerid , i51AdeMmi_i51ADECALLBACK_TIMERCB , 1 ) )
	{
		i51AdeOsLog ( 0 , "MMI : i51AdeMmi_ASYN_i51ADECALLBACK , TS F" ) ;		
		return iFALSE ;
	}		
}

static void i51AdeMmi_i51MEDIACALLBACK_TIMERCB ( iU32 id )
{

	iU32 i = 0 ;
	i51AdeOsTimerStop(id) ;
}
static iBOOL i51AdeMmi_ASYN_i51MEDIACALLBACK (  iU32 player )
{

	//	i51MEDIACALLBACK 回调，异步通知

}

iBOOL i51AdeMmiGetScreenScale ( iU16* width , iU16* height )
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "MMI : i51AdeMmiGetScreenScale , S , %x , %x" , width , height ) ;
	#endif	

	*width = UI_device_width ;
	*height = UI_device_height ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "MMI : i51AdeMmiGetScreenScale , E , ret = %x" , iTRUE ) ;
	#endif	
	
	return iTRUE ;

}    

void i51AdeMmiSetPanelUsed ( void* panlAddress )
{

	//	author : william
	//	since : 20110816
	//	PKIG Tech. Co., Ltd. 

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "MMI : i51AdeMmiSetPanelUsed , S , %x" , panlAddress ) ;
	#endif	

	if ( (iU32)panlAddress == (iU32)BaseLayerBuffer ) {

		BaseLayerUsed = iTRUE ;
 
	} else if ( (iU32)panlAddress == (iU32)NewLayerBuffer ) {

		NewLayerUsed = iTRUE ;

	} 

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "MMI : i51AdeMmiSetPanelUsed , E") ;
	#endif	  

}

void i51AdeMmiUpdateScreen ( void )
{



	//	author : william
	//	since : 20110816
	//	PKIG Tech. Co., Ltd. 

	//	Jelo Edit Start 2011.11.22
	//#ifdef i51ADAPTER_DEBUG
	//i51AdeOsLog ( 0 , "i51AdeMmiUpdateScreen S" ) ;
	//#endif	
	//	Jelo Edit Finished
	
	#ifdef i51PROFILINGS
		i51KernelDrawProfiles () ;
	#endif

	gdi_layer_blt_previous ( 0 , 0 , UI_device_width -1 ,UI_device_height - 1 ) ;

	//	Jelo Edit Start 2011.11.22
	//#ifdef i51ADAPTER_DEBUG
	//i51AdeOsLog ( 0 , "i51AdeMmiUpdateScreen E" ) ;
	//#endif	
	//	Jelo Edit Finished

}

void i51AdeMmiUpdateScreenEx ( iRECT * irect , iU32 length ) 
{
    int i;

    for( i=0; i<length; i++ )
    {
        gdi_layer_blt_previous( irect[i].X, irect[i].Y,  irect[i].X + irect[i].Width,  irect[i].Y + irect[i].Height );
    }
}

iU32* i51AdeMmiGetPanel ()
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "MMI : i51AdeMmiGetPanel , S" ) ;
	#endif	
	
	gdi_layer_get_base_handle( &BaseLayer ) ;
	gdi_layer_get_buffer_ptr( &BaseLayerBuffer) ;
	
	PanelInfo[0] = 1 ;
	PanelInfo[1] = ( iU32 ) BaseLayerBuffer ;
	PanelInfo[2] = UI_device_width ;
	PanelInfo[3] = UI_device_height ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "MMI : i51AdeMmiGetPanel , E , ret = %x" , PanelInfo) ;
	#endif	
	return PanelInfo ;
	
}

static iBOOL I51_PrevMemModeMixSnd = iFALSE;
static iBOOL i51_AdeMmiCurrSndmodeIsMix( iU32 type , iU32 dec )
{
  return !( dec == i51_MMI_DEC_FILE_BASED || type == i51_MEDIA_TYPE_MP3 || type == i51_MEDIA_TYPE_AMR );
}

mdi_handle i51_mdi_audio_mma_open_string(void *audio_data, U32 len, U8 mdi_format, U8 repeats, mdi_mma_callback callback)
{
	#if( I51_MTK_VS >=VS_11A_1112 )
        mdi_handle ret;              
		ret =  mdi_audio_mma_open_string(0,audio_data,len,mdi_format,repeats,callback,NULL);
        return ret;
	#else
        mdi_handle ret;              
		ret =  mdi_audio_mma_open_string(audio_data,len,mdi_format,repeats,callback);
        return ret;
	#endif
}
mdi_handle i51_mdi_audio_mma_open_file(void *file_name, U8 repeats, mdi_mma_callback callback)
{
    #if( I51_MTK_VS<=VS_0812 )
        return mdi_audio_mma_open_file ( file_name , repeats ,callback ) ;
    #elif( (I51_MTK_VS>=VS_08B)&&( I51_MTK_VS<=VS_W11 ) )
    	return mdi_audio_mma_open_file ( file_name , repeats ,0, callback ) ;
    #elif( I51_MTK_VS>=VS_11A_1112 )
    	return mdi_audio_mma_open_file ( 0, file_name , repeats ,0, callback , NULL ) ;
    #endif   
}

void i51AdeMmiMediaCreatCB(void)
{
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.
    if(i51AdeMmiMediaCreatCBFnPtr)
        i51AdeMmiMediaCreatCBFnPtr(1);
}

#if( I51_MTK_VS>=VS_11A_1112 )
void i51AdeMmiMediaPlayCB(kal_int32 handle, kal_int32 result, void* user_data)
#else
void i51AdeMmiMediaPlayCB( kal_int32 handle, kal_int32 result )
#endif
{
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.
    
    int i;
    for( i=0; i<AUDIO_CHL_MAX; i++ )
    {
        if( l_AudChls[i].aud_hd == handle && l_AudChls[i].using )
        {
            i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayCB ");
            if((l_AudChls[i].loop_flag == 1 )&& (result != -9006) )
            {
                mdi_audio_mma_play(l_AudChls[i].aud_hd);
            }
            else
            {
                l_AudChls[i].using = iFALSE;
                if( l_AudChls[i].STOP && result != -9006 )
                {
                    l_AudChls[i].STOP(l_AudChls[i].buffer);
                }
                if( l_AudChls[i].aud_hd && result != -9006 )
                { 
                    mdi_audio_mma_stop( l_AudChls[i].aud_hd );
                    mdi_audio_mma_close( l_AudChls[i].aud_hd );
                }        
            }
            break;
        }
    }

}

#if( I51_MTK_VS>=VS_11A_1112 )
void i51AdeMmiMediaSinglePlayCB( mdi_result result, void* user_data)
#else
void i51AdeMmiMediaSinglePlayCB( mdi_result result )
#endif
{
    
    if(l_AudChls[0].using)
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaSinglePlayCB!");

        l_AudChls[0].using = iFALSE;
        if( l_AudChls[0].STOP && result != -9006 )
        {
            l_AudChls[0].STOP(l_AudChls[0].buffer);
        }
        if( (l_AudChls[0].aud_hd == 0) && (result != -9006 ))
            mdi_audio_stop_file();
        else if((l_AudChls[0].aud_hd != 0) && (result != -9006 ))
            mdi_audio_stop_string();
    }

}

iBOOL i51AdeMmiMediaPlayerCreate ( void (*callback) ( iBOOL results ) )
{ 
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.
    mdi_result ret;
    if(l_MediaPlayerCreated) return iTRUE; 

    if(l_QuietWavHd==NULL)
        l_QuietWavHd = i51_mdi_audio_mma_open_string( (void*)(QUIET_WAV), sizeof(QUIET_WAV), MDI_FORMAT_WAV, 0, NULL );
    ret = mdi_audio_mma_play(l_QuietWavHd);
    I51_PrevMemModeMixSnd = iTRUE;
    i51AdeMmiMediaCreatCBFnPtr = callback;
    gui_start_timer(1,i51AdeMmiMediaCreatCB);
    l_MediaPlayerCreated = 1;
    return iTRUE ;  
}
iBOOL i51AdeMmiMediaPlayerRelease () 
{ 
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.
    int i;
    if(!l_MediaPlayerCreated) return iTRUE;
    i51AdeMmiMediaCreatCBFnPtr = iNULL;

    if(!I51_PrevMemModeMixSnd)
    {
        if( l_AudChls[0].using ){
            if(l_AudChls[0].aud_hd==0)
            {
                i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerRelease , stop file !");
                mdi_audio_stop_file();
            }
            else
            {
                i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerRelease , stop Mem !");
                mdi_audio_stop_string();
            }
        }
    }
    else{
        for( i=0; i<AUDIO_CHL_MAX; i++ )
        {
            if( l_AudChls[i].using )
            {
                l_AudChls[i].using = iFALSE;
                l_AudChls[i].buffer = 0;			
                mdi_audio_mma_stop( l_AudChls[i].aud_hd );
                mdi_audio_mma_close( l_AudChls[i].aud_hd );
                l_AudChls[i].aud_hd=0;
            }
        }
    }
	if(l_QuietWavHd){
		mdi_audio_mma_stop(l_QuietWavHd);
		mdi_audio_mma_close(l_QuietWavHd);
		l_QuietWavHd = 0;
	}
    l_MediaPlayerCreated = 0;
    i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerRelease , E !");
    return iTRUE ; 
}
iBOOL i51AdeMmiMediaPlayerPlay ( iU32 priority , void* buffer , iU32 length , iU32 type , iU32 dec , iBOOL loop , void (*STOP)(iHANDLE handle)  ) 
{
    //	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.

    int i ,search_start, search_end, find_chnl;
    iU16 FilePath[64] = {0} ;
    mdi_result ret;
    if( !buffer ) return iFALSE;
    if( type !=i51_MEDIA_TYPE_MIDI && type!= i51_MEDIA_TYPE_WAV && type !=i51_MEDIA_TYPE_MP3  && type !=i51_MEDIA_TYPE_AMR )
        return iFALSE;
    
    if((I51_PrevMemModeMixSnd ==1 ) && (1 == i51_AdeMmiCurrSndmodeIsMix( type , dec )))
    //之前mode和现在要播放的type都支持混音
    {    
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , This Audio is support Mix! type= %d ,priority =%d ,loop =%d,dec =%d",type,priority,loop,dec);
        if( type == i51_MEDIA_TYPE_MIDI ){
           search_start = MIDI_CHL_START;
            search_end = MIDI_CHL_END;
        }else {
            search_start = WAV_CHL_START;
            search_end = WAV_CHL_END;
        }
        find_chnl = -1;
        for( i=search_start ; i<=search_end; i++ ){
		
           if( !l_AudChls[i].using )  break; 
           if( l_AudChls[i].buffer == buffer )
               return iTRUE;    
        }
        if( i <= search_end ){
           find_chnl = i;
        }else{
            iU32 find_priority, find_start_time;
        
            find_priority = priority;
            kal_get_time( &find_start_time );
            for( i=search_start ; i<=search_end; i++ ){
                if( !l_AudChls[i].using )  continue; 
                if( l_AudChls[i].priority > find_priority ){
                   find_chnl = i;
                   find_priority = l_AudChls[i].priority;
                   find_start_time = l_AudChls[i].start_time;
                }else if( l_AudChls[i].priority == find_priority ){
                    if( l_AudChls[i].start_time < find_start_time ){
                        find_chnl = i;
                        find_start_time = l_AudChls[i].start_time;                   
                    }
                 }
            }
        }
        if( find_chnl == -1 ) return iFALSE;
        if( l_AudChls[find_chnl].using )
        {      
            l_AudChls[find_chnl].using = iFALSE;
            if(l_AudChls[find_chnl].STOP)
                l_AudChls[find_chnl].STOP( l_AudChls[find_chnl].buffer );
            l_AudChls[find_chnl].STOP = NULL;
            mdi_audio_mma_stop( l_AudChls[find_chnl].aud_hd );
            mdi_audio_mma_close( l_AudChls[find_chnl].aud_hd );
        }

        l_AudChls[find_chnl].aud_hd = i51_mdi_audio_mma_open_string( buffer, length, i51AudioConvetFormat(type), i51AudioConvetLoop(loop), i51AdeMmiMediaPlayCB );  
        ret = mdi_audio_mma_play( l_AudChls[find_chnl].aud_hd );

        if(ret != 0) 
        {
            if(l_AudChls[find_chnl].aud_hd)
                mdi_audio_mma_close(l_AudChls[find_chnl].aud_hd);
            l_AudChls[find_chnl].aud_hd = 0 ;
            i51AdeOsLog(0, "MMI : Play Audio Fail!ret = %d,l_AudChls[find_chnl].aud_hd = %d",ret,l_AudChls[find_chnl].aud_hd);
            return iFALSE;
        }      
        I51_PrevMemModeMixSnd = iTRUE; 
        l_AudChls[find_chnl].using = 1;
        l_AudChls[find_chnl].priority = priority;  
        l_AudChls[find_chnl].buffer = buffer; 
        l_AudChls[find_chnl].STOP = STOP;
        kal_get_time( &l_AudChls[find_chnl].start_time ); 
        l_AudChls[find_chnl].state = AUDIO_PLAYING;  
        l_AudChls[find_chnl].loop_flag = loop;
                
	    return iTRUE ; 
    }
    else if((I51_PrevMemModeMixSnd ==1 ) && (0 == i51_AdeMmiCurrSndmodeIsMix( type , dec )))
    //之前播放mode支持混音，现在播放type的不支持混音，检查关闭之前的所有混音，并且在audio[0]播放
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , This Audio not support Mix! type= %d ,priority =%d ,loop =%d,dec = %d",type,priority,loop,dec);
        for(i=0;i<AUDIO_CHL_MAX;i++){
            if( !l_AudChls[i].using )  continue; 
            if( l_AudChls[i].priority < priority ){
            return iFALSE;
            }
        }
        
        for( i=0 ; i<AUDIO_CHL_MAX; i++ ){
            if(l_AudChls[i].using ){
                l_AudChls[i].using =0;
                if(l_AudChls[i].STOP)
                    l_AudChls[i].STOP( l_AudChls[i].buffer );
                l_AudChls[i].STOP = NULL;
                mdi_audio_mma_stop(l_AudChls[i].aud_hd);
                mdi_audio_mma_close(l_AudChls[i].aud_hd);
                }
        }

        if(l_QuietWavHd){
		    mdi_audio_mma_stop(l_QuietWavHd);
		    mdi_audio_mma_close(l_QuietWavHd);
		    l_QuietWavHd = 0;
	    }
        
        memset(l_AudChls, 0 ,sizeof(l_AudChls));

        if( dec ==i51_MMI_DEC_FILE_BASED )
        {
            i51AdeOsGetFilePath ( FilePath , (const iU16 *)buffer ) ;
	        #if( I51_MTK_VS>=VS_11A_1112 )
                ret = mdi_audio_play_file_with_vol_path( FilePath, i51AudioConvetLoop_SinglePlay(loop), iNULL, i51AdeMmiMediaSinglePlayCB, iNULL, l_volumeValue-1, MDI_DEVICE_SPEAKER2 );
            #else
                ret = mdi_audio_play_file_with_vol_path( FilePath, i51AudioConvetLoop_SinglePlay(loop), iNULL, (mdi_callback)i51AdeMmiMediaSinglePlayCB, l_volumeValue-1, MDI_DEVICE_SPEAKER2 );       
            #endif
        }
        else{
            #if( I51_MTK_VS>=VS_11A_1112 )
                ret = mdi_audio_play_string_with_vol_path(buffer, length, i51AudioConvetFormat(type), i51AudioConvetLoop_SinglePlay(loop), (mdi_ext_callback)i51AdeMmiMediaSinglePlayCB, iNULL, l_volumeValue-1, MDI_DEVICE_SPEAKER2);
            #else
                ret = mdi_audio_play_string_with_vol_path(buffer, length, i51AudioConvetFormat(type), i51AudioConvetLoop_SinglePlay(loop), iNULL, (mdi_callback)i51AdeMmiMediaSinglePlayCB, l_volumeValue-1, MDI_DEVICE_SPEAKER2);
            #endif
            l_AudChls[0].aud_hd = 1 ;
        }
        if(ret != 0)
        {
            i51AdeOsLog(0, "MMI : Play Audio Fail!ret = %d,l_AudChls[0].aud_hd = %d",ret,l_AudChls[0].aud_hd);
            return iFALSE;
        }   
        I51_PrevMemModeMixSnd = iFALSE;
        l_AudChls[0].using = 1;
        l_AudChls[0].priority = priority;  
        l_AudChls[0].buffer = buffer; 
        l_AudChls[0].STOP = STOP;
        kal_get_time( &l_AudChls[0].start_time ); 
        l_AudChls[0].state = AUDIO_PLAYING;  
        l_AudChls[0].loop_flag = loop;
        
	    return iTRUE ; 

    }
    else //之前的mode不支持混音，无论现在要播放的type都要检查关闭audio[0]
    {
        i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerPlay , Last Audio not supoort Mix! type= %d ,priority =%d ,loop =%d ,dec=%d ",type,priority,loop,dec);
        if( l_AudChls[0].using )
        {
            if( l_AudChls[0].buffer == buffer )
                return iTRUE;
            if( l_AudChls[0].priority < priority )	return iFALSE;  
            
            l_AudChls[0].using =0;
            if(l_AudChls[0].STOP)
                l_AudChls[0].STOP( l_AudChls[0].buffer );
            l_AudChls[0].STOP = NULL;
            if(l_AudChls[0].aud_hd==0)
            {
                mdi_audio_stop_file();
            }
            else{
                mdi_audio_stop_string();
            }
            memset(l_AudChls, 0 ,sizeof(l_AudChls));               
        }
        
        if( dec == i51_MMI_DEC_FILE_BASED )
        {
            i51AdeOsGetFilePath ( FilePath , (const iU16 *)buffer ) ;
	        #if( I51_MTK_VS>=VS_11A_1112 )
                ret = mdi_audio_play_file_with_vol_path( FilePath, i51AudioConvetLoop_SinglePlay(loop), iNULL, i51AdeMmiMediaSinglePlayCB, iNULL, l_volumeValue-1, MDI_DEVICE_SPEAKER2 );
            #else
                ret = mdi_audio_play_file_with_vol_path( FilePath, i51AudioConvetLoop_SinglePlay(loop), iNULL, (mdi_callback)i51AdeMmiMediaSinglePlayCB, l_volumeValue-1, MDI_DEVICE_SPEAKER2 );       
            #endif
            find_chnl = 0;
        }
        else
        {
            if( type == i51_MEDIA_TYPE_MP3 || type == i51_MEDIA_TYPE_AMR )
            {
                #if( I51_MTK_VS>=VS_11A_1112 )
                    ret = mdi_audio_play_string_with_vol_path(buffer, length, i51AudioConvetFormat(type), i51AudioConvetLoop_SinglePlay(loop), (mdi_ext_callback)i51AdeMmiMediaSinglePlayCB, iNULL, l_volumeValue-1, MDI_DEVICE_SPEAKER2);
                #else
                    ret = mdi_audio_play_string_with_vol_path(buffer, length, i51AudioConvetFormat(type), i51AudioConvetLoop_SinglePlay(loop), iNULL, (mdi_callback)i51AdeMmiMediaSinglePlayCB, l_volumeValue-1, MDI_DEVICE_SPEAKER2);
                #endif
                l_AudChls[0].aud_hd = 1 ;
                find_chnl = 0;
            }
            else{
                if(l_QuietWavHd==NULL)
                {
                    l_QuietWavHd = i51_mdi_audio_mma_open_string( (void*)(QUIET_WAV), sizeof(QUIET_WAV), MDI_FORMAT_WAV, 0, NULL );
                    mdi_audio_mma_play(l_QuietWavHd);
                }
                
                if( type == i51_MEDIA_TYPE_MIDI )
                {
                    l_AudChls[0].aud_hd = i51_mdi_audio_mma_open_string( buffer, length, i51AudioConvetFormat(type), i51AudioConvetLoop(loop), i51AdeMmiMediaPlayCB );   
                    find_chnl = 0;
                }
                else  //( type == i51_MEDIA_TYPE_WAV)
                {
                    l_AudChls[WAV_CHL_START].aud_hd = i51_mdi_audio_mma_open_string( buffer, length, i51AudioConvetFormat(type), i51AudioConvetLoop(loop), i51AdeMmiMediaPlayCB );  
                    find_chnl = WAV_CHL_START ;
                }
                ret = mdi_audio_mma_play( l_AudChls[find_chnl].aud_hd );
                if(ret != 0)
                {
                    if(l_AudChls[find_chnl].aud_hd)
                        mdi_audio_mma_close( l_AudChls[find_chnl].aud_hd );
                    l_AudChls[find_chnl].aud_hd = 0;
                }
            }
        }
        
        if(ret != 0)
        {
            i51AdeOsLog(0, "MMI : Play Audio Fail!ret = %d,l_AudChls[find_chnl].aud_hd = %d",ret,l_AudChls[find_chnl].aud_hd);
            return iFALSE;
        }
        l_AudChls[find_chnl].using = 1;
        l_AudChls[find_chnl].priority = priority;  
        l_AudChls[find_chnl].buffer = buffer; 
        l_AudChls[find_chnl].STOP = STOP;
        kal_get_time( &l_AudChls[find_chnl].start_time ); 
        l_AudChls[find_chnl].state = AUDIO_PLAYING;  
        l_AudChls[find_chnl].loop_flag = loop;   
        if(((type == i51_MEDIA_TYPE_MIDI)||(type == i51_MEDIA_TYPE_WAV))&&(dec == i51_MMI_DEC_MEM_BASED))
            I51_PrevMemModeMixSnd = iTRUE ;
        else
            I51_PrevMemModeMixSnd = iFALSE ;
            
        return iTRUE ; 
    }
}
iBOOL i51AdeMmiMediaPlayerPause ( iHANDLE handle )
{
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.

    int i;
    
    if(!I51_PrevMemModeMixSnd)
    {
        if(l_AudChls[0].using && l_AudChls[0].state == AUDIO_PLAYING )
        {
            mdi_audio_pause( iNULL , iNULL );
            l_AudChls[0].state = AUDIO_PAUSE;
            return iTRUE;
        }
        return iFALSE;
    }
    else{
        for( i=0; i<AUDIO_CHL_MAX; i++ )
        {
            if( l_AudChls[i].using && l_AudChls[i].buffer == handle && l_AudChls[i].state == AUDIO_PLAYING )
            {
                mdi_audio_mma_pause( l_AudChls[i].aud_hd );
                l_AudChls[i].state = AUDIO_PAUSE;
                return iTRUE;
            }
        }
        return iFALSE ; 
    }
}
iBOOL i51AdeMmiMediaPlayerResume ( iHANDLE handle ) 
{
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.

    int i;

    if(!I51_PrevMemModeMixSnd)
    {
        if(l_AudChls[0].using && l_AudChls[0].state == AUDIO_PAUSE )
        {
            mdi_audio_resume( iNULL , iNULL );
            l_AudChls[0].state = AUDIO_PLAYING;
            return iTRUE;
        }
        return iFALSE;
    }
    else{
        for( i=0; i<AUDIO_CHL_MAX; i++ )
        {
            if( l_AudChls[i].using && l_AudChls[i].buffer == handle && l_AudChls[i].state == AUDIO_PAUSE )
            {
                mdi_audio_mma_resume( l_AudChls[i].aud_hd );
                l_AudChls[i].state = AUDIO_PLAYING;
                return iTRUE;
            }
        }
        return iFALSE ; 
    }
}

iBOOL i51AdeMmiMediaPlayerStop ( iHANDLE handle )
{
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.
    int i;
    
    if(!I51_PrevMemModeMixSnd)
    {
        if(l_AudChls[0].using )
        {
            l_AudChls[0].using = iFALSE ;
            if(l_AudChls[0].aud_hd==0)
            {
                i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , Stop File !");
                mdi_audio_stop_file();
            }
            else
            {
                i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , Stop Mem !");
                mdi_audio_stop_string();
            }
        }
        return iTRUE;
    }
    else{
        for( i=0; i<AUDIO_CHL_MAX; i++ )
        {
            if( l_AudChls[i].using && l_AudChls[i].buffer == handle  )
            {
                i51AdeOsLog(0, "MMI : i51AdeMmiMediaPlayerStop , Stop MIX %d !",i);
                l_AudChls[i].using = iFALSE;
                mdi_audio_mma_stop( l_AudChls[i].aud_hd );        
                mdi_audio_mma_close( l_AudChls[i].aud_hd );
                return iTRUE;
            }
        }
        return iFALSE ;
    }
}

iS32 i51AdeMmiMediaPlayerFormat( void )
{
return (1<<i51_MEDIA_TYPE_MIDI)|(1<<i51_MEDIA_TYPE_WAV)|(1<<i51_MEDIA_TYPE_AMR)|(1<<i51_MEDIA_TYPE_MP3);
}
iBOOL i51AdeMmiMediaPlayerSetVolume ( iU32 volume )
{
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "MMI : i51AdeMmiMediaPlayerSetVolume , S , %x , %x" ,  volume ) ;
	#endif	
	
	if( volume > 7 ) {
		
		l_volumeValue =  7 ;
		
	}else if( l_volumeValue  <  1 ) {
	
		l_volumeValue = 1 ;	
	} else {
	
		l_volumeValue = volume ;
	}

    mdi_audio_set_volume( VOL_TYPE_MEDIA ,  l_volumeValue-1 ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "MMI : i51AdeMmiMediaPlayerSetVolume , E , ret = %x" , iTRUE ) ;
	#endif	

    return iTRUE ;	
}

iU32 i51AdeMmiMediaPlayerGetVolume ()
{
	//	author : miki
	//	since : 20111216
	//	PKIG Tech. Co., Ltd.

	return l_volumeValue ;
}

//record part
static void i51RecordCB(Media_Event event)
{
    if( event == MEDIA_DATA_NOTIFICATION )
    {
    	U32 nLen = 0;
    	WCHAR *pBuffer = NULL;    	
                
        if( IsRecording && i51AppRecordCB ){
            Media_GetReadBuffer(&pBuffer, &nLen);
            i51AppRecordCB( pBuffer, nLen*2 );
            Media_ReadDataDone(nLen);
        }
    }
}

iS32 i51AdeMmiRecorderStart  ( void* buffer , iU32 bufflen , void (*callback ) ( void* stream , iU32 length ) ) 
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.

    Media_Status  ret;
    if( buffer == NULL || bufflen < 1024*8 || callback == NULL ) 
        return -1;
    
    i51AdeOsLog ( 0 , "MMI : i51AdeMmiRecorderStart ") ;

    i51AppRecordFileCB = iNULL ;
#ifndef WIN32
    if(AM_IsConflictState())
        kal_sleep_task(1000);
#endif
    bufflen /=2;
    i51AppRecordCB = callback;
    Media_SetBuffer( buffer, bufflen );
    ret = Media_Record( MEDIA_FORMAT_PCM_8K, i51RecordCB, NULL);
	if( ret != MEDIA_SUCCESS )
        return -1;
    IsRecording = TRUE;
    return 1;//0=amr, 1=wav
}

/* 内存形式的AMR录音
static iFILE RecFileFh ;
static iU8 recbuffer[8*1024] ;
void i51RecorderFileCB(void* stream , iU32 length)
{
    
    iU32 nWritten ;
        
    if(RecFileFh)
        i51AdeOsFileWrite ( RecFileFh , stream , length , &nWritten );  

}

iS32 i51AdeMmiRecorderStartEx_mem ( iU16* filename , void (*callback ) ( iBOOL finished ) )
{
    
    Media_Status  ret;
    iU32 bufflen =8*1024;
    iU32 nWritten;
    iU8 amrhead[6] = {0x23, 0x21 ,0x41 , 0x4D , 0x52 , 0x0A };

#ifndef WIN32
    if(AM_IsConflictState())
        kal_sleep_task(1000);
#endif
    RecFileFh = i51AdeOsFileOpen( filename ,  i51_OS_FL_ALWAYS_CREATE | i51_OS_FL_WRITE , __FILE__ , __LINE__ );
    if(RecFileFh<=0)
    {
        i51AdeOsLog(0 , "ADE - MMI : i51AdeMmiRecorderStartEx OPEN F");
        return -1;
    }
    i51AdeOsFileWrite ( RecFileFh , amrhead , 6 , &nWritten );
    bufflen /=2;
    i51AppRecordCB = i51RecorderFileCB;
    i51AppRecordFileCB = callback;
    Media_SetBuffer( (iU16*)recbuffer, bufflen );
    ret = Media_Record( MEDIA_FORMAT_AMR, i51RecordCB, NULL);
	if( ret != MEDIA_SUCCESS )
        return -1;
    IsRecording = TRUE;
    return 0;//0=amr, 1=wav

}
*/
#if( I51_MTK_VS>=VS_11A_1112 )
void i51RecorderFileSysCB( mdi_result result, void* user_data)
#else
void i51RecorderFileSysCB( mdi_result result)
#endif
{
    i51AdeOsLog( 0 , "MMI : i51RecorderFileSysCB , %d(-9006 is OK)",result);
    if((result == MDI_AUDIO_TERMINATED )&&i51AppRecordFileCB)
    {
        i51AppRecordFileCB(1);
    }
    
    i51AppRecordFileCB = iNULL ;
}

iBOOL i51AdeMmiRecorderStartEx ( iU16* filename , void (*callback ) ( iBOOL finished ) )
{
    mdi_result ret;
    iU16 Filename[64];

    if( callback == iNULL ) 
        return iFALSE ;

    i51AdeOsLog( 0 , "MMI : i51AdeMmiRecorderStartEx , S");

    i51AppRecordFileCB =callback ;

    if(i51AdeOsFileIsExist(filename))
        i51AdeOsFileDelete(filename);
    
    i51AdeOsGetFilePath( Filename, filename );
    
    #if( I51_MTK_VS>=VS_11A_1112 )
        ret = mdi_audio_start_record((void*)Filename, MEDIA_FORMAT_AMR , 0 , i51RecorderFileSysCB , iNULL );
    #else
        ret = mdi_audio_start_record((void*)Filename, MEDIA_FORMAT_AMR , 0 , iNULL , i51RecorderFileSysCB );
    #endif
    
    if(ret)
    {
        i51AdeOsLog( 0 , "MMI : i51AdeMmiRecorderStartEx , F");
        return iFALSE;
    }
    
    IsRecording = TRUE;
    return iTRUE ;
}

void i51RecorderStopCB ( void )
{
    if(i51AppRecorderStopCB)
        i51AppRecorderStopCB(1);
}


static void i51AdeMmi_RecorderStop_TIMERCB ( iU32 id )
{

	i51AdeOsTimerStop(id) ;
	i51RecorderStopCB () ;
}
static iBOOL i51AdeMmi_ASYN_RecorderStopCB ()
{

	//	i51SlDlCALLBACK 回调，异步通知

	iU32 timerid = 0 ;
	timerid = i51AdeOsTimerCreate ( 0 , 1 ) ;
	if ( 0 == timerid )
	{
		i51AdeOsLog ( 0 , "MMI : i51AdeMmi_ASYN_RecorderStopCB , TC " ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart (timerid , i51AdeMmi_RecorderStop_TIMERCB , 500 ) )
	{
		i51AdeOsLog ( 0 , "MMI : i51AdeMmi_ASYN_RecorderStopCB , TS F" ) ;		
		return iFALSE ;
	}		
}

iBOOL i51AdeMmiRecorderStop ( void (*callback) ( iBOOL results ) )
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
    
    i51AppRecorderStopCB = callback;

    if(!IsRecording)
    {
        i51AdeOsLog ( 0 , "MMI : i51AdeMmiRecorderStop , NO RECORDING" ) ;
        return iTRUE;
    }
    
    IsRecording = iFALSE;
    if(i51AppRecordFileCB)
    {
        mdi_audio_stop_record();
    }
    else
    {
	    Media_Stop();
    }

    i51AdeOsLog ( 0 , "MMI : i51AdeMmiRecorderStop" ) ;	
    
 /*   录内存的AMR接口用
    if(i51AppRecordFileCB)
    {
        i51AppRecordFileCB(1);
        i51AppRecordFileCB=iNULL;
    }
    if(RecFileFh)
    {
        i51AdeOsFileClose( RecFileFh );
        RecFileFh=iNULL;
    }
*/
    i51AdeMmi_ASYN_RecorderStopCB () ;
    return iTRUE;
}

void i51RecordPlayCB(mdi_result result, void* user_data)
{
    if(i51AppRecoderPlayCB)
        i51AppRecoderPlayCB(1);
}

 iBOOL i51AdeMmiRecoderPlay ( void* stream , iU32 length , void (*callback) ( iBOOL results ) ) 
 {
    U16  Filename[64];
    iFILE fh;
    iU32  nWritten;
    mdi_result ret;

	U8 WavHead[44] = { // 44 bytes
						0x52, 0x49, 0x46, 0x46, 0xa2, 0x9e, 0x04, 0x00, 0x57, 0x41, 	// 0 ~ 9
						0x56, 0x45, 0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 	// 10 ~ 19
						0x01, 0x00, 0x01, 0x00, 0x40, 0x1f, 0x00, 0x00, 0x00, 0x7d, 	// 20 ~ 29 16000->3e80 8000->1f40
						0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 	// 30 ~ 39
						0x7e, 0x9e, 0x04, 0x00										// 40 ~ 43
						};

    i51AppRecoderPlayCB = callback;
    fh = i51AdeOsFileOpen(L"temp.wav", i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ );
    i51AdeOsLog( 0, "MMI : 51AdeMmiRecoderPlay , fh =%d", fh );
    if(fh==0) return iFALSE;
    i51AdeOsFileWrite ( fh , WavHead , 44, &nWritten );
    i51AdeOsFileWrite ( fh , stream , length, &nWritten );
    i51AdeOsFileClose( fh );
    i51AdeOsGetFilePath( Filename, L"temp.wav" );

	#if( I51_MTK_VS>=VS_11A_1112 )
        ret = mdi_audio_play_file_with_vol_path( Filename, DEVICE_AUDIO_PLAY_ONCE, NULL, i51RecordPlayCB, NULL, l_volumeValue-1, MDI_DEVICE_SPEAKER2 );
    #else
        ret = mdi_audio_play_file_with_vol_path( Filename, DEVICE_AUDIO_PLAY_ONCE, NULL, (mdi_callback)i51RecordPlayCB, l_volumeValue-1, MDI_DEVICE_SPEAKER2 );       
    #endif

	return iTRUE;
 }

iBOOL i51AdeMmiRecorderPlayStop (void)
{
    mdi_audio_stop_file();
	return iTRUE;
}

void i51AdeMmiRecorderFormat ( iU32* samples , iU32* bitscale , iU32* chanels ) 
{

    //recorde Wavfile,get form wavfile head
    *chanels = 1 ;
    *samples = 8000 ;
    *bitscale = 16 ;

return;

}
void i51AdeMmiGetTextAttr ( iU16* text , iU16* textwidth , iU16* textheight )
{
	//	author : miki
	//	since : 20111010
	//	PKIG Tech. Co., Ltd.
    int w,h;
    gui_measure_string( text , &w, &h );
    *textwidth = w;
    *textheight = h;
}


#define R_OF_565(p)      (((p)&0xf800) >> 8)
#define G_OF_565(p)      (((p)&0x7e0) >> 3)
#define B_OF_565(p)      (((p)&0x1f) << 3)
void i51AdeMmiDrawText ( iU8* panel , iU16 width , iU16 height , const iU16* text , iS32 desx , iS32 desy , iCOLOR color_txt ) 
{
	//	author : miki
	//	since : 20111010
	//	PKIG Tech. Co., Ltd.

    color GuiTxtColor;

    GuiTxtColor = gui_color( R_OF_565(color_txt), G_OF_565(color_txt), B_OF_565(color_txt) );
    gui_set_text_color ( GuiTxtColor ) ;
	gui_move_text_cursor ( desx , desy ) ;
	gui_print_text ( (UI_string_type)text ) ;
}

iBOOL i51AdeMmiCreate ( i51ADECALLBACK callback ) 
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.

	iASSERT ( callback ) ;
	BaseLayerUsed = iFALSE ;
	NewLayerUsed = iFALSE ;
	
	i51AdaCreateCB = callback ;
	//gdi_layer_create ( 0 , 0 , UI_device_width , UI_device_height , &NewLayer ) ;
	if (!i51AdeMmi_ASYN_i51ADECALLBACK ())
		return iFALSE ;
	
	return iTRUE ;

	
} 

iBOOL i51AdeMmiRelease ( void ) 
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
	
	//gdi_layer_free ( NewLayer ) ;
	return iTRUE ;

} 

