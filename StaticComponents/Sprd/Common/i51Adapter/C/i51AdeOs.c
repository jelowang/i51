  
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#include "i51AdeOs.h"
#include "i51AdeStd.h"
#include "i51_define.h"
#include "mmk_timer.h"
#include "mmi_common.h"
#include "mmi_filemgr.h"
#include "mmicom_time.h"
#if defined (SPRD_6600) || defined (SPRD_6620) || defined (SPRD_6800)
#include "mmicc.h"
#include "mmiconnection.h"
#include "mmiset.h"
#endif
#include "mmicl_internal.h"
#include "mmiaudio_ctrl.h "

#include "mmi_nv.h"

#if defined (SPRD_6600)
#include "mmibrowser.h" 
#endif
#if defined (SPRD_6620) || defined (SPRD_6800)
#include "mmipdp_api.h"
#endif
#include "sig_code.h "
#include "mn_api.h"

#ifdef SPRD_6530
#include "mmiconnection_export.h"
#include "mmibrowser_export.h"
#include "mmipdp_export.h"
#include "os_api.h"
#else
#include "mmibrowser_api.h"
#endif

#ifdef i51SIMULATOR 
#define I51ADEMALLOC SCI_ALLOC_APP
#define I51ADEFREE SCI_FREE
#else
#define I51ADEMALLOC  SCI_ALLOC_APP
#define I51ADEFREE SCI_FREE
#endif	

//	最多10个TIMER
#define i51TMMAPMAX 10

//	文件路径长度
#define i51_ADE_OS_FILENAME_LEN 64

static iBOOL i51AdeFileAModeOpen = iFALSE ;
static iU32 i51AdeOsTotallMemory = 0 ;

typedef enum {

	MMINV_I51_SN = ( MMI_MODULE_i51 << 16 ) ,
	MMINV_I51_MAX_ITEM_NUM
	
}i51_NV_ITEM_E ;

typedef struct {

	iU32 CRC ;
	iU32 snvalue1 ;
	iU32 snvalue2 ;
}i51_SN_T ;

const iU16 i51NvLen[] = { sizeof (i51_SN_T) ,} ;
void i51AdeOsSysLog  (iU16 logIndex , ... ) ;

typedef struct 
{

	//	author : Jelo
	//	since : 2011.4.28

	//	notes : TIMER id 映射表( 在某些系统中可以根据情况选择)

	//	系统ID
	iU8 mmkid ;
	iU8 priority ;
	iBOOL loop ;
	iBOOL runing ; 
	//	回调函数
	void (* call)( iU32 id ) ;

} i51TMMAP ;

static iU32 i51PhoneDevice[5]={0};
extern iS32 i51PortWindowHandle ;

//	Log 标志
static iU8 i51AdeOsLogEnable = 0 ;
static i51TMMAP i51tmmap [ i51TMMAPMAX ] = { 0 } ;

#ifdef i51PROFILINGS
//	当前打开的文件总数
static iU32 TotallFileOpened = 0 ;
#endif

static i51ADECALLBACK i51AdeCallback = 0 ;
static iU32 i51AdeCreateTimer = 0 ;
static iU32 i51OsShockTimerID = 0;
static iBOOL i51VibratorIsOpen = 0;

//	i51 正在打电话，2为i51正在打电话， 同时手机也开始播电话
uint8 i51CallStartFlag = 0;

typedef struct CALLINFO{
MN_DUAL_SYS_E dual_sys_i51;
uint8 tele_num_i51[32];
uint8 tele_len_i51;
MMI_STRING_T name_ptr_i51;
MMI_STRING_T disp ;
CC_CALL_SIM_TYPE_E call_sim_type_i51;
uint32 call_type_i51;
MN_SUBADDR_T subaddr ;
}i51CallInfoS;
static i51CallInfoS i51CallInfoStruct = {0};
//	内存泄露检测
# ifdef i51PROFILINGS
typedef struct MMN {

	//	author : Jelo Wang
	//	since : 20110417
	//	(C)TOK
	
	char* file ;
	int line ;
	int length ;
	int address ;
	//	totall spaces used
	int used ;
	
	struct MMN* head ;	
	struct MMN* next ;
	
} MEMORY_MONITOR ;
#define MMT_MASK_LENGTH 4
#define MMT_MASK_APPEND 0xff  
unsigned char* mmt_mask = 0 ;
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 } ;

int MMTInit () {

	//	author : Jelo Wang
	//	since : 20110418 
	//	(C)TOK
	
	mem_monitor.file = 0 ;
	mem_monitor.line = 0 ;
	mem_monitor.length = 0 ;
	mem_monitor.address = 0 ;	
	mem_monitor.head = 0 ;		
	mem_monitor.next = 0 ;

	if ( 0 != mmt_mask )
		I51ADEMALLOC ( mmt_mask ) ;
	mmt_mask = 0 ;
		
	return 1 ;
		
}

void MMTCreateMask ()
{

	//	author : Jelo Wang
	//	since : 20110705
	//	(C)TOK

	int looper = 0 ;
	
	mmt_mask = (unsigned char* ) I51ADEMALLOC ( MMT_MASK_LENGTH ) ;

	for ( looper = 0 ; looper < MMT_MASK_LENGTH ; looper ++ )
	{
		mmt_mask [ looper ] = looper + MMT_MASK_APPEND ;
	}

}

void MMTAdd ( char* file , int line , int length , int address ) {
	
	//	author : Jelo Wang
	//	since : 20110418
	//	(C)TOK

	MEMORY_MONITOR* newmem = 0 ;
		
	newmem	= (MEMORY_MONITOR* ) I51ADEMALLOC ( sizeof(MEMORY_MONITOR) ) ;

	if ( 0 == mmt_mask ) MMTCreateMask () ;
	
	memcpy ( (void*)((unsigned int)address+length) , mmt_mask , MMT_MASK_LENGTH ) ;

	if ( file ) {
		newmem->file = (char* ) I51ADEMALLOC ( strlen ( file ) + 1 ) ;
		strcpy ( newmem->file , file ) ;
	}

	newmem->line = line ; 
	newmem->length = length ;
	newmem->address = address ;
	newmem->head = 0 ;
	newmem->next = 0 ;

	mem_monitor.used = mem_monitor.used + length ;
	
	if ( 0 == mem_monitor.head ) {
		mem_monitor.head = newmem ;
		mem_monitor.next = newmem ;
	} else {
		mem_monitor.next->next = newmem ;
		mem_monitor.next = newmem ;
	}

}

void MMTCheckOverflow () {

	//	author : Jelo
	//	since : 2011.4.10  
	//	(C)TOK
	
	//	notes : 识别溢出内存块

	MEMORY_MONITOR* looper = 0 ;
	unsigned char* mask = 0 ;

	mask = (unsigned char* ) I51ADEMALLOC ( MMT_MASK_LENGTH ) ;

	for ( looper = mem_monitor.head ; looper ;  ) 
	{

		int counter = 0 ;
		
		memcpy ( mask , (void*)((unsigned int )looper->address+looper->length) , MMT_MASK_LENGTH ) ;
		
		for ( counter = 0 ; counter < MMT_MASK_LENGTH ; counter ++ )
		{

			if ( mask [ counter ] != mmt_mask [ counter ] )
			{				
                i51AdeOsLog(0, "OS : MMTCheckOverflow , !!!!! , M : %x , In : '%s' , At line : '%d' - overflowed" , looper->address , looper->file , looper->line);
				break ;
			}
		}			   

		looper=looper->next ;

	}

	I51ADEFREE ( mask ) ;
	
}


void MMTDestroy () {
	
	//	author : Jelo Wang
	//	since : 20110418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	for ( walker = mem_monitor.head ; walker ; ) {
		mem_monitor.next = walker->next ;
		if ( walker->file ) I51ADEFREE ( walker->file ) ;
		I51ADEFREE ( walker ) ;
		walker = mem_monitor.next ; 
	}
  
	if ( 0 != mmt_mask )
		I51ADEFREE ( mmt_mask ) ;
	mmt_mask = 0 ;
	
}


int MMTCheckLeaked () {

	//	author : Jelo Wang
	//	since : 20091129

	int totall = 0 ;
	int leakedtimes = 0 ;
	
	MEMORY_MONITOR* walker = mem_monitor.head ;

    i51AdeOsLog(0, "OS : MMTCheckLeaked , Let's Check Memory Leaking :)");
	
	for ( ;walker;walker=walker->next ) {

		i51AdeOsLog(0, "OS : MMTCheckLeaked , !!! M : %x , S : %d , In '%s' , At line : '%d' - leaked", \
			walker->address,\
			walker->length,\
			walker->file,\
			walker->line\
		) ;
	
		totall = totall + walker->length ; 

		leakedtimes ++ ;
		
	}

	if ( 0 == totall ) {
		
		i51AdeOsLog(0, "OS : MMTCheckLeaked , No Memory Leaking At All :)" ) ;
		
	} 
	
	MMTDestroy () ;
	MMTInit () ;
	
	return 0 ;

}

void MMTFree ( int address ) {
	
	//	author : Jelo Wang
	//	since : 20110418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor.head ;
	MEMORY_MONITOR* pre = &mem_monitor ;
	
	if ( !address ) return ;

	for ( ; walker ; ) {
		
		if ( address == walker->address ) {
			if ( walker == mem_monitor.head ) {
				mem_monitor.head = walker->next ;
				mem_monitor.used = mem_monitor.used - walker->length ;
				if ( walker->file ) I51ADEFREE ( walker->file ) ;
				I51ADEFREE ( walker ) ;
				return ;
			} else {
				pre->next = walker->next ;
				if ( walker == mem_monitor.next ) {
					mem_monitor.next = pre ;
				}
				mem_monitor.used = mem_monitor.used - walker->length ;
				if ( walker->file ) I51ADEFREE ( walker->file ) ;
				I51ADEFREE ( walker ) ;
				return ;
			}
			
		}

		pre = walker ;
		walker = walker->next ;
			
	}
	
}

#endif

#if 0

static void i51AdeOsFotAtomAdd ( const iU16* path , iU32 handle ) {

	//	author : Jelo 
	//	since : 2012.1.6
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	记录被打开的文件。

	typedef struct {

		iU16 path [64] ;
		iU32 handle ;
		
	} i51AdeOsFotAtom ;

	i51AdeOsFotAtom FotAtom = { 0 } ;
	iFILE file = 0 ;
	iS32 length = 0 ;
	iU32 looper = 0 ;

	//	非法文件句柄
	if ( 0 == handle ) {
		return ;
	}

	//	判断是否已经记录下来了。
	if ( iTRUE == i51AdeOsFileIsOpened ( path ) ) {
		return ;
	}
	
	file = i51AdeOsFileOpen ( L"i51AdeOsFot\\i51AdeOsFot.table" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
	
	if ( 0 == file ) {
		file = i51AdeOsFileOpen ( L"i51AdeOsFot\\i51AdeOsFot.table" , i51_OS_FL_ALWAYS_CREATE | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
		if ( 0 == file ) {
            i51AdeOsLog(0, "OS : i51AdeOsFotAtomAdd , Fatal Error At FotAtomAdd");
			return ;
		}
	}

	i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_TAIL ) ;

	i51AdeStdWStrcpy ( FotAtom.path , path ) ;
	FotAtom.handle = handle ;
	i51AdeOsFileWrite ( file , &FotAtom , sizeof(i51AdeOsFotAtom) , 0 ) ;
	i51AdeOsFileClose ( file ) ;
	
	
}

static void i51AdeOsFotAtomDelete ( iU32 handle ) {

	//	author : Jelo 
	//	since : 2012.1.6
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	删除被打开的文件记录。

	typedef struct {

		iU16 path [64] ;
		iU32 handle ;
		
	} i51AdeOsFotAtom ;

	i51AdeOsFotAtom FotAtom = { 0 } ;
	iFILE file = 0 ;
	iS32 length = 0 ;
	iU32 looper = 0 ;
	
	//	非法文件句柄
	if ( 0 == handle ) {
		return ;
	}

	file = i51AdeOsFileOpen ( L"i51AdeOsFot\\i51AdeOsFot.table" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
	
	if ( 0 == file ) {
		return ;
	}

	i51AdeOsFileGetSize ( file , &length ) ;
	length = length / sizeof(i51AdeOsFotAtom) ;
	
	for ( looper = 0 ; looper < length ; looper ++ ) {
		
		i51AdeOsFileRead ( file , &FotAtom , sizeof(i51AdeOsFotAtom) , 0 ) ;
		
		if ( FotAtom.handle == handle ) {
			
			memset ( FotAtom.path , 0 , sizeof(FotAtom.path) ) ;
			FotAtom.handle = 0 ;

			i51AdeOsFileSetSeek ( file , -(sizeof(i51AdeOsFotAtom)) , i51_OS_FL_SEEK_CURRENT ) ;
			
			i51AdeOsFileWrite ( file , &FotAtom , sizeof(i51AdeOsFotAtom) , 0 ) ;
			i51AdeOsFileClose ( file ) ;
			
			return ;
			
		}
		
	}

	i51AdeOsFileClose ( file ) ;	

	return ;
	
}

static iBOOL i51AdeOsFileIsOpened ( const iU16* path ) {

	//	author : Jelo 
	//	since : 2012.1.6
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	判断一个文件是否已经被打开
	
	typedef struct {

		iU16 path [64] ;
		iU32 handle ;
		
	} i51AdeOsFotAtom ;

	i51AdeOsFotAtom FotAtom = { 0 } ;
	iFILE file = 0 ;
	iS32 length = 0 ;
	iU32 looper = 0 ;

	file = i51AdeOsFileOpen ( L"i51AdeOsFot\\i51AdeOsFot.table" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ; 
	
	if ( 0 == file ) {
		return iFALSE ;
	}

	i51AdeOsFileGetSize ( file , &length ) ;
	length = length / sizeof(i51AdeOsFotAtom) ;
	
	for ( looper = 0 ; looper < length ; looper ++ ) {
		
		i51AdeOsFileRead ( file , &FotAtom , sizeof(i51AdeOsFotAtom) , 0 ) ;
		
		if ( 0 == i51AdeStdWStrcmp ( path , FotAtom.path ) ) {
			
			i51AdeStdUnicode2Ascii ( path , (iCHAR* )FotAtom.path ) ;
            i51AdeOsLog(0, "OS : i51AdeOsFileIsOpened , '%s' is Already Opened" , FotAtom.path);
			
			i51AdeOsFileClose ( file ) ;	
			
			return iTRUE ;
			
		}
		
	}

	i51AdeOsFileClose ( file ) ;
		
	return iFALSE ;
	
}

#endif

iU32 i51AdeOsGetMemorySpaceLeft () ; 
static void i51OsShockCallback(iU32 ID)
{
       //	author : steven
	//	since : 2011.12.31 
	/*
	if(iTRUE==i51VibratorIsOpen)
       {   
           GPIO_SetVibrator(0);
       }
       */
       i51AdeOsLog(0, "OS : i51OsShockCallback , Vibrator stop , E");
       GPIO_SetVibrator (0) ; 
       i51AdeOsTimerStop (i51OsShockTimerID) ;
       i51OsShockTimerID = 0;
}

static iBOOL i51AdeOsFileFindDeleteStar ( iU16* string ) {

	//	author : Jelo 
	//	since : 2011.12.23
	//	(C) PKIG Tech. Co., Ltd.

	//	删除通配符*

	iU32 strlen = 0 ;
	iU32 looper = 0 ;
	
	strlen = i51AdeStdWStrlen ( string ) ;

	for ( looper = 0 ; looper < strlen ; looper ++ ) {

		if ( '*' == string [looper] ) {
			string [looper] = 0 ;
			return iTRUE ;
		}
	
	}

	return iFALSE ;
	
}

static void i51AdeOsFileFindDeleteSlash ( iU16* string ) {

	//	author : Jelo 
	//	since : 2011.12.23
	//	(C) PKIG Tech. Co., Ltd.

	//	删除\

	iU32 strlen = 0 ;
	iU32 looper = 0 ;
	iU8* buffer = (iU8* ) string ;
	
	strlen = i51AdeStdWStrlen ( string ) << 1 ;

	for ( looper = strlen-1 ; looper >= 0 ; looper -- ) {

		if ( '\\' == buffer [looper] ) {
			buffer [looper] = 0 ;
			return ;
		}
	
	}

	return ;
	
}


static void  i51AdeOsGetFilePath ( iU16 *FilePath , const iU16 *path )
{

	//	author : william
	//	since : 20110829
	//	PKIG Tech. Co., Ltd.
	
	iU16 *GetCurPath  = iNULL ;
	
	if ( i51AdeFileAModeOpen ) {
		i51AdeStdWStrcpy ( FilePath , path ) ;
	} else {
	
		//相对路径的处理 
	  	GetCurPath =  i51KernelGetApplicationPath () ;
		i51AdeStdWStrcpy ((iU16*)FilePath , (iCHAR*)GetCurPath ) ;
	 	i51AdeStdWStrcat ( (iU16*)FilePath , (iCHAR*)path ) ;
		
	}
	
}

static iU32 i51AdeOsFileAttrSysToi51 ( iU32 SysAttr )
{

	//	author : William
	//	since : 2012.4.23
	//	将系统的文件属性，装换成i51文件属性

	iU32 attr = 0 ;
	if ( SFS_ATTR_RO == (SysAttr & SFS_ATTR_RO) )
		attr |= i51_OS_FL_ATTR_ROL ;
    	else 
		attr |= i51_OS_FL_ATTR_WRT ;

	if ( SFS_ATTR_HIDDEN == (SysAttr & SFS_ATTR_HIDDEN) )
		attr |= i51_OS_FL_ATTR_HID ;
    
	if ( SFS_ATTR_DIR == (SysAttr & SFS_ATTR_DIR) )
		attr |= i51_OS_FL_ATTR_DIR ;

	return attr ;
}



iS32 i51AdeOsTimerMapActive ( iU8 mmkid )
{

	//	author : Jelo
	//	since : 2011.4.20

	//	notes : 激活i51TMMAP 内部的timer 状态，根据不同状态做出处理
	//	若激活某timer 则返回1，否则返回0
	//	该接口不对动态层开放	

	//	该接口不是i51-Adapter标准接口，它是一个全局函数只是为了代码复用。
	
	iS32 looper = 0 ;
	iS32 index = -1 ;

	for ( looper = 0 ; looper < i51TMMAPMAX ; looper ++ )
	{	
		if ( 0 != i51tmmap[looper].call && mmkid== i51tmmap[looper].mmkid )
		{
			i51tmmap[looper].call ( looper+1 ) ; 

			/*
			//	非循环定时器。
			if ( iFALSE == i51tmmap[looper].loop ) {
				i51AdeOsTimerStop ( looper + 1 ) ;				
			} */
			return 1 ;
		}
		
	}

	return 0 ;

}

static void i51AdeOsTimerCallBack ( iU8 timerid , iU32 parameter ) 
{

	i51AdeOsTimerMapActive ( timerid ) ;
	
}

extern iU16* i51PortGetLogPath ( ) ;
extern iBOOL i51PortLogFileIsExist () ;
void i51AdeOsLog ( iS32 module , const iCHAR* log , ... ) 
{	


	static iCHAR buffer [ 256 ] = { 0 } ;		
	iS32 setoff = 0 ;
	iS32 size = 0 ;
	va_list ap ;   
	iFILE i51LogFile = 0 ;
	iU16 *path = NULL ;

	memset ( buffer , 0 , sizeof(buffer) ) ;
//	*(log+255) = '\0' ;
	#ifdef i51SIMULATOR
	{

		FILE *file = 0 ;
		char logpath[64] = {0} ;
		
		strcpy ( logpath , "i51\\" ) ;
		strcat ( logpath , "i51.log" ) ;
		file = fopen(logpath , "a") ;
		
		if ( file > 0 ) {

			iCHAR buffer1 [ 256 ] = { 0 } ;	
			strncpy ( buffer1 , log , 255 ) ;
			va_start ( ap , buffer1 ) ;   
			vsprintf ( buffer , buffer1 , ap ) ;
			va_end ( ap ) ;
			buffer[255 ] = 0 ;

			SCI_TRACE_LOW ( buffer ) ;

			fwrite(buffer , strlen(buffer) , 1 , file) ;
			fwrite("\r\n" , strlen("\r\n") , 1 , file) ;
			fclose(file) ;
		}

	} 
	#else

	strcpy (buffer , "I51PK:") ;
	va_start ( ap , log ) ;   
	vsnprintf ( buffer + strlen(buffer) ,254-strlen(buffer), log , ap ) ;
	va_end ( ap ) ;	    
	buffer[255] = 0 ;
	SCI_TRACE_LOW ( buffer ) ;

	if ( !i51PortLogFileIsExist () ) {

		return ;
	}
	path = i51PortGetLogPath () ;
	i51LogFile = (iFILE)SFS_CreateFile ( path , SFS_MODE_OPEN_EXISTING | SFS_MODE_WRITE , 0 , 0 ) ;
	
	if ( 0 == i51LogFile ) {  
		return ;
	}

	SFS_SetFilePointer ( (SFS_HANDLE ) i51LogFile , 0 , SFS_SEEK_BEGIN ) ;
	SFS_GetFileSize ( (SFS_HANDLE ) i51LogFile , &size ) ;
	SFS_SetFilePointer ( (SFS_HANDLE ) i51LogFile , size , SFS_SEEK_BEGIN ) ;

	SFS_WriteFile ( (SFS_HANDLE ) i51LogFile , buffer , strlen(buffer) , &size , 0 ) ;
	SFS_WriteFile ( (SFS_HANDLE ) i51LogFile , "\r\n" , strlen("\r\n") , &size , 0 ) ; 
	SFS_FlushFile ( (SFS_HANDLE ) i51LogFile , 0 ) ;
	SFS_CloseFile ( (SFS_HANDLE)i51LogFile ) ;
	
	return ;

#endif


}


void i51AdeOsLogEx ( iS32 module , iU8 *buf , iU32 buflen )
{

	iFILE i51LogFile = 0 ;
	iU32 size ;
	i51LogFile = (iFILE)SFS_CreateFile ( L"E:\\i51\\i51.log" , SFS_MODE_OPEN_EXISTING | SFS_MODE_WRITE , 0 , 0 ) ;
	
	if ( 0 == i51LogFile ) {  
		return ;
	}

	SFS_SetFilePointer ( (SFS_HANDLE ) i51LogFile , 0 , SFS_SEEK_BEGIN ) ;
	SFS_GetFileSize ( (SFS_HANDLE ) i51LogFile , &size ) ;
	SFS_SetFilePointer ( (SFS_HANDLE ) i51LogFile , size , SFS_SEEK_BEGIN ) ;

	SFS_WriteFile ( (SFS_HANDLE ) i51LogFile , buf , buflen , &size , 0 ) ;
	SFS_WriteFile ( (SFS_HANDLE ) i51LogFile , "\r\n" , strlen("\r\n") , &size , 0 ) ; 
	SFS_FlushFile ( (SFS_HANDLE ) i51LogFile , 0 ) ;
	SFS_CloseFile ( (SFS_HANDLE)i51LogFile ) ;

}
extern iBOOL i51PortDatFileIsExist() ;
void i51AdeOsSysLog  (iU16 logIndex , ... ) 
{	
	static iCHAR buffer [ 256 ] = { 0 } ;		
	iS32 setoff = 0 ;
	iS32 size = 0 ;
	va_list ap ;   
	iFILE i51LogFile = 0 ;
	iU16 *path = NULL ;
	iCHAR* log ;
	
	memset ( buffer , 0 , sizeof(buffer) ) ;

	log = (iCHAR*)i51AdeOsSysLogGetString(logIndex);
	if(log == 0) return ;
	
	#ifdef i51SIMULATOR
	{

		FILE *file = 0 ;
		char logpath[64] = {0} ;
		
		strcpy ( logpath , "i51\\" ) ;
		strcat ( logpath , "i51.log" ) ;
		file = fopen(logpath , "a") ;
		
		if ( file > 0 ) {

			iCHAR buffer1 [ 256 ] = { 0 } ;	
			strncpy ( buffer1 , log , 255 ) ;
			va_start ( ap , logIndex ) ;   
			vsprintf ( buffer , buffer1 , ap ) ;
			va_end ( ap ) ;
			buffer[255 ] = 0 ;

			SCI_TRACE_LOW ( buffer ) ;

			fwrite(buffer , strlen(buffer) , 1 , file) ;
			fwrite("\r\n" , strlen("\r\n") , 1 , file) ;
			fclose(file) ;
		}

	} 
	#else

	strcpy (buffer , "I51PK:") ;
	va_start ( ap , logIndex ) ;   
	vsnprintf ( buffer + strlen(buffer) ,254-strlen(buffer), log , ap ) ;
	va_end ( ap ) ;	    
	buffer[255] = 0 ;
	SCI_TRACE_LOW ( buffer ) ;

	if ( !i51PortLogFileIsExist () ) {

		return ;
	}

	path = i51PortGetLogPath () ;
	i51LogFile = (iFILE)SFS_CreateFile ( path , SFS_MODE_OPEN_EXISTING | SFS_MODE_WRITE , 0 , 0 ) ;
	
	if ( 0 == i51LogFile ) {  
		return ;
	}

	SFS_SetFilePointer ( (SFS_HANDLE ) i51LogFile , 0 , SFS_SEEK_BEGIN ) ;
	SFS_GetFileSize ( (SFS_HANDLE ) i51LogFile , &size ) ;
	SFS_SetFilePointer ( (SFS_HANDLE ) i51LogFile , size , SFS_SEEK_BEGIN ) ;

	SFS_WriteFile ( (SFS_HANDLE ) i51LogFile , buffer , strlen(buffer) , &size , 0 ) ;
	SFS_WriteFile ( (SFS_HANDLE ) i51LogFile , "\r\n" , strlen("\r\n") , &size , 0 ) ; 
	SFS_FlushFile ( (SFS_HANDLE ) i51LogFile , 0 ) ;
	SFS_CloseFile ( (SFS_HANDLE)i51LogFile ) ;
	
	return ;

#endif


}
iU32 i51AdeOsGetTotallMemoryLeft ( void ) 
{

	//	author : Jelo
	//	since : 2011.4.10

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsGetTotallMemoryLeft , S");
	#endif

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsGetTotallMemoryLeft , E");
	#endif
	
	return i51AdeOsGetMemorySpaceLeft () ;
	
}

#ifdef i51PROFILINGS
iU64 i51AdeOsGetTotallMemory ( void ) 
{

	//	author : Jelo
	//	since : 2011.4.10

	#ifndef SPRD_6530
	return SCI_GetMaxAppHeapAvalidSpace () ;
	#else
	return (1024*1024) ;
	#endif
	
}

iU64 i51AdeOsGetTotallMemoryUsed ( void ) 
{

	//	author : Jelo
	//	since : 2011.4.10

	return mem_monitor.used ;
	
}
#endif

void i51NvRegister ()
{
	//	注册NV
	MMI_RegModuleNv (MMI_MODULE_i51 , i51NvLen, sizeof(i51NvLen)/sizeof(iU16)) ;
}


//	以下是内存系统
void* i51AdeOsMalloc ( iU32 length , const iCHAR* file , iU32 line ) 
{

	//	author : Jelo
	//	since : 2011.4.10

	void* address = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsMalloc , S , %x , %s , %d" , length , file , line);
	#endif
	
	if ( length > i51AdeOsGetMemorySpaceLeft () ) {
        i51AdeOsLog(0, "OS : i51AdeOsMalloc , Overflowed , %d , %d" ,length ,i51AdeOsGetMemorySpaceLeft ());
		return 0 ;
	}
	 
	#ifdef i51PROFILINGS
	{

		if ( length + MMT_MASK_LENGTH > i51AdeOsGetMemorySpaceLeft () ) {
            i51AdeOsLog(0, "OS : i51AdeOsMalloc , Overflowed , %d , %d" ,length + MMT_MASK_LENGTH ,i51AdeOsGetMemorySpaceLeft ());
			return 0 ;
		}
	 
		address = (void*) I51ADEMALLOC ( length + MMT_MASK_LENGTH ) ;
		memset ( address , 0 , length + MMT_MASK_LENGTH ) ;
		MMTAdd ( file , line , length , (int)address ) ;
		i51MemoryOverflowCheck () ;		
		
	} 
	#else
		address = I51ADEMALLOC ( length ) ;
	#endif

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsMalloc , E , %x" , address);
	#endif
		
	return address ;
		
	
}

void* i51AdeOsMallocEx ( iU32 length ) 
{

	//	author : Jelo
	//	since : 2011.6.2

	void* address = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsMallocEx , S , %x " , length);
	#endif
	

	if ( length > i51AdeOsGetMemorySpaceLeft () ) {
        i51AdeOsLog(0, "OS : i51AdeOsMallocEx , Overflowed , %d , %d" ,length ,i51AdeOsGetMemorySpaceLeft ());
		return 0 ;
	}		
	#ifdef i51PROFILINGS
	{
	//	余外申请MMT_MASK_LENGTH 个字节，用来存放溢出检测mask
	//	尾部放一串mask
		if ( length + MMT_MASK_LENGTH > i51AdeOsGetMemorySpaceLeft () ) {
            i51AdeOsLog(0, "OS : i51AdeOsMallocEx , Overflowed , %d , %d" ,length + MMT_MASK_LENGTH ,i51AdeOsGetMemorySpaceLeft ());
			return 0 ;
		}
	
		address = (void*) I51ADEMALLOC ( length + MMT_MASK_LENGTH ) ;
		memset ( address , 0 , length + MMT_MASK_LENGTH ) ;
		MMTAdd ( __FILE__ , __LINE__ , length , (int)address ) ;
		i51MemoryOverflowCheck () ;
	} 
	#else   
		address = I51ADEMALLOC ( length ) ;
	#endif

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsMallocEx , E , %x" , address);
	#endif
		
	return address ;	
	
}

iBOOL i51AdeOsFreeEx ( void* address ) 
{ 

	//	author : Jelo
	//	since : 2011.4.10

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFreeEx , S , %x " , address);
	#endif
	
	# ifdef i51PROFILINGS
		MMTCheckOverflow () ; 
		MMTFree ( (int)address ) ; 
	# endif

	I51ADEFREE ( address ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFreeEx , E");
	#endif
	
	return 1 ;


}

iBOOL i51AdeOsFree ( void* address ) 
{ 

	//	author : Jelo
	//	since : 2011.4.10

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFree , S , %x " , address);
	#endif
	
	# ifdef i51PROFILINGS
		MMTCheckOverflow () ; 
		MMTFree ( (int)address ) ; 
	# endif

	I51ADEFREE ( address ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFree , E");
	#endif
	
	return 1 ;


}

//	以下是文件系统

void i51AdeOsFileAModeEnable () 
{

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileAModeEnable , S");
	#endif
	
	i51AdeFileAModeOpen = iTRUE ;  

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileAModeEnable , E , Mode %x" , i51AdeFileAModeOpen);
	#endif
	
}

void i51AdeOsFileAModeDisable () 
{

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileAModeDisable , S");
	#endif
	
	i51AdeFileAModeOpen = iFALSE ;  

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileAModeDisable , E , Mode %x" , i51AdeFileAModeOpen);
	#endif
	
}

iFILE i51AdeOsFileOpen ( const iU16* path , iU32 mode , const iCHAR* fl , iS32 line ) 
{

	//	author : Jelo
	//	since : 2011.4.13

	//	notes : 成功返回> 0 ，失败0

	iS32 ret = 0 ;
	iU32 sfsmode = 0 ;
  
	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( path , (iCHAR*)path16 ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileOpen , S , %s , %x , %s , %d" , path16 , mode , fl , line);
	#endif
	
	i51AdeOsGetFilePath ( path16 , path ) ; 
	
	if ( i51_OS_FL_READ == (i51_OS_FL_READ & mode) )
	{
		sfsmode = sfsmode | SFS_MODE_READ ; 
	}
	if ( i51_OS_FL_WRITE == (i51_OS_FL_WRITE & mode) )
	{
		sfsmode = sfsmode | SFS_MODE_READ ;
		sfsmode = sfsmode | SFS_MODE_WRITE ;  
	}
	if ( i51_OS_FL_ALWAYS_CREATE == (i51_OS_FL_ALWAYS_CREATE & mode) )
	{
		sfsmode = sfsmode | SFS_MODE_CREATE_ALWAYS ;
	}
	if ( i51_OS_FL_ALWAYS_OPEN == (i51_OS_FL_ALWAYS_OPEN & mode) )
	{
		sfsmode = sfsmode | SFS_MODE_OPEN_ALWAYS ;
	}
	if ( i51_OS_FL_EXISTING_OPEN == (i51_OS_FL_EXISTING_OPEN & mode) )
	{
		sfsmode = sfsmode | SFS_MODE_OPEN_EXISTING ;
	}

	ret = (iS32 )SFS_CreateFile ( path16 , sfsmode , 0 , 0 ) ;		
		
	#ifdef i51PROFILINGS
		//	当前打开的文件总数
		if ( 0 < ret ) TotallFileOpened ++ ;
	#endif

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsFileOpen , E , ret %x" , ret);
	#endif
	
	return (iFILE) ret ;
	
}

iBOOL i51AdeOsFileClose ( iFILE file )
{

	//	author : Jelo
	//	since : 2011.4.13

	//	notes : 成功返回>= 0 ，失败0
	
	int ret = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileClose , S , %x" , file);
	#endif
	
	ret = SFS_CloseFile ( (SFS_HANDLE)file ) ;

	#ifdef i51PROFILINGS
		//	当前打开的文件总数
		if ( SFS_NO_ERROR == ret && 0 < TotallFileOpened ) TotallFileOpened -- ;
	#endif	

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileClose , E , ret %x" , ret);
	#endif
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileRead ( iFILE file , void* DataPtr , iU32 Length , iU32* Read )
{

	//	author : Jelo
	//	since : 2011.4.13

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU32 rad = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileRead , S , %x , %x , %x , %x" , file , DataPtr , Length , Read);
	#endif
	
	ret = SFS_ReadFile ( (SFS_HANDLE ) file , DataPtr , Length ,  &rad , 0 ) ;

	if ( Read ) *Read = rad ;

	{
		if ( rad != Length || ret != SFS_NO_ERROR ) {
			i51AdeOsLog ( i51_LOG_SC , "ADE -OS : i51AdeOsFileRead R F !!!") ;
		}
	}
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileRead , E , ret %x" , ret);
	#endif

	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileReadInt8 ( iFILE file , iU8* integer8 )
{

	//	author : Jelo
	//	since : 2011.6.28

	//	notes : 读取8 位整数
	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU32 rad = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileReadInt8 , S , %x , %x" , file , integer8);
	#endif
	
	ret = SFS_ReadFile ( (SFS_HANDLE ) file , integer8 , sizeof(iU8) ,  &rad , 0 ) ;
	{
		if ( rad != 1 || ret != SFS_NO_ERROR ) {
            i51AdeOsLog(0, "OS : i51AdeOsFileReadInt8 , R F !!!");
		}
	}

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileReadInt8 , E , ret %x" , ret);
	#endif
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileReadInt16 ( iFILE file , iU16* integer16 )
{

	//	author : Jelo
	//	since : 2011.6.28

	//	notes : 读取16 位整数
	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU32 rad = 0 ;
	iU16 value = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileReadInt16 , S , %x , %x" , file , integer16);
	#endif
	
	ret = SFS_ReadFile ( (SFS_HANDLE ) file , integer16 , sizeof(iU16) ,  &rad , 0 ) ;

	{
		if ( rad != 2 || ret != SFS_NO_ERROR ) {
            i51AdeOsLog(0, "OS : i51AdeOsFileReadInt16 , R F !!!");
		}
	}
#ifndef i51SIMULATOR

	#ifndef SPRD_6530
	value = (*integer16 & 0xff00) >> 8 ;
	
	*integer16 = (*integer16 & 0xff) << 8 ;
	*integer16 = *integer16 | value ;
	#endif

#endif	

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileReadInt16 , E , value %x , new value %x" , value , *integer16);
	#endif
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileReadInt32 ( iFILE file , iU32* integer32 )
{

	//	author : Jelo
	//	since : 2011.6.28

	//	notes : 读取32 位整数
	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU32 rad = 0 ;

	iU32 _1h8bit = 0 ;
	iU32 _1l8bit = 0 ;
	iU32 _2h8bit = 0 ;
	iU32 _2l8bit = 0 ;

	#ifdef i51ADAPTER_DEBUG
		iU32 value = 0 ;
	#endif

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileReadInt32 , S , %x , %x" , file , integer32);
	#endif
	
	ret = SFS_ReadFile ( (SFS_HANDLE ) file , integer32 , sizeof(iU32) ,  &rad , 0 ) ;

	{
		if ( rad != 4 || ret != SFS_NO_ERROR ) {
            i51AdeOsLog(0, "OS : i51AdeOsFileReadInt32 , R F !!!");
		}
	}
	#ifdef i51ADAPTER_DEBUG
		value = integer32 ;
	#endif	
	
#ifndef i51SIMULATOR

	#ifndef SPRD_6530
	_1h8bit = (*integer32 & 0xff00) >> 8 ;
	_1l8bit = (*integer32 & 0xff) ;

	_2h8bit = (*integer32 & 0xff000000) >> 24 ;
	_2l8bit = (*integer32 & 0xff0000) >> 16 ;
	
	*integer32 = 0 ;

	*integer32 = *integer32 | (_1h8bit << 16);
	*integer32 = *integer32 | (_1l8bit << 24) ;

	*integer32 = *integer32 | (_2l8bit << 8) ;
	*integer32 = *integer32 | _2h8bit ;
	#endif
#endif

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileReadInt32 , E , value %x , new value %x" , value , *integer32);
	#endif

	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileWrite ( iFILE file , void* DataPtr , iU32 Length , iU32* Written )
{

	//	author : Jelo
	//	since : 2011.4.13

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU32 wr = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileWrite , S , %x , %x , %x , %x" , file , DataPtr , Length , Written);
	#endif
	
	ret = SFS_WriteFile ( (SFS_HANDLE ) file , DataPtr , Length ,  &wr , 0 ) ;

	if ( Written ) *Written = wr ;

	{
		if ( wr != Length || ret != SFS_NO_ERROR ) {
            i51AdeOsLog(0, "OS : i51AdeOsFileWrite , W F !!!");
		}
	}
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileWrite , E , ret %x" , ret);
	#endif
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileWriteInt8 ( iFILE file , iU8* integer8 )
{

	//	author : Jelo
	//	since : 2011.6.28

	//	notes : 写8 位整数
	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU32 wr = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt8 , S , %x , %x" , file , integer8);
	#endif
	
	ret = SFS_WriteFile ( (SFS_HANDLE ) file , integer8 , sizeof(iU8) ,  &wr , 0 ) ;

	{
		if ( wr != 1 || ret != SFS_NO_ERROR ) {
            i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt8 , bad ass !!!");
		}
	}
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt8 , E , ret %x" , ret);
	#endif
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileWriteInt16 ( iFILE file , iU16* integer16 )
{

	//	author : Jelo
	//	since : 2011.6.28

	//	notes : 写16 位整数
	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU32 wr = 0 ;
	iU16 value = 0 ;
	iU16 results = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt16 , S , %x , %x" , file , integer16);
	#endif

	results = *integer16 ;
	
#ifndef i51SIMULATOR

#ifndef SPRD_6530
	value = (results & 0xff00) >> 8 ;
	results = (results & 0xff) << 8 ;
	results = results | value ; 
#else
	results = *integer16 ;
#endif

#endif
	
	ret = SFS_WriteFile ( (SFS_HANDLE ) file , &results , sizeof(iU16) ,  &wr , 0 ) ;

	{
		if ( wr != 2 || ret != SFS_NO_ERROR ) {
            i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt16 , W F !!!");
		}
	}
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt16 , E , value %x , new value %x" , value , results);
	#endif
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileWriteInt32 ( iFILE file , iU32* integer32 )
{

	//	author : Jelo
	//	since : 2011.6.28

	//	notes : 读取32 位整数
	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU32 wr = 0 ;

	iU32 _1h8bit = 0 ;
	iU32 _1l8bit = 0 ;
	iU32 _2h8bit = 0 ;
	iU32 _2l8bit = 0 ;
	iU32 results = 0 ;

	#ifdef i51ADAPTER_DEBUG
		iU32 value = 0 ;
	#endif

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt32 , S , %x , %x" , file , integer32 ) ;
	#endif
	
#ifndef i51SIMULATOR

#ifndef SPRD_6530
	_1h8bit = (*integer32 & 0xff00) >> 8 ;
	_1l8bit = (*integer32 & 0xff) ;

	_2h8bit = (*integer32 & 0xff000000) >> 24 ;
	_2l8bit = (*integer32 & 0xff0000) >> 16 ;
	
	results = results | (_1h8bit << 16);
	results = results | (_1l8bit << 24) ;

	results = results | (_2l8bit << 8) ;
	results = results | _2h8bit ;

#else
	results = *integer32 ;
#endif
#endif

	ret = SFS_WriteFile ( (SFS_HANDLE ) file , &results , sizeof(iU32) ,  &wr , 0 ) ;

	{
		if ( wr != 4 || ret != SFS_NO_ERROR ) {
            i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt32 , W F !!!") ;
		}
	}
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileWriteInt32 , E , value %x , new value %x" , value , results ) ;
	#endif
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileSetSeek ( iFILE file , iS32 offset , iS32 seek )
{

	//	author : Jelo
	//	since : 2011.4.13

	//	notes : 成功返回> 0 ，失败0

	iS32 ret = -1 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileSetSeek , S , %x , %x , %x" , file , offset , seek ) ;
	#endif
	
	switch ( seek ) 
	{
		case i51_OS_FL_SEEK_HEAD :
			ret = SFS_SetFilePointer ( (SFS_HANDLE ) file , offset , SFS_SEEK_BEGIN ) ;
		break ;
		case i51_OS_FL_SEEK_CURRENT :
		{
			iU32 position = 0 ;
			i51AdeOsFileGetSeek ( file , &position , i51_OS_FL_SEEK_HEAD ) ;
			position = position + offset ;
			ret = SFS_SetFilePointer ( (SFS_HANDLE ) file , position , SFS_SEEK_BEGIN ) ;
		}
		break ;
		case i51_OS_FL_SEEK_TAIL :
		{
			iU32 length = 0 ;
			i51AdeOsFileGetSize ( file , &length ) ;
			//	the offset is a Negative number
			length = length + offset ;
			ret = SFS_SetFilePointer ( (SFS_HANDLE ) file , length , SFS_SEEK_BEGIN ) ;
		}
		break ;	
		default :
			ret = -1 ;	
	}

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileSetSeek , E , ret %x" , ret ) ;
	#endif
	
	return ( SFS_NO_ERROR == ret ) ;
	

}

iBOOL i51AdeOsFileGetSize ( iFILE file , iS32* size )
{

	//	author : Jelo
	//	since : 2011.4.13

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileGetSize , S , %x , %x" , file , size ) ;
	#endif
	
	SFS_SetFilePointer ( (SFS_HANDLE ) file , 0 , SFS_SEEK_BEGIN ) ;
	ret = SFS_GetFileSize ( (SFS_HANDLE ) file , size ) ;
	SFS_SetFilePointer ( (SFS_HANDLE ) file , 0 , SFS_SEEK_BEGIN ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileGetSize , E , ret %x" , ret ) ;
	#endif	

	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileCreateDir ( const iU16* path )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;

	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( path , (iCHAR* )path16 ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileCreateDir , S , %s" , path16 ) ;
	#endif
	
	i51AdeOsGetFilePath ( path16 , path ) ; 

	ret = SFS_CreateDirectory ( path16 ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileCreateDir , E , ret %x" , ret ) ;
	#endif	
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileDeleteDir ( const iU16* path )
{

#if 1
	iBOOL ret = iTRUE ;
	iFILE filehandle = 0 ;
	i51FILEINFO fileinfo = {0} ;
//	SFS_FIND_DATA_T attribute = { 0 } ;
 
	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( path , (iCHAR* )path16 ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , S , %s" , path16 ) ;
	#endif

	if ( !i51AdeOsFileIsDirExist(path) ) {
        i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , don't dir exist , F" ) ;
		return iFALSE ;
	}
	
	i51AdeOsFileSetAttr ( path , i51_OS_FL_ATTR_WRT ) ;
	i51AdeStdWStrcpy ( path16 , path ) ;
	i51AdeStdWStrcat ( path16 , L"\\*" ) ;

	filehandle = i51AdeOsFileFind ( path16 ,   &fileinfo) ;
    i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , %d" , filehandle ) ;
	if ( 0 == filehandle ) 
	{

        i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , the dir empty" ) ;
		memset ( path16 , 0 , sizeof(path16) ) ;
		i51AdeOsGetFilePath ( path16 , path ) ;
		if ( SFS_NO_ERROR == SFS_DeleteDirectory ( path16 ) ) {
            i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , delete Sussess" ) ;
			return iTRUE ;
		}
		else {
            i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , delete Fail" ) ;
			return iFALSE ;
		}
	}

	while ( filehandle > 0 && ret )
	{

		iU16* newpath = 0 ; 
		
		newpath = (iU16* ) i51MALLOC ( 
			(i51AdeStdWStrlen (path16) << 1) + ( i51AdeStdWStrlen (fileinfo.name) << 1 ) + 4  ) ;
		 
		if ( i51_OS_FL_ATTR_DIR == (fileinfo.attribute & i51_OS_FL_ATTR_DIR) )
		{	
			
			if ( 0 != newpath )
			{
                i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , find dir" ) ;
				i51AdeStdWStrcpy ( newpath , path ) ;
				newpath = i51AdeStdWStrcat ( newpath , L"\\" ) ; 				
				newpath = i51AdeStdWStrcat ( newpath , fileinfo.name ) ;	
				i51AdeOsFileSetAttr ( newpath , i51_OS_FL_ATTR_WRT ) ;
				i51AdeOsFileDeleteDir ( newpath ) ;
				i51FREE ( newpath ) ;
			}   
		}
		else
		{
						
			if ( 0 != newpath )
			{ 
                i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , find file" ) ;
				i51AdeStdWStrcpy ( newpath , path ) ;
				newpath = i51AdeStdWStrcat ( newpath , L"\\" ) ;
				newpath = i51AdeStdWStrcat ( newpath , fileinfo.name ) ;	
				i51AdeOsFileSetAttr ( newpath , i51_OS_FL_ATTR_WRT ) ;	
				i51AdeOsFileDelete ( newpath) ;
				i51FREE ( newpath ) ;
			}
			
		}
	
		memset(&fileinfo, 0, sizeof(fileinfo));
		ret = i51AdeOsFileFindNext ( filehandle , &fileinfo ) ;
		
	} 
	
	i51AdeOsFileFindClose ( filehandle ) ;
	ret = i51AdeOsFileDeleteDir ( path ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , E , ret %x" , ret ) ;
	#endif	
	
	return iTRUE ;
	
#else
	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = SFS_NO_ERROR ;
	iFILE filehandle = 0 ;
	SFS_FIND_DATA_T attribute = { 0 } ;
 
	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( path , (iCHAR* )path16 ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , S , %s" , path16 ) ;
	#endif
	
	i51AdeOsGetFilePath ( path16 , path ) ; 
	
	i51AdeOsFileSetAttr ( path , i51_OS_FL_ATTR_WRT ) ;
	
	ret = SFS_FindFirstFileCtrl ( path16 , L"*" , L"/A:+NF" , &filehandle , &attribute , 0 ) ;
	
	if ( 0 == filehandle || SFS_NO_ERROR != ret ) 
	{
		SFS_DeleteDirectory ( path16 ) ;
		return iTRUE ;
	}

	while ( SFS_NO_ERROR == ret )
	{

		iU16* newpath = 0 ; 
		
		newpath = (iU16* ) i51MALLOC ( 
			(MMIAPICOM_Wstrlen (path16) << 1) + ( MMIAPICOM_Wstrlen (attribute.name) << 1 ) + 4  ) ;
		 
		if ( SFS_ATTR_DIR == (attribute.attr & SFS_ATTR_DIR) )
		{	
			
			if ( 0 != newpath )
			{

				MMIAPICOM_Wstrcpy ( newpath , path ) ;
				newpath = MMIAPICOM_Wstrcat ( newpath , L"\\" ) ; 				
				newpath = MMIAPICOM_Wstrcat ( newpath , attribute.name ) ;	
				i51AdeOsFileSetAttr ( newpath , i51_OS_FL_ATTR_WRT ) ;
				i51AdeOsFileDeleteDir ( newpath ) ;
				i51FREE ( newpath ) ;
			}   
		}
		else
		{
						
			if ( 0 != newpath )
			{ 
			
				MMIAPICOM_Wstrcpy ( newpath , path ) ;
				newpath = MMIAPICOM_Wstrcat ( newpath , L"\\" ) ;
				newpath = MMIAPICOM_Wstrcat ( newpath , attribute.name ) ;	
				i51AdeOsFileSetAttr ( newpath , i51_OS_FL_ATTR_WRT ) ;	
				SFS_DeleteFile ( newpath , 0 ) ;
				i51FREE ( newpath ) ;
			}
			
		}
	
		SCI_MEMSET(&attribute, 0, sizeof(SFS_FIND_DATA_T));
		ret = SFS_FindNextFile ( filehandle , &attribute ) ;
		
	} 
	
	SFS_FindClose ( filehandle ) ;
	ret = SFS_DeleteDirectory ( path16 ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileDeleteDir , E , ret %x" , ret ) ;
	#endif	
	
	return ( SFS_NO_ERROR == ret ) ;
	#endif
	
}

iFILE i51AdeOsFileFind ( const iU16* pathex , i51FILEINFO* info )
{

	//	author : Jelo 
	//	since : 2011.8.18
	//	(C) PKIG Tech. Co., Ltd.

	iFILE file = 0 ;	
	SFS_FIND_DATA_T attribute = { 0 } ;

	iU16 path [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;
	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

//	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( pathex , (iCHAR* )path16 ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileFind , S , %s , %x" , path16 , info ) ;
//	#endif
	
	i51AdeStdWStrcpy ( path , pathex ) ;
	i51AdeOsFileFindDeleteStar ( path ) ;
	i51AdeOsGetFilePath ( path16 , path ) ; 
	i51AdeOsFileFindDeleteSlash ( path16 ) ; 
	
	if ( 0 < i51AdeStdWStrlen ( path ) ) {
		
		i51AdeOsFileSetAttr ( path , i51_OS_FL_ATTR_WRT ) ;

	}
	
	//	E:\\i51\\i51KIT*
	SFS_FindFirstFileCtrl ( path16 , L"*" , L"/A:+NF" , &file , &attribute , 0 ) ;	

	//	OK
	//SFS_FindFirstFileCtrl ( L"E:\\i51\\i51KIT" , L"*" , L"/A:+NF" , &file , &attribute , 0 ) ;
	//	FAILED
	//SFS_FindFirstFileCtrl ( L"E:\\i51\\i51KIT\\" , L"*" , L"/A:+NF" , &file , &attribute , 0 ) ;

    i51AdeOsLog(0, "OS : i51AdeOsFileFind , %d" , file ) ;
	if ( !i51AdeStdWStrlen (attribute.name)) {
        i51AdeOsLog(0, "OS : i51AdeOsFileFind , Not Find , F" ) ;
		return 0 ;
	}
	if ( 0 < file )
	{
		MMIAPICOM_Wstrcpy ( info->name , attribute.name ) ;
              info->CreateTime.Year = attribute.create_Date.year;
              info->CreateTime.Month = attribute.create_Date.mon;
              info->CreateTime.Day = attribute.create_Date.mday;
              info->CreateTime.Hour = attribute.create_time.hour;
              info->CreateTime.Minute = attribute.create_time.min;
              info->CreateTime.Second = attribute.create_time.sec;

              info->EditedTime.Year = attribute.modify_Date.year;
              info->EditedTime.Month = attribute.modify_Date.mon;
              info->EditedTime.Day = attribute.modify_Date.mday;
              info->EditedTime.Hour = attribute.modify_time.hour;
              info->EditedTime.Minute = attribute.modify_time.min;
              info->EditedTime.Second = attribute.modify_time.sec;

		info->attribute = i51AdeOsFileAttrSysToi51 (attribute.attr ) ;
	}

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsFileFind , E , %x , %x" , file , info->attribute ) ;
	#endif
	
	return file ;
	
}

iBOOL i51AdeOsFileFindNext ( iFILE file , i51FILEINFO* info ) 
{

	//	author : Jelo 
	//	since : 2011.8.18
	//	(C) PKIG Tech. Co., Ltd.
	
	iS32 ret =0 ;
	
	SFS_FIND_DATA_T attribute = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileFindNext , S , %x , %x" , file , info ) ;
	#endif
	
	ret = SFS_FindNextFile ( file , &attribute ) ;

	if ( 0 == ret )
	{
		MMIAPICOM_Wstrcpy ( info->name , attribute.name ) ;
              info->CreateTime.Year = attribute.create_Date.year;
              info->CreateTime.Month = attribute.create_Date.mon;
              info->CreateTime.Day = attribute.create_Date.mday;
              info->CreateTime.Hour = attribute.create_time.hour;
              info->CreateTime.Minute = attribute.create_time.min;
              info->CreateTime.Second = attribute.create_time.sec;

              info->EditedTime.Year = attribute.modify_Date.year;
              info->EditedTime.Month = attribute.modify_Date.mon;
              info->EditedTime.Day = attribute.modify_Date.mday;
              info->EditedTime.Hour = attribute.modify_time.hour;
              info->EditedTime.Minute = attribute.modify_time.min;
              info->EditedTime.Second = attribute.modify_time.sec;

		info->attribute = i51AdeOsFileAttrSysToi51 (attribute.attr ) ;

	}

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileFindNext , E , %x , %x" , ret , info->attribute ) ;
	#endif
	
	return ( SFS_ERROR_NONE == ret ) ;
		
}

iBOOL i51AdeOsFileFindClose ( iFILE file )
{

	//	author : Jelo 
	//	since : 2011.8.18
	//	(C) PKIG Tech. Co., Ltd.

	iS32 ret =0 ;
	
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileFindClose , S , %x" , file ) ;
	#endif
	
	ret = SFS_FindClose ( file ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileFindClose , E , ret %x" , ret ) ;
	#endif
	
	return ( SFS_ERROR_NONE == ret ) ;
	
	
}

iBOOL i51AdeOsFileGetSeek ( iFILE file , iS32* seek , iS32 se )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileGetSeek , S , %x" , file ) ;
	#endif
	
	switch ( se )
	{
		case i51_OS_FL_SEEK_HEAD :
			ret = SFS_GetFilePointer ( file , SFS_SEEK_BEGIN , seek ) ;
		break ;
		case i51_OS_FL_SEEK_CURRENT :
			ret = SFS_NO_ERROR ;
			*seek = 0 ;
		//	ret = SFS_GetFilePointer ( file , SFS_SEEK_CUR , seek ) ;
		break ;
		case i51_OS_FL_SEEK_TAIL :
			ret = SFS_GetFilePointer ( file , SFS_SEEK_END , seek ) ;
		break ;

	}

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileGetSeek , E , ret %x" , ret ) ;
	#endif	

	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileDelete ( const iU16* path )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( path , (iCHAR* )path16 ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileDelete , S , %s" , path16 ) ;
	#endif
	
	i51AdeOsGetFilePath ( path16 , path ) ; 
	
	ret = SFS_DeleteFile ( path16 , 0 ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileDelete , E , ret %x" , ret ) ;
	#endif	
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iU32 i51AdeOsFileGetAttr ( const iU16* path )
{

	//	author : Jelo
	//	since : 2011.4.16

	iS32 ret = -1 ;
	iFILE file = 0 ;
	iU32 attr = 0 ;
	SFS_FIND_DATA_T attribute = { 0 } ;

	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( path , (iCHAR* )path16 ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileGetAttr , S , %s" , path16 ) ;
	#endif
	
	i51AdeOsGetFilePath ( path16 , path ) ; 
	
	file = SFS_FindFirstFile ( path16 , &attribute ) ;

	if ( !file ) 
	{
		return i51_OS_FL_ATTR_ERR ;
	}

	attr = i51AdeOsFileAttrSysToi51 ( attribute.attr ) ;

	SFS_FindClose ( file ) ;
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileGetAttr , E , attr %x" , attr ) ;
	#endif	
	
	return attr ;
	
}

iBOOL i51AdeOsFileSetAttr ( const iU16* path , iU32 attr )
{

 	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;
	iS32 newattr = 0 ;
	iFILE file = 0 ;

	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( path , (iCHAR* )path16 ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileSetAttr , S , %s , %x" , path16 , attr ) ;
	#endif
	
	i51AdeOsGetFilePath ( path16 , path ) ; 	


	if ( i51_OS_FL_ATTR_ROL == (attr & i51_OS_FL_ATTR_ROL) )
		newattr = newattr | SFS_ATTR_RO ;
	if ( i51_OS_FL_ATTR_HID == (attr & i51_OS_FL_ATTR_HID) ) {
		
		newattr = newattr | SFS_ATTR_HIDDEN ;
		
	}
//	if ( i51_OS_FL_ATTR_DIR == (attr & i51_OS_FL_ATTR_DIR) ) 
//		newattr = newattr | SFS_ATTR_DIR ;
//	if ( i51_OS_FL_ATTR_WRT == (attr & i51_OS_FL_ATTR_WRT) ) newattr = newattr & 0xfffffffE ;
//	if ( i51_OS_FL_ATTR_WRT == (attr & i51_OS_FL_ATTR_WRT) ) 
//		newattr = newattr & SFS_ATTR_ARCH ;
	

	ret = SFS_SetAttr ( path16 , newattr ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileSetAttr , E , ret %x" , ret ) ;
	#endif	
	
	return ( SFS_NO_ERROR == ret ) ;
	
}
iBOOL i51AdeOsFileFlush ( iFILE file )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0

	iS32 ret = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileFlush , S , %x" , file ) ;
	#endif	
	
	ret = SFS_FlushFile ( file , 0 ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileFlush , E , ret %x" , ret ) ;
	#endif	
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileCopy ( iU16* srcpathex , iU16* destpathex  )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0
	
	iS32 ret = -1 ;

	iU8 buffer [ 512 ] = { 0 } ;

	iU32 TotallData = 0 ;
	iU32 TotallReads = 0 ;
	iU32 CurrentReads = 0 ;
	iU32 OldFile = 0 ;
	iU32 NewFile = 0 ;

	iU16 srcpath [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;
	iU16 destpath [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( srcpathex , (iCHAR* )srcpath ) ;
		i51AdeStdUnicode2Ascii ( destpathex , (iCHAR* )destpath ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileCopy , S , %s , %s" , srcpath , destpath ) ;
	#endif
	
	i51AdeOsGetFilePath ( srcpath , srcpathex ) ; 
	i51AdeOsGetFilePath ( destpath , destpathex ) ; 
	
	OldFile = SFS_CreateFile ( srcpath , SFS_MODE_READ | SFS_MODE_OPEN_EXISTING , 0 , 0 ) ;
	
	if ( 0 == OldFile ) 
		return 0 ;
	
	NewFile = SFS_CreateFile ( destpath , SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS , 0 , 0 ) ;
	
	if ( 0 == NewFile ) 
	{ 
		SFS_CloseFile ( OldFile ) ;
		return 0 ;
	}

	i51AdeOsFileGetSize ( OldFile , &TotallData ) ;
	
	for ( TotallReads = 0 ; TotallReads < TotallData ; )
	{
	
		ret = SFS_ReadFile ( OldFile , buffer , sizeof(buffer) , &CurrentReads , 0 ) ;

		if ( SFS_NO_ERROR != ret ) goto Release ;
		
		TotallReads = TotallReads + CurrentReads ;
		ret = SFS_WriteFile ( NewFile , buffer , CurrentReads , &ret , 0 ) ;

		if ( SFS_NO_ERROR != ret ) goto Release ;
		
	}
	
Release :
	
	SFS_CloseFile ( OldFile ) ;
	SFS_CloseFile ( NewFile ) ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileCopy , E , ret %x" , ret ) ;
	#endif	
	
	return ( SFS_NO_ERROR == ret ) ;
	
}

iBOOL i51AdeOsFileRename ( const iU16* oldnameex , const iU16* newnameex )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0

	iS32 ret = -1 ;

	iU8 buffer [ 512 ] = { 0 } ;

	iU32 TotallData = 0 ;
	iU32 TotallReads = 0 ;
	iU32 CurrentReads = 0 ;
	iU32 OldFile = 0 ;
	iU32 NewFile = 0 ;

	iU16 oldname [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;
	iU16 newname [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( oldnameex , (iCHAR* )oldname ) ;
		i51AdeStdUnicode2Ascii ( newnameex , (iCHAR* )newname ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileRename , S , %s , %s" , oldname , newname ) ;
	#endif
	
	i51AdeOsGetFilePath ( oldname , oldnameex ) ; 
	i51AdeOsGetFilePath ( newname , newnameex ) ; 
	
	OldFile = SFS_CreateFile ( oldname , SFS_MODE_READ | SFS_MODE_OPEN_EXISTING , 0 , 0 ) ;
	
	if ( 0 == OldFile ) 
		return 0 ;
	
	NewFile = SFS_CreateFile ( newname , SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS , 0 , 0 ) ;
	
	if ( 0 == NewFile ) 
	{ 
		SFS_CloseFile ( OldFile ) ;
		return 0 ;
	}

	i51AdeOsFileGetSize ( OldFile , &TotallData ) ;
	
	for ( TotallReads = 0 ; TotallReads < TotallData ; )
	{
	
		ret = SFS_ReadFile ( OldFile , buffer , sizeof(buffer) , &CurrentReads , 0 ) ;

		if ( SFS_NO_ERROR != ret ) goto Release ;
		
		TotallReads = TotallReads + CurrentReads ;
		ret = SFS_WriteFile ( NewFile , buffer , CurrentReads , &ret , 0 ) ;

		if ( SFS_NO_ERROR != ret ) goto Release ;
		
	}
	
Release :
	
	SFS_CloseFile ( OldFile ) ;
	SFS_CloseFile ( NewFile ) ;

	ret = SFS_DeleteFile ( oldname , 0 ) ; 

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileRename , E , ret %x" , ret ) ;
	#endif	
	
	return ( SFS_NO_ERROR == ret ) ;
	
	
}

iBOOL i51AdeOsFileMove ( iU16* oldnameex , iU16* newnameex )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0

	iS32 ret = -1 ;

	iU8 buffer [ 512 ] = { 0 } ;

	iU32 TotallData = 0 ;
	iU32 TotallReads = 0 ;
	iU32 CurrentReads = 0 ;
	iU32 OldFile = 0 ;
	iU32 NewFile = 0 ;

	iU16 oldname [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;
	iU16 newname [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;

	#ifdef i51ADAPTER_DEBUG
		i51AdeStdUnicode2Ascii ( oldnameex , (iCHAR* )oldname ) ;
		i51AdeStdUnicode2Ascii ( newnameex , (iCHAR* )newname ) ;
        i51AdeOsLog(0, "OS : i51AdeOsFileMove , S , %s , %s" , oldname , newname ) ;
	#endif
	
	i51AdeOsGetFilePath ( oldname , oldnameex ) ; 
	i51AdeOsGetFilePath ( newname , newnameex ) ; 
	
	OldFile = SFS_CreateFile ( oldname , SFS_MODE_READ | SFS_MODE_OPEN_EXISTING , 0 , 0 ) ;
	
	if ( 0 == OldFile ) 
		return 0 ;
	
	NewFile = SFS_CreateFile ( newname , SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS , 0 , 0 ) ;
	
	if ( 0 == NewFile ) 
	{ 
		SFS_CloseFile ( OldFile ) ;
		return 0 ;
	}

	i51AdeOsFileGetSize ( OldFile , &TotallData ) ;
	
	for ( TotallReads = 0 ; TotallReads < TotallData ; )
	{
	
		ret = SFS_ReadFile ( OldFile , buffer , sizeof(buffer) , &CurrentReads , 0 ) ;

		if ( SFS_NO_ERROR != ret ) goto Release ;
		
		TotallReads = TotallReads + CurrentReads ;
		ret = SFS_WriteFile ( NewFile , buffer , CurrentReads , &ret , 0 ) ;

		if ( SFS_NO_ERROR != ret ) goto Release ;
		
	}
	
Release :
	
	SFS_CloseFile ( OldFile ) ;
	SFS_CloseFile ( NewFile ) ;

	ret = SFS_DeleteFile ( oldname , 0 ) ; 

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "OS : i51AdeOsFileMove , E , ret %x" , ret ) ;
	#endif	
	
	return ( SFS_NO_ERROR == ret ) ;
	
	
}

iBOOL i51AdeOsFileIsExist ( const iU16* path )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0
	
	SFS_HANDLE file = 0 ;
	iBOOL result = iFALSE ;
	SFS_FIND_DATA_T findfile = { 0 } ;
	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;
	
	i51AdeOsGetFilePath ( path16 , path ) ; 	
	file =  SFS_FindFirstFile ( path16 , &findfile ) ;	

        if(SFS_INVALID_HANDLE != file)
        {
            if(!(SFS_ATTR_DIR&findfile.attr))
            {
                result = iTRUE;
            }
        }
	SFS_FindClose ( file ) ;
	
	return result ;
	
}

iBOOL i51AdeOsFileIsDirExist ( const iU16* path )
{

	//	author : Jelo
	//	since : 2011.4.16

	//	notes : 成功返回> 0 ，失败0
	
	SFS_HANDLE file = 0 ;
	iBOOL result = iFALSE ;
	SFS_FIND_DATA_T findfile = { 0 } ;
	iU16 path16 [ i51_ADE_OS_FILENAME_LEN ] = { 0 } ;
	
	i51AdeOsGetFilePath ( path16 , path ) ; 	
	file =  SFS_FindFirstFile ( path16 , &findfile ) ;	

        if(SFS_INVALID_HANDLE != file)
        {
            if(SFS_ATTR_DIR&findfile.attr)
            {
                result = iTRUE;
            }
        }
	SFS_FindClose ( file ) ;
	
	return result ;
	
}

#ifdef i51PROFILINGS
iU32 i51AdeOsFileTotallOpened ()
{

	//	author : Jelo
	//	since : 2011.7.11

	return TotallFileOpened ;
	
}
#endif

iU32 i51AdeOsTimerCreate ( iBOOL loop , iU32 priority ) 
{

	//	author : Jelo
	//	since : 2011.12.21

	//	将Timer运行模式以及优先级存入TIMERMAP。
	
	iS32 ret = 0 ;	
	
	iS32 looper = 0 ;
	iS32 index = -1 ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsTimerCreate , S , %x , %x" , loop , priority ) ;
	#endif

    i51AdeOsLog(0, "OS : i51AdeOsTimerCreate , %d, %d" , loop , priority ) ;
	for ( looper = 0 ; looper < i51TMMAPMAX ; looper ++ )
	{	

		//	记录i51TMMAP 空挡位置，用于存储新timer
		if ( 0 == i51tmmap[looper].call )
		{		
			index = looper ;
			break ;
		}	
			
	}

	if ( looper < i51TMMAPMAX )
	{
			
		i51tmmap[index].mmkid = 0 ;
		i51tmmap[index].priority = priority ;
		i51tmmap[index].loop = loop ;
		i51tmmap[index].call = 0 ;

		#ifdef i51ADAPTER_DEBUG		
            i51AdeOsLog(0, "OS : i51AdeOsTimerCreate , E , Id %d" , index + 1 ) ;
		#endif

            i51AdeOsLog(0, "OS : i51AdeOsTimerCreate , id  = %d " , index + 1 ) ;
		return index + 1 ;	
		
	}

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsTimerCreate , BE , Id Overflowed" ) ;
	#endif
		
	return 0 ;
 
}

#ifdef i51PROFILINGS
iU32 i51AdeOsTimerGetLive ()
{

	//	author : Jelo
	//	since : 2011.4.20
	//	(C) TOK

	//	notes : 获取当前活跃的TIMER
	//	该接口不对动态层开放

	iS32 looper = 0 ;
	iS32 lives = 0 ;
	
	for ( looper = 0 ; looper < i51TMMAPMAX ; looper ++ )
	{	
		
		if ( 0 != i51tmmap[looper].call )
		{		 
			lives ++ ;
//			break ;
		}
			
	}

	return lives ;
	
	
}
#endif

iBOOL i51AdeOsTimerStart ( iU32 timerid , i51TIMERCALLBACK callback , iU32 time )
{

	//	author : Jelo
	//	since : 2011.4.20

	//	notes : 成功返回> 0 ，失败0
	//	该接口不对动态层开放		

	iU32 timer = 0 ;
	iU32 mmkid = 0 ;
	iS32 looper = 0 ;
	iS32 index = -1 ;

	extern MMI_HANDLE_T MMK_GetFirstAppletHandle( void ) ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsTimerStart , S , %x , %x , %x" , timerid , callback , time ) ;
	#endif

	
    i51AdeOsLog(0, "OS : i51AdeOsTimerStart , %d , %x ,%d " , timerid , callback , time ) ;
	
	if ( 0 >= timerid || timerid > i51TMMAPMAX ) {

        i51AdeOsLog(0, "OS : i51AdeOsTimerStart , id error , F" ) ;
		return 0 ;
	}

	timerid = timerid-1 ;
	
	if ( iTRUE == i51tmmap[timerid].runing && iTRUE == i51tmmap[timerid].loop ) {

        i51AdeOsLog(0, "OS : i51AdeOsTimerStart , F(1)" ) ;
		return 0 ;
	} 

//	mmkid = MMK_CreateWinTimer ( MMK_GetFirstAppletHandle () , time , i51tmmap[timerid].loop ) ;	
	#if defined (SPRD_6600) ||defined (SPRD_6620)
	mmkid = MMK_CreateWinTimer ( VIRTUAL_WIN_ID , time , i51tmmap[timerid].loop ) ;	
	#else
	mmkid = MMK_CreateTimerCallback ( time , i51AdeOsTimerCallBack , 0 , i51tmmap[timerid].loop ) ;
//	MMK_StartTimerCallback ( mmkid , time , i51AdeOsTimerCallBack , 0 , i51tmmap[timerid].loop ) ;	
	#endif
	
	i51tmmap[timerid].call = callback ;
	i51tmmap[timerid].mmkid = mmkid ;
	i51tmmap[timerid].runing = iTRUE ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsTimerStart , E , mmkid %d" , mmkid ) ;
	#endif
	
	return iTRUE ;
	
}

iBOOL i51AdeOsTimerStop ( iU32 timerid ) 
{

	//	author : Jelo
	//	since : 2011.4.20

	//	notes : 成功返回> 0 ，失败0
	//	该接口不对动态层开放	
	
	iS32 ret = 0 ;
	
	iS32 looper = 0 ;
	iS32 index = -1 ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsTimerStop , S , %x" , timerid ) ;
	#endif

        i51AdeOsLog(0, "OS : i51AdeOsTimerStop , %d  " , timerid ) ;
	
	if ( 0 >= timerid || timerid > i51TMMAPMAX ) {

        i51AdeOsLog(0, "OS : i51AdeOsTimerStop , id error , F" ) ;
		return 1 ;
	}

	timerid = timerid-1 ;

	MMK_StopTimer ( i51tmmap[timerid].mmkid ) ;

	memset ( &i51tmmap[timerid] , 0 , sizeof(i51TMMAP) ) ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsTimerStop , E" ) ;
	#endif
	
	return 1 ;
	
}

iCHAR* i51AdeOsGetPathSymbol (void) 
{

	//	author : Jelo
	//	since : 2011.12.21
	
	//	返回系统盘盘符。

	extern const iU16 g_file_device_udisk[] ;

	static iCHAR buffer [ 6 ] = { "_:\\" } ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetPathSymbol , S") ;
	#endif
	
	buffer[0] = g_file_device_udisk[0] ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetPathSymbol , E , %s" , buffer ) ;
	#endif
	
	return buffer ;
	
}

iCHAR* i51AdeOsGetPathSymbolEx (void) 
{

	//	author : Jelo
	//	since : 2011.12.21
	
	//	返回移动存储盘盘符。

	extern const iU16 g_file_device_sdcard[] ;

	static iCHAR buffer [ 6 ] = { "_:\\" } ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetPathSymbolEx , S") ;
	#endif
	
	buffer[0] = g_file_device_sdcard[0] ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetPathSymbolEx , E , %s" , buffer ) ;
	#endif
	
	return buffer ;	
	
}

iBOOL i51AdeOsMMemCardExist (void) 
{

	//	author : Jelo
	//	since : 2011.12.21
	
	iU8 path [64] = { 0 } ;
	iU16 path16 [ 64 ] = { 0 } ;
	iU8* path8 = 0 ;
	iFILE fl = 0 ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsMMemCardExist , S") ;
	#endif	

	strcpy ( path , i51AdeOsGetPathSymbolEx () );
	path8 = strcat ( path , "i51CheckMem.card" ) ;

	i51AdeStdAscii2Unicode ( path8 , path16 ) ;

	i51AdeOsFileAModeEnable () ;
	fl = i51AdeOsFileOpen ( path16 , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
	i51AdeOsFileAModeDisable () ;
		
	if ( 0 == fl ) {

		#ifdef i51ADAPTER_DEBUG		
            i51AdeOsLog(0, "OS : i51AdeOsMMemCardExist , F") ;
		#endif	
	
		return iFALSE ;
	}

	i51AdeOsFileClose ( fl ) ;

	i51AdeOsFileAModeEnable () ;
	i51AdeOsFileDelete ( path16 ) ;
	i51AdeOsFileAModeDisable () ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsMMemCardExist , E OK") ;
	#endif	
	
	return iTRUE ;
	
	
}

iU32 i51AdeOsGetSysDiskSpace ( void ) 
{

	//	Non-Imp

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetSysDiskSpace , S") ;
	#endif	

	iU32 used_high_space = 0 ;
	iU32 used_space = 0 ;
	MMIFILE_GetDeviceUsedSpace(
	g_file_device_udisk,
	1,
	&used_high_space,
	&used_space
	);
    i51AdeOsLog(0, "OS : i51AdeOsGetSysDiskSpace , %d , %d" , used_high_space , used_space ) ;
	
	if ( used_high_space )
		used_space = 4*1024*1024 ; 
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetSysDiskSpace , E") ;
	#endif	
    i51AdeOsLog(0, "OS : i51AdeOsGetSysDiskSpace , All space, %d " , used_space +  i51AdeOsGetSysDiskSpaceLeft () ) ;
	return (used_space/1024 +  i51AdeOsGetSysDiskSpaceLeft ());
}

iU32 i51AdeOsGetSysDiskSpaceLeft ( void ) 
{
	//	Non-Imp

	iU32 free_high_space = 0 ;
	iU32 free_space = 0 ;
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetSysDiskSpaceLeft , S") ;
	#endif	

	MMIFILE_GetDeviceFreeSpace(
	g_file_device_udisk,
	1,
	&free_high_space,
	&free_space
	);
    i51AdeOsLog(0, "OS : i51AdeOsGetSysDiskSpaceLeft , %d , %d" , free_high_space , free_space ) ;
	
	if ( free_high_space )
		free_space = 4*1024*1024 ; 
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetSysDiskSpaceLeft , E") ;
	#endif	
	
	return (free_space/1024) ;
}

iU32 i51AdeOsGetDiskSpace ()
{


	//	author : Jelo
	//	since : 2011.4.21

	//	notes : 获取T 卡总空间
	//	该接口不对动态层开放	

	//	单位(KB)
#if 0
	iU32 hight16 = 0 ;
	iU32 low16 = 0 ;
	iU32 usedspace = 0;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpace , S") ;
	#endif	

#ifdef i51_SPD6800_10A_W11_21

	MMIFILE_GetDeviceUsedSpace (
		MMIFILE_GetDevicePath(MMI_DEVICE_SDCARD), 
		MMIFILE_GetDevicePathLen(MMI_DEVICE_SDCARD),
		&hight16, &low16);
	
	usedspace = ((hight16 << 22) | (low16 >> 10));

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpace , E , %x" , usedspace + i51AdeOsGetDiskSpaceLeft () ) ;
	#endif	

	return usedspace + i51AdeOsGetDiskSpaceLeft () ;
	
#elif defined(i51_SPD6800_10A_W11_09_P2)

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpace , E , 0") ;
	#endif	
	
	return 0 ;

#endif
#else
	iU32 used_high_space = 0 ;
	iU32 used_space = 0 ;
	MMIFILE_GetDeviceUsedSpace(
	g_file_device_sdcard,
	1,
	&used_high_space,
	&used_space
	);
    i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpace , %d , %d" , used_high_space , used_space ) ;
	
	if ( used_high_space )
		used_space = 4*1024*1024 ;
#endif
    i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpace , all space %d" , used_space + i51AdeOsGetDiskSpaceLeft ()  ) ;
	return (used_space/1024 + i51AdeOsGetDiskSpaceLeft () );

	 
}

iU32 i51AdeOsGetDiskSpaceLeft ()
{


	//	author : Jelo
	//	since : 2011.4.21

	//	notes : 获取T 卡剩余空间
	//	该接口不对动态层开放		

	//	单位(KB)
#if 0	
iU32 ret = 0 ;
#ifdef i51_SPD6800_10A_W11_21	

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpaceLeft , S") ;
	#endif	
	
	ret = MMI_GetFreeSpace ( MMI_DEVICE_SDCARD ) ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpaceLeft , E , ret %x" , ret ) ;
	#endif	
	
	return ret ;

#elif defined(i51_SPD6800_10A_W11_09_P2)

	FS_MMC
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpaceLeft , E , ret 0") ;
	#endif	
	
	return 0 ;
else
	ret = MMI_GetFreeSpace ( FS_MMC ) ;

#endif
#else

	iU32 free_high_space = 0 ;
	iU32 free_space = 0 ;
	MMIFILE_GetDeviceFreeSpace(
	g_file_device_sdcard,
	1,
	&free_high_space,
	&free_space
	);
	
    i51AdeOsLog(0, "OS : i51AdeOsGetDiskSpaceLeft , %d , %d" , free_high_space , free_space ) ;
	
	if ( free_high_space )
		free_space = 4*1024*1024 ; 
#endif
	return (free_space/1024) ;
	
	

}

iU32 i51AdeOsGetMemorySpaceLeft ()
{


	//	author : Jelo
	//	since : 2011.4.21

	//	notes : 获取内存剩余空间
	//	该接口不对动态层开放	

	iU32 ret = 0 ;
	 
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetMemorySpaceLeft , S") ;
	#endif	

	#ifndef SPRD_6530
	ret =  SCI_GetMaxAppHeapAvalidSpace () ;
	#else
	ret = 1024*1024  ;
	#endif
	
//	ret = SCI_GetMaxAppHeapAvalidSpace () ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetMemorySpaceLeft , E , ret %x" , ret ) ;
	#endif	
	
	return ret ;
	
}

iU32 i51AdeOsGetCallingTime ()
{

	//	Non-Imp

       MMICL_CALL_ARRAY_BCDINFO_T*   arraycall_info = PNULL;
       MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
       iU32 CallTiem = 0;
	   iU32 i = 0;
	   iU32 j = 0;

       //CC_CALL_TIME_COUNT_T p_cc_time;
       //MMIAPICL_GetLastCallTime(&p_cc_time,MN_DUAL_SYS_2);
       
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsGetCallingTime , S") ;
	#endif	
              
       arraycall_info = SCI_ALLOC_APP(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
       SCI_ASSERT(NULL != arraycall_info);
       
       SCI_MEMSET(arraycall_info,0,sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
       
       //get the value of arraycall_info from nv
       return_value = MMICL_ReadNV(MMICL_CALL_DIALED, arraycall_info);
       for(i = 0; i < arraycall_info->record_num; ++i) //通话记录中记录的拨打号码个数
  	   {
         #if defined(CALL_TIMES_SUPPORT) && defined(SPRD_6530)
	       for(j = 0; j < arraycall_info->call_bcdinfo[i].call_times; ++j) //该拨打号码的通话记录个数
    	   {
                CallTiem += arraycall_info->call_bcdinfo[i].call_duration_time[j]; //call_duration_time以秒为单位
    	   }
         #else //6530之前的机型
           CallTiem += arraycall_info->call_bcdinfo[i].call_duration_time; //call_duration_time以秒为单位
         #endif
       }
       SCI_FREE(arraycall_info);
       
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsGetCallingTime , E :%d",CallTiem) ;
	#endif	
	
	return CallTiem ;
	
}


iU32 i51AdeOsGetNetFlow ()
{

	//	Non-Imp?	

       iU32 recvLen = 0;
	iS32 *siminfo = i51AdeSmsGetSims() ;
	#ifdef SPRD_6600
 	MMIBRW_GPRS_FLOWRATE_T *gprs_flowrate = NULL ;

	if ( !siminfo ) 
		return 0 ;
	if ( siminfo[1] ) {
		
		gprs_flowrate = MMIBRW_GetGlobalGprsFlowRate (siminfo[1]-1) ;
		recvLen += (gprs_flowrate->total_recv + gprs_flowrate->total_sent) ;
		
	}

	if ( siminfo[2] ) {
		
		gprs_flowrate = MMIBRW_GetGlobalGprsFlowRate (siminfo[2]-1) ;
		recvLen += (gprs_flowrate->total_recv + gprs_flowrate->total_sent) ;
		
	}

	#else
       MMIPDP_FLOWRATE_T   gprs_flowrate = {0};
  
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsGetNetFlow , S") ;
	#endif	
	if ( !siminfo ) 
		return 0 ;
	if ( siminfo[1] ) {
		
       MMIAPIPDP_GetFlowrate(&gprs_flowrate, siminfo[1]-1);
       recvLen += gprs_flowrate.total_recv + gprs_flowrate.total_send;
	}

	if ( siminfo[2] ) {
		
       MMIAPIPDP_GetFlowrate(&gprs_flowrate, siminfo[2]-1);
       recvLen += gprs_flowrate.total_recv + gprs_flowrate.total_send;
	}

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetNetFlow , E") ;
	#endif	

	#endif

	return recvLen;
	
}

iU32 i51AdeOsGetTick ()
{

	//	author : Jelo
	//	since : 2011.5.19
	// 	(C) 2011 , PKIG Tech. Co., Ltd. 

	iU32 ret = 0 ;
	
	#ifdef i51ADAPTER_DEBUG		
	//	i51AdeOsLog(0, "OS : i51AdeOsGetTick , S") ;
	#endif	

	//ret = (SCI_GetTickCount () *1024) >> 10 ; 

	ret = SCI_GetTickCount () ; 
    
	#ifdef i51ADAPTER_DEBUG		
	//	i51AdeOsLog(0, "OS : i51AdeOsGetTick , E , ret %d" , ret ) ;
	#endif	
	
	return ret ;
	
}

iBOOL i51AdeOsGetTime ( iU16* year , iU16* month , iU16* day , iU16* hour , iU16* mins , iU16* sec )
{

	//	author : Jelo 
	//	since : 2011.5.19
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	SCI_TIME_T	tmp_time = { 0 } ;
	SCI_DATE_T	tmp_date = { 0 } ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetTime , S , %x , %x , %x , %x , %x , %x" , year , month , day , hour , mins , sec ) ;
	#endif	
	
	TM_GetSysDate(&tmp_date);
	TM_GetSysTime(&tmp_time);	

	*year = tmp_date.year;
	*month = tmp_date.mon;
	*day = tmp_date.mday;

	*hour = tmp_time.hour;
	*mins = tmp_time.min;
	*sec = tmp_time.sec;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetTime , E , %x , %x , %x , %x , %x , %x" , *year , *month , *day , *hour , *mins , *sec ) ;
	#endif	
	
	return iTRUE ;

}

iBOOL i51AdeOsSetTime ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec ) 
{

	//	author : Jelo 
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	SCI_TIME_T	tmp_time = { 0 } ;
	SCI_DATE_T	tmp_date = { 0 } ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetTime , S , %x , %x , %x , %x , %x , %x" , year , month , day , hour , mins , sec ) ;
	#endif	
	
	tmp_date.year = year ;
	tmp_date.mon = month ;
	tmp_date.mday = day ;

	tmp_time.hour = hour ;
	tmp_time.min = mins ;
	tmp_time.sec = sec ;

	TM_SetSysDate(tmp_date);
	TM_SetSysTime(tmp_time);

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetTime , E" ) ;
	#endif	
	
	return iTRUE ;
	
}

iU64 i51AdeOsGetSeconds ( iU16 year , iU16 mon , iU16 day , iU16 hour , iU16 mins , iU16 secs )
{

    //    author : Jelo 
    //    since : 2011.8.2
    //     (C) 2011 , PKIG Tech. Co., Ltd.    

    //    返回从1970-1-1 00:00:00 到当前时间所经过的秒数

#if 1

    iU32 looper = 0 ;

    //    闰年
    iU16 runyear = 0 ;

    iU64 TotallSecs = 0 ;
    
    #define xMINUTE (60)
    #define xHOUR (60*xMINUTE)
    #define xDAY (24*xHOUR)
    #define xYEAR (365*xDAY)

    iU32 month [ 12 ] = {
    
        xDAY*(0),
        xDAY*(31),
        xDAY*(31+28),
        xDAY*(31+28+31),
        xDAY*(31+28+31+30),
        xDAY*(31+28+31+30+31),
        xDAY*(31+28+31+30+31+30),
        xDAY*(31+28+31+30+31+30+31),
        xDAY*(31+28+31+30+31+30+31+31),
        xDAY*(31+28+31+30+31+30+31+31+30),
        xDAY*(31+28+31+30+31+30+31+31+30+31),
        xDAY*(31+28+31+30+31+30+31+31+30+31+30)

    } ;

    #ifdef i51ADAPTER_DEBUG        
        i51AdeOsLog(0, "OS : i51AdeOsGetSeconds , S , %x , %x , %x , %x , %x , %x" , year , month , day , hour , mins , secs ) ;
    #endif    
    
    TotallSecs += xYEAR*(year-1970) ;            

    for (looper = 1970; looper <= year; looper++)
    {

        iBOOL leap = iFALSE ;
        leap = (iBOOL)((!(looper % 4) && (looper % 100)) || !(looper % 400));
        if ( iTRUE == leap )
        {
            if (year == looper)
            {
                if (2 < mon) // 过了3月份才会新增一天，2月29日并不会增加1天，3月1日才能增加
                {
                    runyear = runyear + 1;
                }
            }
            else
            {
                runyear = runyear + 1;
            }
        }
    }

    TotallSecs += month[mon-1];            
    TotallSecs += xDAY*((day-1)+runyear);            
    TotallSecs += xHOUR*hour;            
    TotallSecs += xMINUTE*mins;            
    TotallSecs += secs;                    

    #ifdef i51ADAPTER_DEBUG        
        i51AdeOsLog(0, "OS : i51AdeOsGetSeconds , E , ret %x" , TotallSecs ) ;
    #endif    
    
    return TotallSecs;
    
#else

    iU16 year = 0 ;
    iU16 month = 0 ;
    iU16 day = 0 ;
    iU16 hour = 0 ;
    iU16 mins = 0 ;
    iU16 secs = 0 ;
    iU32 totall = 0 ;

    i51AdeOsGetTime ( &year , &month , &day , &hour , &mins , &secs ) ;

    i51AdeOsLog(0, "OS : i51AdeOsGetSeconds , year %d , month %d , day %d , hour %d , mins %d , secs %d\r\n" ,
        year , month , day , hour , mins , secs ) ;
    
    totall = MMIAPICOM_Tm2Second ( secs , mins , hour , day , month , year ) ;

    i51AdeOsLog(0, "OS : i51AdeOsGetSeconds , totall %d\r\n" , totall ) ; 
    
    return totall ;

#endif

}

iBOOL i51AdeOsSecond2Time ( 
    iU64 TotallSeconds , 
    iU16* year , 
    iU16* month , 
    iU16* day , 
    iU16* hour , 
    iU16* mins , 
    iU16* sec 
)
{
#if 1
    //    返回从1970-1-1 00:00:00 经过TotallSeconds秒数后时间
    
    iU64 i = 0; 
    //    闰年
    iBOOL leap = iFALSE ;

#define xMINUTE (60)
#define xHOUR (60*xMINUTE)
#define xDAY (24*xHOUR)
#define xYEAR (365*xDAY)

    iU32 months[ 12 ] = {
        xDAY*(0),
        xDAY*(31),
        xDAY*(31+28),
        xDAY*(31+28+31),
        xDAY*(31+28+31+30),
        xDAY*(31+28+31+30+31),
        xDAY*(31+28+31+30+31+30),
        xDAY*(31+28+31+30+31+30+31),
        xDAY*(31+28+31+30+31+30+31+31),
        xDAY*(31+28+31+30+31+30+31+31+30),
        xDAY*(31+28+31+30+31+30+31+31+30+31),
        xDAY*(31+28+31+30+31+30+31+31+30+31+30)
    } ;

#ifdef i51ADAPTER_DEBUG        
    i51AdeOsLog(0, "OS : i51AdeOsSecond2Time , S ,%d, %x , %x , %x , %x , %x , %x" , TotallSeconds , year , month , day , hour , mins , sec ) ;
#endif

       //初始化
    *year = 1970;
    *month = 1;
    *day = 1;
    *hour = 0;
    *mins = 0;
    *sec = 0;

    //计算年,i为一年秒数
    do
      {
        leap = (iBOOL)((!(*year % 4) && (*year % 100)) || !(*year % 400));
        i = ( iTRUE == leap ) ? (xYEAR + xDAY) : (xYEAR);
        if (TotallSeconds >= i)
        {
            TotallSeconds -= i;
            ++(*year);
            i = 0;
        }
    } while (i < TotallSeconds);

    //计算月
    if (iTRUE == leap) //leap year
    {
        for (i = 2; i < 12; ++i)
        {
            months[i] += xDAY;
        }
    }
    for (i = 0; months[i] <= TotallSeconds && 12 > i; ++i)
    {
    }
    TotallSeconds -= months[i-1];
    *month = i;

    //计算日
    *day = TotallSeconds / xDAY + 1;
    TotallSeconds %= xDAY;

    //计算时
    *hour = TotallSeconds / xHOUR;
    TotallSeconds %= xHOUR;

    //计算分
    *mins = TotallSeconds / xMINUTE;
    TotallSeconds %= xMINUTE;

    //计算秒
    *sec = TotallSeconds;

    #ifdef i51ADAPTER_DEBUG        
        i51AdeOsLog(0, "OS : i51AdeOsSecond2Time , E , %x , %x , %x , %x , %x , %x" , year , month , day , hour , mins , sec ) ;
    #endif    
    
    return iTRUE ;
#else
	//	author : Jelo 
	//	since : 2011.5.19
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	MMI_TM_T t = {0} ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSecond2Time , S , %x , %x , %x , %x , %x , %x" , year , month , day , hour , mins , sec ) ;
	#endif	
	
	t = MMIAPICOM_Second2Tm ( TotallSeconds ) ;

	*year = t.tm_year ;
	*month = t.tm_mon+1 ;
	*day = t.tm_mday+1 ;
	*hour = t.tm_hour+1 ;
	*mins = t.tm_min+1 ;
	*sec = t.tm_sec+1 ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSecond2Time , E , %x , %x , %x , %x , %x , %x" , year , month , day , hour , mins , sec ) ;
	#endif	
	
	return iTRUE ;

#endif
}

iBOOL i51AdeOsPermanitBufferRead ( iU8* buffer , iU32 length )
{


#if 1
	i51_SN_T sn = {0} ;
	MMI_ReadNVItem ( MMINV_I51_SN , &sn ) ;
	memcpy ( buffer , &sn , sizeof(sn) ) ;
    i51AdeOsLog(0, "OS : i51AdeOsPermanitBufferRead , %d , %d ,%d" , sn.CRC , sn.snvalue1 , sn.snvalue2 ) ;
	
	return iTRUE ;
	
#else
	iFILE file = 0 ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsPermanitBufferRead , S , %x , %x" , buffer , length ) ;
	#endif	
	
	i51FILEOPEN ( file , L"C:\\i51SERIAL" , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN ) ;
	
	if ( 0 == file ) {

		#ifdef i51ADAPTER_DEBUG		
            i51AdeOsLog(0, "OS : i51AdeOsPermanitBufferRead , E , ret 0" ) ;
		#endif	
		
		return 0 ;
	}

	i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileRead ( file , buffer , length , 0 ) ;
	i51AdeOsFileClose ( file ) ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsPermanitBufferRead , E , ret 1" ) ;
	#endif	
		
	return iTRUE ;
#endif	
}

iBOOL i51AdeOsPermanitBufferWrite ( iU8* buffer , iU32 length )
{

#if 1
	i51_SN_T sn = {0} ;
	memcpy ( &sn.CRC , buffer , 4 ) ;
	memcpy ( &sn.snvalue1, buffer + 4 , 4 ) ;
	memcpy ( &sn.snvalue2, buffer + 8 , 4) ;
    i51AdeOsLog(0, "OS : i51AdeOsPermanitBufferWrite , %d , %d ,%d" , sn.CRC , sn.snvalue1 , sn.snvalue2 ) ;
	MMI_WriteNVItem ( MMINV_I51_SN , buffer ) ;
	return iTRUE ;
#else
	iFILE file = 0 ;
	iU16 path [64] = {0} ;
	iU16* symbol = 0 ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsPermanitBufferWrite ,  S , %x , %x" , buffer , length ) ;
	#endif	
	
	i51FILEOPEN ( file , L"C:\\i51SERIAL" , i51_OS_FL_WRITE | i51_OS_FL_EXISTING_OPEN ) ;

	if ( 0 == file )
	{
		i51FILEOPEN ( file , L"C:\\i51SERIAL" , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE ) ;
	}
 
	if ( 0 == file ) {

        i51AdeOsLog(0, "OS : i51AdeOsPermanitBufferWrite , E , ret 0" ) ;
		return iFALSE ;
	}

	i51AdeOsFileAModeEnable ();
	i51AdeOsFileSetAttr ( L"C:\\i51SERIAL" , i51_OS_FL_ATTR_HID | i51_OS_FL_ATTR_ROL ) ;
	i51AdeOsFileAModeDisable ();
		
	i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileWrite ( file , buffer , length , 0 ) ;
	i51AdeOsFileClose ( file ) ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsPermanitBufferWrite , E , ret 1" ) ;
	#endif			
	return iTRUE ;
#endif
	
}

iBOOL i51AdeOsSleep ( iU32 mis )
{

	//	author : Jelo 
	//	since : 2011.8.12
	// 	(C) 2011 , PKIG Tech. Co., Ltd.		

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSleep , S , %x" , mis ) ;
	#endif	
	
	SCI_Sleep ( mis ) ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSleep , E , ret 1" ) ;
	#endif	
	
	return iTRUE ;
 
}


extern PHONE_SERVICE_STATUS_T  g_service_status[] ; 

iBOOL i51AdeOsGetBaseStation ( iS32 simCard , iU32* bsid , iU32* zid ) 
{

	//	Non-Imp

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetBaseStation , S , %x , %x , %x" , simCard , bsid , zid ) ;
	#endif	

    i51AdeOsLog(0, "OS : i51AdeOsGetBaseStation , %d , %x , %x" , simCard , bsid , zid ) ;
	if ( !simCard )
		return iFALSE ;

	*bsid = g_service_status[simCard-1].cell_id ;
	*zid = g_service_status[simCard -1].lac ;

    i51AdeOsLog(0, "OS : i51AdeOsGetBaseStation , basid = %d , zid = %d" , *bsid , *zid ) ;

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetBaseStation , E") ;
	#endif	
	
	return iTRUE ;
	
}

iBOOL i51AdeOsSetAccelerator ( i51ACCCALLBACK callback )
{

	//	Non-Imp


	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetAccelerator , S , %x" , callback ) ;
	#endif	

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetAccelerator , E") ;
	#endif	

	
	return iTRUE ;
	
}

iU32 i51AdeOsCreatePhone () 
{

	//	Non-Imp

       iU32 i = 0;
       iU32 id = 0;

	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsCreatePhone , S") ;
	#endif	
    
       for(i =0;i<5;i++)
       {
             if(i51PhoneDevice[i]==0)
             {
                 i51PhoneDevice[i] = 1;
                 id = i+1 ;
                 break;
             }
       }
        
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsCreatePhone , E:%d",id) ;
	#endif	

	
	return id ;
}
	

iBOOL i51AdeOsStartPhoneCall ( iU32 phoneid , iCHAR* number , void (*callback) ( iBOOL results) ) 
{

	//	Non-Imp

         MMI_PARTY_NUMBER_T      party_num = {0};
         CC_CALL_CONTEXT_T CallTest = {0};
         uint8	copy_len = 0;
         MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
         MN_CALL_TYPE_E call_type = MN_CALL_TYPE_NORMAL;
         MN_CALL_CLIR_E mmi_clir_type =MMIAPISET_GetClirType(dual_sys);//
         iS32 simbuf[32 ];
         iS32 *sims = simbuf;
         sims = i51AdeSmsGetSims ();

         #ifdef i51ADAPTER_DEBUG		
         i51AdeOsLog(0, "OS : i51AdeOsStartPhoneCall , S , %x , %x , %x" , phoneid , number , callback ) ;
 	  #endif	

         i51AdeOsLog(0, "OS : i51AdeOsStartPhoneCall , S ") ;
      
         if(phoneid<1||phoneid>5)return iFALSE;
         if(i51PhoneDevice[phoneid-1]==0)return iFALSE;
            
         if(sims[1]==2)dual_sys = MN_DUAL_SYS_2;
         i51AdeOsLog(0, "OS : i51AdeOsStartPhoneCall , mmi_clir_type:%d",mmi_clir_type) ;
         if( TRUE != MMIAPICOM_GenPartyNumber((uint8 *)number, strlen(number), &party_num)	)
         {
             return iFALSE;
         }
         CallTest.want_mo_call.address.number_type = party_num.number_type ;
         CallTest.want_mo_call.address.number_plan = MN_NUM_PLAN_ISDN_TELE;
             
         copy_len = ( party_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num.num_len;
         MMI_MEMCPY(
             (void*)CallTest.want_mo_call.address.party_num,
             sizeof(CallTest.want_mo_call.address.party_num),
             (void*)party_num.bcd_num,
             sizeof(party_num.bcd_num),
             copy_len
             );
         CallTest.want_mo_call.address.num_len = copy_len ;
         CallTest.want_mo_call.dual_sys = dual_sys;

 
		/* 
	   #ifdef SPRD_6800
        // MMIDEFAULT_EnableKeyRing(iFALSE);
	   #else
	   MMIDEFAULT_EnableKeyRing(iFALSE);
	   #endif
	   */

	
         if (MN_RETURN_SUCCESS != MNCALL_StartCallEx(dual_sys, call_type, 
            &(CallTest.want_mo_call.address), 
            PNULL, 
            mmi_clir_type, 
            PNULL ) )
        {
            i51AdeOsLog(0, "OS : i51AdeOsStartPhoneCall , call Fail , F") ;
             return iFALSE ;
        }


	i51CallStartFlag = 1;
    i51AdeOsLog(0, "OS : i51AdeOsStartPhoneCall , E") ;
	
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsStartPhoneCall , E") ;
	#endif	

	
	return iTRUE ;
}

iBOOL i51AdeOsStopPhoneCall ( iU32 phoneid ) 
{

	//	Non-Imp


       iS32 simbuf[32 ];
       iS32 *sims = simbuf;
       MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
       
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsStopPhoneCall , S , %x" , phoneid ) ;
	#endif	

       i51AdeOsLog(0, "OS : i51AdeOsStopPhoneCall , S ") ;

       if(phoneid<1||phoneid>5)return iFALSE;
       if(i51PhoneDevice[phoneid-1]==0)return iFALSE;

       sims = i51AdeSmsGetSims ();
       if(sims[1]==2)dual_sys = MN_DUAL_SYS_2;
       //
       MNCALL_ReleaseActiveCallsEx(dual_sys);
       //AUDIO_SetDownLinkMute(iFALSE);
       //MMIDEFAULT_EnableKeyRing(iTRUE);

       i51AdeOsLog(0, "OS : i51AdeOsStopPhoneCall , E ") ;

	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsStopPhoneCall , E") ;
	#endif	
		

	return iTRUE ;
}
	

iBOOL i51AdeOsReleasePhone ( iU32 phoneid ) 
{

	//	Non-Imp


	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsReleasePhone , S , %x" , phoneid ) ;
	#endif	

       if((phoneid==0)||(phoneid>5))
       {
            return iFALSE;
       }
       i51PhoneDevice[phoneid-1] = 0;
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsReleasePhone , E") ;
	#endif	

	
	return iTRUE ;
}

iBOOL i51AdeOsRunWapExplorer ( iU16* url )
{
     iCHAR dst[128] = {0};
     MMIBROWSER_ERROR_E result = MMIBROWSER_SUCC;
     MMIBROWSER_ENTRY_PARAM entry_param = {0};

     #ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsRunWapExplorer , S") ;
	 #endif	
     
     i51AdeStdUnicode2Ascii(url , dst);
     entry_param.type = MMIBROWSER_ACCESS_URL;
     entry_param.dual_sys = MN_DUAL_SYS_MAX;
     entry_param.url_ptr = dst;
     entry_param.url_title_ptr = PNULL;
     entry_param.user_agent_ptr = PNULL;
     
     result = MMIBROWSER_Entry(&entry_param);

     #ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsRunWapExplorer , E :%d",result) ;
	 #endif
     
     return (result==MMIBROWSER_SUCC);

}

iBOOL i51AdeOsSetBrowserPage ( iCHAR* homepage ) 
{

	//	Non-Imp
	#ifdef SPRD_6600
	#else
       MMICONNECTION_SETTING_T mmiTEst = {0};
       iU8 indext = 0;
       MN_RETURN_RESULT_E    n_result = MN_RETURN_FAILURE;
       MMI_STRING_T homepage_str;
       iU8 results = 0;
       iS32 simbuf[32 ];
       iS32 *sims = simbuf;
       MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_1;
       sims = i51AdeSmsGetSims ();
       if(sims[1]==2)dual_sys = MN_DUAL_SYS_2;

	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsSetBrowserPage , S , %s" , homepage ) ;
	#endif	


		//	 此处发现1135 版本使用的是 6530
        #ifdef SPRD_6530
        MMINV_READ(MMINV_CONNECTION_SETTING_INFO, &mmiTEst,n_result);//
        SCI_MEMSET(mmiTEst.setting_detail[dual_sys][1].homepage,0,sizeof(mmiTEst.setting_detail[dual_sys][1].homepage))
        SCI_MEMCPY(mmiTEst.setting_detail[dual_sys][1].homepage,homepage,strlen(homepage));
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &mmiTEst);
        MMIBROWSER_SetNetIndexSetting(dual_sys,1);
        MMICONNECTION_Init( );
        #else
        MMINV_READ(MMINV_CONNECTION_SETTING_INFO, &mmiTEst,n_result);//
        SCI_MEMSET(mmiTEst.setting_detail[1].homepage,0,sizeof(mmiTEst.setting_detail[1].homepage))
        SCI_MEMCPY(mmiTEst.setting_detail[1].homepage,homepage,strlen(homepage));
        MMINV_WRITE(MMINV_CONNECTION_SETTING_INFO, &mmiTEst);
        MMIBROWSER_SetNetSettingIndex(dual_sys,1);
        MMICONNECTION_Init( );
	 #endif
  		
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetBrowserPage , E %d",results) ;
	#endif	
	
	return iTRUE ;
	#endif 
	
	return iFALSE ;
}

iBOOL i51AdeOsSetCPUCache (iU32 start_addr, iU32 end_addr) 
{

	//	Non-Imp


	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetCPUCache , S") ;
	#endif	

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetCPUCache , E") ;
	#endif	

	
	return iTRUE ;
}

iBOOL i51AdeOsUnsetCPUCache () 
{

	//	Non-Imp


	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsUnsetCPUCache , S") ;
	#endif	

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsUnsetCPUCache , E") ;
	#endif	

	return iTRUE ;
}

iBOOL i51AdeOsSetShock ( iBOOL open ) 
{

	//	Non-Imp

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetShock , S , %x" , open ) ;
	#endif	
	/*
       if(iTRUE==i51VibratorIsOpen)
       {
           GPIO_SetVibrator(iFALSE);
       }
       GPIO_SetVibrator(open);
       i51VibratorIsOpen = open;
       */

        i51AdeOsLog(0, "OS : i51AdeOsSetShock , %d" , open ) ;
       i51VibratorIsOpen = open ;
       if ( !open )
	   	GPIO_SetVibrator(iFALSE);
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsSetShock , E") ;
	#endif	
	
	return iTRUE ;
	
}

iBOOL i51AdeOsDoShock ( iU32 time ) 
{

	//	Non-Imp

       iBOOL result = iFALSE;
    
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsDoShock , S , %x" , time ) ;
	#endif	

       i51AdeOsLog(0, "OS : i51AdeOsDoShock , %d" , time ) ;
    if ( !i51VibratorIsOpen ) {

        i51AdeOsLog(0, "OS : i51AdeOsDoShock , Vibrator close , F" ) ;
		return iFALSE ;
    }
       i51OsShockTimerID = i51AdeOsTimerCreate (0,1) ;
       result = i51AdeOsTimerStart(i51OsShockTimerID,i51OsShockCallback,time);
     GPIO_SetVibrator (1) ; 

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsDoShock , E") ;
	#endif	
	
	return result ;
	
}

iBOOL i51AdeOsSetScreenLight ( iBOOL open ) 
{

	//	author : Jelo 
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	


	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetScreenLight , S , %x" , open ) ;
	#endif	

	MMIDEFAULT_SetBackLight(open);		

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsSetScreenLight , E") ;
	#endif	
			
	return iTRUE ;
	
}

iBOOL i51AdeOsGetScreenLight () 
{

	//	Non-Imp


	 iBOOL result = iFALSE;
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsGetScreenLight , S") ;
	#endif	

       result = MMIDEFAULT_GetBackLightState();
	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsGetScreenLight , E") ;
	#endif	
		
	return result ;
	
}

static iBOOL i51SetAlwayLight = iFALSE ;

void i51AdeOsPortSetAlwayLight ( iBOOL always )
{
	//	在port内设置是否常亮

	i51SetAlwayLight = always ;
	MMIDEFAULT_AllowTurnOffBackLight(!always);
}
iBOOL i51AdeOsScreenAlwaysLight ( iBOOL always ) 
{

	//	author : Jelo 
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	


	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsScreenAlwaysLight , S , %x" , always ) ;
	#endif	

	if ( i51SetAlwayLight ) 
		return iTRUE ;
	MMIDEFAULT_AllowTurnOffBackLight(!always);
#if 0
	if ( always ) {
		
		MMIDEFAULT_SetAlwaysHalfOnBackLight (always) ;
	} else {

		MMIDEFAULT_SetAlwaysHalfOnBackLight (always) ;
		MMIDEFAULT_AllowTurnOffBackLight (iTRUE) ;
	}
#endif
	
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsScreenAlwaysLight , E") ;
	#endif	
			
	return iTRUE ;
	
}

static void i51AdeCreateCB ( iU32 id )
{
	i51AdeOsTimerStop ( i51AdeCreateTimer ) ;
	i51AdeCreateTimer = 0 ;
	if ( i51AdeCallback ) {
		
		i51AdeCallback (1) ;
	}
}
iBOOL i51AdeOsCreate ( i51ADECALLBACK callback ) 
{

	//	author : Jelo 
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	

	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsCreate , S , %x" , callback ) ;
	#endif	
	
	i51AdeCallback = callback ;

//	i51AdeOsTotallMemory = SCI_GetMaxAppHeapAvalidSpace () ;

//	i51AdeOsLog(0, "OS : i51AdeOsCreate , Totall Memory %d" , i51AdeOsTotallMemory ) ;
	  
//	MMK_CreateTimerCallback ( 10 , i51AdeOsCreateCallBack , iTRUE , iFALSE ) ;
	//	临时方案，正式版需要使用timer
//	callback (1) ;

       i51AdeCreateTimer = i51AdeOsTimerCreate (0,1) ;
	if ( i51AdeCreateTimer == 0 ) {

		return iFALSE ;
	}
       i51AdeOsTimerStart(i51AdeCreateTimer,i51AdeCreateCB,10);


	#ifdef i51ADAPTER_DEBUG		
       i51AdeOsLog(0, "OS : i51AdeOsCreate , E") ;
	#endif	
	
	return iTRUE ;
	
}

iBOOL i51AdeOsRelease ()
{

	//	author : Jelo 
	//	since : 2011.5.25
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : 释放由i51AdeOs.c 内占用的系统资源
	//	该接口不对动态层开放


	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsRelease , S") ;
	#endif	
	
	#ifdef i51ADAPTER_DEBUG		
        i51AdeOsLog(0, "OS : i51AdeOsRelease , E") ;
	#endif	
	
	return 1 ;
	
}

#if defined (I51_MAKECALL_PARA_6)
void i51SetCallInfo(MN_DUAL_SYS_E dual_sys,iU8 *tele_num,iU8 tele_len,MMI_STRING_T* name_ptr,CC_CALL_SIM_TYPE_E call_sim_type,iU32 call_type )
{
        i51AdeOsLog(0, "OS : i51SetCallInfo , sim :%d , telenum :%s , telelen :%d , callsimtype:%d , call_type:%d" , dual_sys , tele_num , tele_len , call_sim_type , call_type ) ;
        i51CallInfoStruct.dual_sys_i51 = dual_sys;
        memcpy(i51CallInfoStruct.tele_num_i51,tele_num,tele_len);
        i51CallInfoStruct.tele_len_i51 = tele_len;
        memcpy(&i51CallInfoStruct.name_ptr_i51,name_ptr,sizeof(MMI_STRING_T));
        i51CallInfoStruct.call_sim_type_i51 = call_sim_type;
        i51CallInfoStruct.call_type_i51 = call_type;
}
#elif defined (I51_MAKECALL_PARA_7)
void i51SetCallInfo(MN_DUAL_SYS_E dual_sys,iU8 *tele_num,iU8 tele_len ,MMI_STRING_T* name_ptr,CC_CALL_SIM_TYPE_E call_sim_type,iU32 call_type , MN_SUBADDR_T *sub_addr )
{
        i51AdeOsLog(0, "OS : i51SetCallInfo , sim :%d , telenum :%s , telelen :%d , callsimtype:%d , call_type:%d" , dual_sys , tele_num , tele_len , call_sim_type , call_type ) ;
        i51CallInfoStruct.dual_sys_i51 = dual_sys;
        memcpy(i51CallInfoStruct.tele_num_i51,tele_num,tele_len);
        i51CallInfoStruct.tele_len_i51 = tele_len;
        memcpy(&i51CallInfoStruct.name_ptr_i51,name_ptr,sizeof(MMI_STRING_T));
        i51CallInfoStruct.call_sim_type_i51 = call_sim_type;
        i51CallInfoStruct.call_type_i51 = call_type;
	 memcpy ( &i51CallInfoStruct.subaddr , sub_addr , sizeof(MN_SUBADDR_T)) ;
}
#elif defined (I51_MAKECALL_PARA_8)
void i51SetCallInfo(MN_DUAL_SYS_E dual_sys,iU8 *tele_num,iU8 tele_len,MMI_STRING_T  *disp_pt ,MMI_STRING_T* name_ptr,CC_CALL_SIM_TYPE_E call_sim_type,iU32 call_type , MN_SUBADDR_T *sub_addr )
{
        i51AdeOsLog(0, "OS : i51SetCallInfo , sim :%d , telenum :%s , telelen :%d , callsimtype:%d , call_type:%d" , dual_sys , tele_num , tele_len , call_sim_type , call_type ) ;
        i51CallInfoStruct.dual_sys_i51 = dual_sys;
        memcpy(i51CallInfoStruct.tele_num_i51,tele_num,tele_len);
        i51CallInfoStruct.tele_len_i51 = tele_len;
        memcpy(&i51CallInfoStruct.name_ptr_i51,name_ptr,sizeof(MMI_STRING_T));
	 memcpy ( &i51CallInfoStruct.disp, disp_pt , sizeof(MMI_STRING_T)) ;
        i51CallInfoStruct.call_sim_type_i51 = call_sim_type;
        i51CallInfoStruct.call_type_i51 = call_type;
	 memcpy ( &i51CallInfoStruct.subaddr , sub_addr , sizeof(MN_SUBADDR_T)) ;
}
#endif

//extern BOOLEAN MMIAPIVT_HandlePsMsg(uint16 msg_id, DPARAM param) ;
MMI_RESULT_E i51CC_HandlePsMsg( uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch( msg_id )
    {
    case APP_MN_SYNC_IND:
        //收到信道分配事件的处理
        {
            i51AdeOsLog(0, "OS : i51CC_HandlePsMsg , i51 Call start ") ;
		#if defined (SPRD_6600) || defined (SPRD_6620) || defined (SPRD_6800)
            MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_INIT, MMICC_AUDIO_DEV_MAX);
        #endif
//            MMICC_EnableVoiceCodec( TRUE );
            AUDIO_SetDownLinkMute(TRUE);
        }
        break;
        
    case APP_MN_CALL_DISCONNECTED_IND:       

        {
             if(i51CallStartFlag==1)
             {      
                 i51AdeOsLog(0, "OS : i51CC_HandlePsMsg , i51 Call End (1) ") ;
              AUDIO_SetDownLinkMute(FALSE);
                  i51CallStartFlag = 0;
             }
             else if(i51CallStartFlag==2)
		{
            i51AdeOsLog(0, "OS : i51CC_HandlePsMsg , i51 Call End (2) ") ;
              AUDIO_SetDownLinkMute(FALSE);
               i51CallStartFlag = 0;

		#if defined (I51_MAKECALL_PARA_6)
		MMIAPICC_MakeCall ( i51CallInfoStruct.dual_sys_i51 , i51CallInfoStruct.tele_num_i51, i51CallInfoStruct.tele_len_i51 ,  NULL , i51CallInfoStruct.call_sim_type_i51 , 0 ) ;
		#elif defined (I51_MAKECALL_PARA_7)
		MMIAPICC_MakeCall ( i51CallInfoStruct.dual_sys_i51 , i51CallInfoStruct.tele_num_i51, i51CallInfoStruct.tele_len_i51 ,  NULL , i51CallInfoStruct.call_sim_type_i51 , 0 , NULL ) ;
		#elif defined (I51_MAKECALL_PARA_8)
		MMIAPICC_MakeCall ( i51CallInfoStruct.dual_sys_i51 , i51CallInfoStruct.tele_num_i51, i51CallInfoStruct.tele_len_i51 , NULL  ,NULL , i51CallInfoStruct.call_sim_type_i51 , 0 , NULL ) ;
		#endif

             }
        }
//	#endif
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

