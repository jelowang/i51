#ifndef _localG2_h_
#define  _localG2_h_

#include "i51.h"
#include "i51KitAde.h"
//#define __SPR__   //大端时打开此宏

typedef struct  
{
	iS16 x;
	iS16 y;
}iPoint;

#define i51KitG2COLOR(R, G, B) \
	( ( (R) >> 3 ) << 11 ) | ( ( (G) >> 2) << 5 ) | ( (B) >> 3 ) 

extern iU32 i51KitG2ImageCreate( iU32  SrcData);
extern iU32 i51KitG2GetImageAttrEx(iU32  SrcData, iU32 * ImageWidth, iU32 * ImageHeight);
extern iCOLOR i51KitG2GetNewColorEx (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha);
extern iBOOL i51KitG2ImageDestory( iU32  SrcData);
extern iCOLOR i51KitG2GetNewColorEx1 (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha);
extern iBOOL i51KitG2DrawImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect);
extern iBOOL i51KitG2MirrorImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect);
extern iBOOL i51KitG2FlipImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect);
extern iS32 i51KitG2SinEx( iS32 Angle);
extern iS32 i51KitG2CosEx( iS32 Angle);
extern iBOOL i51KitG2RotatePointEx(iS32 SrcX, iS32 SrcY, iS32 RotateCenterX,  iS32 RotateCenterY ,iS32 * DesX, iS32 * DesY, iS32 Angle);
extern iBOOL i51KitG2GetGirthPointsEx(iRECT SrcRect, iRECT DesRect, iS16 PointS[][2], iS32 SrcMoveX, iS32 SrcMoveY,iS32 RotateCenterX,  iS32 RotateCenterY ,iU32 RotateType, iS32 Angle );
extern iBOOL RotateRamLinearImageEx  ( iU32 ImageData , iRECT* DesRect , iRECT* SrcRect , iS16 ChangeData[][2]);
extern iBOOL i51KitG2RotateImageEx(iU32 ImageData, iRECT* DesRect , iRECT* SrcRect,iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle );
extern iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color);
extern iBOOL i51KitG2CleanScreenEx(iCOLOR Color, iRECT *Rect);

extern iU8 LocalG2AlphaSys;
extern iU8 AlphaNeed;
extern iU8 AlphaSrc;



#endif