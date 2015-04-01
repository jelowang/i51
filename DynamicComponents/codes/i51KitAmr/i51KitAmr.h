
#ifndef THE_i51KitAmr_H
#define THE_i51KitAmr_H

#include "i51.h"

//#define RELEASE_i51KitAmr

i51KIT_DYNAMIC_KIT_DEF_BEGIN 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitAmrDecoder) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitAmrDecoderEx) 
i51KIT_DYNAMIC_KIT_DEF_END

i51KIT_DYNAMIC_KIT_DEF_TYPE(iFILE , i51KitAmrDecoder , ( iU8* rbuff , iU32 rlength , iU8* wbuff , iU32  wlength ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iFILE , i51KitAmrDecoderEx , ( const iU16 * rfilename,const iU16 * wfilename ) )

#ifdef RELEASE_i51KitAmr

#define i51KitAmrDecoder i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitAmr,i51KitAmrDecoder)
#define i51KitAmrDecoderEx i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitAmr,i51KitAmrDecoderEx)
#endif

i51KIT_DYNAMIC_KIT_DEC(i51KitAmr) ;

#endif

