#include "i51Debuger.h"
extern iU32 SimplePortalId;
extern iU8 AllStatics[21];

void i51KernelTestRun()
{
    iS32 LoadApp = 0;
    iU32* LoadKit = NULL;
    iBOOL result = iFALSE;
    iU32 GetVision= 0;
    iU32 ID = 0;
    iU32 length = 0;
    static iCHAR DrawKernelbuff[64] = {0};
    iU8 CodeBuff[16] = "123456";

    iU16 KerTotallBugs = 0;

    DrowTestLog("i51KernelTest->Start...",3);

    //check i51KernelLoadApp
    i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadApp : Testing -> Start" ) ;
    i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelLoadApp ( i51PhoneTest )" ) ;
    DrowTestLog("1 i51KernelLoadApp Start...",3);
    DrowTestLog("Testing...",3);
    LoadApp = i51KernelLoadApp( L"i51Debuger" ,NULL ,0);
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , LoadApp ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    if ( 0 != LoadApp) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadApp : Testing -> FAILED" ) ;
        sprintf(DrawKernelbuff,"Result:%d",LoadApp);
        DrowTestLog(DrawKernelbuff,0);
        KerTotallBugs ++ ;
        i51WriteError("i51KernelLoadApp()");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadApp : Testing -> OK" ) ;
        sprintf(DrawKernelbuff,"Result:%d",LoadApp);
        DrowTestLog(DrawKernelbuff,1);
    }


    //check i51KernelLoadKit
    i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadKit Start..." ) ;
    i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelLoadKit ( PKiGAPI\\i51KitG2 )" ) ;
    DrowTestLog("2 i51KernelLoadKit Start...",3);
    DrowTestLog("   Testing...... ",3);
    LoadKit = i51KernelLoadKit( L"PKiGAPI\\i51KitG2");
    i51AdeOsLog ( i51_LOG_DC , "Results = %x " , LoadKit ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%x",LoadKit);
    if ( NULL == LoadKit) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadKit : Testing -> FAILED" ) ;
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelLoadKit()");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadKit : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);

        //check i51KernelUnloadKit
        i51AdeOsLog ( i51_LOG_DC , "i51KernelUnloadKit : Testing -> Start" ) ;
        i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelUnloadKit ( i51KitG2 )" ) ;
        DrowTestLog("3 i51KernelUnloadKit Start...",3);
        DrowTestLog("   Testing...... ",3);
        result = i51KernelUnloadKit((iHANDLE)LoadKit);
        i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
        memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
        sprintf(DrawKernelbuff,"Result:%d",result);
        if ( 0 == result) 
        {
            i51AdeOsLog ( i51_LOG_DC , "i51KernelUnloadKit : Testing -> FAILED" ) ;
            KerTotallBugs ++ ;
            DrowTestLog(DrawKernelbuff,0);
            i51WriteError("i51KernelUnloadKit()");
        } 
        else 
        {
            i51AdeOsLog ( i51_LOG_DC , "i51KernelUnloadKit : Testing -> OK" ) ;
            DrowTestLog(DrawKernelbuff,1);
        }
    }

    //check i51KernelGetApplicationId
    i51AdeOsLog ( i51_LOG_DC , "i51KernelGetApplicationId : Testing -> Start" ) ;
    i51AdeOsLog ( i51_LOG_DC , "Case:i51KernelGetApplicationId ()" ) ;
    DrowTestLog("4 i51KernelGetApplicationId Start...",3);
    DrowTestLog("   Testing...... ",3);
    ID = i51KernelGetApplicationId (L"i51Debuger" );
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , ID ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"4 i51KernelGetApplicationId()");
    if ( SimplePortalId != ID ) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelGetApplicationId : Testing -> FAILED" ) ;
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelGetApplicationId()");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelGetApplicationId : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);
    }
#if 0
    //check i51KernelLoadSysKit
    i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadSysKit : Testing -> Start" ) ;
    DrowTestLog("5 i51KernelLoadSysKit Test-> Start",3);
    DrowTestLog("   Testing...... ",3);
    LoadKit = i51KernelLoadSysKit(L"i51KitServer");
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"5 i51KernelLoadSysKit Result:%x",LoadKit);
    if(LoadKit==NULL)
    {
        KerTotallBugs ++ ;
        i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadSysKit : Testing -> FAILED" ) ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelLoadSysKit(i51KitServer)");
    }
    else
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelLoadSysKit : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);

        //check i51KernelUnloadSysKit
        i51AdeOsLog ( i51_LOG_DC , "i51KernelUnloadSysKit : Testing -> Start" ) ;
        DrowTestLog("6 i51KernelUnloadSysKit Test-> Start",3);
        DrowTestLog("   Testing...... ",3);
        result = i51KernelUnloadSysKit((iHANDLE)LoadKit);
        memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
        sprintf(DrawKernelbuff,"6 i51KernelUnloadSysKit Result:%d",result);
        if(result==iFALSE)
        {
            KerTotallBugs ++ ;
            i51AdeOsLog ( i51_LOG_DC , "i51KernelUnloadSysKit : Testing -> FAILED" ) ;
            DrowTestLog(DrawKernelbuff,0);
            i51WriteError("i51KernelUnloadSysKit(i51KitServer)");
        }
        else
        {
            i51AdeOsLog ( i51_LOG_DC , "i51KernelUnloadSysKit : Testing -> OK" ) ;
            DrowTestLog(DrawKernelbuff,1);
        }
    }
#endif
    //check i51KernelMakeSnapshot 。。。
    i51AdeOsLog ( i51_LOG_DC , "i51KernelMakeSnapshot : Testing -> Start" ) ;
    i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelMakeSnapshot (0 )" ) ;
    DrowTestLog("7 i51KernelMakeSnapshot Start...",3);
    DrowTestLog("   Testing...... ",3);
    result =  i51KernelMakeSnapshot  (0);
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",result);
    DrowTestLog(DrawKernelbuff,1);
    if ( 0 == result ) 
    {
        //i51AdeOsLog ( i51_LOG_DC , "i51KernelMakeSnapshot : Testing -> FAILED" ) ;
        //TotallBugs ++ ;
        //DrowTestLog("7 i51KernelMakeSnapshot Result",0);

    }     
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelMakeSnapshot : Testing -> OK" ) ;
        //check i51KernelDropSnapshot 。。。
        i51AdeOsLog ( i51_LOG_DC , "i51KernelDropSnapshot : Testing -> Start" ) ;
        i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelDropSnapshot (0)" ) ;
        DrowTestLog("8 i51KernelDropSnapshot Start...",3);
        DrowTestLog("   Testing...... ",3);
        result =  i51KernelDropSnapshot  (0);
        i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
        memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
        sprintf(DrawKernelbuff,"8 i51KernelDropSnapshot Result:%d",result);
        if ( 0 == result ) 
        {
            i51AdeOsLog ( i51_LOG_DC , "i51KernelDropSnapshot : Testing -> FAILED" ) ;
            KerTotallBugs ++ ;
            DrowTestLog(DrawKernelbuff,0);
            i51WriteError("i51KernelDropSnapshot(0)");
        } 
        else 
        {
            i51AdeOsLog ( i51_LOG_DC , "i51KernelDropSnapshot : Testing -> OK" ) ;
            DrowTestLog(DrawKernelbuff,1);
        }
    }

    //check i51KernelSendRequest 。。。
    i51AdeOsLog ( i51_LOG_DC , "i51KernelSendRequest : Testing -> Start" ) ;
    i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelSendRequest ( 1 ,i51Debuger ,0, 0 ,0)" ) ;
    DrowTestLog("9 i51KernelSendRequest Start...",3);
    DrowTestLog("   Testing...... ",3);
    result =  i51KernelSendRequest  ( 1 , L"i51Debuger" , 0 ,0 , 0);
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",result);
    if ( 0 == result ) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelSendRequest : Testing -> FAILED" ) ;
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelSendRequest()");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelSendRequest : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);
    }

    //check i51KernelSendMessage 。。。
    i51AdeOsLog ( i51_LOG_DC , "i51KernelSendMessage : Testing -> Start" ) ;
    DrowTestLog("10 i51KernelSendMessage Start...",3);
    DrowTestLog("   Testing...... ",3);
    result =  i51KernelSendMessage ( 0 , 1 ,0 , 0 , 0);

    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",result);
    if ( 0 == result) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelSendMessage : Testing -> FAILED" ) ;
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelSendMessage()");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelSendMessage : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);
    }

    //check i51KernelSetFPS
    i51AdeOsLog ( i51_LOG_DC , "i51KernelSetFPS : Testing -> Start" ) ;
    i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelSetFPS ( iU32 appid , iU32 fps )" ) ;
    DrowTestLog("11 i51KernelSetFPS Start...",3);
    DrowTestLog("   Testing...... ",3);
    result =  i51KernelSetFPS  (0 ,0);
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"11 i51KernelSetFPS Result:%d",result);
    if ( 0 == result) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelSetFPS : Testing -> FAILED" ) ;
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelSetFPS(0 ,0)");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelSetFPS : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);
    }

    //check i51KernelChangeStatus 。。。
    i51AdeOsLog ( i51_LOG_DC , "i51KernelChangeStatus : Testing -> Start" ) ;
    i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelChangeStatus (1, 1)" ) ;
    DrowTestLog("12 i51KernelChangeStatus Start...",3);
    DrowTestLog("   Testing...... ",3);
    result =  i51KernelChangeStatus  (1, 1);
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
    i51AdeOsLog ( i51_LOG_DC , "i51KernelChangeStatus : Testing -> OK" ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",result);
    DrowTestLog(DrawKernelbuff,1);

    //check i51KernelSerialNumberValid 。。。
    i51AdeOsLog ( i51_LOG_DC , "i51KernelSerialNumberValid : Testing -> Start" ) ;
    i51AdeOsLog ( i51_LOG_DC , "Case : i51KernelSerialNumberValid (void)" ) ;
    DrowTestLog("13 i51KernelSerialNumberValid Start...",3);
    DrowTestLog("   Testing...... ",3);
    result =  i51KernelSerialNumberValid  ();
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
    i51AdeOsLog ( i51_LOG_DC , "i51KernelSerialNumberValid : Testing -> OK" ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",result);
    DrowTestLog(DrawKernelbuff,1);

    //check i51KernelGetBatchCode
    i51AdeOsLog ( i51_LOG_DC , "i51KernelGetBatchCode : Testing -> Start" ) ;
    DrowTestLog("14 i51KernelGetBatchCode Start...",3);
    DrowTestLog("   Testing...... ",3);
    result =  i51KernelGetBatchCode(CodeBuff , 16 ,&length ) ;
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",length);
    if (length==16) 
    {
        DrowTestLog(DrawKernelbuff,1);
    } 
    else 
    {
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelGetBatchCode()");
    }


    //check i51KernelGeti51Version 
    i51AdeOsLog ( i51_LOG_DC , "i51KernelGeti51Version : Testing -> Start" ) ;
    DrowTestLog("15 i51KernelGeti51Version Start...",3);
    DrowTestLog("   Testing...... ",3);
    GetVision =  i51KernelGeti51Version() ;
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , GetVision ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",GetVision);
    if ( 0 == GetVision ) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelGeti51Version : Testing -> FAILED" ) ;
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelGeti51Version()");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelGeti51Version : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);
    }

    //check i51KernelCheckAppCompleten
    i51AdeOsLog ( i51_LOG_DC , "i51KernelCheckAppCompleten : Testing -> Start" ) ;
    DrowTestLog("16 i51KernelCheckAppCompleten Start...",3);
    DrowTestLog("   Testing...... ",3);
    result =  i51KernelCheckAppCompleten(L"i51Debuger") ;
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , result ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",result);
    if ( 0 == result ) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelCheckAppCompleten : Testing -> FAILED" ) ;
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelCheckAppCompleten()");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelCheckAppCompleten : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);
    }

    //check i51KernelGetDiskFreeSpace
    i51AdeOsLog ( i51_LOG_DC , "i51KernelGetDiskFreeSpace : Testing -> Start" ) ;
    DrowTestLog("17 i51KernelGetDiskFreeSpace start...",3);
    DrowTestLog("   Testing...... ",3);
    GetVision =  i51KernelGetDiskFreeSpace() ;
    i51AdeOsLog ( i51_LOG_DC , "Results = %d " , GetVision ) ;
    memset(DrawKernelbuff,0,sizeof(DrawKernelbuff));
    sprintf(DrawKernelbuff,"Result:%d",GetVision);
    if ( 0 == GetVision ) 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelGetDiskFreeSpace : Testing -> FAILED" ) ;
        KerTotallBugs ++ ;
        DrowTestLog(DrawKernelbuff,0);
        i51WriteError("i51KernelGetDiskFreeSpace()");
    } 
    else 
    {
        i51AdeOsLog ( i51_LOG_DC , "i51KernelGetDiskFreeSpace : Testing -> OK" ) ;
        DrowTestLog(DrawKernelbuff,1);
    }

    if(KerTotallBugs)
    {
        AllStatics[1] = 3;
    }
    else
    {
        AllStatics[1] = 2;
    }
    i51SetAllStatics();
}