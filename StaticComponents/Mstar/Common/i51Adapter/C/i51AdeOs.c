/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by osca.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#include "i51AdeCnf.h"
#include "i51AdeOs.h"
#include "ven_os.h"
#include "ven_stc.h"       /* state control */
#include "ven_handle.h"    /* SDK handle */
#include "ven_kpd.h"       /* keypad events */
#include "ven_ts.h"        /* touch events */
#include "ven_file.h"
#include "ven_util.h"
#include "ven_ui.h"
#include "ven_time.h"
#include "ven_std.h"
#include "ven_drv.h"
#include "ven_wap.h"
#include "mmi_srv_cc.h"		//make call

////////////////////////////////////////////////////////////
//#define __I51_MMI_RAI_SAVE_SN__  //默认使用MMI RAI保存SN

#ifdef __I51_MMI_RAI_SAVE_SN__
#include "mdl_vm_rai.h"
#include "mmi_common_rai_id.h"
#endif

extern iU8* i51AdePortSysLogGetString(iU16 logIndex);
extern iBOOL g_bLogFileExist;
extern iBOOL g_bTraceFileExist;
extern iU16 g_i51LogFileName[];

/*****************************************
注意:2.04.03以上版本才可以使用nvram保存sn.
使用时 1,打开宏_I51_RAI_SAVE_SN__
       2,在ven_util.h中的枚举ven_util_rai_id_category_e插入 VEN_UTIL_RAI_I51,如下:
       
    typedef enum
    {

    	//add your RAI ID category, then add	VEN_UTIL_RAI_START_ID(your RAI ID category) in front of your RAI ID enum.
    	//please reference VEN_UTIL_RAI_START_ID(VEN_UTIL_RAI_SDKTEST) in sdkven_example.c
    	VEN_UTIL_RAI_SDKTEST = 0,
     #ifdef __I51__
    	VEN_UTIL_RAI_I51,
     #endif

     #ifdef __COOL_BAR__
    	VEN_UTIL_RAI_COOLBAR,
     #endif
    }ven_util_rai_id_category_e;

******************************************/
#define __I51_RAI_SAVE_SN__

#define  I51SNBYTE        12
#ifdef __I51_RAI_SAVE_SN__
enum
{
	VEN_UTIL_RAI_START_ID(VEN_UTIL_RAI_I51),
	I51_RAI_ID_1,
	VEN_UTIL_RAI_END_ID(VEN_UTIL_RAI_I51),
};

#endif


//#define i51ADAPTER_DEBUG
//#define i51ASSERT_LOG

extern iU32 g_i51PkigHandle;

#define I51SNFILE   "i51sn\\i51sn"
#define I51SNDIR   "i51sn"

//接口开始
typedef struct MMN {

	//	author : Jelo Wang
	//	since : 20100417
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
//做查找文件的索引
static iS32 index = 0 ;
//保存当前目录文件数目
static iS32 nb_files = 0 ;
//保存打开文件的个数
static iU32 openFileCount = 0 ;
//最大能够打开文件的个数
#define MAX_FILE_OPEN_COUNT	10

//相对路径模式是否开启，默认开启
static iBOOL IsAbsolute = iFALSE ;
//当前目录
char app_path[ 128 ] ;

//  系统最新内存限制   
#ifdef __I51_APP_240X320__
#define SYS_LIMITED_MEM  (200*1024)   // 200 k
#elif defined(__I51_APP_240X400__)
#define SYS_LIMITED_MEM  (200*1024)   // 200 k
#elif defined(__I51_APP_320X480__)
#define SYS_LIMITED_MEM  (200*1024)   // 200 k
#elif defined(__I51_APP_220X176__)
#define SYS_LIMITED_MEM  (200*1024)   // 200 k
#endif

#ifndef __MMI_CALLLOG_SUPPORT_BKLOG__
#define	MMI_CALLLOG_MAX_NUM_TYPE 3
#else
#define	MMI_CALLLOG_MAX_NUM_TYPE 4
#endif
#define MMI_CALLLOG_MAX_NUM_LOGS (20)

typedef struct
{
    u8 u8Idx;
    u8 u8LogType;
} Mmi_CallLogAllLogSortingIdx_t;

typedef struct
{
    u32 u32LastCallTimeAC;
    u32 u32AllMOTimeAC;
    u32 u32AllMTTimeAC;
    u32 u32LastCallTimeACSlave;
    u32 u32AllMOTimeACSlave;
    u32 u32AllMTTimeACSlave;
    u32 au32LogFilter[MMI_CALLLOG_MAX_NUM_TYPE];
    Mmi_CallLogAllLogSortingIdx_t atAllLogsSortingIdx[MMI_CALLLOG_MAX_NUM_TYPE *MMI_CALLLOG_MAX_NUM_LOGS];
    u8 u8IdxToFile[MMI_CALLLOG_MAX_NUM_TYPE][MMI_CALLLOG_MAX_NUM_LOGS];
    u16 u16MOCount;
    u16 u16MTCount;
    u16 u16MOCountSlave;
    u16 u16MTCountSlave;
    u16 u16MissedCount;
#ifdef __MMI_CALLLOG_SUPPORT_BKLOG__
    u16									u16BlockCallCount;
#else
    u16									u16MissedCountSlave;  // no use now, just for alignment.
#endif
    /*
    u8                                  u8Align1;
    u8                                  u8Align2;
    */
} Mmi_CallLogFileHeader_t;

typedef struct
{
    // au8Number[MMI_PHB_NUMBER_LEN];
    u8 au8Number[40];
    u8 au8IMSI[15];  // 15 bytes
    u8 u8Align;
    u32 u32LatestTime;
    u8 u8MissCount;
    u8 u8NumberLen;
    u8 u8SimType;
    u8 u8LogType;
} Mmi_CallLogSimpRecord_t;

typedef struct
{
    Mmi_CallLogFileHeader_t tHeader;
    Mmi_CallLogSimpRecord_t tCallBuf[MMI_CALLLOG_MAX_NUM_TYPE][MMI_CALLLOG_MAX_NUM_LOGS];
    u8 u8CallSize[MMI_CALLLOG_MAX_NUM_TYPE];
    u8 u8ServiceStatus;
} Mmi_CallLogControlBuffer_t;

extern Mmi_CallLogControlBuffer_t g_tCallLogControlBuffer ;
 

typedef struct{
	iU32 ID;
	iBOOL loop;
	iBOOL sign;
	i51TIMERCALLBACK cb;
	iBOOL created;
	
}i51AdeOsTimerStruct;  

typedef struct{
	iU32                dwNum;     /* total files(folders) */
	iU32                dwCurr;    /* current item*/ 
	ven_file_info_t    vfi;       /* file information structure*/
	ven_dir_handle_t   hDir;      /* folder handle */
}FILE_STRUCT;

#define I51MAXTIMER  10
static i51AdeOsTimerStruct i51Timer[I51MAXTIMER]={0};

extern iU16* i51KernelGetApplicationPath () ;

static iU16 g_path[50] = {0};
static iCHAR g_fileName[12] = {0};

//振动开关
static iU32 ShockState = 0;

static i51ADECALLBACK i51AdeCallback = 0 ;

/*	Lynn Edited 20120629,  电话设备	*/
typedef struct
{
	iBOOL bDeviceCreated;
	iBOOL bMakeCall;
	iBOOL bInCall;
	
	MAE_DualMode_t nSimMode;
	MmiMakeCallReq_t tReq;
	void (*callback) ( iBOOL results);
}i51AdeOsPhoneStruct;
#define I51PHONEID 		(1)
#define I51SIMNUMMAX	(2)
static i51AdeOsPhoneStruct g_stI51Phone = {0};
/*	Lynn Edited Finished	*/

/*	Lynn Edited 20120629,  基站信息	*/
extern iU32 g_i51CellInfoZid;
extern iU32 g_i51CellInfoBsid;
extern u8 gi51IsRunning;
/*	Lynn Edited Finished	*/

/*************************************************add your function below **********************************/
static void i51S_AdeOsAssertLog ( const iCHAR* log , ... ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	va_list ap ;
	char g_printfBuf[ 256 ] ;
	ven_file_handle_t hFile = 0 ;
    char *end = "\r\n";
    iS32 ret;
    iU16 *filename=NULL;

    ven_std_memset ( g_printfBuf , 0 , sizeof( g_printfBuf ) ) ;
    va_start ( ap , log ) ;
    vsnprintf ( g_printfBuf ,255, log , ap );
    va_end( ap ) ;

#ifndef __NAND__
	if(0 == ven_std_wstrcmp(VEN_FILE_VOLUME_NAME_NOR,L"/NOR_FLASH_0"))
	{
        filename = L"/NOR_FLASH_0/i51debug.log" ;
	}
	else if(0 == ven_std_wstrcmp(VEN_FILE_VOLUME_NAME_NOR,L"/NOR0"))
	{
	    filename = L"/NOR0/i51debug.log" ;
	}
#else
        filename = L"/NAND/i51debug.log" ;
#endif 
    if(!ven_file_wexists(filename))
    {
        hFile = ( iU32 )ven_file_wfopen(filename, "w+");
    }
    else
    {
        hFile = ( iU32 )ven_file_wfopen(filename, "r+");
    }

    if (hFile == NULL)
    {
        return;
    }
    ven_file_wfseek(hFile, 0, VEN_FILE_SEEK_END);
	ven_file_wfwrite ( g_printfBuf , ven_std_strlen ( g_printfBuf ) ,hFile ) ;
	ven_file_wfwrite ( end ,ven_std_strlen(end) , hFile ) ;
	ret = ven_file_wfclose ( hFile ) ;

}

s32 i51S_fs_raiRead (u32 ident, u16 start, u16 length, void * data)
{
 	return vm_raiRead (ident, start, length, data);
}
iBOOL i51S_fs_raiWrite(u32 ident, u16 size, const void * data)
{
    return vm_raiWrite (ident, size, data);
}

static void i51AdeOsGetFileCreatedOrUpdatedTime(iU32 CreatedOrUpdatedDate, iU16 *Year, iU16 *Month, iU16 *Day, iU16 *Hour, iU16 *Minute, iU16 *Second)
{
	*Year = VEN_FILE_YEAR(CreatedOrUpdatedDate);
	*Month = VEN_FILE_MONTH(CreatedOrUpdatedDate);
	*Day = VEN_FILE_DAY(CreatedOrUpdatedDate);
	*Hour = VEN_FILE_HOURS(CreatedOrUpdatedDate) + 8;
	*Minute = VEN_FILE_MINUTES(CreatedOrUpdatedDate);
	*Second = VEN_FILE_SECONDS(CreatedOrUpdatedDate);
}

static void i51S_timerout(iU16 timerid)
{
	iU16 i;
	
	for(i=0;i<I51MAXTIMER;i++)
	{
		if(i51Timer[i].ID == timerid)
		{
			if(i51Timer[i].cb)
			{
				i51Timer[i].cb(i+1);
				return;
			}
		}
	}
}
//格式化获取当前目录
static int i51S_AdeOswsprintf ( unsigned short *buffer , const char *fmt , ... )
{
	int ilen = 0 , i = 0 ;
    char tempbuf[256] ;

    va_list ap ;
    va_start ( ap , fmt ) ;
	ven_std_vsprintf( tempbuf , fmt , ap ) ;
    va_end(ap);

	ilen = strlen ( tempbuf ) ;

	if ( ilen >= 256 )
        return -1 ;

	for( i = 0 ; i < ilen ; i++ ) {		
		if( tempbuf[i] != '\0' )
			buffer[i] = tempbuf[i] ;
        else
            break ;
	}

    buffer[i] = 0 ;

	return 0 ;
}

static iU32 MyfileAttrchange(iU16 attrValue)
{
	iU32 fileattributes = 0x0 ;

	if ( (attrValue & VEN_FILE_A_RDONLY) ==  VEN_FILE_A_RDONLY ) {
		fileattributes |= i51_OS_FL_ATTR_ROL ;
	}

	if ( (attrValue & VEN_FILE_A_HIDDEN) ==  VEN_FILE_A_HIDDEN ) {
		fileattributes |= i51_OS_FL_ATTR_HID ;	
	}

	if ( (attrValue & VEN_FILE_A_SUBDIR) == VEN_FILE_A_SUBDIR ) {
		fileattributes |= i51_OS_FL_ATTR_DIR ;
	}

	if ( (attrValue & VEN_FILE_A_ARCH) == VEN_FILE_A_ARCH ) {
		fileattributes |= i51_OS_FL_ATTR_WRT ;	
	}

	return fileattributes ;
}

//内部函数改变“\”为“/”
static void *i51S_AdeOsChang_Solidus ( const iU16* path , iU16* newpath ) 
{

    char *p;
    int  len,i;
    if (!newpath || !path) {
        return NULL;
    }

    ven_std_wstrcpy(newpath, path);
    p= (char*)newpath;
    len = ven_std_wstrlen(newpath) * 2;
    for(i=0;i<len;i+=2)
    {
        if( p[i]=='\\' && p[i+1]==0 )  p[i]='/';
    }

    return newpath;
}
//内部函数拼接相对路径
static iU16* i51S_AdeOsJoint ( iU16* newname ,const iU16* oldname ) {

	if (!oldname || !newname) {
        return NULL ;
    }

    ven_std_memset ( newname , 0 , ven_std_wstrlen( newname ) ) ;
    i51S_AdeOsChang_Solidus ( i51KernelGetApplicationPath ( ) , newname ); 

    ven_std_wstrcat ( newname , oldname ) ;
    
	return newname ;

}
iU16 *i51S_AdeFileFixPath(const iU16 *oldpath)
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    //note :外部引用需要，非标准接口
	static iU16 newpath[128];
    iU16 dir[128];
	
    ven_std_memset(newpath,0,sizeof(newpath));
    ven_std_memset(dir,0,sizeof(dir));
    i51S_AdeOsChang_Solidus ( oldpath , dir );

	if ( IsAbsolute ) 
	{
		ven_std_wstrcpy(newpath, dir);
	}
	else
	{
		i51S_AdeOsJoint ( newpath , dir );
	}
	
	return newpath;
}

//内部函数获取内存的可用大小
static iU32 i51S_AdeOsGetMemorySpace (ven_os_heap_status_t *hs)
{
	//ven_os_heap_status_t hs = {0};

	return ven_os_queryHeapStatus(hs);
	
	//return (hs.maxFreeBlock*4/5);
}



//内部函数获取路径的前一个目录
static void i51S_AdeOsGetPath ( iU16* path ){
	
	iU8 TotalSize = 0 ; 
	iU8 index = 0 ;

	TotalSize = ven_std_wstrlen ( path ) ;
	
	for ( index = TotalSize-1 ; index > 0 ; --index ) {
			if ( path [ index ] == '/' ) 
				break ;
		}
	
	path [ index ] = '\0' ;
	
}
static void* i51S_AdeOs_mem_malloc(s32 Size)
{
    void *p=NULL;
    
	p = ven_os_mem_malloc(Size);
	if(p){
        memset(p,0,sizeof(Size));
	}
	else
	{
	    ven_os_heap_status_t hs;
        i51S_AdeOsGetMemorySpace(&hs);
        i51AdeOsLog(i51_LOG_SC, "OS : i51S_AdeOs_mem_malloc Failed,size=%d,maxblock=%d,free=%d",Size,hs.maxFreeBlock,hs.freeSize);	
#ifdef i51ASSERT_LOG
        i51S_AdeOsAssertLog("OS : i51S_AdeOs_mem_malloc Failed,size=%d,maxblock=%d,free=%d",Size,hs.maxFreeBlock,hs.freeSize);
#endif   
	}

	return p;
}
static void i51S_AdeOs_mem_free(void *pData)
{
	ven_os_mem_free(pData);
    pData = NULL;
}


static void GetFileName(const iU16* pDirName)
{

	iU16 i, j = 0;
	iCHAR path[128] = {0};


	i51AdeStdUnicode2Ascii(pDirName, path);
	for ( i = strlen(path); path[i] != '/'; i--)
	{
	}

    memset(g_fileName, 0, 12 * sizeof(iCHAR));
	ven_std_memcpy(g_fileName, path + (i + 1), strlen(path) - (i + 1));
}

static void GetDirName(const iU16* pDirName)
{

	iU16 i = 0;
	iU16* path = pDirName;

	for ( i = i51AdeStdWStrlen(path); path[i] != L'/';  i--)
	{
	}

    memset(g_path, 0, 50 * sizeof(iU16));
	i51AdeStdMemcpy16(g_path, path,  i * 2);
}

static iHANDLE i51_file_findStart(const iU16* pDirName, iU16* pFindFile, iU8* pFindFileAttr, iU32* pCreatedTime, iU32* pUpdatedTime)
{
	FILE_STRUCT*  hSearch = NULL;
	iS32         iRet    = VEN_FILE_ERR_NO_ERROR;

	GetDirName(pDirName);
	GetFileName(pDirName);

	if( NULL == pDirName || NULL == pFindFile || NULL == pFindFileAttr)
	{
		return NULL;
	}

	do
	{
		hSearch = ven_os_mem_malloc(sizeof(FILE_STRUCT));

		if( NULL == hSearch)
		{
			break;
		}

		ven_std_memset(hSearch, 0, sizeof(FILE_STRUCT));

		hSearch->hDir = ven_file_wopendir(g_path);
		if( NULL == hSearch->hDir )
		{
			break;
		}

		iRet = ven_file_wdir_get_nb_files(hSearch->hDir, (iS32*)&hSearch->dwNum);
		if( VEN_FILE_ERR_NO_ERROR != iRet )
		{
			break;
		}

		if( 0 == hSearch->dwNum)
		{
			break;
		}

		iRet = ven_file_wdir_get_info(hSearch->hDir, hSearch->dwCurr, &hSearch->vfi);
		if( VEN_FILE_ERR_NO_ERROR != iRet )
		{
			break;
		}

		hSearch->dwCurr++;

		if (0 == strcmp(g_fileName, "*"))
		{
			i51AdeStdMemcpy16(pFindFile, hSearch->vfi.longFileName, i51AdeStdWStrlen(hSearch->vfi.longFileName) * 2);
			*pFindFileAttr = hSearch->vfi.attribute;
			*pCreatedTime = hSearch->vfi.createdDate;
			*pUpdatedTime = hSearch->vfi.updatedDate;
			return (iHANDLE)hSearch;
		}
	}while(FALSE);

	if( NULL != hSearch)
	{
		if( NULL != hSearch->hDir)
		{
			ven_file_wclosedir(hSearch->hDir);
		}
		ven_os_mem_free(hSearch);
	}

	return NULL;
}

static iBOOL i51_file_findNext(iHANDLE hSearch, iU16* pFindFile,  iU8* pFindFileAttr, iU32* pCreatedTime, iU32* pUpdatedTime)
{
	FILE_STRUCT* pSearch = (FILE_STRUCT*)hSearch;

	if(NULL == hSearch || NULL == pFindFile || NULL == pFindFileAttr)
	{
		return iFALSE;
	}

	if( pSearch->dwCurr == pSearch->dwNum)
	{
		return iFALSE;
	}

	if(VEN_FILE_ERR_NO_ERROR != ven_file_wdir_get_info(pSearch->hDir, pSearch->dwCurr, &pSearch->vfi))
	{
		return iFALSE;
	}

	pSearch->dwCurr++;

	if (0 == strcmp(g_fileName, "*"))
	{
		i51AdeStdMemcpy16(pFindFile, pSearch->vfi.longFileName, i51AdeStdWStrlen(pSearch->vfi.longFileName) * 2);
		*pFindFileAttr = pSearch->vfi.attribute;
		*pCreatedTime = pSearch->vfi.createdDate;
		*pUpdatedTime = pSearch->vfi.updatedDate;
		return iTRUE;
	}

	return iFALSE;
}

static iBOOL i51_file_findStop(iU32 hSearch)
{

	FILE_STRUCT* pSearch = (FILE_STRUCT*)hSearch;

	if(NULL == pSearch)
	{
		return iFALSE;
	}

	if( pSearch->hDir != NULL )
	{
		ven_file_wclosedir(pSearch->hDir);
	}

	ven_os_mem_free((iHANDLE)hSearch);

	return iTRUE;
}

//	切勿删除以下代码
#ifdef i51PROFILINGS
//#if 1
#define MMT_MASK_LENGTH 64 //1024//
unsigned char* mmt_mask = 0 ;
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 } ;



int MMTInit () {

	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK
	
	mem_monitor.file = 0 ;
	mem_monitor.line = 0 ;
	mem_monitor.length = 0 ;
	mem_monitor.address = 0 ;	
	mem_monitor.head = 0 ;		
	mem_monitor.next = 0 ;

	if ( 0 != mmt_mask )
		i51S_AdeOs_mem_free ( mmt_mask ) ;
	mmt_mask = 0 ;
		
	return 1 ;
		
}



void MMTCreateMask ()
{

	//	author : Jelo Wang
	//	since : 20110705
	//	(C)TOK

	int looper = 0 ;
	
	mmt_mask = (unsigned char* ) i51S_AdeOs_mem_malloc ( MMT_MASK_LENGTH ) ;

	for ( looper = 0 ; looper < MMT_MASK_LENGTH ; looper ++ )
	{
		mmt_mask [ looper ] = looper ;
	}

}

void MMTAdd ( char* file , int line , int length , int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* newmem = 0 ;
		
	newmem	= (MEMORY_MONITOR* ) i51S_AdeOs_mem_malloc ( sizeof(MEMORY_MONITOR) ) ;

	if ( 0 == mmt_mask ) MMTCreateMask () ;
	
	memcpy ( (void*)((unsigned int)address+length) , mmt_mask , MMT_MASK_LENGTH ) ;

	if ( file ) {
		newmem->file = (char* ) i51S_AdeOs_mem_malloc ( strlen ( file ) + 1 ) ;
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

	mask = (unsigned char* ) ven_os_mem_malloc(MMT_MASK_LENGTH );
	
	for ( looper = mem_monitor.head ; looper ;  ) 
	{

		int counter = 0 ;
		
		memcpy ( mask , (void*)((unsigned int )looper->address+looper->length) , MMT_MASK_LENGTH ) ;
		
		for ( counter = 0 ; counter < MMT_MASK_LENGTH ; counter ++ )
		{

			if ( mask [ counter ] != mmt_mask [ counter ] )
			{				
				i51AdeOsLog(i51_LOG_SC ,  "!!!!! M : %x , In : '%s' , At line : '%d' - overflowed" , looper->address , looper->file , looper->line ) ;	
				break ;
			}
		}			

		looper=looper->next ;

	}
	ven_os_mem_free ( (void*)mask ) ;
	
}


void MMTDestroy () {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	for ( walker = mem_monitor.head ; walker ; ) {
		mem_monitor.next = walker->next ;
		if ( walker->file ) i51S_AdeOs_mem_free ( walker->file ) ;
		i51S_AdeOs_mem_free ( walker ) ;
		walker = mem_monitor.next ; 
	}

	if ( 0 != mmt_mask )
		i51S_AdeOs_mem_free ( mmt_mask ) ;
	mmt_mask = 0 ;
	
}

int MMTCheckLeaked () {

	//	author : Jelo Wang
	//	since : 20091129

	int totall = 0 ;
	int leakedtimes = 0 ;
	
	MEMORY_MONITOR* walker = mem_monitor.head ;

	i51AdeOsLog(i51_LOG_SC, "i51 : Let's Check Memory Leaking :)" ) ; 
	
	for ( ;walker;walker=walker->next ) {

		i51AdeOsLog(i51_LOG_SC ,  "!!! M : %x , S : %d , In '%s' , At line : '%d' - leaked",
			walker->address,\
			walker->length,\
			walker->file,\
			walker->line\
		) ;\
	
		totall = totall + walker->length ; 

		leakedtimes ++ ;
		
	}

	if ( 0 == totall ) {
		
		i51AdeOsLog(i51_LOG_SC , "i51 : No Memory Leaking At All :)" ) ;
		
	} 
	
	MMTDestroy () ;
	MMTInit () ;
	
	return 0 ;

}

void MMTFree ( int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor.head ;
	MEMORY_MONITOR* pre = &mem_monitor ;
	
	if ( !address ) return ;

	for ( ; walker ; ) {
		
		if ( address == walker->address ) {
			if ( walker == mem_monitor.head ) {
				mem_monitor.head = walker->next ;
				mem_monitor.used = mem_monitor.used - walker->length ;
				if ( walker->file ) i51S_AdeOs_mem_free ( walker->file ) ;
				i51S_AdeOs_mem_free ( walker ) ;
				return ;
			} else {
				pre->next = walker->next ;
				if ( walker == mem_monitor.next ) {
					mem_monitor.next = pre ;
				}
				mem_monitor.used = mem_monitor.used - walker->length ;
				if ( walker->file ) i51S_AdeOs_mem_free ( walker->file ) ;
				i51S_AdeOs_mem_free ( walker ) ;
				return ;
			}
			
		}

		pre = walker ;
		walker = walker->next ;
			
	}
	
}

#endif



//	Memory System
void* i51AdeOsMalloc ( iU32 length , const iCHAR* file , iU32 line ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	ven_os_heap_status_t hs;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMalloc , S , %d , %s , %d", length, file, line);
#endif

	if(length < 4)		//动态申请不小于4字节
		length = 4;

	i51S_AdeOsGetMemorySpace(&hs);
	if ((hs.freeSize - length < SYS_LIMITED_MEM) ||(length > hs.maxFreeBlock))
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMalloc , ERR1, no empty memory , %d, %d, %d",length,hs.maxFreeBlock, hs.freeSize);
		return 0 ;
	}

#ifdef i51PROFILINGS
	{
		void* address = (void*) i51S_AdeOs_mem_malloc ( length + MMT_MASK_LENGTH ) ;
		if ( 0 == address ) {
        	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMalloc , ERR2 ,Failed" ) ;		
        	return 0 ;
        }
		memset ( address , 0 , length + MMT_MASK_LENGTH ) ;
		MMTAdd ( file , line , length , (int)address ) ;
		i51MemoryOverflowCheck () ;  
    #ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMalloc , E");
    #endif
		return address;
	} 
#else
    #ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMalloc , E");
    #endif
	return i51S_AdeOs_mem_malloc ( length ) ;
#endif
}



iBOOL i51AdeOsFree ( void* address ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFree , S , %x", address);
#endif

#ifdef i51PROFILINGS
	 MMTCheckOverflow () ; 
	 MMTFree ( (int)address ) ; 
#endif
	i51S_AdeOs_mem_free ( address ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFree , E");
#endif
	return 1 ;

}



void* i51AdeOsMallocEx ( iU32 length ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    ven_os_heap_status_t hs;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "ADE : i51AdeOsMallocEx S , %d", length);
#endif

    i51S_AdeOsGetMemorySpace(&hs);
	if ( hs.maxFreeBlock - length < SYS_LIMITED_MEM)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMallocEx , ERR1 , no empty memory , %d, %d",length,hs.maxFreeBlock);
		return 0 ;
	}

#ifdef i51PROFILINGS
	{
		// 余外申请MMT_MASK_LENGTH 个字节，用来存放溢出检测mask
		// 尾部放一串mask
		void* address = (void*) i51S_AdeOs_mem_malloc ( length + MMT_MASK_LENGTH ) ;
		if ( 0 == address ) {
        	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMalloc , ERR2, Failed" ) ;		
        	return 0 ;
        }
		memset ( address , 0 , length + MMT_MASK_LENGTH ) ;
		MMTAdd ( __FILE__ , __LINE__ , length , (int)address ) ;
		i51MemoryOverflowCheck () ;

    #ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMallocEx , E");
    #endif
		return address;
	} 
#else   
    #ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMallocEx , E");
    #endif
	 return i51S_AdeOs_mem_malloc ( length ) ;
#endif
}



iBOOL i51AdeOsFreeEx ( void* address ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFreeEx , S , %x", address);
#endif

	#ifdef i51PROFILINGS
	MMTCheckOverflow () ;
	MMTFree ( (int)address ) ;
	#endif
	
	i51S_AdeOs_mem_free( address );
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFreeEx , E");
#endif

	return iTRUE ; 

}



iU32 i51AdeOsGetMemorySpaceLeft (void) 
{

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	ven_os_heap_status_t hs = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetMemorySpaceLeft , S");
#endif

	ven_os_queryHeapStatus(&hs);
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetMemorySpaceLeft , E , %d", hs.freeSize);
#endif

	return hs.freeSize;	
}



#ifdef i51PROFILINGS

iU64 i51AdeOsGetTotallMemory () 
{
	
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_os_heap_status_t hs = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTotallMemory , S");
#endif

	ven_os_queryHeapStatus(&hs);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTotallMemory , E , %d", hs.totalSize);
#endif

	return hs.totalSize;

}
iU64 i51AdeOsGetTotallMemoryUsed () 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTotallMemoryUsed , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTotallMemoryUsed , E");
#endif

	return mem_monitor.used ;
}

#endif

iU32 i51AdeOsGetTotallMemoryLeft () 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_os_heap_status_t hs = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTotallMemoryLeft , S");
#endif

	ven_os_queryHeapStatus(&hs);
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTotallMemoryLeft , E , %d", hs.freeSize);
#endif

	return hs.freeSize;
}


//	File System
void i51AdeOsFileAModeEnable ( void ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	//开启绝对路径模式

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileAModeEnable , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileAModeEnable , E");
#endif

	IsAbsolute = iTRUE ;
}

void i51AdeOsFileAModeDisable ( void ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	//关闭绝对路径模式

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileAModeDisable , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileAModeDisable , E");
#endif

	IsAbsolute = iFALSE ;
}

iBOOL i51AdeOsFileCreateDir ( const iU16* path ) 
{

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

    iBOOL iRet = iTRUE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileCreateDir , S , %x", path);
#endif

	iRet = ven_file_wmakeDir( i51S_AdeFileFixPath(path) ) ;

	if ( ( VEN_FILE_ERR_NO_ERROR == iRet ) || ( VEN_FILE_ERR_ALREADY_EXIST == iRet ) ) {
    
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileCreateDir , E");
#endif
		return iTRUE ;
	}

	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileCreateDir , F");
    return iFALSE ;
}

iBOOL i51AdeOsFileDeleteDir ( const iU16* path ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	iBOOL iRet = iTRUE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileDeleteDir , S , %x", path);
#endif

	iRet = ven_file_wremoveDirAll( i51S_AdeFileFixPath(path) ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileDeleteDir , E , %d", iRet);
#endif

    return ( VEN_FILE_ERR_NO_ERROR == iRet ) ;
}

iBOOL i51AdeOsFileDelete ( const iU16* path ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    iBOOL iRet = iTRUE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileDelete , S , %x", path);
#endif

	iRet = ven_file_wunlink( i51S_AdeFileFixPath(path) ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : 51AdeOsFileDelete , E , %d", iRet);
#endif

    return ( VEN_FILE_ERR_NO_ERROR == iRet );
}

iBOOL i51AdeOsFileIsExist ( const iU16* path ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    iBOOL iRet = iTRUE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileIsExist , S , %x", path);
#endif

	iRet = ven_file_wexists( i51S_AdeFileFixPath(path) ) ;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileIsExist , E , %x", iRet);
#endif

 	return (iTRUE == iRet);
}

iBOOL i51AdeOsFileIsDirExist( const iU16* path ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iBOOL iRet = iTRUE ;
    iU32 attr=0;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileIsDirExist , S , %x", path);
#endif
    //iRet = ven_file_wexists( i51S_AdeFileFixPath(path) ) ;
    iRet = ven_file_wfgetAttr(i51S_AdeFileFixPath(path) , &attr);
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileIsDirExist , E , %x", iRet);
#endif

	return ( iRet == VEN_FILE_ERR_NO_ERROR && ((attr&VEN_FILE_A_SUBDIR) == VEN_FILE_A_SUBDIR) );
}

iBOOL i51AdeOsFileRename ( const iU16* name , const iU16* newname ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    iU16 name0[128];
    iU16 name1[128];
    iBOOL iRet = iTRUE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileRename , S , %x , %x", name, newname);
#endif

    ven_std_memset(name0,0,sizeof(name0));
    ven_std_memset(name1,0,sizeof(name1));

    ven_std_wstrcpy(name0, i51S_AdeFileFixPath(name));
    ven_std_wstrcpy(name1, i51S_AdeFileFixPath(newname));
    iRet = ven_file_wrename ( name0 , name1 ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileRename , E ,  %d", iRet);
#endif

    return ( iRet == VEN_FILE_ERR_NO_ERROR ) ;	
}

static int i51S_AdeOs_fs_get_size(int FileHandle, unsigned int *Size)
{
    unsigned int prevpos = (unsigned int )ven_file_wftell((void* )FileHandle);

    ven_file_wfseek((void *)FileHandle, 0, VEN_FILE_SEEK_END);
    *Size = (unsigned int )ven_file_wftell((void *)FileHandle);
    ven_file_wfseek((void *)FileHandle, prevpos, VEN_FILE_SEEK_SET);

    return 0;
}


iBOOL i51AdeOsFileCopy ( iU16* srcpath  , iU16* destpath ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    //文件复制

	iU8 *pInBuf = NULL ;
	iFILE hFilesrc = 0 ;
	iFILE hFiledest = 0 ;
	iU32 fileSize = 0 ;
	iU16 srcfile[128];
	iU16 destfile[128];
	iBOOL ret = iTRUE;
	iU32 datalen=0;

#define MAXMEMORY   50*1024

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileCopy , S , %x , %x", srcpath, destpath);
#endif

    ven_std_memset(srcfile,0,sizeof(srcfile));
    ven_std_memset(destfile,0,sizeof(destfile));
    ven_std_wstrcpy(srcfile, i51S_AdeFileFixPath(srcpath));
    ven_std_wstrcpy(destfile, i51S_AdeFileFixPath(destpath));
    
    do{
    	hFilesrc = ( iU32 )ven_file_wfopen ( srcfile, "r" ) ;
		if(hFilesrc <= 0)
		{
			ret = iFALSE;
			break;	
		}
    	i51S_AdeOs_fs_get_size(hFilesrc,&fileSize);
    	//pInBuf = ( iU8 * ) i51AdeOsMalloc ( fileSize, __FILE__ , __LINE__) ;
    	pInBuf = ( iU8 * ) i51AdeOsMalloc (MAXMEMORY , __FILE__ , __LINE__) ;
    	if(pInBuf)
    	{
    	    iU32 readt=0;
            hFiledest = ( iU32 )ven_file_wfopen ( destfile , "w" ) ;
			if (hFiledest <= 0||VEN_FILE_ERR_NO_ERROR != ven_file_wferror((ven_file_handle_t)hFilesrc))
			{
			    i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileCopy , ERR1 , create dest file fail!!");
				ret = iFALSE;
				break;
			}
    	    while(datalen <fileSize)
    	    {
                ven_std_memset ( pInBuf , 0 , MAXMEMORY ) ;
        		
        		readt = ven_file_wfread ( pInBuf , MAXMEMORY , (ven_file_handle_t)hFilesrc );
        		datalen += readt;
                ven_file_wfwrite ( pInBuf,readt , ( ven_file_handle_t )hFiledest ) ;
    	    }  
    	}
    	else
    	{
    	    i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileCopy , ERR2 , no empyt memory");
    		ret = iFALSE;
    		break;	
    	}
		
    	
    }while(0);

    if(hFilesrc){
        ven_file_wfclose((ven_file_handle_t) hFilesrc);
    }

    if(hFiledest){
        ven_file_wfclose((ven_file_handle_t) hFiledest);
    }
	
	if(pInBuf)
	{
		i51AdeOsFree ( pInBuf) ;
	}	

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileCopy , E , %d", ret);
#endif

	return ret;
}

iBOOL i51AdeOsFileMove ( iU16* srcpath  , iU16* destpath ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    //文件剪贴复制
	iBOOL iRet = iTRUE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileMove , S , %x , %x", srcpath, destpath);
#endif

	iRet = i51AdeOsFileCopy ( srcpath , destpath ) ;
	
	if ( iRet ) {			
		iRet = i51AdeOsFileDelete( srcpath ) ;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileMove , E , %d", iRet);
#endif

	return iRet ;
}

iU32 i51AdeOsFileGetAttr ( const iU16* path ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

    iU32 fileattributes = 0x0 ;
    iU32 tempattr = 0;
    iBOOL iRet = iTRUE ;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileGetAttr , S , %x", path);
#endif

	iRet = ven_file_wfgetAttr( i51S_AdeFileFixPath(path) , &tempattr ) ;
	 

	if ( VEN_FILE_ERR_INTERNAL_ERROR == iRet ) {
		return i51_OS_FL_ATTR_ERR ;
	}

	if ( (tempattr & VEN_FILE_A_RDONLY) ==  VEN_FILE_A_RDONLY ) {
		fileattributes |= i51_OS_FL_ATTR_ROL ;
	}
		
	if ( (tempattr & VEN_FILE_A_HIDDEN) ==  VEN_FILE_A_HIDDEN ) {
		fileattributes |= i51_OS_FL_ATTR_HID ;	
	}

	if ( (tempattr & VEN_FILE_A_SUBDIR) == VEN_FILE_A_SUBDIR ) {
		fileattributes |= i51_OS_FL_ATTR_DIR ;
	}

	if ( (tempattr & VEN_FILE_A_ARCH) == VEN_FILE_A_ARCH ) {
		fileattributes |= i51_OS_FL_ATTR_WRT ;	
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileGetAttr , E , %d", fileattributes);
#endif

	return fileattributes ;

}

iBOOL i51AdeOsFileSetAttr ( const iU16* path , iU32 attributes ) 
{

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iBOOL iRet = iTRUE ;
	iU32 fileattributes = 0 ;
	 
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileSetAttr , S , %x , %d", path, attributes);
#endif

	iRet = ( ( attributes & i51_OS_FL_ATTR_ROL ) == i51_OS_FL_ATTR_ROL ||
			( attributes & i51_OS_FL_ATTR_WRT ) == i51_OS_FL_ATTR_WRT ||
			( attributes & i51_OS_FL_ATTR_HID ) == i51_OS_FL_ATTR_HID ||
			( attributes & i51_OS_FL_ATTR_DIR ) == i51_OS_FL_ATTR_DIR ) ;

	if ( !iRet )
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileSetAttr , ERR1");
		return iFALSE ;
	}
    //bad param  
	if( (attributes & i51_OS_FL_ATTR_ROL) == i51_OS_FL_ATTR_ROL 
        && (attributes & i51_OS_FL_ATTR_WRT) == i51_OS_FL_ATTR_WRT ) {
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileSetAttr , ERR2");
		return iFALSE ;
	}
	
    ven_file_wfgetAttr( i51S_AdeFileFixPath(path) , &fileattributes ) ;

	if ( (attributes & i51_OS_FL_ATTR_ROL) == i51_OS_FL_ATTR_ROL ) {	
		fileattributes |= VEN_FILE_A_RDONLY ;
	}

	if ( (attributes & i51_OS_FL_ATTR_WRT) == i51_OS_FL_ATTR_WRT ) {
		fileattributes |= VEN_FILE_A_NORMAL ;
		fileattributes &= 0xfe ;
	}

	if (  (attributes & i51_OS_FL_ATTR_HID) == i51_OS_FL_ATTR_HID ) {
		fileattributes |= VEN_FILE_A_HIDDEN ;
	}

	if ( (attributes & i51_OS_FL_ATTR_DIR) == i51_OS_FL_ATTR_DIR ) {
		fileattributes |= VEN_FILE_A_SUBDIR ;
	}

	iRet = ven_file_wfsetAttr ( i51S_AdeFileFixPath(path) , fileattributes ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileSetAttr , E , %d", iRet);
#endif

	return iRet ;
	
}

iFILE i51AdeOsFileOpen ( const iU16* path , iU32 mode , const iCHAR* fl , iS32 line ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iFILE hFile = 0 ;
	int result = 0;
	//iU16 filename[128];
    iU16 *filename = NULL;   
	iU8 str[128] = {0};
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileOpen , S , %x , %d , %s , %d", path, mode, fl, line);
#endif

    if ( openFileCount >= MAX_FILE_OPEN_COUNT )
	{
#ifdef i51ADAPTER_DEBUG
	    i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileOpen ERR : FULL, %d", hFile);
#endif
        return hFile;
        
	}
		
	do{
		
		result = ( 
		           (mode & i51_OS_FL_EXISTING_OPEN) == i51_OS_FL_EXISTING_OPEN
		  		|| (mode & i51_OS_FL_ALWAYS_OPEN) == i51_OS_FL_ALWAYS_OPEN 
				|| (mode & i51_OS_FL_ALWAYS_CREATE) ==  i51_OS_FL_ALWAYS_CREATE )
			  	&& ( (mode & i51_OS_FL_READ) == i51_OS_FL_READ 
				|| (mode & i51_OS_FL_WRITE) == i51_OS_FL_WRITE); 
		  
		
		filename = (iU16*)i51S_AdeOs_mem_malloc(128);
		if ( !result  || !filename) 
		{
            break;
		}

        ven_std_memset((iU8*)filename,0,128);
		//ven_std_memset(filename,0,sizeof(filename));
		ven_std_wstrcpy ( filename , i51S_AdeFileFixPath(path) ) ;	
        if (mode == i51_OS_FL_READ ||mode == (i51_OS_FL_EXISTING_OPEN |i51_OS_FL_READ))
        {
           hFile = ( iU32 )ven_file_wfopen ( filename , "r" ) ; //只读
        }
        else if (mode == i51_OS_FL_WRITE ||mode == (i51_OS_FL_EXISTING_OPEN |i51_OS_FL_WRITE))
        {
           hFile = ( iU32 )ven_file_wfopen ( filename , "r+" ) ;
        }
        else if (mode&i51_OS_FL_ALWAYS_OPEN)
        {
            if(!ven_file_wexists(filename))
            {
                hFile = ( iU32 )ven_file_wfopen(filename, "w+");
            }
            else
            {
                if (mode&i51_OS_FL_READ)
                {
                   hFile = ( iU32 )ven_file_wfopen(filename, "r");
                } 
                else
                {
                    hFile = ( iU32 )ven_file_wfopen(filename, "r+");
                }
            }
        }
        else if (mode&i51_OS_FL_ALWAYS_CREATE)
        {
           hFile = ( iU32 )ven_file_wfopen(filename, "w+");
        }

		if ( hFile ) {
			openFileCount ++ ;
		}
        else
        {
		    i51AdeOsLog(i51_LOG_SC ,  "OS : i51AdeOsFileOpen Fail ->In : '%s' , At line : '%d',hFile = %d path = %s \r\n" , fl , line, hFile,i51AdeStdUnicode2Ascii(path,str));
        }
	}while(0);
	
    i51S_AdeOs_mem_free(filename);
 
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileOpen , E , %d", hFile);
#endif

	return hFile ;

}

iBOOL i51AdeOsFileClose ( iFILE file ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileClose , S , %d", file);
#endif

	if ( file <= 0 ) 
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileClose , ERR1 , %d", (iS32)file);
		return iFALSE ;
	}
	else
	{
		if ( VEN_FILE_ERR_NO_ERROR == ven_file_wfclose ( ( ven_file_handle_t )file ) ) 
		{
			openFileCount -- ;
#ifdef i51ADAPTER_DEBUG
			i51AdeOsLog(i51_LOG_SC, "ADE : i51AdeOsFileClose E");
#endif
			return iTRUE ;
		}
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileClose , ERR2, %d",file);
		return iFALSE ;
	}
}

iBOOL i51AdeOsFileRead ( iFILE file , void* DataPtr , iU32 Length , iU32* Read ) 
{

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	int count = 0;
	 
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileRead , S , %d , %x , %d , %x", file, DataPtr, Length, Read);
#endif

#ifdef i51ASSERT_LOG
    if(Length >=2*1024*1024 || !DataPtr)
    {
        i51S_AdeOsAssertLog("OS : i51AdeOsFileRead ,DataPtr=%x,Length =%d",DataPtr,Length);
    }
#endif
    
    count = ven_file_wfread ( DataPtr , Length , ( ven_file_handle_t )file ) ;
	if(count < 0)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileRead , F");
		return iFALSE;
	}
	else
	{
		if(Read) 
		{
			*Read = count ;
		}
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileRead , E");
#endif

 	return iTRUE;
}

iBOOL i51AdeOsFileWrite ( iFILE file , void* DataPtr , iU32 Length , iU32* Written ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	int count = 0;
	 
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWrite , S , %d , %x , %d , %x", file, DataPtr, Length, Written);
#endif

#ifdef i51ASSERT_LOG
    if(Length >2*1024*1024)//5M
    {
        i51S_AdeOsAssertLog("OS : i51AdeOsFileWrite ,DataPtr=%x,Length =%d",DataPtr,Length);
        //return;
    }
#endif
    count = ven_file_wfwrite ( DataPtr , Length , ( ven_file_handle_t )file ) ;
	if(count < 0)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWrite  , F");
		return iFALSE;
	}
	else
	{
		if(Written) 
		{
			*Written = count ;
		}
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWrite , E");
#endif

 	return iTRUE;

}

iBOOL i51AdeOsFileSetSeek ( iFILE file , iS32 offset , iS32 seek ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iS32 iRet = VEN_FILE_ERR_INTERNAL_ERROR ; 
	iU8 where;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileSetSeek , S , %d , %d , %d", file, offset, seek);
#endif
	
	switch( seek )
	{
		case i51_OS_FL_SEEK_HEAD :
			where = VEN_FILE_SEEK_SET;
			break ;
		case i51_OS_FL_SEEK_CURRENT :
			where = VEN_FILE_SEEK_CUR;
#ifdef i51ASSERT_LOG			
			if(seek >50*1024*1024)
			{
				i51S_AdeOsAssertLog("OS : i51AdeOsFileSetSeek ,seek =%d",seek);
			}
#endif
			break ;
		case i51_OS_FL_SEEK_TAIL :
			where = VEN_FILE_SEEK_END;
			break ;
		default:
			i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileSetSeek , F");
			return iFALSE;
	}
	
	iRet = ven_file_wfseek ( ( ven_file_handle_t )file , offset , where ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileSetSeek , E , %d", iRet);
#endif

	return ( VEN_FILE_ERR_NO_ERROR == iRet ) ;

}



iBOOL i51AdeOsFileGetSeek ( iFILE file , iS32* position , iS32 se ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	iU32 size=0;
	iS32 cur = ven_file_wftell ( ( ven_file_handle_t )file ) ;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileGetSeek , S , %d , %x , %d", file, position, se);
#endif

    switch( se )
	{
		case i51_OS_FL_SEEK_HEAD :
			*position = cur;
			break ;
		case i51_OS_FL_SEEK_CURRENT :
			*position = 0;
			break ;
		case i51_OS_FL_SEEK_TAIL :
		    i51S_AdeOs_fs_get_size(file, &size);
			*position = size - cur;
			break ;
		default:
			i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileSetSeek , ERR , %d",se);
			return iFALSE;
	}
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileGetSeek , E");
#endif
	return iTRUE ;
}



iBOOL i51AdeOsFileGetSize ( iFILE file , iS32* size ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileGetSize , S , %d , %x", file, size);
#endif

    if(size == NULL)
    {
        i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileGetSize , ERR , size == NULL");
        return;
    }
    i51S_AdeOs_fs_get_size(file, size);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileGetSize , E");
#endif

	return iTRUE ;
}

iBOOL i51AdeOsFileReadInt8 ( iFILE file , iU8* integer8 ){

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	//读取8Bit 数据，以适配大小端
	iU32 Read = 0 ;
	iU8 ret = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileReadInt8 , S , %d , %x", file, integer8);
#endif

	Read = ven_file_wfread ( &ret , 1 , ( ven_file_handle_t )file ) ;
    if(Read >0&&integer8)
    {
        *integer8 = ret;
    }
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileReadInt8 , E , %d", Read);
#endif

	return (0 <  Read );
}

iBOOL i51AdeOsFileReadInt16 ( iFILE file , iU16* integer16 ) {
	
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    //读取16Bit 数据，以适配大小端
	iU32 Read = 0 ;
    iU16 ret = 0;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileReadInt16 , S , %d , %x", file, integer16);
#endif

	Read = ven_file_wfread ( &ret , 2 , ( ven_file_handle_t )file ) ;
    if(Read >0&&integer16)
    {
        *integer16 = ret;
    }
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileReadInt16 , E , %d" , Read);
#endif

	return (0 <  Read );
}

iBOOL i51AdeOsFileReadInt32 ( iFILE file , iU32* integer32 ) {
	
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    //读取32Bit 数据，以适配大小端
	iU32 Read = 0 ;
	iU32 ret = 0;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileReadInt32 , S , %d , %x", file, integer32);
#endif

	Read = ven_file_wfread( &ret , 4 , ( ven_file_handle_t )file ) ;
    if(Read >0&&integer32)
    {
        *integer32 = ret;
    }
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileReadInt32 , E , %d" , Read);
#endif

	return (0 <  Read );
}

iBOOL i51AdeOsFileWriteInt8 ( iFILE file , iU8* integer8 ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 Written = 0 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWriteInt8 , S , %d , %x", file, integer8);
#endif

	Written = ven_file_wfwrite( integer8 , 1 ,( ven_file_handle_t )file ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWriteInt8 , E , %d", Written);
#endif

	return (0 <  Written );
}

iBOOL i51AdeOsFileWriteInt16 ( iFILE file , iU16* integer16 ) {
	
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 Written = 0 ;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWriteInt16 , S , %d , %x", file, integer16);
#endif

	Written = ven_file_wfwrite( integer16 , 2 ,( ven_file_handle_t )file ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWriteInt16 , E , %d", Written);
#endif

	return (0 <  Written );
}

iBOOL i51AdeOsFileWriteInt32 ( iFILE file , iU32* integer32 ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 Written = 0 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWriteInt32 , S , %d , %x", file, integer32);
#endif

	Written = ven_file_wfwrite( integer32 , 4 ,( ven_file_handle_t )file ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileWriteInt32 , E , %d", Written);
#endif

	return (0 <  Written );
}

iFILE i51AdeOsFileFind ( const iU16* path , i51FILEINFO* info ) 
{

	//	author : jone
	//	since : 2011.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd..

	FILE_STRUCT  hSearch = {0};
	iFILE ifile = 0;
	iU16 allDir[50] = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileFind , S , %x , %x", path, info);
#endif

	ven_std_memset(&hSearch, 0, sizeof(FILE_STRUCT));

	if (NULL != info)
	{
		ven_std_memset(info, 0, sizeof(i51FILEINFO));
	}

	if (NULL != path)
	{
		ifile = (iFILE)i51_file_findStart(i51S_AdeFileFixPath(path), hSearch.vfi.longFileName, &hSearch.vfi.attribute, 
			&hSearch.vfi.createdDate, &hSearch.vfi.updatedDate);

		i51AdeStdMemcpy16(info->name, hSearch.vfi.longFileName, i51AdeStdWStrlen(hSearch.vfi.longFileName) * 2);
		info->attribute = (iU16)MyfileAttrchange((iU16)hSearch.vfi.attribute);

		i51AdeOsGetFileCreatedOrUpdatedTime(hSearch.vfi.createdDate, &info->CreateTime.Year, &info->CreateTime.Month,
			&info->CreateTime.Day, &info->CreateTime.Hour, &info->CreateTime.Minute, &info->CreateTime.Second);

		i51AdeOsGetFileCreatedOrUpdatedTime(hSearch.vfi.updatedDate, &info->EditedTime.Year, &info->EditedTime.Month,
			&info->EditedTime.Day, &info->EditedTime.Hour, &info->EditedTime.Minute, &info->EditedTime.Second);
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileFind , S , %d", ifile);
#endif
	return ifile;
}

//ok
iBOOL i51AdeOsFileFindNext ( iFILE filehandle , i51FILEINFO* info )
{

	//	author : jone
	//	since : 2011.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iBOOL result = iFALSE;
	FILE_STRUCT  hSearch = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileFindNext , S , %d , %x", filehandle, info);
#endif

	ven_std_memset(&hSearch, 0, sizeof(FILE_STRUCT));

	if (NULL != info)
	{
		ven_std_memset(info, 0, sizeof(i51FILEINFO));
	}

	if (0 != filehandle)
	{
		result = i51_file_findNext((iHANDLE)filehandle, hSearch.vfi.longFileName, &hSearch.vfi.attribute,
			&hSearch.vfi.createdDate, &hSearch.vfi.updatedDate);

		i51AdeStdMemcpy16(info->name, hSearch.vfi.longFileName, i51AdeStdWStrlen(hSearch.vfi.longFileName) * 2);
		info->attribute = (iU16)MyfileAttrchange((iU16)hSearch.vfi.attribute);

		i51AdeOsGetFileCreatedOrUpdatedTime(hSearch.vfi.createdDate, &info->CreateTime.Year, &info->CreateTime.Month,
			&info->CreateTime.Day, &info->CreateTime.Hour, &info->CreateTime.Minute, &info->CreateTime.Second);

		i51AdeOsGetFileCreatedOrUpdatedTime(hSearch.vfi.updatedDate, &info->EditedTime.Year, &info->EditedTime.Month,
			&info->EditedTime.Day, &info->EditedTime.Hour, &info->EditedTime.Minute, &info->EditedTime.Second);
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileFindNext , E , %d", result);
#endif

	return result;
}

//ok
iBOOL i51AdeOsFileFindClose ( iFILE filehandle )
{

	//	author : jone
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iBOOL ret = iFALSE;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileFindClose , S , %d", filehandle);
#endif

	if(0 != filehandle)
	{
		ret = i51_file_findStop(filehandle);
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileFindClose , E , %d", ret);
#endif

	return ret;
}



iBOOL i51AdeOsFileFlush ( iFILE file ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	// 更新文件。
	//ven_os_invalidate_data_cache ( ) ;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileFlush , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileFlush , E");
#endif
	return iTRUE ;

}



#ifdef i51PROFILINGS
iU32 i51AdeOsFileTotallOpened ( ) {

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileTotallOpened , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsFileTotallOpened , E , %d" , openFileCount);
#endif
		return openFileCount ;
}
#endif

//	Timer
iU32 i51AdeOsTimerCreate ( iBOOL loop , iU32 priority )
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	//  return timerid,取值范围：[1,N]，N > 0。
	int i;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerCreate , S , %d", loop);
#endif
	
	for(i=0;i<I51MAXTIMER;i++)
	{
		if(i51Timer[i].created == 0)
		{
			i51Timer[i].created = iTRUE;
			i51Timer[i].loop = loop;
#ifdef i51ADAPTER_DEBUG
	        i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerCreate , E , %d", i+1);
#endif				
			return i+1;
		}	
	}
	
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerCreate , F ,Timer has been finished");	
	return 0;
}


iBOOL i51AdeOsTimerStart ( iU32 timerid , i51TIMERCALLBACK callback , iU32 time ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    
	iBOOL iRbool = iFALSE ;
	iU32 index = timerid - 1;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerStart , S , %d,%x,%d" , timerid, callback, time);
#endif
	
	if(timerid == 0||timerid >I51MAXTIMER)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerStart , F");
		return iFALSE;
	}

	if(i51Timer[index].created)
	{
		i51Timer[index].cb = callback;
		if(i51Timer[index].sign == 0)
		{
			i51Timer[index].ID = ven_os_createTimer ( i51S_timerout);
			i51Timer[index].sign = 1;
		}
		
		if(i51Timer[index].ID)
		{
			if ( 1 == ven_os_startTimer ( i51Timer[index].ID , i51Timer[index].loop , g_i51PkigHandle , time) )
			iRbool = iTRUE ;
		}
	}
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerStart , E , %d" , iRbool);
#endif
	
	return iRbool ;
}

iBOOL i51AdeOsTimerStop ( iU32 id ){

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 index = id - 1;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerStop S , %d", id);
#endif
	
	if(id == 0||id >I51MAXTIMER)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerStop , ERR1 , ID OUT");
		return iFALSE; 
	}

	if(i51Timer[index].created)
	{
		ven_os_stopTimer (i51Timer[index].ID) ;
		if ( 1 == ven_os_releaseTimer( i51Timer[index].ID ) ) 
		{
			i51Timer[index].created = 0;
			i51Timer[index].ID = 0;
			i51Timer[index].cb = NULL;
			i51Timer[index].sign = 0;
#ifdef i51ADAPTER_DEBUG
	        i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerStop , E");
#endif	
			return iTRUE; 
		}
	}	
	
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerStop , ERR2 , %d" , id); 
	return iFALSE ;
}



#ifdef i51PROFILINGS
iU32 i51AdeOsTimerGetLive ( ) {
	
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU8 Total = 0 ;
	iU8 i = 0 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerGetLive , S");
#endif

	for( i = 0 ; i<I51MAXTIMER ; i++ ) {

		//if ( ven_os_isTimerActive( TimerId[i] ) )
		if(i51Timer[i].created)
			Total ++ ;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsTimerGetLive , E , %d", Total);
#endif

	return Total ;
}
#endif


iU32 i51AdeOsGetSysDiskSpace ( void )
{
	//	author : jone	
	//	since : 2010.10.17	
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	// 获取系统磁盘总空间。(NAND/NOR)
	
	iU32 totalSize = 0, freeSize = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetSysDiskSpace , S");
#endif

	if (0 == strcmp(i51AdeOsGetPathSymbol(), "\\NAND\\"))
	{
		ven_file_wgetVolumeSpaceInfo ( VEN_FILE_DEVICE_TYPE_NAND , &totalSize , &freeSize ) ;
	}
	else
	{
		ven_file_wgetVolumeSpaceInfo ( VEN_FILE_DEVICE_TYPE_NOR , &totalSize , &freeSize ) ;
	}
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetSysDiskSpace , E");
#endif

	return totalSize ;
}

iU32 i51AdeOsGetSysDiskSpaceLeft ( void )
{
	
	//	author : jone	
	//	since : 2010.10.17	
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	// 获取系统磁盘剩余空间。(NAND/NOR)
	
	iU32 totalSize = 0, freeSize = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetSysDiskSpaceLeft , S");
#endif

	if (0 == strcmp(i51AdeOsGetPathSymbol(), "\\NAND\\"))
	{
		ven_file_wgetVolumeSpaceInfo ( VEN_FILE_DEVICE_TYPE_NAND , &totalSize , &freeSize ) ;
	}
	else
	{
		ven_file_wgetVolumeSpaceInfo ( VEN_FILE_DEVICE_TYPE_NOR , &totalSize , &freeSize ) ;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetSysDiskSpaceLeft , E");
#endif

	return freeSize;
}


iU32 i51AdeOsGetDiskSpace ( )
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	// 获取磁盘总空间。(T卡)

	iU32 totalSize = 0, freeSize = 0;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetDiskSpace , S");
#endif

	ven_file_wgetVolumeSpaceInfo ( VEN_FILE_DEVICE_TYPE_SD , &totalSize , &freeSize ) ;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetDiskSpace , E");
#endif

	return totalSize;
}



iU32 i51AdeOsGetDiskSpaceLeft ( )
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	// 获取磁盘剩余空间。(T卡)

	iU32 totalSize = 0, freeSize = 0;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetDiskSpaceLeft , S");
#endif

	ven_file_wgetVolumeSpaceInfo ( VEN_FILE_DEVICE_TYPE_SD , &totalSize , &freeSize ) ;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetDiskSpaceLeft , E");
#endif

	return freeSize;
}



iU32 i51AdeOsGetCallingTime ( ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	//获取通话时长。单位：秒

	iU32 nCallTime = 0;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetCallingTime , S");
#endif

	nCallTime = ( g_tCallLogControlBuffer.tHeader.u32AllMOTimeAC + 
			g_tCallLogControlBuffer.tHeader.u32AllMOTimeACSlave + 
			g_tCallLogControlBuffer.tHeader.u32AllMTTimeAC + 
			g_tCallLogControlBuffer.tHeader.u32AllMTTimeACSlave ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetCallingTime , E , %d" , nCallTime);
#endif

	return nCallTime;
}



iU32 i51AdeOsGetNetFlow ( ) 
{

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	//  获取网络流量。
	//  not suport

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetNetFlow , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetNetFlow , E");
#endif
	return 0 ;
}



iU32 i51AdeOsGetTick ( ) 
{

	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	//  获取CPU时钟。单位：毫秒

    iU32 ntick = 0;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTick , S");
#endif

    ntick = ( iU32 )( 4.615*ven_os_getOSTick () );

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTick , E , %d", ntick);
#endif
    
	return ntick ;

}



iBOOL i51AdeOsGetTime ( iU16* year , iU16* month , iU16* day , iU16* hour , iU16* mins , iU16* sec ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_time_ret_t venTimeRet ;
	ven_time_format_t localTime = { 0 } ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTime , S , %d,%d,%d,%d,%d,%d" , year, month, day, hour, mins, sec);
#endif

	venTimeRet = ven_time_getLocalTime( &localTime ) ;
	
	if( VEN_TIME_RET_SUCCESS == venTimeRet ) {
		*year = localTime.year ;
		*month = localTime.month ;
		*day = localTime.day ;
		*hour = localTime.hour ;
		*mins = localTime.minute ;
		*sec = localTime.second ;
	
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTime , E");
#endif

		return iTRUE ;
	}

	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetTime , F");

	return iFALSE ;
}



iBOOL i51AdeOsSetTime ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_time_ret_t venTimeRet = 0;
	ven_time_format_t localTime = { 0 } ;
	ven_req_data_t reqData = { 0 } ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetTime , S , %d,%d,%d,%d,%d,%d" , year, month, day, hour, mins, sec);
#endif

	reqData.Handle = g_i51PkigHandle ;
    reqData.bSync = iTRUE ;
   
	localTime.year = year ;
    localTime.month = month ;
    localTime.day = day ;
	localTime.dayOfWeek = 2 ;
    localTime.hour = hour ;
    localTime.minute = mins ;
    localTime.second = sec ;
    localTime.secondFracPart = 0 ;

    venTimeRet = ven_time_setLocalTime( &localTime , &reqData ) ;

	if ( VEN_TIME_RET_SUCCESS == venTimeRet ) 
	{
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetTime , E");
#endif
		return iTRUE ;
	}

	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetTime , F");
	return iFALSE ;
}




iU64 i51AdeOsGetSeconds ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 nSeconds = 0 ;
	ven_time_format_t utcTime = { 0 } ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetSeconds , S , %d,%d,%d,%d,%d,%d" , year, month, day, hour, mins, sec);
#endif

	utcTime.year = year ;
	utcTime.month = month ;
	utcTime.day = day ;
	utcTime.dayOfWeek = 1 ;
	utcTime.hour = hour ;
	utcTime.minute = mins ;
	utcTime.second = sec ;

	ven_time_getUTCTimeInSeconds( &utcTime , &nSeconds ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetSeconds , E , %d", nSeconds);
#endif

	return nSeconds ;

}



iBOOL i51AdeOsSecond2Time ( iU64 TotallSeconds , iU16 *year , iU16 *month , iU16 *day , iU16 *hour , iU16 *mins , iU16 *sec ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_time_ret_t iRetime = 0;
	ven_time_format_t utcTime = { 0 } ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSecond2Time , S , %d,%x,%x,%x,%x,%x,%x" , TotallSeconds, year, month, day, hour, mins, sec);
#endif

	iRetime = ven_time_getUTCTimeFromSeconds(TotallSeconds,&utcTime );

	if ( VEN_TIME_RET_SUCCESS == iRetime ) {
		
		*year = utcTime.year ;
		*month = utcTime.month ;
		*day = utcTime.day ;
		*hour = utcTime.hour ;
		*mins = utcTime.minute ;
		*sec = utcTime.second ;
		
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSecond2Time , E");
#endif

		return iTRUE ;
	}

	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSecond2Time , F");

	return  iFALSE ;
}



iBOOL i51AdeOsSleep ( iU32 ms ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSleep , S , %d", ms);
#endif

	ven_os_sleep ( ms ) ;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSleep , E");
#endif

	return iTRUE ;

}

#include "ven_dbg.h"
#define _SDKTEST             VEN_DBG_GROUP_SDK_1
#define _SDKTEST_DBGLVL_OS         _SDKTEST | VEN_DBG_LEVEL_8 

void i51AdeOsLog ( iS32 module , const iCHAR* log , ... ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	va_list ap ;
	char g_printfBuf[ 256 ] ;
	ven_file_handle_t hFile = 0 ;
    char *end = "\r\n";
    char *mod_dc="[51PK] DC : ";
    char *mod_sc="[51PK] SC : ";
    iS32 ret;
    iU16 *logname=NULL;
        
    ven_std_memset ( g_printfBuf , 0 , sizeof( g_printfBuf ) ) ;
    va_start ( ap , log ) ;
    if(module == i51_LOG_DC)
    {
        ven_std_strcpy(g_printfBuf,mod_dc);
        vsnprintf ( g_printfBuf+strlen(mod_dc) ,255-strlen(mod_dc), log , ap );
    }
    else
    {
		ven_std_strcpy(g_printfBuf,mod_sc);
        vsnprintf ( g_printfBuf+strlen(mod_sc) ,255-strlen(mod_sc), log , ap );
    }
    va_end( ap ) ;

	if(g_bTraceFileExist)
	{
		ven_dbg_trace(_SDKTEST_DBGLVL_OS, g_printfBuf);
	}
    
    //if ( ven_file_wexists ( I51LOGFILE) ) 
    if(g_bLogFileExist)
	{
        hFile = ven_file_wfopen ( g_i51LogFileName  , "r+" ) ;
        if (hFile == NULL)
        {
            return;
        }
        ven_file_wfseek(hFile, 0, VEN_FILE_SEEK_END);
		ven_file_wfwrite ( g_printfBuf , ven_std_strlen ( g_printfBuf ) ,hFile ) ;
		ven_file_wfwrite ( end ,ven_std_strlen(end) , hFile ) ;
		ret = ven_file_wfclose ( hFile ) ;
		//ven_os_invalidate_data_cache ( ) ;
	}

}
void i51AdeOsLogEx ( iS32 module , iU8 *buf , iU32 buflen )
{
    //	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_file_handle_t hFile = 0 ;
    char *end = "\r\n";
    iS32 ret;

	hFile = ven_file_wfopen ( g_i51LogFileName  , "r+" ) ;
    
    if (hFile == NULL)
    {
        return;
    }
    ven_file_wfseek(hFile, 0, VEN_FILE_SEEK_END);
	ven_file_wfwrite ( buf , buflen ,hFile ) ;
	ven_file_wfwrite ( end ,ven_std_strlen(end) , hFile ) ;
	ret = ven_file_wfclose ( hFile ) ;


}
void i51AdeOsSysLog(iU16 logIndex , ... )
{
	iU8* log = 0;
	va_list ap ;
	char g_printfBuf[ 256 ] ;
	ven_file_handle_t hFile = 0 ;
    char *end = "\r\n";
    char *mod_dc="[SYS_LOG]";
    char *mod_sc="[51PK] SC : ";
    iS32 ret;
    iU16 *logname=NULL;

	log = i51AdePortSysLogGetString(logIndex);
	if(!log) 
	{
		return;
	}
        
    ven_std_memset ( g_printfBuf , 0 , sizeof( g_printfBuf ) ) ;
    va_start ( ap , logIndex) ;
    ven_std_strcpy(g_printfBuf,mod_dc);
    vsnprintf ( g_printfBuf+strlen(mod_dc) ,255-strlen(mod_dc), log , ap );
    va_end( ap ) ;

	if(g_bTraceFileExist)
	{
		ven_dbg_trace(_SDKTEST_DBGLVL_OS, g_printfBuf);
	}
    
    if(g_bLogFileExist)
	{
        hFile = ven_file_wfopen ( g_i51LogFileName  , "r+" ) ;
        if (hFile == NULL)
        {
            return;
        }
        ven_file_wfseek(hFile, 0, VEN_FILE_SEEK_END);
		ven_file_wfwrite ( g_printfBuf , ven_std_strlen ( g_printfBuf ) ,hFile ) ;
		ven_file_wfwrite ( end ,ven_std_strlen(end) , hFile ) ;
		ret = ven_file_wfclose ( hFile ) ;
		//ven_os_invalidate_data_cache ( ) ;
	}
}
iBOOL i51AdeOsGetBaseStation ( iS32 simCard , iU32* bsid , iU32* zid ) 
{  
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	
	ven_util_CellInfoRsp_t CellInfoRsp = { 0 } ;
	ven_util_get_cell_info_t sim = {0};
	ven_req_data_t tVenReq = { 0 } ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetBaseStation , S , %d , %x , %x", simCard, bsid, zid);
#endif

	if(!gi51IsRunning)	//未启动时返回在Idle获取的Cell Info，规避Vendor App未运行时Ven接口调用失败问题
	{
		*bsid = g_i51CellInfoZid;
		*zid = g_i51CellInfoBsid;
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetBaseStation , ERR1 , i51 is stop , %d,%d", g_i51CellInfoZid, g_i51CellInfoBsid);
		return iTRUE;
	}


    if(simCard == 1||simCard == 2)
    {
        sim.nSimID = simCard-1;
    }
    else
    {
        i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetBaseStation , ERR2 , %d" , simCard);
        return iFALSE;
    }

	tVenReq.Handle = g_i51PkigHandle ;
	tVenReq.bSync = iTRUE ;

	if ( VEN_UTIL_RET_SUCCESS == ven_util_GetCellInfo( &sim , &CellInfoRsp , &tVenReq ) ) 
	{
		if(CellInfoRsp.Serving.Lac[0] == 0xff||CellInfoRsp.Serving.Lac[0] == 0
			||CellInfoRsp.Serving.CellId[0] == 0xff||CellInfoRsp.Serving.CellId[0] == 0)
		{
			i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetBaseStation , ERR3");
			return iFALSE;
		}
		else
		{
			*zid = CellInfoRsp.Serving.Lac[1]+CellInfoRsp.Serving.Lac[0]*256;
			*bsid = CellInfoRsp.Serving.CellId[1]+CellInfoRsp.Serving.CellId[0]*256;
		}
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetBaseStation , E");
#endif

	return iTRUE ;

}

iBOOL i51AdeOsSetAccelerator ( i51ACCCALLBACK callback ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetAccelerator , S , %x", callback);
#endif

	if(NULL == callback)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetAccelerator , F");
		return iFALSE;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetAccelerator , E");
#endif

	return iFALSE ;
}

iBOOL i51AdeOsSetShock ( iBOOL open ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iBOOL iRbool = 0 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetShock , S , %d", open);
#endif

    if( open )
    {
        ShockState = 1;//震动开启
		iRbool = iTRUE;
    }
    else
    {
        ShockState = 0;//震动关闭
        iRbool = ven_drv_stopVibration ();
    }
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetShock , E , %d", iRbool);
#endif

	return iRbool ;
}



iBOOL i51AdeOsSetScreenLight ( iBOOL open )
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iBOOL iRet = iFALSE;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetScreenLight , S , %d", open);
#endif

	if( open )
		iRet = ven_drv_setBacklight(10, 10);
	else
		iRet = ven_drv_setBacklight(10, 0);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetScreenLight , E , %d", iRet);
#endif

	return (iRet == 1) ;
}

iBOOL i51AdeOsScreenAlwaysLight ( iBOOL always )
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_ui_ret_t ret = VEN_UI_RET_FAILED ;
	ven_req_data_t tVenReq = { 0 } ;
	tVenReq.Handle = g_i51PkigHandle ;
	tVenReq.bSync = iTRUE ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsScreenAlwaysLight , S , %d", always);
#endif

	if( always )
		ret = ven_ui_setBacklightBehavior( VEN_UI_BACKLIGHT_ON , &tVenReq ) ;
	else
		ret = ven_ui_setBacklightBehavior( VEN_UI_BACKLIGHT_FORCE_AUTO , &tVenReq ) ;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsScreenAlwaysLight , E , %d", ret);
#endif

	return ( VEN_UI_RET_SUCCESS == ret ) ;
}

iCHAR* i51AdeOsGetPathSymbol () 
{

	//	author : osca
	//	since : 20111011
	//	PKIG Tech. Co., Ltd.

	//	返回系统盘盘符。(统一为MTK格式)
	static iCHAR Disk[32]={0};
	iS32 ret;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetPathSymbol , S");
#endif
    memset(Disk,0,sizeof(Disk));
    

    #ifndef __NAND__
	if(0 == ven_std_wstrcmp(VEN_FILE_VOLUME_NAME_NOR,L"/NOR_FLASH_0"))
	{
        ven_std_strcpy(Disk,"\\NOR_FLASH_0\\");
	}
	else if(0 == ven_std_wstrcmp(VEN_FILE_VOLUME_NAME_NOR,L"/NOR0"))
	{
        ven_std_strcpy(Disk,"\\NOR0\\");
	}
	#else
	if(0 == ven_std_wstrcmp(VEN_FILE_VOLUME_NAME_NAND,L"/NAND"))
	{
        ven_std_strcpy(Disk,"\\NAND\\");
	}
    #endif
   	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetPathSymbol E , %s", Disk);
#endif

	return Disk ;
	
}


iCHAR* i51AdeOsGetPathSymbolEx() 
{
	//	author : osca
	//	since : 20111011
	//	PKIG Tech. Co., Ltd.

	//	返回存储盘盘符。
	
	static iCHAR *Disk = "\\CARD\\" ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetPathSymbolEx , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetPathSymbolEx , E , %s", Disk);
#endif

	return Disk ;
 

}
iBOOL i51AdeOsMMemCardExist () 
{
	//	author : william
	//	since : 20111011
	//	PKIG Tech. Co., Ltd.

	//	判断存储卡是否存在

	iU32 totalSize = 0, freeSize = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMMemCardExist , S");
#endif
	
	ven_file_wgetVolumeSpaceInfo ( VEN_FILE_DEVICE_TYPE_SD , &totalSize , &freeSize ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsMMemCardExist , E");
#endif

	return totalSize >0 ;

}

static void i51S_AdeOsCreateCB ( iU32 timerid )
{

    if(i51AdeCallback)
    {
        i51AdeCallback(1);
    }
    i51AdeOsTimerStop ( timerid ) ;
    
}
iBOOL i51AdeOsCreate ( i51ADECALLBACK callback ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    iU32 timerid;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsCreate , S");
#endif

	if (NULL == callback)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsCreate , F");
		return iFALSE;
	}

    
    i51AdeCallback = callback ;
    timerid = i51AdeOsTimerCreate ( 0 , 0) ;
	i51AdeOsTimerStart ( timerid, i51S_AdeOsCreateCB, 20);
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsCreate , E");
#endif

	return iTRUE ;

}

iBOOL i51AdeOsRelease ( ) 
{
	//	author : osca
	//	since : 2010.10.17
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsRelease , S");
#endif

    i51AdeCallback = 0;
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsRelease , E");
#endif

	return iTRUE ;

}
iBOOL i51AdeOsSetCPUCache (iU32 start_addr, iU32 end_addr)
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetCPUCache , S , %d,%d" , start_addr, end_addr);
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetCPUCache , E");
#endif

    return iFALSE;
}
iBOOL i51AdeOsUnsetCPUCache () 
{

    return iFALSE;
}


iBOOL  i51AdeOsPermanitBufferRead ( iU8* buffer , iU32 length )
{
	iFILE hFile = 0;
	iU32 readbyte;
	iBOOL ret=iTRUE;
	iU16 filepath[64];
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferRead , S , %x,%d" , buffer, length);
#endif

    if (length > I51SNBYTE||length == 0)
    {
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferRead , ERR1 , %d" , length);
        return iFALSE;
    }
    
#if defined(__I51_MMI_RAI_SAVE_SN__)
    ret = i51S_fs_raiRead (MMI_FS_RAI_ITEM_I51SN, 0, length, buffer);    
	if(ret == 0)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferRead , ERR2 , ret = 0");
		return iFALSE;
	}
	else
	{
		return iTRUE;
	}
#elif defined(__I51_RAI_SAVE_SN__)

	ret = ven_util_raiRead(I51_RAI_ID_1, 0, length, buffer);
	if(ret == 0)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferRead , ERR3 , ret = 0");
		return iFALSE;
	}
	else
	{
		return iTRUE;
	}
#else
    do{

        i51AdeOsFileAModeEnable () ;
        i51AdeStdWSprintf(filepath,sizeof(filepath),"%s%s",i51AdeOsGetPathSymbol(),I51SNFILE);
    	hFile = i51AdeOsFileOpen(filepath, i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__, __LINE__);
        if(hFile == 0) 
        {
            i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferRead , ERR4");
            ret = iFALSE;  
            break;
        }

        ret = i51AdeOsFileRead(hFile, buffer, length, &readbyte);
        i51AdeOsFileClose(hFile);
        
     }while(0);

     i51AdeOsFileAModeDisable () ;

    if(ret == iFALSE)
    {
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferRead , ERR5");
    }
    else
    { 
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : 51AdeOsPermanitBufferRead , E");
#endif
    }   
    return ret;
    
#endif /*__I51_MMI_RAI_SAVE_SN__*/
}
iBOOL i51AdeOsPermanitBufferWrite ( iU8* buffer , iU32 length )
{

	iFILE hFile = 0;
	iU32 writebyte;
	iBOOL ret = iTRUE;
	iU32 attr = 0;
    iU16 filepath[64],sndir[64];

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , S , %s, %d", buffer, length);
#endif

    if (length > I51SNBYTE)
    {
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , ERR1 , %d" , length);
        return iFALSE;
    }

#if defined(__I51_MMI_RAI_SAVE_SN__)
    ret = i51S_fs_raiWrite(MMI_FS_RAI_ITEM_I51SN, length, buffer);
	if(ret == 1)
	{
	#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , E");
    #endif
        return iTRUE;
	}
	else
    {
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , ERR2 , %d", ret);
        return iFALSE;
    }
#elif defined(__I51_RAI_SAVE_SN__)
	ret = ven_util_raiWrite(I51_RAI_ID_1, length, buffer);
	if(ret == VEN_UTIL_RET_SUCCESS)
	{
	#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite ,  E");
    #endif
        return iTRUE;
	}
	else
    {
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , ERR3 , %d",ret);
	return iFALSE;
    }
#else  //手机系统盘保存SN
    do{
        i51AdeOsFileAModeEnable () ;
        
        //创建文件夹
        i51AdeStdWSprintf(sndir,sizeof(sndir),"%s%s",i51AdeOsGetPathSymbol(),I51SNDIR);
        if ( 0 == i51AdeOsFileIsDirExist ( sndir ) )
        {
            if ( 0 < i51AdeOsFileCreateDir ( sndir ) ) 
            {			
    			i51AdeOsFileSetAttr ( sndir , i51_OS_FL_ATTR_DIR | i51_OS_FL_ATTR_HID ) ;			
    		} 
    		else 
    		{
    			i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , ERR4 , Folder to create failure");
	                ret = iFALSE;
	                break;
    		}

        }
        //创建sn文件
        i51AdeStdWSprintf(filepath,sizeof(filepath),"%s%s",i51AdeOsGetPathSymbol(),I51SNFILE);
    	hFile = i51AdeOsFileOpen(filepath, i51_OS_FL_WRITE|i51_OS_FL_ALWAYS_CREATE, __FILE__, __LINE__);
        if(hFile<0) 
        {
            i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , ERR5 , File to create failure");
            ret = iFALSE;
            break;  
        }
        
        ret = i51AdeOsFileWrite(hFile, buffer, length, &writebyte);
        i51AdeOsFileClose(hFile);
        
        attr = i51AdeOsFileGetAttr ( filepath );
        i51AdeOsFileSetAttr(filepath, attr|i51_OS_FL_ATTR_HID);
        
    }while(0);

    i51AdeOsFileAModeDisable () ;
    
    if(ret == iFALSE)
    {
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , F , %d", writebyte);
        return iFALSE;
    }
    else
    { 
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsPermanitBufferWrite , E");
#endif
        return iTRUE;
    }   

#endif /*__I51_MMI_RAI_SAVE_SN__*/

}

iU32 i51AdeOsCreatePhone () 
{
	//	author : Lynn
	//	since : 2012.6.29
	//	(C) 2012 , PKIG Tech. Co., Ltd.
	if(g_stI51Phone.bDeviceCreated)
	{
		return 0;
	}
	else
	{
		g_stI51Phone.bDeviceCreated = iTRUE;
		g_stI51Phone.bInCall = iFALSE;
		return I51PHONEID;
	}
}

static void _i51AdeOsStartPhoneCallCb(void *pThis, boolean bResult, u32 nCause)
{	
	//	author : Lynn
	//	since : 2012.6.29
	//	(C) 2012 , PKIG Tech. Co., Ltd.
	if(bResult)
	{
		g_stI51Phone.bInCall = iTRUE;
	}
	g_stI51Phone.callback(bResult);
}

iBOOL i51AdeOsStartPhoneCall ( iU32 phoneid , iCHAR* number , void (*callback) ( iBOOL results) ) 
{
	//	author : Lynn
	//	since : 2012.6.29
	//	(C) 2012 , PKIG Tech. Co., Ltd.

	CCSRV_MO_ERROR_CODE ret = CCSRV_MO_NO_ERROR;
	CCSRV_DUO_STATUS_e eCallStatus = CCSRV_STATUS_UNKNOW;
	ven_Imsi_t  imsi_str = {0};
	MAE_DualMode_t nSimMode = 0;
	iBOOL bSimCardExist = iFALSE;
	iS32 *SimCardInfo = NULL ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , S , %d,%s,%x" , phoneid, number, callback);
#endif

	if(g_stI51Phone.bDeviceCreated)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , ERR1 , no create phone device");	
		return iFALSE;
	}

	if(phoneid != I51PHONEID)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , ERR2 , ID OUT");		
		return iFALSE;
	}

	if(g_stI51Phone.bInCall)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , ERR3");			
		return iFALSE;
	}
	
	//获取SIM卡通话状态
	eCallStatus = SrvCcGetDuoCallStatus();
	if(eCallStatus == CCSRV_MASTER_CALL_EXISTED || eCallStatus == CCSRV_SLAVE_CALL_EXISTED)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , ERR4");				
		return iFALSE;
	}
	
	//判断SIM卡是否存在 
	SimCardInfo = i51AdeSmsGetSims();
	if(SimCardInfo[0] <= 0)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , ERR5");					
		return iFALSE;
	}
	if(SimCardInfo[1] == 1)
	{
		nSimMode = DUAL_MASTER;
	}
	else if(SimCardInfo[1] == 2)
	{
		nSimMode = DUAL_SLAVE;
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , ERR6");						
		return iFALSE;
	}
	
	//拨打电话
	ven_std_memset(&g_stI51Phone.tReq, 0, sizeof(MmiMakeCallReq_t));
	ven_std_strcpy(&g_stI51Phone.tReq.CallNumber[0], number);
	ret = SrvCcMakeCallReq(&g_stI51Phone.tReq, nSimMode, _i51AdeOsStartPhoneCallCb);
	
	if(ret == CCSRV_MO_NO_ERROR)
	{
		g_stI51Phone.callback = callback;
		g_stI51Phone.nSimMode = nSimMode;
		g_stI51Phone.bMakeCall = iTRUE;
		g_stI51Phone.bInCall = iFALSE;
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , E");
#endif		
		return iTRUE;
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStartPhoneCall , F");							
		return iFALSE;
	}
	
}

static void _i51AdeHangUpCallCnfCb( void *pThis,boolean bResult,u32 nCause )
{}

iBOOL i51AdeOsStopPhoneCall ( iU32 phoneid ) 
{
	//	author : Lynn
	//	since : 2012.6.29
	//	(C) 2012 , PKIG Tech. Co., Ltd.
	
	CCSRV_DUO_STATUS_e eCallStatus = CCSRV_STATUS_UNKNOW;
	MAE_Ret nRet = MAE_RET_SUCCESS;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStopPhoneCall , S , %d" , phoneid);
#endif
	
	if(g_stI51Phone.bDeviceCreated 
		|| g_stI51Phone.bMakeCall == iFALSE
		|| phoneid != I51PHONEID)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStopPhoneCall , ERR1");	
		return iFALSE;
	}

	eCallStatus = SrvCcGetDuoCallStatus();
	if(eCallStatus == CCSRV_NO_CALL || eCallStatus == CCSRV_STATUS_UNKNOW)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStopPhoneCall , ERR2");		
		return iFALSE;
	}
	if(g_stI51Phone.nSimMode == DUAL_MASTER && eCallStatus != CCSRV_MASTER_CALL_EXISTED)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStopPhoneCall , ERR3");		
		return iFALSE;
	}
	if(g_stI51Phone.nSimMode == DUAL_SLAVE && eCallStatus != CCSRV_SLAVE_CALL_EXISTED)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStopPhoneCall , ERR4");			
		return iFALSE;
	}

	if(g_stI51Phone.bInCall == iFALSE)
	{
		nRet = SrvCcHangUpOutgoingCallReq(_i51AdeHangUpCallCnfCb);
	}
	else
	{
		nRet = SrvCcHangUpCallReq(_i51AdeHangUpCallCnfCb);
	}

	if(MAE_RET_SUCCESS == nRet)
	{
		g_stI51Phone.bMakeCall = iFALSE;
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStopPhoneCall , E");
#endif		
		return iTRUE;
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsStopPhoneCall , ERR5");				
		return iFALSE;
	}
	
}

iBOOL i51AdeOsReleasePhone ( iU32 phoneid ) 
{
	//	author : Lynn
	//	since : 2012.6.29
	//	(C) 2012 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsReleasePhone , S , %d" , phoneid);
#endif

	if(!g_stI51Phone.bDeviceCreated)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsReleasePhone , ERR , Phone Device not create");	
		return iFALSE;
	}

	if(g_stI51Phone.bMakeCall == iFALSE || g_stI51Phone.bInCall == iFALSE)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsReleasePhone , F");		
		return iFALSE;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsReleasePhone , E");
#endif
	
	return iTRUE;
}

//ok
//时间以毫秒为单位
iBOOL i51AdeOsDoShock (iU32 iTime)
{

	//	author : jone
	//	since : 2011.9.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iS32 result = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsDoShock , S");
#endif

    if(1 == ShockState)
    {
        result = ven_drv_startVibration(iTime);
    }
    else
    {
        i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsDoShock , ERR , Vibration not open");
        return iFALSE;
    }

	if (1 != result)
	{
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsDoShock , F");
		return iFALSE;
	}

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsDoShock , E");
#endif
    
	return iTRUE;
}

//ok
iBOOL i51AdeOsGetScreenLight (void)
{

	//	author : jone
	//	since : 2011.9.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iS32 result = 0;
	ven_DrvDevStatus_e Status = VEN_DRV_BACKLIGHT_STATUS_UNKNOWN;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetScreenLight , S");
#endif

	result =  ven_drv_getBacklightStatus(&Status);
	if (1 == result)
	{
		if (VEN_DRV_BACKLIGHT_STATUS_ON == Status)
		{
#ifdef i51ADAPTER_DEBUG
			i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetScreenLight , E , status == ON");
#endif
			return iTRUE;
		}
		else
		{
#ifdef i51ADAPTER_DEBUG
			i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetScreenLight , E , status == OFF");
#endif
			return iFALSE;
		}
	}

	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsGetScreenLight , F");

	return iFALSE;
}

iBOOL i51AdeOsRunWapExplorer ( iU16* url )
{
    ven_req_data_t ReqData = {0};
    ven_wap_open_browser_info_t tOpenBrowser = {0};
    ven_wap_ret_t nWapRet = VEN_WAP_RET_SUCCESS;
    char dest[256];

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsRunWapExplorer , S , %s" , i51AdeStdUnicode2Ascii(url, dest));
#endif
    
    tOpenBrowser.pURL = url;
    tOpenBrowser.nURLLen = ven_std_wstrlen(url);

    ReqData.Handle = g_i51PkigHandle;
    ReqData.bSync = TRUE;
    ReqData.pfnCb = NULL;
    nWapRet = ven_wap_open_browser(&tOpenBrowser, &ReqData);

#ifdef i51ADAPTER_DEBUG    
    i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsRunWapExplorer , E , %d" , nWapRet);
#endif
    
    return (nWapRet == VEN_WAP_RET_SUCCESS);
    
}

iBOOL i51AdeOsSetBrowserPage ( iCHAR* homepage )
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetBrowserPage , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "OS : i51AdeOsSetBrowserPage , E");
#endif

	return iTRUE;
}
