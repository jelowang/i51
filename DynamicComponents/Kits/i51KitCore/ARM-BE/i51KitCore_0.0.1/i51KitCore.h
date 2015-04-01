
#ifndef THE_i51KitCore_H
#define THE_i51KitCore_H

#include "i51.h"

//最终发布动态库时，将此宏打开
#define RELEASE_i51KitCore

i51KIT_DYNAMIC_KIT_DEF_BEGIN
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreSlDlAbleCreate)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreSlDlCreate)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreSlDlStop)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreSlDlStart)

i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreInstallApplication)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreInstallApplicationStop)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreReinstallApplication)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreReinstallApplicationStop)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreUninstallApplication)	

i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreExtractPackage)	
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitCoreZipUncompress)	
i51KIT_DYNAMIC_KIT_DEF_END

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreSlDlAbleCreate,() )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iHANDLE, i51KitCoreSlDlCreate,(void (*callback) (iBOOL ret)) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreSlDlStop,( iHANDLE dl ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreSlDlStart,( iHANDLE dl , const iCHAR *uri , iU32 urisize ,  const iCHAR* path , const iCHAR* name , iU32 versionId ,i51SlDlCOMMAND cmd , i51SlDlCALLBACK callback ) )

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreInstallApplication,( iU16 *AppName , void (*callback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount ) ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreInstallApplicationStop,() )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreReinstallApplication,( iU16* appname , iCHAR *uri , iU32 version , void (*callback)( iCHAR* currentstep , iS32 status , iU32 step , iU32 totall, iU32 curdl , iU32 dlcount ) ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreReinstallApplicationStop,() )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreUninstallApplication,( iU16* appname ) )

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitCoreExtractPackage,( iFILE handle , iU16 *AppName ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(int , i51KitCoreZipUncompress,(unsigned char *dest,unsigned long *destLen,const unsigned char *source,unsigned long sourceLen) )

#ifdef RELEASE_i51KitCore
#define i51KitCoreSlDlAbleCreate i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreSlDlAbleCreate)
#define i51KitCoreSlDlCreate i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreSlDlCreate)
#define i51KitCoreSlDlStop i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreSlDlStop)
#define i51KitCoreSlDlStart i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreSlDlStart)

#define i51KitCoreInstallApplication i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreInstallApplication)
#define i51KitCoreInstallApplicationStop i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreInstallApplicationStop)
#define i51KitCoreReinstallApplication i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreReinstallApplication)
#define i51KitCoreReinstallApplicationStop i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreReinstallApplicationStop)
#define i51KitCoreUninstallApplication i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreUninstallApplication)

#define i51KitCoreExtractPackage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreExtractPackage)
#define i51KitCoreZipUncompress i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitCore,i51KitCoreZipUncompress)
#endif

i51KIT_DYNAMIC_KIT_DEC(i51KitCore);

#endif

