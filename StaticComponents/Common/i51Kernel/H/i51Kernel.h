
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Kernel_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef THE_i51KERNEL_H
#define THE_i51KERNEL_H

#include "i51.h"

typedef enum {

	//	author : Jelo	
	//	since : 2011.11.4
	//	(C) PKIG Tech. Co., Ltd.

	//	KERNEL 初始化的状态

	//	尚未初始化，或初始化失败
	i51_KERNEL_INIT_FAILED = 0 ,
	//	正在初始化
	i51_KERNEL_INIT_GOING  ,
	//	已经初始化
	i51_KERNEL_INIT_SUCCEED ,
	
} i51KERNELINIT ;

typedef enum {

	//	author : Jelo	
	//	since : 2011.11.04
	//	(C) PKIG Tech. Co., Ltd.	

	//	退出静态i51应用
	i51_KERNEL_EXIT_SAP = 1 ,
	//	退出动态i51应用
	i51_KERNEL_EXIT_DAP = 1 << 1 ,
	
} i51KERNELEXIT ;

#endif

extern iBOOL i51KernelCheckAppCompleten  ( iU16* floder ) ;
extern iBOOL i51KernelInit ( void (*callback)(iBOOL results) ) ; 
extern iBOOL i51KernelPause () ;
extern iBOOL i51KernelResume () ;
extern iBOOL i51KernelExit ( iU32 mode ) ;
extern iBOOL i51KernelSendEvent ( iU32 events , iU32 parameter ) ;
extern iBOOL i51KernelUnloadKit ( iHANDLE handle ) ;
extern iBOOL i51KernelUnloadSysKit ( iHANDLE handle ) ;
extern iBOOL i51KernelSetFPS ( iU32 appid , iU32 fps ) ; 
extern iBOOL i51KernelMakeSnapshot ( iU32 appid ) ;
extern iBOOL i51KernelDropSnapshot ( iU32 appid ) ; 
extern iBOOL i51KernelSendRequest ( iU32 appid , iU16* name , iU32 request , void* buffer , iU32 buffsize ) ; 
extern iBOOL i51KernelSendMessage ( iU32 srcid , iU32 desid , iU32 message , void* buffer , iU32 buffsize ) ;
extern iU16* i51KernelGetApplicationPath () ;
extern iU16* i51KernelGetSystemPath () ;
extern iBOOL i51KernelCheckElder ( iU16* floder ) ;
extern iU32 i51KernelGetInitStatus () ;
extern iS32 i51KernelRunStaticApp ( iU16* name , iU8* buffer , iU32 buflen , void* parameter , iBOOL staticsope , iBOOL frontapp ) ;
extern iS32 i51KernelRunDynamicApp ( iU16* name , void* parameter ) ;
extern iU32* i51KernelLoadKit ( iU16* kitpath ) ;
extern iU32* i51KernelLoadSysKit ( iU16* kitpath ) ;
extern iS32 i51KernelLoadApp ( iU16* floder , void* parameter , iBOOL sober ) ;
extern iU32* i51KernelGetApplicationInterface ( iU32 appid ) ;
extern iU32 i51KernelGetApplicationId ( iU16* floder ) ;
extern iBOOL i51KernelChangeStatus ( iU32 aid , iU32 status ) ;
extern iBOOL i51KernelExtractPackage ( iFILE handle , iU16 *AppName ) ;
extern iBOOL i51KernelSerialNumberValid () ;
extern iBOOL i51KernelGenerateSerialNumber ( void (*callback)( iBOOL results , void* buffer , iU32 buflen ) ) ;
extern iBOOL i51KernelGenerateSerialNumberStop () ;
extern iBOOL i51KernelGetSerialNumber ( void** buffer , iU8* contentlen ) ;
extern iCHAR* i51KernelGeti51Version () ;
extern iU32 i51KernelGetVersion () ;
extern iBOOL i51KernelGetBatchCode ( iU8* buffer , iU32 bufsize , iU32* length ) ;
extern iU32 i51KernelGetDiskFreeSpace () ;
extern iBOOL i51KernelInstallApplication ( iU16 *AppName , void (*callback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount ) ) ;
extern iBOOL i51KernelInstallApplicationStop () ;
extern iBOOL i51KernelUninstallApplication ( iU16* appname ) ;
extern iBOOL i51KernelReinstallApplication ( iU16* appname , iCHAR *uri , iU32 version , void (*callback)( iCHAR* currentstep , iS32 status , iU32 step , iU32 totall, iU32 curdl , iU32 dlcount ) ) ;
extern iBOOL i51KernelReinstallApplicationStop () ;
extern iBOOL i51KernelSetCache ( iU16* name ) ;
extern iBOOL i51KernelUnsetCache ( iU16* name ) ;
extern iS32 i51KernelFormatVersion ( iCHAR* ver ) ;
extern iU32 i51KernelGetApplicationList ( iU8 (*proc)(void *arg, iU16* GetName), void *arg) ;
extern iU32 i51KernelResGetContentSize ( iFILE  file , iU32 imageid ) ;
extern iU32 i51KernelResGetContentEntry (iFILE  file , iU32 imageid ) ;
extern iBOOL i51KernelGetApplicationName( iU16 *SrcName , iU8 * GetName) ;
extern iBOOL i51KernelRunLap ( iU16* name , iBOOL (*i51KitMain) ( iS32 , iU32 , void* ) , iU32* (*i51KitPort) () , void* parameter ) ;
extern iCHAR *i51KernelGeti51Path();
extern iBOOL i51KernelCheckKitCrc(iFILE file);

