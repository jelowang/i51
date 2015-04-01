
#ifndef THE_i51KitPng_H
#define THE_i51KitPng_H

#include "i51.h"

//#define  RELEASE_i51KitPng

i51KIT_DYNAMIC_KIT_DEF_BEGIN
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitPngGetAttribute)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitPngDecodeImage)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitPngDrawImage)
i51KIT_DYNAMIC_KIT_DEF_END

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitPngGetAttribute, (iHANDLE ImageHandle, iU8 ImageHandleType,  iU32 * ImageWidth, iU32 * ImageHeight) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitPngDecodeImage, (iHANDLE ImageHandle, iU8 ImageHandleType, iU32 ImageFileSize, iHANDLE DesHandle, iU8 DesHandleType ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitPngDrawImage, (iU32 TransColor, iRECT* DstRect, iRECT* SrcRect, iHANDLE ImageHandle, iU8 ImageHandleType, iU32 ImageFileSize) )

#ifdef RELEASE_i51KitPng

#define i51KitPngGetAttribute i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitPng,i51KitPngGetAttribute)
#define i51KitPngDecodeImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitPng,i51KitPngDecodeImage)
#define i51KitPngDrawImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitPng,i51KitPngDrawImage)

#endif //RELEASE_i51KitPng

#endif

