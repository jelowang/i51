/*

The Implementing of 'Adapter' , Module : 'SMS'.	
Layer belong : 'Static Components'.
For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Api.xls"
For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"

The 51PK Platform was implementing based on 'The i51 Architecture'.
'The i51 Architecture' is designed by Jelo.

Copyright (C) 2011 , PKIG Tech. Co., Ltd.

*/

#include "i51AdeCnf.h"
#include "i51AdeSms.h"	
#include "ven_util.h"
#include "ven_sdk.h"
#include "ven_msg.h"
#include "ven_socket.h"

#include "i51AdeCnf.h"

#include "mmi_smssrv.h"
#include "mmi_simsrv_def.h"
#include "mae_common_def.h"
#include "ven_setting.h"
#include "mmi_clstbl.h"

#include "mmi_common_rai_id.h"
#include "mmi_util_fs.h"
#include "mmi_phbsrv_data_handle.h"

//#define i51ADAPTER_DEBUG

extern iU8 gi51IsRunning;

//应该传入 AP 的handle   
extern iU32 g_i51PkigHandle; 

//buf lenght 17 use 15
static iU8 i51imsi1[17];

//buf lenght 17 use 15
static iU8 i51imsi2[17];

//buf lenght 17 use 15
static iU8	i51imei[17];

typedef void (*i51ADESMSCALLBACK)( iBOOL result ) ;

//短信钩子最大个数
#define MAX_SMS_HOOK_COUNT		5

typedef struct {

	iBOOL used ;
	i51SMSHOOKCALLBACK i51SmsHookCB ;

 }I51ADASMSHOOK ;

//创建5个短信钩子
static I51ADASMSHOOK i51SmsHookList[MAX_SMS_HOOK_COUNT] = { 0 } ;

//创建发送信息设备的最大个数
#define i51SMSSENDDIVICE_MAX		5

typedef struct 
{
	iU32 creatSate; //创建装态，1为已创建，0未创建

	i51ADESMSCALLBACK callback ;

} SENDSMSDIVICE_STRUCT ;


//创建5个发送信息的设备
static SENDSMSDIVICE_STRUCT i51sendSmsDivice[i51SMSSENDDIVICE_MAX] = {0};

ven_msg_sms_service_center_t smsSrvCenter1 = { 0 };

ven_msg_sms_service_center_t smsSrvCenter2 = { 0 };


static i51ADECALLBACK i51AdeSmsCreate_CB = 0 ;

//电话本相关参数
extern u32 i51GetPhbContactsNum(void);
extern u16 i51GetPhbDhSortByNameTable(u32 index);
extern s32 i51GetPhbSimCardContactsInfo(mmi_raiId_t ident, MmiPhbDhADNData_t **pptADNData);

static iU32 i51PhysicalIndexToRaiID(iU16 u16PhysicalIdx)
{
    iU32 u32RaiID = 0;	
    
    if (u16PhysicalIdx <= (MMI_PHB_ADR_SUPPORT_MAX + MMI_PHB_ADN_SUPPORT_MAX))
    { 
        u32RaiID = MMI_FS_RAI_ITEM_PHB_ADN_M_BACKUP_DATA_START + (u16PhysicalIdx - MMI_PHB_ADR_SUPPORT_MAX) - 1; 
    }
    else   
    {      
        u32RaiID = MMI_FS_RAI_ITEM_PHB_ADN_S_BACKUP_DATA_START + (u16PhysicalIdx - MMI_PHB_ADR_SUPPORT_MAX - MMI_PHB_ADN_SUPPORT_MAX) - 1; 
    }   
    
    return u32RaiID;
}

static iBOOL i51AdeSmsGetSimCardContentNumAndName(iU16* pContentName, iCHAR* pContentNum, iU16 u16PhysicalIdx)
{
	iU32 nRaiID = 0;
	iU32 nNameSize = 0;
	MmiPhbDhADNData_t *ptADNData = NULL;
	iS32 iRret = 0;
	iBOOL bRet = iFALSE;

	nRaiID = i51PhysicalIndexToRaiID(u16PhysicalIdx);
	iRret = i51GetPhbSimCardContactsInfo(nRaiID, &ptADNData);
	if (0 == iRret)
	{
		nNameSize = (ptADNData->u8NameLen + 1) * sizeof(iU16); 
		mae_decode_sim_alpha_id(ptADNData->au8Name, (iU16)ptADNData->u8NameLen, pContentName, &nNameSize);
		i51AdeStdMemcpy16(pContentNum, ptADNData->au8Number, ptADNData->u8NumberLen);
		bRet = iTRUE;
	}
	else
	{
		bRet = iFALSE;
	}

	if(ptADNData != NULL)
	{
		i51AdeOsFree((void *) ptADNData);
		ptADNData = NULL;
	}

	return bRet; 
}

//获取phone里的联系人
static iBOOL i51AdeSmsGetPhoneContentNumAndName(iU16* pContentName, iCHAR* pContentNum, iU16 u16PhysicalIdx)
{
	iS32 s32RetSize = 0;
	MmiPhbContactContent_t sRawData = {0};
	iU8* pu8Idx = NULL;
	iU8 name[41] = {0};
	iU8 nNameLen = 0;
	iU8 nNumLen = 0;
	iU32 nNameSize = 0;

	memset(&sRawData, 0, sizeof(MmiPhbContactContent_t));
	s32RetSize = mmi_fs_raiRead((MMI_FS_RAI_ITEM_PHB_ADR_START + u16PhysicalIdx), 0, sizeof(MmiPhbContactContent_t), &sRawData);

	if (s32RetSize > 0)
	{
		pu8Idx = (iU8*)&sRawData;

		/*Name*/
		nNameLen = *pu8Idx;
		pu8Idx++;
		memcpy(name, pu8Idx, nNameLen);
		nNameSize = (nNameLen + 1) * sizeof(iU16);
		mae_decode_sim_alpha_id(name, (iU16)nNameLen, pContentName, &nNameSize);
		pu8Idx = pu8Idx + nNameLen;

		/*Number*/
		nNumLen = *pu8Idx;
		pu8Idx++;
		if (nNumLen)
		{
			memcpy(pContentNum, pu8Idx, nNumLen);
		}
		return iTRUE;
	}
	else
	{
		return iFALSE;
	}
}

static iBOOL i51AdeSmsGetphbContentNumAndName(iU16* pContentName, iCHAR* pContentNum, iU32 nIndex)
{
	iBOOL ret = iTRUE;
	iU16 u16i51PhysicalIdx = i51GetPhbDhSortByNameTable(nIndex + 1);

	if (u16i51PhysicalIdx <= MMI_PHB_ADR_SUPPORT_MAX)
	{
		ret = i51AdeSmsGetPhoneContentNumAndName(pContentName, pContentNum, u16i51PhysicalIdx);
	}
	else
	{
		ret = i51AdeSmsGetSimCardContentNumAndName(pContentName, pContentNum, u16i51PhysicalIdx);
	}
	return ret;
}

static void smsCreate_cb(iU32 timerid)
{
	if (NULL != i51AdeSmsCreate_CB)
	{
		i51AdeSmsCreate_CB(iTRUE);
	}

	i51AdeOsTimerStop(timerid);
}

static void i51_VEN_sms_send_CB(iU32 UserData, iU32 RspData)
{
	ven_msg_send_sms_rsp_t *Result = (ven_msg_send_sms_rsp_t*)RspData;
	iU32 diviceID = UserData;

	i51AdeOsLog(i51_LOG_SC, "SMS : i51_VEN_sms_send_CB , %d,%x" ,Result->result, i51sendSmsDivice[diviceID].callback);

	if (NULL != i51sendSmsDivice[diviceID].callback)
	{
		if (VEN_MSG_SMS_SEND_RESULT_SUCCESS == Result->result)
		{
			i51sendSmsDivice[diviceID].callback(iTRUE);
		}
		else
		{
			i51sendSmsDivice[diviceID].callback(iFALSE);
		}
	}

}

static void i51_MMI_sms_send_cb(IBase *pObj, iU32 userData, SMS_Send_Result_t *pResult)
{
	iU32 diviceID = userData;

	i51AdeOsLog(i51_LOG_SC, "SMS : i51_MMI_sms_send_cb , %d,%x" , pResult->eResult,i51sendSmsDivice[diviceID].callback);

	if (NULL != i51sendSmsDivice[diviceID].callback)
	{
		if (SMS_SEND_SUCCESS == pResult->eResult)
		{
			i51sendSmsDivice[diviceID].callback(iTRUE);
		}
		else
		{
			i51sendSmsDivice[diviceID].callback(iFALSE);
		}
	}
}

static iBOOL i51AdePublicSmsSend(iU32 device , iS32 sim , iCHAR* phone , iU16* contents , iBOOL save , void (*callback)( iBOOL results ) )
{

	//	author : jone
	//	since : 2011.11.8
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	if (1 == gi51IsRunning)
	{
		//VENDER
		iU16 pTelNO[128] = {0};
		ven_msg_ret_t ret = VEN_MSG_RET_SUCCESS;
		ven_req_data_t ReqData = {0};
		iU8 SimID = E_VEN_MASTER_SIM;
		ven_msg_sms_struct_t ptMsg = {0};

		if (1 != i51sendSmsDivice[device - 1].creatSate)
		{
			i51AdeOsLog(i51_LOG_SC, "SMS : i51AdePublicSmsSend , VEN , ERR1");
			return iFALSE;
		}

		if (1 != sim)
		{
			SimID = E_VEN_SLAVE_SIM;
		}

		i51AdeStdAscii2Unicode(phone, pTelNO);
		ptMsg.pData = contents;
		ptMsg.ptTelNo = pTelNO;
		ptMsg.dataLen = i51AdeStdWStrlen(contents);
		ptMsg.telNoLen = i51AdeStdWStrlen(pTelNO);
		ptMsg.smsType = VEN_MSG_SMS_TYPE_UCS2;
		ptMsg.simId = SimID;
		ptMsg.port = 1234; // should be same as above one.
		ptMsg.saveSentMsg = VEN_MSG_NOT_SAVE_SENT_MSG;
		ptMsg.retrySendMsg = VEN_MSG_NOT_RETRY_SEND_MSG;
		ptMsg.msgPortProp = VEN_MSG_PORT_DISABLE;

		i51sendSmsDivice[device - 1].callback = callback;

		ReqData.Handle = g_i51PkigHandle;
		ReqData.bSync = FALSE;
		ReqData.pfnCb = i51_VEN_sms_send_CB;
		ReqData.userData = (iU32)device - 1;

		ret = ven_msg_sendSMSEx(&ptMsg, &ReqData);

		if (VEN_MSG_RET_SUCCESS == ret)
		{
			return iTRUE;
		}
		else
		{
			i51AdeOsLog(i51_LOG_SC, "SMS : i51AdePublicSmsSend , VEN , ERR2");
			return iFALSE;
		}
	}
	else
	{
		//MMI
		iU16 wsnumber[128] = {0};
		MAE_Ret nRet = MAE_RET_SUCCESS;
		SMS_MSG_SentData_t  pSendData = {0}; //= NULL;
		ISMS *pSMS;
		iU8 simModule = DUAL_MASTER;

		if (0 == phone || 0 == contents)
		{
			i51AdeOsLog(i51_LOG_SC, "SMS : i51AdePublicSmsSend , MMI , ERR1");
			return iFALSE;
		}

		if (1 != i51sendSmsDivice[device - 1].creatSate)
		{
			i51AdeOsLog(i51_LOG_SC, "SMS : i51AdePublicSmsSend , MMI , ERR2");
			return iFALSE;
		}

		i51AdeStdAscii2Unicode(phone, wsnumber);

		if(sim == 2)
			simModule = DUAL_SLAVE;

		SH_CreateInstance(CLSID_SMS, &pSMS, NULL);

		pSendData.simModule = simModule;
		pSendData.ptTelNo = wsnumber;        
		pSendData.telNoLen =  ven_std_wstrlen(wsnumber);
		pSendData.ptData =  contents;         
		pSendData.dataLen = ven_std_wstrlen(contents);
		pSendData.tEncoding = EncodingUCS2;

		i51sendSmsDivice[device - 1].callback = callback;

		nRet = ISMS_SMS_SendSMS_Without_Notify(pSMS, &pSendData,(pfn_sms_send_CB)i51_MMI_sms_send_cb, NULL, device - 1);

		RELEASEIF(pSMS, NULL);

		if( nRet == MAE_RET_SUCCESS)
		{
			return iTRUE;
		}
		else
		{
			i51AdeOsLog(i51_LOG_SC, "SMS : i51AdePublicSmsSend , F , MMI , ERR3");
			return iFALSE;
		}
	}
}


//ok
iS32* i51AdeSmsGetSims ( void )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//获取当前可用的SIM卡编号
	//存储sim卡的信息simCardInfo[0] 代表sim个数，后面表示sim卡编号

        iU16 mcc1=0, mcc2=0;
        iU16 mnc1=0, mnc2=0;
	static iS32	simCardInfo[5] = { 0 } ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetImsi , S");
#endif

	memset (simCardInfo, 0, sizeof(simCardInfo)) ;
        ven_util_getMccMnc(E_VEN_MASTER_SIM,&mcc1,&mnc1);
        ven_util_getMccMnc(E_VEN_SLAVE_SIM,&mcc2,&mnc2);

	if(mcc1 != 0)
	{
		simCardInfo[0] = 1 ;
		simCardInfo[1] = 1 ;
	}
	else if(mcc2 != 0)
	{
		simCardInfo[0] = 1 ;
		simCardInfo[1] = 2 ;
	}
	else if((mcc1 != 0) && (mcc2 != 0))
	{
		simCardInfo[0] = 2 ;
		simCardInfo[1] = 1 ;
		simCardInfo[2] = 2;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetImsi , E , %d,%d,%d" , simCardInfo[0], simCardInfo[1], simCardInfo[2]);
#endif

	return simCardInfo ;
}

//ok
iCHAR* i51AdeSmsGetImsi ( iS32 sim )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	ven_Imsi_t  imsi2_str = {0};
	ven_Imsi_t  imsi1_str = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetImsi , S , %d", sim);
#endif

	switch(sim)
	{
	case 1:
		ven_util_getImsi(&imsi1_str, E_VEN_MASTER_SIM);
		memcpy(i51imsi1, &imsi1_str, 15);

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetImsi , E , %s", i51imsi1);
#endif
		return i51imsi1;
	case 2:
		ven_util_getImsi(&imsi2_str, E_VEN_SLAVE_SIM);
		memcpy(i51imsi2, &imsi2_str, 15);

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetImsi , E , %s", i51imsi2);
#endif
		return i51imsi2;
	default:    
		break;
	}

	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetImsi , F, %d" , sim);

	return NULL;
}

//ok
iCHAR* i51AdeSmsGetImei ( void )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_Imei_t  imei_str = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetImei , S");
#endif

	ven_util_getImei(&imei_str);      
	memcpy(i51imei, &imei_str, 15);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetImei , E , %s" , i51imei);
#endif

	return i51imei;
}

//ok
iCHAR* i51AdeSmsGetCenter ( iS32 sim )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_msg_ret_t ret = VEN_MSG_RET_FAILED;
	ven_req_data_t ReqData = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetCenter , S , %d", sim);
#endif

	ReqData.Handle = g_i51PkigHandle;
	ReqData.pfnCb  = NULL;
	ReqData.bSync  = TRUE;
	ReqData.userData = 0;

	switch (sim)
	{
	case 1:
		//ret = ven_msg_querySMSServiceCenter(E_VEN_MASTER_SIM, &smsSrvCenter1, &ReqData);

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetCenter , E , smsSrvCenter1.addr %s", smsSrvCenter1.addr);
#endif

		return smsSrvCenter1.addr;
	case 2:
		//ret = ven_msg_querySMSServiceCenter(E_VEN_SLAVE_SIM, &smsSrvCenter2, &ReqData);

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetCenter , E , smsSrvCenter2.addr %s", smsSrvCenter2.addr);
#endif

		return smsSrvCenter2.addr;
	default:    
		break;
	}

	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetCenter , F, %d" ,sim);

	return NULL;
}

//ok
iU32 i51AdeSmsCreateDevice (void)
{

	//	author : jone
	//	since : 2011.11.8
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU16 iDiviceID = 0 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsCreateDevice , S");
#endif

	for (; iDiviceID < i51SMSSENDDIVICE_MAX ; iDiviceID ++ )
	{
		if ( !i51sendSmsDivice[iDiviceID].creatSate)	
			break ;
	}

	if( iDiviceID == i51SMSSENDDIVICE_MAX ) 
	{
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsCreateDevice , F");

		return 0;
	}

	ven_std_memset(&i51sendSmsDivice[iDiviceID], 0, sizeof(SENDSMSDIVICE_STRUCT));
	i51sendSmsDivice[iDiviceID].creatSate = 1 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsCreateDevice , E , %d", iDiviceID);
#endif

	return iDiviceID + 1;
}

//oK
iBOOL i51AdeSmsSend ( iU32 device , iS32 sim , iCHAR* phone , iU16* contents , iBOOL save , void (*callback)( iBOOL results ) ) 
{

	//	author : jone
	//	since : 2011.11.8
	//	(C) 2011 , PKIG Tech. Co., Ltd.


	iBOOL result = iFALSE;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsSend , S , %d , %d , %s , %x , %d , %x", device, sim, phone, contents, save, callback);
#endif

	result = i51AdePublicSmsSend(device, sim, phone, contents, save, callback);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsSend , E , %d" , result);
#endif

	return result;
}

iBOOL i51AdeSmsSendEx ( iU32 device , iS32 sim , iCHAR* phone , iCHAR* contents , iBOOL unicode , iBOOL save , void (*callback)( iBOOL results ) )
{

	//	author : jone
	//	since : 2011.11.8
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iBOOL result = iFALSE;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsSendEx , S , %d , %d , %s , %x , %d , %x", device, sim, phone, contents, save, callback);
#endif

	if (iTRUE == unicode)
	{
		result = i51AdePublicSmsSend(device, sim, phone, contents, save, callback);
	}
	else
	{
		iU16 *pwsMessage = NULL;
		iU32 messagelen = ven_std_strlen((char*)contents);

		pwsMessage = i51AdeOsMalloc((messagelen + 1) * 2, __FILE__, __LINE__);
		i51AdeStdAscii2Unicode((char*)contents, pwsMessage);
		pwsMessage[messagelen] = 0;

		result = i51AdePublicSmsSend(device, sim, phone, pwsMessage, save, callback);

		if (NULL == pwsMessage)
		{
			i51AdeOsFree(pwsMessage);
			pwsMessage = NULL;
		}
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsSendEx , E , %d" , result);
#endif

	return result;
}

//ok
iBOOL i51AdeSmsReleaseDevice ( iU32 device ) 
{

	//	author : jone
	//	since : 2011.11.8
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsReleaseDevice, S , %d", device);
#endif

	if ((0 == device) || (device > i51SMSSENDDIVICE_MAX) )
	{
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsReleaseDevice , F , %d" , device);
		return iFALSE;
	}

	if (iTRUE == i51sendSmsDivice[device - 1].creatSate)
	{
		i51AdeStdMemset16(&i51sendSmsDivice[device - 1], 0, sizeof(SENDSMSDIVICE_STRUCT));

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsReleaseDevice , E");
#endif
		return iTRUE;
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsReleaseDevice , F");
		return iFALSE;
	}
}

//ok
iS32 i51AdeSmsCreateHook ( iS32 type )
{
	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iU16 i = 0 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsCreateHook , S" ) ;
#endif	

	for ( i = 0 ; i < MAX_SMS_HOOK_COUNT ; i ++ ) 
	{

		if ( iFALSE == i51SmsHookList[i].used )
        {
            break ;        
        }      
	}

	if ( i == MAX_SMS_HOOK_COUNT) {

		i51AdeOsLog(i51_LOG_SC , "SMS : i51AdeSmsCreateHook , F" ) ;
		return -1 ;
	}

	i51SmsHookList[i].used = iTRUE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsCreateHook , E , %x" , i ) ;
#endif	
	
	return i ;
}

//ok
iBOOL i51AdeSmsSetHook ( iS32 hookid , i51SMSHOOKCALLBACK callback )
{
	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsSetHook , S , %x , %x" , hookid , callback ) ;
#endif	
	
	if (  hookid < 0 || hookid > MAX_SMS_HOOK_COUNT -1 )
	{
		i51AdeOsLog(i51_LOG_SC , "SMS : i51AdeSmsSetHook , F , ID OUT" ) ;
		return iFALSE ;
	}
	
	if ( iNULL == callback ) 
	{
		i51AdeOsLog(i51_LOG_SC , "SMS : i51AdeSmsSetHook , F , callback == NULL" ) ;
		return iFALSE ;
	}

    if(iFALSE == i51SmsHookList[hookid].used)
    {
    	i51AdeOsLog(i51_LOG_SC , "SMS : i51AdeSmsSetHook , F, NOT Created" ) ;
        return iFALSE;
    }
    
	i51SmsHookList[hookid].i51SmsHookCB= callback ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsSetHook E , ret = %x" , iTRUE ) ;
#endif	
	
	return iTRUE ;
}

//ok
iS32 i51AdeSmsRunHook (  iS32 sim ,  iCHAR* number , iU32 numlen , iU16* buffer , iU32 textlen )
{
	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

    iU16 i = 0 ;
    i51SMSHKR result = 0 ;
    i51SMSHOOKCALLBACK SmsHook = 0 ;   

	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsRunHook , S , %x , %x , %x , %x , %x" , sim , number ,numlen , buffer , textlen ) ;

    for ( i = 0 ;i < MAX_SMS_HOOK_COUNT ; i ++)
    {
    	if ( i51SmsHookList[i].i51SmsHookCB!= iNULL ) 
        {
            SmsHook = i51SmsHookList[i].i51SmsHookCB ;
            result = SmsHook( sim , number , numlen , buffer , textlen ) ;
            
            if( result != i51_SMS_HOOK_NORMAL ) 
            {
    		    i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsRunHook , need to delete , %x" , result ) ;
    	        return result ;
    	    }
    	}
    }

    i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsRunHook , E , %d" , i ) ;
    
   //不做任何处理
    return i51_SMS_HOOK_NORMAL;  
}

//ok
iBOOL i51AdeSmsReleaseHook (  iS32 hookid )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 销毁一个钩子
	//	成功返回> 0 ，失败返回 0		

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsReleaseHook , S , %x " , hookid ) ;
#endif	
    
	if (  hookid < 0 || hookid > MAX_SMS_HOOK_COUNT - 1 ) 
    {
		i51AdeOsLog(i51_LOG_SC , "SMS : i51AdeSmsReleaseHook ID OUT" ) ;
		return iFALSE ;
	}
    
	i51SmsHookList[hookid].i51SmsHookCB = iNULL ;
	i51SmsHookList[hookid].used = iFALSE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsReleaseHook , E") ;
#endif	
	
	return iTRUE ;
}

iBOOL i51AdeSmsGetPhoneBook ( i51PHONEBOOK* phonebook , iU32* length )
{
	//	author : jone
	//	since : 2012.3.12
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 nPhbRealSize = 0;
	iBOOL ret = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetPhoneBook , S , %x,%x" , phonebook, length) ;
#endif	

	if (NULL == length)
	{
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetPhoneBook , F , length = NULL");
		return iFALSE;
	}

	//phonebook == 0
	if (NULL == phonebook)
	{
		*length = i51GetPhbContactsNum();
		
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetPhoneBook , E , %d" , *length) ;
#endif	

		return iTRUE;
	}

	if(*length == 0)
	{
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetPhoneBook , F , *length = 0");
		return iFALSE;
	}

	if(*length > i51GetPhbContactsNum())
	{
		*length = i51GetPhbContactsNum();
	}

	//phonebook != 0	
	do 
	{	
		ret = i51AdeSmsGetphbContentNumAndName(phonebook[nPhbRealSize].name, phonebook[nPhbRealSize].number, nPhbRealSize);

		if (iTRUE == ret)
		{
			nPhbRealSize++;
		}
		else
		{
			i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetPhoneBook , F , %d" , ret);
		}

	} while (ret && (nPhbRealSize < *length));

	*length = nPhbRealSize;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsGetPhoneBook , E , %d" , nPhbRealSize) ;
#endif	

	return iTRUE;
}

iBOOL i51AdeSmsCreate ( i51ADECALLBACK callback )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsCreate , S");
#endif

	if (NULL != callback)
	{
		i51AdeSmsCreate_CB = callback;

		i51AdeOsTimerStart(i51AdeOsTimerCreate(iFALSE, 0), smsCreate_cb, 30);

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsCreate , E");
#endif

		return iTRUE;
	}

	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSocCreate , F");

	return iFALSE;
}

iBOOL i51AdeSmsRelease ( void )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsRelease , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SMS : i51AdeSmsRelease , E");
#endif

	return iTRUE ;

}
