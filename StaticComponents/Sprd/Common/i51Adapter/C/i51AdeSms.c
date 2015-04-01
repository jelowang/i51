 
/*

	The Implementing of 'Adapter' , Module : 'SMS'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/
 
#include "i51AdeSms.h"
#include "i51AdeOs.h"
#include "i51_define.h"

#include "scttypes.h"
#include "ctypes.h"
#include "sci_api.h"
#include "tasks_id.h"
#include "mn_type.h"        
#include "sim_macro_switch.h"
#include "sim_type.h"
#include "sim_file_structure.h"
#include "nv_type.h"
#include "mmisms_nv.h"
#include "mmisms_send.h"
#include "mmi_signal_ext.h"
#include "mmisms_app.h"

#include "mn_api.h"
#if defined (SPRD_6600) || defined (SPRD_6620) || defined (SPRD_6800)
#include "mmiphone.h"
#include "mmiconnection.h"
#include "mmipb_internal.h"
#endif

#ifdef SPRD_6530
#include "mmipb_app.h"
#endif


typedef struct 
{

	//	author : Jelo 
	//	since : 2011.6.9
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 短信钩子
       iS32 i51HookType;
	i51SMSHOOKCALLBACK callback ;

} i51SMSHOOKS ;

typedef enum {

	//	author : william
	//	since : 20110830
	//	(C)NON TEST

	//卡1
	I51_SIM_CARD_1 ,
	
	//卡2
	I51_SIM_CARD_2 ,
	
	//卡3
	I51_SIM_CARD_3 ,
	
	//卡4
	I51_SIM_CARD_4 ,


} ENUM_SIM_CARD ;

#define i51SMSHOOKS_MAX 5
#define i51SMSDEVICE_MAX	5
typedef void (*i51ADESMSSENDCALLBACK)(iBOOL result);

static iU32 i51SmsDevice[i51SMSDEVICE_MAX]={0};
static i51ADESMSSENDCALLBACK i51AdeSmsCallback = NULL;
static i51ADESMSSENDCALLBACK i51ADESMSCREATECALLBACK = NULL;
//	短信发送状态: 0 发送过程已结束，1发送过程未结束
static iU32 i51SendTimerID = 0;
static iU8 i51AdeSendIsOpen = 0;
static iU8 i51AdeSmsCreateTimer = 0;
static iU32 i51AdeSmsCreateTimerID = 0;
static i51SMSHOOKS i51SmsHooker [ i51SMSHOOKS_MAX ] = { 0 } ;
MMISMS_OPER_ERR_E SmsSendResult = MMISMS_NO_ERR;

static i51ADECALLBACK i51AdeCallback = 0 ;
static iU32 i51AdeCreateTimer = 0 ;
//extern MMISMS_DELIVER_T                   g_mmisms_deliver; 
extern MMISMS_GLOBAL_T g_mmisms_global; 


static iU8 i51ConvetSimCard ( iS32 format)
{

	//	author : william
	//	since : 20110801
	//	PKIG Tech. Co., Ltd.
	
	switch ( format  ) {

		case 1 :
			return I51_SIM_CARD_1 ;
		case 2 :
			return I51_SIM_CARD_2 ;
		case 3 :
			return I51_SIM_CARD_3 ;
		case 4 :
			return I51_SIM_CARD_4 ;		
		default:
            i51AdeOsLog(0, "SMS : i51ConvetSimCard , NO SIM Card" ) ;
			return 0 ;
	
	}
	
} 


static void i51SmsCreateCallback(iU32 id)
{
      i51AdeSmsCreateTimer = 0;
      if(NULL!=i51ADESMSCREATECALLBACK)
      { 
          i51ADESMSCREATECALLBACK(1);
      }
      i51AdeOsTimerStop(i51AdeSmsCreateTimerID);
}

static void i51SendTimeCallback(iU32 id)
{
       //	author : steven	
	//	since : 2011.12.28
	//	note:监控短信发送是否完成的定时器回调

       iBOOL ree = iFALSE;

      // i51AdeOsLog(0, "SMS : i51SendTimeCallback , g_mmisms_global.is_send_fail :%d",SmsSendResult) ;
       if( SmsSendResult == MMISMS_NO_ERR)
       {
            if(NULL!=i51AdeSmsCallback)
            {
                  i51AdeSmsCallback(iTRUE);
            }
       }
       else
      {
          if(NULL!=i51AdeSmsCallback)
          {
                i51AdeSmsCallback(iFALSE);
          }
     }
      i51AdeSendIsOpen = 0;
         i51AdeOsTimerStop(i51SendTimerID);
         i51SendTimerID = 0;

      
}
static iU8 *i51AdeSmsConvertBcdToDigitalStr(
				uint8         ilength,
				uint8         *ibcd_ptr,      // in: the bcd code string
				uint8         *idigital_ptr   // out: the digital string
				)
{
       //	author : steven	
	//	since : 2011.12.28
	//	(C) PKIG Tech. Co., Ltd.
	
	int32         i;
	uint8         temp;

	// get the first digital
	temp = ((*ibcd_ptr >> 4) &0x0f);
	if (temp >= 0x0a)
	{
		*idigital_ptr = (temp - 0x0a) + 'A';
	}
	else
	{
		*idigital_ptr = temp + '0';
	}
	
	ibcd_ptr++;
	idigital_ptr++;


	for (i=0; i<(ilength - 1); i++)
	{
		temp = *ibcd_ptr;
		// get the low 4 bits
		temp &= 0x0f;
		// A -- F
		if (temp >= 0x0a)
		{
			*idigital_ptr = (temp - 0x0a) + 'A';
		}
		else
		{
			// 1 -- 9
			*idigital_ptr = temp + '0';
		}
		idigital_ptr++;

		temp = *ibcd_ptr;
		// get the high 4 bits
		temp = (temp & 0xf0) >> 4;  
		
		if ((temp == 0x0f) && (i == (ilength -1)))
		{
			*idigital_ptr = '\0';	
			return;
		}
		// A -- F
		else if (temp>=0x0a)
		{
			*idigital_ptr = (temp - 0x0a) + 'A';
		}
		else
		{
			// 1 -- 9
			*idigital_ptr = temp + '0';
		}
		idigital_ptr++;
		ibcd_ptr++;
	}
	*idigital_ptr = '\0';
}

void  i51AdeSmsSendRecord ( iBOOL ret )
{
	//	发送报告
	iBOOL Sending = iFALSE ;
	iU16 i = 0 ;

    i51AdeOsLog(0, "SMS : i51AdeSmsSendRecord") ;
	for ( i = 0 ; i <  i51SMSDEVICE_MAX ; i++ ) {

		if ( i51SmsDevice[i]) {

			Sending = iTRUE ;
			break ;
		}

	}
	if ( Sending ) {

		if (i51AdeSmsCallback) {

            i51AdeOsLog(0, "SMS : i51AdeSmsSendRecord , ret = %d" , ret) ;
			i51AdeSmsCallback (ret) ;
		}
		i51AdeSmsCallback = NULL ;
		
	}
}

iS32* i51AdeSmsGetSims ()
{

	//	author : Jelo	
	//	since : 2011.8.18
	//	(C) PKIG Tech. Co., Ltd.

	static iS32 sims [ 32 ] = { 0 } ;
	MMISET_DUALSYS_TYPE_E	i51_dual_sys = 0;
	iU32 	num = 0;
	iU32  i = 0;

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(0, "SMS : i51AdeSmsGetSims , S") ;
#endif

	for ( i = 0 ; i < 2 ; i++ ) {

		if(MMIPHONE_IsSimOk(i))
		{
		  sims[num+1] = i + 1;
		  num++;
		}
	}
	sims[0] = num;

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(0, "SMS : i51AdeSmsGetSims , E  sims[0]:%d,sims[1]:%d,sims[2]:%d",sims[0],sims[1],sims[2]) ;
#endif
    
       return sims;
}

iCHAR* i51AdeSmsGetImsi ( iS32 sim )
{

	//	author : Jelo	
	//	since : 2011.4.26
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 获取IMSI
	//	成功返回> 0 , 失败返回0	
	//	该接口不对动态层开放

	MN_IMSI_T imsi = {0};
	iU8 len = 0;
	static iU8 temp_str[20] = {0} ;
	iS32 nIndex= sim - 1 ;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetImsi , S :%d",sim) ;
	#endif
    
	if(nIndex >= 0)
	{
		imsi = MNSIM_GetImsiEx(nIndex);
		
		if(MN_MAX_IMSI_ARR_LEN >= imsi.imsi_len)
		{
			i51AdeSmsConvertBcdToDigitalStr ( imsi.imsi_len , imsi.imsi_val , temp_str ) ;
            i51AdeOsLog(0, "SMS : i51AdeSmsGetImsi , IMSI %s , SIM %d\r\n" , temp_str , sim  ) ;
			return temp_str ;
		}
	}

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetImsi , E:%s",temp_str) ;
	#endif
    
	return temp_str ;

}

iCHAR* i51AdeSmsGetImei ()
{

	//	author : Jelo	
	//	since : 2011.4.26
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 获取IMEI
	//	成功返回> 0 , 失败返回<0	
	
	iU8 imeiStr[MN_MAX_IMEI_LENGTH << 1] = {0};
	MN_DUAL_SYS_E dual_sys = 0;
	iU8   imei[MN_MAX_IMEI_LENGTH] = {0};
	static iU8 ime_str[MN_MAX_IMEI_LENGTH << 1];

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetImei , S") ;
	#endif
    
	memset(ime_str, 0,MN_MAX_IMEI_LENGTH << 1);


	dual_sys = MMIAPISET_GetActiveSim () ;
	
	if ( MNNV_GetIMEIEx ( dual_sys , imei ) )
	{
//		MMIAPICOM_BcdToStr ( 0 , imei , MN_MAX_IMEI_LENGTH << 1 , (iCHAR* ) imeiStr ) ;
		i51AdeSmsConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei, imeiStr);
		memcpy ( ime_str, (imeiStr+1) , (MN_MAX_IMEI_LENGTH<<1)-1 ) ;

        i51AdeOsLog(0, "SMS : i51AdeSmsGetImei , IMEI %s\r\n" , ime_str ) ;
			
	}

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetImei , E:%s",ime_str) ;
	#endif

	return ime_str ;

}

iCHAR* i51AdeSmsGetCenter ( iS32 sim )
{

	//	author : Jelo	
	//	since : 2011.4.26
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 获取短信中心号
	//	成功返回>= 0 , 失败返回0	

	static iU8 smsc [ 64 ] = { 0 } ;
	iU8 smsclen = 0 ;
       iS32 simT = sim - 1;

    #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "SMS : i51AdeSmsGetCenter , S:%d",sim) ;
	#endif
    
	MMISMS_AppGetSCAddr ( simT , smsc , &smsclen , 64 ) ;

    i51AdeOsLog(0, "SMS : i51AdeSmsGetCenter , sim %d , smsc %s , length %d\r\n" , sim , smsc , smsclen ) ;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetCenter , E:%s",smsc) ;
	#endif
    
	return smsc ;
	
}

iU32 i51AdeSmsCreateDevice ()
{
       //	author : steven
	//	since : 2011.12.28
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 创建短信设备
	//	成功返回> 0 , 失败返回0

       iU32 i = 0;

    #ifdef i51ADAPTER_DEBUG
       i51AdeOsLog(0, "SMS : i51AdeSmsCreateDevice , S") ;
	#endif

       i51AdeOsLog(0, "SMS : i51AdeSmsCreateDevice") ;
	
       for(i =0;i<5;i++)
       {
             if(i51SmsDevice[i]==0)
             {
                 i51SmsDevice[i] = 1;
                 #ifdef i51ADAPTER_DEBUG
                    i51AdeOsLog(0, "SMS : i51AdeSmsCreateDevice , E:%d ",i+1) ;
          	     #endif
                 return i+1;
             }
       }
       return 0 ; 
}

iBOOL i51AdeSmsReleaseDevice ( iU32 device ) 
{
       //	author : steven
	//	since : 2011.12.28
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 释放短信设备
	//	成功返回iTRUE , 失败返回iFALSE

       #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsReleaseDevice , S :%d",device) ;
	#endif

        i51AdeOsLog(0, "SMS : i51AdeSmsReleaseDevice , %d" , device ) ;
	
      if(device>5||device==0)return iFALSE;
      i51SmsDevice[device-1] = 0;

       #ifdef i51ADAPTER_DEBUG
      i51AdeOsLog(0, "SMS : i51AdeSmsReleaseDevice , E :%d",1) ;
	#endif
      return iTRUE ; 
}

iBOOL i51AdeSmsSend ( iU32 device , iS32 sim , iCHAR* phone , iU16* contents , iBOOL save , void (*callback)( iBOOL results )   )
{


	//	author : Jelo	
	//	since : 2011.5.24
	//	(C) PKIG Tech. Co., Ltd.	

	//	notes : 为了兼容n多卡模式，该接口内部自己选择一个可用的sim卡。

#ifdef SPRD_6530

	iS32 ret = 0 ;	
	iU8 reportbak = 0 ;
	iU8 savsms = 0 ;
	iU32 smschanel = 0 ;
	MMISMS_SEND_T *send_info_ptr = PNULL;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsSend , S dev:%d,sim:%d,phone:%s,contents:%x,save:%d", device , sim ,  phone , contents , save ) ;
	#endif
        i51AdeOsLog(0, "SMS : i51AdeSmsSend , %d,sim:%d,phone:%s,contents:%x,save:%d", device , sim ,  phone , contents , save ) ;
	if(0==device||device>5 ||0 == sim|| NULL == phone )
		return iFALSE;
    
       if(!i51SmsDevice[device-1])return iFALSE;

	smschanel = i51ConvetSimCard (sim) ;
//	smschanel = MN_DUAL_SYS_MAX ;
//	smschanel = g_mmisms_global.edit_content.dual_sys ;
		
	savsms = save ;
	
	send_info_ptr = MMISMS_CreateSendSMS();

	if ( NULL == send_info_ptr)
	{
        i51AdeOsLog(0, "SMS : i51AdeSmsSend , Create F") ;
		return iFALSE ;
	}

	//	保存系统设置的发送报告
	reportbak = MMISMS_GetStatusReportFlag () ;
	MMISMS_SetStatusReportFlag ( 0 ) ;
	
	MMISMS_ClearResendCount(send_info_ptr);
	MMISMS_ClearDestNum () ;

	send_info_ptr->send_content.is_need_save = iFALSE ; 
	send_info_ptr->send_content.is_enough_space_backsend = iFALSE ;
	send_info_ptr->send_content.dual_sys = smschanel ;
//	send_info_ptr->send_content.dual_sys = g_mmisms_global.edit_content.dual_sys;
	
	MMISMS_SetAddressToMessage ( (iU8*)phone , strlen(phone) , &(send_info_ptr->send_content.dest_info.dest_list)) ;
	
	MMISMS_SetMessageContent ( i51AdeStdWStrlen(contents) , contents , iTRUE , &send_info_ptr->send_content ) ;
	
	MMISMS_InitSendSaveResult(send_info_ptr);
	
	MMISMS_AppSendSms(smschanel,savsms, send_info_ptr);
	
	MMISMS_SetOperStatus ( MMISMS_NO_OPER ) ;
	
 	SCI_MEMSET ( &g_mmisms_global.edit_content , 0 , sizeof(MMISMS_SMSEDIT_CONTENT_T) ) ;	
	//	恢复系统设置
	MMISMS_SetStatusReportFlag ( reportbak ) ;
	
	i51AdeSmsCallback =callback;
	
	
#else
	iS32 ret = 0 ;	
	iU8 reportbak = 0 ;
	iU8 savsms = 0 ;
	iU32 smschanel = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsSend , S dev:%d,sim:%d,phone:%s,contents:%x,save:%d", device , sim ,  phone , contents , save ) ;
	#endif

    i51AdeOsLog(0, "SMS : i51AdeSmsSend , %d,sim:%d,phone:%s,contents:%x,save:%d", device , sim ,  phone , contents , save ) ;
	if(0==device||device>5 ||0 == sim|| NULL == phone )
		return iFALSE;
    
       if(!i51SmsDevice[device-1])return iFALSE;

	smschanel = i51ConvetSimCard (sim) ;
		
	savsms = save ;
	//	backup the current setting
 	reportbak = MMISMS_GetStatusReportFlag () ;
	
	//	不要短信回执
	MMISMS_SetStatusReportFlag ( 0 ) ;
	MMISMS_ClearResendCount () ;	
	MMISMS_ClearUpdateAddr () ;
	MMISMS_ClearDestNum () ;
	
	MMISMS_SetGloblalMsgContent (i51AdeStdWStrlen(contents) , contents) ; 
	
	MMISMS_SetNewMsgTOSend () ;
	MMISMS_SetDestNum ( (const iU8*)phone , strlen(phone)) ;
	MMISMS_InitSendSaveResult () ;
	
	ret = MMISMS_AppSendSms ( smschanel , savsms ) ;
	MMISMS_SetOperStatus ( MMISMS_NO_OPER ) ;
 	SCI_MEMSET ( &g_mmisms_global.message , 0 , sizeof(MMISMS_CONTENT_T) ) ;	
	MMISMS_SetStatusReportFlag ( reportbak ) ;
	i51AdeSmsCallback =callback;
#endif
	
    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsSend , E :%d",ret) ;
	#endif
	
	return iTRUE ;

}

iBOOL i51AdeSmsSendEx ( iU32 device , iS32 sim , iCHAR* phone , iCHAR* contents , iBOOL unicode , iBOOL save , void (*callback)( iBOOL results ) )
{
#ifdef SPRD_6530

	iS32 ret = 0 ;	
	iU8 reportbak = 0 ;
	iU8 savsms = 0 ;
	iU32 smschanel = 0 ;
	MMISMS_SEND_T *send_info_ptr = PNULL;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsSendEx , S dev:%d,sim:%d,phone:%s,contents:%x,save:%d", device , sim ,  phone , contents , save ) ;
	#endif
    
    i51AdeOsLog(0, "SMS : i51AdeSmsSendEx , %d,sim:%d,phone:%s,contents:%x,save:%d", device , sim ,  phone , contents , save ) ;
	if(0==device||device>5 ||0 == sim|| NULL == phone )
		return iFALSE;
    
       if(!i51SmsDevice[device-1])return iFALSE;



	smschanel = i51ConvetSimCard (sim) ;
//	smschanel = MN_DUAL_SYS_MAX ;
//	smschanel = g_mmisms_global.edit_content.dual_sys ;
		
	savsms = save ;
	
	send_info_ptr = MMISMS_CreateSendSMS();

	if ( NULL == send_info_ptr)
	{
        i51AdeOsLog(0, "SMS : i51AdeSmsSendEx , Create F") ;
		return iFALSE ;
	}

	//	保存系统设置的发送报告
	reportbak = MMISMS_GetStatusReportFlag () ;
	MMISMS_SetStatusReportFlag ( 0 ) ;
	
	MMISMS_ClearResendCount(send_info_ptr);
	MMISMS_ClearDestNum () ;

	send_info_ptr->send_content.is_need_save = iFALSE ; 
	send_info_ptr->send_content.is_enough_space_backsend = iFALSE ;
	send_info_ptr->send_content.dual_sys = smschanel ;
//	send_info_ptr->send_content.dual_sys = g_mmisms_global.edit_content.dual_sys;
	
	MMISMS_SetAddressToMessage ( (iU8*)phone , strlen(phone) , &(send_info_ptr->send_content.dest_info.dest_list)) ;

	if ( unicode ) {
		
		MMISMS_SetMessageContent ( i51AdeStdWStrlen((iU16*)contents) , (iU16*)contents , iTRUE , &send_info_ptr->send_content ) ;
	}else {

		iU16 *pcontents = 0 ;
		pcontents = (iU16*)SCI_ALLOC_APP (strlen(contents)*2+2) ;
		if ( !pcontents ) {
            i51AdeOsLog(0, "SMS : i51AdeSmsSendEx , malloc F") ;
			return iFALSE ;
		}
		memset ( pcontents , 0 ,strlen(contents)*2+2  ) ;
		MMIAPICOM_StrToWstr ((iU8*) contents , pcontents) ;
		MMISMS_SetMessageContent ( i51AdeStdWStrlen((iU16*)pcontents) , (iU16*)pcontents , iTRUE , &send_info_ptr->send_content ) ;
		SCI_FREE(pcontents) ;

	}
	
	MMISMS_InitSendSaveResult(send_info_ptr);
	
	MMISMS_AppSendSms(smschanel,savsms, send_info_ptr);
	
	MMISMS_SetOperStatus ( MMISMS_NO_OPER ) ;
	
 	SCI_MEMSET ( &g_mmisms_global.edit_content , 0 , sizeof(MMISMS_SMSEDIT_CONTENT_T) ) ;	
	//	恢复系统设置
	MMISMS_SetStatusReportFlag ( reportbak ) ;
	
	i51AdeSmsCallback =callback;
	
	
#else
	iS32 ret = 0 ;	
	iU8 reportbak = 0 ;
	iU8 savsms = 0 ;
	iU32 smschanel = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsSendEx , S dev:%d,sim:%d,phone:%s,contents:%x,save:%d", device , sim ,  phone , contents , save ) ;
	#endif

    i51AdeOsLog(0, "SMS : i51AdeSmsSendEx , %d,sim:%d,phone:%s,contents:%x,save:%d", device , sim ,  phone , contents , save ) ;
	if(0==device||device>5 ||0 == sim|| NULL == phone )
		return iFALSE;
    
       if(!i51SmsDevice[device-1])return iFALSE;

	smschanel = i51ConvetSimCard (sim) ;
		
	savsms = save ;
	//	backup the current setting
 	reportbak = MMISMS_GetStatusReportFlag () ;
	
	//	不要短信回执
	MMISMS_SetStatusReportFlag ( 0 ) ;
	MMISMS_ClearResendCount () ;	
	MMISMS_ClearUpdateAddr () ;
	MMISMS_ClearDestNum () ;
	
//	MMISMS_SetGloblalMsgContent (i51AdeStdWStrlen(contents) , contents) ; 
	if ( unicode ) {
		
		MMISMS_SetGloblalMsgContent (i51AdeStdWStrlen(contents) , contents) ; 
	}else {

		iU16 *pcontents = 0 ;
		pcontents = (iU16*)SCI_ALLOC_APP (strlen(contents)*2+2) ;
		if ( !pcontents ) {
            i51AdeOsLog(0, "SMS : i51AdeSmsSendEx , malloc F") ;
			return iFALSE ;
		}
		memset ( pcontents , 0 ,strlen(contents)*2+2  ) ;
		MMIAPICOM_StrToWstr ((iU8*) contents , pcontents) ;
		MMISMS_SetGloblalMsgContent (i51AdeStdWStrlen(pcontents) , pcontents) ; 
		SCI_FREE(pcontents) ;

	}
	
	MMISMS_SetNewMsgTOSend () ;
	MMISMS_SetDestNum ( (const iU8*)phone , strlen(phone)) ;
	MMISMS_InitSendSaveResult () ;
	
	ret = MMISMS_AppSendSms ( smschanel , savsms ) ;
	MMISMS_SetOperStatus ( MMISMS_NO_OPER ) ;
 	SCI_MEMSET ( &g_mmisms_global.message , 0 , sizeof(MMISMS_CONTENT_T) ) ;	
	MMISMS_SetStatusReportFlag ( reportbak ) ;
	i51AdeSmsCallback =callback;
#endif
	
       #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsSendEx , E :%d",ret) ;
	#endif
	
	return iTRUE ;
}

iS32 i51AdeSmsCreateHook ( iS32 type ) 
{


	//	author : Jelo
	//	since : 2011.6.9
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 获取一个可用Hook，返回id
	//	成功返回>= 0 ，失败返回< 0

	iU32 looper = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsCreateHook , S:%d",type) ;
	#endif

    i51AdeOsLog(0, "SMS : i51AdeSmsCreateHook , %d",type) ;
	
	for ( looper = 0 ; looper < i51SMSHOOKS_MAX ; looper ++ )
	{
        if ( 0 == i51SmsHooker [ looper ].callback ) 
        {      
            i51SmsHooker [ looper ].i51HookType = type;
            #ifdef i51ADAPTER_DEBUG
                i51AdeOsLog(0, "SMS : i51AdeSmsCreateHook , E:%d",looper) ;
            #endif
            return looper ;
        }
	}

	return -1 ;

}

iBOOL i51AdeSmsSetHook ( iS32 hookid , i51SMSHOOKCALLBACK callback  )
{

	//	author : Jelo
	//	since : 2011.6.9
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 短信钩子，成功返回> 0 ，失败返回0
	//	应用层只提供回调函数

	//	成功返回> 0 ，失败返回 0	

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsSetHook , S:%d",hookid) ;
	#endif

    i51AdeOsLog(0, "SMS : i51AdeSmsSetHook , %d , %x",hookid , callback) ;
	
	if ( 0 > hookid || hookid >= i51SMSHOOKS_MAX )
	{
		return 0 ;
	}

	i51SmsHooker [ hookid ].callback = callback ;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsSetHook , E:%d",1) ;
	#endif
	return 1 ;
	
}

iBOOL i51AdeSmsReleaseHook ( iS32 hookid ) 
{  

	//	author : Jelo
	//	since : 2011.6.9
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 销毁一个钩子
	//	成功返回> 0 ，失败返回 0		

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsReleaseHook , S , %d",hookid) ;
	#endif

    i51AdeOsLog(0, "SMS : i51AdeSmsReleaseHook , %d",hookid) ;
    
	if ( 0 > hookid || hookid >= i51SMSHOOKS_MAX )
	{
		return 0 ;
	}

	i51SmsHooker [ hookid ].callback = 0 ;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsReleaseHook , E:%d",1) ;
	#endif
	return 1 ;
	
}

iS32 i51AdeSmsRunHook (  iS32 sim ,  iCHAR* number , iU32 numlen , iU16* buffer , iU32 textlen )
{


	//	author : Jelo
	//	since : 2011.6.9
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 获取一个可用Hook，返回id
	//	成功返回>= 0 ，失败返回< 0

	//	该接口不对应用层开放

	iS32 ret = i51_SMS_HOOK_NORMAL ;
	iU32 looper = 0 ;	

	iU32 tick = 0 ;

       #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsRunHook , S sim:%d,num:%s,numlen:%d,text:%x,textlen:%d",sim ,  number , numlen , buffer , textlen) ;
	#endif

    i51AdeOsLog(0, "SMS : i51AdeSmsRunHook , sim:%d,num:%s,numlen:%d,text:%x,textlen:%d",sim ,  number , numlen , buffer , textlen) ;
	tick = i51AdeOsGetTick() ;
	for ( looper = 0 ; looper < i51SMSHOOKS_MAX ; looper ++ )
	{
		if ( 0 < (iS32)i51SmsHooker [ looper ].callback )  {
			ret = i51SmsHooker [ looper ].callback ( sim , number , numlen , buffer , textlen ) ;
			if( ret != i51_SMS_HOOK_NORMAL ) {				
                i51AdeOsLog(0, "SMS : i51AdeSmsRunHook , have Delete , ret = %x" , ret ) ;
			    return ret ;
	   		 }

		}
	}
    i51AdeOsLog(0, "SMS : i51AdeSmsRunHook , run hook need time , %d " ,i51AdeOsGetTick() - tick ) ;

       #ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(0, "SMS : i51AdeSmsRunHook , E:%d",ret) ;
	#endif
	
	return i51_SMS_HOOK_NORMAL ;
  
}

static void i51AdeCreateCB ( iU32 id )
{
	i51AdeOsTimerStop ( i51AdeCreateTimer ) ;
	i51AdeCreateTimer = 0 ;
	if ( i51AdeCallback ) {
		
		i51AdeCallback (1) ;
	}
}
iBOOL i51AdeSmsCreate ( i51ADECALLBACK callback ) { 

	//	临时方案，正式版需要使用timer
	//callback (1) ;
	
	i51AdeCallback = callback ;
       i51AdeCreateTimer = i51AdeOsTimerCreate (0,1) ;
	if ( i51AdeCreateTimer == 0 ) {

		return iFALSE ;
	}
       i51AdeOsTimerStart(i51AdeCreateTimer,i51AdeCreateCB,10);
	return iTRUE ;

}
iBOOL i51AdeSmsRelease ( void ) 
{
    return 0 ; 
}
iBOOL i51AdeSmsGetPhoneBook ( i51PHONEBOOK* phonebook , iU32* length )
{
    //	author : Steven
    //	since : 2012.2.28

  //	参考MMIPB_InitEntryList

#ifdef SPRD_6530
    iU8           sim_type  = MN_DUAL_SYS_1;
    iU16                     i=0;
    iU16 PhoneNumber = 0;
    iU16                  storage_id = 0;
    iU8 lenen = 0;
    iU8 tele_num[30] = {0};
    iU32 phonenumber = 0;

    MMIPB_ACCESS_POINT_T    access_point = {0};
    MMIPB_ERROR_E           ret = MMIPB_ERROR_ERROR;
    MMIPB_CONTACT_T *contact_info = PNULL;  
    MMIPB_STORAGE_INFO_T    storage_info = {0};
    MMISET_LANGUAGE_TYPE_E  language_type = 0;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetPhoneBook , S phonebook:%x,length :%d",phonebook, *length) ;
    #endif

    if(NULL==phonebook)
    {
        //phonenumber = MMIPB_GetEntryCount();
        phonenumber = MMIAPIPB_GetPhoneContactCount()+MMIAPIPB_GetAllSimContactCount();
        * length = phonenumber;
        i51AdeOsLog(0, "SMS : i51AdeSmsGetPhoneBook , E get length :%d", *length) ;
        return iTRUE;
    }
	
    contact_info = (MMIPB_CONTACT_T *)i51AdeOsMalloc(sizeof(MMIPB_CONTACT_T) , __FILE__ , __LINE__);
    if ( contact_info == PNULL ) {
        i51AdeOsLog(0, "SMS : i51AdeSmsGetPhoneBook , Malloc F") ;
		return iFALSE ;
    }

    //sim卡的记录, 
    for(sim_type = MN_DUAL_SYS_1; sim_type < MMI_DUAL_SYS_MAX; sim_type++)
    {
        // 按照entry_id读出sim卡中的所有记录
        if (MMIAPIPHONE_GetSimExistedStatus(sim_type))
        {
            storage_info = MMIPB_GetContactStorageInfo(MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + sim_type, PHONEBOOK_SIM_STORAGE));
            for (i=1; i<= MIN(storage_info.max_record_num, MMIPB_SIM_MAX_RECORD_NUM); i++)
            {
                if(0 == (i%100))
                {
                    SCI_Sleep(1);
                }
                
                access_point.contact.contact_id = i;
                storage_id = (PHONEBOOK_SIM_STORAGE<<8) | ((MMIPB_STORAGE_SIM1+sim_type) & 0X00FF);
                access_point.contact.storage_id = storage_id;
                memset(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
                ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,access_point,sizeof(MMIPB_CONTACT_T),contact_info); //读操作不需要区分usim
                
                if(MMIPB_ERROR_SUCCESS == ret && (MMIPB_IsContactValid(contact_info)))
                {
                    memset ( tele_num , 0 , sizeof( tele_num ) ) ;
                    lenen=MMIAPICOM_GenNetDispNumber(0,contact_info->number[0].number_len , contact_info->number[0].number,tele_num,30);                       
                    memcpy(phonebook[PhoneNumber].number,tele_num,lenen);
                    memcpy(phonebook[PhoneNumber].name, contact_info->name.wstr ,contact_info->name.wstr_len*2 );
                    PhoneNumber++;
                }
            } 
        }
        else
        {
            i51AdeOsLog(0, "SMS : i51AdeSmsGetPhoneBook , sim(%d) recoder not Exist " , sim_type + 1) ;
        }
    }
    
    //nv记录
    storage_info = MMIPB_GetContactStorageInfo(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE , MMIPB_NORMAL_PHONE_CONTACT));
    for (i=1; i<= storage_info.max_record_num; i++)
    {
        if(0 == (i%10))
        {
            SCI_Sleep(1);
        }
        access_point.contact.contact_id = i;
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        access_point.contact.storage_id = storage_id;
        memset(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
        ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,access_point,sizeof(MMIPB_CONTACT_T),contact_info);
        
        if(MMIPB_ERROR_SUCCESS == ret && (MMIPB_IsContactValid(contact_info)))
        {
            memset ( tele_num , 0 , sizeof( tele_num ) ) ;
            lenen=MMIAPICOM_GenNetDispNumber(0,contact_info->number[0].number_len , contact_info->number[0].number,tele_num,30);        
            memcpy(phonebook[PhoneNumber].number,tele_num,lenen);
            memcpy(phonebook[PhoneNumber].name, contact_info->name.wstr ,contact_info->name.wstr_len*2 );
            PhoneNumber++;
        }
    }
    if ( contact_info ) {
        i51AdeOsFree (contact_info) ;
    }

    *length = PhoneNumber;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetPhoneBook , E phonebook:%x,length :%d",phonebook, *length) ;
    #endif

    return iTRUE;

#else
    iU16 PhoneNumber = 0;
    iU16 i = 0;
    iU8 lenen = 0;
    iU8 tele_num[30] = {0};
    iBOOL read_result = FALSE;
    iU8 group_id_array[MMI_DUAL_SYS_MAX] = {0};
    iU32 phonenumber = 0;
    MN_DUAL_SYS_E   sim_type  = MN_DUAL_SYS_1;
    MMIPB_PHONEBOOK_ENTRY_T pb_sim_entry    = {0};

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetPhoneBook , S phonebook:%x,length :%d",phonebook, *length) ;
    #endif
    
    if(NULL==phonebook)
    {
        phonenumber = MMIPB_GetEntryCount();
        * length = phonenumber;
        i51AdeOsLog(0, "SMS : i51AdeSmsGetPhoneBook , E , get length :%d", *length) ;
        return iTRUE;
    }
    
    for(i=MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        group_id_array[i] = PB_GROUP_SIM_1 + (i-MN_DUAL_SYS_1);
    }
    
    //read sim record
    for(sim_type = MN_DUAL_SYS_1; sim_type < MMI_DUAL_SYS_MAX; sim_type++)
    {
        for (i=1; i<=g_mmipb_entry_list.sim_entry_id_t.max_record_num[sim_type]; i++)//max=250
        {
            pb_sim_entry.entry_id = i;
            pb_sim_entry.group_id = group_id_array[sim_type];
            read_result = MMIPB_ReadPhonebookEntry(PHONEBOOK_SIM_STORAGE, &pb_sim_entry);
            if (read_result)
            {
            		  memset ( tele_num , 0 , sizeof( tele_num ) ) ;
                     lenen=MMIAPICOM_GenNetDispNumber(0,pb_sim_entry.number_t[0].number_len ,pb_sim_entry.number_t[0].number,tele_num,30);
                     memcpy(phonebook[PhoneNumber].number,tele_num,lenen);
			  #ifdef SPRD_6800
			  memcpy(phonebook[PhoneNumber].name,pb_sim_entry.union_t.entry_name.wstr,pb_sim_entry.union_t.entry_name.wstr_len*2);
			  #else
                     memcpy(phonebook[PhoneNumber].name,pb_sim_entry.entry_name.wstr,pb_sim_entry.entry_name.wstr_len*2);
			  #endif
                     PhoneNumber++;
            }
        }
    } 
    
    //read nv record
    for (i=1; i<= g_mmipb_entry_list.nv_entry_id_t.max_record_num[MN_DUAL_SYS_1]; i++)//max=500
    {        
        pb_sim_entry.entry_id = i;
        read_result = MMIPB_ReadPhonebookEntry(PHONEBOOK_NV_STORAGE, &pb_sim_entry);
        if (read_result)
        {
                memset ( tele_num , 0 , sizeof( tele_num ) ) ;
                lenen=MMIAPICOM_GenNetDispNumber(0,pb_sim_entry.number_t[0].number_len ,pb_sim_entry.number_t[0].number,tele_num,30);
                memcpy(phonebook[PhoneNumber].number,tele_num,lenen);
		   #ifdef SPRD_6800
		   memcpy(phonebook[PhoneNumber].name,pb_sim_entry.union_t.entry_name.wstr,pb_sim_entry.union_t.entry_name.wstr_len*2);
		   #else
                memcpy(phonebook[PhoneNumber].name,pb_sim_entry.entry_name.wstr,pb_sim_entry.entry_name.wstr_len*2);
		   #endif
                PhoneNumber++;

        }    
    }
    
    *length = PhoneNumber;

    #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SMS : i51AdeSmsGetPhoneBook , E , phonebook:%x,length :%d",phonebook, *length) ;
    #endif

    return iTRUE;
	
#endif
}
