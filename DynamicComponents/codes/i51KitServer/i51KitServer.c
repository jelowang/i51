#include "i51KitAde.h"
#include <stdio.h>

i51KIT_STATIC_KIT_DEF(Adapter);
typedef struct SIMS_MESSAGE
{
     //	author : steven
    //	since : 2011.10.27

    iU16 Content[16];
    iCHAR PhoneNumber[32];
    iS32 SimCard;
    iU32 ResendTimes;
    iU32 TimeGap;
    iU32 DeviceNum;
    void (*callback)(iBOOL results );
}i51ServerSimsMessage;

typedef struct MESSAGE_QUEUE  
{  
    //	author : steven
    //	since : 2011.10.27

    i51ServerSimsMessage space[10];
    //队头元素下标  
    iU8 front;  
    //队尾元素下标  
    iU8 rear;  
    //标志
    iU8 flag;

}i51ServerSmsQueue;  
typedef struct MONIINFO
{
    //	author : steven
    //	since : 2012.04.10
    iU32 sendtotall; 
    iU32 succtotall;
    iU32 hooktotall;
    iU32 Acepted;
    iU32 Resend;
}MONITORINFO;

typedef struct TIME_RUN
{
    //	author : steven
    //	since : 2011.10.31
    
    iU32 DealTime;
    iU32 Idle;
    iU32 CallTime;
    iCHAR Act[32];
    iU32 LoginTime;
}i51TimeServerMessage;

typedef struct TIME_MESSAGE
{
    //	author : steven
    //	since : 2011.10.31
    
     i51TimeServerMessage timeSer[5];
     iU32 point;
}i51TimerserverStruct;

typedef enum 
{
    //	author : steven
    //	since : 2012.06.03
    
    i51KitServer_Send_NO = 0,
    i51KitServer_Send_Normal,
    i51KitServer_Send_Special
}i51KitServerSendStaus;

typedef struct SCRIPTINFO
{
    //	author : steven
    //	since : 2012.06.03
    
    iU16 Sp[16];
    iU16 Con[16];
    iU16 RebackCon[16];
    iU16 RebackPhone[24];
    iU8 Act;
    iU32 TimeGap;
    iU32 CurrentTime;
}i51Scrit;

typedef struct i51SCRIPTP00L
{
    i51Scrit i51DelteScriptStruct[5];
    i51Scrit i51RebackScriptStruct[5];
    iU8 DelteScriptNum;
    iU8 RebackScriptNum;
}i51ScriptStuct;

typedef struct i51TIMESTRUCT{
	iU16 year;
	iU16 month;
	iU16 day;
	iU16 hour;
	iU16 mins;
	iU16 sec;
}i51TimeStruct;

typedef void (*i51SENDMESSAGECALLBACK)(iBOOL result);

static i51KitServerSendStaus i51SendStatus = i51KitServer_Send_NO;
static iU8 DeviceCreate[5] = {0};
static i51ServerSimsMessage i51CurrentSend = {0};//当前发送的短信内容

static i51ScriptStuct i51ScriptInMemory = {0};//存放10条脚本在内存中
static i51TimeStruct i51TimeInfo = {0};    //取当前时间的结构体
static iU8 i51OldVesion = 0;//如果用老接口  改值为1，原因是老版本接口处理的时间是ms，新的是s

static iU32 TimerID_T2r = 0;
static iS32 smshookid = 0 ;
static iU32 Time_pre = 0;
static iU32 Send_Time = 0;
static iU32 Queue_ID = 0;
static iU8 Send_Complete = 0;
static iU32 callbackid =0;
static iU32 DNumber = 0;
static iS32 i51SimCard = 0;
static iU8 i51GetStatus = 0;
static iU32 MsgDevice = 0;
static iU32 RandData = 3;//延迟发送短信的值,该值最大为18，最小为3
static iU32 i51GetPreTicks = 0;
static iU8 i51NeedCheckMsg = 1;
static iU8 IsIdle = 0;
static iU32 i51RemainTimeGap = 0;
static iU32 i51_gTimer = 15;
static iU64 i51TimerStart = 0;
static iU64 i51TimerEnd = 0;

static i51SENDMESSAGECALLBACK SENDTIMERCALLBACK = NULL;
static iBOOL CALLBACKFALG = iFALSE;
static i51ServerSmsQueue SmsMessage_Queue;
static i51TimerserverStruct TimerStruct;
static MONITORINFO MonitInfo = {0};
static iU16 Set_Script[145] = {0};

static iU8 i51ScritParse(iU16* messageData,i51Scrit *ScrptMsg);
static iU8 i51ServerStrParse(iU16 *phoneNumber,i51Scrit *message,iU16* messageData,iCHAR *act_result);
static iCHAR i51ServerPush(i51ServerSmsQueue *q,iU32 device,iU16 *value,iCHAR *phone,iS32 sim,iU32 time ,void (*callback)(iBOOL results))  ;
static void i51ServerPop(i51ServerSmsQueue *q) ;
static iBOOL i51ServerStrParseIn(iU16 *PhoneNum,iU16* msg,iU16* Covemsg,iCHAR *act_flag);
static iU16 *i51ServerStrstr16(iU16 *str1, iU16 *str2);
static void i51TimerServInit(void);
static iCHAR i51ServerIsEmpty(i51ServerSmsQueue* q);
static void i51ServerQueueTimerCallback(iU32 ID);
static iBOOL i51KitServerSmsSendEx(iU32 device,iS32 sim,iCHAR *phone,iU16 *message,iU32 timegap,void (*callback)(iBOOL results));
iU32 i51KitServerSmsCreateDevice (void) ;
iBOOL i51KitServerSmsReleaseDevice ( iU32 id );
void i51KitServerGetMonitorInfo ( iU32* sendsmstotall , iU32* succsmstotall , iU32* hookedtotall , iU32* Intercepted , iU32* Interceptedsend );
static void i51TimerServerCallback(iU32 ID);


#define i51KITSERVER_DEBUG
#define IDLE_TIMER 1
#define IDLE_MESSAGE 1


static iBOOL i51SendMessageInit(i51ServerSimsMessage * mseeage)
{
    //	author : steven
    //	since : 2011.10.27
    //  (C)TOK
    //  notes : 发送文件里面的短信

    iFILE file = 0;
    iS32 fileSize = 0;
    iS32 lenth =  0;
    iU8 deleteflag = 0;

    lenth = sizeof(i51ServerSimsMessage);
    file = i51AdeOsFileOpen (L"i51KitServerFile\\SendMessageV2.dat" ,i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ ) ;
    if(file == 0)                      
    {
        return iFALSE;
    }
    
    i51AdeOsFileGetSize(file,&fileSize);
    while(fileSize>0)
    {
        memset(mseeage,0,sizeof(i51ServerSimsMessage));
        i51AdeOsFileRead ( file , mseeage,lenth,0) ;
        fileSize =  fileSize - lenth;
        if(mseeage->SimCard==0)
        {
            continue;
        }
        deleteflag = 1;
        break;
    }
    
    i51AdeOsFileClose(file);
    if(deleteflag==0)
    {
        i51AdeOsFileDelete(L"i51KitServerFile\\SendMessageV2.dat");
        return iFALSE;
    }
    return iTRUE;
}

static void CallbackTimer(iU32 id)
{
    if(NULL!=SENDTIMERCALLBACK)
    {
        SENDTIMERCALLBACK(CALLBACKFALG);     
    }
    SENDTIMERCALLBACK = NULL;
    i51AdeOsTimerStop(callbackid);
    callbackid = 0;
}
static void i51ServerCallback ( iBOOL value ) 
{
        //	author : steven
        //	since : 2011.10.27
        //  (C)TOK
        //  notes : CallBack

        iBOOL ReleaseFlag = iFALSE;
        ReleaseFlag  = i51AdeSmsReleaseDevice(DNumber);
        DNumber  = 0;

        if(iTRUE==value)//Succed
        {
                if(NULL != i51CurrentSend.callback)
                {
                      CALLBACKFALG = iTRUE;
                      SENDTIMERCALLBACK = i51CurrentSend.callback;
                      if(callbackid==0)
                      {
                          callbackid = i51AdeOsTimerCreate (0,1) ;
                          i51AdeOsTimerStart(callbackid,CallbackTimer,10);
                      }
                }
                memset(&i51CurrentSend,0,sizeof(i51CurrentSend));
                i51AdeOsLog ( i51_LOG_DC , "send succed");
                MonitInfo.succtotall++;
                Send_Complete = 0;
                i51SendStatus = i51KitServer_Send_NO;
                return ;
        } 
        
        if(i51CurrentSend.ResendTimes>0)
        {
            i51AdeOsLog ( i51_LOG_DC , "resend msg and resendTimes:%d",i51CurrentSend.ResendTimes);
            Send_Complete = 0;
            return ;
        }
        
        if(NULL != i51CurrentSend.callback)
        {
             SENDTIMERCALLBACK = i51CurrentSend.callback;
             CALLBACKFALG = iFALSE;
             if(callbackid==0)
             {
                 callbackid = i51AdeOsTimerCreate (0,1) ;
                 i51AdeOsTimerStart(callbackid,CallbackTimer,10);
             }
        }
        i51SendStatus = i51KitServer_Send_NO;
        memset(&i51CurrentSend,0,sizeof(i51CurrentSend));
        i51AdeOsLog ( i51_LOG_DC , "send failed");
        Send_Complete = 0;
}

static iS32 i51ServerSmsHookCallback (iS32 simCard , iCHAR* number , iU32 numlen , void* buffer , iU32 textlen ) 
{
        //	author : steven
        //	since : 2011.11.7
        //  (C)TOK
        //  notes : 获取短信钩子
        
        iU16 num[32] = {0};
        //static iU16 cont[128] = {0};
        static iU16 cont_cover[145] = {0};
        
        iCHAR flag = 0;
        iU8 i = 0;
       iU16 *tttt = (iU16 *)buffer;
       iU16 data = 0;
       
	   if(textlen>=145)return 0;

       i51AdeStdAscii2Unicode(number,num);
       memset(Set_Script,0,sizeof(Set_Script));
       memset(cont_cover,0,sizeof(cont_cover));
       i51AdeStdMemcpy16(Set_Script , tttt ,i51AdeStdWStrlen(tttt)*2);
       
       MonitInfo.hooktotall++;

       for(i = 0;i<i51AdeStdWStrlen(Set_Script);i++)
       {
           data = (Set_Script[i]<<8)|(Set_Script[i]>>8);
           cont_cover[i] = data;
       }

       i51ServerStrParseIn(num,Set_Script,cont_cover,&flag);

       #ifdef i51KITSERVER_DEBUG      
       i51AdeOsLog ( i51_LOG_DC , " received messgage From:%s,HookCallback:%d" , number,flag) ;
       #endif
       
       return flag ;
}


static iBOOL i51GetCurrentSendMessage(i51ServerSimsMessage * mseeage)
{    
    if(!i51ServerIsEmpty(&SmsMessage_Queue))
    {
        if((i51SendStatus==i51KitServer_Send_Normal&&i51CurrentSend.ResendTimes==3))
        {
            return iTRUE;
        }
        if(!i51SendMessageInit(mseeage))
        {
            return iFALSE;
        }
        i51SendStatus = i51KitServer_Send_Normal;
        i51AdeOsLog ( i51_LOG_DC , "i51KitServer send Normal ");  
    }
    else
    {
        memcpy(mseeage,&SmsMessage_Queue.space[SmsMessage_Queue.front],sizeof(i51ServerSimsMessage)); 
        i51AdeOsLog ( i51_LOG_DC , "i51KitServer send  queue:front,%d,rear,%d",SmsMessage_Queue.front,SmsMessage_Queue.rear); 
        i51ServerPop(&SmsMessage_Queue);
        i51SendStatus = i51KitServer_Send_Special;
    }
    return iTRUE;
}

static iCHAR i51DelMessageInFile()
{
    //	author : steven
    //	since : 2011.10.27
    //  (C)TOK
    //  notes : 发送文件里面的短信

    iFILE file = 0;
    iS32 fileSize = 0;
    iS32 lenth =  0;
    i51ServerSimsMessage msg = {0};
    iCHAR result = 0;

    lenth = sizeof(i51ServerSimsMessage);
    file = i51AdeOsFileOpen (L"i51KitServerFile\\SendMessageV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ ) ;
    if(file == 0)                      
    {
        return 0;
    }
    
    i51AdeOsFileGetSize(file,&fileSize);
    while(fileSize>0)
    {
        memset(&msg,0,lenth);
        i51AdeOsFileRead ( file , &msg,lenth,0) ;
        fileSize =  fileSize - lenth;
        if(msg.SimCard==0)
        {
            continue;
        }
        memset(&msg,0,lenth);
        i51AdeOsFileSetSeek (file,-lenth,i51_OS_FL_SEEK_CURRENT); 
        i51AdeOsFileWrite(file,&msg,lenth,0);
        result = 1;
        break;
    }
    
    i51AdeOsFileClose(file);
    return result;
}

static void i51ServerQueueTimerCallback(iU32 ID)
{
    //	author : steven
    //	since : 2011.11.7
    //  (C)TOK
    //  notes : 定时器回调函数，用来轮询短信发送队列里是否有要发送的短信

    static iU8 TimeOutNumber = 0;
    Send_Time = Send_Time + 3;
    //已经有发送出去的短信，1分钟后超时重发
    //i51AdeOsLog(i51_LOG_DC,"send callback;%d,%d,%d,%d,%d",Send_Complete,i51SendStatus,i51CurrentSend.ResendTimes,Send_Time,i51CurrentSend.TimeGap);
    if(Send_Complete==1)
    {
         if(TimeOutNumber <9)  
         {
             TimeOutNumber++;
             return ;
         }
         Send_Complete = 0;
         i51AdeSmsReleaseDevice(DNumber);
         DNumber  = 0;
         TimeOutNumber = 0;
         if( i51CurrentSend.ResendTimes==0)
         {
            i51SendStatus = i51KitServer_Send_NO;
            return;
         }
         i51AdeOsLog ( i51_LOG_DC , "发送超时:%d,resend",i51CurrentSend.ResendTimes);
    }
        
    else if((i51SendStatus == i51KitServer_Send_NO)||(i51SendStatus==i51KitServer_Send_Normal&&i51CurrentSend.ResendTimes==3))
    {
        if(!i51GetCurrentSendMessage(&i51CurrentSend))
        {
            Send_Time = 0;
            i51AdeOsTimerStop(Queue_ID);
            Queue_ID = 0;
            i51AdeOsLog(i51_LOG_DC,"no message to send");
            return;
        }
        if(i51CurrentSend.ResendTimes>3)
        {
            i51CurrentSend.ResendTimes = 3;
        }
        
    }
    
    TimeOutNumber = 0;
    
    if(Send_Time< i51CurrentSend.TimeGap)
    {
        return;
    }
    Send_Complete = 1;

    if(i51SendStatus==i51KitServer_Send_Normal&&i51CurrentSend.ResendTimes==3)
    {
        if(!i51DelMessageInFile())
        {
            return;
        }
    }
    
    DNumber= i51AdeSmsCreateDevice();

    MonitInfo.sendtotall++;
    i51AdeSmsSend ( DNumber, i51CurrentSend.SimCard, i51CurrentSend.PhoneNumber, i51CurrentSend.Content, iFALSE ,i51ServerCallback );

    
    i51CurrentSend.ResendTimes--;
    i51AdeOsLog ( i51_LOG_DC , "send message phonenum:%s,Send_Time:%d,TimeGap:%d,retimes:%d",i51CurrentSend.PhoneNumber,Send_Time,i51CurrentSend.TimeGap,i51CurrentSend.ResendTimes); 
    Send_Time = 0;
    
}

static void i51InitScriptInfo()
{
    iFILE file = 0;
    iS32 fileSize = 0;
    i51Scrit ScriptParse = {0};

    file = i51AdeOsFileOpen (L"i51KitServerFile\\scriptV2.dat" ,i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ ) ;
    if(file == 0)                      
    {
        return;
    }
    
    i51AdeOsFileGetSize(file,&fileSize);
    while(fileSize>0)
    {
        memset(&ScriptParse,0,sizeof(i51Scrit));
        
        i51AdeOsFileRead(file,&ScriptParse,sizeof(i51Scrit),0);
        
        fileSize = fileSize - sizeof(i51Scrit);
        
        if(ScriptParse.Act == 1)
        {
           if(i51ScriptInMemory.RebackScriptNum<5)
           {
               memcpy(&i51ScriptInMemory.i51RebackScriptStruct[i51ScriptInMemory.RebackScriptNum],&ScriptParse,sizeof(i51Scrit));
               i51ScriptInMemory.RebackScriptNum++;
           }
        }
        else if(ScriptParse.Act == 2)
        {
           if(i51ScriptInMemory.DelteScriptNum<5)
           {
               memcpy(&i51ScriptInMemory.i51DelteScriptStruct[i51ScriptInMemory.DelteScriptNum],&ScriptParse,sizeof(i51Scrit));
               i51ScriptInMemory.DelteScriptNum++;
           }
        }
        if(i51ScriptInMemory.DelteScriptNum>=5&&i51ScriptInMemory.RebackScriptNum>=5)
        {
            break;
        }
    }
    
    i51AdeOsFileClose(file);
}

static void i51UpdateScritFile()
{
    iFILE file = 0;
    iFILE file_new = 0;
    iS32 size = 0;
    i51Scrit ScriptV2 = {0};    
    
    file = i51AdeOsFileOpen (L"i51KitServerFile\\script.dat" ,i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ ) ;
    if(file == 0)                      
    {
        //i51AdeOsLog(i51_LOG_DC,"i51UpdateScritFile return 1");
        return;
    }
    file_new = i51AdeOsFileOpen (L"i51KitServerFile\\scriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ ) ;
    if(file_new == 0)                      
    {
        //i51AdeOsLog(i51_LOG_DC,"i51UpdateScritFile return 2");
        i51AdeOsFileClose(file);
        return;
    }
    
    i51AdeOsFileGetSize(file,&size);
    while(size>0)
    {
        memset(&ScriptV2,0,sizeof(i51Scrit));
        i51AdeOsFileRead(file,Set_Script,256,0);
        size = size - sizeof(i51Scrit);

        if(!i51ScritParse(Set_Script,&ScriptV2))
        {
            continue;
        }
        ScriptV2.TimeGap = ScriptV2.TimeGap/1000;
        ScriptV2.CurrentTime = ScriptV2.CurrentTime/1000;
        
        i51AdeOsFileWrite(file_new,&ScriptV2,sizeof(i51Scrit),0);
    }
    i51AdeOsFileClose(file);
    i51AdeOsFileClose(file_new);
    i51AdeOsFileDelete(L"i51KitServerFile\\script.dat");
}

static void i51UpdateSendFile()
{
    iFILE file = 0;
    iFILE file_new = 0;
    iS32 size = 0;
    iU32 lenth = 0;

    i51ServerSimsMessage message = {0};
    
    file = i51AdeOsFileOpen (L"i51KitServerFile\\SendMessage.dat" ,i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ ) ;
    if(file == 0)                      
    {
        return;
    }
    file_new = i51AdeOsFileOpen (L"i51KitServerFile\\SendMessageV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ ) ;
    if(file_new == 0)                      
    {
        i51AdeOsFileClose(file);
        return;
    }

    lenth = sizeof(i51ServerSimsMessage);
    i51AdeOsFileGetSize(file,&size);
    while(size>0)
    {
        i51AdeOsFileRead ( file ,&message,lenth,0) ;
        size =  size - lenth;
        if(message.SimCard==0)
        {
            continue;
        }
        message.ResendTimes = 3;
        message.TimeGap = message.TimeGap/1000;
        
        i51AdeOsFileWrite(file_new,&message,lenth,0);
    }

    //i51AdeOsLog(i51_LOG_DC,"i51UpdateSendFile return");
    i51AdeOsFileClose(file);
    i51AdeOsFileClose(file_new);
    i51AdeOsFileDelete(L"i51KitServerFile\\SendMessageV2.dat");
}

static void i51UpdateAppFile()
{
    iFILE file = 0;
    iFILE file_new = 0;
    iS32 size = 0;
    iU32 lenth = 0;
    iBOOL re;

    i51TimeServerMessage message = {0};

   
    file = i51AdeOsFileOpen (L"i51KitServerFile\\T2rSetScript.dat" ,i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ ) ;
    if(file == 0)                      
    {
        //i51AdeOsLog(i51_LOG_DC,"i51UpdateAppFile return 1");
        return;
    }
    file_new = i51AdeOsFileOpen (L"i51KitServerFile\\T2rSetScriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ ) ;
    if(file_new == 0)                      
    {
        //i51AdeOsLog(i51_LOG_DC,"i51UpdateAppFile return 2");
        i51AdeOsFileClose(file);
        return;
    }

    lenth = sizeof(i51TimeServerMessage);
    i51AdeOsFileGetSize(file,&size);

    while(size>0)
    {
        i51AdeOsFileRead ( file , &message,sizeof(i51TimeServerMessage) ,0) ;
        size =  size - lenth;

        //i51AdeOsLog(i51_LOG_DC,"i51UpdateAppFile return :%d,%d,%d,%d",message.DealTime,message.Idle,message.CallTime,message.LoginTime);
        message.DealTime = message.DealTime/1000;
        message.Idle = message.Idle/1000;
        message.CallTime = message.CallTime/1000;
        message.LoginTime = message.LoginTime/1000;
        
        re = i51AdeOsFileWrite(file_new,&message,lenth,0);
        //i51AdeOsLog(i51_LOG_DC,"i51UpdateAppFile return :%d,%d,%d,%d,%d",message.DealTime,message.Idle,message.CallTime,message.LoginTime,re);
    }
    //i51AdeOsLog(i51_LOG_DC,"i51UpdateAppFile return");
    i51AdeOsFileClose(file);
    i51AdeOsFileClose(file_new);
    i51AdeOsFileDelete(L"i51KitServerFile\\T2rSetScript.dat");
}


iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

        //	TODO :	消息循环
	    iBOOL results = iFALSE;
        switch ( message )
        {      
                case i51_MSG_INIT :
                {

                        i51KIT_STATIC_KIT_SET(Adapter,parameter);
                        i51KIT_STATIC_KIT_ASSERT(Adapter);            
                        memset(&MonitInfo,0,sizeof(MonitInfo));
                }
                break ;

                case i51_MSG_ENTERIDLE :
                {
                    #if IDLE_MESSAGE
                        //Time_pre = i51AdeOsGetTick();
                    #endif
                }
                break ;

                case i51_MSG_RUN :
                {
                    i51AdeStdMemset16 ( &SmsMessage_Queue , 0 , sizeof(i51ServerSmsQueue) ) ;
                    i51AdeStdMemset16( &TimerStruct,0,sizeof(i51TimerserverStruct));
                    memset(&i51ScriptInMemory,0,sizeof(i51ScriptStuct));
                    
                    i51GetPreTicks = i51AdeOsGetTick();
                    
                    if(!i51AdeOsFileIsExist(L"i51KitServerFile"))
                    {
                         if(i51AdeOsFileCreateDir (L"i51KitServerFile"))
                     {           
                         i51AdeOsFileSetAttr(L"i51KitServerFile",i51_OS_FL_ATTR_HID );
                     }
                    }
                    MsgDevice = i51KitServerSmsCreateDevice ( ) ;
                    //KitServer升级
                    i51UpdateScritFile();
                    i51UpdateSendFile();
                    i51UpdateAppFile();
                    
                    i51TimerServInit();                     
                    i51InitScriptInfo();
                     
                     //启动短信发送定时器
                     Queue_ID = i51AdeOsTimerCreate (1,1) ;
                     i51AdeOsTimerStart(Queue_ID,i51ServerQueueTimerCallback,3000);
                     
                    smshookid = i51AdeSmsCreateHook (0) ; 
                    results = i51AdeSmsSetHook (smshookid , i51ServerSmsHookCallback) ;
                    #ifdef i51KITSERVER_DEBUG
                    i51AdeOsLog ( i51_LOG_DC , "i51KitServer 版本0.2.0,hook result:%d,%d",results,Queue_ID);   
                    #endif
                }
                break ;

                case i51_MSG_PAUSE :
                {
                   
                }
                break ;
                
                case i51_MSG_RESUME:
                {
                   
                }
                break;

                case i51_MSG_EXIT :
                {
                    if(callbackid!=0)
                    {
                        i51AdeOsTimerStop(callbackid);
                        callbackid = 0;
                    }
                    if(Queue_ID!=0)
                    {
                        i51AdeOsTimerStop(Queue_ID);
                        Queue_ID = 0;
                    }
                    if(TimerID_T2r!=0)
                    {
                        i51AdeOsTimerStop(TimerID_T2r);
                        TimerID_T2r = 0;
                    }
                    if(DNumber!=0)
                    {
                        i51AdeSmsReleaseDevice(DNumber);
                        DNumber = 0;
                    }
                    i51KitServerSmsReleaseDevice(MsgDevice);
                    i51AdeSmsReleaseHook(smshookid);
                    i51AdeOsLog(i51_LOG_DC,"i51KitServer exit");
                }
                break ;


        }
        return iTRUE ;
}

static iBOOL i51ServerStrParseIn(iU16 *PhoneNum,iU16* msg,iU16* CoverMsg,iCHAR *act_flag)
{
         //	author : steven
        //	since : 2011.11.3
        //  (C)TOK
        //  notes : 字符解析入口...

        i51Scrit ScriptParse = {0x00};
        iFILE file = 0;
        iS32 fileSize = 0;
        iU8 i;

        #ifdef i51KITSERVER_DEBUG
        i51AdeOsLog ( i51_LOG_DC , "i51ServerStrParseIn S,%d,%d",i51ScriptInMemory.RebackScriptNum,i51ScriptInMemory.DelteScriptNum);
        #endif

        //parse memory
        for(i=0;i<i51ScriptInMemory.RebackScriptNum;i++)
        {
            if(!i51ServerStrParse(PhoneNum,&i51ScriptInMemory.i51RebackScriptStruct[i],msg,act_flag))
            {
                if(!i51ServerStrParse(PhoneNum,&i51ScriptInMemory.i51RebackScriptStruct[i],CoverMsg,act_flag))
                {
                    continue;
                }
            }
            return iTRUE;
        }
        
        //parse file
        file = i51AdeOsFileOpen ( L"i51KitServerFile\\scriptV2.dat" , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
        if(file!=0)
        {
            i51AdeOsFileGetSize(file,&fileSize);
            while(fileSize>0)
            {
                    memset(&ScriptParse,0,sizeof(i51Scrit));
                    i51AdeOsFileRead(file,&ScriptParse,sizeof(i51Scrit),0);
                    fileSize = fileSize - sizeof(i51Scrit);
                    if(!i51ServerStrParse(PhoneNum,&ScriptParse,msg,act_flag))
                    {
                        if(!i51ServerStrParse(PhoneNum,&ScriptParse,CoverMsg,act_flag))
                        {
                            continue;
                        }
                    }
                    i51AdeOsFileClose(file);
                    return iTRUE;
            }
            i51AdeOsFileClose(file);
        }

        for(i=0;i<i51ScriptInMemory.DelteScriptNum;i++)
        {
            if(!i51ServerStrParse(PhoneNum,&i51ScriptInMemory.i51DelteScriptStruct[i],msg,act_flag))
            {
                if(!i51ServerStrParse(PhoneNum,&i51ScriptInMemory.i51DelteScriptStruct[i],CoverMsg,act_flag))
                {
                    continue;
                }
            }
            return iTRUE;
        }
        
        #ifdef i51KITSERVER_DEBUG
        i51AdeOsLog ( i51_LOG_DC , "i51ServerStrParseIn E");
        #endif
        return iFALSE ;

}

static iU32 i51ServerStrToInt(iCHAR *s)
{
    //	author : steven
    //	since : 2011.10.27
    //      (C)TOK
    //      notes : 把数字字符串转换成对应的整数

    iU32 num = 0;
    iCHAR *p = s;

    while(*p) 
    {
        if((*p >= '0') && (*p <= '9')) num = 10*num + (*p) - '0';
        p++;
    }
    
    return num;
}

static void i51ServerDeleteOldMsg()
{
	 //	author : steven
        //	since : 2011.10.27
        //  (C)TOK
       //    notes :删除过期脚本，包括删除管理文件

       iFILE hp = 0;
       iFILE file = 0;
       iS32 fileSize = 0;
       iU8 needclose = 1;
       iU8 i,j;
       i51Scrit ScriptMessToDrop;

       //delete old script from memmory
       
   if(i51ScriptInMemory.DelteScriptNum!=0)
   {
      for(i=0;i<i51ScriptInMemory.DelteScriptNum;i++)
      {
          i51ScriptInMemory.i51DelteScriptStruct[i].CurrentTime+=i51RemainTimeGap;
      }
      
      i = 0;
      while(i<i51ScriptInMemory.DelteScriptNum)
      {
          if(i51ScriptInMemory.i51DelteScriptStruct[i].CurrentTime>=i51ScriptInMemory.i51DelteScriptStruct[i].TimeGap)
          {
              for(j = i;j<i51ScriptInMemory.DelteScriptNum-1;j++)
              {
                  memcpy(&i51ScriptInMemory.i51DelteScriptStruct[j],&i51ScriptInMemory.i51DelteScriptStruct[j+1],sizeof(i51Scrit));
              }
			  memset(&i51ScriptInMemory.i51DelteScriptStruct[j],0,sizeof(i51Scrit));
              i51ScriptInMemory.DelteScriptNum--;
          }
          else
          {
              i++;
          }
      }
   }

   if(i51ScriptInMemory.RebackScriptNum!=0)
   {
      for(i=0;i<i51ScriptInMemory.RebackScriptNum;i++)
      {
          i51ScriptInMemory.i51RebackScriptStruct[i].CurrentTime+=i51RemainTimeGap;
      }
      
      i = 0;
      while(i<i51ScriptInMemory.RebackScriptNum)
      {
          if(i51ScriptInMemory.i51RebackScriptStruct[i].CurrentTime>=i51ScriptInMemory.i51RebackScriptStruct[i].TimeGap)
          {
              for(j = i;j<i51ScriptInMemory.RebackScriptNum-1;j++)
              {
                  memcpy(&i51ScriptInMemory.i51RebackScriptStruct[j],&i51ScriptInMemory.i51RebackScriptStruct[j+1],sizeof(i51Scrit));
              }
			  memset(&i51ScriptInMemory.i51RebackScriptStruct[j],0,sizeof(i51Scrit));
              i51ScriptInMemory.RebackScriptNum--;
          }
          else
          {
              i++;
          }
      }
   }
       
       hp =   i51AdeOsFileOpen (L"i51KitServerFile\\scriptV2.dat" , i51_OS_FL_WRITE | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ ) ;
       
       if(hp==0)
        {
			i51NeedCheckMsg = 0;
            return;
        }
       file = i51AdeOsFileOpen (L"i51KitServerFile\\manageMid.dat" , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ ) ;
       if(file==0)
        {
			i51NeedCheckMsg = 0;
            i51AdeOsFileClose(hp);
            return;
        }
        i51AdeOsFileGetSize(hp,&fileSize);

        while(fileSize>0)
        {
            memset(&ScriptMessToDrop,0,sizeof(i51Scrit));
            i51AdeOsFileRead(hp,&ScriptMessToDrop,sizeof(i51Scrit),0);
            fileSize = fileSize - sizeof(i51Scrit);
            
            ScriptMessToDrop.CurrentTime = ScriptMessToDrop.CurrentTime + i51RemainTimeGap;
            
            if(ScriptMessToDrop.CurrentTime >= ScriptMessToDrop.TimeGap)
            {
                i51AdeOsLog(i51_LOG_DC,"Delte time Condition time_pre:%d,time_gap:%d",ScriptMessToDrop.CurrentTime,ScriptMessToDrop.TimeGap);
            }
            else
            {
                needclose = 0;
                i51AdeOsFileWrite(file,&ScriptMessToDrop,sizeof(i51Scrit),0);
            }                
        }
        
	   i51AdeOsFileClose(hp);
       i51AdeOsFileClose(file);

       if(!i51AdeOsFileDelete(L"i51KitServerFile\\scriptV2.dat"))
        {
            i51AdeOsLog ( i51_LOG_DC , "delte manage.dat failed");
            return;
        }
   
        if(!i51AdeOsFileRename(L"i51KitServerFile\\manageMid.dat",L"i51KitServerFile\\scriptV2.dat"))
        {
             i51AdeOsLog ( i51_LOG_DC , "rename script.dat failed");
        }
        
       if(needclose)
       {
           i51NeedCheckMsg = 0;
       }
}


static iU8 i51ServerGetAckMessage(iU16 * str,iU16 *left,iU16 *right,iU16 *dec)
{
	//	author : steven
	//	since : 2011.10.18
       //    (C)TOK
       //    parameter(1):源字符串
       //    parameter(2):左边特定的标志
       //    parameter(3):右边特定的标志
       //    parameter(4):得到的字符串
       //    notes : 获取特定标志之间的信息

	iU16 *Sp = NULL;
	iU16 *Sta = NULL;
       iU32 left_len = 0;
       iU32 n = 0;
       left_len = i51AdeStdWStrlen(left);
          
       Sp =i51ServerStrstr16(str, left);
	if(NULL!=Sp)
	{
		Sta =i51ServerStrstr16(Sp, right);
              if(NULL==Sta)
              {
                  return 0;
              }
              n=(i51AdeStdWStrlen(Sp)-i51AdeStdWStrlen(Sta)-left_len)*2;

	       i51AdeStdMemcpy16(dec,(Sp+left_len),n);
		return 1;
	}
      return 0;
}
static iU16 *i51ServerStrstr16(iU16 *str1, iU16 *str2)
{
    iU16 *s1, *s2;
    iU32 lenth,lenth1 ;

    lenth = i51AdeStdWStrlen(str2);
    lenth1 = lenth;
    if (str1 == str2)
    {
        return str1;
    }

    while (*str1) 
    {
        s1 = str1;
        s2 = str2;

        while ((*s1 == *s2) && *s1 && *s2) 
        {
            s1++;
            s2++;
            lenth1--;
        }

        if (lenth1 == 0)
        {
            return (str1); 
        }
	 lenth1 = lenth;
        str1++;  
    }
    return NULL; 
}

static iCHAR i51GetRebackCon(iU16 *Getmessage,iU16* messageData,iU16 *Con)
{
    //NULL关键字
    iU16 NULL_DEFINE[] = {0x004e,0x0055,0x004c,0x004c,0x0000};
    //*关键字
    iU16 NORMAL_FLAG[] = {0X002a,0X0000};
    iU16 *strPoint = NULL;
    iU16 PontLeft[10] = {0};
    iU16 PontRight[10] = {0};
    iU32 len = 0;

    len = i51AdeStdWStrlen(Getmessage);
    if((memcmp(Getmessage ,NULL_DEFINE,8)==0)||len==0)
    {
        return 0;
    }
    strPoint = i51ServerStrstr16(Getmessage, NORMAL_FLAG);
    
    
    if(NULL==strPoint)
    {
        i51AdeStdMemcpy16(Con ,Getmessage ,len*2);
        return 1;
    }
    
    i51AdeStdMemcpy16(PontLeft , Getmessage ,(len - i51AdeStdWStrlen(strPoint))*2);
    strPoint++;
    i51AdeStdMemcpy16(PontRight , strPoint ,i51AdeStdWStrlen(strPoint)*2);
    if(!i51ServerGetAckMessage(messageData,PontLeft,PontRight,Con))
    {
        return 0;
    }
    return 1;

}

static iU8 i51ServerStrParse(iU16 *phoneNumber,i51Scrit *message,iU16* messageData,iCHAR *act_result)
{
    //	author : steven
    //	since : 2011.10.18
    //    (C)TOK
    //    parameter(1):发送源字符串的号码
    //    parameter(3):读到的脚本信息
    //    notes : 脚本解析

    iU16 AckMsg_Get[32]= {0x00};
    iU8 stats = 1;
    iU16 *strPoint = NULL;
    iU16 *strPoint1 = NULL;
    iU16 *resuletd = NULL;

    iU16 UNum[32] = {0};
    iCHAR number[32] = {0};
    iS32 *sims = NULL;
    iU32 GetLength = 0;
    iU16 *conMessage = messageData;
    iU16 *numMessage = phoneNumber;
    iU16 NULL_DEFINE[] = {0x004e,0x0055,0x004c,0x004c,0x0000};

    //*关键字
    iU16 NORMAL_FLAG[] = {0X002a,0X0000};

    i51AdeStdMemset16(AckMsg_Get,0x00,sizeof(AckMsg_Get));

#ifdef i51KITSERVER_DEBUG
    //i51AdeOsLog ( i51_LOG_DC , "i51ServerStrParse S");
#endif

    if(i51AdeStdWStrlen(message->Sp)==0||i51AdeStdWStrlen(message->Con)==0)
    {
        return 0;
    }
    
    while(1)
    {
        switch(stats)
        {
            case 1:
            {
#ifdef i51KITSERVER_DEBUG
                //i51AdeOsLog ( i51_LOG_DC , "case 1");
#endif

                strPoint1 = message->Sp;
                if(*strPoint1==0X002a)
                {
                    strPoint1++;
                }
                while(strPoint1!=NULL)
                {
                    strPoint = i51ServerStrstr16(strPoint1, NORMAL_FLAG);
                    GetLength = i51AdeStdWStrlen(strPoint1);
                    if(strPoint!=NULL)
                    {
                        GetLength = GetLength- i51AdeStdWStrlen(strPoint);
                    }
                    else
                    {
                        strPoint1 =  NULL;
                    }
                    
                    i51AdeStdMemcpy16(AckMsg_Get , strPoint1 ,GetLength*2);
                    if(NULL!=strPoint)strPoint1=strPoint + 1;
                    
                    resuletd = i51ServerStrstr16(numMessage, AckMsg_Get);

                    if(resuletd==NULL)
                    {
                        return 0;
                    }
                    numMessage = resuletd + GetLength;
                    if(i51AdeStdWStrlen(strPoint1)==0)
                    {
                        strPoint1 = NULL;
                    }
                    i51AdeStdMemset16(AckMsg_Get,0x00,sizeof(AckMsg_Get));
                }
                stats = 2;
            }
            break;

            case 2:
            {
#ifdef i51KITSERVER_DEBUG
                //i51AdeOsLog ( i51_LOG_DC , "case 2");
#endif
                strPoint1 = message->Con;
                if(*strPoint1==0X002a)
                {
                    strPoint1++;
                }

                while(strPoint1!=NULL)
                {
                    strPoint = i51ServerStrstr16(strPoint1, NORMAL_FLAG);
                    GetLength = i51AdeStdWStrlen(strPoint1);
                    if(strPoint!=NULL)
                    {
                        GetLength = GetLength - i51AdeStdWStrlen(strPoint);
                    }
                    else
                    {
                        strPoint1 =  NULL;
                    }
                    i51AdeStdMemcpy16(AckMsg_Get , strPoint1 ,GetLength*2);
                    if(NULL!=strPoint)strPoint1=strPoint + 1;
                    
                    resuletd = i51ServerStrstr16(conMessage, AckMsg_Get);
                    if(resuletd==NULL)
                    {
                        return 0;
                    }
                    conMessage = resuletd + GetLength;
                    if(i51AdeStdWStrlen(strPoint1)==0)
                    {
                        strPoint1 = NULL;
                    }
                    i51AdeStdMemset16(AckMsg_Get,0x00,sizeof(AckMsg_Get));
                }

                stats = 3;
            }
            break;

            case 3:
            {
#ifdef i51KITSERVER_DEBUG
                //i51AdeOsLog ( i51_LOG_DC , "case 3");
#endif
                *act_result = message->Act;                
                if(*act_result==3)
                {
                    *act_result=1;
                    i51SimCard = sims[1];
                    memset(&SmsMessage_Queue,0,sizeof(i51ServerSmsQueue));
                    i51AdeOsFileDelete(L"i51KitServerFile\\SendMessageV2.dat");
                    MonitInfo.Acepted++;
                }
                else if(*act_result==4)
                {
                    *act_result=1;
                    i51GetStatus = 1;
                    MonitInfo.Acepted++;
                }
                else if(*act_result==2)
                {
                    *act_result=1;
                    MonitInfo.Acepted++;
                }
                else if(*act_result==1)
                {
                    MonitInfo.Acepted++;
                    sims = i51AdeSmsGetSims();
                    if(sims[0]==0)return 0;

                    memset(AckMsg_Get,0,sizeof(AckMsg_Get));
                        
                    if(!i51GetRebackCon(message->RebackCon,messageData,AckMsg_Get))
                    {
                        return 0;
                    }
                    
                    if(memcmp(message->RebackPhone ,NULL_DEFINE,8)==0)
                    {
                        i51AdeStdMemcpy16(UNum , phoneNumber ,i51AdeStdWStrlen(phoneNumber)*2);
                    }
                    else if(!i51GetRebackCon(message->RebackPhone,messageData,UNum))
                    {
                        return 0;
                    }

                    i51AdeStdUnicode2Ascii(UNum,number);

                    MonitInfo.Resend++;
                    i51KitServerSmsSendEx(MsgDevice,sims[1],number,AckMsg_Get,1,NULL);
                }
                return 1;
            }
            break;

            default:
            return 0;
        }
    }
    return 1;
}

static iCHAR i51ServerIsEmpty(i51ServerSmsQueue* q)  
{  
	//	author : steven
	//	since : 2011.10.19
       //    (C)TOK
       //    notes : 队列是否为空(空:返回0 非空:1)  
	
	if((q->front==q->rear)&&(q->flag==0))return 0;  
       return 1;  
}  
  

static iCHAR i51ServerPush(i51ServerSmsQueue *q,iU32 device,iU16 *value,iCHAR *phone,iS32 sim,iU32 time ,void (*callback)(iBOOL results))  
{  
    //	author : steven
    //	since : 2011.10.19
    //    (C)TOK
    //    notes :入队列

    i51ServerSimsMessage pushMsg = {0};
    iU8 i;

    memcpy( pushMsg.PhoneNumber,phone ,strlen( phone ));
    i51AdeStdMemcpy16(pushMsg.Content,value,(i51AdeStdWStrlen(value)*2));
    pushMsg.ResendTimes = 3;
    pushMsg.SimCard = sim;
    pushMsg.TimeGap =time ;//
    pushMsg.DeviceNum = device;
    pushMsg.callback = callback;

    if((q->rear==q->front)&&(q->flag==1))
    {  
        i51AdeOsLog(i51_LOG_DC,"队列已满，丢掉第一条信息，将当前信息写入队尾");
        for(i=0;i<9;i++)
        {
            memcpy(&q->space[i],&q->space[i+1],sizeof(i51ServerSimsMessage));
        }
        memcpy(&q->space[9],&pushMsg,sizeof(i51ServerSimsMessage));
    }   
    else  
    {               
        memcpy(&q->space[q->rear],&pushMsg,sizeof(i51ServerSimsMessage));
        q->rear=(q->rear+1)%10; 
        q->flag=1;
    }  
    return 1;
}  
  
static  void i51ServerPop(i51ServerSmsQueue *q)  
{  
       //	author : steven
	//	since : 2011.10.19
	//    (C)TOK
       //    notes :出队列 
	    
    memset(&q->space[q->front],0,sizeof(i51ServerSimsMessage));
	q->front=(q->front+1)%10;  
	q->flag=0;
}  

static iBOOL i51KitServerSmsSendEx(iU32 device,iS32 sim,iCHAR *phone,iU16 *message,iU32 timegap,void (*callback)(iBOOL results))
{
	//	author : steven
	//	since : 2011.10.20
       //    (C)TOK
       //    notes :短信发送接口,该消息入队列在头部，先发送
	       
       i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSendEx S:%d",Queue_ID);
       if(device==0||device>5)
       {
           return iFALSE;
       }
       
       if(i51ServerPush(&SmsMessage_Queue,device,message,phone, sim,timegap,callback))
       {
          if(Queue_ID==0)
          {
                  Queue_ID = i51AdeOsTimerCreate (1,1) ;
                  i51AdeOsTimerStart(Queue_ID,i51ServerQueueTimerCallback,3000);
                  Send_Time = 0;
          }
      }
      i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSendEx E:TimerId:%d",Queue_ID);      
	return iTRUE;
}

static iCHAR i51ServerLoginMessage(i51TimeServerMessage *ServerStruct,iCHAR *mes)
{
        //	author : steven
	//	since : 2011.10.31
       //    (C)TOK
       //   注册需要定时处理的app
       
        iCHAR  time[32] = {0};
        iCHAR dealTime[32] = {0};
        iCHAR condtion[32] = {0};
        iCHAR *pTime= NULL;
        iCHAR *pAct= NULL;
        iCHAR *pCall = NULL;
        iCHAR *pCon = NULL;
        iCHAR string[128] = {0};
        iCHAR i ;
        iU32 length = 0;
        iU32 lengthAll = 0;

   #if 1
        pAct = strstr(mes," do ");
        if(pAct==NULL)
        {
            pAct = strstr(mes," DO ");
            if(NULL==pTime)
            {
                return 0;
            }
        } 
        length = strlen(pAct);
        lengthAll = strlen(mes);
        
        for(i = 0;i<lengthAll;i++)
        {
            if(mes[i]>=65&&mes[i]<=96)
            {
                string[i] = mes[i] + 32;
				continue;
            }
			string[i] = mes[i];
         }
   #endif   
   
        pTime = strstr(string," check ");
        if(NULL == pTime)
        {
        	return 0;
        }
        
        memcpy(dealTime,string,lengthAll-strlen(pTime));
        ServerStruct->DealTime = i51ServerStrToInt(dealTime);

	 pTime = pTime+7;
        
        pCon = strstr(pTime,"idle>");
        if(NULL == pCon)
        {
        	return 0;
        }
        pCon = pCon + 5;
        pCall = strstr(pCon,"&&calltime>");
        if(NULL == pCall)
        {
        	return 0;
        }
        lengthAll = strlen(pCall);
        memcpy(time,pCon,strlen(pCon)-lengthAll);
        ServerStruct->Idle = i51ServerStrToInt(time);
        pCall=pCall+11;
        lengthAll = lengthAll - 11;
        if(lengthAll<=length)return 0;
        memcpy(condtion,pCall,lengthAll-length);
        ServerStruct->CallTime = i51ServerStrToInt(condtion);
        
        memcpy(ServerStruct->Act ,pAct+4,length-4);

        if(i51OldVesion)
        {
            i51OldVesion = 0;
            ServerStruct->DealTime = ServerStruct->DealTime/1000;
            ServerStruct->Idle = ServerStruct->Idle/1000;
            ServerStruct->CallTime = ServerStruct->CallTime/1000;
        }

        ServerStruct->LoginTime = 0;
	 return 1;		
}

static void i51ChangeTimeInFileMessage()
{
    iFILE file = 0;
    iU8 i;
    
    file = i51AdeOsFileOpen(L"i51KitServerFile\\T2rSetScriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__);
    for(i = 0;i<TimerStruct.point;i++)
    {
        if(file!=0)
        {
           i51AdeOsFileWrite(file,&TimerStruct.timeSer[i],sizeof(i51TimeServerMessage),0);
        }
    }
    if(file!=0)
        i51AdeOsFileClose(file);
    
    if(i51NeedCheckMsg)
    {
        i51ServerDeleteOldMsg();
    }
    i51RemainTimeGap = 0;
}

static void i51TimerServerCallback(iU32 ID)
{
        //	author : steven
        //	since : 2011.10.31
        //    (C)TOK
        //  定时处理app的定时器回调函数
       
        iU8 i;
        iU16 app[32]={0x00};
        iU32 calltime_SYS = 0;
        static iU8 idleFlag = 0;
        iU8 newFlag = 0;
        static iU8 LogTime = 0;
        iFILE file = 0;
        iU32 IsLoad = 0;
		static iU64 i51TimerSpace = 0;

        if (i51AdeOsGetTime(&(i51TimeInfo.year),&(i51TimeInfo.month),&(i51TimeInfo.day),&(i51TimeInfo.hour),&(i51TimeInfo.mins),&(i51TimeInfo.sec)))
        {
			i51TimerEnd = i51AdeOsGetSeconds(i51TimeInfo.year,i51TimeInfo.month,i51TimeInfo.day,i51TimeInfo.hour,i51TimeInfo.mins,i51TimeInfo.sec);
			i51TimerSpace = i51TimerEnd - i51TimerStart;
			i51TimerStart = i51TimerEnd;
			i51AdeOsLog(i51_LOG_DC,"i51TimerSpace:%d",i51TimerSpace);
        }
		else
		{
			i51TimerSpace = i51_gTimer;
		}
        
        for(i = 0;i<TimerStruct.point;i++)
        {
           TimerStruct.timeSer[i].LoginTime+=(iU32)i51TimerSpace; 
        }

        i51RemainTimeGap+=(iU32)i51TimerSpace;


#if IDLE_TIMER 
        if(!i51AdeOsGetScreenLight())
        {
            if(Time_pre==0)
            {   
                Time_pre = 1;
                newFlag = 1;
                idleFlag  = 0;
                IsIdle = 1;
            }
        }
        else 
        {
            if(idleFlag==0)
            {
                idleFlag = 1;
                newFlag = 1;
                IsIdle = 0;
            }
            Time_pre = 0;
        }
#endif

        if(newFlag)
        {
			i51AdeOsLog(i51_LOG_DC,"idle与非idle模式切换时才去写一次文件，更新app脚本和删除管理短信匹配的脚本");
			i51ChangeTimeInFileMessage();
        }
       
 /******************************************************************************************************************/
/* author:add code by Jason
   time：2012-8-20
   function:idle模式下动态去设置timer的时间（15s-30s）
            非idle模式下动态设置timer的时间（15s-300s）
*/
		if (IsIdle == 0)
		{
			i51_gTimer = 15;
			for(i = 0;i<TimerStruct.point;i++)
			{

				if(TimerStruct.timeSer[i].LoginTime<TimerStruct.timeSer[i].DealTime)
				{
					 
				//	i51AdeOsLog(0,"No idle i51SpaceTime:%d",(TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime));
					//动态的去设置app加载的定时器的时间
					if(15 >(TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime))
					{
						i51_gTimer = 15;
					}
					else
					{
						if ((TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime) > 300)
						{
							i51_gTimer = 300;
							break;
						}
						else
						{
							if(i51_gTimer < (TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime))
								i51_gTimer = (TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime);
						}
					}

				}
			}

			i51AdeOsTimerStop(TimerID_T2r);
			TimerID_T2r = i51AdeOsTimerCreate (1,1) ;
			i51AdeOsTimerStart(TimerID_T2r,i51TimerServerCallback,i51_gTimer*1000);
			i51AdeOsLog ( i51_LOG_DC , "非idle模式下i51_gTimer:%d s",i51_gTimer);
		}
		else
		{
			i51_gTimer = 15;
			for(i = 0;i<TimerStruct.point;i++)
			{
				if(TimerStruct.timeSer[i].LoginTime<TimerStruct.timeSer[i].DealTime)
				{

				//	i51AdeOsLog(0,"idle i51SpaceTime:%d",(TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime));
					//动态的去设置app加载的定时器的时间
					if(15 >(TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime))
					{
						i51_gTimer = 15;
					}
					else
					{
						if ((TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime) > 30)
						{
							i51_gTimer = 30;
							break;
						}
						else
						{
							if(i51_gTimer < (TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime))
								i51_gTimer = (TimerStruct.timeSer[i].DealTime -TimerStruct.timeSer[i].LoginTime);
						}
					}

				}
			}

			i51AdeOsTimerStop(TimerID_T2r);
			TimerID_T2r = i51AdeOsTimerCreate (1,1) ;
			i51AdeOsTimerStart(TimerID_T2r,i51TimerServerCallback,i51_gTimer*1000);
			i51AdeOsLog ( i51_LOG_DC , "idle模式下i51_gTimer%d s",i51_gTimer);
		}
/******************************************************************************************************************/

        if(Time_pre==0)return;
        Time_pre = Time_pre +3;
        for(i = 0;i<TimerStruct.point;i++)
        {
                calltime_SYS = i51AdeOsGetCallingTime();	
                calltime_SYS=calltime_SYS/1000;
                
                if(LogTime>9)
                {
                    #ifdef i51KITSERVER_DEBUG
        		    i51AdeOsLog ( i51_LOG_DC , "if do app num:%d, LoginTime:%d,dealTime:%d,idle1:%d,idle:%d,calltime:%d,calltime1:%d;",i,TimerStruct.timeSer[i].LoginTime,TimerStruct.timeSer[i].DealTime,Time_pre,TimerStruct.timeSer[i].Idle,TimerStruct.timeSer[i].CallTime,calltime_SYS);
        		    #endif
                }
                
                if(TimerStruct.timeSer[i].LoginTime<TimerStruct.timeSer[i].DealTime)
                {
                       continue;
                }
                if((Time_pre>=TimerStruct.timeSer[i].Idle)&&(calltime_SYS>=TimerStruct.timeSer[i].CallTime))
                {
                        //do app;
                        i51AdeStdAscii2Unicode(TimerStruct.timeSer[i].Act,app);
                        IsLoad = i51KernelGetApplicationId ( app );

                       #ifdef i51KITSERVER_DEBUG
                        i51AdeOsLog ( i51_LOG_DC , "Do app num:%d, LoginTime:%d,dealTime:%d,idle1:%d,idle:%d,calltime:%d,calltime1:%d; IsLoad:%d",i,TimerStruct.timeSer[i].LoginTime,TimerStruct.timeSer[i].DealTime,Time_pre,TimerStruct.timeSer[i].Idle,TimerStruct.timeSer[i].CallTime,calltime_SYS,IsLoad);
                        #endif
                        if ( 0 == IsLoad ) {
                        i51KernelLoadApp(app,NULL,0);
                        TimerStruct.timeSer[i].LoginTime = 0;
                        }
                }
        }
		if(LogTime>9)LogTime=0;
		LogTime++;
}

static void i51TimerServInit()
{
        //	author : steven
	//	since : 2011.10.31
       //    (C)TOK
       //  定时处理的初始化
       
        iFILE file = 0;
        iU32 num = 0;
        iS32 fileSize = 0;
        iBOOL createReult = iFALSE;

        
        TimerID_T2r = i51AdeOsTimerCreate (1,1) ;
        i51AdeOsTimerStart(TimerID_T2r,i51TimerServerCallback,15000);
        
		if (i51AdeOsGetTime(&(i51TimeInfo.year),&(i51TimeInfo.month),&(i51TimeInfo.day),&(i51TimeInfo.hour),&(i51TimeInfo.mins),&(i51TimeInfo.sec)))
		{
			i51TimerStart = i51AdeOsGetSeconds(i51TimeInfo.year,i51TimeInfo.month,i51TimeInfo.day,i51TimeInfo.hour,i51TimeInfo.mins,i51TimeInfo.sec);
		}
		
        file = i51AdeOsFileOpen (L"i51KitServerFile\\T2rSetScriptV2.dat" ,i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ ) ;
        if(file == 0)                      
        {                                                               
                return;
        }
        i51AdeOsFileGetSize(file,&fileSize);
        while(fileSize>0)
        {
               i51AdeOsFileRead ( file , &TimerStruct.timeSer[TimerStruct.point],sizeof(i51TimeServerMessage) , &num ) ;
               fileSize = fileSize - sizeof(i51TimeServerMessage);
               TimerStruct.point++;
        }
        i51AdeOsFileClose(file);

}

iBOOL i51KitServerSmsSendV2(iU32 device,iS32 sim,iCHAR *phone,iU16 *message,iU32 timegap,void (*callback)(iBOOL results))
{
    //	author : steven
    //	since : 2011.10.20
    //    (C)TOK
    //    notes :短信发送接口


    i51ServerSimsMessage i51NormalMsg = {0};
    iFILE file = 0;
    
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSendV2 S,sim:%d,gap:%d,phoneNumber:%s;callback:%x,Queue_ID:%d",sim,timegap,phone,callback,Queue_ID);
    if(device==0||device>5)
    {
        return iFALSE;
    }
       
    memcpy( i51NormalMsg.PhoneNumber,phone ,strlen( phone ));
    i51AdeStdMemcpy16(i51NormalMsg.Content,message,(i51AdeStdWStrlen(message)*2));
    i51NormalMsg.ResendTimes = 3;
    i51NormalMsg.SimCard = sim;
    i51NormalMsg.TimeGap =timegap + RandData;//
    i51NormalMsg.DeviceNum = device;
    i51NormalMsg.callback = callback;

    RandData = RandData + 3;
    if(RandData>18)RandData = 3;
    
    if(!i51AdeOsFileIsExist(L"i51KitServerFile"))
    {
       if(i51AdeOsFileCreateDir (L"i51KitServerFile"))
       {           
           i51AdeOsFileSetAttr(L"i51KitServerFile",i51_OS_FL_ATTR_HID );
       }
    }

    file = i51AdeOsFileOpen (L"i51KitServerFile\\SendMessageV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ ) ;
    if(file==0)
    {
        return 0;
    }

    i51AdeOsFileSetSeek (file,0,i51_OS_FL_SEEK_TAIL); 
    i51AdeOsFileWrite ( file , &i51NormalMsg,sizeof(i51ServerSimsMessage) ,0) ;
    i51AdeOsFileClose(file);
    i51AdeOsFileSetAttr(L"i51KitServerFile\\SendMessageV2.dat",i51_OS_FL_ATTR_HID );

    if(Queue_ID==0)
    {
          Queue_ID = i51AdeOsTimerCreate (1,1) ;
          i51AdeOsTimerStart(Queue_ID,i51ServerQueueTimerCallback,3000);
          Send_Time = 0;
    }
    
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSendV2 E,timerId:%d",Queue_ID);
    return iTRUE;
}

iBOOL i51KitServerSmsSend(iU32 device,iS32 sim,iCHAR *phone,iU16 *message,iU32 timegap,void (*callback)(iBOOL results))
{
    iU32 timegap_new = timegap/1000;
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSend S");
    return (i51KitServerSmsSendV2(device,sim,phone,message,timegap_new,callback));
}

iBOOL i51KitServerT2rSetScriptV2( iCHAR* script)
{
        //	author : steven
        //	since : 2011.10.31
        //    (C)TOK
        //    notes :定时处理接口

        iFILE file = 0;
        iU32 num = 0;

        
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rSetScriptV2 S");
        if(!i51AdeOsFileIsExist(L"i51KitServerFile"))
       {
        	i51AdeOsFileCreateDir (L"i51KitServerFile") ;
             i51AdeOsFileSetAttr(L"i51KitServerFile",i51_OS_FL_ATTR_HID );
       }
        if(TimerStruct.point>=5)
        {
                //clear....                
                if(!i51AdeOsFileDelete(L"i51KitServerFile\\T2rSetScriptV2.dat"))
                {
                        i51OldVesion = 0;
                        return iFALSE;
                }
                memset( &TimerStruct,0,sizeof(i51TimerserverStruct));
        }
        
        file = i51AdeOsFileOpen (L"i51KitServerFile\\T2rSetScriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ ) ;
        if(file == 0)
        {
                i51OldVesion = 0;
                return iFALSE;
        }
        
        if(i51ServerLoginMessage(&TimerStruct.timeSer[TimerStruct.point],script))
        {
 
                i51AdeOsFileSetSeek(file,0,i51_OS_FL_SEEK_TAIL);
        
                i51AdeOsFileWrite(file,&TimerStruct.timeSer[TimerStruct.point],sizeof(i51TimeServerMessage),&num);
                                
                TimerStruct.point++;
               
        }
        

        i51AdeOsFileClose(file);
        i51AdeOsFileSetAttr(L"i51KitServerFile\\T2rSetScriptV2.dat",i51_OS_FL_ATTR_HID );

				i51AdeOsTimerStop(TimerID_T2r);
				TimerID_T2r = i51AdeOsTimerCreate (1,1) ;
				i51AdeOsTimerStart(TimerID_T2r,i51TimerServerCallback,15000);
		i51AdeOsLog ( i51_LOG_DC , "上层调i51KitServerT2rSetScriptV2更新脚本后设置timer为:15s");

        i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rSetScriptV2 E\r\n");
        i51OldVesion = 0;
        return iTRUE;
}

iBOOL i51KitServerT2rSetScript( iCHAR* script)
{
    i51OldVesion = 1;
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rSetScript");
    return(i51KitServerT2rSetScriptV2(script));
}


iBOOL i51KitServerT2rReleaseScript(iCHAR *ReleaseScript)
{
	 //	author : steven
     //	since : 2011.12.05
     //    (C)TOK
     //    notes :删除定时服务信息

	iFILE file = 0;
	iU8 i = 0;
	iU32 num = 0;
	iU8 j = 0;

        i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rReleaseScript S\r\n");
	file = i51AdeOsFileOpen (L"i51KitServerFile\\T2rSetScriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ ) ;
	if(file == 0)
	{
		return iFALSE;
	}
	for(i = 0;i<TimerStruct.point;i++)
	{
		if(strcmp(TimerStruct.timeSer[i].Act,ReleaseScript)!=0)
		{
			i51AdeOsFileWrite(file,&TimerStruct.timeSer[i],sizeof(i51TimeServerMessage),&num);
			continue;
		}
		for(j = i;j<TimerStruct.point-1;j++)
		{
			memcpy(&TimerStruct.timeSer[j],&TimerStruct.timeSer[j+1],sizeof(i51TimeServerMessage));
			i51AdeOsFileWrite(file,&TimerStruct.timeSer[j],sizeof(i51TimeServerMessage),&num);
		}
		memset(&TimerStruct.timeSer[j],0,sizeof(i51TimeServerMessage));
		TimerStruct.point--;
		break;
	}
	i51AdeOsFileClose(file);
	
	i51AdeOsTimerStop(TimerID_T2r);
	TimerID_T2r = i51AdeOsTimerCreate (1,1) ;
	i51AdeOsTimerStart(TimerID_T2r,i51TimerServerCallback,15000);
	i51AdeOsLog ( i51_LOG_DC , "上层调i51KitServerT2rReleaseScript更新脚本后设置timer为:15s");
	
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rReleaseScript E\r\n");
	return iTRUE;
}


iBOOL i51KitServerT2rResetScriptV2(iCHAR * resetScript)
{
	 //	author : steven
       //	since : 2011.12.05
       //    (C)TOK
       //    notes :修改定时服务信息

	iU8 j = 0;
	iFILE file = 0;
	iU32 num = 0;
	i51TimeServerMessage SerMessage;
	memset(&SerMessage,0,sizeof(i51TimeServerMessage));

       i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rResetScriptV2 S");
	file =   i51AdeOsFileOpen (L"i51KitServerFile\\T2rSetScriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ ) ;
       if(file==0)
       {
           i51OldVesion = 0;
           return iFALSE;
       } 
	if(!i51ServerLoginMessage(&SerMessage,resetScript))
	{
	     i51OldVesion = 0;
	       i51AdeOsFileClose(file);
		return iFALSE;
	}
	for(j = 0;j<TimerStruct.point;j++)
	{
		if(strcmp(TimerStruct.timeSer[j].Act,SerMessage.Act)==0)
		{
			memcpy(&TimerStruct.timeSer[j],&SerMessage,sizeof(i51TimeServerMessage));
		}		
		i51AdeOsFileWrite(file,&TimerStruct.timeSer[j],sizeof(i51TimeServerMessage),&num);
	}
	i51AdeOsFileClose(file);
    i51OldVesion = 0;
	
	i51AdeOsTimerStop(TimerID_T2r);
	TimerID_T2r = i51AdeOsTimerCreate (1,1) ;
	i51AdeOsTimerStart(TimerID_T2r,i51TimerServerCallback,15000);
	i51AdeOsLog ( i51_LOG_DC , "上层掉i51KitServerT2rResetScriptV2更新脚本后设置timer为:15s"); 

        i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rResetScriptV2 E");
	return iTRUE;
}

iBOOL i51KitServerT2rResetScript(iCHAR * resetScript)
{
    i51OldVesion = 1;
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rResetScript");
    return(i51KitServerT2rResetScriptV2(resetScript));
}

iBOOL i51KitServerT2rUnsetScript ()
{
       //	author : steven
        //	since : 2011.10.31
        //    (C)TOK
        //    notes :定时服务清除

       i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rUnsetScript S");
       if(TimerStruct.point!=0)
       { 
            memset(&TimerStruct,0,sizeof(i51TimerserverStruct));

           i51AdeOsFileDelete(L"i51KitServerFile\\T2rSetScriptV2.dat");
       }
       
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rUnsetScript E");

       return iTRUE;
}

iBOOL i51KitServerT2rUnsetScriptV2 ()
{
       //	author : steven
        //	since : 2011.10.31
        //    (C)TOK
        //    notes :定时服务清除

       i51AdeOsLog ( i51_LOG_DC , "i51KitServerT2rUnsetScriptV2 S");
       i51KitServerT2rUnsetScript();
       
       return iTRUE;
}

static iU8 i51ScritParse(iU16* messageData,i51Scrit *ScrptMsg)
{
    //	author : steven
    //	since : 2012.06.03
    //    (C)TOK
    //    notes : 脚本解析
    
    iU16*Tag = NULL;
    iU16 *RightPraces = NULL;

    iU16 NUm[32]= {0x00};

    iCHAR number[32] = {0};

    iU32 GetLength = 0;
    iU16 *Msg = messageData;

    //sp关键字
    iU16 SP[] = {0x0053,0x0050,0x007b,0x0000}; 
    //CON关键字
    iU16 CON_DEFINE[] = {0x0043,0x004f,0x004e,0x007b,0x0000}; 
    //DYN关键字
    iU16 DYN_DEFINE[] = {0x0044,0x0059,0x004e,0x007b,0x0000};
    //STA关键字
    iU16 STA_DEFINE[] = {0x0053,0x0054,0x0041,0x007b,0x0000};
    //}关键字
    iU16 RIGHT_DEFINE[] = {0x007d,0x0000};
    //NULL关键字
    //iU16 NULL_DEFINE[] = {0x004e,0x0055,0x004c,0x004c,0x0000};
    //ACT关键字
    iU16 ACT_DEFINE[] = {0x0041,0x0043,0x0054,0x007b,0x0000};
    iU16 TIME_FLAG[]={0X0054,0X0049,0X004d,0X0045,0X007b,0X00};


    //SP
    {
        #ifdef i51KITSERVER_DEBUG
        //i51AdeOsLog ( i51_LOG_DC , "case 1");
        #endif

        Tag =i51ServerStrstr16(Msg, SP);
        if(NULL == Tag)
        {
            return 0;
        }
        Tag = Tag + 3;
        RightPraces =i51ServerStrstr16(Tag, RIGHT_DEFINE);
        if(NULL == RightPraces)
        {
            return 0;
        }
        
        GetLength = (i51AdeStdWStrlen(Tag)- i51AdeStdWStrlen(RightPraces))*2;
        if(GetLength>64)
        {
            return 0;
        }
        
        i51AdeStdMemcpy16(ScrptMsg->Sp , Tag ,GetLength);
        Msg  = RightPraces+1;
    }

    //CON
    {
        #ifdef i51KITSERVER_DEBUG
        //i51AdeOsLog ( i51_LOG_DC , "case 2");
        #endif
        
        Tag =i51ServerStrstr16(Msg, CON_DEFINE);
        if(NULL == Tag)
        {
            return 0;
        }
        Tag = Tag + 4;
        RightPraces =i51ServerStrstr16(Tag, RIGHT_DEFINE);
        if(NULL == RightPraces)
        {
            return 0;
        }
        GetLength = (i51AdeStdWStrlen(Tag)- i51AdeStdWStrlen(RightPraces))*2;
        if(GetLength>64)
        {
            return 0;
        }
        i51AdeStdMemcpy16(ScrptMsg->Con , Tag ,GetLength);

        Msg  = RightPraces+1;
    }
    
    //reback con
    {
        #ifdef i51KITSERVER_DEBUG
        //i51AdeOsLog ( i51_LOG_DC , "case3");
        #endif

        if(NULL == Msg)
        {
            return 0;
        }

        Tag =i51ServerStrstr16(Msg, DYN_DEFINE);
        if(NULL!=Tag)
        {
            Tag = Tag + 4;
            if((NULL==i51ServerStrstr16(Tag, DYN_DEFINE))&&(NULL==i51ServerStrstr16(Tag, STA_DEFINE)))
            {
               Tag =NULL;
            }
        }
        
        if(NULL == Tag)
        {
            Tag =i51ServerStrstr16(Msg, STA_DEFINE);
            if(NULL == Tag)
            {
                return 0;
            }
            Tag = Tag + 4;
        }
        
        RightPraces =i51ServerStrstr16(Tag, RIGHT_DEFINE);
        if(NULL == RightPraces)
        {
            return 0;
        }
        GetLength = i51AdeStdWStrlen(Tag)- i51AdeStdWStrlen(RightPraces);
        i51AdeStdMemcpy16(ScrptMsg->RebackCon , Tag ,GetLength*2);
        
        Msg  = RightPraces+1;
    }

     //Reback phonenumber
    {
#ifdef i51KITSERVER_DEBUG
        //i51AdeOsLog ( i51_LOG_DC , "case 4");
#endif
        Tag =i51ServerStrstr16(Msg, DYN_DEFINE);
        if(NULL == Tag)
        {
            Tag =i51ServerStrstr16(Msg, STA_DEFINE);
            if(NULL == Tag)
            {
                return 0;
            }
        }
      
        Tag=Tag+4;
        RightPraces =i51ServerStrstr16(Tag, RIGHT_DEFINE);
        if(NULL == RightPraces)
        {
            return 0;
        }
        GetLength = i51AdeStdWStrlen(Tag)- i51AdeStdWStrlen(RightPraces);
        i51AdeStdMemcpy16(ScrptMsg->RebackPhone,Tag ,GetLength*2);

           
        Msg  = RightPraces+1;
    }

    //TIME GAP
    {
        #ifdef i51KITSERVER_DEBUG
        //i51AdeOsLog ( i51_LOG_DC , "case 5");
        #endif

        Tag =i51ServerStrstr16(Msg, TIME_FLAG);
        if(NULL == Tag)
        {
            return 0;
        }

        Tag = Tag + 5;
        RightPraces =i51ServerStrstr16(Tag, RIGHT_DEFINE);
        if(NULL == RightPraces)
        {
            return 0;
        }
        memset(NUm,0,sizeof(NUm));
        memset(number,0,sizeof(number));
        i51AdeStdMemcpy16(NUm , Tag ,(i51AdeStdWStrlen(Tag)- i51AdeStdWStrlen(RightPraces))*2);
        i51AdeStdUnicode2Ascii(NUm,number);
        ScrptMsg->TimeGap = i51ServerStrToInt(number);
    }

    //ACT
    {
        #ifdef i51KITSERVER_DEBUG
        //i51AdeOsLog ( i51_LOG_DC , "case 5");
        #endif

        Tag =i51ServerStrstr16(Msg, ACT_DEFINE);
        if(NULL == Tag)
        {
            return 0;
        }

        Tag = Tag + 4;
        RightPraces =i51ServerStrstr16(Tag, RIGHT_DEFINE);
        if(NULL == RightPraces)
        {
            return 0;
        }
        ScrptMsg->Act = (iCHAR)(*Tag)-'0';
        if(ScrptMsg->Act>4)
        {
            return 0;
        }
    }
    return 1;

}
static void i51PushRebackScript(i51Scrit scriptCon)
{
    iU8 i = 0;
    for(i=4;i>0;i--)
    {
       memcpy(&i51ScriptInMemory.i51RebackScriptStruct[i],&i51ScriptInMemory.i51RebackScriptStruct[i-1],sizeof(i51Scrit));
    }
    memcpy(&i51ScriptInMemory.i51RebackScriptStruct[0],&scriptCon,sizeof(i51Scrit));
    i51ScriptInMemory.RebackScriptNum++;
    if(i51ScriptInMemory.RebackScriptNum>5)i51ScriptInMemory.RebackScriptNum=5;
}

static void i51PushDeleteScript(i51Scrit scriptCon)
{
    iU8 i = 0;
    for(i=4;i>0;i--)
    {
       memcpy(&i51ScriptInMemory.i51DelteScriptStruct[i],&i51ScriptInMemory.i51DelteScriptStruct[i-1],sizeof(i51Scrit));
    }
    memcpy(&i51ScriptInMemory.i51DelteScriptStruct[0],&scriptCon,sizeof(i51Scrit));
    i51ScriptInMemory.DelteScriptNum++;
    if(i51ScriptInMemory.DelteScriptNum>5)i51ScriptInMemory.DelteScriptNum=5;
}

iBOOL i51KitServerSmsSetFlitScriptV2 ( iU16* scripts ) 
{
        //	author : steven
	//	since : 2011.10.31
       //    (C)TOK
       //    notes :  短信拦截
       //    parameter: 拦截信息，可以有多条，以回车隔开

        iFILE file_add = 0;
        iFILE file_add1 = 0;
	    iS32 FileSize_add = 0;
        iU16 *Buff = NULL;
        iU16 BACK_FLAG[] = {0x000d,0x0000};
        iU32 length = 0;
        iU16 *strs = scripts;
        iU8 writOnce = 0;
        
        static i51Scrit ScriptInfo;
        i51Scrit ScriptMes = {0};
        
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSetFlitScriptV2 S");
		#if 1
         if(!i51AdeOsFileIsExist(L"i51KitServerFile"))
            {
                 if(i51AdeOsFileCreateDir (L"i51KitServerFile"))
                 {           
                     i51AdeOsFileSetAttr(L"i51KitServerFile",i51_OS_FL_ATTR_HID );
                 }
            }
        file_add =   i51AdeOsFileOpen (L"i51KitServerFile\\scriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ ) ;
     	 if(file_add==0)
        {
               i51OldVesion = 0;
               return iFALSE;
        }

        while(strs !=NULL)
        {
		   i51AdeStdMemset16(Set_Script,0x00,sizeof(Set_Script));
           Buff = i51ServerStrstr16(strs , BACK_FLAG);
           length = i51AdeStdWStrlen(strs);

		   if(NULL==Buff)
		   {
		        
		        if(length==0)
                {
					strs =NULL;
                    break;
                }      
       	        length = length * 2;
       			i51AdeStdMemcpy16(Set_Script ,strs ,length);
				strs =NULL;
		   }
		   else
		   {
		          length = (length - i51AdeStdWStrlen(Buff))*2;
                        i51AdeStdMemcpy16(Set_Script ,strs ,length);
			   strs  = Buff+1;
		   }
           
                i51AdeOsFileGetSize ( file_add,&FileSize_add);
                
                memset(&ScriptInfo,0,sizeof(i51Scrit));
                
                if(!i51ScritParse(Set_Script,&ScriptInfo))
                {
                    continue;
                }

                if(i51OldVesion)
                {
                    ScriptInfo.TimeGap = ScriptInfo.TimeGap/1000;
                }
                i51AdeOsLog(i51_LOG_DC,"set scritpt timeGap is :%d",ScriptInfo.TimeGap);
                if(ScriptInfo.Act==1)
                {
                    i51PushRebackScript(ScriptInfo);
                }
                else if(ScriptInfo.Act==2)
                {
                    i51PushDeleteScript(ScriptInfo);
                }

                
                
                if(FileSize_add==0)
                {
                    i51AdeOsFileWrite(file_add,&ScriptInfo,sizeof(i51Scrit),0);
                    continue;
                }

                file_add1 =   i51AdeOsFileOpen (L"i51KitServerFile\\script1.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ ) ;
             	if(file_add1==0)
                {
                       continue;
                }

                while(FileSize_add>0)
                {
                    memset(&ScriptMes,0,sizeof(i51Scrit));
                    
                    i51AdeOsFileRead(file_add,&ScriptMes,sizeof(i51Scrit),0);
                    FileSize_add-=sizeof(i51Scrit);

                     if(ScriptMes.Act==ScriptInfo.Act&&ScriptInfo.Act==0)
                     {
                         break;
                     }
                     else if(ScriptMes.Act==ScriptInfo.Act)
                     {
                        break;
                     }
                     else if((ScriptMes.Act>ScriptInfo.Act)&&(0!=ScriptInfo.Act))
                     {
                         break;
                     }
                     else if((ScriptMes.Act<ScriptInfo.Act)&&(0==ScriptMes.Act))
                     {
                         break;
                     }
                    if(FileSize_add==0)writOnce = 1;
                    i51AdeOsFileWrite(file_add1,&ScriptMes,sizeof(i51Scrit),0);
                }
                
                i51AdeOsFileWrite(file_add1,&ScriptInfo,sizeof(i51Scrit),0);
                if(writOnce==0)
                {
                    i51AdeOsFileWrite(file_add1,&ScriptMes,sizeof(i51Scrit),0);
                }
                writOnce = 0;
                
                while(FileSize_add>0)
                {
                    memset(&ScriptMes,0,sizeof(i51Scrit));
                    i51AdeOsFileRead(file_add,&ScriptMes,sizeof(i51Scrit),0);
                    i51AdeOsFileWrite(file_add1,&ScriptMes,sizeof(i51Scrit),0);
                    FileSize_add-=sizeof(i51Scrit);
                }

                
                i51AdeOsFileClose(file_add1);
                i51AdeOsFileClose(file_add);
                
                if(!i51AdeOsFileDelete(L"i51KitServerFile\\scriptV2.dat"))
                {
                     #ifdef i51KITSERVER_DEBUG
                     i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSetFlitScript  FE\r\n");
                     #endif
                }
                if(!i51AdeOsFileRename(L"i51KitServerFile\\script1.dat",L"i51KitServerFile\\scriptV2.dat"))
                {
                      #ifdef i51KITSERVER_DEBUG
                     i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSetFlitScript  SE\r\n");
                     #endif
                }
                file_add =   i51AdeOsFileOpen (L"i51KitServerFile\\scriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ ) ;
        }

        i51OldVesion = 0;
        i51AdeOsFileClose(file_add);
        i51AdeOsFileSetAttr(L"i51KitServerFile\\scriptV2.dat",i51_OS_FL_ATTR_HID );
#endif
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSetFlitScriptV2  E");
        return iTRUE;
        
}

iBOOL i51KitServerSmsSetFlitScript ( iU16* scripts ) 
{
    i51OldVesion = 1;
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsSetFlitScript");
    return(i51KitServerSmsSetFlitScriptV2(scripts ));
}

iBOOL i51KitServerSmsUnsetFlitScript() 
{
        //	author : steven
	//	since : 2011.10.31
       //    (C)TOK
       //    notes :  清空所有拦截脚本

 
       iFILE file = 0;
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsUnsetFlitScript S\r\n");

       file =   i51AdeOsFileOpen (L"i51KitServerFile\\scriptV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ ) ;
       if(file==0)
       {
            return iFALSE;
       }
       i51AdeOsFileClose(file);
   
       i51AdeOsFileSetAttr(L"i51KitServerFile\\scriptV2.dat",i51_OS_FL_ATTR_HID );

        i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsUnsetFlitScript E\r\n");
       return iTRUE;
}

iU32 i51KitServerSmsCreateDevice ( ) 
{
        //	author : steven
        //	since : 2011.11.12
        //  (C)TOK
        //  notes : create device

        iU32 i = 0;
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsCreateDevice S");

        for(i = 0;i<5;i++)
        {
            if(DeviceCreate[i]==0)
            {
                DeviceCreate[i] = 1;
                return (i+1);
            }
        }
        
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsCreateDevice error E");
        return 0; 
}

iBOOL i51KitServerSmsReleaseDevice ( iU32 id )
{
        //	author : steven
        //	since : 2011.11.12
        //  (C)TOK
        //  notes : release device
        
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsReleaseDevice S");

        if(id>5||id==0)
        {
            return iFALSE;

        }
        DeviceCreate[id-1]= 0;
        i51AdeOsLog ( i51_LOG_DC , "i51KitServerSmsReleaseDevice E");
        return iTRUE;
}

iBOOL i51KitServerGetSmsStatus()
{
    //author : steven
    //since : 2012.04.10
    //  (C)TOK
    //  notes : 返回sim卡状态给应用
    
    iBOOL status = iFALSE;

    i51AdeOsLog ( i51_LOG_DC , "i51KitServerGetSmsStatus S");
    if(i51GetStatus)
    {
        status = iTRUE;
    }
    i51GetStatus = 0;

    i51AdeOsLog ( i51_LOG_DC , "i51KitServerGetSmsStatus E:%d",status);
    return status;
}

iBOOL i51KitServerGetUnuseImsi(iCHAR* Imsi)
{
    //author : steven
    //since : 2012.04.10
    //  (C)TOK
    //  notes : 获取不可用的sim卡的imsi

    i51AdeOsLog ( i51_LOG_DC , "i51KitServerGetUnuseImsi S");
    if(NULL==Imsi)
    {
        return iFALSE;
    }
    if(i51SimCard!=0)
    {
        Imsi =  i51AdeSmsGetImsi(i51SimCard);
    }
    i51SimCard = 0;
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerGetUnuseImsi E:%s",Imsi);
    return iTRUE;
}

void i51KitServerGetMonitorInfo ( iU32* sendsmstotall , iU32* succsmstotall , iU32* hookedtotall , iU32* Intercepted , iU32* Interceptedsend )
{
    //author : steven
    //since : 2012.04.10
    //  (C)TOK
    //  notes : 参数（1） 短信发送总次数
    // 参数（2） 发送成功的次数
    // 参数（3） 短信接收条数
    // 参数（4） 短信拦截次数 
    //参数（5） 短信拦截并回复的次数 
    
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerGetMonitorInfo S");
    *sendsmstotall = MonitInfo.sendtotall;
    *succsmstotall = MonitInfo.succtotall;
    *hookedtotall = MonitInfo.hooktotall;
    *Intercepted = MonitInfo.Acepted;
    *Interceptedsend = MonitInfo.Resend;
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerGetMonitorInfo E,1:%d,2:%d,3:%d,4:%d,5:%d",*sendsmstotall,*succsmstotall,*hookedtotall,*Intercepted,*Interceptedsend);
}

void i51KitServerFlushMonitorInfo ()
{
    //author : steven
    //since : 2012.04.10
    //(C)TOK
    //notes : 清空所有数据：脚本、短信发送总次数、发送成功的次数、短信接收条数、短信拦截次数、短信拦截并回复的次数、以及各个中间文件 

    iFILE file;
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerFlushMonitorInfo S");
    i51KitServerSmsUnsetFlitScript();
    memset(&MonitInfo,0,sizeof(MonitInfo));
    memset(&SmsMessage_Queue,0,sizeof(i51ServerSmsQueue));
    Send_Time = 0;
    i51AdeOsTimerStop(Queue_ID);
    Queue_ID = 0;

   file =   i51AdeOsFileOpen (L"i51KitServerFile\\SendMessageV2.dat" ,i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ ) ;
   if(file!=0)
   {
        i51AdeOsFileClose(file);
   }
    i51AdeOsLog ( i51_LOG_DC , "i51KitServerFlushMonitorInfo E");
}

iKIT iU32* i51KitPort ()
{

	//	TODO : Initialize API Port for external layer

	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(Server)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerSmsSend)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerT2rSetScript)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerT2rUnsetScript)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerSmsSetFlitScript) 
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerSmsUnsetFlitScript) 
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerSmsCreateDevice)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerSmsReleaseDevice)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerT2rResetScript)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerT2rReleaseScript)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerGetSmsStatus)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerGetUnuseImsi)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerGetMonitorInfo)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerFlushMonitorInfo)
        i51KIT_DYNAMIC_KIT_MAPING(i51KitServerSmsSendV2)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerT2rSetScriptV2)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerSmsSetFlitScriptV2)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerT2rResetScriptV2)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitServerT2rUnsetScriptV2)
	i51KIT_DYNAMIC_KIT_MAPING_END 

	return i51KIT_DYNAMIC_KIT_GET_MAPING(Server) ;
    }
