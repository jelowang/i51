  
/*

	The Implementing of 'Adapter' , Module : 'OS'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Api.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/
#include "i51Scripts.h"
#include "i51AdeCnf.h"
#include "i51AdeMmi.h"
#include "i51AdeOs.h"
#include "i51Kernel.h"
#include "kal_release.h"
#include "mmi_include.h"
#include "ProtocolEvents.h"
#include "med_utility.h"
#include "fs_func.h"
#include "TimerEvents.h"
#include "layer4_context.h"
#if( I51_MTK_VS >= VS_11B_1212 )
#include "rmmi_context.h"
#endif
#include "app_datetime.h"
#include "gdi_internal.h"
#if( I51_MTK_VS>=VS_09B )
#include "..\..\plutommi\Service\Gsm3gCallSrv\gcallsrvprot.h"
#include "..\..\plutommi\Service\Gsm3gCallSrv\gcallsrvdef.h"
#endif
#include "aud_defs.h"
#include "msf_cfg.h"
#include "conversions.h"
#include "BrowserGprots.h"
#if( I51_MTK_VS<=VS_0812 )
#include "wapadp.h"
#else
#include "browser_api.h"
#endif
#if( I51_MTK_VS>=VS_W10 )
#include "GpioSrvGprot.h"
#include "FileMgrSrvGProt.h"
#include "mmi_rp_app_i51_def.h"
#include "CallLogSrvGprot.h"
#include "CallLogSrvIprot.h"
#include "browsersrvgprot.h"
#include "wap_adp.h"
#elif( (I51_MTK_VS>=VS_09A)&&(I51_MTK_VS<=VS_09B) )
#include "GeneralDeviceGprot.h"
#include "WAPProfileGProt.h"
#elif( I51_MTK_VS<=VS_08B )
#include "gpioInc.h"
	#if (I51_MTK_VS !=VS_0812 )
	#include "WAPProfileGProt.h"
	#endif
#endif
#if defined(MTK_6235_09B)
#include "mtpnp_ad_master_if.h"
#include "mtpnp_ad_master_common_def.h"
#elif(I51_MTK_VS <=VS_09B )
#include "bra_cmn.h"
#include "mtpnp_ad_master_if.h"
#include "mtpnp_ad_master_common_def.h"
#endif
#ifndef YEARFORMATE
#define YEARFORMATE 2000
#endif 

//能够创建定时器的最大个数
#define MAX_TIMER_COUNT	10
//最大能够打开文件的个数
#define MAX_FILE_OPEN_COUNT	10

//	文件长度
#define I51_ADE_OS_FILENAME_LEN	64



//定时器回调的函数指针类型
typedef void (*I51OSTIMERSYSTEMCALLBACK ) ( void ) ; 
typedef void (*I51OSGETGPRSNETCOUNTCALLBACK) (iU32  TotalSent, iU32 TotalReceived, iU32 LastSent, iU32 LastReceived);


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

typedef struct
{

	//	author : william
	//	since : 20110719
	//	(C)NON TEST

	//定时器结构定义

	//定时器下标
	iU16	timerId ;

	//等待执行的时间
	iS32	delay ;

	//定时器是否在使用
	iBOOL	used ;

	//执行函数
	i51TIMERCALLBACK	timerCallback ;

	//调用系统定时器，需要的回调类型

	I51OSTIMERSYSTEMCALLBACK timerSysCallback ;

	//是否循环
	iBOOL	isloop ;

    //优先级
    iU32 priority ;

} ADATimer ;


#define ARRAY_COUNT(a)  (sizeof(a)/sizeof(a[0]))

//定时器id的最小值
static iU32	timeridmin = I51_TIMER_ID_START ;

//定时器id的最大值
static iU32	timeridmax = I51_TIMER_ID_END ;

////能够创建定时器的最大个数
static iU32 maxtimecount = MAX_TIMER_COUNT ;

//定时器的结构数组
static ADATimer adatimer[MAX_TIMER_COUNT] = { 0 };

//相对路径模式是否开启，默认开启
static iBOOL AbsoluteModeOpen =  iFALSE ;

//存放网址
static iU16* url_CB=iNULL ;

void* i51_med_alloc_ext_mem (kal_int32 size);
void i51_med_free_ext_mem (void **pointer);


//	切勿删除以下代码
#ifdef i51PROFILINGS 
//#if 1  
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
		i51_med_free_ext_mem ( (void**)&mmt_mask ) ;
	mmt_mask = 0 ;
		
	return 1 ;
		
}

void MMTCreateMask ()
{

	//	author : Jelo Wang
	//	since : 20110705
	//	(C)TOK

	int looper = 0 ;
	
	mmt_mask = (unsigned char* ) i51_med_alloc_ext_mem ( MMT_MASK_LENGTH ) ;

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
		
	newmem	= (MEMORY_MONITOR* ) i51_med_alloc_ext_mem ( sizeof(MEMORY_MONITOR) ) ;

	if ( 0 == mmt_mask ) MMTCreateMask () ;
	
	memcpy ( (void*)((unsigned int)address+length) , mmt_mask , MMT_MASK_LENGTH ) ;

	if ( file ) {
		newmem->file = (char* ) i51_med_alloc_ext_mem ( strlen ( file ) + 1 ) ;
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

	mask = (unsigned char* ) i51_med_alloc_ext_mem ( MMT_MASK_LENGTH ) ;

	for ( looper = mem_monitor.head ; looper ;  ) 
	{

		int counter = 0 ;
		
		memcpy ( mask , (void*)((unsigned int )looper->address+looper->length) , MMT_MASK_LENGTH ) ;
		
		for ( counter = 0 ; counter < MMT_MASK_LENGTH ; counter ++ )
		{

			if ( mask [ counter ] != mmt_mask [ counter ] )
			{				
				i51AdeOsLog ( 0 ,  "OS : MMTCheckOverflow , !!!!!!!!! M : %x , In : '%s' , At line : '%d' - overflowed" , looper->address , looper->file , looper->line ) ;	
				break ;
			}
		}			   

		looper=looper->next ;

	}

	i51_med_free_ext_mem ( (void**)&mask ) ;
	
}


void MMTDestroy () {
	
	//	author : Jelo Wang
	//	since : 20110418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	for ( walker = mem_monitor.head ; walker ; ) {
		mem_monitor.next = walker->next ;
		if ( walker->file ) i51_med_free_ext_mem ( (void**)&walker->file ) ;
		i51_med_free_ext_mem ( (void**)&walker ) ;
		walker = mem_monitor.next ; 
	}
  
	if ( 0 != mmt_mask )
		i51_med_free_ext_mem ( (void**)&mmt_mask ) ;
	mmt_mask = 0 ;
	
}


int MMTCheckLeaked () {

	//	author : Jelo Wang
	//	since : 20091129

	int totall = 0 ;
	int leakedtimes = 0 ;
	
	MEMORY_MONITOR* walker = mem_monitor.head ;

	i51AdeOsLog ( 0 , "OS : MMTCheckLeaked , Let's Check Memory Leaking :)" ) ; 
	
	for ( ;walker;walker=walker->next ) {

		i51AdeOsLog ( 0 ,  "OS : MMTCheckLeaked , !!!!!! M : %x , S : %d , In '%s' , At line : '%d' - leaked",
			walker->address,\
			walker->length,\
			walker->file,\
			walker->line\
		) ;\
	
		totall = totall + walker->length ; 

		leakedtimes ++ ;
		
	}

	if ( 0 == totall ) {
		
		i51AdeOsLog ( 0 ,  "OS : MMTCheckLeaked , No Memory Leaking At All :)" ) ;
		
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
				if ( walker->file ) i51_med_free_ext_mem ( (void**)&walker->file ) ;
				i51_med_free_ext_mem ( (void**)&walker ) ;
				return ;
			} else {
				pre->next = walker->next ;
				if ( walker == mem_monitor.next ) {
					mem_monitor.next = pre ;
				}
				mem_monitor.used = mem_monitor.used - walker->length ;
				if ( walker->file ) i51_med_free_ext_mem ( (void**)&walker->file ) ;
				i51_med_free_ext_mem ( (void**)&walker ) ;
				return ;
			}
			
		}

		pre = walker ;
		walker = walker->next ;
			
	}
	
}

#endif


static void i51GetgprsNetCountCard2CB(iU32  TotalSent, iU32 TotalReceived, iU32 LastSent, iU32 LastReceived) ;
static void i51GetGprsNetCount ( iS32 cardNum , I51OSGETGPRSNETCOUNTCALLBACK callback) ;
I51OSGETGPRSNETCOUNTCALLBACK i51GetgprsNetCountCB ;

//***get net count  *****
static iU32  i51NetCountTotalSent1;
static iU32  i51NetCountTotalRecv1;
static iU32  i51NetCountTotalSent2;
static iU32  i51NetCountTotalRecv2;

//打开文件个数的统计
static iU32 openFileCount = 0 ;

//获取网络流量定时器id
static iS32 i51NetCountTimeId ;

static i51ADECALLBACK i51AdaCreateCB ;

#if( I51_MTK_VS>=VS_W10 )
static U32  i51TotalSent, i51TotalReceived, i51LastSent, i51LastReceived;
#endif

static iU32 i51AdeOsGetTotalCallOutTime ( iS32 cardNum )
{
	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
//获取通话时长
#if( I51_MTK_VS <=VS_09B )
    MYTIME total_out_time;
    iS16 pError;

	if(cardNum==1)
	{
	    memset(&total_out_time, 0, sizeof(total_out_time));
	    ReadRecord(NVRAM_EF_CALL_TIME_LID, 2, (void *) &total_out_time, sizeof(MYTIME), &pError);
	    return  ((total_out_time.nHour) * 60 * 60 + (total_out_time.nMin))*60;
	}else if(cardNum==2)
	{
#ifdef __MMI_DUAL_SIM_MASTER__ 
		memset(&total_out_time, 0, sizeof(total_out_time));
	    ReadRecord(NVRAM_EF_CALL_TIME_LID, 5, (void *) &total_out_time, sizeof(MYTIME), &pError);
	    return ((total_out_time.nHour) * 60 * 60 + (total_out_time.nMin))*60;
#else
		return 0;
#endif
	}	
	return 0;

#else
    #ifdef __MMI_CLOG_CALL_TIME__
    srv_clog_ctime_data_struct call_time;   
    srv_clog_ctime_get_data((mmi_sim_enum)cardNum, SRV_CLOG_CALL_TYPE_NON_VOIP,
                #if( (I51_MTK_VS >=VS_W11)&&(I51_MTK_VS <=VS_11B_1224) )
                    0,
                #endif
                &call_time, 1);
    return call_time.total_dialed ;
    #else
    return (30*60);
    #endif
#endif
}

static iU32 i51AdeOsGetTotalCallInTime ( iS32 card_num )
{
	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
//获取通话时长
#if( I51_MTK_VS <=VS_09B )
    MYTIME total_in_time;
    S16 pError;

	if(card_num==1)
	{
	    memset(&total_in_time, 0, sizeof(total_in_time));
	    ReadRecord(NVRAM_EF_CALL_TIME_LID, 3, (void *) &total_in_time, sizeof(MYTIME), &pError);
	    return  (total_in_time.nHour) * 60 * 60 + (total_in_time.nMin) * 60;
	}else if(card_num==2)
	{
#ifdef __MMI_DUAL_SIM_MASTER__ 
		memset(&total_in_time, 0, sizeof(total_in_time));
	    ReadRecord(NVRAM_EF_CALL_TIME_LID, 6, (void *) &total_in_time, sizeof(MYTIME), &pError);
	    return (total_in_time.nHour) * 60 * 60 + (total_in_time.nMin) * 60;
#else
		return 0;
#endif
	}
	return 0;
    
#else
    #ifdef __MMI_CLOG_CALL_TIME__
    srv_clog_ctime_data_struct call_time;
    srv_clog_ctime_get_data((mmi_sim_enum)card_num, SRV_CLOG_CALL_TYPE_NON_VOIP,
                #if( (I51_MTK_VS >=VS_W11))
                    0,
                #endif
                &call_time, 1);
    return call_time.total_recved;
    #else
    return 30 * 60;
    #endif
#endif
}


static void i51GetgprsNetCountCard1CB(iU32  TotalSent, iU32 TotalReceived, iU32 LastSent, iU32 LastReceived)
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	//得到卡1的流量数据赋值
	i51NetCountTotalSent1 = TotalSent ;
	i51NetCountTotalRecv1 = TotalReceived ;

	//做下面的操作是为了调用 i51GetGprsNetCount ( 1 ,  i51GetgprsNetCountCard1CB) 
	//时就得到两张卡的网络流量数据
	i51GetGprsNetCount ( 2 ,  i51GetgprsNetCountCard2CB) ;
	
	
}

static void i51GetgprsNetCountCard2CB(iU32  TotalSent, iU32 TotalReceived, iU32 LastSent, iU32 LastReceived)
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	//卡2的流量数据赋值
	i51NetCountTotalSent2 = TotalSent ;
	i51NetCountTotalRecv2 = TotalReceived ;

	if(i51AdaCreateCB) {
		
		i51AdaCreateCB ( iTRUE ) ;

	}

}

static void i51GetGPRSNetCountMid(void *info)
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
   // 发送消息获得网络流量的回调
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* micha1020 */
    iU32 TotalSent, TotalReceived, LastSent, LastReceived;
    mmi_ps_get_gprs_data_counter_rsp_struct *localPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    localPtr = (mmi_ps_get_gprs_data_counter_rsp_struct*) info;

    if (localPtr->result == MMI_TRUE)
    {
        TotalSent = localPtr->counter_info.total_tx_data;
        TotalReceived = localPtr->counter_info.total_rx_data;
        LastSent = localPtr->counter_info.last_tx_data;
        LastReceived = localPtr->counter_info.last_rx_data;
    }
    else
    {
        TotalSent = 0;
        TotalReceived = 0;
        LastSent = 0;
        LastReceived = 0;
    }
	
	if( i51GetgprsNetCountCB )
		i51GetgprsNetCountCB(TotalSent,TotalReceived,LastSent,LastReceived);

}





static void i51GetGprsNetCount ( iS32 cardNum , I51OSGETGPRSNETCOUNTCALLBACK callback)
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
#if( I51_MTK_VS>=VS_W10 )
    l4c_gprs_statistics_info_struct counter_info;

    i51GetgprsNetCountCB = callback;

    if(l4c_ps_get_gprs_data_counter_req(&counter_info))
    {
    	  i51TotalSent     = counter_info.total_tx_data;
        i51TotalReceived = counter_info.total_rx_data;
        i51LastSent      = counter_info.last_tx_data;
        i51LastReceived  = counter_info.last_rx_data;
    #ifdef __MMI_DUAL_SIM_MASTER__  
        i51TotalSent     = i51TotalSent/2;
        i51TotalReceived =  i51TotalReceived/2;
        i51LastSent      =  i51LastSent/2;
       i51LastReceived  =  i51LastReceived/2;
        if( cardNum == 1 ){
        	i51TotalSent     += counter_info.total_tx_data%2;
            i51TotalReceived += counter_info.total_rx_data%2;
            i51LastSent      += counter_info.last_tx_data%2;
            i51LastReceived  += counter_info.last_rx_data%2;
        }
    #endif

    }
  if ( i51GetgprsNetCountCB )
  	i51GetgprsNetCountCB ( i51TotalSent , i51TotalReceived , i51LastSent , i51LastReceived ) ;
  	
#else
        MYQUEUE Message;

    	i51GetgprsNetCountCB = callback ;

    #ifndef  __MMI_DUAL_SIM_MASTER__
    	cardNum = 1;
    #endif

    	if(cardNum==1){
    		Message.oslSrcId = MOD_MMI;
    		SetProtocolEventHandler(i51GetGPRSNetCountMid, PRT_MMI_PS_GET_GPRS_DATA_COUNTER_RSP);
    		Message.oslDestId = MOD_L4C;
    	}
    #if defined(__MMI_DUAL_SIM_MASTER__)
    	else{
    		Message.oslSrcId = MOD_MMI;	
    		SetSlaveProtocolEventHandler(i51GetGPRSNetCountMid, PRT_MMI_PS_GET_GPRS_DATA_COUNTER_RSP);
        	Message.oslDestId = MOD_L4C_2;
    	}
    #endif 
        Message.oslMsgId = PRT_MMI_PS_GET_GPRS_DATA_COUNTER_REQ;
        Message.oslDataPtr = iNULL;
        Message.oslPeerBuffPtr = iNULL;
        OslMsgSendExtQueue(&Message);
#endif
}


void StartPriorityTimer(U16 timerid, U32 delay, FuncPtr funcPtr, iU32 priority)
{
    if(priority==0)
        StartNonAlignTimer(timerid, delay, funcPtr);
    else  StartTimer(timerid, delay, funcPtr);
}

static void i51AdeOsTimerCallback( iU32 id , void(*callback)(void) )
{
    
	iU16 timerid = adatimer[id].timerId  ;

	if ( adatimer[id].isloop)		StartPriorityTimer ( timeridmin + timerid - 1 , adatimer[id].delay , callback , adatimer[id].priority) ;

	if ( adatimer[id].timerCallback )
		adatimer[id].timerCallback ( timerid ) ;

}

//定义10 回调用来标识id

static void i51AdeOsTimerCallback1 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(0,i51AdeOsTimerCallback1);

}

static void i51AdeOsTimerCallback2 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(1,i51AdeOsTimerCallback2);

}

static void i51AdeOsTimerCallback3 ( void )
{

	//	author : william
	//	since : 20110719
	//	(C)NON TEST

    i51AdeOsTimerCallback(2,i51AdeOsTimerCallback3);

}

static void i51AdeOsTimerCallback4 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(3,i51AdeOsTimerCallback4);

}

static void i51AdeOsTimerCallback5 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(4,i51AdeOsTimerCallback5);

}

static void i51AdeOsTimerCallback6 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(5,i51AdeOsTimerCallback6);

}

static void i51AdeOsTimerCallback7 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(6,i51AdeOsTimerCallback7);

}

static void i51AdeOsTimerCallback8 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(7,i51AdeOsTimerCallback8);

}

static void i51AdeOsTimerCallback9 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(8,i51AdeOsTimerCallback9);

}

static void i51AdeOsTimerCallback10 ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

    i51AdeOsTimerCallback(9,i51AdeOsTimerCallback10);

}

static void i51AdeOsTimerInit () 
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	adatimer[0].timerSysCallback =  i51AdeOsTimerCallback1 ;
	adatimer[1].timerSysCallback =  i51AdeOsTimerCallback2 ;
	adatimer[2].timerSysCallback =  i51AdeOsTimerCallback3 ;
	adatimer[3].timerSysCallback =  i51AdeOsTimerCallback4 ;
	adatimer[4].timerSysCallback =  i51AdeOsTimerCallback5 ;
	adatimer[5].timerSysCallback =  i51AdeOsTimerCallback6 ;
	adatimer[6].timerSysCallback =  i51AdeOsTimerCallback7 ;
	adatimer[7].timerSysCallback =  i51AdeOsTimerCallback8 ;
	adatimer[8].timerSysCallback =  i51AdeOsTimerCallback9 ;
	adatimer[9].timerSysCallback =  i51AdeOsTimerCallback10 ;
	
}

void  i51AdeOsGetFilePath ( iU16 *FilePath , const iU16 *path )
{

	//	author : william
	//	since : 20110829
	//	PKIG Tech. Co., Ltd.
	iU16 *GetCurPath  = iNULL ;

//	memset ( FilePath , 0 , I51_ADE_OS_FILENAME_LEN ) ;
	
	if ( AbsoluteModeOpen )
		mmi_ucs2cpy ( (iCHAR*)FilePath , (iCHAR*)path ) ;
	else {

		//相对路径的处理 
	  	GetCurPath =  i51KernelGetApplicationPath () ;
		mmi_ucs2cpy ((iCHAR*)FilePath , (iCHAR*)GetCurPath ) ;
	 	mmi_ucs2cat ( (iCHAR*)FilePath , (iCHAR*)path ) ;
		
	}

	
}

void* i51_med_alloc_ext_mem (kal_int32 size)
{
    if( size <4 ) size = 4; //防止分配过小死机
    size &= 0x7FFFFFFF;
#if( I51_MTK_VS >=VS_11A_1112 )
 	#if defined(__MED_IN_ASM__)
		return gdi_alloc_ext_mem_cacheable(size);//gdi_alloc_ext_mem(size);
	#else
		return med_alloc_ext_mem_cacheable(size);//med_alloc_ext_mem (size);
	#endif
#elif( I51_MTK_VS >=VS_09A )
	return med_alloc_ext_mem_cacheable(size);//med_alloc_ext_mem (size);
#else
    return med_alloc_ext_mem (size);
#endif

}

void i51_med_free_ext_mem (void **pointer)
{
#if( I51_MTK_VS >=VS_11A_1112 )
 	#if defined(__MED_IN_ASM__)
		gdi_free_ext_mem(pointer);
 	#else
		med_free_ext_mem (pointer);
	#endif
#else
    med_free_ext_mem (pointer);
#endif

}

void* i51AdeOsMalloc ( iU32 length , const iCHAR* file , iU32 line )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsMalloc , S , %x %s %x" , length , file , line ) ;
	#endif
	
	#ifdef i51PROFILINGS
	{
        void* address = (void*) i51_med_alloc_ext_mem ( length + MMT_MASK_LENGTH ) ;
        if ( 0 == address ) {
        	i51AdeOsLog ( 0 , "OS : i51AdeOsMalloc Failed,malloc:%d,left:%d",length+4,i51AdeOsGetTotallMemoryLeft() ) ;		
        	return 0 ;
        }
        memset ( address , 0 , length + MMT_MASK_LENGTH ) ;
        MMTAdd ((iCHAR*)file , line , length , (int)address ) ;
        i51MemoryOverflowCheck () ; 

        #ifdef i51ADAPTER_DEBUG
        i51AdeOsLog ( 0 , "OS : i51AdeOsMalloc , E" ) ;
        #endif

        return address;
	} 
	#else
    {
        void *pAlloc=NULL;
		pAlloc = i51_med_alloc_ext_mem ( length ) ;

        if(pAlloc==NULL)
            i51AdeOsLog ( 0 , "OS : i51AdeOsMalloc , Failed,malloc:%d,left:%d",length,i51AdeOsGetTotallMemoryLeft() );

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsMalloc , E" ) ;
		#endif
		return pAlloc;
    }
	#endif
	
}

iBOOL i51AdeOsFree ( void* address )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFree , S , %x" , address ) ;
	#endif
	
	#ifdef i51PROFILINGS
		MMTCheckOverflow () ; 
		MMTFree ( (int)address ) ; 
	#endif

	i51_med_free_ext_mem ( &address ) ;
	
	#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsFree , E" ) ;
	#endif
	return iTRUE ;
	
}

void* i51AdeOsMallocEx ( iU32 length )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsMallocEx , S , %x" , length ) ;
	#endif
	
	#ifdef i51PROFILINGS
		{
		void* address = (void*) i51_med_alloc_ext_mem ( length + MMT_MASK_LENGTH ) ;
		if ( 0 == address ) {

			i51AdeOsLog ( 0 , "OS : i51AdeOsMallocEx , BE" ) ;	
			return 0 ;
		}
		memset ( address , 0 , length + MMT_MASK_LENGTH ) ;
		MMTAdd ((iCHAR*)__FILE__ , __LINE__ , length , (int)address ) ;
		i51MemoryOverflowCheck () ; 
		
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsMallocEx , E" ) ;
		#endif
		
		return address;
		} 
	#else
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsMallocEx , E" ) ;
		#endif
	
		return i51_med_alloc_ext_mem ( length ) ;
	#endif

}

iBOOL i51AdeOsFreeEx ( void* address )
{

	//	author : william
	//	since : 20110724  
	//	PKIG Tech. Co., Ltd.  

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFreeEx , S , %x" , address ) ;
	#endif

	 #ifdef i51PROFILINGS
		MMTCheckOverflow () ; 
		MMTFree ( (int)address ) ; 
	#endif

	i51_med_free_ext_mem ( &address ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFreeEx , E" ) ;
	#endif
	
	return iTRUE ;

}

void i51AdeOsFileAModeEnable () 
{

	//	author : william
	//	since : 20110829
	//	PKIG Tech. Co., Ltd.

	//开启绝对路径模式

	AbsoluteModeOpen = iTRUE ;
	

}

void i51AdeOsFileAModeDisable () 
{
	//	author : william
	//	since : 20110829
	//	PKIG Tech. Co., Ltd.

	//关闭绝对路径模式

	AbsoluteModeOpen = iFALSE ;
	
}


iBOOL i51AdeOsFileCreateDir ( const iU16* path )
{

	//	author : william
	//	since : 20110812
	//	PKIG Tech. Co., Ltd.

	iS32	result ;
	iU16 FilePath[I51_ADE_OS_FILENAME_LEN] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileCreateDir , S , %x" , path ) ;
	#endif
	
	i51AdeOsGetFilePath ( FilePath , path ) ;
	result = FS_CreateDir ( FilePath ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileCreateDir , E , %x" , result ) ;
	#endif	
	
	if ( !result ) 
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileCreateDir , BE" );
		return iFALSE ;
	}
	
		
}


static iU16 i51AdeOsConvetGetAttr ( iU16 srcAttr )
{
	
	iU16 finalattributes = 0 ;
	if ( (srcAttr & FS_ATTR_READ_ONLY) ==  FS_ATTR_READ_ONLY ) {
		
		finalattributes |= i51_OS_FL_ATTR_ROL ;
		
	}
		
	if ( (srcAttr & FS_ATTR_HIDDEN) ==  FS_ATTR_HIDDEN ) {
		
		finalattributes |= i51_OS_FL_ATTR_HID ;	
	}

	if ( (srcAttr & FS_ATTR_DIR) == FS_ATTR_DIR ) {
		
		finalattributes |= i51_OS_FL_ATTR_DIR ;
		
	}
	if ( (srcAttr & FS_ATTR_ARCHIVE) == FS_ATTR_ARCHIVE ) {
		
		finalattributes |= i51_OS_FL_ATTR_WRT ;
		
	}
	return finalattributes ;
}
iBOOL i51AdeOsFileFindClose ( iFILE filehandle )
{

	//	author : william
	//	since : 20110812
	//	PKIG Tech. Co., Ltd.
	
	iBOOL result ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileFindClose , S , %x" , filehandle ) ;
	#endif

	result = FS_FindClose ( filehandle ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileFindClose , E , %x" , result ) ;
	#endif
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileFindClose , BE" );
		return iFALSE ;
	}
}

iFILE i51AdeOsFileFind ( const iU16* path , i51FILEINFO* info ) 
{

	//	author : william
	//	since : 20110812
	//	PKIG Tech. Co., Ltd.
	
	FS_DOSDirEntry fileInfo = {0} ;
	iU16 fileName[I51_ADE_OS_FILENAME_LEN] = {0} ;
	iU16 filepath[I51_ADE_OS_FILENAME_LEN] = {0} ;
	iFILE findFile ;
	iU16 FilePath[I51_ADE_OS_FILENAME_LEN] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileFind , S , %x , %x" , path , info ) ;
	#endif
	
	i51AdeOsGetFilePath ( FilePath , path ) ;
	findFile = FS_FindFirst ( FilePath , 0 , 0 , &fileInfo ,fileName , ARRAY_COUNT(fileName) ) ;
	mmi_ucs2cpy ((iCHAR*) info->name ,(iCHAR*) fileName ) ;
	info->attribute = i51AdeOsConvetGetAttr (fileInfo.Attributes) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileFind , E" ) ;
	#endif

	return findFile ;
	
}

iBOOL i51AdeOsFileFindNext ( iFILE filehandle , i51FILEINFO* info )
{

	//	author : william
	//	since : 20110812
	//	PKIG Tech. Co., Ltd.
	
	FS_DOSDirEntry fileInfo = {0} ;
	iU16 fileName[I51_ADE_OS_FILENAME_LEN] = {0} ;
	iS32 result ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileFindNext S , %x , %x" , filehandle , info ) ;
	#endif

	result =  FS_FindNext(filehandle, &fileInfo, fileName, ARRAY_COUNT(fileName)) ;
	mmi_ucs2cpy ((iCHAR*) info->name , (iCHAR*) fileName ) ;
	info->attribute = i51AdeOsConvetGetAttr (fileInfo.Attributes) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileFindNext E , %x" , result ) ;
	#endif

	if ( !result )
		return iTRUE ;
	else  {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileFindNext , BE" );
		return iFALSE ;

	}

	
}
iBOOL i51AdeOsFileDeleteDir( const iU16* path )
{



	//	author : william
	//	since : 20110719
	//	(C)NON TEST

	//删除文件可以对子文件进行删除

	iS32	result ;
	iU16 FilePath[I51_ADE_OS_FILENAME_LEN] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileDeleteDir , S , %x" , path ) ;
	#endif
	
	i51AdeOsGetFilePath ( FilePath , path ) ;
	result = FS_XDelete( FilePath  , FS_FILE_TYPE | FS_DIR_TYPE | FS_RECURSIVE_TYPE , iNULL , 0 ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileDeleteDir , E , %x" , result ) ;
	#endif
	
	if ( result >= 0 )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileDeleteDir , BE ,  %x" , result);
		return iFALSE ;
	}

}



iFILE i51AdeOsFileOpen ( const iU16* path , iU32 mode , const iCHAR* fl , iS32 line )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	iU32 flag = 0 ;
	iS32 result ;  
	iU16 FilePath[I51_ADE_OS_FILENAME_LEN] = {0} ;
    iCHAR filepath8bit[I51_ADE_OS_FILENAME_LEN] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileOpen , S , %x , %x , %x , %x" , path , mode , fl , line ) ;
	#endif
	
	if ( openFileCount > MAX_FILE_OPEN_COUNT ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileOpen , BE1.1") ;
		return 0 ; 

	}
	
	i51AdeOsGetFilePath ( FilePath , path ) ;

      result = ( (mode & i51_OS_FL_EXISTING_OPEN) == i51_OS_FL_EXISTING_OPEN
	  		|| (mode & i51_OS_FL_ALWAYS_OPEN) == i51_OS_FL_ALWAYS_OPEN 
			|| (mode & i51_OS_FL_ALWAYS_CREATE) ==  i51_OS_FL_ALWAYS_CREATE )
		  	&& ( (mode & i51_OS_FL_READ) == i51_OS_FL_READ 
			|| (mode & i51_OS_FL_WRITE) == i51_OS_FL_WRITE ) ;
//		  	|| (mode & i51_OS_FL_TAIL_WRITE) ==  i51_OS_FL_TAIL_WRITE ) ;
	  
	if ( !result ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileOpen , BE1.2") ;
		return 0 ;
	}
	
	if ( (mode & i51_OS_FL_ALWAYS_OPEN) == i51_OS_FL_ALWAYS_OPEN ) {
		
		flag |= FS_CREATE ;

	}
	
	if( (mode & i51_OS_FL_ALWAYS_CREATE) ==  i51_OS_FL_ALWAYS_CREATE ) {
		
		flag |= FS_CREATE_ALWAYS ;	

	}
		
	if(  (mode & i51_OS_FL_READ) == i51_OS_FL_READ ) {
		
		flag |= FS_READ_ONLY ;

	}

	if( (mode & i51_OS_FL_WRITE) == i51_OS_FL_WRITE ) {
		
		flag |= FS_READ_WRITE ;

	}

//	flag |= FS_OPEN_SHARED ;
	result = FS_Open ( ( iU16 *) FilePath , flag );
	if ( result > 0 ) {
		
		openFileCount ++ ;
	}else {
		result = 0 ;
	}

    i51AdeStdUnicode2Ascii (FilePath,filepath8bit) ;
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileOpen E , result %x,filename:%s" , result , filepath8bit ) ;
    if(!result)		
        i51AdeOsLog ( 0 , "OS : %s Open F",filepath8bit) ;
    return result ;
	
}	

iBOOL i51AdeOsFileClose ( iFILE file )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileClose , S , %x" , file ) ;
	#endif
	
	result = FS_Close ( file ) ;
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileClose , E , result %x ,iFILE %x" , result ,file) ;
    
	openFileCount -- ;
    
	if ( !result ) {
		return iTRUE ;
		
	} else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileClose , BE" );
		return iFALSE ;

	}

}

iBOOL i51AdeOsFileRead ( iFILE file , void* DataPtr , iU32 Length , iU32* Read )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileRead , S , %x , %x , %x , %x" , file ,  DataPtr , Length , Read ) ;
	#endif
	result = FS_Read ( file , DataPtr , Length , Read ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileRead , E , %x" , result ) ;
	#endif
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileRead , BE , %d" , result );
		return iFALSE ;
	}

}

iBOOL i51AdeOsFileWrite ( iFILE file , void* DataPtr , iU32 Length, iU32* Written )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileWrite , S , %x , %x , %x , %x" , file ,  DataPtr , Length , Written ) ;
	#endif
	
	result = FS_Write ( file , DataPtr , Length , Written ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileWrite , E , %x" , result ) ;
	#endif
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileWrite , BE , %d" , result );
		return iFALSE ;

	}

}

iBOOL i51AdeOsFileSetSeek ( iFILE file , iS32 offset , iS32 seek )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result ;
	iS32 finalseek = seek ;
	iS32 size ;
	iS32 CurPosition ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetSeek S , %x , %x , %x " , file ,  offset , seek ) ;
	#endif
	
	i51AdeOsFileGetSize ( file , &size ) ;

	if ( i51_OS_FL_SEEK_HEAD ==  seek ) {

		if ( offset > size ) {

			i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetSeek , BE1.1" );
			return iFALSE ;
		}
		finalseek = FS_FILE_BEGIN ;
		
	}else if ( i51_OS_FL_SEEK_CURRENT ==  seek ) {

		i51AdeOsFileGetSeek ( file , &CurPosition , 0 ) ;
		if ( CurPosition + offset > size ) {

			i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetSeek , BE1.2" );
			return iFALSE ;
		}
		finalseek = FS_FILE_CURRENT ;
		
	}else if ( i51_OS_FL_SEEK_TAIL ==  seek ) {

		if ( offset > 0 || offset < -size ) {

			i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetSeek , BE1.3" );
			return iFALSE ;
		}
		finalseek = FS_FILE_END ;
		
	}else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetSeek , BE1.4" );
		return iFALSE ;
	}

	FS_Seek ( file , offset , finalseek ) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetSeek , E ") ;
	#endif
	return iTRUE ;


}


iBOOL i51AdeOsFileGetSeek ( iFILE file , iS32* position , iS32 se )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result = 0 ; 
    iU32    filesize ;
    
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetSeek , S , %x , %x , %x " , file , position , se) ;
	#endif
    if( se == i51_OS_FL_SEEK_HEAD )
	    result = FS_GetFilePosition ( file , ( iU32 *) position ) ;
    
    else if( se == i51_OS_FL_SEEK_TAIL )
    {
        result = FS_GetFileSize( file , &filesize );
        if(!result)
        {
            result = FS_GetFilePosition ( file , ( iU32 *) position ) ;
            *position = ( iS32 )filesize - *position;
        }
        else
        {
	        i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetSeek , get filesize fail ! ,%x" , result ) ;
        }
    }
    
    else
        *position = 0;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetSeek , E ,%x" , result ) ;
	#endif
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "ADE - OS : i51AdeOsFileGetSeek BE , %d" , result );
		return iFALSE ;
	}

}

iBOOL i51AdeOsFileRename ( const iU16* name , const iU16* newname )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result ;
	iU16 FilePath1[I51_ADE_OS_FILENAME_LEN]= {0} ;
	iU16 FilePath2[I51_ADE_OS_FILENAME_LEN] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "i51AdeOsFileRename S , %x , %x " , name , newname ) ;
	#endif
	
	i51AdeOsGetFilePath ( FilePath1 , name ) ;
	i51AdeOsGetFilePath ( FilePath2 , newname ) ;
	
	result = FS_Rename ( FilePath1 , FilePath2 ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileRename , E , %x , %x " , name , newname ) ;
	#endif
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileRename , BE , %d" , result );
		return iFALSE ;

	}

}

iBOOL i51AdeOsFileGetSize ( iFILE file , iS32* size )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetSize , S , %x , %x , %x " , file , size) ;
	#endif
	result = FS_GetFileSize ( file , ( iU32* ) size ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetSize , E") ;
	#endif
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetSize , BE , %d" , result) ;
		return iFALSE ;
	}

}

iBOOL i51AdeOsFileDelete ( const iU16* path  )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result ;
	iU16 FilePath[I51_ADE_OS_FILENAME_LEN]= {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileDelete , S , %x" , path) ;
	#endif

	iASSERT ( NULL != path ) ;
	i51AdeOsGetFilePath ( FilePath , path ) ;
	result = FS_Delete ( FilePath ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileDelete , E") ;
	#endif	
	
	if ( !result )
		return iTRUE ;
	else  {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileDelete , BE , %d" , result) ;
		return iFALSE ;
	}

}

iU32 i51AdeOsFileGetAttr ( const iU16* path  )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result = 0 ;
    iU16    FilePath[I51_ADE_OS_FILENAME_LEN] = {0} ;
	iU32	finalattributes = 0x0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetAttr , S , %x" , path) ;
	#endif
	
	iASSERT ( NULL != path ) ;
	i51AdeOsGetFilePath ( FilePath , path ) ;

	result = FS_GetAttributes ( FilePath ) ;
	if ( result < 0 ) {
		i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetAttr , BE , %d" , result) ;
		return i51_OS_FL_ATTR_ERR ;
	}

	finalattributes = i51AdeOsConvetGetAttr(result) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileGetAttr , E") ;
	#endif
	
	return finalattributes ;
	
}

iBOOL i51AdeOsFileSetAttr ( const iU16* path , iU32 attributes )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	iU8	finalattributes = 0 ;
	iU32	result = 0 ;
	iU16 FilePath[I51_ADE_OS_FILENAME_LEN]= {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetAttr , S , %x , %x" , path ,attributes ) ;
	#endif

	iASSERT ( NULL != path ) ;
	result = ( ( attributes & i51_OS_FL_ATTR_ROL ) == i51_OS_FL_ATTR_ROL ||
			( attributes & i51_OS_FL_ATTR_WRT ) == i51_OS_FL_ATTR_WRT ||
			( attributes & i51_OS_FL_ATTR_HID ) == i51_OS_FL_ATTR_HID ||
			( attributes & i51_OS_FL_ATTR_DIR ) == i51_OS_FL_ATTR_DIR ) ;
	if ( !result ) return iFALSE ;

	if( (attributes & i51_OS_FL_ATTR_ROL) == i51_OS_FL_ATTR_ROL && (attributes & i51_OS_FL_ATTR_WRT) == i51_OS_FL_ATTR_WRT ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetAttr , BE1.1") ;
		return iFALSE ;
	}
		
	if ( (attributes & i51_OS_FL_ATTR_ROL) == i51_OS_FL_ATTR_ROL ) {
		
		finalattributes |= FS_ATTR_READ_ONLY ;

	}

	if ( (attributes & i51_OS_FL_ATTR_WRT) == i51_OS_FL_ATTR_WRT ) {
		
		finalattributes |= FS_ATTR_ARCHIVE ;

	}

	if (  (attributes & i51_OS_FL_ATTR_HID) == i51_OS_FL_ATTR_HID ) {
		
		finalattributes |= FS_ATTR_HIDDEN ;

	}

	if ( (attributes & i51_OS_FL_ATTR_DIR) == i51_OS_FL_ATTR_DIR ) {
		
//		finalattributes |= FS_ATTR_DIR ;
		
	}
		
	i51AdeOsGetFilePath ( FilePath , path ) ;
	result = FS_SetAttributes( FilePath , finalattributes ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetAttr , E") ;
	#endif
	
	if ( !result ) 
		return iTRUE ;
	 else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileSetAttr , BE1.2 , %d" , result) ;
	 	return iFALSE ;
	 }

}

iBOOL i51AdeOsFileFlush ( iFILE file )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileFlush , S , %x" , file) ;
	#endif
	
	result = FS_Commit ( file ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileFlush , E" ) ;
	#endif
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileFlush , BE , %d" , result) ;
		return iFALSE ;
	}

}

iBOOL i51AdeOsFileCopy ( iU16* srcpath  , iU16* destpath  )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	iS32	result = 0 ;
	iU16 FilePath1[I51_ADE_OS_FILENAME_LEN] = {0} ;
	iU16 FilePath2[I51_ADE_OS_FILENAME_LEN]  = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileCopy , S , %x , %x" , srcpath , destpath ) ;
	#endif	

	iASSERT ( NULL != srcpath && NULL !=  destpath ) ;
	
	i51AdeOsGetFilePath ( FilePath1 , srcpath ) ;
	i51AdeOsGetFilePath ( FilePath2 , destpath ) ;
	
	result = FS_Move ( FilePath1 , FilePath2 , FS_MOVE_COPY , iNULL , iNULL , 0) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileCopy , E") ;
	#endif	
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileCopy , BE , %d" , result) ;
		return iFALSE ;
	}
}

iBOOL i51AdeOsFileMove ( iU16* srcpath  , iU16* destpath )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iS32	result = 0 ;
	iU16 FilePath1[I51_ADE_OS_FILENAME_LEN] = {0} ;
	iU16 FilePath2[I51_ADE_OS_FILENAME_LEN] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileMove , S , %x , %x" , srcpath , destpath ) ;
	#endif	
	
	iASSERT ( NULL != srcpath && NULL !=  destpath ) ;
	
	i51AdeOsGetFilePath ( FilePath1 , srcpath ) ;
	i51AdeOsGetFilePath ( FilePath2 , destpath ) ;
	
	result = FS_Move ( FilePath1 , FilePath2 , FS_MOVE_KILL, iNULL , iNULL , 0 ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileMove , E" ) ;
	#endif	
	
	if ( !result )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileMove , BE , %d" , result) ;
		return iFALSE ;
	}

}

iBOOL i51AdeOsFileIsExist ( const iU16* path )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	iS32	result = 0 ;
	iU16 FilePath[I51_ADE_OS_FILENAME_LEN] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileIsExist , S , %x" , path) ;
	#endif	

	iASSERT ( NULL != path ) ;
	i51AdeOsGetFilePath ( FilePath , path ) ;
	result = FS_GetAttributes( FilePath ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileIsExist , E" ) ;
	#endif	
	
	if ( result >= 0 )
		return iTRUE ;
	else {

		i51AdeOsLog ( 0 , "OS : i51AdeOsFileIsExist , BE , %d" , result) ;
		return iFALSE ;
	}

}

iBOOL i51AdeOsFileIsDirExist ( const iU16* path  )
{
	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	int  result;
	iU16 FilePath[I51_ADE_OS_FILENAME_LEN] = {0} ;

	iASSERT ( NULL != path ) ;
	i51AdeOsGetFilePath ( FilePath , path ) ;
	result = FS_GetAttributes( FilePath ) ;
	
	if ( result >= 0 )
		return iTRUE ;
	else {
		i51AdeOsLog ( 0 , "OS : i51AdeOsFileIsDirExist , BE , %d" , result) ;
		return iFALSE ;
	}

}

 iU32 i51AdeOsTimerCreate ( iBOOL loop , iU32 priority )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	iU16	i = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerCreate , S , %x" , loop) ;
	#endif	
	
	for ( i = 0 ; i < maxtimecount ; i ++ ) {

		//	Jelo Edit Start 2011.11.23
		//if ( adatimer[i].used == 0 )	break ;
		if ( adatimer[i].timerId == 0 )	break ;
		//	Jelo Edit Finished
	}

	if ( i == maxtimecount ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsTimerCreate , BE") ;
		return 0 ;
	}

	adatimer[i].timerId = i + 1  ;
	adatimer[i].isloop = loop ;
    adatimer[i].priority = priority;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerCreate , E") ;
	#endif	

	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerCreate , %d , %d , ret = %d" , loop , priority , i+1 ) ;
	return i + 1 ;
 
}

 iBOOL i51AdeOsTimerStart ( iU32 timerid , i51TIMERCALLBACK callback , iU32 time )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerStart , S , %x , %x , %x" , timerid , callback , time ) ;
	#endif	

	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerStart , %d , %x , %d" , timerid , callback ,time ) ;
	//	Jelo Eidt Start 2011.11.30
	//	不进行断言
	//	iASSERT ( timerid >= 1 && timerid <= MAX_TIMER_COUNT ) ;
	if ( timerid < 1 || timerid > MAX_TIMER_COUNT ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsTimerStart , ID out" ) ;
		return iFALSE ;
	}
	//	Jelo Finish 

	adatimer[timerid - 1].timerCallback =   callback ;
	adatimer[timerid - 1].delay = time ;
	StartPriorityTimer( timeridmin + timerid - 1 , time, adatimer[timerid - 1].timerSysCallback, adatimer[timerid - 1].priority) ;
	adatimer[timerid - 1].used = iTRUE ;	

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerStart , E" ) ;
	#endif	
	return iTRUE ;
}

iBOOL i51AdeOsTimerStop ( iU32 id )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerStop , S , %x" , id ) ;
	#endif

	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerStop , %d" , id ) ;

//	iASSERT ( id >= 1 && id <= MAX_TIMER_COUNT ) ;
	if ( id < 1 || id > MAX_TIMER_COUNT ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsTimerStop , ID out" ) ;
		return iFALSE ;
	}
	StopTimer ( timeridmin + id -1 ) ;
	adatimer[id - 1].timerId = 0 ;
	adatimer[id - 1].used = iFALSE ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsTimerStop , E" ) ;
	#endif	
	
	return iTRUE ;

}

iU32 i51AdeOsGetDiskSpace ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	//获取存储卡容量(单位KB)
	FS_DiskInfo     diskinfo = {0} ;
	iU16  drivepath[8] = {0} ;
	iS32	result = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetDiskSpace , S" ) ;
	#endif	
	
      kal_wsprintf ( drivepath ,  "%c:\\" , FS_GetDrive ( FS_DRIVE_V_REMOVABLE , 1 , FS_NO_ALT_DRIVE ) ) ;
	result = FS_GetDiskInfo ( ( iU16* ) drivepath , &diskinfo , FS_DI_BASIC_INFO ) ;
      if ( result >= 0 ) {
          if ( diskinfo.SectorsPerCluster * diskinfo.BytesPerSector == 0 ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsGetDiskSpace , BE" ) ;
              return 0;
         }
	   //磁盘空间(byte) = 簇* 每簇多少扇区* 每个扇区多少字节
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsGetDiskSpace , E" ) ;
		#endif	
	   return	diskinfo.TotalClusters * diskinfo.SectorsPerCluster * diskinfo.BytesPerSector/1024 ;
      }
	  

}

iU32 i51AdeOsGetDiskSpaceLeft ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	
	//获取磁盘的剩余空间

	FS_DiskInfo     diskinfo = {0} ;
	unsigned short  drivepath[8] = {0} ;
	iS32	result = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetDiskSpaceLeft , S" ) ;
	#endif	
	
	//获取存储卡剩余容量(单位KB)
      kal_wsprintf ( drivepath ,  "%c:\\" , FS_GetDrive ( FS_DRIVE_V_REMOVABLE , 1 , FS_NO_ALT_DRIVE ) ) ;
	result = FS_GetDiskInfo ( ( iU16* ) drivepath , &diskinfo , FS_DI_FREE_SPACE ) ;
      if ( result >= 0 ) {
          if ( diskinfo.SectorsPerCluster * diskinfo.BytesPerSector == 0 ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsGetDiskSpaceLeft , BE" ) ;
		return 0;
      	    }
		  
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetDiskSpaceLeft , E" ) ;
	#endif	
	
	   return	diskinfo.FreeClusters * diskinfo.SectorsPerCluster * diskinfo.BytesPerSector/1024 ;
          
      }

}

iU32 i51AdeOsGetSysDiskSpace ( void )
{

	//	author : Jelo
	//	since : 20111102
	//	PKIG Tech. Co., Ltd.

	//获取系统盘总空间
	FS_DiskInfo     diskinfo = {0};
	iU16  drivepath[8] = {0} ;
	iS32	result = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetSysDiskSpace , S" ) ;
	#endif	
     
      kal_wsprintf ( drivepath ,  "%c:\\" , FS_GetDrive ( FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM , 1 , FS_NO_ALT_DRIVE ) ) ;
	result = FS_GetDiskInfo ( ( iU16* ) drivepath , &diskinfo , FS_DI_BASIC_INFO ) ;
      if ( result >= 0 ) {
          if ( diskinfo.SectorsPerCluster * diskinfo.BytesPerSector == 0 ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsGetSysDiskSpace , BE" ) ;
		return 0;
         }
	   //磁盘空间(byte) = 簇* 每簇多少扇区* 每个扇区多少字节
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsGetSysDiskSpace , E" ) ;
		#endif	
		return	diskinfo.TotalClusters * diskinfo.SectorsPerCluster * diskinfo.BytesPerSector/1024 ;
      }

}

iU32 i51AdeOsGetSysDiskSpaceLeft ( void )
{

	//	author : Jelo
	//	since : 20111102
	//	PKIG Tech. Co., Ltd.
	
	//	获取系统盘剩余空间

	FS_DiskInfo     diskinfo = {0} ;
	unsigned short  drivepath[8] = {0} ;
	iS32	result = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetSysDiskSpaceLeft , S" ) ;
	#endif	
	
	//获取存储卡剩余容量(单位KB)
      memset ( drivepath , 0 , sizeof ( drivepath ) ) ;
      kal_wsprintf ( drivepath ,  "%c:\\" , FS_GetDrive ( FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM , 1 , FS_NO_ALT_DRIVE ) ) ;
	result = FS_GetDiskInfo ( ( iU16* ) drivepath , &diskinfo , FS_DI_FREE_SPACE ) ;
      if ( result >= 0 ) {
	  	
		if ( diskinfo.SectorsPerCluster * diskinfo.BytesPerSector == 0 ) {

			i51AdeOsLog ( 0 , "OS : i51AdeOsGetSysDiskSpaceLeft , BE" ) ;
			return 0;
		}
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsGetSysDiskSpaceLeft , E" ) ;
		#endif	
		return	diskinfo.FreeClusters * diskinfo.SectorsPerCluster * diskinfo.BytesPerSector/1024 ;
          
      }

}


#ifdef i51PROFILINGS
typedef struct{

    KAL_ADM_ID  ext_mem_pool_id;
    iU32   ext_mem_alloc_count;
    iU32   ext_mem_left_size;
	
}med_ext_mem_struct;
extern med_ext_mem_struct g_med_ext_mem_cntx;
iU64 i51AdeOsGetTotallMemory ()
{

	//	author : william
	//	since : 20110831
	//	PKIG Tech. Co., Ltd.
	
	iU64 size ;
	//size =  kal_adm_get_max_alloc_size ( g_med_ext_mem_cntx.ext_mem_pool_id ) ;
	size = g_med_ext_mem_cntx.ext_mem_alloc_count ;
	
	return size ;
}
#endif

#ifdef i51PROFILINGS
iU64 i51AdeOsGetTotallMemoryUsed ()
{

	//	author : william
	//	since : 20110831
	//	PKIG Tech. Co., Ltd.

	iU64 MemoryUsedSize = 0 ;

	MemoryUsedSize = mem_monitor.used ;	
	return MemoryUsedSize ;
}
#endif

iU32 i51AdeOsGetTotallMemoryLeft ( void )
{

	//	author : william
	//	since : 20110831
	//	PKIG Tech. Co., Ltd.

	iU32 MemoryLeftSize = 0 ;
        
    #ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetTotallMemoryLeft , S" ) ;
	#endif	

    #if( I51_MTK_VS >=VS_11A_1112 )
 	    #if defined(__MED_IN_ASM__)
        {
            extern iU16 I51_GROUP_ID;
            extern MMI_ID mmi_frm_get_root_app(MMI_ID allocator_id);
            extern U32 mmi_frm_asm_get_total_left_size_r(mmi_id id);
            MMI_ID ROOT_ID ;
            ROOT_ID = mmi_frm_get_root_app(I51_GROUP_ID);
            MemoryLeftSize = mmi_frm_asm_get_total_left_size_r(ROOT_ID);
        }
	    #else
        {
            MemoryLeftSize = med_ext_left_size () ;
        }
	    #endif
    #else
        MemoryLeftSize = med_ext_left_size () ;
    #endif

	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetTotallMemoryLeft , E , ret = %d" , MemoryLeftSize) ;
	#endif	
	
	return MemoryLeftSize ;
	
}

iU32 i51AdeOsGetCallingTime ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iU32 CalltimeTotal = 0 ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetCallingTime , S" ) ;
	#endif	

	CalltimeTotal = i51AdeOsGetTotalCallOutTime ( 1 ) + i51AdeOsGetTotalCallOutTime ( 2 ) +
                    i51AdeOsGetTotalCallInTime ( 1 ) + i51AdeOsGetTotalCallInTime ( 2 ); 
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetCallingTime , E , ret = %d" , CalltimeTotal ) ;
	#endif	
	
	return CalltimeTotal ;

}

iU32 i51AdeOsGetNetFlow ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iU32 NetFlowTotal= 0 ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetNetFlow , S" ) ;
	#endif	

	NetFlowTotal = i51NetCountTotalSent1 + i51NetCountTotalRecv1 + i51NetCountTotalSent2 + i51NetCountTotalRecv2 ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetNetFlow , E , ret = %x" , NetFlowTotal ) ;
	#endif	
	
	return NetFlowTotal ;

}

iU32 i51AdeOsGetTick ( void )	
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	iU32	tick = 0 ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetTick , S" ) ;
	#endif	

	kal_get_time ( &tick ) ;
	tick = kal_ticks_to_milli_secs (tick) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetTick , E , ret = %x" , tick ) ;
	#endif	
	
	return tick ;
	

}

iBOOL i51AdeOsGetTime ( iU16* year , iU16* month , iU16* day , iU16* hour , iU16* mins , iU16* sec )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	applib_time_struct curtime = {0};

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetTime , S , %x , %x , %x , %x , %x , %x " , year , month , day , hour , mins , sec ) ;
	#endif	
	
	applib_dt_get_date_time ( &curtime ) ;
	*year = curtime.nYear ;
	*month = curtime.nMonth ;
	*day = curtime.nDay ;
	*hour = curtime.nHour ;
	*mins = curtime.nMin ;
	*sec = curtime.nSec ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetTime , E , ret = %x" , iTRUE) ;
	#endif	
	
	return iTRUE ;

}

iBOOL i51AdeOsPermanitBufferRead ( iU8* buffer , iU32 length )
{

	//	author : william
	//	since : 20100724
	//	PKIG Tech. Co., Ltd.
	iS16 err16 = 0 ;
	iS32 ret = 0 ;
	iS32 state = 0 ;
	int  i = 0 ;
	U8  temp_buf[16] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferRead , S , %x , %x" , buffer , length ) ;
	#endif	

	for(i=0;i<6;i++){
		state = ReadValue ( I51_NVRAM_SN_SD_1+i ,  temp_buf+i*2 , 2 , &err16) ;
		if ( err16 != NVRAM_READ_SUCCESS || state != 2 )
			break ;
	}
	if(i==6) {

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferRead , E , ret = %x" , iTRUE ) ;
		#endif	
		memcpy ( buffer, temp_buf , length ) ;
		return iTRUE;
	}

	for(i=0;i<2;i++){
		
		state = ReadValue ( I51_NVRAM_SN_DD_1+i ,  temp_buf+i*8 , 8 , &err16) ;
		if ( err16 != NVRAM_READ_SUCCESS || state != 8 )
			break ;
	}
	if(i==2) {

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferRead , ret = %x" , iTRUE ) ;
		#endif	
		memcpy ( buffer, temp_buf , length ) ;
		return iTRUE;
	}


	for(i=0;i<12;i++){
		state = ReadValue ( I51_NVRAM_SN_BD_1+i ,  temp_buf+i , 1 , &err16) ;
		if ( err16 != NVRAM_READ_SUCCESS || state != 1 ) {
			
			break ;
		}
	}
	if ( i == 12 ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferRead , E , ret = %x" , iTRUE ) ;
		memcpy ( buffer, temp_buf , length ) ;
		return iTRUE ;
	}
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferRead , E , ret = %x" , iFALSE ) ;
	#endif	
	return iFALSE ;

}



iBOOL i51AdeOsPermanitBufferWrite ( iU8* buffer , iU32 length )
{

	//	author : william
	//	since : 20100724
	//	PKIG Tech. Co., Ltd.
	iS16 err16 = 0;
	iS32 ret = 0 ;
	iS32 state = 0 ;
	int  i = 0;
	U8  temp_buf[16] = {0};

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferWrite , S , %x , %x" , buffer , length ) ;
	#endif	

	memcpy ( temp_buf ,  buffer, length ) ;

	for(i=0;i<6;i++){
		
		state = WriteValue ( I51_NVRAM_SN_SD_1+i ,  temp_buf+i*2 , 2 , &err16) ;
		if ( err16 != NVRAM_WRITE_SUCCESS || state != 2 )
			break ;
	}
	if(i==6) {

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferWrite , E , ret = %x" , iTRUE) ;
		#endif	
		
		return iTRUE;
	}


	for(i=0;i<2;i++){
		
		state = WriteValue ( I51_NVRAM_SN_DD_1+i ,  temp_buf+i*8 , 8 , &err16) ;
		if ( err16 != NVRAM_WRITE_SUCCESS || state != 8 )
			break ;
	}
	if(i==2) {
		
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferWrite , E , ret = %x" , iTRUE) ;
		#endif	
		return iTRUE;
	}


	for(i=0;i<12;i++){
		
		state = WriteValue ( I51_NVRAM_SN_BD_1+i ,  temp_buf+i , 1 , &err16) ;
		if ( err16 != NVRAM_WRITE_SUCCESS || state != 1 ) {
			break; 
		}
	}
	if ( i == 12 ) {

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferWrite , E , ret = %x" , iTRUE) ;
		#endif	
		return iTRUE;

	}

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsPermanitBufferWrite , E , ret = %x" , iFALSE ) ;
	#endif		
	return iFALSE ;

}

void i51AdeOsSysLog ( iU16 logIndex , ... )
{
	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	va_list ap = {0} ;
	iCHAR MSGLog[256] = {0} ;
	iU16 filename[32] = {0} ;
	FS_HANDLE  FH = 0 ;
	iU32 nWriteLen ;
    iCHAR* log;

    if(!i51LogisExist())
        return ;
    log =(iCHAR*)i51AdePortSysLogGetString(logIndex) ;
    if(!log ) return;
    
	va_start ( ap , logIndex ) ;
	vsnprintf ( MSGLog ,254, log , ap ) ;
	va_end ( ap ) ;
    MSGLog[255]=0;

	#ifdef i51SIMULATOR
		printf (MSGLog) ;
		printf ("\n");
	#endif

	kal_wsprintf( filename , "%c:\\i51\\%s" , 
	   			 FS_GetDrive( FS_DRIVE_V_REMOVABLE , 1 , FS_NO_ALT_DRIVE ) , 
	  			 "i51.log" );

	FH = FS_Open( filename , FS_READ_WRITE ) ;
	if( FH < 0 )
	{
        kal_wsprintf( filename , "%c:\\i51\\%s" , 
	   			'C' /*FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM) )*/ , 
	  			 "i51.log" );
		FH = FS_Open ( filename , FS_READ_WRITE) ;
		if(FH < 0) return ;
	}

	FS_Seek ( FH , 0 , FS_FILE_END ) ;
	FS_Write (FH , MSGLog , strlen ( MSGLog ) , (UINT*) &nWriteLen ) ;
	FS_Write ( FH , "\r\n" , 2 , (UINT*) &nWriteLen ) ;
	FS_Close ( FH ) ;
	
}

void i51AdeOsLog ( iS32 module , const iCHAR* log , ... )
{
	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	va_list ap = {0} ;
	iCHAR MSGLog[256] = {0} ;
	iU16 filename[32] = {0} ;
	FS_HANDLE  FH = 0 ;
	iU32 nWriteLen ;

//	Jelo Edit Start 2011.11.22
//	#ifdef i51ADAPTER_DEBUG
//	i51AdeOsLog ( 0 , "i51AdeOsLog S , %x , %x" , module , log ) ;
//	#endif	
//	Jelo Edit Finished

    if(!i51LogisExist())
        return ;
	va_start ( ap , log ) ;
	vsnprintf ( MSGLog ,254, log , ap ) ;
	va_end ( ap ) ;
    MSGLog[255]=0;

	#ifdef i51SIMULATOR
		printf (MSGLog) ;
		printf ("\n");
	#endif

	kal_wsprintf( filename , "%c:\\i51\\%s" , 
	   			 FS_GetDrive( FS_DRIVE_V_REMOVABLE , 1 , FS_NO_ALT_DRIVE ) , 
	  			 "i51.log" );

	FH = FS_Open( filename , FS_READ_WRITE ) ;
	if( FH < 0 )
	{
        kal_wsprintf( filename , "%c:\\i51\\%s" , 
	   			'C' /*FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM) )*/ , 
	  			 "i51.log" );
		FH = FS_Open ( filename , FS_READ_WRITE) ;
		if(FH < 0) return ;
	}

	FS_Seek ( FH , 0 , FS_FILE_END ) ;
	FS_Write (FH , MSGLog , strlen ( MSGLog ) , (UINT*) &nWriteLen ) ;
	FS_Write ( FH , "\r\n" , 2 , (UINT*) &nWriteLen ) ;
	FS_Close ( FH ) ;

//	Jelo Edit Start 2011.11.22
//	#ifdef i51ADAPTER_DEBUG
//	i51AdeOsLog ( 0 , "i51AdeOsLog E") ;
//	#endif			
//	Jelo Edit Finished		

	
}

void i51AdeOsLogEx ( iS32 module , iU8 *buf , iU32 buflen )
{
	//	author : miki
	//	since : 20120201
	//	PKIG Tech. Co., Ltd.

	iU16 filename[32] = {0} ;
	FS_HANDLE  FH = 0 ;
	iU32 nWriteLen ;

    if(!i51LogisExist())
        return ;
	kal_wsprintf( filename , "%c:\\i51\\%s" , 
	   			 FS_GetDrive( FS_DRIVE_V_REMOVABLE , 1 , FS_NO_ALT_DRIVE ) , 
	  			 "i51.log" );

	FH = FS_Open( filename , FS_READ_WRITE ) ;
	if( FH < 0 ){
        kal_wsprintf( filename , "%c:\\i51\\%s" , 
	   			'C' /*FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM) )*/ , 
	  			 "i51.log" );
		FH = FS_Open ( filename , FS_READ_WRITE) ;
		if(FH < 0) return ;
	}

	FS_Seek ( FH , 0 , FS_FILE_END ) ;
	FS_Write (FH , buf , buflen , (UINT*) &nWriteLen ) ;
	FS_Write ( FH , "\r\n" , 2 , (UINT*) &nWriteLen ) ;
	FS_Close ( FH ) ;	
	
}


#ifdef i51PROFILINGS
iU32 i51AdeOsFileTotallOpened ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.

	return openFileCount ;

}




iU32 i51AdeOsTimerGetLive ( void )
{

	//	author : william
	//	since : 20110724
	//	PKIG Tech. Co., Ltd.
	// 当前活跃的timer 数量

	iU16 i ;
	iU32	count = 0 ;

	for ( i = 0 ; i < maxtimecount ; i ++ ){

		if ( adatimer[i].used )	count ++ ;
		
	}

	return count ;
}
#endif



iBOOL i51AdeOsGetBaseStation ( iS32 simCard , iU32* bsid , iU32* zid )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	
//根据sim卡获取基站信息
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetBaseStation , S , %x , %x , %x" , simCard , bsid , zid ) ;
	#endif	
	
#ifdef __MMI_DUAL_SIM_MASTER__     

	if(simCard !=1&& simCard!=2 ) {     

	i51AdeOsLog ( 0 , "OS : i51AdeOsGetBaseStation , BE" ) ;
	*bsid = *bsid = 0xffff ;
	return iFALSE ;   
	
	}           
	*bsid = rmmi_cntxt_g[simCard-1].register_status.cell_id ;  
	*zid = rmmi_cntxt_g[simCard-1].register_status.lac_value ;
	
#else    

	*bsid = rmmi_cntxt_g.register_status.cell_id ;  
	*zid = rmmi_cntxt_g.register_status.lac_value ;    
#endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetBaseStation , E , ret = %x" , iTRUE ) ;
	#endif	
	return iTRUE ;   

}

iBOOL i51AdeOsSetAccelerator ( i51ACCCALLBACK callback )
{

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSetAccelerator , S , %x" , callback ) ;
	#endif	

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSetAccelerator , E , ret = %x" , iTRUE ) ;
	#endif	

	//重力感应
	return iTRUE ;   
	
}


void (*i51AdeOsPhoneCallback) ( iBOOL results) ;
static iU8 i51AdeOsPhoneCallId = 0 ;
//	通话前音量
static iU8 i51AdeOsPhoneVol = 0 ; 
static iU8 i51AdeOsPhoneSimSel = 0 ;
#if( I51_MTK_VS >VS_09A)
static void i51AdeOsPhoneConnectInd(void *info, int mod_src) ;
static void i51AdeOsPhoneReleaseInd(void *info, int mod_src) ;
static void i51AdeOsPhoneDialInd(void *info, int mod_src) ;
static void i51AdeOsPhoneStartInd(void *info, int mod_src) ;
static void i51AdeOsPhoneStopInd(void *info, int mod_src) ;
static void  i51SetProtocolEventHandler ( void (*callback)(void *info , int mod_src) , iU16 event  ) ;
#else
static void i51AdeOsPhoneConnectInd(void *info) ;
static void i51AdeOsPhoneReleaseInd(void *info) ;
static void i51AdeOsPhoneDialInd(void *info) ;
static void i51AdeOsPhoneStartInd(void *info) ;
static void i51AdeOsPhoneStopInd(void *info) ;
static void  i51SetProtocolEventHandler ( void (*callback)(void *info) , iU16 event  ) ;
#endif
static void i51ClearProtocolEventHandler (iU16 event ) ;
static void i51AdeOsPhoneVolReset ()
{

	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	//	重置系统通话声音
	
	iU8 vel_level ;
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneVolReset , S") ;

	vel_level = aud_get_volume_level (AUD_MODE_NORMAL , AUD_VOLUME_SPH) ;
	i51AdeOsPhoneVol = aud_get_volume_gain(VOL_NORMAL ,AUD_VOLUME_SPH, vel_level) ;
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneVol , i51AdeOsPhoneVol = %d " , i51AdeOsPhoneVol ) ;
	aud_speech_set_output_volume(0, 0) ;

}

static void i51AdeOsPhoneVolResume ()
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	//	恢复系统通话声音

	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneVolResume , S") ;

	aud_speech_set_output_volume(i51AdeOsPhoneVol, 0) ;
}


static void i51AdeOsPhoneDlrReset ()
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDlrReset , S") ;
	/*重置系统电话相关回调*/
	i51SetProtocolEventHandler(i51AdeOsPhoneConnectInd, PRT_OUTGOINGCALL_CONNECTED);
    	i51SetProtocolEventHandler(i51AdeOsPhoneReleaseInd, PRT_HELDCALL_DISCONNECTED);
	i51SetProtocolEventHandler(i51AdeOsPhoneDialInd, PRT_OUTGOINGCALL_EVENT_RSP);
	i51ClearProtocolEventHandler(PRT_PLAY_GSM_TONE);
	i51ClearProtocolEventHandler(PRT_INCOMINGCALL_DROPPED);
	i51ClearProtocolEventHandler(PRT_NOTIFY_SS_EVENT);

}

static iU16 i51GetL4ModId ()
{
#ifdef __MMI_DUAL_SIM_MASTER__

	if (i51AdeOsPhoneSimSel == 1 ) 
		return MOD_L4C ;
	else
		return	MOD_L4C_2 ;
#else
	return MOD_L4C ;
#endif
}

static void i51ClearProtocolEventHandler (iU16 event )
{
#if( I51_MTK_VS <=VS_09B )
 #ifdef __MMI_DUAL_SIM_MASTER__
	if (i51AdeOsPhoneSimSel == 1 ) {

		ClearProtocolEventHandler(event);
	}else {

		ClearSlaveProtocolEventHandler (event ) ;
	}
#else
	ClearProtocolEventHandler(event);
 #endif
#elif( I51_MTK_VS >=VS_W11 )
	srv_gcall_reset_ps_event_handler(event);
#endif	
}
static void i51AdeOsPhoneDlrResume ()
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	#if( I51_MTK_VS >=VS_09B )
	extern void srv_gcall_phdlr_call_connect_ind (void *info, int mod_src) ;
	extern void srv_gcall_phdlr_dial_ind (void *info, int mod_src) ;
	extern void srv_gcall_phdlr_internal_alert_ind (void *info, int mod_src) ;
	extern void srv_gcall_phdlr_call_disconnect_ind (void *info, int mod_src) ;
	extern void srv_gcall_phdlr_call_release_ind (void *info, int mod_src) ;
	extern void srv_gcall_phdlr_notify_ss_ind(void *info, int mod_src) ;
	
	/*恢复系统电话相关回调*/
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDlrResume , S") ;
	i51SetProtocolEventHandler(srv_gcall_phdlr_call_connect_ind, PRT_OUTGOINGCALL_CONNECTED);
	i51SetProtocolEventHandler(srv_gcall_phdlr_dial_ind, PRT_OUTGOINGCALL_EVENT_RSP);
	i51SetProtocolEventHandler(srv_gcall_phdlr_internal_alert_ind, PRT_PLAY_GSM_TONE);
  	i51SetProtocolEventHandler(srv_gcall_phdlr_call_disconnect_ind, PRT_INCOMINGCALL_DROPPED);
  	i51SetProtocolEventHandler(srv_gcall_phdlr_call_release_ind, PRT_HELDCALL_DISCONNECTED);
	i51SetProtocolEventHandler(srv_gcall_phdlr_notify_ss_ind, PRT_NOTIFY_SS_EVENT);
	#else
	extern void PsCBackPlayInternalTone(void *info) ;
	extern void OutgoingCallConnected(void *info) ;
	extern void PsCbackOutgoingCallIdSync(void *info) ;
	extern void PsCBackNotifySSInd(void *info) ;
	i51SetProtocolEventHandler(OutgoingCallConnected, PRT_OUTGOINGCALL_CONNECTED);
	i51SetProtocolEventHandler(PsCbackOutgoingCallIdSync, PRT_OUTGOINGCALL_EVENT_RSP);
	i51SetProtocolEventHandler(PsCBackPlayInternalTone, PRT_PLAY_GSM_TONE);
//  	SetProtocolEventHandler(srv_gcall_phdlr_call_disconnect_ind, PRT_INCOMINGCALL_DROPPED);
//  	SetProtocolEventHandler(srv_gcall_phdlr_call_release_ind, PRT_HELDCALL_DISCONNECTED);
	i51SetProtocolEventHandler(PsCBackNotifySSInd, PRT_NOTIFY_SS_EVENT);
	#endif
	
}

#if( I51_MTK_VS >=VS_09B )
static void  i51SetProtocolEventHandler ( void (*callback)(void *info , int mod_src) , iU16 event  ) 
{
#if( I51_MTK_VS >=VS_W11 )
	srv_gcall_set_ps_event_handler (callback , event ) ;
#elif( I51_MTK_VS ==VS_09B )
	if (i51AdeOsPhoneSimSel == 1 ) {

		SetProtocolEventHandler(callback , event);
	}else {

		SetSlaveProtocolEventHandler ( callback , event ) ;
	}
#endif	
}
static void i51AdeOsPhoneConnectInd(void *info, int mod_src)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneConnectInd , S") ;
	
}

static void i51AdeOsPhoneReleaseInd(void *info, int mod_src)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneReleaseInd , S") ;
	i51AdeOsPhoneDlrResume () ;
	i51AdeOsPhoneVolResume () ;
}

static void i51AdeOsPhoneDialInd(void *info, int mod_src)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	mmi_cc_dial_ind_struct *msg  = (mmi_cc_dial_ind_struct*)info ;
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDialInd , S") ;
	if ( msg ->result.flag == L4C_OK ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDialInd , %d" ,  msg ->call_id ) ;
		//	拨号中
		if ( msg ->call_id == -1) {

			i51AdeOsPhoneDlrResume () ;
		} else {

		//	拨号成功
			i51AdeOsPhoneCallId = msg ->call_id ;
			i51AdeOsPhoneVolReset () ;
		}

	}else {

		//	拨号失败
		i51AdeOsPhoneDlrResume () ;
	}
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDialInd , S") ;
}

static void i51AdeOsPhoneStartInd(void *info, int mod_src)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneStartInd , S") ;
}

static void i51AdeOsPhoneStopInd(void *info, int mod_src)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneStopInd , S") ;
}
#else
static void  i51SetProtocolEventHandler ( void (*callback)(void *info) , iU16 event  ) 
{
	
 #ifdef __MMI_DUAL_SIM_MASTER__

	if (i51AdeOsPhoneSimSel == 1 ) {

		SetProtocolEventHandler(callback , event);
	}else {

		SetSlaveProtocolEventHandler ( callback , event ) ;
	}
#else
	SetProtocolEventHandler(callback , event);
#endif
	
}

static void i51AdeOsPhoneConnectInd(void *info)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneConnectInd , S") ;
	
}

static void i51AdeOsPhoneReleaseInd(void *info)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneReleaseInd , S") ;
	i51AdeOsPhoneDlrResume () ;
	i51AdeOsPhoneVolResume () ;
}

static void i51AdeOsPhoneDialInd(void *info)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	mmi_cc_dial_ind_struct *msg  = (mmi_cc_dial_ind_struct*)info ;
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDialInd , S") ;
	if ( msg ->result.flag == L4C_OK ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDialInd , %d" ,  msg ->call_id ) ;
		//	拨号中
		if ( msg ->call_id == -1) {

			i51AdeOsPhoneDlrResume () ;
		} else {

		//	拨号成功
			i51AdeOsPhoneCallId = msg ->call_id ;
			i51AdeOsPhoneVolReset () ;
		}

	}else {

		//	拨号失败
		i51AdeOsPhoneDlrResume () ;
	}
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDialInd , S") ;
}

static void i51AdeOsPhoneStartInd(void *info)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneStartInd , S") ;
}

static void i51AdeOsPhoneStopInd(void *info)
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneStopInd , S") ;
}
#endif
iU32 i51AdeOsCreatePhone () 
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	iS32 *siminfo = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsCreatePhone , S") ;
	#endif		
	
	siminfo = (iS32*)i51AdeSmsGetSims() ;
	if ( siminfo[1] == 1 ) {

		i51AdeOsPhoneSimSel = 1 ;
	} else {

		i51AdeOsPhoneSimSel = 2 ;
	}
	
	i51AdeOsLog ( 0 , "OS : i51AdeOsCreatePhone , %d" , i51AdeOsPhoneSimSel ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsCreatePhone , E , ret = %x" , 1 ) ;
	#endif	
	return 1 ;
}

iBOOL i51AdeOsStartPhoneCall ( iU32 phoneid , iCHAR* number , void (*callback) ( iBOOL results) ) 
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.

	MYQUEUE Message;
    	mmi_ss_parsing_string_req_struct *req;
			
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsStartPhoneCall , S , %x , %s , %x" , phoneid , number , callback ) ;
	#endif	

	i51AdeOsLog ( 0 , "OS : i51AdeOsStartPhoneCall , S" ) ;
	iASSERT (callback) ;

	i51AdeOsPhoneDlrReset () ;
	
	i51AdeOsPhoneCallback = callback ;	
    	Message.oslSrcId = MOD_MMI ;
    	Message.oslDestId = i51GetL4ModId ();  //MOD_L4C;srv_gcall_get_l4c_mod_id();
    	Message.oslMsgId = PRT_USSDVSS_REQ ;
    	Message.oslPeerBuffPtr = NULL;

    	req = (mmi_ss_parsing_string_req_struct*)OslConstructDataPtr(sizeof(mmi_ss_parsing_string_req_struct));

    	req->length = strlen(number);
    	memcpy(req->input, number, req->length);

    	req->dcs = 0x0f;
    	req->ip_string[0] = 0;
    	req->source_id =  LMMI_SRC;
    	req->call_type = CSMCC_VOICE_CALL;
	#if( I51_MTK_VS >VS_0812 )
    	req->als_type = ALS_DIAL_BY_SETTING;
    	req->ecc_info = INFO_ECC_CHECK_BY_MODEM ;
	#else
//	req->als_type = 0 ;
//	req->ecc_info = 0 ;
	#endif

    	Message.oslDataPtr = (oslParaType*)req;
	i51SetProtocolEventHandler(i51AdeOsPhoneStartInd , PRT_USSDVSS_RSP);
    	OslMsgSendExtQueue(&Message);
		
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsPhoneDataCalReqInd , E , ret = %x" , iTRUE) ;
	#endif	
	
	return iTRUE ;
}

iBOOL i51AdeOsStopPhoneCall ( iU32 phoneid ) 
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	
	MYQUEUE Message;
	mmi_cc_chld_req_struct *req;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsStopPhoneCall , S , %x" , phoneid ) ;
	#endif	


	Message.oslSrcId =  MOD_MMI ;
	Message.oslDestId = i51GetL4ModId ();; //MOD_L4C;//srv_gcall_get_l4c_mod_id();
	Message.oslMsgId = PRT_CALLENDSPECIFIC_EVENT ;

	req = (mmi_cc_chld_req_struct*)OslConstructDataPtr(sizeof(mmi_cc_chld_req_struct));

	req->opcode =  CSMCC_REL_SPECIFIC_CALL ;
	req->call_id = i51AdeOsPhoneCallId ;
	req->source_id = LMMI_SRC;

	Message.oslDataPtr = (oslParaType*)req;
	Message.oslPeerBuffPtr = NULL;
	i51SetProtocolEventHandler(i51AdeOsPhoneStopInd, PRT_END_CHLD_RSP);

	OslMsgSendExtQueue(&Message);	

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsStopPhoneCall , E , ret = %x" , iTRUE ) ;
	#endif	
	return iTRUE ;
}


iBOOL i51AdeOsReleasePhone ( iU32 phoneid ) 
{
	//	author : william
	//	since : 20111207
	//	PKIG Tech. Co., Ltd.
	

	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsReleasePhone , S , %x" , phoneid ) ;
	#endif	

	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsReleasePhone , E , ret = %x" , iTRUE ) ;
	#endif	
	return iTRUE ;
}

iBOOL i51AdeOsSetBrowserPage ( iCHAR* homepage ) 
{
#if( I51_MTK_VS <VS_W10 )
  #ifndef MSF_CFG_WAP_PROF_SUPPORT
  	/*     MTK_6225_0812     */
    bra_cmn_set_local_hompage_url (homepage); 
    bra_cmn_set_homepage ("", homepage);

    #ifdef i51ADAPTER_DEBUG
    i51AdeOsLog ( 0 , "OS : i51AdeOsSetBrowserPage , S--1 , %x" , homepage ) ;
    #endif	

  #else // MSF_CFG_WAP_PROF_SUPPORT
	mmi_wap_prof_result_enum result;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSetBrowserPage , S--2 , %x" , homepage ) ;
	#endif	
  
    #ifndef MTK_6235_09B
	bra_cmn_set_local_hompage_url(homepage);
	#endif
	
	#ifdef __MMI_WAP_PROF__
	
	 	#ifdef __MMI_WAP_DUAL_SIM__
		  if((MTPNP_AD_Get_UsableSide()==MTPNP_AD_SIMCARD1_USABLE)||(MTPNP_AD_Get_UsableSide()==MTPNP_AD_DUALSIM_USABLE))
			result = mmi_wap_prof_set_url_as_homepage(MMI_WAP_PROF_SIMID_SIM1 , (U8*)homepage);
		  else if(MTPNP_AD_Get_UsableSide()==MTPNP_AD_SIMCARD2_USABLE)
			result = mmi_wap_prof_set_url_as_homepage(MMI_WAP_PROF_SIMID_SIM2 , (U8*)homepage);
	    #else //__MMI_WAP_DUAL_SIM__
		    result = mmi_wap_prof_set_url_as_homepage(0, (U8*)homepage);
	    #endif
	  if(result == MMI_WAP_PROF_ACCESS_DENIED_ERR)	return iFALSE;
	  
	#else //__MMI_WAP_PROF__
 	  mmi_brw_prof_wap_set_homepage((S8 *)homepage);
	#endif
	
  #endif // MSF_CFG_WAP_PROF_SUPPORT

#else	
	#ifdef BROWSER_SUPPORT
		iU8 homepage_url[(WAP_MAX_SEARCH_URL_LENGTH + 1) * ENCODING_LENGTH];
		srv_brw_error_enum result;
			
		mmi_chset_utf8_to_ucs2_string((U8*) homepage_url, (WAP_MAX_URL_LENGTH + 1) * ENCODING_LENGTH,(U8*) homepage);
		result = srv_brw_set_as_homepage((U8 *)homepage_url);
		if(result == SRV_BRW_ERROR_INVALID_URL)	return iFALSE;
	#endif
#endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSetBrowserPage , E , ret = %x" , iTRUE ) ;
	#endif	
	return iTRUE ;
}


static void i51_VibratorOn(void)
{
#if( I51_MTK_VS >=VS_W10 )
    srv_vibrator_on() ;
#else 
    VibratorOn() ;
#endif
}

static void i51_VibratorOff(void)
{
#if( I51_MTK_VS >=VS_W10 )

	srv_vibrator_off() ;
#else 
	VibratorOff() ;
#endif
}

static iBOOL i51ShockIsOpen= iFALSE; 
 iBOOL i51AdeOsSetShock ( iBOOL open )
{	   
    i51ShockIsOpen = open; 
    if(!open)
        i51_VibratorOff();
	return iTRUE ;
}

iBOOL i51AdeOsDoShock ( iU32 time ) 
{
    if(!i51ShockIsOpen) return iFALSE;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsDoShock , S , %x" , time ) ;
	#endif	

    i51_VibratorOn();
    if(time<200) 
        time =200;
    gui_start_timer(time, i51_VibratorOff);

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsDoShock , E , ret = %x" , iTRUE ) ;
	#endif	

	return iTRUE ;
}

iBOOL i51AdeOsSetScreenLight ( iBOOL open )
{ 
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSetScreenLight , S , %x" , open ) ;
	#endif	

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSetScreenLight , E , ret = %x" , iTRUE ) ;
	#endif	
	
	return iTRUE ;
}

iBOOL i51AdeOsGetScreenLight () 
{
	iBOOL ret = 0 ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetScreenLight , S" ) ;
	#endif	

#if( I51_MTK_VS <=VS_09B )
	ret = IsBacklightOn() ;
#elif( I51_MTK_VS >=VS_W10 )
	ret = srv_backlight_is_on(SRV_BACKLIGHT_TYPE_MAINLCD);
#endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetScreenLight , E , ret = %x" , ret ) ;
	#endif	
	
	return ret ;
}

iBOOL i51AdeOsScreenAlwaysLight ( iBOOL always )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	//背光常亮

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsScreenAlwaysLight , S , %x" , always ) ;
	#endif	

#if( I51_MTK_VS >=VS_11A_1112 )
	if ( always )
        srv_backlight_turn_on(0);        
	else
        srv_backlight_turn_off();
#else
	if ( always )
        TurnOnBacklight(0);  
	else
        TurnOffBacklight();
#endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsScreenAlwaysLight , E , ret = %x" , iTRUE) ;
	#endif	

	return iTRUE ;
}

iBOOL i51AdeOsSetTime ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	//设置时间

	MYTIME mytm = {0};
  	rtc_format_struct tm = {0};

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSetTime , S , %x , %x , %x , %x , %x , %x " , year , month , day , hour , mins , sec ) ;
	#endif	
	
	mytm.nYear = year;
	mytm.nMonth = month;
	mytm.nDay = day;
	mytm.nHour = hour;
	mytm.nMin = mins;
	mytm.nSec = sec;
	mytm.DayIndex = 0;

	//i51SetProtocolEventHandler(PhnsetSetTimeResHdlrForAT, HW_SET_TIME_REQ_SUCCESS);
	mmi_dt_set_rtc_dt(&mytm);	
	
	tm.rtc_year = year-YEARFORMATE;
	tm.rtc_mon = month;
	tm.rtc_day = day;
	tm.rtc_hour = hour;
	tm.rtc_min = mins;
	tm.rtc_sec = sec;
	tm.rtc_wday = 0;

  	SetDateTime(&tm);

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSetTime , E , ret = %x" , iTRUE) ;
	#endif	
	
	return iTRUE ;
	
	
}

iU64 i51AdeOsGetSeconds ( iU16 year , iU16 month , iU16 day , iU16 hour , iU16 mins , iU16 sec )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	//根据时间得到时间戳
			  
	iU64 SecondN = 0 ;
	applib_time_struct curtime = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetSeconds , S , %x , %x , %x , %x , %x , %x " , year , month , day , hour , mins , sec ) ;
	#endif	
	
	curtime.nYear = year ;
	curtime.nMonth = month ;
	curtime.nDay = day ;
	curtime.nHour = hour ;
	curtime.nMin = mins ;
	curtime.nSec = sec ;
	curtime.DayIndex = 0 ;
	
	SecondN = applib_dt_mytime_2_utc_sec ( &curtime , 0 ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetSeconds , E , ret = %x" , SecondN) ;
	#endif	
	
	return SecondN ;
}

iBOOL i51AdeOsSleep ( iU32 ms )
{
	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetSeconds , S , %x" , ms) ;
	#endif	
	
	//系统休眠
	kal_sleep_task ( ms ) ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetSeconds , E , ret = %x" , iTRUE) ;
	#endif	
	
	return iTRUE ;
}

iBOOL i51AdeOsSecond2Time ( iU64 TotallSeconds , iU16 *year , iU16 *month , iU16 *day , iU16 *hour , iU16 *mins , iU16 *sec )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	//根据时间戳得到时间	

	iBOOL result = 0 ;
	applib_time_struct curtime = {0} ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSecond2Time , S , %x , %x , %x , %x , %x , %x , %x" , TotallSeconds , year ,month ,day ,hour ,mins ,sec) ;
	#endif	
	
	result = applib_dt_utc_sec_2_mytime ( TotallSeconds , &curtime , 0) ;
	*year = curtime.nYear ;
	*month = curtime.nMonth ;
	*day = curtime.nDay ;
	*hour = curtime.nHour ;
	*mins = curtime.nMin ;
	*sec = curtime.nSec ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsSecond2Time , E , ret = %x" , iTRUE) ;
	#endif	
	return result ;
	
}

iBOOL i51AdeOsFileReadInt8 ( iFILE file , iU8* integer8 )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.

	iBOOL ret = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileReadInt8 , S , %x , %x" , file , integer8 ) ;
	#endif	
	
	ret = i51AdeOsFileRead ( file , integer8 , 1 , 0 ) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileReadInt8 , E , ret = %x" , ret) ;
	#endif	
	
	return ret ;
}

iBOOL i51AdeOsFileReadInt16 ( iFILE file , iU16* integer16 )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	
	iBOOL ret = 0 ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileReadInt16 , S , %x , %x" , file , integer16 ) ;
	#endif	
	
	ret = i51AdeOsFileRead ( file , integer16 , 2 , 0 ) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileReadInt16 , E , ret = %x" , ret) ;
	#endif	

	return ret ;
}

iBOOL i51AdeOsFileReadInt32 ( iFILE file , iU32* integer32 )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	
	iBOOL ret = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileReadInt32 , S , %x , %x" , file , integer32 ) ;
	#endif	
	
	ret =  i51AdeOsFileRead ( file , integer32 , 4 , 0 ) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileReadInt32 , E , ret = %x" , ret) ;
	#endif	
	return ret ;
}

iBOOL i51AdeOsFileWriteInt8 ( iFILE file , iU8* integer8 )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	
	iBOOL ret = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileReadInt8 , S , %x , %x" , file , integer8 ) ;
	#endif	
	
	ret = i51AdeOsFileWrite ( file , integer8 , 1 , 0 ) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileWriteInt8 , E , ret = %x" , ret) ;
	
	#endif	
	return ret ;
}

iBOOL i51AdeOsFileWriteInt16 ( iFILE file , iU16* integer16 )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	
	iBOOL ret = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileWriteInt16 , S , %x , %x" , file , integer16 ) ;
	#endif	
	
	ret = i51AdeOsFileWrite ( file , integer16 , 2 , 0 ) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileWriteInt16 , E , ret = %x" , ret) ;
	#endif	
	
	return ret ;
}

iBOOL i51AdeOsFileWriteInt32 ( iFILE file , iU32* integer32 )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.
	
	iBOOL ret = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileWriteInt32 , S , %x , %x" , file , integer32 ) ;
	#endif	
	
	ret = i51AdeOsFileWrite ( file , integer32 , 4 , 0 ) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsFileWriteInt32 , E , ret = %x" , ret) ;
	#endif	
	
	return ret ;
}

iBOOL i51AdeOsMMemCardExist () 
{
	//	author : william
	//	since : 20111011
	//	PKIG Tech. Co., Ltd.

	//	判断存储卡是否存在
	iS32 DiskCount = 0  ;
	iBOOL ret = 0 ;
	
#if( I51_MTK_VS>=VS_W10 )
	    iU8 letter;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsMMemCardExist , S -1 " ) ;
	#endif	

 do {
	    letter = srv_fmgr_drv_get_letter_by_type(SRV_FMGR_DRV_CARD_TYPE);
	    if( srv_fmgr_drv_is_valid(letter) )
	    {
	        if( srv_fmgr_drv_is_accessible(letter) )
	        {
	            DiskCount =  1;
		      break ;
	        }
	    }
	    letter = srv_fmgr_drv_get_letter_by_type(SRV_FMGR_DRV_CARD_2_TYPE);
	    if( srv_fmgr_drv_is_valid(letter) )
	    {
	        if( srv_fmgr_drv_is_accessible(letter) )
	        {
	            DiskCount =  1;
		      break ;
	        }
	    }   

 	}while (0) ;
#else
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsMMemCardExist , S -2 " ) ;
	#endif	

	    DiskCount =  fmgr_is_msdc_present ();
#endif


	if ( DiskCount > 0 )
		ret = iTRUE ;
	else
		ret = iFALSE ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsMMemCardExist , E , ret = %x" , ret ) ;
	#endif	
	
	return ret ;



}

iCHAR* i51AdeOsGetPathSymbol () 
{

	//	author : william
	//	since : 20111011
	//	PKIG Tech. Co., Ltd.

	//	返回系统盘盘符。

	static iCHAR Disk[10] = {0} ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetPathSymbol , S" ) ;
	#endif	
	
	sprintf ( Disk , "%c:\\" , FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM) ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetPathSymbol , E , ret = %s" , Disk ) ;
	#endif	
	
	return Disk ;
	
}


iCHAR* i51AdeOsGetPathSymbolEx() 
{
	//	author : william
	//	since : 20111011
	//	PKIG Tech. Co., Ltd.

	//	返回存储盘盘符。
	
	static iCHAR Disk[10] = {0} ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetPathSymbolEx , S" ) ;
	#endif	
	
	sprintf ( Disk , "%c:\\" , FS_GetDrive( FS_DRIVE_V_REMOVABLE , 1 , FS_NO_ALT_DRIVE ) ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "OS : i51AdeOsGetPathSymbolEx , E , ret = %s" , Disk ) ;
	#endif	

	return Disk ;

}

void i51AdeOsRunWapExplorer_CB(void)
{
	//	author : jons
	//	since : 20120502
	//	PKIG Tech. Co., Ltd.

	iS32 ret=-1;
	
	#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog ( 0 , "OS : i51AdeOsRunWapExplorer_CB , S"  ) ;
	#endif

#if defined(WAP_SUPPORT ) || defined(BROWSER_SUPPORT)
	i51KernelExit( i51_KERNEL_EXIT_DAP );
	ret=wap_start_browser(WAP_BROWSER_GOTO_URL, (const kal_uint8*)url_CB);
	i51_med_free_ext_mem( (void**)&url_CB);
#endif	

	#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog ( 0 , "OS : i51AdeOsRunWapExplorer_CB , E , ret=%d"  , ret) ;
	#endif

}

iBOOL i51AdeOsRunWapExplorer ( iU16* url ) 
{
	//	author : jons
	//	since : 20120502
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog ( 0 , "OS : i51AdeOsRunWapExplorer , S"  ) ;
	#endif

#if defined(WAP_SUPPORT ) || defined(BROWSER_SUPPORT)
	url_CB= (iU16*)i51_med_alloc_ext_mem((i51AdeStdWStrlen(url)+1)<<1);
	if ( 0 == url_CB ) {

		i51AdeOsLog ( 0 , "OS : i51AdeOsMallocEx , BE" ) ;	
		return iFALSE ;
	}
	mmi_ucs2cpy ( (iCHAR*)url_CB , (iCHAR*)url ) ;
	gui_start_timer( 1, i51AdeOsRunWapExplorer_CB );
#else
	return iFALSE;
#endif

	#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog ( 0 , "OS : i51AdeOsRunWapExplorer , E"  ) ;
	#endif

    return iTRUE;
}

iBOOL i51AdeOsCreate ( i51ADECALLBACK callback )
{

	//	author : william
	//	since : 20110808
	//	PKIG Tech. Co., Ltd.

	i51AdaCreateCB = callback ;
	i51AdeOsTimerInit () ;
	i51GetGprsNetCount ( 1 ,  i51GetgprsNetCountCard1CB) ;
	return iTRUE ;
	
}

iBOOL i51AdeOsRelease ( void )
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
	return iTRUE ;
}	


#if 0

#if defined(__MANUAL_CACHE__) 
#include "cache_sw.h"
#include "cache_hw.h"
#define SIZE_1KB  0x400
#define SIZE_4MB  0x400000
#define SIZE_16MB 0x1000000
#define MIN_CACHE_CHANNEL_LEN   (SIZE_1KB)
#define MAX_CACHE_CHANNEL_LEN   (SIZE_16MB)
extern iBOOL CodeCacheGetChannel(iU16 *channel);
#endif

static void i51_cache_setup_segment(iU32 start_addr, iU32 end_addr, iU32 total_size)
#if !defined(__MANUAL_CACHE__)
{
}
#else
{
    CacheMenuType  CACHEsetting;
    iU32     size;
    iBOOL       retval;

    /* specify enable flag of CACHE setting */
    CACHEsetting.c_enable = 1;      

    /* check start address align to MIN_CACHE_CHANNEL_LEN */
    iASSERT( (start_addr & (MIN_CACHE_CHANNEL_LEN-1)) == 0x0 );

    /* check end address & start address + total_size */
    iASSERT( start_addr + total_size <= end_addr );

    while (total_size > 0) {
        /* try to find a max-fit size */
        for (size = MIN_CACHE_CHANNEL_LEN; size <= MAX_CACHE_CHANNEL_LEN; size <<= 1) {
            /* check if the start address is aligned to the size */
            if (start_addr & (size-1)) {
                /* not aligned */

                /* revert to the previous size */
                size >>= 1;
                break;
            }

            /* check if size best hit */
            if (size == total_size) break;

            /* Now determine if coverage premitted or not */
            if (size > total_size) {
                /* check if the configuration of this size will overwrite the next region */
                if ((start_addr + size) > end_addr) {
                    /* revert to the previous size */
                    size >>= 1;
                }

                /* find the fit size and exit */
                break;
            }

            /* exit when reach max size */
            if (size == MAX_CACHE_CHANNEL_LEN) break;

        } /* for ( size = MIN_CACHE_CHANNEL_LEN ... MAX_CACHE_CHANNEL_LEN ) */

        /* specify the start address of CACHE setting */
        CACHEsetting.c_addr = start_addr;

        /* specify the range of CACHE setting */
        CACHEsetting.c_range = size;

        /* get a CACHE cahnnel */
        retval = CodeCacheGetChannel(&CACHEsetting.c_channel);

        /* ensure that a channel is available */
        iASSERT(retval == KAL_TRUE);

        /* configure the CACHE */
        CodeCacheSetting(&CACHEsetting);

        /* move start_addr to the next address */
        start_addr += size;

        /* calculate the remaining size of the remaining part */
        if (size >= total_size) {
            /* no remaining part */
            total_size = 0;
        } else {
            total_size -= size;
        }
    }
}
#endif


static void i51_cache_dump_register (void)
{}

static int i51_cache_get_free_channel_number (void)
#if !defined(__MANUAL_CACHE__) 
{
    return 0;
}
#else
{
    int num = 0;
    int i;
    
    for (i = 0; i < 8; i ++) 
        if (((*(MPU_CACHE0+i)) & 1) == 0) num ++;
    
    return num;
}
#endif


extern iBOOL MPUFreeChannel(iU16 *channel);
extern iBOOL CodeCacheFreeChannel(iU16 *channel);
extern void CacheInit(void);

iBOOL i51AdeOsUnsetCPUCache ()
#if !defined(__MANUAL_CACHE__)
{
	return iTRUE ;
}
#else
{
    iU16 i;
    iU32  interruptMask;

    for (i = 0; i < 8; i ++) {
        if ((*(MPU_PORT0+i)) & 1) {
            MPUFreeChannel (&i);
        }
    }
    
    for (i = 0; i < 8; i ++) {
        if ((*(MPU_CACHE0+i)) & 1) {
            CodeCacheFreeChannel (&i);
        }
    }

    interruptMask = SaveAndSetIRQMask();

    *MPU_PORT0 = 0x000000c0;
    *MPU_PORT1 = 0x000000c0;
    *MPU_PORT2 = 0x000000c0;
    *MPU_PORT3 = 0x000000c0;
    *MPU_PORT4 = 0x000000c0;
    *MPU_PORT5 = 0x000000c0;
    *MPU_PORT6 = 0x000000c0;
    *MPU_PORT7 = 0x000000c0;
    
    *MPU_CACHE0 = 0;
    *MPU_CACHE1 = 0;
    *MPU_CACHE2 = 0;
    *MPU_CACHE3 = 0;
    *MPU_CACHE4 = 0;
    *MPU_CACHE5 = 0;
    *MPU_CACHE6 = 0;
    *MPU_CACHE7 = 0;

    RestoreIRQMask(interruptMask);    

    i51_cache_dump_register ();

    CacheInit();

    i51_cache_dump_register ();

	return iTRUE ;
	
}
#endif


iBOOL i51AdeOsSetCPUCache (iU32 start_addr, iU32 end_addr)
#if !defined(__MANUAL_CACHE__)
{ return 0; }
#else
{
    iU32 start;
    iU32 end;
    int size;
    int i;
    int pre_space[5];
    int post_space[5];
    int space[5];
    int num;

    num = i51_cache_get_free_channel_number();

    if (num < 4) {
        i51AdeOsUnsetCPUCache ();
        num = i51_cache_get_free_channel_number();
        
        if (num < 4) {
            return iFALSE ;
        }
    }
    
    for (i = 0; i < 5; i ++) {
        int n;
        
        start  = start_addr & (~((SIZE_1KB<<i) - 1));
        end    = (end_addr + SIZE_1KB) & (~(SIZE_1KB - 1));

        pre_space[i] = start_addr - start;

        size = end - start;
        n = 1;
        while (size > 0) {
            unsigned int max_range;
            unsigned int use_range;

            max_range = start & (~(start-1));

            if (size <= SIZE_1KB) {
                use_range = SIZE_1KB;
            } else {
                use_range = max_range;
                while (use_range > size) {
                    use_range >>= 1;
                    if (use_range < size) {
                        use_range <<= 1;
                        break;
                    }
                }
            }
            
            start += use_range;
            size -= use_range;

            n ++;
            
            if (n >= num) break;
        }

        post_space[i] = start - end_addr;

        if (pre_space[i] < 0 || post_space[i] < 0)
            space[i] = -1;
        else {
            space[i] = pre_space[i] + post_space[i];
        }
    }
    
    {
        int min = -1;
        
        for (i = 0; i < 5; i ++) {
            if (space[i] >= 0) {
                if (min < 0) min = i;
                else if (space[i] < space[min]) min = i;
            }
        }

        if (min >= 0) {
            start  = start_addr & (~((SIZE_1KB<<min) - 1));
            end    = (end_addr + SIZE_1KB) & (~(SIZE_1KB - 1));

            size = end - start;
            num = 1;
            while (size > 0) {
                unsigned int max_range;
                unsigned int use_range;

                max_range = start & (~(start-1));
                use_range = max_range;

                while (use_range > size) {
                    use_range >>= 1;
                    if (use_range < size) {
                        use_range <<= 1;
                        break;
                    }
                }

                i51_cache_setup_segment (start, (start+use_range), use_range);

                start += use_range;
                size -= use_range;
            
                if (num == 4) break;
            }

            i51_cache_dump_register ();
        }
    }

    return iTRUE ;
}
#endif

static void i51_cache_init (void)
#if !defined(__MANUAL_CACHE__)
{}
#else
{
    i51_cache_dump_register ();
}
#endif

#endif



#if defined(__MANUAL_CACHE__) 
#include "cache_sw.h"
#include "cache_hw.h"
#define SIZE_1KB  0x400
#define SIZE_4MB  0x400000
#define SIZE_16MB 0x1000000
#define MIN_CACHE_CHANNEL_LEN   (SIZE_1KB)
#define MAX_CACHE_CHANNEL_LEN   (SIZE_16MB)
extern kal_bool CodeCacheGetChannel(kal_uint16 *channel);
#endif

void i51_cache_setup_segment(kal_uint32 start_addr, kal_uint32 end_addr, kal_uint32 total_size)
#if !defined(__MANUAL_CACHE__)
{}
#else
{
    CacheMenuType  CACHEsetting;
    kal_uint32     size;
    kal_bool       retval;

    /* specify enable flag of CACHE setting */
    CACHEsetting.c_enable = 1;      

    /* check start address align to MIN_CACHE_CHANNEL_LEN */
    ASSERT( (start_addr & (MIN_CACHE_CHANNEL_LEN-1)) == 0x0 );

    /* check end address & start address + total_size */
    ASSERT( start_addr + total_size <= end_addr );

    while (total_size > 0) {
        /* try to find a max-fit size */
        for (size = MIN_CACHE_CHANNEL_LEN; size <= MAX_CACHE_CHANNEL_LEN; size <<= 1) {
            /* check if the start address is aligned to the size */
            if (start_addr & (size-1)) {
                /* not aligned */

                /* revert to the previous size */
                size >>= 1;
                break;
            }

            /* check if size best hit */
            if (size == total_size) break;

            /* Now determine if coverage premitted or not */
            if (size > total_size) {
                /* check if the configuration of this size will overwrite the next region */
                if ((start_addr + size) > end_addr) {
                    /* revert to the previous size */
                    size >>= 1;
                }

                /* find the fit size and exit */
                break;
            }

            /* exit when reach max size */
            if (size == MAX_CACHE_CHANNEL_LEN) break;

        } /* for ( size = MIN_CACHE_CHANNEL_LEN ... MAX_CACHE_CHANNEL_LEN ) */

        /* specify the start address of CACHE setting */
        CACHEsetting.c_addr = start_addr;

        /* specify the range of CACHE setting */
        CACHEsetting.c_range = size;

        /* get a CACHE cahnnel */
        retval = CodeCacheGetChannel(&CACHEsetting.c_channel);

        /* ensure that a channel is available */
        ASSERT(retval == KAL_TRUE);

        /* configure the CACHE */
        CodeCacheSetting(&CACHEsetting);

        /* move start_addr to the next address */
        start_addr += size;

        /* calculate the remaining size of the remaining part */
        if (size >= total_size) {
            /* no remaining part */
            total_size = 0;
        } else {
            total_size -= size;
        }
    }
}
#endif


void i51_cache_dump_register (void)
{}

int i51_cache_get_free_channel_number (void)
#if !defined(__MANUAL_CACHE__) 
{
    return 0;
}
#else
{
    int num = 0;
    int i;
    
    for (i = 0; i < 8; i ++) 
        if (((*(MPU_CACHE0+i)) & 1) == 0) num ++;
    
    return num;
}
#endif


extern kal_bool MPUFreeChannel(kal_uint16 *channel);
extern kal_bool CodeCacheFreeChannel(kal_uint16 *channel);
extern void CacheInit(void);
iBOOL i51AdeOsUnsetCPUCache (void)
#if !defined(__MANUAL_CACHE__)
{
	return iTRUE ;
}
#else
{
    kal_uint16 i;
    kal_uint32  interruptMask;

    for (i = 0; i < 8; i ++) {
        if ((*(MPU_PORT0+i)) & 1) {
            MPUFreeChannel (&i);
        }
    }
    
    for (i = 0; i < 8; i ++) {
        if ((*(MPU_CACHE0+i)) & 1) {
            CodeCacheFreeChannel (&i);
        }
    }

    interruptMask = SaveAndSetIRQMask();

    *MPU_PORT0 = 0x000000c0;
    *MPU_PORT1 = 0x000000c0;
    *MPU_PORT2 = 0x000000c0;
    *MPU_PORT3 = 0x000000c0;
    *MPU_PORT4 = 0x000000c0;
    *MPU_PORT5 = 0x000000c0;
    *MPU_PORT6 = 0x000000c0;
    *MPU_PORT7 = 0x000000c0;
    
    *MPU_CACHE0 = 0;
    *MPU_CACHE1 = 0;
    *MPU_CACHE2 = 0;
    *MPU_CACHE3 = 0;
    *MPU_CACHE4 = 0;
    *MPU_CACHE5 = 0;
    *MPU_CACHE6 = 0;
    *MPU_CACHE7 = 0;

    RestoreIRQMask(interruptMask);    

    i51_cache_dump_register ();

    CacheInit();

    i51_cache_dump_register ();

	return iTRUE ;
	
}
#endif


iBOOL i51AdeOsSetCPUCache (kal_uint32 start_addr, kal_uint32 end_addr)
#if !defined(__MANUAL_CACHE__)
{
	return 0; 
}
#else
{
    kal_uint32 start;
    kal_uint32 end;
    int size;
    int i;
    int pre_space[5];
    int post_space[5];
    int space[5];
    int num;
    
    num = i51_cache_get_free_channel_number();

    if (num < 4) {
        i51AdeOsUnsetCPUCache ();
        num = i51_cache_get_free_channel_number();
        
        if (num < 4) {
            return -1;
        }
    }
    
    for (i = 0; i < 5; i ++) {
        int n;
        
        start  = start_addr & (~((SIZE_1KB<<i) - 1));
        end    = (end_addr + SIZE_1KB) & (~(SIZE_1KB - 1));

        pre_space[i] = start_addr - start;

        size = end - start;
        n = 1;
        while (size > 0) {
            unsigned int max_range;
            unsigned int use_range;

            max_range = start & (~(start-1));

            if (size <= SIZE_1KB) {
                use_range = SIZE_1KB;
            } else {
                use_range = max_range;
                while (use_range > size) {
                    use_range >>= 1;
                    if (use_range < size) {
                        use_range <<= 1;
                        break;
                    }
                }
            }
            
            start += use_range;
            size -= use_range;

            n ++;
            
            if (n >= num) break;
        }

        post_space[i] = start - end_addr;

        if (pre_space[i] < 0 || post_space[i] < 0)
            space[i] = -1;
        else {
            space[i] = pre_space[i] + post_space[i];
        }
    }
    
    {
        int min = -1;
        
        for (i = 0; i < 5; i ++) {
            if (space[i] >= 0) {
                if (min < 0) min = i;
                else if (space[i] < space[min]) min = i;
            }
        }

        if (min >= 0) {
            start  = start_addr & (~((SIZE_1KB<<min) - 1));
            end    = (end_addr + SIZE_1KB) & (~(SIZE_1KB - 1));

            size = end - start;
            num = 1;
            while (size > 0) {
                unsigned int max_range;
                unsigned int use_range;

                max_range = start & (~(start-1));
                use_range = max_range;

                while (use_range > size) {
                    use_range >>= 1;
                    if (use_range < size) {
                        use_range <<= 1;
                        break;
                    }
                }

                i51_cache_setup_segment (start, (start+use_range), use_range);

                start += use_range;
                size -= use_range;
            
                if (num == 4) break;
            }

            i51_cache_dump_register ();
        }
    }

    return 0;
}
#endif


void i51_cache_init (void)
#if !defined(__MANUAL_CACHE__)
{}
#else
{
    i51_cache_dump_register ();
}
#endif


