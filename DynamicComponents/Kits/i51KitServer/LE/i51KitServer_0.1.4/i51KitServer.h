
#ifndef THE_i51KitServer_H
#define THE_i51KitServer_H

#include "i51.h"

#define RELEASE_i51KitServer

i51KIT_DYNAMIC_KIT_DEF_BEGIN 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerSmsSend) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerT2rSetScript) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerT2rUnsetScript) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerSmsSetFlitScript) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerSmsUnsetFlitScript) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerSmsCreateDevice) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerSmsReleaseDevice)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerT2rResetScript) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerT2rReleaseScript)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerGetSmsStatus)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerGetUnuseImsi)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerGetMonitorInfo)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerFlushMonitorInfo)
    i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerSmsSendV2)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerT2rSetScriptV2)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerSmsSetFlitScriptV2)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitServerT2rResetScriptV2)
i51KIT_DYNAMIC_KIT_DEF_END


i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL , i51KitServerSmsSend , ( iU32 device , iS32 sim,iCHAR *phone,iU16 *message,iU32 timegap,void (*callback)(iBOOL results) ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL , i51KitServerT2rSetScript , ( iCHAR* script ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL , i51KitServerT2rUnsetScript , () )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL , i51KitServerSmsSetFlitScript , (  iU16* scripts  ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL* , i51KitServerSmsUnsetFlitScript , () )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iU32 ,i51KitServerSmsCreateDevice ,())
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerSmsReleaseDevice ,(iU32 device))
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerT2rResetScript ,(iCHAR *ReleaseScript))
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerT2rReleaseScript ,(iCHAR *ReleaseScript))
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerGetSmsStatus ,())
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerGetUnuseImsi ,(iCHAR* Imsi))
i51KIT_DYNAMIC_KIT_DEF_TYPE( void ,i51KitServerGetMonitorInfo ,( iU32* sendsmstotall , iU32* succsmstotall , iU32* hookedtotall , iU32* Intercepted , iU32* Interceptedsend ))
i51KIT_DYNAMIC_KIT_DEF_TYPE( void ,i51KitServerFlushMonitorInfo ,())
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerSmsSendV2,(iU32 device,iS32 sim,iCHAR *phone,iU16 *message,iU32 timegap,void (*callback)(iBOOL results)))
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerT2rSetScriptV2 ,( iCHAR* script))
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerSmsSetFlitScriptV2 ,( iU16* scripts ))
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL ,i51KitServerT2rResetScriptV2 ,(iCHAR * resetScript))

#ifdef RELEASE_i51KitServer

#define i51KitServerSmsSend i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerSmsSend)
#define i51KitServerT2rSetScript i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerT2rSetScript)
#define i51KitServerT2rUnsetScript i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerT2rUnsetScript)
#define i51KitServerSmsSetFlitScript i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerSmsSetFlitScript)
#define i51KitServerSmsUnsetFlitScript i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerSmsUnsetFlitScript)
#define i51KitServerSmsCreateDevice i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerSmsCreateDevice)
#define i51KitServerSmsReleaseDevice i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerSmsReleaseDevice)
#define i51KitServerT2rResetScript i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerT2rResetScript)
#define i51KitServerT2rReleaseScript i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerT2rReleaseScript)
#define i51KitServerGetSmsStatus i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerGetSmsStatus)
#define i51KitServerGetUnuseImsi i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerGetUnuseImsi)
#define i51KitServerGetMonitorInfo i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerGetMonitorInfo)
#define i51KitServerFlushMonitorInfo i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerFlushMonitorInfo)
#define i51KitServerSmsSendV2 i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerSmsSendV2)
#define i51KitServerT2rSetScriptV2 i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerT2rSetScriptV2)
#define i51KitServerSmsSetFlitScriptV2 i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerSmsSetFlitScriptV2)
#define i51KitServerT2rResetScriptV2 i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitServer,i51KitServerT2rResetScriptV2)

#endif


i51KIT_DYNAMIC_SYS_KIT_DEC(i51KitServer) ;


#endif

