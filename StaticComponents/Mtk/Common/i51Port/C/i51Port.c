
/*

	The Implementing of 'i51Port' , Module : 'PORT'.	
	Layer belong : 'Static Components'.
	For more details about 'i51Port' you could read : "The_i51_SC_Port_Interface.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/


#include "i51Scripts.h"
#include "i51.h"
#include "i51AdeCnf.h"
#include "i51AdeOs.h"
#include "i51AdeMmi.h"
#include "i51AdeSms.h"
#include "i51AdeSoc.h"
#include "i51AdeStd.h"
#include "i51Kernel.h"
#include "i51Port.h"
#include "mmi_include.h"
#if( I51_MTK_VS >=VS_09A )
#include "GeneralDeviceGprot.h"
#include "mmi_frm_events_gprot.h"
#elif( I51_MTK_VS ==VS_08B )
#include "gpioInc.h"
#endif
#include "CommonScreensResDef.h"
#include "CommonScreens.h"

#if( I51_MTK_VS >=VS_W10 )
#include "mmi_rp_app_i51_def.h"
//#if( I51_MTK_VS >=VS_11A_1112 )
#if defined(__COSMOS_MMI_PACKAGE__)||defined(__I51_1224_MENUSTYLE__)
#ifdef __I51_Portal__
#include "mmi_rp_app_i51_portal_def.h"
#endif
#endif
#include "UcmSrvGprot.h"
#endif

#if( I51_MTK_VS >= VS_11B_1212 )
#include "mmi_frm_scenario_gprot.h"
#include "mmi_frm_history_gprot.h"
#include "gs_srv_vfx_framework.h"
#endif

#include "TimerEvents.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"

#define SP_EXIT_PROMPT  1
static iBOOL l_BootWaited = iFALSE;
static iBOOL l_InI51 = iFALSE; //是否存在i51屏幕,用于确定进入15屏幕是开始进入还是恢复性进入
static void (*l_EntyMenuFnPtr)(void);
int i51SapAppBootPara[2] = { 20*60, 60 };

#if( I51_MTK_VS >= VS_11B_1212 )
iU16 I51_GROUP_ID = APP_I51_BASE;
iU16 I51_SCREEN_ID = APP_I51_BASE+3;
#else
iU16 I51_SCREEN_ID = APP_I51_BASE;
#endif

#define I51_KERNEL_P4_VERSION 	1014
#define I51_ADAPTER_P4_VERSION 	1023
#define I51_LOX_INDEX_SIZE 	2
iU8* g_pu8LogDataBaseBuf ;
iBOOL i51LogFileisExist ;

//***********input******
typedef struct 
{
	U16 i51_value;  
	U16 mtk_value;  
} I51_KEY_MATCH;

static I51_KEY_MATCH I51keyMap[] =
{
    {i51_KEY_NUM0, KEY_0},
    {i51_KEY_NUM1, KEY_1},
    {i51_KEY_NUM2, KEY_2},
    {i51_KEY_NUM3, KEY_3},
    {i51_KEY_NUM4, KEY_4},
    {i51_KEY_NUM5, KEY_5},
    {i51_KEY_NUM6, KEY_6},
    {i51_KEY_NUM7, KEY_7},
    {i51_KEY_NUM8, KEY_8},
    {i51_KEY_NUM9, KEY_9},
    {i51_KEY_LEFT , KEY_LSK},
    {i51_KEY_RIGHT , KEY_RSK},
    {i51_KEY_ARROW_UP , KEY_UP_ARROW},
    {i51_KEY_ARROW_DOWN , KEY_DOWN_ARROW},
    {i51_KEY_ARROW_LEFT , KEY_LEFT_ARROW},
    {i51_KEY_ARROW_RIGHT , KEY_RIGHT_ARROW},
    {i51_KEY_CALL , KEY_SEND},
    {i51_KEY_END_CALL , KEY_END},
    {i51_KEY_STAR , KEY_STAR},
    {i51_KEY_SHARP , KEY_POUND},
    {i51_KEY_VOL_UP , KEY_VOL_UP},
    {i51_KEY_VOL_DOWN , KEY_VOL_DOWN},
    {i51_KEY_ENTER , KEY_ENTER} 
};

//正在初始化，请稍候...
#ifdef __I51_APP_LANG_EN__
U16 l_WaitPrompt[]= L"Initializing, Please wait";   
#else
U16 l_WaitPrompt[12+1] = { 0x6b63,0x5728,0x521d,0x59cb,0x5316,0xff0c,0x8bf7,0x7a0d,0x5019,0x002e,0x002e,0x002e,0x0000
};
#endif
static void i51EntryScreen(void);
static void i51ExitSreen(void);
static void i51CheckPlatQuit(void);
static void i51SetInputNotify(void);

int  i51EntryDo(void);
void i51ExitDo(void);
void I51_menu_entry(void);
iBOOL i51PortCreateFrontEnd ();
iBOOL i51PortReleaseFrontEnd ();
iBOOL i51PortFrontEndCreated(void);

int i51EntryDo(void)
{
	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
    #if defined(__MMI_BACKGROUND_CALL__)
    if(srv_ucm_is_background_call())
    //if( srv_ucm_query_call_count(SRV_UCM_ACTIVE_STATE, SRV_UCM_CALL_TYPE_ALL, NULL) != 0 )
    {
        //需要先停止打电话！
        #ifdef __I51_APP_LANG_EN__
        U16 s2[]= L"Please hang up the phone!";
        #else
        U16 s2[9+1] = { 0x9700,0x8981,0x5148,0x505c,0x6b62,0x6253,0x7535,0x8bdd,0xff01,0x0000};
        #endif
        DisplayPopup((PU8)s2, IMG_GLOBAL_WARNING, 0, 3000, 0);
        return 1;
    }
    #endif
    if(mdi_audio_get_state() == MDI_AUDIO_PLAY)
//    if( mdi_audio_is_playing(MDI_AUDIO_PLAY_ANY) )
    {   
        //需要先关闭音乐播放程序！
        #ifdef __I51_APP_LANG_EN__
        U16 s1[]= L"Please Stop the Music !";
        #else
        U16 s1[12+1] = { 0x9700,0x8981,0x5148,0x5173,0x95ed,0x97f3,0x4e50,0x64ad,0x653e,0x7a0b,0x5e8f,0xff01,0x0000};        
        #endif
        DisplayPopup((PU8)s1, IMG_GLOBAL_WARNING, 0, 3000, 0);       
        return 1 ;
    }

    i51AdeOsLog( 0 ,"PORT : i51EntryDo , MEM left:%d",i51AdeOsGetTotallMemoryLeft());
    
    return 0;
}

void i51ExitDo(void)
{
	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.

    i51AdeOsLog( 0 ,"PORT : i51ExitDo , MEM left:%d",i51AdeOsGetTotallMemoryLeft());
	
}


static void i51EntryScreen(void)
{
	//	author : Miki
	//	since : 2011.12.2
	// 	(C) PKIG Tech. Co., Ltd.
    

	#if( I51_MTK_VS >= VS_11B_1212 )
	MMI_ID parent_id;  

   // parent_id=mmi_frm_group_get_active_id();

    parent_id = mmi_frm_group_get_top_parent_group_id(mmi_frm_group_get_active_id());

	mmi_frm_group_create(parent_id, 

                          I51_GROUP_ID, 

                          NULL,

                          NULL);
    mmi_frm_group_enter(I51_GROUP_ID, MMI_FRM_NODE_SMART_CLOSE_FLAG);
	
    if (!mmi_frm_scrn_enter(

            I51_GROUP_ID,//group id

            I51_SCREEN_ID,//screnn id 

            i51ExitSreen,

            (FuncPtr)i51PortCreateFrontEnd,

            MMI_FRM_FULL_SCRN))

    {
        return;
    }
	#else
	EntryNewScreen (I51_SCREEN_ID, i51ExitSreen, (FuncPtr)i51PortCreateFrontEnd, NULL);
	#endif	

    i51AdeOsLog( i51_LOG_SC, "PORT : EntrySreen , S , Front app run = %d", l_InI51 );
    gdi_layer_set_source_key(0,0);
    
    #ifdef __MTK_TARGET__
    #ifdef __MMI_AUDIO_PLAYER__
    mdi_audio_suspend_background_play();       
    #endif 
    #endif 
   
    i51AdeMmiMediaPlayerSetVolume( i51AdeMmiMediaPlayerGetVolume(0) );

    {
        stFontAttribute f={0};
        f.size = MEDIUM_FONT;
        gui_set_font(&f);
    }
#ifdef __MMI_TOUCH_SCREEN__
    mmi_pen_config_sampling_period(MMI_PEN_SAMPLING_PERIOD_2, MMI_PEN_SAMPLING_PERIOD_2);
#endif
}

static void i51ExitSreen (void)
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
    i51AdeOsLog( i51_LOG_SC, "PORT : ExitSreen S, Front app run = %d", l_InI51 );
    if(l_InI51)//comforn no by call after release front end
        i51KernelSendEvent(i51_MSG_PAUSE, 0);

    gui_start_timer( 1, i51CheckPlatQuit );  
    
    #ifdef __MTK_TARGET__
    #ifdef __MMI_AUDIO_PLAYER__
    mdi_audio_resume_background_play();
    #endif
    #endif  

#ifdef __MMI_TOUCH_SCREEN__
    mmi_pen_config_sampling_period(MMI_PEN_SAMPLING_PERIOD_1, MMI_PEN_SAMPLING_PERIOD_2);
#endif

    i51AdeOsLog( i51_LOG_SC, "PORT : ExitSreen , E");
}

static U16 i51GetMatchKey(U16 mtk_value) 
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
	int i = 0;
	U16 ret = 0xffff;
	
	int num = sizeof(I51keyMap) / sizeof(I51_KEY_MATCH);
	for (i = 0; i < num; i++) 
	{
		if (I51keyMap[i].mtk_value== mtk_value) 
		{
			ret = I51keyMap[i].i51_value;
			break;
		}
	}
	
	return ret;
}

static void i51KeyNotify(void) 
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
	unsigned short keyCode = 0xffff ;
	unsigned short keyType = 0xffff ;

	GetkeyInfo(&keyCode, &keyType);
    
    i51AdeOsLog(i51_LOG_SC, "PORT : i51KeyNotify , keyCode = %d , keyType = %d", keyCode, keyType );
    if(keyType== KEY_EVENT_DOWN){
        keyType = i51_MSG_KEYDOWN;
    }else if(keyType== KEY_EVENT_UP){
        keyType = i51_MSG_KEYUP;
    }else if(keyType== KEY_LONG_PRESS){
        //keyType = i51_MSG_KEYLONGPRESS; 
    }else if(keyType== KEY_REPEAT){
        //keyType = i51_MSG_KEYREPEAT ;
    }
	#if(I51_MTK_VS >= VS_11A_1112)
	if( keyCode == KEY_END || keyCode == KEY_HOME ){//挂机键退出    
	#else
	if( keyCode == KEY_END ){//挂机键退出    
	#endif
        if(keyType == i51_MSG_KEYDOWN){                   
            i51KernelExit( i51_KERNEL_EXIT_DAP ); 
        }
        return;
    }

    keyCode = i51GetMatchKey(keyCode);
	
	if(keyCode!=0xffff)
    i51KernelSendEvent(keyType,keyCode);

}

static void i51PenDown(mmi_pen_point_struct point) 
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
    i51KernelSendEvent( i51_MSG_PADDOWN, (point.x<<16)|point.y );
}
 
static void i51PenUp(mmi_pen_point_struct point) 
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
    i51KernelSendEvent( i51_MSG_PADUP, (point.x<<16)|point.y );
}

static void i51PenMove(mmi_pen_point_struct point) 
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
    i51KernelSendEvent( i51_MSG_PADMOVE, (point.x<<16)|point.y);
}

static void i51SetInputNotify (void)
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
	int i /*num*/;

    /*key*/
	//num = sizeof(I51keyMap) / sizeof(I51_KEY_MATCH);
	for (i = 0; i < MAX_KEYS; i++) 
	{
		SetKeyHandler(i51KeyNotify, i, KEY_EVENT_DOWN);
		SetKeyHandler(i51KeyNotify, i, KEY_EVENT_UP);
	}

    /* pen */
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_register_down_handler(i51PenDown);
	mmi_pen_register_up_handler(i51PenUp);
	mmi_pen_register_move_handler(i51PenMove);
#endif
}

static void i51CheckPlatQuit(void)
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
#if( I51_MTK_VS >= VS_11B_1212 )
	MMI_ID ParentScreen,Currentscreen;

	mmi_frm_get_active_scrn_id(&ParentScreen,&Currentscreen);
    if (Currentscreen != I51_SCREEN_ID )
    {
        if(!mmi_frm_group_is_present(I51_GROUP_ID))//!mmi_frm_scrn_is_present(I51_GROUP_ID,I51_SCREEN_ID,MMI_FRM_NODE_EXCLUDE_ACTIVE_SCRN_FLAG) )//!IsScreenPresent (I51_SCREEN_ID) )//2??úàúê·?á???D 
        {         
			if(l_InI51) 
            {   
                i51AdeOsLog( 0, "PORT : i51CheckPlatQuit , check find NoneScreen's i51");          
                i51KernelExit( i51_KERNEL_EXIT_DAP );        
            }
        }
        else
        {
            gui_start_timer( 100, i51CheckPlatQuit );
        }
    } 
#else	
    if ( GetActiveScreenId() != I51_SCREEN_ID )//不是当前屏幕
    {
        if( !IsScreenPresent (I51_SCREEN_ID) )//不在历史屏幕中
        {           
            if(l_InI51) 
            {   
                i51AdeOsLog( 0, "PORT : i51CheckPlatQuit , check find NoneScreen's i51");          
                i51KernelExit( i51_KERNEL_EXIT_DAP );        
            }
        }
        else
        {
            gui_start_timer( 100, i51CheckPlatQuit );
        }
    }    	
#endif
}

static void i51EntryI51Check(void)
{
    //	author : Miki
	//	since : 2011.12.2
	// 	(C) PKIG Tech. Co., Ltd.
#if( I51_MTK_VS == VS_11B_1224 )
	if( GetActiveScreenId() != SCR_ID_ALERT_0 )
#else
	if( GetActiveScreenId() != SCR_CONFIRM_SCREEN )
#endif
	{
		gui_cancel_timer( i51EntryI51Check );
	}
    else if( i51KernelGetInitStatus() == i51_KERNEL_INIT_SUCCEED ) 
	{   
        gui_cancel_timer(i51EntryI51Check);
        if(l_EntyMenuFnPtr)
		    l_EntyMenuFnPtr(); 
	}
    else  
    {
        if(i51KernelGetInitStatus() == i51_KERNEL_INIT_FAILED && l_BootWaited ) 
            i51KernelInit(0);
  		gui_start_timer( 1000, i51EntryI51Check );  
    }
}

iBOOL i51NeedWaitEntry( void (*EntyMenuFnPtr)(void) )
{
    //	author : Miki
	//	since : 2011.12.2
	// 	(C) PKIG Tech. Co., Ltd.
    if ( i51KernelGetInitStatus() != i51_KERNEL_INIT_SUCCEED )
    {
        l_EntyMenuFnPtr = EntyMenuFnPtr;
        gui_start_timer(1000, i51EntryI51Check);    
        DisplayConfirm(0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (UI_string_type)l_WaitPrompt, IMG_GLOBAL_PROGRESS, 0);
	    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP); 
        return iTRUE;
    }else return iFALSE;
}

#ifdef __I51_SIMPLE_INSTALL__
#if defined(__I51_TINY__)
extern iU32 i51KitStaticCoreAdapter[];
void i51SimpleLunchApp( U16 *pAppName )
{
    typedef struct
	{
		unsigned short *appname;
		int logosize;
		int dispsize;
		char logotype;//0 文字 1图片
		char gprsprompt;
		void *dispname;//游戏名称字模
		void *logo;//游戏说明 图片buffer或文字字模
	}BooterParam;
	static BooterParam param = {0};
	i51KITMAIN i51KitMain = 0;
    
	param.appname = pAppName;
#ifdef __I51_APP_GPRS_PROMPT__
    param.gprsprompt = 1;
#else
    param.gprsprompt = 0;
#endif
    if( i51KernelCheckAppCompleten(pAppName) ){
        i51KernelRunDynamicApp( pAppName, NULL );
    }
    else{
        i51KernelRunStaticApp ( L"Appmenu" , (unsigned char* )i51SapGetAppmenu () , (unsigned int )i51SapGetAppmenuSize () , &param, 0 , 1 ) ;
#if 1
		//akilae add since 2012.7.20	
		i51KitMain = (i51KITMAIN ) i51SlLoaderGetSymbolEx ( L"Appmenu" , "i51KitMain" ) ;
		if(NULL == i51KitMain){
		i51AdeOsLog(0, "PORT : i51SimpleLunchApp Get i51KitMain F");
		return;
		}
		i51KitMain( 0, 0x00FFFFFF , (void*)i51KitStaticCoreAdapter );
#endif		
    }
}
#else
void i51SimpleLunchApp( U16 *pAppName )
{
    typedef struct
	{
		unsigned short *appname;
		int logosize;
		int dispsize;
		char logotype;//0 文字 1图片
		char gprsprompt;
		void *dispname;//游戏名称字模
		void *logo;//游戏说明 图片buffer或文字字模
	}BooterParam;
	static BooterParam param = {0};
	param.appname = pAppName;
#ifdef __I51_APP_GPRS_PROMPT__
    param.gprsprompt = 1;
#else
    param.gprsprompt = 0;
#endif
    if( i51KernelCheckAppCompleten(pAppName) )
        i51KernelRunDynamicApp( pAppName, NULL );
    else
        i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetAppmenu () , (unsigned int )i51SapGetAppmenuSize () , &param, 0 , 1 ) ;
}
#endif
#endif

void i51Debuger(void)
{
	extern iS32 i51KernelRunDynamicAppEx ( iU16* name , void* parameter ) ;

    if( i51NeedWaitEntry(i51Debuger) ) return;

    if( i51EntryDo()== SP_EXIT_PROMPT ) return;

	i51KernelRunDynamicAppEx ( L"i51Debuger" , 0 ) ;
}
void i51Explorer(void)
{
    
	extern iS32 i51KernelRunStaticApp ( iU16* name , iU8* buffer , iU32 buflen , void* parameter , iBOOL staticscope , iBOOL frontapp ) ;
	extern const unsigned char* i51SapGeti51Explorer () ;
	extern const int i51SapGeti51ExplorerSize () ;	
	
    if( i51NeedWaitEntry(i51Explorer) ) return;

    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
	    i51KernelRunStaticApp ( L"i51Explorer" , (unsigned char* )i51SapGeti51Explorer () , (unsigned int )i51SapGeti51ExplorerSize () , 0 , 0 , 1 ) ;	

}

#ifdef __I51_Portal__
void I51_menu_entry(void)
{	

    extern void *i51SapPortalParameter(void);

    if( i51NeedWaitEntry( I51_menu_entry ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"Portal");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapPortalParameter(), 0 , 1 ) ;	
#endif
/*
	extern iS32 i51KernelRunStaticApp ( iU16* name , iU8* buffer , iU32 buflen , void* parameter , iBOOL staticscope , iBOOL frontapp ) ;
	extern const unsigned char* i51SapGeti51Explorer () ;
	extern const int i51SapGeti51ExplorerSize () ;	
	
    if( i51NeedWaitEntry(I51_menu_entry) ) return;

    if( i51EntryDo()== SP_EXIT_PROMPT ) return;

	i51KernelRunStaticApp ( L"i51Explorer" , (unsigned char* )i51SapGeti51Explorer () , (unsigned int )i51SapGeti51ExplorerSize () , 0 , 0 , 1 ) ;	
*/
}


/* 菜单高亮函数 */

void I51_mmi_menu_highlight (void)
{
    SetLeftSoftkeyFunction (I51_menu_entry,KEY_EVENT_UP); 
    SetKeyHandler (I51_menu_entry,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void I51_mmi_menu_init (void)
{
    SetHiliteHandler (MENU_ID_FNG_I51_1, I51_mmi_menu_highlight);
    SetHiliteHandler (MENU_ID_FNG_I51_2, I51_mmi_menu_highlight);
}
#endif

iBOOL i51PortCreateFrontEnd ()
{

	//	author : Jelo 
	//	since : 2011.10.19
	// 	(C) PKIG Tech. Co., Ltd.
    
    i51AdeOsLog(i51_LOG_SC, "PORT : i51PortCreateFrontEnd , S");
    
    i51EntryScreen();

    i51SetInputNotify();

    if(l_InI51){
        i51AdeOsLog(i51_LOG_SC, "PORT : i51PortCreateFrontEnd , resume notify");
        #if defined(__MMI_BACKGROUND_CALL__)
        if(srv_ucm_is_background_call())
        //if(srv_ucm_query_call_count(SRV_UCM_ACTIVE_STATE, SRV_UCM_CALL_TYPE_ALL, NULL) != 0 )
        {
            i51AdeOsLog(i51_LOG_SC, "PORT : i51PortCreateFrontEnd , Have background dial");
            i51KernelExit( i51_KERNEL_EXIT_DAP );
            return iFALSE;
        }
        #endif
        i51KernelSendEvent(i51_MSG_RESUME, 0);
    }else
         i51AdeOsLog(i51_LOG_SC, "PORT : i51PortCreateFrontEnd , fontend app will run");
    
    l_InI51 = iTRUE;

    i51AdeOsLog(i51_LOG_SC, "PORT : i51PortCreateFrontEnd , E");
	return iTRUE ;
	
}

iBOOL i51PortReleaseFrontEnd ()
{

	//	author : Jelo 
	//	since : 2011.10.19
	// 	(C) PKIG Tech. Co., Ltd.
	#if( I51_MTK_VS >= VS_11B_1212 )
		MMI_ID ParentScreen,Currentscreen;
	#endif
	
    i51AdeOsLog(i51_LOG_SC, "PORT : i51PortReleaseFrontEnd , Release Front-End S");
    l_InI51 = iFALSE;	

	#if( I51_MTK_VS >= VS_11B_1212 )
	
		mmi_frm_get_active_scrn_id(&ParentScreen,&Currentscreen);
    if( Currentscreen == I51_SCREEN_ID )
    {
         GoBackHistory();
    }
    if( mmi_frm_group_is_present(I51_GROUP_ID)) 
    {
        mmi_frm_group_close(I51_GROUP_ID); 
    }
	#else
	
    if( GetActiveScreenId() == I51_SCREEN_ID )
        GoBackHistory();
    if( IsScreenPresent (I51_SCREEN_ID) )//在历史屏幕中                             
        DeleteScreenIfPresent (I51_SCREEN_ID); 
	
	#endif
	
    i51ExitDo();
    i51AdeOsLog(i51_LOG_SC, "PORT : i51PortReleaseFrontEnd , Release Front-End E");
    return iTRUE ;
	
}

iBOOL i51PortFrontEndCreated(void)
{
    return l_InI51;
}

void i51PortQuitFrontEndApp(void)
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
    i51KernelExit( i51_KERNEL_EXIT_DAP );    		
}


static char I51_lmainlcd_backlight = 1;

void i51PioLevelReqCatch(gpio_device_enum device, U8 level) 
{
	//	author : Miki
	//	since : 2011.12.2
	// 	(C) PKIG Tech. Co., Ltd.
    if( device==GPIO_DEV_LED_MAINLCD  && l_BootWaited && i51KernelGetInitStatus() == i51_KERNEL_INIT_SUCCEED )
    {
        if(level == LED_LIGHT_LEVEL0)
        {
            if(I51_lmainlcd_backlight == 1)
            { 
              	i51AdeOsLog(1,"PORT : i51PioLevelReqCatch , Entry Idle S");
              	i51KernelSendEvent( i51_MSG_ENTERIDLE, NULL);
              	I51_lmainlcd_backlight = 0;
              	i51AdeOsLog(1,"PORT : i51PioLevelReqCatch , Entry Idle E");              
            }
			
        }
        else
        {
            if(I51_lmainlcd_backlight == 0)
            { 
		        i51AdeOsLog(1,"PORT : i51PioLevelReqCatch , Exit Idle S");
                i51KernelSendEvent( i51_MSG_EXITIDLE, NULL);
                I51_lmainlcd_backlight = 1;
                i51AdeOsLog(1,"PORT : i51PioLevelReqCatch , Exit Idle E");	
            }
        }
    }	
}
void i51PortShutDownNotify(void)
{
    i51AdeOsLog(1,"PORT : i51PortShutDownNotify , S");

    i51KernelExit( i51_KERNEL_EXIT_SAP ); 

    i51AdeOsLog(1,"PORT : i51PortShutDownNotify , E");
}

static void i51RunBootApp( iBOOL results )
{
    extern const unsigned char* i51SapGetAppBoot(void);
    extern const int i51SapGetAppBootSize(void);
    U16 filename[256];

    if(!results) return;

    
    kal_wsprintf( filename, "%c:\\i51CALLTIME", FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE) );
    if (FS_GetAttributes (filename) >= 0)  
        i51SapAppBootPara[0]= 60; 
    else {
        kal_wsprintf( filename, "C:\\i51CALLTIME" );
        if (FS_GetAttributes (filename) >= 0) 
            i51SapAppBootPara[0]= 60; 
    } 

	i51KernelRunStaticApp ( L"AppBoot" , (unsigned char* )i51SapGetAppBoot () , (unsigned int )i51SapGetAppBootSize () , i51SapAppBootPara, 1, 0 ) ;	
}

iBOOL i51LogisExist( void )
{
    return i51LogFileisExist;
}

iBOOL i51AdePortSysLogLoadData(iU16* LDfilename)
{
    iS32 filesize = 0 ;
    iFILE datfl ;
    iU32 hread ;
    
    datfl = i51AdeOsFileOpen(LDfilename , i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ , __FILE__ , __LINE__ );
    i51AdeOsFileGetSize( datfl , &filesize);
    g_pu8LogDataBaseBuf = (iU8*)i51AdeOsMalloc( filesize-2 , __FILE__ , __LINE__ );
    if(!g_pu8LogDataBaseBuf)
    {
        i51AdeOsFileClose(datfl);
        i51AdeOsLog( 0 ,"PORT : i51AdeOsSysLogLoadData , MEM F"); 
        return iFALSE;
    }
            
    i51AdeOsFileSetSeek( datfl , 2 , i51_OS_FL_SEEK_HEAD );
    i51AdeOsFileRead( datfl , (void*)g_pu8LogDataBaseBuf , filesize-2 , &hread);
    i51AdeOsFileClose(datfl);

}

void i51adePortInitLogFile( void )
{
    iU16 logfilename[32]= {0};
    iU16 LDfilename[32]= {0};

    
    i51AdeOsFileAModeEnable();
    
    kal_wsprintf( logfilename , "%c:\\i51\\%s" , 
	   			 FS_GetDrive( FS_DRIVE_V_REMOVABLE , 1 , FS_NO_ALT_DRIVE ) , 
	  			 "i51.log" );

    kal_wsprintf( LDfilename , "%c:\\i51\\%d_%d.dat" , 
	   			 FS_GetDrive( FS_DRIVE_V_REMOVABLE , 1 , FS_NO_ALT_DRIVE ) , 
	  			 I51_KERNEL_P4_VERSION , I51_ADAPTER_P4_VERSION );
    

    if(i51AdeOsFileIsExist(logfilename))
    {
        i51LogFileisExist = iTRUE;
        
        if(i51AdeOsFileIsExist(LDfilename))
        {
            i51AdePortSysLogLoadData(LDfilename);
        }
        else
            i51AdeOsLog(0, "缺少日志数据库无法打印底层日志. 请下载版本号为 %d_%d 的日志数据库", I51_KERNEL_P4_VERSION, I51_ADAPTER_P4_VERSION);
    }
    else
    {//T卡中不存在，判断系统盘是否存在i51.log
        kal_wsprintf( logfilename , "%c:\\i51\\%s" , 
	   			 'C' , 
	  			 "i51.log" );

        kal_wsprintf( LDfilename , "%c:\\i51\\%d_%d.dat" , 
	   			 'C' , 
	  			 I51_KERNEL_P4_VERSION , I51_ADAPTER_P4_VERSION );

        if(i51AdeOsFileIsExist(logfilename))
        {
            i51LogFileisExist = iTRUE;
        
            if(i51AdeOsFileIsExist(LDfilename))
            {
                i51AdePortSysLogLoadData(LDfilename);
            }
            else
                i51AdeOsLog(0, "缺少日志数据库无法打印底层日志. 请下载版本号为 %d_%d 的日志数据库", I51_KERNEL_P4_VERSION, I51_ADAPTER_P4_VERSION);
        }
        
    }
    
    i51AdeOsFileAModeDisable();
}

extern char    *i51AdeImpVerStr;
void i51BootDelay(void)
{ 

    i51adePortInitLogFile();
    
	i51AdeOsLog(1, "PORT : i51BootDelay , Init Kernel; PlatformId=%s ProjectId=%d CustomerId=%d perforce ReVersion: Kel 47 loader 8 dl 5", i51AdeCnfGetPlatformId(), i51AdeCnfGetProjectId (), i51AdeCnfGetCustomerId ());  		

    i51KernelInit(i51RunBootApp);     
    
    l_BootWaited == iTRUE;
}

void i51Boot(void)
{
    //	author : Miki
	//	since : 2011.12.2
	// 	(C) PKIG Tech. Co., Ltd.
    StartTimer( I51_TIMER_ID_END, 18000, i51BootDelay );
}

iBOOL i51PortMain ( iS32 aid , iU32 message , void* parameter ) 
{
	return iTRUE;
}

iBOOL i51CurrScreenIsI51(void)
{
#if( I51_MTK_VS >= VS_11B_1212 )
	MMI_ID ParentScreen,Currentscreen;
	mmi_frm_get_active_scrn_id(&ParentScreen,&Currentscreen);
    return Currentscreen == I51_SCREEN_ID ;
#else	
    return GetActiveScreenId() == I51_SCREEN_ID ;
#endif
}
void  i51NewSmsAfter(void)
{
#ifdef __I51_APP_LANG_EN__
    U16 NewSmsStr[]= L"new sms";
#else
    U16 NewSmsStr[] = { 0x65b0,0x6d88,0x606f,0x0000 };
#endif
    if( l_InI51 && i51CurrScreenIsI51() )
    {
        DisplayPopup( (U8*)NewSmsStr, 0, 0, 3000, 0 );
    }
}
void  i51PortOnNewSms(void)
{
    if(l_InI51){
        i51KernelSendEvent( i51_MSG_PAUSE, 0 );
        gui_start_timer( 20, i51NewSmsAfter );
    }
}

iU8* i51AdePortSysLogGetString(iU16 logIndex)
{
    iU16 offsize ;

    if(iNULL==g_pu8LogDataBaseBuf)
        return iNULL;

    offsize = g_pu8LogDataBaseBuf[logIndex*I51_LOX_INDEX_SIZE]|(g_pu8LogDataBaseBuf[logIndex*I51_LOX_INDEX_SIZE+1]<<8) ;

    return g_pu8LogDataBaseBuf + offsize ;

}