 
/*

	The Implementing of 'Adapter' , Module : 'SOC'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Api.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/
#include "i51Scripts.h"	
#include "i51AdeCnf.h"
#include "i51AdeSoc.h"
#include "i51AdeStd.h"
#include "mmi_include.h"
#include "kal_release.h"
#include "app2soc_struct.h"
#include "soc_api.h"
#include "EngineerModeSocketDemoApp.h"

#include "custom_data_account.h"
#include "ProtocolEvents.h"
#include "DataAccountDef.h"
#include "DataAccountEnum.h"

#if( (I51_MTK_VS <=VS_08B)||(I51_MTK_VS ==VS_09B)||(I51_MTK_VS ==VS_W10) )
#ifdef __MMI_DUAL_SIM_MASTER__        
#include "mtpnp_ad_master_common_def.h"
#endif
#endif

#if(I51_MTK_VS >=VS_W10)
#include "DataAccountStruct.h"
#include "mmi_frm_events_gprot.h"
#include "SimCtrlSrvGprot.h"
#include "Dtcntsrvgprot.h"
#include "ImeiSrvGprot.h"
#include "DtcntSrv.h"
#include "Servicedefs.h"
#include "cbm_api.h"
#ifdef __TCPIP__
#if !defined(__COSMOS_MMI_PACKAGE__)
#include "mmi_rp_app_dataaccount_def.h"
#endif
#endif /* __TCPIP__ */
#include "DtcntSrvIntStruct.h"

extern srv_dtcnt_store_info_context g_srv_dtcnt_store_ctx;
#endif

#if(I51_MTK_VS <=VS_09A)
#include "wgui_status_icons.h"
#endif

#include "Wgui_categories_util.h"

#define i51_MULTI_MODE

// soc 成功
#define ADA_SOC_SUCCESS	1

//soc 等待
#define ADA_SOC_DELAY	0

//soc 失败
#define ADA_SOC_FAILED		-1



//socket 最大的个数 
#define MAX_SOCKET_COUNT	5

//最大的链路数
#define MAX_LINKED_COUNT	32

#define ACCOUNT_NAME_LEN         64
#define APN_LEN                  100
#define USERNAME_LEN             32
#define PASSWORD_LEN             32
#define DNSADDRESS_LEN           4
#define PROXY_LEN                32

#define RERECIVE_MODE_OPEN

typedef enum {

	//	author : william
	//	since : 20110830
	//	(C)NON TEST

	//卡1
	I51_SIM_CARD_1 ,
	
	//卡2
	I51_SIM_CARD_2 ,
	
	//卡3
	I51_SIM_CARD_3 ,
	
	//卡4
	I51_SIM_CARD_4 ,


} ENUM_SIM_CARD ;

typedef enum {

	//	author : william
	//	since : 20110830
	//	(C)NON TEST

	//cmwap 网络
	I51_NET_CONNCT_CMWAP = 1 ,
	
	//cmnet 网络
	I51_NET_CONNCT_CMNET ,
	
	// 3g网络
	I51_NET_CONNCT_3G ,
	
	//wifi
	I51_NET_CONNCT_WIFI 
	
}ENUM_NET_CONNCT_MODE ;

typedef struct {

	//	author : william
	//	since : 20110719
	//	(C)NON TEST

	//socket 结构的定义

	//socket 索引
	iS32	index ;

	//soc id
	iS8	socId ;

	//是否被使用
	iBOOL	used ;

	//请求数据
	void *sendData ;
	
	//接受数据
	void *recvData ;

	iU32	sendLength ;
	//接受到数据的长度
	iU32	recvLength ;

	iU32	recvCount ;

	iU32 timerid ;
	
	//连接回调
	i51SOCCONCBK socConCb ;

	//发送回调
	i51SOCSENDCBK socSendCb ;

	//接受回调
	i51SOCRECVCBK	socRecvCb ;
	
	
} ADASOC ;

 struct i51SOCCALLBACKDATA{
	
	//	author : william
	//	since : 20111107
	//	(C)NON TEST

	//	保存回调参数，用于异步调用
	//	三种状态，连接，放松接收对应值分别为0  , 1 , 2
	iS32 netState ;
	iU32 timerid ;
//	iS32 socket ;
	struct {

		iS32 state ;
		i51SOCCONCBK callback ;
	}i51Connect ;

	struct {

		iS32 state ;
		iU32 textlen ;
		i51SOCSENDCBK callback ;
	}i51Send ;

	struct {
		
		iS32 state ;	
		iU8 *buffer ;
		iU32 bufferlen ;
		i51SOCRECVCBK callback ;
	}i51Receive ;

}i51SocCallbackData[MAX_SOCKET_COUNT] = {0} ;


/**
 * 数据账户结构体
 */
typedef struct 
{

	 /*!<账号名称, 必须为unicode编码*/
	unsigned char name[ACCOUNT_NAME_LEN];  

	  /*!<接入点名称, ASCII*/
	char apn[APN_LEN];                     

	 /*!<认证方式*/
	unsigned char auth_type;               

	 /*!<用户名, ASCII*/
	char user_name[USERNAME_LEN];          

	/*!<密码,ASCII*/
	char password[PASSWORD_LEN];            

	  /*!<DNS ip 地址*/
	unsigned char dns[DNSADDRESS_LEN];    

	 /*!<代理服务器*/
	unsigned char proxy[PROXY_LEN];        

	 /*!<端口*/
	unsigned short port;                   
	
}ADADataAccount;

//	打开链路回调
//typedef void (*i51SocOpenLinkLayerCallback)(iBOOL result) ;
struct {

	//	soc链路 结构

	//	联网模式1：CMWAP、2：CMNET、3：3G、4：WIFI。
	iU8 i51NetMode ;
	//	选择的sim卡
	iU8 i51ChooseSim  ;

	//	Jelo Edited Start 2011.12.15
	//创建链路的次数
     // iU32 i51CreateLinkCount ;
	//	Jelo Editing Finished
	
	//	链路是否已经存在
	iBOOL LinkExsit ;
	
	iBOOL used[MAX_LINKED_COUNT] ;
	//	创建链路链路id
	iU32 layerid ;
	//	回调内参数，每个回调对用自己的参数
	iBOOL CreateLayerState[MAX_LINKED_COUNT] ;
	//	回调都对用自己的timerid
	iU32 timerid[MAX_LINKED_COUNT] ;
	//	创建链路回调
	
	i51LLAYERCALLBACK callback[MAX_LINKED_COUNT] ;
	
	
}i51SocLinkLayer = {0} ;


//获取数据账户回调
typedef  void (*I51SOCFINDACONTINDEXCALLBACK) ( iS8 ret_index );

//解析域名是阻塞回调
typedef iS32 (*I51SOCGETHOSTNAMENOTIFYCALLBACK) ( void * ) ;

//联网数据账户
static iS32	i51NWKAcountId = -1 ;

//查找数据账户索引回调
static I51SOCFINDACONTINDEXCALLBACK i51findAcountIndexFunPtr ;

static i51ADECALLBACK i51AdaCreateCB ;

//查找的网络类型
static iCHAR * i51apnStr;

//数组账户索引值
static  iS8 i51AcountIndex = 0 ;



static const iU8 i51CmwapDefaultIndex = 3; 
static const iU8 i51CmnetDefaultIndex = 4; 

//socket 列表
static ADASOC adaSocList[MAX_SOCKET_COUNT] = { 0 } ;

//soc的最大数
static iS32	maxSocketCount = MAX_SOCKET_COUNT ;


//存放域名转换得到的ip地址
static iCHAR	DnsAddress[28] = { 0 } ;


//获取域名上层注册回调
static i51DNSCALLBACK i51DnsCB ;

//	Jelo Added Since 2011.12.15
#ifdef i51PROFILINGS
iU32 i51AdeSocGetRatioCount = 0 ;
#endif
//	Jelo Adding Finished

//声请应用id 用来保存链路
#if(I51_MTK_VS >=VS_08B)
static iU8 i51AppId = 0 ;
#else
#define i51AppId	0 ;
#endif

i51LLAYERCALLBACK i51SetLinkModeCB ;


#if(I51_MTK_VS <=VS_0812)
void i51AdeSingleNetHandler(void){}

void i51AdeSafeClearNetHandler(U16 EventID)
{
    mmi_frm_clear_protocol_event_handler( EventID); 
}
#else
	#if(I51_MTK_VS ==VS_08B)
	#include "FrameworkProt.h"
	#else
	#include "mmi_frm_events.h"
	#endif
extern const mmi_frm_event_static_info_struct g_event_table_static_info[EVENT_TABLE_END];
extern mmi_frm_event_runtime_info_struct g_event_table_runtime_info[EVENT_TABLE_END];
void i51AdeSingleNetHandler(void)
{
    S16 idx ;
    S16 maxEvents = g_event_table_runtime_info[EVENT_TABLE_MASTER].max_events;
    PseventInfo *eventTable = g_event_table_static_info[EVENT_TABLE_MASTER].table;

    for (idx = 0; idx < maxEvents; idx++)
    {
        if (eventTable[idx].eventID == MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND ||
            eventTable[idx].eventID == MSG_ID_APP_SOC_NOTIFY_IND )
        {
            if ( eventTable[idx].flagMulti ==0 )
            {   
                eventTable[idx].flagMulti =1;                
            }
        } 
    }
}

static BOOL i51AdeFindNetHandler(U16 EventID,PsFuncPtr* pFuncPtr, MMI_BOOL *pIsMultiInTable)
{
    S16 idx ;
    S16 maxEvents = g_event_table_runtime_info[EVENT_TABLE_MASTER].max_events;
    PseventInfo *eventTable = g_event_table_static_info[EVENT_TABLE_MASTER].table;

    for (idx = 0; idx < maxEvents; idx++)
    {
        if (eventTable[idx].eventID == EventID )
        {
            *pFuncPtr = eventTable[idx].entryFuncPtr;
            *pIsMultiInTable = eventTable[idx].flagMulti;
            return TRUE;
        } 
    }
    return FALSE;
}

void i51AdeSafeClearNetHandler(U16 EventID)
{
    PsFuncPtr FuncPtr;
    MMI_BOOL IsMultiInTable;
    while( i51AdeFindNetHandler( EventID, &FuncPtr, &IsMultiInTable) )
    {
        if(IsMultiInTable)
            mmi_frm_clear_protocol_event_handler( EventID, (PsIntFuncPtr)FuncPtr ); 
        else  mmi_frm_clear_protocol_event_handler( EventID, NULL );                  
    }
}

#endif


static int i51SetGprsDataAccount( signed char ItemIndex , ADADataAccount *pDataAccount ) ;
static int i51GetGprsDataAccount( signed char ItemIndex ) ;
static void i51AdeSocPlanAsynCallBack (iS32 socket) ;

static iU8 i51ConvetSimCard ( iS32 format)
{

	//	author : william
	//	since : 20110801
	//	PKIG Tech. Co., Ltd.
	
	switch ( format  ) {

		case 1 :
			return I51_SIM_CARD_1 ;
		case 2 :
			return I51_SIM_CARD_2 ;
		case 3 :
			return I51_SIM_CARD_3 ;
		case 4 :
			return I51_SIM_CARD_4 ;		
	
	}
	
} 
static iS32 i51AdeSocGetHostByNameNotify ( void *msg )
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
	
	 app_soc_get_host_by_name_ind_struct *dns_ind;
	 iU32 value ;

	 if (!msg) return MMI_FALSE;
	 dns_ind = ( app_soc_get_host_by_name_ind_struct * ) msg ;
	 if ( dns_ind->result == MMI_FALSE ) return MMI_FALSE ;

	 if ( dns_ind->request_id == DNS ) {

		 memset ( DnsAddress , 0 , sizeof ( DnsAddress ) ) ;
		 memcpy ( DnsAddress , dns_ind->addr , dns_ind->addr_len ) ;
		  sprintf(
                (iCHAR*)DnsAddress,
                "%d.%d.%d.%d",
                (iU8) * DnsAddress,
                (iU8) * (DnsAddress + 1),
                (iU8) * (DnsAddress + 2),
                (iU8) * (DnsAddress + 3));
		  
	i51AdeSocInetAton ( DnsAddress , &value ) ;
	if( iNULL != i51DnsCB )
		i51DnsCB ( value ) ;	
	 	
	 } else {
	 
		 return MMI_FALSE ;
		 
	 }

	 return MMI_TRUE ;

}

static iU8  i51CustomGetCsdProfileNum(void)
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
	
#if(I51_MTK_VS >=VS_W10)
    return 0;
#else 
    return CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM;
#endif
}

static iU8 i51CustomGetGprsProfileNum(void)
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
	
    return MAX_GPRS_PROFILE_NUM;
}

static void i51toLower(char *str)
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
	
    char *ptr;
    
    ptr = str;
    while (*ptr != 0)
    {
        if ((*ptr >= 'A') && (*ptr <= 'Z'))
        {
            *ptr += 'a' - 'A';
        }
        ptr++;
    }
    
}


/*
#ifndef VS_W11

static int i51SimCardSelect(int sim)
{

	//	author : william
	//	since : 20110719
	//	PKIG Tech. Co., Ltd.
	
#ifdef __MMI_DUAL_SIM_MASTER__
    char cards[2] = {0};
    switch (MTPNP_AD_Get_UsableSide())
    {
        case MTPNP_AD_DUALSIM_USABLE:
            cards[0] = cards[1] = 1;
            break;
        case MTPNP_AD_SIMCARD1_USABLE:
            cards[0] = 1;
            break;
        case MTPNP_AD_SIMCARD2_USABLE:
            cards[1] = 1;
            break;
    }
    sim &= 1;
    if (cards[0] && cards[1])
    {
        switch (MTPNP_AD_Get_Startup_Mode())
        {
            case MTPNP_AD_NVRAM_DUALCARD:
            case MTPNP_AD_NVRAM_CARD1:
                sim = 0;
                break;
            case MTPNP_AD_NVRAM_CARD2:
                sim = 1;
                break;
        }
    }
    return (cards[sim] ? sim : ((sim+1)&1));
#else
    return 0;
#endif
}

#else

static int i51SimCardSelect(int sim)
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.
	
#if (MMI_MAX_SIM_NUM >= 2)
    char cards[2] = {0,0};
    srv_mode_switch_type_enum  sw_mode = srv_mode_switch_get_current_mode();
    if( sw_mode == SRV_MODE_SWITCH_SIM_ALL )
    {
        cards[0] = cards[1] = 1;
    }else if( sw_mode == SRV_MODE_SWITCH_SIM1 )
    {
        cards[0] = 1;
    }else if( sw_mode == SRV_MODE_SWITCH_SIM2 )
    {
        cards[1] = 1;
    }
    if( srv_sim_ctrl_is_available(MMI_SIM1) && cards[0] )
    {
        return 0;
    }
    if( srv_sim_ctrl_is_available(MMI_SIM2) && cards[1] )
    {
        return 1;
    }  
    return 0;
#else
    return 0;
#endif
} 

#endif
*/

static void  i51GetAcountid(void)
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	//获取到数据账户

	//注册一个用户id保存链路
	#if(I51_MTK_VS >=VS_08B)
	   	 if ( i51AppId == 0) {
		 	
	        #if(I51_MTK_VS <VS_W10)
	        i51AppId = cbm_register_app_id(STR_GLOBAL_OK, IMG_GLOBAL_OK);	
	        #else
	        cbm_register_app_id(&i51AppId);	
	        #endif
			
	   	 }
	#endif

	#if(I51_MTK_VS <=VS_0812)
		i51NWKAcountId = always_ask_encode_data_account_id ( i51AcountIndex , i51SocLinkLayer.i51ChooseSim + 1 , 0 , 0 ) ;
	#else
		i51NWKAcountId = cbm_encode_data_account_id ( i51AcountIndex , i51SocLinkLayer.i51ChooseSim , i51AppId , 0 ) ;
	#endif

}


#if(I51_MTK_VS >=VS_W10)

extern mmi_dtcnt_list_disp_struct g_mmi_dtcnt_list_item[SRV_DTCNT_PROF_MAX_ACCOUNT_NUM];
static void  i51FindDataAccountIndexCB(void)
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

    if( i51findAcountIndexFunPtr ) 
        i51findAcountIndexFunPtr( i51AcountIndex );    
}
#else


static void i51SetGprsAccountRsp(void *info) 
{	


	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

   /**
     * @fn		static void i51SetGprsAccountRsp(void *info)
   */
    mmi_ps_set_gprs_data_account_rsp_struct *localPtr = (mmi_ps_set_gprs_data_account_rsp_struct*) info;


    if (info){
        if( localPtr->result == KAL_TRUE ){
            if( i51findAcountIndexFunPtr ) 
                i51findAcountIndexFunPtr( i51AcountIndex );
                return;
        }
    }
    if( i51findAcountIndexFunPtr ) 
        i51findAcountIndexFunPtr( -1 );    
}


static void i51GetGprsAccountRsp(void *info) 
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.


   /**
 * @fn		static void i51GetGprsAccountRsp(void *info)
 */
    mmi_ps_get_gprs_data_account_rsp_struct *localPtr;
    char apn[20];

    if (!info) return;

    localPtr = (mmi_ps_get_gprs_data_account_rsp_struct*) info;

    strcpy(apn, (char*) localPtr->gprs_account.apn );
    i51toLower(apn);
    
	if( strcmp( apn, "uniwap") == 0 )
		strcpy( apn, "cmwap" );
    if( strcmp( i51apnStr, apn ) == 0 )
    {
        if( i51findAcountIndexFunPtr ) 
            i51findAcountIndexFunPtr(i51AcountIndex);
        return;
    }
    i51AcountIndex ++;
    if( i51AcountIndex >= i51CustomGetGprsProfileNum() ){
         ADADataAccount ll_da;

        memset(&ll_da, 0, sizeof(ADADataAccount));
        strcpy(ll_da.apn, i51apnStr);
        if( strcmp( i51apnStr, "cmwap")==0 ){
            mmi_ucs2cpy((S8*)ll_da.name, (const S8*)L"ChinaMobile CMWAP");
            i51AcountIndex = i51CmwapDefaultIndex;
        }else if( strcmp( i51apnStr, "cmnet")==0 ){
            mmi_ucs2cpy((S8*)ll_da.name, (const S8*)L"ChinaMobile CMNET");
            i51AcountIndex = i51CmnetDefaultIndex;                     
        }
        i51SetGprsDataAccount (i51AcountIndex, &ll_da);
        return;
    }
    i51GetGprsDataAccount(i51AcountIndex);
    
}



static int i51SetGprsDataAccount(signed char ItemIndex, ADADataAccount *pDataAccount)
{


	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

   /**
 * @fn  int i51SetGprsDataAccount(signed char itemindex, DataAccount* pDataAccount)
 * @brief	设置数据帐户
 * 
 * @retval  0   成功
 * @retval  <0   失败
 */

   
    mmi_ps_set_gprs_data_account_req_struct *myMsgPtr;
    U8 ref_count;
    U16 msg_len;
    MYQUEUE SendMessage;


    if (ItemIndex >= MAX_DATA_ACCOUNT_GPRS_LIMIT || pDataAccount == iNULL ) return -1; //注:要包含头文件custom_data_account.h

    myMsgPtr = (mmi_ps_set_gprs_data_account_req_struct*) OslConstructDataPtr(sizeof(mmi_ps_set_gprs_data_account_req_struct));

    ref_count = myMsgPtr->ref_count;
    msg_len   = myMsgPtr->msg_len;
    memset(myMsgPtr, '\0', sizeof(mmi_ps_set_gprs_data_account_req_struct));
    myMsgPtr->ref_count = ref_count;
    myMsgPtr->msg_len = msg_len;

    myMsgPtr->gprs_account.context_id = ItemIndex + 1;
    myMsgPtr->gprs_account.authentication_type = pDataAccount->auth_type;

    //set da name
    myMsgPtr->gprs_account.name_length = mmi_ucs2strlen((PS8)pDataAccount->name) * 2;;
    myMsgPtr->gprs_account.name_dcs = DATA_ACCOUNT_UCS; /* 0x8 */
    if (myMsgPtr->gprs_account.name_length > 0)
    {
        int index = 0;        
        while (index < myMsgPtr->gprs_account.name_length)
        {
            myMsgPtr->gprs_account.name[index] = pDataAccount->name[index + 1];
            myMsgPtr->gprs_account.name[index + 1] = pDataAccount->name[index];
            index += 2;
        }
        myMsgPtr->gprs_account.name[index] = 0x00;
        myMsgPtr->gprs_account.name[index+ 1]  = 0x00;
    }
    
    //set da apn
    strcpy((char *) myMsgPtr->gprs_account.apn, (const char *) pDataAccount->apn);
    myMsgPtr->gprs_account.apn_length = strlen((const char *) pDataAccount->apn);

    //set da username, passwd, dns
    strcpy((char *) myMsgPtr->gprs_account.user_name, pDataAccount->user_name);
    strcpy((char *) myMsgPtr->gprs_account.password, pDataAccount->password);
    memcpy((void*)myMsgPtr->gprs_account.dns, (void*)pDataAccount->dns, DNSADDRESS_LEN);

    #if( (I51_MTK_VS >=VS_08B)&&(I51_MTK_VS <=VS_09B) )
    myMsgPtr->gprs_account.read_only = 0;
    #ifdef __MANUAL_CACHE__ //6235_09A mmi_ps_set_gprs_data_account_req_struct 没有下面成员
    /* revised for GPRS profile default value */
    myMsgPtr->gprs_account.dcomp_algo = 0x02;
    myMsgPtr->gprs_account.hcomp_algo = 0x02;
    myMsgPtr->gprs_account.pdp_type = 0x21;
    myMsgPtr->gprs_account.pdp_addr_len = 0x01; 
    #endif  
    #endif

    #if( (I51_MTK_VS >=VS_08B)&&(I51_MTK_VS <=VS_09B) )
    myMsgPtr->profile_type = DATA_ACCT_GPRS_PROF;
    #endif

    SendMessage.oslSrcId = MOD_MMI;
    SendMessage.oslDestId = MOD_L4C;
    SendMessage.oslMsgId = PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ;
    SendMessage.oslDataPtr = (oslParaType*) myMsgPtr;
    SendMessage.oslPeerBuffPtr = iNULL;
    SetProtocolEventHandler(i51SetGprsAccountRsp, PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP);
    OslMsgSendExtQueue(&SendMessage);

    return 0;
}

static int i51GetGprsDataAccount(signed char ItemIndex)
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

    
/**
 * @fn  int i51GetGprsDataAccount(unsigned char ItemIndex)
 * @brief	获取数据帐户
 * 
 * @retval  0   成功
 * @retval  <0   失败
 */
 
    mmi_ps_get_gprs_data_account_req_struct *myMsgPtr;
    MYQUEUE SendMessage;


    if (ItemIndex >= MAX_DATA_ACCOUNT_GPRS_LIMIT ) return -1;

    myMsgPtr = (mmi_ps_get_gprs_data_account_req_struct*) OslConstructDataPtr(sizeof(mmi_ps_get_gprs_data_account_req_struct));

    myMsgPtr->profile_id = ItemIndex + 1;

    #if( (I51_MTK_VS >=VS_08B)&&(I51_MTK_VS <=VS_09B) )
    myMsgPtr->profile_type = 1; //DATA_ACCT_GPRS_PROF;
    #endif

    SendMessage.oslSrcId = MOD_MMI;
    SendMessage.oslDestId = MOD_L4C;
    SendMessage.oslMsgId = PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ;
    SendMessage.oslDataPtr = (oslParaType*) myMsgPtr;
    SendMessage.oslPeerBuffPtr = iNULL;

    SetProtocolEventHandler (i51GetGprsAccountRsp, PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP);
    OslMsgSendExtQueue(&SendMessage);

    return 0;
}

#endif



static void  i51AdeSoc_I51SOCFINDACONTINDEXCALLBACK_TIMERCB ( iU32 id )
{

	i51AdeOsTimerStop(id) ;
	i51FindDataAccountIndexCB () ;
}
static iBOOL  i51AdeSoc_ASYN_I51SOCFINDACONTINDEXCALLBACK ()
{

	//	 回调，异步通知

	iU32 timerid = 0 ;
	timerid = i51AdeOsTimerCreate ( 0 , 1 ) ;
	if ( 0 == timerid )
	{
		i51AdeOsLog ( 0 , "SOC : i51AdeSoc_ASYN_I51SOCFINDACONTINDEXCALLBACK , TC " ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart (timerid , i51AdeSoc_I51SOCFINDACONTINDEXCALLBACK_TIMERCB , 1 ) )
	{
		i51AdeOsLog ( 0 , "SOC : i51AdeSoc_ASYN_I51SOCFINDACONTINDEXCALLBACK , TS F" ) ;		
		return iFALSE ;
	}		
}




 void  i51AdeSocFindAcountIndex( char*  apnStr, I51SOCFINDACONTINDEXCALLBACK callback )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	//	author : william
	//	since : 20110719
	//	(C)NON TEST

	//查找数据账户索引
	
    iASSERT( apnStr && callback );
	i51findAcountIndexFunPtr = callback;
	i51apnStr = apnStr;
#if( I51_MTK_VS >=VS_W10 )
    {
        int i;
       // int selectSim = i51SimCardSelect(0) + 1;
        BOOL findPoxy ;
        U8  tempName[SRV_DTCNT_PROF_MAX_APN_LEN+1];

        i51AcountIndex = -1;
        strcpy(tempName, apnStr );
        i51toLower(tempName);
        if( strcmp("cmwap", tempName) ==0 ) findPoxy = TRUE;
        else  if( strcmp("cmnet", tempName) ==0 ) findPoxy = FALSE;
        else  goto FIND_END;
       
        for (i = 0; i < SRV_DTCNT_PROF_MAX_ACCOUNT_NUM; i++)
        {   
            srv_dtcnt_account_info_st *pi;
            pi = &g_srv_dtcnt_store_ctx.acc_list[i];
            
            if (pi->bearer_type == SRV_DTCNT_BEARER_GPRS && pi->sim_info == i51SocLinkLayer.i51ChooseSim+1 )            
            {      
                strcpy( tempName, g_srv_dtcnt_store_ctx.acc_list[i].dest_name );
                i51toLower(tempName);
                if(findPoxy)
                {
                    if( strcmp("cmwap", tempName) ==0 || strcmp("uniwap", tempName) ==0 )
                    { 
                        i51AcountIndex = pi->acc_id;
                        break;
                    }
                }else
                {   
                    if( strcmp("cmnet", tempName) ==0 || strcmp("uninet", tempName) ==0 )
                    { 
                        i51AcountIndex = pi->acc_id;
                        break;
                    }
                }
            }
        }
FIND_END:
    	i51AdeSoc_ASYN_I51SOCFINDACONTINDEXCALLBACK () ;
     }
#else
    i51AcountIndex = 0;
	i51GetGprsDataAccount( 0 );
#endif
}


void i51AdeSocNotify(void *message)
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	iU16	i ;
	app_soc_notify_ind_struct *soc_notify= (app_soc_notify_ind_struct *)message;

	i51AdeOsLog ( 0 , "SOC : i51AdeSocNotify , S " ) ;
	for ( i = 0 ; i < maxSocketCount ; i++ ) {

		if ( adaSocList[i].used == iTRUE && soc_notify ->socket_id == adaSocList[i].socId ) {			
			break ;
		}
	}

	if ( i == maxSocketCount )	return  ;
   
	i51AdeOsLog ( 0 , "SOC : i51AdeSocNotify , soc id = %d, event_type = %d" , i, soc_notify->event_type) ;
	switch(soc_notify->event_type)
	{
	
		case SOC_CONNECT:
		{
			i51SocCallbackData[i].netState = 0 ;
			if(soc_notify->result) {

				i51SocCallbackData[i].i51Connect.state = iTRUE;	
				i51AdeSocPlanAsynCallBack (i) ;
//				if ( adaSocList[i].socConCb )
//					adaSocList[i].socConCb ( iTRUE , i ) ;	
			}else {

				i51SocCallbackData[i].i51Connect.state = iFALSE;	
				i51AdeSocPlanAsynCallBack (i) ;
//				if ( adaSocList[i].socConCb )
//					adaSocList[i].socConCb ( iFALSE , i ) ;
//				i51AdeSocClose ( i ) ;	

			}
			break;
			
		}

		case SOC_WRITE:
		{
#if 0
			i51SocCallbackData[i].netState = 1 ;
			if(soc_notify->result) {

				i51AdeSocSend ( i , \
								adaSocList[i].sendData , \
								adaSocList[i].sendLength , \
								adaSocList[i].socSendCb) ; 
			}else {

				i51SocCallbackData[i].i51Send.state = iFALSE;	
				i51AdeSocPlanAsynCallBack (i) ;
//				if ( adaSocList[i].socSendCb )
//					adaSocList[i].socSendCb ( iFALSE , i , 0  ) ;
//				i51AdeSocClose ( i ) ;	
			}
#endif
			break;
		}

		case SOC_READ:
		{
#if 0			
			i51SocCallbackData[i].netState = 2 ;
			#ifndef RERECIVE_MODE_OPEN
			if(soc_notify->result) {

			if ( adaSocList[i].socRecvCb )
				i51AdeSocReceive( i , \
								adaSocList[i].recvData, \
								adaSocList[i].recvLength , \
								adaSocList[i].socRecvCb) ; 		
			}else {

				i51SocCallbackData[i].i51Receive.state = iFALSE;	
				i51AdeSocPlanAsynCallBack (i) ;			
//				if ( adaSocList[i].socRecvCb )
//					adaSocList[i].socRecvCb ( iFALSE , i  , iNULL , 0) ;		
//				i51AdeSocClose ( i ) ;		
			}
			#endif
#endif			
			break;	
		}

		case SOC_CLOSE:
		{
			i51AdeOsLog ( 0 , "SOC : i51AdeSocNotify , SOC_CLOSE  %d\r\n" , i ) ;	
			i51AdeSocClose ( i ) ;
			break;
			
		}
		default:
		{
			i51AdeOsLog ( 0 , "SOC : i51AdeSocNotify , default  %d\r\n" , i ) ;	
			i51AdeSocClose ( i ) ;
			break;
			
		}
	}
    i51AdeOsLog ( 0 , "SOC : i51AdeSocNotify , E" ) ;
}

static void i51AdeSocNotifyIND ( void ) 
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

   #if( I51_MTK_VS >=VS_08B )
	#ifdef i51_MULTI_MODE
	   mmi_frm_set_protocol_event_handler( MSG_ID_APP_SOC_NOTIFY_IND , (PsIntFuncPtr) i51AdeSocNotify , MMI_TRUE ) ;
	#else
	  SetProtocolEventHandler ( i51AdeSocNotify , MSG_ID_APP_SOC_NOTIFY_IND ) ;
	#endif
  #else
	  SetProtocolEventHandler ( i51AdeSocNotify , MSG_ID_APP_SOC_NOTIFY_IND ) ;
  #endif
}

static void i51AdeSocExitNotify ( void )
{

	//	author : william
	//	since : 20110915
	//	PKIG Tech. Co., Ltd.

  #if( I51_MTK_VS >=VS_08B )
	#ifdef i51_MULTI_MODE
		mmi_frm_clear_protocol_event_handler ( MSG_ID_APP_SOC_NOTIFY_IND ,(PsIntFuncPtr) i51AdeSocNotify ) ;
		mmi_frm_clear_protocol_event_handler ( MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND , (PsIntFuncPtr)i51AdeSocGetHostByNameNotify ) ;
	#else
		ClearProtocolEventHandler( MSG_ID_APP_SOC_NOTIFY_IND ) ;
		ClearProtocolEventHandler( MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND ) ;
	#endif
  #else
		ClearProtocolEventHandler( MSG_ID_APP_SOC_NOTIFY_IND ) ;
		ClearProtocolEventHandler( MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND ) ;
  #endif

}

static char *l_aSocStateStr[3]={"connect","send","revceive"};
static void i51AdeSocAsynCallBack1 ( iU32 id )
{

	//	author : william
	//	since : 20111107
	//	PKIG Tech. Co., Ltd.
	//	第1个soc
	
    i51AdeOsLog(0, "SOC : i51AdeSocAsynCallBack1 , call soc %d  %s cb", 0, l_aSocStateStr[i51SocCallbackData[0].netState] );
	if ( id )
		i51AdeOsTimerStop (id) ;
    if( adaSocList[0].used == 0 ) return;
	
	if ( i51SocCallbackData[0].netState == 0 ) {

		//	连接时
		 i51SocCallbackData[0].i51Connect.callback ( i51SocCallbackData[0].i51Connect.state , 0) ;

	} 
	else if ( i51SocCallbackData[0].netState == 1 ) {

		//	发送时
		 i51SocCallbackData[0].i51Send.callback ( i51SocCallbackData[0].i51Send.state , 0 ,  i51SocCallbackData[0].i51Send.textlen) ;
	} 
	else if ( i51SocCallbackData[0].netState == 2 ) {

		i51SocCallbackData[0].i51Receive.callback ( i51SocCallbackData[0].i51Receive.state ,
												0 ,
												i51SocCallbackData[0].i51Receive.buffer ,   
												i51SocCallbackData[0].i51Receive.bufferlen ) ;

	} 

}

static void i51AdeSocAsynCallBack2 ( iU32 id )
{
	//	author : william
	//	since : 20111107
	//	PKIG Tech. Co., Ltd.
	//	第2个soc
	
    i51AdeOsLog(0, "SOC : i51AdeSocAsynCallBack2 , call soc %d  %s cb", 1, l_aSocStateStr[i51SocCallbackData[1].netState] );
	if ( id )
		i51AdeOsTimerStop (id) ;
    if( adaSocList[1].used == 0 ) return;

	if ( i51SocCallbackData[1].netState == 0 ) {

		//	连接时
		 i51SocCallbackData[1].i51Connect.callback ( i51SocCallbackData[1].i51Connect.state , 1) ;

	} 
	else if ( i51SocCallbackData[1].netState == 1 ) {

		//	发送时
		 i51SocCallbackData[1].i51Send.callback ( i51SocCallbackData[1].i51Send.state , 1 , i51SocCallbackData[1].i51Send.textlen) ;
	} 
	else if ( i51SocCallbackData[1].netState == 2 ) {

		//	接收时
		i51SocCallbackData[1].i51Receive.callback ( i51SocCallbackData[1].i51Receive.state ,
												1 ,
												i51SocCallbackData[1].i51Receive.buffer , 
												i51SocCallbackData[1].i51Receive.bufferlen ) ;
	} 


}

static void i51AdeSocAsynCallBack3 ( iU32 id )
{
	//	author : william
	//	since : 20111107
	//	PKIG Tech. Co., Ltd.
	//	第3个soc	
    i51AdeOsLog(0, "SOC : i51AdeSocAsynCallBack3 , call soc %d  %s cb", 2, l_aSocStateStr[i51SocCallbackData[2].netState] );
	if ( id )
		i51AdeOsTimerStop (id) ;
    if( adaSocList[2].used == 0 ) return;

	if ( i51SocCallbackData[2].netState == 0 ) {

		//	连接时
		 i51SocCallbackData[2].i51Connect.callback ( i51SocCallbackData[2].i51Connect.state , 2) ;

	} 
	else if ( i51SocCallbackData[2].netState == 1 ) {

		//	发送时
		 i51SocCallbackData[2].i51Send.callback ( i51SocCallbackData[2].i51Send.state , 2 , i51SocCallbackData[2].i51Send.textlen) ;
	} 
	else if ( i51SocCallbackData[2].netState == 2 ) {

		//	接收时
		i51SocCallbackData[2].i51Receive.callback ( i51SocCallbackData[2].i51Receive.state ,
												2 ,
												i51SocCallbackData[2].i51Receive.buffer , 
												i51SocCallbackData[2].i51Receive.bufferlen ) ;
	} 


}

static void i51AdeSocAsynCallBack4 ( iU32 id )
{
	//	author : william
	//	since : 20111107
	//	PKIG Tech. Co., Ltd.
	//	第4个soc	
    i51AdeOsLog(0, "SOC : i51AdeSocAsynCallBack4 , call soc %d  %s cb", 3, l_aSocStateStr[i51SocCallbackData[3].netState] );
	if ( id )
		i51AdeOsTimerStop (id) ;
    if( adaSocList[3].used == 0 ) return;

	if ( i51SocCallbackData[3].netState == 0 ) {

		//	连接时
		 i51SocCallbackData[3].i51Connect.callback ( i51SocCallbackData[3].i51Connect.state , 3) ;

	} 
	else if ( i51SocCallbackData[3].netState == 1 ) {

		//	发送时
		 i51SocCallbackData[3].i51Send.callback ( i51SocCallbackData[3].i51Send.state , 3 , i51SocCallbackData[3].i51Send.textlen) ;
	} 
	else if ( i51SocCallbackData[3].netState == 2 ) {

		//	接收时
		i51SocCallbackData[3].i51Receive.callback ( i51SocCallbackData[3].i51Receive.state ,
												3 ,
												i51SocCallbackData[3].i51Receive.buffer , 
												i51SocCallbackData[3].i51Receive.bufferlen ) ;
	} 


}

static void i51AdeSocAsynCallBack5 ( iU32 id )
{
	//	author : william
	//	since : 20111107
	//	PKIG Tech. Co., Ltd.
	//	第5个soc	
    i51AdeOsLog(0, "SOC : i51AdeSocAsynCallBack5 , call soc %d  %s cb", 4, l_aSocStateStr[i51SocCallbackData[4].netState] );
	if ( id )
		i51AdeOsTimerStop (id) ;
    if( adaSocList[4].used == 0 ) return;

	if ( i51SocCallbackData[4].netState == 0 ) {

		//	连接时
		 i51SocCallbackData[4].i51Connect.callback ( i51SocCallbackData[4].i51Connect.state , 4) ;

	} 
	else if ( i51SocCallbackData[4].netState == 1 ) {

		//	发送时
		 i51SocCallbackData[4].i51Send.callback ( i51SocCallbackData[4].i51Send.state , 4 , i51SocCallbackData[4].i51Send.textlen) ;
	} 
	else if ( i51SocCallbackData[4].netState == 2 ) {

		//	接收时
		i51SocCallbackData[4].i51Receive.callback ( i51SocCallbackData[4].i51Receive.state ,
												4 ,
												i51SocCallbackData[4].i51Receive.buffer , 
												i51SocCallbackData[4].i51Receive.bufferlen ) ;
	} 


}
static void i51AdeSocPlanAsynCallBack (iS32 socket)
{

	//	author : william
	//	since : 20111107
	//	PKIG Tech. Co., Ltd.
	
	//	以异步方式通知上层做准备
	//	接受时，一定是异步，不需要在异步

	if ( socket == 0 ) {

		if ( i51SocCallbackData[0].netState != 2 )
			i51AdeOsTimerStart (i51AdeOsTimerCreate (0, 1), i51AdeSocAsynCallBack1,1) ;
		else
			i51AdeSocAsynCallBack1 (0) ;
	}
	else if ( socket == 1 ) {
		
		if ( i51SocCallbackData[1].netState != 2 )
			i51AdeOsTimerStart (i51AdeOsTimerCreate (0, 1), i51AdeSocAsynCallBack2,1) ;
		else		
			i51AdeSocAsynCallBack2 (0) ;
	}

	else if ( socket == 2 ) {

		if ( i51SocCallbackData[2].netState != 2 )
			i51AdeOsTimerStart (i51AdeOsTimerCreate (0, 1), i51AdeSocAsynCallBack3,1) ;
		else		
			i51AdeSocAsynCallBack3 (0) ;
	}

	else if ( socket == 3 ) {

		if ( i51SocCallbackData[3].netState != 2 )
			i51AdeOsTimerStart (i51AdeOsTimerCreate (0, 1), i51AdeSocAsynCallBack4,1) ;
		else		
			i51AdeSocAsynCallBack4 (0) ;
	}

	else if ( socket == 4 ) {

		if ( i51SocCallbackData[4].netState != 2 )
			i51AdeOsTimerStart (i51AdeOsTimerCreate (0, 1), i51AdeSocAsynCallBack5,1) ;
		else	
			i51AdeSocAsynCallBack5 (0) ;
	}
}

static void  i51AdeSoc_i51ADECALLBACK_TIMERCB ( iU32 id )
{

	i51AdeOsTimerStop(id) ;
	i51AdaCreateCB (iTRUE) ;
}
static iBOOL  i51AdeSoc_ASYN_i51ADECALLBACK ()
{

	//	 回调，异步通知

	iU32 timerid = 0 ;
	timerid = i51AdeOsTimerCreate ( 0 , 1 ) ;
	if ( 0 == timerid )
	{
		i51AdeOsLog ( 0 , "SOC : i51AdeSoc_ASYN_i51ADECALLBACK , TC " ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart (timerid , i51AdeSoc_i51ADECALLBACK_TIMERCB , 1 ) )
	{
		i51AdeOsLog ( 0 , "SOC : i51AdeSoc_ASYN_i51ADECALLBACK , TS F" ) ;		
		return iFALSE ;
	}		
}

//	Jelo Added Since 2011.12.15
#define i51AdeSocAidTableLength 5
static iU32 i51AdeSocAidTable [ i51AdeSocAidTableLength ]  = { 0 } ;
static iU32 i51AdeSocLastAddAid = 0 ;

static iBOOL i51AdeSocAidTableEmpty () {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	判断应用链路表是否空。
	
	iBOOL empty = iTRUE ;
	iU32 looper = 0 ;
	
	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( 0 != i51AdeSocAidTable[looper] ) {
			empty = iFALSE ;
		}
		
	}
	
	return empty ;
	
}

static iBOOL i51AdeSocAidTableFull () {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	判断应用链路表是否满。
	
	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( 0 == i51AdeSocAidTable[looper] ) {
			return iFALSE ;
		}
		
	}

	return iTRUE ;
	
}

static iBOOL i51AdeSocAidTableAdd ( iU32 aid ) {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	添加应用链路表项目。
	
	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( 0 == i51AdeSocAidTable[looper] ) {
			i51AdeSocAidTable[looper] = aid ;
			return iTRUE ;
		}
		
	}

	return iTRUE ;
	
}

static iBOOL i51AdeSocAidTableDelete ( iU32 aid ) {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	删除应用链路表项目。
	
	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( aid == i51AdeSocAidTable[looper] ) {
			i51AdeSocAidTable[looper] = 0 ;
			return iTRUE ;
		}
		
	}
	
	return iTRUE ;
	
}

static iBOOL i51AdeSocAidTableFind ( iU32 aid ) {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	查找应用链路表项目。
	
	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( aid == i51AdeSocAidTable[looper] ) {
			return iTRUE ;
		}
		
	}
	
	return iFALSE ;
	
}

static iBOOL i51AdeSocAidTableClear () {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	清空应用链路表。

	i51AdeStdMemset16 ( i51AdeSocAidTable , 0 , sizeof(i51AdeSocAidTable) ) ;

	return iTRUE ;
	
}
//	Jelo Adding Finished


static void  i51getAcountIndexCB (  iS8 ret_index )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.a	
	if (  ret_index >= 0 )  {
		
		i51AcountIndex = ret_index + i51CustomGetCsdProfileNum () ;
		i51GetAcountid () ;	

		//	Jelo Adding Start 2011.12.15
		//	将Aid 创建链路的情况记录下来
		i51AdeSocAidTableAdd ( i51AdeSocLastAddAid ) ;
		i51AdeSocLastAddAid = 0 ;
		//	Jelo Adding Finished
		
		i51SocLinkLayer.LinkExsit = iTRUE ;

		//	Jelo Edited Start 2011.12.15
		//	i51SocLinkLayer.i51CreateLinkCount ++ ;
		//	Jelo Editing Finished
		
		if (i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1])
			i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1](iTRUE) ;
		
		
	}  else  {		
		if (i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1])
			i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1](iFALSE) ;	
		i51AdeSocLastAddAid = 0 ;
		i51SocLinkLayer.layerid = 0 ;		
	}	
}

iU32 i51AdeSocHtonl ( iU32 value )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.


	//三十二位长整数转换为网络序

	iU32 l32netvalue = 0 ;
	
#if( I51_MTK_VS >VS_0812 )
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocHtonl , S , %x" , value ) ;
	#endif	

	l32netvalue = soc_htonl( value ) ;
#else
    U8 *buf = (U8*)&value;
    U8 t;
    
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocHtonl , S , %x" , value ) ;
	#endif	
    
    t = buf[0];
    buf[0]=buf[3];
    buf[3]=t;
    t = buf[1];
    buf[1]=buf[2];
    buf[2]=t;  
    l32netvalue = value;  
#endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocHtonl , E , ret = %x" , l32netvalue ) ;
	#endif	

    return l32netvalue;
}
typedef unsigned char   U8;

iU16 i51AdeSocHtons ( iU16 value )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	//十六位短整数转换为网络序

	iU16 l16netvalue = 0 ;
		
#if( I51_MTK_VS >VS_0812 )
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocHtons , S , %x" , value ) ;
	#endif	

	l16netvalue = soc_htons( value ) ;
#else
    U8 *buf = (U8*)&value;
    U8 t;

    #ifdef i51ADAPTER_DEBUG
    i51AdeOsLog ( 0 , "SOC : i51AdeSocHtons , S , %x" , value ) ;
    #endif	

    t = buf[0];
    buf[0]=buf[1];
    buf[1]=t;
    l16netvalue = value;  
#endif

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocHtons , E , ret = %x" , l16netvalue ) ;
	#endif	
    return l16netvalue;
}
 iBOOL i51AdeSocInetAton ( iCHAR* ip , iU32* value )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	//字符串IP地址转换为数值

	iCHAR ipAddress[16] = { 0 } ;
	iCHAR valueAddress[4] = { 0 } ;
	iCHAR *startStr ;
	iCHAR  *endStr ;
	iU32 splitValue[4] = {0} ;
	kal_bool ipValidity = FALSE;
	iU16	i ;
	iU16 j = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocInetAton , S , %x , %x" , ip , value ) ;
	#endif	
	
	if(ip[0] == '.') {

		i51AdeOsLog ( 0 , "SOC : i51AdeSocInetAton , BE1.1" ) ;
		return iFALSE;
	}

	if ((soc_ip_check( ip , ( iU8 * ) valueAddress , &ipValidity) == FALSE) || (ipValidity == FALSE))
	{
		 i51AdeOsLog ( 0 , "SOC : i51AdeSocInetAton , BE1.2" ) ;
	    return iFALSE;
	}

	startStr = ip ;
	
	for (  i = 0 ; i < 3 ; i++ ) {

		memset ( valueAddress , 0 , sizeof ( valueAddress ) ) ;
		endStr = strstr(startStr, ".") ;
		memcpy ( valueAddress , startStr , endStr - startStr ) ;
		splitValue[i] = atoi ( valueAddress ) << ( 8 * ( 3 - i) ) ;
		startStr = endStr + 1 ;
		
	}
	
	memset ( valueAddress , 0 , sizeof ( valueAddress ) ) ;
	strcpy ( valueAddress , startStr ) ;
	splitValue[i] = atoi ( valueAddress )  ;

	endStr = strstr(startStr, ".") ;
	//查找到3个'.'还查找到了点，那么这是不合法的
	if ( iNULL != endStr )	return iFALSE ;  
	*value = splitValue[0] + splitValue[1] + splitValue[2] + splitValue[3] ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocInetAton , E , ret = %x" , iTRUE ) ;
	#endif	

	return iTRUE ;
}

	
 iS32 i51AdeSocGetHostByName ( iCHAR* name  ,i51DNSCALLBACK callback )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.


	iS32	result = 0 ;
	iS32 addressLength = 0 ;
	iU32 value = 0 ;
	iS32 GetHostbyNameRet = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocGetHostByName , S , %x , %x" , name , callback ) ;
	#endif	
	
	iASSERT ( name && callback ) ;
	i51DnsCB = callback ;
	memset ( DnsAddress , 0 , sizeof ( DnsAddress ) ) ;
	result = soc_gethostbyname (  KAL_FALSE ,
            							MOD_MMI ,
            							DNS , 
            							name , 
            							( iU8* )DnsAddress ,( iU8* ) &addressLength , 0 , i51NWKAcountId ) ;

	if ( SOC_SUCCESS == result ) {
		  sprintf(
                (iCHAR*)DnsAddress,
                "%d.%d.%d.%d",
                (iU8) * DnsAddress,
                (iU8) * (DnsAddress + 1),
                (iU8) * (DnsAddress + 2),
                (iU8) * (DnsAddress + 3));
		  
	i51AdeSocInetAton ( DnsAddress , &value ) ;
	if( iNULL != i51DnsCB )
		i51DnsCB ( value ) ;	
	GetHostbyNameRet =  ADA_SOC_SUCCESS ;
		
		

	} else if ( SOC_WOULDBLOCK == result ) {

#if( I51_MTK_VS >VS_0812 )
	mmi_frm_set_protocol_event_handler( MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND , (PsIntFuncPtr) i51AdeSocGetHostByNameNotify ,MMI_TRUE); 
#else
	mmi_frm_set_protocol_event_handler( (PsFuncPtr) i51AdeSocGetHostByNameNotify , MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
#endif
	GetHostbyNameRet =  ADA_SOC_DELAY ;

	}  else {
	
	GetHostbyNameRet =  ADA_SOC_FAILED ;
	}

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocGetHostByName , E , ret = %x" , GetHostbyNameRet ) ;
	#endif	

	return GetHostbyNameRet;
}

iS32 i51AdeSocGet ( void )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	
	iU16	i = 0 ;
	iS32 result = 0 ;
	iS8 value = 0  ;

	i51AdeOsLog ( 0 , "SOC : i51AdeSocGet , S") ;	
	
	do
	{
		for ( i = 0 ; i < maxSocketCount ; i ++ ) {

			if ( !adaSocList[i].used)	break ;
			
		}

		if( i == maxSocketCount ) 	{
			
			result =  -1 ;
			break ;
			
		}

		result = i ;
		adaSocList[i].index = 0 ;

		#ifndef MMI_ON_HARDWARE_P
			soc_init_win32() ;
		#endif
		
		adaSocList[i].socId = soc_create ( 0 , 0 , 0 , MOD_MMI , i51NWKAcountId ) ;
        
        i51AdeOsLog ( 0 , "SOC : i51AdeSocGet , sys soc id = %d" , adaSocList[i].socId ) ;	
		
		if ( adaSocList[i].socId < 0 ) {
			
			result =  -1 ;
			break ;
			
		}

	   value = 1 ;
	    if ( soc_setsockopt (adaSocList[i].socId , SOC_NBIO, &value, sizeof(value)) < 0) { 
			
	        soc_close ( adaSocList[i].socId ) ;
	        result =  -1 ;
		  break ;
			
	    }
	    
	    value = SOC_READ | SOC_WRITE | SOC_CLOSE | SOC_CONNECT;
		
	    if (soc_setsockopt(adaSocList[i].socId , SOC_ASYNC, &value, sizeof(value)) < 0) {	
			
	        soc_close (adaSocList[i].socId );
	         result =  -1 ;
		  break ;  
		  
	    }
			
	}while (0) ;
	adaSocList[i].used = iTRUE ;
	adaSocList[i].index = result ;

   #if( I51_MTK_VS >=VS_08B )

   		//	Jelo Editing Start 2011.12.15
	    //	if ( i51AppId!= 0 && i51SocLinkLayer.i51CreateLinkCount == 1) {
		//	Jelo Edting Finished

   		//	Jelo Adding Start 2011.12.15
 	  if ( i51AppId!= 0 ) {
	  	//	Jelo Adding Finished
   	
	        cbm_hold_bearer(
	            #if( I51_MTK_VS <VS_W10 )
	            MOD_MMI,
	            #endif
	            i51AppId);
	 
	    }
    #endif

	i51AdeOsLog ( 0 , "SOC : i51AdeSocGet , E , ret = %d" , result ) ;	
	
	return result;
	
} 



void i51AdeSocConnect ( iS32 socket , i51SOCADDR* address , i51SOCCONCBK callback )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.


	iS32 result = 0 ;
	sockaddr_struct conn_addr = {0};

	i51AdeOsLog ( 0 , "SOC : i51AdeSocConnect , S , soc_id=%d , %x , %x" , socket , address , callback ) ;
	
//	iASSERT ( callback && socket >= 0 && socket < MAX_SOCKET_COUNT ) ;
	iASSERT ( callback ) ;
	if (socket < 0 || socket > MAX_SOCKET_COUNT - 1) {

		i51AdeOsLog ( 0 , "SOC : i51AdeSocConnect , ID OUT" ) ;	
		return ;
	}
	i51SocCallbackData[socket].netState = 0 ;
	i51SocCallbackData[socket].i51Connect.callback = callback ;
	
	adaSocList[socket].socConCb = callback ;
	memset ( &conn_addr , 0 , sizeof ( conn_addr ) ) ;

	i51AdeOsLog ( 0 , "SOC : i51AdeSocConnect\r\n" ) ;	
	i51AdeOsLog ( 0 , "SOC : i51AcountIndex = %d\r\n" , i51AcountIndex ) ;	
	i51AdeOsLog ( 0 , "SOC : i51NWKAcountId = %d\r\n" , i51NWKAcountId ) ;
	

	if ( i51SocLinkLayer.i51NetMode == I51_NET_CONNCT_CMWAP) { 
	
		//链接移动网关

		#ifndef  i51SIMULATOR
		i51AdeOsLog ( 0 , "SOC : i51AdeSocConnect (1)\r\n" ) ;	
		conn_addr.addr_len = 4 ;
		conn_addr.addr[0] = 10 ;
		conn_addr.addr[1] = 0 ;
		conn_addr.addr[2] = 0 ;
		conn_addr.addr[3] = 172;
		conn_addr.port= 80;
		#else

		i51AdeOsLog ( 0 , "SOC : i51AdeSocConnect (2)\r\n" ) ;	
		conn_addr.addr_len = 4 ;
		if ( address ->ipaddr == 0 ) {

			conn_addr.addr[0] = 121 ;
			conn_addr.addr[1] = 37 ;
			conn_addr.addr[2] = 60 ;
			conn_addr.addr[3] = 20; 
			conn_addr.port= 80;
		} else {

			conn_addr.addr[3] = (address ->ipaddr)&0x000000ff ;
			conn_addr.addr[2] = ((address ->ipaddr)&0x0000ff00) >> 8 ;
			conn_addr.addr[1] = ((address ->ipaddr)&0x00ff0000) >> 16  ;
			conn_addr.addr[0] = ((address ->ipaddr)&0xff000000) >> 24;
			conn_addr.port= address ->port ;
		}
		
		#endif

	} else if ( i51SocLinkLayer.i51NetMode == I51_NET_CONNCT_CMNET) {

		conn_addr.addr_len = 4 ;
		conn_addr.addr[3] = (address ->ipaddr)&0x000000ff ;
		conn_addr.addr[2] = ((address ->ipaddr)&0x0000ff00) >> 8 ;
		conn_addr.addr[1] = ((address ->ipaddr)&0x00ff0000) >> 16  ;
		conn_addr.addr[0] = ((address ->ipaddr)&0xff000000) >> 24;
		conn_addr.port= address ->port ;
		

	}
	
	result = soc_connect( adaSocList[socket].socId , &conn_addr ) ;

	if ( result == SOC_SUCCESS ) {
		
		i51SocCallbackData[socket].i51Connect.state = iTRUE;	
		i51AdeSocPlanAsynCallBack (socket) ;
		
	}else if ( result ==  SOC_WOULDBLOCK ) {

		i51AdeSocNotifyIND () ;
		
	}else {

		i51AdeOsLog ( 0 , "SOC : SOC CON ret = %d\r\n" , result ) ;	
		i51SocCallbackData[socket].i51Connect.state = iFALSE ;	
//		i51AdeSocClose (adaSocList[socket].socId) ;
		i51AdeSocPlanAsynCallBack (socket) ;
		
	}

	i51AdeOsLog ( 0 , "SOC : i51AdeSocConnect , E" ) ;	
	
} 

void i51AdeSocSend ( iS32 socket , void* data , iU32 length , i51SOCSENDCBK callback )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.


	iS32 result = 0 ;

	i51AdeOsLog ( 0 , "SOC : i51AdeSocSend , S , soc_id=%d , data=%x , length=%d , callback=%x" , socket , data , length , callback ) ;	
	
//	iASSERT ( callback && socket >= 0 && socket < MAX_SOCKET_COUNT ) ;
	iASSERT ( callback ) ;
	if (socket < 0 || socket > MAX_SOCKET_COUNT - 1) {

		i51AdeOsLog ( 0 , "SOC : i51AdeSocSend , ID OUT" ) ;	
		return ;
	}
	i51SocCallbackData[socket].netState = 1 ;
	i51SocCallbackData[socket].i51Send.callback = callback ;
	
	adaSocList[socket].sendData = data ;
	adaSocList[socket].sendLength = length ;
	adaSocList[socket].socSendCb = callback ;
	
	result = soc_send ( adaSocList[socket].socId , data ,length , 0 ) ;

	if (result > 0)
		i51SocCallbackData[socket].i51Send.textlen = result ;
	else
		i51SocCallbackData[socket].i51Send.textlen = 0 ;
	
         
//	result = soc_send ( adaSocList[socket].socId , "GET http://s.pkingame.net/puff/240_320/MM1/1.jpg HTTP/1.1\r\nAccept: image/*,audio/*\r\nAccept-Language: zh-cn\r\nCache-Control: no-cache\r\n\r\n" ,strlen("GET http://s.pkingame.net/puff/240_320/MM1/1.jpg HTTP/1.1\r\nAccept: image/*,audio/*\r\nAccept-Language: zh-cn\r\nCache-Control: no-cache\r\n\r\n") , 0 ) ;

	if ( result > 0 ) {

		i51SocCallbackData[socket].i51Send.state = iTRUE ;
		i51AdeSocPlanAsynCallBack (socket) ;
		
	}else if ( result ==  SOC_WOULDBLOCK ) {

		i51AdeSocNotifyIND () ;

	}else {

		i51AdeOsLog ( 0 , "SOC : i51AdeSocSend , ret = %x\r\n" , result ) ;	
		i51SocCallbackData[socket].i51Send.state = iFALSE ;
//		i51AdeSocClose (adaSocList[socket].socId) ;
		i51AdeSocPlanAsynCallBack (socket) ;
		
	}


	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocSend , E" ) ;
	#endif		


} 


static void i51AdeReceiveing ( iU32 id  )
{


	//	开启定时接受数据
	iU16 i = 0 ;
	iS32 result = 0 ;

	for ( i = 0 ; i < MAX_SOCKET_COUNT ; i ++ ) {

		if ( adaSocList[i].timerid == id )
			break ;

	}
	if ( i == MAX_SOCKET_COUNT ) {

		i51AdeOsLog ( 0 , "SOC : i51AdeReceiveing , NOT FIND TIMER id= %x" , id ) ;	
		i51SocCallbackData[i].i51Receive.state = iFALSE ;
		i51AdeSocPlanAsynCallBack (i) ;
		return ;
	}

	result = soc_recv ( adaSocList[i].socId , adaSocList[i].recvData , adaSocList[i].recvLength , 0 ) ;
	
	if (result>0) {


		i51SocCallbackData[i].i51Receive.bufferlen = result ;

		//	Jelo Added Since 2011.12.15
		#ifdef i51PROFILINGS
			i51AdeSocGetRatioCount = i51SocCallbackData[i].i51Receive.bufferlen ;
		#endif
		//	Jelo Adding Finished
	
	}
	else
		i51SocCallbackData[i].i51Receive.bufferlen = 0 ;
	


	if ( result > 0 ) {

		i51SocCallbackData[i].i51Receive.state = iTRUE;
		i51SocCallbackData[i].i51Receive.bufferlen = result ;
		//i51AdeSocPlanAsynCallBack (i) ;
    	i51SocCallbackData[i].i51Receive.callback ( i51SocCallbackData[i].i51Receive.state ,
    									0 ,
    									i51SocCallbackData[i].i51Receive.buffer ,   
    									i51SocCallbackData[i].i51Receive.bufferlen ) ;
	}else if ( result < 0 ){
        if ( result == SOC_WOULDBLOCK ) {
		    i51AdeSocNotifyIND () ;
	    }else{
    		i51AdeOsLog ( 0 , "SOC : i51AdeReceiveing , ret = %x\r\n" , result ) ;	
    		i51SocCallbackData[i].i51Receive.state = iFALSE ;
    		//i51AdeSocPlanAsynCallBack (i) ;
    	    i51SocCallbackData[i].i51Receive.callback ( i51SocCallbackData[i].i51Receive.state ,
    									0 ,
    									i51SocCallbackData[i].i51Receive.buffer ,   
    									i51SocCallbackData[i].i51Receive.bufferlen ) ;
        }
	}
		 
	
}

void i51AdeSocReceive ( iS32 socket , void* data , iU32 length , i51SOCRECVCBK callback )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.


	iS32 result = 0 ;

	i51AdeOsLog ( 0 , "SOC : i51AdeSocReceive , S , soc_id=%d , %x , %d , %x" , socket , data , length , callback ) ;	
	
//	iASSERT ( callback && socket >= 0 && socket < MAX_SOCKET_COUNT ) ;
	iASSERT ( callback ) ;
	if (socket < 0 || socket > MAX_SOCKET_COUNT - 1) {

		i51AdeOsLog ( 0 , "SOC : i51AdeSocReceive , ID OUT" ) ;	
		return ;
	}
	
//	i51SocCallbackData[socket].netState = 2 ;
	i51SocCallbackData[socket].i51Receive.callback = callback ;
	i51SocCallbackData[socket].i51Receive.buffer = data ;
	
	adaSocList[socket].recvData = data ;
	adaSocList[socket].recvLength = length ;
	adaSocList[socket].socRecvCb = callback ;
#ifdef RERECIVE_MODE_OPEN	
	adaSocList[socket].timerid = i51AdeOsTimerCreate ( 1 , 1 ) ;
	i51AdeOsLog ( 0 ,"SOC : i51AdeSocReceive , timerid = %d , socket = %d  " , adaSocList[socket].timerid , socket ) ;
	if ( 0 == adaSocList[socket].timerid )
	{
		i51AdeOsLog ( i51_LOG_SC , "SOC : i51AdeSocReceive , TC F" ) ;
		i51SocCallbackData[socket].i51Receive.state = iFALSE ;
		i51AdeSocPlanAsynCallBack (socket) ;
		return  ;
	}

	if ( 0 == i51AdeOsTimerStart ( adaSocList[socket].timerid  , i51AdeReceiveing , 1 ) )
	{
		i51AdeOsLog ( i51_LOG_SC , "SOC : i51AdeSocReceive , TS F" ) ;	
		i51SocCallbackData[socket].i51Receive.state = iFALSE ;
		i51AdeSocPlanAsynCallBack (socket) ;
		return  ;
	}		

	
#else
	
	result = soc_recv ( adaSocList[socket].socId , data ,length , 0 ) ;
	
	if (result>0) {

		i51SocCallbackData[socket].i51Receive.bufferlen = result ;
	}
	else
		i51SocCallbackData[socket].i51Receive.bufferlen = 0 ;


	while ( result != SOC_WOULDBLOCK ) {

		if ( result >= SOC_SUCCESS ) {

			i51SocCallbackData[socket].i51Receive.state = iTRUE;
			i51SocCallbackData[socket].i51Receive.bufferlen = result ;
			i51AdeSocPlanAsynCallBack (socket) ;
			if ( adaSocList[socket].used) {
				//	防止同步调用i51AdeSocPlanAsynCallBack 时，处理完成soc关闭
				result = soc_recv ( adaSocList[socket].socId , data ,length , 0 ) ;
			} else {

				break ;
			}
			
		}else {

			i51AdeOsLog ( 0 , "SOC : i51AdeSocReceive , ret = %x\r\n" , result ) ;	
			i51SocCallbackData[socket].i51Receive.state = iFALSE ;
//			i51AdeSocClose (adaSocList[socket].socId) ;
			i51AdeSocPlanAsynCallBack (socket) ;
			break ;
		}

	}


	if ( result == SOC_WOULDBLOCK ) {

		i51AdeSocNotifyIND () ;
	}

#endif
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocReceive , E" ) ;
	#endif	
	
} 


#ifdef i51PROFILINGS
iU32 i51AdeSocGetLive ( void )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	//获取当前活跃的Socket数量：　 

	iU16	i ;
	iS32 count = 0 ;

	for ( i = 0 ; i < maxSocketCount ; i ++ ) {

		if ( adaSocList[i].used)		count ++ ;

	}

	//	Jelo Added Since 2011.12.15
	if ( 0 == count ) {
		i51AdeSocGetRatioCount = 0 ;
	}
	//	Jelo Adding Finished
	
	return count ;

} 

iU32 i51AdeSocGetRatio ( void )
{

	//	author : jelo
	//	since : 20111215
	//	PKIG Tech. Co., Ltd.
	
	return i51AdeSocGetRatioCount ;
	
}

#endif

iBOOL i51AdeSocClose ( iS32 socket )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocClose , S , %x" , socket ) ;
	#endif		
	
//	iASSERT( socket >= 0 && socket < MAX_SOCKET_COUNT ) ;
	if (socket < 0 || socket > MAX_SOCKET_COUNT - 1) {

		i51AdeOsLog ( 0 , "SOC : i51AdeSocClose , ID OUT" ) ;	
		return iFALSE ;
	}
	
	soc_close ( adaSocList[socket].socId) ;
	if ( adaSocList[socket].timerid )
		i51AdeOsTimerStop ( adaSocList[socket].timerid) ;
	memset ( &adaSocList[socket] , 0 , sizeof (adaSocList[socket]) ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocClose , E , ret = %x" , iTRUE ) ;
	#endif	
	
	return iTRUE;

} 

iS32* i51AdeSocGetLinkLayerMode ( void )
{	

	//	author : william
	//	since : 20110816
	//	PKIG Tech. Co., Ltd.

	//获取网络模式

	static iS32 LinkMode[8] = {0} ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocGetLinkLayerMode , S") ;
	#endif		

	LinkMode[0] = 2 ;
	LinkMode[1] = 1 ;
	LinkMode[2] = 2 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocGetLinkLayerMode , E , ret = %x" , LinkMode ) ;
	#endif	
	
	return LinkMode ;

}

iU32 i51AdeSocCreateLinkLayer ( iS32 mode , iS32 sim ) 
{

	//	创建一个链路

	iU32 i = 0 ;
	iU32 ret = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocCreateLinkLayer , S , %x , %x" , mode ,sim) ;
	#endif	

	i51AdeOsLog ( 0 , "SOC : i51AdeSocCreateLinkLayer , %d ,%d" , mode , sim) ;
	//	在转数据账户的时候需要用到
	i51SocLinkLayer.i51NetMode = mode ;
	i51SocLinkLayer.i51ChooseSim = i51ConvetSimCard (sim) ;

	for ( i = 0 ; i < MAX_LINKED_COUNT; i ++ ) {

		if (!i51SocLinkLayer.used[i])
			break ;

	}
	if ( i == MAX_LINKED_COUNT )
		ret = 0 ;
	else {

		i51SocLinkLayer.used[i] = iTRUE ;
		ret =  i + 1 ;

	}

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocCreateLinkLayer , E , ret = %x" , ret ) ;
	#endif	
	
	return ret ;
	
}


static void i51AdeMmi_i51LLAYERCALLBACK_TIMERCB ( iU32 id )
{

	iU32 i = 0 ;
	i51AdeOsTimerStop(id) ;
	for ( i = 0 ; i < MAX_LINKED_COUNT ; i ++ ) {

		if ( i51SocLinkLayer.timerid[i] == id )
			break ;
	}

	if ( i == MAX_LINKED_COUNT ) {

		i51AdeOsLog ( 0 , "SOC : i51AdeMmi_i51LLAYERCALLBACK_TIMERCB , id OUT " ) ;
		return ;
	}
    if( i51SocLinkLayer.used[i] && i51SocLinkLayer.callback[i] ) ;
	    i51SocLinkLayer.callback[i]( i51SocLinkLayer.CreateLayerState[i]) ;
}
static iBOOL i51AdeMmi_ASYN_i51LLAYERCALLBACK (  iU32 layerid )
{

	//	i51MEDIACALLBACK 回调，异步通知

	i51SocLinkLayer.timerid[layerid - 1] = i51AdeOsTimerCreate ( 0 , 1 ) ;
	if ( 0 == i51SocLinkLayer.timerid[layerid - 1]  )
	{
		i51AdeOsLog ( 0 , "SOC : i51AdeMmi_ASYN_i51LLAYERCALLBACK , TC " ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart (i51SocLinkLayer.timerid[layerid - 1] , i51AdeMmi_i51LLAYERCALLBACK_TIMERCB , 1 ) )
	{
		i51AdeOsLog ( 0 , "SOC : i51AdeMmi_ASYN_i51LLAYERCALLBACK , TS F" ) ;		
		return iFALSE ;
	}		

}

iBOOL i51AdeSocOpenLinkLayer ( iU32 aid , iU32 layerid , i51LLAYERCALLBACK callback  ) 
{

	//	author : william
	//	since : 20110816
	//	PKIG Tech. Co., Ltd.
	
	//	打开链路，真正创建链路的时候，只有链路真正没有关闭，就不会创建

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocOpenLinkLayer , S , %x , %x , %x" , aid , layerid ,callback) ;
	#endif	

	i51AdeOsLog ( 0 , "SOC : i51AdeSocOpenLinkLayer , %d ,%d ,%x" , aid , layerid) ;
	iASSERT ( callback ) ;

	if (layerid < 1 || layerid > MAX_LINKED_COUNT ) {
		i51AdeOsLog ( 0 , "SOC : Link-Layer id overflowed" ) ;	
		return iFALSE ;
	}	
	
	//	Jelo Added Since 2011.12.15
	i51SocLinkLayer.callback[layerid-1] = callback ;	
	//	判断应用链路表是否满
	//	如果满直接返回
	if ( i51AdeSocAidTableFull () ) {
	
		i51AdeOsLog ( 0 , "SOC : Link Layer Already Created") ;
		i51SocLinkLayer.CreateLayerState[layerid-1] = iTRUE ;
		i51AdeMmi_ASYN_i51LLAYERCALLBACK (layerid) ;
		
		return iTRUE ;
		
	} else if ( i51SocLinkLayer.LinkExsit ) {

		i51SocLinkLayer.CreateLayerState[layerid-1] = iTRUE ;
		i51AdeMmi_ASYN_i51LLAYERCALLBACK (layerid) ;	
	
		//	将Aid 创建链路的情况记录下来		
		if ( 0 == i51AdeSocAidTableFind ( aid ) ) {
			i51AdeSocAidTableAdd ( aid ) ;
		}
		
		return iTRUE ;
		
	} else {

		if ( 0 != i51SocLinkLayer.layerid ) {

			//	Jelo Edited Start 2011.12.16
			//	正在创建链路
			//	i51SocLinkLayer.CreateLayerState[layerid-1] = iFALSE ;
			//	i51AdeMmi_ASYN_i51LLAYERCALLBACK (layerid) ;
			//	Jelo Edting Finished
			
			i51AdeOsLog ( 0 , "SOC : Link-Layer is Opening") ;
			
			return iFALSE ;
			
		}
	
	}
	
	i51AdeSocLastAddAid = aid ;	
	i51SocLinkLayer.layerid = layerid ;	
	//	Jelo Adding Finished
				
	//	Jelo Edited Start 2011.12.15
	//	i51SocLinkLayer.callback[layerid-1] = callback ;
	//	if ( i51SocLinkLayer.LinkExsit ) {		
	//		
	//		i51SocLinkLayer.i51CreateLinkCount++ ;
	//		i51SocLinkLayer.CreateLayerState[layerid-1] = iTRUE ;
	//		i51AdeMmi_ASYN_i51LLAYERCALLBACK (layerid) ;
		
	//		#ifdef i51ADAPTER_DEBUG
	//		i51AdeOsLog ( 0 , "i51AdeSocOpenLinkLayer E , ret = %x" , iTRUE ) ;
	//		#endif	
		
	//		return iTRUE ;
	//	} else {
	//			//	链路没有创建成功的时候
	//			if (i51SocLinkLayer.layerid) {
	//			//	正在创建链路
	//			i51SocLinkLayer.CreateLayerState[layerid-1] = iFALSE ;
	//			i51AdeMmi_ASYN_i51LLAYERCALLBACK (layerid) ;
	//			i51AdeOsLog ( 0 , "ADE - SOC : i51AdeSocOpenLinkLayer BE1.1") ;
	//			return iFALSE ;
	//		}

	//	}	
	//	Jelo Eidting Finished

	if ( 1 == i51SocLinkLayer.i51NetMode ) {

		//	Jelo Edited Start 2011.12.15
		//	记录正在创建链路的id
		//	i51SocLinkLayer.layerid = layerid ;
		//	Eidting Finished
		
		i51AdeSocFindAcountIndex ( "cmwap" ,  i51getAcountIndexCB ) ;

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "SOC : i51AdeSocOpenLinkLayer , E , ret = %x" , iTRUE ) ;
		#endif	
		
		return iTRUE ;
		
	} else if ( 2 == i51SocLinkLayer.i51NetMode ) {

		//	Jelo Edited Start 2011.12.15
		//	记录正在创建链路的id
		//	i51SocLinkLayer.layerid = layerid ;
		//	Eidting Finished
		
		i51AdeSocFindAcountIndex ( "cmnet" ,  i51getAcountIndexCB ) ;
		
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "SOC : i51AdeSocOpenLinkLayer , E , ret = %x" , iTRUE ) ;
		#endif	
		
		return iTRUE ;

	} else {

		//	Jelo Adding Start 2011.12.15
		i51AdeSocLastAddAid = 0 ;
		i51SocLinkLayer.layerid = 0 ;
		
		i51AdeOsLog ( 0 , "SOC : Not Support Link-Layer Mode %x" , i51SocLinkLayer.i51NetMode ) ;
		//	Jelo Adding Finished
		
		//	Jelo Edited Start 2011.12.15
		//	i51SocLinkLayer.CreateLayerState[layerid-1] = iFALSE ;
		//	i51AdeMmi_ASYN_i51LLAYERCALLBACK (layerid) ;
		//	i51AdeOsLog ( 0 , "ADE - SOC : i51AdeSocOpenLinkLayer BE1.2") ;
		//	Jelo Eidting Finished
		
		return iFALSE ;
	}

}

iBOOL i51AdeSocCloseLinkLayer ( iU32 aid ,  iU32 layerid ) 
{

	//	author : william
	//	since : 20110816
	//	PKIG Tech. Co., Ltd.
	
	//	关闭链路

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocCloseLinkLayer , S , %x , %x" , aid , layerid ) ;
	#endif	

	i51AdeOsLog ( 0 , "SOC : i51AdeSocCloseLinkLayer , %d ,%d" , aid , layerid) ;

	//	Jelo Added Since 2011.12.15
	//	如果链路没有创建过，则直接返回iTRUE。
	//	不影响上层逻辑。
	if ( iFALSE == i51SocLinkLayer.LinkExsit ) {
		return iTRUE ;
	} 
	
	i51AdeSocAidTableDelete ( aid ) ;
	//	检查链路表，如果存在应用依赖链路层，则不关闭链路。
	if ( iFALSE == i51AdeSocAidTableEmpty () ) {
		return iTRUE ; 
	}
	//	Jelo Adding Finished
	
	//	Jelo Edited Start 2011.12.15
#if 0	
	i51SocLinkLayer.i51CreateLinkCount -- ;
	//	防止没有创建成功，就colse	
	if ( i51SocLinkLayer.i51CreateLinkCount <= 0  ) {
		
		i51SocLinkLayer.i51CreateLinkCount = 0 ;
		
 #if defined(VS_0812)
	    {
	        ilm_struct *ilm_send = iNULL;
	        soc_close_nwk_account_by_id (MOD_MMI, i51NWKAcountId);
	        ilm_send = allocate_ilm(MOD_MMI);
	        ilm_send->msg_id = MSG_ID_APP_SOC_DEACTIVATE_REQ;
	        ilm_send->peer_buff_ptr = iNULL;
	        ilm_send->local_para_ptr = iNULL;
	        SEND_ILM(MOD_MMI, MOD_SOC, SOC_APP_SAP, ilm_send);
	    }
 #else
        cbm_release_bearer (
            #if !defined(VS_W10)&& !defined(VS_W11)&& !defined(VS_11A_1112)&& !defined(VS_11B_1132)&& !defined(VS_11B_1212)
            MOD_MMI,
            #endif
            i51AppId);
  
        if ( i51AppId != 0) {	 
          	 cbm_deregister_app_id (i51AppId);
            i51AppId = 0;	
        }
#endif
    	i51AdeSocExitNotify () ;
    	HideStatusIcon(STATUS_ICON_GPRS_SERVICE); 
    	#ifdef __MMI_DUAL_SIM_MASTER__
    		HideStatusIcon(STATUS_ICON_SLAVE_GPRS_SERVICE); 
    	#endif
    	UpdateStatusIcons();
			
	}
#endif	
	//	Jelo Eidting Finished

//	Jelo Added Since 2011.12.15
 #if( I51_MTK_VS <=VS_0812 )
	    {
	        ilm_struct *ilm_send = iNULL;
	        soc_close_nwk_account_by_id (MOD_MMI, i51NWKAcountId);
	        ilm_send = allocate_ilm(MOD_MMI);
	        ilm_send->msg_id = MSG_ID_APP_SOC_DEACTIVATE_REQ;
	        ilm_send->peer_buff_ptr = iNULL;
	        ilm_send->local_para_ptr = iNULL;
	        SEND_ILM(MOD_MMI, MOD_SOC, SOC_APP_SAP, ilm_send);
	    }
 #else
        cbm_release_bearer (
            #if( I51_MTK_VS <VS_W10 )
            MOD_MMI,
            #endif
            i51AppId);
  
        if ( i51AppId != 0) {	 
          	 cbm_deregister_app_id (i51AppId);
            i51AppId = 0;	
        }
#endif
    	i51AdeSocExitNotify () ;
    	HideStatusIcon(STATUS_ICON_GPRS_SERVICE); 
    	#ifdef __MMI_DUAL_SIM_MASTER__
    		HideStatusIcon(STATUS_ICON_SLAVE_GPRS_SERVICE); 
    	#endif
    	UpdateStatusIcons();

	i51SocLinkLayer.LinkExsit = iFALSE ;
	i51SocLinkLayer.layerid= 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocCloseLinkLayer , E , ret = %x" , iTRUE ) ;
	#endif	
	
	return iTRUE ;
//	Jelo Adding Finished	
	
}

iBOOL i51AdeSocReleaseLinkLayer ( iU32 layerid ) 
{

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocReleaseLinkLayer , S , %x" , layerid ) ;
	#endif	

	i51AdeOsLog ( 0 , "SOC : i51AdeSocReleaseLinkLayer , %d" ,layerid) ;
	
	if (  layerid < 1 || layerid > MAX_LINKED_COUNT  )
	{
		i51AdeOsLog ( 0 , "SOC : i51AdeSocReleaseLinkLayer , BE " ) ;
		return iFALSE ;
	}
	i51SocLinkLayer.callback[layerid-1] = 0 ;
	i51SocLinkLayer.used[layerid-1] = iFALSE ;
    i51DnsCB = NULL;//防止退出程序后，域名解析回调去调用此程序指针
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocReleaseLinkLayer , E , ret = %x" , iTRUE ) ;
	#endif		
	return iTRUE ;
}


//	Jelo Edited Start 2011.12.15
#if 0
iBOOL i51AdeSocBuildLinkLayer ( iS32 mode , iS32 sim ,  i51LLAYERCALLBACK callback  )
{
	//	author : william
	//	since : 20110816
	//	PKIG Tech. Co., Ltd.

	//设置链路模式 1：CMWAP、2：CMNET、3：3G、4：WIFI。

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "i51AdeSocBuildLinkLayer S , %x , %x , %x" , mode , sim , callback ) ;
	#endif	
	
	i51SetLinkModeCB = callback ;
	i51SocLinkLayer.i51CreateLinkCount ++ ;

	iASSERT ( callback ) ;
	if ( i51SocLinkLayer.i51CreateLinkCount > 1 ) {

		callback ( iTRUE ) ;

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "i51AdeSocBuildLinkLayer E , ret = %x" , iTRUE ) ;
		#endif		
		
		return iTRUE ;

	}
	
	i51SocLinkLayer.i51NetMode = mode ;
	i51SocLinkLayer.i51ChooseSim = i51ConvetSimCard (sim) ;
	if ( 1 == mode ) {

		i51AdeSocFindAcountIndex ( "cmwap" ,  i51getAcountIndexCB ) ;
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "i51AdeSocBuildLinkLayer E , ret = %x" , iTRUE ) ;
		#endif		
		return iTRUE ;
		
	} else if ( 2 == mode ) {

		i51AdeSocFindAcountIndex ( "cmnet" ,  i51getAcountIndexCB ) ;
		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "i51AdeSocBuildLinkLayer E , ret = %x" , iTRUE ) ;
		#endif		
		
		return iTRUE ;

	} else {

		i51AdeOsLog ( 0 , "ADE - SOC : i51AdeSocBuildLinkLayer BE" ) ;
		return iFALSE ;

	}


}  
#endif

//	Jelo Eidting Finished

/*
//	Committed by Jelo
//	since 2011.11.8
iBOOL i51AdeSocReleaseLinkLayer (iS32 sim , i51LLAYERCALLBACK callback  )
{
	i51SocLinkLayer.i51CreateLinkCount -- ;
	if ( i51SocLinkLayer.i51CreateLinkCount == 0  ) {

 #if defined(VS_0812)
	    {
	        ilm_struct *ilm_send = iNULL;

	        soc_close_nwk_account_by_id (MOD_MMI, i51NWKAcountId);
	    
	        ilm_send = allocate_ilm(MOD_MMI);
	        ilm_send->msg_id = MSG_ID_APP_SOC_DEACTIVATE_REQ;
	        ilm_send->peer_buff_ptr = iNULL;
	        ilm_send->local_para_ptr = iNULL;
	        SEND_ILM(MOD_MMI, MOD_SOC, SOC_APP_SAP, ilm_send);
	    }
 #else
       cbm_release_bearer (
            #if !defined(VS_W10) && !defined(VS_W11)
            MOD_MMI,
            #endif
            i51AppId);
  
  
	 if ( i51AppId != 0) {
	 
      	 cbm_deregister_app_id (i51AppId);
        i51AppId = 0;
		
   	 }
#endif
	i51AdeSocExitNotify () ;
	HideStatusIcon(STATUS_ICON_GPRS_SERVICE); 
	#ifdef __MMI_DUAL_SIM_MASTER__
		HideStatusIcon(STATUS_ICON_SLAVE_GPRS_SERVICE); 
	#endif
	UpdateStatusIcons();
			
	}

	if (callback)
		callback ( iTRUE ) ;
	return iTRUE ;

}
*/

void i51AdeSocGetLinkLayerStatus ( iBOOL* linked , iS32* simid , iS32* mode ) 
{

	//	author : william
	//	since : 20111121
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocGetLinkLayerStatus , S , %x , %x , %x" , linked , simid , mode ) ;
	#endif	

	//	Jelo Edited Start 2011.12.15
	//	i51AdeOsLog ( 0 , "ADE - SOC i51AdeSocGetLinkLayerStatus S") ;
	//	if ( i51SocLinkLayer.i51CreateLinkCount > 0 ) {
	//	Jelo Editing Finished
	
	//	Jelo Adding Start 2011.12.15
	if ( iTRUE == i51SocLinkLayer.LinkExsit ) {
	//	Jelo Adding Finished
	
		*linked = iTRUE ;
		*simid = i51SocLinkLayer.i51ChooseSim + 1 ;
		*mode = i51SocLinkLayer.i51NetMode ;
		
	} else {

		*linked = iFALSE;
		*simid = 0 ;
		*mode = 0 ;
	}

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "SOC : i51AdeSocGetLinkLayerStatus , E" ) ;
	#endif		
		
}

iBOOL i51AdeSocCreate ( i51ADECALLBACK callback )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	iASSERT (callback) ;
	i51AdaCreateCB= callback ;
	if ( !i51AdeSoc_ASYN_i51ADECALLBACK() )
		return iFALSE ;
	
	return iTRUE ;
	

} 

iBOOL i51AdeSocRelease ( void )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.
	return iTRUE ;

} 
