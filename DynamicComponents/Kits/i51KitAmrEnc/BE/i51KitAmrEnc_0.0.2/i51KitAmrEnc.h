
#ifndef THE_i51KitAmrEnc_H
#define THE_i51KitAmrEnc_H

#include "i51.h"

//#define RELEASE_i51KitAmrEnc

i51KIT_DYNAMIC_KIT_DEF_BEGIN

i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitAmrGetLength)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitAmrGetLengthEx)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitAmrEncoder)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitAmrEncoderEx)

i51KIT_DYNAMIC_KIT_DEF_END


i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitAmrGetLength, (iFILE inFile, iU32 *outLength) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitAmrGetLengthEx, (iU8 *inBuffer, iU32 inBuffLen, iU8 *outLength) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitAmrEncoder, (iFILE inFile, iU8 *outBuffer, iU32 outBufSize, void (*process)(iU32 , iU32), void (*result)(iU32)) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitAmrEncoderEx, (iU8 *inBuffer, iU32 inBuffLen, iU8 *outBuffer, iU32 outBufSize, void (*process)(iU32 , iU32), void (*result)(iU32)) )


#ifdef RELEASE_i51KitAmrEnc

#define i51KitAmrGetLength i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitAmrEnc,i51KitAmrGetLength)
#define i51KitAmrGetLengthEx i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitAmrEnc,i51KitAmrGetLengthEx)
#define i51KitAmrEncoder i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitAmrEnc,i51KitAmrEncoder)
#define i51KitAmrEncoderEx i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitAmrEnc,i51KitAmrEncoderEx)

#endif 

#endif

