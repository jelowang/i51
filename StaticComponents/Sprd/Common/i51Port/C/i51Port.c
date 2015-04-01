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
#include "i51_define.h"
#include "i51AdeCnf.h"
#include "i51AdeOs.h"
#include "i51AdeSoc.h"
#include "i51AdeMmi.h"
#include "i51AdeStd.h"
#include "i51AdeSms.h"
#include "i51Kernel.h"
#include "i51_text.h"
#include "i51_image.h"
#include "i51_EntryGame.h"


#include "std_header.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "socket_api.h"
#ifdef __I51_TINY__
#include "i51KitCore.h"
#endif

#if defined (SPRD_6600) || defined (SPRD_6620) || defined (SPRD_6800)
#include "mmiset.h"
#endif

#ifdef SPRD_6530
#include "mmi_filemgr.h"
#include "mmimp3_export.h"
#include "mmimp3_music_internal.h"
#endif

#include "mmk_type.h"
#include "mmi_anim.h"
#include "mmipub.h"


#define I51_KERNEL_P4_VERSION  983 
#define I51_ADAPTER_P4_VERSION  984 

#define I51_LOG_IDXTBL_SIZE (2)
iU8*  g_pu8LogDataBaseBuf = NULL; 

void i51AdeOsSysLogLoadData();
typedef struct {

	//	author : Jelo	
	//	since : 2011.4.24
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : i51 菜单状态机

	iU8 window ;
	iU8 scope ;
	
	//	输入法相关
	//	输入法类型
	i51EDITORTYPE edtype ;
	void* edbuffer ;
	iU32 edlen ;
	iU32 inputlen ;
	void (*EditorCallBack) ( void* buffer , iU32 textlen ) ;

	iBOOL painted ;
	
} i51GUI ;

enum {

	//	author : Jelo	
	//	since : 2011.8.23
	//	(C) PKIG Tech. Co., Ltd.

	//	主窗口
	i51_GUI_MAIN_WINDOW ,
	//	输入法窗口
	i51_GUI_EDITOR_WINDOW ,

}  ;

typedef struct 
{

	//	author : Jelo	
	//	since : 2011.12.21
	//	(C) PKIG Tech. Co., Ltd.
	
	iU16 i51_value ;   
	iU16 sprd_value ;
	
} i51KEYMAP ;

iS32 i51PortWindowHandle = 0 ;
iS32 i51EditWindowHandle = 0 ;
static i51GUI i51Gui = { 0 } ;
static iBOOL i51InWin = iFALSE ;
static iBOOL i51focusing = iFALSE ;
static iBOOL l_BootWaited = iFALSE;

//	日志文件路径
static iU16 LogPath[64] = {0} ;
//	日志文件是否存在
static iBOOL LogFileExist = iFALSE ;
//数据文件是否存在
static iU16 DatPath[64] = {0};
static iBOOL DatFileExist = iFALSE;

//	记录down消息
iU32 RecoderDownKey = 0 ;

static MMI_RESULT_E i51GuiEditorWindowMessages (
	
	MMI_WIN_ID_T win_id ,   
	MMI_MESSAGE_ID_E msg_id , 
	DPARAM	param
) ;

MMI_RESULT_E i51GuiWindowMessages ( 
	MMI_WIN_ID_T winid , 
	MMI_MESSAGE_ID_E msgid ,
	DPARAM param ) ;

MMI_RESULT_E i51GuiEditorWindowMessages ( 

	MMI_WIN_ID_T winid , 
	MMI_MESSAGE_ID_E msgid , 
	DPARAM param 

) ;

typedef enum {

	//	author : Jelo 
	//	since : 2011.4.13

	//	notes : 窗口ID
	
	MMI_i51_WIN_ID_BASED = ( MMI_MODULE_i51 << 16 ) ,
	MMI_i51_WIN_ID ,
	MMI_i51_EDITOR_WIN_ID ,
	MMi_i51_WIN_ID_END ,

} MMI_i51_WIN_IDS_DEF ;

typedef enum {

	//	author : Jelo 
	//	since : 2011.5.25

	//	notes : 空间ID

	MMI_i51_CTRL_ID_BASED = ( MMI_MODULE_i51 << 16 ) ,
	//	输入法控件
	MMI_i51_CTRL_ID_EDITOR ,
	MMI_i51_CTRL_ID_END ,
	
} MMI_i51_CTRL_DEF ;



WINDOW_TABLE(MMI_i51_WIN_TAB) = 
{

	//	author : Jelo 
	//	since : 2011.4.13

	//	notes : 平台窗口配置
	
	WIN_ID(MMI_i51_WIN_ID) ,
	WIN_FUNC((iU32)i51GuiWindowMessages) ,
	END_WIN

} ;

WINDOW_TABLE( MMI_i51_TEXTEDIT_WIN_TAB ) = 
{

	//	author : Jelo 
	//	since : 2011.6.10
#if defined (SPRD_6600) || defined (SPRD_6620)
	WIN_ID(MMI_i51_EDITOR_WIN_ID) ,
	WIN_TITLE(TXT_EDIT),
	WIN_FUNC((uint32)i51GuiEditorWindowMessages),    
	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
	END_WIN
#else
//	#ifdef PDA_UI_ANDROID_SOFTKEY
	WIN_STYLE(WS_HAS_SOFTKEY),//如遇报错可以直接注释掉
//	#endif
	WIN_PRIO(WIN_ONE_LEVEL),
	WIN_STYLE (WS_HAS_BUTTON_SOFTKEY) , //如遇报错可以直接注释掉
	WIN_FUNC((uint32)i51GuiEditorWindowMessages),    
	WIN_ID(MMI_i51_EDITOR_WIN_ID),
	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
//	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_TITLE(TXT_EDIT),
	END_WIN
#endif

};

static i51KEYMAP i51KeyMap[] =
{

	//	author : Jelo	
	//	since : 2011.12.21
	//	(C) PKIG Tech. Co., Ltd.
	
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
	{i51_KEY_LEFT , KEY_OK},
	{i51_KEY_RIGHT , KEY_CANCEL},
	{i51_KEY_ARROW_UP , KEY_UP},
	{i51_KEY_ARROW_DOWN , KEY_DOWN},
	{i51_KEY_ARROW_LEFT , KEY_LEFT},
	{i51_KEY_ARROW_RIGHT , KEY_RIGHT},
	{i51_KEY_CALL , KEY_GREEN},
	{i51_KEY_END_CALL , KEY_RED},
	{i51_KEY_STAR , KEY_STAR},
	{i51_KEY_SHARP , KEY_HASH},
	{i51_KEY_VOL_UP , KEY_UPSIDEKEY},
	{i51_KEY_VOL_DOWN , KEY_DOWNSIDEKEY},
	{i51_KEY_ENTER , KEY_ENTER},
	{i51_KEY_ENTER , KEY_WEB},
	#if defined (PDA_UI_SUPPORT_ANDROID) || defined (PDA_UI_SUPPORT)
	{i51_KEY_END_CALL , KEY_HOME},
	{i51_KEY_END_CALL , KEY_MENU},
	{i51_KEY_END_CALL , KEY_POWER},
	#endif
	
};

//	通话时长，和idle
int i51SapAppBootPara[2] = { 20*60, 60 };

//	i51当前声音的大小
static iS32 i51CurVolume = -1 ;

extern iS32 i51KernelRunStaticApp ( iU16* name , iU8* buffer , iU32 buflen , void* parameter , iBOOL staticscope , iBOOL frontapp ) ;
extern const int i51SapGetBootMenu () ;	
extern void MMK_SetCurCtrlHighligAction( BOOLEAN is_ctrl_higlig_oper) ;

static void i51RunBootApp( iBOOL results ) ;
static iU16 i51PortMatchKey ( iU16 sprd_value) 
{

	//	author : Miki
	//	since : 2011.8.30
	// 	(C) PKIG Tech. Co., Ltd.
	
	iS32 i = 0;
	iU16 ret = 0xffff;
	
	iS32 num = sizeof(i51KeyMap) / sizeof(i51KEYMAP) ;
	
	for (i = 0; i < num; i++) {
		if (i51KeyMap[i].sprd_value== sprd_value) {
			ret = i51KeyMap[i].i51_value;
			break;
		}
	}
	
	return ret;
}


/*
void i51GuiKeyEventConverter ( iU32 message )
{

	//	author : Jelo 
	//	since : 2011.4.13

	//	notes : 将Spredtrum 键盘消息转换为i51 消息

	iU32 i51event = 0 ;
	iU32 keyevent = 0 ;

	iU32 keynumber = 0 ;

	keynumber = message & 0xff ;
	message = message & 0xff00 ;
	
	if ( message == KEY_PRESSED )
	{
		i51event = i51_KEYDOWN ;
		keyevent = 1 ;
	}
	else if ( message == KEY_RELEASED )
	{
		i51event = i51_KEYUP ;
		keyevent = 1 ;
	}
	else if ( message == KEY_REPEATED )
	{
		i51event = i51_KEYREPEAT ;
		keyevent = 1 ;
	}
	else if ( message == KEY_LONG_PRESSED )
	{
		i51event = i51_KEYLONGPRESS ;
		keyevent = 1 ;
	}
	
	if ( 0 == keyevent ) return ;
		
	if ( keynumber == KEY_0 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM0 ) ;
	} 
	else if ( keynumber == KEY_1 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM1 ) ;
	} 
	else if ( keynumber == KEY_2 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM2 ) ;
	} 
	else if ( keynumber == KEY_3 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM3 ) ;
	} 
	else if ( keynumber == KEY_4 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM4 ) ;
	} 
	else if ( keynumber == KEY_5 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM5 ) ;
	} 
	else if ( keynumber == KEY_6 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM6 ) ;
	} 
	else if ( keynumber == KEY_7 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM7 ) ;
	} 
	else if ( keynumber == KEY_8 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM8 ) ;
	} 
	else if ( keynumber == KEY_9 )
	{
		i51KernelCallEvent ( i51event , i51_KEY_NUM9 ) ;
	} 		
	else if ( keynumber == KEY_UP )
	{
		i51KernelCallEvent ( i51event , i51_KEY_ARROW_UP ) ;
	}     
	else if ( keynumber == KEY_DOWN )
	{
		i51KernelCallEvent ( i51event , i51_KEY_ARROW_DOWN ) ;
	} 
	else if ( keynumber == KEY_LEFT )
	{
		i51KernelCallEvent ( i51event , i51_KEY_ARROW_LEFT ) ;
	} 
	else if ( keynumber == KEY_RIGHT )
	{
		i51KernelCallEvent ( i51event , i51_KEY_ARROW_RIGHT ) ;
	} 
	else if ( keynumber == KEY_STAR )
	{
		i51KernelCallEvent ( i51event , i51_KEY_STAR ) ;
	} 
	else if ( keynumber == KEY_HASH )
	{
		i51KernelCallEvent ( i51event , i51_KEY_SHARP ) ;
	} 
	else if ( keynumber == KEY_GREEN )
	{
		i51KernelCallEvent ( i51event , i51_KEY_CALL  ) ;
	} 		
	else if ( keynumber == KEY_RED )
	{
		i51KernelCallEvent ( i51event , i51_KEY_END_CALL  ) ;
	} 	
	else if ( keynumber == KEY_OK )
	{
		i51KernelCallEvent ( i51event , i51_KEY_LEFT  ) ;
	} 	
	else if ( keynumber == KEY_CANCEL )
	{
		i51KernelCallEvent ( i51event , i51_KEY_RIGHT ) ;
	} 	
	else if ( keynumber == KEY_WEB )
	{
		i51KernelCallEvent ( i51event , i51_KEY_ENTER ) ;
	} 			
		       
}
*/

 
 extern iU32 i51AdemmiGetHandle() ;


static void i51PortUpVolume ( iU16 keyvalue )
{
	iU32 value = 0 ;
	iU32 handle = 0;
	if ( keyvalue != i51_KEY_VOL_UP && i51CurVolume != -1 )
		return ;
    i51AdeOsLog(0, "PORT : i51PortUpVolume , %d" , i51CurVolume );
	if ( i51CurVolume >= 7 )
		return ;
	handle = i51AdemmiGetHandle();
	 if(handle != 0) {
		i51CurVolume ++ ;
		#ifdef SPRD_6530
			MMISRVAUD_SetVolume(handle, i51CurVolume);
		#else
			AUDIO_SetVolume ((iU32)i51CurVolume) ;
		  #endif  /*SPRD_6530*/
	 }
   
	  
}

static void i51PortDownVolume ( iU16 keyvalue )
{
	iU32 value = 0 ;
	iU32 handle = 0;
	if ( keyvalue != i51_KEY_VOL_DOWN && i51CurVolume != -1 )
		return ;
    i51AdeOsLog(0, "PORT : i51PortDownVolume , %d" , i51CurVolume );
	if ( i51CurVolume <= 0 )
		return ;
	handle = i51AdemmiGetHandle();
	 if(handle != 0) {
	 	
		i51CurVolume -- ;
		#ifdef SPRD_6530	
			MMISRVAUD_SetVolume(handle, i51CurVolume);
		#else
			AUDIO_SetVolume ((iU32)i51CurVolume) ;
		#endif  /*SPRD_6530*/
	 }
  
	  
}

iU32  i51PortGetCurVolumeValue ()
{
	//	获取当前i51的音量值

	
	if ( i51CurVolume == -1 ) {

		i51CurVolume = 5 ;
		return 5 ;
	} else 
		return (iU32)i51CurVolume ;
}
void i51GuiTouchEventConverter ( iU32 message , iU32 x , iU32 y )
{

	//	author : Jelo 
	//	since : 2011.4.13

	//	notes : 将Spredtrum 触屏消息转换为i51 消息 

	//	高十六位存放x，低十六位存放y
	iU32 parameters = (( 0xffff & x ) << 16)|( 0xffff & y ) ;
	
	if ( MSG_TP_PRESS_DOWN == message )
	{
		i51KernelSendEvent ( i51_MSG_PADDOWN , parameters ) ;
	}
	else if ( MSG_TP_PRESS_UP == message )
	{
		i51KernelSendEvent ( i51_MSG_PADUP , parameters ) ;
	}
	else if ( MSG_TP_PRESS_MOVE == message ) 
	{
		i51KernelSendEvent ( i51_MSG_PADMOVE , parameters ) ;	
	} 


}

extern void i51AdeOsPortSetAlwayLight ( iBOOL always ) ;
MMI_RESULT_E i51GuiWindowMessages ( MMI_WIN_ID_T winid , MMI_MESSAGE_ID_E msgid , DPARAM param ) {

	//	author : Jelo 
	//	since : 2011.4.13

	//	notes : 窗口消息

	MMI_RESULT_E ret = MMI_RESULT_TRUE ;
	iU32 parameters = 0 ;
	iU32 message = 0 ;

	iU32 i51event = 0 ;
	iU32 keyevent = 0 ;

	iU32 keynumber = 0 , i51KeyNumber = 0 ;	
	

	extern iS32 i51AdeOsTimerMapActive ( iU8 mmkid ) ;

/*	
	if ( i51_GUI_MAIN_WINDOW != i51Gui.window )
		return 0 ;
*/		
	
	i51MemoryOverflowCheck () ;

	if ( KEY_FLIP == (msgid & 0xff) ) {
		
		//	滑盖退出
		MMK_CloseWin(winid) ;
		i51KernelExit (  i51_KERNEL_EXIT_DAP ) ;
		
		return 1 ;
	}

	//	处理按键
	keynumber = msgid & 0xff ;
	message = msgid & 0xff00 ;

	i51KeyNumber = i51PortMatchKey ( keynumber ) ;

    i51AdeOsLog(0, "PORT : i51GuiWindowMessages , keynumber = %d" , keynumber );
	if ( message == KEY_PRESSED )
	{
        i51AdeOsLog(0, "PORT : i51GuiWindowMessages , down keynumber = %d" , keynumber );
		i51event = i51_MSG_KEYDOWN ;
		keyevent = 1 ;
		RecoderDownKey = keynumber ;
	}
	else if ( message == KEY_RELEASED || message == KEY_LONG_RELEASED )
	{
        i51AdeOsLog(0, "PORT : i51GuiWindowMessages , up keynumber = %d" , keynumber );
		i51event = i51_MSG_KEYUP ;
		keyevent = 1 ;

		if ( RecoderDownKey != keynumber  ) {

			//	没有down消息的记录
            i51AdeOsLog(0, "PORT : i51GuiWindowMessages , keynumber = %d , RecoderDownKey = %d" , keynumber , RecoderDownKey );
			return 0;
		}
		i51PortUpVolume (i51KeyNumber) ;
		i51PortDownVolume (i51KeyNumber) ;
		RecoderDownKey = 0 ;
	}

#ifdef PDA_UI_SUPPORT_ANDROID
	#if 0
		//	针对于有的pad按键，只有up消息或者down消息，要将此处打开
		if ( i51PortMatchKey ( keynumber )  == i51_KEY_END_CALL || i51PortMatchKey ( keynumber )  == i51_KEY_END_CALL  )
		{
            i51AdeOsLog(0, "PORT : i51GuiWindowMessages , press home or menu" );
			/*
			i51InWin = 0 ;
			i51KernelExit ( i51_KERNEL_EXIT_DAP ) ;
			//	红键退出
			MMK_CloseWin(winid) ; 
			*/
			//	退到待机界面
			MMK_ReturnIdleWin () ;
			return 1;
		}
	#else
		if ( (i51PortMatchKey ( keynumber )  == i51_KEY_END_CALL)&& (message == KEY_RELEASED || message == KEY_LONG_RELEASED) )
		{
            i51AdeOsLog(0, "PORT : i51GuiWindowMessages , press home or menu" );
			/*
			i51InWin = 0 ;
			i51KernelExit ( i51_KERNEL_EXIT_DAP ) ;
			//	红键退出
			MMK_CloseWin(winid) ; 
			*/
			//	退到待机界面
			MMK_ReturnIdleWin () ;
			return 1;
		}	
	#endif
#endif


	if ( i51KeyNumber != i51_KEY_END_CALL  ) {

		//	如果是i51_KEY_END_CALL 我们不通知给上层处理
	
		//	发送按键消息
		if ( 1 == keyevent ) {
			
			#if 0
			//	针对于有的pad按键，只有up消息或者down消息，要将此处打开

			if ( keynumber == KEY_POWER ) {

                i51AdeOsLog(0, "PORT : i51GuiWindowMessages , close backlight ") ;
				i51AdeOsScreenAlwaysLight ( iFALSE ) ;
	//			MMK_SetCurCtrlHighligAction (iFALSE) ;
				return 0 ;
			} else if ( keynumber == KEY_HOME ) {

                i51AdeOsLog(0, "PORT : i51GuiWindowMessages , KEY_HOME ") ;
			} else if (keynumber == KEY_MENU) {

                i51AdeOsLog(0, "PORT : i51GuiWindowMessages , KEY_MENU ") ;
			}

		
			i51KernelSendEvent (  i51_MSG_KEYDOWN , i51KeyNumber ) ;	
			i51KernelSendEvent (  i51_MSG_KEYUP , i51KeyNumber ) ;	
			#else
			i51KernelSendEvent (  i51event , i51PortMatchKey ( keynumber ) ) ;	
			#endif

		}
	}
	
	switch ( msgid )  
	{

		case MSG_TIMER :
			i51AdeOsTimerMapActive ( *((iU32*)param) ) ;
		break ;
			
		case MSG_TP_PRESS_DOWN :
			//	高十六位存放x，低十六位存放y
			parameters = (( 0xffff & MMK_GET_TP_X(param) ) << 16)|( 0xffff & MMK_GET_TP_Y(param) ) ;		
			i51KernelSendEvent ( i51_MSG_PADDOWN , parameters ) ;	
			
		break ;
		
		case MSG_TP_PRESS_UP :
			//	高十六位存放x，低十六位存放y
			parameters = (( 0xffff & MMK_GET_TP_X(param) ) << 16)|( 0xffff & MMK_GET_TP_Y(param) ) ;	
			i51KernelSendEvent ( i51_MSG_PADUP , parameters ) ;	
			
		break ;
		
		case MSG_TP_PRESS_MOVE :
		{
			//	高十六位存放x，低十六位存放y
			parameters = (( 0xffff & MMK_GET_TP_X(param) ) << 16)|( 0xffff & MMK_GET_TP_Y(param) ) ;
			i51KernelSendEvent ( i51_MSG_PADMOVE , parameters ) ;	
	
		}
		break ;
		 
		case MSG_OPEN_WINDOW :
		{	  
			
 			i51InWin = 1 ;
			i51focusing = 1 ;
//			i51AdeOsLog(0, "PORT : i51GuiWindowMessages , angel %d (1)" , MMK_GetScreenAngle() );
			//	设置屏幕支持的角度
			#if defined(SPRD_6530) || defined(SPRD_6800)
			MMK_SetWinSupportAngle(winid, WIN_SUPPORT_ANGLE_0);
			#endif
//			MMK_SetWinAngle(winid, WIN_SUPPORT_ANGLE_0, FALSE);    
//			i51AdeOsLog(0, "PORT : i51GuiWindowMessages , angel %d (2)" , MMK_GetScreenAngle() );

			//	公司样机6530 要注释掉
//			#ifdef PDA_UI_SUPPORT_ANDROID
			MMK_DestroyControl (MMITHEME_GetStatusBarCtrlId()) ;
//			#endif
            i51AdeOsLog(0, "PORT : i51GuiWindowMessages , open win" );	
		} 

		break ;

		#if defined (PDA_UI_SUPPORT_ANDROID) || defined (PDA_UI_SUPPORT)
		case MSG_KEYUP_HOME:
		case MSG_KEYUP_MENU :
		#endif
	   	case MSG_KEYUP_RED :			/*
			i51InWin = 0 ;
			i51KernelExit ( i51_KERNEL_EXIT_DAP ) ;
			//	红键退出
			MMK_CloseWin(winid) ; 
			*/
			//	退到待机界面
            i51AdeOsLog(0, "PORT : i51GuiWindowMessages , press red" );
			MMK_ReturnIdleWin () ;
		break ;
  
		case MSG_GET_FOCUS :
			i51focusing = 1 ;
            i51AdeOsLog(0, "PORT : i51GuiWindowMessages , get focus" );
//			MMK_DestroyControl (MMITHEME_GetStatusBarCtrlId()) ;
			i51AdeOsPortSetAlwayLight (iTRUE) ;
			i51AdeMmiUpdateScreen() ;
			i51KernelSendEvent ( i51_MSG_RESUME , parameters ) ;	
			
		break ;
		
		case MSG_LOSE_FOCUS : 
			i51focusing = 0 ;
            i51AdeOsLog(0, "PORT : i51GuiWindowMessages , lose focus" );
			i51KernelSendEvent ( i51_MSG_PAUSE , parameters ) ;	
			
		break ; 
	
		case MSG_CLOSE_WINDOW :
            i51AdeOsLog(0, "PORT : i51GuiWindowMessages , colse win" );
			/*
			if ( i51InWin && !i51focusing )
			{
				//	有中断事件，进入到中断时间的界面
				i51AdeOsLog(0, "PORT : i51GuiWindowMessages , %d , %d" , i51InWin , i51focusing );
				i51KernelExit ( i51_KERNEL_EXIT_DAP ) ;
				MMK_CloseWin(winid) ; 	
			} */
			if ( i51InWin ) {

				//	直接进入其他界面退出
                i51AdeOsLog(0, "PORT : i51GuiWindowMessages , to other menu , %d , %d" , i51InWin , i51focusing );
				i51KernelExit ( i51_KERNEL_EXIT_DAP ) ;
				MMK_CloseWin(winid) ; 	
			}
			
			i51focusing = 0 ;
			i51InWin = 0 ;		
		break ;
	
	}

	return ret ;
	
}

//for test
/*
static void i51ChangeData(void* str,iU32 len )
{
    iCHAR data;
    iCHAR *p = (iCHAR *)str;
    iU32 i = 0;
    
    while(i<len*2)
    {
        data = p[i];
        p[i] = p[i+1];
        p[i+1] = data;
        i= i+2;
    }
}
*/
static MMI_RESULT_E i51GuiEditorWindowMessages (
	
	MMI_WIN_ID_T win_id ,   
	MMI_MESSAGE_ID_E msg_id , 
	DPARAM	param
)
{

	//	author : Jelo 
	//	since : 2011.6.11
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 处理输入法消息
#if 1
	MMI_RESULT_E			recode = MMI_RESULT_TRUE;
	MMI_STRING_T 			text = {0};
	iCHAR *Digital = 0 ;

//	i51AdeOsLog(0, "PORT : i51GuiEditorWindowMessages , S");
	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:
            i51AdeOsLog(0, "PORT : i51GuiEditorWindowMessages , open win S" );
                     #if (defined(SPRD_6620) || defined(SPRD_6600))
                            // 注:第二个参数是字体大小，第三个参数是字体颜色，第四个参数是背景颜色，请只修改第三个参数
//                        GUIEDITBOX_SetEditBoxStyle( MMI_i51_CTRL_ID_EDITOR, MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR, MMI_BACKGROUND_COLOR); 
                     #else
//                        GUIEDIT_SetFontColor ( MMI_i51_CTRL_ID_EDITOR , MMI_BLACK_COLOR ) ;
                     #endif
			if (i51_EDITOR_NUMBER == i51Gui.edtype)
			{

				#if  defined(SPRD_6800) || defined(SPRD_6530)
				GUIEDIT_SetMaxLen(MMI_i51_CTRL_ID_EDITOR, i51Gui.edlen);
				GUIEDIT_SetString(MMI_i51_CTRL_ID_EDITOR, i51Gui.edbuffer, i51Gui.edlen);
				#else
				if ( i51AdeStdWStrlen(i51Gui.edbuffer) ) {
					Digital = (iCHAR*)i51AdeOsMalloc (i51AdeStdWStrlen(i51Gui.edbuffer), __FILE__ , __LINE__) ;
					i51AdeStdUnicode2Ascii ( i51Gui.edbuffer , Digital ) ;
					GUIEDITBOX_SetTextEditBoxMaxLen(MMI_i51_CTRL_ID_EDITOR, i51Gui.edlen, i51Gui.edlen);
					GUIEDITBOX_SetDigitalEditBoxStringInfo(MMI_i51_CTRL_ID_EDITOR,  (iU16*)Digital, i51AdeStdWStrlen(i51Gui.edbuffer));
				}else {

                    i51AdeOsLog(0, "PORT : i51GuiEditorWindowMessages , open win (4)" );
					GUIEDITBOX_SetTextEditBoxMaxLen(MMI_i51_CTRL_ID_EDITOR, i51Gui.edlen, i51Gui.edlen);
					GUIEDITBOX_SetDigitalEditBoxStringInfo(MMI_i51_CTRL_ID_EDITOR,  i51Gui.edbuffer, i51AdeStdWStrlen(i51Gui.edbuffer));
				}
				
				if ( Digital )
				i51AdeOsFree (Digital) ;
				#endif
				
			} else{
			
				if (  i51_EDITOR_PASSWORD != i51Gui.edtype ) {

				#if defined (SPRD_6800) || defined (SPRD_6530)
				GUIEDIT_SetMaxLen(MMI_i51_CTRL_ID_EDITOR, i51Gui.edlen);
				GUIEDIT_SetString(MMI_i51_CTRL_ID_EDITOR, i51Gui.edbuffer, i51Gui.edlen);
				#else
				GUIEDITBOX_SetTextEditBoxMaxLen(MMI_i51_CTRL_ID_EDITOR, i51Gui.edlen, i51Gui.edlen);
				GUIEDITBOX_SetTextEditBoxStringInfo(MMI_i51_CTRL_ID_EDITOR, i51Gui.edbuffer, i51AdeStdWStrlen(i51Gui.edbuffer));
				#endif
				}
			}
			MMK_SetAtvCtrl(win_id, MMI_i51_CTRL_ID_EDITOR);
            i51AdeOsLog(0, "PORT : i51GuiEditorWindowMessages , open win E" );
		break;
		case MSG_FULL_PAINT:
		break;

		case MSG_CTL_PENOK:
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:
		case MSG_APP_OK:
            i51AdeOsLog(0, "PORT : i51GuiEditorWindowMessages , MSG_APP_OK" );
			#if defined (SPRD_6800) || defined (SPRD_6530)
			if (i51_EDITOR_NUMBER == i51Gui.edtype)
			{
				GUIEDIT_GetString(MMI_i51_CTRL_ID_EDITOR, &text);
			}
			else
			{
				GUIEDIT_GetString(MMI_i51_CTRL_ID_EDITOR, &text);
			}
			
			#else
			if (i51_EDITOR_NUMBER == i51Gui.edtype)
			{
				GUIEDITBOX_GetDigitalEditBoxInfo(MMI_i51_CTRL_ID_EDITOR, &text);
			}
	            else if ( i51_EDITOR_PASSWORD == i51Gui.edtype ) 
	            {
	                GUIEDITBOX_GetPasswordEditBoxInfo ( MMI_i51_CTRL_ID_EDITOR, &text ) ;
	            }
			else
			{
				GUIEDITBOX_GetTextEditBoxInfo(MMI_i51_CTRL_ID_EDITOR, &text);
			}
			#endif
			memset(i51Gui.edbuffer, 0, i51Gui.edlen*2);
			memcpy(i51Gui.edbuffer, text.wstr_ptr, text.wstr_len*2);
			i51Gui.inputlen = text.wstr_len ;
//            i51ChangeData(i51Gui.edbuffer,text.wstr_len);
			if ( i51Gui.EditorCallBack )
			i51Gui.EditorCallBack ( i51Gui.edbuffer , i51Gui.inputlen ) ;
			MMK_CloseWin(win_id);	
			MMK_CloseWin(i51EditWindowHandle);
		break;
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
            i51AdeOsLog(0, "PORT : i51GuiEditorWindowMessages , CANCEL" );
			if ( i51Gui.EditorCallBack )
			i51Gui.EditorCallBack (0 , 0 ) ;
			MMK_CloseWin(win_id);//返回上一级
			MMK_CloseWin(i51EditWindowHandle);

//			MMK_CloseWin(i51EditWindowHandle);
			
		break;
		case MSG_CLOSE_WINDOW:
            i51AdeOsLog(0, "PORT : i51GuiEditorWindowMessages , colse win" );
		break;
		default:
			recode = MMI_RESULT_FALSE;
		break;
	}

//	i51AdeOsLog(0, "PORT : i51GuiEditorWindowMessages , E");
	return recode;
#else
	MMI_RESULT_E recode = MMI_RESULT_TRUE ;
	MMI_STRING_T text = {0} ;
	MMI_HANDLE_T edit_ctrl_handle = MMK_GetCtrlHandleByWin ( win_id , MMI_i51_CTRL_ID_EDITOR ) ;

	if ( KEY_OK == (msg_id & 0xff) || KEY_CANCEL == (msg_id & 0xff) )
	{
		
		if ( KEY_RELEASED == (msg_id & 0xff00) )
		{
			
			GUIEDIT_GetString(edit_ctrl_handle,  &text);
			
			if  ( 0 < text.wstr_len )
			{	

				if ( i51Gui.edlen <= (text.wstr_len<<1) )
				{
					i51Gui.edlen = GUI_WstrToGB ( i51Gui.edbuffer , text.wstr_ptr , i51Gui.edlen>>1 ) ;
					i51Gui.EditorCallBack ( i51Gui.edbuffer , i51Gui.edlen ) ;			
				}
				else
				{
					i51Gui.edlen = GUI_WstrToGB ( i51Gui.edbuffer , text.wstr_ptr , text.wstr_len ) ;
					i51Gui.EditorCallBack ( i51Gui.edbuffer , text.wstr_len ) ;
				} 
				
			}
			
			MMK_CloseWin(win_id);
				     					
		}

	}
			
	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:

			switch(i51Gui.edtype)
			{
				case i51_EDITOR_NUMBER :
					GUIEDIT_SetMaxLen(edit_ctrl_handle, i51Gui.edlen);
					GUIEDIT_SetString(edit_ctrl_handle, i51Gui.edbuffer, i51Gui.edlen);
				break;
            
				case i51_EDITOR_PASSWORD :
				{
					BOOLEAN is_disp_im_icon = TRUE; 
					GUIEDIT_SetIm(edit_ctrl_handle, GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
					GUIEDIT_SetMaxLen(edit_ctrl_handle, i51Gui.edlen);
					GUIEDIT_SetStyle(edit_ctrl_handle, GUIEDIT_STYLE_MULTI);
					GUIEDIT_SetAlign(edit_ctrl_handle, ALIGN_LEFT);
					GUIEDIT_SetDispImIcon(edit_ctrl_handle, &is_disp_im_icon, PNULL);
					GUIEDIT_SetString(edit_ctrl_handle, i51Gui.edbuffer, i51Gui.edlen);
				}
				break;
           
				case i51_EDITOR_TEXT :
					GUIEDIT_SetIm(edit_ctrl_handle, GUIIM_TYPE_SET_ALL, GUIIM_TYPE_MULTITAP);
					GUIEDIT_SetTextMaxLen(edit_ctrl_handle, i51Gui.edlen, i51Gui.edlen);
					GUIEDIT_SetString(edit_ctrl_handle, i51Gui.edbuffer, i51Gui.edlen);
				break;
		
			}
			MMK_SetAtvCtrl(win_id, edit_ctrl_handle);
		break;
    
		case MSG_CTL_PENOK:
		case MSG_CTL_MIDSK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_CTL_CANCEL :
		case MSG_APP_CANCEL :
		break ;
		   
		case MSG_CLOSE_WINDOW :
			i51Gui.window = i51_GUI_MAIN_WINDOW ;
		break ;
		   
		default :
			recode = MMI_RESULT_FALSE;
		break;

	}

	return recode ;
#endif

	return 0 ;

}


iS32 i51GuiInitEditorWindow ( i51EDITORTYPE edtype , iS8* buffer , iU32 length , void (*callback) ( void* buffer , iU32 textlen ) )
{	

#if defined(SPRD_6600)||defined(SPRD_6620)
	
	MMI_CONTROL_CREATE_T    create    = {0};
	GUI_EDITBOX_INIT_DATA_T init_data = {0};
	MMI_HANDLE_T win_handle = PNULL;
	MMI_HANDLE_T  handle        = MMI_INVALID_ID;
	IGUICTRL_T    *ctrl_ptr     = NULL;
	GUI_EDITBOX_TYPE_E EditboxType = 0 ;
	 

	GUI_RECT_T rect = {
	            MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT,
	            MMI_EDITBOX_FULLSCREEN_CLIENT_TOP,
	            MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT,
	            MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM
	        };	
	
    i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , S");
// 临时解决6620V信密码框无法切换输入法问题
if ( edtype == i51_EDITOR_PASSWORD ) 
{
edtype = i51_EDITOR_TEXT;
}
	switch(edtype)
	{
	case i51_EDITOR_NUMBER :
	    EditboxType = EDITBOX_DIGITAL_E; 
	    break;
	    
	case i51_EDITOR_PASSWORD :
	    EditboxType = EDITBOX_PASSWORD_E; 
	 break;
	    
	case i51_EDITOR_TEXT :
	    EditboxType = EDITBOX_TEXT_E; 
	break;
	    
	default:
	    return;//Quit
	}
	
	
	   handle = MMK_CreateWin((uint32*)MMI_i51_TEXTEDIT_WIN_TAB, PNULL);
	   i51EditWindowHandle = handle ;
	    if(!handle || MMI_INVALID_ID==handle) {
	        return;
	    }



        i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , (1)");
	
	    if ( edtype == i51_EDITOR_TEXT ) {

			rect.bottom = MMI_EDITBOX_FULLSCREEN_CLIENT_BOTTOM ;
			rect.top = MMI_EDITBOX_FULLSCREEN_CLIENT_TOP ;
			rect.left = MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT ;
			rect.right = MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT ;
			
			init_data.add_data.text_add.max_ucs2_len        = length;
			init_data.add_data.text_add.max_default_len     = length;			
			init_data.add_data.text_add.allow_inputmethod   = IM_DEFAULT_ALL_INPUT_MODE_SET;
			init_data.add_data.text_add.cur_inputmethod     = IM_SMART_MODE;
		  
	     } else if ( edtype == i51_EDITOR_NUMBER ) {

			rect.bottom =  MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT ;
			rect.top = MMI_TITLE_HEIGHT ;
			rect.left = MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT ;
			rect.right = MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT ;
			
			 init_data.max_str_len = length;
//		        init_data.add_data.text_add.allow_inputmethod   = IM_DIGITAL_MODE;
//		        init_data.add_data.text_add.cur_inputmethod     = IM_DIGITAL_MODE;
			
	     	} else {

			rect.bottom =  MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT ;
			rect.top = MMI_TITLE_HEIGHT ;
			rect.left = MMI_EDITBOX_FULLSCREEN_CLIENT_LEFT ;
			rect.right = MMI_EDITBOX_FULLSCREEN_CLIENT_RIGHT ;
			init_data.max_str_len = length;
//		    init_data.add_data.text_add.allow_inputmethod   = IM_DIGITAL_MODE | IM_ENGLISH_MODE ;
//		    init_data.add_data.text_add.cur_inputmethod     = IM_ENGLISH_MODE;

	     	}

	    init_data.type          = EditboxType;
	    init_data.rect          = &rect;	
        i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , (2)");
	  
	    create.ctrl_id           = MMI_i51_CTRL_ID_EDITOR;
	    create.guid              = SPRD_GUI_EDITBOX_ID;
	    create.parent_win_handle = handle;
	    create.init_data_ptr     = &init_data;
        i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , (3)");
	    ctrl_ptr = MMK_CreateControl(&create);
        i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , (4)");

	    if(PNULL == ctrl_ptr) {
	        MMK_CloseWin(MMI_i51_CTRL_ID_EDITOR);
	        return;
	    }
		
		i51Gui.edtype = edtype ;	
		i51Gui.edbuffer = buffer ;
		i51Gui.edlen = length/2 -1 ;
		i51Gui.EditorCallBack = callback ;
#else

	MMI_HANDLE_T handle = 0 ;
	MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
	GUIEDIT_INIT_DATA_T         edit_init = {0};
	MMI_CONTROL_CREATE_T        create = {0};
	GUIEDIT_TYPE_E          editbox_type = GUIEDIT_TYPE_NULL; 
	GUIIM_TYPE_T allow_im = GUIIM_TYPE_NONE;
	GUIIM_TYPE_T init_im = GUIEDIT_TYPE_DIGITAL;
 
	switch(edtype)
	{
	case i51_EDITOR_NUMBER :
	    editbox_type = GUIEDIT_TYPE_DIGITAL; 
	    break;
	    
	case i51_EDITOR_PASSWORD :
	    editbox_type = GUIEDIT_TYPE_PASSWORD; 
	 break;
	    
	case i51_EDITOR_TEXT :
	    editbox_type = GUIEDIT_TYPE_TEXT; 
	break;
	    
	default:
	    return;//Quit
	}
   
	 handle = MMK_CreateWin((uint32*)MMI_i51_TEXTEDIT_WIN_TAB, PNULL);
	 i51EditWindowHandle = handle ;
	 if(!handle || MMI_INVALID_ID==handle) {
	        return;
	  }



     i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , (1)");

	//edit init data
	edit_init.both_rect = MMITHEME_GetWinClientBothRect(handle);
	edit_init.type        = editbox_type;
	edit_init.str_max_len = length ;

	#if defined(PDA_UI_SUPPORT_ANDROID)
//	edit_init.is_hide_softkeyboard = iTRUE ;
	#endif

	//create edit control
	create.ctrl_id           = MMI_i51_CTRL_ID_EDITOR;
	create.guid              = SPRD_GUI_EDITBOX_ID;
	create.init_data_ptr     = &edit_init;
	create.parent_win_handle = handle;
	MMK_CreateControl(&create); 

    i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , (2)");

    // 因为没有edit_init可以初始化的输入法属性结构，输入法属性修改必须在输入法控件创建MMK_CreateControl之后修改
	if (  i51_EDITOR_PASSWORD == edtype) {
       	allow_im = GUIEDIT_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH;
       	init_im = GUIEDIT_TYPE_DIGITAL;
        GUIEDIT_SetIm(MMI_i51_CTRL_ID_EDITOR, allow_im, init_im);
	}

    i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , (3)");

//	MMK_SetWinSupportAngle(handle, support_angle);
 //     MMK_SetWinAngle(handle, MMK_GetScreenAngle(), FALSE);

	i51Gui.edtype = edtype ;	
	i51Gui.edbuffer = buffer ;
	i51Gui.edlen = length/2 - 1 ;
	i51Gui.EditorCallBack = callback ;
		
//	return (iS32 ) handle ;
#endif
    i51AdeOsLog(0, "PORT : i51GuiInitEditorWindow , E");
	return 0 ;
	
}

#if 0
void i51PortExit () 
{

	//	author : Jelo 
	//	since : 2011.7.29
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
 
	MMK_CloseWin ( i51PortWindowHandle ) ;	
}

iBOOL i51PortExitReq () 
{

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	i51PortExit () ;
	
}

iS32 i51PortEntry () 
{  

	//	author : Jelo  
	//	since : 2011.4.13
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 初始化i51 Booter，接口可能会有不同的实现，但是接口定义不可以改变

    i51AdeOsLog(0, "PORT : i51PortEntry , Static Layer\r\n" ) ;

	i51PortWindowHandle = (iU32) MMK_CreateWin ( (iU32* ) MMI_i51_WIN_TAB , PNULL ) ;	

	#if 0  
		AllocConsole();
		freopen( "CONOUT$", "w+t", stdout );
	#endif

	return 1 ;

}
#endif

#include "mmiaudio_ctrl.h"
/*
static void i51PortEntryBeforePause ()
{
	//	进入i51前的一些处理动作暂停或其他处理

	#ifdef SPRD_6800
	MMIAUDIO_PauseBgPlay (MMIBGPLAY_MODULE_MP3 ) ;
	#elif defined (SPRD_6530)
	{
	}
	#else
	MMIAUDIO_PauseBgPlay ( MMIBGPLAY_TYPE_MP3 , MMIBGPLAY_MODULE_MP3 ) ;
	#endif
	
}

static void i51PortExitAfterResume ()
{	//	退出i51之后动作的恢复或其他处理
	#ifdef SPRD_6800
	MMIAUDIO_ResumeBgPlay (MMIBGPLAY_MODULE_MP3 ) ;
	#elif defined (SPRD_6530)
	{
	}	
	#else
	MMIAUDIO_ResumeBgPlay ( MMIBGPLAY_TYPE_MP3 , MMIBGPLAY_MODULE_MP3 ) ;
	#endif

}
*/


iBOOL i51PortCreateFrontEnd (void) {

	//	author : Jelo  
	//	since : 2011.12.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

    i51AdeOsLog(0, "PORT : i51PortCreateFrontEnd , S");
	
	#if defined (UI_P3D_SUPPORT)
	
  	 MMI_Disable3DEffect(MMI_3D_EFFECT_BROWSER);//	如遇报错直接注释掉
	#endif

	#if defined (SPRD_6800) || defined(SPRD_6530)
	MMK_SetScreenAngle(LCD_ANGLE_0);
	#endif

	i51AdeOsPortSetAlwayLight (iTRUE) ;
	i51PortWindowHandle = (iU32) MMK_CreateWin ( (iU32* ) MMI_i51_WIN_TAB , PNULL ) ;


    i51AdeOsLog(0, "PORT : i51PortCreateFrontEnd , %x" , i51PortWindowHandle );
	
	return 1 ;
}

iBOOL i51PortReleaseFrontEnd (void) {

	//	author : Jelo  
	//	since : 2011.12.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
    i51AdeOsLog(0, "PORT : i51PortReleaseFrontEnd , S");
	i51AdeOsPortSetAlwayLight (iFALSE) ;
	MMK_CloseWin ( i51PortWindowHandle ) ;	
	#if defined (UI_P3D_SUPPORT)
  	 MMI_Enable3DEffect(MMI_3D_EFFECT_BROWSER);//	如遇报错直接注释掉
	#endif

     i51AdeOsLog(0, "PORT : i51PortReleaseFrontEnd , E");
	
	return 1 ;
	
}


#ifdef __I51_SIMPLE_INSTALL__

#ifdef __I51_TINY__
extern iU32 i51KitStaticCoreAdapter[] ;
void i51SimpleLunchApp( iU16 *pAppName )
{
    typedef struct
	{
		iU16 *appname;
		iS32 logosize;
		iS32 dispsize;
		iU8 logotype;//0 文字 1图片
		iU8 gprsprompt;
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
            i51AdeOsLog(0, "PORT : i51SimpleLunchApp , Get i51KitMain F");
		return;
		}
		i51KitMain( 0, 0x00FFFFFF , (void*)i51KitStaticCoreAdapter );
#endif		
    	}
}

#else
void i51SimpleLunchApp( iU16 *pAppName )
{
    typedef struct
	{
		iU16 *appname;
		iS32 logosize;
		iS32 dispsize;
		iU8 logotype;//0 文字 1图片
		iU8 gprsprompt;
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
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetAppmenu () , (unsigned int )i51SapGetAppmenuSize () , &param, 0 , 1 ) ;
}
#endif
#endif

static iBOOL i51CheckThirdCpRunning ()
{

if ( MMIAPIMP3_IsPlayerPlaying()) {

    i51AdeOsLog(0, "PORT : i51CheckThirdCpRunning , MP3 Play");
	MMIPUB_OpenAlertWinByTextId(PNULL,TXT_i51_PLAYING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	return iFALSE;
}


#ifdef SPRD_QQ
	{
		extern char  QIsQQRuning();
		extern BOOLEAN QVM_DisplayExitConfirm(void);
		if(QIsQQRuning())
		{
			if(QVM_DisplayExitConfirm())
			{
	               return iFALSE ;
			}
		}
	}
#endif			
	
#ifdef FM_SUPPORT
	{
		extern BOOLEAN MMIAPIFM_Stop(void);
		MMIAPIFM_Stop();
	}
#endif
#ifdef MET_PLAT_QQ_SUPPORT
	{
		extern  BOOLEAN MET_PLAT_qq_is_backrun(void);
		extern int met_qq_conflict_promt(void);
		if(MET_PLAT_qq_is_backrun()) //QQ后台运行中
		{
			met_qq_conflict_promt();    // 退出并提示是否关闭QQ, 以便再次进入
			return iFALSE ;
		}
	}
#endif    
#ifdef MCARE_V31_SUPPORT
	{
	//extern MBOOL McareV31_IsMcareRunning(MVOID);
	//extern MBOOL MCareV31_Exit_Confirm(MVOID);
	//if(McareV31_IsMcareRunning)
	//{
		if(MCareV31_Exit_Confirm())
		{
			return iFALSE;		
		}
	//}
	}
#endif    

	return iTRUE ;
}

static iBOOL i51PortUdiskisUsing()
{

        if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
        {
            i51AdeOsLog(0, "PORT : i51PortUdiskisUsing , card using");
		return iTRUE ;
        }
	  return iFALSE ;
}
static  void i51PortCheckTCard ()
{
	memset ( LogPath , 0 ,sizeof(LogPath) ) ;
	memset ( DatPath, 0 ,sizeof(DatPath) ) ;
	#ifdef SPRD_6530
	if ( MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)) )
	#else
//	if ( MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD)) )
	if ( MMIFILE_GetDeviceStatus(MMIFILE_DEVICE_SDCARD, 1) )
	#endif
	{
		// 	T卡存在

		i51AdeStdWStrcpy ( LogPath , L"E:\\i51\\i51.log") ;
		i51AdeStdWSprintf(DatPath, sizeof(DatPath),"E:\\i51\\%d_%d.dat",I51_KERNEL_P4_VERSION,I51_ADAPTER_P4_VERSION);
	} 
	#ifdef SPRD_6530
	else if ( MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK)) )
	#else
//	else if ( MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)) )
	else if ( MMIFILE_GetDeviceStatus(MMIFILE_DEVICE_UDISK, 1) )
	#endif
	{

		i51AdeStdWStrcpy ( LogPath , L"D:\\i51\\i51.log") ;
		i51AdeStdWSprintf(DatPath, sizeof(DatPath),"D:\\i51\\%d_%d.dat",I51_KERNEL_P4_VERSION,I51_ADAPTER_P4_VERSION);
		
	} else {

		LogFileExist = iFALSE ;
		DatFileExist = iFALSE;
	}

	if ( LogPath[0] != '\0' ) {

		SFS_HANDLE file = 0 ;
		SFS_FIND_DATA_T findfile = { 0 } ;

		file =  SFS_FindFirstFile ( LogPath , &findfile ) ;	

		if(SFS_INVALID_HANDLE != file)
		{
		    if(!(SFS_ATTR_DIR&findfile.attr))
		    {
		        LogFileExist = iTRUE;
		    }
		}
		SFS_FindClose ( file ) ;
	}
		//判断数据文件是否存在
	if ( DatPath[0] != '\0' ) {

		SFS_HANDLE file = 0 ;
		SFS_FIND_DATA_T findfile = { 0 } ;

		file =  SFS_FindFirstFile ( DatPath , &findfile ) ;	

		if(SFS_INVALID_HANDLE != file)
		{
		    if(!(SFS_ATTR_DIR&findfile.attr))
		    {
		        DatFileExist = iTRUE;
		    }
		}
		SFS_FindClose ( file ) ;
	}
}

iU16* i51PortGetLogPath ()
{

	return LogPath ;
}

iU16* i51PortGetDatPath()
{
	return DatPath;
}
iBOOL i51PortLogFileIsExist ()
{
	return LogFileExist ;
}

iBOOL i51PortDatFileIsExist()
{
	return DatFileExist;
}
static iBOOL i51PortCheckStorage ()
{
	//	检测存储空间

#ifdef SPRD_6530
//if ( iTRUE != MMIAPIUDISK_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK)) 
// 	&& iTRUE != MMIAPIUDISK_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD))  )
if ( iTRUE !=MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK))
	&& iTRUE !=MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)))

#else
//if ( iTRUE != MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)) 
// 	&& iTRUE != MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD))  )

    if ((iTRUE != MMIFILE_GetDeviceStatus(MMIFILE_DEVICE_UDISK, 1))
       && (iTRUE != MMIFILE_GetDeviceStatus(MMIFILE_DEVICE_SDCARD, 1))) 
#endif
{
        
        if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
        {
            i51AdeOsLog(0, "PORT : i51PortCheckStorage , card using");
		    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_i51_NO_CARD_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            i51AdeOsLog(0, "PORT : i51PortCheckStorage , no card");
		    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_i51_NO_CARD,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
		return iFALSE;
}
 	return iTRUE ;

}

static iBOOL i51CheckKernelInitSucceed ()
{
	//	检测初始化是否成功
	if ( !l_BootWaited ) {

/*		
		MMI_STRING_T    str = {0};
		MMI_GetLabelTextByLang(TXT_MAINMENU_i51, &str);
		MMIPUB_OpenWaitWin(1, &str, PNULL, PNULL, MMI_i51_WIN_ID, PNULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, 0);
*/
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_i51_SYS_INIT,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		return iFALSE;
	}

	 if ( i51KernelGetInitStatus() == i51_KERNEL_INIT_FAILED ){

		i51KernelInit(i51RunBootApp); 
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_i51_SYS_INIT,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
//		i51KernelInit(0);
		return iFALSE;
	 }else if ( i51KernelGetInitStatus() == i51_KERNEL_INIT_GOING ) {

		return iFALSE;
	 }else {

		return iTRUE ;
	 }
}

static void i51RunBootApp( iBOOL results )
{

	extern const unsigned char* i51SapGetAppBoot(void);
	extern const int i51SapGetAppBootSize(void);

	SFS_FIND_DATA_T findfile = { 0 } ;
	SFS_HANDLE file = 0 ;
	l_BootWaited = iTRUE ;

	file =  SFS_FindFirstFile ( L"E:\\i51CALLTIME" , &findfile ) ;	

	if(SFS_INVALID_HANDLE != file)
	{
		if(!(SFS_ATTR_DIR&findfile.attr))
		{
			//	如果存在这个文件，通话时长为1 分钟
            i51AdeOsLog(0, "PORT : i51RunBootApp , call time 1 min");  
		  i51SapAppBootPara[0] = 60 ;
		}
	}
	SFS_FindClose ( file ) ;

	if(!results) return;
	i51KernelRunStaticApp ( L"AppBoot" , (unsigned char* )i51SapGetAppBoot () , (unsigned int )i51SapGetAppBootSize () , i51SapAppBootPara, 1, 0 ) ;	
	
}

static void i51BootDelay(iU32 timer)
{ 
	i51PortCheckTCard () ;
	i51AdeOsSysLogLoadData();
    i51AdeOsLog(0, "PORT : i51BootDelay , Init Kernel");  
	i51AdeOsTimerStop (timer) ;
	i51KernelInit(i51RunBootApp);     
}

static void i51Boot(void)
{
    //	author : Miki
	//	since : 2011.12.2
	// 	(C) PKIG Tech. Co., Ltd.
//    gui_start_timer( 18000, i51BootDelay );
	i51AdeOsTimerStart ( i51AdeOsTimerCreate ( 0 , 0 ) , i51BootDelay ,  15000 ) ;
	
}

void i51GameCenterInit ()
{
	//	i51初始化

	//	注册NV
//	i51AdeOsLog(0, "PORT : i51GameCenterInit , start init");
	i51Boot () ;
//	i51KernelInit(i51RunBootApp); 
}

iBOOL i51PortMain ( iS32 aid , iU32 message , void* parameter ) 
{

}

iBOOL i51PortRunningDebuger ( iU8 *InputText )
{
	if ( strcmp ( InputText , "*#20111221*#") != 0) {

        i51AdeOsLog(0, "PORT : i51PortRunningDebuger , %s" ,InputText);
		return iFALSE ;
	}
	i51KernelRunDynamicAppEx ( L"i51Debuger" , 0 ) ;
	return iTRUE ;

}

iBOOL i51PortRunningExplorer ( iU8 *InputText )
{
	if ( strcmp ( InputText , "*#51*#") != 0) {

        i51AdeOsLog(0, "PORT : i51PortRunningExplorer , %s" ,InputText);
		return iFALSE ;
	}

	#ifdef __I51_Explorer__

	i51PortEntryExplorer () ;
	#endif
	return iTRUE ;
}
#include "i51_EntryGame.c"


void i51AdeOsSysLogLoadData()
{
	iS32 FileSize = 0;
	iFILE pFile = 0;
	iU16* FilePath = NULL;
	
	if(i51PortLogFileIsExist())
	{
		if(i51PortDatFileIsExist())
		{
			FilePath = i51PortGetDatPath () ;
			i51AdeOsFileAModeEnable();
			pFile = i51AdeOsFileOpen(FilePath, i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ, __FILE__, __LINE__);

			if(pFile != 0)
			{
				i51AdeOsFileGetSize(pFile, &FileSize);
				//去掉头两个字节数据
				g_pu8LogDataBaseBuf = (iU8*)i51AdeOsMalloc(FileSize-2,__FILE__, __LINE__);
				i51AdeOsFileSetSeek(pFile, 2, i51_OS_FL_SEEK_HEAD);
				i51AdeOsFileRead(pFile, g_pu8LogDataBaseBuf,FileSize-2,0);		
			}
			else
			{
                i51AdeOsLog(0, "PORT : i51AdeOsSysLogLoadData , 缺少日志数据库无法打印底层日志 . 请下载版本号为 %d_%d 的日志数据库 ", 
			                          I51_KERNEL_P4_VERSION, I51_ADAPTER_P4_VERSION); 
			}
			i51AdeOsFileClose(pFile);     
			i51AdeOsFileAModeDisable();
		}
	   else
		{
            i51AdeOsLog(0, "PORT : i51AdeOsSysLogLoadData , 缺少日志数据库无法打印底层日志 . 请下载版本号为 %d_%d 的日志数据库 ", 
			                          I51_KERNEL_P4_VERSION, I51_ADAPTER_P4_VERSION); 
		}
	}
	
}

iU8* i51AdeOsSysLogGetString(iU16 logIndex)
{
	iU16 nOffset = 0;

	if(g_pu8LogDataBaseBuf == 0)
	{
		return 0;
	}

	nOffset = g_pu8LogDataBaseBuf [I51_LOG_IDXTBL_SIZE * logIndex]
		|g_pu8LogDataBaseBuf [I51_LOG_IDXTBL_SIZE*logIndex+1] <<8;

	return g_pu8LogDataBaseBuf +nOffset;
}
