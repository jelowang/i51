
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

#endif


i51KIT_DYNAMIC_SYS_KIT_DEC(i51KitServer) ;


#endif

