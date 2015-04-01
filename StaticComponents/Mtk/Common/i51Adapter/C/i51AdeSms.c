
 
/*

	The Implementing of 'Adapter' , Module : 'SMS'.	
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
#include "i51AdeSms.h"	
#include "mmi_include.h"
#include "kal_release.h"
#include "MMIDataType.h"
#include "custom_mmi_default_value.h"

#if( I51_MTK_VS >=VS_09B )
#include "smssrvgprot.h"
#include "smscuigprot.h"
#include "smslib_enum.h"
#include "smslib_def.h"
#include "PhbSrvGprot.h"
#include "..\..\plutommi\Service\SmsSrv\SmsStorageSrv.h"
#elif( I51_MTK_VS <=VS_09A )
#include "ProtocolEvents.h"
#include "MessagesMiscell.H"
#include "SMSApi.h"
#include "SMSStruct.h"
#include "wgui_status_icons.h"
#include "PhoneBookGprot.h"
#endif

#if( I51_MTK_VS >=VS_W11 )
#include "SimCtrlSrvGprot.h"
#include "ModeSwitchSrvGProt.h"
#endif

#if( I51_MTK_VS <=VS_W10 )
#ifdef __MMI_DUAL_SIM_MASTER__        
#include "mtpnp_ad_master_common_def.h"
#include "mtpnp_ad_master_if.h"
#if( I51_MTK_VS <=VS_09B )
#if !defined(MTK_6235_09B)
#include "Mtpnp_pfal_messagesetup.h"
#endif
#endif
#endif
#endif

#if( I51_MTK_VS >VS_0812 )
#include "gui_typedef.h"
#endif
#include "msf_def.h"
#include "mms_def.h"
#include "msig.h"
#include "fldmgr.h"
#ifdef __MMI_MMS_2__
#include "MmsSrvGprot.h"
#endif
#include "mms_def.h"
#include "msf_int.h"

#define MAX_SMS_HOOK_COUNT	5 
#define SRV_SMS_INVALID_NUM   0xff

#define CODE_TYPE_ASCII    1  
#define CODE_TYPE_UCS2    2  

#ifdef __MMI_DUAL_SIM_MASTER__
#define CAN_INIT_COUNT 5
#else
#define CAN_INIT_COUNT 3
#endif


typedef void (*I51ADASMSCALLBACK)  ( iBOOL value ) ;
static iU32 InitCount ;
static iBOOL GetIMEI ;
static iBOOL GetIMSI1 ;
static iBOOL GetIMSI2 ;
static iBOOL GetSC1 ;
static iBOOL GetSC2 ;


//buf lenght 17 use 15
static iCHAR	i51imei[17] ;

//buf lenght 17 use 15
static iCHAR i51imsi1[17] ;

//buf lenght 17 use 15
static iCHAR i51imsi2[17] ;

static iCHAR i51sc1[41];
static iCHAR i51sc2[41];




static I51ADASMSCALLBACK i51sendSmsCB[5] ;
static int i51CurSendDev = 0;

static i51ADECALLBACK i51AdaCreateCB ;

i51SMSHKR i51adaMmsState=0;

typedef struct {

	iBOOL used ;
	i51SMSHOOKCALLBACK i51SmsHookCB ;

 }I51ADASMSHOOK ;
I51ADASMSHOOK i51SmsHookList[MAX_SMS_HOOK_COUNT] = { 0 } ;

static void CheckInitOverCB(void)
{
    InitCount++ ;
	if ( InitCount == CAN_INIT_COUNT ) {

		if ( GetIMEI && (GetIMSI1 ||GetIMSI2) && (GetSC1 ||GetSC2) ) {
			if (i51AdaCreateCB )
				i51AdaCreateCB ( iTRUE) ;
			
		} else {
			if (i51AdaCreateCB )
				i51AdaCreateCB ( iFALSE ) ;

		}
		i51AdaCreateCB = iNULL ;

	}
}	

#if( I51_MTK_VS <=VS_09B )
static void i51GetImeiRsp (void *info)
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	
	mmi_nw_get_imei_rsp_struct  * rsp = (mmi_nw_get_imei_rsp_struct *) info;	
	SetProtocolEventHandler ( iNULL , MSG_ID_MMI_NW_GET_IMEI_RSP ) ;

    GetIMEI = iTRUE ;
	if (rsp->result == 1) {		
		memcpy (i51imei, rsp->imei, 15);
	}
    CheckInitOverCB();
}

static void i51GetImsi1Rsp ( void *info )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	
	mmi_smu_get_imsi_rsp_struct *rsp = ( mmi_smu_get_imsi_rsp_struct * ) info;

 	SetProtocolEventHandler ( iNULL , MSG_ID_MMI_SMU_GET_IMSI_RSP ) ;

    GetIMSI1 = iTRUE ;
	if ( rsp->result == 1 ) {
        memcpy ( i51imsi1 , rsp->imsi+1 , 15 ) ;	
    }
	
	CheckInitOverCB();
}

static void i51GetImsi2Rsp ( void *info )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	
    #ifdef __MMI_DUAL_SIM_MASTER__
    {
		mmi_smu_get_imsi_rsp_struct *rsp = (mmi_smu_get_imsi_rsp_struct*) info ;

		SetSlaveProtocolEventHandler ( iNULL , MSG_ID_MMI_SMU_GET_IMSI_RSP ) ;

        GetIMSI2 = iTRUE ;
		if ( rsp->result == 1 ) {
             memcpy ( i51imsi2 , rsp->imsi+1 , 15 ) ;			 
		} 
        CheckInitOverCB();
    }
    #endif
}
#endif

//************ "sc get" code start ****************
#if( I51_MTK_VS <=VS_09A )

static void i51SmsGetSc1Rsp (void *info)
{
	MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT *rsp = (MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT *)info;

	ClearProtocolEventHandler (PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
    GetSC1 = iTRUE ;
	if (rsp->result == TRUE) {
    	memcpy (i51sc1, rsp->sc_addr.number, rsp->sc_addr.length);
	}

    CheckInitOverCB();
}

static void i51_sms_get_sc1_req_6225_625309a_623509a (void)
{
    MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT *msgReq;
    S16 errorCode = 0;
    S8 nvramActiveProfileIndex = 0;

    ReadValue(NVRAM_SMS_ACTIVE_PROFILE_INDEX, &nvramActiveProfileIndex, DS_BYTE, &errorCode);

    msgReq = (MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*) OslConstructDataPtr(sizeof(MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT));
    msgReq->profile_no = nvramActiveProfileIndex;
    SetProtocolEventHandler (i51SmsGetSc1Rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
    mmi_frm_sms_send_message(MOD_MMI,
                             MOD_L4C,
                             0,
                             PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ,
                             (oslParaType*) msgReq,
                             iNULL);
}


static void i51SmsGetSc2Rsp (void *info)
{
#ifdef __MMI_DUAL_SIM_MASTER__
	ST_MTPNP_AD_SMS_GET_PROFILE_PARAMS_RSP *msgRsp = (ST_MTPNP_AD_SMS_GET_PROFILE_PARAMS_RSP *) info;

	MTPNP_PFAL_Remove_Event_Handler (MTPNP_PFAL_MSG_SMS_GET_PROFILE_PARAMS_RSP);

    GetSC2 = iTRUE ;
	if (msgRsp->result == TRUE) {
	    memcpy (i51sc2, msgRsp->sc_addr.number, msgRsp->sc_addr.length);
	}

    CheckInitOverCB();
#endif

}


static void i51_sms_get_sc2_req_6225_625309a_623509a (void)
{

#ifdef __MMI_DUAL_SIM_MASTER__
    MTPNP_PFAL_Register_Event_Handler (MTPNP_PFAL_MSG_SMS_GET_PROFILE_PARAMS_RSP, i51SmsGetSc2Rsp);
    MTPNP_AD_SMS_Get_Profile_Params_Req (0);

#endif
}

#elif( I51_MTK_VS >=VS_09B )
 
static void i51SmsGetSc1Rsp (srv_sms_callback_struct* callback_data)
{
	if (callback_data->result == MMI_TRUE) {

		GetSC1 = iTRUE ;
		memset (i51sc1, 0, sizeof(i51sc1));
		memcpy((S8*)i51sc1, (S8*)callback_data->action_data, sizeof(i51sc1));
        i51AdeOsLog ( i51_LOG_DC , "SMS : i51SmsGetSc1Rsp , i51sc1 = %s" , i51sc1) ;
	} else {

	GetSC1 = iFALSE ;

	}

    CheckInitOverCB();
}


 static void i51_sms_get_sc1_req_625309b(void)
{

    srv_sms_get_setting_para(SRV_SMS_ACTIVE_SC_ADDR, 
                             iNULL,
                             SRV_SMS_SIM_1, 
                             i51SmsGetSc1Rsp, 
                             iNULL);
}

static void i51SmsGetSc2Rsp (srv_sms_callback_struct* callback_data)
{
#ifdef __MMI_DUAL_SIM_MASTER__
	if (callback_data->result == MMI_TRUE) {
		
		 GetSC2 = iTRUE ; 
		memset (i51sc2, 0, sizeof(i51sc2));
		memcpy((S8*)i51sc2, (S8*)callback_data->action_data, sizeof(i51sc2));
        i51AdeOsLog ( i51_LOG_DC , "SMS : i51SmsGetSc2Rsp , i51sc2 = %s" , i51sc2) ;
	} else {

		GetSC2 = iFALSE ;

	}
    CheckInitOverCB();
#endif
}

 static void i51_sms_get_sc2_req_625309b (void)
{    
    srv_sms_get_setting_para(SRV_SMS_ACTIVE_SC_ADDR, 
                             iNULL,
                             SRV_SMS_SIM_2, 
                             i51SmsGetSc2Rsp, 
                             iNULL);
}

#endif 
//************ "sc get" code end ****************

static  void i51GetImeiReq ()
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	//	获取IMIE请求


#if( I51_MTK_VS <=VS_09B )
	ilm_struct Message;

	SetProtocolEventHandler ( i51GetImeiRsp , MSG_ID_MMI_NW_GET_IMEI_RSP ) ;
	Message.oslSrcId   = MOD_MMI ;
	Message.oslDestId  = MOD_L4C ;
	Message.oslMsgId   = MSG_ID_MMI_NW_GET_IMEI_REQ ;
	Message.oslDataPtr = iNULL ;
	Message.oslPeerBuffPtr = iNULL ;
	OslMsgSendExtQueue ( &Message ) ;
	
#else

    iS32 *cardSelect = 0 ;
    MMI_BOOL ret ;
   
	
    memset( i51imei , 0, sizeof( i51imei ) ) ;
//    cardSelect = i51SimCardSelect ( 0 ) ;
    cardSelect = i51AdeSmsGetSims() ;


    if ( 1 == cardSelect[1] ) {
		
        ret = srv_imei_get_imei (MMI_SIM1 , ( char * ) i51imei , sizeof ( i51imei ) ) ;
    } else {
    
        ret = srv_imei_get_imei( MMI_SIM2 , ( char * ) i51imei , sizeof ( i51imei ) ) ;
		
    }    

    GetIMEI = iTRUE ;
	CheckInitOverCB();
#endif
}

static void i51GetImsi1Req ( void )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	//	获取卡一IMSI 请求


#if( I51_MTK_VS >=VS_W10 )

    unsigned char imsi[17] ;
    memset ( imsi , 0 , sizeof(imsi) ) ;
	
    srv_sim_ctrl_get_imsi (MMI_SIM1 , (char *) imsi , 17 ) ;
    memcpy ( i51imsi1 , imsi + 1 , 15 ) ;

    GetIMSI1 = iTRUE;
    CheckInitOverCB();
#else    

	ilm_struct Message;

	SetProtocolEventHandler ( i51GetImsi1Rsp , MSG_ID_MMI_SMU_GET_IMSI_RSP ) ;

	Message.oslMsgId  = MSG_ID_MMI_SMU_GET_IMSI_REQ ;
	Message.oslSrcId  = MOD_MMI ;
	Message.oslDestId = MOD_L4C ;	
	Message.oslSapId  = MMI_L4C_SAP ;
	Message.oslDataPtr = iNULL ;
	Message.oslPeerBuffPtr = iNULL ;
	OslMsgSendExtQueue ( &Message ) ;	
	

#endif
}

static void i51GetImsi2Req ( void )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	//	获取卡二IMSI 请求
	
#ifdef __MMI_DUAL_SIM_MASTER__
#if( I51_MTK_VS >=VS_W10 )
	    unsigned char imsi[17] ;

	    memset ( imsi , 0 , sizeof(imsi) ) ;
	    srv_sim_ctrl_get_imsi ( MMI_SIM2 , (char*)imsi , 17 ) ;
	    memcpy ( i51imsi2 , imsi + 1 , 15 ) ;
        
        GetIMSI2 = TRUE;
        CheckInitOverCB();
#else
	    ilm_struct Message ;

	    SetSlaveProtocolEventHandler ( i51GetImsi2Rsp , MSG_ID_MMI_SMU_GET_IMSI_RSP ) ;

	    Message.oslMsgId  = MSG_ID_MMI_SMU_GET_IMSI_REQ ;
	    Message.oslSrcId  = MOD_MMI ;
	    Message.oslDestId = MOD_L4C_2 ;
	    Message.oslDataPtr = iNULL ;
	    Message.oslPeerBuffPtr = iNULL ;
	    OslMsgSendExtQueue ( &Message ) ;	
#endif
#else

	//
#endif
}


static void i51SmsGetSc1Req (void)
{

	
	//	author : william
	//	since : 20110829
	//	PKIG Tech. Co., Ltd.
	//	获取卡一短信中心号码的请求
	
    #if( I51_MTK_VS <=VS_09A )
    i51_sms_get_sc1_req_6225_625309a_623509a ();
    #endif

    #if( I51_MTK_VS >=VS_09B )
     i51_sms_get_sc1_req_625309b ();
    #endif
}

void i51SmsGetSc2Req (void)
{
    #ifdef __MMI_DUAL_SIM_MASTER__
    {
        #if( I51_MTK_VS <=VS_09A )
        i51_sms_get_sc2_req_6225_625309a_623509a ();
        #endif
        
        #if( I51_MTK_VS >=VS_09B )
        i51_sms_get_sc2_req_625309b ();
        #endif
    }

    #endif
}


#if( I51_MTK_VS <=VS_09A )
//sel sim in i51_sms_send_6225_625309a_623509a function internal code already
#elif( I51_MTK_VS <=VS_W10 )
static void i51SendSmsSelSim(SMS_HANDLE sms_handler)
{
#ifdef __MMI_DUAL_SIM_MASTER__
    {
        E_MTPNP_AD_SIMCARD_USABLE_SIDE state = MTPNP_AD_Get_UsableSide();

        switch (state)
        {
            case MTPNP_AD_DUALSIM_USABLE:
                if (mmi_bootup_is_nw_registration_ok()) 
                {
                    srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_1);
                }
                else if(mmi_bootup_is_sim2_nw_registration_ok())
                {
                    srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_2);
                }
                break;

            case MTPNP_AD_SIMCARD1_USABLE:
                if (mmi_bootup_is_nw_registration_ok()) 
                {
                    srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_1);              
                }
                break;

            case MTPNP_AD_SIMCARD2_USABLE:
                if(mmi_bootup_is_sim2_nw_registration_ok())
                {
                    srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_2);
                }
                break;

            default:
                break;
        }
    }
#else
    if (mmi_bootup_is_nw_registration_ok()) 
    {
        srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_1);
    }
#endif    
}

#elif( I51_MTK_VS >=VS_W11 )

static void i51SendSmsSelSim(SMS_HANDLE sms_handler)
{
    if( srv_sim_ctrl_is_available( MMI_SIM1 ) )
    {
        if(!srv_nw_info_sim_is_rejected(MMI_SIM1))
        {
            srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_1);
            return;
        }        
    }
    #if (MMI_MAX_SIM_NUM >= 2)
    if( srv_sim_ctrl_is_available( MMI_SIM2 ) )
    {            
        if(!srv_nw_info_sim_is_rejected(MMI_SIM2))
        {
            srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_2);
            return;
        }        
    }    
    #endif 
    #if (MMI_MAX_SIM_NUM >= 3)
    if( srv_sim_ctrl_is_available( MMI_SIM3 ) )
    {            
        if(!srv_nw_info_sim_is_rejected(MMI_SIM3))
        {
            srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_3);
            return;
        }        
    }    
    #endif 
    #if (MMI_MAX_SIM_NUM >= 4)
    if( srv_sim_ctrl_is_available( MMI_SIM4 ) )
    {            
        if(!srv_nw_info_sim_is_rejected(MMI_SIM4))
        {
            srv_sms_set_sim_id(sms_handler, SRV_SMS_SIM_4);
            return;
        }        
    }    
    #endif     
}

#endif

static iBOOL SmsSendResult;
static void SmsSendAsynTimerCB( iU32 id  ) 
{
    if ( i51CurSendDev && i51sendSmsCB[i51CurSendDev-1] )
        i51sendSmsCB[i51CurSendDev-1] (SmsSendResult);
    i51sendSmsCB[i51CurSendDev-1] = NULL;
    i51CurSendDev = 0;
    i51AdeOsTimerStop(id);
}
static void SmsSendFailedTimerCB( iU32 id  ) 
{
    int i;
    for( i=0;i<5;i++ )
    {
        if( i== i51CurSendDev-1 ) continue;
        if ( i51sendSmsCB[i] ){
            i51sendSmsCB[i] (iFALSE);
            i51sendSmsCB[i] = NULL;
        }
    }
    i51AdeOsTimerStop(id);
}

#if( I51_MTK_VS <=VS_09A )
static void i51_sms_send_6225_625309a_623509a_callback (void *dummy, module_type mod, U16 result)
{
    if ( result == MMI_FRM_SMS_OK ) {
        SmsSendResult = iTRUE;		
    } else {
        SmsSendResult = iFALSE;
    }
    i51AdeOsTimerStart( i51AdeOsTimerCreate( iFALSE, 1), SmsSendAsynTimerCB, 1 );//防止同步回调
}

static int i51_sms_send_6225_625309a_623509a (unsigned char *pNumber, unsigned char CodingType, unsigned char *pMessage , I51ADASMSCALLBACK callback)
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	
    mmi_frm_sms_send_struct sendData;

    if (pMessage == iNULL || pNumber == iNULL || CodingType > CODE_TYPE_UCS2) { return -1; }

    if (IsMessagesReEntering()) { return -2; }

    memset((void*)&sendData, 0, sizeof(mmi_frm_sms_send_struct));

    if (CodingType == CODE_TYPE_ASCII)
    {
        sendData.stringlength = strlen((const char*)pMessage);
        sendData.string = OslMalloc ((sendData.stringlength + 1)*2);
        mmi_asc_to_ucs2 ((char*)sendData.string, (char*)pMessage);
        sendData.dcs = SMSAL_DEFAULT_DCS;    
    }
    else if (CodingType == CODE_TYPE_UCS2)
    {
        sendData.stringlength = mmi_ucs2strlen((const char*)pMessage);
        sendData.string = OslMalloc ((sendData.stringlength + 1) * 2);
        mmi_ucs2cpy ((char*)sendData.string, (char*)pMessage);
        sendData.dcs = SMSAL_UCS2_DCS;
    }
    
    sendData.sendcheck = MMI_FRM_SMS_NOT_ALLOW_EMPTY_SC | MMI_FRM_SMS_SCR;

    #if( (I51_MTK_VS >=VS_08B)&& (I51_MTK_VS <=VS_09A ))
    sendData.sendrequire |= MMI_FRM_SMS_NOT_DISP_SENDING_ICON;
    #endif

    strcpy((char *)sendData.number, (const char *)pNumber);

#ifdef __MMI_DUAL_SIM_MASTER__
    {
        E_MTPNP_AD_SIMCARD_USABLE_SIDE state = MTPNP_AD_Get_UsableSide();
        
        MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
        switch (state)
        {
            case MTPNP_AD_DUALSIM_USABLE:
                if (mmi_bootup_is_nw_registration_ok()) 
                {
                    MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
                }
                else if(mmi_bootup_is_sim2_nw_registration_ok())
                {
                    MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
                }
                break;

            case MTPNP_AD_SIMCARD1_USABLE:
                if (mmi_bootup_is_nw_registration_ok()) 
                {
                    MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
                }
                break;

            case MTPNP_AD_SIMCARD2_USABLE:
                if(mmi_bootup_is_sim2_nw_registration_ok())
                {
                    MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);	
                }
                break;

            default:
                break;
        }
    }
#endif

  //  i51_sms_status_report(0); 

    mmi_frm_sms_send_sms ((PsFuncPtrU16)i51_sms_send_6225_625309a_623509a_callback, MOD_MMI, &sendData);

    return 1;    
}
#elif( I51_MTK_VS >=VS_09B )
static void i51_sms_send_625309b_callback( srv_sms_callback_struct* callback_data )
{    
    if ( callback_data->result == MMI_TRUE ) {
        SmsSendResult = iTRUE;		
    } else {
        SmsSendResult = iFALSE;
    }
    i51AdeOsTimerStart( i51AdeOsTimerCreate( iFALSE, 1), SmsSendAsynTimerCB, 1 );//防止同步回调

}
static int i51_sms_send_625309b (unsigned char *pNumber, unsigned char CodingType, unsigned char *pMessage , I51ADASMSCALLBACK callback)
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	
    SMS_HANDLE sms_handler;
    S8 sms_addr[(CUI_SMS_MAX_ADDR_LEN + 1) *ENCODING_LENGTH];
    S8* content_ucs2_buffer = iNULL;
    U8 content_asc_str_len = 0;

    if (pMessage == iNULL || pNumber == iNULL || CodingType > CODE_TYPE_UCS2) { return -1; }
    
	#if( I51_MTK_VS >=VS_11A_1112 )
		if(srv_sms_is_bg_send_action_busy()||srv_sms_is_sms_busy())	{ return -2; }
	#else
        if (IsMessagesReEntering()) { return -2; }
    #endif

    sms_handler = srv_sms_get_send_handle();
    //set number
    memset(sms_addr, 0, sizeof(sms_addr));
    mmi_asc_n_to_ucs2((S8*)sms_addr, (S8*)pNumber, strlen((const char*)pNumber));
    srv_sms_set_address(sms_handler, sms_addr);
    srv_sms_set_status_report(sms_handler, MMI_FALSE );
    srv_sms_set_not_allow_empty_sc(sms_handler);
    srv_sms_set_no_sending_icon(sms_handler);

    srv_sms_set_content_dcs(sms_handler, SRV_SMS_DCS_UCS2);
    if (CodingType == CODE_TYPE_ASCII)
    {
        content_asc_str_len = strlen((const char*)pMessage);
        content_ucs2_buffer = OslMalloc ((content_asc_str_len+1)*2);
        mmi_asc_to_ucs2 ((char*)content_ucs2_buffer, (char*)pMessage);
    }
    else if (CodingType == CODE_TYPE_UCS2)
    {
        content_asc_str_len = mmi_ucs2strlen((const char*)pMessage);
        content_ucs2_buffer = OslMalloc ((content_asc_str_len+1) * 2);
        mmi_ucs2cpy ((char*)content_ucs2_buffer, (char*)pMessage);
    }
    srv_sms_set_content(sms_handler, content_ucs2_buffer, ((content_asc_str_len + 1) * 2));

    i51SendSmsSelSim(sms_handler);
    
    srv_sms_send_msg(sms_handler, i51_sms_send_625309b_callback, iNULL);

    OslMfree(content_ucs2_buffer);
    return 1;    
}
#endif

iCHAR* i51AdeSmsGetImsi ( iS32 sim )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	iCHAR *pImsi = NULL ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsGetImsi , S , %x" , sim ) ;
	#endif	
	
	
	if ( sim == 1 ) {

		pImsi =  i51imsi1 ;
	}else if ( sim == 2 ) {

		pImsi =  i51imsi2 ;
	}else {

		pImsi =  iNULL ;
	}

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsGetImsi , E , ret = %x" , pImsi ) ;
	#endif	
	
	return pImsi ;

	

}

iCHAR* i51AdeSmsGetImei ( void )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsGetImei , S" ) ;
	#endif	

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsGetImei , E , ret = %x" , i51imei ) ;
	#endif	
	
	return i51imei ;

}

iCHAR* i51AdeSmsGetCenter ( iS32 sim ) 
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.

	static iCHAR i51Getsc1[41] = {0};
	static iCHAR i51Getsc2[41] = {0};
	iCHAR *pSc = NULL ;
	

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsGetCenter , S , %x" , sim ) ;
	#endif	
	
	memset ( i51Getsc1 , 0 , sizeof ( i51Getsc1 ) ) ;
	memset ( i51Getsc2 , 0 , sizeof ( i51Getsc2 ) ) ;

	if ( i51sc1[0] &&  i51sc1[1] ) {

		strcpy ( i51Getsc1 , i51sc1 ) ;
	} else {

		mmi_ucs2_to_asc ( i51Getsc1 , i51sc1 ) ;
	}

	if ( i51sc2[0] &&  i51sc2[1] ) {

		strcpy ( i51Getsc2 , i51sc2 ) ;
	} else {

		mmi_ucs2_to_asc ( i51Getsc2 , i51sc2 ) ;
	}


	if ( sim == 1 )
		pSc =  i51Getsc1 ;
	else if ( sim == 2 )
		pSc =  i51Getsc2 ;
	else
		pSc = NULL ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsGetCenter , E , ret = %x" , pSc ) ;
	#endif	
	
	return pSc ;
	
}

iU32 i51AdeSmsCreateDevice () 
{
    int i;
    for(i=0;i<5;i++)
    {
        if( !i51sendSmsCB[i] ) break;
    }
    if(i==5) return 0;
	else return i+1 ;
}

iBOOL i51AdeSmsSend ( iU32 device , iS32 sim , iCHAR* phone , iU16* contents , iBOOL save , void (*callback)( iBOOL results ) )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	/**
	* @fn		int I51_sms_send (unsigned char *pNumber, unsigned char CodingType, unsigned char *pMessage, int oid, FSMSSendCallback *callback)
	* @brief	短信发送
	* 
	* @param	pNumber		号码
	* @param	CodingType	编码
	* @param	pMessage	短消息正文
	* @param	oid			发送短消息的应用oid
	* @param	callback	短信消息回调
	* 
	* @retval	1	OK
	* @retval	<0	Error
	* 
	* @note
	* @li	CodingType取值：CODE_TYPE_ASCII, CODE_TYPE_UCS2
	*/

	iBOOL ret = 0 ;

    if( device<1 || device>5 || !callback ){
        return iFALSE;
    }

    if( i51CurSendDev ){
        i51sendSmsCB[device-1] = callback;
        i51AdeOsTimerStart( i51AdeOsTimerCreate( iFALSE, 1), SmsSendFailedTimerCB, 1 );//防止同步回调
        return iTRUE;
    }
	i51CurSendDev = device ; 
    i51sendSmsCB[i51CurSendDev-1] = callback ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsSend , S , %x , %x , %x , %x , %x , %x" , device , sim , phone , contents , save , callback ) ;
	#endif	
	
    #if( I51_MTK_VS <=VS_09A )
   	 ret = i51_sms_send_6225_625309a_623509a ( ( iU8* )phone , CODE_TYPE_UCS2 ,( iU8* )contents , callback ) ;
    #endif
    
    #if( I51_MTK_VS >=VS_09B )
    	ret = i51_sms_send_625309b ( ( iU8* ) phone , CODE_TYPE_UCS2 , ( iU8* ) contents , callback ) ;
    #endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsSend , E , ret = %x" , ret ) ;
	#endif	

    return ret ;
  
}
iBOOL i51AdeSmsSendEx ( iU32 device , iS32 sim , iCHAR* phone , iCHAR* contents , iBOOL unicode , iBOOL save , void (*callback)( iBOOL results ) ) 
{
	//	author : jons
	//	since : 20120319
	//	PKIG Tech. Co., Ltd.
	/**
	*
	* @brief	短信发送,包含CODE_TYPE_ASCII编码和CODE_TYPE_UCS2编码2种
	*
	* @param	device		暂时没用
	* @param	sim			sim卡号，暂时没用
	* @param	phone		电话号码
	* @param	contents		短消息正文	
	* @param	unicode		是否CODE_TYPE_UCS2编码
	* @param	save			是否保存，暂时没用
	* @param	callback		短信消息回调
	* 
	* @retval	1	OK
	* @retval	<0	Error
	* 
	* @note
	* @CodingType取值：CODE_TYPE_ASCII, CODE_TYPE_UCS2
	*/

	iBOOL ret = 0 ;
	iU8 CodingType ;

    if( device<1 || device>5 || !callback ){
        return iFALSE;
    }
    if( i51CurSendDev ){
        i51sendSmsCB[device-1] = callback;
        i51AdeOsTimerStart( i51AdeOsTimerCreate( iFALSE, 1), SmsSendFailedTimerCB, 1 );//防止同步回调
        return iTRUE;
    }
	i51CurSendDev = device ; 
    i51sendSmsCB[i51CurSendDev-1] = callback ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsSendEx , S ,  phone_number=%s , %#x , %d , %#x" ,  phone , contents , save , callback ) ;
	#endif	
	
	if(unicode == iTRUE)
		CodingType = CODE_TYPE_UCS2 ;
	else
		CodingType = CODE_TYPE_ASCII ;
	
    #if( I51_MTK_VS <=VS_09A )
   	 ret = i51_sms_send_6225_625309a_623509a ( ( iU8* )phone , CodingType ,( iU8* )contents , callback ) ;
    #endif
    
    #if( I51_MTK_VS >=VS_09B )
    	ret = i51_sms_send_625309b ( ( iU8* ) phone , CodingType , ( iU8* ) contents , callback ) ;
    #endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsSendEx , E , ret = %x" , ret ) ;
	#endif	
		
	return ret ;
}

iBOOL i51AdeSmsReleaseDevice ( iU32 device ) 
{
    if( device>=1 && device<=5){
        i51sendSmsCB[ device - 1 ] = NULL;
        if( i51CurSendDev == device ) 
            i51CurSendDev = 0;
    }
	return iTRUE ;
}

iS32* i51AdeSmsGetSims ( void )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.

	//获取当前可用的SIM卡编号

	//存储sim卡的信息simCardInfo[0] 代表sim个数，后面表示sim卡编号
	static iS32	simCardInfo[5] = { 0 } ;
	
#if( I51_MTK_VS <=VS_W10 )
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsReleaseDevice , S -1 " ) ;
	#endif	

    memset ( simCardInfo , 0 , sizeof(simCardInfo)) ;
	#ifdef __MMI_DUAL_SIM_MASTER__
	    switch (MTPNP_AD_Get_UsableSide())
	    {	    
	        case MTPNP_AD_DUALSIM_USABLE:
			simCardInfo[0] = 2 ;
			simCardInfo[1] = 1 ;
			simCardInfo[2] = 2;
	            break;
	        case MTPNP_AD_SIMCARD1_USABLE:
	     		simCardInfo[0] = 1 ;
			simCardInfo[1] = 1 ;
	            break;
	        case MTPNP_AD_SIMCARD2_USABLE:
	          	simCardInfo[0] = 1 ;
			simCardInfo[1] = 2 ;
	            break;
				
	    }

	#else
		simCardInfo[0] =  1 ;
		simCardInfo[1] =  1 ;
	#endif

#else

    srv_mode_switch_type_enum  sw_mode = srv_mode_switch_get_current_mode();
    int i = 1;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsReleaseDevice , S -2 " ) ;
	#endif	
	
    memset ( simCardInfo , 0 , sizeof(simCardInfo)) ;
    if( srv_sim_ctrl_is_available(MMI_SIM1) && (sw_mode&SRV_MODE_SWITCH_SIM1) )
    {
        simCardInfo[i++] = 1;
        simCardInfo[0]++;
    }
    #if (MMI_MAX_SIM_NUM >= 2)
    if( srv_sim_ctrl_is_available(MMI_SIM2) && (sw_mode&SRV_MODE_SWITCH_SIM2) )
    {
        simCardInfo[i++] = 2;
        simCardInfo[0]++;
    } 
    #endif   
    #if (MMI_MAX_SIM_NUM >= 3)  
    if( srv_sim_ctrl_is_available(MMI_SIM3) && (sw_mode&SRV_MODE_SWITCH_SIM3) )
    {
        simCardInfo[i++] = 3;
        simCardInfo[0]++;
    } 
    #endif 
    #if (MMI_MAX_SIM_NUM >= 4)  
    if( srv_sim_ctrl_is_available(MMI_SIM4) && (sw_mode&SRV_MODE_SWITCH_SIM4) )
    {
        simCardInfo[i++] = 4;
        simCardInfo[0]++;
    }   
    #endif

#endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsReleaseDevice , E , ret = %x" , simCardInfo ) ;
	#endif	

	return simCardInfo ;

}

//111111111111111111111111111111111111111111111111111111111111111111111111111111111111
#if( I51_MTK_VS <=VS_09A )
void i51_del_sms_cb (void *pdata, module_type unknow1, U16 unknow2){   }

/**
 * @fn		static void i51SmsMasterInd (void *inMsg)
 * @brief	主卡的短消息通知函数
 */
int i51SmsMasterInd (void *inMsg)
{
	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
    MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT *msgInd = (MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT*) inMsg;

    if (inMsg == NULL) { return  0; }
    i51AdeOsLog(1, "SMS : i51SmsMasterInd , recv sms len=%d", msgInd->no_msg_data );
    if (msgInd->no_msg_data > 0) 
    {
        int msg_len=0;
        unsigned char *pData;
            
        //i51AdeOsBufLog(1, msgInd->msg_data, msgInd->no_msg_data, 1);
        //i51AdeOsBufLog(1, msgInd->msg_data, msgInd->no_msg_data, 0); 
        if (msgInd->dcs != SMSAL_UCS2_DCS)
        {
            pData = (unsigned char *)OslMalloc(msgInd->no_msg_data*2 + 2);
            msg_len = msgInd->no_msg_data;
            mmi_asc_n_to_ucs2( (S8*)pData, (S8*)msgInd->msg_data, msg_len );
        }
        else
        {
            pData = (unsigned char *)OslMalloc(msgInd->no_msg_data + 2);
            msg_len = msgInd->no_msg_data/2;
            memcpy(pData, msgInd->msg_data,msgInd->no_msg_data);
        }
        if( i51AdeSmsRunHook( 0, (iCHAR*)msgInd->oa_number.number, (iU32)strlen((iCHAR*)msgInd->oa_number.number), 
               (iU16*)pData, msg_len ) == i51_SMS_HOOK_DELETE )
        {
            mmi_frm_sms_delete_sms(i51_del_sms_cb, MOD_MMI, MMI_FRM_SMS_APP_AWAITS, msgInd->index); 
            OslMfree(pData);
            i51AdeOsLog(1,"SMS : sms deled, return");
            return 1;    
        }      
        OslMfree(pData);        
    }

    i51AdeOsLog(1,"SMS : sms no del, return");
    return 0;
}

/**
 * @fn		static void i51SmsSlaveInd (void *inMsg)
 * @brief	附卡的短消息通知函数
 */
#ifdef __MMI_DUAL_SIM_MASTER__   
int i51SmsSlaveInd (void *inMsg) 
{
	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
    ST_MTPNP_AD_DELIVER_SMS *msgInd = (ST_MTPNP_AD_DELIVER_SMS *) inMsg;

    if (inMsg == NULL) { return  0; }
    i51AdeOsLog(1, "SMS : recv sms len=%d", msgInd->no_msg_data );
    if ( msgInd->no_msg_data > 0 )
    {
        int msg_len=0;
        unsigned char *pData;
        
        //i51AdeOsBufLog(1, msgInd->msg_data, msgInd->no_msg_data, 1);
        //i51AdeOsBufLog(1, msgInd->msg_data, msgInd->no_msg_data, 0); 
            
        if (msgInd->dcs != MTPNP_CODING_SCHEME_UCS2)
        {
            pData = (unsigned char *)OslMalloc(msgInd->no_msg_data*2 + 2);
            msg_len = msgInd->no_msg_data;
            mmi_asc_n_to_ucs2( (S8*)pData, (S8*)msgInd->msg_data, msg_len );
        }
        else
        {
            pData = (unsigned char *)OslMalloc(msgInd->no_msg_data + 2);
            msg_len = msgInd->no_msg_data/2;
            memcpy(pData, msgInd->msg_data,msgInd->no_msg_data);
        }
        if( i51AdeSmsRunHook( 0, (iCHAR*)msgInd->oa_number.number, (iU32)strlen((iCHAR*)msgInd->oa_number.number), 
               (iU16*)pData, msg_len ) == i51_SMS_HOOK_DELETE )
        {
            mmi_frm_sms_delete_sms(i51_del_sms_cb, MOD_MMI, MMI_FRM_SMS_APP_AWAITS, msgInd->index); 
            OslMfree(pData);
            i51AdeOsLog(1,"SMS : sms deled, return");
            return 1;    
        }      
        OslMfree(pData); 
    }

    i51AdeOsLog(1,"SMS : sms no del, return");
    return 0;
}
#endif

//22222222222222222222222222222222222222222222222222222222222222222222222222222
#elif( I51_MTK_VS >=VS_09B )
#include "ProtocolEvents.h"
#include "..\..\plutommi\Service\SmsSrv\SmsDispatchSrv.h"
#include "SmsSrvGprot.h"
#include "..\..\plutommi\Service\SmsSrv\SmsStorageCoreSrv.h"
#include "smslib_enum.h"
#include "smslib_def.h"
#include "..\..\plutommi\Service\SmsSrv\SmsStorageSrv.h"

#define SRV_SMS_INVALID_NUM   0xff

#if( I51_MTK_VS >=VS_W10 )
typedef enum
{
    SRV_SMS_FCS_NO_ERR        = 0x00,
    /* (U)SIM SMS storage full */
    SRV_SMS_FCS_SIM_FULL    = 0xd0,
    /* No SMS storage capability in (U)SIM */
    SRV_SMS_FCS_NO_SIM_MEM,
    /* Error in MS */
    SRV_SMS_FCS_ERR_IN_SM,
    /* Memory Capacity Exceeded */
    SRV_SMS_FCS_MEM_EXCEED,
    
    SRV_SMS_UNKNOWN_ERR = 0xff
}srv_sms_tp_fcs_enum;
#endif

#if( I51_MTK_VS >=VS_W10 )
extern void i51_sms_send_sms_ack(MMI_BOOL result,srv_sms_tp_fcs_enum error_cause,
    srv_sms_sim_enum sim_id,U16 pdu_id,U8 msg_class,MMI_BOOL is_msg_wait,MMI_BOOL is_store);
#else  
extern void i51_sms_send_sms_ack(srv_sms_sim_enum sim_id, U32 msg_id, U8 mem, MMI_BOOL result, U32 error_cause);
#endif


typedef struct tagI51SmsInfo
{
	U16 m_Index;//索引
	U8 m_Dcs;//编码格式
	U8 m_Num[64];//号码内容  
	U8 m_NumLen;//号码长度
	BOOL m_FlashSms;//是否闪信
	char *m_MsgData;
	U16 m_MsgDataLen;
}I51SmsInfo;
extern void  i51PortOnNewSms(void);
BOOL i51SmsFilter(void *evt)
{
    mmi_evt_sms_disp_struct *ev_data = NULL;
    srv_sms_sim_enum sim_id = 0;
    mmi_sms_new_msg_pdu_ind_struct *data = NULL;
    smslib_general_struct *app_lib_data = NULL;
	I51SmsInfo smsinfo;
    int buffLen = 0;
    U8 smsnumber[32];
    U8 is_obj_present =0;
    U16 textLen =0;
    BOOL bDelFlag = FALSE;
	i51AdeOsLog(0,"SMS : i51SmsFilter , find sms coming");
    ev_data = (mmi_evt_sms_disp_struct *)evt;
    memset(&smsinfo,0,sizeof(I51SmsInfo));
    
    if (NULL != ev_data && ev_data->msg_id == PTR_MSG_ID_SRV_SMS_DELIVER_IND)
    {
        PU8 textBuff = NULL;
	    PU8 textBuffShifted = NULL;
        PU8  TPUD[1];
	    U8 TPUDLEN[1];
		sim_id = ev_data->sim_id;
        data = (mmi_sms_new_msg_pdu_ind_struct *)ev_data->data;
        app_lib_data = OslMalloc(sizeof(smslib_general_struct));
        if (NULL == app_lib_data)
        {
            return FALSE;
        }
        memset(app_lib_data, 0, sizeof(smslib_general_struct));
        smslib_decode_pdu(data->pdu, (kal_uint8)data->pdu_length, app_lib_data);
        smslib_get_msg_content(KAL_FALSE, app_lib_data, NULL);        
        if (app_lib_data->tpdu.msg_len == 0)
        {
            smslib_dealloc_sms_struct(app_lib_data);
            OslMfree((void *)app_lib_data);
            return FALSE;
        }

        if (app_lib_data->tpdu.mti == 2 && app_lib_data->tpdu.msg_class == 4)
        {
            smslib_dealloc_sms_struct(app_lib_data);
            OslMfree((void *)app_lib_data);
            return FALSE;
        }
        
        smsinfo.m_Index = ev_data->msg_id; 
        if (0xFFFF == smsinfo.m_Index)
        {
            smsinfo.m_FlashSms = 1;            
        }	    
        smsinfo.m_Dcs = srv_sms_smslib_alphabet_to_dcs(app_lib_data->tpdu.alphabet_type);
        memset(smsnumber,0,sizeof(smsnumber));
        srv_sms_convert_l4_num_to_ascii_num(smsnumber, &app_lib_data->forMMI_TPAddr, SRV_SMS_MAX_ADDR_LEN);
        strncpy((char*)smsinfo.m_Num, (char*)smsnumber, sizeof(smsinfo.m_Num) -1);	   
        smsinfo.m_NumLen = strlen((char *)smsinfo.m_Num);
        TPUDLEN[0] = app_lib_data->tpdu_len;
        TPUD[0] = app_lib_data->forMMI_UserData;
		buffLen = app_lib_data->forMMI_UserData_length;

        if (app_lib_data->tpdu.alphabet_type == SMSLIB_GSM7_BIT)
		{
			buffLen *= 2;
		}		
		buffLen = (buffLen < SRV_SMS_MAX_BUFF_SIZE - ENCODING_LENGTH) ? buffLen : (SRV_SMS_MAX_BUFF_SIZE - ENCODING_LENGTH);

        textBuff = (U8 *)OslMalloc(buffLen + 2);
		if (textBuff == NULL)
		{
			smslib_dealloc_sms_struct(app_lib_data);
            OslMfree((void *)app_lib_data);
            return FALSE;
		}
		memset(textBuff, 0, buffLen + 2);		
		textBuffShifted = textBuff;       
        
        EMSMsgPreview(
        	1,
        	smsinfo.m_Dcs,
        	(BYTE)GetUDHIBit(app_lib_data->tpdu.fo),
        	TPUD,
        	TPUDLEN,
        	buffLen,
        	textBuffShifted,
        	&is_obj_present,
        	&textLen);
        textBuff[textLen] = '\0';
        textBuff[textLen + 1] = '\0';
        smsinfo.m_MsgDataLen = textLen/2;
        smsinfo.m_MsgData = (char*)textBuff;
#if( I51_MTK_VS >=VS_W10 )
        i51_sms_send_sms_ack(KAL_TRUE,0, ev_data->sim_id,SRV_SMS_INVALID_PDU_ID,app_lib_data->tpdu.msg_class,0,0);
#else
	    i51_sms_send_sms_ack(ev_data->sim_id, ev_data->msg_id, SRV_SMS_STORAGE_UNSPECIFIC, KAL_TRUE, 0);
#endif
        i51AdeOsLog(0,"SMS : rev sms");
        i51AdeOsLog(0,"SMS : smsinfo.m_Index=%d, smsinfo.m_Dcs=%d, smsinfo.m_Num=%s, smsinfo.m_NumLen=%d, smsinfo.m_FlashSms=%d, smsinfo.m_MsgData=%x, smsinfo.m_MsgDataLen=%d",
	        smsinfo.m_Index, smsinfo.m_Dcs, smsinfo.m_Num, smsinfo.m_NumLen, smsinfo.m_FlashSms, smsinfo.m_MsgData, smsinfo.m_MsgDataLen );
        i51AdeOsLogEx(0, (iU8*)smsinfo.m_MsgData, smsinfo.m_MsgDataLen*2 );

        i51AdeOsLog( 0, "SMS : ev_data->sim_id=%d",ev_data->sim_id );   
        if( i51AdeSmsRunHook( (ev_data->sim_id&1)-1, (iCHAR*)smsinfo.m_Num, smsinfo.m_NumLen, (iU16*)smsinfo.m_MsgData, smsinfo.m_MsgDataLen ) 
                == i51_SMS_HOOK_DELETE )
        {
			i51AdeOsLog(0,"SMS : del sms");
            if( !smsinfo.m_FlashSms )//del no flash sms
                srv_sms_delete_msg( smsinfo.m_Index, NULL, (void*)0xDE );            
            bDelFlag = TRUE;
        } 

        smslib_dealloc_sms_struct(app_lib_data);
        OslMfree((void *)app_lib_data); 
        OslMfree(textBuff);
//#undef  __I51_SMS_SOUND__     
#ifdef  __I51_SMS_SOUND__   //if new sms confilct sound\record, use the macro
        if(!bDelFlag)
            i51PortOnNewSms();
#endif
        return bDelFlag;
        
    }  

}

#endif //333333333333333333333333333333333333333333333333333333333333333333333333333333

#ifdef __MMI_MMS_2__
 void i51AdaMmsDeleteMsgCallback(srv_mms_result_enum result, void *rsp_data, S32 user_data)
{
	#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC,"SMS : i51AdaMmsDeleteMsgCallback , MMS  Del  S  ");
	#endif

	
	#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC,"SMS : i51AdaMmsDeleteMsgCallback , MMS  Del  E  ");
	#endif
}
#endif

S8  i51MmsFilter(U32 msg_id , char *address  )
{
	iU8 error = 0;
	iCHAR address_buffer[25] = {0};

#ifdef __MMI_MMS_2__
	app_ucs2_str_to_asc_str( (char *)address_buffer,  address );
#else
	memcpy(address_buffer, address, strlen(address));
#endif	
	i51AdeOsLog(i51_LOG_SC,"SMS : i51MmsFilter , del  MMS  address_buffer = %s , address = %s",address_buffer,address);

	if( i51AdeSmsRunHook( 0, address_buffer, (iU32)strlen((char *)address_buffer), NULL, 0 ) == i51_SMS_HOOK_DELETE )
	{
	#ifdef __MMI_MMS_2__
	  #if( I51_MTK_VS ==VS_09B )
	  {
		srv_mms_delete_req_struct msg_req;

		memset(&msg_req, 0, sizeof(srv_mms_delete_req_struct));
		msg_req.msg_id[0] = msg_id;
		msg_req.no_of_msg = 1;
		msg_req.box = MMA_FOLDER_INBOX;	
		msg_req.user_data = NULL;
		msg_req.call_back = i51AdaMmsDeleteMsgCallback;
		msg_req.app_id=MMA_APP_ID_MMS_APP;
		srv_mms_delete((void*)&msg_req);
	  }
	  #else
			srv_mms_delete_msg(msg_id, (U8 *)&error);
	  #endif
	#else	//__MMI_MMS_2__
		#if( I51_MTK_VS <=VS_09B )
			fldrMgrDeleteMessage(msg_id);
		#else
			fldrMgrDeleteMessage(msg_id, MMS_SUFFIX_NOTIFICATION, MSF_MMS_STORAGE_PHONE);
		#endif
	#endif	//__MMI_MMS_2__
		i51adaMmsState = i51_SMS_HOOK_DELETE;


		i51AdeOsLog(i51_LOG_SC,"SMS : i51MmsFilter , del  MMS  success ! ");
		return TRUE;    
       }
	
	i51AdeOsLog(i51_LOG_SC,"SMS : i51MmsFilter , del  MMS  fail ! ");
	return FALSE;
}

S8 i51AdaMmsCheck (S32 icon_id)
{
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC,"SMS : i51AdaMmsCheck , MMS  Check  S ! ");
#endif

  if( i51_SMS_HOOK_DELETE == i51adaMmsState )
  {
  #if defined( __MMI_MMS_2__) || defined(OBIGO_Q03C_MMS_V01)
      if( icon_id==STATUS_ICON_MMS_BUSY ||	icon_id==STATUS_ICON_MMS_UNREAD ||
	icon_id==STATUS_ICON_EDGE || icon_id==STATUS_ICON_EDGEC || 
	icon_id==STATUS_ICON_GPRS_SERVICE ||icon_id==STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR
    #ifdef __MMI_DUAL_SIM_MASTER__
	|| icon_id==STATUS_ICON_MMS_BUSY_SIM_1 || icon_id==STATUS_ICON_MMS_BUSY_SIM_2 || icon_id==STATUS_ICON_SLAVE_EDGEC || 
	icon_id==STATUS_ICON_SLAVE_EDGE ||	icon_id==STATUS_ICON_SLAVE_GPRS_SERVICE ||
	icon_id==STATUS_ICON_SLAVE_GPRS_ATT_NO_PDP_INDICATOR
    #endif
	)
  #endif 
	{
		i51AdeOsLog(i51_LOG_SC,"SMS : i51AdaMmsCheck , MMS  Check  E--1 ! ");
		return TRUE;
	}
  }else
	{
		//i51AdeOsLog(i51_LOG_SC," i51AdaMmsCheck   MMS  Check  E--2 ! ");
		return FALSE;
	}
}

iS32 i51AdeSmsCreateHook ( iS32 type )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.

	iU16 i = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsCreateHook , S" ) ;
	#endif	

	for ( i = 0 ; i < MAX_SMS_HOOK_COUNT ; i ++ ) {

		if ( iFALSE == i51SmsHookList[i].used )	break ;
	}

	if ( i == MAX_SMS_HOOK_COUNT) {

		i51AdeOsLog ( 0 , "SMS : i51AdeSmsCreateHook , BE" ) ;
		return -1 ;
	}

	i51SmsHookList[i].used = iTRUE ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsCreateHook , E , ret = %x" , i ) ;
	#endif	
	
	return i ;

}

iBOOL i51AdeSmsSetHook ( iS32 hookid , i51SMSHOOKCALLBACK callback ) 
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	//i51SmsHookListCB

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsSetHook , S , %x , %x" , hookid , callback ) ;
	#endif	
	
//	iASSERT ( hookid >= 0 && hookid < MAX_SMS_HOOK_COUNT ) ;

	if (  hookid < 0 || hookid > MAX_SMS_HOOK_COUNT -1 ) {

		i51AdeOsLog ( 0 , "SMS : i51AdeSmsSetHook , ID OUT" ) ;
		return iFALSE ;
	}
	
	if ( iNULL == callback ) {

		i51AdeOsLog ( 0 , "SMS : i51AdeSmsSetHook , BE" ) ;
		return iFALSE ;
	}
	i51SmsHookList[hookid].i51SmsHookCB= callback ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsSetHook , E , ret = %x" , iTRUE ) ;
	#endif	
	
	return iTRUE ;
	

}

iS32 i51AdeSmsRunHook (  iS32 simCard ,  iCHAR* number , iU32 numlen , iU16* buffer , iU32 textlen )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	
    iU16 i = 0 ;
    i51SMSHKR result = 0 ;
    i51SMSHOOKCALLBACK SmsHook = 0 ;   

   	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsRunHook , S , %x , %x , %x , %x , %x" , simCard , number ,numlen , buffer , textlen ) ;
	#endif	

    for ( i = 0 ;i < MAX_SMS_HOOK_COUNT ; i ++) {

	if ( i51SmsHookList[i].i51SmsHookCB!= iNULL ) {
		
	   SmsHook =  i51SmsHookList[i].i51SmsHookCB ;
	   result = SmsHook( simCard , number , numlen , buffer , textlen ) ;
	    if( result != i51_SMS_HOOK_NORMAL ) {

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "SMS : i51AdeSmsRunHook , E , ret = %x" , result ) ;
		#endif	
	       return result ;
	    }
	}
    }

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsRunHook , E , ret = %x" , i51_SMS_HOOK_NORMAL ) ;
	#endif	
   //不做任何处理
    return i51_SMS_HOOK_NORMAL;  

}

 iBOOL i51AdeSmsReleaseHook (  iS32 id)
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.


   	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsReleaseHook , S , %x " , id ) ;
	#endif	

//	iASSERT ( id >= 0 && id < MAX_SMS_HOOK_COUNT ) ;
	if (  id < 0 || id > MAX_SMS_HOOK_COUNT -1 ) {

		i51AdeOsLog ( 0 , "SMS : i51AdeSmsReleaseHook , ID OUT" ) ;
		return iFALSE ;
	}
	i51SmsHookList[id].i51SmsHookCB = iNULL ;
	i51SmsHookList[id].used = iFALSE ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SMS : i51AdeSmsReleaseHook , E , ret = %x" , iTRUE ) ;
	#endif	
	
	return iTRUE ;
	
	

}

#if( I51_MTK_VS <VS_W11 )

int i51AdeSmsGetDefautSim(void)
{
#ifdef __MMI_DUAL_SIM_MASTER__
    switch (MTPNP_AD_Get_UsableSide())
    {
        case MTPNP_AD_DUALSIM_USABLE:
            return 1;
            break;
        case MTPNP_AD_SIMCARD1_USABLE:
            return 1;
            break;
        case MTPNP_AD_SIMCARD2_USABLE:
            return 2;
            break;
    }
#else
    return 0;
#endif
}

#else

int i51AdeSmsGetDefautSim(void)
{
    srv_mode_switch_type_enum  sw_mode = srv_mode_switch_get_current_mode();

    if( srv_sim_ctrl_is_available(MMI_SIM1) && (sw_mode&SRV_MODE_SWITCH_SIM1) )
    {
        return 1;
    }
    #if (MMI_MAX_SIM_NUM >= 2)
    if( srv_sim_ctrl_is_available(MMI_SIM2) && (sw_mode&SRV_MODE_SWITCH_SIM2) )
    {
        return 2;
    } 
    #endif   
    #if (MMI_MAX_SIM_NUM >= 3)  
    if( srv_sim_ctrl_is_available(MMI_SIM3) && (sw_mode&SRV_MODE_SWITCH_SIM3) )
    {
        return 3;
    } 
    #endif 
    #if (MMI_MAX_SIM_NUM >= 4)  
    if( srv_sim_ctrl_is_available(MMI_SIM4) && (sw_mode&SRV_MODE_SWITCH_SIM4) )
    {
        return 4;
    }   
    #endif
    return 1;
} 

#endif

iBOOL i51AdeSmsGetPhoneBook ( i51PHONEBOOK* phonebook , iU32* length )
{
	//	author : jons
	//	since : 20120227
	//	PKIG Tech. Co., Ltd.

	
#if( I51_MTK_VS <=VS_09A )
	extern MMI_PHB_ENTRY_BCD_STRUCT PhoneBook[];	//电话簿结构数组，已经包括sim卡和手机里的。
	extern U16 PhoneBookEntryCount;	//有多少项电话簿结构
	extern U16 g_phb_name_index[];	 //按姓名排列的结构index
	extern U8 mmi_phb_convert_to_digit_by_store_index(U8 *dest, U8 *source, U8 max_dest_len, U16 store_index);	//BCD编码转换为ascii
	extern U8 mmi_phb_convert_to_digit(U8 *dest, U8 *source, U8 max_dest_len);
#else
	extern MMI_BOOL srv_phb_get_number(U16 store_index, U16 *number, U16 len);
	extern MMI_BOOL srv_phb_get_name(U16 store_index, U16 *name, U16 len);
	extern S8* srv_phb_datamgr_get_number(U16 index);

	static  srv_phb_filter_struct phb_filter ;
#endif

	iU16   phone_number[44]  , contact_count ,  phone_num_index , num=0 ;
	iU16* temp_array ;


	if( phonebook==iNULL)
	{
		#if( I51_MTK_VS <=VS_09A )
			*length=PhoneBookEntryCount;
		#else
			*length = srv_phb_get_used_contact(PHB_STORAGE_MAX);
		#endif

		if(*length==0)
		{
			i51AdeOsLog(2,"SMS : i51AdeSmsGetPhoneBook , phone num  is zero ! " );
			return iFALSE;
		}

		return	iTRUE;
	}else{
		#if( I51_MTK_VS <=VS_09A )
			if( PhoneBookEntryCount<(*length) )
				*length=PhoneBookEntryCount;
		#else
			if( srv_phb_get_used_contact(PHB_STORAGE_MAX) <= (*length ) )
			{
				*length = srv_phb_get_used_contact(PHB_STORAGE_MAX);
			}
		#endif

			if(*length==0)
			{
				i51AdeOsLog(2,"SMS : i51AdeSmsGetPhoneBook , phone num  is zero ! " );
				return iFALSE;
			}
		}
	

#if( I51_MTK_VS <=VS_09A )
	#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(2,"SMS : i51AdeSmsGetPhoneBook , length = %d ,  max_num = %d",*length , PhoneBookEntryCount ) ;
	#endif

    while( num<(*length) )
    {
	phone_num_index = g_phb_name_index[num];

	if (mmi_ucs2strlen((S8*) PhoneBook[phone_num_index].alpha_id.name) >= (sizeof(phonebook[num].name) /2))
		mmi_ucs2ncpy((S8*) phonebook[num].name, (S8*) PhoneBook[phone_num_index].alpha_id.name , sizeof(phonebook[num].name) /2 );
	
	else if (mmi_ucs2strlen((S8*) PhoneBook[phone_num_index].alpha_id.name))
	{
		mmi_ucs2cpy((S8*) phonebook[num].name, (S8*) PhoneBook[phone_num_index].alpha_id.name);
	}


	 if (((PhoneBook[phone_num_index].tel.type& 0x90) == 0x90)&& (PhoneBook[phone_num_index].tel.number[0] != 0xFF)\
	 	&&(PhoneBook[phone_num_index].tel.length <= (sizeof(phonebook[num].number)-2)))
	{
		phonebook[num].number[0] = '+';
		#if( I51_MTK_VS <=VS_0812 )
		mmi_phb_convert_to_digit((U8 *)(phonebook[num].number)+1, PhoneBook[phone_num_index].tel.number, \
			PhoneBook[phone_num_index].tel.length + 1);
		#else
		mmi_phb_convert_to_digit_by_store_index((U8 *)(phonebook[num].number)+1, PhoneBook[phone_num_index].tel.number, \
			PhoneBook[phone_num_index].tel.length + 1,phone_num_index);
		#endif
	}
	else if (PhoneBook[phone_num_index].tel.length <= (sizeof(phonebook[num].number)-1))
	{
		#if( I51_MTK_VS <=VS_0812 )
		mmi_phb_convert_to_digit((U8 *)(phonebook[num].number), PhoneBook[phone_num_index].tel.number, \
			PhoneBook[phone_num_index].tel.length + 1);
		#else
		mmi_phb_convert_to_digit_by_store_index((U8 *)(phonebook[num].number), PhoneBook[phone_num_index].tel.number, \
			PhoneBook[phone_num_index].tel.length + 1, phone_num_index);
		#endif
	}

	#ifdef i51ADAPTER_DEBUG
	    i51AdeOsLog(2,"SMS : i51AdeSmsGetPhoneBook , phone_number =%s  ,  phone_length =%d  ,  type =%u  ,  g_phb_name_index =%d" , phonebook[num].number ,  \
			PhoneBook[phone_num_index].tel.length  ,  PhoneBook[phone_num_index].tel.type ,  phone_num_index);
	#endif
	num++ ;

    }

#else
	
	memset(&phb_filter, 0, sizeof(srv_phb_filter_struct));
	phb_filter.storage = PHB_STORAGE_MAX;
	phb_filter.field_filter = SRV_PHB_ENTRY_FIELD_ALL;


	temp_array = (U16*)i51_med_alloc_ext_mem((*length)*sizeof(U16));
	contact_count = (U16)srv_phb_oplib_build_contact_list(&phb_filter, temp_array, *length );

	i51AdeOsLog(2,"SMS : i51AdeSmsGetPhoneBook , contact_count=%d ", contact_count );

	while(num < ( *length ))
	{
		srv_phb_get_number(temp_array[num], phone_number, sizeof(phonebook[num].number));
		srv_phb_get_name(temp_array[num], phonebook[num].name, sizeof(phonebook[num].name));
		i51AdeStdUnicode2Ascii(phone_number , (iS8*) phonebook[num].number );		
	#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(2, "SMS : i51AdeSmsGetPhoneBook , phone_number=%s  ",phonebook[num].number );
	#endif
		num++;
	}
	
	i51_med_free_ext_mem((void **)&temp_array);
#endif

	return iTRUE ;
}


 iBOOL i51AdeSmsCreate ( i51ADECALLBACK callback )
{

	//	author : william
	//	since : 20110802
	//	PKIG Tech. Co., Ltd.
	
	i51AdaCreateCB = callback ;
	InitCount = 0 ;

	iASSERT ( callback ) ;
	memset (i51imei, 0, sizeof(i51imei)) ;
	memset (i51imsi1, 0, sizeof(i51imsi1)) ;
	memset (i51imsi2, 0, sizeof(i51imsi2)) ;
	memset (i51sc1, 0, sizeof(i51sc1)) ;
	memset (i51sc2, 0, sizeof(i51sc2)) ;
	
	GetIMEI = iFALSE ;
	GetIMSI1 = iFALSE ;
	GetIMSI2 = iFALSE ;
	GetSC1 = iFALSE ;
	GetSC2 = iFALSE ; 

	i51GetImeiReq () ;
	i51GetImsi1Req () ;
	i51SmsGetSc1Req () ;
	
	#ifdef __MMI_DUAL_SIM_MASTER__
    i51AdeOsLog(0, "SMS : __MMI_DUAL_SIM_MASTER__ define");
	i51GetImsi2Req () ;
	i51SmsGetSc2Req () ;
	#endif

	return iTRUE ;

}

iBOOL i51AdeSmsRelease ( )
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
	
	return iTRUE ;

}


