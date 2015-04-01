
#include "i51KitAde.h"
#include "TReport.h"

//i51KIT_STATIC_KIT_DEF(Adapter);

iBOOL i51KitTReportCreateHeader ( iFILE file ) {

    //    author : jelo
    //    since : 2012.4.10
    //    PKIG Tech. Co., Ltd.    

    //    生成测试报告文件头

    TREPORT report = { 0 } ;

    if ( 0 == file ) {
        iLog ( "KIT - TR : Create Header F") ;
        return iFALSE ;
    }


    report.Magic[0] = 'T' ;
    report.Magic[1] = 'R' ;
    report.Version = 1 ;
    report.TotallCases = 0 ;
    report.CaseEntry = sizeof(TREPORT) ;

    i51AdeOsGetTime ( 
        &report.CreateTime.Year ,
        &report.CreateTime.Month ,
        &report.CreateTime.Day ,
        &report.CreateTime.Hour ,
        &report.CreateTime.Minute ,
        &report.CreateTime.Second
        ) ;

    // DotpayInfo 暂不对接，初始化为0
    report.DotpayInfo.DotpayVersion = 0;
    report.DotpayInfo.InitTime1 = 0;
    report.DotpayInfo.InitTime2 = 0;

    i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
    i51AdeOsFileWrite ( file , &report , sizeof(TREPORT) , 0 ) ;

    return iTRUE ;

}

iBOOL i51KitTReportReadHeader ( iFILE file , TREPORT* report ) {

    //    author : jelo
    //    since : 2012.4.10
    //    PKIG Tech. Co., Ltd.    

    //    读取测试报告文件头

    if ( 0 == file ) {
        iLog ( "KIT - TR : Get Header F") ;
        return iFALSE ;
    }

    i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
    i51AdeOsFileRead ( file , report , sizeof(TREPORT) , 0 ) ;

    return iTRUE ;

}

iBOOL i51KitTReportUpdateHeader ( iFILE file , TREPORT* report ) {

    //    author : jelo
    //    since : 2012.4.10
    //    PKIG Tech. Co., Ltd.    

    //    更新测试报告文件头

    if ( 0 == file ) {
        iLog ( "KIT - TR : Write Header F") ;
        return iFALSE ;
    }

    i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
    i51AdeOsFileWrite ( file , report , sizeof(TREPORT) , 0 ) ;

    return iTRUE ;

}


iBOOL i51KitTReportAddCase ( iFILE file , iU32 casetype , iBOOL results ) {

    //    author : jelo
    //    since : 2012.4.10
    //    PKIG Tech. Co., Ltd.

    //    添加测试用例测试结果

    TREPORT report = { 0 } ;
    CASE cases = { 0 } ;

    if ( 0 == file ) {
        iLog ( "KIT - TR : Add Case F") ;
        return iFALSE ;
    }

    cases.type = casetype ;
    cases.results = results ;

    i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_TAIL ) ;
    i51AdeOsFileWrite ( file , &cases.type , sizeof(cases.type) , 0 ) ;
    i51AdeOsFileWrite ( file , &cases.results , sizeof(cases.results) , 0 ) ;

    //    文件头用例计数器加一
    i51KitTReportReadHeader ( file , &report ) ;
    report.TotallCases ++ ;
    i51KitTReportUpdateHeader ( file , &report ) ;

    return iTRUE ;


}