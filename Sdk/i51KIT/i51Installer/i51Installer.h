
#ifndef THE_InstallApp_H
#define THE_InstallApp_H

#include "i51.h"
//模板内按钮控件的个数
#define I51_BUTTON_CONTROL_MAX_COUNT 	2

//模板内公布栏控件的个数
#define I51_TIP_CONTROL_MAX_COUNT 	1

//模板内进度条控件的个数
#define I51_PROCESS_CONTROL_MAX_COUNT 	1

//公布栏最大的条数
#define I51_TIP_INFO_MAX_COUNT		10
//公布栏内能够显示的文字的长度 
#define I51_TIP_INFO_MAX_LENGTH		32

//提示框能够显示的长度
#define I51_MSG_INFO_MAX_LENGTH	100


typedef enum {

	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	模板类型

	//非自适应模板
	i51_SAP_GUI_TEMPLATE_NOT_ADEPTIVE  ,
	
	//自适应模板
	i51_SAP_GUI_TEMPLATE_ADEPTIVE  ,
	

	
}  i51SAPGUI_TEMPLATE ;

typedef enum {

	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	控件类型

	//按钮控件
	i51_SAP_GUI_CONTROL_BUTTON  ,
	
	//公布栏控件
	i51_SAP_GUI_CONTROL_TIP  ,
	
	//进度条控件
	i51_SAP_GUI_CONTROL_PROCESS  ,
	
}  i51SAPGUI_CONTROL ;

typedef enum {

	//	author : william
	//	since : 20111011
	//	PKIG Tech. Co., Ltd.
	//	提示框类型

	//纯色提示框
	i51_SAP_GUI_MSG_SIMPLE ,
	//图片提示框
	i51_SAP_GUI_MSG_IMAGE ,
	


} ;




//控件回调
typedef void (*i51SAPGUICONTROLCALLBACK) ( i51SAPGUI_CONTROL ControlType ) ;

typedef struct {
	
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	按钮控件

	//相对屏幕左上角的x坐标/如果不是图片的就是背景坐标
	iS32 x ;

	//相对于屏幕左上角y坐标
	iS32 y ;

	//按钮图片的宽度/如果不是图片的，就是画背景的宽度和高度
	iS32 width ;

	//按钮图片的高度
	iS32 height ;

	//正常状态下的图片
	iHANDLE NomalImg ;

	//选中后的图片
	iHANDLE SelectedImg ;

	/*******针对于文字按钮******/
	iU8 *text ;

	iCOLOR TextColor ;

	//按钮文字的宽度
	iU16 TextWidth ;

	//按钮文字的高度
	iU16 TextHeght ;

	//按钮文字的个数
	iU16 TextLength ;

	//文字按钮的正常颜色
	iCOLOR buttonNormalColor ;

	//文字按钮选中后的颜色
	iCOLOR buttonSelectColor ;
	/**********************************/

	//控件在模板内是否被使用
	iBOOL used ;

	//按钮控件的回调
	i51SAPGUICONTROLCALLBACK callback ;

} iSAPBUTTON ;

typedef struct {
	
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	公布栏控件

	//相对屏幕左上角的x坐标
	iS32 x ;

	//相对于屏幕左上角y坐标
	iS32 y ;

	//进度条图片或者素模板进度条的宽度
	iS32 width ;

	//进度条图片或者素模板进度条的高度
	iS32 height ;

	//	进度条的进度
	iS32 seek ;

	//空的进度条图片
	iHANDLE EmptyImg ;

	//满的进度条图片
	iHANDLE FillImg ;

	//素模板进度条空的颜色
	iCOLOR EmptyColor ;
	
	
	//素模板进度条满的颜色
	iCOLOR FillColor ;	
	

	iCOLOR PointColor ;

	//控件在模板内是否被使用
	iBOOL used ;
	//	当前下载第几个
	iU32 curdl ;
	//	共需要下载多少个
	iU32 dlcount ;

	//按钮控件的回调
	i51SAPGUICONTROLCALLBACK callback ;

} iSAPPROGRESS ;



typedef struct {
	
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	公布栏控件

	//通告栏图片相对屏幕左上角的x坐标
	iS32 x ;

	//通告栏图片相对于屏幕左上角y坐标
	iS32 y ;

	//公布栏图片或者素模板公布栏的宽度
	iS32 width ;

	//公布栏图片或者素模板公布栏的高度	
	iS32 height ;

	//公告栏图片
	iHANDLE TipImg ;

	//公告栏文字相对屏幕左上角的x坐标，相对于x坐标
	iS32 TextX ;

	//公告栏文字相对于屏幕左上角y坐标，现对于y坐标
	iS32 TextY ;

	//公布栏公布的文字
	iCHAR text[I51_TIP_INFO_MAX_COUNT][I51_TIP_INFO_MAX_LENGTH+1] ; /*???????????????*/

	//	已有公布栏信息的条目
	iU32 TextCount ;

	//文字的宽度
	iU16 TextWidth ;

	//文字的高度
	iU16 TextHeght ;

	//文字的个数
	iU16 TextLength ;

	//公布栏文字颜色
	iCOLOR TextColor ;

	//公布栏的填充颜色
	iCOLOR FillColor ;

	//控件在模板内是否被使用
	iBOOL used ;

	//按钮控件的回调
	i51SAPGUICONTROLCALLBACK callback ;

} i51SAPTIP ;


typedef struct {

	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	模板

	//模板类型
	iU32 type ;

	//背景图片
	iHANDLE* bgimage ;

	//背景颜色
	iCOLOR bgcolor ;

	//按钮控件
	iSAPBUTTON button[I51_BUTTON_CONTROL_MAX_COUNT] ;

	//公布栏控件
	i51SAPTIP tip[I51_TIP_CONTROL_MAX_COUNT] ;

	//进度条控件
	iSAPPROGRESS process[I51_PROCESS_CONTROL_MAX_COUNT] ;
	
} iSAPTEMPLATE ;


#if 0
typedef struct {

	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	提示框

	//提示框类型
	iU32 type ;

	//提示图片相对屏幕左上角的x坐标
	iS32 x ;

	//提示图片相对于屏幕左上角y坐标
	iS32 y ;

	//提示图片或者素模板公布栏的宽度
	iS32 width ;

	//提示图片或者素模板公布栏的高度	
	iS32 height ;

	//背景图片
	iHANDLE* bgimage ;

	//背景颜色
//	iCOLOR bgcolor ;

	iCOLOR color1 ;

	iCOLOR color2 ;
	
	iCOLOR color3 ;

	//提示文字相对屏幕左上角的x坐标，相对于x坐标
	iS32 TextX ;

	//提示文字相对于屏幕左上角y坐标，现对于y坐标
	iS32 TextY ;

	//公布栏公布的文字
	iCHAR text[I51_MSG_INFO_MAX_LENGTH] ; /*???????????????*/

	//公布栏文字颜色
	iCOLOR TextColor ;

	//按钮控件
	iSAPBUTTON button[I51_BUTTON_CONTROL_MAX_COUNT] ;



} iSAPMESSAGEBOX ;
#endif

extern iSAPTEMPLATE* i51SlCtrlCreateTemplate ( iU32 type , iHANDLE* bgimage , iCOLOR bgcolor ) ;
extern iBOOL i51SlCtrlTempInsertButtom ( iSAPTEMPLATE* template , iSAPBUTTON* button  ) ;
extern iBOOL i51SlCtrlTempInsertProgress ( iSAPTEMPLATE* template , iSAPPROGRESS* progress  ) ;
extern iBOOL i51SlCtrlTempSetProcessSeek ( iSAPTEMPLATE* template , iSAPPROGRESS* progress  , iU32 seek ) ;
extern iBOOL i51SlCtrlThempInsertTip ( iSAPTEMPLATE* template , i51SAPTIP* tip ) ;
extern iBOOL i51SlCtrlTempDraw ( iSAPTEMPLATE* template ) ;
//extern iSAPMESSAGEBOX* i51SlCtrlCreateMessagebox ( iU32 type , iHANDLE* bgimage , iCOLOR color1  ,iCOLOR color2  ,iCOLOR color3  , iS32 x , iS32 y , iS32 width ,iS32 height , iS32 TextX , iS32 TextY ,iCHAR * text , iS32 TextLength , iCOLOR TextColor )  ;
//extern iBOOL i51SlCtrlMessageboxInsertButtom ( iSAPMESSAGEBOX* messagebox , iSAPBUTTON* Button  ) ;
//extern iBOOL i51SlCtrlMessageboxInsertButtom ( iSAPMESSAGEBOX* messagebox , iSAPBUTTON* Button  )  ;

#endif


