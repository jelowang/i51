

 
/*

	The Implementing of 'Kernel Components' , Module : 'Kernel'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	Developing by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#include <stdlib.h>
#include <string.h>
#include "i51AdeCnf.h"
#include "i51AdeMmi.h"
#include "i51AdeOs.h"
#include "i51AdeSoc.h"
#include "i51AdeStd.h"
#include "i51AdeSms.h"
#include "i51SlStruct.h"
#include "i51Port.h"
#include "i51SlDl.h"
#include "i51Kernel.h"
#include "i51SlLoader.h"
//akilae add since 2012.7.11 
#include "i51KitCore.h"
//akilae add finished

#ifdef i51SIMULATOR
#include <windows.h>
#endif




//	控制KitServer 内存模式
//	如果开宏表示静态RAM，否则动态分配
#define i51_KERNEL_STATIC_MEMORY

//	William Add Start 2012.7.10
#if defined (SPRD_6600) || defined (SPRD_6620) || defined (SPRD_6800)  || defined (SPRD_6530) 
//	针对展讯加载过程中死机的特殊处理
#define __SPRD_CLEAR_CACHE__
#endif
//	William Add Finish

static void i51KernelCopyDirTree(iU16 *DirFrom,iU16 *DirTo);

#define i51_KERNEL_DEFAULT_FPS 22
//	Kernel Implemention Version

//	Jelo Edited 20120302
//	plus one since bug solving  
//#define i51_KERNEL_IMP_VERSION 23
//	Kernel Design Version
//#define i51_KERNEL_DES_VERSION 2
//	最大值3.15.1023
#define i51_KERNEL_VERSION "0.3.23"
//	Jelo Edited Finished

#define i51_KERNEL_STATIC_MEMORY_SIZE 20*1024
//	Max Kit of SAP list  
#define i51_KERNEL_SAP_KIT_LIST_MAX 2  
//	Max Kit of DAP list    
#define i51_KERNEL_DAP_KIT_LIST_MAX 20
//	Max References of Kit
#define i51_KERNEL_iCL_MAX_REF 16

typedef void (*i51KERNELTIMERCALLBACK) ( iU32 id ) ;

static void  i51AdeOsCreateCB ( iBOOL success ) ;
static void  i51AdeMmiCreateCB ( iBOOL success ) ;
static void  i51AdeSocCreateCB ( iBOOL success ) ;
static void  i51AdeSmsCreateCB ( iBOOL success ) ;
static void  i51AdeStdCreateCB ( iBOOL success ) ; 
static void i51KernelReleaseDapKit () ; 
static iBOOL i51KernelCreateDir () ;  
static iBOOL i51KernelStopTimer ( iBOOL ) ;
static iU16* i51KernelGetApplicationPathExternal () ;
static iU16* i51KernelGetApplicationPathInternal () ;
static void i51KernelCopy2MovableDisk ()  ;
static iBOOL i51KernelKitListDropKit ( iHANDLE kithandle ) ;
void i51KernelUnloadKitCore (iU32 id) ;
iBOOL  i51KernelLoadKitCore (iU32 id) ;

//	注意：
//	Kit 与App有以下区别 
//	Kit默认运行态为i51_APP_STATUS_SLEEP
//	App运行态只能是i51_APP_STATUS_SOBER
//	SysKit 与AppKit有以下区别
//	SysKit默认从i51Sys目录下加载
//	AppKit从开发指定目录加载
//	只有一个静态套件：i51KitServer

//	外部下载传进回调
static void (*KernelExternDLCallback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount  ) ;
void (*i51KernelGetSnCallback)( iBOOL results , void* buffer , iU32 buflen )  ;

//crc,re,资源版本号，文件格式版本号，资源的个数 占用的字节数
#define CRC_HEAD_RESCOUNT_COUNT	(sizeof ( iU32 )+2+4+4+4+4)
//读取资源个数的文件偏移量  src re ,crc,re,资源版本号，文件格式版本号
#define RES_COUNT_SETOFF	(sizeof ( iU32 )+2+4+4)
//	id的字符串形式的最大长度
#define RES_ID_STR_LEN	32
typedef struct 
{

	//	author : william
	//	since : 2011.8.24
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	//id的字符串
	iCHAR ID[RES_ID_STR_LEN] ;
	//偏移量
	iU32 SetOff ;  
	iU32 size ;
	
} I51_RES_BIN_INDEX_INFO ;



typedef struct {

	//	需要下载的列表，为在文件上的目标，真正下载的时候需要转换成下载路径
	iCHAR i51KernelInstallNeedDlList[i51_KERNEL_iCL_MAX_REF][64]  ;
	//	需要下载的版本id
	iU32 i51KernelInstallNeedDlVerionID[i51_KERNEL_iCL_MAX_REF] ;
	//	不需要解压列表,第一个为不要解压的个数，后面不需要解压的下标
	iU32 i51KernelNoUmpressIndex[i51_KERNEL_iCL_MAX_REF+1]  ;
	//	下载后的目标路径
//	iCHAR i51KernelInstallDestPath[i51_KERNEL_iCL_MAX_REF][64] ;
	//	应用名称
	iCHAR i51KernelAppName[64] ;
	//	完成安装需要需要文件的总大小
	iU32 i51KerneltotalSize ;
	//	已经下载的大小
	iU32 i51KerneldlSize ;
	//	需要下载的个数
	iU32 i51KernelInstallNeedDlCount ;
	//	正在下载现在下载列表中第几个1开始
	iU32 i51KernelDlIndex ;
	//	解压开始下标
	iU32 i51KernelStartUnpressIndex ;
	//	从服务器得到需要下载的个数
	iU32 i51KernelFromServerDlCount ;
	//	安装时的状态码
	iS32 state ;
	//	安装时模式 0 安装1 更新
	iS32 i51KernelInstallMode ;
	// 更新地址
	iCHAR i51KernelUri[32] ;
	
} i51INSTALLER ;


typedef struct
{
	//	author : william
	//	since : 2012.4.17
	// 	(C) PKIG Tech. Co., Ltd.	

	//	下载器已经被创建情况，用定时器通知上层
	//	用来异步通知启用的定时器id
	// Install 定时器
	iU32 Installtimerid ;
	//	Reinstall 定时器
	iU32 Reinstalltimerid ;
	//	获取sn
	iU32 GetSntimerid ;
	//	安装应用名称
	iCHAR InstallAppname[64] ;
	//	更新应用名称
	iCHAR ReinstallAppname[64] ;
	//	install 回调
	void (*IntallCallback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount  ) ;
	//	Reinstall回调
	void (*ReinstallCallback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount  ) ;
	//	回去sn回调
	void (*GetSncallback)( iBOOL results , void* buffer , iU32 buflen ) ;
	
}I51DOWNLOADERRUNING ;


typedef enum {

	//	author : Jelo	
	//	since : 2011.10.20
	//	(C) PKIG Tech. Co., Ltd.

	//	静态作用域，指套件被加载到了静态内存
	i51_KERNEL_KIT_SCOPE_STATIC = 0 ,
	//	动态作用域，指套件被加载到了动态内存
	i51_KERNEL_KIT_SCOPE_DYNAMIC ,

} i51KITSCOPE ;

typedef enum {

	//	author : Jelo	
	//	since : 2011.10.18
	//	(C) PKIG Tech. Co., Ltd.

	//	系统盘
	i51_KERNEL_DISK_MODE_SYSTEM = 0 ,
	//	移动盘
	i51_KERNEL_DISK_MODE_MOVABLE ,

} i51DISKMODE ;

typedef struct  {

	//	author : Jelo	
	//	since : 2011.10.10
	//	(C) PKIG Tech. Co., Ltd.	

	iU16 name [32] ;
	//	value like i51_REQ_LOAD
	iU32 request ;
	//	消息对象, 0 时表示发送给kernel的消息
	iU32 object ;
	//	消息发送者
	iU32 sender ;
	//	消息参数缓存
	iU8* buffer ;
	//	缓存长度
	iU32 buffsize ;

} i51KERNELREQ ;

typedef struct  {

	//	author : Jelo	
	//	since : 2011.8.23
	//	(C) PKIG Tech. Co., Ltd.

	//	1+1+1+2+4+66+1+1+4+4 = sizeof 85

	//	被加载次数
	//	当卸载为0 时才真正被释放

	iU8 scope ;
	//	if *.kit lib == iTRUE
	//	else lib == iFALSE
	iBOOL lib ;
	//	不接受i51_MSG_PAUSE 消息
	iBOOL DontRevPause ;
		
	iU16 ldtimes ;
	iU32 id ;
	//	App Name
	iU16 name [ 32+1 ] ;
	iU8 status ;
	//	退出后的状态
	//	默认进入休眠状态
	iU8 StatusAfterExit ;
	
	i51KITMAIN i51KitMain ;
	i51KITPORT i51KitPort ;

} i51KERNELKIT ;

typedef struct {

	//	author : Jelo	
	//	since : 2011.8.25
	//	(C) PKIG Tech. Co., Ltd.

	//	各种全局变量


	i51KERNELINIT InitStatus;

	//	0 为系统盘模式
	//	1 为移动盘模式	
	iCHAR DiskMode ; 
 
	iBOOL FrontEndEnable ;

	iU32 fps ;

	//	SAP = Static Application based on i51 
	i51KERNELKIT SapKitList[i51_KERNEL_SAP_KIT_LIST_MAX] ;
	iU32 SapKitListTotall ;	
 
	//	DAP = Dynamic Application based on i51
	i51KERNELKIT* DapKitList ;  
	iU32 DapKitListTotall ;
	
	i51SlDSQUEUE queue ; 
	
	//	清醒状态的Kit 数量
	//	当它大于0 并且等于1时CurrentSoberKit 才有意义
	iS32 TotallSober ;
	i51KERNELKIT CurrentSoberKit ; 
	
	iU32 Timer ;
	iBOOL TimerRuning ;

	iU32 ReqTimer ;
	iBOOL ReqTimerRuning ;

	//	Jelo Edit Since 2012.12.6
	//	iU8 NotCreateRefTable ;
	//	Jelo Edit Finished
	
	iBOOL PauseCantSend ;
	
    //  Pause status of kernel
    //  Skip LOADEX Req , Just do that
	iBOOL Pause ;
		
	#ifdef i51PROFILINGS
		iU8 ShowProfilings ;
		iU32 TotallMemory ;
		iU16 FpsCounter ; 
	#endif
	   
} i51KERNEL ;



//	Jelo Edit 2012.6.8
//	静态套件 加载到此处
#ifdef i51_KERNEL_STATIC_MEMORY
	static iU8 i51KernelStaticMemory [i51_KERNEL_STATIC_MEMORY_SIZE] = { 0 } ;
#else
	static iU8* i51KernelStaticMemory = 0 ;
#endif
//	Jelo Edit Finished

static iU8 i51KernelStaticMemoryAppBoot [2*1024] = { 0 } ;
static i51KERNEL i51Kernel = { 0 } ; 
static i51KERNELTIMERCALLBACK KernelTimerCallback = 0 ;
static i51INSTALLER i51KernelInstaller = { 0 } ;
static I51DOWNLOADERRUNING i51DownloaderRunning = {0} ;

//	Jelo Add Since 2012.4.16
//	加载的KitServer 版本
static iU32 i51KitServerVersion = 0 ;
//	Jelo Add Finished

i51KIT_STATIC_KIT_MAPING_BEGIN(Adapter)    
	//	OS
	i51KIT_STATIC_KIT_MAPING(i51AdeOsMalloc)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFree)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetTotallMemoryLeft)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileCreateDir) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileFind) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileFindNext)  
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileFindClose) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileDeleteDir)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileOpen) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileClose) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileRead) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileWrite) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileSetSeek) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileGetSeek) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileRename) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileGetSize) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileDelete) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileGetAttr) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileSetAttr) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileFlush) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileCopy) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileMove) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileIsExist) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileIsDirExist) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileReadInt8) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileReadInt16) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileReadInt32) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileWriteInt8) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileWriteInt16) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileWriteInt32) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsTimerCreate) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsTimerStart) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsTimerStop) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetCallingTime) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetNetFlow) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetTick) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetTime) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsLog) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetBaseStation)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsSetAccelerator) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsCreatePhone)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsStartPhoneCall) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsStopPhoneCall) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsReleasePhone)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsSetBrowserPage)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsSetShock) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsDoShock)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsSetScreenLight) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetScreenLight)	
	i51KIT_STATIC_KIT_MAPING(i51AdeOsScreenAlwaysLight) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsSetTime) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetSeconds) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsSecond2Time) 
	i51KIT_STATIC_KIT_MAPING(i51AdeOsSleep)

	//	MMI
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiGetScreenScale)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiGetPanel)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerCreate)  
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerRelease)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerPlay)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerPause)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerResume)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerStop)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerSetVolume)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerGetVolume)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiRecorderStart)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiRecorderStop)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiRecoderPlay)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiRecorderPlayStop)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiSetPanelUsed)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiUpdateScreen)

	//	SOC
	i51KIT_STATIC_KIT_MAPING(i51AdeSocHtonl)
	i51KIT_STATIC_KIT_MAPING(i51AdeSocHtons) 
	i51KIT_STATIC_KIT_MAPING(i51AdeSocInetAton) 
	i51KIT_STATIC_KIT_MAPING(i51AdeSocGetHostByName) 
	i51KIT_STATIC_KIT_MAPING(i51AdeSocGet) 
	i51KIT_STATIC_KIT_MAPING(i51AdeSocConnect)
	i51KIT_STATIC_KIT_MAPING(i51AdeSocSend)
	i51KIT_STATIC_KIT_MAPING(i51AdeSocReceive)
	i51KIT_STATIC_KIT_MAPING(i51AdeSocClose) 
	i51KIT_STATIC_KIT_MAPING(i51AdeSocGetLinkLayerMode) 
	i51KIT_STATIC_KIT_MAPING(i51AdeSocGetLinkLayerStatus)
	i51KIT_STATIC_KIT_MAPING(i51AdeSocCreateLinkLayer)
	i51KIT_STATIC_KIT_MAPING(i51AdeSocOpenLinkLayer)
	i51KIT_STATIC_KIT_MAPING(i51AdeSocCloseLinkLayer)
	i51KIT_STATIC_KIT_MAPING(i51AdeSocReleaseLinkLayer)  

	//	SMS
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsGetSims)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsGetImsi) 
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsGetImei)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsGetCenter)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsCreateDevice)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsSend)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsReleaseDevice)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsCreateHook)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsSetHook)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsReleaseHook)

	//	STD
	i51KIT_STATIC_KIT_MAPING(i51AdeStdCrc32)
	i51KIT_STATIC_KIT_MAPING(i51AdeStdWSprintf) 
	i51KIT_STATIC_KIT_MAPING(i51AdeStdEntryEditor) 
	i51KIT_STATIC_KIT_MAPING(i51AdeStdAscii2Unicode) 
	i51KIT_STATIC_KIT_MAPING(i51AdeStdUnicode2Ascii) 
	i51KIT_STATIC_KIT_MAPING(i51AdeStdMemset16) 
	i51KIT_STATIC_KIT_MAPING(i51AdeStdMemcpy16)
	i51KIT_STATIC_KIT_MAPING(i51AdeStdWStrlen)
	i51KIT_STATIC_KIT_MAPING(i51AdeStdWStrcpy)
	i51KIT_STATIC_KIT_MAPING(i51AdeStdWStrcmp)
	i51KIT_STATIC_KIT_MAPING(i51AdeStdWStrcat)
	i51KIT_STATIC_KIT_MAPING(i51AdeStdUncompress) 

	//	CNF
	i51KIT_STATIC_KIT_MAPING(i51AdeCnfGetVersion)
	i51KIT_STATIC_KIT_MAPING(i51AdeCnfGetProjectId)
	i51KIT_STATIC_KIT_MAPING(i51AdeCnfGetCustomerId)
	i51KIT_STATIC_KIT_MAPING(i51AdeCnfGetPlatformId)
	i51KIT_STATIC_KIT_MAPING(i51AdeCnfGetEndian)
	
	//	KERNEL
	i51KIT_STATIC_KIT_MAPING(i51KernelLoadKit)
	i51KIT_STATIC_KIT_MAPING(i51KernelLoadSysKit)
	i51KIT_STATIC_KIT_MAPING(i51KernelLoadApp)
	i51KIT_STATIC_KIT_MAPING(i51KernelUnloadKit)
	i51KIT_STATIC_KIT_MAPING(i51KernelUnloadSysKit) 
	i51KIT_STATIC_KIT_MAPING(i51KernelMakeSnapshot)
	i51KIT_STATIC_KIT_MAPING(i51KernelDropSnapshot)
	i51KIT_STATIC_KIT_MAPING(i51KernelSetFPS)	
	i51KIT_STATIC_KIT_MAPING(i51KernelSendRequest)
	i51KIT_STATIC_KIT_MAPING(i51KernelSendMessage)	
	i51KIT_STATIC_KIT_MAPING(i51KernelGetApplicationId)
	i51KIT_STATIC_KIT_MAPING(i51KernelChangeStatus)
	i51KIT_STATIC_KIT_MAPING(i51KernelExtractPackage)
	i51KIT_STATIC_KIT_MAPING(i51KernelSerialNumberValid)
	i51KIT_STATIC_KIT_MAPING(i51KernelGenerateSerialNumber)
	i51KIT_STATIC_KIT_MAPING(i51KernelGenerateSerialNumberStop)
	i51KIT_STATIC_KIT_MAPING(i51KernelGetSerialNumber)
	i51KIT_STATIC_KIT_MAPING(i51KernelGetVersion)
	i51KIT_STATIC_KIT_MAPING(i51KernelGetBatchCode)
	i51KIT_STATIC_KIT_MAPING(i51KernelGetDiskFreeSpace)
	i51KIT_STATIC_KIT_MAPING(i51KernelInstallApplication)
	i51KIT_STATIC_KIT_MAPING(i51KernelInstallApplicationStop)
	i51KIT_STATIC_KIT_MAPING(i51KernelReinstallApplication)
	i51KIT_STATIC_KIT_MAPING(i51KernelReinstallApplicationStop)	
	i51KIT_STATIC_KIT_MAPING(i51KernelUninstallApplication)
	i51KIT_STATIC_KIT_MAPING(i51KernelGetApplicationInterface)
	i51KIT_STATIC_KIT_MAPING(i51KernelCheckAppCompleten)
	
	//	Sl-API
	i51KIT_STATIC_KIT_MAPING(i51SlDlCreate) 
	i51KIT_STATIC_KIT_MAPING(i51SlDlStop)
	i51KIT_STATIC_KIT_MAPING(i51SlDlStart)
	
	//	C STD
	i51KIT_STATIC_KIT_MAPING(memchr)
	i51KIT_STATIC_KIT_MAPING(memcmp)
	i51KIT_STATIC_KIT_MAPING(memcpy)
	i51KIT_STATIC_KIT_MAPING(memmove)  
	i51KIT_STATIC_KIT_MAPING(memset)
	i51KIT_STATIC_KIT_MAPING(strcat)
	i51KIT_STATIC_KIT_MAPING(strchr)
	i51KIT_STATIC_KIT_MAPING(strcmp)
	i51KIT_STATIC_KIT_MAPING(strcoll)
	i51KIT_STATIC_KIT_MAPING(strcpy)
	i51KIT_STATIC_KIT_MAPING(strcspn)
	i51KIT_STATIC_KIT_MAPING(strlen)
	i51KIT_STATIC_KIT_MAPING(strncat)
	i51KIT_STATIC_KIT_MAPING(strncmp)
	i51KIT_STATIC_KIT_MAPING(strncpy)
	i51KIT_STATIC_KIT_MAPING(strpbrk)
	i51KIT_STATIC_KIT_MAPING(strrchr) 
	i51KIT_STATIC_KIT_MAPING(strspn)
	i51KIT_STATIC_KIT_MAPING(strstr)
	i51KIT_STATIC_KIT_MAPING(strxfrm)
	i51KIT_STATIC_KIT_MAPING(rand)	
	i51KIT_STATIC_KIT_MAPING(srand)
	i51KIT_STATIC_KIT_MAPING(sprintf)

	//	MMI Extension 2011.12.28
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiGetTextAttr)
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiDrawText)

	//	Kernel Extension 2012.1.6
	i51KIT_STATIC_KIT_MAPING(i51KernelGeti51Version)

	//	Sms Extension 2012.3.3
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsGetPhoneBook)
	i51KIT_STATIC_KIT_MAPING(i51AdeSmsSendEx)

	//	Os Extension 2012.3.6
	i51KIT_STATIC_KIT_MAPING(i51AdeOsRunWapExplorer)

	//	Os Extension 2012.3.29
	i51KIT_STATIC_KIT_MAPING(i51AdeCnfGetType)

	//	Mmi Extension 2012.3.31
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiUpdateScreenEx)

	//	Sl-API Extension 2012.4.16
	i51KIT_STATIC_KIT_MAPING(i51SlDlAbleCreate)

	//	MMI Extension 2012.4.19
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiRecorderFormat)

	//	MMI Extension 2012.4.25
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiMediaPlayerFormat)

	//	MMI Extension 2012.7.6
	i51KIT_STATIC_KIT_MAPING(i51AdeMmiRecorderStartEx)
	
	
i51KIT_STATIC_KIT_MAPING_END

//akilae add since 2012.7.11
iU32 i51KitStaticCoreAdapter[] = {
	i51KIT_STATIC_KIT_MAPING(i51AdeOsPermanitBufferRead)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsPermanitBufferWrite)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileAModeEnable)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsFileAModeDisable)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetSysDiskSpace)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetSysDiskSpaceLeft)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetDiskSpace)
	i51KIT_STATIC_KIT_MAPING(i51AdeOsGetDiskSpaceLeft)
	i51KIT_STATIC_KIT_MAPING(i51KernelGeti51Path)
	i51KIT_STATIC_KIT_MAPING(i51AdeCnfGetCpu)
	i51KIT_STATIC_KIT_MAPING(atoi)
	i51KIT_STATIC_KIT_MAPING(i51MiniSlDlStart)
	i51KIT_STATIC_KIT_MAPING(i51MiniSlDlStop)
	i51KIT_STATIC_KIT_MAPING(i51KernelCheckKitCrc)
} ;
i51KIT_DYNAMIC_KIT_DEF(i51KitCore);
//akilae add finished


#ifdef i51PROFILINGS

#define EX_FONT_CHAR(value) (value)
#define EX_FONT_UNICODE_VAL(value) (value),
#define EX_FONT_ANSI_VAL(value) ,

struct {

	//	author : Jelo 
	//	12x12 字模数据
	 
	unsigned short value; 
	unsigned char data[14];

} AnsiFont7x14[94] = 
{
	{
		EX_FONT_CHAR('!')
			EX_FONT_ANSI_VAL(0x0021)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('\"')
			EX_FONT_ANSI_VAL(0x0022)
		{0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('#')
			EX_FONT_ANSI_VAL(0x0023)
		{0x00, 0x00, 0x00, 0x00, 0x14, 0x28, 0x7c, 0x28, 0x28, 0x7c, 0x28, 0x50, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('$')
			EX_FONT_ANSI_VAL(0x0024)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x40, 0x30, 0x08, 0x70, 0x20, 0x20, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('%')
			EX_FONT_ANSI_VAL(0x0025)
		{0x00, 0x00, 0x00, 0x00, 0x20, 0x50, 0x20, 0x78, 0x10, 0x28, 0x10, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('&')
			EX_FONT_ANSI_VAL(0x0026)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x20, 0x20, 0x58, 0x48, 0x3c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('\'')
			EX_FONT_ANSI_VAL(0x0027)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('(')
			EX_FONT_ANSI_VAL(0x0028)
		{0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(')')
			EX_FONT_ANSI_VAL(0x0029)
		{0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('*')
			EX_FONT_ANSI_VAL(0x002a)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x7c, 0x10, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('+')
			EX_FONT_ANSI_VAL(0x002b)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(',')
			EX_FONT_ANSI_VAL(0x002c)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x20, 0x00}
	},
	{
		EX_FONT_CHAR('-')
			EX_FONT_ANSI_VAL(0x002d)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('.')
			EX_FONT_ANSI_VAL(0x002e)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('/')
			EX_FONT_ANSI_VAL(0x002f)
		{0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('0')
			EX_FONT_ANSI_VAL(0x0030)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('1')
			EX_FONT_ANSI_VAL(0x0031)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('2')
			EX_FONT_ANSI_VAL(0x0032)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x08, 0x10, 0x20, 0x40, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('3')
			EX_FONT_ANSI_VAL(0x0033)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('4')
			EX_FONT_ANSI_VAL(0x0034)
		{0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0x28, 0x48, 0x7c, 0x08, 0x1c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('5')
			EX_FONT_ANSI_VAL(0x0035)
		{0x00, 0x00, 0x00, 0x00, 0x3c, 0x20, 0x20, 0x38, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('6')
			EX_FONT_ANSI_VAL(0x0036)
		{0x00, 0x00, 0x00, 0x00, 0x1c, 0x20, 0x40, 0x78, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('7')
			EX_FONT_ANSI_VAL(0x0037)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x44, 0x04, 0x08, 0x08, 0x10, 0x10, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('8')
			EX_FONT_ANSI_VAL(0x0038)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('9')
			EX_FONT_ANSI_VAL(0x0039)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x08, 0x70, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(':')
			EX_FONT_ANSI_VAL(0x003a)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(';')
			EX_FONT_ANSI_VAL(0x003b)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x30, 0x20, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('<')
			EX_FONT_ANSI_VAL(0x003c)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x60, 0x18, 0x04, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('=')
			EX_FONT_ANSI_VAL(0x003d)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('>')
			EX_FONT_ANSI_VAL(0x003e)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x30, 0x0c, 0x30, 0x40, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('?')
			EX_FONT_ANSI_VAL(0x003f)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x08, 0x10, 0x10, 0x00, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('@')
			EX_FONT_ANSI_VAL(0x0040)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x4c, 0x54, 0x54, 0x4c, 0x40, 0x3c, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('A')
			EX_FONT_ANSI_VAL(0x0041)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x28, 0x28, 0x38, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('B')
			EX_FONT_ANSI_VAL(0x0042)
		{0x00, 0x00, 0x00, 0x00, 0xf8, 0x44, 0x44, 0x78, 0x44, 0x44, 0xf8, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('C')
			EX_FONT_ANSI_VAL(0x0043)
		{0x00, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('D')
			EX_FONT_ANSI_VAL(0x0044)
		{0x00, 0x00, 0x00, 0x00, 0xf0, 0x48, 0x44, 0x44, 0x44, 0x48, 0xf0, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('E')
			EX_FONT_ANSI_VAL(0x0045)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x24, 0x28, 0x38, 0x28, 0x24, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('F')
			EX_FONT_ANSI_VAL(0x0046)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x24, 0x28, 0x38, 0x28, 0x20, 0x70, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('G')
			EX_FONT_ANSI_VAL(0x0047)
		{0x00, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x40, 0x40, 0x4e, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('H')
			EX_FONT_ANSI_VAL(0x0048)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x44, 0x7c, 0x44, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('I')
			EX_FONT_ANSI_VAL(0x0049)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('J')
			EX_FONT_ANSI_VAL(0x004a)
		{0x00, 0x00, 0x00, 0x00, 0x3c, 0x08, 0x08, 0x08, 0x48, 0x48, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('K')
			EX_FONT_ANSI_VAL(0x004b)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x48, 0x70, 0x48, 0x44, 0xe6, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('L')
			EX_FONT_ANSI_VAL(0x004c)
		{0x00, 0x00, 0x00, 0x00, 0x70, 0x20, 0x20, 0x20, 0x24, 0x24, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('M')
			EX_FONT_ANSI_VAL(0x004d)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x6c, 0x6c, 0x54, 0x44, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('N')
			EX_FONT_ANSI_VAL(0x004e)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x64, 0x64, 0x54, 0x54, 0x4c, 0xec, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('O')
			EX_FONT_ANSI_VAL(0x004f)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('P')
			EX_FONT_ANSI_VAL(0x0050)
		{0x00, 0x00, 0x00, 0x00, 0x78, 0x24, 0x24, 0x24, 0x38, 0x20, 0x70, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('Q')
			EX_FONT_ANSI_VAL(0x0051)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x1c, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('R')
			EX_FONT_ANSI_VAL(0x0052)
		{0x00, 0x00, 0x00, 0x00, 0x78, 0x24, 0x24, 0x24, 0x38, 0x24, 0x72, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('S')
			EX_FONT_ANSI_VAL(0x0053)
		{0x00, 0x00, 0x00, 0x00, 0x34, 0x4c, 0x40, 0x38, 0x04, 0x44, 0x78, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('T')
			EX_FONT_ANSI_VAL(0x0054)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x54, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('U')
			EX_FONT_ANSI_VAL(0x0055)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('V')
			EX_FONT_ANSI_VAL(0x0056)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x44, 0x48, 0x28, 0x28, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('W')
			EX_FONT_ANSI_VAL(0x0057)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x54, 0x54, 0x54, 0x54, 0x28, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('X')
			EX_FONT_ANSI_VAL(0x0058)
		{0x00, 0x00, 0x00, 0x00, 0xc6, 0x44, 0x28, 0x10, 0x28, 0x44, 0xc6, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('Y')
			EX_FONT_ANSI_VAL(0x0059)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x28, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('Z')
			EX_FONT_ANSI_VAL(0x005a)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x44, 0x08, 0x10, 0x20, 0x44, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('[')
			EX_FONT_ANSI_VAL(0x005b)
		{0x00, 0x00, 0x00, 0x00, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x18, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('\\')
			EX_FONT_ANSI_VAL(0x005c)
		{0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x20, 0x10, 0x10, 0x10, 0x08, 0x08, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(']')
			EX_FONT_ANSI_VAL(0x005d)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('^')
			EX_FONT_ANSI_VAL(0x005e)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('_')
			EX_FONT_ANSI_VAL(0x005f)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe}
	},
	{
		EX_FONT_CHAR('`')
			EX_FONT_ANSI_VAL(0x0060)
		{0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('a')
			EX_FONT_ANSI_VAL(0x0061)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x3c, 0x44, 0x7e, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('b')
			EX_FONT_ANSI_VAL(0x0062)
		{0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x58, 0x64, 0x44, 0x44, 0xf8, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('c')
			EX_FONT_ANSI_VAL(0x0063)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x40, 0x40, 0x3c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('d')
			EX_FONT_ANSI_VAL(0x0064)
		{0x00, 0x00, 0x00, 0x00, 0x0c, 0x04, 0x34, 0x4c, 0x44, 0x44, 0x3e, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('e')
			EX_FONT_ANSI_VAL(0x0065)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x7c, 0x40, 0x3c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('f')
			EX_FONT_ANSI_VAL(0x0066)
		{0x00, 0x00, 0x00, 0x00, 0x0c, 0x10, 0x3c, 0x10, 0x10, 0x10, 0x3c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('g')
			EX_FONT_ANSI_VAL(0x0067)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x4c, 0x44, 0x44, 0x3c, 0x04, 0x38, 0x00}
	},
	{
		EX_FONT_CHAR('h')
			EX_FONT_ANSI_VAL(0x0068)
		{0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x58, 0x64, 0x44, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('i')
			EX_FONT_ANSI_VAL(0x0069)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x70, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('j')
			EX_FONT_ANSI_VAL(0x006a)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x70, 0x00}
	},
	{
		EX_FONT_CHAR('k')
			EX_FONT_ANSI_VAL(0x006b)
		{0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x5c, 0x48, 0x70, 0x48, 0xdc, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('l')
			EX_FONT_ANSI_VAL(0x006c)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('m')
			EX_FONT_ANSI_VAL(0x006d)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x54, 0x54, 0x54, 0xfe, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('n')
			EX_FONT_ANSI_VAL(0x006e)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x64, 0x44, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('o')
			EX_FONT_ANSI_VAL(0x006f)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('p')
			EX_FONT_ANSI_VAL(0x0070)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x64, 0x44, 0x44, 0x78, 0x40, 0xe0, 0x00}
	},
	{
		EX_FONT_CHAR('q')
			EX_FONT_ANSI_VAL(0x0071)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x4c, 0x44, 0x44, 0x3c, 0x04, 0x0e, 0x00}
	},
	{
		EX_FONT_CHAR('r')
			EX_FONT_ANSI_VAL(0x0072)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x30, 0x20, 0x20, 0x78, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('s')
			EX_FONT_ANSI_VAL(0x0073)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x40, 0x38, 0x04, 0x78, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('t')
			EX_FONT_ANSI_VAL(0x0074)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xf8, 0x40, 0x40, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('u')
			EX_FONT_ANSI_VAL(0x0075)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x44, 0x44, 0x4c, 0x36, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('v')
			EX_FONT_ANSI_VAL(0x0076)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x48, 0x28, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('w')
			EX_FONT_ANSI_VAL(0x0077)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x54, 0x54, 0x28, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('x')
			EX_FONT_ANSI_VAL(0x0078)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x28, 0x10, 0x28, 0x6c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('y')
			EX_FONT_ANSI_VAL(0x0079)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x28, 0x28, 0x10, 0x10, 0x70, 0x00}
	},
	{
		EX_FONT_CHAR('z')
			EX_FONT_ANSI_VAL(0x007a)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x48, 0x10, 0x24, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('{')
			EX_FONT_ANSI_VAL(0x007b)
		{0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x10, 0x10, 0x30, 0x10, 0x10, 0x10, 0x08, 0x00}
	},
	{
		EX_FONT_CHAR('|')
			EX_FONT_ANSI_VAL(0x007c)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('}')
			EX_FONT_ANSI_VAL(0x007d)
		{0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x10, 0x10, 0x18, 0x10, 0x10, 0x10, 0x20, 0x00}
	},
	{
		EX_FONT_CHAR('~')
			EX_FONT_ANSI_VAL(0x007e)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00}
	}
};

static void CORENRDrawPoint ( 
	iU16* panel , 	
	iU16 panel_width ,
	iU16 panel_height ,
	iU32 desx ,
	iU32 desy ,
	iU16 color 
) { 

	if ( desx < 0 || desy < 0 ) return ;
	else if ( desx >= panel_width || desy >= panel_height ) return ;
	
	panel[desy*panel_width+desx] = color ;

}

static void CORENRDrawRect ( 
	iU16* panel , 	
	iU16 panel_width ,
	iU16 panel_height ,
	iU32 desx ,
	iU32 desy ,
	iU32 rect_w ,
	iU32 rect_h ,	
	iU16 color 
) { 

	iU32 x = desx ;
	iU32 y = desy ;
	
	iU32 xborder = x + rect_w ;
	iU32 yborder = y + rect_h ;

	for ( y = desy ; y < panel_height && y < yborder ; y ++ )
	{

		for ( x = desx ; x < panel_width && x < xborder ; x ++ )
		{
			CORENRDrawPoint ( panel , panel_width , panel_height , x , y , color ) ;
		}
	
	}

}

static void CORENRDrawTextModel  ( iS32 x , iS32 y , const iCHAR* words , iU16 color ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	iS32 font_walker = 0;
	iS32 rowwalker = 0;
	iS32 colwalker = 0;
	iS32 wdlen = 0;
	iU32* panel = 0 ;

	//	14x14 点阵
	# define MATRIXSCALE 14

	if ( !words ) return ;

	wdlen = (MATRIXSCALE) / 8 ;

	panel = i51AdeMmiGetPanel () ;
		
	for( font_walker = 0; font_walker < MATRIXSCALE; font_walker ++ ) {
		for( rowwalker = 0 ;rowwalker < wdlen; rowwalker ++ ) {
	
			for(colwalker = 0; colwalker < 8; colwalker ++ ) {
				if(words[font_walker * wdlen + rowwalker] & (0x80 >> colwalker)) {
					CORENRDrawPoint ( (iU16*)panel[1] , (iU16)panel[2], (iU16)panel[3] , x+rowwalker*8+colwalker , y+font_walker , color ) ;
				}
			} 
		}
	}
		
}

static void* CORENRSearchTextModel ( iCHAR element ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	iS32 walker = 0 ;	

	for ( walker = 0 ; walker < 94 ; walker ++ ) {
		if ( EX_FONT_CHAR ( element ) == AnsiFont7x14 [ walker ].value ) 
			return AnsiFont7x14 [ walker ].data ;
	}
	return 0 ;

}

void CORENRDrawText ( iS32 x , iS32 y , const iCHAR* text , iU16 color ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	iS32 walker = 0 ;
	
	static iU16 lcdwidth = 0 ;
	static iU16 lcdheight = 0 ;
	
	if ( 0 == lcdwidth || 0 == lcdheight ) {
		i51AdeMmiGetScreenScale ( &lcdwidth , &lcdheight ) ;
	}
	if ( !text ) return ;
	
	for ( ; text[walker] !='\0' && x < lcdwidth ; walker ++ ) {
		CORENRDrawTextModel ( x  , y , (char*)CORENRSearchTextModel( text[walker]) , color );
		x = x + 8 ;
	}

}

void i51KernelDrawProfiles () {

	//	author : Jelo 
	//	since : 2011.7.11
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	iCHAR buffer [ 128 ] = { "NULL" } ;

	iU32* panel = i51AdeMmiGetPanel () ; 
	
	static iU32 snap = 0 ; 
	static iU32 WarningTimes = 0 ;

	if ( iTRUE == i51Kernel.ShowProfilings ) {
			 
		CORENRDrawRect ( (iU16*)panel[1] , panel[2] , panel[3] , 0 , 0 , panel[2] , 28 , 100 ) ;
		
		sprintf ( buffer , "M:%d/T:%d/F:%d/S:%d/FPS:%d", 
			i51AdeOsGetTotallMemoryUsed () , 
			i51AdeOsTimerGetLive () ,
			i51AdeOsFileTotallOpened () ,
			i51AdeSocGetLive () ,	
			i51Kernel.FpsCounter 
			) ;

		CORENRDrawText ( 0 , 0 , buffer , 0x7e0 ) ;

		sprintf ( buffer , "SOBER:%d/KIT:%d/NET:%d", 
			i51Kernel.TotallSober ,
			i51Kernel.DapKitListTotall ,
			i51AdeSocGetRatio() ) ;		
		
		CORENRDrawText ( 0 , 14 , buffer , 0x7e0 ) ;		
			
	}

	if ( 0 < i51SlDSQueueGetTotall ( &i51Kernel.queue ) ) {
		
		sprintf ( buffer , "REQ:%d" , i51SlDSQueueGetTotall ( &i51Kernel.queue ) ) ; 

		//	printf REQ
		CORENRDrawRect ( (iU16*)panel[1] , panel[2] , panel[3] , 0 , 16 , panel[2] , 14 , 100 ) ;
		CORENRDrawText ( 0 , 16 , buffer , 0x7e0 ) ;
		
	}	
	
	//	对240*320的机器，内存使用量建议小于等于500KB
	if ( 153600 >= (panel[2]*panel[3] << 1) ) {
		
		if ( 512000 < i51AdeOsGetTotallMemoryUsed () ) {
			
			if ( 0 == (snap%15) && WarningTimes < 8 ) {

				CORENRDrawRect ( (iU16*)panel[1] , panel[2] , panel[3] , 0 , 86 , panel[2] , 14*3 , 0xf800 ) ;
				CORENRDrawText ( 10 , 86 , "Warning : " , 0 )  ;
				CORENRDrawText ( 10 , 100 , "Risk Level of Memory Using!!" , 0 ) ;
				CORENRDrawText ( 10 , 114 , "Less Than 500KB Recommend!!!" , 0 ) ;
				
				WarningTimes ++ ;
	  
			} else {

				WarningTimes = 0 ;
				snap ++ ;

			}
			
		}
		
	} else {
	//	其它分辨率，建议小于等于800KB

		if ( 819200 < i51AdeOsGetTotallMemoryUsed () ) {
			
			if ( 0 == (snap%15) && WarningTimes < 8 ) { 

				CORENRDrawRect ( (iU16*)panel[1] , panel[2] , panel[3] , 0 , 86 , panel[2] , 14*3 , 0xf800 ) ;
				CORENRDrawText ( 10 , 86 , "Warning : " , 0 )  ;
				CORENRDrawText ( 10 , 100 , "Risk Level of Memory Using!!" , 0 ) ;
				CORENRDrawText ( 10 , 114 , "Less Than 800KB Recommend!!!" , 0 ) ;
				
				WarningTimes ++ ;
	      
			} else {

				WarningTimes = 0 ;
				snap ++ ;

			}
			
		}

	}
	
		
}

#endif

//	William Add Start 2012.7.10
#ifdef __SPRD_CLEAR_CACHE__
static void i51KernelDump ( iU16* path , void* pool , iU32 length ) {

	//	author : Jelo 
	//	since : 2011.6.2
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	iU32 written = 0 ;
	iFILE file = i51AdeOsFileOpen ( path , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
		
	if ( !file ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : C DUMP F" ) ;		
		return ;
	} 

	i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileWrite ( file , pool , length , &written ) ;
	i51AdeOsFileFlush ( file ) ;
	i51AdeOsFileClose ( file ) ;
		

}
#endif
//	William Add Finish


static iBOOL i51KernelCreateFrontEnd () {

	//	author : Jelo 
	//	since : 2011.11.8
	// 	(C) PKIG Tech. Co., Ltd.	

	if ( iFALSE == i51Kernel.FrontEndEnable ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Create Front-End") ;	
		if ( iFALSE == i51PortCreateFrontEnd () ) {			
			i51AdeOsLog ( i51_LOG_SC , "KEL : Create Front-End Failed") ;	
			return iFALSE ;
		}
	}

	i51Kernel.FrontEndEnable = iTRUE ;
	i51Kernel.PauseCantSend = iFALSE ;	

	i51KernelLoadKitCore ((iU32)i51KernelCreateFrontEnd) ;
	
	return iTRUE ;
		
}

static iBOOL i51KernelReleaseFrontEnd () {

	//	author : Jelo 
	//	since : 2011.11.8
	// 	(C) PKIG Tech. Co., Ltd.	
	
	//	释放前台环境
	if ( iTRUE == i51Kernel.FrontEndEnable ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Release Front-End S") ;	
		i51PortReleaseFrontEnd () ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Release Front-End E") ;	
	}
	
	i51Kernel.FrontEndEnable = iFALSE ;		
	i51Kernel.PauseCantSend = iFALSE ;

	i51KernelUnloadKitCore((iU32)i51KernelCreateFrontEnd) ;
	return iTRUE ;
		
}

static void i51KernelTimerPositiveCallback ( iU32 id ) {

	//	author : Jelo
	//	since : 2011.10.15
	// 	(C) PKIG Tech. Co., Ltd.

	if ( 0 != KernelTimerCallback ) {
		KernelTimerCallback ( iTRUE ) ;
		KernelTimerCallback = 0 ;		
	}

	i51AdeOsTimerStop ( id ) ;

}

static void i51KernelTimerNegitiveCallback ( iU32 id )  {

	//	author : Jelo
	//	since : 2011.10.15
	// 	(C) PKIG Tech. Co., Ltd.
	
	if ( 0 != KernelTimerCallback ) {
		KernelTimerCallback ( iFALSE ) ;
		KernelTimerCallback = 0 ;
	}
	
	i51AdeOsTimerStop ( id ) ;

}

static void  i51AdeOsCreateCB ( iBOOL success ) {

	//	author : Jelo
	//	since : 2011.10.15
	// 	(C) PKIG Tech. Co., Ltd.
	
	if ( iFALSE == success ) {

		//	初始化失败
		i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerNegitiveCallback , 30 ) ;

		i51Kernel.InitStatus = i51_KERNEL_INIT_FAILED ;			
		i51AdeOsLog ( i51_LOG_SC , "KEL : Init Failed , At i51AdeOs" ) ;	
		
		return ;
		
	} 

	i51AdeMmiCreate ( i51AdeMmiCreateCB ) ;
	
}

static void  i51AdeMmiCreateCB ( iBOOL success ) {

	//	author : Jelo
	//	since : 2011.10.15
	// 	(C) PKIG Tech. Co., Ltd.

	if ( iFALSE == success ) {

		i51AdeOsRelease(); 
		
		//	初始化失败
		i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerNegitiveCallback , 30 ) ;

		i51Kernel.InitStatus = i51_KERNEL_INIT_FAILED ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Init Failed , At i51AdeMmi" ) ;	
		
		return ;
		
	} 

	i51AdeSocCreate ( i51AdeSocCreateCB ) ;

	
}
static void  i51AdeSocCreateCB( iBOOL success )  {

	//	author : Jelo
	//	since : 2011.10.15
	// 	(C) PKIG Tech. Co., Ltd.
	
	if ( iFALSE == success ) {
		
		i51AdeOsRelease(); 
		i51AdeMmiRelease(); 
		
		//	初始化失败
		i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerNegitiveCallback , 30 ) ;	

		i51Kernel.InitStatus = i51_KERNEL_INIT_FAILED ;			
		i51AdeOsLog ( i51_LOG_SC , "KEL : Init Failed , At i51AdeSoc" ) ;	
		
		return ;
		
	} 

	i51AdeSmsCreate ( i51AdeSmsCreateCB ) ;

	
}

static void  i51AdeSmsCreateCB( iBOOL success )  {

	//	author : Jelo
	//	since : 2011.10.15
	// 	(C) PKIG Tech. Co., Ltd.
	
	if ( iFALSE == success ) {

		i51AdeOsRelease(); 
		i51AdeMmiRelease(); 
		i51AdeSocRelease(); 
		
		//	初始化失败
		i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerNegitiveCallback , 30 ) ;	

		i51Kernel.InitStatus = i51_KERNEL_INIT_FAILED ;			
		i51AdeOsLog ( i51_LOG_SC , "KEL : Init Failed , At i51AdeSms" ) ;	
		
		return ;  
		
	} 

	i51AdeStdCreate ( i51AdeStdCreateCB ) ;
	
}

static void  i51AdeStdCreateCB( iBOOL success )  {

	//	author : Jelo
	//	since : 2011.10.15
	// 	(C) PKIG Tech. Co., Ltd.

	iS32 ret = 0 ;
	
	if ( iFALSE == success ) {

		i51AdeOsRelease(); 
		i51AdeMmiRelease(); 
		i51AdeSocRelease(); 
		i51AdeSmsRelease(); 
		
		//	初始化失败
		i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerNegitiveCallback , 30 ) ;

		i51Kernel.InitStatus = i51_KERNEL_INIT_FAILED ;			
		i51AdeOsLog ( i51_LOG_SC , "KEL : Init Failed , Ade F" ) ;	
		
		return ;
		
	} 

	//	Jelo Added Since 20120229
	i51KernelCopy2MovableDisk () ;
	//	Jelo Added Finished
	
	if ( iFALSE == i51KernelCreateDir () ) {
		i51Kernel.InitStatus = i51_KERNEL_INIT_FAILED ;
		//	初始化失败
		i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerNegitiveCallback , 30 ) ;	
		i51AdeOsLog ( i51_LOG_SC , "KEL : Init Failed , Dir F" ) ;		
		return ;
	}
	
	//	Jelo Add Since 2012.6.8
	#ifndef i51_KERNEL_STATIC_MEMORY
	i51KernelStaticMemory = i51AdeOsMalloc ( sizeof(iU8) * i51_KERNEL_STATIC_MEMORY_SIZE , __FILE__ , __LINE__ ) ;
	if ( 0 == i51KernelStaticMemory ) {
		i51Kernel.InitStatus = i51_KERNEL_INIT_FAILED ;
		//	初始化失败
		i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerNegitiveCallback , 30 ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Init Failed , Mem F" ) ;
		return ;		
	}
	#endif
	//	Jelo Add Finished
	
	//	初始化成功
	i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerPositiveCallback , 30 ) ;	

	i51Kernel.InitStatus = i51_KERNEL_INIT_SUCCEED ;
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Init Succeed" ) ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Load i51KitServer" ) ;
	 
	//	加载系统服务
	ret = (iS32 ) i51KernelLoadSysKit ( L"i51KitServer.kit" ) ;

	if ( 0 != ret ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Load i51KitServer Succeed" ) ;
	} else {	
		i51AdeOsLog ( i51_LOG_SC , "KEL : Load i51KitServer Failed" ) ;	
	}
			
	
}

static void i51KernelSetCurrentSorber () {
	
	//	author : Jelo	
	//	since : 2011.10.21
	//	(C) PKIG Tech. Co., Ltd.

	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {

		if ( i51_APP_STATUS_SOBER == i51Kernel.DapKitList[looper].status ) {
			memcpy ( &i51Kernel.CurrentSoberKit  , &i51Kernel.DapKitList[looper] , sizeof(i51Kernel.CurrentSoberKit) ) ;
			return ;
		}
		
	}
	
	//	说明没有找到sober
	memset ( &i51Kernel.CurrentSoberKit , 0 , sizeof(i51Kernel.CurrentSoberKit) ) ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : NO Sober Exist , Stop Kernel Timer" ) ;	
	i51KernelStopTimer ( iFALSE ) ;
			
	return ;	
	
}

static iS32 i51KernelSendSoberKitMsg ( iU32 message , void* parameter ) {


	//	author : Jelo	
	//	since : 2011.8.25
	//	(C) PKIG Tech. Co., Ltd.

	//	向Sober  发送消息

	iU32 looper = 0 ;
	
	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {

		if ( i51_APP_STATUS_SOBER == i51Kernel.DapKitList[looper].status ) {

			//	避免PAUSE消息重复发送
			if ( i51_MSG_PAUSE == message ) {
				if ( iTRUE == i51Kernel.DapKitList[looper].DontRevPause ) {
					return iTRUE ;
				}
				i51Kernel.DapKitList[looper].DontRevPause = iTRUE ;
			} else if ( i51_MSG_RESUME == message ) {
				i51Kernel.DapKitList[looper].DontRevPause = iFALSE ;
			}			
			
			i51Kernel.DapKitList[looper].i51KitMain (  i51Kernel.DapKitList[looper].id , message , parameter ) ;
			
		}
		
	}	

	return iTRUE ;
	
}

static iS32 i51KernelSendDapKitMsgEx ( iU32 message , void* parameter ) {


	//	author : Jelo	
	//	since : 2011.10.20
	//	(C) PKIG Tech. Co., Ltd.

	//	广播消息

	iU32 looper = 0 ;
	
	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {

		//	避免PAUSE消息重复发送
		if ( i51_MSG_PAUSE == message ) {
			if ( iTRUE == i51Kernel.DapKitList[looper].DontRevPause ) {
				return iTRUE ;
			}
			i51Kernel.DapKitList[looper].DontRevPause = iTRUE ;
		} else if ( i51_MSG_RESUME == message ) {
			i51Kernel.DapKitList[looper].DontRevPause = iFALSE ;
		}			
			
		i51Kernel.DapKitList[looper].i51KitMain ( i51Kernel.DapKitList[looper].id , message , parameter ) ;
		
	}	

	return iTRUE ;
	
	
}


static iS32 i51KernelSendSapKitMsg ( iU32 message , void* parameter ) {


	//	author : Jelo	
	//	since : 2011.11.15
	//	(C) PKIG Tech. Co., Ltd.

	//	广播消息
	
	iU32 looper = 0 ;
	
	for ( looper = 0 ; looper < i51Kernel.SapKitListTotall ; looper ++ ) {

		//	避免PAUSE消息重复发送
		if ( i51_MSG_PAUSE == message ) {
			if ( iTRUE == i51Kernel.SapKitList[looper].DontRevPause ) {
				return iTRUE ;
			}
			i51Kernel.SapKitList[looper].DontRevPause = iTRUE ;
		} else if ( i51_MSG_RESUME == message ) {
			i51Kernel.SapKitList[looper].DontRevPause = iFALSE ;
		}
		
		i51Kernel.SapKitList[looper].i51KitMain ( i51Kernel.SapKitList[looper].id , message , parameter ) ;
		
	}

	return iTRUE ;
	
	
}

static iS32 i51KernelSendKitMsg ( iU32 id , iU32 message , void* parameter ) {


	//	author : Jelo	
	//	since : 2011.10.20
	//	(C) PKIG Tech. Co., Ltd.

	//	非广播消息

	iU32 looper = 0 ;
	
	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {

		if ( id == i51Kernel.DapKitList[looper].id ) {

			//	避免PAUSE消息重复发送
			if ( i51_MSG_PAUSE == message ) {
				if ( iTRUE == i51Kernel.DapKitList[looper].DontRevPause ) {
					return iTRUE ;
				}
				i51Kernel.DapKitList[looper].DontRevPause = iTRUE ;
			} else if ( i51_MSG_RESUME == message ) {
				i51Kernel.DapKitList[looper].DontRevPause = iFALSE ;
			}
			
			i51Kernel.DapKitList[looper].i51KitMain ( i51Kernel.DapKitList[looper].id , message , parameter ) ;
			
			return iTRUE ;
			
		}
		
	}	

	return iTRUE ;
	
}

static iS32 i51KernelExitApp () {


	//	author : Jelo	
	//	since : 2011.12.12
	//	(C) PKIG Tech. Co., Ltd.

	//	强制退出i51-APP
	
	iU32 looper = 0 ;
	iU32 totall = i51Kernel.DapKitListTotall ;
	iU32 counter = 0 ;
	iU32 appid [ 32 ] = { 0 } ;

	//	给所有DAP 发送暂停消息
	i51KernelSendDapKitMsgEx ( i51_MSG_PAUSE , 0 ) ;

	for ( looper = 0 ; looper < totall ; looper ++ ) {
		if ( iFALSE == i51Kernel.DapKitList[looper].lib ) {
			appid [ counter ] = i51Kernel.DapKitList[looper].id ;
			counter ++ ;
		}
	}
	
	for ( looper = 0 ; looper < counter ; looper ++ ) {
	//	i51KernelSendKitMsg ( appid [ looper ] , i51_MSG_EXIT , 0 ) ;
	//	i51SlLoaderUnload ( appid [ looper ] ) ;
		//i51KernelSendRequest ( appid [ looper ] , 0 , i51_REQ_EXIT , 0 , 0 ) ;
		//	发送暂停消息
		i51KernelSendKitMsg ( appid [ looper ] , i51_MSG_PAUSE , 0 ) ; 
		//	退出应用
		i51KernelKitListDropKit ( (void*)appid [ looper ] ) ;
	
	}

//	i51Kernel.DapKitListTotall = 0 ;
//	if ( 0 != i51Kernel.DapKitList ) {
//		i51AdeOsFree ( i51Kernel.DapKitList ) ;
//		i51Kernel.DapKitList = 0 ;
//	}

	return iTRUE ;
	
	
}

static void i51KernelRunTimerLogical ( iU32 parameter ) {

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.

	#ifdef i51PROFILINGS
	static iU32 lastticks = 0 ;
	#endif

	i51MemoryOverflowCheck () ;
	
	if ( 0 == i51Kernel.TotallSober ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : No Sober Exist , Stop Timer At TL" ) ;
		i51KernelStopTimer ( iFALSE  ) ;
		return ;
	}   

	#ifdef i51PROFILINGS
	{
		iS32 gap = (i51AdeOsGetTick () - lastticks) ;

		if ( 0 < gap ) {	
			i51Kernel.FpsCounter = 1000 / gap ;
		}
		
		lastticks = i51AdeOsGetTick () ;
	}
	#endif

	//	only one sober exist
	if ( 1 == i51Kernel.TotallSober ) {	
		i51Kernel.CurrentSoberKit.i51KitMain ( i51Kernel.CurrentSoberKit.id , i51_MSG_PAINT , 0 ) ;
	}  else {
		i51KernelSendSoberKitMsg ( i51_MSG_PAINT , 0 ) ;
	}

}

static iBOOL i51KernelCreateTimer () {

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.

	i51AdeOsLog ( i51_LOG_SC , "KEL : Create Timer S" ) ;	
	 
	if ( 0 != i51Kernel.Timer )  {
		i51AdeOsTimerStop ( i51Kernel.Timer ) ;
		i51Kernel.Timer = 0 ;
	}
	
	i51Kernel.Timer = i51AdeOsTimerCreate ( iTRUE , 0 ) ;

	if ( 0 == i51Kernel.Timer ) {	
		i51AdeOsLog ( i51_LOG_SC , "KEL : Create Timer F" ) ;	
		return iFALSE ;
	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : Create Timer E" ) ;	
	
	return iTRUE ;
	
}

static iBOOL i51KernelStartTimer ( iU32 time , void (*callback) ( iU32 parameter ) ) {

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.

	//	启动kernel timer，用于发送i51_MSG_PAINT消息
	
	iBOOL ret = iFALSE ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Start Timer S , 0x%x , 0x%x" , time , callback ) ;	
	
	ret = i51KernelCreateTimer () ;

	if ( iFALSE == ret ) {
		return ret ;
	}

	//	先创建前台环境
	i51KernelCreateFrontEnd () ;

	ret = i51AdeOsTimerStart ( i51Kernel.Timer , callback , time ) ;
	
	if ( iTRUE == ret ) {
		i51Kernel.TimerRuning = iTRUE ;
	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : Start Timer E , ret 0x%x" , ret ) ;	
	
	return ret ;

} 

static iBOOL i51KernelStopTimer ( iBOOL release_fe ) {

	//	author : Jelo 
	//	since : 2011.8.25
	// 	(C) PKIG Tech. Co., Ltd.

	//	是否释放前台环境

	iBOOL ret = iFALSE ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Stop Timer S , 0x%x , 0x%x" , i51Kernel.Timer , release_fe ) ;	
	
	if ( 0 == i51Kernel.Timer ) {
		return iTRUE ;
	}
	
	i51Kernel.TimerRuning = iFALSE ;
	
	ret = i51AdeOsTimerStop ( i51Kernel.Timer ) ;

	if ( iTRUE == ret ) {
		i51Kernel.TimerRuning = iFALSE ;
		i51Kernel.Timer = 0 ;
	}

 	if ( iTRUE == release_fe ) {		
		i51KernelReleaseFrontEnd () ;	
 	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : Stop Timer E , ret 0x%x" , ret ) ;	
	
	return ret ;

}

static iBOOL i51KernelCreateDir () {

	//	author : Jelo	
	//	since : 2011.4.28
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : 创建运行目录

	iU32 looper = 0 ;
	iCHAR absolute [64] = { 0 } ;
	iU16 abw [64] = { 0 } ;

	iCHAR* type1dir [] = { "i51" , "i51\\i51KIT" , "i51\\i51SYS" , 0 } ;

#ifdef i51SIMULATOR
	CreateDirectoryW ( L"i51" , 0 ) ;
	CreateDirectoryW ( L"i51\\i51KIT" , 0 ) ;
	CreateDirectoryW ( L"i51\\i51SYS" , 0 ) ;
#endif

	i51AdeOsFileAModeEnable () ;
	
	for ( looper = 0 ; looper < 3 ; looper ++ ) {

		memset ( absolute , 0 , sizeof(absolute) ) ;
		memset ( abw , 0 , sizeof(abw) ) ;

		if ( i51_KERNEL_DISK_MODE_SYSTEM == i51Kernel.DiskMode ) {		
			sprintf ( (iCHAR*)absolute , "%s%s" , i51AdeOsGetPathSymbol (), type1dir[looper] );
		} else {
			sprintf ( (iCHAR*)absolute , "%s%s" , i51AdeOsGetPathSymbolEx () , type1dir[looper] );
		}

		i51AdeStdAscii2Unicode ( absolute , abw ) ;

		if ( 1 == i51AdeOsFileIsDirExist ( abw ) ) 
			continue ;
		
		if ( 0 < i51AdeOsFileCreateDir ( abw ) ) {			
			i51AdeOsFileSetAttr ( abw , i51_OS_FL_ATTR_DIR | i51_OS_FL_ATTR_HID ) ;			
		} else {
			i51AdeOsLog ( i51_LOG_SC , "KEL : Create Dir F" ) ;	
			goto i51DIR_ERROR ;
		}
			 
	}

	i51AdeOsFileAModeDisable () ;
	
	return iTRUE ;	

i51DIR_ERROR:	

	i51AdeOsFileAModeDisable () ;

	return iFALSE ;
}

static i51KERNELKIT* i51KernelKitListGetKit ( iU16* name ) {

	//	author : Jelo 
	//	since : 2011.11.5
	// 	(C) PKIG Tech. Co., Ltd.

	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {

		if ( 0 == i51AdeStdWStrcmp ( name , i51Kernel.DapKitList[looper].name) ) {
			return &i51Kernel.DapKitList[looper] ;
		}
		
	}	

	return 0 ;
	
	
}

static void* i51KernelKitListCreateKit ( 

	iU16* name ,
	i51KITMAIN i51kitmain , 
	i51KITPORT i51KitPort ,
	iHANDLE kithandle , 
	iU8 status ,
	iBOOL lib ,
	iU32 scope ,
	iU32 fps 
	
) {

	//	author : Jelo 
	//	since : 2011.9.7
	// 	(C) PKIG Tech. Co., Ltd.

	i51KERNELKIT* i51Kit = 0 ;
	
	iBOOL ret = iFALSE ;

	if ( i51_KERNEL_KIT_SCOPE_DYNAMIC == scope ) {

		if ( i51Kernel.DapKitListTotall >= i51_KERNEL_DAP_KIT_LIST_MAX ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : Too Many Dap Kits Here" ) ;
			return 0 ;
		}

		if ( 0 == i51Kernel.DapKitList ) {
			i51Kernel.DapKitList = (i51KERNELKIT* ) i51MALLOC ( sizeof(i51KERNELKIT) * i51_KERNEL_DAP_KIT_LIST_MAX ) ;
			if ( 0 == i51Kernel.DapKitList ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Not Enough Mem for DAP" ) ;
				return 0 ;
			} 
			memset ( i51Kernel.DapKitList , 0 , sizeof(i51KERNELKIT) * i51_KERNEL_DAP_KIT_LIST_MAX ) ;
		}  
		
	} else if ( i51_KERNEL_KIT_SCOPE_STATIC == scope ) {

		if ( i51Kernel.SapKitListTotall >= i51_KERNEL_SAP_KIT_LIST_MAX ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : Too Many Sap Kits Here" ) ;
			return 0 ;
		}

	}
	
	//	加载到动态存储区域的套件 
	if ( i51_KERNEL_KIT_SCOPE_DYNAMIC == scope ) {

		i51AdeStdWStrcpy ( i51Kernel.DapKitList[i51Kernel.DapKitListTotall].name , name ) ;
		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].id = (iU32)kithandle ;
		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].status = status ;
		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].i51KitMain = i51kitmain ;
		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].i51KitPort = i51KitPort ;
		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].lib = lib ;
		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].scope = scope ;
//		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].fps = fps ;
		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].ldtimes = 1 ;
		i51Kernel.DapKitList[i51Kernel.DapKitListTotall].DontRevPause = iFALSE ;

		i51Kit = &i51Kernel.DapKitList[i51Kernel.DapKitListTotall] ;

		i51Kernel.DapKitListTotall ++ ; 
		
	} else {

		i51AdeStdWStrcpy ( i51Kernel.SapKitList[i51Kernel.SapKitListTotall].name , name ) ;
		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].id = (iU32)kithandle ;
		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].status = status ;
		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].i51KitMain = i51kitmain ;
		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].i51KitPort = i51KitPort ;
		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].lib = lib ;
		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].scope = scope ;
//		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].fps = fps ;
		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].ldtimes = 1 ; 
		i51Kernel.SapKitList[i51Kernel.SapKitListTotall].DontRevPause = iFALSE ;		

		i51Kit = &i51Kernel.SapKitList[i51Kernel.SapKitListTotall] ;
 
		i51Kernel.SapKitListTotall ++ ;
		
	}

	i51KernelSetCurrentSorber () ;

	if ( 0 != i51Kit && i51_APP_STATUS_SOBER == i51Kit->status ) {
		i51Kernel.TotallSober ++ ;
	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : Totall Sober %d" , i51Kernel.TotallSober ) ;

	return (void* ) i51Kit ;
  
}

static iBOOL i51KernelKitListDropKit ( iHANDLE kithandle ) {

	//	author : Jelo 
	//	since : 2011.10.20
	// 	(C) PKIG Tech. Co., Ltd.
	
	iU32 status = 0 ;
	iU32 looper = 0 ;
	iU32 walker = 0 ;

	i51KITMAIN i51KitMain = 0 ;
	
	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelKitListDropKit S , Dap Totall %d" , i51Kernel.DapKitListTotall ) ;
	#endif

	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {

		if ( (iU32) kithandle == i51Kernel.DapKitList[looper].id ) {

			//	注意: 此处可能会产生递归调用
			//	因此DAP列表的移动要在通知KIT退出前处理。
			
			i51KernelSendKitMsg ( (iU32 )kithandle , i51_MSG_PAUSE , 0 ) ;

			i51KitMain = i51Kernel.DapKitList[looper].i51KitMain ;
			status = i51Kernel.DapKitList[looper].status ;

			#ifdef i51KERNEL_DEBUG	
			{
				iCHAR name [32] = { 0 } ;
				i51AdeStdUnicode2Ascii ( i51Kernel.DapKitList[looper].name , name ) ;
				i51AdeOsLog ( 0 , "Drop Kit '%s'" , name ) ;
			}
			#endif

			
			i51Kernel.DapKitListTotall -- ;
			
			//	move
			for ( walker = looper ; walker < i51Kernel.DapKitListTotall ; walker ++ ) {

				#if 0
				i51AdeStdWStrcpy ( i51Kernel.DapKitList[walker].name , i51Kernel.DapKitList[walker+1].name ) ;
				i51Kernel.DapKitList[walker].id = i51Kernel.DapKitList[walker+1].id ;
				i51Kernel.DapKitList[walker].status = i51Kernel.DapKitList[walker+1].status ;
				i51Kernel.DapKitList[walker].lib = i51Kernel.DapKitList[walker+1].lib ;
				i51Kernel.DapKitList[walker].scope = i51Kernel.DapKitList[walker+1].scope ;
				i51Kernel.DapKitList[walker].fps = i51Kernel.DapKitList[walker+1].fps ;
				i51Kernel.DapKitList[walker].ldtimes = i51Kernel.DapKitList[walker+1].ldtimes ;		
				i51Kernel.DapKitList[walker].i51KitMain = i51Kernel.DapKitList[walker+1].i51KitMain ;
				i51Kernel.DapKitList[walker].i51KitPort = i51Kernel.DapKitList[walker+1].i51KitPort ;
				#endif 
				
				memcpy ( &i51Kernel.DapKitList[walker] , &i51Kernel.DapKitList[walker+1] , sizeof(i51KERNELKIT) ) ;
				
			}	

			//	如果是Sober ，则更新Sober 统计信息
			if ( i51_APP_STATUS_SOBER == status ) {
				
				i51KernelSetCurrentSorber () ;
				i51Kernel.TotallSober -- ;
	
			}
			
			i51KitMain ( (iU32 )kithandle , i51_MSG_EXIT , 0 ) ;

			i51SlLoaderUnload ( (iS32) kithandle ) ;

			break ;
			 
		}
		
	}	 

	if ( 0 == i51Kernel.DapKitListTotall && 0 != i51Kernel.DapKitList ) {
		i51AdeOsFree ( i51Kernel.DapKitList ) ;
		i51Kernel.DapKitList = 0 ;
	}

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelKitListDropKit E" ) ;
	#endif
	
	return iTRUE ;
	
}

static iBOOL i51KernelCallSapAppMenu(iU16 *pAppName)
{
	//	author : akilae 
	//	since : 2012.07.16
	// 	(C) PKIG Tech. Co., Ltd.
	
	//	notes : 静态加载AppMenu 去下载
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
	param.gprsprompt = 0;
	
	ret = i51KernelRunStaticApp ( L"Appmenu" , (unsigned char* )i51SapGetAppmenu () , (unsigned int )i51SapGetAppmenuSize () , &param, 0 , 1 ) ;
	//akilae add since 2012.7.20	
	i51KitMain = (i51KITMAIN ) i51SlLoaderGetSymbolEx ( L"Appmenu" , "i51KitMain" ) ;
	if(NULL == i51KitMain){
	i51AdeOsLog(0, "KEL : Get i51KitMain F");
	return iFALSE ;
	}
	i51KitMain( 0, 0x00FFFFFF , (void*)i51KitStaticCoreAdapter );
	return iTRUE ;
}

static void i51KernelDispatch ( iU32 timer ) {

	//	author : Jelo 
	//	since : 2011.10.10
	// 	(C) PKIG Tech. Co., Ltd.

	//	notes : 消息分发

	#ifdef i51KERNEL_DEBUG	
		iU32 stime = 0 ;
		iU32 etime = 0 ;
	#endif
	
	iBOOL ret = iFALSE ;
	i51KERNELREQ* req = 0 ;

 	i51AdeOsLog ( i51_LOG_SC , "KEL : Dispatched S , Mt : %d , St : %d" , i51SlDSQueueGetTotall ( &i51Kernel.queue ) , i51Kernel.Pause ) ; 
 
	if ( iTRUE == i51SlDSQueueIsEmpty (&i51Kernel.queue) ) {

		i51AdeOsTimerStop ( i51Kernel.ReqTimer ) ;
		i51Kernel.ReqTimerRuning = iFALSE ;

		if ( 0 == i51Kernel.DapKitListTotall ) {
			i51KernelReleaseFrontEnd () ;
			i51MemoryLeakCheck () ; 
			//	Jelo Edit Since 2012.12.6
			//	i51Kernel.NotCreateRefTable = iFALSE ; 
			//	Jelo Edit Finished
		}

		i51AdeOsLog ( i51_LOG_SC , "KEL : MSG-Queue Empty" ) ;
		
		return ;
	 
	}

	#ifdef i51KERNEL_DEBUG	
		stime = i51AdeOsGetTick () ;
	#endif	

	ret = i51SlDSQueueOut ( &i51Kernel.queue , (iHANDLE)&req ) ;

	if ( iFALSE == ret || 0 == req ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : E , Queue Out F" ) ;
		i51AdeOsTimerStart ( i51Kernel.ReqTimer , i51KernelDispatch , 10 ) ;  
		return ;
	}

	if ( iTRUE == i51Kernel.Pause ) {
		if ( i51_REQ_LOADEX == req->request ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : Loadex Skiped" ) ;
			i51SlDSQueueIn ( &i51Kernel.queue , (iHANDLE)req ) ;
			i51AdeOsTimerStart ( i51Kernel.ReqTimer , i51KernelDispatch , 10 ) ;  	
			return ;
		}
			
	}
		
	#ifdef i51KERNEL_DEBUG	
	{
		iCHAR name [32] = { 0 } ;
		iU32 looper = 0 ;

		//	req to kernel 
		//	REQ is App sending to kernel 
		if ( 0 == req->object ) {
			
			for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {
				if (req->sender == i51Kernel.DapKitList[looper].id ) {
					i51AdeStdUnicode2Ascii ( i51Kernel.DapKitList[looper].name , name ) ;
					i51AdeOsLog ( 0 , "KEL : Req Sender '%s'" , name ) ;
				}
			} 			
			i51AdeOsLog ( 0 , "KEL : Req Object 'i51 Kernel'" ) ;
			i51AdeOsLog ( 0 , "KEL : Req Value 0x%x" , req->request ) ;
			
			
		} else {

		//	MSG is App sending to App
		
			for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {
				if (req->sender == i51Kernel.DapKitList[looper].id ) {
					i51AdeStdUnicode2Ascii ( i51Kernel.DapKitList[looper].name , name ) ;
					i51AdeOsLog ( 0 , "KEL : Msg Sender '%s'" , name ) ;
				}
			} 	 
			
			for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {
				if (req->object == i51Kernel.DapKitList[looper].id ) {
					i51AdeStdUnicode2Ascii ( i51Kernel.DapKitList[looper].name , name ) ;
					i51AdeOsLog ( 0 , "KEL : Msg Object '%s'" , name ) ;
				}
			} 
			i51AdeOsLog ( 0 , "KEL : Msg Value 0x%x" , req->request ) ;  

		}
		
	} 
	#endif
		
	//	request to kernel
	if ( 0 == req->object ) {	
		
		if ( i51_REQ_LOADEX == req->request ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : LOADEX S , 0x%x" , req->sender ) ; 

			//	发送暂停消息
			i51KernelSendKitMsg ( req->sender , i51_MSG_PAUSE , 0 ) ; 			
  			i51KernelKitListDropKit ( (iHANDLE)req->sender ) ;

			//	检测完整性
			if ( iFALSE == i51KernelCheckAppCompleten ( (iU16* ) req->name ) ) {

				//extern const iU8* i51SapGeti51Installer () ;
				//extern const iS32 i51SapGeti51InstallerSize () ;

				i51AdeOsLog ( i51_LOG_SC , "KEL : App COMP F" ) ; 
				i51AdeOsLog ( i51_LOG_SC , "KEL : Installer S" ) ; 
			
			//	不完整则安装
			#if 0
				ret = i51KernelRunStaticApp ( L"i51Installer" , (iU8* ) i51SapGeti51Installer () , (iU32 )i51SapGeti51InstallerSize () ,  (void* ) req->name , 0 , iTRUE ) ;
			#else
				ret = i51KernelCallSapAppMenu(req->name);		//akilae add 2012.7.16
			#endif
			
				i51AdeOsLog ( i51_LOG_SC , "KEL : Installer E , 0x%x" , ret ) ;  
			
			} else {

				//	完整则加载
				ret = i51KernelRunDynamicApp ( (iU16* ) req->name , req->buffer ) ;  

				if ( 0 == ret ) { 
					i51AdeOsLog ( i51_LOG_SC , "KEL : LOADEX F" ) ; 
					i51KernelSetCurrentSorber () ;  
				}

			}

			i51AdeOsLog ( i51_LOG_SC , "KEL : LOADEX E" ) ; 
						
		} else if ( i51_REQ_EXIT == req->request ) {

			//	发送暂停消息
			i51KernelSendKitMsg ( req->sender , i51_MSG_PAUSE , 0 ) ; 
			//	退出应用
			i51KernelKitListDropKit ( (iHANDLE)req->sender ) ;
			
			//	释放前台环境 
			if ( 0 == i51Kernel.TotallSober ) {
				i51KernelReleaseFrontEnd () ;	
				//akilae add since 2012.7.12
				//i51KIT_DYNAMIC_KIT_UNLOAD(i51KitCore);
				i51MiniSlDlStop(0);
				//i51AdeOsLog(0, "KEL: unload i51KitCore 2");
				//akilae add finished

			}
			
			#ifdef i51KERNEL_DEBUG	
			i51AdeOsLog ( i51_LOG_SC , "KEL : Totall Kits %d" , i51Kernel.DapKitListTotall ) ;
			#endif
		
		} 
		//	Jelo Add Start 2012.4.13
		else if ( i51_REQ_INSTALLED == req->request ) {

			extern iBOOL i51PortMain ( iS32 aid , iU32 message , void* parameter ) ;
			//	通知Port 上层应用安装了一个i51应用程序。
			//	此时Port 根据此实现自己定义的逻辑。
			i51AdeOsLog ( i51_LOG_SC , "KEL : REQ-Installed S" ) ;
			i51PortMain ( -1 , req->request , req->buffer ) ;
			i51AdeOsLog ( i51_LOG_SC , "KEL : REQ-Installed E" ) ;
			
		} else if ( i51_REQ_UNINSTALLED == req->request ) {

			extern iBOOL i51PortMain ( iS32 aid , iU32 message , void* parameter ) ;
			//	通知Port 上层应用卸载了一个i51应用程序。
			//	此时Port 根据此实现自己定义的逻辑。
			i51AdeOsLog ( i51_LOG_SC , "KEL : REQ-Uninstalled S" ) ;
			i51PortMain ( -1 , req->request , req->buffer ) ;
			i51AdeOsLog ( i51_LOG_SC , "KEL : REQ-Uninstalled E" ) ;  
			
		}
		//	Jelo Add Finished 
		
		#ifdef i51KERNEL_DEBUG	
			etime = i51AdeOsGetTick () ;
			i51AdeOsLog ( i51_LOG_SC , "KEL : REQ Dispatched , Cost %d" , etime-stime ) ; 
		#else 	
			i51AdeOsLog ( i51_LOG_SC , "KEL : REQ Dispatched" ) ; 
		#endif
			 
	} else {
	//	message to app

		i51KernelSendKitMsg ( req->object , req->request , req->buffer ) ;

		#ifdef i51KERNEL_DEBUG	
			etime = i51AdeOsGetTick () ;
			i51AdeOsLog ( i51_LOG_SC , "KEL : REQ Dispatched , Cost %d" , etime-stime ) ; 
		#else 	
			i51AdeOsLog ( i51_LOG_SC , "KEL : MSG Dispatched" ) ;
		#endif
		
	}

	if ( 0 != req->buffer ) { 
		i51AdeOsFree ( req->buffer ) ;
	}
			
	i51AdeOsFree ( req ) ;

	i51AdeOsTimerStart ( i51Kernel.ReqTimer , i51KernelDispatch , 10 ) ; 

}

static iU16* i51KernelGetApplicationPathExternal () {

	//	author : Jelo 
	//	since : 2012.2.28
	// 	(C) PKIG Tech. Co., Ltd.		

	//	获取移动盘下的i51目录

	iCHAR path8bit [ 64 ] = { 0 } ;		
	static iU16 path16bit [ 64 ] = { 0 } ; 

	memset ( path16bit , 0 , sizeof(path16bit) ) ;

	sprintf ( path8bit , "%si51" , i51AdeOsGetPathSymbolEx () ) ;	
  
	i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;

	return path16bit ;
	
}


static iU16* i51KernelGetApplicationPathInternal () {

	//	author : Jelo 
	//	since : 2012.2.28
	// 	(C) PKIG Tech. Co., Ltd.		

	//	获取系统盘下的i51目录

	iCHAR path8bit [ 64 ] = { 0 } ;		
	static iU16 path16bit [ 64 ] = { 0 } ; 

	memset ( path16bit , 0 , sizeof(path16bit) ) ;

	sprintf ( path8bit , "%si51" , i51AdeOsGetPathSymbol () ) ;	
  
	i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;

	return path16bit ;
	
}

static iU16* i51KernelGetApplicationPathEx () {

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	//	获取系统盘下的i51KIT目录

	iCHAR path8bit [ 64 ] = { 0 } ;		
	static iU16 path16bit [ 64 ] = { 0 } ; 

	memset ( path16bit , 0 , sizeof(path16bit) ) ;

	sprintf ( path8bit , "%si51\\i51KIT\\" , i51AdeOsGetPathSymbol () ) ;	
  
	i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;

	return path16bit ;
	
}

static iU16* i51KernelGetSystemPathEx () {

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	//	获取系统盘下的i51SYS目录

	iCHAR path8bit [ 64 ] = { 0 } ;		
	static iU16 path16bit [ 64 ] = { 0 } ; 

	memset ( path16bit , 0 , sizeof(path16bit) ) ;
	
	sprintf ( path8bit , "%si51\\i51SYS\\" , i51AdeOsGetPathSymbol () ) ;	
	
	i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;
		
	return path16bit ;
	
}

static void i51KernelChooseDiskMode () {

	//	author : Jelo 
	//	since : 2011.10.18
	// 	(C) PKIG Tech. Co., Ltd.

	if ( iTRUE == i51AdeOsMMemCardExist () ) {		
		i51Kernel.DiskMode = i51_KERNEL_DISK_MODE_MOVABLE ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Using App-Disk Mode" ) ;
	} else {
		i51Kernel.DiskMode = i51_KERNEL_DISK_MODE_SYSTEM ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Using Sys-Disk Mode" ) ;
	}

}

static void i51KernelReleaseDapKit ()
{

	//	author : Jelo 
	//	since : 2011.10.20
	// 	(C) PKIG Tech. Co., Ltd.		

	//	销毁DAP

	iU32 looper = 0 ;
	iU32 totall = i51Kernel.DapKitListTotall ;
	//	删除kit时，先将daplist中的id遍历到一个列表中。
	//	然后逐个删除。
	iU32 idarray [ 64 ] = { 0 } ;

	//	release dap
	for ( looper = 0 ; looper < totall ; looper ++ ) {
	//	i51KernelKitListDropKit ( (iHANDLE ) i51Kernel.DapKitList[looper].id ) ;
		idarray [ looper ] = i51Kernel.DapKitList[looper].id ;
	}	

	for ( looper = 0 ; looper < totall ; looper ++ ) {
		i51KernelKitListDropKit ( (iHANDLE ) idarray [ looper ] ) ;
	}	

	i51Kernel.DapKitListTotall = 0 ;
		
	if ( 0 != i51Kernel.DapKitList ) {
		i51AdeOsFree ( i51Kernel.DapKitList ) ;
		i51Kernel.DapKitList = 0 ;
	}
	
	return ; 
	
} 

#ifdef i51SIMULATOR

static void i51KernelSwap2DllName ( iU16* path , iU16* pathnew ) {


	//	author : Jelo 
	//	since : 2011.10.21
	// 	(C) PKIG Tech. Co., Ltd.	

	//	change *.* to *.dll

	iU32 looper = 0 ;
	iU32 length = 0 ;
	
	length = i51AdeStdWStrlen ( path ) << 1 ;
	
	i51AdeStdWStrcpy ( pathnew , path ) ;

	for ( looper = 0 ; looper < length ; looper ++ ) {

		if ( '.' == pathnew[looper] ) {
			//	skip '.'
			looper ++ ;			
			break ;
		}
		
	}

	if ( looper >= length ) {
		return ;
	}
	
	pathnew[looper] = 'd' ;
	looper ++ ;
		
	pathnew[looper] = 'l' ;
	looper ++ ;

	pathnew[looper] = 'l' ;

	return ;
	
}
#endif

static iU16* i51KernelGetFileName ( iU16* path , iU16* filename ) {

	//	author : Jelo 
	//	since : 2011.10.21
	// 	(C) PKIG Tech. Co., Ltd.	
 
	//	Get File name form pattern like "C:\\dir\\file.exe"
	//	file
		
	iU32 looper = 0 ;
	iU32 length = 0 ;
	iU32 walker = 0 ;
	iU16* buffer = 0 ;
	iU16* desbuf = 0 ;
		
	if ( 0  == filename || 0 == path ) {
		return 0 ;
	}	
	
	buffer = path ;
	length = i51AdeStdWStrlen ( path ) ;
	
	for ( looper = length-1 ; looper > 0 ; looper -- ) {
		
		if( '\\'  == buffer[looper] ) {
			//	skip '\\'
			looper ++ ;
			break ;
		}
		
	}

	desbuf = filename ;

	for ( ; looper < length ; ) {
		
		if ( '.' == buffer[looper] )
		{
			desbuf [walker] = '\0' ;
			return filename ;
		}
		
		desbuf [walker] = buffer[looper] ;

		looper ++ ;
		walker ++ ;
	
	}

	desbuf [walker] = '\0' ;
			
	return filename ;
	
}

static iU16* i51KernelMakei51Path ( iU16* buffer , iU32 buflen , iU16* floder ) {


	//	author : Jelo 
	//	since : 2011.10.24
	// 	(C) PKIG Tech. Co., Ltd.		

	//	生成i51目录
	//	如floder
	//	输出x:\i51\i51KIT\floder\floder.i51

	iS32 length = 0 ;
	
	length = i51AdeStdWStrlen (i51KernelGetApplicationPath ()) ;
	length += i51AdeStdWStrlen ( floder ) ;
	length += i51AdeStdWStrlen ( L"\\" ) ;
	length += i51AdeStdWStrlen ( floder ) ;
	length += i51AdeStdWStrlen ( L".i51" ) ;
	length += 2 ;

	iASSERT ( length < buflen ) ;
		
	buffer = i51AdeStdWStrcpy ( buffer , i51KernelGetApplicationPath () ) ;
	buffer = i51AdeStdWStrcat ( buffer , floder ) ;
	buffer = i51AdeStdWStrcat ( buffer , L"\\" ) ;
	buffer = i51AdeStdWStrcat ( buffer , floder ) ;	
	buffer = i51AdeStdWStrcat ( buffer , L".i51" ) ;	

	return buffer ;

}

static void i51KernelKitAntiHang ( iU16* kitname , iBOOL set , iBOOL syspath ) {

	//	author : Jelo 
	//	since : 2011.11.4
	// 	(C) PKIG Tech. Co., Ltd.

	//	参数(1) Kit 
	//	参数(2) set 还是unset
	//	参数(3) i51KernelGetSystemPath 还是i51KernelGetApplicationPath

	iFILE file = 0 ;
	iU16 path [ 64 ] = { 0 } ;
	iU16* path16 = 0 ;
	iU32 length = 0 ;

	//Jelo 2012.51.31
	//iBOOL ret = 0 ;
	//Jelo Committed finished

	if ( iTRUE == syspath ) {
		length = i51AdeStdWStrlen (i51KernelGetSystemPath ()) ;
	} else {
		length = i51AdeStdWStrlen (i51KernelGetApplicationPath ()) ;
		length += i51AdeStdWStrlen ( kitname ) ;	
		length += i51AdeStdWStrlen ( L"\\" ) ;	
	}

	length += i51AdeStdWStrlen ( kitname ) ;	
	length += i51AdeStdWStrlen ( L".anti" ) + 2 ;	
	iASSERT ( length < sizeof(path) ) ;

	if ( iTRUE == syspath ) {
		path16 = i51AdeStdWStrcat ( path , i51KernelGetSystemPath () ) ;
	} else {
		path16 = i51AdeStdWStrcat ( path , i51KernelGetApplicationPath () ) ;
		path16 = i51AdeStdWStrcat ( path16 , kitname ) ;		

		path16 = i51AdeStdWStrcat ( path16 , L"\\" ) ;	
			
	}
	
	path16 = i51AdeStdWStrcat ( path16 , kitname ) ;	
	path16 = i51AdeStdWStrcat ( path16 , L".anti" ) ;

	if ( iTRUE == set ) {
		
		i51FILEOPEN(file,path16,i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE) ;
		
		if ( 0 != file ) {
			i51AdeOsFileClose ( file ) ;
		}
		
	} else {

		i51AdeOsFileAModeEnable () ;
		//Jelo 2012.51.31
		//ret = i51AdeOsFileIsExist ( path16 ) ;
		//Jelo Committed finished
		i51AdeOsFileDelete ( path16 ) ;
		i51AdeOsFileAModeDisable () ;			
	
	}
	
}

static iBOOL i51KernelKitCheckAntiHang ( iU16* kitname , iBOOL syspath ) {

	//	author : Jelo 
	//	since : 2011.11.4
	// 	(C) PKIG Tech. Co., Ltd.	

	//	返回iTRUE 表示防崩溃

	iFILE file = 0 ;
	iU16 path [ 64 ] = { 0 } ;
	iU16* path16 = 0 ;
	iU32 length = 0 ;
	iBOOL ret = 0 ;

	if ( iTRUE == syspath ) {
		length = i51AdeStdWStrlen (i51KernelGetSystemPath ()) ;
	} else {
		length = i51AdeStdWStrlen (i51KernelGetApplicationPath ()) ;
		length += i51AdeStdWStrlen ( kitname ) ;	
		length += i51AdeStdWStrlen ( L"\\" ) ;	
	}

	length += i51AdeStdWStrlen ( kitname ) ;	
	length += i51AdeStdWStrlen ( L".anti" ) + 2 ;	
	iASSERT ( length < sizeof(path) ) ;

	if ( iTRUE == syspath ) {
		path16 = i51AdeStdWStrcat ( path , i51KernelGetSystemPath () ) ;
	} else {
		path16 = i51AdeStdWStrcat ( path , i51KernelGetApplicationPath () ) ;
		path16 = i51AdeStdWStrcat ( path16 , kitname ) ;		

		path16 = i51AdeStdWStrcat ( path16 , L"\\" ) ;	
			
	}
	
	path16 = i51AdeStdWStrcat ( path16 , kitname ) ;	

	path16 = i51AdeStdWStrcat ( path16 , L".anti" ) ;
	
	i51AdeOsFileAModeEnable () ;
	ret = i51AdeOsFileIsExist ( path16 ) ;  
	i51AdeOsFileAModeDisable () ;	

	return ret ;
	
}

static iU32 i51KernelGetAppBitmap ( iU16* floder ){

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	//	参数如: 只能获取*.i51 的bitmap
	
	iS32 ret = 0 ;
	iFILE file = 0 ; 
	iS32 length = 0 ;
	iU16 path [ 256 ] = { 0 } ;
	iU16* ppath = 0 ;
	i51HEADER i51Header = { 0 } ;

//	真机
#ifndef i51SIMULATOR
{

	ppath = i51KernelMakei51Path ( path , sizeof(path) , floder ) ;
	
	i51FILEOPEN(file,ppath,i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN) ;

	if ( 0 == file ) {
		return 0 ;
	}
	
	i51AdeOsFileSetSeek ( file , 36 , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt16 ( file , &i51Header.Bitmap ) ; 
	i51AdeOsFileClose ( file ) ;
	
	return i51Header.Bitmap ;
	
} 
#elif defined(i51SIMULATOR)
{

	if ( i51_ADE_CNF_VER_MTK == i51AdeCnfGetType () ) {
		
		iU16 dllpath [ 256 ] = { L"..\\..\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;
		
	} else if ( i51_ADE_CNF_VER_MSTAR == i51AdeCnfGetType () ) {
	
		iU16 dllpath [ 256 ] = { L".\\SimFileSys\\CARD\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;
		
	} else if ( i51_ADE_CNF_VER_SPRD == i51AdeCnfGetType () ) {

		iU16 dllpath [ 256 ] = { L".\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;	
	}else if ( i51_ADE_CNF_VER_CS == i51AdeCnfGetType () ) {
		
		iU16 dllpath [ 256 ] = { L"..\\Bin\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;
		
	}
	
	return 0xffff ;
}
#endif

}

iCHAR* i51KernelGeti51Path () 
{

	//	author : william
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	//	获取目录
	//	如果当前是系统目录模式，则从系统盘符寻找
	//	其它情况从T卡盘符寻找

	static iCHAR path8bit [ 64 ] = { 0 } ;		
//	static iU16 path16bit [ 64 ] = { 0 } ; 

//	memset ( path16bit , 0 , sizeof(path16bit) ) ;

	if ( i51_KERNEL_DISK_MODE_MOVABLE == i51Kernel.DiskMode ) {
		sprintf ( path8bit , "%si51\\" , i51AdeOsGetPathSymbolEx () ) ;	
	} else if ( i51_KERNEL_DISK_MODE_SYSTEM == i51Kernel.DiskMode ) {
		sprintf ( path8bit , "%si51\\" , i51AdeOsGetPathSymbol () ) ;	
	}
  
//	i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;

	return path8bit ;
	
}


//	For i51KernelInstallApplication、i51KernelReinstallApplication
//

static void i51KernelDeleteAllPackage ()
{
	iU16 path[128] = {0} ;
	i51FILEINFO fileinfo = {0} ;
	iFILE file ;
	i51AdeStdWSprintf ( path , sizeof( path ) , "%s\\*.package" ,i51KernelInstaller.i51KernelAppName) ;
	if (file = i51AdeOsFileFind (path , &fileinfo ) ) {

		do {
			memset( path , 0 , sizeof(path) ) ;
			
			i51AdeStdAscii2Unicode ( i51KernelInstaller.i51KernelAppName , path ) ;
			i51AdeStdWStrcat ( path ,L"\\" ) ;
			i51AdeStdWStrcat ( path , fileinfo.name) ;
			i51AdeOsFileDelete (path) ;	
			memset( &fileinfo , 0 , sizeof(fileinfo) ) ;
		}while ( i51AdeOsFileFindNext(file , &fileinfo) ) ;
		i51AdeOsFileFindClose (file) ;
	}
	
}

static void i51KernelGenerateSerialNumberCallback( iS32 state , iU32 textlen , iU32 offset , iU32 totall , iU32 refcount ) 
{
	//	author : william
	//	since : 2011.11.4
	// 	(C) PKIG Tech. Co., Ltd.	
	//	下载sn回调

	if ( state == i51_SL_DL_ERROR_AGAIN_DLSTART )
	{
		//	提示需要重新dlstart
		i51SlDlStart( 0 ,
		0 , 
		0 , 
		0 , 
		0  , 
		0 ,
		i51_SAP_DLCOM_SERIALNUMBER , 
		i51KernelGenerateSerialNumberCallback ) ;		
		return ;
	}
	i51SlDlStop(0) ;
	if ( i51_SL_DL_FINISHED == state ) {

		void *sn = iNULL ;
		iU8 snLen = 0 ;
		if (!i51KernelSerialNumberValid()) {

			i51KernelGetSnCallback ( iFALSE , 0, 0 ) ;
			return ;
		}
			
		i51KernelGetSerialNumber ( &sn , &snLen ) ;
		i51KernelGetSnCallback ( iTRUE , sn , snLen ) ;

	}else if ( state < 0 ) {

		i51KernelGetSnCallback ( iFALSE , 0 , 0 ) ;
	}
	
}

static void i51KernelGetSNCreateDlCallback (iBOOL ret) 
{
	//	author : william
	//	since : 2011.11.16
	// 	(C) PKIG Tech. Co., Ltd.	
	//	获取sn 创建dl的回调。

	if ( ret ) {
		i51SlDlStart( 0 ,
		0 , 
		0 , 
		0 , 
		0  , 
		0 ,
		i51_SAP_DLCOM_SERIALNUMBER , 
		i51KernelGenerateSerialNumberCallback ) ;		
	} else {

		i51KernelGetSnCallback ( iFALSE , 0 , 0 ) ;
		i51SlDlStop(0) ;
	}
	

}

static iU32 i51KernelGetKitVersion ( iU16* kitpath ) {

	//	author : Jelo 
	//	since : 2011.8.25
	// 	(C) PKIG Tech. Co., Ltd.

	iU32 length = 0 ;
	iU16 path [ 256 ]  = { 0 } ;
	iU16* ppath = 0 ;
	iU32 version = 0 ;
	iFILE file = 0 ;
	iBOOL ret = iFALSE ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelGetKitVersion S" ) ;
	#endif
	
	length = i51AdeStdWStrlen (i51KernelGetApplicationPath ()) ;	
	length += i51AdeStdWStrlen ( kitpath ) + 2 ;

	iASSERT ( length < sizeof(path) ) ;	
			
	ppath = i51MALLOC ( length<<1 ) ;

	if ( 0 == ppath ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Fatal E , MEM Alloc F" ) ;
		return 0 ;
	}

	ppath = i51AdeStdWStrcpy ( path , i51KernelGetApplicationPath  () ) ;	
	ppath = i51AdeStdWStrcat ( ppath , kitpath ) ;
 
	i51FILEOPEN( file , ppath , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN ) ;

	i51FREE ( ppath ) ;
	
	if ( 0 == file ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Kit Not Found" ) ;
		return 0 ;
	}
	
	ret = i51SlLoaderGetAttr ( file , 0 , &version , 0 , 0 , 0 ) ;
	
	i51FILECLOSE(file) ;

	if ( iFALSE == ret ) {
		return 0 ;
	}

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelGetKitVersion E" ) ;
	#endif
	
	return version ;
	

}

static void  i51KernelGetSnUseDlFailCB (iU32 id)
{


	//	Jelo Edited 2012-4-17	
	//	author : william
	//	since : 2012.4.17
	// 	(C) PKIG Tech. Co., Ltd.	

	//	获取sn 互斥异步回调
	//	i51AdeOsLog ( i51_LOG_SC , "KEL : i51KernelGetSnUseDlFailCB" ) ;	
	//	if ( i51DownloaderRunning.GetSntimerid)
	//		i51AdeOsTimerStop (i51DownloaderRunning.GetSntimerid ) ;
	//	i51DownloaderRunning.GetSntimerid  = 0 ;

	//	if ( i51DownloaderRunning.GetSncallback) {

	//			i51DownloaderRunning.GetSncallback (  iFALSE , 0 , 0 ) ;
	//	}

	if ( i51DownloaderRunning.GetSntimerid )
		i51AdeOsTimerStop ( i51DownloaderRunning.GetSntimerid ) ;

	i51DownloaderRunning.GetSntimerid  = 0 ;

	if ( i51DownloaderRunning.GetSncallback) {
		i51DownloaderRunning.GetSncallback (  iFALSE , 0 , 0 ) ;
	}	
	
	//	Jelo Edited Finished
	
}



static iBOOL i51KernelUnloadSysKitEx () {

	//	author : Jelo 
	//	since : 2012.4.16
	// 	(C) PKIG Tech. Co., Ltd.

	//	卸载系统套件，即SAP，它存放于静态内存，不需要释放

	i51KERNELKIT* KitServer = 0 ;
	iU32 looper = 0 ;
	
	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelUnloadSysKit S , 0x%x" , handle ) ;
	#endif

	for ( looper = 0 ; looper < i51Kernel.SapKitListTotall ; looper ++ ) {
		if ( 0 == i51AdeStdWStrcmp ( i51Kernel.SapKitList[looper].name , L"i51KitServer" ) ) {
			KitServer = &i51Kernel.SapKitList[looper] ;
			break ;
		}
	}

	if ( 0 == KitServer ) {
		return iTRUE ;
	}
	
	if ( 0 != KitServer ) {
		KitServer->i51KitMain ( KitServer->id , i51_MSG_PAUSE , 0 ) ;
		KitServer->i51KitMain ( KitServer->id , i51_MSG_EXIT , 0 ) ;
	}

	//	move
	for ( ; looper < i51Kernel.SapKitListTotall-1 ; looper ++ ) {
		i51AdeStdMemcpy16 ( &i51Kernel.SapKitList[looper] , &i51Kernel.SapKitList[looper+1] , sizeof(i51KERNELKIT) ) ;
	}
	i51Kernel.SapKitListTotall -- ;
		
	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelUnloadSysKit E" ) ;
	#endif	
	
	return iTRUE ;
	
}

#if 0
iBOOL i51KernelCheckAppCompleten  ( iU16* floder ) {

	//	author : Jelo 
	//	since : 2011.10.27
	// 	(C) PKIG Tech. Co., Ltd.	

	//	检查应用完整性
	//	包括: Kernel版本、Adapter版本、大小端、依赖项、依赖项版本

	//	i51Compiler的约定，一个路径不会超过128字节
	#define i51_KERNEL_iCL_MAX_PATH 128+1
	//	i51Compiler的约定，最多有i51_KERNEL_iCL_MAX_REF个依赖
	#define i51_KERNEL_iCL_MAX_REF 16 

	iCHAR buffer [ 128 ] = { 0 } ;
	iU32 looper = 0 ;
	iU32 length = 0 ;	
	iU32 counter = 0 ;	
	iU16 path [64 ] = { 0 } ;
	iU16* path16 = 0 ;
	iU32 version = 0 ;
	iU32 kelversion = 0 ;
	iU32 adaversion = 0 ;
	iU32 endian = 0 ;
	iU32 i51SllDKitRefTotall = 0 ;		
	iBOOL ret = iFALSE ;
	iFILE file = 0 ;

	//	Jelo Edit Since 2012.12.6
	//	iFILE RefFile = 0 ;
	//	Jelo Edit Finished
		
	i51SllDKITREF* i51SllDKitRef = 0 ;

	//	Jelo Edit Since 2012.12.6
	//	notes : 依赖检测在运行时只检测一次
	//	检测通过的依赖项存入i51SllDKitRefFile 。
	//	下次检测时首先搜索i51SllDKitRefFile，如果搜到则返回iTRUE。	
	//	i51SllDKITREF* i51SllDKitRefTable = 0 ;
	//	Jelo Edit Finished
	
	{
		i51AdeStdUnicode2Ascii ( floder , buffer ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Check Comp '%s'" , buffer ) ;
	}
	
	length = i51AdeStdWStrlen ( floder ) ;	
	length += i51AdeStdWStrlen ( L"\\" ) ;	
	length += i51AdeStdWStrlen ( floder ) ;	
	length += i51AdeStdWStrlen ( L".package" ) + 2 ;	
	iASSERT ( length < sizeof(path) ) ;

	{
		//	首先检测package存不存在。
		path16 = i51AdeStdWStrcpy ( path , floder ) ;
		path16 = i51AdeStdWStrcat ( path16 , L".package" ) ;	
		
		file = i51AdeOsFileOpen (path16 , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;

		//	存在package，则先解包。
		if ( 0 != file ) { 

			i51AdeOsLog ( i51_LOG_SC , "KEL : Pack Detected" ) ;
			
			ret = i51KernelExtractPackage ( file , floder ) ;
			i51AdeOsFileClose ( file ) ;
			
			if ( iFALSE == ret ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Ext Pack E" ) ;
				return ret ;  
			}

			i51AdeOsFileDelete ( path16 ) ; 
			i51AdeOsLog ( i51_LOG_SC , "KEL : Pack Exted" ) ;
			
		}  

	}
		
	path16 = i51AdeStdWStrcpy ( path , floder ) ;	
	path16 = i51AdeStdWStrcat ( path16 , L"\\" ) ;	
	path16 = i51AdeStdWStrcat ( path16 , floder ) ;	
	path16 = i51AdeStdWStrcat ( path16 , L".i51" ) ;	
	
	file = i51AdeOsFileOpen(path16,i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ,__FILE__,__LINE__);

	if ( 0 == file ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : %s NE" , buffer ) ;
		return iFALSE ;
	}

#ifndef i51SIMULATOR
	//	检查加载器能否加载该文件
	if ( iFALSE == i51SlLoaderLoadable ( file ) ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : OBJ E" , buffer ) ;
		i51AdeOsFileClose ( file ) ;
		return iFALSE ;
	}
	
	//	 检查Kernel版本、Adapter版本、大小端
	i51SlLoaderGetAttr ( file , 0 , 0 , &kelversion , &adaversion , &endian ) ; 
	
	//	kernel 版本检测
	if ( kelversion > i51KernelGetVersion () ) {
		i51AdeOsFileClose ( file ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : KEL %d UM" , kelversion ) ;
		return iFALSE ;
	}

	//	adapter 版本检测
	if ( adaversion > i51AdeCnfGetVersion () ) {		
		i51AdeOsFileClose ( file ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : ADE %d UM" , adaversion ) ;
		return iFALSE ;
	}	

	//	大小端检测
	if ( endian != i51AdeCnfGetEndian () ) {
		i51AdeOsFileClose ( file ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Endian %d UM" , endian ) ;
		return iFALSE ;
	}
#endif
	
	i51SllDKitRef = (i51SllDKITREF* ) i51MALLOC ( i51_KERNEL_iCL_MAX_REF*sizeof(i51SllDKITREF) ) ;

	if ( 0 == i51SllDKitRef ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Mem F" ) ;
		i51AdeOsFileClose ( file ) ;
		return iFALSE ;		
	}

	//	Jelo Edit 2011.12.31
	i51SlLoaderGetKitRef ( file , i51SllDKitRef , i51_KERNEL_iCL_MAX_REF , &i51SllDKitRefTotall ) ;
	i51AdeOsFileClose ( file ) ;
	//if ( 0 == i51SlLoaderGetKitRef ( file , i51SllDKitRef , i51_KERNEL_iCL_MAX_REF , &i51SllDKitRefTotall ) ) {
	//	i51FREE ( i51SllDKitRef ) ;
	//	i51AdeOsFileClose ( file ) ;
	//	return iTRUE ;
	//}
	//	Jelo Edit Finished
	
	if ( 0 == i51SllDKitRefTotall ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : No Ref" ) ;
		i51FREE ( i51SllDKitRef ) ;
		return iTRUE ;
	}

	//	Jelo Edit Since 2012.12.6
	//	x:\i51KIT\i51SYS\i51KitRef.table
	//	path16 = i51AdeStdWStrcpy ( path , i51KernelGetSystemPath () ) ;
	//	i51AdeStdWStrcat ( path16 , L"i51KitRef.table" ) ;
	
	//if ( iFALSE == i51Kernel.NotCreateRefTable ) {

		//	创建Ref.Table
		//i51AdeOsFileAModeEnable () ;
		//RefFile = i51AdeOsFileOpen ( path16 , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
		//i51AdeOsFileAModeDisable () ;		

		//i51Kernel.NotCreateRefTable = iTRUE ;

	//} else {

	//	i51AdeOsFileAModeEnable () ;
	//	RefFile = i51AdeOsFileOpen ( path16 , i51_OS_FL_WRITE | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
	//	i51AdeOsFileAModeDisable () ;
		
	//}
	
 
	//if ( 0 == RefFile ) {
	//	i51FREE ( i51SllDKitRef ) ;
	//	i51AdeOsLog ( i51_LOG_SC , "KEL : Ref.Table F" ) ;
	//	return iFALSE ;
	//}
	
	//i51AdeOsFileGetSize ( RefFile , (iS32*)&length ) ;
	
	//if ( 0 < length ) {
	//	i51SllDKitRefTable = (i51SllDKITREF* ) i51MALLOC ( length ) ;
	//}
			
	//if ( 0 != i51SllDKitRefTable ) {
		
	//	i51AdeOsFileRead ( RefFile , i51SllDKitRefTable , length , 0 ) ;

	//	length = length / sizeof(i51SllDKITREF) ;
		
	//	ret = iTRUE ;

	//	for ( looper = 0 ; looper < i51SllDKitRefTotall ; looper ++ ) {		

	//		for ( counter = 0 ; counter < length ; counter ++ ) {
					
	//			if ( 0 == strcmp ( i51SllDKitRefTable[counter].path , i51SllDKitRef[looper].path ) ) {	
										
	//				break ;

	//			}
					
	//		}

	//		if ( counter >= length ) {
	//			ret = iFALSE ;
	//			break ;
	//		}

	//	}

	//	if ( iTRUE == ret ) {
 
	//		i51AdeOsLog ( i51_LOG_SC , "KEL : Ref Check OK in Ref.Table" ) ;
	//		i51FREE ( i51SllDKitRef ) ;		
	//		i51FREE ( i51SllDKitRefTable ) ;
	//		i51AdeOsFileClose ( RefFile ) ;
				
	//		return iTRUE ;
				
	//	}

	//	i51FREE ( i51SllDKitRefTable ) ;
			
	//}

	//	for ( ; looper < i51SllDKitRefTotall ; looper ++ ) {
	for ( looper = 0 ; looper < i51SllDKitRefTotall ; looper ++ ) {
	//	Jelo Edit Finished
	
		iBOOL ret = iFALSE ;
		iBOOL exist = iFALSE ;
		iU16 path16bit [ 128 ] = { 0 } ;
		iU16* ppath = 0 ;

		strcpy ( buffer , i51KernelGeti51Path () ) ;
		strcat ( buffer , i51SllDKitRef[looper].path ) ;
		
		ppath = i51AdeStdAscii2Unicode ( buffer , path16bit ) ;

		//	Jelo Add Since 2012.12.6
		i51AdeOsFileAModeEnable () ;
		exist = i51AdeOsFileIsExist ( ppath ) ;
		i51AdeOsFileAModeDisable () ;
		if ( iFALSE == exist ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : Ref %s , NE" , i51SllDKitRef[looper].path ) ;
			goto REFCHECKFAILED ;
		}
		//	Jelo Add Finished

		i51AdeOsFileAModeEnable () ;
		file = i51AdeOsFileOpen ( ppath , i51_OS_FL_WRITE | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
		i51AdeOsFileAModeDisable () ;

		//	如果依赖文件并且可能已经被打开
		//	则跳过
		if ( 0 == file ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : Ref %s , Opened" , i51SllDKitRef[looper].path ) ;
			continue ;			
		} 
		  
		//	检查re 版本
		if ( 0 != strstr ( i51SllDKitRef[looper].path , ".re" ) ) {

			iU32 version = 0 ;

			//	skip crc
			i51AdeOsFileSetSeek ( file , 6 , i51_OS_FL_SEEK_HEAD ) ;
			i51AdeOsFileReadInt32 ( file , &version ) ;
			i51AdeOsFileClose ( file ) ; 

			if ( version < i51SllDKitRef[looper].version ) {
				
				i51AdeOsLog ( i51_LOG_SC , "KEL : Ref %s , Local %d UM %d" , 
					i51SllDKitRef[looper].path , version , i51SllDKitRef[looper].version ) ;				

				goto REFCHECKFAILED ;
				
			}
			
			
		} else {
		//	检查套件版本
					
			i51SlLoaderGetAttr ( file , 0 , &version , &kelversion , &adaversion , &endian ) ; 			
			i51AdeOsFileClose ( file ) ; 

			if ( version < i51SllDKitRef[looper].version ) {
				
				i51AdeOsLog ( i51_LOG_SC , "KEL : Ref %s , Local %d UM %d" , 
					i51SllDKitRef[looper].path , version , i51SllDKitRef[looper].version ) ;				

				goto REFCHECKFAILED ;
				
			}
			
			//	kernel 版本检测
			if ( kelversion > i51KernelGetVersion () ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : KEL %d UM" , kelversion ) ;
				goto REFCHECKFAILED ;
			}

			//	adapter 版本检测
			if ( adaversion > i51AdeCnfGetVersion () ) {		
				i51AdeOsLog ( i51_LOG_SC , "KEL : ADE %d UM" , adaversion ) ;
				goto REFCHECKFAILED ;
			}	

			//	大小端检测
			if ( endian != i51AdeCnfGetEndian () ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Endian %d UM" , endian ) ;
				goto REFCHECKFAILED ;
			}							
			
		}
		
		//	Jelo Edit Since 2012.12.6
		//	if ( 0 != RefFile )  {
			//	将依赖写入Ref.table
		//		i51AdeOsFileWrite ( RefFile , (void* )&i51SllDKitRef[looper] , sizeof(i51SllDKITREF) , 0 ) ;
		//		i51AdeOsFileFlush ( RefFile ) ;
		//	}
		//	Jelo Edit Finished
			
	} 

	i51FREE ( i51SllDKitRef ) ; 

	//	Jelo Edit Since 2012.12.6
	//	i51AdeOsFileClose ( RefFile ) ;
	//	Jelo Edit Finished
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Chek Comp E" ) ;
	
	return iTRUE ;

REFCHECKFAILED :

	i51FREE ( i51SllDKitRef ) ;

	//	Jelo Edit Since 2012.12.6
	//	i51AdeOsFileClose ( RefFile ) ;
	//	Jelo Edit Finished

	i51AdeOsLog ( i51_LOG_SC , "KEL : Chek Comp BE" ) ;
	
	return iFALSE ;


}
#endif

#if 1
iBOOL i51KernelCheckKitCrc ( iFILE file ) {

	//	author : Jelo 
	//	since : 2012.6.15
	// 	(C) PKIG Tech. Co., Ltd.	

	iU32 crc = 0 ;
	iS32 seek = 0 ;
	iU8* i51buffer = 0 ; 
	iS32 filelength = 0 ;

	i51AdeOsFileGetSeek ( file , &seek , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileGetSize ( file , &filelength ) ;
	i51AdeOsFileSetSeek ( file , seek , i51_OS_FL_SEEK_HEAD ) ;

	if ( 0 == filelength ) {
		return iFALSE ;
	}
	
	i51buffer = (iU8* ) i51MALLOC ( filelength-sizeof(crc) ) ;
	
	if ( 0 == i51buffer ) {		
		i51AdeOsLog ( i51_LOG_SC , "KEL : CRC MEM F" ) ;
		return iFALSE ;
	} 
  
	i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileRead ( file , &crc , sizeof(crc) , 0 ) ;
	i51AdeOsFileRead ( file , i51buffer , filelength-sizeof(crc) , 0 ) ;
	i51AdeOsFileSetSeek ( file , seek , i51_OS_FL_SEEK_HEAD ) ;
	
	if ( crc != i51AdeStdCrc32 ( i51buffer , filelength-sizeof(crc) , 0 ) ) {
		
		i51FREE ( i51buffer ) ;
	
		i51AdeOsLog ( i51_LOG_SC , "KEL : CRC F" ) ;
		
		return iFALSE ;
		
	}

	i51FREE ( i51buffer ) ;
	
	return iTRUE ;

}

static iBOOL i51KernelCheckCompleten ( iU16* Abspath , iU8 RefType , iBOOL CheckRefVersion , iU32 RefVersion ) {

	//	author : Jelo 
	//	since : 2012.6.7
	// 	(C) PKIG Tech. Co., Ltd.	

	//	RefType = 0 : *.i51
	//	RefType = 1 : *.Kit
	
	//	i51Compiler的约定，一个路径不会超过128字节
	#define i51_KERNEL_iCL_MAX_PATH 128+1
	//	i51Compiler的约定，最多有i51_KERNEL_iCL_MAX_REF个依赖
	#define i51_KERNEL_iCL_MAX_REF 16 

	iCHAR buffer [ i51_KERNEL_iCL_MAX_PATH ] = { 0 } ;
	iU16 path [64 ] = { 0 } ;

	iU16* floder = 0 ;
	iU16* ppath = 0 ;
	
	iU32 looper = 0 ;
	iU32 length = 0 ;	
	iU32 counter = 0 ;
	iU16* path16 = 0 ;
	iU32 version = 0 ;
	iU32 kelversion = 0 ;
	iU32 adaversion = 0 ;
	iU32 endian = 0 ;
	iU32 i51SllDKitRefTotall = 0 ;		
	iBOOL ret = iFALSE ;
	iFILE file = 0 ;
		
	i51SllDKITREF* i51SllDKitRef = 0 ;

	//	for *.i51
	if ( 0 == RefType ) {

		floder = path ;
		
		i51KernelGetFileName ( Abspath , floder ) ;
		
		length = i51AdeStdWStrlen ( floder ) ;	
		length += i51AdeStdWStrlen ( L"\\" ) ;	
		length += i51AdeStdWStrlen ( floder ) ;	
		length += i51AdeStdWStrlen ( L".package" ) + 2 ;	
		iASSERT ( length < sizeof(path) ) ;

		//	首先检测package存不存在。
		path16 = i51AdeStdWStrcpy ( path , floder ) ;
		path16 = i51AdeStdWStrcat ( path16 , L".package" ) ;	
		
		file = i51AdeOsFileOpen (path16 , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;

		//	存在package，则先解包。
		if ( 0 != file ) { 

			i51AdeOsLog ( i51_LOG_SC , "KEL : Pack Detected" ) ;
			
			ret = i51KernelExtractPackage ( file , floder ) ;
			i51AdeOsFileClose ( file ) ;
			
			if ( iFALSE == ret ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Ext Pack E" ) ;
				return ret ;  
			}

			i51AdeOsFileDelete ( path16 ) ; 
			i51AdeOsLog ( i51_LOG_SC , "KEL : Pack Exted" ) ;
			
		}  			
	
		
	}

	i51AdeOsFileAModeEnable () ;
	i51FILEOPEN(file,Abspath,i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ);
	i51AdeOsFileAModeDisable () ;
		
	if ( 0 == file ) {
		i51AdeStdUnicode2Ascii ( Abspath , buffer ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Ref '%s' NE" , buffer ) ; 
		return iFALSE ;
	}		
	

#ifndef i51SIMULATOR

	if ( iFALSE == i51KernelCheckKitCrc (file) ) {
		i51AdeOsFileClose ( file ) ;
		return iFALSE ;		
	}

	//	检查加载器能否加载该文件
	if ( iFALSE == i51SlLoaderLoadable ( file ) ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : OBJ E" , buffer ) ;
		i51AdeOsFileClose ( file ) ;
		return iFALSE ;
	}
	
	//	 检查Version、Kernel版本、Adapter版本、大小端
	i51SlLoaderGetAttr ( file , 0 , &version , &kelversion , &adaversion , &endian ) ; 

	if ( iTRUE == CheckRefVersion ) {
		//	检查自身版本
		if ( version < RefVersion ) {

			i51AdeStdUnicode2Ascii ( Abspath , buffer ) ;
			i51AdeOsLog ( i51_LOG_SC , "KEL : Ref '%s' , Local %d UM %d" , buffer , version , RefVersion ) ;				

			i51AdeOsFileClose ( file ) ;		
			return iFALSE ;
					
		}
	}
	
	//	kernel 版本检测
	if ( kelversion > i51KernelGetVersion () ) {
		i51AdeOsFileClose ( file ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : KEL %d UM" , kelversion ) ;
		return iFALSE ;
	}

	//	adapter 版本检测
	if ( adaversion > i51AdeCnfGetVersion () ) {		
		i51AdeOsFileClose ( file ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : ADE %d UM" , adaversion ) ;
		return iFALSE ;
	}	

	//	大小端检测
	if ( endian != i51AdeCnfGetEndian () ) {
		i51AdeOsFileClose ( file ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Endian %d UM" , endian ) ;
		return iFALSE ;
	}
#endif
	
	i51SllDKitRef = (i51SllDKITREF* ) i51MALLOC ( i51_KERNEL_iCL_MAX_REF*sizeof(i51SllDKITREF) ) ;

	if ( 0 == i51SllDKitRef ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Mem F" ) ;
		i51AdeOsFileClose ( file ) ;
		return iFALSE ;		
	}

	i51SlLoaderGetKitRef ( file , i51SllDKitRef , i51_KERNEL_iCL_MAX_REF , &i51SllDKitRefTotall ) ;
	i51AdeOsFileClose ( file ) ;
	
	if ( 0 == i51SllDKitRefTotall ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : No Ref" ) ;
		i51FREE ( i51SllDKitRef ) ;
		return iTRUE ;
	}

	for ( looper = 0 ; looper < i51SllDKitRefTotall ; looper ++ ) {

		iBOOL ret = iFALSE ;
		iBOOL exist = iFALSE ;
		iU16 path16bit [ 64 ] = { 0 } ;
		iU16* ppath = 0 ;

		//	检查re 版本
		if ( 0 != strstr ( i51SllDKitRef[looper].path , ".re" ) ) {

			iU32 version = 0 ;

			//	Jelo Add 2012.6.7
			strcpy ( buffer , i51KernelGeti51Path () ) ;
			strcat ( buffer , i51SllDKitRef[looper].path ) ;		
			ppath = i51AdeStdAscii2Unicode ( buffer , path16bit ) ;

			//	Jelo Add Since 2012.12.6
			i51AdeOsFileAModeEnable () ;
			exist = i51AdeOsFileIsExist ( ppath ) ;
			i51AdeOsFileAModeDisable () ;
			if ( iFALSE == exist ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Re-Ref %s , NE" , i51SllDKitRef[looper].path ) ;
				goto REFCHECKFAILED ;
			}
			//Jelo Add Finished

			i51AdeOsFileAModeEnable () ;
			file = i51AdeOsFileOpen ( ppath , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
			i51AdeOsFileAModeDisable () ;

			//	如果依赖文件并且可能已经被打开
			//	则跳过
			if ( 0 == file ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Re-Ref %s , Opened" , i51SllDKitRef[looper].path ) ;
				continue ;			
			}
			//	Jelo Add Finished

			//	skip crc
			i51AdeOsFileSetSeek ( file , 6 , i51_OS_FL_SEEK_HEAD ) ;
			i51AdeOsFileReadInt32 ( file , &version ) ;
			i51AdeOsFileClose ( file ) ; 

			if ( version < i51SllDKitRef[looper].version ) {
				
				i51AdeOsLog ( i51_LOG_SC , "KEL : Re-Ref %s , Local %d UM %d" , 
					i51SllDKitRef[looper].path , version , i51SllDKitRef[looper].version ) ;				

				goto REFCHECKFAILED ;
				
			}
			
			
		}
		//	检查i51 版本
		else if ( 0 != strstr ( i51SllDKitRef[looper].path , ".i51" ) ) {

			strcpy ( buffer , i51KernelGeti51Path () ) ;
			strcat ( buffer , i51SllDKitRef[looper].path ) ;		
			ppath = i51AdeStdAscii2Unicode ( buffer , path ) ;
		
			ret = i51KernelCheckCompleten ( ppath , 0 , iTRUE , i51SllDKitRef[looper].version ) ;
		
			if ( iFALSE == ret ) {
					
				//	只删除i51				
				i51AdeOsFileAModeEnable () ;
				i51AdeOsFileDelete ( ppath ) ;
				i51AdeOsFileAModeDisable () ;

				i51AdeOsLog ( i51_LOG_SC , "KEL : Del '%s'" ,  buffer ) ;

				goto REFCHECKFAILED ;
				
			}				

		}
		//	检查kit 版本
		else if ( 0 != strstr ( i51SllDKitRef[looper].path , ".kit" ) ) {
		
		//	检查套件版本
			strcpy ( buffer , i51KernelGeti51Path () ) ;
			strcat ( buffer , i51SllDKitRef[looper].path ) ;		
			ppath = i51AdeStdAscii2Unicode ( buffer , path ) ;
		
			ret = i51KernelCheckCompleten ( ppath , 1 , iTRUE , i51SllDKitRef[looper].version ) ;
		
			if ( iFALSE == ret ) {

				i51AdeOsLog ( i51_LOG_SC , "KEL : Del '%s'" ,  buffer ) ;
				
				i51AdeOsFileAModeEnable () ;
				i51AdeOsFileDelete ( ppath ) ;
				i51AdeOsFileAModeDisable () ;

				goto REFCHECKFAILED ;
				
			}				
			
		}
			
	} 

	i51FREE ( i51SllDKitRef ) ; 
		
	return iTRUE ;

REFCHECKFAILED :

	i51FREE ( i51SllDKitRef ) ;
	
	return iFALSE ;

			
}

iBOOL i51KernelCheckAppCompleten  ( iU16* floder ) {

	//	author : Jelo 
	//	since : 2011.10.27
	// 	(C) PKIG Tech. Co., Ltd.	

	//	检查应用完整性

	iCHAR path8[64] = { 0 } ;
	iU16 path16[64] = { 0 } ;
	iU16 flagfile[64] = { 0 } ;
	iU16* pointer = 0 ;
	iBOOL ret = 0 ;

	i51AdeStdUnicode2Ascii ( floder , path8 ) ;
	i51AdeOsLog ( i51_LOG_SC , "KEL : Chk Cpl of '%s'" , path8 ) ;

	pointer = i51AdeStdWStrcpy ( path16 , i51KernelGetSystemPath  () ) ;	
	pointer = i51AdeStdWStrcat ( pointer , L"i51KitServer.kit" ) ;

	i51AdeOsFileAModeEnable () ;
	ret = i51AdeOsFileIsExist ( pointer ) ;
	i51AdeOsFileAModeDisable () ;

	if ( iFALSE == ret ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : KitServer NE" ) ;
		return iFALSE ;
	}
		
	strcpy ( path8 , i51KernelGeti51Path () ) ;
	i51AdeStdAscii2Unicode ( path8 ,  flagfile ) ;

	pointer = i51AdeStdWStrcat ( flagfile , L"i51KIT\\" ) ;	
	pointer = i51AdeStdWStrcat ( pointer , floder ) ;	
	pointer = i51AdeStdWStrcat ( pointer , L"\\" ) ;	  
	pointer = i51AdeStdWStrcat ( pointer , floder ) ;	
	pointer = i51AdeStdWStrcat ( pointer , L".comp" ) ;

	i51AdeOsFileAModeEnable () ;
	ret = i51AdeOsFileIsExist ( pointer ) ;
	i51AdeOsFileAModeDisable () ;

	if ( iTRUE == ret ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Chk Cpl Skiped , OK") ;
		return ret ;
	}
	
	strcpy ( path8 , i51KernelGeti51Path () ) ;
	i51AdeStdAscii2Unicode ( path8 ,  path16 ) ;

	pointer = i51AdeStdWStrcat ( path16 , L"i51KIT\\" ) ;	
	pointer = i51AdeStdWStrcat ( pointer , floder ) ;	
	pointer = i51AdeStdWStrcat ( pointer , L"\\" ) ;	  
	pointer = i51AdeStdWStrcat ( pointer , floder ) ;	
	pointer = i51AdeStdWStrcat ( pointer , L".i51" ) ;
	
	ret = i51KernelCheckCompleten ( pointer , 0 , iFALSE , 0 ) ;

	if ( iFALSE == ret ) {

		i51AdeStdAscii2Unicode ( path8 ,  path16 ) ;

		i51AdeStdWStrcpy ( path16 , L"i51KIT\\" ) ;	
		pointer = i51AdeStdWStrcat ( path16 , floder ) ;	

		//	删除整个APP目录
		i51AdeOsFileAModeEnable () ;
		i51AdeOsFileDeleteDir ( pointer ) ;
		i51AdeOsFileAModeDisable () ;
		
		i51AdeStdUnicode2Ascii ( floder , path8 ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Del App '%s'" , path8 ) ;	
		
	} else {

		iFILE file = 0 ;
		i51AdeOsFileAModeEnable () ;
		file = i51AdeOsFileOpen ( flagfile , i51_OS_FL_ALWAYS_CREATE | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
		if ( 0 != file ) i51AdeOsFileClose ( file ) ;
		i51AdeOsFileAModeDisable () ;
		
	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : Chk Cpl Finished , Ret = %d" , ret ) ;
	
	return ret ;

}
#endif

static iU16 *i51KernelStrstr16(iU16 *str1, iU16 *str2)
{
    //	author : stever
    //	since : 20120224
    //	notes:解析一个iU16的字符串str1是否包含字符串str2不包含返回NULL
    
    iU16 *s1, *s2;
    iU32 lenth,lenth1 ;

    lenth = i51AdeStdWStrlen(str2);
    lenth1 = lenth;
    if (str1 == str2)
    {
        return str1;
    }

    while (*str1) 
    {
        s1 = str1;
        s2 = str2;

        while ((*s1 == *s2) && *s1 && *s2) 
        {
            s1++;
            s2++;
            lenth1--;
        }

        if (lenth1 == 0)
        {
            return (str1); 
        }
	 lenth1 = lenth;
        str1++;  
    }
    return NULL; 
}

static void i51KernelFileCpy(iU16 *srcPath,iU16 *drcPath,iU16 *FileName)
{
       //	author : stever
	//	since : 20120224
	//	notes:复制文件
	

	 iU16 nameF[256] = {0};
	 iU16 nameT[256] = {0};
	 iU16 comFLag[] = {'\\',0x00};
        iCHAR ContainerData[10] = {0};
        iFILE file = 0;
        iU32 readLen = 0;

       memset(nameF,0,sizeof(nameF));
       memset(nameT,0,sizeof(nameT));

	i51AdeStdWStrcat(nameF,srcPath);
	i51AdeStdWStrcat(nameF,comFLag);
	i51AdeStdWStrcat(nameF,FileName);

	i51AdeStdWStrcat(nameT,drcPath);
	i51AdeStdWStrcat(nameT,comFLag);
	i51AdeStdWStrcat(nameT,FileName);	

        i51AdeOsFileAModeEnable();
        file = i51AdeOsFileOpen(nameF,i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ,__FILE__,__LINE__);
        i51AdeOsFileAModeDisable();
        if(file>0)
        {
            i51AdeOsFileRead (file , ContainerData ,10 ,&readLen) ;
            i51AdeOsFileAModeEnable();
            i51AdeOsFileClose ( file) ;
            i51AdeOsFileAModeDisable();
           
           if((ContainerData[4]=='i'&&ContainerData[5]=='5'&&ContainerData[6]=='1'&&ContainerData[7]=='G'&&ContainerData[8]==1)
            ||(ContainerData[4]=='i'&&ContainerData[5]=='5'&&ContainerData[6]=='1'&&ContainerData[7]=='C'&&ContainerData[8]==2))
            {
                i51AdeOsLog ( 0 ,  "don't write") ;	
                return;
            }
        }
       i51AdeOsFileAModeEnable();
       i51AdeOsLog(0, "cope file ");
       i51AdeOsFileCopy(nameF, nameT);
       i51AdeOsFileAModeDisable();
}

static void i51KernelCopyDirTree(iU16 *DirFrom,iU16 *DirTo)
{
	iU16 name_Dir[64] = {0};
	iU16 MiddleBuff[64] = {0};
	iU16 name_DirTo[64] = {0};
	iU16 NORMAL_FLAG[] = {0X002e,0X0000};
	iU16 DirFlag[] = {'\\','*',0x00};
	iU16 comFLag[] = {'\\',0x00};
	i51FILEINFO FileInfo = {0};
	iFILE file = 0;
	iU16 *str = NULL;
	iU16 *strP = NULL;
	iU16 *strq = NULL;
	iBOOL result = 0;
	iU32 attr = 0;
    static iCHAR fileF[128] = {0};

	memset(name_Dir,0,sizeof(name_Dir));
	memset(name_DirTo,0,sizeof(name_DirTo));

	i51AdeOsFileAModeEnable();
    memset(fileF, 0, sizeof(fileF));
    i51AdeStdUnicode2Ascii(DirTo, fileF);
    i51AdeOsLog(0, "DirTo = %s", fileF);
	i51AdeOsFileCreateDir(DirTo);
	i51AdeOsFileAModeDisable();


	i51AdeStdWStrcat(name_Dir,DirFrom);
	i51AdeStdWStrcat(name_Dir,DirFlag);

	i51AdeOsFileAModeEnable();
	file = i51AdeOsFileFind( name_Dir , &FileInfo);
	i51AdeOsFileAModeDisable();
	if(file==0)return;

	do{
		str = NULL;
		str = i51KernelStrstr16(FileInfo.name,NORMAL_FLAG);
		if(str!=NULL)
		{
			i51KernelFileCpy(DirFrom,DirTo,FileInfo.name);	
		}
		else
		{
			memset(MiddleBuff,0,sizeof(MiddleBuff));
			i51AdeStdWStrcat(MiddleBuff,DirFrom);
			i51AdeStdWStrcat(MiddleBuff,comFLag);
			i51AdeStdWStrcat(MiddleBuff,FileInfo.name);

			i51AdeOsFileAModeEnable();
			attr = i51AdeOsFileGetAttr (MiddleBuff);
			i51AdeOsFileAModeDisable();

			if(i51_OS_FL_ATTR_DIR&attr)
			{
				memset(name_DirTo,0,sizeof(name_DirTo));
				i51AdeStdWStrcat(name_DirTo,DirTo);
				i51AdeStdWStrcat(name_DirTo,comFLag);
				i51AdeStdWStrcat(name_DirTo,FileInfo.name);

				i51KernelCopyDirTree(MiddleBuff,name_DirTo);
			}
			else
			{
				i51KernelFileCpy(DirFrom,DirTo,FileInfo.name);
			}
		}

		memset(&FileInfo,0,sizeof(i51FILEINFO));
		i51AdeOsFileAModeEnable();
		result = i51AdeOsFileFindNext(file , &FileInfo);
		i51AdeOsFileAModeDisable();
		if(result==0)
			break;

	}while (1) ;

	i51AdeOsFileAModeEnable();
	i51AdeOsFileFindClose(file);
	i51AdeOsFileAModeDisable();
}

static void i51KernelCopy2MovableDisk () {

	//	author : Jelo
	//	since : 2012.2.29
	// 	(C) PKIG Tech. Co., Ltd.

	//	notes :根据情况 将系统盘的i51目录复制到T卡。

	//	该接口对应用层不开放。
	
	i51AdeOsFileAModeEnable();
	if ( i51_KERNEL_DISK_MODE_MOVABLE == i51Kernel.DiskMode ) {
		//	外部盘不存在i51目录
		if ( iFALSE == i51AdeOsFileIsDirExist ( i51KernelGetApplicationPathExternal () ) ) {
			//	但在系统盘存在i51目录
			if ( iTRUE == i51AdeOsFileIsDirExist ( i51KernelGetApplicationPathInternal () ) ) {

				iU32 st = 0 ; 
				iU32 et = 0 ;

				i51AdeOsLog ( i51_LOG_SC , "KEL : Copy i51-Dir Start" ) ;

				st = i51AdeOsGetTick () ;
				i51KernelCopyDirTree ( i51KernelGetApplicationPathInternal () , i51KernelGetApplicationPathExternal () ) ;
				et = i51AdeOsGetTick () ;  

				i51AdeOsLog ( i51_LOG_SC , "KEL : Copy i51-Dir Finished , Cost %d" , et-st ) ;
				
			}
		} 
	}
	i51AdeOsFileAModeDisable();	

}

iU32 i51KernelGetApplicationList ( iU8 (*proc)(void *arg, iU16* GetName), void *arg) {

	//	author : Otto 
	//	since : 2012.3.8
	// 	(C) PKIG Tech. Co., Ltd.
	
	iU16 path[64] = {0} ;
	iU32 CurrAppNum = 0;
	i51FILEINFO fileinfo = {0} ;
	iFILE file = 0 ;

	if ((file = i51AdeOsFileFind (L"*" , &fileinfo )) <= 0 ) {
		return iFALSE;
	}

	do {

		memset (path , 0 , sizeof(path)) ;
		i51AdeStdWStrcpy (path ,fileinfo.name) ;
		i51AdeStdWStrcat (path ,L"\\") ;
		i51AdeStdWStrcat (path ,fileinfo.name) ;
		i51AdeStdWStrcat (path ,L".i51") ;
		if ( i51AdeOsFileIsExist(path) )
		{

			//	Jelo Add 20120315
			memset (path , 0 , sizeof(path)) ;
			i51AdeStdWStrcpy (path ,fileinfo.name) ;
			i51AdeStdWStrcat (path ,L"\\") ;
			i51AdeStdWStrcat (path ,L"icon.re") ;			
			if ( i51AdeOsFileIsExist(path) ) {
				CurrAppNum ++ ;
				if( iNULL==proc(arg, fileinfo.name) )
				{
					goto FIND_END;
				}
					
			}
						
			//CurrAppNum ++ ;
			//if( iNULL==proc(arg, fileinfo.name) )
			//{
			//	goto FIND_END;
			//}

			//	Jelo Add Finished
			
		}	
		
	}while ( i51AdeOsFileFindNext(file , &fileinfo)) ;

FIND_END:	
	if ( file > 0 )
		i51AdeOsFileFindClose (file) ;

	return CurrAppNum;
	
}

iS32 i51KernelFormatVersion ( iCHAR* ver ) {

	//	author : Jelo 
	//	since : 2012.3.2
	// 	(C) PKIG Tech. Co., Ltd.	
	
	//	11111111 11111111 11111111 11111111
	//	分别存放主版本.次版本.修正版本
	//	最大范围 7.31.255
	
	iS32 version = 0 ;
	iS32 length = 0 ;
	iS32 counter = 0 ;
	iS32 looper = 0 ;
	iS32 times = 0 ;
	iCHAR number [ 4 ] = { 0 } ;
	iS32 shift [3] = { 13 , 8 , 0 } ; 

	length = strlen(ver) ;
	
	for ( counter = 0 ; counter < length ; ) {	

		memset ( number , 0 , sizeof(number) ) ;
		for ( looper = counter ; ver[looper] != '.' && ver[looper] != 0 ; looper ++ ) {

			number [ looper-counter ] = ver [ looper ] ;
			
		}
		
		// skip '.'
		counter = looper+1 ;

		version = version | (atoi ( number ) << shift[times]) ;
		times = times + 1 ;
		
	}

	return version ;
	
	
}

iBOOL i51KernelInit ( void (*callback)(iBOOL results) ) {

	//	author : Jelo 
	//	since : 2011.8.24
	// 	(C) PKIG Tech. Co., Ltd.		

	//	返回值表示函数调用成功
	//	初始化结果通过回调传递

	iU16 width = 0 ;
	iU16 height = 0 ;

	//	Jelo Edting Since 2011.12.22
	//	#ifdef i51KERNEL_DEBUG
	//	i51AdeOsLog ( i51_LOG_SC , "i51KernelInit S , 0x%x" , callback ) ;
	//	#endif
	//	Jelo Edting Finished
	
	KernelTimerCallback = (i51KERNELTIMERCALLBACK ) callback ;

	//	Jelo Edting Since 2011.12.22
	//	i51AdeMmiGetScreenScale ( &width , &height ) ;
	//	Jelo Edting Finished

	if ( i51_KERNEL_INIT_GOING == i51Kernel.InitStatus ) {
		return iTRUE ;
	} else if ( i51_KERNEL_INIT_SUCCEED == i51Kernel.InitStatus ) {
		i51AdeOsTimerStart ( i51AdeOsTimerCreate ( iFALSE , 1 ) , i51KernelTimerPositiveCallback , 30 ) ;
		return iTRUE ;
	}

	//	Jelo Adding Since 2011.12.22
	//	首先选盘

	memset ( &i51Kernel , 0 , sizeof(i51KERNEL) ) ;
	
	i51KernelChooseDiskMode () ;
	
	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "i51KernelInit S , 0x%x" , callback ) ;
	#endif

	i51AdeMmiGetScreenScale ( &width , &height ) ;
	//	Jelo Adding Finished
	
	i51Kernel.InitStatus = i51_KERNEL_INIT_GOING ;
	
	i51AdeOsLog ( i51_LOG_SC , "i51 : Hello There" ) ;
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Init Start" ) ;
	i51AdeOsLog ( i51_LOG_SC , "KEL : Kernel Version %s" , i51_KERNEL_VERSION ) ;
	
	if ( i51_ADE_CNF_VER_MTK == i51AdeCnfGetType() ) {		
		i51AdeOsLog ( i51_LOG_SC , "KEL : Adapter MTK , W %d H %d" , width , height ) ;
	} else if ( i51_ADE_CNF_VER_MSTAR == i51AdeCnfGetType() ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Adapter MSTAR , W %d H %d" , width , height ) ;
	} else if ( i51_ADE_CNF_VER_SPRD == i51AdeCnfGetType() ) {	
		i51AdeOsLog ( i51_LOG_SC , "KEL : Adapter SPRD , W %d H %d" , width , height ) ;
	}	 else if ( i51_ADE_CNF_VER_CS == i51AdeCnfGetType() ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Adapter CS , W %d H %d" , width , height ) ;
	}
	i51AdeOsLog ( i51_LOG_SC , "KEL : Adapter Version %s" , i51AdeCnfGetVersionEx () ) ;
    


	//	Jelo Edting Since 2011.12.22
	//	memset ( &i51Kernel , 0 , sizeof(i51KERNEL) ) ;
	//	Jelo Edting Finished
	
	#ifdef i51PROFILINGS
		i51Kernel.TotallMemory = i51AdeOsGetTotallMemory () ;
	#endif	

	//	i51KernelChooseDiskMode () ;
		
	i51AdeOsCreate ( i51AdeOsCreateCB ) ; 

	i51Kernel.Pause = iFALSE ;
		
	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "i51KernelInit E" ) ;
	#endif
	
	return iTRUE ;

}

#if 0
iBOOL i51KernelPause () {  
 
	//	author : Jelo 
	//	since : 2011.8.24
	// 	(C) PKIG Tech. Co., Ltd.

	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "i51KernelPause S" ) ;
	#endif
	
	i51KernelSendDapKitMsgEx ( i51_MSG_PAUSE , 0 ) ;

	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "i51KernelPause E" ) ;
	#endif
	
	return iTRUE ;
	
}

iBOOL i51KernelResume () {

	//	author : Jelo 
	//	since : 2011.8.24
	// 	(C) PKIG Tech. Co., Ltd.

	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "i51KernelResume S" ) ;
	#endif	

	i51KernelSendDapKitMsgEx ( i51_MSG_RESUME , 0 ) ;

	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "i51KernelResume E" ) ;
	#endif
	
	return iTRUE ;
	
} 
#endif

iBOOL i51KernelExit ( iU32 mode ) {

	//	author : Jelo 
	//	since : 2011.8.24
	// 	(C) PKIG Tech. Co., Ltd.

	i51AdeOsLog ( i51_LOG_SC , "KEL : i51 Exit S" ) ;
	
	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "i51KernelExit S , 0x%x" , mode ) ;
	#endif	
	
	if ( i51_KERNEL_EXIT_DAP == (i51_KERNEL_EXIT_DAP&mode) ) {

		i51AdeOsLog ( i51_LOG_SC , "KEL : Exit DAP S" ) ;

		i51KernelExitApp () ;
		i51KernelStopTimer ( iFALSE ) ;
		//akilae add since 2012.7.12
//		i51AdeOsLog(0, "KEL: unload i51KitCore");
//		i51KIT_DYNAMIC_KIT_UNLOAD(i51KitCore);
		i51MiniSlDlStop(0);
		//akilae add finished
		
		//	对于SAP来说因为使用的静态内存，
		//	对于它的释放，我们只需要将其在loader中的符号表释放即可。
		//	注意：SapKitList需要保留，因为sap仍然需要kernel的管理
		//	anti-hang for sap
		//{
			
		//	iU32 looper = 0 ; 

		//	for ( looper = 0 ; looper < i51Kernel.SapKitListTotall ; looper ++ ) {
		//		i51SlLoaderUnload ( (iS32) i51Kernel.SapKitList[looper].id ) ;		
		//	}			
		
		//}		
		//i51KernelReleaseFrontEnd () ;	
		i51AdeOsLog ( i51_LOG_SC , "KEL : Exit DAP E , Dap %d , Sober %d" , i51Kernel.DapKitListTotall , i51Kernel.TotallSober ) ;

	} 

	//	只有在关机时才会退出SAP
	if ( i51_KERNEL_EXIT_SAP == (i51_KERNEL_EXIT_SAP&mode)  ) {

		i51AdeOsLog ( i51_LOG_SC , "KEL : Exit SAP S" ) ;

		i51KernelSendSapKitMsg ( i51_MSG_PAUSE , 0 ) ;
		i51KernelSendSapKitMsg ( i51_MSG_EXIT , 0 ) ;

		//	Jelo Edit Since 2012.1.6
		//	anti-hang for sap
		//{
			
		//	iU32 looper = 0 ;
  
		//	for ( looper = 0 ; looper < i51Kernel.SapKitListTotall ; looper ++ ) {
		//		i51KernelKitAntiHang ( i51Kernel.SapKitList[looper].name , iFALSE , iTRUE ) ; 
		//	}			
		
		//}
		//	Jelo Edit Finished
		
		i51AdeMmiRelease () ; 
		i51AdeSocRelease () ;  
		i51AdeSmsRelease () ; 
		i51AdeStdRelease () ; 
		i51AdeOsRelease () ; 
		
		memset ( &i51Kernel , 0 , sizeof(i51Kernel) ) ;
		
		i51AdeOsLog ( i51_LOG_SC , "KEL : Exit SAP E" ) ;
		
	}
	
	//	Jelo Edit Since 2012.12.6
	//	Create RefTable at Next Runing
	//	i51Kernel.NotCreateRefTable = iFALSE ;
	//	Jelo Edit Finished

	//	Jelo Edit Since 2012.4.20
	#if 0
	if ( 0 == i51Kernel.DapKitListTotall && iTRUE == i51SlDSQueueIsEmpty (&i51Kernel.queue) ) {
		i51KernelReleaseFrontEnd () ;
		i51MemoryLeakCheck () ; 
	} else {

		//	清空Kernel 消息队列中的交替加载消息
		//	防止Kernel 退出后继续加载其它应用
		//	因为在Mstar平台中，当KernelExit调用后vendor线程会被释放。
		//	此时调用某些系统接口会引起异常。
		
		i51KERNELREQ* req = 0 ;
		iU16 rear = i51Kernel.queue.rear ;
		iU16 front = i51Kernel.queue.front ;

		i51AdeOsLog ( i51_LOG_SC , "KEL : S Lex Req" ) ;
		
		for ( ; rear != front ; ) {
			
			req = (i51KERNELREQ* ) i51Kernel.queue.space[front] ;
			front = ( front + 1 ) % i51SlDSQueueSpaceMax () ;

			if ( i51_REQ_LOADEX == req->request ) {
				req->request = 0xffffffff ;
				i51AdeOsLog ( i51_LOG_SC , "KEL : Lex Req Droped" ) ;
			}
			
		}

		i51AdeOsLog ( i51_LOG_SC , "KEL : S Lex Req Finished" ) ;
			
	}
	#endif
	//	Jelo Edit Finished
	
	//	Jelo Add Since 2012.4.20
	if ( iFALSE == i51SlDSQueueIsEmpty (&i51Kernel.queue) ) {	

		//	清空消息队列
		
		i51KERNELREQ* req = 0 ;
		iBOOL ret = 0 ;
		
		i51AdeOsLog ( i51_LOG_SC , "KEL : C REQ Queue" ) ;

		while ( iFALSE == i51SlDSQueueIsEmpty (&i51Kernel.queue) ) {

			ret = i51SlDSQueueOut ( &i51Kernel.queue , (iHANDLE)&req ) ;
		
			if ( iTRUE == ret ) {
				if ( 0 != req->buffer ) { 
					i51AdeOsFree ( req->buffer ) ;
				}						
				i51AdeOsFree ( req ) ;
			}
			
		}
		
		i51AdeOsLog ( i51_LOG_SC , "KEL : C REQ Queue Finished" ) ;
			
	}

	i51KernelReleaseFrontEnd () ;
	i51MemoryLeakCheck () ; 
	//	Jelo Add Finished
	
	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "i51KernelExit E") ;
	#endif

	i51Kernel.Pause = iFALSE ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : i51 Exit E" ) ;
	
	return iTRUE ;
	
}

iBOOL i51KernelSendEvent ( iU32 events , iU32 parameter ) {

	//	author : Jelo 
	//	since : 2011.8.24
	// 	(C) PKIG Tech. Co., Ltd.


	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelSendEvent S , 0x%x , 0x%x" , events , parameter ) ;
	#endif
	
	#ifdef i51_LOADER_DBG
	if ( i51_MSG_KEYUP == events ) {
		i51SlLoaderDumpAll () ;
	}  
	#endif

	if ( i51_MSG_PAUSE == events ) {
		i51KernelStopTimer ( iFALSE ) ;
	} else if ( i51_MSG_RESUME == events ) {
		i51KernelStartTimer ( 1000 / i51_KERNEL_DEFAULT_FPS , i51KernelRunTimerLogical ) ;	
	}

	//	Jelo Edited Start 2011.12.2
	//	按键、触屏消息是个特例，它们只能发送给SOBER
	if ( i51_MSG_KEYUP <= events && events <= i51_MSG_PADMOVE ) {

		if ( 1 == i51Kernel.TotallSober ) {		
			i51Kernel.CurrentSoberKit.i51KitMain ( i51Kernel.CurrentSoberKit.id , events , (void*)parameter ) ;
		} else {
			i51KernelSendSoberKitMsg ( events , (void*)parameter ) ;
		}
			
	} else {
	//	其它消息则广播
		i51KernelSendDapKitMsgEx ( events , (void*)parameter ) ;	
	}	
	//	only one sober exist
	//if ( 1 == i51Kernel.TotallSober ) {		
	//	i51Kernel.CurrentSoberKit->i51KitMain ( i51Kernel.CurrentSoberKit->id , events , (void*)parameter ) ;
	//} else {
	//	i51KernelSendSoberKitMsg ( events , (void*)parameter ) ;
	//}
	//	Jelo Edited Finish

	//	send message to static kit
	if ( 0 < i51Kernel.SapKitListTotall ) {
		i51KernelSendSapKitMsg ( events , (void* ) parameter ) ;
	}

	#ifdef i51PROFILINGS
	if ( i51_MSG_PADUP == events ) {

		iU32 x = (parameter&0xffff0000) >> 16 ;
		iU32 y = parameter&0xffff ;
		
		if ( 0 <= x && 30 >= x && 0 <= y && 30 >= y ) { 
			i51Kernel.ShowProfilings = !i51Kernel.ShowProfilings ;
		}
		
	}  
	#endif 

    if ( i51_MSG_PAUSE == events ) {
        i51Kernel.Pause = iTRUE ;  
    } else if ( i51_MSG_RESUME == events ) {
        i51Kernel.Pause = iFALSE ;
    }
    
	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelSendEvent E" ) ;
	#endif
	
	return iTRUE ;   

}

iBOOL i51KernelUnloadKit ( iHANDLE handle ) {

	//	author : Jelo 
	//	since : 2011.9.8
	// 	(C) PKIG Tech. Co., Ltd.

	//	卸载套件

	iU32 looper = 0 ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelUnloadKit S , 0x%x" , handle ) ;
	#endif
	
	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {

		if ( i51_KERNEL_KIT_SCOPE_DYNAMIC == i51Kernel.DapKitList[looper].scope ) {

			//	*.i51 可以没有 i51KitPort
			if ( 0 != i51Kernel.DapKitList[looper].i51KitPort 
				&& (iU32 ) handle == (iU32 ) i51Kernel.DapKitList[looper].i51KitPort () ) {

				#ifdef i51KERNEL_DEBUG	
				{
					iCHAR buffer [ 64 ] = { 0 } ;
					i51AdeStdUnicode2Ascii ( i51Kernel.DapKitList[looper].name , buffer ) ;
					i51AdeOsLog ( i51_LOG_SC , "Unload Kit '%s' , Load Times %d" , buffer , i51Kernel.DapKitList[looper].ldtimes ) ;
				}
				#endif
				i51Kernel.DapKitList[looper].ldtimes -- ;
				
				if ( 0 >= i51Kernel.DapKitList[looper].ldtimes ) {

					//	发送暂停消息
					i51KernelSendKitMsg ( i51Kernel.DapKitList[looper].id , i51_MSG_PAUSE , 0 ) ; 					
					i51KernelKitListDropKit ( (iHANDLE )i51Kernel.DapKitList[looper].id ) ;
								
				}

				#ifdef i51KERNEL_DEBUG	
				i51AdeOsLog ( i51_LOG_SC , "i51KernelUnloadKit E ") ;
				#endif
	
				return iTRUE ;
				
			}		
			
		}
		
	}	

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelUnloadKit BE , Not Found Kit 0x%x" , handle ) ;
	#endif
				
	return iFALSE ;
	

}

iBOOL i51KernelUnloadSysKit ( iHANDLE handle ) {

	//	author : Jelo 
	//	since : 2011.11.5
	// 	(C) PKIG Tech. Co., Ltd.

	//	卸载系统套件，即SAP，它存放于静态内存，不需要释放
	
	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelUnloadSysKit S , 0x%x" , handle ) ;
	#endif

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelUnloadSysKit E" ) ;
	#endif	
	
	return iTRUE ;
	
}

iBOOL i51KernelSetFPS ( iU32 appid , iU32 fps ) {

	//	author : Jelo 
	//	since : 2011.8.25
	// 	(C) PKIG Tech. Co., Ltd.

	iU32 looper = 0 ;
 
	i51AdeOsLog ( i51_LOG_SC , "KEL : Set FPS , 0x%x , %d , 0x%x" , appid , fps , i51Kernel.TotallSober ) ;
	
	if ( 0 == i51Kernel.TotallSober ) {

		i51AdeOsLog ( i51_LOG_SC , "KEL : BE , No Sober Exist" ) ;		
	
		return iFALSE ;
		
	}

	//	Jelo Add 2012.1.5
	//	帧率没有发生改变
	if ( fps == i51Kernel.fps ) {

		i51AdeOsLog ( i51_LOG_SC , "KEL : BE , Same FPS" ) ;	
		
		return iTRUE ;

	}
		
	i51KernelStopTimer ( iFALSE ) ;

	if ( 0 < fps ) {					
		i51KernelStartTimer ( 1000 / fps , i51KernelRunTimerLogical ) ;						
	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : E , FPS Changed" ) ;	
			
	return iTRUE ;
	//	Jelo Add Finished

//	Jelo Edit 2012.1.5		
#if 0
	if ( appid == i51Kernel.CurrentSoberKit->id ) {	

		//	帧率没有发生改变
		if ( fps == i51Kernel.fps ) {
			return iTRUE ;
		}
		
		i51KernelStopTimer ( iFALSE ) ;

		if ( 0 < fps ) {					
			i51KernelStartTimer ( 1000 / fps , i51KernelRunTimerLogical ) ;						
		}

		return iTRUE ;
				
	}

	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {

		if ( appid == i51Kernel.DapKitList[looper].id ) {

			//	帧率没有发生改变
			if ( fps == i51Kernel.fps ) {
				return iTRUE ;
			}
		
			i51Kernel.fps = fps ;

			i51KernelStopTimer ( iFALSE ) ;

			if ( 0 < fps ) {		
				//	当某套件的FPS 大于0时，其状态自动转换为SOBER。
				//	Jelo Edit 2011.12.30
				//	i51Kernel.DapKitList[looper].status = i51_APP_STATUS_SOBER ;
				//	Jelo Edit Finished
				i51KernelStartTimer ( 1000 / fps , i51KernelRunTimerLogical ) ;						
			}
							
			return iTRUE ;

		}		
		
	}	

	return iTRUE ;
#endif	
//	Jelo Edit Finished


}

iBOOL i51KernelMakeSnapshot ( iU32 appid ) {

	//	author : Jelo 
	//	since : 2011.9.16
	// 	(C) PKIG Tech. Co., Ltd.

	iBOOL ret = iFALSE ;
	
	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelMakeSnapshot S , 0x%x" , appid ) ;
	#endif
	
	ret = i51SlLoaderMakeSnapshot ( appid ) ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelMakeSnapshot E , ret = 0x%x" , ret ) ;
	#endif
	
	return ret ;
	
}

iBOOL i51KernelDropSnapshot ( iU32 appid ) {

	//	author : Jelo 
	//	since : 2011.9.16
	// 	(C) PKIG Tech. Co., Ltd.

	iBOOL ret = iFALSE ;
	
	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelDropSnapshot S , 0x%x" , appid ) ;
	#endif
	
	ret = i51SlLoaderDropSnapshot ( appid ) ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelDropSnapshot E , ret = 0x%x" , ret ) ;
	#endif
	
	return ret ;	
	  
}

iBOOL i51KernelSendRequest ( iU32 appid , iU16* name , iU32 request , void* buffer , iU32 buffsize ) {

	//	author : Jelo 
	//	since : 2011.10.10
	// 	(C) PKIG Tech. Co., Ltd.

	i51KERNELREQ* req = 0 ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Send REQ Start , 0x%x , 0x%x , 0x%x , 0x%x" , appid , request , buffer , buffsize ) ;	
	
	req = (i51KERNELREQ* ) i51AdeOsMalloc ( sizeof(i51KERNELREQ) , __FILE__ , __LINE__ ) ;

	if ( 0 == req ) {	
		i51AdeOsLog ( i51_LOG_SC , "KEL : Fatal Error , N MEM For REQ" ) ;	
		return iFALSE ;
	}

	memset ( req , 0 , sizeof(i51KERNELREQ) ) ;
		
	if ( 0 != name ) {
		i51AdeStdWStrcpy ( req->name , name ) ;
	}
	
	req->request = request ;
	req->sender = appid ;
	req->object = 0 ;
	req->buffer = 0 ;

	if ( 0 != buffer ) {
	
		req->buffer = (iU8* ) i51MALLOC ( buffsize ) ;

		if ( 0 == req->buffer ) {
			
			i51AdeOsLog ( i51_LOG_SC , "KEL : Fatal Error , N MEM For REQ 2" ) ;
			req->buffsize = 0 ;
			
		} else {

			memcpy ( req->buffer , buffer , buffsize ) ;
			req->buffsize = buffsize ;
			
		}
  
	}
	
	if ( iFALSE == i51SlDSQueueIn ( &i51Kernel.queue , (iHANDLE)req ) ) {
		
		i51AdeOsLog ( i51_LOG_SC , "KEL : REQ Queue Full" ) ;	
		
		if ( 0 != req->buffer ) {
			i51AdeOsFree ( req->buffer ) ;
		}	
		
		i51AdeOsFree ( req ) ;
		
		return iFALSE ;
		
	}

	//	start timer
	if ( iFALSE == i51Kernel.ReqTimerRuning ) {
		i51Kernel.ReqTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
		i51AdeOsTimerStart ( i51Kernel.ReqTimer , i51KernelDispatch , 5 ) ;
		i51Kernel.ReqTimerRuning = (i51Kernel.ReqTimer > 0) ;
	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : Send REQ Finished , Rtr : %d , Tid : %d" , i51Kernel.ReqTimerRuning , i51Kernel.ReqTimer ) ;	
	
	return iTRUE ;
	
}

iBOOL i51KernelSendMessage ( iU32 srcid , iU32 desid , iU32 message , void* buffer , iU32 buffsize ) {

	//	author : Jelo 
	//	since : 2011.10.10
	// 	(C) PKIG Tech. Co., Ltd.

	i51KERNELREQ* req = 0 ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Send MSG Start , 0x%x , 0x%x , 0x%x , 0x%x , 0x%x" , srcid , desid , message , buffer , buffsize ) ;	
	
	req = (i51KERNELREQ* ) i51AdeOsMalloc ( sizeof(i51KERNELREQ) , __FILE__ , __LINE__ ) ;

	if ( 0 == req ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Fatal Error , N MEM For REQ" ) ;
		return iFALSE ;
	}

	memset ( req , 0 , sizeof(i51KERNELREQ) ) ;
	
	req->request = message ;
	req->sender = srcid ;
	req->object = desid ;
	req->buffer = 0 ;

	if ( 0 != buffer ) {
	
		req->buffer = (iU8* ) i51MALLOC ( buffsize ) ;

		if ( 0 == req->buffer ) {
			
			i51AdeOsLog ( i51_LOG_SC , "KEL : Fatal Error , N MEM For REQ 2" ) ;
			req->buffsize = 0 ;
			
		} else {

			memcpy ( req->buffer , buffer , buffsize ) ;
			req->buffsize = buffsize ;
			
		}

	}

	if ( iFALSE == i51SlDSQueueIn ( &i51Kernel.queue , (iHANDLE)req ) ) {

		i51AdeOsLog ( i51_LOG_SC , "KEL : MSG Queue Full" ) ;	
		
		if ( 0 != req->buffer ) {
			i51AdeOsFree ( req->buffer ) ;
		}
		
		i51AdeOsFree ( req ) ;
		
		return iFALSE ;
		
	}

	//	start timer
	if ( iFALSE == i51Kernel.ReqTimerRuning ) {
		i51Kernel.ReqTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
		i51AdeOsTimerStart ( i51Kernel.ReqTimer , i51KernelDispatch , 5 ) ;
		i51Kernel.ReqTimerRuning = (i51Kernel.ReqTimer > 0) ;
	}	

	i51AdeOsLog ( i51_LOG_SC , "KEL : Send MSG Finished" ) ;	
	
	return iTRUE ;
	
}

iU16* i51KernelGetApplicationPath () {

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	//	获取目录
	//	如果当前是系统目录模式，则从系统盘符寻找
	//	其它情况从T卡盘符寻找

	iCHAR path8bit [ 64 ] = { 0 } ;		
	static iU16 path16bit [ 64 ] = { 0 } ; 

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelGetApplicationPath S" ) ;
	#endif
	
	memset ( path16bit , 0 , sizeof(path16bit) ) ;

	if ( i51_KERNEL_DISK_MODE_MOVABLE == i51Kernel.DiskMode ) {
		sprintf ( path8bit , "%si51\\i51KIT\\" , i51AdeOsGetPathSymbolEx () ) ;	
	} else if ( i51_KERNEL_DISK_MODE_SYSTEM == i51Kernel.DiskMode ) {
		sprintf ( path8bit , "%si51\\i51KIT\\" , i51AdeOsGetPathSymbol () ) ;	
	}
	
    #ifdef i51KERNEL_DEBUG	
  	i51AdeOsLog ( i51_LOG_SC , "KEL : Kit Path %s" , path8bit ) ;	
	#endif
	i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelGetApplicationPath E" ) ;
	#endif	

	return path16bit ;
	
}

iU16* i51KernelGetSystemPath () {

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	//	获取目录
	//	如果当前是系统目录模式，则从系统盘符寻找
	//	其它情况从T卡盘符寻找

	iCHAR path8bit [ 64 ] = { 0 } ;		
	static iU16 path16bit [ 64 ] = { 0 } ; 

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelGetSystemPath S" ) ;
	#endif	
	
	memset ( path16bit , 0 , sizeof(path16bit) ) ;
	
	if ( i51_KERNEL_DISK_MODE_MOVABLE == i51Kernel.DiskMode ) {
		sprintf ( path8bit , "%si51\\i51SYS\\" , i51AdeOsGetPathSymbolEx () ) ;	
	} else if ( i51_KERNEL_DISK_MODE_SYSTEM == i51Kernel.DiskMode ) {
		sprintf ( path8bit , "%si51\\i51SYS\\" , i51AdeOsGetPathSymbol () ) ;	
	}

	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( i51_LOG_SC , "KEL : Sys Path %s" , path8bit ) ;	
	#endif
	
	i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelGetSystemPath E" ) ;
	#endif	
	
	return path16bit ;
	
}

//	jelo Edited 20120314
/*
iBOOL i51KernelCheckElder ( iU16* floder ) {

	//	author : Jelo 
	//	since : 2011.10.11
	// 	(C) PKIG Tech. Co., Ltd.

	iBOOL SysFileExist = iFALSE ;
	iBOOL ret = iFALSE ;
	iU32 length = 0 ;
	iU16 path [ 256 ] = { 0 } ;
	iU16* ppath = 0 ;
	iU16 Tpath [ 256 ] = { 0 } ;
	iU16* Tppath = 0 ;	

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelCheckElder S" ) ;
	#endif
	
	iASSERT(0!=floder);

	//	拼接系统盘路径
	length = i51AdeStdWStrlen (i51KernelGetApplicationPathEx () ) ;
	length += i51AdeStdWStrlen ( floder ) ;
	length += 2 ;

	iASSERT ( length < sizeof(path) ) ;
			
	ppath = i51AdeStdWStrcpy ( path , i51KernelGetApplicationPathEx () ) ;
	ppath = i51AdeStdWStrcat ( ppath , floder ) ;

	i51AdeOsFileAModeEnable () ;
	SysFileExist = i51AdeOsFileIsDirExist ( ppath ) ;
	i51AdeOsFileAModeDisable () ;	

	//	查看系统盘是否有数据需要复制
	if ( iTRUE == SysFileExist ) {

		//	拼接T卡路径			
		Tppath = i51AdeStdWStrcpy ( Tpath , i51KernelGetApplicationPath () ) ;
		Tppath = i51AdeStdWStrcat ( Tppath , floder ) ;		
		
		//	将系统盘数据复制到T卡
		ret = i51AdeOsFileCopy ( ppath , Tppath ) ;

		#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelCheckElder BE" ) ;
		#endif
	
		return ret ;	

	}	

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelCheckElder E" ) ;
	#endif
	
	return SysFileExist ;

}
*/
//	Jelo Edited Finished

iU32 i51KernelGetInitStatus () {

	//	author : Jelo 
	//	since : 2011.11.8
	// 	(C) PKIG Tech. Co., Ltd.	

	//	返回初始化状态

	iU32 ret = 0 ;
	
	ret = i51Kernel.InitStatus ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Status 0x%x" , ret ) ;
	
	return ret ;
	
}

iS32 i51KernelRunStaticApp ( iU16* name , iU8* buffer , iU32 buflen , void* parameter , iBOOL staticsope , iBOOL frontapp ) {


	//	author : Jelo 
	//	since : 2011.10.18
	// 	(C) PKIG Tech. Co., Ltd.

	//	该接口只能加载i51 应用程序，无法加载KIT

	iS32 ret = 0 ;
	iS32 dll = 0 ;
	iFILE file = 0 ;  
	iS32 length = 0 ;
	iU32 scope = 0 ;
	iU16 path [ 256 ] = { 0 } ;
	iU16* ppath = 0 ;
	iU32 status = i51_APP_STATUS_SLEEP ;

	i51KERNELKIT* i51Kit = 0 ;
	i51KITMAIN kitmain = 0 ;

	{
		i51AdeStdUnicode2Ascii ( name , (iCHAR* ) path ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Run SAP '%s' , 0x%x , 0x%x , 0x%x , 0x%x , 0x%x" , path , buffer , buflen , parameter , staticsope , frontapp ) ;
	}
	
	if ( i51_KERNEL_INIT_SUCCEED != i51Kernel.InitStatus ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Fatal Error , Kernel Is Not Ready" ) ;
		return i51_KERNEL_LOAD_ERR_MEM ;
	}

	//	create dir if it was formated
	i51KernelCreateDir () ;

	//	需要静态存储区域
	if ( iTRUE == staticsope ) {
		scope = i51_KERNEL_KIT_SCOPE_STATIC ;	
	} else {
		scope = i51_KERNEL_KIT_SCOPE_DYNAMIC ;		
	}

	if ( iTRUE == frontapp ) {
		status = i51_APP_STATUS_SOBER ;
	}
	
#ifdef i51SIMULATOR
{

	if ( i51_ADE_CNF_VER_SPRD == i51AdeCnfGetType () ) {
		
#if 0		
		DWORD WriteSize = 0;
		HANDLE file = 0 ;
			
		CreateDirectoryW ( L"i51" , 0 ) ;
		CreateDirectoryW ( L"i51\\i51KIT" , 0 ) ;

		
//		ppath = i51AdeStdWStrcpy ( path , L".\\i51\\i51KIT\\" ) ;
		ppath = i51AdeStdWStrcpy ( path , L"i51\\i51KIT\\" ) ;
		ppath = i51AdeStdWStrcat ( ppath , name ) ;

		CreateDirectoryW ( ppath , 0 ) ;
		
		ppath = i51AdeStdWStrcat ( ppath , L"\\" ) ;
		ppath = i51AdeStdWStrcat ( ppath , name ) ;
		ppath = i51AdeStdWStrcat ( ppath , L".dll" ) ;

		file = CreateFile( ppath , GENERIC_READ , 0 , 0 , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , 0 ) ;

		if ( 0 == file ) {
			file = CreateFileW ( ppath , GENERIC_WRITE , FILE_SHARE_WRITE , 0 , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , 0 ) ;		
			WriteFile ( file , buffer , buflen , &WriteSize , 0 ) ;
		}
		
		CloseHandle ( file ) ;	
	#else
		FILE *file = 0 ;
		char apppath[64] = {0} ;
		char appname[32] = {0} ;
		CreateDirectoryW ( L"i51" , 0 ) ;
		CreateDirectoryW ( L"i51\\i51KIT" , 0 ) ;
		
		ppath = i51AdeStdWStrcpy ( path , L".\\i51\\i51KIT\\" ) ;
		ppath = i51AdeStdWStrcat ( ppath , name ) ;
		CreateDirectoryW ( ppath , 0 ) ;
		
		i51AdeStdUnicode2Ascii ( name , appname ) ;
		strcpy ( apppath , "i51\\i51KIT\\" ) ;
		strcat ( apppath , appname ) ;
		strcat ( apppath , "\\" ) ;
		strcat ( apppath , appname ) ;
		strcat ( apppath , ".dll" ) ;

		file = fopen(apppath , "rb") ;
		if ( file <=0 ) {

			file = fopen (apppath , "wb") ;
			fwrite(buffer , buflen , 1 , file) ;
			
		}
		fclose(file) ;
	#endif
									  
		
	} else {
	
		if ( iFALSE == i51AdeOsFileIsDirExist ( name ) ) {
			
			ret = i51AdeOsFileCreateDir ( name ) ;

			if ( iFALSE == ret ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Run Static App , F1" ) ;
				return iFALSE ;
			}
		
			ppath = i51AdeStdWStrcpy ( path , name ) ;
			ppath = i51AdeStdWStrcat ( ppath , L"\\" ) ;
			ppath = i51AdeStdWStrcat ( ppath , name ) ;
			ppath = i51AdeStdWStrcat ( ppath , L".dll" ) ;

			file = i51AdeOsFileOpen ( ppath , i51_OS_FL_WRITE|i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;

			if ( 0 == file ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Run Static App , F2" ) ;
				return iFALSE ;
			}
				
			ret = i51AdeOsFileWrite ( file , buffer , buflen , 0 ) ;
				
			if ( iFALSE == ret ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : Run Static App , F3" ) ;
				i51AdeOsFileClose ( file ) ;
				return iFALSE ;	
			}	

			i51AdeOsFileClose ( file ) ;	

		}
		
	}
	
	if ( i51_ADE_CNF_VER_MTK == i51AdeCnfGetType () ) {
		
		iU16 dllpath [ 256 ] = { L"..\\..\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , name ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , name ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;

		dll = i51SlLoaderLoad ( name , dllpath , 0 , 0 ) ;
		
		//dll = LoadLibraryW ( dllpath ) ;

	} else if ( i51_ADE_CNF_VER_MSTAR == i51AdeCnfGetType () ) {
	
		iU16 dllpath [ 256 ] = { L".\\SimFileSys\\CARD\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , name ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , name ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;

		dll = i51SlLoaderLoad ( name , dllpath , 0 , 0 ) ;
		
		//dll = LoadLibraryW ( dllpath ) ;
		
	}  else if ( i51_ADE_CNF_VER_SPRD == i51AdeCnfGetType () ) {

		iU16 dllpath [ 256 ] = { L".\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , name ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , name ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;

		dll = i51SlLoaderLoad ( name , dllpath , 0 , 0 ) ;
	}else if ( i51_ADE_CNF_VER_CS == i51AdeCnfGetType () ) {
		
		iU16 dllpath [ 256 ] = { L"..\\Bin\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , name ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , name ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;

		dll = i51SlLoaderLoad ( name , dllpath , 0 , 0 ) ;
	}

	if ( 0 == dll ) {  
		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S2" ) ; 
		return i51_KERNEL_LOAD_ERR_NE ;
	}

	kitmain = (i51KITMAIN ) i51SlLoaderGetSymbol ( dll , "i51KitMain" ) ;

	iASSERT(kitmain) ;

	i51Kit = (i51KERNELKIT* ) i51KernelKitListCreateKit ( name , kitmain , 0 , (iHANDLE)dll , status , iFALSE , scope , 22 ) ;
	
	if ( 0 == i51Kit ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 3" ) ;	
		FreeLibrary ( dll ) ;
		return i51_KERNEL_LOAD_ERR_MEM ;
	}

	//	Jelo Edit Start 2011.11.26
	//	create front end when timer started
	//	if ( iFALSE == i51KernelCreateFrontEnd () ) {			
	//		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 4" ) ;	 
	//		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;
	//		return i51_KERNEL_LOAD_ERR_FE ;
	//	}
	//	Jelo Edit Finish

	//	Jelo Add Start 2011.12.26
	if ( iTRUE == frontapp ) {
		//	为解决应用在i51_MSG_INIT 调用MMI接口时所出现的问题。
		//	创建前台环境要在i51_MSG_INIT
		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;
	}
	//	Jelo Add Finish	

	if ( iFALSE == i51Kit->i51KitMain ( i51Kit->id , i51_MSG_INIT , (void*)i51KIT_STATIC_KIT_GET_MAPING(Adapter) ) ) {
		
		i51AdeOsLog ( i51_LOG_SC , "KEL : i51KitMain F , S 5" ) ;
		//	发送暂停消息
		i51KernelSendKitMsg ( i51Kit->id , i51_MSG_PAUSE , 0 ) ; 			
		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;

		//	Jelo Add Start 2011.12.26
		//	释放前台环境 
		if ( 0 == i51Kernel.TotallSober ) {
			i51KernelReleaseFrontEnd () ;					
		}
		//	Jelo Edit Finish
		
		return i51_KERNEL_LOAD_ERR_MEM ;
		
	}

	//	Jelo Edit Start 2011.12.26
	//	if ( iTRUE == frontapp ) {
		//	需要在RUN 消息之前创建前台环境
		//	否则某些平台会出现未定义错误。
	//		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;
	//	}
	
	i51Kit->i51KitMain ( i51Kit->id , i51_MSG_RUN , parameter ) ;
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Run SAP Finished" ) ;
	
	return i51_KERNEL_LOAD_ERR_NO ;
	
}
#else
{

	//	对于静态app需要有一个统一内存管理的机制
	//	以下是临时方案。
	if ( 0 == i51AdeStdWStrcmp ( name , L"AppBoot") ) {
		
		static iU32 bootloaded = 0 ;

		if ( iTRUE == bootloaded ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : AppBoot Already Loaded" ) ;
			return i51_KERNEL_LOAD_ERR_NO ;
		} 

		dll = i51SlLoaderLoadEx ( name , buffer , buflen , i51KernelStaticMemoryAppBoot , sizeof(i51KernelStaticMemoryAppBoot) ) ;

		if ( 0 == dll ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 2" ) ;
			return i51_KERNEL_LOAD_ERR_NE ;
		}

		kitmain = (i51KITMAIN ) i51SlLoaderGetSymbol ( dll , "i51KitMain" ) ;
		//	对于AppBoot来说，加载后立即释放其占用的SlLOADER 内存
		i51SlLoaderUnload ( (iS32) dll ) ;			
		if ( 0 == kitmain ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , i51KitMain F" ) ;				
			return i51_KERNEL_LOAD_ERR_NE ;
		}
		
		bootloaded = iTRUE ;

	} else {

		dll = i51SlLoaderLoadEx ( name , buffer , buflen , 0 , 0 ) ;

		if ( 0 == dll ) {
			i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 2" ) ;
			return i51_KERNEL_LOAD_ERR_NE ;
		}
		
		kitmain = (i51KITMAIN ) i51SlLoaderGetSymbol ( dll , "i51KitMain" ) ;		
		if ( 0 == kitmain ) {	
			i51SlLoaderUnload ( (iS32) dll ) ;				
			i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , i51KitMain F" ) ;	
			return i51_KERNEL_LOAD_ERR_NE ;
		}

	}
 	
	i51Kit = (i51KERNELKIT* ) i51KernelKitListCreateKit ( name , kitmain , 0 , (iHANDLE)dll , status , iFALSE , scope , 22 ) ;
	
	if ( 0 == i51Kit ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 3" ) ;	
		i51SlLoaderUnload ( dll ) ;
		return i51_KERNEL_LOAD_ERR_MEM ; 
	}

	//	Jelo Edit Start 2011.11.26
	//	create front end when timer started	
	//	if ( iFALSE == i51KernelCreateFrontEnd () ) {
	//		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 4" ) ;	 
	//		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;
	//		return i51_KERNEL_LOAD_ERR_FE ;
	//	}
	//	Jelo Edit Finish

	//	Jelo Add Start 2011.12.26
	if ( iTRUE == frontapp ) {
		//	为解决应用在i51_MSG_INIT 调用MMI接口时所出现的问题。
		//	创建前台环境要在i51_MSG_INIT
		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;
	}
	//	Jelo Add Finish	
	
	if ( iFALSE == i51Kit->i51KitMain ( i51Kit->id , i51_MSG_INIT , (void*)i51KIT_STATIC_KIT_GET_MAPING(Adapter) ) ) {
		
		i51AdeOsLog ( i51_LOG_SC , "KEL : i51KitMain F , S 5" ) ;	
		//	发送暂停消息
		i51KernelSendKitMsg ( i51Kit->id , i51_MSG_PAUSE , 0 ) ; 	
		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;

		//	Jelo Add Start 2011.12.26
		//	释放前台环境 
		if ( 0 == i51Kernel.TotallSober ) {
			i51KernelReleaseFrontEnd () ;	 
		}
		//	Jelo Edit Finish
		
		return i51_KERNEL_LOAD_ERR_MEM ;

	}
 
	//	Jelo Edit Start 2011.12.26
	//	if ( iTRUE == frontapp ) {
		//	需要在RUN 消息之前创建前台环境
		//	否则某些平台会出现未定义错误。
	//		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;
	//	}

	i51Kit->i51KitMain ( i51Kit->id , i51_MSG_RUN , parameter ) ;
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Run SAP Finished" ) ;
	
	return i51_KERNEL_LOAD_ERR_NO ;	
}
#endif


}

iS32 i51KernelRunDynamicApp ( iU16* name , void* parameter ) {

	//	author : Jelo 
	//	since : 2011.10.20
	// 	(C) PKIG Tech. Co., Ltd.

	//	该接口只能加载i51 应用程序，无法加载KIT
	
	iBOOL ret = iFALSE ; 

	i51AdeOsLog ( i51_LOG_SC , "KEL : Run DAP 0x%x , 0x%x" , name , parameter ) ;
	
	if ( i51_KERNEL_INIT_SUCCEED != i51Kernel.InitStatus ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Fatal Error , Kernel Is Not Ready" ) ;
		return iFALSE ;
	}	
  
	//	create dir if it was formated
	i51KernelCreateDir () ;

	//	jelo Edited 20120314
	//if ( i51_KERNEL_DISK_MODE_MOVABLE == i51Kernel.DiskMode ) {
	//	i51KernelCheckElder ( name ) ;
	//}
	//	jelo Edited Finished
 
	ret = i51KernelLoadApp ( name , parameter , iTRUE ) ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Run DAP , ret = 0x%x" , ret ) ;
	
	return ( ret == i51_KERNEL_LOAD_ERR_NO ); ;
	
}

iS32 i51KernelRunDynamicAppEx ( iU16* name , void* parameter ) {

	//	author : Jelo 
	//	since : 2011.12.22
	// 	(C) PKIG Tech. Co., Ltd.

	//	加载APP，如果不存在则首先安装。
	
	iBOOL ret = iFALSE ; 

	i51AdeOsLog ( i51_LOG_SC , "KEL : Run Dap & Install 0x%x , 0x%x" , name , parameter ) ; 
	
	if ( iFALSE == i51KernelCheckAppCompleten ( (iU16* ) name ) ) {

		//extern const iU8* i51SapGeti51Installer () ;
		//extern const iS32 i51SapGeti51InstallerSize () ;

		i51AdeOsLog ( i51_LOG_SC , "KEL : App COMP F" ) ; 
		i51AdeOsLog ( i51_LOG_SC , "KEL : Installer S" ) ; 
			
		//	不完整则安装
		#if 0
		ret = i51KernelRunStaticApp ( L"i51Installer" , (iU8* ) i51SapGeti51Installer () , (iU32 )i51SapGeti51InstallerSize () ,  (void* ) name , 0 , iTRUE ) ;
		#else
			ret = i51KernelCallSapAppMenu(name);		//akilae add 2012.7.16
		#endif
				
		i51AdeOsLog ( i51_LOG_SC , "KEL : Installer E , 0x%x" , ret ) ;  
			
	} else {

		//	完整则加载
		ret = i51KernelRunDynamicApp ( (iU16* ) name , parameter ) ;  

		if ( 0 == ret ) { 
			i51AdeOsLog ( i51_LOG_SC , "KEL : LOADEX F" ) ; 
			i51KernelSetCurrentSorber () ;  
		}

	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : Run Dap & Install Finished , ret = 0x%x" , ret ) ;
	
	return ( ret == i51_KERNEL_LOAD_ERR_NO ); ;
	
}

iU32* i51KernelLoadKit ( iU16* kitpath ) {

	//	author : Jelo 
	//	since : 2011.9.8
	// 	(C) PKIG Tech. Co., Ltd.	

	iU32 length = 0 ;
	iU16 name [ 32+1 ] = { 0 } ;
	iU16 path [ 64 ]  = { 0 } ;
	iU16* ppath = 0 ;
	iS32 dll = 0 ;
	iBOOL ret = iFALSE ;
	iFILE file = 0 ;
		
	i51KITMAIN i51KitMain = 0 ;
	i51KITPORT i51KitPort = 0 ;
	i51KERNELKIT* i51Kit = 0 ;

	{
		i51AdeStdUnicode2Ascii ( kitpath , (iCHAR* ) path ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Load Kit '%s'" , path ) ;
	}
		
	i51KernelGetFileName ( kitpath , name ) ;
	
	i51KitPort = (i51KITPORT ) i51SlLoaderGetSymbolEx ( name , "i51KitPort" ) ;

	if ( 0 != i51KitPort ) {

		//	加载次数+1
		i51Kit = i51KernelKitListGetKit ( name ) ;
		
		if ( 0 != i51Kit ) {
			i51Kit->ldtimes ++ ;
		}

		i51AdeOsLog ( i51_LOG_SC , "KEL : Alread Loaded , Times = %d" , i51Kit->ldtimes ) ;
	
		return i51KitPort () ;

	}

	length = i51AdeStdWStrlen (i51KernelGetApplicationPath () ) ;
	length += i51AdeStdWStrlen ( kitpath ) + 2 ;	
	iASSERT ( length < sizeof(path) ) ;

	ppath = i51AdeStdWStrcpy ( path , i51KernelGetApplicationPath  () ) ;	
	ppath = i51AdeStdWStrcat ( ppath , kitpath ) ;

	//	模拟器
	#ifdef i51SIMULATOR
	{
		
		iU16 pathnew [ 128 ] = { 0 } ;
		iU16* path = 0 ;

		if ( i51_ADE_CNF_VER_MTK == i51AdeCnfGetType () ) {

			path = (iU16* )i51MALLOC ( 
				(strlen("..\\..\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\")+\
				(i51AdeStdWStrlen(kitpath) ) + 1) << 1 ) ;
				
			i51AdeStdWStrcpy ( path , L"..\\..\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\" ) ;

			i51KernelSwap2DllName ( kitpath , pathnew ) ;			
			i51AdeStdWStrcat ( path , pathnew ) ;
			
			dll = i51SlLoaderLoad ( name , path , 0 , 0 ) ;					

		} else if ( i51_ADE_CNF_VER_MSTAR == i51AdeCnfGetType () ) {

			iCHAR path8bit [ 128 ] = { 0 } ;
			iCHAR path16bit [ 128 ] = { 0 } ;

			sprintf ( path8bit , ".\\SimFileSys%si51\\i51KIT\\" , i51AdeOsGetPathSymbolEx () ) ;
			i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;
				
			path = (iU16* )i51MALLOC ( 
				(strlen(path8bit)+\
				(i51AdeStdWStrlen(kitpath) ) + 1) << 1 ) ;
				
			i51AdeStdWStrcpy ( path , path16bit ) ;
			i51KernelSwap2DllName ( kitpath , pathnew ) ;			
			i51AdeStdWStrcat ( path , pathnew ) ;
				
			dll = i51SlLoaderLoad ( name , path , 0 , 0 ) ;		
			
		} else if ( i51_ADE_CNF_VER_SPRD == i51AdeCnfGetType () ) {

			path = (iU16* )i51MALLOC ( 
				(strlen(".\\i51\\i51KIT\\")+\
				(i51AdeStdWStrlen(kitpath) ) + 1) << 1 ) ;
				
			i51AdeStdWStrcpy ( path , L".\\i51\\i51KIT\\" ) ;

			i51KernelSwap2DllName ( kitpath , pathnew ) ;			
			i51AdeStdWStrcat ( path , pathnew ) ;
			
			dll = i51SlLoaderLoad ( name , path , 0 , 0 ) ;			
			
		}else if ( i51_ADE_CNF_VER_CS == i51AdeCnfGetType () ) {

			path = (iU16* )i51MALLOC ( 
				(strlen("..\\Bin\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\")+\
				(i51AdeStdWStrlen(kitpath) ) + 1) << 1 ) ;
				
			i51AdeStdWStrcpy ( path , L"..\\Bin\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\" ) ;

			i51KernelSwap2DllName ( kitpath , pathnew ) ;			
			i51AdeStdWStrcat ( path , pathnew ) ;
			
			dll = i51SlLoaderLoad ( name , path , 0 , 0 ) ;					

		}

		i51FREE ( path ) ;			
						  
	}
	#else
	{
	//	真机	
		dll = i51SlLoaderLoad ( name , ppath , 0 , 0 ) ;	
	}
	#endif

	if ( 0 == dll ) {			
		i51AdeOsLog ( i51_LOG_SC , "KEL : Kit Exist F" ) ;
		return 0 ;
	}

	i51KitMain = (i51KITMAIN ) i51SlLoaderGetSymbol ( dll , "i51KitMain" ) ;	
	i51KitPort = (i51KITPORT ) i51SlLoaderGetSymbol ( dll , "i51KitPort" ) ;
	
	if ( 0 == i51KitMain ) {
		return 0 ;
	}
	
	if ( 0 == i51KitPort ) {
		return 0 ;
	}

	i51Kit = (i51KERNELKIT* ) i51KernelKitListCreateKit ( name , i51KitMain , i51KitPort , (iHANDLE)dll , 
			i51_APP_STATUS_SLEEP , iTRUE , i51_KERNEL_KIT_SCOPE_DYNAMIC , 0 ) ;

	if ( 0 == i51Kit ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : FE , CreateKit F" ) ;
		return 0 ;
	}

	//	William Add Start 2012.7.10
	#ifdef __SPRD_CLEAR_CACHE__
	i51KernelDump ( L"Kernel.dump" , i51Kit->id , 4 ) ;
	#endif
	//	William Add Finishi

	ret = i51Kit->i51KitMain ( i51Kit->id , i51_MSG_INIT , (void*)i51KIT_STATIC_KIT_GET_MAPING(Adapter) ) ;

	if ( iFALSE == ret ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Kit i51KitMain F" ) ; 
		//	发送暂停消息
		i51KernelSendKitMsg ( i51Kit->id , i51_MSG_PAUSE , 0 ) ; 			
		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;
		return iFALSE ;
	}

	//	William Add Start 2012.7.10
	#ifdef __SPRD_CLEAR_CACHE__
	i51KernelDump ( L"Kernel.dump" , i51Kit->id , 4 ) ;
	#endif
	//	William Add Finishi



	i51Kit->i51KitMain ( i51Kit->id , i51_MSG_RUN , 0 ) ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Load Kit Finished , Id 0x%x , Handle 0x%x" , i51Kit->id , i51Kit->i51KitPort () ) ;

	//	William Add Start 2012.7.10
	#ifdef __SPRD_CLEAR_CACHE__
	i51KernelDump ( L"Kernel.dump" , i51Kit->id , 4 ) ;
	{
		iU32 *ret = 0 ;
		ret = i51Kit->i51KitPort () ;
		i51KernelDump ( L"Kernel.dump" , i51Kit->id , 4 ) ;
		return ret ;
	}
	#else
	return i51Kit->i51KitPort () ; //Wiiliam modify with 2012.7.11
	#endif
	//	William Add Finish
	

}

iU32* i51KernelLoadSysKit ( iU16* kitpath ) {
	
	//	author : Jelo 
	//	since : 2011.11.2
	// 	(C) PKIG Tech. Co., Ltd.		

	//	加载系统套件
	//	i51SYS\\

	iU32 length = 0 ;
	iU16 name [ 32+1 ] = { 0 } ;
	iU16 path [ 64 ]  = { 0 } ;
	iU16* ppath = 0 ;
	iS32 dll = 0 ;
	iBOOL ret = iFALSE ;
	iFILE file = 0 ;
	iBOOL reload = iFALSE ;
		
	i51KITMAIN i51KitMain = 0 ;
	i51KITPORT i51KitPort = 0 ;
	i51KERNELKIT* i51Kit = 0 ;

	{
		i51AdeStdUnicode2Ascii ( kitpath , (iCHAR* ) path ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Load Sys-Kit '%s'" , path ) ;
	}
	
	i51KernelGetFileName ( kitpath , name ) ;

	{

		//	检查该KIT是否已经加载过
		
		iU32 looper = 0 ;
  
		for ( looper = 0 ; looper < i51Kernel.SapKitListTotall ; looper ++ ) {
			
			if ( 0 == i51AdeStdWStrcmp ( i51Kernel.SapKitList[looper].name , name ) ) {
				if ( 0 != i51Kernel.SapKitList[looper].i51KitPort ) {

					//	Jelo Edit Since 2012.4.16
					i51AdeOsLog ( i51_LOG_SC , "KEL : Alread Loaded Sys-Kit" ) ;
					{
						
						iFILE ServerFile = 0 ;
						iU32 Version = 0 ;
						
						length = i51AdeStdWStrlen (i51KernelGetSystemPath ()) ;
						length += i51AdeStdWStrlen ( kitpath ) + 2 ;	
						iASSERT ( length < sizeof(path) ) ;

						ppath = i51AdeStdWStrcpy ( path , i51KernelGetSystemPath  () ) ;	
						ppath = i51AdeStdWStrcat ( ppath , kitpath ) ;		

						
						i51FILEOPEN(ServerFile,path,i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN) ;
						i51SlLoaderGetAttr ( ServerFile , 0 , &Version , 0 , 0 , 0 ) ;
						i51AdeOsFileClose ( ServerFile ) ;

						//	如果本地版本小于等于已加载版本
						if ( Version <= i51KitServerVersion ) {
							i51AdeOsLog ( i51_LOG_SC , "KEL : Sys-Kit Matched , H 0x%x" , i51Kernel.SapKitList[looper].i51KitPort () ) ;
							return i51Kernel.SapKitList[looper].i51KitPort () ;
						}

						i51KernelUnloadSysKitEx () ;
						i51AdeOsLog ( i51_LOG_SC , "KEL : V (%x>%x) Unmatched  , Reload Sys-Kit" , Version , i51KitServerVersion ) ;
						reload = iTRUE ;
					}
					//	return i51Kernel.SapKitList[looper].i51KitPort () ;
					//	Jelo Edit Finished
					
					
				}				
			}
			
		}	
		
	}
 
	length = i51AdeStdWStrlen (i51KernelGetSystemPath ()) ;
	length += i51AdeStdWStrlen ( kitpath ) + 2 ;	
	iASSERT ( length < sizeof(path) ) ;

	ppath = i51AdeStdWStrcpy ( path , i51KernelGetSystemPath  () ) ;	
	ppath = i51AdeStdWStrcat ( ppath , kitpath ) ;

	//	防崩溃机制
	if ( iTRUE == i51KernelKitCheckAntiHang ( name , iTRUE ) ) {
		i51KernelKitAntiHang ( name , iFALSE , iTRUE ) ;
		i51AdeOsFileAModeEnable () ;
		i51AdeOsFileDelete ( ppath ) ;
		i51AdeOsFileAModeDisable () ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : We Gotta Anti-Hang Here , SYS-KIT is Demaged" ) ;  
		return 0 ;
	}
	
	//	模拟器
	#ifdef i51SIMULATOR
	{
		
		iU16 pathnew [ 128 ] = { 0 } ;
		iU16* path = 0 ;

		if ( i51_ADE_CNF_VER_MTK == i51AdeCnfGetType () ) {

			path = (iU16* )i51MALLOC ( 
				(strlen("..\\..\\WIN32FS\\DRIVE_E\\i51\\i51SYS\\")+\
				(i51AdeStdWStrlen(kitpath) ) + 1) << 1 ) ;
				
			i51AdeStdWStrcpy ( path , L"..\\..\\WIN32FS\\DRIVE_E\\i51\\i51SYS\\" ) ;

			i51KernelSwap2DllName ( kitpath , pathnew ) ;			
			i51AdeStdWStrcat ( path , pathnew ) ;
			
			dll = i51SlLoaderLoad ( name , path , (void* )i51KernelStaticMemory , i51_KERNEL_STATIC_MEMORY_SIZE ) ;					

		} else if ( i51_ADE_CNF_VER_MSTAR == i51AdeCnfGetType () ) {

			iCHAR path8bit [ 128 ] = { 0 } ;
			iCHAR path16bit [ 128 ] = { 0 } ;

			sprintf ( path8bit , ".\\SimFileSys%si51\\i51SYS\\" , i51AdeOsGetPathSymbolEx () ) ;
			i51AdeStdAscii2Unicode ( path8bit , path16bit ) ;
				
			path = (iU16* )i51MALLOC ( 
				(strlen(path8bit)+\
				(i51AdeStdWStrlen(kitpath) ) + 1) << 1 ) ;
				
			i51AdeStdWStrcpy ( path , path16bit ) ;
			i51KernelSwap2DllName ( kitpath , pathnew ) ;			
			i51AdeStdWStrcat ( path , pathnew ) ;
				
			dll = i51SlLoaderLoad ( name , path , (void* )i51KernelStaticMemory , i51_KERNEL_STATIC_MEMORY_SIZE ) ;		
			
		} else if ( i51_ADE_CNF_VER_SPRD == i51AdeCnfGetType () ) {

			path = (iU16* )i51MALLOC ( 
				(strlen(".\\i51\\i51SYS\\")+\
				(i51AdeStdWStrlen(kitpath) ) + 1) << 1 ) ;
				
			i51AdeStdWStrcpy ( path , L".\\i51\\i51SYS\\" ) ;

			i51KernelSwap2DllName ( kitpath , pathnew ) ;			
			i51AdeStdWStrcat ( path , pathnew ) ;
			
			dll = i51SlLoaderLoad ( name , path , (void* )i51KernelStaticMemory , i51_KERNEL_STATIC_MEMORY_SIZE ) ;					

		}else  if ( i51_ADE_CNF_VER_CS == i51AdeCnfGetType () ) {

			path = (iU16* )i51MALLOC ( 
				(strlen("..\\Bin\\WIN32FS\\DRIVE_E\\i51\\i51SYS\\")+\
				(i51AdeStdWStrlen(kitpath) ) + 1) << 1 ) ;
				
			i51AdeStdWStrcpy ( path , L"..\\Bin\\WIN32FS\\DRIVE_E\\i51\\i51SYS\\" ) ;

			i51KernelSwap2DllName ( kitpath , pathnew ) ;			
			i51AdeStdWStrcat ( path , pathnew ) ;
			
			dll = i51SlLoaderLoad ( name , path , (void* )i51KernelStaticMemory , i51_KERNEL_STATIC_MEMORY_SIZE ) ;					

		}

		i51FREE ( path ) ;			
						  
	}
	#else
	{
	//	真机
		dll = i51SlLoaderLoad ( name , ppath , (void* )i51KernelStaticMemory , i51_KERNEL_STATIC_MEMORY_SIZE ) ;					
	//	dll = i51SlLoaderLoad ( name , ppath , 0 , 0 ) ; 
	}
	#endif  

	if ( 0 == dll ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Kit Exist F" ) ;
		return 0 ;
	}

	i51KitMain = (i51KITMAIN ) i51SlLoaderGetSymbol ( dll , "i51KitMain" ) ;	
	i51KitPort = (i51KITPORT ) i51SlLoaderGetSymbol ( dll , "i51KitPort" ) ;
	  
	if ( 0 == i51KitMain ) {
		return 0 ;
	} 
	
	if ( 0 == i51KitPort ) {
		return 0 ;
	}

	//	对于SYS-KIT来说，加载后立即释放其占用的SlLOADER 内存
	i51SlLoaderUnload ( (iS32) dll ) ;		
				
	i51Kit = (i51KERNELKIT* ) i51KernelKitListCreateKit ( name , i51KitMain , i51KitPort , (iHANDLE)dll ,  
			i51_APP_STATUS_SLEEP , iTRUE , i51_KERNEL_KIT_SCOPE_STATIC , 0 ) ;

	if ( 0 == i51Kit ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : FE , CreateKit F" ) ;
		return 0 ;
	}

	i51KernelKitAntiHang ( name , iTRUE , iTRUE ) ;
	
	ret = i51Kit->i51KitMain ( i51Kit->id , i51_MSG_INIT , (void*)i51KIT_STATIC_KIT_GET_MAPING(Adapter) ) ;

	if ( iFALSE == ret ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Kit i51KitMain F" ) ; 
		//	发送暂停消息
		i51KernelSendKitMsg ( i51Kit->id , i51_MSG_PAUSE , 0 ) ; 	
		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;		
		return iFALSE ;
	}

	i51Kit->i51KitMain ( i51Kit->id , i51_MSG_RUN , 0 ) ;

	i51KernelKitAntiHang ( name , iFALSE , iTRUE ) ;

	//	Jelo Add Since 2012.4.16
	{
		iFILE ServerFile = 0 ;
		i51FILEOPEN(ServerFile,path,i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN) ;
		i51SlLoaderGetAttr ( ServerFile , 0 , &i51KitServerVersion , 0 , 0 , 0 ) ;
		i51AdeOsFileClose ( ServerFile ) ;
	}

	if ( iTRUE == reload ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : Tell Apps Sys-Kit Has Reloaded") ;
		//	通知上层应用系统服务模块需要重新加载。
		i51KernelSendSapKitMsg ( i51_MSG_RELOADSERVER , 0 ) ;
		i51KernelSendDapKitMsgEx ( i51_MSG_RELOADSERVER , 0 ) ;
	}
	//	Jelo Add Finished

	i51AdeOsLog ( i51_LOG_SC , "KEL : Load Sys-Kit Finished , ID 0x%x , V 0x%x , H 0x%x" , i51Kit->id , i51KitServerVersion , i51Kit->i51KitPort () ) ;
	
	return i51Kit->i51KitPort () ; 	

}

iS32 i51KernelLoadApp ( iU16* floder , void* parameter , iBOOL sober ) {

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	//	参数如: i51KernelRunApplication ( L"Boxer" ) 
	
	iS32 ret = 0 ;
	iS32 dll = 0 ;
	iFILE file = 0 ; 
	iS32 length = 0 ;
	iU16 path [ 64 ] = { 0 } ;
	iU16* ppath = 0 ;
	iU32 status = i51_APP_STATUS_SLEEP ;
	
	i51KERNELKIT* i51Kit = 0 ;
	i51KITMAIN kitmain = 0 ;

	{
		i51AdeStdUnicode2Ascii ( floder , (iCHAR* ) path ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Load i51-App '%s , 0x%x , 0x%x'" , path ) ;
	}  

	if ( 0 != i51SlLoaderGetSymbolEx ( floder , "i51KitMain" ) ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : i51-App '%s' Alread Loaded" , path ) ;
		return i51_KERNEL_LOAD_ERR_NO ;		
	}

	ppath = i51KernelMakei51Path ( path , sizeof(path) , floder ) ;

	if ( iFALSE == i51KernelCheckAppCompleten ( floder ) ) {  
		i51AdeOsLog ( i51_LOG_SC , "KEL : Ref E" ) ;
		return i51_KERNEL_LOAD_ERR_REF ;
	}

	if ( iTRUE == sober ) {
		status = i51_APP_STATUS_SOBER ;
	}
	
//	真机
#ifndef i51SIMULATOR

	dll = i51SlLoaderLoad ( floder , ppath , 0 , 0 ) ;
 	
	if ( 0 == dll ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 2" ) ;
		return i51_KERNEL_LOAD_ERR_NE ;
	}

	kitmain = (i51KITMAIN ) i51SlLoaderGetSymbol ( dll , "i51KitMain" ) ;
	
	if ( 0 == kitmain ) {
		return i51_KERNEL_LOAD_ERR_NE ;
	}

	i51Kit = (i51KERNELKIT* ) i51KernelKitListCreateKit ( floder , kitmain , 
		(i51KITPORT ) i51SlLoaderGetSymbol ( dll , "i51KitPort" ) , (iHANDLE)dll , 
			status , iFALSE , i51_KERNEL_KIT_SCOPE_DYNAMIC , 22 ) ;

	if ( 0 == i51Kit ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 3" ) ;	
		i51SlLoaderUnload ( (iS32) dll ) ;
		return i51_KERNEL_LOAD_ERR_MEM ;
	} 

	//	Jelo Edit Start 2011.11.26
	//	create front end when timer started	
	//	if ( iFALSE == i51KernelCreateFrontEnd () ) {			
	//		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 4" ) ;	 
	//		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;
	//		return i51_KERNEL_LOAD_ERR_FE ;
	//	}
	//	Jelo Edit Finish

	//	Jelo Add Start 2011.12.26
	if ( iTRUE == sober ) {
		//	为解决应用在i51_MSG_INIT 调用MMI接口时所出现的问题。
		//	创建前台环境要在i51_MSG_INIT
		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;
	}
	//	Jelo Add Finish


	//	William Add Start 2012.7.10
	#ifdef __SPRD_CLEAR_CACHE__
	i51KernelDump ( L"Kernel.dump" , i51Kit->id , 4 ) ;
	#endif
	//	William Add Finish

	
	if ( iFALSE == i51Kit->i51KitMain ( i51Kit->id , i51_MSG_INIT , (void*)i51KIT_STATIC_KIT_GET_MAPING(Adapter) ) ) {
		
		i51AdeOsLog ( i51_LOG_SC , "KEL : i51KitMain F , S 5" ) ;	
		//	发送暂停消息
		i51KernelSendKitMsg ( i51Kit->id , i51_MSG_PAUSE , 0 ) ; 	
		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;

		//	Jelo Add Start 2011.12.26
		//	释放前台环境 
		if ( 0 == i51Kernel.TotallSober ) {
			i51KernelReleaseFrontEnd () ;					
		}
		//	Jelo Edit Finish
		
		return i51_KERNEL_LOAD_ERR_MEM ;
		
	}

	//	Jelo Edit Start 2011.12.26
	//	if ( iTRUE == sober ) {
		
		//	需要在RUN 消息之前创建前台环境
		//	否则某些平台会出现未定义错误。
	//		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;

	//	}
	//	Jelo Edit Finish


	//	William Add Start 2012.7.10
      #ifdef __SPRD_CLEAR_CACHE__
	i51KernelDump ( L"Kernel.dump" , i51Kit->id , 4 ) ;
	#endif
	//	William Add Finishi
	
	i51Kit->i51KitMain ( i51Kit->id , i51_MSG_RUN , parameter ) ;

	//	William Add Start 2012.7.10
	#ifdef __SPRD_CLEAR_CACHE__
	i51KernelDump ( L"Kernel.dump" , i51Kit->id , 4 ) ;
	#endif
	//	William Add Finish
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Load i51-App Finished , Id 0x%x" , i51Kit->id ) ;
	
	return i51_KERNEL_LOAD_ERR_NO ;
	
#elif defined(i51SIMULATOR)
{

	if ( i51_ADE_CNF_VER_MTK == i51AdeCnfGetType () ) {
		
		iU16 dllpath [ 256 ] = { L"..\\..\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , floder ) ;  
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;

		dll = i51SlLoaderLoad ( floder , dllpath , 0 , 0 ) ;
		
		//dll = LoadLibraryW ( dllpath ) ;

	} else if ( i51_ADE_CNF_VER_MSTAR == i51AdeCnfGetType () ) {
	
		iU16 dllpath [ 256 ] = { L".\\SimFileSys\\CARD\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;

		dll = i51SlLoaderLoad ( floder , dllpath , 0 , 0 ) ;
		
		//dll = LoadLibraryW ( dllpath ) ;
		
	} else if ( i51_ADE_CNF_VER_SPRD == i51AdeCnfGetType () ) {

		iU16 dllpath [ 256 ] = { L".\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;

		dll = i51SlLoaderLoad ( floder , dllpath , 0 , 0 ) ;
		
	}else if ( i51_ADE_CNF_VER_CS == i51AdeCnfGetType () ) {
		
		iU16 dllpath [ 256 ] = { L"..\\Bin\\WIN32FS\\DRIVE_E\\i51\\i51KIT\\" } ;
		i51AdeStdWStrcat ( dllpath , floder ) ;
		i51AdeStdWStrcat ( dllpath , L"\\" ) ;
		i51AdeStdWStrcat ( dllpath , floder ) ;  
		i51AdeStdWStrcat ( dllpath , L".dll" ) ;

		dll = i51SlLoaderLoad ( floder , dllpath , 0 , 0 ) ;
	}

	if ( 0 == dll ) {

		iS32 error = i51SlLoaderGetLastError () ;

		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 2" ) ; 

		if ( i51_LD_ERROR_CRC == error ) {
			return i51_KERNEL_LOAD_ERR_CRC ;
		} else {
			return i51_KERNEL_LOAD_ERR_NE ;
		}
		
	}

	kitmain = (i51KITMAIN ) i51SlLoaderGetSymbol ( dll , "i51KitMain" ) ;

	if ( 0 == kitmain ) {
		return i51_KERNEL_LOAD_ERR_NE ;
	}
	
	i51Kit = (i51KERNELKIT* ) i51KernelKitListCreateKit ( floder , kitmain , 
		(i51KITPORT ) i51SlLoaderGetSymbol ( dll , "i51KitPort" ) , (iHANDLE)dll , 
			status , iFALSE , i51_KERNEL_KIT_SCOPE_DYNAMIC , 22 ) ;
	
	if ( 0 == i51Kit ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 3" ) ;	
		FreeLibrary ( dll ) ;
		return i51_KERNEL_LOAD_ERR_MEM ;
	}

	//	Jelo Edit Start 2011.11.26
	//	create front end when timer started		
	//	if ( iFALSE == i51KernelCreateFrontEnd () ) {			
	//		i51AdeOsLog ( i51_LOG_SC , "KEL : R APP F , S 4" ) ;	 
	//		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;
	//		return i51_KERNEL_LOAD_ERR_FE ;
	//	} 
	//	Jelo Edit Finish


	//	Jelo Add Start 2011.12.26
	if ( iTRUE == sober ) {
		//	为解决应用在i51_MSG_INIT 调用MMI接口时所出现的问题。
		//	创建前台环境要在i51_MSG_INIT
		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;
	}
	//	Jelo Add Finish
	
	if ( iFALSE == i51Kit->i51KitMain ( i51Kit->id , i51_MSG_INIT , (void*)i51KIT_STATIC_KIT_GET_MAPING(Adapter) ) ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : i51KitMain F , S 5" ) ;	
		//	发送暂停消息
		i51KernelSendKitMsg ( i51Kit->id , i51_MSG_PAUSE , 0 ) ; 	
		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;

		//	Jelo Add Start 2011.12.26
		//	释放前台环境 
		if ( 0 == i51Kernel.TotallSober ) {
			i51KernelReleaseFrontEnd () ;					
		}
		//	Jelo Edit Finish
			
		return i51_KERNEL_LOAD_ERR_MEM ;
	}

	//	Jelo Edit Start 2011.12.26
	//	if ( iTRUE == sober ) {
		 
		//	需要在RUN 消息之前创建前台环境
		//	否则某些平台会出现未定义错误。
	//		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;

	//	}
	//	Jelo Edit Finish
	
	i51Kit->i51KitMain ( i51Kit->id , i51_MSG_RUN , parameter ) ;
	 
	i51AdeOsLog ( i51_LOG_SC , "KEL : Load i51-App Finished , Id 0x%x" , i51Kit->id ) ;
	
	return i51_KERNEL_LOAD_ERR_NO ;
	
}
#endif

	  
}

iU32* i51KernelGetApplicationInterface ( iU32 appid ) {

	//	author : Jelo 
	//	since : 2011.11.7
	// 	(C) PKIG Tech. Co., Ltd.		

	//	获取*.i51内部的接口

	iU32 looper = 0 ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Get i51-Api , id = 0x%x" , appid ) ;

	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {
		
		//	*.i51 可以没有 i51KitPort
		if ( 0 != i51Kernel.DapKitList[looper].i51KitPort 
			&& appid == (iU32 ) i51Kernel.DapKitList[looper].id ) {

			#ifdef i51KERNEL_DEBUG	
			{
				iCHAR buffer [ 64 ] = { 0 } ;
				i51AdeStdUnicode2Ascii ( i51Kernel.DapKitList[looper].name , buffer ) ;
				i51AdeOsLog ( 0 , "App Name %s , Id %d , Looper %d , i51Port 0x%x" ,
					buffer , i51Kernel.DapKitList[looper].id,looper,i51Kernel.DapKitList[looper].i51KitPort() ) ;
			}
			#endif

			return i51Kernel.DapKitList[looper].i51KitPort () ;

		}
		 
	}	

	i51AdeOsLog ( i51_LOG_SC , "KEL : Get i51-Api Failed, Not Found id" ) ;

	return 0 ;

}
  
iU32 i51KernelGetApplicationId ( iU16* floder )
{


	//	author : Jelo 
	//	since : 2011.8.24
	// 	(C) PKIG Tech. Co., Ltd.		

	//	成功大于0，失败等于0

	iU32 looper = 0 ;
	
	#ifdef i51KERNEL_DEBUG	
		iCHAR buffer [ 64 ] = { 0 } ;
		i51AdeStdUnicode2Ascii ( floder , (iCHAR* ) buffer ) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Get i51-App Id , Name '%s'" , buffer ) ;
	#else
		i51AdeOsLog ( i51_LOG_SC , "KEL : Get i51-App Id , Name = 0x%x" , floder ) ;
	#endif
	
	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {
		
		//	*.i51 可以没有 i51KitPort
		if ( 0 == i51AdeStdWStrcmp ( i51Kernel.DapKitList[looper].name , floder ) ) {

			#ifdef i51KERNEL_DEBUG	
			{  
				iCHAR buffer [ 64 ] = { 0 } ; 
				i51AdeStdUnicode2Ascii ( i51Kernel.DapKitList[looper].name , buffer ) ;
				i51AdeOsLog ( 0 , "App Name %s , Id %d , Looper %d , i51Port 0x%x" ,
					buffer ,i51Kernel.DapKitList[looper].id,looper,i51Kernel.DapKitList[looper].i51KitPort ) ;
			}
			#endif     
			      
			return i51Kernel.DapKitList[looper].id ;
			
		}
		
	}	 
  
	i51AdeOsLog ( i51_LOG_SC , "KEL : Get i51-App Id Failed, Not Found id" ) ;
	
	return 0 ;	


}

iBOOL i51KernelChangeStatus ( iU32 aid , iU32 status ) {

	//	author : Jelo 
	//	since : 2011.8.24  
	// 	(C) PKIG Tech. Co., Ltd.		

	//	设置运行模式
	//	只能对*.i51进行设置，*.kit默认方式为睡眠状态，且无法改变。

	iU32 looper = 0 ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : CS 0x%x , 0x%x , Totall Sober = %d" , aid , status , i51Kernel.TotallSober ) ;
	
	for ( looper = 0 ; looper < i51Kernel.DapKitListTotall ; looper ++ ) {
		
		if ( aid == i51Kernel.DapKitList[looper].id ) {

			//	如果是Sober ，则更新Sober 统计信息
			if ( i51_APP_STATUS_SOBER == status ) {

				if ( (iU32 )i51Kernel.CurrentSoberKit.id != (iU32 )&i51Kernel.DapKitList[looper].id ) {
					i51Kernel.DapKitList[looper].status = status ;
					i51Kernel.TotallSober ++ ;
					i51KernelSetCurrentSorber () ;
				}				

				if ( iFALSE == i51Kernel.TimerRuning ) {
					i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;
				}
  
			} else {

				if ( i51_APP_STATUS_SOBER == i51Kernel.DapKitList[looper].status ) {
					i51Kernel.DapKitList[looper].status = status ;
					i51Kernel.TotallSober -- ;
					i51KernelSetCurrentSorber () ;
				} else {
					i51Kernel.DapKitList[looper].status = status ;
				}

			}

			if ( 0 == i51Kernel.TotallSober ) { 
				i51AdeOsLog ( i51_LOG_SC , "KEL : NO Sober Exist , Stop Kernel Timer" ) ;	
				i51KernelStopTimer ( iTRUE ) ;
			}

			i51AdeOsLog ( i51_LOG_SC , "KEL : CS Finished , Totall Sober = %d" , i51Kernel.TotallSober ) ;
			
			return iTRUE ;

		}
		
	}	

	i51AdeOsLog ( i51_LOG_SC , "KEL : CS BE , Totall Sober = %d" , i51Kernel.TotallSober ) ;
	
	return iFALSE ;	

	
}

#define MAX_LOADKITCORE_COUNT 10
static iU32 LoadIdTable[MAX_LOADKITCORE_COUNT] = {0} ;
static iBOOL i51KernelLoadKitCoreExist ( iU32 loadid )
{
	//	判断加载的地方是否已经加载过
	iU16 i = 0 ;
	iBOOL ret = iFALSE ;
	for ( ; i < MAX_LOADKITCORE_COUNT ; i++ ) {

		if ( LoadIdTable[i] == loadid ) {
			
			return iTRUE ;
		}
	}
	return ret ;
}

static iBOOL i51KernelLoadKitCoreAddByFontEnd( )
{
	//	如果创建前台，添加由前台环境load kitcore
	//	避免在创建的前台环境的时候，T卡里面没有kitCore

	iU16 i = 0 ;
	iBOOL ret = iFALSE ;

	if ( !i51Kernel.FrontEndEnable ) {

		return iFALSE ;
	}

	if ( i51KernelLoadKitCoreExist((iU32)i51KernelCreateFrontEnd) ) {

		return iTRUE ;
	} 

	for ( ; i < MAX_LOADKITCORE_COUNT ; i++ ) {

		if ( LoadIdTable[i] == 0 ) {

			LoadIdTable[i] = (iU32)i51KernelCreateFrontEnd ;
			return iTRUE ;
		}
	}
	return ret ;
	
	
}

static iBOOL i51KernelLoadKitCoreAdd ( iU32 loadid )
{
	
	iU16 i = 0 ;
	iBOOL ret = iFALSE ;

	i51KernelLoadKitCoreAddByFontEnd () ;
	
	if ( i51KernelLoadKitCoreExist(loadid) ) {

		return iTRUE ;
	} 
	
	for ( ; i < MAX_LOADKITCORE_COUNT ; i++ ) {

		if ( LoadIdTable[i] == 0 ) {

			LoadIdTable[i] = loadid ;
			return iTRUE ;
		}
	}
	return ret ;
}


static iBOOL i51KernelLoadKitCoreDelete ( iU32 loadid )
{
	iU16 i = 0 ;
	iBOOL ret = iFALSE ;
	for ( ; i < MAX_LOADKITCORE_COUNT ; i++ ) {

		if ( LoadIdTable[i] == loadid ) {
			
			LoadIdTable[i] = 0 ;
			return iTRUE ;
		}
	}
	return ret ;	
}

static iBOOL i51KernelLoadKitCoreTableEmpty ()
{
	iU16 i = 0 ;
	iBOOL ret = iTRUE ;
	for ( ; i < MAX_LOADKITCORE_COUNT ; i++ ) {

		if ( LoadIdTable[i] != 0 ) {
			
			return iFALSE ;
		}
	}
	return ret ;	
}

iBOOL i51KernelLoadKitCore ( iU32 kitcoreid )
{
	i51KITMAIN i51KitMain = NULL;
	i51AdeOsLog(0, "KEL : i51KernelLoadKitCore");

	if ( !i51KernelLoadKitCoreTableEmpty() ) {

		i51KernelLoadKitCoreAdd (kitcoreid) ;
		i51AdeOsLog(0, "KEL: Load i51KitCore Aready");
		return iTRUE ;
	}
	
	i51KIT_DYNAMIC_KIT_LOAD(i51KitCore, L"PKiGAPI\\i51KitCore.kit");
	if(0 ==  i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitCore)){
		i51AdeOsLog(0, "KEL: Load i51KitCore F");
		return iFALSE;
	}
	
	i51KitMain = (i51KITMAIN ) i51SlLoaderGetSymbolEx ( L"i51KitCore" , "i51KitMain" ) ;
	if(NULL == i51KitMain){
		i51AdeOsLog(0, "KEL : Get i51KitMain F");
		return iFALSE;
	}
	i51KitMain( 0, 0x00FFFFFF , (void*)i51KitStaticCoreAdapter );
	i51KernelLoadKitCoreAdd (kitcoreid) ;
	return iTRUE;
}

void i51KernelUnloadKitCore (  iU32 kitcoreid )
{
	i51AdeOsLog(0, "KEL : i51KernelUnloadKitCore");
	i51KernelLoadKitCoreDelete (kitcoreid) ;
	
	if (i51KernelLoadKitCoreTableEmpty()) {
		
		i51KIT_DYNAMIC_KIT_UNLOAD(i51KitCore);
		i51AdeOsLog(0, "KEL : i51KernelUnloadKitCore ready unloead");
	}
}


iBOOL i51KernelCheckKitCoreExist(void){
	//	author : akilae 
	//	since : 2012.7.16
	// 	(C) PKIG Tech. Co., Ltd.	

	//	检查i51KitCore.kit是否存在
	
	//akilae add since 2012.7.11 
	iBOOL bNeedDl = iFALSE;
	iBOOL ret = iFALSE;
	iFILE	 file;
	i51KITMAIN i51KitMain = NULL;
	iU16 absPath[128] = {0};
	
	i51AdeStdWStrcpy(absPath, i51KernelGetApplicationPath());
	i51AdeStdWStrcat(absPath, L"PKiGAPI\\i51KitCore.kit");
	i51FILEOPEN(file, absPath, i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ);
	if(!file)	
		bNeedDl = iTRUE;
	else{
		ret = i51KernelCheckKitCrc(file);
		i51AdeOsFileClose(file);
		if(!ret)	bNeedDl = iTRUE;
	}
	
	if(bNeedDl){	
		return iFALSE;
	}else{		

	/*
		if(i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitCore))	//akilae++ 20120717
		{
			//#ifdef i51KERNEL_DEBUG
			i51AdeOsLog(0, "KEL : Skip load i51KitCore");
			//#endif
			return iTRUE;
		}
		
		i51KIT_DYNAMIC_KIT_LOAD(i51KitCore, L"PKiGAPI\\i51KitCore.kit");
		if(0 ==  i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitCore)){
			i51AdeOsLog(0, "KEL: Load i51KitCore F");
			return iFALSE;
		}
		
		i51KitMain = (i51KITMAIN ) i51SlLoaderGetSymbolEx ( L"i51KitCore" , "i51KitMain" ) ;
		if(NULL == i51KitMain){
			i51AdeOsLog(0, "KEL : Get i51KitMain F");
			return iFALSE;
		}
		i51KitMain( 0, 0x00FFFFFF , (void*)i51KitStaticCoreAdapter );
*/
		return iTRUE;
	}
	//akilae add finished
}

iBOOL i51KernelExtractPackage ( iFILE handle , iU16 *AppName ) 
{
	iBOOL ret = iFALSE ;
//	akilae add since 2012.7.11
	if(i51KernelLoadKitCore((iU32)i51KernelExtractPackage))
		ret = i51KitCoreExtractPackage(handle, AppName);		
	else{
		i51AdeOsLog(0, "KEL: Extract Package F");
		return iFALSE;
	}
	i51KernelUnloadKitCore ((iU32)i51KernelExtractPackage) ;
	return ret ;
//	akilae add finished
}

iBOOL i51KernelSerialNumberValid () {
	
	//	author : william
	//	since : 2011.11.4
	// 	(C) PKIG Tech. Co., Ltd.	
	//	判断sn是否有效
	iU8 sn[12+1] = {0} ;
	iU32 SrcCcr = 0 ;
	iU32 DestCrc = 0 ;
	iBOOL ret = iFALSE ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelSerialNumberValid S" ) ;
	#endif
	
	if ( i51AdeOsPermanitBufferRead(sn , 12) ) {

		//	进行src校验
		SrcCcr = i51AdeStdCrc32 ( sn + 4 , 8 , iNULL ) ;
		memcpy ( &DestCrc , sn , 4 ) ;
		if ( DestCrc == SrcCcr )
			ret = iTRUE ;
	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : SN Valid , ret = 0x%x" , ret ) ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelSerialNumberValid E" ) ;
	#endif
	
	return ret ;
	
}


void  i51KernelGenSnTimerCB(iU32 id)
{
    void *sn = iNULL ;
	iU8 snLen = 0 ;

    i51AdeOsTimerStop(id);
	i51KernelGetSerialNumber ( &sn , &snLen ) ;
    if(i51DownloaderRunning.GetSncallback)
	    i51DownloaderRunning.GetSncallback ( iTRUE , sn , snLen ) ;
}
	
iBOOL i51KernelGenerateSerialNumber ( void (*callback)( iBOOL results , void* buffer , iU32 buflen ) ) {

	//	author : william
	//	since : 2011.11.4
	// 	(C) PKIG Tech. Co., Ltd.	
	//	获取下载sn

	//	Jelo Edited 2012-4-17	

	char* Pattern = "PLATFORM%s_IMSI%s_IMEI%s_PRO%d_T(%d%d%d%d%d%d%d)_DISK%d_MEM%d_%d_%d" ;
	iCHAR Simple[256] = { 0 } ;

	iU8 snbuffer[8] = {0} ;
	iU8 PermanitBuffer[12] = {0} ;	
	
	iS32* sims = 0 ;
	iU16 Year = 0 ;
	iU16 Month = 0 ;
	iU16 Day = 0 ;
	iU16 Hour = 0 ;
	iU16 Minute = 0 ;
	iU16 Second = 0 ;

	iU32 crc = 0 ;
	iU32 tick = 0 ;
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGenerateSerialNumber S" ) ;
	#endif

	i51AdeOsLog ( i51_LOG_SC , "KEL : SN GEN S" ) ;
	
	i51AdeOsGetTime ( &Year , &Month , &Day , &Hour , &Minute , &Second ) ;
	sims = i51AdeSmsGetSims () ;

	//	有SIM 卡
	if ( 0 < sims[0] ) {
		sprintf ( Simple , Pattern , 
			i51AdeCnfGetPlatformId () , 
			i51AdeSmsGetImsi ( sims[1] ) ,
			i51AdeSmsGetImei () ,
			i51AdeCnfGetProjectId () ,
			Year , Month , Day , Hour , Minute , Second ,
			i51KernelGetDiskFreeSpace () ,
			i51AdeOsGetTotallMemoryLeft () ,
			rand () ,
			i51AdeOsGetTick () 
		) ;
	} else {
		sprintf ( Simple , Pattern , 
			i51AdeCnfGetPlatformId () , 
			i51AdeOsGetTick () ,
			i51AdeSmsGetImei () ,
			i51AdeCnfGetProjectId () ,
			Year , Month , Day , Hour , Minute , Second ,
			i51KernelGetDiskFreeSpace () ,
			i51AdeOsGetTotallMemoryLeft () ,
			rand () ,
			i51AdeOsGetTick () 
		) ;
	}

	crc = i51AdeStdCrc32 ( (iU8* )Simple , strlen(Simple) , 0 ) ;

	i51DownloaderRunning.GetSncallback= callback ;
	i51DownloaderRunning.GetSntimerid = i51AdeOsTimerCreate ( 0 , 1 ) ; 

	if ( i51DownloaderRunning.GetSntimerid == 0 ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : C T F" ) ;	
		return iFALSE ;
	}

	{

		iU32 snCrc = 0 ;		

		tick = i51AdeOsGetTick () ;
		
		memcpy ( snbuffer , &crc , sizeof(iU32) ) ;
		memcpy ( snbuffer + sizeof(iU32) , &tick , sizeof(iU32) ) ;
		
		//	Jelo Add 20120606
		//	Little-Endian Transform
		if ( i51_ADE_CNF_BIG_ENDIAN == i51AdeCnfGetEndian () ) {	
			
			iU32 value = 0 ;
			
			value = snbuffer[3] ;
			snbuffer[3] = snbuffer [0] ;
			snbuffer[0] = value ;
			value = snbuffer[1] ;
			snbuffer[1] = snbuffer[2] ;
			snbuffer[2] = value ;


			value = snbuffer[7] ;
			snbuffer[7] = snbuffer [4] ;
			snbuffer[4] = value ;
			value = snbuffer[5] ;
			snbuffer[5] = snbuffer[6] ;
			snbuffer[6] = value ;			
			
		}
		//	Jelo Add Finished

		snCrc = i51AdeStdCrc32 ( snbuffer , sizeof(iU32)+sizeof(iU32) , 0 ) ;
		
		memcpy ( PermanitBuffer , &snCrc , sizeof(iU32) ) ;
		memcpy ( PermanitBuffer + sizeof(iU32) , snbuffer , sizeof(iU32)+sizeof(iU32) ) ;
		
		if ( !i51AdeOsPermanitBufferWrite ( PermanitBuffer , 12 )) {
			
			i51AdeOsLog ( 0 , "KEL : SN F" ) ;
			
			if ( 0 == i51AdeOsTimerStart (  i51DownloaderRunning.GetSntimerid  , i51KernelGetSnUseDlFailCB , 100 ) ) {
				i51AdeOsLog ( i51_LOG_SC , "KEL : S T F" ) ;	
			}	
			
			return iFALSE ;
			
		}
		
	}	
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : SN STR = %s" , Simple ) ;
	i51AdeOsLog ( i51_LOG_SC , "KEL : SN = 0x%x %x" , crc , tick ) ; 

	//callback ( iTRUE , snbuffer , sizeof(iU32)+sizeof(iU32) ) ;
    i51AdeOsTimerStart (  i51DownloaderRunning.GetSntimerid  , i51KernelGenSnTimerCB , 100 );

	i51AdeOsLog ( i51_LOG_SC , "KEL : SN GEN E" ) ;
	
	#ifdef i51KERNEL_DEBUG
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGenerateSerialNumber E" ) ;
	#endif

	return iTRUE ;
	
#if 0
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGenerateSerialNumber S" ) ;
	#endif

	i51AdeOsLog ( i51_LOG_SC , "KEL : Generate SN") ;
	
	iASSERT (callback) ;
	if (! i51SlDlAbleCreate () ) {

		i51AdeOsLog ( i51_LOG_SC , "KEL : Getsn dl aready create" ) ;
		i51DownloaderRunning.GetSncallback= callback ;
		i51DownloaderRunning.GetSntimerid = i51AdeOsTimerCreate ( 0 , 1 ) ; 
		if ( i51DownloaderRunning.GetSntimerid == 0 ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : Getsn create timer F" ) ;	
			return iFALSE ;
		}
		if ( 0 == i51AdeOsTimerStart (  i51DownloaderRunning.GetSntimerid  , i51KernelGetSnUseDlFailCB , 30*1000 ) )
		{
			i51AdeOsLog ( i51_LOG_SC , "KEL : Getsn  timer start F" ) ;	
			return iFALSE ;
		}		
		
		return iFALSE ;
	}


	i51KernelGetSnCallback = callback ;
      i51SlDlCreate (i51KernelGetSNCreateDlCallback) ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGenerateSerialNumber E" ) ;
	#endif
	
	return iTRUE ;
#endif	
	//	Jelo Edited Finished
	
	
}

iBOOL i51KernelGenerateSerialNumberStop () {

	//	author : william
	//	since : 2011.11.4
	// 	(C) PKIG Tech. Co., Ltd.	
	//	停止下载sn

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGenerateSerialNumberStop S" ) ;
	#endif

	i51AdeOsLog ( i51_LOG_SC , "KEL : Generate SN Stop") ;
	
	//	Jelo Edited 2012-4-28	
	//	i51SlDlStop(0) ;
	//	Jelo Finished

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGenerateSerialNumberStop E" ) ;
	#endif
	
	return iTRUE ;

}

iBOOL i51KernelGetSerialNumber ( void** buffer , iU8* contentlen ) {

	//	author : william
	//	since : 2011.11.4
	// 	(C) PKIG Tech. Co., Ltd.	
	//	获取sn 
	static iU8 sn[12+1] = {0} ;
	iBOOL ret = iFALSE ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGetSerialNumber S , 0x%x , 0x%x" , buffer , contentlen ) ;
	#endif
	
	iASSERT (buffer&&contentlen) ;
	if (i51KernelSerialNumberValid()) {
		//	获取之前先判断有效性
	 	ret  = i51AdeOsPermanitBufferRead ( sn , 12 ) ;
		
		if (ret) {
			
			*buffer = sn + 4 ;
			*contentlen = 8 ;

			i51AdeOsLog ( i51_LOG_SC , "KEL : Get SN Finished , length %d" , *contentlen ) ;
			
		}
		
	}

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGetSerialNumber E , 0x%x , 0x%x" , buffer , contentlen ) ;
	#endif
	
	return ret ;
	
}

iCHAR* i51KernelGeti51Version () {

	//	author : Jelo 
	//	since : 2011.10.30
	// 	(C) PKIG Tech. Co., Ltd.	

	//	获取i51中间件版本号。

	static iCHAR buffer [64] = { 0 } ;
	iS32 address = (iS32 ) buffer ;
	
//	sprintf ( buffer , "0.%d.%d.Build%d.%d-%s%s" , 
//		i51_KERNEL_DES_VERSION , 
//		i51_ADE_CNF_DES_VERSION , 
//		i51_KERNEL_IMP_VERSION , 
//		i51AdeCnfGetVersionEx () , 
//		__DATE__ , 
//		__TIME__ ) ;

	strcpy ( (iCHAR* )address , i51_KERNEL_VERSION ) ;
	buffer [ strlen(i51_KERNEL_VERSION) ] = 0x20 ;
	address = address + (strlen(i51_KERNEL_VERSION)+1) ;
	strcpy ( (iCHAR* )address , i51AdeCnfGetVersionEx () ) ;

	return buffer ;
	
}

iU32 i51KernelGetVersion () {

	//	author : Jelo 
	//	since : 2011.10.30
	// 	(C) PKIG Tech. Co., Ltd.	
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGetVersion S") ;
	#endif
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGetVersion E" ) ;
	#endif		

	return i51KernelFormatVersion ( i51_KERNEL_VERSION ) ;
	
}

iBOOL i51KernelGetBatchCode ( iU8* buffer , iU32 bufsize , iU32* length ) {

	//	author : Jelo 
	//	since : 2011.10.30
	// 	(C) PKIG Tech. Co., Ltd.	

	//	获取批号	

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGetBatchCode S , 0x%x , 0x%x , 0x%x" , buffer , bufsize , length ) ;
	#endif
	
	iASSERT( bufsize >= 16 ) ;

#define i51KernelSetU32Stream(p,n)\
	*(*p)++ = (unsigned char)((n>>24)&0xff);\
	*(*p)++ = (unsigned char)((n>>16)&0xff);\
	*(*p)++ = (unsigned char)((n>>8)&0xff);\
	*(*p)++ = (unsigned char)(n & 0xff);

	i51KernelSetU32Stream(&buffer,((iU32)i51AdeOsLog));
	i51KernelSetU32Stream(&buffer,((iU32)i51AdeOsFileOpen));
	i51KernelSetU32Stream(&buffer,((iU32)i51AdeSmsSend));
	i51KernelSetU32Stream(&buffer,((iU32)i51AdeStdWSprintf));

	*length = 16 ;

	i51AdeOsLog ( i51_LOG_SC , "Batch Code length = %d" , *length ) ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGetBatchCode E , length = %d" , *length ) ;
	#endif
	
}

iU32 i51KernelGetDiskFreeSpace () {

	//	author : Jelo 
	//	since : 2011.10.30
	// 	(C) PKIG Tech. Co., Ltd.	

	//	获取剩余空间，单位KB

	iU32 left = 0 ;
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGetDiskFreeSpace S" ) ;
	#endif
	
	if ( i51_KERNEL_DISK_MODE_SYSTEM == i51Kernel.DiskMode ) {

		left = i51AdeOsGetSysDiskSpaceLeft () ;
		
	} else {
	
		left = i51AdeOsGetDiskSpaceLeft () ;

	}

	i51AdeOsLog ( i51_LOG_SC , "KEL : Free Space = %d" , left ) ;
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelGetDiskFreeSpace E" ) ;
	#endif

	return left ;
	
}


struct {

	iCHAR currentstep[32] ;
	iU32 step ;
	iS32 state ;
	iU32 total ;
	iU32 curdl ;
	iU32 dlcount ;

	iU32 InstallerType ;
	iU32 InstallerTimerid ;
	void (*i51KernelInstallApplicationSaveCB)( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount  ) ;
	 
}ApplicationCBParamer = {0};

static void i51KernelInstallApplicationTimerCB ( iU32 timerid )
{
	i51AdeOsTimerStop (ApplicationCBParamer.InstallerTimerid) ;
	ApplicationCBParamer.InstallerTimerid  = 0 ;
	if ( ApplicationCBParamer.i51KernelInstallApplicationSaveCB )
		ApplicationCBParamer.i51KernelInstallApplicationSaveCB ( ApplicationCBParamer.currentstep ,
		ApplicationCBParamer.state ,
		ApplicationCBParamer.step ,
		ApplicationCBParamer.total ,
		ApplicationCBParamer.curdl ,
		ApplicationCBParamer.dlcount ) ;

	if (ApplicationCBParamer.InstallerType == 0 ) 
		i51KernelUnloadKitCore  ((iU32)i51KernelInstallApplication) ;
	else if ( ApplicationCBParamer.InstallerType == 1 )
		i51KernelUnloadKitCore  ((iU32)i51KernelReinstallApplication) ;
		
}
static void i51KernelInstallApplicationCB( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount  )
{
	
	if ( i51_SL_DL_RUNING != state ) {

		if ( currentstep != NULL )
		strcpy ( ApplicationCBParamer.currentstep , currentstep ) ;
		ApplicationCBParamer.state = state ;
		ApplicationCBParamer.step = step ;
		ApplicationCBParamer.total = total ;
		ApplicationCBParamer.curdl = curdl ;
		ApplicationCBParamer.dlcount = dlcount ;
		
		ApplicationCBParamer.InstallerTimerid = i51AdeOsTimerCreate ( 0 , 1 ) ;
		if ( ApplicationCBParamer.InstallerTimerid <= 0 ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : installer CB CT F ") ;
			if ( ApplicationCBParamer.i51KernelInstallApplicationSaveCB )
				ApplicationCBParamer.i51KernelInstallApplicationSaveCB ( ApplicationCBParamer.currentstep ,
				ApplicationCBParamer.state ,
				ApplicationCBParamer.step ,
				ApplicationCBParamer.total ,
				ApplicationCBParamer.curdl ,
				ApplicationCBParamer.dlcount ) ;			
		}
		i51AdeOsTimerStart (ApplicationCBParamer.InstallerTimerid ,i51KernelInstallApplicationTimerCB , 10 ) ;
		
	}else {
	
		if ( ApplicationCBParamer.i51KernelInstallApplicationSaveCB )
			ApplicationCBParamer.i51KernelInstallApplicationSaveCB (currentstep ,
			state ,
			step ,
			total ,
			curdl ,
			dlcount ) ;			
	}
}
 
iBOOL i51KernelInstallApplication ( iU16 *AppName , void (*callback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount ) ) 
{
	//	author : akilae
	//	since : 2012.7.13
	// 	(C) PKIG Tech. Co., Ltd.	
	memset ( &ApplicationCBParamer , 0 , sizeof(ApplicationCBParamer) ) ;
	ApplicationCBParamer.i51KernelInstallApplicationSaveCB = callback ;
	ApplicationCBParamer.InstallerType = 0 ;
	if(iTRUE == i51KernelCheckKitCoreExist() && iTRUE == i51KernelLoadKitCore ((iU32)i51KernelInstallApplication)){
		
		return i51KitCoreInstallApplication(AppName, i51KernelInstallApplicationCB);
	}else{
		i51MiniDlCoreKit(2, 0, AppName, NULL, callback);
		return iTRUE;
	}
}

iBOOL i51KernelInstallApplicationStop () {
	//	author : akilae
	//	since : 2012.7.13
	// 	(C) PKIG Tech. Co., Ltd.	
	iBOOL ret  = iTRUE ;
	if ( ApplicationCBParamer.InstallerTimerid ) 
		i51AdeOsTimerStop (ApplicationCBParamer.InstallerTimerid) ;
	ApplicationCBParamer.i51KernelInstallApplicationSaveCB = 0 ;

	
	if(iTRUE == i51KernelCheckKitCoreExist()) {
		
		ret = i51KitCoreInstallApplicationStop();
		i51KernelUnloadKitCore  ((iU32)i51KernelInstallApplication) ;
	}
	else
		i51MiniSlDlStop(0);
	
	return ret ;	
}

iBOOL i51KernelUninstallApplication ( iU16* appname ) { 
	//	author : akilae
	//	since : 2012.7.13
	// 	(C) PKIG Tech. Co., Ltd.	

	iU32 ret = iTRUE ;
	if(iTRUE == i51KernelCheckKitCoreExist() && iTRUE == i51KernelLoadKitCore ((iU32)i51KernelUninstallApplication)) {

		ret =  i51KitCoreUninstallApplication(appname);
	}
	i51KernelUnloadKitCore  ((iU32)i51KernelUninstallApplication) ;
	return ret ;	
}

iBOOL i51KernelReinstallApplication ( iU16* appname , iCHAR *uri , iU32 version , void (*callback)( iCHAR* currentstep , iS32 status , iU32 step , iU32 totall, iU32 curdl , iU32 dlcount ) ) { 
	//	author : akilae
	//	since : 2012.7.13
	// 	(C) PKIG Tech. Co., Ltd.	

	memset ( &ApplicationCBParamer , 0 , sizeof(ApplicationCBParamer) ) ;
	ApplicationCBParamer.i51KernelInstallApplicationSaveCB = callback ;
	ApplicationCBParamer.InstallerType = 1 ;	
	if(iTRUE == i51KernelCheckKitCoreExist() && iTRUE == i51KernelLoadKitCore ((iU32)i51KernelReinstallApplication)){

		return i51KitCoreReinstallApplication(appname, uri, version, i51KernelInstallApplicationCB);
	}else{
		i51MiniDlCoreKit(3, version, appname, uri, callback);
		return iTRUE;
	}
}

iBOOL i51KernelReinstallApplicationStop () {
	
	//	author : akilae
	//	since : 2012.7.13
	// 	(C) PKIG Tech. Co., Ltd.	
	iBOOL ret = iTRUE ;
	if(iTRUE == i51KernelCheckKitCoreExist()) {
		
		ret = i51KitCoreReinstallApplicationStop();
		i51KernelUnloadKitCore  ((iU32)i51KernelReinstallApplication) ;
	}
	else
		i51MiniSlDlStop(0);
	return ret ;	
}

#if 0
iBOOL i51KernelSetCache ( iU16* name ) {

	//	author : Jelo
	//	since : 2011.11.22
	// 	(C) PKIG Tech. Co., Ltd.	
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelSetCache S" ) ;
	#endif


	i51AdeOsLog ( i51_LOG_SC , "KEL : Set Cache 0x%x , CW %d" , name , i51Kernel.CacheWatcher ) ;

	//	Only set once
	if ( 0 < i51Kernel.CacheWatcher ) {
		return iTRUE ;
	}

	i51SlLoaderSetCache ( name ) ;

	i51Kernel.CacheWatcher ++ ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelSetCache E" ) ;
	#endif
	
	return iTRUE ;

}

iBOOL i51KernelUnsetCache ( iU16* name ) {

	//	author : Jelo
	//	since : 2011.11.22
	// 	(C) PKIG Tech. Co., Ltd.
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelUnsetCache S" ) ;
	#endif

	i51AdeOsLog ( i51_LOG_SC , "KEL : Unset Cache , CW %d" , i51Kernel.CacheWatcher ) ;

	i51Kernel.CacheWatcher -- ;
	
	if ( 0 != i51Kernel.CacheWatcher ) { 
		return iTRUE ;
	}
		 
	i51SlLoaderUnsetCache ( name ) ;  
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelUnsetCache E" ) ;
	#endif
	
	return iTRUE ;

}
#endif

iU32 i51KernelResGetContentSize ( iFILE  file , iU32 imageid ) 
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取指定其他资源的大小
	
	iU32 TextCount = 0 ;
	iU32 BinCount = 0 ;
	iU32 setoff = 0 ;
	iU32 size = 0;
	
//	I51_RES_BIN_INDEX_INFO i51ResBinIndexInfo = {0} ;

	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetContentSize S , %x , %x" , file ,imageid) ;
	#endif		
	
	i51AdeOsFileSetSeek ( file ,RES_COUNT_SETOFF  , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&TextCount ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&BinCount ) ;

	if ( imageid + 1 > BinCount || imageid + 1 <= 0  ) {

		i51AdeOsLog ( i51_LOG_DC , "ker i51KernelResGetContentSize BE") ;	
		return 0 ;
	}

	
	i51AdeOsFileSetSeek ( file ,  CRC_HEAD_RESCOUNT_COUNT + sizeof ( I51_RES_BIN_INDEX_INFO) * TextCount  + sizeof (I51_RES_BIN_INDEX_INFO) * imageid , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileSetSeek ( file ,RES_ID_STR_LEN  , i51_OS_FL_SEEK_CURRENT ) ;
	i51AdeOsFileReadInt32 ( file , &setoff ) ;
	i51AdeOsFileReadInt32 ( file , &size ) ;

	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( 0 , "i51KernelResGetContentSize E , ret = %x" , iTRUE ) ;
	#endif
	
	return size ;
	
}

iU32 i51KernelResGetContentEntry (iFILE  file , iU32 imageid ) 
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取指定其他资源的入口即文件的偏移量
	
	iU32 TextCount = 0 ;
	iU32 BinCount = 0 ;
	iU32 setoff = 0 ;
	iU32 size = 0;
	
//	I51_RES_BIN_INDEX_INFO i51ResBinIndexInfo = {0} ;

	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( 0 , "i51KernelResGetContentEntry S , %x , %x" , file ,imageid) ;
	#endif	
	
	i51AdeOsFileSetSeek ( file ,  RES_COUNT_SETOFF, i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&TextCount ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&BinCount ) ;

	if ( imageid + 1 > BinCount || imageid + 1 <= 0  ) {

		i51AdeOsLog ( i51_LOG_DC , "ker  i51KernelResGetContentEntry BE") ;	
		return 0 ;
	}
	
	i51AdeOsFileSetSeek ( file ,  CRC_HEAD_RESCOUNT_COUNT + sizeof ( I51_RES_BIN_INDEX_INFO) * TextCount  + sizeof (I51_RES_BIN_INDEX_INFO) * imageid , i51_OS_FL_SEEK_HEAD ) ;
//	i51AdeOsFileRead ( file , &i51ResBinIndexInfo , sizeof( i51ResBinIndexInfo ) , 0 ) ;
	i51AdeOsFileSetSeek ( file ,RES_ID_STR_LEN  , i51_OS_FL_SEEK_CURRENT ) ;	i51AdeOsFileReadInt32 ( file , &setoff ) ;
	i51AdeOsFileReadInt32 ( file , &size ) ;
	#ifdef i51KERNEL_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetContentIDString E ") ;
	#endif
	
	return setoff ;
	

}

iBOOL i51KernelGetApplicationName( iU16 *SrcName , iU8 * GetName)
{

	//	author : william
	//	since : 2012.3.9
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	输入应用名获取其文件结构中的中文名称。
	
	iU16 FullName[64] = {0} ;
	iFILE File = iNULL;
	iU8 RetFile = iFALSE;

	memset (FullName , 0 , sizeof(FullName)) ;
	i51AdeStdWStrcpy (FullName ,SrcName) ;
	i51AdeStdWStrcat (FullName ,L"\\") ;
	i51AdeStdWStrcat (FullName ,SrcName) ;
	i51AdeStdWStrcat (FullName ,L".i51") ;

	if ( iFALSE==i51AdeOsFileIsExist(FullName) )  goto ERROR_LABLE;

	File = i51AdeOsFileOpen ( FullName , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
	if( iNULL==File ) goto ERROR_LABLE;

	RetFile = i51AdeOsFileSetSeek(File, 8, i51_OS_FL_SEEK_HEAD);
	if( iNULL==RetFile ) goto ERROR_LABLE;

	RetFile = i51AdeOsFileRead(File, GetName, 32, 0);
	if( iNULL==RetFile ) goto ERROR_LABLE;
	
	i51AdeOsFileClose(File);
	File = iNULL;
	return iTRUE;

ERROR_LABLE:
	
	i51AdeOsFileClose(File);
	File = iNULL;
	return iFALSE;
	
}

iBOOL i51KernelRunLap ( iU16* name , iBOOL (*i51KitMain) ( iS32 , iU32 , void* ) , iU32* (*i51KitPort) () , void* parameter )
{

	//	author : william
	//	since : 2012.5.30
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	Run Lap = Library-based app

	iBOOL ret = 0 ;
	i51KERNELKIT* i51Kit = 0 ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Run LAP S" ) ;
	
	i51Kit = (i51KERNELKIT* ) i51KernelKitListCreateKit ( 
		
		name , 
		i51KitMain , 
		i51KitPort , 
		(iHANDLE)i51KitMain , 
		i51_APP_STATUS_SOBER , 
		iFALSE , 
		i51_KERNEL_KIT_SCOPE_DYNAMIC , 
		22 
		
	) ;
	
	if ( 0 == i51Kit ) {
		i51AdeOsLog ( i51_LOG_SC , "KEL : R LAP F" ) ;	
		return iFALSE ;
	}

	i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;

	ret = i51Kit->i51KitMain ( i51Kit->id , i51_MSG_INIT , (void*)i51KIT_STATIC_KIT_GET_MAPING(Adapter) ) ;
		
	if ( iFALSE == ret ) {
		
		i51AdeOsLog ( i51_LOG_SC , "KEL : Lap Main F" ) ;
		//	发送暂停消息
		i51KernelSendKitMsg ( i51Kit->id , i51_MSG_PAUSE , 0 ) ; 			
		i51KernelKitListDropKit ( (iHANDLE )i51Kit->id ) ;
		i51KernelReleaseFrontEnd () ;					
		
		return iFALSE ;
		 
	}

	//	Jelo Edit Start 2011.12.26
	//	if ( iTRUE == frontapp ) {
		//	需要在RUN 消息之前创建前台环境
		//	否则某些平台会出现未定义错误。
	//		i51KernelStartTimer ( 45 , i51KernelRunTimerLogical ) ;
	//	}
	
	i51Kit->i51KitMain ( i51Kit->id , i51_MSG_RUN , parameter ) ;
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Run LAP Finished" ) ;
	
	return iTRUE ;

}
