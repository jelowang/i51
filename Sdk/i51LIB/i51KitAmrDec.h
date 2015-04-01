
#ifndef THE_i51KitAmrDec_H
#define THE_i51KitAmrDec_H

#include "i51.h"

#define RELEASE_i51KitAmrDec

i51KIT_DYNAMIC_KIT_DEF_BEGIN 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitAmrDecoder) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitAmrDecoderEx) 
i51KIT_DYNAMIC_KIT_DEF_END

i51KIT_DYNAMIC_KIT_DEF_TYPE(iS32 ,i51KitAmrDecoder ,( iU8* rbuff , iU32 rlength , iU8* wbuff , iU32  wlength ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iS32 ,i51KitAmrDecoderEx ,( const iU16 * rfilename,const iU16 * wfilename ) )

#ifdef RELEASE_i51KitAmrDec

#define i51KitAmrDecoder i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitAmrDec,i51KitAmrDecoder)
#define i51KitAmrDecoderEx i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitAmrDec,i51KitAmrDecoderEx)
#endif

i51KIT_DYNAMIC_KIT_DEC(i51KitAmrDec) ;

#endif

