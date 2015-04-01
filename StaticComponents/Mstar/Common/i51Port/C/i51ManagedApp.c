#include "mmi_baseapplet.h"
#include "mmi_launcher_common.h"
#include "mmi_audiorecordersrv.h"
#include "mmi_mediasrv_common.h"
#include "mmi_common_dialog.h"
#include "mmi_srv_fmradio.h"
#include "mmi_srv_audioplayer.h"
#include "i51.h"
#include "i51AdeOs.h"
#include "i51AdeMmi.h"
#include "i51AdeSms.h"
#include "i51AdeSoc.h"
#include "i51AdeStd.h"
#include "i51Kernel.h"
#include "i51Port.h"
#include "i51Script.h"
#include "i51AdeMStar.h"


typedef struct
{
    APPLET_BASE_ELEMENTS;
} i51ManagedAppData_t;

extern void i51AdeMmiRecorderGetDataCb(void* pData, u32 nDataLen);
extern void i51AdeMmiRecorderStopCb(iBOOL bResult);
extern void i51AdeMmiRecorderStatusCb(iBOOL bResult);
extern iU8 gi51IsRunning;

static boolean i51ManagedAppStartCB(IApplet *pApplet, u32 nEvt, u32 nParam1, u32 nParam2);
static boolean i51ManagedAppStopCB(IApplet *pApplet, u32 nEvt, u32 nParam1, u32 nParam2);
static boolean i51ManagedAppSuspendCB(IApplet *pApplet, u32 nEvt, u32 nParam1, u32 nParam2);
static boolean i51ManagedAppResumeCB(IApplet *pApplet, u32 nEvt, u32 nParam1, u32 nParam2);
static void i51ManagedLaunchi51App(u32 param);
static void i51ManagedAppRecorderMonitorStart(void *pMonitorBase);
static void i51ManagedAppRecorderMonitorStop(void *pMonitorBase);

#define I51MGRAPPDEBUG

const u8 g_i51EntryWithoutSim = 0;		//当客户要求无SIM卡也可进入时，设置为1

#ifdef __I51_APP_LANG_CN__
#define I51_STR_QUIT_BACKGROUNDAPP_INFO	L"请先退出后台应用"
#define I51_STR_INSERT_SIM_INFO			L"请插入SIM卡"
#else
#define I51_STR_QUIT_BACKGROUNDAPP_INFO	L"Please quit background applications first"
#define I51_STR_INSERT_SIM_INFO			L"Please insert SIM card"
#endif


void i51LaunchManagedApp(IApplet *pApplet, u32 nParam)
{
	u8 nMasterSimStatus = COMMON_SIM_STATUS_NOSIM, nSlaveSimStatus = COMMON_SIM_STATUS_NOSIM;
	MMI_Common_GetSIMStatus(pApplet, &nMasterSimStatus, &nSlaveSimStatus);

	/*	判断SIM卡是否存在	*/
	if(nMasterSimStatus == COMMON_SIM_STATUS_ACTIVE || nSlaveSimStatus == COMMON_SIM_STATUS_ACTIVE || g_i51EntryWithoutSim)
	{
		FmradioSrvStatus_e eFMStatus = SrvFmradioGetStatus();
		AudioPlayerSrvPlayerStatus_e ePlayState = AUDIOPLAYERSRV_IDLE;
		SrvAudioPlayerGetPlayerStatus(&ePlayState);
	
		/*	判断后台应用	*/
		if ((AUDIOPLAYERSRV_PLAY == ePlayState) || (AUDIOPLAYERSRV_PAUSE == ePlayState) || (FMRADIOSRV_STATUS__PLAY ==eFMStatus)
#ifdef __SKYENGINE__		
		|| ( SH_QueryApplet(CLSID_SKY_ENGINE_QQ_APP))	
#endif
#ifdef __NCCQQ_MMI__
		|| ( SH_QueryApplet(CLSID_QQSRV))
#endif
#ifdef __TENCENT_MCARE__
		|| ( SH_QueryApplet(CLSID_TENCENT_MCARE))
#endif
			)
		{
			MMI_CreateInformationDialog(pApplet, COMMON_DIALOG_WARNINGDIALOG,I51_STR_QUIT_BACKGROUNDAPP_INFO, LAYOUT_STYLE_VERTICAL, COMMON_DIALOG_AUTOCLOSE_TIME, NULL); 	
		}
		else
		{
			IApplet * pApp = NULL;
		    SH_StartAppletEx(CLSID_I51MGRAPP, APM_LAUNCH_NORMAL, &pApp, nParam, NULL);
		}
	}
	else
	{
		MMI_CreateInformationDialog(pApplet, COMMON_DIALOG_WARNINGDIALOG,I51_STR_INSERT_SIM_INFO, LAYOUT_STYLE_VERTICAL, COMMON_DIALOG_AUTOCLOSE_TIME, NULL); 	
	}
	
}



BEGIN_EVENTCB_TABLE(g_tEventToFuncCBTbl)
    {MAE_EVT_APP_START,		i51ManagedAppStartCB},
    {MAE_EVT_APP_STOP,		i51ManagedAppStopCB},
    {MAE_EVT_APP_SUSPEND,	i51ManagedAppSuspendCB},
    {MAE_EVT_APP_RESUME,	i51ManagedAppResumeCB},
END_EVENTCB_TABLE;

MAE_Ret i51ManagedAppNew(MAE_ClsId nClsId, void **ppObj)
{
	return BaseAppletInit(nClsId, ppObj, sizeof(i51ManagedAppData_t), g_tEventToFuncCBTbl);
}

static boolean i51ManagedAppStartCB(IApplet *pApplet, u32 nEvt, u32 nParam1, u32 nParam2)
{
	i51ManagedAppRecorderMonitorStart((void*)pApplet);
	i51ManagedLaunchi51App(nParam2);
	return TRUE;
}

static boolean i51ManagedAppStopCB(IApplet *pApplet, u32 nEvt, u32 nParam1, u32 nParam2)
{
	i51ManagedAppRecorderMonitorStop((void*)pApplet);
	return TRUE;
}

static boolean i51ManagedAppSuspendCB(IApplet *pApplet, u32 nEvt, u32 nParam1, u32 nParam2)
{
	return TRUE;
}

static boolean i51ManagedAppResumeCB(IApplet *pApplet, u32 nEvt, u32 nParam1, u32 nParam2)
{
	SH_CloseApplet(FALSE);
	return TRUE;
}

static void i51ManagedAppLaunchVendorAppRspCB(void *pUserData, MAE_Ret nRspCode)
{
    FREEIF(pUserData);
}

static void i51ManagedLaunchi51App(u32 param)
{
    CmnAppLaunchRspCbInfo_t tRspCb = {0};
    u32 *pSxmParam = MALLOC(sizeof(u32));
    MAE_WChar Url[128];
    int nLen = 128;
	u16 nClsId = CLSID_I51BASE;

    if (NULL != pSxmParam)
    {
        *pSxmParam = param;
        SWPRINTF(Url, nLen, L"Launch?APM_Mode=%d&ClsId=%d&VenData=%d&VenDataLen=%d&TransInId=%d&TransOutId=%d", APM_LAUNCH_NORMAL, nClsId, (u32)pSxmParam, sizeof(u32), 0, 0);
        tRspCb.pvUserData = (void *)pSxmParam;
        tRspCb.pfnLaunchListenerCb = i51ManagedAppLaunchVendorAppRspCB;
        if (MAE_RET_SUCCESS != LaunchApp(MAE_CLSID_2_SCHEMEID(nClsId), Url, &tRspCb))
        {
            FREEIF(pSxmParam);
        }
    }
}

///////////////////////////////PCM Recorder
static void i51ManagedAppPcmGetRecStreamDataCb(void *pThis, u16 *pData, u32 nDataLen, u32 nUserData)
{
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppPcmGetRecStreamDataCb S , nDataLen = %d", nDataLen);
	
	i51AdeMmiRecorderGetDataCb((void*)pData, nDataLen);
}

static void i51ManagedAppStartPCMRecordCb(void *pThis, u32 eErrorCode)
{
	//PCM没有状态回调，暂不处理
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppPcmGetRecStreamDataCb S , eErrorCode = %d", eErrorCode);
}

static void i51ManagedAppPCMRecStatusCb(void *pThis, u32 nCusData, AudioRecSrvStatusInd_e eStatus, AudioRecSrvStatusNotifyInfo_t *pInfo)
{
	//PCM没有状态回调，暂不处理
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppPcmGetRecStreamDataCb S , eStatus = %d", eStatus);
}

static void i51ManagedAppStopPCMRecordCb(void *pThis, u32 eErrorCode)
{
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStopPCMRecordCb S , eErrorCode = %d", eErrorCode);
	
	if(MEDIASRV_ERR__OK == eErrorCode)
	{
		i51AdeMmiRecorderStopCb(iTRUE);
	}
	else
	{
		i51AdeMmiRecorderStopCb(iFALSE);
	}
}

static void i51ManagedAppStartPcmRecord(void)
{
	MAE_Ret nRet = MAE_RET_SUCCESS;

	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStartPcmRecord , S");

	SrvAudioRecSetGetRecStreamDataCb(i51ManagedAppPcmGetRecStreamDataCb, 0);
		
	nRet = SrvAudioRecStartStreamRecord(i51ManagedAppStartPCMRecordCb);

	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStartPcmRecord , E , nRet = %d", nRet);
}

static void i51ManagedAppStopPcmRecord(void)
{	
	MAE_Ret nRet = MAE_RET_SUCCESS;

	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStopPcmRecord , S");

	SrvAudioRecSetGetRecStreamDataCb(NULL, 0);
	
	nRet = SrvAudioRecStopStreamRecord(i51ManagedAppStopPCMRecordCb);

	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStopPcmRecord , E, nRet = %d", nRet);
}

//////////////////////////////////////AMR Record
static void i51ManagedAppAMRRecorderStatusIndCb(void *pData, u32 nCusData, AudioRecSrvStatusInd_e eStatus, AudioRecSrvStatusNotifyInfo_t *pInfo)
{
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppAMRRecorderStatusIndCb , S");
	
	if(eStatus == AUDIO_REC_SRV_STATUS__ERROR)
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppAMRRecorderStatusIndCb , ERR 1");
		i51AdeMmiRecorderStatusCb(iFALSE);		//错误状态报告
	}
	else if(eStatus == AUDIO_REC_SRV_STATUS__SAPCE_FULL)
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppAMRRecorderStatusIndCb , ERR 2");
		i51AdeMmiRecorderStatusCb(iTRUE);		//状态报告: 盘符已满
	}
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppAMRRecorderStatusIndCb , E");
}

static void i51ManagedAppStartRecordAMRCb(void *pThis, MediaSrvErrCode_e nErrCode)
{	
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStartRecordAMRCb , S, nErrCode = %d", nErrCode);
	if(nErrCode != MEDIASRV_ERR__OK)
	{
		i51AdeMmiRecorderStatusCb(iFALSE);		//错误状态报告
	}
}

static void i51ManagedAppStopRecordAMRCb(void *pThis, MediaSrvErrCode_e nErrCode, AudioRecSrvFileInfo_t *pFileInfo)
{	
	if(nErrCode == MEDIASRV_ERR__OK)
	{
		i51AdeMmiRecorderStopCb(iTRUE);
	}
	else
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStopRecordAMRCb , S, nErrCode = %d", nErrCode);
		i51AdeMmiRecorderStopCb(iFALSE);
	}
}

static void i51ManagedAppStartAMRRecord(void)
{
	MAE_Ret nRet = MAE_RET_SUCCESS;
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStartAMRRecord , S");
	
	nRet = SrvAudioRecStartRecord(MEDIASRV_AUD_CODEC__AMR_NB, MEDIASRV_REC_PATH__ABSOLUTE, (MAE_WChar*)&i51Recoder.fileName[0], i51ManagedAppStartRecordAMRCb);

	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStartAMRRecord , nRet = %d", nRet);
	if(nRet == MAE_RET_SUCCESS)
	{
		SrvAudioRecSetRecStatusCb(i51ManagedAppAMRRecorderStatusIndCb, 0);
	}
	else
	{
		i51AdeMmiRecorderStatusCb(iFALSE);		//错误状态报告
	}
}

static void i51ManagedAppStopAMRRecord(void)
{
	MAE_Ret nRet = MAE_RET_SUCCESS;

	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStopAMRRecord , S");
	
	SrvAudioRecSetRecStatusCb(NULL, 0);
	
	nRet = SrvAudioRecStopRecord(i51ManagedAppStopRecordAMRCb);

	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppStopAMRRecord , ret = %d", nRet);
	
	if(MAE_RET_SUCCESS != nRet)
	{
		i51AdeMmiRecorderStopCb(iFALSE);
	}
}

///////////////////Record Monitor
static void i51ManagedAppRecorderMonitor(void* pUser, void* pUserData)
{
	if(!gi51IsRunning)
		return;

	if(g_i51RecorderActionFlag == I51_RECORDER_ACTION_NONE)
		return;

	if(g_i51RecorderActionFlag == I51_RECORDER_ACTION_START_PCM)
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppRecorderMonitor , Start PCM Record");
		i51ManagedAppStartPcmRecord();
	}
	else if(g_i51RecorderActionFlag == I51_RECORDER_ACTION_STOP_PCM)
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppRecorderMonitor , Stop PCM Record");
		i51ManagedAppStopPcmRecord();
	}
	else if(g_i51RecorderActionFlag == I51_RECORDER_ACTION_START_AMR)
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppRecorderMonitor , Start AMR Record");
		i51ManagedAppStartAMRRecord();
	}
	else if(g_i51RecorderActionFlag == I51_RECORDER_ACTION_STOP_AMR)
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppRecorderMonitor , Stop AMR Record");
		i51ManagedAppStopAMRRecord();
	}

	g_i51RecorderActionFlag = I51_RECORDER_ACTION_NONE;
}

static void i51ManagedAppRecorderMonitorStart(void *pMonitorBase)
{
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppRecorderMonitorStart , S");
	if(!SH_IsTimerActive(i51ManagedAppRecorderMonitor, pMonitorBase))
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppRecorderMonitorStart , Monitor Start");
		SH_CreatePeriodicTimer(100, i51ManagedAppRecorderMonitor, pMonitorBase, 0);
	}
}

static void i51ManagedAppRecorderMonitorStop(void *pMonitorBase)
{
	i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppRecorderMonitorStop , S");
	if(SH_IsTimerActive(i51ManagedAppRecorderMonitor, pMonitorBase))
	{
		i51AdeOsLog( i51_LOG_SC ,"PORT : i51ManagedAppRecorderMonitorStop , Monitor Stop");
		SH_CancelTimerEx (i51ManagedAppRecorderMonitor, pMonitorBase);
	}
}

