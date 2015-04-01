#include "i51Debuger.h"
#include "TReport.h"

iU8 MTestStatus = 0;
iU8 MediaOpen = 0;
iU8 InterFoucs = 1;
iU32 MediaResult[4] = {0};
extern iCOLOR InterColor0[9];
extern iCOLOR InterColor1[9];
extern iCOLOR InterWordColor[9];
extern iU8 PaintStatus;

static iU32 SendId = 0;
static iU32 PhoneId = 0;
static iU32 PlayEndTime = 0;
static iU32 TotalError= 0;
static iU8 SMSReset = 0;
static iU8 sendOnce = 0;
static iU8 InputStatus[3] = {0};
static iU8 SMSStatus = 0;
static iU8 SocketStatus = 0;
static iU8 MMIStatus = 0;
static iU8 CallStatus = 0;
static iU8 ShockStatus = 0;
static iU8 HookStatus = 0;
static iU8 TimeStatus = 0;
static iU8 LinkerStatus = 0;
static i51SOCADDR address = {0};
static iU32 layerid = 0;
static iS32 SocId = 0;
static iU32 ShockTime = 0;
static iU32 PlayMediaTime = 0;
static iU8 ColorFlag0 = 0;
static iU8 ColorFlag1 = 0;
static iU8 ColorFlag2 = 0;
static iS32 smshookid = 0;
static iCHAR SocketHead[1024] = {0};
static iCHAR SockRecBuff[2048];
static void* Handle = NULL;
static iU16 SendCBuff[] = {0x77ed,0x4fe1,0x6d4b,0x8bd5,0x00};
static iU16 EditorBuff[32]={0};
static iU8 PlayType = 0;
static iCHAR TestBuff[10][64]={0};
#if 0
static iCHAR TestBuff[0][64] = {0};
static iCHAR TestBuff[1][64] = {0};
static iCHAR TestBuff[2][64] = {0};
static iCHAR TestBuff[3][64] = {0};
static iCHAR TestBuff[4][64] = {0};
static iCHAR TestBuff[5][64] = {0};
static iCHAR TestBuff[6][64] = {0};
static iCHAR TestBuff[7][64] = {0};
static iCHAR TestBuff[8][64] = {0};
static iCHAR TestBuff[9][64] = {0};
#endif
static iU8 RecFirstFlag = 0;
static iU32 RecAllLen = 0;
static iU32 RecedLen = 0;
static iU32 TimeOutId = 0;
static iU8 reTimes = 0;
static iU8 OutTimeStatus = 0;
static void OutTimerLinker(iU32 ID);

extern iU8 Runstatus;
extern iFILE ResouceFile;
extern iFILE TypeheadHandle;
extern iU8 AllStatics[21];
extern iU8 YPoint;
extern iCOLOR CloseColor;
extern iCOLOR OkColor;
extern iS32 BenchmarkState;
extern iU32 CurrSysTime;
extern iU32 OldSysTime;
extern iU32 SimplePortalId;

typedef struct SETTIME
{
    iU16 year; 
    iU16 month;
    iU16 day;
    iU16 hour;
    iU16 mins;
    iU16 sec;
}TimeStruct;
static TimeStruct SetOsTime;

#define i51HTTP_PROXY_HEADER_PATTERN "GET /Lighthouse.jpg HTTP/1.1\r\nHost:  hz.51pk.com:80\r\nContent-Length:  %9d\r\n\r\n"

#define MAX_U_LONG 0X7FFFFFFF

#if 1
typedef struct{
    iU8		    szRiff[4];			/* "RIFF" */
    iU32		dwFileSize;			/* file size */
    iU8		    szWaveFmt[8];		/* "WAVEfmt " */
    iU32		dwFmtSize;			/* 16 */
    iU16        wFormatTag;         /* format type */
    iU16        nChannels;          /* number of channels (i.e. mono, stereo...) */
    iU32        nSamplesPerSec;     /* sample rate */
    iU32        nAvgBytesPerSec;    /* for buffer estimation */
    iU16        nBlockAlign;        /* block size of data */
    iU16        wBitsPerSample;     /* number of bits per sample of mono data */
}WAVE_HEADER;
#define SIZE_WAVE_HEADER sizeof(WAVE_HEADER)

typedef struct{
    iU8		szData[4];			/* "data" */
    iU32    dwDataSize;         /*pcm data size*/
}WAVE_DATA_HEADER;
#define SIZE_WAVE_DATA_HEADER sizeof(WAVE_DATA_HEADER)

WAVE_HEADER wavFileHeader;
WAVE_DATA_HEADER wavDataHeader;

iU8 WavHead[44] = { // 44 bytes
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, //0-15
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, //16-31
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, //32 ~ 43
};

extern const iU8 SoundDataStdMid[];
extern const iU32 SoundDataStdMidLength;
extern const iU16 SoundDataStdMidPath[] = {'i','5','1','D','e','b','u','g','e','r','\\','S','o','u','n','d','\\','S','t','d','M','i','d','.','m','i','d',0x00};
extern const iU8 *pSoundDataStdMid = SoundDataStdMid;

extern const iU8 SoundDataStdWav[];
extern const iU32 SoundDataStdWavLength;
extern const iU16 SoundDataStdWavPath[] = {'i','5','1','D','e','b','u','g','e','r','\\','S','o','u','n','d','\\','S','t','d','W','a','v','.','w','a','v',0x00};
extern const iU8 *pSoundDataStdWav = SoundDataStdWav;

extern const iU8 SoundDataStdAmr[];
extern const iU32 SoundDataStdAmrLength;
extern const iU16 SoundDataStdAmrPath[] = {'i','5','1','D','e','b','u','g','e','r','\\','S','o','u','n','d','\\','S','t','d','A','m','r','.','a','m','r',0x00};
extern const iU8 *pSoundDataStdAmr = SoundDataStdAmr;

extern const iU8 SoundDataStdMp3[];
extern const iU32 SoundDataStdMp3Length;
extern const iU16 SoundDataStdMp3Path[] = {'i','5','1','D','e','b','u','g','e','r','\\','S','o','u','n','d','\\','S','t','d','M','p','3','.','m','p','3',0x00};
extern const iU8 *pSoundDataStdMp3 = SoundDataStdMp3;

extern iU32 SoundDataStdRecordPcmLength = 100*1024;
extern const iU16 SoundDataStdRecordPcmPath[] = {'i','5','1','D','e','b','u','g','e','r','\\','S','o','u','n','d','\\','R','e','c','o','r','d','P','c','m','.','w','a','v',0x00};
extern iU8 *pSoundDataStdRecordPcm = NULL;

extern iU32 SoundDataStdRecordAmrLength = 100*1024;
extern const iU16 SoundDataStdRecordAmrPath[] = {'i','5','1','D','e','b','u','g','e','r','\\','S','o','u','n','d','\\','R','e','c','o','r','d','A','m','r','.','a','m','r',0x00};
extern iU8 *pSoundDataStdRecordAmr = NULL;

extern iU32 SoundBufferLengthMax = 100*1024;
extern iU32 SoundBufferLength = 0; //20*1024;
extern iU8 *SoundBuffer = NULL;

extern const iU8 *pCurSoundData = NULL;
extern iU32 CurSoundDataLength = 0;
extern const iU16 *pCurSoundDataPath = NULL;
extern i51METYPE CurSoundFormatType = i51_MEDIA_TYPE_MIDI;
extern i51MMIDEC CurSoundDataType = i51_MMI_DEC_FILE_BASED;
static iU8 *pCurStatus = iFALSE;
//extern iU16 i51MmiSubRunStatus = 0; // 0 为MMI界面，1-6为各个sub界面

typedef struct
{
    iU8 sound;
    iU8 mid;
    iU8 wav;
    iU8 amr;
    iU8 mp3;
    iU8 recordpcm;
    iU8 recordamr;
    iU8 curFous;
}STATUS_SOUND_T;

extern STATUS_SOUND_T status_sound = {0};
#endif

static iU32 StrToInt(iCHAR *s)
{
    iU32 num = 0;
    iCHAR *p = s;
    while(*p) 
    {
        if(num >= MAX_U_LONG/10)
        {
            return 0;
        }
        if((*p >= '0') && (*p <= '9')) num = 10*num + (*p) - '0';
        p++;
    }
    return num;
}
void InterSetFous(iU8 fous)
{
    iU8 i = 0;
    if (fous == 0)return;
    if(fous>9)return;

    for(i = 0;i<9;i++)
    {
        InterColor0[i]=i51KitG2COLOR(7, 24, 40);
        InterColor1[i]=i51KitG2COLOR(7, 24, 40);
        InterWordColor[i] = i51KitG2COLOR(255, 255, 255);
    }
    InterFoucs = fous;
    InterColor0[fous-1]=i51KitG2COLOR(231, 236, 242);
    InterColor1[fous-1]=i51KitG2COLOR(198,205,210);
    InterWordColor[fous-1] = i51KitG2COLOR(0, 0, 0);
}
void SetMoveFous(iU8 fous)
{
    if(fous>9)return;

    InterColor0[InterFoucs - 1]=i51KitG2COLOR(7, 24, 40);
    InterColor1[InterFoucs - 1]=i51KitG2COLOR(7, 24, 40);
    InterWordColor[InterFoucs - 1] = i51KitG2COLOR(255, 255, 255);

    InterColor0[fous-1]=i51KitG2COLOR(231, 236, 242);
    InterColor1[fous-1]=i51KitG2COLOR(198,205,210);
    InterWordColor[fous-1] = i51KitG2COLOR(0, 0, 0);
    InterFoucs = fous;
}

void StartCallCallback ( iBOOL results)
{
    memset(TestBuff[3],0,sizeof(TestBuff[3]));
    if(results)
    {
        sprintf(TestBuff[3],"connect SUCCEED...");
        CallStatus = 2;
    }
    else
    {
        TotalError++;
        sprintf(TestBuff[3],"connect FALSED...");
        CallStatus = 1;
    }
}

static void EditorCallback_Shock(void* buffer , iU32 textlen )
{
    iU16 *str = (iU16 *)buffer;
    iCHAR strd[32] = {0};
    iU16 buff[16] = {0};
    if(textlen==0||textlen>32)
    {
        sprintf(TestBuff[0],"Set Shock Time 0s...");
        return;
    }
    memcpy(buff,buffer,textlen*2);
    memset(TestBuff[0],0,sizeof(TestBuff[0]));
    memset(TestBuff[1],0,sizeof(TestBuff[1]));
    memset(TestBuff[2],0,sizeof(TestBuff[2]));
    memset(TestBuff[3],0,sizeof(TestBuff[3]));
    i51AdeStdUnicode2Ascii(str, strd );
    ShockTime = StrToInt(strd);
    sprintf(TestBuff[0],"Set Shock Time %d s...",ShockTime);
}
static void EditorCallback_Call(void* buffer , iU32 textlen )
{
    iCHAR strd[32] = {0};
    iU16 buff[32]={0};
    if(textlen==0||textlen>32)
    {
        sprintf(TestBuff[0],"Set Shock Time 0s...");
        return;
    }
    memcpy(buff,buffer,textlen*2);
    i51AdeStdUnicode2Ascii(buff, strd );
    PhoneId = i51AdeOsCreatePhone();
    memset(TestBuff[0],0,sizeof(TestBuff[0]));
    memset(TestBuff[1],0,sizeof(TestBuff[1]));
    memset(TestBuff[2],0,sizeof(TestBuff[2]));
    memset(TestBuff[3],0,sizeof(TestBuff[3]));
    memset(TestBuff[4],0,sizeof(TestBuff[4]));
    memset(TestBuff[5],0,sizeof(TestBuff[5]));
    if(PhoneId==0||PhoneId>5)
    {
        sprintf(TestBuff[0],"i51AdeOsCreatePhone Failed");
        TotalError++;
        CallStatus = 1;
        return;
    }
    sprintf(TestBuff[0],"i51AdeOsCreatePhone Succeed...");
    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
    InterFoucs = 10;
    if(i51AdeOsStartPhoneCall(PhoneId ,strd, StartCallCallback))
    {
        sprintf(TestBuff[1],"i51AdeOsStartPhoneCall  Succeed...");
        sprintf(TestBuff[2],"Wait for connect to %s....",strd);
        CallStatus = 2;
        InterSetFous(2);
        InterWordColor[0] = i51KitG2COLOR(128,128,128);
    }
    else
    {
        sprintf(TestBuff[1],"i51AdeOsStartPhoneCall  Failed...");
        TotalError++;
        CallStatus = 1;
    }
}

static void EditorCallback_Input0(void* buffer , iU32 textlen )
{
    iCHAR strscr[32] = "test";
    iCHAR strDotpay[] = "dotpay";
    iCHAR strd[32] = {0};
    iU16 buff[32] = {0};
    iS32 stlen = 0;
    if(textlen==0||textlen>32)
    {
        i51AdeOsLog(0,"EditorCallback_Input0 back 0");
        sprintf(TestBuff[2],"NO");
        InputStatus[0] = 1;
        ColorFlag0 = 0;
        return;
    }
    memcpy(buff,buffer,textlen*2);
    i51AdeStdUnicode2Ascii(buff, (iCHAR*) strd );
    if(memcmp(strd,strDotpay,strlen(strd))==0&&(strlen(strd)-strlen(strDotpay)==0))
    {
        //i51KernelSendRequest ( SimplePortalId , L"DotpayTest" , i51_REQ_LOADEX , 0 , 0 ) ;
        extern void i51DotpayDo(void);
        i51DotpayDo();
        return;
    }
    stlen = memcmp(strd,strscr,strlen(strd));
    memset(TestBuff[2],0,sizeof(TestBuff[2]));

    if(stlen==0&&(strlen(strd)==strlen(strscr)))
    {
        ColorFlag0 = 1;
        sprintf(TestBuff[2],"YES");
        i51AdeOsLog(0,"input0 succeed");
        InputStatus[0] = 2;
    }
    else
    {
        TotalError++;
        ColorFlag0 = 0;
        sprintf(TestBuff[2],"NO");
        i51AdeOsLog(0,"input0 failed");
        InputStatus[0] = 1;
    }
}
static void EditorCallback_Input1(void* buffer , iU32 textlen )
{
    iCHAR strscr[32] = "1234";
    iCHAR strd[32] = {0};
    iU16 buff[32] = {0};
    iS32 stlen = 0;
    if(textlen==0||textlen>32)
    {
        sprintf(TestBuff[3],"NO");
        i51AdeOsLog(0,"EditorCallback_Input1 back 0");
        ColorFlag1 = 0;
        InputStatus[1] = 1;
        return;
    }
    memcpy(buff,buffer,textlen*2);
    i51AdeStdUnicode2Ascii(buff, (iCHAR*) strd );
    stlen = memcmp(strd,strscr,strlen(strd));
    memset(TestBuff[3],0,sizeof(TestBuff[3]));

    if(stlen==0&&(strlen(strd)==strlen(strscr)))
    {
        sprintf(TestBuff[3],"YES");
        i51AdeOsLog(0,"input1 succeed");
        ColorFlag1 = 1;
        InputStatus[1] = 2;
    }
    else
    {
        TotalError++;
        sprintf(TestBuff[3],"NO");
        i51AdeOsLog(0,"input1 failed");
        ColorFlag1 = 0;
        InputStatus[1] = 1;
    }

}
static void EditorCallback_Input2(void* buffer , iU32 textlen )
{
    iU16 strscr[32] ={0x6d4b,0x8bd5,0x00};
    iU16 buff[32] = {0};
    iS32 stlen = 0;
    memset(TestBuff[4],0,sizeof(TestBuff[4]));


    if(textlen==0||textlen>32)
    {
        sprintf(TestBuff[4],"NO");
        ColorFlag2 = 0;
        InputStatus[2] = 1;
        i51AdeOsLog(0,"input2 failed");
        return;
    }
    memcpy(buff,buffer,textlen*2);
    stlen = i51AdeStdWStrcmp(strscr,buff);

    if(stlen==0)
    {
        sprintf(TestBuff[4],"YES");
        ColorFlag2 = 1;
        InputStatus[2] = 2;
        i51AdeOsLog(0,"input2 succeed");
    }
    else
    {
        TotalError++;
        sprintf(TestBuff[4],"NO");
        i51AdeOsLog(0,"input2 failed");
        ColorFlag2 = 0;
        InputStatus[2] = 1;
    }
}

static void EditorCallback_Sim0(void* buffer , iU32 textlen )
{
    iCHAR str[32] = {0};
    iU16 buff[32] = {0};
    if(textlen==0||textlen>32)
    {
        sprintf(TestBuff[0],"PhoneNumber is NULL");
    }
    memcpy(buff,buffer,textlen*2);

    i51AdeStdUnicode2Ascii(buff, (iCHAR*) str);
    memset(TestBuff[9],0,sizeof(TestBuff[9]));
    memcpy(TestBuff[9],str,strlen(str));
    sprintf(TestBuff[0],"PhoneNumber is:%s",str);
    sprintf(TestBuff[1],"Press To send:\"短信测试\"",str);
    i51AdeOsLog(0,"xx Input PhoneNumber:%s",str);
    InterSetFous(2);
    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
}

static void EditorCallback_Time(void* buffer , iU32 textlen )
{
    iCHAR str[32] = {0};
    iU16 buff[32] = {0};
    iCHAR num[10] = {0};
    iCHAR *p = NULL;
    iCHAR *q = NULL;
    if(textlen==0||textlen>32)
    {
        memset(&SetOsTime,0,sizeof(SetOsTime));
        return;
    }
    memcpy(buff,buffer,textlen*2);
    i51AdeStdUnicode2Ascii(buff, (iCHAR*) str);
    p = (iCHAR*)str;
    q = strstr(p,",");
    if(q==NULL)
    {
        memset(&SetOsTime,0,sizeof(SetOsTime));
        SetTime();
        return;
    }
    memcpy(num,(void *)p,(iU32)strlen(p)-(iU32)strlen(q));
    SetOsTime.year = (iU16)StrToInt((iCHAR *)num);

    p = q+1;
    q = strstr(p,",");
    if(q==NULL)
    {
        memset(&SetOsTime,0,sizeof(SetOsTime));
        SetTime();
        return;
    }
    memset((void*)num,0,sizeof(num));
    memcpy(num,(void *)p,(iU32)strlen(p)-(iU32)strlen(q));
    SetOsTime.month = (iU16)StrToInt((iCHAR *)num);

    p = q+1;
    q = strstr(p,",");
    if(q==NULL)
    {
        memset(&SetOsTime,0,sizeof(SetOsTime));
        SetTime();
        return;
    }
    memset(num,0,sizeof(num));
    memcpy(num,(void *)p,(iU32)strlen(p)-(iU32)strlen(q));
    SetOsTime.day = (iU16)StrToInt((iCHAR *)num);

    p = q+1;
    q = strstr(p,",");
    if(q==NULL)
    {
        memset(&SetOsTime,0,sizeof(SetOsTime));
        SetTime();
        return;
    }
    memset(num,0,sizeof(num));
    memcpy(num,(void *)p,(iU32)strlen(p)-(iU32)strlen(q));
    SetOsTime.hour = (iU16)StrToInt((iCHAR *)num);

    p = q+1;
    q = strstr(p,",");
    if(q==NULL)
    {
        memset(&SetOsTime,0,sizeof(SetOsTime));
        SetTime();
        return;
    }
    memset(num,0,sizeof(num));
    memcpy(num,(void *)p,(iU32)strlen(p)-(iU32)strlen(q));
    SetOsTime.mins = (iU16)StrToInt((iCHAR *)num);

    p = q+1;
    if(p!=NULL)
    {
        memset(num,0,sizeof(num));
        memcpy(num,(void *)p,(iU32)strlen(p));
        SetOsTime.sec = (iU16)StrToInt((iCHAR *)num);
    }
    else 
        SetOsTime.sec = 0;
    SetTime();
}
static void SendCallback( iBOOL results ) 
{
    if(results)
    {   
        i51AdeOsLog(0,"i51Debuger i51SendCallback TRUE ");
        if(Runstatus==10&&sendOnce==0)
        {
            sprintf(TestBuff[3],"SMS Callback Succeed");
            sprintf(TestBuff[4],"Press To Send:\"SMS Test\"");
            sendOnce = 1;
            InterSetFous(3);
            InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
            InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
            i51AdeSmsReleaseDevice(SendId);
        }
        else
        {
            iS32* sims = NULL;
            sims = i51AdeSmsGetSims();
            sprintf(TestBuff[6],"SMS Callback Succeed");
            sprintf(TestBuff[7],"请确认短信发送成功。");
            sprintf(TestBuff[8],"注意:该项必须遍历所有sim卡!");
            sprintf(TestBuff[9],"当前sim卡编号为: %d", sims[1]);
            i51AdeOsLog(0, "i51Debuger CALL Test is ok, sim card total:%d, index: %d", sims[0], sims[1]);    
            sendOnce = 2;
            InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
            InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
            InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
            i51AdeSmsReleaseDevice(SendId);
            SMSReset = 1;
        }
        SMSStatus = 2;

    }
    else
    {
        i51AdeOsLog(0,"i51Debuger i51SendCallback FALSE");
        TotalError++;
        if(Runstatus==10&&sendOnce==0)
        {
            sprintf(TestBuff[3],"SMS Callback1 FALSED");
        }
        else
        {
            sprintf(TestBuff[6],"SMS Callback2 FALSED");
        }
        SMSStatus = 1;
    }
}

static iU8 SMSMutualTest(iCHAR* phone , iU16* contents)
{
    iS32* sims = NULL;
    iU8 result = 0;

    SendId = i51AdeSmsCreateDevice();
    if(SendId==0||SendId>5)
    {
        TotalError++;
        result = 1;
        return 1;
    }
    sims = i51AdeSmsGetSims(  );
    if(sims[0]==0||sims[1]==0)
    {
        TotalError++;
        result = 2;
        return 2;
    }

    i51AdeOsLog(0,"i51AdeSmsCreateDevice succed");
    if(!i51AdeSmsSend(SendId, sims[1] , phone , contents ,0,SendCallback))
    {
        TotalError++;
        result = 3;
    }
    return result;
}

static void ReceiveCallback( iBOOL state , iS32 socket , iU8* buffer , iU32 textlen )
{
    iFILE file = 0;
    iCHAR *pData = NULL;
    iU32 len = 0;
    iCHAR *strS = NULL;
    iCHAR *strE = NULL;
    iCHAR datalen[32] = {0};
    iU32 i = 0;

    i51AdeOsLog(0,"soc receive back len:%d,%d",textlen,state);

    if(TimeOutId!=0)
    {
        i51AdeOsTimerStop(TimeOutId);
        TimeOutId = 0;
        reTimes = 0;
    }
    len = textlen;
    pData = (iCHAR *)buffer;
    if(state)
    {
        file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\xxx.re" , i51_OS_FL_ALWAYS_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
        if(file!=0)
        {
            i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_TAIL ) ;
            i51AdeOsFileWrite ( file , buffer, textlen , 0 ) ;
            i51AdeOsFileClose(file);
        }

        if(RecFirstFlag==0)
        {
            RecFirstFlag = 1;
            memset(TestBuff[6],0,sizeof(TestBuff[6]));
            sprintf(&TestBuff[6][1],"Receiving...");
            TestBuff[6][0] = 1;
            SocketStatus = 2;

            i51AdeOsLog(0," RecFirstFlag is 0");

            strS = strstr((iCHAR *)buffer,"Content-length:");
            if(NULL==strS)
            {
                strS = strstr((iCHAR *)buffer,"Content-Length:");
                if(NULL==strS)
                {
                    i51AdeOsLog(0,"return 1");
                    TestBuff[8][0]=2;
                    memset(TestBuff[7],0,sizeof(TestBuff[7]));
                    sprintf(TestBuff[7],"Receive FAILED");
                    RecFirstFlag = 0;
                    i51AdeSocClose(SocId);
                    i51AdeSocCloseLinkLayer(SimplePortalId,layerid);
                    i51AdeSocReleaseLinkLayer(layerid);
                    return;
                }
            }
            //i51AdeOsLog(0," strS is 1");

            strE = strstr(strS,"\r\n");
            if(NULL==strE)
            {
                i51AdeOsLog(0,"return 2");
                TestBuff[8][0]=2;
                memset(TestBuff[7],0,sizeof(TestBuff[7]));
                sprintf(TestBuff[7],"Receive FAILED");
                RecFirstFlag = 0;
                i51AdeSocClose(SocId);
                i51AdeSocCloseLinkLayer(SimplePortalId,layerid);
                i51AdeSocReleaseLinkLayer(layerid);
                return;
            }
            //i51AdeOsLog(0," strE is 1");

            strS = strS+15;
            memcpy(datalen,strS,strlen(strS)-strlen(strE));
            RecAllLen = StrToInt(datalen);
            i51AdeOsLog(0,"Get RecAllLen:%d",RecAllLen);

            for(i = 0;i<textlen;i++)
            {
                if(buffer[i]=='\r'&&buffer[i+1]=='\n'&&buffer[i+2]=='\r'&&buffer[i+3]=='\n')
                {
                    break;
                }
            }
            if(i==0)
            {
                TestBuff[8][0]=2;
                memset(TestBuff[7],0,sizeof(TestBuff[7]));
                sprintf(TestBuff[7],"Receive FAILED");
                RecFirstFlag = 0;
                i51AdeSocClose(SocId);
                i51AdeSocCloseLinkLayer(SimplePortalId,layerid);
                i51AdeSocReleaseLinkLayer(layerid);
                RecAllLen = 0;
                return;
            }

            strS = (iCHAR *)(buffer + i + 4);
            len = textlen - i-4;
            pData = strS;
        }

        RecedLen = RecedLen + len;

        file = i51AdeOsFileOpen ( L"i51Debuger\\i51PhoneTestFile\\recd.jpg" , i51_OS_FL_ALWAYS_OPEN|i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
        if(file!=0)
        {
            i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_TAIL ) ;
            i51AdeOsFileWrite ( file , pData, len , 0 ) ;
            i51AdeOsFileClose(file);
        }
        if(RecedLen>=RecAllLen)
        {
            RecFirstFlag = 0;
            i51AdeSocClose(SocId);
            i51AdeSocCloseLinkLayer(SimplePortalId,layerid);
            i51AdeSocReleaseLinkLayer(layerid);
            i51AdeOsLog(0,"Download succeed...");
            memset(TestBuff[7],0,sizeof(TestBuff[7]));
            sprintf(TestBuff[7],"Receive Completed");
            TestBuff[8][0]=2;
        }
    }   
    else
    {
        sprintf(&TestBuff[6][1],"i51AdeSocReceive");
        TestBuff[6][0] = 2;
        i51AdeSocClose(SocId);
        i51AdeSocCloseLinkLayer(SimplePortalId,layerid);
        i51AdeSocReleaseLinkLayer(layerid);
        TestBuff[8][0]=2;
    }
}

static void SocketSendCallback( iBOOL state , iS32 socket , iU32 textlen )
{
    memset(TestBuff[5],0,sizeof(TestBuff[5]));
    sprintf(&TestBuff[5][1],"i51AdeSocSend");
    if(TimeOutId!=0)
    {
        i51AdeOsTimerStop(TimeOutId);
        TimeOutId = 0;
        reTimes = 0;
    }
    if(state)
    {
        TestBuff[5][0] = 1;
        i51AdeOsLog(0,"send...");

        reTimes = 0;
        OutTimeStatus = 2;
        TimeOutId = i51AdeOsTimerCreate(1,1);
        i51AdeOsTimerStart(TimeOutId,OutTimerLinker,30000);
        i51AdeSocReceive( SocId ,SockRecBuff,2048,ReceiveCallback) ;
    }
    else
    {
        TestBuff[5][0] = 2;
        TotalError++;
        i51AdeSocClose(SocId);
        i51AdeSocCloseLinkLayer(SimplePortalId,layerid);
        i51AdeSocReleaseLinkLayer(layerid);
        SocketStatus = 1;
        TestBuff[8][0]=2;
    }
}


static void SockitConnectCallback( iBOOL state , iS32 socket )
{
    if(TimeOutId!=0)
    {
        i51AdeOsTimerStop(TimeOutId);
        TimeOutId = 0;
        reTimes = 0;
    }
    memset(TestBuff[4],0,sizeof(TestBuff[4]));
    sprintf(&TestBuff[4][1],"i51AdeSocConnect");
    if(state)
    {
        TestBuff[4][0] = 1;
        i51AdeOsLog(0,"Connet succeed:%d",strlen(SocketHead));

        reTimes = 0;
        OutTimeStatus = 1;
        TimeOutId = i51AdeOsTimerCreate(1,1);
        i51AdeOsTimerStart(TimeOutId,OutTimerLinker,30000);
        i51AdeSocSend(SocId ,(void*)SocketHead ,strlen(SocketHead), SocketSendCallback );
    }
    else
    {
        TestBuff[4][0] = 2;
        TotalError++;
        i51AdeSocClose(SocId);
        i51AdeSocReleaseLinkLayer(layerid);
        SocketStatus = 1;
        TestBuff[8][0]=2;
    }
}


static void LinkerCallback( iBOOL state )
{
    memset(TestBuff[2],0,sizeof(TestBuff[2]));
    memset(TestBuff[3],0,sizeof(TestBuff[3]));
    sprintf(&TestBuff[2][1],"i51AdeSocOpenLinkLayer");
    if(state)
    {
        TestBuff[2][0] = 1;
        SocId = i51AdeSocGet();
        sprintf(&TestBuff[3][1],"i51AdeSocGet");
        if(SocId<0||SocId>4)
        {
            i51AdeOsLog(0,"i51AdeSocGet FALSE");
            TestBuff[3][0] = 2;
            TotalError++;
            SocketStatus = 1;
            TestBuff[8][0]=2;
            return;

        }
        TestBuff[3][0] = 1;
        reTimes = 0;
        OutTimeStatus = 0;
        TimeOutId = i51AdeOsTimerCreate(1,1);
        i51AdeOsTimerStart(TimeOutId,OutTimerLinker,30000);
        i51AdeSocConnect(SocId, &address , SockitConnectCallback);
    }
    else
    {
        TestBuff[2][0] = 2;
        TotalError++;
        i51AdeSocCloseLinkLayer(SimplePortalId,layerid);
        SocketStatus = 1;
        TestBuff[8][0]=2;
    }
}
static void OutTimerLinker(iU32 ID)
{
    if(reTimes==3)
    {
        if(OutTimeStatus==0)
        {
            memset(TestBuff[4],0,sizeof(TestBuff[4]));
            sprintf(&TestBuff[4][1],"i51AdeSocConnect");
            TestBuff[4][0] = 2;
        }
        else if(OutTimeStatus==1)
        {
            memset(TestBuff[5],0,sizeof(TestBuff[5]));
            sprintf(&TestBuff[5][1],"i51AdeSocSend");
            TestBuff[5][0] = 2;
        }
        else if(OutTimeStatus==2)
        {
            memset(TestBuff[6],0,sizeof(TestBuff[6]));
            sprintf(&TestBuff[6][1],"i51AdeSocReceive");
            TestBuff[6][0] = 2;
        }

        reTimes = 0;
        i51AdeSocClose(SocId);
        i51AdeSocReleaseLinkLayer(layerid);
        i51AdeOsLog(0,"i51AdeSocConnect TimeOut");

        i51AdeOsTimerStop(ID);
        TimeOutId = 0;
    }
    else
    {
        reTimes++;
        if(OutTimeStatus==0)
        {
            i51AdeSocConnect(SocId, &address , SockitConnectCallback);
        }
        else if(OutTimeStatus==1)
        {
            i51AdeSocSend(SocId ,(void*)SocketHead ,strlen(SocketHead), SocketSendCallback );
        }
        else if(OutTimeStatus==2)
        {
            i51AdeSocReceive( SocId ,SockRecBuff,2048,ReceiveCallback) ;
        }

    }  
}
iBOOL DoSocket()
{
    iS32 *LinkMode = NULL ;
    iS32 *sims = NULL ;    
    iCHAR *ss="http://hz.51pk.com/Lighthouse.jpg";
    TREPORT report = {0};
    iCHAR* point = NULL;
    static iCHAR *pp ="120.197.129.19";
    iU32 net = 0;

    LinkMode =  i51AdeSocGetLinkLayerMode () ;
    sims = i51AdeSmsGetSims () ;

    if(sims[0]==0)return iFALSE;

    point = i51AdeSmsGetCenter (sims[1]);
    if(point!=NULL)
    {
        memcpy(report.Sim.IMSC,point,strlen(point));
        point = NULL;
    }
    point = i51AdeSmsGetImsi(sims[1]);
    if(point!=NULL)
    {
        memcpy(report.Sim.IMSI,point,strlen(point));
        point = NULL;
    }
    i51UpdataReport(1,report);
    layerid = i51AdeSocCreateLinkLayer( LinkMode[2] , sims[1]) ;
    memset(SocketHead,0,sizeof(SocketHead));
    sprintf(SocketHead,i51HTTP_PROXY_HEADER_PATTERN,0);

    memset(TestBuff[0],0,sizeof(TestBuff[0]));
    memset(TestBuff[1],0,sizeof(TestBuff[1]));
    sprintf(&TestBuff[0][1],"i51AdeSocCreateLinkLayer");
    if(!layerid)
    {
        TestBuff[0][0] = 2;
        i51AdeOsLog(0,"i51AdeSocCreateLinkLayer FALSE");
        TotalError++;
        SocketStatus = 1;
        TestBuff[8][0]=2;
        return iFALSE;
    }
    TestBuff[0][0] = 1;
    i51AdeSocInetAton(pp,&net);
    address.port = 80;
    address.ipaddr = net;
    sprintf(&TestBuff[1][1],"i51AdeSocInetAton");
    if(address.ipaddr==0)
    {
        TestBuff[1][0] = 2;
        i51AdeSocReleaseLinkLayer(layerid);
        TestBuff[8][0]=2;
        return iFALSE;

    }
    else
    {
        TestBuff[1][0] = 1;
    }

    i51AdeOsLog(0,"address.port:%d,address.port:%d",address.port,address.ipaddr);
    i51AdeSocOpenLinkLayer(SimplePortalId,layerid, LinkerCallback);
    return iTRUE;
}

static void MediaCallback(void* handle)
{
    return;
}

static void  MediaRecordCallback( void* stream , iU32 length )
{
    static iU32 total = 0;
    iU32 ReadLen = 0;
    iFILE file = 0;

    if (0 == total)
    {//first of all, write WavHead
        total = 44;
        memcpy(&pSoundDataStdRecordPcm[0], WavHead, 44);
        CurSoundDataLength = 44;
    }

    if(length>0)
    {
    #if 0 //不要在回调里面写文件
        file = i51AdeOsFileOpen (SoundDataStdRecordPcmPath, i51_OS_FL_WRITE | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
        if(file!=0)
        {
            i51AdeOsFileSetSeek ( file,0,i51_OS_FL_SEEK_TAIL );
            i51AdeOsFileWrite(file , stream ,length, &ReadLen );
            i51AdeOsFileClose(file);
            i51AdeOsLog(0,"Record Callback len: %d, total: %d, -> %d", length, total, total + length);
        }
        i51AdeOsLog(0,"Record Callback end: %d",length);
#endif
        if(total < SoundDataStdRecordPcmLength)
        {
            if(total + length  >= SoundDataStdRecordPcmLength)
            {
                memcpy(&pSoundDataStdRecordPcm[total],stream,SoundDataStdRecordPcmLength - total);
                CurSoundDataLength = SoundDataStdRecordPcmLength;
//                i51AdeOsLog(0,"Record Data is Full, append %d", SoundDataStdRecordPcmLength - total);
            }
            else
            {
                memcpy(&pSoundDataStdRecordPcm[total],stream,length);
                CurSoundDataLength = total + length;
//                i51AdeOsLog(0,"Record Data append %d", length);
            }
        }
        else
        {
//            i51AdeOsLog(0,"Record Data is Full");
        }

        total += length;
    }

    return;
}

static void RecordCallbackNull(iBOOL reslut)
{
    i51AdeOsLog(0,"Record Callback, rs : %d", reslut);
}

static void RecordStopCallback(iBOOL reslut)
{
    i51AdeOsLog(0,"Record Stop Callback, rs : %d", reslut);
}

static void RecordPlayCallback(iBOOL reslut)
{
    i51AdeOsLog(0,"Record Play Callback, rs : %d", reslut);
}

#if 0
static void MediaCreateCallback(iBOOL result)
{
    if(result>0)
    {
        switch(PlayType)
        {
        case 1://play with memory
            {
                if(i51AdeMmiMediaPlayerPlay( 0 , (void*)MediaData , 19528 ,1, i51_MMI_DEC_MEM_BASED , 1 , MediaCallback ))
                {
                    Handle =(void *)MediaData;
                    TestBuff0[31] = 1;
                    sprintf(TestBuff0,"YES");
                    MMIStatus = 2;
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = 0xffff;
                    InterWordColor[4] = 0xffff;
                    InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
                else
                {
                    TestBuff0[31] = 0;
                    sprintf(TestBuff0,"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    InterWordColor[0] = i51KitG2COLOR(0,0,0);
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        case 2://play with file
            {
                if(i51AdeMmiMediaPlayerPlay( 0 , (void*)MediaPath , (iU32)strlen((iCHAR *)MediaPath) ,1, i51_MMI_DEC_FILE_BASED , 1 , MediaCallback ))
                {
                    Handle =(void *)MediaPath;
                    TestBuff1[31] = 1;
                    sprintf(TestBuff1,"YES");
                    i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay succeed");
                    MMIStatus = 2;
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = 0xffff;
                    InterWordColor[4] = 0xffff;
                    InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
                else
                {
                    TestBuff1[31] = 0;
                    sprintf(TestBuff1,"NO");
                    TotalError++;
                    i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay falsed");
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = i51KitG2COLOR(0,0,0);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        case 3://record pcm start /stop
            {
                MediaRecordBUFF = (iCHAR *)i51AdeOsMalloc ( 1024*8, __FILE__ , __LINE__ ) ;
                if(NULL==MediaRecordBUFF)
                {
                    sprintf(TestBuff5,"NO");
                    i51AdeOsLog(0,"Record start malloc error");
                    return;
                }
                MediaRecordData = (iCHAR *)i51AdeOsMalloc ( 1024*4, __FILE__ , __LINE__ ) ;
                if(NULL==MediaRecordData)
                {
                    sprintf(TestBuff5,"NO");
                    i51AdeOsLog(0,"Record data malloc error");
                    return;
                }
                if(i51AdeMmiRecorderStart(MediaRecordBUFF ,1024*8,MediaRecordCallback)>=0)
                { 
                    TestBuff5[31] = 1;
                    sprintf(TestBuff5,"YES");
                    MMIStatus = 2;
                    InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
                else
                {
                    TestBuff5[31] = 0;
                    sprintf(TestBuff5,"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = i51KitG2COLOR(0,0,0);
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        case 4://record pcm play start / stop
            {
                if(i51AdeMmiRecoderPlay( MediaRecordData , 4096 , RecordPlayCallback ) )
                {
                    TestBuff6[31] = 1;
                    sprintf(TestBuff6,"YES");
                    MMIStatus = 2;
                    InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[6] = 0xffff;
                    InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
                else
                {
                    TestBuff6[31] = 0;
                    sprintf(TestBuff6,"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = i51KitG2COLOR(0,0,0);
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        case 5://record amr start
            {
                TestBuff7[31] = 1;
                sprintf(TestBuff7,"YES");
                i51AdeOsLog(0,"Record Amr S ok");
                MMIStatus = 2;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = 0xffff;
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
            break;
        case 6://record amr play start / stop
            {
                PaintStatus = 2;
                if(i51AdeMmiMediaPlayerPlay( 0 , (void*)AmrPath, (iU32)strlen((iCHAR *)AmrPath) ,2, i51_MMI_DEC_FILE_BASED , 1 , MediaCallback ))
                {
                    TestBuff8[31] = 1;
                    sprintf(TestBuff8,"YES");
                    i51AdeOsLog(0,"------------->record amr play start succeed");
                    MMIStatus = 2;
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[8] = 0xffff;
                }
                else
                {
                    PaintStatus = 1;
                    TestBuff8[31] = 0;
                    sprintf(TestBuff8,"NO");
                    TotalError++;
                    i51AdeOsLog(0,"record amr play start falsed");
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[8] = i51KitG2COLOR(0,0,0);
                }
            }
            break;
        }
    }
}
#endif

static void MediaCreateCallback(iBOOL result)
{
    if(result>0)
    {
        switch(PlayType)
        {
        case 0://play with memory
            {
                if(i51AdeMmiMediaPlayerPlay( 0 , (void*)pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType, 1 , MediaCallback ))
                {
                    Handle =(void *)pCurSoundData;
                    TestBuff[0][31] = 1;
                    sprintf(TestBuff[0],"YES");
                    MMIStatus = 2;
                    *pCurStatus = 2;
                    PaintStatus = 2;
                    i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay ok: %x %d %d %d", pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType);
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = 0xffff;
                    InterWordColor[4] = 0xffff;
                }
                else
                {
                    TestBuff[0][31] = 0;
                    sprintf(TestBuff[0],"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    *pCurStatus = 1;
                    PaintStatus = 1;
                    i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay fail: %x %d %d %d", pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType);
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        case 1://play with file
            {
                if(i51AdeMmiMediaPlayerPlay( 0 , (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath) ,CurSoundFormatType, CurSoundDataType , 1 , MediaCallback ))
                {
                    Handle =(void *)pCurSoundDataPath;
                    TestBuff[1][31] = 1;
                    sprintf(TestBuff[1],"YES");
                    MMIStatus = 2;
                    *pCurStatus = 2;
                    PaintStatus = 2;
                    i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay ok: %x %d %d %d", (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath), CurSoundFormatType, CurSoundDataType);
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = 0xffff;
                    InterWordColor[4] = 0xffff;
                }
                else
                {
                    TestBuff[1][31] = 0;
                    sprintf(TestBuff[1],"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    *pCurStatus = 1;
                    PaintStatus = 1;
                    i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay fail: %x %d %d %d", (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath), CurSoundFormatType, CurSoundDataType);
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        case 2://Record Pcm Memory Start
            {
                if(NULL == pSoundDataStdRecordPcm)
                {
                    pSoundDataStdRecordPcm = (iU8 *)i51AdeOsMalloc ( SoundDataStdRecordPcmLength, __FILE__ , __LINE__ ) ;
                    if(NULL == pSoundDataStdRecordPcm)
                    {
                        sprintf(TestBuff[0],"NO");
                        i51AdeOsLog(0,"Record data malloc error");
                        return;
                    }
                    else
                    {
                        i51AdeOsLog(0,"malloc pSoundDataStdRecordPcm memory");
                    }
                    pCurSoundData = pSoundDataStdRecordPcm;
                    CurSoundDataLength = 0;
                }
                if(NULL == SoundBuffer)
                {
                    SoundBuffer = (iU8 *)i51AdeOsMalloc ( 1024*8, __FILE__ , __LINE__ ) ;
                    if(NULL == SoundBuffer)
                    {
                        sprintf(TestBuff[0],"NO");
                        i51AdeOsLog(0,"Record buffer malloc error");
                        return;
                    }
                    else
                    {
                        i51AdeOsLog(0,"malloc SoundBuffer memory");
                    }
                }
                if(i51AdeMmiRecorderStart(SoundBuffer ,1024*8,MediaRecordCallback)>=0)
                { 
                    TestBuff[0][31] = 1;
                    sprintf(TestBuff[0],"YES");
                    MMIStatus = 2;
                    *pCurStatus = 2;
                    PaintStatus = 2;
                    i51AdeOsLog(0,"i51AdeMmiRecorderStart ok: %x %d %x", SoundBuffer, 1024*8, MediaRecordCallback);
                    InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                }
                else
                {
                    TestBuff[0][31] = 0;
                    sprintf(TestBuff[0],"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    *pCurStatus = 1;
                    PaintStatus = 1;
                    i51AdeOsLog(0,"i51AdeMmiRecorderStart fail: %x %d %x", SoundBuffer, 1024*8, MediaRecordCallback);
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        case 3://Record Amr File Start
            {
                if(i51AdeMmiRecorderStartEx((iU16 *)pCurSoundDataPath, RecordCallbackNull))
                {
                    TestBuff[0][31] = 1;
                    sprintf(TestBuff[0],"YES");
                    MMIStatus = 2;
                    *pCurStatus = 2;
                    PaintStatus = 2;
                    i51AdeOsLog(0,"i51AdeMmiRecorderStartEx ok: %x %x", pCurSoundDataPath, RecordCallbackNull);
                    InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                }
                else
                {
                    TestBuff[0][31] = 0;
                    sprintf(TestBuff[0],"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    *pCurStatus = 1;
                    PaintStatus = 1;
                    i51AdeOsLog(0,"i51AdeMmiRecorderStartEx fail: %x %x", pCurSoundDataPath, RecordCallbackNull);
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        case 4://recorder play start Pcm Memory
            {
                if (i51_ADE_CNF_VER_CS == i51AdeCnfGetType())
                {
                    if(i51AdeMmiMediaPlayerPlay( 0 , (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath) ,CurSoundFormatType, i51_MMI_DEC_FILE_BASED , 1 , MediaCallback ))
                    {
                        Handle =(void *)pCurSoundDataPath;
                        TestBuff[2][31] = 1;
                        sprintf(TestBuff[2],"YES");
                        MMIStatus = 2;
                        *pCurStatus = 2;
                        PaintStatus = 2;
                        i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory cs ok: %x %d %d %d", (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath), CurSoundFormatType, i51_MMI_DEC_FILE_BASED);
                        InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[2] = 0xffff;
                        InterWordColor[3] = 0xffff;
                    }
                    else
                    {
                        TestBuff[2][31] = 0;
                        sprintf(TestBuff[2],"NO");
                        TotalError++;
                        i51AdeMmiMediaPlayerRelease();
                        MMIStatus = 1;
                        *pCurStatus = 1;
                        PaintStatus = 1;
                        i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory cs fail: %x %d %d %d", (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath), CurSoundFormatType, i51_MMI_DEC_FILE_BASED);
                        InterWordColor[0] = 0xffff;
                        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[2] = 0xffff;
                        InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    }
                }
                else
                {
                    if(i51AdeMmiMediaPlayerPlay( 0 , (void*)pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType, 1 , MediaCallback ))
                    {
                        Handle =(void *)pCurSoundData;
                        TestBuff[2][31] = 1;
                        sprintf(TestBuff[2],"YES");
                        MMIStatus = 2;
                        *pCurStatus = 2;
                        PaintStatus = 2;
                        i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory ok: %x %d %d %d", pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType);
                        InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[2] = 0xffff;
                        InterWordColor[3] = 0xffff;
                    }
                    else
                    {
                        TestBuff[2][31] = 0;
                        sprintf(TestBuff[2],"NO");
                        TotalError++;
                        i51AdeMmiMediaPlayerRelease();
                        MMIStatus = 1;
                        *pCurStatus = 1;
                        PaintStatus = 1;
                        i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory fail: %x %d %d %d", pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType);
                        InterWordColor[0] = 0xffff;
                        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[2] = 0xffff;
                        InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    }
                }
#if 0
                if (i51_ADE_CNF_VER_CS == i51AdeCnfGetType())
                {
                    if(i51AdeMmiMediaPlayerPlay( 0 , (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath) ,CurSoundFormatType, i51_MMI_DEC_FILE_BASED , 1 , MediaCallback ))
                    {
                        Handle =(void *)pCurSoundDataPath;
                        TestBuff[2][31] = 1;
                        sprintf(TestBuff[2],"YES");
                        MMIStatus = 2;
                        *pCurStatus = 2;
                        PaintStatus = 2;
                        i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory cs ok: %x %d %d %d", (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath), CurSoundFormatType, i51_MMI_DEC_FILE_BASED);
                        InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[2] = 0xffff;
                        InterWordColor[3] = 0xffff;
                    }
                    else
                    {
                        TestBuff[2][31] = 0;
                        sprintf(TestBuff[2],"NO");
                        TotalError++;
                        i51AdeMmiMediaPlayerRelease();
                        MMIStatus = 1;
                        *pCurStatus = 1;
                        PaintStatus = 1;
                        i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory cs fail: %x %d %d %d", (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath), CurSoundFormatType, i51_MMI_DEC_FILE_BASED);
                        InterWordColor[0] = 0xffff;
                        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[2] = 0xffff;
                        InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    }
                }
                else
                {
                    if(i51AdeMmiMediaPlayerPlay( 0 , (void*)pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType, 1 , MediaCallback ))
                    {
                        Handle =(void *)pCurSoundData;
                        TestBuff[2][31] = 1;
                        sprintf(TestBuff[2],"YES");
                        MMIStatus = 2;
                        *pCurStatus = 2;
                        PaintStatus = 2;
                        i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory ok: %x %d %d %d", pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType);
                        InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[2] = 0xffff;
                        InterWordColor[3] = 0xffff;
                    }
                    else
                    {
                        TestBuff[2][31] = 0;
                        sprintf(TestBuff[2],"NO");
                        TotalError++;
                        i51AdeMmiMediaPlayerRelease();
                        MMIStatus = 1;
                        *pCurStatus = 1;
                        PaintStatus = 1;
                        i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory fail: %x %d %d %d", pCurSoundData, CurSoundDataLength, CurSoundFormatType, CurSoundDataType);
                        InterWordColor[0] = 0xffff;
                        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                        InterWordColor[2] = 0xffff;
                        InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    }
                }
				
                if(i51AdeMmiRecoderPlay( (void *)pCurSoundData, CurSoundDataLength, RecordPlayCallback ) )
                {
                    TestBuff[2][31] = 1;
                    sprintf(TestBuff[2],"YES");
                    MMIStatus = 2;
                    i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory ok: %x %d %x", pCurSoundData, CurSoundDataLength,RecordPlayCallback);
                    InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = 0xffff;
                }
                else
                {
                    TestBuff[2][31] = 0;
                    sprintf(TestBuff[2],"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    i51AdeOsLog(0,"i51Debuger recorder_play start Pcm Memory fail: %x %d %x", pCurSoundData, CurSoundDataLength,RecordPlayCallback);
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                }
#endif
            }
            break;
        case 5://recorder play start Amr File
            {
                if(i51AdeMmiMediaPlayerPlay( 0 , (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath) ,CurSoundFormatType, CurSoundDataType , 1 , MediaCallback ))
                {
                    Handle =(void *)pCurSoundDataPath;
                    TestBuff[2][31] = 1;
                    sprintf(TestBuff[2],"YES");
                    MMIStatus = 2;
                    *pCurStatus = 2;
                    PaintStatus = 2;
                    i51AdeOsLog(0,"i51Debuger recorder play start Amr File ok: %x %d %d %d", (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath), CurSoundFormatType, CurSoundDataType);
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = 0xffff;
                }
                else
                {
                    TestBuff[2][31] = 0;
                    sprintf(TestBuff[2],"NO");
                    TotalError++;
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 1;
                    *pCurStatus = 1;
                    PaintStatus = 1;
                    i51AdeOsLog(0,"i51Debuger recorder play start Amr File fail: %x %d %d %d", (void*)pCurSoundDataPath, (iU32)strlen((iCHAR *)pCurSoundDataPath), CurSoundFormatType, CurSoundDataType);
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                }
            }
            break;
        }
    }
}

void i51InputCheckDo(iU8 para)
{
    i51STDEDITOR editor = {0};
    InterSetFous(para+1);
    if(para==0)
    {
        memset(EditorBuff,0,sizeof(EditorBuff));
        editor.type = i51_EDITOR_TEXT;
        editor.buffer = (void *)EditorBuff;
        editor.length = 64;
        editor.callback = EditorCallback_Input0;
        i51AdeStdEntryEditor ( &editor );
        i51AdeMmiUpdateScreen();
    }
    else if(para==1)
    {

        memset(EditorBuff,0,sizeof(EditorBuff));
        editor.type = i51_EDITOR_NUMBER;
        editor.buffer = (void *)EditorBuff;
        editor.length = 64;
        editor.callback = EditorCallback_Input1;
        i51AdeStdEntryEditor ( &editor );
        i51AdeMmiUpdateScreen();
    }
    else if(para==2)
    {
        memset(EditorBuff,0,sizeof(EditorBuff));
        editor.type = i51_EDITOR_TEXT;
        editor.buffer = (void *)EditorBuff;
        editor.length = 64;
        editor.callback = EditorCallback_Input2;
        i51AdeStdEntryEditor ( &editor );
        i51AdeMmiUpdateScreen();
    }
}

void i51InterInputTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown_input[5] = {0};    

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightHead = height/10;
    HeightEnd = height/12;
    HeightPerOne = (height -HeightHead -HeightEnd)/9;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;


    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event )
            {
                if(IsDown_input[3])
                {
                    IsDown_input[3] = iFALSE;
                    Runstatus = 4;
                    InterSetFous(1);
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_input[3] = iTRUE;
            }
        }
        else 
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_input[4])
                {
                    IsDown_input[4] = iFALSE;

                    i51InputCheckDo(InterFoucs-1);
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_input[4] = iTRUE;
            }
        }
        return;
    }
    for(i = 0;i<3;i++)
    {
        if ( x > 0  && x <width &&y >=HeightHead+HeightPerOne*i &&y < HeightHead+HeightPerOne*(i+1))
        {
            if(i51_MSG_PADUP == event &&IsDown_input[i])
            {
                IsDown_input[i] = iFALSE;
                i51InputCheckDo(i);

            }
            else if(i51_MSG_PADDOWN == event)
            {
                IsDown_input[i] = iTRUE;
            }
        }
    }
}

void i51SMSDO(iU8 para)
{
    i51STDEDITOR editor = {0};

    if(para==0)
    {
        if(sendOnce != 0)
        {
            return;
        }
        memset(EditorBuff,0,sizeof(EditorBuff));
        editor.type = i51_EDITOR_NUMBER;
        editor.buffer = (void *)EditorBuff;
        editor.length = 64;
        editor.callback = EditorCallback_Sim0;
        i51AdeStdEntryEditor ( &editor );
        i51AdeMmiUpdateScreen();;
    }
    else if(para==2)
    {
        if(sendOnce != 0)
        {
            return;
        }
        memset(TestBuff[4],0,sizeof(TestBuff[4]));
        if(strlen(TestBuff[9])==0)
        {
            sprintf(TestBuff[2],"PoneNumber is NULL...");
            return;
        }
        InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
        InterColor0[1]=i51KitG2COLOR(7, 24, 40);
        InterColor1[1]=i51KitG2COLOR(7, 24, 40);
        InterFoucs = 10;
        if(0==SMSMutualTest(TestBuff[9] ,SendCBuff))
        {
            sprintf(TestBuff[2],"SMS send TRUE,Waiting...");
        }
        else
        {
            sprintf(TestBuff[2],"SMS send  FALSE....");
            TotalError++;
            SMSStatus = 1;
        }
    }
    else if(para==5)
    {
        if(1!=sendOnce)
        {
            return;
        }
        if(strlen(TestBuff[9])==0)
        {
            sprintf(TestBuff[5],"PoneNumber is NULL...");
            return;
        }
        InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
        InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
        InterColor0[1]=i51KitG2COLOR(7, 24, 40);
        InterColor1[1]=i51KitG2COLOR(7, 24, 40);
        InterColor0[2]=i51KitG2COLOR(7, 24, 40);
        InterColor1[2]=i51KitG2COLOR(7, 24, 40);
        InterColor0[0]=i51KitG2COLOR(7, 24, 40);
        InterColor1[0]=i51KitG2COLOR(7, 24, 40);
        InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
        InterFoucs = 10;
        if(0==SMSMutualTest(TestBuff[9] ,L"SMS Test"))
        {
            sprintf(TestBuff[5],"SMS send TRUE,Waiting...");
        }
        else
        {
            sprintf(TestBuff[5],"SMS send  FALSE....");
            TotalError++;
            SMSStatus = 1;
        }
    }
}
void SMSDoOk()
{
    if(!SMSReset)
    {
        if(InterFoucs==1)
        {
            i51SMSDO(0);
        }
        else if(InterFoucs==2)
        {
            i51SMSDO(2);
        }
        else if(InterFoucs==3)
        {
            i51SMSDO(5);
        }
    }
    else
    {
        sendOnce = 0;
        InterSetFous(1);
        memset(TestBuff[0] ,0,sizeof(TestBuff[0]));
        memset(TestBuff[1],0,sizeof(TestBuff[1]));
        memset(TestBuff[2],0,sizeof(TestBuff[2]));
        memset(TestBuff[3] ,0,sizeof(TestBuff[3]));
        memset(TestBuff[4] ,0,sizeof(TestBuff[4]));
        memset(TestBuff[5],0,sizeof(TestBuff[5]));
        memset(TestBuff[6],0,sizeof(TestBuff[6]));
        memset(TestBuff[7],0,sizeof(TestBuff[7]));
        memset(TestBuff[8],0,sizeof(TestBuff[8]));
        memset(TestBuff[9],0,sizeof(TestBuff[9]));
        SMSReset = 0;
    }
}
void i51InterSMSTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown_SMS[8] = {0};

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightPerOne = 24;
    HeightHead = height/10;
    HeightEnd = height/12;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_SMS[6])
                {
                    IsDown_SMS[6] = iFALSE;
                    Runstatus = 4;
                    InterSetFous(2);
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_SMS[6] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_SMS[7])
                {
                    SMSDoOk();
                    IsDown_SMS[7] = iFALSE;
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_SMS[7] = iTRUE;
            }
        }
        return;
    }
    for(i = 0;i<6;i++)
    { 
        if ( x > 0  && x <width &&y >=height/10+24*i &&y < height/10+24*(i+1))
        {
            if(i51_MSG_PADUP == event&&IsDown_SMS[i] )
            {
                IsDown_SMS[i]  = iFALSE;
                i51SMSDO(i);
            } 
            else if(i51_MSG_PADDOWN == event)
            {
                IsDown_SMS[i]  = iTRUE;
            }
        }
    }
}

void i51InterSocketTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    static iBOOL IsDown_Socket[3] = {0};

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightPerOne = 24;
    HeightHead = height/10;
    HeightEnd = height/12;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;


    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                CloseColor = 0xffff;
                if(IsDown_Socket[1])
                {
                    IsDown_Socket[1] = iFALSE;

                    Runstatus = 4;
                    InterSetFous(3);

                    RecedLen = 0;
                    RecAllLen = 0;
                    i51AdeSocClose(SocId);
                    i51AdeSocCloseLinkLayer(SimplePortalId,layerid);
                    i51AdeSocReleaseLinkLayer(layerid);
                }
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_Socket[1] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                OkColor = 0xffff;
                if(IsDown_Socket[2])
                {
                    IsDown_Socket[2] = iFALSE;
                    InterFoucs = 10;
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterColor0[0]=i51KitG2COLOR(7, 24, 40);
                    InterColor1[0]=i51KitG2COLOR(7, 24, 40);
                    if(TestBuff[8][0]==0)
                    {
                        DoSocket();
                        TestBuff[8][0]=1;
                    }
                    else if(TestBuff[8][0]==0)
                    {
                        RecedLen = 0;
                        RecAllLen = 0;
                        RecFirstFlag = 0;
                        memset(TestBuff[0],0,sizeof(TestBuff[0]));
                        memset(TestBuff[1],0,sizeof(TestBuff[0]));
                        memset(TestBuff[2],0,sizeof(TestBuff[0]));
                        memset(TestBuff[3],0,sizeof(TestBuff[0]));
                        memset(TestBuff[4],0,sizeof(TestBuff[0]));
                        memset(TestBuff[5],0,sizeof(TestBuff[0]));
                        memset(TestBuff[6],0,sizeof(TestBuff[0]));
                        memset(TestBuff[7],0,sizeof(TestBuff[0]));
                        memset(TestBuff[8],0,sizeof(TestBuff[0]));
                        memset(TestBuff[9],0,sizeof(TestBuff[0]));
                        InterSetFous(1);
                    }
                }

            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_Socket[2] = iTRUE;
            }
        }
        return;
    }

    if ( x > 0  && x <width &&y >=height/12&&y < height/12+24 )
    {
        if(i51_MSG_PADUP == event&&IsDown_Socket[0])
        {
            IsDown_Socket[0] = iFALSE;
            if(TestBuff[8][0]==0)
            {
                InterFoucs = 10;
                InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                InterColor0[0]=i51KitG2COLOR(7, 24, 40);
                InterColor1[0]=i51KitG2COLOR(7, 24, 40);
                DoSocket();
                TestBuff[8][0]=1;
            }
        }
        else if(i51_MSG_PADDOWN == event)
        {
            IsDown_Socket[0] = iTRUE;
        }
    }
}

#if 1
void i51MmiSubPlayerDO(iU8 para)
{
    switch(para)
    {
    case 0://play with memory
        {
            if(InterWordColor[0]!=0xffff&&InterWordColor[0]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            i51AdeOsLog(0,"i51Debuger sound player running Type Memory");
            InterSetFous(1);
            memset(TestBuff[0],0,sizeof(TestBuff[0]));
            CurSoundDataType = i51_MMI_DEC_MEM_BASED;

            PlayType = 0;
            i51AdeMmiMediaPlayerCreate(MediaCreateCallback);                     
        }
        break;
    case 1://play with file
        {
            iFILE file = 0;
            iU32 ReadLen = 0; 
            if(InterWordColor[1]!=0xffff&&InterWordColor[1]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            i51AdeOsLog(0,"i51Debuger sound player running Type File");
            InterSetFous(2);
            memset(TestBuff[1],0,sizeof(TestBuff[1]));
            CurSoundDataType = i51_MMI_DEC_FILE_BASED;

            if (!i51AdeOsFileIsExist (pCurSoundDataPath))
            {
                iFILE file = 0;
                iS32 fileSize = 0;

                file = i51AdeOsFileOpen (pCurSoundDataPath , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
                if(file == 0)
                {
                    i51AdeOsLog ( i51_LOG_DC , "i51Debuger sound write file data is cannot open." );
                }
                i51AdeOsFileSetSeek ( file,0,i51_OS_FL_SEEK_HEAD);
                i51AdeOsFileWrite(file, (void *)pCurSoundData , CurSoundDataLength, &ReadLen) ;
                i51AdeOsFileClose(file);
            }

            PlayType = 1;
            i51AdeMmiMediaPlayerCreate(MediaCreateCallback);
        }
        break;

    case 2://stop
        {
            if(InterWordColor[2]!=0xffff&&InterWordColor[2]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            memset(TestBuff[2],0,sizeof(TestBuff[2]));
            if(NULL==Handle)
            {
                TestBuff[2][31] = 1;
                sprintf(TestBuff[2],"YES");
                MMIStatus = 2;
                *pCurStatus = 2;
                PaintStatus = 1;
                return;
            }
            InterSetFous(3);

            if(i51AdeMmiMediaPlayerStop(Handle))
            {
                Handle = NULL;
                TestBuff[2][31] = 1;
                sprintf(TestBuff[2],"YES");
                i51AdeMmiMediaPlayerRelease();
                MMIStatus = 2;
                *pCurStatus = 2;
                PaintStatus = 1;
                i51AdeOsLog(0,"i51Debuger sound player running Stop is ok");
                InterWordColor[0] = 0xffff;
                InterWordColor[1] = 0xffff;
                InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
            }
            else
            {
                TestBuff[2][31] = 0;
                sprintf(TestBuff[2],"NO");
                TotalError++;
                MMIStatus = 1;
                *pCurStatus = 1;
                PaintStatus = 1;
                i51AdeOsLog(0,"i51Debuger sound player running Stop is fail");
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = 0xffff;
            }
        }
        break;
    case 3://pause
        {
            if(InterWordColor[3]!=0xffff&&InterWordColor[3]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            memset(TestBuff[3],0,sizeof(TestBuff[3]));
            InterSetFous(4);

            if(i51AdeMmiMediaPlayerPause(Handle))
            {
                TestBuff[3][31] = 1;
                sprintf(TestBuff[3],"YES");
                MMIStatus = 2;
                *pCurStatus = 2;
		   PaintStatus = 2;
                i51AdeOsLog(0,"i51Debuger sound player running Pause is ok");
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = 0xffff;
            }
            else
            {
                TestBuff[3][31] = 0;
                sprintf(TestBuff[3],"NO");
                TotalError++;
                MMIStatus = 1;
                *pCurStatus = 1;
		   PaintStatus = 2;
                i51AdeOsLog(0,"i51Debuger sound player running Pause is fail");
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = 0xffff;
            }
        }
        break;
    case 4://resume
        {
            if(InterWordColor[4]!=0xffff&&InterWordColor[4]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            memset(TestBuff[4],0,sizeof(TestBuff[4]));
            InterSetFous(5);

            if(i51AdeMmiMediaPlayerResume(Handle))
            {
                TestBuff[4][31] = 1;
                sprintf(TestBuff[4],"YES");
                MMIStatus = 2;
		   PaintStatus = 2;
                i51AdeOsLog(0,"i51Debuger sound player running Resume is ok");
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = 0xffff;
            }
            else
            {
                TestBuff[4][31] = 0;
                sprintf(TestBuff[4],"NO");
                TotalError++;
                MMIStatus = 1;
		   PaintStatus = 2;
                i51AdeOsLog(0,"i51Debuger sound player running Resume is fail");
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = 0xffff;
            }
        }
        break;
    default:
        {
            i51AdeOsLog(0,"i51MmiSubDO error parameter");
        }
        break;
    }
}

#if 0
void i51MmiSubWritePcmFileHeaderAtFront(void)
{
    iFILE file = 0;

    file = i51AdeOsFileOpen (SoundDataStdRecordPcmPath, i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
    if(file == 0)
    {
        i51AdeOsLog ( i51_LOG_DC , "i51Debuger sound test Write Pcm File Header is cannot open." );
    }
    i51AdeOsFileSetSeek ( file,0,i51_OS_FL_SEEK_TAIL);
    i51AdeOsFileWrite ( file , WavHead , 44 , 0 ) ;
    i51AdeOsFileClose(file);

    return;
}
#endif

static void ChangeWavHeadBigendiantoLittle(iU8 *point)
{
    iU8 temp = 0;

#if 0
    typedef struct{
        iU8		    szRiff[4];			/* "RIFF" */
        iU32		dwFileSize;			/* file size */
        iU8		    szWaveFmt[8];		/* "WAVEfmt " */
        iU32		dwFmtSize;			/* dwFmtSize 16 */
        iU16        wFormatTag;         /* format type */
        iU16        nChannels;          /* number of channels (i.e. mono, stereo...) mono */
        iU32        nSamplesPerSec;     /* sample rate 8000 */
        iU32        nAvgBytesPerSec;    /* for buffer estimation */
        iU16        nBlockAlign;        /* block size of data */
        iU16        wBitsPerSample;     /* number of bits per sample of mono data */
    }WAVE_HEADER;

    typedef struct{
        iU8		szData[4];			/* "data" */
        iU32    dwDataSize;         /*pcm data size*/
    }WAVE_DATA_HEADER;
#endif
    /* -------------- WAVE_HEADER -------------- */
    /* "RIFF" */
    point += 4;

    /* file size */
    temp = *(point + 0);
    *(point + 0) = *(point + 3);
    *(point + 3) = temp;

    temp = *(point + 1);
    *(point + 1) = *(point + 2);
    *(point + 2) = temp;
    point += 4;

    /* "WAVEfmt " */
    point += 8;

    /* dwFmtSize 16 */
    temp = *(point + 0);
    *(point + 0) = *(point + 3);
    *(point + 3) = temp;

    temp = *(point + 1);
    *(point + 1) = *(point + 2);
    *(point + 2) = temp;
    point += 4;

    /* format type */
    temp = *(point + 0);
    *(point + 0) = *(point + 1);
    *(point + 1) = temp;
    point += 2;

    /* number of channels (i.e. mono, stereo...) mono */
    temp = *(point + 0);
    *(point + 0) = *(point + 1);
    *(point + 1) = temp;
    point += 2;

    /* sample rate 8000 */
    temp = *(point + 0);
    *(point + 0) = *(point + 3);
    *(point + 3) = temp;

    temp = *(point + 1);
    *(point + 1) = *(point + 2);
    *(point + 2) = temp;
    point += 4;

    /* for buffer estimation */
    temp = *(point + 0);
    *(point + 0) = *(point + 3);
    *(point + 3) = temp;

    temp = *(point + 1);
    *(point + 1) = *(point + 2);
    *(point + 2) = temp;
    point += 4;

    /* block size of data */
    temp = *(point + 0);
    *(point + 0) = *(point + 1);
    *(point + 1) = temp;
    point += 2;

    /* number of bits per sample of mono data */
    temp = *(point + 0);
    *(point + 0) = *(point + 1);
    *(point + 1) = temp;
    point += 2;

    /* -------------- WAVE_DATA_HEADER -------------- */
    /* "data" */
    point += 4;

    /*pcm data size*/
    temp = *(point + 0);
    *(point + 0) = *(point + 3);
    *(point + 3) = temp;

    temp = *(point + 1);
    *(point + 1) = *(point + 2);
    *(point + 2) = temp;
    point += 4;

    return;
}

void i51MmiSubWritePcmFileHeaderAtEnd(void)
{
    iFILE file = 0;
    iU32 nWritten = 0;

    //wav header
    i51AdeStdMemcpy16(wavFileHeader.szRiff,"RIFF",4);
    wavFileHeader.dwFileSize = CurSoundDataLength - 8;
    i51AdeStdMemcpy16(wavFileHeader.szWaveFmt,"WAVEfmt ",8);
    wavFileHeader.dwFmtSize = 16;
    wavFileHeader.wFormatTag = 1;
    wavFileHeader.nChannels = 1; //mono
    wavFileHeader.nSamplesPerSec = 8000;   //采样率
    wavFileHeader.wBitsPerSample = 16;//8;//16; 只能使用16
    wavFileHeader.nAvgBytesPerSec = wavFileHeader.nChannels*wavFileHeader.wBitsPerSample*wavFileHeader.nSamplesPerSec/8;
    wavFileHeader.nBlockAlign = wavFileHeader.nChannels*wavFileHeader.wBitsPerSample/8;
    //wav data header
    i51AdeStdMemcpy16(wavDataHeader.szData, "data", 4);
    wavDataHeader.dwDataSize = CurSoundDataLength - 44;

//    i51AdeOsLog ( i51_LOG_DC , "-----------> %d %d", CurSoundDataLength - 8, CurSoundDataLength - 44);

    memcpy(&pSoundDataStdRecordPcm[0], &wavFileHeader, 36);
    memcpy(&pSoundDataStdRecordPcm[36], &wavDataHeader, 8);

    if (i51_ADE_CNF_BIG_ENDIAN == i51AdeCnfGetEndian())
    {
        ChangeWavHeadBigendiantoLittle(&pSoundDataStdRecordPcm[0]);
    }
#if 0
    //将文件数据写到文件中，播放
    file = i51AdeOsFileOpen (SoundDataStdRecordPcmPath, i51_OS_FL_WRITE | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
    if(file == 0)
    {
        i51AdeOsLog ( i51_LOG_DC , "i51Debuger sound test Write Pcm File Header is cannot open." );
    }
    i51AdeOsFileSetSeek ( file,0,i51_OS_FL_SEEK_HEAD);
    i51AdeOsFileWrite(file,(void*)&pSoundDataStdRecordPcm[0],44,&nWritten); // 直接写入文件
    i51AdeOsFileClose(file);
#endif
    file = i51AdeOsFileOpen (SoundDataStdRecordPcmPath, i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
    if(file!=0)
    {
        i51AdeOsFileSetSeek ( file,0,i51_OS_FL_SEEK_HEAD );
        i51AdeOsFileWrite(file , &pSoundDataStdRecordPcm[0], CurSoundDataLength, &nWritten);
        i51AdeOsFileClose(file);
    }
    else
    {
        i51AdeOsLog ( i51_LOG_DC , "i51Debuger sound test Write Pcm File Header is cannot open." );
    }

    return;
}

void i51MmiSubRecorderDO(iU8 para)
{
    //    static unsigned int RunOnce = 0; //第一次进该测试类型界面，初始化值

    switch(para)
    {
    case 0://recorder start
        {
            if(InterWordColor[0]!=0xffff&&InterWordColor[0]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(1);
            memset(TestBuff[0],0,sizeof(TestBuff[0]));

            if (i51_MMI_DEC_MEM_BASED == CurSoundDataType)
            {
                PlayType = 2;
                //i51MmiSubWritePcmFileHeaderAtFront();
                i51AdeOsLog(0,"i51Debuger sound recorder start Pcm Memory");
            }
            if (i51_MMI_DEC_FILE_BASED == CurSoundDataType)
            {
                PlayType = 3;
                i51AdeOsLog(0,"i51Debuger sound recorder start Amr File");
            }

            i51AdeMmiMediaPlayerCreate(MediaCreateCallback);
        }
        break;
    case 1://recorder stop
        {
            if(InterWordColor[1]!=0xffff&&InterWordColor[1]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(2);
            memset(TestBuff[1],0,sizeof(TestBuff[1]));

            if(i51AdeMmiRecorderStop(RecordStopCallback))
            { 
                TestBuff[1][31] = 1;
                sprintf(TestBuff[1],"YES");
                i51AdeMmiMediaPlayerRelease();
                MMIStatus = 2;
                *pCurStatus = 2;
                PaintStatus = 1;
                InterSetFous(3);
                i51AdeOsLog(0,"i51Debuger sound recorder stop is ok");
                InterWordColor[0] = 0xffff;
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                if (i51_MMI_DEC_MEM_BASED == CurSoundDataType)
                {
                    i51MmiSubWritePcmFileHeaderAtEnd();
                    i51AdeOsLog(0,"i51Debuger sound recorder stop Pcm Memory update file head");
                }
            }
            else
            {
                TestBuff[1][31] = 0;
                sprintf(TestBuff[1],"NO");
                TotalError++;
                MMIStatus = 1;
                *pCurStatus = 1;
                PaintStatus = 1;
                InterSetFous(2);
                i51AdeOsLog(0,"i51Debuger sound recorder stop is fail");
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = 0xffff;
                InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
            }

            if(NULL != SoundBuffer)
            {
                i51AdeOsFree(SoundBuffer);
                SoundBuffer = NULL;
                i51AdeOsLog(0,"free SoundBuffer memory");
            }
        }
        break;

    case 2://recorder play
        {
            if(InterWordColor[2]!=0xffff&&InterWordColor[2]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            memset(TestBuff[2],0,sizeof(TestBuff[2]));
            InterSetFous(3);

            if (i51_MMI_DEC_MEM_BASED == CurSoundDataType)
            {
                PlayType = 4;
                i51AdeOsLog(0,"i51Debuger sound recorder_play start Pcm Memory");
            }
            if (i51_MMI_DEC_FILE_BASED == CurSoundDataType)
            {
                PlayType = 5;
                i51AdeOsLog(0,"i51Debuger sound recorder_play start Amr File");
            }

            i51AdeMmiMediaPlayerCreate(MediaCreateCallback);         
        }
        break;
    case 3://stop
        {
            if(InterWordColor[3]!=0xffff&&InterWordColor[3]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            memset(TestBuff[3],0,sizeof(TestBuff[3]));
            InterSetFous(4);
            if(i51AdeMmiMediaPlayerStop(Handle))
            {
                Handle = NULL;
                TestBuff[3][31] = 1;
                sprintf(TestBuff[3],"YES");
                i51AdeMmiMediaPlayerRelease();
                InterSetFous(3);
                MMIStatus = 2;
                *pCurStatus = 2;
                PaintStatus = 1;
                i51AdeOsLog(0,"i51Debuger sound recorder_play stop is ok");
                InterWordColor[0] = 0xffff;
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
            }
            else
            {
                TestBuff[3][31] = 0;
                sprintf(TestBuff[3],"NO");
                TotalError++;
                MMIStatus = 1;
                *pCurStatus = 1;
                PaintStatus = 1;
                i51AdeOsLog(0,"i51Debuger sound recorder_play stop is fail");
                InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[3] = 0xffff;
            }
        }
        break;
#if 0
    case 3://stop
        {
            if(InterWordColor[3]!=0xffff&&InterWordColor[3]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            memset(TestBuff[3],0,sizeof(TestBuff[3]));
            InterSetFous(4);

            if (i51_MMI_DEC_MEM_BASED == CurSoundDataType)
            {
                if(!i51AdeMmiRecorderPlayStop())
                {
                    TestBuff[3][31] = 1;
                    sprintf(TestBuff[3],"YES");
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 2;
                    i51AdeOsLog(0,"i51Debuger sound recorder_play stop Pcm Memory is ok");
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                }
                else
                {
                    TestBuff[3][31] = 0;
                    sprintf(TestBuff[3],"NO");
                    TotalError++;
                    MMIStatus = 1;
                    i51AdeOsLog(0,"i51Debuger sound recorder_play stop Pcm Memory is fail");
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = 0xffff;
                }
            }
            if (i51_MMI_DEC_FILE_BASED == CurSoundDataType)
            {
                if(i51AdeMmiMediaPlayerStop(Handle))
                {
                    Handle = NULL;
                    TestBuff[3][31] = 1;
                    sprintf(TestBuff[3],"YES");
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 2;
                    i51AdeOsLog(0,"i51Debuger sound recorder_play stop Amr File is ok");
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = 0xffff;
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                }
                else
                {
                    TestBuff[3][31] = 0;
                    sprintf(TestBuff[3],"NO");
                    TotalError++;
                    MMIStatus = 1;
                    i51AdeOsLog(0,"i51Debuger sound recorder_play stop Amr File is fail");
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = 0xffff;
                }
            }
        }
        break;
    case 4://pause
        {
            if(InterWordColor[4]!=0xffff&&InterWordColor[4]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(5);
            memset(TestBuff[4],0,sizeof(TestBuff[4]));
            if(i51AdeMmiMediaPlayerResume(Handle))
            {
                TestBuff[4][31] = 1;
                sprintf(TestBuff[4],"YES");
                MMIStatus = 2;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
            else
            {
                TestBuff[4][31] = 0;
                sprintf(TestBuff[4],"NO");
                TotalError++;
                MMIStatus = 1;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = i51KitG2COLOR(0, 0, 0);
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
        }
        break;
    case 5://resume
        {
            if(InterWordColor[5]!=0xffff&&InterWordColor[5]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(6);
            memset(TestBuff[5],0,sizeof(TestBuff[5]));
            if(i51AdeMmiMediaPlayerResume(Handle))
            {
                TestBuff[5][31] = 1;
                sprintf(TestBuff[5],"YES");
                MMIStatus = 2;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
            else
            {
                TestBuff[5][31] = 0;
                sprintf(TestBuff[5],"NO");
                TotalError++;
                MMIStatus = 1;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = i51KitG2COLOR(0, 0, 0);
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
        }
        break;
#endif
    default:
        {
            i51AdeOsLog(0,"i51MmiSubDO error parameter");
        }
        break;
    }
}

void setMmiSoundStatus(void)
{
    iU16 i = 0;
    memset(TestBuff ,0,sizeof(TestBuff));

    for (i = 0; i < 6; ++i)
    {
        switch (AllStatics[15+i])
        {
        case 0:
            {
                sprintf(TestBuff[i],"");
            }
            break;
        case 1:
            {
                sprintf(TestBuff[i],"NO");
            }
            break;
        case 2:
            {
                sprintf(TestBuff[i],"YES");
            }
            break;
        default:
            {
            }
            break;
        }
    }
#if 0
    status_sound.sound = AllStatics[11]; //MMIStatus
    if (0 == status_sound.sound)
    {
        status_sound.sound = 1;
        for (i = 0; 10 > i; ++i)
        {
            sprintf(TestBuff[i],"");
        }
    }
    if (2 == status_sound.sound)
    {
        for (i = 0; 10 > i; ++i)
        {
            sprintf(TestBuff[i],"YES");
        }
    }
    if (1 == status_sound.sound)
    {
        switch (status_sound.mid)
        {
        case 0:
            {
                TestBuff[0][31] = 1;
                sprintf(TestBuff[0],"");
            }
            break;
        case 1:
            {
                TestBuff[0][31] = 1;
                sprintf(TestBuff[0],"NO");
            }
            break;
        case 2:
            {
                TestBuff[0][31] = 1;
                sprintf(TestBuff[0],"YES");
            }
            break;
        default:
            {
            }
            break;
        }

        switch (status_sound.wav)
        {
        case 0:
            {
                TestBuff[1][31] = 1;
                sprintf(TestBuff[1],"");
            }
            break;
        case 1:
            {
                TestBuff[1][31] = 1;
                sprintf(TestBuff[1],"NO");
            }
            break;
        case 2:
            {
                TestBuff[1][31] = 1;
                sprintf(TestBuff[1],"YES");
            }
            break;
        default:
            {
            }
            break;
        }

        switch (status_sound.amr)
        {
        case 0:
            {
                TestBuff[2][31] = 1;
                sprintf(TestBuff[2],"");
            }
            break;
        case 1:
            {
                TestBuff[2][31] = 1;
                sprintf(TestBuff[2],"NO");
            }
            break;
        case 2:
            {
                TestBuff[2][31] = 1;
                sprintf(TestBuff[2],"YES");
            }
            break;
        default:
            {
            }
            break;
        }

        switch (status_sound.mp3)
        {
        case 0:
            {
                TestBuff[3][31] = 1;
                sprintf(TestBuff[3],"");
            }
            break;
        case 1:
            {
                TestBuff[3][31] = 1;
                sprintf(TestBuff[3],"NO");
            }
            break;
        case 2:
            {
                TestBuff[3][31] = 1;
                sprintf(TestBuff[3],"YES");
            }
            break;
        default:
            {
            }
            break;
        }

        switch (status_sound.recordpcm)
        {
        case 0:
            {
                TestBuff[4][31] = 1;
                sprintf(TestBuff[4],"");
            }
            break;
        case 1:
            {
                TestBuff[4][31] = 1;
                sprintf(TestBuff[4],"NO");
            }
            break;
        case 2:
            {
                TestBuff[4][31] = 1;
                sprintf(TestBuff[4],"YES");
            }
            break;
        default:
            {
            }
            break;
        }

        switch (status_sound.recordamr)
        {
        case 0:
            {
                TestBuff[5][31] = 1;
                sprintf(TestBuff[5],"");
            }
            break;
        case 1:
            {
                TestBuff[5][31] = 1;
                sprintf(TestBuff[5],"NO");
            }
            break;
        case 2:
            {
                TestBuff[5][31] = 1;
                sprintf(TestBuff[5],"YES");
            }
            break;
        default:
            {
            }
            break;
        }
    }
#endif
}

void setMmiSoundDataSrc(void)
{
    if (i51AdeOsFileIsExist (pCurSoundDataPath))
    {
        iFILE file = 0;
        iS32 fileSize = 0;

        file = i51AdeOsFileOpen (pCurSoundDataPath , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
        if(file == 0)
        {
            i51AdeOsLog ( i51_LOG_DC , "i51Debuger sound test using user's data is cannot open." );
        }
        i51AdeOsFileGetSize(file,&fileSize);
        if (CurSoundDataLength != fileSize)
        {
            SoundBufferLength = (fileSize < SoundBufferLengthMax) ? fileSize : SoundBufferLengthMax;
            SoundBuffer = (iU8*)i51AdeOsMalloc ( SoundBufferLength ,__FILE__ ,__LINE__);
            i51AdeOsLog(0,"SoundBuffer malloc memory");

            i51AdeOsFileSetSeek ( file,0,i51_OS_FL_SEEK_HEAD);
            i51AdeOsFileRead ( file , SoundBuffer , SoundBufferLength , 0 ) ;

            pCurSoundData = SoundBuffer;
            CurSoundDataLength = SoundBufferLength;

            i51AdeOsLog ( i51_LOG_DC , "i51Debuger sound test using user's data." );
        }
        else
        {
            i51AdeOsLog ( i51_LOG_DC , "i51Debuger sound test using std data." );
        }
        i51AdeOsFileClose(file);
    }
    else
    {
        i51AdeOsLog ( i51_LOG_DC , "i51Debuger sound test using std data." );
    }

    return;
}

void i51MMIDO(iU8 para)
{
    switch(para)
    {
    case 0://Sound Type Mid
        {
            if(InterWordColor[0]!=0xffff&&InterWordColor[0]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(1);
            status_sound.curFous = 1;

            pCurSoundData = pSoundDataStdMid;
            CurSoundDataLength = SoundDataStdMidLength;
            pCurSoundDataPath = SoundDataStdMidPath;
            CurSoundFormatType = i51_MEDIA_TYPE_MIDI;
            pCurStatus = &AllStatics[15];
            setMmiSoundDataSrc();

            i51AdeOsLog(0,"i51Debuger sound player Mid %x, %d", pCurSoundData, CurSoundDataLength);
            Runstatus = 18;
        }
        break;
    case 1://Sound Type Wav
        {
            if(InterWordColor[1]!=0xffff&&InterWordColor[1]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(1);
            status_sound.curFous = 2;

            pCurSoundData = pSoundDataStdWav;
            CurSoundDataLength = SoundDataStdWavLength;
            pCurSoundDataPath = SoundDataStdWavPath;
            CurSoundFormatType = i51_MEDIA_TYPE_WAV;
            pCurStatus = &AllStatics[16];
            setMmiSoundDataSrc();

            i51AdeOsLog(0,"i51Debuger sound player Wav %x, %d", pCurSoundData, CurSoundDataLength);
            Runstatus = 18;
        }
        break;

    case 2://Sound Type Amr
        {
            if(InterWordColor[2]!=0xffff&&InterWordColor[2]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(1);
            status_sound.curFous = 3;

            pCurSoundData = pSoundDataStdAmr;
            CurSoundDataLength = SoundDataStdAmrLength;
            pCurSoundDataPath = SoundDataStdAmrPath;
            CurSoundFormatType = i51_MEDIA_TYPE_AMR;
            pCurStatus = &AllStatics[17];
            setMmiSoundDataSrc();

            i51AdeOsLog(0,"i51Debuger sound player Amr %x, %d", pCurSoundData, CurSoundDataLength);
            Runstatus = 18;
        }
        break;
    case 3://Sound Type Mp3
        {
            if(InterWordColor[3]!=0xffff&&InterWordColor[3]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(1);
            status_sound.curFous = 4;

            pCurSoundData = pSoundDataStdMp3;
            CurSoundDataLength = SoundDataStdMp3Length;
            pCurSoundDataPath = SoundDataStdMp3Path;
            CurSoundFormatType = i51_MEDIA_TYPE_MP3;
            pCurStatus = &AllStatics[18];
            setMmiSoundDataSrc();

            i51AdeOsLog(0,"i51Debuger sound player Mp3 %x, %d", pCurSoundData, CurSoundDataLength);
            Runstatus = 18;
        }
        break;
    case 4://Sound Type Recorder Pcm
        {
            if(InterWordColor[4]!=0xffff&&InterWordColor[4]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(1);
            status_sound.curFous = 5;

            pCurSoundData = pSoundDataStdRecordPcm;
            CurSoundDataLength = SoundDataStdRecordPcmLength;
            pCurSoundDataPath = SoundDataStdRecordPcmPath;
            CurSoundFormatType = i51_MEDIA_TYPE_WAV;
            CurSoundDataType = i51_MMI_DEC_MEM_BASED;
            pCurStatus = &AllStatics[19];
            //setMmiSoundDataSrc();

            i51AdeOsLog(0,"i51Debuger sound player Record Pcm %x, %d", pCurSoundData, CurSoundDataLength);
            Runstatus = 19;
        }
        break;
    case 5://Sound Type Recorder Amr
        {
            if(InterWordColor[5]!=0xffff&&InterWordColor[5]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(1);
            status_sound.curFous = 6;

            pCurSoundData = pSoundDataStdRecordAmr;
            CurSoundDataLength = SoundDataStdRecordAmrLength;
            pCurSoundDataPath = SoundDataStdRecordAmrPath;
            CurSoundFormatType = i51_MEDIA_TYPE_AMR;
            CurSoundDataType = i51_MMI_DEC_FILE_BASED;
            pCurStatus = &AllStatics[20];
            //setMmiSoundDataSrc();

            i51AdeOsLog(0,"i51Debuger sound player Record Amr %x, %d", pCurSoundData, CurSoundDataLength);
            Runstatus = 19;
        }
        break;
    default:
        {
            i51AdeOsLog(0,"i51MmiDo Sound Sub error parameter");
        }
        break;
    }

    memset(TestBuff ,0,sizeof(TestBuff));
}

#if 0
void InterKeyDoMmiSound(iU8 num)
{
    TimeStruct GetTime = {0};

    memset(TestBuff[0] ,0,sizeof(TestBuff[0]));
    memset(TestBuff[1],0,sizeof(TestBuff[1]));
    memset(TestBuff[2],0,sizeof(TestBuff[2]));
    memset(TestBuff[3] ,0,sizeof(TestBuff[3]));
    memset(TestBuff[4] ,0,sizeof(TestBuff[4]));
    memset(TestBuff[5],0,sizeof(TestBuff[5]));
    memset(TestBuff[6],0,sizeof(TestBuff[6]));
    memset(TestBuff[7],0,sizeof(TestBuff[7]));
    memset(TestBuff[8],0,sizeof(TestBuff[8]));
    memset(TestBuff[9],0,sizeof(TestBuff[9]));
    Runstatus = 9+num;
    InterSetFous(1);
    sendOnce = 0;
    if(Runstatus == 18)
    {
        InterWordColor[0] = i51KitG2COLOR(0, 0, 0);
        InterWordColor[1] = 0xffff;
        InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
        InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
        InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
    }
    if(Runstatus == 19)
    {
        InterWordColor[0] = i51KitG2COLOR(0, 0, 0);
        InterWordColor[1] = 0xffff;
        InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
        InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
        InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
        InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
    }
}

void InterKeyBackMmiSound( )
{
    iU8 inputStatu = 0;
    if(InputStatus[0]==2&&InputStatus[1]==2&&InputStatus[2]==2)
    {
        inputStatu = 1;//suncceed
    }
    else if(InputStatus[0]==0&&InputStatus[1]==0&&InputStatus[2]==0)
    {
        inputStatu = 2;//untest
    }
    else 
    {
        inputStatu = 0;//failed
    }

    Runstatus = 0;
    InterSetFous(1);
    if(SMSStatus==1||SocketStatus==1||MMIStatus==1||ShockStatus==1||inputStatu==0||AllStatics[3]==1||HookStatus==1||TimeStatus==1)
    {
        AllStatics[4] = 3;
    }
    else if(SMSStatus==2&&SocketStatus==2&&MMIStatus==2&&ShockStatus==2&&inputStatu==1&&HookStatus==2&&TimeStatus==2)
    {
        if(AllStatics[3]==0)
        {
            AllStatics[4] = 1;
        }
        else
        {
            AllStatics[4] = AllStatics[3];
        }
    }
    else if(SMSStatus==0&&SocketStatus==0&&MMIStatus==0&&ShockStatus==0&&inputStatu==2&&HookStatus==0&&TimeStatus==0&&AllStatics[3]==0)
    {
        AllStatics[4] = 0;
    }
    else
    {
        AllStatics[4] = 1;
    }    
    AllStatics[6] = InputStatus[0];
    AllStatics[7] = InputStatus[1];
    AllStatics[8] = InputStatus[2];
    AllStatics[9] = SMSStatus;
    AllStatics[10] = SocketStatus;
    AllStatics[11] = MMIStatus;
    AllStatics[12] = ShockStatus;
    AllStatics[13] = HookStatus;
    AllStatics[14] = TimeStatus;

    i51SetAllStatics();
}
#endif

void i51InterMMISubPlayerTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown_SubPlayer[7] = {0};
    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightHead = height/10;
    HeightEnd = height/12;
    HeightPerOne = (height -HeightHead -HeightEnd)/9;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_SubPlayer[5])
                {
                    IsDown_SubPlayer[5] = iFALSE; 
                    Runstatus = 12;
                    InterSetFous(status_sound.curFous); 
                    PaintStatus = 1;
                    setMmiSoundStatus();
                    i51AdeMmiMediaPlayerRelease();
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_SubPlayer[5] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_SubPlayer[6])
                {
                    IsDown_SubPlayer[6] = iFALSE; 
                    i51MmiSubPlayerDO(InterFoucs-1); 
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_SubPlayer[6] = iTRUE;
            }
        }
        return;
    }

    for(i = 0;i<5;i++)
    {
        if ( x > 0  && x <width &&y >=HeightHead+HeightPerOne*i &&y < HeightHead+HeightPerOne*(i+1))
        {
            if(i51_MSG_PADUP == event &&IsDown_SubPlayer[i])
            {
                IsDown_SubPlayer[i] = iFALSE ; 
                i51MmiSubPlayerDO(i); 
            }
            else if(i51_MSG_PADDOWN == event)
            {
                IsDown_SubPlayer[i] = iTRUE;
            }
        }
    }

}

void i51InterMMISubRecorderTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown_SubRecorder[6] = {0};
    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightHead = height/10;
    HeightEnd = height/12;
    HeightPerOne = (height -HeightHead -HeightEnd)/9;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_SubRecorder[4])
                {
                    IsDown_SubRecorder[4] = iFALSE;
                    Runstatus = 12;
                    InterSetFous(status_sound.curFous); 
                    PaintStatus = 1;
                    setMmiSoundStatus();
                    i51AdeMmiMediaPlayerRelease();
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
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_SubRecorder[4] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_SubRecorder[5])
                {
                    IsDown_SubRecorder[5] = iFALSE; 
                    i51MmiSubRecorderDO(InterFoucs-1); 
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_SubRecorder[5] = iTRUE;
            }
        }
        return;
    }

    for(i = 0;i<4;i++)
    {
        if ( x > 0  && x <width &&y >=HeightHead+HeightPerOne*i &&y < HeightHead+HeightPerOne*(i+1))
        {
            if(i51_MSG_PADUP == event &&IsDown_SubRecorder[i])
            {
                IsDown_SubRecorder[i] = iFALSE ; 
                i51MmiSubRecorderDO(i); 
            }
            else if(i51_MSG_PADDOWN == event)
            {
                IsDown_SubRecorder[i] = iTRUE;
            }
        }
    }

}

void i51InterMMITouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown_MMI[11] = {0};
    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightHead = height/10;
    HeightEnd = height/12;
    HeightPerOne = (height -HeightHead -HeightEnd)/9;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_MMI[9])
                {
                    IsDown_MMI[9] = iFALSE; 
                    Runstatus = 4;
                    InterSetFous(4); 
                    PaintStatus = 1;
                    i51AdeMmiMediaPlayerRelease();
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
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_MMI[9] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_MMI[10])
                {
                    IsDown_MMI[10] = iFALSE; 
                    i51MMIDO(InterFoucs-1); 
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_MMI[10] = iTRUE;
            }
        }
        return;
    }

    for(i = 0;i<9;i++)
    {
        if ( x > 0  && x <width &&y >=HeightHead+HeightPerOne*i &&y < HeightHead+HeightPerOne*(i+1))
        {
            if(i51_MSG_PADUP == event &&IsDown_MMI[i])
            {
                IsDown_MMI[i] = iFALSE ; 
                i51MMIDO(i); 
            }
            else if(i51_MSG_PADDOWN == event)
            {
                IsDown_MMI[i] = iTRUE;
            }
        }
    }

}

void DrowMmiSubPlayerTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU32 DrawWidth = 0;
    iU32 DrawHeight = 0;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/9;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"MMI - Sound Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"MMI - Sound Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    //    if (i51_ADE_CNF_VER_CS == i51AdeCnfGetType())
    //    {
    //        i51AdeOsLog(0,"MMITest , CPU type is MIPS, don't test memory");
    //        DrawText(rect.Width ,rect.Height ,L"1 Play With Memory",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    //        InterSetFous(2);
    //    }
    //    else
    {
        DrawText(rect.Width ,rect.Height ,L"1 Play With Memory",rect.X ,rect.Y ,InterWordColor[0]);
    }

    i51KitG2FontGetDemension (TypeheadHandle,2,3,0,0,0,0,&DrawWidth,&DrawHeight);
    DrawWidth = (DrawWidth/2) + 5;

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[0],0,&rect , &rect,0, 0,  0,  InterWordColor[0] , 0, 0); 
    i++;


    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"2 Play With File",rect.X ,rect.Y ,InterWordColor[1]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[1],0,&rect , &rect,0, 0,  0,  InterWordColor[1] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"3 Playing Stop",rect.X ,rect.Y ,InterWordColor[2]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[2],0,&rect , &rect,0, 0,  0,  InterWordColor[2] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[3],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[3],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"4 Playing Pause",rect.X ,rect.Y ,InterWordColor[3]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[3],0,&rect , &rect,0, 0,  0,  InterWordColor[3] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[4],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[4],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"5 Playing Resume",rect.X ,rect.Y ,InterWordColor[4]);


    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[4],0,&rect , &rect,0, 0,  0,  InterWordColor[4] , 0, 0);
    i++;

    //DrawBottom();    
    DrawTowButton();
    i51KitG2Update();
}

void DrowMmiSubRecorderTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU32 DrawWidth = 0;
    iU32 DrawHeight = 0;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/9;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"MMI - Record Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"MMI - Record Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"1 Recorder Start",rect.X ,rect.Y ,InterWordColor[0]);
    i51KitG2FontGetDemension (TypeheadHandle,2,3,0,0,0,0,&DrawWidth,&DrawHeight);
    DrawWidth = (DrawWidth/2) + 5;

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[0],0,&rect , &rect,0, 0,  0,  InterWordColor[0] , 0, 0); 
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"2 Recorder Stop",rect.X ,rect.Y ,InterWordColor[1]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[1],0,&rect , &rect,0, 0,  0,  InterWordColor[1] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"3 Recorder Play",rect.X ,rect.Y ,InterWordColor[2]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[2],0,&rect , &rect,0, 0,  0,  InterWordColor[2] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[3],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[3],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"4 Playing Stop",rect.X ,rect.Y ,InterWordColor[3]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[3],0,&rect , &rect,0, 0,  0,  InterWordColor[3] , 0, 0);
    i++;
#if 0
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[4],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[4],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"5 Playing Pause",rect.X ,rect.Y ,InterWordColor[4]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[4],0,&rect , &rect,0, 0,  0,  InterWordColor[4] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[5],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[5],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"6 Playing Resume",rect.X ,rect.Y ,InterWordColor[5]);


    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[5],0,&rect , &rect,0, 0,  0,  InterWordColor[5] , 0, 0);
    i++;
#endif
    //DrawBottom();    
    DrawTowButton();
    i51KitG2Update();
}

void DrowMMITest()
{

    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU32 DrawWidth = 0;
    iU32 DrawHeight = 0;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0,StrHeight = 0;

    InterSetFous(status_sound.curFous); 
    setMmiSoundStatus();

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/9;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"MMI Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"MMI Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"1 Midi Sound Test",rect.X ,rect.Y ,InterWordColor[0]);

    i51KitG2FontGetDemension (TypeheadHandle,2,3,0,0,0,0,&DrawWidth,&DrawHeight);
    DrawWidth = (DrawWidth/2) + 5;

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[0],0,&rect , &rect,0, 0,  0,  InterWordColor[0] , 0, 0); 
    i++;


    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"2 Wav Sound Test",rect.X ,rect.Y ,InterWordColor[1]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[1],0,&rect , &rect,0, 0,  0,  InterWordColor[1] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"3 Amr Sound Test",rect.X ,rect.Y ,InterWordColor[2]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[2],0,&rect , &rect,0, 0,  0,  InterWordColor[2] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[3],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[3],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"4 Mp3 Sound Test",rect.X ,rect.Y ,InterWordColor[3]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[3],0,&rect , &rect,0, 0,  0,  InterWordColor[3] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[4],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[4],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"5 Recorder Pcm Memory Test",rect.X ,rect.Y ,InterWordColor[4]);


    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[4],0,&rect , &rect,0, 0,  0,  InterWordColor[4] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[5],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[5],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"6 Recorder Amr File Test",rect.X ,rect.Y ,InterWordColor[5]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[5],0,&rect , &rect,0, 0,  0,  InterWordColor[5] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    memset(TestBuff[6],0,sizeof(TestBuff[6]));
    sprintf(TestBuff[6],"请确认声音模块正常工作!");
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[6],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    memset(TestBuff[6],0,sizeof(TestBuff[6]));
    sprintf(TestBuff[6],"互芯只支持wav,amr,mp3文件格式");
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[6],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    memset(TestBuff[7],0,sizeof(TestBuff[7]));
    sprintf(TestBuff[7],"所有平台都支持pcm, Amr录音");
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[7],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    //DrawBottom();    
    DrawTowButton();
    i51KitG2Update();
}
#else
void i51MMIDO(iU8 para)
{
    static unsigned int RecordRunStatus = 0; //0, Record start; 1, Record stop;

    switch(para)
    {
    case 0://play with memory
        {
            if(InterWordColor[0]!=0xffff&&InterWordColor[0]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(1);
            memset(TestBuff[0],0,sizeof(TestBuff[0]));

            i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay MediaData %x",SoundDataStdWav);
            PlayType = 1;
            i51AdeMmiMediaPlayerCreate(MediaCreateCallback);                     
        }
        break;
    case 1://play with file
        {
            iFILE file = 0;
            iU32 ReadLen = 0; 
            if(InterWordColor[1]!=0xffff&&InterWordColor[1]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }

            file = i51AdeOsFileOpen (L"i51Debuger\\i51Phone.wav" , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN , __FILE__ , __LINE__ ) ;
            if(file!=0)
            {
                i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay i51AdeOsFileOpen succeed");
                i51AdeOsFileWrite(file ,SoundDataStdWav , SoundDataStdWavLength, &ReadLen) ;
                i51AdeOsFileClose(file);
            }
            InterSetFous(2);
            memset(TestBuff[1],0,sizeof(TestBuff[1]));

            i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay %x",MediaPath);
            PlayType = 2;
            i51AdeMmiMediaPlayerCreate(MediaCreateCallback);
        }
        break;

    case 2://stop
        {
            if(InterWordColor[2]!=0xffff&&InterWordColor[2]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            memset(TestBuff[2],0,sizeof(TestBuff[2]));
            if(NULL==Handle)
            {
                TestBuff[2][31] = 1;
                sprintf(TestBuff[2],"YES");
                MMIStatus = 2;
                return;
            }
            InterSetFous(3);
            if(i51AdeMmiMediaPlayerStop(Handle))
            {
                Handle = NULL;
                TestBuff[2][31] = 1;
                sprintf(TestBuff[2],"YES");
                i51AdeMmiMediaPlayerRelease();
                MMIStatus = 2;
                InterWordColor[0] =0xffff;
                InterWordColor[1] = 0xffff;
                InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[5] = 0xffff;
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = 0xffff;
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
            else
            {
                TestBuff[2][31] = 0;
                sprintf(TestBuff[2],"NO");
                TotalError++;
                MMIStatus = 1;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = i51KitG2COLOR(0, 0, 0);
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = 0xffff;
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
        }
        break;
    case 3://pause
        {
            if(InterWordColor[3]!=0xffff&&InterWordColor[3]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            memset(TestBuff[3],0,sizeof(TestBuff[3]));
            InterSetFous(4);
            if(i51AdeMmiMediaPlayerPause(Handle))
            {
                TestBuff[3][31] = 1;
                sprintf(TestBuff[3],"YES");
                MMIStatus = 2;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[4] = 0xffff;
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
            else
            {
                TestBuff[3][31] = 0;
                sprintf(TestBuff[3],"NO");
                TotalError++;
                MMIStatus = 1;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = i51KitG2COLOR(0, 0, 0);
                InterWordColor[4] = 0xffff;
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
        }
        break;
    case 4://resume
        {
            if(InterWordColor[4]!=0xffff&&InterWordColor[4]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }
            InterSetFous(5);
            memset(TestBuff[4],0,sizeof(TestBuff[4]));
            if(i51AdeMmiMediaPlayerResume(Handle))
            {
                TestBuff[4][31] = 1;
                sprintf(TestBuff[4],"YES");
                MMIStatus = 2;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
            else
            {
                TestBuff[4][31] = 0;
                sprintf(TestBuff[4],"NO");
                TotalError++;
                MMIStatus = 1;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = 0xffff;
                InterWordColor[3] = 0xffff;
                InterWordColor[4] = i51KitG2COLOR(0, 0, 0);
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
        }
        break;
    case 5://record pcm start / stop
        {
            memset(TestBuff[5],0,sizeof(TestBuff[5]));
            PlayType = 3;
            InterSetFous(6);
            if(InterWordColor[5]!=0xffff&&InterWordColor[5]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }

            if (0 == RecordRunStatus)
            {//record pcm start 
                RecordRunStatus = 1;
                i51AdeMmiMediaPlayerCreate(MediaCreateCallback); 
            }
            else if (1 == RecordRunStatus)
            {//record pcm stop 
                RecordRunStatus = 0;
                if(i51AdeMmiRecorderStop(RecordStopCallback))
                { 
                    TestBuff[5][31] = 1;
                    sprintf(TestBuff[5],"YES");
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 2;
                    InterWordColor[0] =0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = 0xffff;
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = 0xffff;
                }
                else
                {
                    TestBuff[5][31] = 0;
                    sprintf(TestBuff[5],"NO");
                    TotalError++;
                    MMIStatus = 1;
                    InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = i51KitG2COLOR(0, 0, 0);
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
                if(NULL!=MediaRecordBUFF)
                {
                    i51AdeOsFree(MediaRecordBUFF);
                    i51AdeOsLog(0,"free record memory");
                    MediaRecordBUFF = NULL;
                }
            }
        }
        break;
    case 6://record pcm play start / stop
        {
            memset(TestBuff[6],0,sizeof(TestBuff[6]));
            PlayType = 4;
            InterSetFous(7);
            if(InterWordColor[6]!=0xffff&&InterWordColor[6]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }

            if (0 == RecordRunStatus)
            {//record pcm play start
                RecordRunStatus = 1;
                i51AdeMmiMediaPlayerCreate(MediaCreateCallback); 
            }
            else if (1 == RecordRunStatus)
            {//record pcm play stop
                RecordRunStatus = 0;
                i51AdeMmiRecorderPlayStop();
                {
                    TestBuff[6][31] = 1;
                    sprintf(TestBuff[6],"YES");
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 2;
                    InterWordColor[0] =0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = 0xffff;
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = 0xffff;
                }
                if(NULL!=MediaRecordData)
                {
                    i51AdeOsFree(MediaRecordData);
                    i51AdeOsLog(0,"free record memory");
                    MediaRecordData = NULL;
                }
#if 0
                else
                {
                    TestBuff[8][31] = 0;
                    sprintf(TestBuff[8],"NO");
                    TotalError++;
                    MMIStatus = 1;
                    InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[8] = i51KitG2COLOR(0, 0, 0);
                }
#endif
            }
        }
        break;
    case 7://record amr start / stop
        {
            memset(TestBuff[7],0,sizeof(TestBuff[7]));
            PlayType = 5;
            InterSetFous(8);
            if(InterWordColor[7]!=0xffff&&InterWordColor[7]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }

            if (0 == RecordRunStatus)
            {//record amr start 
                RecordRunStatus = 1;
                i51AdeMmiRecorderStartEx(AmrPath, MediaCreateCallback);
                TestBuff[7][31] = 1;
                sprintf(TestBuff[7],"YES");
                i51AdeOsLog(0,"i51MMIDO Record Amr S ok");
                MMIStatus = 2;
                InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[7] = 0xffff;
                InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
            }
            else if (1 == RecordRunStatus)
            {//record amr stop 
                RecordRunStatus = 0;
                if(i51AdeMmiRecorderStop(RecordStopCallback))
                { 
                    TestBuff[7][31] = 1;
                    sprintf(TestBuff[7],"YES");
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 2;
                    InterWordColor[0] =0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = 0xffff;
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = 0xffff;
                }
                else
                {
                    TestBuff[7][31] = 0;
                    sprintf(TestBuff[7],"NO");
                    TotalError++;
                    MMIStatus = 1;
                    InterWordColor[0] =i51KitG2COLOR(128, 128, 128);
                    InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[6] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[7] = i51KitG2COLOR(0, 0, 0);
                    InterWordColor[8] = i51KitG2COLOR(128, 128, 128);
                }
            }
        }
        break;
    case 8://record amr play start / stop
        {
            memset(TestBuff[8],0,sizeof(TestBuff[8]));
            PlayType = 6;
            InterSetFous(9);
            if(InterWordColor[8]!=0xffff&&InterWordColor[8]!=i51KitG2COLOR(0, 0, 0))
            {
                return;
            }

            if (0 == RecordRunStatus)
            {//record amr play start
                RecordRunStatus = 1;
                i51AdeOsLog(0,"Amr record amr play start");
                i51AdeMmiMediaPlayerCreate(MediaCreateCallback); 
            }
            else if (1 == RecordRunStatus)
            {//record amr play stop
                RecordRunStatus = 0;
                if(i51AdeMmiMediaPlayerStop((void *)AmrPath))
                {
                    TestBuff[8][31] = 1;
                    sprintf(TestBuff[8],"YES");
                    i51AdeMmiMediaPlayerRelease();
                    MMIStatus = 2;
                    InterWordColor[0] =0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = 0xffff;
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = i51KitG2COLOR(0, 0, 0);
                }
                else
                {
                    TestBuff[8][31] = 0;
                    sprintf(TestBuff[8],"NO");
                    TotalError++;
                    MMIStatus = 1;
                    InterWordColor[0] = 0xffff;
                    InterWordColor[1] = 0xffff;
                    InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[4] = i51KitG2COLOR(128, 128, 128);
                    InterWordColor[5] = 0xffff;
                    InterWordColor[6] = 0xffff;
                    InterWordColor[7] = 0xffff;
                    InterWordColor[8] = i51KitG2COLOR(0, 0, 0);
                }
                PaintStatus = 1;
            }
        }
        break;
    }
}

void i51InterMMITouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown_MMI[11] = {0};
    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightHead = height/10;
    HeightEnd = height/12;
    HeightPerOne = (height -HeightHead -HeightEnd)/10;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_MMI[9])
                {
                    IsDown_MMI[9] = iFALSE;

                    Runstatus = 4;
                    InterSetFous(4);
                    i51AdeMmiMediaPlayerRelease();
                    if(NULL!=MediaRecordBUFF)
                    {
                        i51AdeOsFree(MediaRecordBUFF);
                        MediaRecordBUFF = NULL;
                        i51AdeOsLog(0,"free record memory");
                    }
                    if(NULL!=MediaRecordData)
                    {
                        i51AdeOsFree(MediaRecordData);
                        i51AdeOsLog(0,"free record play memory");
                        MediaRecordData = NULL;
                    }
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_MMI[9] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_MMI[10])
                {
                    IsDown_MMI[10] = iFALSE;

                    i51MMIDO(InterFoucs-1);
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_MMI[10] = iTRUE;
            }
        }
        return;
    }

    for(i = 0;i<9;i++)
    {
        if ( x > 0  && x <width &&y >=HeightHead+HeightPerOne*i &&y < HeightHead+HeightPerOne*(i+1))
        {
            if(i51_MSG_PADUP == event &&IsDown_MMI[i])
            {
                IsDown_MMI[i] = iFALSE ;
                i51MMIDO(i);
            }
            else if(i51_MSG_PADDOWN == event)
            {
                IsDown_MMI[i] = iTRUE;
            }
        }
    }
}

void DrowMMITest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU32 DrawWidth = 0;
    iU32 DrawHeight = 0;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/10;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"MMI Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"MMI Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    if (i51_ADE_CNF_VER_CS == i51AdeCnfGetType())
    {
        i51AdeOsLog(0,"MMITest , CPU type is MIPS, don't test memory");
        DrawText(rect.Width ,rect.Height ,L"1.Play With Memory",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
        InterSetFous(2);
    }
    else
    {
        DrawText(rect.Width ,rect.Height ,L"1.Play With Memory",rect.X ,rect.Y ,InterWordColor[0]);
    }

    i51KitG2FontGetDemension (TypeheadHandle,2,3,0,0,0,0,&DrawWidth,&DrawHeight);
    DrawWidth = (DrawWidth/2) + 5;

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[0],0,&rect , &rect,0, 0,  0,  InterWordColor[0] , 0, 0); 
    i++;


    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"2 Play With File",rect.X ,rect.Y ,InterWordColor[1]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[1],0,&rect , &rect,0, 0,  0,  InterWordColor[1] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"3 Playing Stop",rect.X ,rect.Y ,InterWordColor[2]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[2],0,&rect , &rect,0, 0,  0,  InterWordColor[2] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[3],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[3],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"4 Playing Pause",rect.X ,rect.Y ,InterWordColor[3]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[3],0,&rect , &rect,0, 0,  0,  InterWordColor[3] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[4],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[4],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"5 Playing Resume",rect.X ,rect.Y ,InterWordColor[4]);


    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[4],0,&rect , &rect,0, 0,  0,  InterWordColor[4] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[5],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[5],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"6 RecordMem Start/End",rect.X ,rect.Y ,InterWordColor[5]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[5],0,&rect , &rect,0, 0,  0,  InterWordColor[5] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[6],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[6],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"7 RecordMem Play S/E",rect.X ,rect.Y ,InterWordColor[6]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[6],0,&rect , &rect,0, 0,  0,  InterWordColor[6] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[7],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[7],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"8 RecordAmr Start/End",rect.X ,rect.Y ,InterWordColor[7]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[7],7,&rect , &rect,0, 0,  0,  InterWordColor[7] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[8],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[8],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"9 RecordAmr Play S/E",rect.X ,rect.Y ,InterWordColor[8]);

    rect.X = width - DrawWidth;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,TestBuff[8],0,&rect , &rect,0, 0,  0,  InterWordColor[8] , 0, 0);
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    memset(TestBuff[9],0,sizeof(TestBuff[9]));
    sprintf(TestBuff[9],"请确认声音模块正常工作!");
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[9],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    //DrawBottom();    
    DrawTowButton();
    i51KitG2Update();
}
#endif

void i51CallDo(iU8 para)
{
    i51STDEDITOR editor = {0};
    iCOLOR cc;
    switch(para)
    {
    case 0:
        {
            cc = i51KitG2COLOR(128, 128, 128);
            //            if(InterWordColor[0] == cc)
            {
                //                return;
            }
            InterSetFous(1);
            memset(EditorBuff,0,sizeof(EditorBuff));
            editor.type = i51_EDITOR_NUMBER;
            editor.buffer = (void *)EditorBuff;
            editor.length = 64;
            editor.callback = EditorCallback_Call;
            i51AdeStdEntryEditor ( &editor );
            i51AdeMmiUpdateScreen();;
        }
        break;
    case 1:
        {
            cc = i51KitG2COLOR(128, 128, 128);
            //             if(InterWordColor[1] == cc)
            {
                //                 return;
            }
            InterSetFous(2);
            InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
            InterFoucs = 10;
            if(i51AdeOsStopPhoneCall(PhoneId))
            {
                sprintf(TestBuff[4],"i51AdeOsStopPhoneCall SUCCEED..." );
                CallStatus = 2;
            }
            else
            {
                sprintf(TestBuff[4],"i51AdeOsStopPhoneCall FALSED..." );
                TotalError++;
                CallStatus = 1;
            }
            if(i51AdeOsReleasePhone(PhoneId))
            {
                sprintf(TestBuff[5],"i51AdeOsReleasePhone SUCCEED..." );
                CallStatus = 2;
                InterSetFous(1);
                InterWordColor[1] = i51KitG2COLOR(128,128,128);
            }
            else
            {
                sprintf(TestBuff[5],"i51AdeOsReleasePhone FALSED..." );
                TotalError++;
                CallStatus = 1;
            }
            if(2 == CallStatus)
            {
                iS32* sims = NULL;
                sims = i51AdeSmsGetSims();
                sprintf(TestBuff[6],"请确认电话已经拨通。");
                sprintf(TestBuff[7],"注意:该项必须遍历所有sim卡!");
                sprintf(TestBuff[8],"当前sim卡编号为: %d", sims[1]);
                i51AdeOsLog(0, "i51Debuger CALL Test is ok, sim card total:%d, index: %d", sims[0], sims[1]);    
            }
        }
        break;
    }
}
void i51InterCallTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown_Call[4] = {0};


    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightHead = height/10;
    HeightEnd = height/12;
    HeightPerOne = (height -HeightHead -HeightEnd)/9;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;


    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event )
            {
                if(IsDown_Call[2])
                {
                    IsDown_Call[2] = iFALSE;
                    Runstatus = 4;
                    InterSetFous(5);
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_Call[2] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_Call[3])
                {
                    IsDown_Call[3] = iFALSE;
                    i51CallDo(InterFoucs-1);
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_Call[3] = iTRUE;
            }
        }
        return;
    }
    for(i = 0;i<2;i++)
    {
        if ( x > 0  && x <width &&y >=HeightHead+HeightPerOne*i &&y < HeightHead+HeightPerOne*(i+1))
        {
            if(i51_MSG_PADUP == event&&IsDown_Call[i] )
            {
                IsDown_Call[i] = iFALSE ;
                i51CallDo(i);
            }
            else if(i51_MSG_PADDOWN == event)
            {
                IsDown_Call[i] = iTRUE;
            }
        }
    }
}

void i51ShockDo(iU8 para)
{
    i51STDEDITOR editor = {0};
    iCOLOR col = 0;
    col = i51KitG2COLOR(128,128,128);
    switch(para)
    {
    case 0:
        {
            memset(EditorBuff,0,sizeof(EditorBuff));
            editor.type = i51_EDITOR_NUMBER;
            editor.buffer = (void *)EditorBuff;
            editor.length = 64;
            editor.callback = EditorCallback_Shock;
            i51AdeStdEntryEditor ( &editor );
            i51AdeMmiUpdateScreen();
            InterSetFous(2);
            InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
            InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
        }
        break;
    case 1:
        {
            memset(TestBuff[1],0,sizeof(TestBuff[1]));
            if(ShockTime==0)
            {
                sprintf(TestBuff[1],"Shock Time is 0....");
                break ;
            }
            if(InterWordColor[1]==col)
            {
                sprintf(TestBuff[1],"shock is enabled");
                break;
            }
            memset(TestBuff[2],0,sizeof(TestBuff[2]));
            memset(TestBuff[3],0,sizeof(TestBuff[3]));

            if(i51AdeOsSetShock(iTRUE))
            {
                sprintf(TestBuff[1],"Set Shock Open SUCCEED...");
                ShockStatus = 2;

                InterSetFous(3);

                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
            }
            else
            {
                sprintf(TestBuff[1],"Set Shock Open FALSED...");
                TotalError++;
                ShockStatus = 1;
            }
        }
        break;
    case 2:
        {
            memset(TestBuff[2],0,sizeof(TestBuff[2]));
            if(InterWordColor[2]==col)
            {
                sprintf(TestBuff[2],"Shock is not enable");
                break;
            }

            if(i51AdeOsDoShock(ShockTime*1000))//
            {
                sprintf(TestBuff[2],"i51AdeOsDoShock  SUCCEED...");

                InterSetFous(4);
                InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                ShockStatus = 2;
            }
            else
            {
                sprintf(TestBuff[2],"i51AdeOsDoShock FALSED...");
                TotalError++;
                ShockStatus = 1;
            }
        }
        break;
    case 3:
        {
            memset(TestBuff[3],0,sizeof(TestBuff[3]));
            if(InterWordColor[3]==col)
            {
                sprintf(TestBuff[3],"Shock is not enable");
                break;
            }

            if(i51AdeOsSetShock(iFALSE))
            {
                sprintf(TestBuff[3],"Set Shock Close SUCCEED...");
                InterSetFous(2);
                InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
                InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
                ShockStatus = 2;
            }
            else
            {
                sprintf(TestBuff[3],"Set Shock Close FALSED...");
                TotalError++;
                ShockStatus = 1;
            }

            memset(TestBuff[4],0,sizeof(TestBuff[4]));
            sprintf(TestBuff[4],"请确认手机震动正常!");
        }
        break;
    }
}
void i51InterShockTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown_Shock[6] = {0};


    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightHead = height/10;
    HeightEnd = height/12;
    HeightPerOne = (height -HeightHead -HeightEnd)/9;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_Shock[4])
                {
                    IsDown_Shock[4] = iFALSE;
                    Runstatus = 4;
                    InterSetFous(6);
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_Shock[4] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_Shock[5])
                {
                    IsDown_Shock[5] = iFALSE;
                    i51ShockDo(InterFoucs-1);
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_Shock[5] = iTRUE;
            }
        }
        return;
    }

    for(i = 0;i<4;i++)
    {
        if ( x > 0  && x <width &&y >=HeightHead+HeightPerOne*i &&y < HeightHead+HeightPerOne*(i+1))
        {
            if(i51_MSG_PADUP == event&&IsDown_Shock[i])
            {
                IsDown_Shock[i] = iFALSE ;
                i51ShockDo(i);
            }
            else if(i51_MSG_PADDOWN == event)
            {
                IsDown_Shock[i] = iTRUE;
            }
        }
    }
}

void i51SetTimeDo(void)
{
    i51STDEDITOR editor = {0};
    memset(TestBuff[1],0,sizeof(TestBuff[1]));
    memset(TestBuff[2],0,sizeof(TestBuff[2]));
    memset(TestBuff[3],0,sizeof(TestBuff[3]));
    memset(EditorBuff,0,sizeof(EditorBuff));
    editor.type = i51_EDITOR_TEXT;
    editor.buffer = (void *)EditorBuff;
    editor.length = 64;
    editor.callback = EditorCallback_Time;
    i51AdeStdEntryEditor ( &editor );
    i51AdeMmiUpdateScreen();
}
void i51InterSetTimeTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;

    static iBOOL IsDown_Time[3] = {0};

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightPerOne = 24;
    HeightHead = height/10;
    HeightEnd = height/12;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_Time[1])
                {
                    IsDown_Time[1] = iFALSE;
                    Runstatus = 4;
                    InterSetFous(8);
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_Time[1] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_Time[2])
                {
                    IsDown_Time[2] = iFALSE;
                    InterFoucs = 10;
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterColor0[0]=i51KitG2COLOR(7, 24, 40);
                    InterColor1[0]=i51KitG2COLOR(7, 24, 40);
                    i51SetTimeDo();
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_Time[2] = iTRUE;
            }
        }
        return;
    }

    if ( x > 0  && x <width &&y >=HeightHead &&y < HeightHead+HeightPerOne)
    {
        if(i51_MSG_PADUP == event&&IsDown_Time[0])
        {
            IsDown_Time[0] = iFALSE;
            InterFoucs = 10;
            InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
            InterColor0[0]=i51KitG2COLOR(7, 24, 40);
            InterColor1[0]=i51KitG2COLOR(7, 24, 40);
            i51SetTimeDo();
        }
        else if(i51_MSG_PADDOWN == event)
        {
            IsDown_Time[0] = iTRUE;
        }
    }
}

void i51InterHookTouchDo ( iU16 event , iU32 value )
{
    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    static iBOOL IsDown_Hook[3] = {0};

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightPerOne = 24;
    HeightHead = height/10;
    HeightEnd = height/12;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event )
            {
                if(IsDown_Hook[1])
                {
                    IsDown_Hook[1] = iFALSE;
                    Runstatus = 4;
                    InterSetFous(7);
                }
                CloseColor = 0xffff;
                if(smshookid)
                {
                    i51AdeSmsReleaseHook(smshookid);
                    smshookid = 0;
                }

            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown_Hook[1] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown_Hook[2])
                {
                    IsDown_Hook[2] = iFALSE;

                    InterFoucs = 10;
                    InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
                    InterColor0[0]=i51KitG2COLOR(7, 24, 40);
                    InterColor1[0]=i51KitG2COLOR(7, 24, 40);
                    SetHook();
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown_Hook[2] = iTRUE;
            }
        }
        return;
    }

    if ( x > 0  && x <width &&y >=HeightHead &&y < HeightHead+HeightPerOne)
    {
        if(i51_MSG_PADUP == event&&IsDown_Hook[0])
        {
            IsDown_Hook[0] = iFALSE;
            InterFoucs = 10;
            InterWordColor[0] = i51KitG2COLOR(128, 128, 128);
            InterColor0[0]=i51KitG2COLOR(7, 24, 40);
            InterColor1[0]=i51KitG2COLOR(7, 24, 40);
            SetHook();
        }
        else if(i51_MSG_PADDOWN == event)
        {
            IsDown_Hook[0] = iTRUE;
        }
    }
}

void i51DotpayDo(void)
{
    AllStatics[6] = InputStatus[0];
    AllStatics[7] = InputStatus[1];
    AllStatics[8] = InputStatus[2];
    AllStatics[9] = SMSStatus;
    AllStatics[10] = SocketStatus;
    AllStatics[11] = MMIStatus;
    AllStatics[12] = ShockStatus;
    AllStatics[13] = HookStatus;
    AllStatics[14] = TimeStatus;

    i51SetAllStatics();

    i51KernelSendRequest ( SimplePortalId , L"DotpayTest" , i51_REQ_LOADEX , 0 , 0 ) ;
    return;
}

void i51InterDotpayTouchDo ( iU16 event , iU32 value )
{
    i51DotpayDo();
    return;
}

void ResetInterStatus()
{
    SMSStatus=0;
    SocketStatus=0;
    MMIStatus=0;
    CallStatus=0;
    ShockStatus=0;
    HookStatus = 0;
    TimeStatus = 0;
    InputStatus[0]=0;
    InputStatus[1]=0;
    InputStatus[2]=0;
    memset(AllStatics,0,sizeof(AllStatics)); 
    i51SetAllStatics();
}
void ResetInter()
{
    SMSStatus=0;
    SocketStatus=0;
    MMIStatus=0;
    CallStatus=0;
    ShockStatus=0;
    HookStatus = 0;
    TimeStatus = 0;
    InputStatus[0]=0;
    InputStatus[1]=0;
    InputStatus[2]=0;
}

void UpdataInterStatus()
{
    iU8 inputStatu = 0;

    InputStatus[0] = AllStatics[6];
    InputStatus[1] = AllStatics[7];
    InputStatus[2] = AllStatics[8];
    SMSStatus = AllStatics[9];
    SocketStatus = AllStatics[10];
    MMIStatus = AllStatics[11];
    ShockStatus = AllStatics[12];
    HookStatus = AllStatics[13];
    TimeStatus = AllStatics[14];

    if(InputStatus[0]==2&&InputStatus[1]==2) //&&InputStatus[2]==2)
    {
        inputStatu = 1;//succeed
    }
    else if(InputStatus[0]==0&&InputStatus[1]==0) //&&InputStatus[2]==0)
    {
        inputStatu = 2;//untest
    }
    else
    {
        inputStatu = 0;//failed
    }    

    if(SMSStatus==1||SocketStatus==1||MMIStatus==1||ShockStatus==1||inputStatu==0||AllStatics[3]==1||HookStatus==1||TimeStatus==1)
    {
        AllStatics[4] = 3;
    }
    else if(SMSStatus==2&&SocketStatus==2&&MMIStatus==2&&ShockStatus==2&&HookStatus==2&&TimeStatus==2&&inputStatu==1)
    {
        if(AllStatics[3]==0)
        {
            AllStatics[4] = 1;
        }
        else 
        {
            AllStatics[4] = AllStatics[3];
        }

    }
    else if(SMSStatus==0&&SocketStatus==0&&MMIStatus==0&&ShockStatus==0&&HookStatus==0&&TimeStatus==0&&inputStatu==2&&AllStatics[3]==0)
    {
        AllStatics[4] = 0;
    }    
    else
    {
        AllStatics[4] = 1;
    }
}

void InterKeyDo(iU8 num)
{
    TimeStruct GetTime = {0};

    memset(TestBuff[0] ,0,sizeof(TestBuff[0]));
    memset(TestBuff[1],0,sizeof(TestBuff[1]));
    memset(TestBuff[2],0,sizeof(TestBuff[2]));
    memset(TestBuff[3] ,0,sizeof(TestBuff[3]));
    memset(TestBuff[4] ,0,sizeof(TestBuff[4]));
    memset(TestBuff[5],0,sizeof(TestBuff[5]));
    memset(TestBuff[6],0,sizeof(TestBuff[6]));
    memset(TestBuff[7],0,sizeof(TestBuff[7]));
    memset(TestBuff[8],0,sizeof(TestBuff[8]));
    memset(TestBuff[9],0,sizeof(TestBuff[9]));
    Runstatus = 9+num;
    InterSetFous(1);
    sendOnce = 0;
    if(Runstatus == 12)
    {
        InterWordColor[0] = i51KitG2COLOR(0, 0, 0);
        InterWordColor[1] = 0xffff;
        InterWordColor[2] = 0xffff;
        InterWordColor[3] = 0xffff;
        InterWordColor[4] = 0xffff;
        InterWordColor[5] = 0xffff;
    }
    if(Runstatus == 13)
    {
        Runstatus =Runstatus - 9;
        //         InterSetFous(1);
        //         InterWordColor[1] = i51KitG2COLOR(128, 128, 128);
    }
    if(Runstatus == 14)
    {
        InterWordColor[1] = 0xffff;
        InterWordColor[2] = i51KitG2COLOR(128, 128, 128);
        InterWordColor[3] = i51KitG2COLOR(128, 128, 128);
    }
    if(Runstatus == 16)
    {
        i51AdeOsGetTime(&GetTime.year, &GetTime.month, &GetTime.day ,&GetTime.hour , &GetTime.mins , &GetTime.sec );
        sprintf(TestBuff[0],"Current Time:%d:%d:%d:%d:%d:%d",GetTime.year, GetTime.month, GetTime.day ,GetTime.hour , GetTime.mins , GetTime.sec);
    }
    if(Runstatus == 17) //dotpay
    {
        i51DotpayDo();
    }
}

void InterKeyBack( )
{
    iU8 inputStatu = 0;
    if(InputStatus[0]==2&&InputStatus[1]==2)//&&InputStatus[2]==2)
    {
        inputStatu = 1;//suncceed
    }
    else if(InputStatus[0]==0&&InputStatus[1]==0)//&&InputStatus[2]==0)
    {
        inputStatu = 2;//untest
    }
    else 
    {
        inputStatu = 0;//failed
    }

    Runstatus = 0;
    InterSetFous(1);
    if(SMSStatus==1||SocketStatus==1||MMIStatus==1||ShockStatus==1||inputStatu==0||AllStatics[3]==1||HookStatus==1||TimeStatus==1)
    {
        AllStatics[4] = 3;
    }
    else if(SMSStatus==2&&SocketStatus==2&&MMIStatus==2&&ShockStatus==2&&inputStatu==1&&HookStatus==2&&TimeStatus==2)
    {
        if(AllStatics[3]==0)
        {
            AllStatics[4] = 1;
        }
        else
        {
            AllStatics[4] = AllStatics[3];
        }
    }
    else if(SMSStatus==0&&SocketStatus==0&&MMIStatus==0&&ShockStatus==0&&inputStatu==2&&HookStatus==0&&TimeStatus==0&&AllStatics[3]==0)
    {
        AllStatics[4] = 0;
    }
    else
    {
        AllStatics[4] = 1;
    }    
    AllStatics[6] = InputStatus[0];
    AllStatics[7] = InputStatus[1];
    AllStatics[8] = InputStatus[2];
    AllStatics[9] = SMSStatus;
    AllStatics[10] = SocketStatus;
    AllStatics[11] = MMIStatus;
    AllStatics[12] = ShockStatus;
    AllStatics[13] = HookStatus;
    AllStatics[14] = TimeStatus;

    i51SetAllStatics();
}

void i51InterTTouchDo ( iU16 event , iU32 value )
{

    iU32 x= 0 , y = 0 ;
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    static iBOOL IsDown[10] = {0};

    i51AdeMmiGetScreenScale ( &width , &height ) ;

    HeightHead = height/10;
    HeightEnd = height/12;
    HeightPerOne = (height -HeightHead -HeightEnd)/9;

    x =  ( value >>16 ) & 0x0000ffff ;
    y = value & 0x0000ffff ;

    if(y>height -HeightEnd &&y<height)
    {
        if(x>width/2&&x<width)
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown[8])
                {
                    IsDown[8] = iFALSE;
                    InterKeyBack();
                }
                CloseColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                CloseColor = i51KitG2COLOR(150,150,150);
                IsDown[8] = iTRUE;
            }
        }
        else
        {
            if( i51_MSG_PADUP == event)
            {
                if(IsDown[9])
                {
                    IsDown[9] = iFALSE;
                    InterKeyDo(InterFoucs-1);
                }
                OkColor = 0xffff;
            }
            else if(i51_MSG_PADDOWN == event)
            {
                OkColor = i51KitG2COLOR(150,150,150);
                IsDown[9] = iTRUE;
            }
        }
        return;
    }
    for(i = 0;i<9;i++)
    {
        if ( x > 0  && x <width &&y >=HeightHead+HeightPerOne*i &&y < HeightHead+HeightPerOne*(i+1))
        {
            if(i51_MSG_PADDOWN == event)
            {
                IsDown[i] = iTRUE;
            }
            else if(i51_MSG_PADUP == event&&IsDown[i] )
            {
                memset(IsDown,0,sizeof(IsDown));
                InterKeyDo(i);
            }
        }
    }
}

static void DrawDownLoad(iRECT re)
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

    rect.X = re.X;
    rect.Y = re.Y;
    rect.Height = 48;
    rect.Width = Twidth - 20;
    i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);    
    i51KitG2FontDraw (TypeheadHandle,2,"[NO]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
}

#if 0
static void DrawBottom()
{
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 StrWidth = 0 ,StrHeight = 0;
    iU16 HeightEnd = 0;
    iRECT rect = {0};

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;

    if(OkColor== 0xffff)
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =6;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(OkColor,&rect);

        rect.X =6+1;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =6+StrWidth+4;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =6+20;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2;
        rect.Height = HeightEnd;
        rect.Width = width/2;
        DrawText(width ,HeightEnd ,L"Ok",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =6+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(OkColor,&rect);

        rect.X =6+1+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4+2;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =6+StrWidth+4+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =6+20;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
        rect.Height = HeightEnd;
        rect.Width = width/2;
        DrawText(width ,HeightEnd ,L"Ok",rect.X ,rect.Y ,0);
    }


    if(CloseColor == 0xffff)
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =width - StrWidth-12;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =width - StrWidth-12+1;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =width - StrWidth-12+StrWidth+4;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =width - StrWidth-10;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2;
        rect.Height = HeightEnd;
        rect.Width = width/2;
        DrawText(rect.Width  ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =width - StrWidth-12+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =width - StrWidth-12+1+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4+2;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =width - StrWidth-12+StrWidth+4+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =width - StrWidth-10+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
        rect.Height = HeightEnd;
        rect.Width = width/2;
        DrawText(rect.Width  ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,0);
    }
}
#endif
static void DrowInterStatus(iRECT rect,iU8 Line)
{
    iRECT rrcte = {0};
    iU16 StrWidth = 0,StrHeight = 0;
    iU16 NoWidth = 0,NoHeight = 0;

    memcpy(&rrcte,&rect,sizeof(iRECT));
    i51AdeMmiGetTextAttr (L"NO", &NoWidth , &NoHeight);
    switch(Line)
    {
    case 1:
        {
            if(InputStatus[0]==0&&InputStatus[1]==0)//&&InputStatus[2]==0)
            {
            }
            else if(InputStatus[0]==2&&InputStatus[1]==2)//&&InputStatus[2]==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    case 2:
        {
            if(SMSStatus==0)
            {
            }

            else if(SMSStatus==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    case 3:
        {
            if(SocketStatus==0)
            {
            }

            else if(SocketStatus==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    case 4:
        {
            if(MMIStatus==0)
            {
            }

            else if(MMIStatus==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    case 5:
        {
            if(CallStatus==0)
            {
            }

            else if(CallStatus==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    case 6:
        {
            if(ShockStatus==0)
            {
            }

            else if(ShockStatus==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    case 7:
        {
            if(HookStatus==0)
            {
            }

            else if(HookStatus==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    case 8:
        {
            if(TimeStatus==0)
            {
            }

            else if(TimeStatus==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    case 9:
        {
            if(AllStatics[3]==0)
            {
            }                     
            else if(AllStatics[3]==2)
            {
                i51AdeMmiGetTextAttr (L"[", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"YES",rrcte.X ,rrcte.Y ,i51KitG2COLOR(0,255,0));
                i51AdeMmiGetTextAttr (L"[YES", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
            else
            {
                i51AdeMmiGetTextAttr (L"[NO ]", &StrWidth , &StrHeight);
                DrawText(rrcte.Width ,rrcte.Height ,L"[",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
                rrcte.X = rect.X + StrWidth/2-NoWidth/2;
                DrawText(rrcte.Width ,rrcte.Height ,L"NO",rrcte.X ,rrcte.Y ,i51KitG2COLOR(240, 65, 85));
                i51AdeMmiGetTextAttr (L"[NO ", &StrWidth , &StrHeight);
                rrcte.X = rect.X + StrWidth;
                DrawText(rrcte.Width ,rrcte.Height ,L"]",rrcte.X ,rrcte.Y ,InterWordColor[Line-1]);
            }
        }
        break;
    }
}

void DrowMenu()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU16 HeightOne = 0;
    iU8 i = 0;
    iU16 StrWidth = 0 ,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;
    HeightOne = (height - HeightHead - HeightEnd)/9; // su_modify

    HeightPerOne = (height - HeightEnd - HeightHead)/9;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();
    //i51AdeMmiGetTextAttr (L"Interactive", &StrWidth , &StrHeight);
    i51AdeMmiGetTextAttr (L"MMI", &StrWidth , &StrHeight);

    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"MMI",rect.X ,rect.Y ,0xffff);


    HeightHead = HeightHead+8;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width-10;
    DrawText(rect.Width ,rect.Height ,L"1.Input Method Test",rect.X ,rect.Y ,InterWordColor[0]);

    i51AdeMmiGetTextAttr (L"[YES]", &StrWidth , &StrHeight);
    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,1);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"2.SMS Test",rect.X ,rect.Y ,InterWordColor[1]);

    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,2);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);


    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"3.Socket Test",rect.X ,rect.Y ,InterWordColor[2]);

    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,3);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[3],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[3],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width-10;
    DrawText(rect.Width ,rect.Height ,L"4.Sound Test",rect.X ,rect.Y ,InterWordColor[3]);
    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,4);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[4],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[4],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"5.Call  Test",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,5);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[5],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[5],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"6.Shock  Test",rect.X ,rect.Y ,InterWordColor[5]);
    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,6);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[6],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[6],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"7.Hook Test",rect.X ,rect.Y ,InterWordColor[6]);
    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,7);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[7],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[7],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"8.Set Time",rect.X ,rect.Y ,InterWordColor[7]);
    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,8);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne-2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[8],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne+12;
    rect.Height = 9;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[8],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"9.Dotpay Test",rect.X ,rect.Y ,InterWordColor[8]);
    rect.X = width - StrWidth-5;
    rect.Y = HeightHead+i*HeightOne;
    rect.Height = HeightPerOne;
    rect.Width = StrWidth;
    DrowInterStatus(rect,9);
    i++;

    //DrawBottom();    
    DrawTowButton();
    i51AdeMmiUpdateScreen();
}

#if 0
void DrowSMSTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0, StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/8;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"SMS", &StrWidth , &StrHeight);

    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"SMS",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Press To Input PhoneNumber",rect.X ,rect.Y ,InterWordColor[0]);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[0] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[1] ,0,&rect , &rect,0, 0,  0,  InterWordColor[1] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[2] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[3] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[4] ,0,&rect , &rect,0, 0,  0,  InterWordColor[2] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[5] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[6] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    ///DrawBottom();    
    DrawTowButton();
    if(OkColor== 0xffff)
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =6;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(OkColor,&rect);

        rect.X =6+1;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =6+StrWidth+4;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =6+20;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2;
        rect.Height = HeightEnd;
        rect.Width = width/2;
        if(!SMSReset)
            DrawText(width ,HeightEnd ,L"Ok",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
        else
        {
            rect.X =6+5;
            DrawText(width ,HeightEnd ,L"Reset",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
        }
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =6+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(OkColor,&rect);

        rect.X =6+1+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4+2;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =6+StrWidth+4+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =6+20;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
        rect.Height = HeightEnd;
        rect.Width = width/2;
        //DrawText(width ,HeightEnd ,L"Ok",rect.X ,rect.Y ,0);
        if(!SMSReset)
            DrawText(width ,HeightEnd ,L"Ok",rect.X ,rect.Y ,0);
        else
        {
            rect.X =6+5;
            DrawText(width ,HeightEnd ,L"Reset",rect.X ,rect.Y ,0);
        }
    }


    if(CloseColor == 0xffff)
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =width - StrWidth-12;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =width - StrWidth-12+1;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4;
        rect.Height = 3;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =width - StrWidth-12+StrWidth+4;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-1;
        rect.Height = StrHeight+5;
        rect.Width = 3;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);

        rect.X =width - StrWidth-10;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2;
        rect.Height = HeightEnd;
        rect.Width = width/2;
        DrawText(rect.Width  ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));
    }
    else
    {
        i51AdeMmiGetTextAttr (L"Cancel", &StrWidth , &StrHeight);
        rect.X =width - StrWidth-12+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+2;
        rect.Height = StrHeight+4;
        rect.Width = StrWidth+4;
        i51KitG2CleanScreen(CloseColor,&rect);

        rect.X =width - StrWidth-12+1+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-2+StrHeight+4+2;
        rect.Height = 2;
        rect.Width = StrWidth+5;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =width - StrWidth-12+StrWidth+4+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2-1+2;
        rect.Height = StrHeight+5;
        rect.Width = 2;
        i51KitG2CleanScreen(i51KitG2COLOR(90,90,90),&rect);

        rect.X =width - StrWidth-10+2;
        rect.Y = (height-HeightEnd) +HeightEnd/2 -StrHeight/2+2;
        rect.Height = HeightEnd;
        rect.Width = width/2;
        DrawText(rect.Width  ,HeightEnd ,L"Cancel",rect.X ,rect.Y ,0);
    }
    i51AdeMmiUpdateScreen();
}
#else
void DrowSMSTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0, StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/8;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"SMS", &StrWidth , &StrHeight);

    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"SMS",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Press To Input PhoneNumber",rect.X ,rect.Y ,InterWordColor[0]);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[0] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[1] ,0,&rect , &rect,0, 0,  0,  InterWordColor[1] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[2] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[3] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[4] ,0,&rect , &rect,0, 0,  0,  InterWordColor[2] , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[5] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[6] ,0,&rect , &rect,0, 0,  0,  0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[7],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[8],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[9],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    ///DrawBottom();    
    DrawTowButton();
    i51AdeMmiUpdateScreen();
}
#endif

void DrowInputTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 DrawWidth = 0;
    iU16 DrawHeight = 0;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iCOLOR color = 0;
    iU16 StrWidth = 0 , StrHeight = 0;
    iU16 PrintBuff[5] = {0};
    static iU16 TestCBuff[]={0x0033,0x002e,0x0049,0x006e,0x0070,0x0075,0x0074,0x0020,0x0053,0x0074,0x0072,0x0069,0x006e,0x0067,0x003a,0x6d4b,0x8bd5,0x00};

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/9;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Input Method", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Input Method",rect.X ,rect.Y ,0xffff);
    //i51KitG2FontGetDemension (TypeheadHandle,2,3,0,0,0,0,&DrawWidth,&DrawHeight);
    i51AdeMmiGetTextAttr(L"YES", &DrawWidth , &DrawHeight);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);    
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"1.input string:test",rect.X ,rect.Y ,InterWordColor[0]);

    rect.X = width - DrawWidth-11;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    if(ColorFlag0==0)
        color = i51KitG2COLOR(240, 65, 85);
    else 
        color = i51KitG2COLOR(0,255,0);
    i51AdeStdAscii2Unicode(TestBuff[2] ,PrintBuff);
    DrawText(rect.Width ,rect.Height ,PrintBuff,rect.X ,rect.Y ,color);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width-10;
    DrawText(rect.Width ,rect.Height ,L"2.input Number:1234",rect.X ,rect.Y ,InterWordColor[1]);

    rect.X = width - DrawWidth-11;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    if(ColorFlag1==0)
        color = i51KitG2COLOR(240, 65, 85);
    else 
        color = i51KitG2COLOR(0,255,0);
    memset(PrintBuff,0,sizeof(PrintBuff));
    i51AdeStdAscii2Unicode(TestBuff[3] ,PrintBuff);
    DrawText(rect.Width ,rect.Height ,PrintBuff,rect.X ,rect.Y ,color);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);
    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,TestCBuff,rect.X ,rect.Y ,InterWordColor[2]);

    rect.X = width - DrawWidth-11;
    rect.Y = HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    if(ColorFlag2==0)
        color = i51KitG2COLOR(240, 65, 85);
    else 
        color = i51KitG2COLOR(0,255,0);
    memset(PrintBuff,0,sizeof(PrintBuff));
    i51AdeStdAscii2Unicode(TestBuff[4] ,PrintBuff);
    DrawText(rect.Width ,rect.Height ,PrintBuff,rect.X ,rect.Y ,color);
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = 24;
    rect.Width = width;
    memset(TestBuff[5],0,sizeof(TestBuff[5]));
    sprintf(TestBuff[5],"提示: ");
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[5],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = 24;
    rect.Width = width;
    memset(TestBuff[6],0,sizeof(TestBuff[6]));
    sprintf(TestBuff[6],"  第三项中文输入法测试可选,");
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[6],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = 24;
    rect.Width = width;
    memset(TestBuff[7],0,sizeof(TestBuff[7]));
    sprintf(TestBuff[7],"  样机不支持中文时,可忽略.");
    i51KitG2FontDraw (TypeheadHandle,2, (void *)TestBuff[7],0,&rect , &rect,0, 0,  0,  0xffff , 0, 0); 
    i++;

    //DrawBottom();    
    DrawTowButton();

    i51AdeMmiUpdateScreen();
}

void DrowSocketTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0, StrHeight = 0;
    iU32 DrawWidth = 0;
    iU32 DrawHeight = 0;
    iU32 a = 0;
    iU32 b = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/8;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Socket Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Socket Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Press To DownLoad",rect.X ,rect.Y ,InterWordColor[0]);
    i++;

    i51KitG2FontGetDemension (TypeheadHandle,2,5,0,0,0,0,&DrawWidth,&DrawHeight);
    DrawWidth = DrawWidth/2;
    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,&TestBuff[0][1],0,&rect , &rect,0, 0,  0,0xffff, 0, 0);  

    rect.X = width -DrawWidth-5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    if(TestBuff[0][0]==1)
        i51KitG2FontDraw (TypeheadHandle,2,"[YES]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    else  if(TestBuff[0][0]==2)
        i51KitG2FontDraw (TypeheadHandle,2,"[NO]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,&TestBuff[1][1],0,&rect , &rect,0, 0,  0,0xffff, 0, 0);

    rect.X = width -DrawWidth-5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    if(TestBuff[1][0]==1)
        i51KitG2FontDraw (TypeheadHandle,2,"[YES]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    else  if(TestBuff[1][0]==2)
        i51KitG2FontDraw (TypeheadHandle,2,"[NO]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,&TestBuff[2][1],0,&rect , &rect,0, 0,  0,0xffff, 0, 0);

    rect.X = width -DrawWidth-5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    if(TestBuff[2][0]==1)
        i51KitG2FontDraw (TypeheadHandle,2,"[YES]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    else  if(TestBuff[2][0]==2)
        i51KitG2FontDraw (TypeheadHandle,2,"[NO]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,&TestBuff[3][1],0,&rect , &rect,0, 0,  0,0xffff, 0, 0);

    rect.X = width -DrawWidth-5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    if(TestBuff[3][0]==1)
        i51KitG2FontDraw (TypeheadHandle,2,"[YES]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    else  if(TestBuff[3][0]==2)
        i51KitG2FontDraw (TypeheadHandle,2,"[NO]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,&TestBuff[4][1],0,&rect , &rect,0, 0,  0,0xffff, 0, 0);

    rect.X = width -DrawWidth-5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    if(TestBuff[4][0]==1)
        i51KitG2FontDraw (TypeheadHandle,2,"[YES]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    else  if(TestBuff[4][0]==2)
        i51KitG2FontDraw (TypeheadHandle,2,"[NO]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,&TestBuff[5][1],0,&rect , &rect,0, 0,  0,0xffff, 0, 0);

    rect.X = width -DrawWidth-5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    if(TestBuff[5][0]==1)
        i51KitG2FontDraw (TypeheadHandle,2,"[YES]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    else  if(TestBuff[5][0]==2)
        i51KitG2FontDraw (TypeheadHandle,2,"[NO]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,&TestBuff[6][1],0,&rect , &rect,0, 0,  0,0xffff, 0, 0);

    rect.X = width -DrawWidth-5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;

    if(TestBuff[6][6]==2)
        i51KitG2FontDraw (TypeheadHandle,2,"[NO]",0,&rect , &rect,0, 0,  0,0xffff, 0, 0); 
    i++;

    if(TestBuff[6][0]==1)
    {
        if(RecAllLen==0)a=0;
        else
        {
            a = (RecedLen < RecAllLen) ? RecedLen : RecAllLen;
            a = (a*100)/RecAllLen;
        }
        memset(TestBuff[9],0,sizeof(TestBuff[9]));
        TestBuff[9][0]='%';
        sprintf(&TestBuff[9][1],"%d",a);

        rect.X = 10;
        rect.Y =HeightHead+i*24;
        rect.Height = 24;
        rect.Width = width-20;
        i51KitG2CleanScreen(i51KitG2COLOR(180,180,180),&rect);    

        rect.X = 10;
        rect.Y =HeightHead+i*24;
        rect.Height = 24;
        rect.Width = ((width-20)*a) / 100;
        i51KitG2CleanScreen(i51KitG2COLOR(0,255,0),&rect);

        rect.X = 10+(width-20)/2-10;
        rect.Y =HeightHead+i*24+4;
        rect.Height = 24;
        rect.Width = (width-20)/2;
        i51KitG2FontDraw (TypeheadHandle,2,TestBuff[9],0,&rect , &rect,0, 0,  0,0, 0, 0); 
        i++;

        rect.X = 5;
        rect.Y =HeightHead+i*24;
        rect.Height = 24;
        rect.Width = width;
        i51KitG2FontDraw (TypeheadHandle,2,TestBuff[7],0,&rect , &rect,0, 0,  0,0xffff, 0, 0);
    }

    //DrawBottom();    
    DrawTowButton();    
    i51AdeMmiUpdateScreen();
}

void DrowMakeCallTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/9;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Call  Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Call  Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"1.Start Call",rect.X ,rect.Y ,i51KitG2COLOR(128, 128, 128));//InterWordColor[0]
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"2 Stop Call",rect.X ,rect.Y ,InterWordColor[1]);
    i++;

    i++;
    HeightHead = HeightHead+i*HeightPerOne;
    i = 0;
    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[0] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[1] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[2] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[3] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[4] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[5] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[6] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[7] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[8] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    //DrawBottom();    
    DrawTowButton();
    i51AdeMmiUpdateScreen();
}

void DrowShockTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/9;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Shock  Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Shock  Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"1 Input Shock Time(s)",rect.X ,rect.Y ,InterWordColor[0]);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[1],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[1],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"2 Enable Shock",rect.X ,rect.Y ,InterWordColor[1]);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[2],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[2],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width-10;
    DrawText(rect.Width ,rect.Height ,L"3 Do Shock",rect.X ,rect.Y ,InterWordColor[2]);
    i++;

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[3],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*HeightPerOne+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[3],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = HeightPerOne;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"4 Disable Shock",rect.X ,rect.Y ,InterWordColor[3]);
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[0] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    //i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne+24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[1] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    //i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne+48;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[2] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    //i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne+72;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[3] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    //i++;

    rect.X = 5;
    rect.Y =HeightHead+i*HeightPerOne+96;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,1,(void *)TestBuff[4] ,0,&rect , &rect,0, 0,  0,  i51KitG2COLOR(240, 65, 85) , 0, 0);  
    i++;

    //DrawBottom();    
    DrawTowButton();
    i51AdeMmiUpdateScreen();
}

void DrawHookTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/9;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Hook  Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Hook  Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);
    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"1 Set Hook",rect.X ,rect.Y ,InterWordColor[0]);
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[0] ,0,&rect , &rect,0, 0,  0, 0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[1] ,0,&rect , &rect,0, 0,  0, 0xffff, 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[2] ,0,&rect , &rect,0, 0,  0, 0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[3] ,0,&rect , &rect,0, 0,  0, 0xffff , 0, 0);  
    i++;

    //DrawBottom();    
    DrawTowButton();
    i51AdeMmiUpdateScreen();
}

void DrawSetTimeTest()
{
    iRECT rect = {0};
    iU16 width = 0 ;
    iU16 height = 0 ;
    iU16 HeightPerOne = 0;
    iU16 HeightHead = 0;
    iU16 HeightEnd = 0;
    iU8 i = 0;
    iU16 StrWidth = 0,StrHeight = 0;

    i51AdeMmiGetScreenScale ( &width , &height ) ;
    HeightEnd = height/12;
    HeightHead = height/10;

    HeightPerOne = (height - HeightEnd - HeightHead)/8;

    i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),0);
    i51PhoneDrawRect();

    i51AdeMmiGetTextAttr (L"Set Time Test", &StrWidth , &StrHeight);
    rect.X = width/2-StrWidth/2;
    rect.Y = HeightHead/2-StrHeight/2;
    rect.Height = HeightHead;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"Set Time Test",rect.X ,rect.Y ,0xffff);

    rect.X = 5;
    rect.Y = HeightHead+i*24+2;
    rect.Height = 14;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor0[0],&rect);

    rect.X = 5;
    rect.Y = HeightHead+i*24+16;
    rect.Height = 8;
    rect.Width = width-10;
    i51KitG2CleanScreen(InterColor1[0],&rect);

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    DrawText(rect.Width ,rect.Height ,L"1 Set Time",rect.X ,rect.Y ,InterWordColor[0]);
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24+2;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,"Input Format:yyyy,mm,dd,hh,mm,ss" ,0,&rect , &rect,0, 0,  0, 0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[0] ,0,&rect , &rect,0, 0,  0, 0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[1] ,0,&rect , &rect,0, 0,  0, 0xffff, 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[2] ,0,&rect , &rect,0, 0,  0, 0xffff , 0, 0);  
    i++;

    rect.X = 5;
    rect.Y =HeightHead+i*24;
    rect.Height = 24;
    rect.Width = width;
    i51KitG2FontDraw (TypeheadHandle,2,(void *)TestBuff[3] ,0,&rect , &rect,0, 0,  0, 0xffff , 0, 0);  
    i++;

    //DrawBottom();    
    DrawTowButton();
    i51AdeMmiUpdateScreen();
}

static void PlayCallback(void* handle)
{
    iCHAR printbuff[64] = {0};

    i51AdeOsLog(0,"play callback...");
    PlayEndTime = i51AdeOsGetTick();
    sprintf(printbuff,"Play end Cost Time:%dms",PlayEndTime-OldSysTime);
    DrowG2TestLog(printbuff);

    if(i51AdeMmiMediaPlayerRelease())
    {
        i51AdeOsLog(0,"play stop...");
        MediaOpen = 0;
        DrowG2TestLog("Play Stoped");
        if(MTestStatus < 4)
            MTestStatus++;
    }
    else
    {
        MediaOpen = 1;
        i51AdeOsLog(0,"Play stop failed ");
        DrowG2TestLog("Play Stop failed");
    }
}
static void PlayTimerCallback(iU32 ID)
{
    i51AdeMmiMediaPlayerStop(Handle);
    i51AdeMmiMediaPlayerRelease();
    i51AdeOsTimerStop(PlayMediaTime);
    PlayMediaTime = 0;
    i51AdeOsLog(0,"play stop...");
    MediaOpen = 0;
    DrowG2TestLog("Play Stoped");
    if(MTestStatus < 4)
        MTestStatus++;
}
static void  CreateCallback(iBOOL result)
{
    iCHAR printfbuff[64] = {0};
    if(result)
    {
        switch(MTestStatus)
        {
        case 0://wav memory
            {
                if (i51_ADE_CNF_VER_CS == i51AdeCnfGetType())
                {
                    CurrSysTime = i51AdeOsGetTick();
                    MediaResult[0] = CurrSysTime - OldSysTime;

                    i51AdeOsLog(0,"TestMediaSpeed , CPU type is MIPS, don't test wav memory:%dms",MediaResult[0]);
                    memset(printfbuff,0,sizeof(printfbuff));
                    sprintf(printfbuff,"MIPS, don't test wav memory.");
                    DrowG2TestLog(printfbuff);

                    PlayCallback(0);
                }
                else
                {
                    OldSysTime = i51AdeOsGetTick();
                    if(i51AdeMmiMediaPlayerPlay( 0 , (void*)pSoundDataStdWav , SoundDataStdWavLength ,1, i51_MMI_DEC_MEM_BASED ,1 , PlayCallback ))//19528, loop = 1通过SPRD_6620
                    {
                         Handle = (void*)pSoundDataStdWav;
                        CurrSysTime = i51AdeOsGetTick();
                        MediaResult[0] = CurrSysTime - OldSysTime;
                        PlayMediaTime = i51AdeOsTimerCreate (0,1) ;
                        i51AdeOsTimerStart(PlayMediaTime,PlayTimerCallback,3000);

                        i51AdeOsLog(0,"TestMediaSpeed play wav memory play :%dms",MediaResult[0]);
                        memset(printfbuff,0,sizeof(printfbuff));
                        sprintf(printfbuff,"Play Wav With Memory Cost Time:%dms",MediaResult[0]);
                        DrowG2TestLog(printfbuff);
                        DrowG2TestLog("Playing.......");
                    }
                    else
                    {
                        i51AdeOsLog(0,"TestMediaSpeed play wav memory play filed");
                        MediaOpen = 0;
                        DrowG2TestLog("Play Wav With Memory failed");
                    }
                }
            }
            break;
        case 1://wav File
            {
                OldSysTime = i51AdeOsGetTick();
                if(i51AdeMmiMediaPlayerPlay( 0 , (void*)SoundDataStdWavPath , strlen((iCHAR*)SoundDataStdWavPath) ,1, i51_MMI_DEC_FILE_BASED , 1 , PlayCallback )) // loop = 1通过SPRD_6620
                {
                     Handle = (void*)SoundDataStdWavPath;
                    CurrSysTime = i51AdeOsGetTick();
                    MediaResult[1] = CurrSysTime - OldSysTime;
                    PlayMediaTime = i51AdeOsTimerCreate (0,1) ;
                    i51AdeOsTimerStart(PlayMediaTime,PlayTimerCallback,3000);

                    i51AdeOsLog(0,"TestMediaSpeed play wav file play. :%dms",MediaResult[1]);
                    memset(printfbuff,0,sizeof(printfbuff));
                    sprintf(printfbuff,"Play Wav With File Cost Time:%dms",MediaResult[1]);
                    DrowG2TestLog(printfbuff);
                    DrowG2TestLog("Playing.......");
                }
                else
                {
                    i51AdeOsLog(0,"TestMediaSpeed play wav file play filed");
                    MediaOpen = 0;
                    DrowG2TestLog("Play Wav With File failed");
                }
            }
            break;
        case 2://Mid memory
            {
                if (i51_ADE_CNF_VER_CS == i51AdeCnfGetType())
                {
                    CurrSysTime = i51AdeOsGetTick();
                    MediaResult[0] = CurrSysTime - OldSysTime;

                    i51AdeOsLog(0,"TestMediaSpeed , CPU type is MIPS, don't test Mid memory:%dms",MediaResult[0]);
                    memset(printfbuff,0,sizeof(printfbuff));
                    sprintf(printfbuff,"MIPS, don't test Mid memory.");
                    DrowG2TestLog(printfbuff);

                    PlayCallback(0);
                }
                else
                {
                    OldSysTime = i51AdeOsGetTick();
                    if(i51AdeMmiMediaPlayerPlay( 0 , (void*)SoundDataStdMid , SoundDataStdMidLength ,0, i51_MMI_DEC_MEM_BASED , 1 , PlayCallback )) // loop = 1通过SPRD_6620
                    {
                        Handle = (void*)SoundDataStdMid;
                        CurrSysTime = i51AdeOsGetTick();
                        MediaResult[2] = CurrSysTime - OldSysTime;
                        PlayMediaTime = i51AdeOsTimerCreate (0,1) ;
                        i51AdeOsTimerStart(PlayMediaTime,PlayTimerCallback,3000);

                        i51AdeOsLog(0,"TestMediaSpeed play mid memory play :%dms",MediaResult[2]);
                        memset(printfbuff,0,sizeof(printfbuff));
                        sprintf(printfbuff,"Play Mid With Memory Cost Time:%dms",MediaResult[2]);
                        DrowG2TestLog(printfbuff);
                        DrowG2TestLog("Playing.......");
                    }
                    else
                    {
                        i51AdeOsLog(0,"TestMediaSpeed play mid memory play filed");
                        MediaOpen = 0;
                        DrowG2TestLog("Play Mid With Memory Failed");
                    }
                }
            }
            break;
        case 3://Mid File
            {
                if (i51_ADE_CNF_VER_CS == i51AdeCnfGetType())
                {
                    CurrSysTime = i51AdeOsGetTick();
                    MediaResult[0] = CurrSysTime - OldSysTime;

                    i51AdeOsLog(0,"TestMediaSpeed , CPU type is MIPS, don't test mid file:%dms",MediaResult[0]);
                    memset(printfbuff,0,sizeof(printfbuff));
                    sprintf(printfbuff,"MIPS, don't test mid file.");
                    DrowG2TestLog(printfbuff);

                    PlayCallback(0);
                }
                else
                {
                    OldSysTime = i51AdeOsGetTick();
                    if(i51AdeMmiMediaPlayerPlay( 0 , (void*)SoundDataStdMidPath , strlen((iCHAR*)SoundDataStdMidPath) ,0,i51_MMI_DEC_FILE_BASED , 1 , PlayCallback )) // loop = 1通过SPRD_6620
                    {
                        Handle = (void*)SoundDataStdMidPath;
                        CurrSysTime = i51AdeOsGetTick();
                        MediaResult[3] = CurrSysTime - OldSysTime;
                        PlayMediaTime = i51AdeOsTimerCreate (0,1) ;
                        i51AdeOsTimerStart(PlayMediaTime,PlayTimerCallback,3000);
                        i51AdeOsLog(0,"TestMediaSpeed play mid file play :%dms",MediaResult[3]);
                        memset(printfbuff,0,sizeof(printfbuff));
                        sprintf(printfbuff,"Play Mid With File Cost Time:%dms",MediaResult[3]);
                        DrowG2TestLog(printfbuff);
                        DrowG2TestLog("Playing.......");
                    }
                    else
                    {
                        i51AdeOsLog(0,"TestMediaSpeed play mid file play filed");
                        MediaOpen = 0;
                        DrowG2TestLog("Play Mid With File Play failed");
                    }
                }
            }
            break;
        }
    }
}

void TestMediaSpeed()
{
    iFILE file = 0;
    iU32 ReadLen = 0;

    if(MediaOpen==1)
    {
        return;
    }
    MediaOpen = 1;
    i51AdeMmiMediaPlayerSetVolume(7);
    switch(MTestStatus)
    {
    case 0:
        {
            YPoint = 0;
            i51AdeOsLog(0,"TestMediaSpeed play wav memory create");
            DrowG2TestLog("Play Wav With Memory Create");
            //OldSysTime = i51AdeOsGetTick();
            i51AdeMmiMediaPlayerCreate(CreateCallback); 
        }
        break;
    case 1:
        {
            file = i51AdeOsFileOpen (SoundDataStdWavPath, i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
            if(file!=0)
            {
                i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay i51AdeOsFileOpen succeed");
                i51AdeOsFileWrite(file, (void *)SoundDataStdWav , SoundDataStdWavLength, &ReadLen) ;
                i51AdeOsFileClose(file);
            }
            DrowG2TestLog("Play Wav With File Create");
            i51AdeOsLog(0,"TestMediaSpeed play wav file create");
            //OldSysTime = i51AdeOsGetTick();
            i51AdeMmiMediaPlayerCreate(CreateCallback); 

        }
        break;
    case 2:
        {
            i51AdeOsLog(0,"TestMediaSpeed play mid memory create");
            DrowG2TestLog("Play Mid With Memory Create");
            //OldSysTime = i51AdeOsGetTick();
            i51AdeMmiMediaPlayerCreate(CreateCallback); 
        }
        break;
    case 3:
        {
            file = i51AdeOsFileOpen (SoundDataStdMidPath, i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
            if(file!=0)
            {
                i51AdeOsLog(0,"i51AdeMmiMediaPlayerPlay i51AdeOsFileOpen succeed");
                i51AdeOsFileWrite(file, (void *)SoundDataStdMid, SoundDataStdMidLength, &ReadLen) ;
                i51AdeOsFileClose(file);
            }
            DrowG2TestLog("Play Mid With File Create");
            //OldSysTime = i51AdeOsGetTick();
            i51AdeOsLog(0,"TestMediaSpeed play mid File create");
            i51AdeMmiMediaPlayerCreate(CreateCallback); 
        }
        break;
    case 4:
        {
            BenchmarkState = BENCHMARK_G2;
            MediaOpen = 0;
        }
        break;
    }
}

static iS32 i51HookCallback (iS32 simCard , iCHAR* number , iU32 numlen , void* buffer , iU32 textlen ) 
{
    i51AdeOsLog(0,"received a message from:%s",number);
    sprintf(TestBuff[2],"Recieved A Message ");
    sprintf(TestBuff[3],"Hook Test Succeed");
    HookStatus = 2;
    if(smshookid)
    {
        i51AdeSmsReleaseHook(smshookid);
        smshookid = 0;
    }
    return 1;
}
void SetHook()
{
    smshookid = i51AdeSmsCreateHook (0) ; 
    if(i51AdeSmsSetHook (smshookid , i51HookCallback ))
    {
        i51AdeOsLog(0,"SetHook Succeed,ID:%d",smshookid);
        sprintf(TestBuff[0],"SetHook Succeed,Hook ID:%d",smshookid);
        sprintf(TestBuff[1],"Send a Message To This Phone.....");
        HookStatus = 2;
    }
    else
    {
        sprintf(TestBuff[0],"SetHook FAILED");
        HookStatus = 1;
    }
}
void SetTime()
{
    TimeStruct GetTime = {0};
    sprintf(TestBuff[1],"Set Time:%d:%d:%d:%d:%d:%d",SetOsTime.year, SetOsTime.month, SetOsTime.day ,SetOsTime.hour , SetOsTime.mins , SetOsTime.sec);
    if(i51AdeOsSetTime(SetOsTime.year, SetOsTime.month, SetOsTime.day ,SetOsTime.hour , SetOsTime.mins , SetOsTime.sec ))
    {
        i51AdeOsLog(0,"Set Time Succeed");
        sprintf(TestBuff[2],"Set OS Time Succeed");
        memset(&GetTime,0,sizeof(TimeStruct));
        i51AdeOsGetTime(&GetTime.year, &GetTime.month, &GetTime.day ,&GetTime.hour , &GetTime.mins , &GetTime.sec );
        sprintf(TestBuff[3],"Get Time %d:%d:%d:%d:%d:%d",GetTime.year, GetTime.month, GetTime.day ,GetTime.hour , GetTime.mins , GetTime.sec);
        TimeStatus = 2;
    }
    else
    {
        i51AdeOsLog(0,"Set Time Failed");
        sprintf(TestBuff[2],"Set OS Time Failed");
        TimeStatus = 1;
    }
}
