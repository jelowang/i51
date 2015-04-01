#include "i51Debuger.h"
#include "TReport.h"
i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

iU8 YPoint = 0;
iU8 Runstatus = 0;
iU8 AllStatics[21] = {0};
iU32 SimplePortalId = 0 ;
iCOLOR CloseColor = 0;
iCOLOR OkColor = 0;
iCOLOR ReportColor1 = 0xffff;
iCOLOR ReportColor2 = 0xffff;
static iU32 DoubleClick = 0;
static iU8 DoubleFlag = 0;
static iU8 FrameNume = 0;
static iU32 DoublePara = 0;
static iCHAR ReportBuff[20] = "Send Report?";
static iCHAR ReportDo[10] = "Send";
static iU8 WriteAllStatus = 0;
static iU8 WaveFlag = 0;
static iU8 KeyDoubleFlag = 0;

iCOLOR InterColor0[9] = {0};
iCOLOR InterColor1[9] = {0};
iCOLOR InterWordColor[9] = {0};

//static iU8 DFocus = 1;
static iU32 PlatId = 0;
static i51SOCADDR Reportaddress = {0};
static iU32 Reportlayerid = 0;
static iS32 ReportSocId = 0;
static iCHAR *ReportData = NULL;
static iBOOL CommitReport = 0;
static iU32 ReportTimeid = 0;
static iU8 SendButton = 0;

static void DrowKernel(void);
static void DrowAdapter(void);
static void Drowi51Loger(void);
static void i51GetLoger(void);
static void CreateReport();
static void ChangeDatatoHost();
static void ChangeDatatoHostB();

extern iU8 PaintStatus = 1;
extern iU16 ScreenWidth;
extern iU16 ScreenHeight;
extern iU32 OldSysTime;
extern iFILE TypeheadHandle;
extern iU32 CurrSysTime;
extern iS32 BenchmarkState;
extern iHANDLE VP_BK;
extern iHANDLE VP_Road;
extern iHANDLE VP_Snidget;
extern iHANDLE VP_Bee;
extern iHANDLE CurrentImageContainer;
extern iCHAR *SoundBuffer;
extern iCHAR *pSoundDataStdRecordPcm;

extern iU8 InterFoucs ;
//#define i51HTTP_REPORT_HEADER_PATTERN "POST /servlet/FileReaderServlet HTTP/1.1\r\nHost:  http://hz.51pk.com:80\r\nConnection: Close\r\n\r\n"
//#define i51HTTP_REPORT_HEADER_PATTERN "POST /servlet/FileReaderServlet HTTP/1.1\r\nX-Online-Host :  http://hz.51pk.com:80\r\nConnection: Close Content-Length:  %9d\r\n\r\n"
//#define i51HTTP_REPORT_HEADER_PATTERN "POST /servlet/FileReaderServlet HTTP/1.1\r\nX-Online-Host :  http://hz.51pk.com:80\r\nConnection: Close "
//#define i51HTTP_REPORT_HEADER_PATTERN "POST /servlet/FileReaderServlet HTTP/1.1\r\nX-Online-Host :  hz.51pk.com:80\r\n "
#define i51HTTP_REPORT_HEADER_PATTERN    "POST /servlet/FileReaderServlet HTTP/1.1\r\nHost: hz.51pk.com:80\r\nAccept-Encoding: gzip\r\nX-Online-Host: hz.51pk.com:80\r\nAccept-Language: zh-CN, en-US\r\nUser-Agent: NokiaN73-2/2.0626 S60/3.0 Profile/MIDP-2.0 Configuration/CLDC-1.1\r\nCookie: BAIDUID=81EC29DFA5EE5A49A0676CA23AC94F93:FG=1; BAIDU_WISE_UID=wiaui_1324128233_1167\r\nAccept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Charset: utf-8, iso-8859-1, utf-16, *;q=0.7\r\n"
//#define i51HTTP_REPORT_HEADER_PATTERN    "POST /MainServlet HTTP/1.1\r\nHost: sa.pkingame.net:80\r\nAccept-Encoding: gzip\r\nX-Online-Host: sa.pkingame.net:80\r\nAccept-Language: zh-CN, en-US\r\nUser-Agent: NokiaN73-2/2.0626 S60/3.0 Profile/MIDP-2.0 Configuration/CLDC-1.1\r\nCookie: BAIDUID=81EC29DFA5EE5A49A0676CA23AC94F93:FG=1; BAIDU_WISE_UID=wiaui_1324128233_1167\r\nAccept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Charset: utf-8, iso-8859-1, utf-16, *;q=0.7\r\n"

static void CloseReport()
{
    iFILE file = 0;

    CommitReport = 0;
    SendButton = 0;
    ResetInterStatus();
    memset(ReportBuff,0,sizeof(ReportBuff));
    sprintf(ReportBuff,"Send Report?");
    memset(ReportDo,0,sizeof(ReportDo));
    sprintf(ReportDo,"Send");
    if(ReportTimeid!=0)
    {
        i51AdeOsTimerStop(ReportTimeid);
        ReportTimeid = 0;
    }
    i51AdeOsLog(0,"Report bak Error.dat rs: %d", i51AdeOsFileCopy(L"i51Debuger\\i51PhoneTestFile\\Error.dat", L"i51Debuger\\i51PhoneTestFile\\Error.dat.bak"));
    i51AdeOsLog(0,"Report bak Status.dat rs: %d", i51AdeOsFileCopy(L"i51Debuger\\i51PhoneTestFile\\Status.dat", L"i51Debuger\\i51PhoneTestFile\\Status.dat.bak"));
    i51AdeOsLog(0,"Report bak TestReport.tr rs: %d", i51AdeOsFileCopy(L"i51Debuger\\i51PhoneTestFile\\TestReport.tr", L"i51Debuger\\i51PhoneTestFile\\TestReport.tr.bak"));
    i51AdeOsLog(0,"Report bak Testt.tr rs: %d", i51AdeOsFileCopy(L"i51Debuger\\i51PhoneTestFile\\Testt.tr", L"i51Debuger\\i51PhoneTestFile\\Testt.tr.bak"));

    file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\TestReport.tr" , i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
    if(file!=0)i51AdeOsFileClose (file) ;
    file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\Testt.tr" , i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
    if(file!=0)i51AdeOsFileClose (file) ;
}
static void DoReportOk()
{
    iFILE file = 0;
    if(WriteAllStatus)
    {
        file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\TestReport.tr" , i51_OS_FL_EXISTING_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
        if(file!=0)
        {
            i51AdeOsLog(0,"write Case Times");
            i51KitTReportAddCase (file,i51_CASE_ADAPTER,iTRUE) ;
            i51KitTReportAddCase (file,i51_CASE_KERNEL,iTRUE) ;
            i51KitTReportAddCase (file,i51_CASE_BENCHMARK,iTRUE) ;
            i51KitTReportAddCase (file,i51_CASE_INTER,iTRUE) ;
            i51KitTReportAddCase (file,i51_CASE_ENVIR,iTRUE) ;
            i51AdeOsFileClose (file) ;
        }
        if ((PlatId==2) && (i51_ADE_CNF_BIG_ENDIAN == i51AdeCnfGetEndian()))
        {
            ChangeDatatoHostB();
        }
        else
        {
            ChangeDatatoHost();
        }

        WriteAllStatus = 0;
    }
    memset(ReportBuff,0,sizeof(ReportBuff));
    sprintf(ReportBuff,"Sending...");

    CreateReport();
}
static iU32 GetSendData()
{
    iFILE file = 0;
    iS32 fileSize = 0;
    iU32 length = 0;
    iCHAR * po = NULL;
    iU32 ReadLen = 0;
    iCHAR buff[30] = {0};
    length = strlen(i51HTTP_REPORT_HEADER_PATTERN);


    file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\Testt.tr" , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
    if(file==0)
    {
        return 0 ;
    }
    i51AdeOsFileGetSize(file,&fileSize);
    if( NULL!= ReportData)
    {
        i51AdeOsFree((void *)ReportData);
    }

    ReportData = (iCHAR *)i51AdeOsMalloc(length+fileSize+50, __FILE__ , __LINE__);

    if(NULL==ReportData)
    {
        i51AdeOsFileClose(file);
        return 0;
    }
    memset(ReportData,0,sizeof(ReportData));
    memset(buff,0x00,sizeof(buff));
    sprintf(buff,"Content-Length:%9d\r\n\r\n",fileSize);

    memcpy(ReportData,i51HTTP_REPORT_HEADER_PATTERN,length);
    po = ReportData + length;
    memcpy(po,buff,strlen(buff));
    po = po+strlen(buff);
    i51AdeOsFileRead(file, (void*)po ,fileSize,&ReadLen);
    i51AdeOsFileClose(file);
    i51AdeOsLog(0,"str:%d,filesize:%d",length,fileSize);
    i51AdeOsFileClose(file);

    return (length+fileSize+30);
}

static void ReportRecCallback( iBOOL state , iS32 socket , iU8* buffer , iU32 textlen )
{
    iFILE file = 0;
    i51AdeOsLog(0,"i51 rec ..:%d,%d",state,textlen);

    if(state)
    {
        file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\tttttt.tr" , i51_OS_FL_ALWAYS_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
        if(file!=0)
        {
            i51AdeOsLog(0,"write");
            i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_TAIL ) ;
            i51AdeOsFileWrite ( file , "one write\r\n", strlen("one write\r\n") , 0 ) ;
            i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_TAIL ) ;
            i51AdeOsFileWrite ( file , buffer, textlen , 0 ) ;
            i51AdeOsFileClose(file);
        }
    }
    else
    {
        i51AdeSocClose(ReportSocId);
        i51AdeSocCloseLinkLayer(SimplePortalId,Reportlayerid);
        i51AdeSocReleaseLinkLayer(Reportlayerid);
    }
}

static void ColseCallback(iU32 ID)
{
    static iU8 statuNum = 0;
    statuNum++;

    memset(ReportBuff,0,sizeof(ReportBuff));
    if(statuNum==1)
    {
        sprintf(ReportBuff,"Close -3-");
    }
    else  if(statuNum==2)
    {
        sprintf(ReportBuff,"Close -2-");
    }
    else  if(statuNum==3)
    {
        sprintf(ReportBuff,"Close -1-");
    }
    else
    {
        statuNum = 0;
        CloseReport();
        i51AdeOsTimerStop(ReportTimeid);
        ReportTimeid = 0;
        SendButton = 0;
    }
}

static void ReportSocketSendCallback( iBOOL state , iS32 socket , iU32 textlen )
{
    static iCHAR buff[2048] = {0};

    i51AdeOsLog(0," i51AdeSocSend callback:state:%d,textlen:%d",state,textlen);
    if(state!=1)
    {
        SendButton = 1;
        memset(ReportBuff,0,sizeof(ReportBuff));
        sprintf(ReportBuff,"Send Failed");
        memset(ReportDo,0,sizeof(ReportDo));
        sprintf(ReportDo,"Resend");

        i51AdeSocClose(ReportSocId);
        i51AdeSocCloseLinkLayer(SimplePortalId,Reportlayerid);
        i51AdeSocReleaseLinkLayer(Reportlayerid);
    }
    else
    {
        SendButton = 0;
        memset(ReportDo,0,sizeof(ReportDo));
        memset(ReportBuff,0,sizeof(ReportBuff));
        sprintf(ReportBuff,"Send Succeed");
        ReportTimeid = i51AdeOsTimerCreate (1,1) ;
        i51AdeOsTimerStart(ReportTimeid,ColseCallback,1000);
        i51AdeSocReceive( ReportSocId ,buff,2048,ReportRecCallback) ;
    }
}
static void ReportSockitConnectCallback( iBOOL state , iS32 socket )
{
    iU32 len = 0;
    iU32 dataLength = 0;
    iFILE file = 0;
    i51AdeOsLog(0," i51AdeSocConnect callback:%d",state );
    if(state)
    {
        SendButton = 0;
        dataLength = GetSendData();
        if(dataLength>0)
        {   
            len = strlen(ReportData);
            i51AdeOsLog(0,"to i51AdeSocSend ture:len%d,dataLength:%d,ReportSocId:%d",len,dataLength,ReportSocId);

            file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\senddata.tr" , i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
            if(file!=0)
            {
                i51AdeOsFileWrite(file , ReportData ,dataLength,0);
            }
            i51AdeOsFileClose(file);

            i51AdeSocSend(ReportSocId ,(void*)ReportData ,dataLength, ReportSocketSendCallback );
            if(NULL!=ReportData)
            {
                i51AdeOsFree(ReportData);
                ReportData = NULL;
            }
            i51AdeOsLog(0,"to i51AdeSocSend");
        }
    }
    else
    {
        SendButton = 1;
        memset(ReportBuff,0,sizeof(ReportBuff));
        sprintf(ReportBuff,"Send Failed");
        i51AdeOsLog(0," i51AdeSocConnect callback false");
        memset(ReportDo,0,sizeof(ReportDo));
        sprintf(ReportDo,"Resend");
        i51AdeSocClose(ReportSocId);
        i51AdeSocCloseLinkLayer(SimplePortalId,Reportlayerid);
        i51AdeSocReleaseLinkLayer(Reportlayerid);
    }
}
static void ReportLinkerCallback( iBOOL state )
{
    i51AdeOsLog(0,"ReportLinkerCallback");
    if(state)
    {
        SendButton = 0;
        ReportSocId = i51AdeSocGet();
        if(ReportSocId<0||ReportSocId>4)
        {
            memset(ReportDo,0,sizeof(ReportDo));
            sprintf(ReportDo,"Resend");
            memset(ReportBuff,0,sizeof(ReportBuff));
            sprintf(ReportBuff,"Send Failed");
            SendButton = 1;
            return;
        }

        i51AdeOsLog(0,"to i51AdeSocConnect ReportSocId:%d,net:%d",ReportSocId,Reportaddress.ipaddr);

        i51AdeSocConnect(ReportSocId, &Reportaddress , ReportSockitConnectCallback);
    }
    else
    {
        SendButton = 1;
        memset(ReportBuff,0,sizeof(ReportBuff));
        sprintf(ReportBuff,"Send Failed");
        memset(ReportDo,0,sizeof(ReportDo));
        sprintf(ReportDo,"Resend");
        i51AdeOsLog(0,"ReportLinkerCallback false");
        i51AdeSocReleaseLinkLayer(Reportlayerid);
    }
}

static void ReportGetIpCallback(iU32 add)
{
    Reportaddress.port = 80;
    Reportaddress.ipaddr = add;
    i51AdeOsLog(0,"Get ReportIp:%d",add);
}

static void CreateReport()
{
    iS32 *LinkMode = NULL ;
    iS32 *sims = NULL ;    
    static iCHAR *ss="hz.51pk.com";
    static iCHAR *pp ="120.197.129.19";
    iU32 net = 0;

    LinkMode =  i51AdeSocGetLinkLayerMode () ;
    sims = i51AdeSmsGetSims () ;
    i51AdeOsLog(0,"LinkMode[0]:%d,LinkMode[1]:%d,LinkMode[2]:%d ",LinkMode[0],LinkMode[1],LinkMode[2]);;
    Reportlayerid = i51AdeSocCreateLinkLayer( LinkMode[2] , sims[1]) ;

    SendButton = 0;
    if(!Reportlayerid)
    {
        SendButton = 1;
        memset(ReportBuff,0,sizeof(ReportBuff));
        sprintf(ReportBuff,"Send Failed");

        memset(ReportDo,0,sizeof(ReportDo));
        sprintf(ReportDo,"Resend");
        return;
    }

    i51AdeSocInetAton(pp,&net);

    Reportaddress.port = 80;
    Reportaddress.ipaddr = net;//i51AdeSocHtonl(net);


    i51AdeSocOpenLinkLayer(SimplePortalId,Reportlayerid, ReportLinkerCallback);
}


void i51WriteError(void *str)
{
    iFILE file = 0;
    iU32 WriteLen = 0;
    file = i51AdeOsFileOpen (L"i51Debuger\\i51PhoneTestFile\\Error.dat" , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN , __FILE__ , __LINE__ ) ;
    if(file!=0)
    {
        i51AdeOsFileSetSeek(file ,0 , i51_OS_FL_SEEK_TAIL);
        i51AdeOsFileWrite(file ,  str ,strlen(str),&WriteLen);
        i51AdeOsFileWrite(file ,  "\r\n" ,strlen("\r\n"),&WriteLen);
        i51AdeOsFileClose(file);
    }
}

void DrowCheckResult()
{
    iFILE file = 0;
    iCHAR ErrorBuff[64] = {0};
    iCHAR data =0;
    iU32 Read =0;
    iS32 size = 0;
    iU8 i = 0;
    iU16 num = 0;
    file = i51AdeOsFileOpen (L"i51Debuger\\i51PhoneTestFile\\Error.dat" , i51_OS_FL_WRITE |i51_OS_FL_ALWAYS_OPEN , __FILE__ , __LINE__ ) ;
    if(file!=0)
    {
        i51AdeOsFileGetSize(file ,&size );
        YPoint = 0;
        if(size==0)
        {
            DrowTestLog("TotallBugs is 0,Check Passed.....",3);
            i51AdeOsFileClose(file);
            i51KitG2Update();
            return;
        }
        DrowTestLog("Error List:",3);
        while(size>0)
        {
            i51AdeOsFileRead(file ,&data,1,&Read );
            size--;
            if(data=='\r')
            {
                DrowTestLog(ErrorBuff,0);
                num++;
                memset(ErrorBuff,0,sizeof(ErrorBuff));
                i = 0;
            }
            if(data!='\n'&&data!='\r')
            {
                ErrorBuff[i] = data;
                i++;
            }
        }

        memset(ErrorBuff,0,sizeof(ErrorBuff));
        sprintf(ErrorBuff,"TotallBugs is:%d",num);
        DrowTestLog(ErrorBuff,3);

        i51AdeOsFileClose(file);
        file = i51AdeOsFileOpen (L"i51Debuger\\i51PhoneTestFile\\Error.dat" , i51_OS_FL_WRITE |i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
        if(file!=0)
        {
            i51AdeOsFileClose(file);
        }
        i51KitG2Update();
    }
}


void DrowTestLog(iCHAR * text,iU8 DrowFlag)
{
    iU16 lwidth = 0 ;
    iU16 lheight = 0 ;
    iRECT rect = {0};
    iU32 DrawWidth = 0;
    iU32 DrawHeight = 0;
    iCOLOR color;
    iU16 HeightEnd = 0;
    iU16 HeightHead = 0;

    i51AdeMmiGetScreenScale ( &lwidth , &lheight ) ;
    HeightEnd = lheight/12;
    HeightHead = lheight/10;


    if(YPoint>=((lheight-HeightHead-HeightEnd)/14)||YPoint==0)
    {
        rect.X = 5;
        rect.Y = HeightHead+1;
        rect.Height = lheight - HeightHead-HeightEnd-1;
        rect.Width = lwidth-6;
        i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),&rect);
        YPoint=0;
    }

    rect.X = 5;
    rect.Y = YPoint*14+HeightHead;
    rect.Height = 14;
    rect.Width = lwidth;
    if(lwidth==240&&lheight==320)
        i51KitG2FontDraw (TypeheadHandle,0,text ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    else
        i51KitG2FontDraw (TypeheadHandle,1,text ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);

    i51KitG2FontGetDemension (TypeheadHandle,1,3,0,0,0,0,&DrawWidth,&DrawHeight);
    if(DrowFlag==0)
    {              
        rect.X = lwidth-DrawWidth-1;
        rect.Y = YPoint*14+HeightHead;
        rect.Height = 14;
        rect.Width = DrawWidth;
        color = i51KitG2COLOR(240, 65, 85);
        if(lwidth==240&&lheight==320)
            i51KitG2FontDraw (TypeheadHandle,0,"NO" ,0,&rect , &rect,0, 0,  0,  color , 0, 0);
        else
            i51KitG2FontDraw (TypeheadHandle,1,"NO" ,0,&rect , &rect,0, 0,  0,  color , 0, 0);
    }
    else if(DrowFlag==1)
    {
        rect.X = lwidth-DrawWidth-1;
        rect.Y = YPoint*14+HeightHead;
        rect.Height = 14;
        rect.Width = DrawWidth;
        color = i51KitG2COLOR(0,255,0);
        if(lwidth==240&&lheight==320)
            i51KitG2FontDraw (TypeheadHandle,0,"YES" ,0,&rect , &rect,0, 0,  0,  color , 0, 0);
        else
            i51KitG2FontDraw (TypeheadHandle,1,"YES" ,0,&rect , &rect,0, 0,  0,  color , 0, 0);
    }

    YPoint++;
    i51KitG2Update();

}

void DrowG2TestLog(iCHAR * text)
{
    iU16 lwidth = 0 ;
    iU16 lheight = 0 ;
    iRECT rect = {0};
    iU16 HeightEnd = 0;
    iU16 HeightHead = 0;

    i51AdeMmiGetScreenScale ( &lwidth , &lheight ) ;
    HeightEnd = lheight/12;
    HeightHead = lheight/10;


    if(YPoint>=((lheight-HeightHead)/14))
    {
        rect.X = 5;
        rect.Y = HeightHead+1;
        rect.Height = lheight - HeightHead-2;
        rect.Width = lwidth-6;
        i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),&rect);
        YPoint=0;
    }

    rect.X = 5;
    rect.Y = YPoint*14+HeightHead;
    rect.Height = 14;
    rect.Width = lwidth;
    i51KitG2FontDraw (TypeheadHandle,1,text ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);

    YPoint++;
    i51KitG2Update();
}

void i51PhoneDrawTitle(void *tile,iU16 num)
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightPerHead = 0;
    iU16 HeightPerEnd = 0;


    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightPerEnd = (height)/12;
    HeightPerHead = (height)/10;
    HeightPerOne = (height-HeightPerHead-HeightPerEnd)/8;
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();
    rect.X = width/4;
    rect.Y = HeightPerHead/4+2;
    rect.Height = HeightPerHead;
    rect.Width = width;

}
void i51PhoneDrawRect()
{
    iS32 Point[2][2] = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    Point[0][0] = 0;
    Point[0][1] = 0;
    Point[1][0] = width;
    Point[1][1] = 0;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 0;
    Point[0][1] = 0;
    Point[1][0] = 0;
    Point[1][1] = height;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = width-1;
    Point[0][1] = 0;
    Point[1][0] = width-1;
    Point[1][1] = height;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 0;
    Point[0][1] = height-1;
    Point[1][0] = width;
    Point[1][1] = height-1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 0;
    Point[0][1] = height/10;
    Point[1][0] = width;
    Point[1][1] = height/10;
    i51KitG2DrawLine( Point, 0xffff);
    if(!(Runstatus==3&&(BenchmarkState==BENCHMARK_CPU||BenchmarkState==BENCHMARK_IO||BenchmarkState==BENCHMARK_MEDIA)))
    {  
        Point[0][0] = 0;
        Point[0][1] = height- height/12;
        Point[1][0] = width;
        Point[1][1] = height- height/12;
        i51KitG2DrawLine( Point, 0xffff);    
    }
}

void DrawText(iU16 width , iU16 height , const iU16* text , iS32 desx , iS32 desy , iCOLOR color)
{
    iU32* panel = NULL;
    panel = i51AdeMmiGetPanel ();
    i51AdeMmiDrawText ((iU8*)panel ,width ,height , text ,desx ,desy ,color);
}


static void DrowStatus(iRECT rect,iU8 Line)
{
    iU8 num = AllStatics[Line-1];
    iRECT rrcte = {0};
    iU16 StrWidth = 0,StrHeight = 0;
    iU16 StrAllWidth = 0,StrAllHeight = 0;
    iU16 StrConWidth = 0,StrConHeight = 0;
    iU16 strlength = 0;

    memcpy(&rrcte,&rect,sizeof(iRECT));
    if(Line>4)Line--;
    i51AdeMmiGetTextAttr (L"CHECKING", &StrAllWidth , &StrAllHeight);
    i51AdeMmiGetTextAttr (L"[", &StrConWidth , &StrConHeight);
    strlength = StrConWidth;
    if(ScreenWidth==240&&ScreenHeight==320)
    {
        strlength = StrConWidth/2;
    }
    switch(num)
    {
    case 0:
        {
            DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            i51AdeMmiGetTextAttr (L"UNTEST", &StrWidth , &StrHeight);
            rrcte.X = rect.X + strlength +StrAllWidth/2 - StrWidth/2;
            DrawText(rrcte.Width ,rrcte.Height ,L"UNTEST",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);

            rrcte.X = rect.X + StrConWidth +StrAllWidth;

            DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
        }
        break;
    case 1:
        {
            DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            rrcte.X = rect.X + strlength;
            DrawText(rrcte.Width ,rect.Height ,L"CHECKING",rrcte.X ,rrcte.Y ,0XFF00);

            rrcte.X = rect.X + StrConWidth + StrAllWidth;
            DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
        }
        break;
    case 2:
        {  
            DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            i51AdeMmiGetTextAttr (L"PASSED", &StrWidth , &StrHeight);
            rrcte.X = rect.X +strlength+StrAllWidth/2 - StrWidth/2;
            DrawText(rrcte.Width ,rrcte.Height ,L"PASSED",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));

            rrcte.X = rect.X + StrConWidth+StrAllWidth;
            DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
        }
        break;
    case 3:
        {
            DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            i51AdeMmiGetTextAttr (L"FAILED", &StrWidth , &StrHeight);
            rrcte.X = rect.X+strlength+ StrAllWidth/2 - StrWidth/2;
            DrawText(rrcte.Width ,rrcte.Height ,L"FAILED",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));

            rrcte.X = rect.X + StrConWidth + StrAllWidth;
            DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
        }
        break;
    }
}

static void DrawReport()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 1;
    iRECT rect = {0};
    iU16 StrWidth = 0,StrHeight = 0;
    iU16 OkWidth = 0,OkHeight = 0;
    iU16 databuff[20] = {0};
    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;

    HeightHead = Theight/10;
    HeightEnd = Theight/12;
    HeightPerOne = (Theight - HeightEnd - HeightHead)/8;

    rect.X = 22;
    rect.Y = Theight-HeightEnd -(Theight-HeightEnd)/3-8;
    rect.Height = (Theight-HeightEnd)/3+3;
    rect.Width = Twidth - 40+3;
    i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

    rect.X = 20;
    rect.Y = Theight-HeightEnd -(Theight-HeightEnd)/3-10;
    rect.Height = (Theight-HeightEnd)/3;
    rect.Width = Twidth - 40;
    i51KitG2CleanScreen(0xffff,&rect);

    i51AdeStdAscii2Unicode(ReportBuff , databuff);
    i51AdeMmiGetTextAttr (databuff, &StrWidth , &StrHeight);
    rect.X = 20+(Twidth - 40)/2 - StrWidth/2;
    rect.Y = Theight-HeightEnd -(Theight-HeightEnd)/3+(Theight-HeightEnd)/6-StrHeight/2-20;
    rect.Height = (Theight-HeightEnd)/3;
    rect.Width = Twidth - 40;
    DrawText(Twidth ,HeightHead ,databuff,rect.X ,rect.Y ,i51KitG2COLOR(160,160,160));

    memset(databuff,0,sizeof(databuff));
    i51AdeStdAscii2Unicode(ReportDo , databuff);
    i51AdeMmiGetTextAttr (databuff, &OkWidth , &OkHeight);
    if(SendButton)
    {
        if(ReportColor1== 0xffff)
        {
            i51AdeMmiGetTextAttr (L"Resend", &StrWidth , &StrHeight);
            rect.X =24-2;
            rect.Y = Theight-HeightEnd -42-2;
            rect.Height = StrHeight+4;
            rect.Width = StrWidth+4;
            i51KitG2CleanScreen(ReportColor1,&rect);

            rect.X =24-1;
            rect.Y = Theight-HeightEnd -42-2+StrHeight+4;
            rect.Height = 3;
            rect.Width = StrWidth+5;
            i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

            rect.X =24+StrWidth+2;
            rect.Y = Theight-HeightEnd -42-1;
            rect.Height = StrHeight+5;
            rect.Width = 3;
            i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

            rect.X =24-2+(StrWidth+4)/2-OkWidth/2;
            rect.Y = Theight-HeightEnd -42;
            rect.Height = 32;
            rect.Width = StrWidth;
            DrawText(StrWidth ,32 ,databuff,rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
        }
        else
        {
            i51AdeMmiGetTextAttr (L"Resend", &StrWidth , &StrHeight);
            rect.X =24;
            rect.Y = Theight-HeightEnd -42;
            rect.Height = StrHeight+4;
            rect.Width = StrWidth+4;
            i51KitG2CleanScreen(ReportColor1,&rect);

            rect.X =24+1;
            rect.Y = Theight-HeightEnd -42+StrHeight+4;
            rect.Height = 2;
            rect.Width = StrWidth+5;
            i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

            rect.X =24+StrWidth+4;
            rect.Y = Theight-HeightEnd -42+1;
            rect.Height = StrHeight+5;
            rect.Width = 2;
            i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

            rect.X =24+(StrWidth+4)/2-OkWidth/2;
            rect.Y = Theight-HeightEnd -42+2;
            rect.Height = 32;
            rect.Width =StrWidth;
            DrawText(StrWidth ,32 ,databuff,rect.X ,rect.Y ,0);
        }
    }

    i51AdeMmiGetTextAttr (L"Close", &OkWidth , &OkHeight);
    if(ReportColor2 == 0xffff)
    {
        i51AdeMmiGetTextAttr (L"Resend", &StrWidth , &StrHeight);
        rect.X =Twidth-StrWidth-30-2;
        rect.Y = Theight-HeightEnd -44;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(ReportColor2,&rect);

        rect.X =Twidth-StrWidth-30-1;
        rect.Y = Theight-HeightEnd -42+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =Twidth-StrWidth-30+StrWidth+4;
        rect.Y =Theight-HeightEnd -42+1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =Twidth-StrWidth-30+(StrWidth+4)/2-OkWidth/2;
        rect.Y = Theight-HeightEnd -42;
        rect.Height = 32;
        rect.Width = (Twidth - 40)/2;
        DrawText(StrWidth ,32 ,L"Close",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Resend", &StrWidth , &StrHeight);
        rect.X =Twidth-StrWidth-30;
        rect.Y = Theight-HeightEnd -42;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(ReportColor2,&rect);

        rect.X =Twidth-StrWidth-30+1;
        rect.Y = Theight-HeightEnd -42+StrHeight+4;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =Twidth-StrWidth-30+StrWidth+4;
        rect.Y = Theight-HeightEnd -42-1;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =Twidth-StrWidth-30+2+(StrWidth+4)/2-OkWidth/2;
        rect.Y = Theight-HeightEnd -42+2;
        rect.Height = 32;
        rect.Width = (Twidth - 40)/2;
        DrawText(StrWidth ,32 ,L"Close",rect.X ,rect.Y ,0);
    }
}
void DrawCloseButton()
{
    iU16 StrWidth = 0,StrHeight = 0;
    iU16 StrOKWidth = 0,StrOKHeight = 0;
    iU32 GetWidth = 0,GetHeight = 0;
    iU32 GetOkWidth = 0,GetOkHeight = 0;
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iRECT rect = {0};
    iU16 HeightEnd = 0;

#define PERLEN 6

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightEnd = Theight/12;
    if(Twidth==240&&Theight==320)
    {
        i51KitG2FontGetDemension (TypeheadHandle,1,6,0,0,0,0,&GetWidth,&GetHeight);
        i51KitG2FontGetDemension (TypeheadHandle,1,2,0,0,0,0,&GetOkWidth,&GetOkHeight);
        GetWidth = GetWidth/2;
        GetOkWidth = GetOkWidth/2;
        if(CloseColor == 0xffff)
        {
            rect.X =Twidth - PERLEN -GetWidth;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2-1+2;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

            rect.X =Twidth - PERLEN -GetWidth-2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2-1;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(CloseColor,&rect);

            rect.X =Twidth - PERLEN -GetWidth-1;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2FontDraw (TypeheadHandle,1,"Cancel" ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(128, 128, 128) , 0, 0);
        }
        else
        {
            rect.X =Twidth - PERLEN -GetWidth+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+3;
            rect.Height = GetHeight+1;
            rect.Width = GetWidth+1;
            i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

            rect.X =Twidth - PERLEN -GetWidth-2+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+1;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(CloseColor,&rect);

            rect.X =Twidth - PERLEN -GetWidth-1+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+2;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2FontDraw (TypeheadHandle,1,"Cancel" ,0,&rect , &rect,0, 0,  0, 0 , 0, 0);
        }
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        if(CloseColor == 0xffff)
        {
            rect.X =Twidth - PERLEN -StrWidth;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

            rect.X =Twidth - PERLEN -StrWidth-2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(CloseColor,&rect);

            rect.X =Twidth - PERLEN -StrWidth-1;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
        }
        else
        {
            rect.X =Twidth - PERLEN -StrWidth+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2+2;
            rect.Height = StrHeight+1;
            rect.Width = StrWidth+1;
            i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

            rect.X =Twidth - PERLEN -StrWidth-2+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(CloseColor,&rect);

            rect.X =Twidth - PERLEN -StrWidth-1+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,0);
        }
    }

}
void DrawTowButton()
{
    iU16 StrWidth = 0,StrHeight = 0;
    iU16 StrOKWidth = 0,StrOKHeight = 0;
    iU32 GetWidth = 0,GetHeight = 0;
    iU32 GetOkWidth = 0,GetOkHeight = 0;
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iRECT rect = {0};
    iU16 HeightEnd = 0;

#define PERLEN 6

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightEnd = Theight/12;
    if(Twidth==240&&Theight==320)
    {
        i51KitG2FontGetDemension (TypeheadHandle,1,6,0,0,0,0,&GetWidth,&GetHeight);
        i51KitG2FontGetDemension (TypeheadHandle,1,2,0,0,0,0,&GetOkWidth,&GetOkHeight);
        GetWidth = GetWidth/2;
        GetOkWidth = GetOkWidth/2;
        if(OkColor== 0xffff)
        {
            rect.X =PERLEN;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+1;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

            rect.X =PERLEN-2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2-1;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(OkColor,&rect);

            rect.X =PERLEN-2+(GetWidth+2-GetOkWidth)/2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2FontDraw (TypeheadHandle,1,"Ok" ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(128, 128, 128) , 0, 0);
        }
        else
        {
            rect.X =PERLEN+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+3;
            rect.Height = GetHeight+1;
            rect.Width = GetWidth+1;
            i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

            rect.X =PERLEN;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+1;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(OkColor,&rect);

            rect.X =PERLEN+(GetWidth+2-GetOkWidth)/2+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+2;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2FontDraw (TypeheadHandle,1,"Ok" ,0,&rect , &rect,0, 0,  0,  0 , 0, 0);
        }

        if(CloseColor == 0xffff)
        {
            rect.X =Twidth - PERLEN -GetWidth;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2-1+2;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

            rect.X =Twidth - PERLEN -GetWidth-2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2-1;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(CloseColor,&rect);

            rect.X =Twidth - PERLEN -GetWidth-1;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2FontDraw (TypeheadHandle,1,"Cancel" ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(128, 128, 128) , 0, 0);
        }
        else
        {
            rect.X =Twidth - PERLEN -GetWidth+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+3;
            rect.Height = GetHeight+1;
            rect.Width = GetWidth+1;
            i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

            rect.X =Twidth - PERLEN -GetWidth-2+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+1;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2CleanScreen(CloseColor,&rect);

            rect.X =Twidth - PERLEN -GetWidth-1+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -GetHeight/2+2;
            rect.Height = GetHeight+2;
            rect.Width = GetWidth+2;
            i51KitG2FontDraw (TypeheadHandle,1,"Cancel" ,0,&rect , &rect,0, 0,  0, 0 , 0, 0);
        }
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Ok", &StrOKWidth , &StrOKHeight);
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        if(OkColor== 0xffff)
        {
            rect.X =PERLEN;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2+1;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

            rect.X =PERLEN-2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(OkColor,&rect);

            rect.X =PERLEN-2+(StrWidth+2-StrOKWidth)/2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            DrawText(Twidth ,HeightEnd ,L"Ok",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
        }
        else
        {
            rect.X =PERLEN+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2+1+2;
            rect.Height = StrHeight+1;
            rect.Width = StrWidth+1;
            i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

            rect.X =PERLEN-2+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(OkColor,&rect);

            rect.X =PERLEN-2+(StrWidth+2-StrOKWidth)/2+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            DrawText(Twidth ,HeightEnd ,L"Ok",rect.X ,rect.Y ,0);
        }

        if(CloseColor == 0xffff)
        {
            rect.X =Twidth - PERLEN -StrWidth;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

            rect.X =Twidth - PERLEN -StrWidth-2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(CloseColor,&rect);

            rect.X =Twidth - PERLEN -StrWidth-1;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
        }
        else
        {
            rect.X =Twidth - PERLEN -StrWidth+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2+2;
            rect.Height = StrHeight+1;
            rect.Width = StrWidth+1;
            i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

            rect.X =Twidth - PERLEN -StrWidth-2+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            i51KitG2CleanScreen(CloseColor,&rect);

            rect.X =Twidth - PERLEN -StrWidth-1+2;
            rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
            rect.Height = StrHeight+2;
            rect.Width = StrWidth+2;
            DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,0);
        }
    }

}


static void i51PhoneTestDrawCopyrights () 
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 1;
    iRECT rect = {0};
    iU16 StrWidth = 0,StrHeight = 0;
    iU16 title[20] = {0};

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;

    HeightHead = Theight/10;
    HeightEnd = Theight/12;

    HeightPerOne = (Theight - HeightEnd - HeightHead)/8;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (title, &StrWidth , &StrHeight);
    rect.X = Twidth/2-StrWidth/2;
    rect.Y =HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;
    switch(PlatId)
    {
    case 0:
        {
            i51AdeMmiGetTextAttr (L"MTK DEBUGER", &StrWidth , &StrHeight);
            rect.X = Twidth/2-StrWidth/2;
            rect.Y =HeightHead/2-StrHeight/2;
            rect.Height = HeightHead;
            rect.Width = Twidth;
            DrawText(Twidth ,HeightHead ,L"MTK DEBUGER",rect.X ,rect.Y ,0xffff);
        }
        break;
    case 1:
        {
            i51AdeMmiGetTextAttr (L"MSTAR DEBUGER", &StrWidth , &StrHeight);
            rect.X = Twidth/2-StrWidth/2;
            rect.Y =HeightHead/2-StrHeight/2;
            rect.Height = HeightHead;
            rect.Width = Twidth;
            DrawText(Twidth ,HeightHead ,L"MSTAR DEBUGER",rect.X ,rect.Y ,0xffff);
        }
        break;
    case 2:
        {
            i51AdeMmiGetTextAttr (L"SPRD DEBUGER", &StrWidth , &StrHeight);
            rect.X = Twidth/2-StrWidth/2;
            rect.Y =HeightHead/2-StrHeight/2;
            rect.Height = HeightHead;
            rect.Width = Twidth;
            DrawText(Twidth ,HeightHead ,L"SPRD DEBUGER",rect.X ,rect.Y ,0xffff);
        }

        break;
    case 3:
        {
            i51AdeMmiGetTextAttr (L"COOLSAND DEBUGER", &StrWidth , &StrHeight);
            rect.X = Twidth/2-StrWidth/2;
            rect.Y =HeightHead/2-StrHeight/2;
            rect.Height = HeightHead;
            rect.Width = Twidth;
            DrawText(Twidth ,HeightHead ,L"COOLSAND DEBUGER",rect.X ,rect.Y ,0xffff);
        }
        break;
    }
    DrawText(Twidth ,HeightHead ,title,rect.X ,rect.Y ,0xffff);

    i51AdeMmiGetTextAttr (L"[CHECKING]", &StrWidth , &StrHeight);

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = 14;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);//

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2+2;
    rect.Height = 9;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);//

    rect.X = 6;
    rect.Y = i*HeightPerOne+HeightPerOne/2-11;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrawText(Twidth ,HeightPerOne ,L"1.Adapter",rect.X ,rect.Y ,InterWordColor[0]);

    rect.X = Twidth - StrWidth-5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrowStatus(rect,1);
    i++;


    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = 14;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2+2;
    rect.Height = 9;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);

    rect.X = 6;
    rect.Y = i*HeightPerOne+HeightPerOne/2-11;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrawText(Twidth ,HeightPerOne ,L"2.Kernel",rect.X ,rect.Y ,InterWordColor[1]);

    rect.X =  Twidth - StrWidth-5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrowStatus(rect,2);
    i++;

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = 14;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2+2;
    rect.Height = 9;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);

    rect.X = 6;
    rect.Y = i*HeightPerOne+HeightPerOne/2-11;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrawText(Twidth ,HeightPerOne ,L"3.Benchmark",rect.X ,rect.Y ,InterWordColor[2]);

    rect.X =  Twidth - StrWidth-5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrowStatus(rect,3);
    i++;

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = 14;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor0[3],&rect);

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2+2;
    rect.Height = 9;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor1[3],&rect);

    rect.X = 6;
    rect.Y = i*HeightPerOne+HeightPerOne/2-11;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrawText(Twidth ,HeightPerOne ,L"4.MMI",rect.X ,rect.Y ,InterWordColor[3]);

    rect.X =  Twidth - StrWidth-5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrowStatus(rect,5);
    i++;

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = 14;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor0[4],&rect);

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2+2;
    rect.Height = 9;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor1[4],&rect);

    rect.X = 6;
    rect.Y = i*HeightPerOne+HeightPerOne/2-11;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrawText(Twidth ,HeightPerOne ,L"5.System",rect.X ,rect.Y ,InterWordColor[4]);

    rect.X = Twidth - StrWidth-5;
    rect.Y =i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;
    DrowStatus(rect,6);
    i++;

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2-12;
    rect.Height = 14;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor0[5],&rect);

    rect.X = 5;
    rect.Y = i*HeightPerOne+HeightPerOne/2+2;
    rect.Height = 9;
    rect.Width = Twidth-10;
    i51KitG2CleanScreen(InterColor1[5],&rect);

    rect.X = 6;
    rect.Y = i*HeightPerOne+HeightPerOne/2-11;
    rect.Height = HeightPerOne;
    rect.Width = Twidth;

    if(CommitReport)
    {
        DrawReport();
    }
    DrawTowButton();
    i51AdeMmiUpdateScreen();
}


void SetFous(iU8 fous)
{
    iU8 i = 0;
    InterFoucs = fous;
    for(i = 0;i<9;i++)
    {
        InterColor0[i]=i51KitG2COLOR(7, 24, 40);
        InterColor1[i]=i51KitG2COLOR(7, 24, 40);
        InterWordColor[i] = i51KitG2COLOR(255, 255, 255);
    }
    InterColor0[fous-1]=i51KitG2COLOR(231, 236, 242);
    InterColor1[fous-1]=i51KitG2COLOR(198,205,210);
    InterWordColor[fous-1] = i51KitG2COLOR(0, 0, 0);
}
void i51KeyEvent1 ()
{
    AllStatics[0] = 1;
    i51SetAllStatics();

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    Runstatus = 1;
    SetFous(1);
    YPoint = 0;
    DrowAdapter();
    if(i51AdeOsFileIsDirExist ( L"CreateDirTest" ))
    {
        i51AdeOsFileDeleteDir ( L"CreateDirTest" ) ;
    }
    i51AdeOsFileDelete ( L"CreateFileTest" ) ;
    if(i51AdeOsFileIsDirExist ( L"Dir1" ))
    {
        i51AdeOsFileDeleteDir ( L"Dir1" ) ;
    }
    if(i51AdeOsFileIsDirExist ( L"Dir111" ))
    {
        i51AdeOsFileDeleteDir ( L"Dir111" ) ;
    }

    i51AdeTesterRun () ;
    DrowCheckResult();
}

void i51KeyEvent2 ( )
{
    AllStatics[1] = 1;
    i51SetAllStatics();
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    Runstatus = 2;
    SetFous(2);
    YPoint = 0;
    DrowKernel();
    YPoint = 0;
    i51KernelTestRun();
    DrowCheckResult();
}

void i51KeyEvent3 ( )
{
    AllStatics[2] = 1;
    i51SetAllStatics();
    Runstatus = 3;
    SetFous(3);
}

void i51KeyEvent4( )
{
    //ResetInter();
    AllStatics[4] = 1;
    i51SetAllStatics();
    Runstatus = 4;
    SetFous(4);
    InterSetFous(1);
}

void i51KeyEvent5()
{
    AllStatics[5] = 1; 
    i51SetAllStatics();
    Runstatus = 5;
    SetFous(5);
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    Drowi51Loger();
    i51GetLoger();
    AllStatics[5] = 2; 
    i51SetAllStatics();
    YPoint = 0;
}

static void i51LeftKeyEvent()
{
    switch(Runstatus)
    {
    case 0:
        {
            if(CommitReport==0)
            {
                switch(InterFoucs)
                {
                case 1:
                    i51KeyEvent1 ();
                    break;
                case 2:
                    i51KeyEvent2 ();
                    break;
                case 3:
                    i51KeyEvent3 ();
                    break;
                case 4:
                    i51KeyEvent4 ();
                    break;
                case 5:
                    i51KeyEvent5 ();
                    break;
                }
            }
            else
            {
                if(SendButton)
                    DoReportOk();
            }
        }
        break;
    case 4:
        {
            InterKeyDo(InterFoucs-1);
        }
        break;
    case 9:
        {
            i51InputCheckDo(InterFoucs-1);
        }
        break;
    case 10:
        {
            SMSDoOk();
        }
        break;
    case 11:
        {
            DoSocket();
        }
        break;
    case 12:
        {
            i51MMIDO(InterFoucs-1);
        }
        break;
    case 13:
        {
            i51CallDo(InterFoucs-1);
        }
        break;
    case 14:
        {
            i51ShockDo(InterFoucs-1);
        }
        break;
    case 15:
        {
            SetHook();
        }
        break;
    case 16:
        {
            i51SetTimeDo();
        }
        break;
    case 18:
        {
            i51MmiSubPlayerDO(InterFoucs-1);
        }
        break;
    case 19:
        {
            i51MmiSubRecorderDO(InterFoucs-1);
        }
        break;
    }

}
void i51RightKeyEvent()
{
    if(Runstatus>=9&&Runstatus<=17)
    {
        InterSetFous(Runstatus-8);
        Runstatus = 4;
        return;
    }
    if(Runstatus>=18&&Runstatus<=19)
    {
        Runstatus = 12;
        return;
    }
    switch(Runstatus)
    {
    case 0:
        {
            if(CommitReport==0)
            {
                i51KernelSendRequest ( SimplePortalId , NULL , i51_REQ_EXIT , 0 , 0 ) ;
            }
            else
            {
                CloseReport();
            }
        }
        break;
    case 1:
        {
            Runstatus = 0;
        }
        break;
    case 2:
        {
            Runstatus = 0;
        }
        break;
    case 3:
        {
            Runstatus = 0;
        }
        break;
    case 4:
        {
            InterKeyBack( );
        }
        break;
    case 5:
        {
            Runstatus = 0;
        }
        break;
    }
}
static void i51SimpleInterMove( iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU8 i;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightPerHead = 0;
    iU16 HeightPerEnd = 0;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightPerEnd = (height)/12;
    HeightPerHead = (height)/10;
    HeightPerOne = (height-HeightPerHead-HeightPerEnd)/9;

    if( y >=(height-HeightPerEnd)&&y < height)
    {
        if(x>(width/2)&&x<width)
        {
            CloseColor = i51KitG2COLOR(150,150,150);
        }
        else if(x>0&&x<width/2)
        {
            OkColor = i51KitG2COLOR(150,150,150);
        }
        return;
    }
    CloseColor = 0xffff;
    OkColor = 0xffff;
    for(i = 0;i<9;i++)
    {
        if ( x > 0  && x <width &&y >=(i*HeightPerOne+HeightPerHead)&&y < ((i+1)*HeightPerOne+HeightPerHead))
        {
            switch(i)
            {
            case 0:
                {
                    if(Runstatus==12)
                    {
                        if(InterWordColor[0]==0xffff||InterWordColor[0]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(1);
                        }
                    }
                    else if(Runstatus==14)
                    {
                        //if(InterWordColor[0]==0xffff||InterWordColor[0]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(1);
                        }
                    }
                }
                break;
            case 1:
                {
                    if(Runstatus==12)
                    {
                        if(InterWordColor[1]==0xffff||InterWordColor[1]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(2);
                        }
                    }
                    else if(Runstatus==14)
                    {
                        if(InterWordColor[1]==0xffff||InterWordColor[1]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(2);
                        }
                    }
                }
                break;
            case 2:
                {

                    if(Runstatus==12)
                    {
                        if(InterWordColor[2]==0xffff||InterWordColor[2]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(3);
                        }
                    }
                    else if(Runstatus==14)
                    {
                        if(InterWordColor[2]==0xffff||InterWordColor[2]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(3);
                        }
                    }
                }
                break;
            case 3:
                {

                    if(Runstatus==12)
                    {
                        if(InterWordColor[3]==0xffff||InterWordColor[3]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(4);
                        }
                    }
                    else if(Runstatus==14)
                    {
                        if(InterWordColor[3]==0xffff||InterWordColor[3]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(4);
                        }
                    }
                }
                break;
            case 4:
                {

                    if(Runstatus==12)
                    {
                        if(InterWordColor[4]==0xffff||InterWordColor[4]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(5);
                        }
                    }
                }
                break;
            case 5:
                {

                    if(Runstatus==12)
                    {
                        if(InterWordColor[5]==0xffff||InterWordColor[5]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(6);
                        }
                    }
                }
                break;
            case 6:
                {
                    if(Runstatus==12)
                    {
                        if(InterWordColor[6]==0xffff||InterWordColor[6]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(7);
                        }
                    }
                }
                break;
            case 7:
                {
                    if(Runstatus==12)
                    {
                        if(InterWordColor[7]==0xffff||InterWordColor[7]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(8);
                        }
                    }
                }
                break;
            case 8:
                {
                    if(Runstatus==12)
                    {
                        if(InterWordColor[8]==0xffff||InterWordColor[8]==i51KitG2COLOR(0, 0, 0))
                        {
                            SetMoveFous(9);
                        }
                    }
                }
                break;
            default:break;
            }
            break;
        }             
    }
}
static void i51SimplePortalMove( iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU8 i;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightPerHead = 0;
    iU16 HeightPerEnd = 0;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightPerEnd = (height)/12;
    HeightPerHead = (height)/10;
    HeightPerOne = (height-HeightPerHead-HeightPerEnd)/8;

    if(CommitReport)
    {
        if(y>(height-HeightPerEnd -40)&&y<(height-HeightPerEnd -8))
        {
            if(x>22&&x<(width - 40)/2)
            {
                ReportColor1 = i51KitG2COLOR(150,150,150);
            }
            else if(x>(width - 40)/2&&x<(width - 40))
            {
                ReportColor2 = i51KitG2COLOR(150,150,150);
            }
        }
        else
        {
            ReportColor1 = 0xffff;
            ReportColor2 = 0xffff;
        }
        return;
    }

    if( y >=(height-HeightPerEnd)&&y < height)
    {
        if(x>(width/2)&&x<width)
        {
            CloseColor = i51KitG2COLOR(150,150,150);
        }
        else if(x>0&&x<width/2&&(Runstatus==0||Runstatus==4||(Runstatus>=9&&Runstatus<=16)))
        {
            OkColor = i51KitG2COLOR(150,150,150);
        }
        return;
    }
    CloseColor = 0xffff;
    OkColor = 0xffff;

    for(i = 0;i<9;i++)
    {
        if ( x > 0  && x <width &&y >=(i*HeightPerOne+HeightPerHead)&&y < ((i+1)*HeightPerOne+HeightPerHead))
        {
            switch(i)
            {
            case 0:
                {
                    if(Runstatus==0)
                    {
                        SetFous(1);
                    }
                    else if(Runstatus==4)
                    {
                        InterSetFous(1);
                    }
                    else if(Runstatus==9)
                    {
                        InterSetFous(1);
                    }
                }
                break;
            case 1:
                {
                    if(Runstatus==0)
                    {
                        SetFous(2);
                    }
                    else if(Runstatus==4)
                    {
                        InterSetFous(2);
                    }
                    else if(Runstatus==9)
                    {
                        InterSetFous(2);
                    }

                }
                break;
            case 2:
                {
                    if(Runstatus==0)
                    {
                        SetFous(3);
                    }
                    else if(Runstatus==4)
                    {
                        InterSetFous(3);
                    }
                    else if(Runstatus==9)
                    {
                        InterSetFous(3);
                    }

                }
                break;
            case 3:
                {
                    if(Runstatus==0)
                    {
                        SetFous(4);
                    }
                    else if(Runstatus==4)
                    {
                        InterSetFous(4);
                    }

                }
                break;
            case 4:
                {
                    if(Runstatus==0)
                    {
                        SetFous(5);
                    }
                    else if(Runstatus==4)
                    {
                        InterSetFous(5);
                    }
                }
                break;
            case 5:
                {
                    if(Runstatus==4)
                    {
                        InterSetFous(6);
                    }

                }
                break;
            case 6:
                {
                    if(Runstatus==4)
                    {
                        InterSetFous(7);
                    }

                }
                break;
            case 7:
                {
                    if(Runstatus==4)
                    {
                        InterSetFous(8);
                    }

                }
                break;
            case 8:
                {
                }
                break;
            default:break;
            }
            break;
        }             
    }
}

static void ChangeDatatoHost()
{
    iFILE file = 0,file1;
    TREPORT report = {0};
    iS32 filesize = 0;
    iCHAR *buffP = NULL;
    iCHAR *point = NULL;
    iU16 dat[10] = {0};

    file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\TestReport.tr" , i51_OS_FL_EXISTING_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
    if(file==0)
        return;

    i51AdeOsFileGetSize(file,&filesize);
    buffP = (iCHAR *)i51AdeOsMalloc(filesize, __FILE__ , __LINE__);
    if(buffP==NULL)
        return;

    memset(buffP,0,filesize);
    point = buffP;
    i51KitTReportReadHeader ( file , &report );
    i51AdeOsLog(0,"send case timese:%d",report.TotallCases);

    memcpy(point,&report.Magic[0],sizeof(iCHAR));
    point = point + sizeof(iCHAR);
    memcpy(point,&report.Magic[1],sizeof(iCHAR));
    point = point + sizeof(iCHAR);

    memcpy(point,&report.Version,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.TotallCases,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.CaseEntry,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(point,&report.CreateTime.Year,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.CreateTime.Month,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.CreateTime.Day,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.CreateTime.Hour,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.CreateTime.Minute,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.CreateTime.Second,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(point,&report.PhoneInfo.Endian,sizeof(iS8));
    point = point + sizeof(iS8);
    memcpy(point,&report.PhoneInfo.KernelVersion,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.PhoneInfo.AdapterVersion,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.PhoneInfo.Projectid,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.PhoneInfo.CustomerId,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(point,&report.PhoneInfo.Platform,64);
    point = point + 64;
    memcpy(point,&report.PhoneInfo.Resolution,10);
    point = point + 10;
    memcpy(point,&report.PhoneInfo.Sn,16);
    point = point + 16;
    memcpy(point,&report.PhoneInfo.IMEI,32);
    point = point + 32;
    memcpy(point,&report.PhoneInfo.BatchCode,16);
    point = point + 16;

    memcpy(point,&report.PhoneInfo.DiskFreeSpace,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.PhoneInfo.MemoryAvalible,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(point,&report.Sim.IMSI,32);
    point = point + 32;
    memcpy(point,&report.Sim.IMSC,32);
    point = point + 32;
    memcpy(point,&report.Sim.bid,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.Sim.zid,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(point,&report.KitServerInfo.sendsmstotall,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.KitServerInfo.succsmstotall,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.KitServerInfo.hookedtotall,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.KitServerInfo.Intercepted,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.KitServerInfo.Interceptedsend,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(point,&report.Benchmark.Integer,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.Float,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.FSO,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.FsSeek,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.FsWrite,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.FsRead,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.Memory,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.Graphics,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.Image,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.Font,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.Sound,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.UPS,sizeof(iU16));
    point = point + sizeof(iU16);
    memcpy(point,&report.Benchmark.TotallCost,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(point,&report.DotpayInfo.DotpayVersion,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.DotpayInfo.InitTime1,sizeof(iU32));
    point = point + sizeof(iU32);
    memcpy(point,&report.DotpayInfo.InitTime2,sizeof(iU32));
    point = point + sizeof(iU32);

    if(filesize>sizeof(TREPORT))
    {
        i51AdeOsFileRead ( file , point ,(filesize - sizeof(TREPORT)) , 0 ) ;
    }
    i51AdeOsFileClose (file) ;


    file1 = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\Testt.tr" , i51_OS_FL_ALWAYS_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
    if(file1==0)
    {
        i51AdeOsFree(buffP);
        return;
    }
    i51AdeOsFileWrite ( file1 , buffP , filesize , 0 ) ;
    i51AdeOsFileClose (file1) ;
    i51AdeOsFree(buffP);

}

static void ChangeDatatoHostB()
{
    iFILE file = 0,file1;
    TREPORT report = {0};
    iS32 filesize = 0;
    iCHAR *buffP = NULL;
    iCHAR *point = NULL;
    iU16 dat[10] = {0};
    iCHAR MIDBUFF[4] = {0};
    iCHAR midbuf[2] = {0};
    iCHAR da;
    iCHAR endat[40] = {0};
    iCHAR i = 0;

    file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\TestReport.tr" , i51_OS_FL_EXISTING_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
    if(file==0)
        return;

    i51AdeOsFileGetSize(file,&filesize);
    buffP = (iCHAR *)i51AdeOsMalloc(filesize, __FILE__ , __LINE__);
    if(buffP==NULL)
        return;

    memset(buffP,0,filesize);
    point = buffP;
    i51KitTReportReadHeader ( file , &report );
    i51AdeOsLog(0,"send case timese:%d",report.TotallCases);

    memcpy(point,&report.Magic[0],sizeof(iCHAR));
    point = point + sizeof(iCHAR);

    memcpy(point,&report.Magic[1],sizeof(iCHAR));
    point = point + sizeof(iCHAR);

    memcpy(MIDBUFF,&report.Version,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.TotallCases,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.CaseEntry,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(midbuf,&report.CreateTime.Year,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.CreateTime.Month,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.CreateTime.Day,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.CreateTime.Hour,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.CreateTime.Minute,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.CreateTime.Second,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(point,&report.PhoneInfo.Endian,sizeof(iS8));
    point = point + sizeof(iS8);

    memcpy(MIDBUFF,&report.PhoneInfo.KernelVersion,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.PhoneInfo.AdapterVersion,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.PhoneInfo.Projectid,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.PhoneInfo.CustomerId,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(point,&report.PhoneInfo.Platform,64);
    point = point + 64;
    memcpy(point,&report.PhoneInfo.Resolution,10);
    point = point + 10;
    memcpy(point,&report.PhoneInfo.Sn,16);
    point = point + 16;
    memcpy(point,&report.PhoneInfo.IMEI,32);
    point = point + 32;
    memcpy(point,&report.PhoneInfo.BatchCode,16);
    point = point + 16;

    memcpy(MIDBUFF,&report.PhoneInfo.DiskFreeSpace,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.PhoneInfo.MemoryAvalible,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(point,&report.Sim.IMSI,32);
    point = point + 32;
    memcpy(point,&report.Sim.IMSC,32);
    point = point + 32;

    memcpy(MIDBUFF,&report.Sim.bid,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.Sim.zid,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.KitServerInfo.sendsmstotall,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.KitServerInfo.succsmstotall,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.KitServerInfo.hookedtotall,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.KitServerInfo.Intercepted,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.KitServerInfo.Interceptedsend,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(midbuf,&report.Benchmark.Integer,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.Float,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.FSO,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.FsSeek,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.FsWrite,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.FsRead,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.Memory,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.Graphics,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.Image,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.Font,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.Sound,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.UPS,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(midbuf,&report.Benchmark.TotallCost,sizeof(iU16));
    da = midbuf[0];
    midbuf[0] = midbuf[1];
    midbuf[1] = da;
    memcpy(point,midbuf,sizeof(iU16));
    point = point + sizeof(iU16);

    memcpy(MIDBUFF,&report.DotpayInfo.DotpayVersion,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.DotpayInfo.InitTime1,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    memcpy(MIDBUFF,&report.DotpayInfo.InitTime2,sizeof(iU32));
    da = MIDBUFF[0];
    MIDBUFF[0] = MIDBUFF[3];
    MIDBUFF[3] = da;

    da = MIDBUFF[1];
    MIDBUFF[1] = MIDBUFF[2];
    MIDBUFF[2] = da;
    memcpy(point,MIDBUFF,sizeof(iU32));
    point = point + sizeof(iU32);

    if(filesize>sizeof(TREPORT))
    {
        //i51AdeOsFileRead ( file , point ,(filesize - sizeof(TREPORT)) , 0 ) ;
        i51AdeOsFileRead ( file , endat ,20 , 0 ) ;
        while(i<20)
        {
            da = endat[i];
            endat[i] = endat[i+1];
            endat[i+1] = da;
            i = i+2;
        }
        memcpy(point,endat,20);
    }
    i51AdeOsFileClose (file) ;


    file1 = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\Testt.tr" , i51_OS_FL_ALWAYS_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
    if(file1==0)
    {
        i51AdeOsFree(buffP);
        return;
    }
    i51AdeOsFileWrite ( file1 , buffP , filesize , 0 ) ;
    i51AdeOsFileClose (file1) ;
    i51AdeOsFree(buffP);

}

static void i51SimplePortalTouchDo ( iU16 event , iU32 value )
{

    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightPerHead = 0;
    iU16 HeightPerEnd = 0;
    iU8 i = 1;
    static iBOOL IsDown[12] = {0};

    TREPORT report = {0};
    iS32 filesize = 0;
    iU32 len = 0;
    iU16 bb[10] = {0};

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightPerEnd = (height)/12;
    HeightPerHead = (height)/10;
    HeightPerOne = (height-HeightPerHead-HeightPerEnd)/8;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(CommitReport&&Runstatus==0)
    {
        if(y>(height-HeightPerEnd -40)&&y<(height-HeightPerEnd -8))
        {
            if(x>22&&x<(width - 40)/2)
            {
                if(i51_MSG_PADDOWN==event)
                {
                    ReportColor1 = i51KitG2COLOR(150,150,150);
                    IsDown[10] = iTRUE;
                }
                else if(i51_MSG_PADUP==event)
                {
                    if(IsDown[10]&&SendButton)
                    {
                        IsDown[10] = iFALSE;
                        DoReportOk();
                    }
                    ReportColor1 = 0xffff;
                }
            }
            else if(x>(width - 40)/2&&x<(width - 40))
            {
                if(i51_MSG_PADDOWN==event)
                {
                    ReportColor2 = i51KitG2COLOR(150,150,150);
                    IsDown[11] = iTRUE;
                }
                else if(i51_MSG_PADUP==event)
                {
                    if(IsDown[11])
                    {
                        IsDown[11] = iFALSE;
                        CloseReport();
                    }
                    ReportColor2 = 0xffff;
                }
            }
        }

        return;
    }

    if(Runstatus==1||Runstatus==2||Runstatus==5)
    {
        if(y>=(height-HeightPerEnd)&&y < height&&x>(width/2)&&x<width)
        {
            if(i51_MSG_PADDOWN==event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown[9] = iTRUE;
            }
            else if(i51_MSG_PADUP==event)
            {
                CloseColor = 0xffff;
                if(IsDown[9])
                {
                    IsDown[9] = iFALSE;
                    Runstatus = 0;
                }
            }
        }
        return;
    }

    if(y>=(height-HeightPerEnd)&&y < height&&x>0&&x<width/2)
    {
        if(i51_MSG_PADDOWN==event)
        {
            OkColor = i51KitG2COLOR(150,150,150);
            IsDown[7] = iTRUE;
        }  
        else if(i51_MSG_PADUP==event)
        {
            OkColor = 0xffff;
            if(IsDown[7])
            {
                IsDown[7] = iFALSE;
                i51LeftKeyEvent();
            }
        }
        return;
    }
    else if( y >=(height-HeightPerEnd)&&y < height&&x>(width/2)&&x<width)
    {
        if(i51_MSG_PADDOWN==event)
        {
            CloseColor = i51KitG2COLOR(150,150,150);
            IsDown[8] = iTRUE;
        }  
        else if(i51_MSG_PADUP==event)
        {
            CloseColor = 0xffff;
            if(IsDown[8])
            {
                IsDown[8] = iFALSE;
                memset(IsDown,0,sizeof(IsDown));
                i51RightKeyEvent();
            }
        }
        return;
    }

    for(i = 0;i<6;i++)
    {
        if ( x > 0  && x <width &&y >=(i*HeightPerOne+HeightPerHead)&&y < ((i+1)*HeightPerOne+HeightPerHead))
        {
            if(i51_MSG_PADDOWN==event)
            {
                IsDown[i] = iTRUE;
            }      
            else if (i51_MSG_PADUP == event&&IsDown[i]==iTRUE)
            {
                IsDown[i] = iFALSE;
                memset(IsDown,0,sizeof(IsDown));
                switch(i)
                {
                case 0:
                    {
                        i51KeyEvent1 () ;
                    }
                    break;
                case 1:
                    {
                        i51KeyEvent2 () ;
                    }
                    break;
                case 2:
                    {
                        i51KeyEvent3 ( ) ;
                    }
                    break;
                case 3:
                    {
                        i51KeyEvent4 () ;
                    }
                    break;
                case 4:
                    {
                        i51KeyEvent5 ( ) ;
                    }
                    break;
                case 5:
                    {                                                  
#if 0
                        ///i51KitTReportReadHeader ( file , &report );
                        file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\Testt.tr" , i51_OS_FL_EXISTING_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
                        i51AdeOsFileGetSize(file,&filesize);
                        i51KitTReportReadHeader ( file , &report );
                        i51AdeOsFileRead ( file , bb , 20 , 0 ) ;
                        i51AdeOsFileClose (file) ;
#endif

                    }
                    break;
                default:break;
                }
                break;
            }             
        }
    }    
}
void i51SetAllStatics()
{
//    int i = 0;
    iFILE file = 0;
    iU32 ReadLen = 0;
    file = i51AdeOsFileOpen (L"i51Debuger\\i51PhoneTestFile\\Status.dat" , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN , __FILE__ , __LINE__ ) ;
    if(file!=0)
    {
        i51AdeOsFileWrite(file , (void *)AllStatics ,21, &ReadLen );
        i51AdeOsFileClose(file);
    }
//   for (i = 0; i < 21; ++i)
//   {
//      i51AdeOsLog(0,"-----------> Set %d : %x", i, AllStatics[i]);
//   }
}
void i51GetAllStatus()
{
//    int i = 0;
    iFILE file = 0;
    iU32 ReadLen = 0;
    memset(AllStatics,0,sizeof(AllStatics));
    file = i51AdeOsFileOpen (L"i51Debuger\\i51PhoneTestFile\\Status.dat" , i51_OS_FL_READ | i51_OS_FL_ALWAYS_OPEN , __FILE__ , __LINE__ ) ;
    if(file!=0)
    {
        i51AdeOsFileRead(file ,(void *)AllStatics ,21,&ReadLen);
        i51AdeOsFileClose(file);
    }
//   for (i = 0; i < 21; ++i)
//   {
//      i51AdeOsLog(0,"-----------> Get %d : %x", i, AllStatics[i]);
//   }
    UpdataInterStatus();
}

static void i51GetLoger()
{
    iU8* sn = 0;
    iU8 pp[8] = {0};
    iU8 snlen = 0 ;
    iS32 looper = 0 ;
    iS32* sims = 0 ;
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 ResolutionWidth = 0;
    iU16 ResolutionHeight = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU16 OneHeight = 0;
    iRECT rect = {0};
    iU8 j = 0;
    static iCHAR buffer [128] = { 0 } ;
    iCHAR *vision = NULL;
    static iCHAR VisionBuff[128] ={0};
    static iCHAR KernelBuff[128] = {0};
    iU32 COdeLen = 0;
    iU32 bsid = 0 ,zid =  0;
    iU32 Bytes = 0;
    static TREPORT report = {0};
    iCHAR* Platform = NULL;
    iU32 FontId = 0;

    memset(&report,0,sizeof(TREPORT));
    memset(VisionBuff,0,sizeof(VisionBuff));
    memset(KernelBuff,0,sizeof(KernelBuff));
    memset(buffer,0,sizeof(buffer));
    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightEnd = Theight/12;
    HeightHead = Theight/10;

    if(Twidth==240&&Theight==320)
    {
        OneHeight = 12;
        FontId = 0;
    }
    else
    {
        OneHeight = 16;
        FontId = 2;
    }


    sprintf ( (iCHAR *)buffer , ":%s" , i51KernelGeti51Version () ) ;
    //i51AdeOsLog(0,"i51KernelGeti51Version:%s",buffer);
    vision = strstr(buffer," ");
    memcpy(KernelBuff,&buffer[1],strlen(buffer)-1-strlen(vision));
    vision++;
    memcpy(VisionBuff,vision,strlen(vision));

    memset(buffer,0,sizeof(buffer));
    report.PhoneInfo.KernelVersion = i51KernelGetVersion ();
    sprintf ( (iCHAR *)buffer , "Kernel Version:%s,0x%d" ,KernelBuff,i51KernelGetVersion () ) ;
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;


    memset(buffer,0,sizeof(buffer));
    report.PhoneInfo.AdapterVersion = i51AdeCnfGetVersion ();
    sprintf ( (iCHAR *)buffer , "Adapter Version:%s,0x%d" , VisionBuff,i51AdeCnfGetVersion () ) ;
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    report.PhoneInfo.Projectid = i51AdeCnfGetProjectId ();
    sprintf ( (iCHAR *)buffer , "Project ID:%d" , i51AdeCnfGetProjectId () ) ;
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    report.PhoneInfo.CustomerId = i51AdeCnfGetCustomerId ();
    sprintf ( (iCHAR *)buffer , "Customer ID:%d" , i51AdeCnfGetCustomerId () ) ;
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    Platform = i51AdeCnfGetPlatformId ();
    memcpy(report.PhoneInfo.Platform,Platform,strlen(Platform));
    sprintf ( (iCHAR *)buffer , "Platform:%s" , Platform) ;
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    i51AdeMmiGetScreenScale (&ResolutionWidth, &ResolutionHeight);
    sprintf ( (iCHAR *)report.PhoneInfo.Resolution , "%dX%d" , ResolutionWidth, ResolutionHeight) ;
    sprintf ( (iCHAR *)buffer , "Resolution: %dX%d" , ResolutionWidth, ResolutionHeight) ;
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;  

    memset(buffer,0,sizeof(buffer));
    if(i51AdeCnfGetEndian () ==1)
    {
        sprintf ( (iCHAR *)buffer , "Endian:Big") ;
        report.PhoneInfo.Endian = 1;
    }
    else
    {
        sprintf ( (iCHAR *)buffer , "Endian:Little") ;
        report.PhoneInfo.Endian = 0;
    }
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    i51KernelGetSerialNumber ( (void*)&sn , (iU8*)&snlen ) ;
    if(snlen!=8)
    {
        sprintf ( (iCHAR *)buffer , "SN:NULL") ;
    }
    else
    {
        if(i51AdeCnfGetEndian () ==1)
        {
            sprintf ( (iCHAR *)buffer , "SN:0x%d %d %d %d %d %d %d %d",sn[0],sn[1],sn[2],sn[3],sn[4],sn[5],sn[6],sn[7]) ;
            memcpy(report.PhoneInfo.Sn,sn,8);
        }
        else
        {
            for(looper = 0;looper<4;looper++)
            {
                pp[looper] = sn[3-looper];
            }
            for(looper = 4;looper<8;looper++)
            {
                pp[looper] = sn[7-looper+4];
            }
            sprintf ( (iCHAR *)buffer , "SN:0x%d %d %d %d %d %d %d %d",pp[0],pp[1],pp[2],pp[3],pp[4],pp[5],pp[6],pp[7]) ;
            memcpy(report.PhoneInfo.Sn,pp,8);
        }
    }  
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    sims = i51AdeSmsGetSims () ;

    for ( looper = 0 ; looper < sims[0]; looper ++ ) 
    {
        memset(buffer,0,sizeof(buffer));
        sprintf ( (iCHAR *)buffer , "Imsi%d:%s" , looper+1 , i51AdeSmsGetImsi(sims[looper+1]) ) ;
        rect.X = 5;
        rect.Y = HeightHead+j*OneHeight;
        rect.Height = OneHeight;
        rect.Width = Twidth;
        i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
        j++;
    }

    for ( looper = 0 ; looper < sims[0]; looper ++ ) 
    {
        memset(buffer,0,sizeof(buffer));
        sprintf ( (iCHAR *)buffer , "Smsc%d:%s" , looper+1 , i51AdeSmsGetCenter (sims[looper+1]) ) ;
        rect.X = 5;
        rect.Y = HeightHead+j*OneHeight;
        rect.Height = OneHeight;
        rect.Width = Twidth;
        i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
        j++;

        memset(buffer,0,sizeof(buffer));
        i51AdeOsGetBaseStation(sims[looper+1], & bsid , &zid );
        sprintf ( (iCHAR *)buffer , "Sms%d Base Station: bsid:%d,zid:%d" , looper+1 ,bsid,zid) ;
        rect.X = 5;
        rect.Y = HeightHead+j*OneHeight;
        rect.Height = OneHeight;
        rect.Width = Twidth;
        i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
        j++;
    }

    Platform = i51AdeSmsGetImei ();
    memcpy(report.PhoneInfo.IMEI,Platform,strlen(Platform));
    sprintf ( (iCHAR *)buffer , "Imei:%s" , i51AdeSmsGetImei () ) ;
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    memset(VisionBuff,0,sizeof(VisionBuff));
    i51KernelGetBatchCode((iU8 *)VisionBuff , 128, &COdeLen);
    memcpy(report.PhoneInfo.BatchCode,VisionBuff,16);
    sprintf(buffer,"Batch Code:0x%x,0x%x,0x%x,0x%x ",VisionBuff[0],VisionBuff[1],VisionBuff[2],VisionBuff[3] );
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    sprintf(buffer,"           0x%x,0x%x,0x%x,0x%x",VisionBuff[4],VisionBuff[5],VisionBuff[6],VisionBuff[7]);
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;
    memset(buffer,0,sizeof(buffer));
    sprintf(buffer,"           0x%x,0x%x,0x%x,0x%x",VisionBuff[8],VisionBuff[9],VisionBuff[10],VisionBuff[11]);
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;
    memset(buffer,0,sizeof(buffer));
    sprintf(buffer,"           0x%x,0x%x,0x%x,0x%x",VisionBuff[12],VisionBuff[13],VisionBuff[14],VisionBuff[15]);
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    report.PhoneInfo.DiskFreeSpace = i51KernelGetDiskFreeSpace();
    sprintf(buffer,"Disk Free Space:%d(KB)",i51KernelGetDiskFreeSpace());
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);
    j++;

    memset(buffer,0,sizeof(buffer));
    Bytes = i51AdeOsGetTotallMemoryLeft();
    Bytes = Bytes/1024;
    report.PhoneInfo.MemoryAvalible = Bytes;
    sprintf(buffer,"Memory Avalible:%d(KB)",Bytes);
    rect.X = 5;
    rect.Y = HeightHead+j*OneHeight;
    rect.Height = OneHeight;
    rect.Width = Twidth;
    i51KitG2FontDraw (TypeheadHandle,FontId,buffer ,0,&rect , &rect,0, 0,  0,  0XFFFF , 0, 0);

    i51UpdataReport(0,report);
    i51AdeMmiUpdateScreen();
}

#if 1
static void Drowi51Loger()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    i51AdeMmiGetTextAttr (L"System", &StrWidth , &StrHeight);
    HeightEnd = Theight/12;
    HeightHead = Theight/10;

    i51PhoneDrawRect();
    rect.X = Twidth/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"System" ,rect.X ,rect.Y, 0xffff);

    rect.X =2;
    rect.Y = Theight - HeightEnd+2;
    rect.Height = HeightEnd-4;
    rect.Width = Twidth-4;
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),&rect);
    DrawCloseButton();

    i51AdeMmiUpdateScreen();
}
#else
static void Drowi51Loger()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    i51AdeMmiGetTextAttr (L"System", &StrWidth , &StrHeight);
    HeightEnd = Theight/12;
    HeightHead = Theight/10;

    i51PhoneDrawRect();
    rect.X = Twidth/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"System" ,rect.X ,rect.Y, 0xffff);

    rect.X =2;
    rect.Y = Theight - HeightEnd+2;
    rect.Height = HeightEnd-4;
    rect.Width = Twidth-4;
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),&rect);
    if(CloseColor == 0xffff)
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =Twidth - StrWidth-12;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =Twidth - StrWidth-12+1;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =Twidth - StrWidth-12+StrWidth+4;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =Twidth - StrWidth-10;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2;
        rect.Height = HeightEnd;
        rect.Width = Twidth/2;
        DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =Twidth - StrWidth-12+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =Twidth - StrWidth-12+1+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4+2;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =Twidth - StrWidth-12+StrWidth+4+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =Twidth - StrWidth-10+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
        rect.Height = HeightEnd;
        rect.Width = Twidth/2;
        DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,0);
    }

    i51AdeMmiUpdateScreen();
}
#endif

#if 0
static void DrowKernel()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    i51AdeMmiGetTextAttr (L"Test Adapter", &StrWidth , &StrHeight);
    HeightEnd = Theight/12;
    HeightHead = Theight/10;

    i51PhoneDrawRect();
    rect.X = Twidth/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"Test Kernel" ,rect.X ,rect.Y, 0xffff);

    rect.X =2;
    rect.Y = Theight - HeightEnd+2;
    rect.Height = HeightEnd-4;
    rect.Width = Twidth-4;
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),&rect);
    if(CloseColor == 0xffff)
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =Twidth - StrWidth-12;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =Twidth - StrWidth-12+1;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =Twidth - StrWidth-12+StrWidth+4;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =Twidth - StrWidth-10;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2;
        rect.Height = HeightEnd;
        rect.Width = Twidth/2;
        DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =Twidth - StrWidth-12+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =Twidth - StrWidth-12+1+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4+2;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =Twidth - StrWidth-12+StrWidth+4+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =Twidth - StrWidth-10+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
        rect.Height = HeightEnd;
        rect.Width = Twidth/2;
        DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,0);
    }

    i51AdeMmiUpdateScreen();
}

static void DrowAdapter()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightEnd = Theight/12;
    HeightHead = Theight/10;
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Test Adapter", &StrWidth , &StrHeight);
    rect.X = Twidth/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"Test Adapter" ,rect.X ,rect.Y, 0xffff);

    rect.X =2;
    rect.Y = Theight - HeightEnd+2;
    rect.Height = HeightEnd-4;
    rect.Width = Twidth-4;
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),&rect);
    if(CloseColor == 0xffff)
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =Twidth - StrWidth-12;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =Twidth - StrWidth-12+1;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =Twidth - StrWidth-12+StrWidth+4;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =Twidth - StrWidth-10;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2;
        rect.Height = HeightEnd;
        rect.Width = Twidth/2;
        DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =Twidth - StrWidth-12+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =Twidth - StrWidth-12+1+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4+2;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =Twidth - StrWidth-12+StrWidth+4+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =Twidth - StrWidth-10+2;
        rect.Y = (Theight-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
        rect.Height = HeightEnd;
        rect.Width = Twidth/2;
        DrawText(Twidth ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,0);
    }

    i51AdeMmiUpdateScreen();
}
#else
static void DrowKernel()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    i51AdeMmiGetTextAttr (L"Kernel", &StrWidth , &StrHeight);
    HeightEnd = Theight/12;
    HeightHead = Theight/10;

    i51PhoneDrawRect();
    rect.X = Twidth/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"Kernel" ,rect.X ,rect.Y, 0xffff);

    rect.X =2;
    rect.Y = Theight - HeightEnd+2;
    rect.Height = HeightEnd-4;
    rect.Width = Twidth-4;
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),&rect);
    DrawCloseButton();
    i51AdeMmiUpdateScreen();
}

static void DrowAdapter()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightEnd = Theight/12;
    HeightHead = Theight/10;
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Adapter", &StrWidth , &StrHeight);
    rect.X = Twidth/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"Adapter" ,rect.X ,rect.Y, 0xffff);

    rect.X =2;
    rect.Y = Theight - HeightEnd+2;
    rect.Height = HeightEnd-4;
    rect.Width = Twidth-4;
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),&rect);
    DrawCloseButton();
    i51AdeMmiUpdateScreen();
}
#endif

#if 1
void DrawG2Result()
{
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 nWidth = 0;
    iU16 nHeight = 0;
    iU8 i = 1;
    iU8 j = 0;
    iRECT rect = {0};
    iS32 Point[2][2] = {0};
    iU16 DrawWidth;
    iU16 WordWidth,WordHeight;
    iU16 HeightEnd = 0;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51AdeMmiGetScreenScale ( &nWidth , &nHeight );
    HeightEnd = nHeight/12;
    DrawWidth = nWidth/5;

    if(nWidth==240&&nHeight==320)
    {
        HeightPerOne = 16;
        HeightHead = 28;
    }
    else
    {
        HeightPerOne = 24;
        HeightHead = 32;
    }
    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);

    Point[0][0] = 1;
    Point[0][1] = 1;
    Point[1][0] = nWidth-1;
    Point[1][1] = 1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 1;
    Point[0][1] = 1;
    Point[1][0] = 1;
    Point[1][1] = nHeight-1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = nWidth-1;
    Point[0][1] = 1;
    Point[1][0] = nWidth-1;
    Point[1][1] = nHeight-1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 1;
    Point[0][1] = nHeight-1;
    Point[1][0] = nWidth-1;
    Point[1][1] = nHeight-1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 1;
    Point[0][1] =nHeight - nHeight/12;
    Point[1][0] = nWidth-1;
    Point[1][1] = nHeight - nHeight/12;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 1;
    Point[0][1] = HeightHead;
    Point[1][0] = nWidth-1;
    Point[1][1] = HeightHead;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = DrawWidth*2;
    Point[0][1] =HeightHead+HeightPerOne;
    Point[1][0] = nWidth - 1;
    Point[1][1] =HeightHead+HeightPerOne;
    i51KitG2DrawLine( Point, 0xffff);  
    for(i = 2;i<16;i++)
    {
        Point[0][0] = 1;
        Point[0][1] =HeightHead+HeightPerOne*i;
        Point[1][0] = nWidth - 1;
        Point[1][1] =HeightHead+HeightPerOne*i;
        i51KitG2DrawLine( Point, 0xffff);  
    }

    i--;
    Point[0][0] = DrawWidth*2;
    Point[0][1] = HeightHead;
    Point[1][0] = DrawWidth*2;
    Point[1][1] = HeightHead+HeightPerOne*i;
    i51KitG2DrawLine( Point, 0xffff);  

    Point[0][0] = DrawWidth*3;
    Point[0][1] = HeightHead+HeightPerOne;
    Point[1][0] = DrawWidth*3;
    Point[1][1] = HeightHead+HeightPerOne*i;
    i51KitG2DrawLine( Point, 0xffff);  

    Point[0][0] = DrawWidth*4;
    Point[0][1] = HeightHead+HeightPerOne;
    Point[1][0] = DrawWidth*4;
    Point[1][1] = HeightHead+HeightPerOne*i;
    i51KitG2DrawLine( Point, 0xffff);  

#if 1
    i51AdeMmiGetTextAttr(L"RESULTS",&WordWidth,&WordHeight);
    rect.X = nWidth/2-WordWidth/2;
    rect.Y = HeightHead/2-WordHeight/2;
    rect.Height = HeightHead;
    rect.Width = nWidth;
    DrawText(rect.Width , rect.Height ,L"RESULTS" ,rect.X ,rect.Y, 0xffff);

    i51AdeMmiGetTextAttr(L"Moudle",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne-WordHeight/2;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Moudle" ,rect.X ,rect.Y, 0xffff);

    i51AdeMmiGetTextAttr(L"Cost",&WordWidth,&WordHeight);
    rect.X = DrawWidth*2+DrawWidth*3/2-WordHeight/2;
    rect.Y = HeightHead+HeightPerOne/2-WordHeight/2+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*3;
    DrawText(rect.Width , rect.Height ,L"Cost" ,rect.X ,rect.Y, 0xffff);

    j= 1;
    i51AdeMmiGetTextAttr(L"Now",&WordWidth,&WordHeight);
    rect.X = DrawWidth*2+DrawWidth/2-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth;
    DrawText(rect.Width , rect.Height ,L"Now" ,rect.X ,rect.Y, 0xffff);

    i51AdeMmiGetTextAttr(L"Slow",&WordWidth,&WordHeight);
    rect.X = DrawWidth*3+DrawWidth/2-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth;;
    DrawText(rect.Width , rect.Height ,L"Slow" ,rect.X ,rect.Y, 0xffff);

    i51AdeMmiGetTextAttr(L"Fast",&WordWidth,&WordHeight);
    rect.X = DrawWidth*4+DrawWidth/2-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth;
    DrawText(rect.Width , rect.Height ,L"Fast" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Integer",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Integer" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Float",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Float" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"FSO",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"FSO" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"FS Seek",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"FS Seek" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"FS Write",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"FS Write" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"FS Read",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"FS Read" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Memory",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Memory" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Graphics",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Graphics" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Image",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Image" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Font",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Font" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Sound",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Sound" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"UPS",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"UPS" ,rect.X ,rect.Y, 0xffff);
    j++;

    rect.X =2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne-1;
    rect.Width = DrawWidth*2-2;
    i51KitG2CleanScreen(i51KitG2COLOR(128, 128, 128),&rect);
    rect.X =DrawWidth*2+2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne-1;
    rect.Width = DrawWidth-2;
    i51KitG2CleanScreen(i51KitG2COLOR(128, 128, 128),&rect);
    rect.X =DrawWidth*3+2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne-1;
    rect.Width = DrawWidth-2;
    i51KitG2CleanScreen(i51KitG2COLOR(128, 128, 128),&rect);
    rect.X =DrawWidth*4+2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne-1;
    rect.Width = DrawWidth-2;
    i51KitG2CleanScreen(i51KitG2COLOR(128, 128, 128),&rect);

    i51AdeMmiGetTextAttr(L"Total Cost",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightHead+HeightPerOne*j+1;
    rect.Height = HeightPerOne;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Total Cost" ,rect.X ,rect.Y, 0xffff);
    j++;
#endif

    DrawCloseButton();
}
#else
void DrawG2Result()
{
    iU16 HeightPerOne = 0;
    iU16 nWidth = 0;
    iU16 nHeight = 0;
    iU8 i = 1;
    iU8 j = 0;
    iRECT rect = {0};
    iS32 Point[2][2] = {0};
    iU16 DrawWidth;
    iU16 WordWidth,WordHeight;
    iU16 StrWidth = 0 ,StrHeight = 0;
    iU16 HeightEnd = 0;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51AdeMmiGetScreenScale ( &nWidth , &nHeight );
    HeightPerOne = 32;
    HeightEnd = nHeight/12;
    DrawWidth = nWidth/5;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    Point[0][0] = 1;
    Point[0][1] = 1;
    Point[1][0] = nWidth-1;
    Point[1][1] = 1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 1;
    Point[0][1] = 1;
    Point[1][0] = 1;
    Point[1][1] = nHeight-1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = nWidth-1;
    Point[0][1] = 1;
    Point[1][0] = nWidth-1;
    Point[1][1] = nHeight-1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 1;
    Point[0][1] = nHeight-1;
    Point[1][0] = nWidth-1;
    Point[1][1] = nHeight-1;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 1;
    Point[0][1] =nHeight - nHeight/12;
    Point[1][0] = nWidth-1;
    Point[1][1] = nHeight - nHeight/12;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = 1;
    Point[0][1] = 32;
    Point[1][0] = nWidth-1;
    Point[1][1] = 32;
    i51KitG2DrawLine( Point, 0xffff);

    Point[0][0] = DrawWidth*2;
    Point[0][1] =HeightPerOne+24;
    Point[1][0] = nWidth - 1;
    Point[1][1] =HeightPerOne+24;
    i51KitG2DrawLine( Point, 0xffff);  
    for(i = 2;i<16;i++)
    {
        Point[0][0] = 1;
        Point[0][1] =HeightPerOne+24*i;
        Point[1][0] = nWidth - 1;
        Point[1][1] =HeightPerOne+24*i;
        i51KitG2DrawLine( Point, 0xffff);  
    }

    i--;
    Point[0][0] = DrawWidth*2;
    Point[0][1] = HeightPerOne;
    Point[1][0] = DrawWidth*2;
    Point[1][1] = HeightPerOne+24*i;
    i51KitG2DrawLine( Point, 0xffff);  

    Point[0][0] = DrawWidth*3;
    Point[0][1] = HeightPerOne+24;
    Point[1][0] = DrawWidth*3;
    Point[1][1] = HeightPerOne+24*i;
    i51KitG2DrawLine( Point, 0xffff);  

    Point[0][0] = DrawWidth*4;
    Point[0][1] = HeightPerOne+24;
    Point[1][0] = DrawWidth*4;
    Point[1][1] = HeightPerOne+24*i;
    i51KitG2DrawLine( Point, 0xffff);  

#if 1
    i51AdeMmiGetTextAttr(L"RESULTS",&WordWidth,&WordHeight);
    rect.X = nWidth/2-WordWidth/2;
    rect.Y = 16-WordHeight/2;
    rect.Height = 32;
    rect.Width = nWidth;
    DrawText(rect.Width , rect.Height ,L"RESULTS" ,rect.X ,rect.Y, 0xffff);

    i51AdeMmiGetTextAttr(L"Moudle",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = 32+48/2-WordHeight/2;
    rect.Height = 48;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Moudle" ,rect.X ,rect.Y, 0xffff);

    i51AdeMmiGetTextAttr(L"Cost",&WordWidth,&WordHeight);
    rect.X = DrawWidth*2+DrawWidth*3/2-WordHeight/2;
    rect.Y = HeightPerOne+24/2-WordHeight/2;
    rect.Height = 24;
    rect.Width = DrawWidth*3;
    DrawText(rect.Width , rect.Height ,L"Cost" ,rect.X ,rect.Y, 0xffff);

    j= 1;
    i51AdeMmiGetTextAttr(L"Now",&WordWidth,&WordHeight);
    rect.X = DrawWidth*2+DrawWidth/2-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth;
    DrawText(rect.Width , rect.Height ,L"Now" ,rect.X ,rect.Y, 0xffff);

    i51AdeMmiGetTextAttr(L"Slow",&WordWidth,&WordHeight);
    rect.X = DrawWidth*3+DrawWidth/2-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth;;
    DrawText(rect.Width , rect.Height ,L"Slow" ,rect.X ,rect.Y, 0xffff);

    i51AdeMmiGetTextAttr(L"Faster",&WordWidth,&WordHeight);
    rect.X = DrawWidth*4+DrawWidth/2-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth;
    DrawText(rect.Width , rect.Height ,L"Faster" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Integer",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Integer" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Float",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Float" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"FSO",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"FSO" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"FS Seek",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"FS Seek" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"FS Write",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"FS Write" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"FS Read",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"FS Read" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Memory",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Memory" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Graphics",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Graphics" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Image",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Image" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Font",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Font" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"Sound",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Sound" ,rect.X ,rect.Y, 0xffff);
    j++;

    i51AdeMmiGetTextAttr(L"UPS",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"UPS" ,rect.X ,rect.Y, 0xffff);
    j++;

    rect.X =2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 22;
    rect.Width = DrawWidth*2-2;
    i51KitG2CleanScreen(i51KitG2COLOR(128, 128, 128),&rect);
    rect.X =DrawWidth*2+2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 22;
    rect.Width = DrawWidth-2;
    i51KitG2CleanScreen(i51KitG2COLOR(128, 128, 128),&rect);
    rect.X =DrawWidth*3+2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 22;
    rect.Width = DrawWidth-2;
    i51KitG2CleanScreen(i51KitG2COLOR(128, 128, 128),&rect);
    rect.X =DrawWidth*4+2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 22;
    rect.Width = DrawWidth-2;
    i51KitG2CleanScreen(i51KitG2COLOR(128, 128, 128),&rect);

    i51AdeMmiGetTextAttr(L"Total Cost",&WordWidth,&WordHeight);
    rect.X = DrawWidth-WordWidth/2;
    rect.Y = HeightPerOne+24*j+2;
    rect.Height = 24;
    rect.Width = DrawWidth*2;
    DrawText(rect.Width , rect.Height ,L"Total Cost" ,rect.X ,rect.Y, 0xffff);
    j++;
#endif

    i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
    if(CloseColor == 0xffff)
    {
        rect.X =nWidth - StrWidth-12;
        rect.Y = (nHeight-HeightEnd) +HeightEnd/2 -StrHeight/2-2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =nWidth - StrWidth-12+1;
        rect.Y = (nHeight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =nWidth - StrWidth-12+StrWidth+4;
        rect.Y = (nHeight-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =nWidth - StrWidth-10;
        rect.Y = (nHeight-HeightEnd) +HeightEnd/2 -StrHeight/2;
        rect.Height = HeightEnd;
        rect.Width = nWidth/2;
        DrawText(rect.Width ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    }
    else
    {
        rect.X =nWidth - StrWidth-12+2;
        rect.Y = (nHeight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =nWidth - StrWidth-12+1+2;
        rect.Y = (nHeight-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4+2;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =nWidth - StrWidth-12+StrWidth+4+2;
        rect.Y = (nHeight-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =nWidth - StrWidth-10+2;
        rect.Y = (nHeight-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
        rect.Height = HeightEnd;
        rect.Width = nWidth/2;
        DrawText(rect.Width ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,0);
    }
}
#endif

void DrowG2CPU()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0 , StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightHead = Theight/10;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"CPU Test", &StrWidth , &StrHeight);
    rect.X = Twidth/2 -StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"CPU Test" ,rect.X ,rect.Y, 0xffff);
}

void DrowG2IO()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0 , StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightHead = Theight/10;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"IO Test", &StrWidth , &StrHeight);
    rect.X = Twidth/2 -StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"IO Test" ,rect.X ,rect.Y, 0xffff);
}

void DrowG2Sound()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0 , StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightHead = Theight/10;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Sound Test", &StrWidth , &StrHeight);
    rect.X = Twidth/2 -StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"Sound Test" ,rect.X ,rect.Y, 0xffff);
}

void DrowG2G2()
{
    iU16 Twidth = 0 ;
    iU16 Theight = 0 ;
    iU16 HeightHead = 0;
    iRECT rect = {0};
    iU16 StrWidth = 0 , StrHeight = 0;

    i51AdeMmiGetScreenScale ( &Twidth , &Theight ) ;
    HeightHead = Theight/10;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"G2 Test", &StrWidth , &StrHeight);
    rect.X = Twidth/2 -StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = Twidth;

    DrawText(rect.Width , rect.Height ,L"G2 Test" ,rect.X ,rect.Y, 0xffff);
}
static void MainPaint()
{
    if((Runstatus==0&&CommitReport==0)||Runstatus==4||Runstatus==18||Runstatus==19||(Runstatus>=9&&Runstatus<=16&&InterFoucs!=10))
    {
        WaveFlag++;
    }
    else
    {
        WaveFlag = 0;
    }

    if(WaveFlag==20)
    {
        if(Runstatus==0)
        {
            InterColor0[InterFoucs-1]=i51KitG2COLOR(7, 24, 40);
            InterColor1[InterFoucs-1]=i51KitG2COLOR(7, 24, 40);
            InterWordColor[InterFoucs-1] = i51KitG2COLOR(255, 255, 255);
        }
        else
        {
            InterColor0[InterFoucs-1] = i51KitG2COLOR(7, 24, 40);
            InterColor1[InterFoucs-1] = i51KitG2COLOR(7, 24, 40);
            InterWordColor[InterFoucs-1] = i51KitG2COLOR(255, 255, 255);
        }
    }
    if(WaveFlag==30)
    {
        WaveFlag = 0;
        if(Runstatus==0)
        {
            InterColor0[InterFoucs-1]=i51KitG2COLOR(231, 236, 242);
            InterColor1[InterFoucs-1]=i51KitG2COLOR(198,205,210);
            InterWordColor[InterFoucs-1] = i51KitG2COLOR(0, 0, 0);
        }
        else
        {
            InterColor0[InterFoucs-1] = i51KitG2COLOR(231, 236, 242);
            InterColor1[InterFoucs-1] = i51KitG2COLOR(198,205,210);
            InterWordColor[InterFoucs-1] = i51KitG2COLOR(0, 0, 0);
        }
    }
    switch(Runstatus)
    {
    case 0:
        {
            i51PhoneTestDrawCopyrights () ;
        }
        break;
    case 1:
        {
            DrowAdapter();
        }
        break;
    case 2:
        {
            DrowKernel();
        }
        break;
    case 3:
        {
            BenchmarkMain();
        }
        break;
    case 4:
        {
            DrowMenu();
        }
        break;
    case 5:
        {
            Drowi51Loger();
        }
        break;
    case 9:
        {
            DrowInputTest();
        }
        break;
    case 10:
        {
            DrowSMSTest();
        }
        break;
    case 11:
        {
            DrowSocketTest();
        }
        break;
    case 12:
        {
            DrowMMITest();
        }
        break;
    case 13:
        {
            DrowMakeCallTest();
        }
        break;
    case 14:
        {
            DrowShockTest();
        }
        break;
    case 15:
        {
            DrawHookTest();
        }
        break;
    case 16:
        {
            DrawSetTimeTest();
        }
        break;
    case 18:
        {
            DrowMmiSubPlayerTest();
        }
        break;
    case 19:
        {
            DrowMmiSubRecorderTest();
        }
        break;
    }
}

static void i51KeyEvent( iU16 event,iU32 para)
{
    static iBOOL keyStatus[13] = {0};
    iU8 keyFouns = 0;

    switch ( (iU32 ) para ) {
        case i51_KEY_NUM1 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[0] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[0])
                {
                    keyStatus[0] = iFALSE;
                    if(Runstatus==0)
                    {
                        i51KeyEvent1() ;
                    }
                    else if(Runstatus==4)
                    {
                        InterKeyDo(0);
                    }
                    else if(Runstatus==9)
                    {
                        i51InputCheckDo(0);
                    }
                    else if(Runstatus==12)
                    {
                        i51MMIDO(0);
                    }
                    else if(Runstatus == 13)
                    {
                        i51CallDo(0);
                    }
                    else if(Runstatus == 14)
                    {
                        i51ShockDo(0);
                    }
                    else if(Runstatus == 15)
                    {
                        SetHook();
                    }
                    else if(Runstatus == 16)
                    {
                        i51SetTimeDo();
                    }
                    else if(Runstatus == 18)
                    {
                        i51MmiSubPlayerDO(0);
                    }
                    else if(Runstatus == 19)
                    {
                        i51MmiSubRecorderDO(0);
                    }
                }
            }
            break ;
        case i51_KEY_NUM2 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[1] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[1])
                {
                    keyStatus[1] = iFALSE;
                    if(Runstatus==0)
                    {
                        i51KeyEvent2() ;
                    }
                    else if(Runstatus==4)
                    {
                        InterKeyDo(1);
                    }
                    else if(Runstatus==9)
                    {
                        i51InputCheckDo(1);
                    }
                    else if(Runstatus==12)
                    {
                        i51MMIDO(1);
                    }
                    else if(Runstatus == 13)
                    {
                        i51CallDo(1);
                    }
                    else if(Runstatus == 14)
                    {
                        i51ShockDo(1);
                    }
                    else if(Runstatus == 18)
                    {
                        i51MmiSubPlayerDO(1);
                    }
                    else if(Runstatus == 19)
                    {
                        i51MmiSubRecorderDO(1);
                    }
                }
            }
            break ;
        case i51_KEY_NUM3 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[2] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[2])
                {
                    keyStatus[2] = iFALSE;
                    if(Runstatus==0)
                    {
                        i51KeyEvent3() ;
                    }
                    else if(Runstatus==4)
                    {
                        InterKeyDo(2);
                    }
                    else if(Runstatus==9)
                    {
                        i51InputCheckDo(2);
                    }
                    else if(Runstatus==12)
                    {
                        i51MMIDO(2);
                    }
                    else if(Runstatus == 14)
                    {
                        i51ShockDo(2);
                    }
                    else if(Runstatus == 18)
                    {
                        i51MmiSubPlayerDO(2);
                    }
                    else if(Runstatus == 19)
                    {
                        i51MmiSubRecorderDO(2);
                    }
                }
            }
            break ;
        case i51_KEY_NUM4 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[3] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[3])
                {
                    keyStatus[3] = iFALSE;
                    if(Runstatus==0)
                    {
                        i51KeyEvent4() ;
                    }
                    else if(Runstatus==4)
                    {
                        InterKeyDo(3);
                    }
                    else if(Runstatus==12)
                    {
                        i51MMIDO(3);
                    }
                    else if(Runstatus == 14)
                    {
                        i51ShockDo(3);
                    }
                    else if(Runstatus == 18)
                    {
                        i51MmiSubPlayerDO(3);
                    }
                    else if(Runstatus == 19)
                    {
                        i51MmiSubRecorderDO(3);
                    }
                }
            }
            break ;
        case i51_KEY_NUM5 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[4] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[4])
                {
                    keyStatus[4] = iFALSE;
                    if(Runstatus==0)
                    {
                        i51KeyEvent5() ;
                    }
                    else if(Runstatus==4)
                    {
                        InterKeyDo(4);
                    }
                    else if(Runstatus==12)
                    {
                        i51MMIDO(4);
                    }
                    else if(Runstatus == 18)
                    {
                        i51MmiSubPlayerDO(4);
                    }
                }
            }
            break ;
        case i51_KEY_NUM6 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[5] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[5])
                {
                    keyStatus[5] = iFALSE;
                    if(Runstatus==4)
                    {
                        InterKeyDo(5);
                    }
                    else if(Runstatus==12)
                    {
                        i51MMIDO(5);
                    }
                }
            }
            break ;
        case i51_KEY_NUM7 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[6] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[6])
                {
                    keyStatus[6] = iFALSE;
                    if(Runstatus==4)
                    {
                        InterKeyDo(6);
                    }
                }
            }
            break ;
        case i51_KEY_NUM8 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[7] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[7])
                {
                    keyStatus[7] = iFALSE;
                    if(Runstatus==4)
                    {
                        InterKeyDo(7);
                    }
                }
            }
            break ;
        case i51_KEY_NUM9 :
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[8] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[8])
                {
                    keyStatus[8] = iFALSE;
                    if(Runstatus==4)
                    {
                        InterKeyDo(8);
                    }
                }
            }
            break ;

        case i51_KEY_ARROW_DOWN:
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[9] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[9])
                {
                    keyStatus[9] = iFALSE;
                    if(Runstatus==0&&CommitReport==0)
                    {
                        InterFoucs++;
                        if(InterFoucs>5)InterFoucs=1;
                        SetFous(InterFoucs);
                    }
                    else if(Runstatus==4)
                    {
                        InterFoucs++;
                        if(InterFoucs>9)InterFoucs=1;
                        InterSetFous(InterFoucs);
                    }
                    else if(Runstatus==9)
                    {
                        InterFoucs++;
                        if(InterFoucs>3)InterFoucs=1;
                        InterSetFous(InterFoucs);
                    }
                    else if(Runstatus == 12)
                    {
                        keyFouns = InterFoucs;
                        while(1)
                        {
                            keyFouns++;
                            if(keyFouns>6)keyFouns=1;
                            if(InterWordColor[keyFouns-1]==0xffff||InterWordColor[keyFouns-1]==i51KitG2COLOR(0, 0, 0))
                            {
                                SetMoveFous(keyFouns);
                                break;
                            }
                        }
                    }
                    else if(Runstatus == 18)
                    {
                        keyFouns = InterFoucs;
                        while(1)
                        {
                            keyFouns++;
                            if(keyFouns>5)keyFouns=1;
                            if(InterWordColor[keyFouns-1]==0xffff||InterWordColor[keyFouns-1]==i51KitG2COLOR(0, 0, 0))
                            {
                                SetMoveFous(keyFouns);
                                break;
                            }
                        }
                    }
                    else if(Runstatus == 19)
                    {
                        keyFouns = InterFoucs;
                        while(1)
                        {
                            keyFouns++;
                            if(keyFouns>4)keyFouns=1;
                            if(InterWordColor[keyFouns-1]==0xffff||InterWordColor[keyFouns-1]==i51KitG2COLOR(0, 0, 0))
                            {
                                SetMoveFous(keyFouns);
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case i51_KEY_ARROW_UP:
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[10] = iTRUE;
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[10])
                {
                    keyStatus[10] = iFALSE;
                    if(Runstatus==0&&CommitReport==0)
                    {
                        InterFoucs--;
                        if(InterFoucs==0)InterFoucs=5;
                        SetFous(InterFoucs);
                    }
                    else if(Runstatus==4)
                    {
                        InterFoucs--;
                        if(InterFoucs==0)InterFoucs=9;
                        InterSetFous(InterFoucs);
                    }
                    else if(Runstatus==9)
                    {
                        InterFoucs--;
                        if(InterFoucs==0)InterFoucs=3;
                        InterSetFous(InterFoucs);
                    }
                    else if(Runstatus == 12)
                    {
                        keyFouns = InterFoucs;
                        while(1)
                        {
                            keyFouns--;
                            if(keyFouns==0)keyFouns=6;
                            if(InterWordColor[keyFouns-1]==0xffff||InterWordColor[keyFouns-1]==i51KitG2COLOR(0, 0, 0))
                            {
                                SetMoveFous(keyFouns);
                                break;
                            }
                        }
                    }
                    else if(Runstatus == 18)
                    {
                        keyFouns = InterFoucs;
                        while(1)
                        {
                            keyFouns--;
                            if(keyFouns==0)keyFouns=5;
                            if(InterWordColor[keyFouns-1]==0xffff||InterWordColor[keyFouns-1]==i51KitG2COLOR(0, 0, 0))
                            {
                                SetMoveFous(keyFouns);
                                break;
                            }
                        }
                    }
                    else if(Runstatus == 19)
                    {
                        keyFouns = InterFoucs;
                        while(1)
                        {
                            keyFouns--;
                            if(keyFouns==0)keyFouns=4;
                            if(InterWordColor[keyFouns-1]==0xffff||InterWordColor[keyFouns-1]==i51KitG2COLOR(0, 0, 0))
                            {
                                SetMoveFous(keyFouns);
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case i51_KEY_RIGHT:
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[11] = iTRUE;
                    if(CommitReport==0)
                    {
                        CloseColor = i51KitG2COLOR(150,150,150);
                    }
                    else
                    {
                        ReportColor2 = i51KitG2COLOR(150,150,150);
                    }
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[11])
                {
                    if(CommitReport==0)
                    {
                        CloseColor = 0xffff;
                    }
                    else
                    {
                        ReportColor2 = 0xffff;
                    }
                    keyStatus[11] = iFALSE;
                    i51RightKeyEvent();
                }
            }
            break;
        case i51_KEY_LEFT:
            {
                if(i51_MSG_KEYDOWN == event)
                {
                    keyStatus[12] = iTRUE;
                    if(CommitReport==0)
                    {
                        OkColor = i51KitG2COLOR(150,150,150);
                    }
                    else
                    {
                        ReportColor1 = i51KitG2COLOR(150,150,150);
                    }
                }
                else if((i51_MSG_KEYUP == event)&&keyStatus[12])
                {
                    //OkColor = 0xffff;
                    if(CommitReport==0)
                    {
                        OkColor = 0xffff;
                    }
                    else
                    {
                        ReportColor1 = 0xffff;
                    }
                    keyStatus[12] = iFALSE;
                    i51LeftKeyEvent();
                }
            }
            break;
    }
}
static void DoubleCallback(iU32 ID)
{
    i51AdeOsSetShock(iFALSE);
    i51AdeOsTimerStop(ID);
}
static void i51DoDouble()
{
    iU32 id;
    FrameNume++;
    if(FrameNume>=5)
    {
        if((DoubleClick==1&&CommitReport==0&&KeyDoubleFlag==0)||(Runstatus==0&&DoubleClick==1&&CommitReport==0&&KeyDoubleFlag==1&&i51_KEY_LEFT==DoublePara))
        {
            ResetInterStatus();
            i51AdeOsSetShock(iTRUE);
            i51AdeOsDoShock(100);
            id = i51AdeOsTimerCreate (0,1) ;
            i51AdeOsTimerStart(id,DoubleCallback,100);

            if(KeyDoubleFlag==1)
            {
                OkColor = 0xffff;
            }
            KeyDoubleFlag = 0;
        }
        else
        {
            if(KeyDoubleFlag==0)
            {
                i51SimplePortalTouchDo (i51_MSG_PADUP , DoublePara) ;
            }
            else
            {
                i51KeyEvent(i51_MSG_KEYUP,DoublePara);
                KeyDoubleFlag = 0;
            }

        }
        DoubleClick = 0;
        FrameNume = 0;
        DoubleFlag = 0;
    }
}

static void GetCheckStatus()
{        
    if(AllStatics[0]==2&&AllStatics[1]==2&&AllStatics[2]==2&&AllStatics[4]==2&&AllStatics[5]==2)
        //if(AllStatics[5]==2)
    {
        CommitReport = 1;
        SendButton = 1;
        WriteAllStatus = 1;
        SetFous(1);
    }
    else
    {
        CommitReport = 0;
        SendButton = 0;
    }
}
#if 0
void ReadTest()
{
    TREPORT report = {0};
    iFILE file = 0;
    iU16 datab[10] = {0};
    iU8 i =0;
    iCHAR *p = NULL;
    iCHAR da = 0;

    file = i51AdeOsFileOpen ( L"Testt.tr" , i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
    if(file!=0)
    {
        i51AdeOsFileRead(file,report.Magic,2,0);
        i51AdeOsFileRead(file,&report.Version,4,0);
        i51AdeOsFileRead(file,&report.TotallCases,4,0);
        i51AdeOsFileRead(file,&report.CaseEntry,4,0);

        i51AdeOsFileRead(file,&report.CreateTime.Year,2,0);
        i51AdeOsFileRead(file,&report.CreateTime.Month,2,0);
        i51AdeOsFileRead(file,&report.CreateTime.Day,2,0);
        i51AdeOsFileRead(file,&report.CreateTime.Hour,2,0);
        i51AdeOsFileRead(file,&report.CreateTime.Minute,2,0);
        i51AdeOsFileRead(file,&report.CreateTime.Second,2,0);

        i51AdeOsFileRead(file,&report.PhoneInfo.Endian,1,0);
        i51AdeOsFileRead(file,&report.PhoneInfo.KernelVersion,4,0);
        i51AdeOsFileRead(file,&report.PhoneInfo.AdapterVersion,4,0);
        i51AdeOsFileRead(file,&report.PhoneInfo.Projectid,4,0);
        i51AdeOsFileRead(file,report.PhoneInfo.Platform,64,0);
        i51AdeOsFileRead(file,report.PhoneInfo.Sn,16,0);
        i51AdeOsFileRead(file,report.PhoneInfo.IMEI,32,0);
        i51AdeOsFileRead(file,report.PhoneInfo.BatchCode,16,0);
        i51AdeOsFileRead(file,&report.PhoneInfo.DiskFreeSpace,4,0);
        i51AdeOsFileRead(file,&report.PhoneInfo.MemoryAvalible,4,0);

        i51AdeOsFileRead(file,report.Sim.IMSI,32,0);
        i51AdeOsFileRead(file,report.Sim.IMSC,32,0);
        i51AdeOsFileRead(file,&report.Sim.bid,4,0);
        i51AdeOsFileRead(file,&report.Sim.zid,4,0);

        i51AdeOsFileRead(file,&report.KitServerInfo.sendsmstotall,4,0);
        i51AdeOsFileRead(file,&report.KitServerInfo.succsmstotall,4,0);
        i51AdeOsFileRead(file,&report.KitServerInfo.hookedtotall,4,0);
        i51AdeOsFileRead(file,&report.KitServerInfo.Intercepted,4,0);
        i51AdeOsFileRead(file,&report.KitServerInfo.Interceptedsend,4,0);

        i51AdeOsFileRead(file,&report.Benchmark.Integer,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.Float,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.FSO,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.FsSeek,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.FsWrite,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.FsRead,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.Memory,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.Graphics,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.Image,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.Font,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.Sound,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.UPS,2,0);
        i51AdeOsFileRead(file,&report.Benchmark.TotallCost,2,0);
        i51AdeOsFileRead(file,datab,20,0);

        i51AdeOsFileClose(file);
    }
}
#endif

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

    //    TODO :    ÏûÏ¢Ñ­»·
    iU32 x= 0 , y = 0 ;
    iBOOL rest = 0;
    iU32 value = (iU32)parameter;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    switch ( message )
    {
    case i51_MSG_PAINT :
        {
            if(CommitReport==0)
            {
                GetCheckStatus();
            }                    
            if(DoubleFlag)
            {
                i51DoDouble();
            }
            else if(2 == PaintStatus)
            {
                MainPaint();
                PaintStatus = 0;
            }
            else if(1 == PaintStatus)
            {
                MainPaint();
            }
        }
        break ;

    case i51_MSG_RUN :
        {
            iFILE file = 0;
            TypeheadHandle = i51KitStdResOpen(L"i51Debuger\\Typehead.re");
            rest = i51KitG2FontCachEnable(TypeheadHandle,0);
            i51AdeOsLog(0,"i51Debuger Version:0.0.15");

            OldSysTime = i51AdeOsGetTick();
            i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight );
            CloseColor = 0xffff;
            OkColor = 0xffff;
            SimplePortalId = aid;
            PlatId =  i51AdeCnfGetType();
            i51AdeOsLog(0,"SYSTEM ID:%d",PlatId);
            i51AdeOsScreenAlwaysLight(1);
            if(!i51AdeOsFileIsDirExist(L"i51Debuger\\i51PhoneTestFile"))
            {
                i51AdeOsFileCreateDir (L"i51Debuger\\i51PhoneTestFile") ;
                i51AdeOsFileSetAttr(L"i51Debuger\\i51PhoneTestFile",i51_OS_FL_ATTR_HID );
                file = i51AdeOsFileOpen (L"i51Debuger\\i51PhoneTestFile\\Error.dat" , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN , __FILE__ , __LINE__ ) ;
                if(file!=0)
                {
                    i51AdeOsFileClose(file);
                }
            }
            if(!i51AdeOsFileIsExist(L"i51Debuger\\i51PhoneTestFile\\TestReport.tr"))
            {
                file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\TestReport.tr" , i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                i51KitTReportCreateHeader (file);
                i51AdeOsFileClose(file);
            }
            if(!i51AdeOsFileIsDirExist(L"i51Debuger\\Sound"))
            {
                i51AdeOsFileCreateDir (L"i51Debuger\\Sound") ;
            }
            i51GetAllStatus();
            //ReadTest();

        }
        break ;

    case i51_MSG_INIT :
        {
            i51KIT_STATIC_KIT_SET(Adapter,parameter);
            i51KIT_STATIC_KIT_ASSERT(Adapter);

            SetFous(1);
            i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
            if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) )
            {
                return 0;
            }
            i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");

            if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ) 
            {
                return 0;
            }
        }
        break ;

    case i51_MSG_KEYUP :
        {
            DoubleFlag = 1;
            if(FrameNume==0)
            {
                DoubleClick = 0;
            }
            DoublePara = (iU32)parameter;
            KeyDoubleFlag = 1;
            //i51KeyEvent(i51_MSG_KEYUP,(iU32 ) parameter);
        }
        break ;

    case i51_MSG_KEYDOWN :
        DoubleClick = 1;
        i51KeyEvent(i51_MSG_KEYDOWN,(iU32 ) parameter);
        break ;

    case i51_MSG_PADUP :
        {
            switch(Runstatus)
            {
            case 0:
                {
                    DoubleFlag = 1;
                    if(FrameNume==0)
                    {
                        DoubleClick = 0;
                    }
                    DoublePara = (iU32)parameter;
                }
                break;
            case 1:
                {
                    i51SimplePortalTouchDo (i51_MSG_PADUP , (iU32)parameter) ;
                }
                break;
            case 2:
                {
                    i51SimplePortalTouchDo (i51_MSG_PADUP , (iU32)parameter) ;
                }
                break;
            case 3:
                {
                    if(BenchmarkState==BENCHMARK_RESULT&&(y>ScreenHeight-ScreenHeight/12&&y<ScreenHeight&&x>ScreenWidth/2&&x<ScreenWidth))
                    {

                        Runstatus = 0;

                        CloseColor = 0xffff;
                        BenchmarkState = BENCHMARK_IDLE;
                        break;
                    }
                }
                break;
            case 4:
                {
                    i51InterTTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 5:
                {
                    i51SimplePortalTouchDo (i51_MSG_PADUP , (iU32)parameter) ;
                }
                break;
            case 8:
                {
                    if(y>ScreenHeight-ScreenHeight/12&&y<ScreenHeight&&x>ScreenWidth/2&&x<ScreenWidth)
                    {
                        Runstatus = 3;
                    }
                }
                break;
            case 9:
                {
                    i51InterInputTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 10:
                {
                    i51InterSMSTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 11:
                {
                    i51InterSocketTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 12:
                {
                    i51InterMMITouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 13:
                {
                    i51InterCallTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 14:
                {
                    i51InterShockTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 15:
                {
                    i51InterHookTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 16:
                {
                    i51InterSetTimeTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 18:
                {
                    i51InterMMISubPlayerTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            case 19:
                {
                    i51InterMMISubRecorderTouchDo (i51_MSG_PADUP , (iU32)parameter);
                }
                break;
            }
        }         
        break ;
    case i51_MSG_PADDOWN:
        {
            switch(Runstatus)
            {
            case 0:
                {
                    DoubleClick = 1;
                    i51SimplePortalTouchDo (i51_MSG_PADDOWN , (iU32)parameter) ;
                }
                break;
            case 1:
                {
                    i51SimplePortalTouchDo (i51_MSG_PADDOWN , (iU32)parameter) ;
                }
                break;
            case 2:
                {
                    i51SimplePortalTouchDo (i51_MSG_PADDOWN , (iU32)parameter) ;
                }
                break;
            case 3:
                {
                    if(BenchmarkState==BENCHMARK_RESULT&&(y>ScreenHeight-ScreenHeight/12&&y<ScreenHeight&&x>ScreenWidth/2&&x<ScreenWidth))
                    {
                        CloseColor = i51KitG2COLOR(150,150,150);
                    }
                }
                break;
            case 4:
                {
                    i51InterTTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 5:
                {
                    i51SimplePortalTouchDo (i51_MSG_PADDOWN , (iU32)parameter) ;
                }
                break;
            case 8:
                {
                    if(y>ScreenHeight-ScreenHeight/12&&y<ScreenHeight&&x>ScreenWidth/2&&x<ScreenWidth)
                    {
                        Runstatus = 3;
                    }
                }
                break;
            case 9:
                {
                    i51InterInputTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 10:
                {
                    i51InterSMSTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 11:
                {
                    i51InterSocketTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 12:
                {
                    i51InterMMITouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 13:
                {
                    i51InterCallTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 14:
                {
                    i51InterShockTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 15:
                {
                    i51InterHookTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 16:
                {
                    i51InterSetTimeTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 18:
                {
                    i51InterMMISubPlayerTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            case 19:
                {
                    i51InterMMISubRecorderTouchDo (i51_MSG_PADDOWN , (iU32)parameter);
                }
                break;
            }
        }
        break;
    case i51_MSG_PADMOVE :
        {
            if(Runstatus==12||Runstatus==14)
            {
                i51SimpleInterMove((iU32)parameter);
            }
            else
            {
                i51SimplePortalMove((iU32)parameter) ;
            }

        }
        break ;

    case i51_MSG_PAUSE :
        PaintStatus = 0;
        break ;
    case i51_MSG_RESUME:
        PaintStatus = 1;
        break;

    case i51_MSG_EXIT :

        i51AdeOsScreenAlwaysLight(0);
        if(NULL!=ReportData)
        {
            i51AdeOsFree(ReportData);
            ReportData = NULL;
        }
        if(NULL != SoundBuffer)
        {
            i51AdeOsFree(SoundBuffer);
            SoundBuffer = NULL;
            i51AdeOsLog(0,"free SoundBuffer memory");
        }
        if(NULL != pSoundDataStdRecordPcm)
        {
            i51AdeOsFree(pSoundDataStdRecordPcm);
            pSoundDataStdRecordPcm = NULL;
            i51AdeOsLog(0,"free pSoundDataStdRecordPcm memory");
        }
        i51AdeOsLog(0,"i51KitG2FontCachDisable");
        rest = i51KitG2FontCachDisable(TypeheadHandle,0);
        i51AdeOsLog(0,"i51KitG2FontCachDisable:%d",rest);
        i51KitStdResClose(TypeheadHandle);
        TypeheadHandle = iNULL;
#if 1
        if(VP_Bee!=NULL)
            i51KitG2VPDestroy(VP_Bee);
        if(VP_BK!=NULL)
            i51KitG2VPDestroy(VP_BK);
        if(VP_Road!=NULL)
            i51KitG2VPDestroy(VP_Road);
        if(VP_Snidget!=NULL)
            i51KitG2VPDestroy(VP_Snidget);

        VP_Bee = iNULL;
        VP_BK = iNULL;
        VP_Road = iNULL;
        VP_Snidget = iNULL;
        if(CurrentImageContainer!=NULL)
            i51KitG2CloseImageContainer(CurrentImageContainer);
        CurrentImageContainer = iNULL;
        i51KIT_DYNAMIC_KIT_UNLOAD(i51KitStd);
        i51KIT_DYNAMIC_KIT_UNLOAD(i51KitG2);
#endif
        break ;
    }
    return iTRUE ;

}

void i51UpdataReport(iU8 para,TREPORT report)
{
    iFILE file = 0;
    TREPORT updataReport = {0};
    file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\TestReport.tr" , i51_OS_FL_EXISTING_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ; 
    if(file==0)return;

    i51KitTReportReadHeader ( file , &updataReport );

    switch(para)
    {
    case 0:
        {
            memcpy(&updataReport.PhoneInfo,&report.PhoneInfo,sizeof(report.PhoneInfo));
        }
        break;
    case 1:
        {
            memcpy(&updataReport.Sim,&report.Sim,sizeof(report.Sim));
        }
        break;
    case 2:
        {
            memcpy(&updataReport.Benchmark,&report.Benchmark,sizeof(report.Benchmark));
        }
        break;
    default:break;
    }
    i51KitTReportUpdateHeader (file , &updataReport );
    i51AdeOsFileClose (file) ;
}
iKIT iU32* i51KitPort ()
{

    //    TODO : Initialize API Port for external layer

    //    i51KIT_DYNAMIC_KIT_MAPING_BEGIN(DlDemo)
    //    i51KIT_DYNAMIC_KIT_MAPING_END 

    //    return i51KIT_DYNAMIC_KIT_GET_MAPING(DlDemo) ;

}
