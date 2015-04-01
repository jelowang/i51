
/*

The Implementing of 'i51Port' , Module : 'PORT'.	
Layer belong : 'Static Components'.
For more details about 'i51Port' you could read : "The_i51_SC_Port_Interface.xls"
For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"

The 51PK Platform was implementing based on 'The i51 Architecture'.
'The i51 Architecture' is designed by Jelo.

Copyright (C) 2011 , PKIG Tech. Co., Ltd.

*/
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

#include "ven_sdk.h"
#include "ven_handle.h"
#include "ven_stc.h"
#include "ven_ts.h"
#include "ven_os.h"
#include "ven_kpd.h"
#include "ven_graphics.h"
#include "ven_codec.h"
#include "ven_ui.h"
#include "ven_util.h"
#include "ven_msg.h"
#include "ven_drv.h"
#include "mmi_launcher_common.h"
#include "mmi_mmsrv_def.h"
#include "mmi_common_util.h"
#include "mmi_srv_mobilitymgr.h"

//出包版本
const char *i51PageVersion = "0.1.03";
//日志
#define I51_KERNEL_P4_VERSION 855
#define I51_ADAPTER_P4_VERSION 833

const iU16* I51_ROOT_DIR = L"/i51/";
const iU16* I51_LOG_FILENAME = L"i51.log";
const iU16* I51_TRACE_FILENAME = L"i51.trace";

#define I51_LOG_IDXTBL_SIZE (2)
#define I51_LOG_HEAD_SIZE	(2)
#define I51_LOG_FILENAME_SIZE (128)

static iU8* g_pu8LogDataBaseBuf = 0;
iBOOL g_bLogFileExist = iFALSE;
iBOOL g_bTraceFileExist = iFALSE;
iU16 g_i51LogFileName[I51_LOG_FILENAME_SIZE];

iBOOL i51AdePortSysLogLoadData();

//测试接口

iU32 g_i51PkigHandle = 0;
ven_GraphicsCanvas *g_i51PkigCanvas = NULL;
iS32 g_i51MediaHandle = 0;
ven_FontAttr *g_i51PkigFontNormal = NULL;//字体属性

static iU8 g_i51ExitFlag = 0;
static iS32 i51PortHintScreenActive = 0;

static char i51_backlight_statu = 1;


int gi51Game_mode = 0;
int gi51Game_id = 0;
int gi51PowerOnDelay = 0;
iU8 gi51IsRunning = 0;

#define I51PORT_GETSTATION_TIMER_INTERVAL	(1000)
#define I51PORT_GETSTATION_TIMER_LOOP_COUNT	(60 * 10)
iU32 g_i51CellInfoZid = 0;
iU32 g_i51CellInfoBsid = 0;
static iU32 g_i51CellInfoTimerCount = 0;

typedef struct{
    u8 APType;
    u32 Data1;
    u32 Data2;
}osca_ap_cb_data_t;
//***********input******
typedef struct 
{
	iU16 i51_value;  
	iU16 mst_key;  
} I51_KEY_MATCH;

static I51_KEY_MATCH I51keyMap[] =
{
	{i51_KEY_NUM0, VEN_KEY_0},
	{i51_KEY_NUM1, VEN_KEY_1},
	{i51_KEY_NUM2, VEN_KEY_2},
	{i51_KEY_NUM3, VEN_KEY_3},
	{i51_KEY_NUM4, VEN_KEY_4},
	{i51_KEY_NUM5, VEN_KEY_5},
	{i51_KEY_NUM6, VEN_KEY_6},
	{i51_KEY_NUM7, VEN_KEY_7},
	{i51_KEY_NUM8, VEN_KEY_8},
	{i51_KEY_NUM9, VEN_KEY_9},
	{i51_KEY_LEFT , VEN_KEY_SKLEFT},
	{i51_KEY_RIGHT , VEN_KEY_SKRIGHT},
	{i51_KEY_ARROW_UP , VEN_KEY_UP},
	{i51_KEY_ARROW_DOWN , VEN_KEY_DOWN},
	{i51_KEY_ARROW_LEFT , VEN_KEY_LEFT},
	{i51_KEY_ARROW_RIGHT , VEN_KEY_RIGHT},
	{i51_KEY_CALL , VEN_KEY_SEND},
	{i51_KEY_END_CALL , VEN_KEY_END},
	{i51_KEY_STAR , VEN_KEY_STAR},
	{i51_KEY_SHARP , VEN_KEY_HASH},
	{i51_KEY_VOL_UP , VEN_KEY_SIDE_UP},
	{i51_KEY_VOL_DOWN , VEN_KEY_SIDE_DN},
	{i51_KEY_ENTER , VEN_KEY_ACTION} 
};

static int  i51EntryDo(void);
static void i51ExitDo(void);
static void _i51PortInitLogFile();

//extern iS32 i51KernelRunStaticApp ( iU16* name , iU8* buffer , iU32 buflen , void* parameter , iBOOL staticscope ) ;

//extern iS32 i51KernelRunDynamicApp ( iU16* name , void* parameter ) ;
extern unsigned char* i51SapGeti51Installer();
extern int i51SapGeti51InstallerSize();


extern const unsigned char* i51SapGeti51Explorer ();
extern const int i51SapGeti51ExplorerSize ();

extern const unsigned char* i51SapGetAppBoot ();
extern const int i51SapGetAppBootSize ();
static void i51PortHintScreenExit(void)
{
	i51ExitDo();
}

/********************************移植插代码*********************************************/
#define i51CALLTIME  20   // 通话测试,运营测试结束后改20


iU16 p_i51start_data[32];
void i51Port_start_Data_Init(unsigned short * pData)
{
    if(pData){
        ven_std_wstrcpy(p_i51start_data,pData);
    }  
}
void i51PortShutDownNotify(void)
{
    i51KernelExit( i51_KERNEL_EXIT_SAP ); 
}
void i51PortQuitFrontEndApp(void)
{
    if(i51PortHintScreenActive)
	{
	    i51AdeOsLog ( i51_LOG_SC , "PORT : i51PortQuitFrontEndApp,提示屏幕退出" ) ; 
		i51PortHintScreenActive = 0;
		i51PortHintScreenExit();
		ven_stc_notifyStop(g_i51PkigHandle);
	}
	else
	{
	    i51AdeOsLog ( i51_LOG_SC , "PORT : i51PortQuitFrontEndApp, exit frontApp" ) ; 
		i51KernelExit( i51_KERNEL_EXIT_DAP);
	}
    
}
void i51_backlight_setting_catch(u8 level)
{
    if(i51KernelGetInitStatus() == i51_KERNEL_INIT_SUCCEED)
    {
        if(level == 0)
        {
            if(i51_backlight_statu == 1)
            { 
                i51AdeOsLog ( i51_LOG_SC , "PORT : i51 idle entry" ) ;
                i51KernelSendEvent( i51_MSG_ENTERIDLE, NULL);
                i51_backlight_statu = 0;
            }
        }
        else
        {
            if(i51_backlight_statu == 0)
            { 
                i51AdeOsLog ( i51_LOG_SC , "PORT : i51 idle exit" ) ;
                i51KernelSendEvent( i51_MSG_EXITIDLE, NULL);
                i51_backlight_statu = 1;
            }
        }
    }	
}

extern ven_msg_sms_service_center_t smsSrvCenter1;
extern ven_msg_sms_service_center_t smsSrvCenter2;
//sms center
void i51PortGetServiceCenterNum(u8 *masterAddr, u8 *slaveAddr)
{
    if(ven_std_strlen((char*)masterAddr) > 0)
   {
      ven_std_memset (smsSrvCenter1.addr, 0, sizeof(smsSrvCenter1.addr));
      ven_std_strcpy (smsSrvCenter1.addr, (const char*)masterAddr) ;
   }


   if( ven_std_strlen((char *)slaveAddr) > 0)
   {
       ven_std_memset (smsSrvCenter2.addr, 0, sizeof(smsSrvCenter2.addr));
       ven_std_strcpy (smsSrvCenter2.addr, (const char*)slaveAddr) ;
   }
}

//////////////////////////////////////////////////////////////////////
#define R_OF_565(p)      (((p)&0xf800) >> 8)
#define G_OF_565(p)      (((p)&0x7e0) >> 3)
#define B_OF_565(p)      (((p)&0x1f) << 3)

iBOOL i51PortRunHintScreen (int type) 
{
    ven_GraphicsDisplayInfo_t DispInfo;
    ven_GraphicsRect_t rect;
    iU16 *str1,*str2,*str3;
    

#ifdef __I51_APP_LANG_EN__
	iU16 *hitstr1 = L"The system is initializing,";
    iU16 *hitstr2 = L"please try again later!";
    iU16 *backstr= L"Return";
    iU16 *hitstr4 = L"file not found,";
    iU16 *hitstr5 = L"please check";
#else
	iU16 *hitstr1 = L"系统正在初始化，";
    iU16 *hitstr2 = L"请稍后重试！";
    iU16 *backstr= L"返回";
    iU16 *hitstr4 = L"游戏文件不存在，";
    iU16 *hitstr5 = L"请检查文件";
#endif
	
	if(type == 0)
	{
        str1 = hitstr1;
        str2 = hitstr2;
        str3 = backstr;
	}
	else
	{
        str1 = hitstr4;
        str2 = hitstr5;
        str3 = backstr;
	}
		
	i51AdeOsLog ( i51_LOG_SC , "PORT : 提示屏幕进入" ) ;   
	i51PortHintScreenActive = 1;
    i51EntryDo();
    if (g_i51PkigCanvas && g_i51PkigFontNormal)
    {
        ven_graphics_setColor(g_i51PkigCanvas, VEN_CLR_BRUSH, 0xffff0000); //black
        ven_graphics_setColor(g_i51PkigCanvas, VEN_CLR_BG, 0x0000); //black
        ven_graphics_setColor(g_i51PkigCanvas, VEN_CLR_TEXT, 0xffffffff ); //black
        ven_graphics_clearCanvas( g_i51PkigCanvas );

        ven_graphics_getDisplayInfo( &DispInfo );
        ven_graphics_drawText(g_i51PkigCanvas, g_i51PkigFontNormal,str1 , DispInfo.width/2 - 24*3-12, DispInfo.height/2 -15);
        ven_graphics_drawText(g_i51PkigCanvas, g_i51PkigFontNormal,str2 , DispInfo.width/2 - 24*3-12, DispInfo.height/2 +15);
	    ven_graphics_drawText(g_i51PkigCanvas, g_i51PkigFontNormal,str3 , DispInfo.width - 40, DispInfo.height - 40);

	    rect.x = 0;
        rect.y = 0;
        rect.w = DispInfo.width;
        rect.h = DispInfo.height;
        ven_graphics_setDirtyArea(g_i51PkigCanvas, &rect);
        ven_graphics_screenUpdate( g_i51PkigCanvas, VEN_GRA_UPDATE_NORMAL);
    }
      
}

iBOOL i51PortRunStaticApp ( iU16* name , iU8* buffer , iU32 buflen , void* parameter,iU16 flag ,iBOOL frontapp) 
{
    return i51KernelRunStaticApp ( name , buffer , buflen , parameter ,flag ,frontapp);
}
static iBOOL i51PortInRect(iU16 x,iU16 y)
{
	ven_GraphicsDisplayInfo_t DispInfo;
	ven_GraphicsRect_t rectR={0,0,0,0};

	ven_graphics_getDisplayInfo( &DispInfo );
	if(x >DispInfo.width -80&&x< DispInfo.width 
		&&y>DispInfo.height-80&&y<DispInfo.height)
	{
		return iTRUE;
	}
	return iFALSE;
}
static iU16 i51PortHintScreenInputHandle(iU32 events , iU32 parameter )
{
	iU16 flag = 0;
	
	switch(events){
		case i51_MSG_KEYUP:
			if(parameter == i51_KEY_RIGHT)
			{
				flag = 1;
			}
			break;
		case i51_MSG_PADUP:
			{	
				iU16 x = parameter >>16;
				iU16 y = parameter &0x0000ffff;
				if(i51PortInRect(x,y))
				{
					flag = 1;
				}
			}
			break;
	}
	if(flag){
		i51AdeOsLog ( i51_LOG_SC , "PORT : 提示屏幕退出" ) ; 
		i51PortHintScreenActive = 0;
		i51PortHintScreenExit();
		ven_stc_notifyStop(g_i51PkigHandle);	
	}

}

static iU16 i51GetMatchKey(iU16 mst_key) 
{

	//	author : osca
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.

	int i = 0;
	iU16 ret = 0xffff;

	int num = sizeof(I51keyMap) / sizeof(I51_KEY_MATCH);
	for (i = 0; i < num; i++) 
	{
		if (I51keyMap[i].mst_key == mst_key) 
		{
			ret = I51keyMap[i].i51_value;
			break;
		}
	}

	return ret;
}
void i51KpdHandle(ven_kpd_act_t keyAct, ven_kpd_key_code_t keyCode)
{
	iU32 i51KeyCode = 0xffff;
	iU32 i51KeyType = 0xffff;

	switch(keyAct)
	{
	case VEN_KPD_ACT_PRESS:
		i51KeyType = i51_MSG_KEYDOWN;
		break;	
	case VEN_KPD_ACT_RELEASE:
		i51KeyType = i51_MSG_KEYUP;
		break;	
	default:
		break;
	}

	i51AdeOsLog ( i51_LOG_SC , "PORT : i51KpdHandle Entry. keyCode = %d", keyCode ) ;
	//如果没有定义VEN_KEY_END或者VEN_KEY_HOME就把没定义的哪项去掉
	if((keyAct == VEN_KPD_ACT_PRESS) && ((keyCode == VEN_KEY_END) || (keyCode == VEN_KEY_HOME)))
	{
		i51PortQuitFrontEndApp();
		return;
	}			

	i51KeyCode = i51GetMatchKey(keyCode);
	if(i51KeyType != 0xffff && i51KeyCode != 0xffff)
	{
		if(i51PortHintScreenActive)
		{
			i51PortHintScreenInputHandle(i51KeyType,i51KeyCode);
		}
		else
		{
			i51KernelSendEvent(i51KeyType,i51KeyCode);	
		}
	}
}

void i51TsHandle(ven_ts_evt_t type, u32 x, u32 y)
{
	iU32 i51TSCode = 0xffff;
	switch(type)
	{
	case VEN_TS_EVENT_PRESS:
		i51TSCode = i51_MSG_PADDOWN;
		break;	
	case VEN_TS_EVENT_RELEASE:
		i51TSCode = i51_MSG_PADUP;
		break;	
	case VEN_TS_EVENT_MOVE:
		i51TSCode = i51_MSG_PADMOVE;
		break;	
	default:
		break;		
	}

	if(i51TSCode != 0xffff)
	{
		if(i51PortHintScreenActive)
		{
			i51PortHintScreenInputHandle(i51TSCode, x<<16|y);
		}
		else
		{
			i51KernelSendEvent( i51TSCode, x<<16|y);
		}
	}	

} 

static int i51EntryDo(void)
{

	//	author : osca
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.

	ven_req_data_t tVenReq = {0};
	ven_GraphicsDisplayInfo_t DispInfo;

	if(NULL == g_i51PkigCanvas)
	{
		//创建画布
		ven_graphics_getDisplayInfo( &DispInfo );
		ven_graphics_createCanvas( &g_i51PkigCanvas, DispInfo.width, DispInfo.height);
	}

	if(NULL == g_i51PkigFontNormal)
	{
		ven_font_createFontAttribute( VEN_FONT_SIZE_NORMAL, VEN_FONT_STYLE_PLAIN, &g_i51PkigFontNormal );
	}

	if(g_i51MediaHandle == 0)
	{
		//打开媒体句柄
		tVenReq.Handle = g_i51PkigHandle;
		tVenReq.bSync  = TRUE;
		ven_codec_openMedia(&g_i51MediaHandle, &tVenReq);
	}

	//注册按键                  
	ven_ts_subscribe(g_i51PkigHandle, i51TsHandle );
	ven_kpd_subscribe(g_i51PkigHandle, i51KpdHandle );

	//ven_kpd_enablePressTone(g_i51PkigHandle);
    //ven_drv_subscribeAcs(g_i51PkigHandle, VEN_DRV_ACS_TYPE_HEADSET | VEN_DRV_ACS_TYPE_MEMCARD, 0);

	// Set LCD backlight always on
	tVenReq.Handle = g_i51PkigHandle;
	tVenReq.bSync = iTRUE;

	//ven_ui_setBacklightBehavior(VEN_UI_BACKLIGHT_ON, &tVenReq);
	i51AdeOsScreenAlwaysLight ( 1 );
    //ven_ui_setInterruptLevel(VEN_UI_INT_DENY_DIALOG, &tVenReq);

	return 0;
}

static void i51ExitDo(void)
{

	//	author : osca
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.

	ven_req_data_t tVenReq = {0};
	iU32 ret=0;

    ven_std_wstrcpy(p_i51start_data,L"");

	if(NULL != g_i51PkigCanvas)
	{
		//释放画布
		ven_graphics_releaseCanvas( g_i51PkigCanvas );
		g_i51PkigCanvas = NULL;
	}

	if(NULL != g_i51PkigFontNormal)
	{
		ven_font_releaseFontAttribute(g_i51PkigFontNormal);
		g_i51PkigFontNormal = NULL;
	}

	if(0 != g_i51MediaHandle)
	{
		//关闭媒体句柄
		
		tVenReq.Handle = g_i51PkigHandle;
		tVenReq.bSync  = TRUE;
		ret = ven_codec_closeMedia(g_i51MediaHandle, &tVenReq);
		g_i51MediaHandle = 0;

		i51AdeOsLog ( i51_LOG_SC , "PORT : EXIT CLOSE MEDIA ret=%d",ret ) ;
	}

	//反注册按键   
	ven_ts_unsubscribe(g_i51PkigHandle);
	ven_kpd_unsubscribe(g_i51PkigHandle);

	// Set LCD backlight always on
	tVenReq.Handle = g_i51PkigHandle;
	tVenReq.bSync = iTRUE;

    i51AdeOsScreenAlwaysLight ( 0 );
	//ven_ui_setBacklightBehavior(VEN_UI_BACKLIGHT_AUTO, &tVenReq);
}

int i51SapAppBootPara[2] = {i51CALLTIME*60, 60};
#define I51CALLTIME1  L"/CARD/i51CALLTIME"
#define I51CALLTIME2  L"/NAND/i51CALLTIME"
#define I51CALLTIME3  L"/NOR0/i51CALLTIME"
#define I51CALLTIME4  L"/NOR_FLASH_0/i51CALLTIME"
static void i51KernelInitCB(iBOOL result)
{
    int ret,i;
    iU16 *fn[4]={I51CALLTIME1,I51CALLTIME2,I51CALLTIME3,I51CALLTIME4};

    i51AdeOsLog ( i51_LOG_SC , "PORT : i51KernelInitCB result=%d", result ) ; 
    i51AdeOsLog ( i51_LOG_SC , "PORT : i51PageVersion=%s", i51PageVersion ) ;
#ifndef __I51_APP_LANG_EN__   
	if(result == 1)
	{
	    for(i=0;i<4;i++){
	    
            if ( ven_file_wexists(fn[i]) ) //通话时间测试
            {
                i51SapAppBootPara[0] = 1*60;
                break;
            }
	    }
        i51PortRunStaticApp ( L"AppBoot" , (unsigned char*)i51SapGetAppBoot(), (unsigned int)i51SapGetAppBootSize() ,  i51SapAppBootPara,1,0) ; 
	} 
#endif	
}
static void i51bootTimerCB(iU16 timerid)
{
	ven_os_stopTimer(timerid);
	ven_os_releaseTimer(timerid);
    gi51PowerOnDelay = 1;

	_i51PortInitLogFile();		//KernelInit前初始化Log文件

	i51KernelInit ( i51KernelInitCB );	
}
extern void i51PortRunDynamicAPP(iU16 *name);

static void pkig_i51_start(void *pData,iU32 len)
{
	ven_rsp_data_t tVenRSp = {0};
	ven_req_data_t tVenReq = {0};
	iU32 timerid,ret;
	iU32 bsid,zid;
	iU32 arg=0;
    ven_os_heap_status_t hs = {0};

	
	if(pData && (len > 0))
	{
		arg = *(iU32*)pData;
		gi51Game_mode = (arg&0xf000)>>12;
		gi51Game_id = arg&0x0fff;	  
	}
	    
	i51AdeOsLog ( i51_LOG_SC , "PORT : pkig_i51_start gi51Game_id=%d", gi51Game_id ) ; 
    ven_os_queryHeapStatus(&hs);
    i51AdeOsLog ( i51_LOG_SC , "PORT : pkig_i51_start,memory: maxFreeblock=%d,totalFree=%d", hs.maxFreeBlock,hs.freeSize) ;
    
    switch(i51KernelGetInitStatus())
    {
    case i51_KERNEL_INIT_SUCCEED:
#if defined( __CM_CHANGUI_UI__)||defined(__I51_DYNAMICS_UI__)
        if( ! ven_std_wstrcmp(p_i51start_data,L"") )
        {
    #ifndef __I51_APP_LANG_EN__
    #ifdef __I51_TINY__
	    Tinyi51PortRunSapGame();
    #else	
            i51PortRunSapGame();
    #endif	
    #else
            i51PortRunSapGameEx(); //外文版直接加载
    #endif
        }
        else
        {
            i51PortRunDynamicAPP(p_i51start_data); //百变ui加载
        }
#else
    #ifndef __I51_APP_LANG_EN__
    #ifdef __I51_TINY__
	    Tinyi51PortRunSapGame();
    #else	
            i51PortRunSapGame();
    #endif	
    #else
        i51PortRunSapGameEx(); //外文版直接加载
    #endif
#endif
    	break;
    case i51_KERNEL_INIT_GOING:
        i51AdeOsLog(0,"PORT : Kernel正在初始化");
        i51PortRunHintScreen(0);
        break;
    case i51_KERNEL_INIT_FAILED://	尚未初始化，或初始化失败
        i51AdeOsLog(0,"PORT : Kernel初始化失败");
        if(gi51PowerOnDelay == 1)
        {
            i51KernelInit ( i51KernelInitCB );
        }
        i51PortRunHintScreen(0);
        break;
    default:
        break;
    }

	tVenRSp.Handle = g_i51PkigHandle;
	tVenRSp.retData = VEN_STC_SUCCESS;
	ven_stc_startRsp(&tVenRSp);

}
static void pkig_i51_stop(void)
{
	ven_rsp_data_t tVenRSp;	

	i51AdeOsLog ( i51_LOG_SC , "PORT : EXIT S" ) ; 
	
	g_i51ExitFlag = 1;
	if(i51PortHintScreenActive)
	{
		i51PortHintScreenActive = 0;
		i51PortHintScreenExit();
		tVenRSp.Handle = g_i51PkigHandle;
    	tVenRSp.retData = VEN_STC_SUCCESS;
    	ven_stc_stopRsp(&tVenRSp);
	}
	else
	{
		i51KernelExit( i51_KERNEL_EXIT_DAP);
	}

	i51AdeOsLog ( i51_LOG_SC , "PORT : EXIT E" ) ; 
	
}
static void pkig_i51_suspend(void)
{
	ven_rsp_data_t tVenRSp;
	ven_req_data_t tVenReq = {0};

    ven_os_heap_status_t memstatus = { 0 } ;

	if(!gi51IsRunning)
	{
		return;
	}
    
    ven_os_queryHeapStatus ( &memstatus ) ;	
	i51AdeOsLog ( i51_LOG_SC , "PORT : PAUSE S , Fm : %d , Mb : %d , Ts : %d" , memstatus.freeSize , memstatus.maxFreeBlock , memstatus.totalSize ) ; 

	if(i51PortHintScreenActive )  
	{
		i51PortHintScreenExit();
	}
	else
	{
		//i51KernelPause();
		i51KernelSendEvent( i51_MSG_PAUSE, NULL);
	}

	//反注册按键   
	ven_ts_unsubscribe(g_i51PkigHandle);
	ven_kpd_unsubscribe(g_i51PkigHandle);
	i51AdeOsScreenAlwaysLight ( 0 );
    //i51ExitDo();
	
	tVenRSp.Handle = g_i51PkigHandle;
	tVenRSp.retData = VEN_STC_SUCCESS;
	ven_stc_suspendRsp(&tVenRSp);

	i51AdeOsLog ( i51_LOG_SC , "PORT : PAUSE E" ) ;

}
static void pkig_i51_resume(void)
{
	ven_rsp_data_t tVenRSp;		
    ven_req_data_t tVenReq = {0};
    
	i51AdeOsLog ( i51_LOG_SC , "PORT : RESUME S" ) ; 

    tVenRSp.Handle = g_i51PkigHandle;
	tVenRSp.retData = VEN_STC_SUCCESS;
	ven_stc_resumeRsp(&tVenRSp);
    //i51EntryDo();
    
	//注册按键                  
	ven_ts_subscribe(g_i51PkigHandle, i51TsHandle );
	ven_kpd_subscribe(g_i51PkigHandle, i51KpdHandle );
	i51AdeOsScreenAlwaysLight ( 1 );
	if(i51PortHintScreenActive )
	{
		i51PortRunHintScreen(0);
	}
	else
	{
		//i51KernelResume();
		i51KernelSendEvent( i51_MSG_RESUME, NULL);
	}

	i51AdeOsLog ( i51_LOG_SC , "PORT : RESUME E" ) ; 
		
}

void ven_i51_init(void)
{
	//	author : osca
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.

	ven_stc_ctrl_func_t tFunc = {0};

	tFunc.pfnStart = pkig_i51_start;
	tFunc.pfnStop = pkig_i51_stop;
	tFunc.pfnSuspend = pkig_i51_suspend;
	tFunc.pfnResume = pkig_i51_resume;

	g_i51PkigHandle = ven_handle_open( E_VEN_I51 );
	ven_stc_regFuncs( g_i51PkigHandle, &tFunc );
	
    gi51PowerOnDelay = 0;  
	{
		iU16 timerid;
		timerid = ven_os_createTimer(i51bootTimerCB);
		ven_os_startTimer(timerid,0,g_i51PkigHandle,15000);
	}
}

iBOOL i51PortCreateFrontEnd ()
{

	//	author : Jelo 
	//	since : 2011.10.19
	// 	(C) PKIG Tech. Co., Ltd.

    ven_os_heap_status_t memstatus = { 0 } ;
    ven_os_queryHeapStatus ( &memstatus ) ;	

	i51AdeOsLog ( i51_LOG_SC , "PORT : Create Front-End S , Fm : %d , Mb : %d , Ts : %d" , memstatus.freeSize , memstatus.maxFreeBlock , memstatus.totalSize ) ;

#ifdef  i51PROFILINGS
    i51AdeOsLog ( i51_LOG_SC , "PORT : Create Front-End total openfile=%d", i51AdeOsFileTotallOpened()) ; 
#endif

	i51EntryDo();
	g_i51ExitFlag = 0;
	gi51IsRunning = 1;

	i51AdeOsLog ( i51_LOG_SC , "PORT : Create Front-End E" ) ; 
	
	return iTRUE ;

}  

iBOOL i51PortReleaseFrontEnd ()
{

	//	author : Jelo 
	//	since : 2011.10.19
	// 	(C) PKIG Tech. Co., Ltd.
    ven_rsp_data_t tVenRSp;	

    ven_os_heap_status_t memstatus = { 0 } ;
    ven_os_queryHeapStatus ( &memstatus ) ;	
	
	i51AdeOsLog ( i51_LOG_SC , "PORT : Release Front-End S , Fm : %d , Mb : %d , Ts : %d" , memstatus.freeSize , memstatus.maxFreeBlock , memstatus.totalSize ) ; 
	 
	i51ExitDo();
	
	if(!g_i51ExitFlag)
	{
		i51AdeOsLog ( i51_LOG_SC , "PORT : Release by App" ) ; 
		ven_stc_notifyStop(g_i51PkigHandle);	
	}
	else
	{
        tVenRSp.Handle = g_i51PkigHandle;
    	tVenRSp.retData = VEN_STC_SUCCESS;
    	ven_stc_stopRsp(&tVenRSp);
	}

#ifdef  i51PROFILINGS
    i51AdeOsLog ( i51_LOG_SC , "PORT : Release Front-End total openfile=%d", i51AdeOsFileTotallOpened()) ; 
#endif
    
	i51AdeOsLog ( i51_LOG_SC , "PORT : Release Front-End E" ) ; 

    gi51IsRunning = 0;
	return iTRUE ;

}


///////////////////////////////////////////////////////////////
int i51GetGameIconPath(u16 *name, u16* pSysimagePath)
{
    unsigned int hFileIconRe = 0;
    unsigned char* pImageData = NULL;
    unsigned int hFileImage = 0;
    u16 iconRePath[128] = {0};
    u16 imagePath[128] = {0};
	u16 sysimagePath[128] = {0};
    iU32 nImageSize = 0;
    iU32 nIconPos = 0;

    memset (iconRePath, 0 , 128 * sizeof(u16)) ;
    memset (imagePath, 0 , 128 * sizeof(u16)) ;   
    i51AdeStdWStrcat(iconRePath, name);
    i51AdeStdWStrcat(iconRePath, L"\\icon.re");
    
    i51AdeStdWStrcat(imagePath, name);
    i51AdeStdWStrcat(imagePath, L"\\");
    i51AdeStdWStrcat(imagePath, name);
    i51AdeStdWStrcat(imagePath, L".png");

    //icon.re是否存在
    if( !pSysimagePath || iFALSE == i51AdeOsFileIsExist(iconRePath))
    {
        return 0;
    }

    do{
        //判断xxx.png是否存在
        if(iFALSE == i51AdeOsFileIsExist(imagePath))
        {
            //打开icon.re  
            hFileIconRe = i51AdeOsFileOpen(iconRePath, i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__, __LINE__);
            if(0 != hFileIconRe)
            {
                nImageSize = i51KernelResGetContentSize ( hFileIconRe, 1);
                nIconPos = i51KernelResGetContentEntry (hFileIconRe, 1); 
                i51AdeOsFileSetSeek(hFileIconRe, nIconPos, i51_OS_FL_SEEK_HEAD);

                pImageData = (unsigned char*)i51AdeOsMalloc(nImageSize,  __FILE__ , __LINE__);
                if(NULL != pImageData && nImageSize)
                {
                    i51AdeOsFileRead(hFileIconRe, pImageData, nImageSize, 0); 
                    i51AdeOsFileClose(hFileIconRe); 
                    hFileIconRe = 0;
                }
                else
                {
                    break;
                }

                //创建png图片
                hFileImage = i51AdeOsFileOpen(imagePath, i51_OS_FL_ALWAYS_OPEN|i51_OS_FL_WRITE, __FILE__, __LINE__);
                if(0 != hFileImage)
                {
                    i51AdeOsFileWrite(hFileImage, pImageData, nImageSize, 0);
                    i51AdeOsFileClose(hFileImage); 
                }
            }

        }
    }while(0);
    
    if(0 != hFileIconRe){
        i51AdeOsFileClose(hFileIconRe); 
        hFileIconRe = 0;
    }
    
    if(NULL != pImageData){
        i51AdeOsFree(pImageData);
        pImageData = NULL;
    }
    
    i51AdeStdMemset16(sysimagePath, 0, sizeof(sysimagePath));
    if(i51AdeOsMMemCardExist())//T卡存在怎使用T卡
    {
        i51AdeStdWStrcat(sysimagePath, L"fs:/CARD/i51/i51KIT/");
    }
    else//没有T卡则使用系统盘
    {
        i51AdeStdWStrcat(sysimagePath, L"fs:/PHON/i51/i51KIT/");
    }

    i51AdeStdWStrcat(sysimagePath, name);
    i51AdeStdWStrcat(sysimagePath, L"/");
    i51AdeStdWStrcat(sysimagePath, name);
    i51AdeStdWStrcat(sysimagePath, L".png");   
    i51AdeStdMemcpy16(pSysimagePath, sysimagePath, sizeof(sysimagePath));

    return 1;
}

typedef struct{

    u16 name[32];
    u16 nParam;
    //u16 nString[STR_LENTH];//app name string
    //u16 nImageUrl[IMG_PATH_LENTH];//app image path
    
}i51AppInfo;

#define MAX_GAMEICON  50
i51AppInfo  gi51AppList[MAX_GAMEICON];//最大支持动态图标 50
int gi51appindex = 0;   //应用下标
int gi51appnum = 0;     //外置应用个数

iU8 i51GetApp(void *arg, iU16* GetName)
{

    if(0 != ven_std_wstrcmp(GetName,L"Portal"))
    {
        if(arg != NULL) //得到应用名称
        {
            if(GetName != NULL&&gi51appindex<MAX_GAMEICON)
            {
                ven_std_wstrcpy(gi51AppList[gi51appindex].name,GetName);
                gi51AppList[gi51appindex].nParam = gi51appindex;
                gi51appindex ++;
            }
        }
        else //得到应用个数
        {
            gi51appnum ++;  
        }
    }
    
    return 1;
}


u32 i51PortGetApplicationList(void)
{
    int appnum=0,i;
    
    gi51appindex = 0;
 
    i51KernelGetApplicationList ( i51GetApp, gi51AppList );
    i51AdeOsLog ( i51_LOG_SC , "PORT : i51PortGetApplicationList: gi51appindex = %d",gi51appindex ) ;
    return gi51appindex;
}

u32 i51PortGetApplicationNum(void)
{
    gi51appnum = 0;
    
    i51KernelGetApplicationList ( i51GetApp, NULL );

    i51AdeOsLog ( i51_LOG_SC , "PORT : i51PortGetApplicationNum gi51appnum = %d",gi51appnum ) ; 
    return gi51appnum;
}

static void _LaunchVendorAppRspCB(void *pUserData, u32 nRspCode)
{
    FREEIF(pUserData);
}
bool i511PortStartApp(u16 nClsId,u32 nParam)
{

    u16 Url[128];
    int nLen = 128;
    CmnAppLaunchRspCbInfo_t tRspCb = {0};
    u32 *pSxmParam = MALLOC(sizeof(u32));
    #define SWPRINTF        swprintf
    
    i51AdeOsLog ( i51_LOG_SC , "PORT : i511PortStartApp nClsId = %d,nParam=%d",nClsId ,nParam) ;
    if (NULL != pSxmParam)
    {
        extern void i51Port_start_Data_Init(unsigned short * pData);
        *pSxmParam = nParam;
        
        i51Port_start_Data_Init(gi51AppList[nParam].name);
        SWPRINTF(Url, nLen, L"Launch?APM_Mode=%d&ClsId=%d&VenData=%d&VenDataLen=%d&TransInId=%d&TransOutId=%d", 0, nClsId, (u32)pSxmParam, sizeof(u32), 0, 0);
        tRspCb.pvUserData = (void *)pSxmParam;
        tRspCb.pfnLaunchListenerCb = _LaunchVendorAppRspCB;
        if (0 != LaunchApp(MAE_CLSID_2_SCHEMEID(nClsId), Url, &tRspCb))
        {
            FREEIF(pSxmParam);
            return FALSE;
        }
    }  
    return TRUE;
}

u32 i511PortUninstallApp(u16 nClsId, u32 nParam)
{
    u32 index=0;
    u16 path[128];

    i51AdeOsLog ( i51_LOG_SC , "PORT : i511PortUninstallApp nParam = %d",nParam ) ; 
    
    index = gi51AppList[nParam].nParam;
    
    memset (path , 0 , sizeof(path)) ;
	i51AdeStdWStrcpy (path ,gi51AppList[index].name) ;
	i51AdeStdWStrcat (path ,L"\\") ;
	i51AdeStdWStrcat (path ,gi51AppList[index].name) ;
	i51AdeStdWStrcat (path ,L".i51") ;

    return i51AdeOsFileDelete(path);

}
int i51PortGetAppParam(u16 index)
{
    return gi51AppList[index].nParam;
}
u16 *i51PortGetAppName(u16 index)
{
    return gi51AppList[index].name;
}



#ifdef __CUSTOMER_CHENYU__

 
typedef struct
{
 iU16 wIcon[FILEMGR_MAX_FILEPATH_SIZE];//游戏的图标路径，待机的入口需要一个90*90的PNG图标，这个需要跟随你们的应用下载到T卡中。
 iU16 wText[IDLEAPP_MAX_USER_APP_NAME_LEN + 1];//下载应用的名字，用于待机的显示
 char szVendor[IDLEAPP_MAX_USER_APP_VENDOR_NAME_LEN];//厂商标示符，目前无用
 iU16 nClsId;//APP ID用于区别不同的应用并调用它
 iU32 nParam;//APP参数，整数类型参数
 iU8  szParam[IDLEAPP_MAX_USER_APP_TEXT_PARAM_LEN + 1];//APP参数，字符串类型参数

} IdleAppUserApp_t; 
extern bool _IdleAppCommonDeleteApp(void *pApplet, s32 nIdx, IdleAppUserApp_t *ptUserApp);
extern bool _IdleAppCommonAddApp(void *pApplet, IdleAppUserApp_t *ptUserApp);
#endif/* __CUSTOMER_CHENYU__*/

#ifdef __CUSTOMER_YJQ__

typedef enum
{
    REQ_WISE_MAINMENU_SAVE_RAI,
    REQ_WISE_MAINMENU_DELETE_RAI,
    MSG_EVENT_MAX
} app_msg_event_e;

typedef struct 
{
	u8 app_name_ch[32];//应用的中文名  必须要的
	u8 app_name_en[32];//应用的英文名  必须要的
	u8 image_path[128];//应用的图片路径  必须要的
	u16 provider;//应用的厂商标识   必须要的
	u32 appid;//应用的唯一标识  必须要的
	char nParam2[24];//此参数为预留的
	u32 total_size;//应用的总大小
	u32 cur_size; //当前已经下载的大小
	char start_data[128];//启动应用的参数
	char filepath[128];  //  应用的保存路径
}APPINFO_T;

typedef struct
{
    u32     type;                 /* Event type          */
    u32     DataLen;             /* Event Data lenght   */
    void*   pData;               /* Event Data pointer  */
} app_msg_userdata_t;

#include "mdl_vm_rai.h"
#include "mmi_common_rai_id.h"

#endif


extern iU16 *i51S_AdeFileFixPath(const iU16 *oldpath);

iBOOL i51PortMain ( iS32 aid , iU32 message , void* parameter )
{
    iAPPINFO *info = (iAPPINFO *)parameter;
    //int addd = CLSID_I51BASE;
    char temp[256]={0};

    switch(message)
    {

        case i51_REQ_INSTALLED:
            i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortMain install indentifer=%s",i51AdeStdUnicode2Ascii(info->identifer,temp));
            i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortMain install icon=%s",i51AdeStdUnicode2Ascii(info->icon,temp));
            i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortMain install fullpath=%s",i51AdeStdUnicode2Ascii(i51S_AdeFileFixPath(info->icon),temp));    
            i51AdeOsLog ( i51_LOG_SC , "PORT : i51PortMain install name=%s",info->name);
        
#ifdef __CUSTOMER_CHENYU__
            {
                IdleAppUserApp_t Appinfo={0};

                ven_std_wstrcpy(Appinfo.wIcon,i51S_AdeFileFixPath(info->icon)); //图标
                ven_std_GBToUCS2(info->name, sizeof(info->name), Appinfo.wText, sizeof(Appinfo.wText));//字符串
                i51AdeStdUnicode2Ascii(info->identifer,Appinfo.szParam);//APP参数，字符串类型参数

                ven_std_strcpy(Appinfo.szVendor,"51PK");//厂商标识
                Appinfo.nParam = 0;//启动参数 ???
                Appinfo.nClsId = 0;//CLSID ???
                
                _IdleAppCommonAddApp(NULL, &Appinfo); 
            }    
#elif defined(__CUSTOMER_YJQ__)
            {
                iS32  ret = 1; 
                APPINFO_T Appinfo={0};
                app_msg_userdata_t * AppData = NULL;

                ven_std_strcpy(Appinfo.app_name_ch,info->name);
                ven_std_strcpy(Appinfo.app_name_en,info->name);
                i51AdeStdUnicode2Ascii(i51S_AdeFileFixPath(info->icon),Appinfo.image_path)
                Appinfo.provider = 2;
                Appinfo.appid = 0;// ???
                ven_std_strcpy(Appinfo.start_data,info->identifer);
               

                //i51S_fs_raiRead (MMI_FS_RAI_ITEM_I51APPMENU, 0, length, buffer);  
                //ret = i51S_fs_raiWrite(MMI_FS_RAI_ITEM_I51APPMENU, length, buffer);
                
                AppData = ven_os_mem_malloc(sizeof(app_msg_userdata_t));	
                AppData->type = REQ_WISE_MAINMENU_SAVE_RAI;
                AppData->pData = (void*)&Appinfo;
                AppData->DataLen = sizeof(APPINFO_T);
                ret = ven_os_sendMessage(g_i51PkigHandle,SYS_HANDLE,(void *) AppData);
                if(ret == -1)
                {
                    ven_os_mem_free(AppData);
                }
            }
#elif defined(__CUSTOMER_CHENXIANG__)

#endif
            break;
        case i51_REQ_UNINSTALLED:
            i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortMain uninstall indentifer=%s",i51AdeStdUnicode2Ascii(info->identifer,temp));
            i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortMain install icon=%s",i51AdeStdUnicode2Ascii(info->icon,temp));
            i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortMain install fullpath=%s",i51AdeStdUnicode2Ascii(i51S_AdeFileFixPath(info->icon),temp)); 
#ifdef __CUSTOMER_CHENYU__
            {
                IdleAppUserApp_t Appinfo={0};

                ven_std_wstrcpy(Appinfo.wIcon,i51S_AdeFileFixPath(info->icon)); //图标
                ven_std_GBToUCS2(info->name, sizeof(info->name), Appinfo.wText, sizeof(Appinfo.wText));//字符串
                i51AdeStdUnicode2Ascii(info->identifer,Appinfo.szParam);//APP参数，字符串类型参数

                ven_std_strcpy(Appinfo.szVendor,"51PK");//厂商标识
                Appinfo.nParam = 0;//启动参数 ???
                Appinfo.nClsId = 0;//CLSID ???
                
                _IdleAppCommonDeleteApp(NULL,-1, &Appinfo); 
            }    
#elif defined(__CUSTOMER_YJQ__)
            {
                iS32  ret = 1; 
                APPINFO_T Appinfo={0};
                app_msg_userdata_t * AppData = NULL;

                ven_std_strcpy(Appinfo.app_name_ch,info->name);
                ven_std_strcpy(Appinfo.app_name_en,info->name);
                i51AdeStdUnicode2Ascii(i51S_AdeFileFixPath(info->icon),Appinfo.image_path)
                Appinfo.provider = 2;
                Appinfo.appid = 0;// ???
                ven_std_strcpy(Appinfo.start_data,info->identifer);
               

                //i51S_fs_raiRead (MMI_FS_RAI_ITEM_I51APPMENU, 0, length, buffer);  
                //ret = i51S_fs_raiWrite(MMI_FS_RAI_ITEM_I51APPMENU, length, buffer);
                
                AppData = ven_os_mem_malloc(sizeof(app_msg_userdata_t));	
                AppData->type = REQ_WISE_MAINMENU_DELETE_RAI;
                AppData->pData = (void*)&Appinfo;
                AppData->DataLen = sizeof(APPINFO_T);
                ret = ven_os_sendMessage(g_i51PkigHandle,SYS_HANDLE,(void *) AppData);
                if(ret == -1)
                {
                    ven_os_mem_free(AppData);
                }
            }
#elif defined(__CUSTOMER_CHENXIANG__)

#endif
            break;    

    }


}

void i51PortGetStationInfoAsyncCb(void *pApplet, MMSRV_CellInfoCnf_t *pData, MAE_DualMode_t eDualMode)
{
	g_i51CellInfoZid = pData->Serving.Lac[1] + pData->Serving.Lac[0]*256;
	g_i51CellInfoBsid = pData->Serving.CellId[1] + pData->Serving.CellId[0]*256;

	i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortGetStationInfoAsyncCb. g_i51CellInfoZid = %d, g_i51CellInfoBsid = %d", 
		g_i51CellInfoZid, g_i51CellInfoBsid);
}

void i51PortGetStationInfoAsync(void *pUserData)
{
	u8 eSIMStatus_Master = COMMON_SIM_STATUS_NOSIM;
	u8 eSIMStatus_Slave = COMMON_SIM_STATUS_NOSIM;
	MMSRV_CellInfoReq_t Req = {0};
	MAE_DualMode_t eMode;
	u32 ret = 0;

	i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortGetStationInfoAsync S");

	MMI_Common_GetSIMStatus((IBase *)pUserData, &eSIMStatus_Master, &eSIMStatus_Slave);
		
	if(eSIMStatus_Master == COMMON_SIM_STATUS_ACTIVE)
	{
		i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortGetStationInfoAsync eSIMStatus_Master = %d", eSIMStatus_Master);
		eMode = DUAL_MASTER;
	}
	else if(eSIMStatus_Slave == COMMON_SIM_STATUS_ACTIVE)
	{
		i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortGetStationInfoAsync eSIMStatus_Slave = %d", eSIMStatus_Slave);
		eMode = DUAL_SLAVE;
	}
	else
	{
		i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortGetStationInfoAsync sim error");
		return;
	}

	Req.mode = MMI_MMSRV_CELL_ONE_SHOT;
	Req.reqDump = MMI_MMSRV_CELL_DUMP_MAIN_NEI_ALL;
	
	ret = SrvMobilityMgrCellInfoReq(&Req, eMode, i51PortGetStationInfoAsyncCb);

	i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortGetStationInfoAsync E. ret = %d", ret);

}

void i51PortGetStationTimerCb(void *pUserData)
{
	if(g_i51CellInfoTimerCount >= I51PORT_GETSTATION_TIMER_LOOP_COUNT)
	{
		g_i51CellInfoTimerCount = 0;
		i51PortGetStationInfoAsync(pUserData);
	}
	SH_CreateTimer(I51PORT_GETSTATION_TIMER_INTERVAL, i51PortGetStationTimerCb, (void *)pUserData);
}

void i51PortGetStationStart(void *pUserData)		//需要在进入Idle的Start和Resume事件处理中调用，参数为Applet指针
{
	i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortGetStationStart S");
	if(SH_IsTimerActive(i51PortGetStationTimerCb, (void *)pUserData))
	{
		SH_CancelTimer(i51PortGetStationTimerCb, (void *)pUserData);
	}

	i51PortGetStationInfoAsync(pUserData);
	
	g_i51CellInfoTimerCount = 0;
	SH_CreateTimer(I51PORT_GETSTATION_TIMER_INTERVAL, i51PortGetStationTimerCb, (void *)pUserData);
}

void i51PortGetStationStop(void *pUserData)			//需要在进入Idle的Stop和Suspend事件处理中调用，参数为Applet指针
{
	i51AdeOsLog ( i51_LOG_SC ,"PORT :i51PortGetStationStop S");
	if(SH_IsTimerActive(i51PortGetStationTimerCb, (void *)pUserData))
	{
		SH_CancelTimer(i51PortGetStationTimerCb, (void *)pUserData);
		g_i51CellInfoTimerCount = 0;
	}
}

static void _i51PortInitLogFile()
{
	iU16 traceFileName[I51_LOG_FILENAME_SIZE];
	iU16 logDatabaseFileName[I51_LOG_FILENAME_SIZE];
	iU16 root[I51_LOG_FILENAME_SIZE];
	iU16 dataBaseName[64];
	iU32 bufLen = I51_LOG_FILENAME_SIZE * sizeof(iU16);
	
	ven_std_memset(g_i51LogFileName, 0, bufLen);
	ven_std_memset(traceFileName, 0, bufLen);
	ven_std_memset(logDatabaseFileName, 0, bufLen);
	ven_std_memset(root, 0, bufLen);

	if(i51AdeOsMMemCardExist())
	{
		i51AdeStdWStrcpy(root, VEN_FILE_VOLUME_NAME_SD);
	}
	else
	{
#ifndef __NAND__
		i51AdeStdWStrcpy(root, VEN_FILE_VOLUME_NAME_NOR);
#else
		i51AdeStdWStrcpy(root, VEN_FILE_VOLUME_NAME_NAND);
#endif   
	}
	i51AdeStdWStrcat(root, I51_ROOT_DIR);
	
	i51AdeStdWStrcpy(g_i51LogFileName, root);
	i51AdeStdWStrcpy(traceFileName, root);
	i51AdeStdWStrcpy(logDatabaseFileName, root);

	i51AdeStdWStrcat(g_i51LogFileName, I51_LOG_FILENAME);
	i51AdeStdWStrcat(traceFileName, I51_TRACE_FILENAME);

	ven_std_memset(dataBaseName, 0, sizeof(iU16) * 64);
	SWPRINTF(dataBaseName, 64, L"%d_%d.dat", I51_KERNEL_P4_VERSION, I51_ADAPTER_P4_VERSION);
	i51AdeStdWStrcat(logDatabaseFileName, dataBaseName);

	g_bLogFileExist = ven_file_wexists(g_i51LogFileName);
	g_bTraceFileExist = ven_file_wexists(traceFileName);

	if(g_bLogFileExist || g_bTraceFileExist)
	{
		if(i51AdePortSysLogLoadData((iU16*)&logDatabaseFileName[0]) == iFALSE)
		{
			i51AdeOsLog(0, "缺少日志数据库无, 法打印底层日志. 请下载版本号为 %d_%d 的日志数据库", I51_KERNEL_P4_VERSION, I51_ADAPTER_P4_VERSION);
		}
	}
	
}

iBOOL i51AdePortSysLogLoadData(iU16* pLogDataFileName)
{
	ven_file_handle_t fhLogFile = 0;
	iBOOL ret = iFALSE;
	iS32 nLogSize = 0;
	iU32 nReadSize = 0;
	iS32 nResult = 0;

	do
	{
		if(!ven_file_wexists(pLogDataFileName))
		{
			break;
		}
		
		if((fhLogFile = ven_file_wfopen ( pLogDataFileName , "r" )) == 0)
		{
			break;	
		}
		
		if(VEN_FILE_ERR_NO_ERROR != ven_file_wfseek(fhLogFile, 0, VEN_FILE_SEEK_SET)
			|| VEN_FILE_ERR_NO_ERROR != ven_file_wfseek(fhLogFile, 0, VEN_FILE_SEEK_END))
		{
			break;
		}
		
		if((nLogSize = ven_file_wftell(fhLogFile)) <= 0)
		{
			break;
		}
		if(nLogSize < I51_LOG_HEAD_SIZE)
		{
			break;
		}
		nLogSize -= I51_LOG_HEAD_SIZE;
		
		if(g_pu8LogDataBaseBuf)
		{
			i51AdeOsFree(g_pu8LogDataBaseBuf);
			g_pu8LogDataBaseBuf = 0;
		}
		g_pu8LogDataBaseBuf = (iU8*)i51AdeOsMalloc(nLogSize, __FILE__ , __LINE__ );
		if(!g_pu8LogDataBaseBuf)
		{
			break;	
		}
		

		if(VEN_FILE_ERR_NO_ERROR != ven_file_wfseek(fhLogFile, I51_LOG_HEAD_SIZE, VEN_FILE_SEEK_SET))
		{
			break;	
		}
		
		if(nLogSize != ven_file_wfread ((void*)g_pu8LogDataBaseBuf , nLogSize, fhLogFile) )
		{
			break;	
		}

		ret = iTRUE;
	}while(0);

	if(fhLogFile)
	{
		ven_file_wfclose(fhLogFile);
		fhLogFile = NULL;
	}
	if(!ret)
	{
		i51AdeOsFree(g_pu8LogDataBaseBuf);
		g_pu8LogDataBaseBuf = 0;
	}
	return ret;
	
}

iU8* i51AdePortSysLogGetString(iU16 logIndex)
{
	iU16 nOffset = 0;

	if(g_pu8LogDataBaseBuf == 0)
	{
		return 0;
	}

	nOffset = g_pu8LogDataBaseBuf[I51_LOG_IDXTBL_SIZE  * logIndex] | g_pu8LogDataBaseBuf[I51_LOG_IDXTBL_SIZE  * logIndex + 1] << 8;
	return g_pu8LogDataBaseBuf + nOffset;
	
}
