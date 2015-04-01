
/*

	The Implementing of 'Adapter' , Module : 'OS'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef i51ADEOS_H
#define i51ADEOS_H

#include "i51.h"

//	Memory System
extern void* i51AdeOsMalloc ( iU32 length , const iCHAR* file , iU32 line ) ;
extern iBOOL i51AdeOsFree ( void* address ) ;
extern void* i51AdeOsMallocEx ( iU32 length ) ;
extern iBOOL i51AdeOsFreeEx ( void* address ) ;
extern iU32 i51AdeOsGetTotallMemoryLeft ( void ) ;
#ifdef i51PROFILINGS
extern iU64 i51AdeOsGetTotallMemory (void) ;
extern iU64 i51AdeOsGetTotallMemoryUsed (void) ;
#endif

//	File System
extern void i51AdeOsFileAModeEnable (void) ;
extern void i51AdeOsFileAModeDisable (void) ;
extern iBOOL i51AdeOsFileCreateDir ( const iU16* path ) ;
extern iBOOL i51AdeOsFileDeleteDir ( const iU16* path ) ;
extern iBOOL i51AdeOsFileDelete ( const iU16* path  ) ;
extern iBOOL i51AdeOsFileIsExist ( const iU16* path ) ;
extern iBOOL i51AdeOsFileIsDirExist ( const iU16* path  ) ;
extern iBOOL i51AdeOsFileRename ( const iU16* oldname , const iU16* newname ) ;
extern iBOOL i51AdeOsFileCopy ( iU16* srcpath  , iU16* destpath ) ;
extern iBOOL i51AdeOsFileMove ( iU16* srcpath  , iU16* destpath ) ;
extern iU32 i51AdeOsFileGetAttr ( const iU16* path  ) ;
extern iBOOL i51AdeOsFileSetAttr ( const iU16* path , iU32 attributes ) ;
extern iFILE i51AdeOsFileOpen ( const iU16* path , iU32 mode , const iCHAR* fl , iS32 line ) ;
extern iBOOL i51AdeOsFileClose ( iFILE file ) ;
extern iBOOL i51AdeOsFileRead ( iFILE file , void* DataPtr , iU32 Length , iU32* Read ) ;
extern iBOOL i51AdeOsFileWrite ( iFILE file , void* DataPtr , iU32 Length, iU32* Written ) ;
extern iBOOL i51AdeOsFileSetSeek ( iFILE file , iS32 offset , iS32 seek ) ;
extern iBOOL i51AdeOsFileGetSeek ( iFILE file , iS32* position , iS32 seek ) ;
extern iBOOL i51AdeOsFileRename ( const iU16* name , const iU16* newname ) ;
extern iBOOL i51AdeOsFileGetSize ( iFILE file , iS32* size ) ;
extern iBOOL i51AdeOsFileReadInt8 ( iFILE file , iU8* integer8 ) ;
extern iBOOL i51AdeOsFileReadInt16 ( iFILE file , iU16* integer16 ) ;
extern iBOOL i51AdeOsFileReadInt32 ( iFILE file , iU32* integer32 ) ;
extern iBOOL i51AdeOsFileWriteInt8 ( iFILE file , iU8* integer8 ) ;
extern iBOOL i51AdeOsFileWriteInt16 ( iFILE file , iU16* integer16 ) ;
extern iBOOL i51AdeOsFileWriteInt32 ( iFILE file , iU32* integer32 ) ;
extern iFILE i51AdeOsFileFind ( const iU16* path , i51FILEINFO* info ) ;
extern iBOOL i51AdeOsFileFindNext ( iFILE filehandle , i51FILEINFO* info ) ;
extern iBOOL i51AdeOsFileFindClose ( iFILE file ) ;
extern iBOOL i51AdeOsFileFlush ( iFILE file ) ;
extern iCHAR* i51AdeOsGetPathSymbol (void) ;
extern iCHAR* i51AdeOsGetPathSymbolEx (void) ;
extern iBOOL i51AdeOsMMemCardExist (void) ;
#ifdef i51PROFILINGS
extern iU32 i51AdeOsFileTotallOpened ( void ) ;
#endif

//	Timer
//	0
//	1
extern iU32 i51AdeOsTimerCreate ( iBOOL loop , iU32 priority ) ;
extern iBOOL i51AdeOsTimerStart ( iU32 timerid , i51TIMERCALLBACK callback , iU32 time ) ;
extern iBOOL i51AdeOsTimerStop ( iU32 id ) ;

#ifdef i51PROFILINGS
extern iU32 i51AdeOsTimerGetLive ( void ) ;
#endif

extern iBOOL  i51AdeOsPermanitBufferRead ( iU8* buffer , iU32 length ) ;
extern iBOOL i51AdeOsPermanitBufferWrite ( iU8* buffer , iU32 length ) ;
	
extern iU32 i51AdeOsGetDiskSpace ( void ) ;
extern iU32 i51AdeOsGetDiskSpaceLeft ( void ) ;
extern iU32 i51AdeOsGetSysDiskSpace ( void ) ;
extern iU32 i51AdeOsGetSysDiskSpaceLeft ( void ) ;
extern iU32 i51AdeOsGetCallingTime ( void ) ;
extern iU32 i51AdeOsGetNetFlow ( void ) ;
extern iU32 i51AdeOsGetTick ( void ) ;
extern iBOOL i51AdeOsGetTime ( iU16* year , iU16* month , iU16* day , iU16* hour , iU16* mins , iU16* sec ) ;
extern iBOOL i51AdeOsSetTime ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec ) ;
extern iU64 i51AdeOsGetSeconds ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec )  ;
extern iBOOL i51AdeOsSecond2Time ( iU64 TotallSeconds , iU16 *year , iU16 *month , iU16 *day , iU16 *hour , iU16 *mins , iU16 *sec ) ;
extern iBOOL i51AdeOsSetTime ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec ) ;
extern iU64 i51AdeOsGetSeconds ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec )  ;
extern iBOOL i51AdeOsSecond2Time ( iU64 TotallSeconds , iU16 *year , iU16 *month , iU16 *day , iU16 *hour , iU16 *mins , iU16 *sec ) ;
extern iBOOL i51AdeOsSleep ( iU32 ms ) ;
extern void i51AdeOsSysLog ( iU16 logIndex , ... );
extern void i51AdeOsLog ( iS32 module , const iCHAR* log , ... ) ;
extern void i51AdeOsLogEx ( iS32 module , iU8 *buf , iU32 buflen ) ;
extern iBOOL i51AdeOsGetBaseStation ( iS32 simCard , iU32* bsid , iU32* zid ) ;
extern iBOOL i51AdeOsSetAccelerator ( i51ACCCALLBACK callback ) ;
extern iU32 i51AdeOsCreatePhone () ;
extern iBOOL i51AdeOsStartPhoneCall ( iU32 phoneid , iCHAR* number , void (*callback) ( iBOOL results) ) ; 
extern iBOOL i51AdeOsStopPhoneCall ( iU32 phoneid ) ;
extern iBOOL i51AdeOsReleasePhone ( iU32 phoneid ) ;
extern iBOOL i51AdeOsSetBrowserPage ( iCHAR* homepage ) ;
extern iBOOL i51AdeOsSetShock ( iBOOL open ) ;
extern iBOOL i51AdeOsDoShock ( iU32 time ) ;
extern iBOOL i51AdeOsSetScreenLight ( iBOOL open ) ;
extern iBOOL i51AdeOsGetScreenLight () ;
extern iBOOL i51AdeOsScreenAlwaysLight ( iBOOL always ) ;
extern iBOOL i51AdeOsSetCPUCache (iU32 start_addr, iU32 end_addr) ;
extern iBOOL i51AdeOsUnsetCPUCache () ;
extern iBOOL i51AdeOsRunWapExplorer ( iU16* url ) ;
extern iBOOL i51AdeOsCreate ( i51ADECALLBACK callback ) ;
extern iBOOL i51AdeOsRelease ( void ) ;

//	ÄÚ´æÐ¹Â¶¼ì²â
#ifdef i51PROFILINGS	
extern int MMTInit (void) ;
extern int MMTCheckLeaked (void) ;
extern void MMTCheckOverflow (void) ;
#define i51MemoryLeakCheckInit() MMTInit () ;
#define i51MemoryLeakCheck() MMTCheckLeaked () ;
#define i51MemoryOverflowCheck() MMTCheckOverflow () ;

#else
#define i51MemoryLeakCheckInit() 
#define i51MemoryLeakCheck() 
#define i51MemoryOverflowCheck() 
#endif

#ifdef i51KC_FILE_LEAK_CHECK	
extern void i51FlMonitorInit () ;
extern void i51FlMonitorCheckLeak () ;
#define i51FLLeakCheckInit() i51FlMonitorInit () ;
#define i51FLLeakCheck() i51FlMonitorCheckLeak () ; 

#else
#define i51FLLeakCheckInit()  
#define i51FLLeakCheck() 
#endif

#ifdef i51STATIC

#define i51FILEOPEN(file,path,mode)\
	i51AdeOsFileAModeEnable ();\
	file = i51AdeOsFileOpen ( path , mode , __FILE__ , __LINE__ ) ;\
	i51AdeOsFileAModeDisable () ;

#define i51FILECLOSE(handle)\
	i51AdeOsFileClose ( handle ) ;

#define i51MALLOC(length) i51AdeOsMalloc ( length , __FILE__,__LINE__ ) 
#define i51FREE(address) i51AdeOsFree ( address ) 

#endif

#endif
