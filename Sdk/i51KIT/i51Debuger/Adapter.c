#include "i51Debuger.h"

extern iU8 AllStatics[21];
void i51AdeTesterRun () {

    //    author : Jelo 
    //    since : 2011.11.1
    //     (C) 2011 , PKIG Tech. Co., Ltd.    

    iU32 results = 0 ;
    iU16 AdeTotallBugs = 0;
    iU8 state = 0;
    static iCHAR drawbuff[64] = {0};
    //iBOOL returnFlag = 0;

    switch (state) {

        //    For i51AdeOs Testing
        case 0 :
#if 1
            //    i51AdeOsMalloc
            //    i51AdeOsFree
            DrowTestLog("i51AdeTest Start........",3);
            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsMalloc Start..." ) ;
            DrowTestLog("Case : 1024*1024*1024",3);
            i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsMalloc ( 1024*1024*1024 , __FILE__ , __LINE__ )" ) ;
            //    分配1G内存
            //results = (iU32)i51AdeOsMalloc ( 1024*1024*1024 , __FILE__ , __LINE__ ) ;
            results = (iU32)i51AdeOsMalloc ( 1024 , __FILE__ , __LINE__ ) ;

            i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;
            memset(drawbuff,0,sizeof(drawbuff));
            sprintf(drawbuff,"Results = %x " , results ) ;
            DrowTestLog(drawbuff,3);

            //    分配1bytes内存
            //DrowTestLog("Case : i51AdeOsMalloc ( 1 , __FILE__ , __LINE__ )",3);
            //i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsMalloc ( 1 , __FILE__ , __LINE__ )" ) ;
            //results = (iU32)i51AdeOsMalloc ( 1 , __FILE__ , __LINE__ ) ;

            i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

            if ( 0 == results ) {
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsMalloc : Testing -> FAILED" ) ;
                AdeTotallBugs ++ ;
                DrowTestLog("i51AdeOsMalloc",0);
                i51WriteError("i51AdeOsMalloc(1024);");
            } else {                

                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsMalloc : Testing -> OK" ) ;
                DrowTestLog("i51AdeOsMalloc",1);

                i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFree:Testing->Start" ) ;
                DrowTestLog("i51AdeOsFree Start...",3);
                i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFree ( ... )" ) ;
                i51AdeOsFree ( (void*) results ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFree:Testing->OK" ) ;
                DrowTestLog("i51AdeOsFree",1);
            }

            //    i51AdeOsGetTotallMemoryLeft
            i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsGetTotallMemoryLeft : Testing -> Start" ) ;
            i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsGetTotallMemoryLeft ( void )" ) ;
            DrowTestLog("i51AdeOsGetTotallMemoryLeft Start...",3);
            results = i51AdeOsGetTotallMemoryLeft () ;
            i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;
            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsGetTotallMemoryLeft : Testing -> OK" ) ;
            DrowTestLog("i51AdeOsGetTotallMemoryLeft ",1);

            //    i51AdeOsFileCreateDir
            i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCreateDir : Testing -> Start" ) ;
            i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileCreateDir ( \"CreateDirTest\" )" ) ;
            DrowTestLog("i51AdeOsFileCreateDir Start...",3);
            results = i51AdeOsFileCreateDir ( L"CreateDirTest" ) ;
            i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

            if ( 0 == results ) {
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCreateDir : Testing -> FAILED" ) ;
                AdeTotallBugs ++ ;
                DrowTestLog("i51AdeOsFileCreateDir",0);
                i51WriteError("i51AdeOsFileCreateDir () ;");
            } else {
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCreateDir : Testing -> OK" ) ;
                DrowTestLog("i51AdeOsFileCreateDir",1);
            }

            //    i51AdeOsFileIsDirExist
            i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> Start" ) ;
            i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileIsDirExist ( \"CreateDirTest\" )" ) ;
            DrowTestLog("i51AdeOsFileIsDirExist Start...",3);
            DrowTestLog("Case:i51AdeOsFileIsDirExist ()" ,3);
            results = i51AdeOsFileIsDirExist ( L"CreateDirTest" ) ;
            i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

            if ( 0 == results ) {
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> FAILED" ) ;
                AdeTotallBugs ++ ;
                DrowTestLog("i51AdeOsFileIsDirExist ",0);
                i51WriteError("i51AdeOsFileIsDirExist ();");
            } else {
                //    继续下一个
                i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileIsDirExist ( \"CreateDirTestEx\" )" ) ;
                DrowTestLog("Case:i51AdeOsFileIsDirExist ()" ,3);
                results = i51AdeOsFileIsDirExist ( L"CreateDirTestEx" ) ;
                i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;                

                if ( 0 == results ) {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> OK" ) ;        
                    DrowTestLog("i51AdeOsFileIsDirExist" ,1);
                } else {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> FAILED" ) ;
                    AdeTotallBugs ++ ;
                    i51WriteError("i51AdeOsFileIsDirExist ( );");
                    DrowTestLog("i51AdeOsFileIsDirExist" ,0);
                }

            }

            //    i51AdeOsFileDeleteDir
            i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> Start 1" ) ;
            i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileDeleteDir ()" ) ;
            DrowTestLog("i51AdeOsFileDeleteDir Start..." ,3);
            DrowTestLog("Case:i51AdeOsFileDeleteDir ()" ,3);
            results = i51AdeOsFileDeleteDir ( L"CreateDirTest" ) ;
            i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

            if ( 0 == results ) {
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> FAILED" ) ;
                AdeTotallBugs ++ ;
                DrowTestLog("i51AdeOsFileDeleteDir " ,0);
                i51WriteError("i51AdeOsFileDeleteDir ();");
            } else {

                i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileDeleteDir ( \"CreateDirTestEx\" )" ) ;
                DrowTestLog("Case:i51AdeOsFileDeleteDir()" ,3);
                results = i51AdeOsFileDeleteDir ( L"CreateDirTestEx" ) ;
                i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

                if ( 0 == results ) {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> OK" ) ;    
                    DrowTestLog("i51AdeOsFileDeleteDir" ,1);
                } else {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> FAILED" ) ;
                    AdeTotallBugs ++ ;
                    DrowTestLog("i51AdeOsFileDeleteDir" ,0);
                    i51WriteError("i51AdeOsFileDeleteDir();");
                }

            }

            //    i51AdeOsFileOpen
            i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen Start..." ) ;
            DrowTestLog("i51AdeOsFileOpen:Testing -> Start" ,3);
            DrowTestLog("case EXISTING_OPEN |READ",3);
            i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileOpen ( L\"CreateFileTest , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__\")" ) ;
            results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
            i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

            if ( 0 != results ) {
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen : Testing -> FAILED" ) ;
                AdeTotallBugs ++ ;
                DrowTestLog("i51AdeOsFileOpen" ,0);
                i51WriteError("i51AdeOsFileOpen ();");
            } else {

                //    继续下一个
                DrowTestLog("case ALWAYS_OPEN |WRITE" ,3);
                i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileOpen ( L\"CreateFileTest , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__\")" ) ;
                results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

                if ( 0 == results ) {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen : Testing -> FAILED" ) ;
                    AdeTotallBugs ++ ;
                    DrowTestLog("i51AdeOsFileOpen" ,0);
                    i51WriteError("i51AdeOsFileOpen ();");
                } else {

                    //    i51AdeOsFileClose
                    DrowTestLog("i51AdeOsFileClose Start..." ,1);
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileClose : Testing -> Start" ) ;
                    results = i51AdeOsFileClose ( results ) ;
                    i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileClose" ) ;
                    i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

                    if ( 0 == results ) {
                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileClose : Testing -> FAILED" ) ;
                        AdeTotallBugs ++ ;
                        DrowTestLog("i51AdeOsFileClose" ,0);
                        i51WriteError("i51AdeOsFileClose ( );");
                    } else {

                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileClose : Testing -> OK" ) ;
                        DrowTestLog("i51AdeOsFileClose" ,1);

                        i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileOpen ( L\"CreateFileTest , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__\")" ) ;
                        results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
                        i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

                        if ( 0 == results ) {
                            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen : Testing -> FAILED" ) ;
                            AdeTotallBugs ++ ;
                            DrowTestLog ("i51AdeOsFileOpen" ,0) ;
                            i51WriteError("i51AdeOsFileOpen();");
                        } else {

                            iBOOL ret = 0 ;
                            iU32 length = 0 ;

                            i51AdeOsFileClose ( results ) ;
                            results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;

                            //    i51AdeOsFileWrite
                            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileWrite : Testing -> Start" ) ;
                            i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileWrite ( results , \"TEST\" , 4 , 0 )" ) ;
                            DrowTestLog ("i51AdeOsFileWrite Start..." ,3) ;
                            ret = i51AdeOsFileWrite ( results , "TEST" , 4 , 0 ) ;
                            i51AdeOsFileWrite ( results , "TEST" , 4 , &length ) ;
                            i51AdeOsLog ( i51_LOG_DC , "Results = %x " , ret ) ;
                            i51AdeOsFileClose ( results ) ;

                            if ( length == 4 && ret == iTRUE ) {

                                iCHAR buffer [ 10 ] = { 0 } ;
                                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileWrite : Testing -> OK" ) ;
                                DrowTestLog ("i51AdeOsFileWrite" ,1) ;

                                i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;

                                //    i51AdeOsFileRead
                                results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
                                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileRead : Testing -> Start" ) ;
                                DrowTestLog ("i51AdeOsFileRead Start..." ,3) ;
                                i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileRead ( results , buffer , 8 , 0 )" ) ;
                                ret = i51AdeOsFileRead ( results , buffer , 8 , 0 ) ;                                
                                i51AdeOsLog ( i51_LOG_DC , "Results = %d " , ret ) ;
                                i51AdeOsFileClose ( results ) ;

                                if ( 0 == ret || 0 != strcmp (buffer,"TESTTEST") ) {
                                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileRead : Testing -> FAILED" ) ;
                                    AdeTotallBugs ++ ;
                                    DrowTestLog ("i51AdeOsFileRead " ,0) ;
                                    i51WriteError("i51AdeOsFileRead();");
                                } else {

                                    iU32 counter = 0 ;

                                    results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
                                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileRead : Testing -> Start" ) ;
                                    DrowTestLog ("i51AdeOsFileRead Start..." ,3) ;
                                    i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileRead ( results , buffer , 8 , &counter )" ) ;
                                    ret = i51AdeOsFileRead ( results , buffer , 8 , &counter ) ;                                
                                    i51AdeOsLog ( i51_LOG_DC , "Results = %x " , ret ) ;
                                    i51AdeOsFileClose ( results ) ;

                                    if ( 0 == ret || 8 != counter ) {
                                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileRead : Testing -> FAILED" ) ;
                                        AdeTotallBugs ++ ;
                                        DrowTestLog ("i51AdeOsFileRead" ,0) ;
                                        i51WriteError("i51AdeOsFileRead( );");
                                    } 

                                }

                            } else {
                                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileWrite : Testing -> FAILED" ) ;
                                AdeTotallBugs ++ ;
                                DrowTestLog ("i51AdeOsFileWrite" ,0) ;
                                i51WriteError("i51AdeOsFileWrite();");
                            }
                        }

                    }

                }

            }
#endif        

            //    For i51AdeOsFileFind、i51AdeOsFileFindNext、i51AdeOsFileFindClose Testing
            {

                iFILE file = 0 ;
                i51FILEINFO info = { 0 } ;
                iCHAR bbb[32] = {0};
                iBOOL done = iTRUE  ;
                iU8 num1 = 0,num2 = 0;

                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind : Testing -> Start" ) ;
                DrowTestLog("i51AdeOsFileFind Start..." ,3);

                i51AdeOsFileCreateDir ( L"Dir1" ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCreateDir dir1" ) ;
                i51AdeOsFileCreateDir ( L"Dir1\\Dir2" ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCreateDir dir2" ) ;
                //results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\CreateFileTest1.xxx" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\Test1.xx" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen Test1.xx" ) ;
                i51AdeOsFileClose ( results ) ;
                i51AdeOsLog ( i51_LOG_DC , "close Test1.xx" ) ;
                //i51AdeOsFileSetAttr ( L"Dir1\\Dir2\\CreateFileTest1.xxx" , i51_OS_FL_ATTR_HID ) ;
                i51AdeOsFileSetAttr ( L"Dir1\\Dir2\\Test1.xx" , i51_OS_FL_ATTR_HID ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileSetAttr Test1.xx" ) ;
                //results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\CreateFileTest2.x" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\Test2.x" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen Test2.x" ) ;
                i51AdeOsFileClose ( results ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileClose Test2.x" ) ;

                file = i51AdeOsFileFind ( L"Dir1\\Dir2\\*" , &info ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFindClose,%d",file ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind Dir1\\Dir2\\*,reslut:%d",file ) ;
                i51AdeStdUnicode2Ascii(info.name,bbb);
                if ( 0 != file ) 
                {
                    while ( 1 ) 
                    {  
                        if (( 0 == i51AdeStdWStrcmp ( info.name , L"Test1.xx" ))&&(i51_OS_FL_ATTR_HID == (info.attribute & i51_OS_FL_ATTR_HID )))
                        {
                            num1++;
                            i51AdeOsLog(0,"i51AdeOsFileFind Test1.xx succeed");
                        }
                        if ( 0 == i51AdeStdWStrcmp ( info.name , L"Test2.x" )) 
                        {
                            num2++;
                            i51AdeOsLog(0,"i51AdeOsFileFind Test2.x succeed");
                        }

                        i51AdeOsLog ( i51_LOG_DC , "goon check" ) ;
                        memset(&info,0,sizeof(info));
                        if ( 0 == i51AdeOsFileFindNext ( file , &info ) ) 
                        {
                            break ;
                        }
                    }
                    if(num1==1&&num2==1)
                    {
                        i51AdeOsLog(0,"i51AdeOsFileFind SUCCEED");
                        DrowTestLog ("i51AdeOsFileFind()" ,1) ;
                    }
                    else
                    {
                        i51AdeOsLog(0,"i51AdeOsFileFind FAILED");
                        AdeTotallBugs ++ ;
                        DrowTestLog("i51AdeOsFileFind " ,0);
                        i51WriteError("i51AdeOsFileFind();");
                    }
                    i51AdeOsFileFindClose ( file ) ;
                }
                else
                {
                    AdeTotallBugs ++ ;
                    DrowTestLog("i51AdeOsFileFind " ,0);
                    i51WriteError("i51AdeOsFileFind();");

                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind FAILED1") ;
                }                            
            }

#if 1
            {
                iU32 ret = 0 ;
                iU32 ret2 = 0 ;

                //    i51AdeOsFileIsDirExist、i51AdeOsFileDeleteDir
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> Start" ) ;
                DrowTestLog("i51AdeOsFileIsDirExist  Start..." ,3);
                ret = i51AdeOsFileIsDirExist ( L"Dir1" ) ;
                ret2 = i51AdeOsFileIsDirExist ( L"Dir1\\Dir2" ) ;

                if ( 0 == ret || 0 == ret2 ) {

                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> FAILED" ) ;
                    DrowTestLog("i51AdeOsFileIsDirExist " ,0);
                    i51WriteError("i51AdeOsFileIsDirExist();");

                    if ( 0 == ret && 0 == ret2 ) {
                        AdeTotallBugs += 2 ;
                    } else {
                        AdeTotallBugs ++ ;
                    }

                } else {

                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> OK" ) ;
                    DrowTestLog("i51AdeOsFileIsDirExist " ,1);

                    i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> Start22" ) ;
                    DrowTestLog("i51AdeOsFileDeleteDir Start..." ,3);

                    ret = i51AdeOsFileDeleteDir ( L"Dir1" ) ;
                    i51AdeOsLog(0,"Results:%d",ret);

                    if ( 0 == ret ) {
                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> FAILED" ) ;
                        AdeTotallBugs ++ ;
                        DrowTestLog("i51AdeOsFileDeleteDir " ,0);
                        i51WriteError("i51AdeOsFileDeleteDir( );");
                    } else {

                        ret = i51AdeOsFileIsDirExist ( L"Dir1" ) ;
                        ret2 = i51AdeOsFileIsDirExist ( L"Dir1\\Dir2" ) ;
                        i51AdeOsLog(0,"Results:%d,Results2",ret,ret2);

                        if ( 1 == ret || 1 == ret2 ) {

                            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> FAILED" ) ;
                            DrowTestLog("i51AdeOsFileDeleteDir " ,0);
                            i51WriteError("i51AdeOsFileDeleteDir( );");

                            if ( 1 == ret && 1 == ret2 ) {
                                AdeTotallBugs += 2 ;
                            } else {
                                AdeTotallBugs ++ ;
                            }

                        } else {
                            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> OK" ) ;
                            DrowTestLog("i51AdeOsFileDeleteDir " ,1);
                        }


                    }

                }
                {
                    //    SEEK
                    iS32 seek = 0 ;
                    iS32 size = 0 ;

                    i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                    i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> Start" ) ;
                    DrowTestLog("SEEK : Start..." ,3);
                    results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                    i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_HEAD);

                    if ( 0 != seek ) {
                        i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
                        AdeTotallBugs ++ ;
                        DrowTestLog("SEEK : Testing " ,0);
                        i51WriteError("i51AdeOsFileGetSeek();");
                    } else {

                        i51AdeOsFileSetSeek ( results,8,i51_OS_FL_SEEK_HEAD ) ;
                        i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_HEAD);//i51_OS_FL_SEEK_CURRENT
                        //i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_TAIL);
                        i51AdeOsFileGetSize(results,&size);
                        i51AdeOsLog(0," case:i51AdeOsFileSetSeek ( results,8,i51_OS_FL_SEEK_HEAD ) ;");
                        i51AdeOsLog(0,"Result:seek:%d,size:%d",seek,size);
                        if ( 0 >= seek || seek != size ) {
                            i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
                            DrowTestLog("SEEK : Testing " ,0);
                            AdeTotallBugs ++ ;
                            i51WriteError("i51AdeOsFileSetSeek();");
                        } else {
                            i51AdeOsFileSetSeek ( results,4,i51_OS_FL_SEEK_HEAD ) ;
                            i51AdeOsFileSetSeek ( results,-2,i51_OS_FL_SEEK_CURRENT ) ;
                            i51AdeOsFileSetSeek ( results,1,i51_OS_FL_SEEK_CURRENT ) ;
                            i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_HEAD);//i51_OS_FL_SEEK_CURRENT
                            //i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_CURRENT);
                            i51AdeOsLog(0," Case:i51AdeOsFileSetSeek ( results,4,i51_OS_FL_SEEK_HEAD ) ;");
                            i51AdeOsLog(0," Case:i51AdeOsFileSetSeek ( results,-2,i51_OS_FL_SEEK_CURRENT ) ;");
                            i51AdeOsLog(0," Case1:i51AdeOsFileSetSeek ( results,1,i51_OS_FL_SEEK_CURRENT ) ;");
                            i51AdeOsLog(0,"Result:size:%d",seek);
                            if ( 3 != seek  ) {
                                i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
                                AdeTotallBugs ++ ;
                                DrowTestLog("SEEK : Testing " ,0);
                                i51WriteError("i51AdeOsFileSetSeek();");
                            } else {
                                i51AdeOsFileSetSeek ( results,0,i51_OS_FL_SEEK_TAIL ) ;
                                i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_HEAD);//i51_OS_FL_SEEK_CURRENT
                                //i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_CURRENT);
                                i51AdeOsLog(0," Case:i51AdeOsFileSetSeek ( results,0,i51_OS_FL_SEEK_TAIL ) ;");
                                if ( 8 != seek ) {
                                    i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
                                    AdeTotallBugs ++ ;
                                    DrowTestLog("SEEK : Testing " ,0);
                                    i51WriteError("i51AdeOsFileSetSeek();");
                                } else {
                                    i51AdeOsFileSetSeek ( results,-1,i51_OS_FL_SEEK_TAIL ) ;
                                    i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_HEAD);//i51_OS_FL_SEEK_CURRENT
                                    //i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_CURRENT);
                                    i51AdeOsLog(0," Case:i51AdeOsFileSetSeek ( results,-1,i51_OS_FL_SEEK_TAIL ) ;");
                                    if ( 7 != seek ) {
                                        i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
                                        AdeTotallBugs ++ ;
                                        DrowTestLog("SEEK : Testing " ,0);
                                        i51WriteError("i51AdeOsFileSetSeek();");
                                    } else {
                                        i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> OK" ) ;
                                        DrowTestLog("SEEK : Testing " ,1);
                                    }
                                }
                            }
                        }

                    }

                    i51AdeOsFileClose ( results ) ;

                }
            }
#endif
#if 1
            //    i51AdeOsFileSetAttr
            {
                iU32 attr = 0 ;
                i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileSetAttr : Testing -> Start" ) ;
                DrowTestLog("i51AdeOsFileSetAttr Start..." ,3);
                i51AdeOsFileSetAttr ( L"CreateFileTest" , i51_OS_FL_ATTR_ROL | i51_OS_FL_ATTR_HID ) ;
                attr = i51AdeOsFileGetAttr ( L"CreateFileTest" ) ;

                if ( (i51_OS_FL_ATTR_ROL != (attr & i51_OS_FL_ATTR_ROL )) 
                    && (i51_OS_FL_ATTR_HID != (attr & i51_OS_FL_ATTR_HID )) ) {

                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileSetAttr : Testing -> FAILED" ) ;
                        AdeTotallBugs ++ ;
                        i51WriteError("i51AdeOsFileSetAttr();");
                        DrowTestLog("i51AdeOsFileSetAttr:Testing " ,0);

                } else {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileSetAttr : Testing -> OK" ) ;
                    DrowTestLog("i51AdeOsFileSetAttr()" ,1);
                }

                i51AdeOsFileSetAttr ( L"CreateFileTest" , i51_OS_FL_ATTR_WRT ) ;

            }

            //    i51AdeOsFileDelete
            {
                iU32 ret = 0 ;
                i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDelete : Testing -> Start" ) ;
                DrowTestLog("i51AdeOsFileDelete Start..." ,3);
                ret = i51AdeOsFileDelete ( L"CreateFileTest" ) ;
                if ( 0 == ret ) {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDelete : Testing -> FAILED" ) ;
                    AdeTotallBugs ++ ;
                    DrowTestLog("i51AdeOsFileDelete()" ,0);
                    i51WriteError("i51AdeOsFileDelete();");
                } else {

                    ret = i51AdeOsFileIsExist ( L"CreateFileTest" ) ;

                    if ( 0 != ret ) {
                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDelete : Testing -> FAILED" ) ;
                        AdeTotallBugs ++ ;
                        DrowTestLog("i51AdeOsFileDelete()" ,0);
                        i51WriteError("i51AdeOsFileDelete();");
                    } else {
                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDelete : Testing -> OK" ) ;
                        DrowTestLog("i51AdeOsFileDelete()" ,1);
                    }

                }
            }

            //    i51AdeOsFileCopy
            {

                iU32 ret = 0 ;
                iCHAR writebuffer [ 10 ] = {"TESTTEST"} ;
                iCHAR readbuffer [ 10 ] = { 0 } ;

                i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> Start" ) ;
                DrowTestLog("i51AdeOsFileCopy Start..." ,3);
                i51AdeOsFileCreateDir ( L"Dir1" ) ;
                i51AdeOsFileCreateDir ( L"Dir1\\Dir2" ) ;
                results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\CreateFileTest" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                i51AdeOsFileWrite ( results , writebuffer , strlen("TESTTEST") , 0 ) ;
                i51AdeOsFileClose ( results ) ;

                i51AdeOsFileCreateDir ( L"Dir111" ) ;
                i51AdeOsFileCreateDir ( L"Dir111\\Dir222" ) ;                

                i51AdeOsFileCopy ( L"Dir1\\Dir2\\CreateFileTest" , L"Dir111\\Dir222\\CreateFileTest2" ) ;

                ret = i51AdeOsFileIsExist ( L"Dir111\\Dir222\\CreateFileTest2" ) ;

                if ( 0 == ret ) {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> FAILED" ) ;
                    AdeTotallBugs ++ ;
                    DrowTestLog("i51AdeOsFileCopy()" ,0);
                    i51WriteError("i51AdeOsFileCopy();");
                } else {

                    results = i51AdeOsFileOpen ( L"Dir111\\Dir222\\CreateFileTest2" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
                    i51AdeOsFileRead ( results , readbuffer , strlen("TESTTEST") , 0 ) ;
                    i51AdeOsFileClose ( results ) ;

                    if ( 0 != strcmp ( readbuffer , "TESTTEST" ) ) {

                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> FAILED" ) ;
                        AdeTotallBugs ++ ;
                        DrowTestLog("i51AdeOsFileCopy ()" ,0);
                        i51WriteError("i51AdeOsFileCopy();");

                    } else {

                        ret = i51AdeOsFileIsExist ( L"Dir1\\Dir2\\CreateFileTest" ) ;

                        if ( 0 == ret ) {
                            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> FAILED" ) ;
                            i51WriteError("i51AdeOsFileCopy();");
                            AdeTotallBugs ++ ;
                            DrowTestLog("i51AdeOsFileCopy()" ,0);
                        } else {
                            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> OK" ) ;
                            DrowTestLog("i51AdeOsFileCopy()" ,1);
                        }

                    }

                }

                //    clear up
                i51AdeOsFileDeleteDir ( L"Dir1" ) ;
                i51AdeOsFileDeleteDir ( L"Dir111" ) ;

            }

            //    i51AdeOsFileMove
            {

                iU32 ret = 0 ;
                iCHAR writebuffer [ 10 ] = {"TESTTEST"} ;
                iCHAR readbuffer [ 10 ] = { 0 } ;

                i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> Start" ) ;
                DrowTestLog("i51AdeOsFileMove Start..." ,3);
                i51AdeOsFileCreateDir ( L"Dir1" ) ;
                i51AdeOsFileCreateDir ( L"Dir1\\Dir2" ) ;
                results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\CreateFileTest" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
                i51AdeOsFileWrite ( results , writebuffer , strlen("TESTTEST") , 0 ) ;
                i51AdeOsFileClose ( results ) ;

                i51AdeOsFileCreateDir ( L"Dir111" ) ;
                i51AdeOsFileCreateDir ( L"Dir111\\Dir222" ) ;                

                i51AdeOsFileMove ( L"Dir1\\Dir2\\CreateFileTest" , L"Dir111\\Dir222\\CreateFileTest2" ) ;

                ret = i51AdeOsFileIsExist ( L"Dir111\\Dir222\\CreateFileTest2" ) ;

                if ( 0 == ret ) {
                    i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> FAILED" ) ;
                    AdeTotallBugs ++ ;
                    DrowTestLog("i51AdeOsFileMove()" ,0);
                    i51WriteError("i51AdeOsFileMove();");
                } else {

                    results = i51AdeOsFileOpen ( L"Dir111\\Dir222\\CreateFileTest2" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
                    i51AdeOsFileRead ( results , readbuffer , strlen("TESTTEST") , 0 ) ;
                    i51AdeOsFileClose ( results ) ;

                    if ( 0 != strcmp ( readbuffer , "TESTTEST" ) ) {

                        i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> FAILED" ) ;
                        AdeTotallBugs ++ ;
                        DrowTestLog("i51AdeOsFileMove()" ,0);
                        i51WriteError("i51AdeOsFileMove();");

                    } else {

                        ret = i51AdeOsFileIsExist ( L"Dir1\\Dir2\\CreateFileTest" ) ;

                        if ( 0 != ret ) {
                            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> FAILED" ) ;
                            AdeTotallBugs ++ ;
                            DrowTestLog("i51AdeOsFileMove()" ,0);
                            i51WriteError("i51AdeOsFileMove();");
                        } else {
                            i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> OK" ) ;
                            DrowTestLog("i51AdeOsFileMove()" ,1);
                        }

                    }

                }

                //    clear up
                i51AdeOsFileDeleteDir ( L"Dir1" ) ;
                i51AdeOsFileDeleteDir ( L"Dir111" ) ;

            }


            {
                //i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetSims)
                //i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetImsi)
                //i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetImei)
                //i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetCenter)
                iS32* sims = i51AdeSmsGetSims () ;

                i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                i51AdeOsLog ( i51_LOG_DC , "SIM  Start..." ) ;
                DrowTestLog("SIM : Testing -> Start" ,3);

                if ( 0 == sims ) {
                    i51AdeOsLog ( i51_LOG_DC , "SIM : Testing -> FAILED" ) ;
                    AdeTotallBugs ++ ;
                    DrowTestLog("SIM : Testing " ,0);
                    i51WriteError("i51AdeSmsGetSims ();");
                } else {

                    iCHAR* imsi = 0 ;
                    iCHAR* imei = 0 ;
                    iCHAR* imc = 0 ;
                    iBOOL ret = iTRUE ;

                    if ( 0 < sims[0] ) {

                        iS32 counter = 0 ;

                        imei = i51AdeSmsGetImei () ;

                        if ( 0 == imei ) {
                            AdeTotallBugs ++ ;
                            ret = iFALSE ;
                            i51WriteError("i51AdeSmsGetImei ();");
                        }                                        

                        for ( counter = 1 ; counter < sims[0]+1 ; counter ++ ) {

                            imsi = i51AdeSmsGetImsi ( sims[counter] ) ;                                    
                            imc = i51AdeSmsGetCenter ( sims[counter] ) ;

                            memset(drawbuff,0,sizeof(drawbuff));
                            if ( 0 == imsi ) {
                                AdeTotallBugs ++ ;
                                ret = iFALSE ;
                                sprintf(drawbuff,"i51AdeSmsGetImsi (sims[%d]);",counter);
                                i51WriteError(drawbuff);
                            }                                    

                            if ( 0 == imc ) {
                                AdeTotallBugs ++ ;
                                ret = iFALSE ;
                                sprintf(drawbuff,"i51AdeSmsGetCenter (sims[%d]);",counter);
                                i51WriteError(drawbuff);
                            }

                            if ( iFALSE == ret ) { 
                                i51AdeOsLog ( i51_LOG_DC , "SIM : Testing -> FAILED" ) ;
                                DrowTestLog("SIM : Testing " ,0);
                            } else {
                                i51AdeOsLog ( i51_LOG_DC , "SIM : Testing -> OK" ) ;
                                DrowTestLog("SIM : Testing " ,1);
                            }

                        }

                    }

                }

            }

            //    i51AdeStdWStrlen、i51AdeStdWStrcpy、i51AdeStdWStrcmp、i51AdeStdWStrcat
            {

                iU16 desrc [32] = { 0 } ;
                iU16* retsrc = 0 ;
                iBOOL ret = iTRUE ;

                i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
                i51AdeOsLog ( i51_LOG_DC , "WStr Start..." ) ;
                DrowTestLog("WStr : Testing -> Start" ,3);

                if ( 4 != i51AdeStdWStrlen ( L"TEST" ) ) {
                    AdeTotallBugs ++ ;
                    ret = iFALSE ;
                    i51WriteError("i51AdeStdWStrlen () ;");
                } 

                retsrc = i51AdeStdWStrcpy ( desrc , L"TEST" ) ;

                if ( (iU32 )retsrc != (iU32 )desrc ) {
                    AdeTotallBugs ++ ;
                    ret = iFALSE ;
                    i51WriteError("i51AdeStdWStrcpy () ;");
                }

                if ( 0 != i51AdeStdWStrcmp (desrc,L"TEST") ) {
                    AdeTotallBugs ++ ;
                    ret = iFALSE ;
                    i51WriteError("i51AdeStdWStrcmp () ;");
                }

                retsrc = i51AdeStdWStrcat ( desrc , L"Adapter" ) ;

                if ( (iU32 )retsrc != (iU32 )desrc ) {
                    AdeTotallBugs ++ ;
                    ret = iFALSE ;
                    i51WriteError("i51AdeStdWStrcat () ;");
                }

                if ( 0 != i51AdeStdWStrcmp (desrc,L"TESTAdapter") ) {
                    AdeTotallBugs ++ ;
                    ret = iFALSE ;
                    i51WriteError("i51AdeStdWStrcmp () ;");
                }

                if ( iFALSE == ret ) {
                    i51AdeOsLog ( i51_LOG_DC , "WStr : Testing -> FAILED" ) ;
                    DrowTestLog("WStr : Testing" ,0);
                } else {
                    i51AdeOsLog ( i51_LOG_DC , "WStr : Testing -> OK" ) ;
                    DrowTestLog("WStr : Testing" ,1);
                }

            }

            state = 1 ;

            i51AdeOsLog ( i51_LOG_DC , "i51AdeTester Results:" ) ;
            i51AdeOsLog ( i51_LOG_DC , "Totall Bugs : %d" , AdeTotallBugs ) ;

            if(AdeTotallBugs)
            {
                AllStatics[0] = 3;
            }
            else
            {
                AllStatics[0] = 2;
            }
            i51SetAllStatics();
#endif
            break ;


    }

}

