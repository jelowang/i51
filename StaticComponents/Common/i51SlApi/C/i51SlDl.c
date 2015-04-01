#include "i51kernel.h"
#include "i51AdeOs.h"
#include "i51AdeSoc.h"
//akilae add since 2012.7.11
#include "i51KitCore.h"

#define I51_CACHE_MAX_LEN	(2048)
#define I51_IP_ADDR	"120.197.129.19"
#define I51_DL_TIME_OUT	(30*1000)
#define i51_SEND_DATE "GET /%s/%s HTTP/1.1\r\nHost:  hz.51pk.com:80\r\nContent-Length:          0\r\n\r\n"
//#define i51_SEND_DATE	"POST /%s/%s%s HTTP/1.1\r\nHost: hz.51pk.com:80\r\nAccept-Encoding: gzip\r\nX-Online-Host: hz.51pk.com:80\r\nAccept-Language: zh-CN, en-US\r\nUser-Agent: NokiaN73-2/2.0626 S60/3.0 Profile/MIDP-2.0 Configuration/CLDC-1.1\r\nCookie: BAIDUID=81EC29DFA5EE5A49A0676CA23AC94F93:FG=1; BAIDU_WISE_UID=wiaui_1324128233_1167\r\nAccept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Charset: utf-8, iso-8859-1, utf-16, *;q=0.7\r\nContent-Length:          0\r\n\r\n"
typedef void (*KernelInstallAppCb)(iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount );
typedef struct
{
	//	installer参数
	struct {

		iU32 type;
		iU32 version;
		iU16 AppName[64];
		iCHAR uri[64] ;
		KernelInstallAppCb callback ;
	} InstallerParamer;

	//	2 为installer ,3 为ReInstaller
	iS8 type ;

	struct
	{
	
		i51SlDlCALLBACK callback;	
	}MiniDlStartParamer;	

	struct {

		//	0 cmwap ，1 cmnet 
		iU32 netmode ;
		iU32 CreateLinkTimerId ;
		iU32 layerid ;
		iS32 socketid ;
		iU32 ContentLen ;
		iU32 RevcLen ;
		iFILE RevcFile ;
		i51SOCADDR address ;
		iBOOL ParseHttpHeader ;
		iCHAR cache[I51_CACHE_MAX_LEN] ;
		
	}socket ;
	iU32 monitortimerType ;
	iU32 monitortimer ;
	iU32 recordtime ;
}I51MINIDL ;

static I51MINIDL i51MiniDl = {0} ;

iBOOL i51MiniSlDlStart (iHANDLE dl , const iCHAR *uri , iU32 urisize , const iCHAR* path , const iCHAR* name , iU32 versionId ,i51SlDlCOMMAND cmd , i51SlDlCALLBACK callback)  ;
iBOOL i51MiniSlDlStop( iHANDLE dl ) ;
static void i51MiniSocketStart () ;
static void i51MiniSocketFailProcess(iBOOL state) ;

static void i51MiniTimerOutCB ( iU32 id )
{

	//	author : william
	//	since : 20111125
	//	PKIG Tech. Co., Ltd.


	if (  i51AdeOsGetTick () - i51MiniDl.recordtime > I51_DL_TIME_OUT) {


		if ( i51MiniDl.monitortimer > 0 ) {

			i51AdeOsTimerStop ( i51MiniDl.monitortimer ) ;
			i51MiniDl.monitortimer  = 0 ;
		}	

		//	soc超时
		i51AdeOsLog ( 0 , "SL - Dl : timerout") ;
		i51MiniSocketFailProcess(iFALSE);
	}
	
}

static void i51MiniTimerOutStart ( iU32 monitortimerType )
{
	//	author : william
	//	since : 20111125
	//	PKIG Tech. Co., Ltd.

	//	参数为1，创建链路开启
	//	参数为2，soc开启
	
	i51MiniDl.monitortimer = i51AdeOsTimerCreate(1,1) ;
	i51MiniDl.monitortimerType = monitortimerType ;
	
	if ( i51MiniDl.monitortimer == 0 ) {

		//i51SlDlStopProcess () ;
		i51AdeOsLog ( 0 , "SL - Dl : i51MiniTimerOutStart CT F" ) ;
		//i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_CREATETIMER, 0 , 0 , 0 , 0 ) ;
		return ;
	}

	i51AdeOsTimerStart (i51MiniDl.monitortimer , i51MiniTimerOutCB ,3*1000) ;
	//i51AdeOsLog ( 0 , "SL - Dl : i51SlDlTimerStart timerid = %d" , i51SlDlDownloader.monitortimer) ;
	i51MiniDl.recordtime = i51AdeOsGetTick() ;

}


static iBOOL i51MiniCreateDir () {


	iU32 looper = 0 ;
	iCHAR absolute [64] = { 0 } ;
	iU16 abw [64] = { 0 } ;

	iCHAR* type1dir [] = { "i51" , "i51\\i51KIT" , "i51\\i51KIT\\PKiGAPI" , 0 } ;

	i51AdeOsFileAModeEnable () ;
	
	for ( looper = 0 ; looper < 3 ; looper ++ ) {

		memset ( absolute , 0 , sizeof(absolute) ) ;
		memset ( abw , 0 , sizeof(abw) ) ;

		if ( iTRUE == i51AdeOsMMemCardExist () )  {	
			
			sprintf ( (iCHAR*)absolute , "%s%s" , i51AdeOsGetPathSymbolEx () , type1dir[looper] );
		} else {
			sprintf ( (iCHAR*)absolute , "%s%s" , i51AdeOsGetPathSymbol (), type1dir[looper] );
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


static void i51MiniDlKitCoreCb( iS32 state , iU32 textlen , iU32 offset , iU32 totall , iU32 refcount )
{

	//	此函数只有i51MiniDlCoreKit调用的时候才会被调用
	if(iFALSE == state){
		if((i51MiniDl.type == 2 || i51MiniDl.type == 3) && i51MiniDl.InstallerParamer.callback != NULL)
			i51MiniDl.InstallerParamer.callback(0, iFALSE, 0, 0, 0, 0);
	}
	
	else if(state && textlen == totall)
	{
		if(i51MiniDl.type == 2 && i51MiniDl.InstallerParamer.callback != NULL){
			
			i51KernelInstallApplication( i51MiniDl.InstallerParamer.AppName, i51MiniDl.InstallerParamer.callback);
		}else if(i51MiniDl.type == 3 && i51MiniDl.InstallerParamer.callback != NULL){
		
			i51KernelReinstallApplication(i51MiniDl.InstallerParamer.AppName,  i51MiniDl.InstallerParamer.uri , i51MiniDl.InstallerParamer.version,  i51MiniDl.InstallerParamer.callback );
		}
	} else {

		i51MiniDl.InstallerParamer.callback("i51KitCore.kit", 0 ,  i51MiniDl.socket.RevcLen, i51MiniDl.socket.ContentLen, 1, 1);
	}
}


static void i51MiniSocketFailProcess(iBOOL state)
{
	i51MiniSlDlStop(0);
	if ( NULL == i51MiniDl.MiniDlStartParamer.callback ) {

		i51AdeOsLog(0, "SL -DL : CB NULL");
		return ;
	}

	if ( state ) 
		i51MiniDl.MiniDlStartParamer.callback(iTRUE, i51MiniDl.socket.ContentLen, 0, i51MiniDl.socket.ContentLen, 0);
	else
		i51MiniDl.MiniDlStartParamer.callback(iFALSE, 0, 0, 0, 0);
		
}



int i51Strncmp(iCHAR *src, iCHAR *dst, iS32 n)
{
    iS32 i;
    iCHAR a,b;
    for(i=0; i<n; i++)
    {
        a=src[i];
        if ((a >= 'A') && (a <= 'Z'))
            a += 'a' - 'A';

        b=dst[i];
        if ((b >= 'A') && (b <= 'Z'))
            b += 'a' - 'A';   

        if(a!=b) return -1;
    }
    return 0;
}

static iBOOL i51GetWord(iCHAR *src, iCHAR **ppFind, int *pFindLen)//获取src串的第一个单词，单词间以 " " "\r" "\n" 字符分割
//遇到/r/n/r/n返回非, Find为返回单词的首地址，FindLen为单词长度
{
    *ppFind = 0;
    *pFindLen=0;
    while(1)
    {
        if( i51Strncmp( src, "\r\n\r\n", 4) == 0 )
            return iFALSE;
        if( *src==' ' || *src=='\r' || *src=='\n' )
        {
            src++;
        }else{
            *ppFind = src;
            break;
        }
    }
    while(1)
    {
        src++;
        (*pFindLen)++; 
        if( *src==' ' || *src=='\r' || *src=='\n' )
        {
            return iTRUE;
        }
    }
    return iFALSE;
}

static void i51MiniSocketReceiveCB( iBOOL state , iS32 socket , iU8* buffer , iU32 textlen )
{

	iU32 RvceLen = textlen ;
	iU8 *revebuffer  = buffer ;
	i51MiniDl.recordtime = i51AdeOsGetTick() ;
	if ( !state ) {

		i51AdeOsLog(0, "SL -DL : RECE F");
		goto RVCE_ERROR ;
	}

	
	if ( !i51MiniDl.socket.ParseHttpHeader ) {

		iCHAR *pFind = NULL ;
		iS32 FindLen = 0 ;
		iU32 ContentLen = 0 ;
		iCHAR LenBuf[10] = {0} ;
		iU32 FindHttpEnd = 0 ;
		iBOOL FindHttpMark = iFALSE ; 


		i51MiniDl.socket.RevcLen = 0 ;
		i51MiniDl.socket.ContentLen = 0 ;
		
		i51AdeOsLogEx ( i51_LOG_SC , buffer , textlen ) ;
		//	跳过http
		if ( !i51GetWord ((iCHAR*) buffer , &pFind , &FindLen ) ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : http F") ;
			goto RVCE_ERROR ;
		}
		//	跳过/1.1
		if ( !i51GetWord ( pFind + FindLen  , &pFind , &FindLen ) ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : 1.1 F") ;
			goto RVCE_ERROR ;
		}
		//	分析状态码
		if ( i51Strncmp ( pFind , "200" , 3 ) != 0 ) {

			//i51AdeOsLog ( i51_LOG_SC , "SL - DL : State Code Err %s " , buffer ) ;
			goto RVCE_ERROR ;
		}

		while(1){
			
		    if( !i51GetWord(pFind+FindLen, &pFind, &FindLen) ) 
		        break;
			
//		    if ( i51Strncmp ( pFind , "application/octet-stream", 24 ) == 0 )
//		        Findi51Mark = 1;
			
		    if ( i51Strncmp ( pFind , "Content-length: ", 16 ) == 0 ){
				
		        if( !i51GetWord(pFind+FindLen, &pFind, &FindLen) ) 
		            break;
		        memset( LenBuf, 0, sizeof(LenBuf) );
		        strncpy( LenBuf, pFind, FindLen );
		        ContentLen = atoi( pFind );
			  i51MiniDl.socket.ContentLen = ContentLen ;
		    } 
			
		}
		if ( i51MiniDl.socket.ContentLen == 0 ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : None Find length" ) ;
			goto RVCE_ERROR ;
		}
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : filelen = %d " ,i51MiniDl.socket.ContentLen) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : come here(2) , %d , %d" ,FindHttpEnd , textlen ) ;
		while ( FindHttpEnd < textlen ) {

			if ( buffer[FindHttpEnd] != '\r' ) {
				FindHttpEnd ++ ;
				continue ;
			}
			if ( buffer[FindHttpEnd] == '\r' && buffer[FindHttpEnd+1] == '\n'  && buffer[FindHttpEnd+2] == '\r'  && buffer[FindHttpEnd+3] == '\n' ) {

				FindHttpMark = iTRUE ;
				break ;
			}else {

				FindHttpEnd ++ ;
				continue ;
			}
		}

		if ( !FindHttpMark) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : without find End") ;
			goto RVCE_ERROR ;
		}
		
		//i51MiniDl.socket.RevcLen = textlen - (FindHttpEnd+4) ;
		RvceLen = textlen - (FindHttpEnd+4) ;
		revebuffer = buffer + FindHttpEnd + 4 ;
		i51MiniDl.socket.ParseHttpHeader = iTRUE ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : RvceLen = %d" ,RvceLen ) ;
		
	}

	i51AdeOsLog ( i51_LOG_SC , "SL - DL : RevcFile = %d" ,  i51MiniDl.socket.RevcFile ) ;
	if ( i51MiniDl.socket.RevcFile  <= 0) {

		i51MiniCreateDir () ;

		i51MiniDl.socket.RevcFile = i51AdeOsFileOpen ( L"PKiGAPI\\i51KitCore.kit.tmp" , i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
		
		if (i51MiniDl.socket.RevcFile  <= 0) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : File Open F") ;
			goto RVCE_ERROR ;
		}
	}

	
	if( !i51AdeOsFileWrite ( i51MiniDl.socket.RevcFile , revebuffer, RvceLen , 0 )){	

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : File write F") ;
		goto RVCE_ERROR;
	}

	i51MiniDl.socket.RevcLen += RvceLen ;
	if ( i51MiniDl.socket.RevcLen ==  i51MiniDl.socket.ContentLen ) {

		if ( i51MiniDl.socket.RevcFile > 0 ) {

			i51AdeOsFileClose ( i51MiniDl.socket.RevcFile) ;
			i51MiniDl.socket.RevcFile = 0 ;
		}		
		if (! i51AdeOsFileRename (L"PKiGAPI\\i51KitCore.kit.tmp" , L"PKiGAPI\\i51KitCore.kit") ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : kit rename F") ;
			goto RVCE_ERROR ;
		}
		i51MiniSocketFailProcess(iTRUE);
		
	} else {

		if( i51MiniDl.MiniDlStartParamer.callback)
			 i51MiniDl.MiniDlStartParamer.callback(iTRUE,  i51MiniDl.socket.RevcLen, 0, i51MiniDl.socket.ContentLen, 0);
	}

	return ;

	RVCE_ERROR:
	i51MiniSocketFailProcess(iFALSE);

}

static void i51MiniSocketSendCB( iBOOL state , iS32 socket , iU32 textlen )
{
	i51AdeOsLog(0,"SocketSendCallback:%d",state);
	i51MiniDl.recordtime = i51AdeOsGetTick() ;
	if(state){

		memset ( i51MiniDl.socket.cache , 0 , sizeof(i51MiniDl.socket.cache) ) ;	
		i51AdeSocReceive( i51MiniDl.socket.socketid ,i51MiniDl.socket.cache,I51_CACHE_MAX_LEN,i51MiniSocketReceiveCB) ;
	}
	else{
		
		i51MiniSocketFailProcess(iFALSE);
	}
}

static void i51MiniSocketConnectCB( iBOOL state , iS32 socket )
{
	iCHAR serverDirName[32] = {0};
	i51AdeOsLog(0,"SocketConnectCallback:%d",state);
	i51MiniDl.recordtime = i51AdeOsGetTick() ;
	if(state) {

		memset ( i51MiniDl.socket.cache , 0 , sizeof(i51MiniDl.socket.cache) ) ;	
		
		if(i51AdeCnfGetCpu() == i51_CPU_MIPS)
			strcpy(serverDirName, "i51KitCore_MIPS");
		else if(i51AdeCnfGetCpu() == i51_CPU_ARM &&i51AdeCnfGetEndian() ==i51_ADE_CNF_LITTEL_ENDIAN)
			strcpy(serverDirName, "i51KitCore_ARM_LE");
		else
			strcpy(serverDirName, "i51KitCore_ARM_BE");
		
		 sprintf (  i51MiniDl.socket.cache , i51_SEND_DATE , serverDirName , "i51KitCore.kit") ;
		 i51AdeOsLogEx ( i51_LOG_SC , (iU8*)i51MiniDl.socket.cache , strlen (i51MiniDl.socket.cache) ) ;
		 i51AdeSocSend(i51MiniDl.socket.socketid ,(void*)i51MiniDl.socket.cache ,strlen(i51MiniDl.socket.cache), i51MiniSocketSendCB );
	}
	else{
		
		i51MiniSocketFailProcess(iFALSE);
	}
}


static void i51MiniSocketStart ()
{
	iU32 IpValue = 0 ;
	i51MiniDl.socket.socketid = i51AdeSocGet();
	if(i51MiniDl.socket.socketid < 0)
	{
		i51AdeOsLog(0,"i51AdeSocGet F");
		i51MiniSocketFailProcess(iFALSE);
		return;
	}

	i51AdeSocInetAton(I51_IP_ADDR ,&IpValue);
	i51MiniDl.socket.address.ipaddr = IpValue ;
	i51MiniDl.socket.address.port = 80 ;
	i51MiniTimerOutStart (2) ;
	i51AdeSocConnect(i51MiniDl.socket.socketid, &i51MiniDl.socket.address , i51MiniSocketConnectCB);
}
static void i51MiniOpenLinkCallback( iBOOL state )
{
	i51AdeOsLog(0,"i51MiniOpenLinkCallback:%d",state);
	i51MiniDl.recordtime = i51AdeOsGetTick() ;
	if ( i51MiniDl.monitortimer > 0 ) {

		i51AdeOsTimerStop ( i51MiniDl.monitortimer ) ;
		i51MiniDl.monitortimer  = 0 ;
	}
	
	if(state) {
		
		i51MiniSocketStart () ;
	}
	else {

		i51AdeOsLog(0,"OpenLinkCallback:%d",state);
		i51MiniSocketFailProcess(iFALSE);
	}
}

static void i51MiniCreateLinkTimerCB ( iU32 id )
{
	i51AdeOsTimerStop(i51MiniDl.socket.CreateLinkTimerId) ;
	i51MiniDl.socket.CreateLinkTimerId = 0 ;
	i51MiniSocketFailProcess(iFALSE);
	
}

void i51MiniDlCoreKit(iU32 type, iU32 inVersion, iU16 *inAppName, iCHAR *inUri,  KernelInstallAppCb installCallback)
{
	iCHAR url[128] = {0};
	iCHAR serverDirName[32] = {0};

	i51AdeOsLog ( i51_LOG_SC , "SL - DL : dl kitcore" ) ;
	
	i51MiniDl.type = type ;
	i51MiniDl.InstallerParamer.version = inVersion ;
	
	i51AdeStdWStrcpy ( i51MiniDl.InstallerParamer.AppName , inAppName ) ;
	if ( type == 3 ) {

		//	只有Reinstaller 这个参数才有效
		strcpy ( i51MiniDl.InstallerParamer.uri , inUri) ;
	}
	
	i51MiniDl.InstallerParamer.callback = installCallback ;


	if(i51AdeCnfGetCpu() == i51_CPU_MIPS)
		strcpy(serverDirName, "i51KitCore_MIPS");
	else if(i51AdeCnfGetCpu() == i51_CPU_ARM &&i51AdeCnfGetEndian() ==i51_ADE_CNF_LITTEL_ENDIAN)
		strcpy(serverDirName, "i51KitCore_ARM_LE");
	else
		strcpy(serverDirName, "i51KitCore_ARM_BE");

	sprintf(url, "GET /%s/%s HTTP/1.1\r\nHost:  hz.51pk.com:80\r\nContent-Length:          0\r\n\r\n",  serverDirName, "i51KitCore.kit");	
	i51MiniSlDlStart(0, url, strlen(url), "i51\\i51KIT\\PKiGAPI",  "i51KitCore.kit", 0, i51_SAP_DLCOM_BITSTREAM, i51MiniDlKitCoreCb);       

}



iBOOL i51MiniSlDlStart (iHANDLE dl , const iCHAR *uri , iU32 urisize , const iCHAR* path , const iCHAR* name , iU32 versionId ,i51SlDlCOMMAND cmd , i51SlDlCALLBACK callback) 
{

	iS32 *LinkMode = NULL ;
	iS32 *sims = NULL ;	
	if(NULL == uri || NULL == path || NULL == name){
		
		i51AdeOsLog(0, "i51MiniSlDlStart: uri or path or name is Empty, Fail");
		return iFALSE;
	}

	i51AdeOsLog(0, "SL -DL : minidl start uri=%s,path=%s, name=%s, urisize=%d", uri, path, name, urisize);

	i51MiniDl.MiniDlStartParamer.callback = callback;

	LinkMode =  i51AdeSocGetLinkLayerMode() ;
	sims = (iS32*)i51AdeSmsGetSims() ;

	if ( sims[0] == 0 ) {

		i51MiniDl.socket.CreateLinkTimerId= i51AdeOsTimerCreate (iFALSE , 1) ;
		i51AdeOsTimerStart (i51MiniDl.socket.CreateLinkTimerId, i51MiniCreateLinkTimerCB , 1) ;
		return iFALSE ;
	}
	i51MiniDl.socket.layerid = i51AdeSocCreateLinkLayer( LinkMode[2] , sims[1]) ;
	
	if(!i51MiniDl.socket.layerid ){

		i51MiniDl.socket.CreateLinkTimerId= i51AdeOsTimerCreate (iFALSE , 1) ;
		i51AdeOsTimerStart (i51MiniDl.socket.CreateLinkTimerId, i51MiniCreateLinkTimerCB , 1) ;
		return iFALSE ;
 	}

	if(iFALSE==i51AdeSocOpenLinkLayer((iU32)i51MiniSlDlStart , i51MiniDl.socket.layerid, i51MiniOpenLinkCallback)){

		i51AdeOsLog(0,"i51AdeSocOpenLinkLayer false");
		i51MiniDl.socket.CreateLinkTimerId= i51AdeOsTimerCreate (iFALSE , 1) ;
		i51AdeOsTimerStart (i51MiniDl.socket.CreateLinkTimerId, i51MiniCreateLinkTimerCB , 1) ;
		return iFALSE ;
	}
	i51MiniTimerOutStart (1) ;
	return iTRUE;
}


iBOOL i51MiniSlDlStop( iHANDLE dl )
{

	i51AdeOsLog(0, "SL -DL : minidl stop");
	if ( i51MiniDl.socket.CreateLinkTimerId > 0 ) {

		i51AdeOsTimerStop (i51MiniDl.socket.CreateLinkTimerId) ;
		i51MiniDl.socket.CreateLinkTimerId = 0 ;
	}

	
	if(i51MiniDl.socket.socketid>=0 ) 	
		i51AdeSocClose(i51MiniDl.socket.socketid);

	i51AdeSocCloseLinkLayer((iU32)i51MiniSlDlStart,i51MiniDl.socket.layerid);
	i51AdeSocReleaseLinkLayer(i51MiniDl.socket.layerid);

	i51MiniDl.socket.ParseHttpHeader = iFALSE ;
	if ( i51MiniDl.socket.RevcFile > 0 ) {

		i51AdeOsFileClose ( i51MiniDl.socket.RevcFile) ;
		i51MiniDl.socket.RevcFile = 0 ;
	}

	if ( i51MiniDl.monitortimer > 0 ) {
		
		i51AdeOsTimerStop ( i51MiniDl.monitortimer ) ;
		i51MiniDl.monitortimer  = 0 ;
	}
	return iTRUE;
}


iBOOL i51SlDlStart (iHANDLE dl , const iCHAR *uri , iU32 urisize , const iCHAR* path , const iCHAR* name , iU32 versionId ,i51SlDlCOMMAND cmd , i51SlDlCALLBACK callback) 
{
	//akilae add since 2012.7.13
	if(iNULL != i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitCore)){
		return i51KitCoreSlDlStart(dl, uri, urisize, path, name, versionId, cmd, callback);
	}
	
	if(iTRUE == i51KernelCheckKitCoreExist()){
		return i51KitCoreSlDlStart(dl, uri, urisize, path, name, versionId, cmd, callback);
	}else{
	/*
		sldlHandle = dl;
		sldlUriSize  = urisize;
		strcpy(slDlUri, uri);
		strcpy(slDlPath, path);
		strcpy(slDlName, name);
		sldlVersionId = versionId;
		sldlCmd = cmd;
		sldlCallback = callback;
		i51MiniDlCoreKit(1, 0, NULL, NULL, NULL);
		return iTRUE;
		*/
	}
	//akilae add end
}

iBOOL i51SlDlAbleCreate () {
	//akilae add since 2012.7.11	
	if(iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitCore))	{
		i51AdeOsLog(0, "i51SlDlAbleCreate F");
		return iFALSE;
	}
	
	return i51KitCoreSlDlAbleCreate();
	//akilae add finished
}

iHANDLE i51SlDlCreate (void (*callback) (iBOOL ret)) {	
	//akilae add since 2012.7.11	

	if(iTRUE == i51KernelCheckKitCoreExist() && iTRUE == i51KernelLoadKitCore ((iU32)i51SlDlCreate)) {

		return (iHANDLE)i51KitCoreSlDlCreate(callback);		
	}else {
	
		i51AdeOsLog(0, "i51SlDlCreate F");
		return iFALSE;
	}
	
	
	//akilae add finished
}


iBOOL i51SlDlStop( iHANDLE dl )
{
	//akilae add since 2012.7.11	
	iBOOL ret = iFALSE ;
	if(iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitCore)){
		i51AdeOsLog(0, "SLDL : SlDl Stop F");
		return iFALSE;
	}
	ret  = i51KitCoreSlDlStop(dl);
	i51KernelUnloadKitCore ((iU32)i51SlDlCreate) ;
	return ret ;
	//akilae add finished
}

