#include "i51AdeSoc.h"
#include "ven_socket.h"

#include "i51AdeCnf.h"


//#define  i51ADAPTER_DEBUG

//应该传入 AP 的handle   
extern iU32 g_i51PkigHandle; 

//建立链路的SImID
static iU8 buildLinkesimID;

//网络的网络模式
static iU8 i51NetMode = 1;

static iBOOL IsCreatedLinkLayerState = iFALSE; //是否已经创建了链路

static iBOOL IsCreatingLinkLayerState = iFALSE;//是否正在创建链路
static iU8 gi51RealLinkLayerID = 0;       //真正创建链路的ID

//	Jelo Added Since 2011.12.15
#ifdef i51PROFILINGS
iU32 i51AdeSocGetRatioCount = 0 ;
#endif
//	Jelo Adding Finished

typedef struct {

	//!< MStar 的socket结构体
	ven_socket_t mstar_socket;	

	//是否被使用
	iBOOL	used ;

	//请求数据
	void *sendData ;

	//请求数据长度
	iU32	sendLength ;

	//请求状态
	iBOOL sendStat;

	//接受数据
	void *recvData ;

	//接受到数据的长度
	iU32	recvLength ;

	//连接回调
	i51SOCCONCBK socContCB;

	//发送回调
	i51SOCSENDCBK socSendCb ;

	//接受回调
	i51SOCRECVCBK	socRecvCb ;

} i51SCOKET ;

//创建socket的最大个数
#define i51SOCMAX_COUNT 5

//创建5个套接字
static i51SCOKET i51_socket_table[i51SOCMAX_COUNT]={ 0 };

typedef struct {

	iS32 mode;
	iS32 simID;
	iU8 creatSate;//创建状态，1为以创建，0没有创建
	iU32 tag;
	iU8 openstate;
	i51LLAYERCALLBACK i51buildLinkCB;

} i51LINKDIVICE;


//创建5链路
static i51LINKDIVICE i51_LinkDivice[i51SOCMAX_COUNT]={ 0 };

//实际创建链路的回调
i51LLAYERCALLBACK realCreateLink_CB;

//	Jelo Editing Since 2011.12.15	
//创建链路的次数
//static iU32 i51CreateLinkCount = 0 ;
//	Jelo Editing Finished

static i51ADECALLBACK i51AdeCallback = 0 ;

static i51DNSCALLBACK i51DnsCallBack = 0;

static iU32 nHost = 0;


//	Jelo Added Since 2011.12.15
#define i51AdeSocAidTableLength 5
static iU32 i51AdeSocAidTable [ i51AdeSocAidTableLength ]  = { 0 } ;
static iU32 i51AdeSocLastAddAid = 0 ;

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




static void i51AdeSocCreateCallBack(iU16 timerid)
{
	if (NULL != i51AdeCallback)
	{
		i51AdeCallback(iTRUE);
	}

	i51AdeOsTimerStop(timerid);
}

static i51AdeSocGetHostByName_BC(iU16 timerid)
{
	if (NULL != i51DnsCallBack)
	{
		i51DnsCallBack(nHost);
	}

	i51AdeOsTimerStop(timerid);
}

//已经创建了链路
static void i51AdeSocOpenLink_CB(iU16 timerid)
{
	iU32 i;

	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocOpenLink_CB , %d",timerid);

	for(i=0;i<i51SOCMAX_COUNT;i++)
	{
        if(i51_LinkDivice[i].tag == timerid&&i51_LinkDivice[i].creatSate != 0)
        {
            if(i51_LinkDivice[i].i51buildLinkCB)
            {
                i51_LinkDivice[i].i51buildLinkCB(iTRUE);
                i51_LinkDivice[i].tag = 0xffffffff;
                break;
            }
        }
	}

	i51AdeOsTimerStop(timerid);
}

static void Sys_suspendLink_CB(ven_socket_ret_t result)
{
	ven_socket_ret_t ret;

	i51AdeOsLog(i51_LOG_SC, "SOC : Sys_suspendLink_CB , %d", result);

	if (VEN_SOCKET_RET_COM_CLOSE_BY_PEER == result)
	{
		if ( ven_socket_data_link_exist() )
		{
			ret = ven_socket_close_datalink( g_i51PkigHandle ) ;
		}

		//if (VEN_SOCKET_RET_SUCCESS == ret)
		{
			IsCreatedLinkLayerState = iFALSE;

			//	Jelo Adding Since 2011.12.15	
			i51AdeSocAidTableClear () ;
			//	Jelo Adding Finished
				
			//	Jelo Editing Since 2011.12.15	
			//	i51CreateLinkCount=0;
			//	Jelo Editing Finished
			
			buildLinkesimID = 0;
			i51NetMode = 1;
		}

	}
}

static void i51_socket_connectCB(ven_socket_t *s, ven_socket_ret_t result)
{
	int i;
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51_socket_connectCB , S, %d,%d", result, s->socket);
#endif
    
	for(i = 0; i < i51SOCMAX_COUNT; ++i)
	{
		if(i51_socket_table[i].mstar_socket.socket == s->socket && i51_socket_table[i].socContCB)
		{
		    i51AdeOsLog(i51_LOG_SC, "SOC : i51_socket_connectCB ,  %d", result);
			if (VEN_SOCKET_RET_SUCCESS == result)
			{
				i51_socket_table[i].socContCB(iTRUE, i);
			} 
			else
			{
				i51_socket_table[i].socContCB(iFALSE, i);
			}

			break;
		}
	}
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51_socket_connectCB , E, %d,%d", i, s->socket);
#endif
}

static void i51_socket_SendCB(ven_socket_t *s, ven_socket_ret_t result)
{
	int i;
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51_socket_SendCB , S, %d,%d", result, s->socket);
#endif
	for(i = 0; i < i51SOCMAX_COUNT; ++i)
	{
		if(i51_socket_table[i].mstar_socket.socket == s->socket && i51_socket_table[i].socSendCb)
		{

			i51AdeOsLog(i51_LOG_SC, "SOC : i51_socket_SendCB , %d", result);

			if (VEN_SOCKET_RET_SUCCESS == result)
			{
				i51_socket_table[i].socSendCb(iTRUE, i, i51_socket_table[i].sendLength);
			}
			else
			{
				i51_socket_table[i].socSendCb(iFALSE, i, i51_socket_table[i].sendLength);
			}

			break;
		}
	}
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51_socket_SendCB , E, %d,%d" , i, s->socket);
#endif
}

static void i51_socket_recvCB(ven_socket_t *s, iU8 *buf, s32 len)
{
	int i = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51_socket_recvCB , S, %d,%x,%d" , s->socket, buf, len);
#endif
	//	Jelo Added Since 2011.12.15
	#ifdef i51PROFILINGS
		i51AdeSocGetRatioCount = len ;
	#endif
	//	Jelo Adding Finished

	for(i = 0; i < i51SOCMAX_COUNT; ++i)
	{
		if(i51_socket_table[i].mstar_socket.socket == s->socket && i51_socket_table[i].socRecvCb)
		{
			if (0 < len && buf)
			{
				i51_socket_table[i].socRecvCb(iTRUE, i, buf, len);
			}
			else
			{
				i51_socket_table[i].socRecvCb(iFALSE, i, buf, len);
			}
			break;
		}
	}
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51_socket_recvCB , E, %d,%d" , i , s->socket);
#endif
}

static iU32 i51_DSM_ntohl(iU64 netlong)
{
	iU8 *p = (iU8 *)&netlong;
	iS32 result = 0;

	result = (p[0]<<24)|(p[1] <<16)|(p[2]<<8)|(p[3]);

	return result;
}

//第一次创建链路
static void i51AdeSoc_DataLinkerCB( ven_socket_ret_t result )
{

    ven_socket_ret_t sret;

    i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSoc_DataLinkerCB ,  %d,%d" , result, realCreateLink_CB);

    IsCreatingLinkLayerState = iFALSE;

    if(i51_LinkDivice[gi51RealLinkLayerID - 1].openstate == 0)
    {
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSoc_DataLinkerCB , ERR , linklayer had been colsed , %d",gi51RealLinkLayerID);
	return;
    }
    if (VEN_SOCKET_RET_SUCCESS == result||VEN_SOCKET_RET_DATA_LINK_ALREADY_ACTIVATED == result)
    {

        //	Jelo Editing Since 2011.12.15	
        //	i51CreateLinkCount++;
        //	Editing Finished

        IsCreatedLinkLayerState = iTRUE;
        i51AdeSocLastAddAid = 0 ;
        sret = ven_socket_regStatusCB(g_i51PkigHandle, Sys_suspendLink_CB);

        if (VEN_SOCKET_RET_SUCCESS != sret)
        {
            i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSoc_DataLinkerCB , ERR ,  set filed");
        }

        if (NULL != realCreateLink_CB)
        {
            realCreateLink_CB(iTRUE);
        }
    }
    else
    {
        if (NULL != realCreateLink_CB)
        {

            //	Jelo Added Since 2011.12.15
            i51AdeSocAidTableDelete( i51AdeSocLastAddAid ) ;
            i51AdeSocLastAddAid = 0 ;	
            //	Jelo Adding Finished

            realCreateLink_CB(iFALSE);
        }
    }
}

static iBOOL i51SocBuildLinkLayer ( char*  apnStr, iU32 layerID)
{

	//	author : jone
	//	since : 2011.11.12
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_setting_ret_t settingret = VEN_SETTING_RET_FAILED;
	ven_setting_network_profile_t Profile = { 0 } ;
	ven_common_sim_id_t Sim = E_VEN_MASTER_SIM;	
	ven_setting_sim_function_type_t  simtype;
    
	if (NULL == apnStr)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51SocBuildLinkLayer ,  ERR");	
		return iFALSE;
	}

    if(1 != i51_LinkDivice[layerID - 1].simID)
    {
        Sim = E_VEN_SLAVE_SIM;
    }

	ven_std_memcpy ( Profile.GprsApn , apnStr , ven_std_strlen ( apnStr ) ) ;
	Profile.ProxyIp[0]=0 ;
	Profile.ProxyIp[1]=0 ;
	Profile.ProxyIp[2]=0 ;
	Profile.ProxyIp[3]=0 ;
	Profile.nProxyPort=0 ;
	Profile.tProtocol = VEN_SETTING_NETWORK_PROTOCOL_HTTP ;

	IsCreatingLinkLayerState = iTRUE;

	ven_socket_create_datalink_async( g_i51PkigHandle ,Sim, &Profile , (ven_socket_create_datalink_cbfunc_t)i51AdeSoc_DataLinkerCB) ;

	realCreateLink_CB = i51_LinkDivice[layerID - 1].i51buildLinkCB;
	i51NetMode = i51_LinkDivice[layerID - 1].mode ;
	buildLinkesimID = i51_LinkDivice[layerID - 1].simID;
	i51_LinkDivice[layerID - 1].openstate = 1;
	gi51RealLinkLayerID = layerID;
    //	Jelo Adding Start 2011.12.15
    //	将Aid 创建链路的情况记录下来
    i51AdeSocAidTableAdd ( i51AdeSocLastAddAid ) ;
    //	Jelo Adding Finished
    
	return iTRUE;
} 


iU32 i51AdeSocHtonl ( iU32 value ) {

	//	author : jone
	//	since : 2010.09.7
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 ret = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocHtonl , S , %d", value);
#endif

	ret = ven_socket_htonl ( value ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocHtonl , E , %d", ret);
#endif

	return ret;
}

iU16 i51AdeSocHtons ( iU16 value ) {

	//	author : jone
	//	since : 2010.09.7
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU16 ret = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocInetAton , S , %d", value);
#endif

	ret = ven_socket_htons ( value ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocInetAton , E , %d" , ret);
#endif

	return ret ;

}

iBOOL i51AdeSocInetAton ( iCHAR* ip, iU32* value ) {

	//	author : jone
	//	since : 2010.09.7
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	int  point = 0 ;
	iU32 i = 0 ;
	iBOOL ret = iFALSE;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocInetAton , S , %s,%x" , ip, value);
#endif

	for (i = 0 ;i < strlen (ip) ; i++)
	{ 
		if( ip[i] == '.' )
			point++ ;
	}

	if(point != 3 )
	{

		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocInetAton , F");
		return iFALSE ;
	}

	ret = ven_socket_inet_aton ( ip , (struct in_addr*)value ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocInetAton , E , %d" , ret);
#endif
	
	return ret;
}

iS32 i51AdeSocGetHostByName ( iCHAR* name , i51DNSCALLBACK callback ) 
{

	//	author : jone
	//	since : 2011.11.12
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	struct hostent *phost;
	struct in_addr inaddr = {0};
	ven_socket_ret_t ret = VEN_SOCKET_RET_FAILED;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetHostByName , S , %s,%x" , name, callback);
#endif

	if (NULL == name || NULL == callback)
	{

		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetHostByName , ERR1");
		return 0;
	}

	i51DnsCallBack = callback;
	phost = ven_socket_allocHostent () ;
	ret = ven_socket_gethostbyname((iU32)g_i51PkigHandle, (const iU8*)name, phost);

	if (ret == VEN_SOCKET_RET_SUCCESS)
	{
		inaddr = *(struct in_addr*) (phost->h_addr_list[0]) ;    
		ven_socket_freeHostent (phost);
	
		nHost = inaddr.S_un.S_addr;
		i51AdeOsTimerStart(i51AdeOsTimerCreate(iFALSE, 0), i51AdeSocGetHostByName_BC, 30);

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetHostByName , E");
#endif

		return 1 ;
	}
	else
	{
		ven_socket_freeHostent (phost);

		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetHostByName , ERR2");
		return 0;
	}
}

//ok
iS32 i51AdeSocGet ( void ) 
{

	//	author : jone
	//	since : 2011.11.12
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU16	i = 0;
	iS32 result = -1;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGet , S");
#endif

	do
	{
		for ( i = 0 ; i < i51SOCMAX_COUNT ; i ++ )
		{
			if ( !i51_socket_table[i].used)	
				break ;
		}

		if( i == i51SOCMAX_COUNT ) 
		{
			i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGet , F , SOC_FULL");
			return -1;
		}

		ven_std_memset(&i51_socket_table[i], 0, sizeof(i51SCOKET));

		if ( VEN_SOCKET_RET_SUCCESS == ven_socket_createsocket ( g_i51PkigHandle , AF_INET , SOCK_STREAM , IPPROTO_TCP , &( i51_socket_table[i].mstar_socket ) ) ) 
		{	
			result = i; 
			i51_socket_table[i].used = iTRUE ;
		}

	}while (0) ;

	if(-1 == result)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGet ,  ERR");
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGet , E , %d,%d" , result , i51_socket_table[result].mstar_socket.socket);
#endif

	return result;
} 

//ok
void i51AdeSocConnect ( iS32 socket , i51SOCADDR* address , i51SOCCONCBK callback )
{

	//	author : jone
	//	since : 2011.11.12
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	struct sockaddr_in sa = {0};
	ven_socket_ret_t ret;

 #ifdef i51ADAPTER_DEBUG   
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocConnect , S , %d,%x,%x" , socket, address, callback);
 #endif
    if (socket < 0 || socket > i51SOCMAX_COUNT) 
	{
		i51AdeOsLog(i51_LOG_SC , "SOC : i51AdeSocConnect , ERR , ID OUT" ) ;	
		return ;
	}
	
	if (NULL == address || NULL == callback||i51_socket_table[socket].used == 0)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocConnect , F , %d,%d" , i51_socket_table[socket].used,socket);
		return;
	}
	
	memset ( &sa , 0 , sizeof ( sa ) ) ;
	sa.sin_family = AF_INET ;

	if ( i51NetMode == 1)
	{ 
		//链接移动网关

		sa.sin_port = ven_socket_htons (80) ;
#ifndef  i51SIMULATOR
		ven_socket_inet_aton ("10.0.0.172", &sa.sin_addr) ;
#else
		if(address->ipaddr == 0)
		{
			ven_socket_inet_aton ("121.37.60.20", &sa.sin_addr) ;            
		}
		else
		{
			sa.sin_port = ven_socket_htons(address->port);
			sa.sin_addr.s_addr = address->ipaddr ;
		}
#endif

	}
	else if ( i51NetMode == 2)
	{
		sa.sin_port = ven_socket_htons(address->port);
		sa.sin_addr.s_addr = address->ipaddr ;
	}

	i51_socket_table[socket].socContCB = callback;

	ret = ven_socket_connect_async(&i51_socket_table[socket].mstar_socket, (struct sockaddr*)&sa, sizeof(struct sockaddr_in), (ven_socket_connect_cbfunc_t)i51_socket_connectCB);

 #ifdef i51ADAPTER_DEBUG   
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocConnect , E , %d,%d" , ret, i51NetMode);
#endif
} 

//ok
void i51AdeSocSend ( iS32 socket , void* data , iU32 length , i51SOCSENDCBK callback )
{

	//	author : jone
	//	since : 2011.11.12
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_socket_ret_t ret;
	
 #ifdef i51ADAPTER_DEBUG   
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocSend , S , %d,%x,%d,%x" , socket, data, length, callback);
 #endif
    if (socket < 0 || socket > i51SOCMAX_COUNT) 
	{
		i51AdeOsLog(i51_LOG_SC , "SOC : i51AdeSocSend , ERR , ID OUT" ) ;	
		return ;
	}
	if (NULL == data || NULL == callback||i51_socket_table[socket].used == 0)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocSend , F, %d,%d" , i51_socket_table[socket].used,socket);
		return;
	}

	i51_socket_table[socket].socSendCb = callback;
	i51_socket_table[socket].sendData = data;
	i51_socket_table[socket].sendLength = length;

	ret = ven_socket_send_async(&i51_socket_table[socket].mstar_socket, (const iU8*)data, length, (ven_socket_send_cbfunc_t)i51_socket_SendCB, 0);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocSend , E , %d" , ret);
#endif
} 

//ok
void i51AdeSocReceive ( iS32 socket , void* data , iU32 length , i51SOCRECVCBK callback ) 
{

	//	author : jone
	//	since : 2011.11.12
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	
	ven_socket_ret_t ret;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocReceive , S , %d,%x,%d,%x", socket, data, length, callback);
#endif

    if (socket < 0 || socket > i51SOCMAX_COUNT) 
	{
		i51AdeOsLog(i51_LOG_SC , "SOC : i51AdeSocReceive , ERR,  ID OUT" ) ;	
		return ;
	}
	
	if (NULL == data || NULL == callback||i51_socket_table[socket].used == 0)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocReceive , F , %d,%d" , i51_socket_table[socket].used,socket);
		return;
	}

	i51_socket_table[socket].recvData = data;
	i51_socket_table[socket].recvLength = length;
	i51_socket_table[socket].socRecvCb = callback;
	ret = ven_socket_regRecvCB(&i51_socket_table[socket].mstar_socket, (ven_socket_recv_cbfunc_t)i51_socket_recvCB, 0);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocReceive , E , %d",ret);
#endif
} 

#ifdef i51PROFILINGS

iU32 i51AdeSocGetLive ( void ) {

	//	author : discut
	//	since : 2010.09.7
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU16	i ;
	iS32 count = 0 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLive , S");
#endif

	for ( i = 0 ; i < 5 ; i ++ ) {
		if ( i51_socket_table[i].used )
			count ++ ;
	}

	//	Jelo Added Since 2011.12.15
	if ( 0 == count ) {
		i51AdeSocGetRatioCount = 0 ;
	}
	//	Jelo Adding Finished
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLive , E");
#endif

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

iS32* i51AdeSocGetLinkLayerMode() {

	//	author : jone
	//	since : 2010.09.7
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	static iS32 mode [ 3 ] = { 0 } ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLinkLayerMode , S");
#endif

	mode [ 0 ] = 2 ;
	mode [ 1 ] = 1 ;
	mode [ 2 ] = 2 ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLinkLayerMode , E");
#endif

	return mode ;
}

iBOOL i51AdeSocClose ( iS32 socket )
{

	//	author : jone
	//	since : 2011.11.12
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocClose , S , %d", socket);
#endif
	if (iTRUE == i51_socket_table[socket].used)
	{
		if (VEN_SOCKET_RET_SUCCESS == ven_socket_closesocket(&( i51_socket_table[socket].mstar_socket)))
		{
			memset(&i51_socket_table[socket], 0, sizeof(i51SCOKET));
#ifdef i51ADAPTER_DEBUG
			i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocClose , E");
#endif
			return iTRUE;
		}

		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocClose , ERR1 , %d" , socket);
		return iFALSE;
	}

	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocClose ERR2 , %d" , socket);
	return iFALSE;

}

void i51AdeSocGetLinkLayerStatus ( iBOOL* linked , iS32* simid , iS32* mode )
{

	//	author : jone
	//	since : 2011.11.12
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLinkLayerStatus S , %x %x %x", linked, simid, mode);
#endif

	if (NULL == linked || NULL == simid || NULL == mode)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLinkLayerStatus , F");
		return;
	}

	*linked = IsCreatedLinkLayerState;

	if (iTRUE == IsCreatedLinkLayerState)
	{
		*simid = buildLinkesimID;
		*mode = i51NetMode;
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLinkLayerStatus , linklayer is true , %d,%d" , buildLinkesimID,i51NetMode);
#endif
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLinkLayerStatus , F , linklayer is false");
		return;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocGetLinkLayerStatus , E , %x,%x,%x" , linked, simid, mode);
#endif
}

//ok
iU32 i51AdeSocCreateLinkLayer ( iS32 mode , iS32 sim )
{
	//	author : jone
	//	since : 2011.11.8
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	iU16 i = 0 ;

	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCreateLinkLayer , S , %d,%d" , mode, sim);

	for ( ; i < i51SOCMAX_COUNT ; i ++ )
	{
		if ( !i51_LinkDivice[i].creatSate)	
			break ;
	}

	if( i  == i51SOCMAX_COUNT ) 
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCreateLinkLayer , F");
		return 0;
	}

	ven_std_memset(&i51_LinkDivice[i], 0, sizeof(i51LINKDIVICE));

	i51_LinkDivice[i].mode = mode;
	i51_LinkDivice[i].simID = sim;
	i51_LinkDivice[i].creatSate = 1 ;

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCreateLinkLayer , E");
#endif
	return i + 1;
}

//ok
iBOOL i51AdeSocOpenLinkLayer ( iU32 aid , iU32 layerid , i51LLAYERCALLBACK callback  )
{

	//	author : jone
	//	since : 2011.11.8
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	iU32 timerid;
	iU8 id = 0;

#ifdef i51ADAPTER_DEBUG		
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocOpenLinkLayer , S , %d,%x" , layerid, callback);
#endif
	if (0 == layerid||layerid >5 || NULL == callback || i51_LinkDivice[layerid - 1].creatSate == 0)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocOpenLinkLayer , ERR1");
		return iFALSE;
	}
	id = layerid - 1;
    
	i51_LinkDivice[id].i51buildLinkCB = callback;
	
	//	Jelo Added Since 2011.12.15
	//	判断应用链路表是否满
	//	如果满直接返回
	if ( i51AdeSocAidTableFull () ) {
	
		i51AdeOsLog(i51_LOG_SC , "SOC : i51AdeSocOpenLinkLayer , Link Layer Already Created") ;
	
		timerid = i51AdeOsTimerCreate(iFALSE, 0);
		i51AdeOsTimerStart(timerid, i51AdeSocOpenLink_CB, 30);
		i51_LinkDivice[id].tag = timerid;
		return iTRUE ;
		
	} else if ( iFALSE == i51AdeSocAidTableEmpty () ) {

	    timerid = i51AdeOsTimerCreate(iFALSE, 0);
		i51AdeOsTimerStart(timerid, i51AdeSocOpenLink_CB, 30);
		i51_LinkDivice[id].tag = timerid;
		i51_LinkDivice[id].openstate = 1;
		
		//	将Aid 创建链路的情况记录下来		
		if ( 0 == i51AdeSocAidTableFind ( aid ) ) {
			i51AdeSocAidTableAdd ( aid ) ;
		}
		
		return iTRUE ;
		
	}
	//	Jelo Adding Finished
	if (iFALSE == IsCreatingLinkLayerState)
	{
		//	Jelo Added Since 2011.12.15
		i51AdeSocLastAddAid = aid ;	
		//	Jelo Adding Finished
	
		if ( 1 == i51_LinkDivice[id].mode ) 
		{
			i51SocBuildLinkLayer ( "cmwap" , layerid) ;
#ifdef i51ADAPTER_DEBUG			
			i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocOpenLinkLayer , E, cmwap");
#endif
			return iTRUE ;
		} 
		else if ( 2 == i51_LinkDivice[id].mode ) 
		{
			i51SocBuildLinkLayer ( "cmnet" , layerid ) ;
#ifdef i51ADAPTER_DEBUG			
			i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocOpenLinkLayer , E, cmnet");
#endif
			return iTRUE ;
		} 
		else
		{
			//	Jelo Added Since 2011.12.15
			i51AdeSocLastAddAid = 0 ;	
			//	Jelo Adding Finished
			i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocOpenLinkLayer , ERR2");
			return iFALSE ;
		}
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocOpenLinkLayer , ERR3");
		return iFALSE;
	}
	
}

iBOOL i51AdeSocCloseLinkLayer ( iU32 aid , iU32 layerid )
{

	//	author : jone
	//	since : 2011.11.8
	//	(C) 2011 , PKIG Tech. Co., Ltd.

	ven_socket_ret_t ret = VEN_SOCKET_RET_FAILED ;
	
#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCloseLinkLayer , S ,%d,%d" , aid, layerid);
#endif
	if (0 == layerid||layerid >5)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCloseLinkLayer , ERR1 , %d" , layerid);
		return iFALSE;
	}

	//	Jelo Added Since 2011.12.15
	//	检查链路表，如果不存在链路则立即返回。
	if ( iTRUE == i51AdeSocAidTableEmpty () ) {
		return iTRUE ; 
	}
	//	Jelo Adding Finished
	
	if (i51_LinkDivice[layerid - 1].i51buildLinkCB == realCreateLink_CB)
	{
		realCreateLink_CB = NULL;
		gi51RealLinkLayerID = 0;
	}

	//	Jelo Editing Since 2011.12.15
	//	if (0 != i51CreateLinkCount)
	//	{
	//		i51CreateLinkCount -- ;
	//	}
	//	Jelo Editing Finished

	if (NULL != i51_LinkDivice[layerid - 1].i51buildLinkCB)
	{
		i51_LinkDivice[layerid - 1].i51buildLinkCB = NULL;
		i51_LinkDivice[layerid - 1].openstate = 0;
	}

	//	Jelo Added Since 2011.12.15
	//	如果链路没有创建过，则直接返回iTRUE。
	//	不影响上层逻辑。
	i51AdeSocAidTableDelete ( aid ) ;
	//	检查链路表，如果存在应用依赖链路层，则不关闭链路。
	if ( iFALSE == i51AdeSocAidTableEmpty () ) {
	
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCloseLinkLayer , ERR2 , someone using linklayer");
		return iTRUE ; 	
	}
	//	Jelo Adding Finished

	//	Jelo Editing Since 2011.12.15
	//	if (0 == i51CreateLinkCount)
	//	Editing Finished
	
	{
		if ( ven_socket_data_link_exist() )
		{
			ret = ven_socket_close_datalink( g_i51PkigHandle ) ;
		}

		if( VEN_SOCKET_RET_SUCCESS == ret )
		{
#ifdef i51ADAPTER_DEBUG		
			i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCloseLinkLayer , E");
#endif
			IsCreatedLinkLayerState = iFALSE;

			//	Jelo Editing Since 2011.12.15
			//	i51CreateLinkCount = 0;
			//	Editing Finished	
			buildLinkesimID = 0;
			i51NetMode = 1;

			return iTRUE ;
		} 
		else 
		{
			i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCloseLinkLayer , ERR3");
			return iFALSE ;
		}
	}
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCloseLinkLayer , E");
#endif
	return iTRUE ;
}

iBOOL i51AdeSocReleaseLinkLayer ( iU32 layerid ) 
{

	//	author : jone
	//	since : 2011.11.7
	//	(C) 2011 , PKIG Tech. Co., Ltd.
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocReleaseLinkLayer , S , %d" , layerid);
#endif	
	if (0 == layerid ||layerid >5)
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocReleaseLinkLayer , F , %d" , layerid);
		return iFALSE;
	}

	if (iTRUE == i51_LinkDivice[layerid - 1].creatSate)
	{
		ven_std_memset(&i51_LinkDivice[layerid - 1], 0, sizeof(i51LINKDIVICE));
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocReleaseLinkLayer , E");
#endif
		return iTRUE;
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocReleaseLinkLayer , F");
		return iFALSE;
	}
}

iBOOL i51AdeSocCreate ( i51ADECALLBACK callback ) {

	//	author : jone
	//	since : 2010.09.7
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCreate , S , %x" , callback);
#endif

	if (NULL != callback)
	{
		i51AdeCallback = callback;

		i51AdeOsTimerStart(i51AdeOsTimerCreate(iFALSE, 0), i51AdeSocCreateCallBack, 30);

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCreate , E");
#endif
		return iTRUE;
	}

	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocCreate , F");

	return iFALSE ;
} 

iBOOL i51AdeSocRelease ( void ) {

	//	author : jone
	//	since : 2010.09.7
	//	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocRelease , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "SOC : i51AdeSocRelease , E");
#endif

	return iTRUE ;
} 
