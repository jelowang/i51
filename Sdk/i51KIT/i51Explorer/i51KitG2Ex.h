#ifndef _localG2_h_
#define  _localG2_h_

#include "i51.h"
#include "i51KitAde.h"
#define i51KITG2_ALPHA_NONEED 0XFF


#define i51KitG2COLOR(R, G, B) \
	( ( (R) >> 3 ) << 11 ) | ( ( (G) >> 2) << 5 ) | ( (B) >> 3 )
extern iU32 i51KitG2ImageCreate( iU32  SrcData);
extern iU32 i51KitG2GetImageAttrEx(iU32  SrcData, iU32 * ImageWidth, iU32 * ImageHeight);
extern iCOLOR i51KitG2GetNewColorEx (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha);
extern iBOOL i51KitG2ImageDestory( iU32  SrcData);
extern iBOOL i51KitG2DrawImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect);
extern iBOOL i51KitG2CleanScreenEx(iCOLOR Color, iRECT *Rect);
extern iBOOL i51KitG2CleanScreenBaseColorEx(iCOLOR Color, iRECT *Rect,iALPHA alpha);
extern iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color);
#endif