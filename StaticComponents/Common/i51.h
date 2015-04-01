
/*

	Common Definations of 'i51'
	For more details about 'i51' you could read : "The_i51_Architecture_2.0.xls"	
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef _THE_i51_ARCHITECTURE_H
#define _THE_i51_ARCHITECTURE_H

//	输出系统信息
//	1. 内存占用量
//	2. 线程数
//	3.socket 数
//	最终发布时，该宏为关闭状态 
//#define i51PROFILINGS 

#ifdef WIN32
#undef i51PROFILINGS
#define i51PROFILINGS
#endif

#ifdef __I51__  
//	静态层视图 
#define i51STATIC
#else
//	动态层视图    
#define i51DYNAMIC
#endif

#if defined(WIN32) 
#include <assert.h>
#define iASSERT	 
#else
#define iASSERT(X)	
#endif

#if defined(WIN32) && defined(i51STATIC)
#define i51SIMULATOR
#define __align(x)
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

//	类型定义
#define iS8 signed char
#define iU8 unsigned char
#define iS16 signed short
#define iU16 unsigned short
#define iS32 signed int
#define iU32 unsigned int
#define iS64 signed long
#define iU64 unsigned long
#define iBOOL unsigned char
#define iCHAR char
#define iWCHAR iU16
#define iTRUE 1
#define iFALSE 0
#define iNULL 0
#define iFILE iU32
#define iHANDLE void*
#define iCOLOR iU16
#define iALPHA iU8

#ifdef WIN32
#define iKIT extern _declspec (dllexport) 
#else  
#define iKIT 
#endif

#ifndef i51STATIC
#define iLog(...) i51AdeOsLog  ( i51_LOG_DC , __VA_ARGS__ )
#endif

#define __I51_TINY__

//#define i51_LOADER_DBG 
//#define i51ADAPTER_DEBUG 
//#define i51KERNEL_DEBUG 
  
//	Marco Definations of i51 Static Kit
#define i51KIT_STATIC_KIT_DEF_BEGIN \
	enum\
	{

#define i51KIT_STATIC_KIT_DEF_PORT(KIT)\
	i51_STATIC_KIT_##KIT##Offset ,
	
#define i51KIT_STATIC_KIT_DEF_END \
	};

#define i51KIT_STATIC_KIT_DEF(MODULE)\
	iU32* i51KitStatic##MODULE = 0 

#define i51KIT_STATIC_KIT_DEC(MODULE)\
	extern iU32* i51KitStatic##MODULE 

#define i51KIT_STATIC_KIT_DEF_TYPE(RET,KIT,PARA)\
	typedef RET (*KIT##FuncType) PARA ; 

#define i51KIT_STATIC_KIT_DEF_CALL(MODULE,KIT)\
	((KIT##FuncType)(i51KitStatic##MODULE[i51_STATIC_KIT_##KIT##Offset]))

#ifndef WIN32 
//	Mapping of Static Kits	
#define i51KIT_STATIC_KIT_MAPING_BEGIN(MODULE) \
	__align(4) static iU32 i51KitStatic##MODULE [] =\
	{
#else
//	Mapping of Static Kits	
#define i51KIT_STATIC_KIT_MAPING_BEGIN(MODULE) \
	static iU32 i51KitStatic##MODULE [] =\
	{
#endif
	
#define i51KIT_STATIC_KIT_MAPING(Kit)\
	(iU32)Kit ,
	
#define i51KIT_STATIC_KIT_MAPING_END \
	} ;

#define i51KIT_STATIC_KIT_GET_MAPING(MODULE)\
	i51KitStatic##MODULE 

#define i51KIT_STATIC_KIT_SET(MODULE,KIT)\
	i51KitStatic##MODULE = (iU32*)KIT ;

#define i51KIT_STATIC_KIT_ASSERT(MODULE)\
	if ( 0 == i51KitStatic##MODULE )\
	{\
		return 0 ;\
	}

//	Marco Definations of i51 Dynamic Kit
#define i51KIT_DYNAMIC_KIT_DEF_BEGIN \
	enum\
	{\

#define i51KIT_DYNAMIC_KIT_DEF_PORT(KIT)\
	i51_KIT_DYNAMIC_##KIT##Offset ,
	
#define i51KIT_DYNAMIC_KIT_DEF_END \
	};

#define i51KIT_DYNAMIC_KIT_DEF(Kit)\
	iU32* i51KitDynamic##Kit##Port = 0 

#define i51KIT_DYNAMIC_KIT_DEC(Kit) \
	extern iU32* i51KitDynamic##Kit##Port 

//	for sys kit
#define i51KIT_DYNAMIC_SYS_KIT_DEF(Kit)\
	iU32* i51KitDynamic##Kit##Port = 0 

#define i51KIT_DYNAMIC_SYS_KIT_DEC(Kit) \
	extern iU32* i51KitDynamic##Kit##Port 
//	end

#define i51KIT_DYNAMIC_KIT_DEF_TYPE(RET,KIT,PARA)\
	typedef RET (*KIT##FuncType) PARA ;

#define i51KIT_DYNAMIC_KIT_DEF_CALL(KIT,NAME)\
	((NAME##FuncType)(i51KitDynamic##KIT##Port[i51_KIT_DYNAMIC_##NAME##Offset]))

#define i51KIT_DYNAMIC_KIT_LOAD(KIT,PATH)\
	i51KitDynamic##KIT##Port = i51KernelLoadKit ( PATH ) ;

#define i51KIT_DYNAMIC_KIT_UNLOAD(Kit)\
	if ( 0 != i51KitDynamic##Kit##Port )\
	{\
		i51KernelUnloadKit ( (iHANDLE)i51KitDynamic##Kit##Port);\
		i51KitDynamic##Kit##Port = 0; \
	}


//	for sys kit
#define i51KIT_DYNAMIC_SYS_KIT_LOAD(KIT,PATH)\
	i51KitDynamic##KIT##Port = i51KernelLoadSysKit ( PATH ) ;

#define i51KIT_DYNAMIC_SYS_KIT_UNLOAD(Kit)\
	if ( 0 != i51KitDynamic##Kit##Port )\
	{\
		i51KernelUnloadSysKit ( (iHANDLE)i51KitDynamic##Kit##Port);\
	}
	
#define i51KIT_DYNAMIC_SYS_KIT_PROTOTYPE(Kit)\
	i51KitDynamic##Kit##Port
	
//	end

#define i51KIT_DYNAMIC_KIT_PROTOTYPE(Kit)\
	i51KitDynamic##Kit##Port
	
#ifndef WIN32 
//	Mapping of Dynamic Kits
#define i51KIT_DYNAMIC_KIT_MAPING_BEGIN(MODULE) \
	__align(4) static iU32 i51Dynamic##MODULE##Kit [] =\
	{
#else
//	Mapping of Dynamic Kits
#define i51KIT_DYNAMIC_KIT_MAPING_BEGIN(MODULE) \
	static iU32 i51Dynamic##MODULE##Kit [] =\
	{
#endif
	
#define i51KIT_DYNAMIC_KIT_MAPING(Kit)\
	(iU32)Kit ,
	
#define i51KIT_DYNAMIC_KIT_MAPING_END \
	} ;

#define i51KIT_DYNAMIC_KIT_GET_MAPING(MODULE)\
	i51Dynamic##MODULE##Kit 

typedef enum {

	//	author : Jelo 
	//	since : 2012.6.26
	// 	(C) 2011 , PKIG Tech. Co., Ltd.  
	
	i51_CPU_ARM = 0 ,
	i51_CPU_MIPS ,

} ;

typedef struct {

	//	author : Jelo
	//	since : 2012.4.13
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	//	应用名称
	iCHAR name[32] ;
	//	*.i51文件名
	iU16 identifer[32] ;
	//	应用的图标路径，相对于i51KIT目录的路径。
	//	例如Portal\Portal.png,必须是png文件。
	iU16 icon [32] ;
	
} iAPPINFO ;

typedef struct
{

	//	author : Otto
	//	since : 2011.8.19
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    
	//	裁剪矩形
	//	左上角
	iS16 X ;
	iS16 Y ;
	//	绘制宽高
	iS16 Width ;
	iS16 Height ;

} iRECT ;

typedef struct
{

	//	author : Otto
	//	since : 2012.6.21
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    
	//	裁剪矩形
	//	左上角
	iS32 X ;
	iS32 Y ;
	//	绘制宽高
	iS32 Width ;
	iS32 Height ;

} iRECTEX ;

typedef enum
{

	//	author : william
	
	//	since : 20111012
	//	PKIG Tech. Co., Ltd.
	
	i51_ADE_CNF_VER_MTK = 0 ,
	i51_ADE_CNF_VER_MSTAR ,
	i51_ADE_CNF_VER_SPRD,
    	i51_ADE_CNF_VER_CS
	
} i51ADECNF ;

typedef enum {

	//	author : jelo
	//	since : 20111112
	//	PKIG Tech. Co., Ltd.

	i51_ADE_CNF_LITTEL_ENDIAN = 0 ,
	i51_ADE_CNF_BIG_ENDIAN ,
	
} i51ADECNFENDIAN ;

typedef enum
{

	//	author : Jelo
	//	since : 2011.8.23
	//	(C) PKIG Tech. Co., Ltd.

	i51_MSG_INIT = 0x1000 ,
	//	1
	i51_MSG_RUN ,
	//	2
	i51_MSG_ENTERIDLE ,
	//	3
	i51_MSG_EXITIDLE ,
	//	4
	i51_MSG_PAINT ,
	//	5
	i51_MSG_PAUSE ,
	//	6
	i51_MSG_RESUME ,
	//	7
	i51_MSG_EXIT ,
	//	8
	i51_MSG_KEYUP ,
	//	9
	i51_MSG_KEYDOWN ,
	//	10
	i51_MSG_PADUP ,
	//	11
	i51_MSG_PADDOWN ,
	//	12
	i51_MSG_PADMOVE ,
	//	13
	i51_REQ_LOADEX ,
	//	14
	i51_REQ_EXIT ,
	//	15
	i51_REQ_INSTALLED ,
	//	16
	i51_REQ_UNINSTALLED ,
	//	17
	i51_MSG_RELOADSERVER ,	
		

} i51MSG ;
	
typedef enum 
{


	//	author : Jelo
	//	since : 2011.4.13
	//	(C) PKIG Tech. Co., Ltd.
	
	i51_LOG_DC = 1 ,
	i51_LOG_SC = 1 << 1 ,
	
} i51OSLOG ;

typedef enum 
{

	//	author : Jelo
	//	since : 2011.4.13
	//	(C) PKIG Tech. Co., Ltd.
	
	//	notes : 文件系统参数

	//	文件存在时打开
	//	默认打开方式
	i51_OS_FL_EXISTING_OPEN 		= 1 ,
	//	如果文件存在时打开，不存在时创建并打开
	i51_OS_FL_ALWAYS_OPEN		= 1 << 1 ,
	//	总是首选创建文件并打开
	i51_OS_FL_ALWAYS_CREATE 		= 1 << 2  ,
	//	只读模式
	i51_OS_FL_READ 				= 1 << 3  ,
	//	写模式包含读
	i51_OS_FL_WRITE 				= 1 << 4  ,
	
	//	文件属性: 错误，起因: 文件不存在或其它
	i51_OS_FL_ATTR_ERR 			= 1 << 5 ,
	//	文件属性: 只读	
	i51_OS_FL_ATTR_ROL 			= 1 << 6 ,
	//	文件属性: 可写	
	i51_OS_FL_ATTR_WRT 			= 1 << 7 ,
	//	文件属性: 隐藏	
	i51_OS_FL_ATTR_HID 			= 1 << 8 ,
	//	文件属性: 目录	
	i51_OS_FL_ATTR_DIR 			= 1 << 9 ,

	//	从文件头开始定位
	i51_OS_FL_SEEK_HEAD 			= 1 << 10 ,
	//	从文件当前位置开始定位
	i51_OS_FL_SEEK_CURRENT 		= 1 << 11 ,	
	//	从文件尾逆向定位

	i51_OS_FL_SEEK_TAIL 			= 1 << 12 
		
} i51OSFL ;

typedef enum 
{

	//	author : Jelo
	//	since : 2011.5.3
	//	(C) PKIG Tech. Co., Ltd.
	
	//	notes : 键盘事件、触屏事件

	i51_KEY_NUM0 = 1 ,
	i51_KEY_NUM1 ,
	i51_KEY_NUM2 ,
	i51_KEY_NUM3 ,
	i51_KEY_NUM4 ,
	i51_KEY_NUM5 ,
	i51_KEY_NUM6 ,
	i51_KEY_NUM7 ,
	i51_KEY_NUM8 ,
	i51_KEY_NUM9 ,
	//	左软键
	i51_KEY_LEFT ,
	//	右软键
	i51_KEY_RIGHT ,
	//	音量UP
	i51_KEY_VOL_UP ,
	//	音量DOWN
	i51_KEY_VOL_DOWN ,	
	//	OK 键
	i51_KEY_ENTER ,
	//	上键
	i51_KEY_ARROW_UP ,
	//	下键
	i51_KEY_ARROW_DOWN ,
	//	左键
	i51_KEY_ARROW_LEFT ,
	//	右键
	i51_KEY_ARROW_RIGHT ,
	//	星号键
	i51_KEY_STAR ,
	//	井号键
	i51_KEY_SHARP ,
	//	呼叫键
	i51_KEY_CALL ,
	//	关机键
	i51_KEY_END_CALL 
	
} i51KEYS ;

typedef enum {

	//	author : Jelo	
	//	since : 2011.10.15
	//	(C) PKIG Tech. Co., Ltd.

	//	no error
	i51_KERNEL_LOAD_ERR_NO = 0 ,
	//	*.i51 File Not Exist
	i51_KERNEL_LOAD_ERR_NE ,	
	//	References not exists
	i51_KERNEL_LOAD_ERR_REF ,
	//	Crc failed
	i51_KERNEL_LOAD_ERR_CRC ,
	//	Not Enough Memory
	i51_KERNEL_LOAD_ERR_MEM ,
	//	Create Front-End Error
	i51_KERNEL_LOAD_ERR_FE ,	
	
} ;

typedef enum 
{

	//	author : Jelo
	//	since : 2011.4.13
	//	(C) PKIG Tech. Co., Ltd.
	
	//	notes : 图像解码器参数

	//	基于文件方式
	i51_MMI_DEC_FILE_BASED ,
	//	基于内存方式
	i51_MMI_DEC_MEM_BASED 
	
} i51MMIDEC ;

typedef enum 
{

	//	author : Jelo
	//	since : 2012.1.6
	//	(C) PKIG Tech. Co., Ltd.
	
	//	notes : 文件存在标识

	//	文件不存在
	i51_FILE_NOT_EXIST = 0 ,
	//	文件存在
	i51_FILE_EXIST ,
	//	文件存在且已经被打开
	i51_FILE_EXIST_OPENED ,
	
} i51FILEEXIST ;


typedef enum {

	//	author : Jelo	
	//	since : 2011.8.23
	//	(C) PKIG Tech. Co., Ltd.

	//	应用程序状态

	//	清醒状态
	i51_APP_STATUS_SOBER ,
	//	睡眠状态
	i51_APP_STATUS_SLEEP ,
	//	休眠状态
	i51_APP_STATUS_DORMANCY ,
	
} i51KERNELSTA ;

/*
typedef enum 
{

 	//	author : Jelo
	//	since : 2011.5.24
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 短信接口枚举值

	//	短信发送返回值
	//	短信发送成功
	i51_SMS_SEND_SUCCESS ,
	//	短信发送失败
	i51_SMS_SEND_FAILED ,

	//	保存发送记录
	i51_SMS_SDSAVE_YES ,
	//	不保存发送记录
	i51_SMS_SDSAVE_NOT 
	
} i51SMSVALUE ;
*/

typedef enum 
{

 	//	author : Jelo
	//	since : 2011.6.9
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 短信钩子返回值

	i51_SMS_HOOK_NORMAL = 0 ,		
	i51_SMS_HOOK_DELETE , 
	
} i51SMSHKR ;

typedef enum 
{

	//	author : Jelo
	//	since : 2011.4.26
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 输入法类型

	//	纯数字输入
	i51_EDITOR_NUMBER ,
	//	密码输入
	i51_EDITOR_PASSWORD ,
	//	数字加文字输入
	i51_EDITOR_TEXT 
	
} i51EDITORTYPE ;

typedef enum
{

	//	author : Jelo
	//	since : 2011.4.26
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 多媒体格式

	i51_MEDIA_TYPE_MIDI = 0 ,
	i51_MEDIA_TYPE_WAV  ,
	i51_MEDIA_TYPE_AMR  ,
	i51_MEDIA_TYPE_MP3  	
	
} i51METYPE ;


/*
typedef enum
{

	//	author : Jelo
	//	since : 2011.6.22
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : SOCKET connect , send , recieve 三个接口的返回值

	i51_SOC_CONNECT_SUCCESS ,
	i51_SOC_CONNECT_FAILED ,
	i51_SOC_SEND_SUCCESS ,
	i51_SOC_SEND_FAILED ,
	i51_SOC_RECEIVE_SUCCESS ,
	i51_SOC_RECEIVE_FAILED 

} i51SOCSTATE ;
*/

typedef enum  
{
     
	//	author : Jelo
	//	since : 2011.6.20
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : dl下载时的错误码


	//	正在下载
	i51_SL_DL_RUNING = 0 ,
	//	下载完成
	i51_SL_DL_FINISHED = 1 ,
	//	网络出错
	i51_SL_DL_ERROR_NET = -1 ,
	//	创建链路失败
	i51_SL_DL_ERROR_CREATELINK =  -2 ,
	//	创建soc失败
	i51_SL_DL_ERROR_CREATESOC = -3 ,
	//	网络超时
	i51_SL_DL_ERROR_TIMEROUT = -4 ,
	//	错误请求命令
	i51_SL_DL_ERROR_QUEMIND = -9001 ,
	//	crc校验错误
	i51_SL_DL_ERROR_CRC = -9002 ,
	//	请求数据解析错误
	i51_SL_DL_ERROR_PARSEDATA = -9999 ,
	//	底层协议版本过期
	i51_SL_DL_ERROR_BOTTOMVERSION = -1006 ,
	//	服务器维护，暂停服务
	i51_SL_DL_ERROR_SERVERPAUSE = -1009 ,
	//	获取sn失败
	i51_SL_DL_ERROR_GETSN = -2001 ,
	//	错误sn
	i51_SL_DL_ERROR_SN = -2002 ,
	//	错误文件名
	i51_SL_DL_ERROR_FILENAME = -2003 ,
	//	剩余空间不足
	i51_SL_DL_ERROR_LASTSPACE = - 2004 ,
	//	下载地址错误
	i51_SL_DL_ERROR_URI = - 2005 ,
	//	没有更新或找不到
	i51_SL_DL_ERROR_NOTFIND = -2006 ,
	//	操作文件出错
	i51_SL_DL_ERROR_FILE = -10 ,
	//	创建TIMER出错
	i51_SL_DL_ERROR_CREATETIMER = -20 ,
	//	重新dlstart
	i51_SL_DL_ERROR_AGAIN_DLSTART = -30,
	//	如果返回这个参数，在链路不存在的情况下，创建链路将会是cmnet方式
	i51_SL_DL_ERROR_NEXT_CMNET = -40
	
} i51SlDlVALUE ;

typedef enum  
{
     
	//	author : Jelo
	//	since : 2011.6.20
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 安装时的错误码


	//	正在下载
	i51_KERNEL_INSTALL_RUNING = i51_SL_DL_RUNING ,
	//	下载完成
	i51_KERNEL_INSTALL_FINISHED = i51_SL_DL_FINISHED ,
	//	网络出错
	i51_KERNEL_INSTALL_ERROR_NET = i51_SL_DL_ERROR_NET ,
	//	创建链路失败
	i51_KERNEL_INSTALL_ERROR_CREATELINK = i51_SL_DL_ERROR_CREATELINK ,
	//	创建soc失败
	i51_KERNEL_INSTALL_ERROR_CREATESOC = i51_SL_DL_ERROR_CREATESOC ,
	//	网络超时
	i51_KERNEL_INSTALL_ERROR_TIMEROUT = i51_SL_DL_ERROR_TIMEROUT ,
	//	错误请求命令
	i51_KERNEL_INSTALL_ERROR_QUEMIND = i51_SL_DL_ERROR_QUEMIND ,
	//	crc校验错误
	i51_KERNEL_INSTALL_ERROR_CRC = i51_SL_DL_ERROR_CRC ,
	//	请求数据解析错误
	i51_KERNEL_INSTALL_ERROR_PARSEDATA = i51_SL_DL_ERROR_PARSEDATA ,
	//	底层协议版本过期
	i51_KERNEL_INSTALL_ERROR_BOTTOMVERSION = i51_SL_DL_ERROR_BOTTOMVERSION ,
	//	服务器维护，暂停服务
	i51_KERNEL_INSTALL_ERROR_SERVERPAUSE = i51_SL_DL_ERROR_SERVERPAUSE ,
	//	获取sn失败
	i51_KERNEL_INSTALL_ERROR_GETSN = i51_SL_DL_ERROR_GETSN ,
	//	错误sn
	i51_KERNEL_INSTALL_ERROR_SN = i51_SL_DL_ERROR_SN ,
	//	错误文件名
	i51_KERNEL_INSTALL_ERROR_FILENAME = i51_SL_DL_ERROR_FILENAME ,
	//	剩余空间不足
	i51_KERNEL_INSTALL_ERROR_LASTSPACE = i51_SL_DL_ERROR_LASTSPACE ,
	//	下载地址错误
	i51_KERNEL_INSTALL_ERROR_URI = i51_SL_DL_ERROR_URI  ,
	//	没有更新或找不到
	i51_KERNEL_INSTALL_ERROR_NOTFIND = i51_SL_DL_ERROR_NOTFIND ,
	//	操作文件出错
	i51_KERNEL_INSTALL_ERROR_FILE = i51_SL_DL_ERROR_FILE ,
	//	解压出错
	i51_KERNEL_INSTALL_ERROR_UNPRESS = -11 ,
	//	文件完整性有问题(crc)
	i51_KERNEL_INSTALL_ERROR_IMCOMPLETE = -12 ,
	//	分配内存出错
	i51_KERNEL_INSTALL_ERROR_MALLC = -13,
	//	不是此类文件 (比如说re文件)
	i51_KERNEL_INSTALL_ERROR_ELSEFILE = -14 ,
	//	下载器已经在使用
	i51_KERNEL_INSTALL_ERROR_DOWNLOAER_USING = -15 ,
	//	创建TIMER出错
	i51_KERNEL_INSTALL_ERROR_CREATETIMER = i51_SL_DL_ERROR_CREATETIMER ,
	//	如果返回这个参数，在链路不存在的情况下，创建链路将会是cmnet方式
	i51_KERNEL_INSTALL_ERROR_NEXT_CMNET = i51_SL_DL_ERROR_NEXT_CMNET
	

} i51KERNELINSTALL ;

typedef enum   
{

	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : 下载资源类型

	i51_SAP_DLCOM_SERIALNUMBER = 1 ,
	i51_SAP_DLCOM_BITSTREAM = 2 
	
} i51SlDlCOMMAND ;

typedef struct {

	//	author : Jelo
	//	since : 2011.10.28
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	iU16 version ;
	iCHAR path [ 32 ] ;

} i51SllDKITREF ;

typedef struct 
{

	//	author : Jelo
	//	since : 2011.4.26
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	//	notes : i51 平台输入法参数
	//	value between i51EDITORTYPE
	iU32 type ;	
	//	给输入法提供的数据内存
	void* buffer ;
	//	单位Bytes
	iU32 length ;
	
	//	使用该回调将输入结果传给应用层
	void (*callback) ( void* text, iU32 textlen ) ;
		
} i51STDEDITOR ;

/*
typedef struct
{

 	//	author : Jelo
	//	since : 2011.5.24
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 平台短信结构

	//	目标号码
	iS8* number ;
	//	短信内容(UC2)
	iU16* content ;
	//	短信字符个数
	iU32 contlen ;
	//	号码字符个数
	iU32 numlen ;

	//	表示用哪张卡发送
	//	sim卡编号通过i51AdeSmsGetSims 获取得到
	iS32 sim ;

	//	callback function
	//	成功返回iTRUE
	//	失败返回iFALSE
	void (*callback)( iBOOL results ) ;

	//	短信是否需要保存在发件箱
	iBOOL save ;
	
} i51SMS ;
*/

typedef struct 
{

	//	author : Jelo
	//	since : 2011.4.22
	//	(C) PKIG Tech. Co., Ltd.
	
	iU16 port ;
	iU32 ipaddr ;
	
} i51SOCADDR ;

/*
typedef struct 
{

 	//	author : Jelo
	//	since : 2011.5.6
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 批量合并层，结合void i51AdeMmiLayerMerges ( ... ) 接口使用

	//	层的内存地址
	iU16* buffer ;
	//	层的宽度
	iU16 width ;
	//	层的高度
	iU16 height ;
	//	透明色, 0 时表示不透明
	iU16 key ;
	//	半透明系数，范围: 0~255 ，255时表示无半透明效果，0 时全透明
	iU16 alpha ;
	//	合并到 第0 层的位置
	iU32 desx ;
	iU32 desy ;

} i51MERGEPANEL ;
*/

typedef struct
{

 	//	author : Jelo
	//	since : 2011.8.11
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	iU16 name[ 256 ] ;
	//	i51OSFL code
	iU16 attribute ;
	
	//	文件创建时间
	struct
	{
	
		iU16 Year ;
		iU16 Month ;
		iU16 Day ;
		iU16 Hour ;
		iU16 Minute ;
		iU16 Second ;
		
	}  CreateTime ;

	//	文件修改时间
	struct
	{
	
		iU16 Year ;
		iU16 Month ;
		iU16 Day ;
		iU16 Hour ;
		iU16 Minute ;
		iU16 Second ;
		
	}  EditedTime ;
	
} i51FILEINFO ;

typedef struct 
{

 	//	author : Jelo
	//	since : 2012.2.22
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	联系人姓名
	iU16 name[ 16 ] ;
	//	联系人电话号码
	iCHAR number [44] ;

} i51PHONEBOOK ;

typedef void (*i51SOCCONCBK) ( iBOOL state , iS32 socket ) ;
typedef void (*i51SOCSENDCBK) ( iBOOL state , iS32 socket , iU32 textlen ) ;
typedef void (*i51SOCRECVCBK) ( iBOOL state , iS32 socket , iU8* buffer , iU32 textlen ) ;
typedef iS32 (*i51SMSHOOKCALLBACK) (iS32 simCard , iCHAR* number , iU32 numlen , void* buffer , iU32 length ) ;
typedef void (*i51MEDIACALLBACK) ( iU32 value ) ;
typedef void (*i51LLAYERCALLBACK) ( iBOOL state ) ;
typedef void (*i51ACCCALLBACK) ( iS32 x , iS32 y , iU32 gspeed ) ;
typedef void (*i51RECCALLBACK ) ( void* stream , iU32 length ) ;
typedef void (*i51DNSCALLBACK) ( iU32 address ) ;
typedef void (*i51ADECALLBACK) ( iBOOL success ) ;
typedef  void ( *i51TIMERCALLBACK ) ( iU32 id ) ; 
typedef void (*i51SlDlCALLBACK ) ( iS32 state , iU32 textlen , iU32 offset , iU32 totall , iU32 refcount ) ;
typedef iBOOL (*i51KITMAIN) ( iS32 , iU32 , void* ) ;
typedef iU32* (*i51KITPORT) () ; 

#endif

