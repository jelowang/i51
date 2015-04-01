 
/*

	Common Definations of 'i51'
	For more details about 'i51' you could read : "The_i51_Architecture_2.0.xls"	
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef THE_i51KITADE_H
#define THE_i51KITADE_H

#include "i51.h"

typedef enum {
	
	//	author : Jelo
	//	since : 2011.10.24
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	i51_RES_240x320 = 1 ,
	i51_RES_240x400 = 1 << 1 ,
	i51_RES_320x240 = 1 << 2 ,
	i51_RES_320x480 = 1 << 3 ,
	i51_RES_272x480 = 1 << 4 ,
	
} i51RES ;

i51KIT_STATIC_KIT_DEF_BEGIN
	//	OS
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsMalloc)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFree)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetTotallMemoryLeft)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileCreateDir) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileFind) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileFindNext)  
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileFindClose) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileDeleteDir)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileOpen) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileClose) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileRead) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileWrite) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileSetSeek) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileGetSeek) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileRename) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileGetSize) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileDelete) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileGetAttr) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileSetAttr) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileFlush) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileCopy) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileMove) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileIsExist) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileIsDirExist) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileReadInt8) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileReadInt16) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileReadInt32) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileWriteInt8) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileWriteInt16) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileWriteInt32) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsTimerCreate) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsTimerStart) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsTimerStop) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetCallingTime) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetNetFlow) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetTick) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetTime) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsLog) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetBaseStation)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsSetAccelerator) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsCreatePhone)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsStartPhoneCall) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsStopPhoneCall) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsReleasePhone)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsSetBrowserPage)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsSetShock) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsDoShock)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsSetScreenLight) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetScreenLight)	
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsScreenAlwaysLight) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsSetTime) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetSeconds) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsSecond2Time) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsSleep)

	//	MMI
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiGetScreenScale)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiGetPanel)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerCreate)  
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerRelease)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerPlay)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerPause)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerResume)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerStop)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerSetVolume)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerGetVolume)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiRecorderStart)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiRecorderStop)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiRecoderPlay)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiRecorderPlayStop)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiSetPanelUsed)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiUpdateScreen)

	//	SOC
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocHtonl)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocHtons) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocInetAton) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocGetHostByName) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocGet) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocConnect)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocSend)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocReceive)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocClose) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocGetLinkLayerMode) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocGetLinkLayerStatus)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocCreateLinkLayer)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocOpenLinkLayer)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocCloseLinkLayer)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSocReleaseLinkLayer)  

	//	SMS
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetSims)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetImsi) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetImei)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetCenter)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsCreateDevice)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsSend)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsReleaseDevice)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsCreateHook)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsSetHook)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsReleaseHook)

	//	STD
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdCrc32)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdWSprintf) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdEntryEditor) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdAscii2Unicode) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdUnicode2Ascii) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdMemset16) 
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdMemcpy16)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdWStrlen)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdWStrcpy)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdWStrcmp)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdWStrcat)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeStdUncompress) 

	//	CNF
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeCnfGetVersion)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeCnfGetProjectId)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeCnfGetCustomerId)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeCnfGetPlatformId)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeCnfGetEndian)
	
	//	KERNEL
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelLoadKit)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelLoadSysKit)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelLoadApp)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelUnloadKit)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelUnloadSysKit) 
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelMakeSnapshot)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelDropSnapshot)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelSetFPS)	
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelSendRequest)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelSendMessage)	
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGetApplicationId)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelChangeStatus)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelExtractPackage)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelSerialNumberValid)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGenerateSerialNumber)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGenerateSerialNumberStop)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGetSerialNumber)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGetVersion)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGetBatchCode)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGetDiskFreeSpace)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelInstallApplication)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelInstallApplicationStop)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelReinstallApplication)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelReinstallApplicationStop)	
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelUninstallApplication)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGetApplicationInterface)
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelCheckAppCompleten)
	
	//	Sl-API
	i51KIT_STATIC_KIT_DEF_PORT(i51SlDlCreate) 
	i51KIT_STATIC_KIT_DEF_PORT(i51SlDlStop)
	i51KIT_STATIC_KIT_DEF_PORT(i51SlDlStart)
	
	//	C STD
	i51KIT_STATIC_KIT_DEF_PORT(memchr)
	i51KIT_STATIC_KIT_DEF_PORT(memcmp)
	i51KIT_STATIC_KIT_DEF_PORT(memcpy)
	i51KIT_STATIC_KIT_DEF_PORT(memmove)  
	i51KIT_STATIC_KIT_DEF_PORT(memset)
	i51KIT_STATIC_KIT_DEF_PORT(strcat)
	i51KIT_STATIC_KIT_DEF_PORT(strchr)
	i51KIT_STATIC_KIT_DEF_PORT(strcmp)
	i51KIT_STATIC_KIT_DEF_PORT(strcoll)
	i51KIT_STATIC_KIT_DEF_PORT(strcpy)
	i51KIT_STATIC_KIT_DEF_PORT(strcspn)
	i51KIT_STATIC_KIT_DEF_PORT(strlen)
	i51KIT_STATIC_KIT_DEF_PORT(strncat)
	i51KIT_STATIC_KIT_DEF_PORT(strncmp)
	i51KIT_STATIC_KIT_DEF_PORT(strncpy)
	i51KIT_STATIC_KIT_DEF_PORT(strpbrk)
	i51KIT_STATIC_KIT_DEF_PORT(strrchr) 
	i51KIT_STATIC_KIT_DEF_PORT(strspn)
	i51KIT_STATIC_KIT_DEF_PORT(strstr)
	i51KIT_STATIC_KIT_DEF_PORT(strxfrm)
	i51KIT_STATIC_KIT_DEF_PORT(rand)	
	i51KIT_STATIC_KIT_DEF_PORT(srand)
	i51KIT_STATIC_KIT_DEF_PORT(sprintf)	

	//	MMI Extension 2011.12.28
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiGetTextAttr)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiDrawText)

	//	Kernel Extension 2012.1.6
	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGeti51Version)

	//	Sms Extension 2012.3.3
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetPhoneBook)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsSendEx)

	//	Os Extension 2012.3.6
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsRunWapExplorer)

	//	Cnf Extension 2012.3.29
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeCnfGetType)

	//	Cnf Extension 2012.3.31
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiUpdateScreenEx)

	//	Sl-API Extension 2012.4.16
	i51KIT_STATIC_KIT_DEF_PORT(i51SlDlAbleCreate)

	//	MMI Extension 2012.4.19
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiRecorderFormat)

	//	MMI Extension 2012.4.25
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiMediaPlayerFormat)

	//	MMI Extension 2012.7.6
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeMmiRecorderStartEx)

i51KIT_STATIC_KIT_DEF_END


//	Type of i51KIT Interfaces belllow
//	OS
i51KIT_STATIC_KIT_DEF_TYPE(void*,i51AdeOsMalloc, ( iU32 length , const iCHAR* file , iU32 line ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFree, ( void* address ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsGetTotallMemoryLeft, ( void ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileCreateDir, ( const iU16* path ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iFILE,i51AdeOsFileFind, ( const iU16* path , i51FILEINFO* info ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileFindNext, ( iFILE filehandle , i51FILEINFO* info ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileFindClose, ( iFILE file ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileDeleteDir, ( const iU16* path ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iFILE,i51AdeOsFileOpen, ( const iU16* path , iU32 mode , const iCHAR* fl , iS32 line ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileClose, ( iFILE file ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileRead, ( iFILE file , void* DataPtr , iU32 Length , iU32* Read ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileWrite, ( iFILE file , void* DataPtr , iU32 Length, iU32* Written ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileSetSeek, ( iFILE file , iS32 offset , iS32 seek ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileGetSeek, ( iFILE file , iS32* position , iS32 seek ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileRename, ( const iU16* name , const iU16* newname ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileGetSize, ( iFILE file , iS32* size ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileDelete, ( const iU16* path  ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsFileGetAttr, ( const iU16* path  ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileSetAttr, ( const iU16* path , iU32 attributes ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileFlush, ( iFILE file ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileCopy, ( iU16* srcpath  , iU16* destpath ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileMove, ( iU16* srcpath  , iU16* destpath ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileIsExist, ( const iU16* path ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileIsDirExist, ( const iU16* path  ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileReadInt8, ( iFILE file , iU8* integer8 ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileReadInt16, ( iFILE file , iU16* integer16 ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileReadInt32, ( iFILE file , iU32* integer32 ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileWriteInt8, ( iFILE file , iU8* integer8 ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileWriteInt16, ( iFILE file , iU16* integer16 ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsFileWriteInt32, ( iFILE file , iU32* integer32 ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsTimerCreate, ( iBOOL loop , iU32 priority ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsTimerStart, ( iU32 timerid , i51TIMERCALLBACK callback , iU32 time ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsTimerStop, ( iU32 id ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsGetCallingTime, ( void ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsGetNetFlow, ( void ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsGetTick, ( void ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsGetTime, ( iU16* year , iU16* month , iU16* day , iU16* hour , iU16* mins , iU16* sec ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeOsLog, ( iS32 module , const iCHAR* log , ... ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsGetBaseStation, ( iS32 simCard , iU32* bsid , iU32* zid ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsSetAccelerator, ( i51ACCCALLBACK callback ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsCreatePhone,() )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsStartPhoneCall,(iU32 phoneid , iCHAR* number , void (*callback) ( iBOOL results)))
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsStopPhoneCall,(iU32 phoneid) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsReleasePhone,(iU32 phoneid) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsSetBrowserPage,(iCHAR* homepage) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsSetShock, ( iBOOL open ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsDoShock,( iU32 time ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsSetScreenLight, ( iBOOL open ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsGetScreenLight,() )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsScreenAlwaysLight, ( iBOOL Always ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsSetTime, ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU64,i51AdeOsGetSeconds, ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsSecond2Time, ( iU64 TotallSeconds , iU16 *year , iU16 *month , iU16 *day , iU16 *hour , iU16 *mins , iU16 *sec ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsSleep, ( iU32 ms ) ) 
//	OS Extension 2012.3.6
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsRunWapExplorer,( iU16* url ))

//	MMI
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiGetScreenScale, ( iU16* width , iU16* height ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32*,i51AdeMmiGetPanel, () ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerCreate, ( void (*callback) ( iBOOL results )) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerRelease,() ) 
//i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerInsertDisk, ( iU32 player , void* disk , iU32 length , i51MMIDEC dec ) ) 
//i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerBackDisk, ( iU32 player ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerPlay, ( iU32 priority , void* handle , iU32 length , iU32 type , i51MMIDEC dec , iBOOL loop , void (*STOP)(void* handle) ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerPause, ( void* handle ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerResume, ( void* handle ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerStop, ( void* handle ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiMediaPlayerSetVolume, ( iU32 volume ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeMmiMediaPlayerGetVolume, () ) 
i51KIT_STATIC_KIT_DEF_TYPE(iS32,i51AdeMmiRecorderStart, ( void* buffer , iU32 bufflen , void (*callback ) ( void* stream , iU32 length ) ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiRecorderStop, ( void (*callback) ( iBOOL results ) ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiRecoderPlay, ( void* stream , iU32 length , void (*callback) ( iBOOL results ) ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiRecorderPlayStop, () ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeMmiSetPanelUsed, ( void* panel ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeMmiUpdateScreen, () ) 

//	MMI Extension 2011.12.28
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeMmiGetTextAttr,(iU16* text , iU16* textwidth , iU16* textheight))
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeMmiDrawText,(iU8* panel , iU16 width , iU16 height , const iU16* text , iS32 desx , iS32 desy , iCOLOR color))
//	MMI Extension 2012.3.31
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeMmiUpdateScreenEx, ( iRECT* rect , iU32 recttotall ) ) 
//	MMI Extension 2012.4.19
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeMmiRecorderFormat, ( iU32* samples , iU32* bitscale , iU32* chanels ) ) 
//	MMI Extension 2012.4.25
i51KIT_STATIC_KIT_DEF_TYPE(iS32,i51AdeMmiMediaPlayerFormat , () ) 
//	MMI Extension 2012.7.6
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeMmiRecorderStartEx , ( iU16* filename , void (*callback ) ( iBOOL finished ) ) ) 



//	SOC
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeSocHtonl, ( iU32 value ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU16,i51AdeSocHtons, ( iU16 value ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSocInetAton, ( iCHAR* ip , iU32* value ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iS32,i51AdeSocGetHostByName, ( iCHAR* name  , i51DNSCALLBACK callback ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iS32,i51AdeSocGet, ( void ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeSocConnect, ( iS32 socket , i51SOCADDR* address , i51SOCCONCBK callback ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeSocSend, ( iS32 socket , void* data , iU32 length , i51SOCSENDCBK callback ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeSocReceive, ( iS32 socket , void* data , iU32 length , i51SOCRECVCBK callback ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSocClose, ( iS32 socket ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iS32*,i51AdeSocGetLinkLayerMode, () ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeSocGetLinkLayerStatus, ( iBOOL* linked , iS32* simid , iS32* mode ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeSocCreateLinkLayer, ( iS32 mode , iS32 sim  ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSocOpenLinkLayer, ( iU32 aid , iU32 layerid , i51LLAYERCALLBACK callback  ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSocCloseLinkLayer, ( iU32 aid , iU32 layerid  ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSocBuildLinkLayer, ( iS32 mode , iS32 sim , i51LLAYERCALLBACK callback  ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSocReleaseLinkLayer, ( iU32 layerid ) ) 

//	SMS
i51KIT_STATIC_KIT_DEF_TYPE(iS32*,i51AdeSmsGetSims, ( void ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,i51AdeSmsGetImsi, ( iS32 sim ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,i51AdeSmsGetImei, ( void ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,i51AdeSmsGetCenter, ( iS32 sim ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeSmsCreateDevice,() )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSmsSend, ( iU32 device , iS32 sim , iCHAR* phone , iU16* contents , iBOOL save , void (*callback)( iBOOL results ) ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSmsReleaseDevice,( iU32 device ) )
i51KIT_STATIC_KIT_DEF_TYPE(iS32,i51AdeSmsCreateHook, ( iS32 type ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSmsSetHook, ( iS32 hookid , i51SMSHOOKCALLBACK callback ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSmsReleaseHook, (  iS32 hookid ) ) 
//	Sms Extension 2012.3.3
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSmsGetPhoneBook,( i51PHONEBOOK* phonebook , iU32* length ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeSmsSendEx, ( iU32 device , iS32 sim , iCHAR* phone , iCHAR* contents , iBOOL save , void (*callback)( iBOOL results ) ) ) 

//	STD
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeStdCrc32, ( iU8* buf , iS32 len , iU32** table ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeStdWSprintf, ( iU16 *buffer , iU32 bytes , const iCHAR* format , ... ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeStdEntryEditor, (  i51STDEDITOR* content ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU16*,i51AdeStdAscii2Unicode, ( const iCHAR* src , iU16* dst ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,i51AdeStdUnicode2Ascii, ( const iU16* src , iCHAR* dst ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeStdMemset16, ( void* dest , iU16 value , iU32 length ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeStdMemcpy16, ( void* dest , void* src , iU32 length ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeStdWStrlen, ( iU16* S ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU16*,i51AdeStdWStrcpy, (  iU16* T , iU16* S ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iS32,i51AdeStdWStrcmp, (  iU16* T , iU16* S ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU16*,i51AdeStdWStrcat, (  iU16* T , iU16* S ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeStdUncompress, ( iU8* dest , iU32* destlen , const iU8* source , iU32 srclen ) ) 

//	CNF
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeCnfGetVersion,() )
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeCnfGetProjectId,() )
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeCnfGetCustomerId,() )
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,i51AdeCnfGetPlatformId,() ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeCnfGetEndian,() )
//	CNF Extension 2012.3.29
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeCnfGetType,())

//	KERNEL
i51KIT_STATIC_KIT_DEF_TYPE(iU32*,i51KernelLoadKit, ( iU16* path ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iU32*,i51KernelLoadSysKit,(iU16* kitpath) )
i51KIT_STATIC_KIT_DEF_TYPE(iS32,i51KernelLoadApp, ( iU16* floder , void* parameter , iBOOL sober ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelUnloadKit,( iHANDLE handle ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelUnloadSysKit,( iHANDLE handle ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelMakeSnapshot, ( iU32 appid ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelDropSnapshot, ( iU32 appid ) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelSetFPS, ( iU32 appid , iU32 fps ) ) 
//i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelGetApplicationAttr, ( iU16* floder , iCHAR* appname , iU32* appv , iU32* kelv , iU32* adev ) ) 	
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelSendRequest,(iU32 appid , iU16* name , iU32 request , void* buffer , iU32 buffsize) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelSendMessage,(iU32 srcid , iU32 desid , iU32 message , void* buffer , iU32 buffsize ) )
//i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51KernelGetAppBitmap, ( iU16* floder ) )
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51KernelGetApplicationId, ( iU16* floder ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelChangeStatus, ( iU32 aid , iU32 status ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelExtractPackage, ( iFILE handle , iU16* floader ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelSerialNumberValid,() )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelGenerateSerialNumber,(void (*callback)( iBOOL results , void* buffer , iU32 buflen )) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelGenerateSerialNumberStop,() )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelGetSerialNumber,( void** buffer , iU8* contentlen ) )
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51KernelGetVersion, () )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelInstallApplication,( iU16 *AppName , void (*callback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total   , iU32 curdl , iU32 dlcount ) ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelInstallApplicationStop,() )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelReinstallApplication,(iU16* appname , iCHAR *uri ,  iU32 version , void (*callback)( iCHAR* currentstep , iS32 status , iU32 step , iU32 totall  , iU32 curdl , iU32 dlcount )) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelReinstallApplicationStop,() )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelUninstallApplication,(iU16* appname) )
i51KIT_STATIC_KIT_DEF_TYPE(iU32*,i51KernelGetApplicationInterface,(iU32 appid) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelCheckAppCompleten,(iU16* appname) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelGetBatchCode , ( iU8* buffer , iU32 bufsize , iU32* length ) )
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51KernelGetDiskFreeSpace , () )

//	Kernel Extension 2012.1.6
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51KernelGeti51Version, () )

//i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelSetCache,(iU16* name) ) 
//i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelUnsetCache,(iU16* name) ) 

//	Sl-API
i51KIT_STATIC_KIT_DEF_TYPE(iHANDLE,i51SlDlCreate,(void (*callback) (iBOOL ret)) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51SlDlStop,( iHANDLE dl ) )
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51SlDlStart,( iHANDLE dl , const iCHAR *uri , iU32 urisize ,  const iCHAR* path , const iCHAR* name , iU32 versionId , i51SlDlCOMMAND cmd , i51SlDlCALLBACK callback ) )
i51KIT_STATIC_KIT_DEF_TYPE(iS32,i51SlLoaderGetKitRef,( iFILE file , i51SllDKITREF* i51SllDKitRefBuf , iU32 MaxRef , iU32* i51SllDKitRefTotall ) )
//	Sl-API Extension 2012.4.16
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51SlDlAbleCreate,() )


//	C STD
i51KIT_STATIC_KIT_DEF_TYPE(void*,memchr,(const void*, iS32, iU32)) 
i51KIT_STATIC_KIT_DEF_TYPE(iS32,memcmp,(const void*, const void*, iU32))
i51KIT_STATIC_KIT_DEF_TYPE(void*,memcpy,(void*, const void*, iU32))
i51KIT_STATIC_KIT_DEF_TYPE(void*,memmove,(void*, const void*, iU32))
i51KIT_STATIC_KIT_DEF_TYPE(void*,memset,(void*, iS32, iU32))
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,strcat,(iCHAR*, const iCHAR*))
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,strchr,(const iCHAR*, iS32) ) 
i51KIT_STATIC_KIT_DEF_TYPE(iS32,strcmp,(const iCHAR*, const iCHAR*))  
i51KIT_STATIC_KIT_DEF_TYPE(iS32,strcoll,(const iCHAR*, const iCHAR*))
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,strcpy,(iCHAR*, const iCHAR*))
i51KIT_STATIC_KIT_DEF_TYPE(iS32,strcspn,(const iCHAR*, const iCHAR*))  
i51KIT_STATIC_KIT_DEF_TYPE(iS32,strlen,(const iCHAR*))
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,strncat,(iCHAR*, const iCHAR*, iU32))
i51KIT_STATIC_KIT_DEF_TYPE(iS32,strncmp,(const iCHAR*, const iCHAR*, iU32))  
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,strncpy,(iCHAR*, const iCHAR*, iU32))
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,strpbrk,(const iCHAR*, const iCHAR*)) 
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,strrchr,(const iCHAR*, iS32))
i51KIT_STATIC_KIT_DEF_TYPE(iS32,strspn,(const iCHAR*, const iCHAR*))  
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,strstr,(const iCHAR*, const iCHAR*))  
i51KIT_STATIC_KIT_DEF_TYPE(iS32,strxfrm,(iCHAR*, const iCHAR*, iU32))
i51KIT_STATIC_KIT_DEF_TYPE(iS32,rand,())	
i51KIT_STATIC_KIT_DEF_TYPE(void,srand,(iU32))
i51KIT_STATIC_KIT_DEF_TYPE(void,sprintf,(iCHAR*, const iCHAR*, ...))

//	Implementation of i51KIT Interfaces belllow
//	OS
#define i51AdeOsMalloc i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsMalloc)
#define i51AdeOsFree i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFree)
#define i51AdeOsGetTotallMemoryLeft i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetTotallMemoryLeft)
#define i51AdeOsFileCreateDir i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileCreateDir) 
#define i51AdeOsFileFind i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileFind) 
#define i51AdeOsFileFindNext i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileFindNext) 
#define i51AdeOsFileFindClose i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileFindClose) 
#define i51AdeOsFileDeleteDir i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileDeleteDir)
#define i51AdeOsFileOpen i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileOpen) 
#define i51AdeOsFileClose i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileClose) 
#define i51AdeOsFileRead i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileRead) 
#define i51AdeOsFileWrite i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileWrite) 
#define i51AdeOsFileSetSeek i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileSetSeek) 
#define i51AdeOsFileGetSeek i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileGetSeek) 
#define i51AdeOsFileRename i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileRename) 
#define i51AdeOsFileGetSize i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileGetSize) 
#define i51AdeOsFileDelete i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileDelete) 
#define i51AdeOsFileGetAttr i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileGetAttr) 
#define i51AdeOsFileSetAttr i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileSetAttr) 
#define i51AdeOsFileFlush i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileFlush) 
#define i51AdeOsFileCopy i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileCopy) 
#define i51AdeOsFileMove i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileMove) 
#define i51AdeOsFileIsExist i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileIsExist) 
#define i51AdeOsFileIsDirExist i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileIsDirExist) 
#define i51AdeOsFileReadInt8 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileReadInt8) 
#define i51AdeOsFileReadInt16 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileReadInt16) 
#define i51AdeOsFileReadInt32 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileReadInt32) 
#define i51AdeOsFileWriteInt8 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileWriteInt8) 
#define i51AdeOsFileWriteInt16 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileWriteInt16) 
#define i51AdeOsFileWriteInt32 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsFileWriteInt32) 
#define i51AdeOsGetDiskSpace i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetDiskSpace) 
#define i51AdeOsGetDiskSpaceLeft i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetDiskSpaceLeft) 
#define i51AdeOsTimerCreate i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsTimerCreate) 
#define i51AdeOsTimerStart i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsTimerStart) 
#define i51AdeOsTimerStop i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsTimerStop) 
#define i51AdeOsGetCallingTime i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetCallingTime) 
#define i51AdeOsGetNetFlow i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetNetFlow) 
#define i51AdeOsGetTick i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetTick) 
#define i51AdeOsGetTime i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetTime) 
#define i51AdeOsLog i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsLog) 
#define i51AdeOsGetBaseStation i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetBaseStation)
#define i51AdeOsSetAccelerator i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsSetAccelerator) 
#define i51AdeOsCreatePhone i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsCreatePhone) 
#define i51AdeOsStartPhoneCall i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsStartPhoneCall) 
#define i51AdeOsStopPhoneCall i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsStopPhoneCall) 
#define i51AdeOsReleasePhone i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsReleasePhone) 
#define i51AdeOsSetBrowserPage i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsSetBrowserPage)
#define i51AdeOsSetShock i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsSetShock) 
#define i51AdeOsDoShock i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsDoShock)
#define i51AdeOsSetScreenLight i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsSetScreenLight) 
#define i51AdeOsGetScreenLight i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetScreenLight)
#define i51AdeOsScreenAlwaysLight i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsScreenAlwaysLight) 
#define i51AdeOsSetTime i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsSetTime) 
#define i51AdeOsGetSeconds i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsGetSeconds) 
#define i51AdeOsSecond2Time i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsSecond2Time) 
#define i51AdeOsSleep i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsSleep) 
//	OS Extension 2012.3.6
#define i51AdeOsRunWapExplorer i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeOsRunWapExplorer) 

//	MMI
#define i51AdeMmiGetScreenScale i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiGetScreenScale)
#define i51AdeMmiGetPanel i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiGetPanel)
#define i51AdeMmiMediaPlayerCreate i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerCreate)
#define i51AdeMmiMediaPlayerRelease i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerRelease)
//#define i51AdeMmiMediaPlayerInsertDisk i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerInsertDisk)
//#define i51AdeMmiMediaPlayerBackDisk i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerBackDisk)
#define i51AdeMmiMediaPlayerPlay i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerPlay)
#define i51AdeMmiMediaPlayerPause i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerPause)
#define i51AdeMmiMediaPlayerResume i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerResume)
#define i51AdeMmiMediaPlayerStop i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerStop)
#define i51AdeMmiMediaPlayerSetVolume i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerSetVolume)
#define i51AdeMmiMediaPlayerGetVolume i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerGetVolume)
#define i51AdeMmiRecorderStart i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiRecorderStart)
#define i51AdeMmiRecorderStop i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiRecorderStop)
#define i51AdeMmiRecoderPlay i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiRecoderPlay)
#define i51AdeMmiRecorderPlayStop i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiRecorderPlayStop)
#define i51AdeMmiSetPanelUsed i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiSetPanelUsed)
#define i51AdeMmiUpdateScreen i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiUpdateScreen)

//	MMI Extension 2011.12.28
#define i51AdeMmiGetTextAttr i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiGetTextAttr)
#define i51AdeMmiDrawText i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiDrawText)
//	MMI Extension 2012.3.31
#define i51AdeMmiUpdateScreenEx i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiUpdateScreenEx)
//	MMI Extension 2012.4.19
#define i51AdeMmiRecorderFormat i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiRecorderFormat)
//	MMI Extension 2012.4.25
#define i51AdeMmiMediaPlayerFormat i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiMediaPlayerFormat )
//	MMI Extension 2012.7.6
#define i51AdeMmiRecorderStartEx i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeMmiRecorderStartEx)

//	SOC
#define i51AdeSocHtonl i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocHtonl)
#define i51AdeSocHtons i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocHtons)
#define i51AdeSocInetAton i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocInetAton)
#define i51AdeSocGetHostByName i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocGetHostByName)
#define i51AdeSocGet i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocGet)
#define i51AdeSocConnect i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocConnect)
#define i51AdeSocSend i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocSend)
#define i51AdeSocReceive i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocReceive)
#define i51AdeSocClose i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocClose)
#define i51AdeSocGetLinkLayerMode i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocGetLinkLayerMode)
#define i51AdeSocGetLinkLayerStatus i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocGetLinkLayerStatus) 
#define i51AdeSocCreateLinkLayer i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocCreateLinkLayer) 
#define i51AdeSocOpenLinkLayer i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocOpenLinkLayer) 
#define i51AdeSocCloseLinkLayer i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocCloseLinkLayer) 
#define i51AdeSocReleaseLinkLayer i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocReleaseLinkLayer) 
#define i51AdeSocBuildLinkLayer i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSocBuildLinkLayer)

//	SMS
#define i51AdeSmsGetSims i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsGetSims)
#define i51AdeSmsGetImsi i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsGetImsi)
#define i51AdeSmsGetImei i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsGetImei)
#define i51AdeSmsGetCenter i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsGetCenter)
#define i51AdeSmsCreateDevice i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsCreateDevice)
#define i51AdeSmsSend i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsSend)
#define i51AdeSmsReleaseDevice i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsReleaseDevice)
#define i51AdeSmsCreateHook i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsCreateHook)
#define i51AdeSmsSetHook i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsSetHook)
#define i51AdeSmsReleaseHook i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsReleaseHook)
//	Sms Extension 2012.3.3
#define i51AdeSmsGetPhoneBook i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsGetPhoneBook)
#define i51AdeSmsSendEx i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeSmsSendEx)

//	STD
#define i51AdeStdCrc32 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdCrc32)
#define i51AdeStdWSprintf i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdWSprintf)
#define i51AdeStdEntryEditor i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdEntryEditor)
#define i51AdeStdAscii2Unicode i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdAscii2Unicode)
#define i51AdeStdUnicode2Ascii i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdUnicode2Ascii)
#define i51AdeStdMemset16 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdMemset16)
#define i51AdeStdMemcpy16 i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdMemcpy16)
#define i51AdeStdWStrlen i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdWStrlen)
#define i51AdeStdWStrcpy i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdWStrcpy)
#define i51AdeStdWStrcmp i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdWStrcmp)
#define i51AdeStdWStrcat i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdWStrcat)
#define i51AdeStdUncompress i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeStdUncompress)

//	CNF
#define i51AdeCnfGetVersion i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeCnfGetVersion)
#define i51AdeCnfGetProjectId i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeCnfGetProjectId)
#define i51AdeCnfGetCustomerId i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeCnfGetCustomerId)
#define i51AdeCnfGetPlatformId i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeCnfGetPlatformId)
#define i51AdeCnfGetEndian i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeCnfGetEndian)
//	Kernel Extension 2012.3.29
#define i51AdeCnfGetType i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51AdeCnfGetType)


//	KERNEL
#define i51KernelLoadKit i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelLoadKit)
#define i51KernelLoadSysKit i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelLoadSysKit)
#define i51KernelLoadApp i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelLoadApp)
#define i51KernelUnloadKit i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelUnloadKit)
#define i51KernelUnloadSysKit i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelUnloadSysKit)
#define i51KernelMakeSnapshot i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelMakeSnapshot)
#define i51KernelDropSnapshot i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelDropSnapshot)
#define i51KernelSetFPS i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelSetFPS)
//#define i51KernelGetApplicationAttr i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGetApplicationAttr)
#define i51KernelSendRequest i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelSendRequest)
#define i51KernelSendMessage i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelSendMessage)
//#define i51KernelGetAppBitmap i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGetAppBitmap)
#define i51KernelGetApplicationId i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGetApplicationId)
#define i51KernelChangeStatus i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelChangeStatus)
#define i51KernelExtractPackage i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelExtractPackage)
#define i51KernelSerialNumberValid i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelSerialNumberValid)
#define i51KernelGenerateSerialNumber i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGenerateSerialNumber)
#define i51KernelGenerateSerialNumberStop i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGenerateSerialNumberStop)
#define i51KernelGetSerialNumber i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGetSerialNumber)
#define i51KernelGetVersion i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGetVersion)
#define i51KernelInstallApplication i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelInstallApplication)
#define i51KernelInstallApplicationStop i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelInstallApplicationStop)
#define i51KernelReinstallApplication i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelReinstallApplication)
#define i51KernelReinstallApplicationStop i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelReinstallApplicationStop)
#define i51KernelUninstallApplication i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelUninstallApplication)
#define i51KernelGetApplicationInterface i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGetApplicationInterface)
#define i51KernelCheckAppCompleten i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelCheckAppCompleten)
#define i51KernelGetBatchCode i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGetBatchCode)
#define i51KernelGetDiskFreeSpace i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGetDiskFreeSpace)
//	Kernel Extension 2012.1.6
#define i51KernelGeti51Version i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelGeti51Version)

//#define i51KernelSetCache i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelSetCache)
//#define i51KernelUnsetCache i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51KernelUnsetCache)

//	Sl-API
#define i51SlDlCreate i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51SlDlCreate)
#define i51SlDlStop i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51SlDlStop) 
#define i51SlDlStart i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51SlDlStart) 
//	Sl-API Extension 2012.4.16
#define i51SlDlAbleCreate i51KIT_STATIC_KIT_DEF_CALL(Adapter,i51SlDlAbleCreate) 

//	C STD
#define memchr i51KIT_STATIC_KIT_DEF_CALL(Adapter,memchr)
#define memcmp i51KIT_STATIC_KIT_DEF_CALL(Adapter,memcmp)
#define memcpy i51KIT_STATIC_KIT_DEF_CALL(Adapter,memcpy)
#define memmove i51KIT_STATIC_KIT_DEF_CALL(Adapter,memmove)
#define memset i51KIT_STATIC_KIT_DEF_CALL(Adapter,memset)
#define strcat i51KIT_STATIC_KIT_DEF_CALL(Adapter,strcat)
#define strchr i51KIT_STATIC_KIT_DEF_CALL(Adapter,strchr)
#define strcmp i51KIT_STATIC_KIT_DEF_CALL(Adapter,strcmp)
#define strcoll i51KIT_STATIC_KIT_DEF_CALL(Adapter,strcoll)
#define strcpy i51KIT_STATIC_KIT_DEF_CALL(Adapter,strcpy)
#define strcspn i51KIT_STATIC_KIT_DEF_CALL(Adapter,strcspn)
#define strlen i51KIT_STATIC_KIT_DEF_CALL(Adapter,strlen)
#define strncat i51KIT_STATIC_KIT_DEF_CALL(Adapter,strncat)
#define strncmp i51KIT_STATIC_KIT_DEF_CALL(Adapter,strncmp)
#define strncpy i51KIT_STATIC_KIT_DEF_CALL(Adapter,strncpy)
#define strpbrk i51KIT_STATIC_KIT_DEF_CALL(Adapter,strpbrk)
#define strrchr i51KIT_STATIC_KIT_DEF_CALL(Adapter,strrchr)
#define strspn i51KIT_STATIC_KIT_DEF_CALL(Adapter,strspn)
#define strstr i51KIT_STATIC_KIT_DEF_CALL(Adapter,strstr)
#define strxfrm i51KIT_STATIC_KIT_DEF_CALL(Adapter,strxfrm)
#define rand i51KIT_STATIC_KIT_DEF_CALL(Adapter,rand)
#define srand i51KIT_STATIC_KIT_DEF_CALL(Adapter,srand)
#define sprintf i51KIT_STATIC_KIT_DEF_CALL(Adapter,sprintf)

//	Static Kits of i51 Standard
i51KIT_STATIC_KIT_DEC(Adapter);

i51KIT_STATIC_KIT_DEF_BEGIN	
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsPermanitBufferRead)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsPermanitBufferWrite)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileAModeEnable)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsFileAModeDisable)
 	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetSysDiskSpace)
 	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetSysDiskSpaceLeft)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetDiskSpace)
	i51KIT_STATIC_KIT_DEF_PORT(i51AdeOsGetDiskSpaceLeft)
 	i51KIT_STATIC_KIT_DEF_PORT(i51KernelGeti51Path)	
 	i51KIT_STATIC_KIT_DEF_PORT(i51AdeCnfGetCpu)	
 	i51KIT_STATIC_KIT_DEF_PORT(atoi) 	
 	i51KIT_STATIC_KIT_DEF_PORT(i51MiniSlDlStart) 	
 	i51KIT_STATIC_KIT_DEF_PORT(i51MiniSlDlStop) 	
 	i51KIT_STATIC_KIT_DEF_PORT(i51KernelCheckKitCrc) 	
i51KIT_STATIC_KIT_DEF_END

//type definition
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsPermanitBufferRead,( iU8* buffer , iU32 length ))
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51AdeOsPermanitBufferWrite,( iU8* buffer , iU32 length ))
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeOsFileAModeEnable,())
i51KIT_STATIC_KIT_DEF_TYPE(void,i51AdeOsFileAModeDisable,())
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsGetSysDiskSpace,())
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsGetSysDiskSpaceLeft,())
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsGetDiskSpace,())
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeOsGetDiskSpaceLeft,())
i51KIT_STATIC_KIT_DEF_TYPE(iCHAR*,i51KernelGeti51Path,())
i51KIT_STATIC_KIT_DEF_TYPE(iU32,i51AdeCnfGetCpu,())
i51KIT_STATIC_KIT_DEF_TYPE(int,atoi,(const char *s))
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51MiniSlDlStart,(iHANDLE dl, const iCHAR * uri, iU32 urisize, const iCHAR * path, const iCHAR * name, iU32 versionId, i51SlDlCOMMAND cmd, i51SlDlCALLBACK callback))
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51MiniSlDlStop,(iHANDLE dl))
i51KIT_STATIC_KIT_DEF_TYPE(iBOOL,i51KernelCheckKitCrc,( iFILE file ))
   
#define i51AdeOsPermanitBufferRead i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeOsPermanitBufferRead)
#define i51AdeOsPermanitBufferWrite i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeOsPermanitBufferWrite)
#define i51AdeOsFileAModeEnable i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeOsFileAModeEnable)
#define i51AdeOsFileAModeDisable i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeOsFileAModeDisable)
#define i51AdeOsGetSysDiskSpace i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeOsGetSysDiskSpace)
#define i51AdeOsGetSysDiskSpaceLeft i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeOsGetSysDiskSpaceLeft)
#define i51AdeOsGetAppDiskSpace i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeOsGetDiskSpace)
#define i51AdeOsGetAppDiskSpaceLeft i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeOsGetDiskSpaceLeft)
#define i51KernelGeti51Path i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51KernelGeti51Path)
#define i51AdeCnfGetCpu i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51AdeCnfGetCpu)
#define atoi i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,atoi)
#define i51MiniSlDlStart i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51MiniSlDlStart)
#define i51MiniSlDlStop i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51MiniSlDlStop)
#define i51KernelCheckKitCrc i51KIT_STATIC_KIT_DEF_CALL(CoreAdapter,i51KernelCheckKitCrc)

i51KIT_STATIC_KIT_DEC(CoreAdapter);
#endif

