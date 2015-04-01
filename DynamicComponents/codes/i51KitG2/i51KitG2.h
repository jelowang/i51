
#ifndef THE_i51KITG2_H
#define THE_i51KITG2_H
#include "i51.h"
//	最终发布动态库时，将此宏打开
//#define RELEASE_i51KitG2

#define i51KitG2COLOR(R, G, B) \
	( ( (R) >> 3 ) << 11 ) | ( ( (G) >> 2) << 5 ) | ( (B) >> 3 ) 

// 字符横屏显示
#define i51KITG2_FONT_WORDS_ROTATE_0     0X0001
// 字符竖屏
#define i51KITG2_FONT_WORDS_ROTATE_90   0X0002
// 字符有下划线
#define i51KITG2_FONT_UNDERLINE 0X0004
// 字符有背景色
#define i51KITG2_FONT_BKCOLOR 0X0008

#define i51KITG2_ALPHA_NONEED 0XFF

/*
typedef struct
{

	//	author : Otto
	//	since : 2011.8.19
	//	(C) 2011 , PKIG Tech. Co., Ltd.
    
	//	裁剪矩形
	//	左上角
	iS16 X ;
	iS16 Y ;
	//	绘制宽高
	iS16 Width ;
	iS16 Height ;

} iRECT ;
*/

enum
{
	i51KitG2_MESSAGE_TYPE_FILE = 0,
	i51KitG2_MESSAGE_TYPE_MEMORY
};

i51KIT_DYNAMIC_KIT_DEF_BEGIN
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2CleanScreen)

	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2DrawPoint)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2DrawLine)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2DrawPolygon)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2DrawArc)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2DrawCircle)
	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FillPolygon)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FillArc)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FillCircle)
	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2CreateImageContainer)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2CloseImageContainer)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2CheckImageContainer)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2OpenImageContainer)
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2UseImageContainer)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2DestroyImageContainer)
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2ImageContainerExist)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2SetContainerContentVersion)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2GetContainerContentVersion)
	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2SetImageMemoryCopy)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2UnsetImageMemoryCopy)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2GetImageAttr)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2DrawImage)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2MirrorImage)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FlipImage)	
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2SetRotateCenter)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2RotateImage)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2ImageFillRect)
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2GetImagePixel)

//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontIsExit)	
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontInstall)	
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontCreat)	
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontSetUse)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontCachEnable)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontCachDisable)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontGetDemension)	
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontSetGap)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontDraw)	
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2FontDrawEx)	
	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPEnable)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPDisable)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPCreate)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPDestroy)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPSetCoord)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPSetActivity)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPGetActivity)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPClean)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPGetAtrribute)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPiSFull)	
//	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2SetVPiSLoop)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2VPRender)
	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2Update)	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2SetAlpha)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2ScreenZoom)

	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2UpdataContainerFromImage )	
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2UpdataContainerFromRe )
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2ContainerImageIdExist)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2GetContainerAttr)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2ContainerDeleteImage)

	// start add by otto 2012.6.11
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG2StopDecode)
	// end add by otto 2012.6.11

i51KIT_DYNAMIC_KIT_DEF_END

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2CleanScreen,( iCOLOR Color, iRECT *Rect) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2DrawPoint,( iS32 X, iS32 Y, iCOLOR Color ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2DrawLine,( iS32 Point[][2], iCOLOR Color) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2DrawPolygon,( iU32 VerticeNumber, iS32 Point[][2], iCOLOR Color) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2DrawArc,( iS32 Point[][2], iCOLOR Color ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2DrawCircle,( iS32 X, iS32 Y, iU32 R, iCOLOR Color ) )

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FillPolygon,( iS32 Num, iS32 Point[][2], iCOLOR Color) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FillArc,( iS32 Point[][2], iCOLOR Color ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FillCircle,( iS32 X, iS32 Y, iS32 R, iCOLOR Color ) )

i51KIT_DYNAMIC_KIT_DEF_TYPE(iHANDLE,i51KitG2CreateImageContainer,(iFILE File, iU32 * ImageId, iU32 TotallImage,  iU16* ContainerPathName, void(* Process)( iU32 , iU32  , iHANDLE), void(* Done)(iHANDLE)))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2CloseImageContainer,( iHANDLE) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2CheckImageContainer,( iU16 * ContainerPathName) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iHANDLE,i51KitG2OpenImageContainer,( iU16 * ContainerPathName ) )
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2UseImageContainer,( iHANDLE handle ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2DestroyImageContainer,( iHANDLE ImageContainer, iU16 *ContainerPathName ) )
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2ImageContainerExist,( iU16*ContainerPathName) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2SetContainerContentVersion,( iHANDLE Handle, iU32 Version) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2GetContainerContentVersion,( iHANDLE Handle, iU32 * Version) )

i51KIT_DYNAMIC_KIT_DEF_TYPE(iU32,i51KitG2SetImageMemoryCopy,( iHANDLE Handle, iU32 TotalImage, iU32 ImageId[]) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iU32,i51KitG2UnsetImageMemoryCopy,( iHANDLE Handle, iU32 TotalImage, iU32 ImageId[]) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2GetImageAttr,(iHANDLE ContainerHandle, iU32 ImageID , iU32 * ImageWidth , iU32 * ImageHeight, iU8 * MemoryCopy) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2DrawImage,(iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2MirrorImage,(iHANDLE ContainerHandle,  iU32 ImageID , iRECT* DesRect , iRECT* SrcRect ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2FlipImage,( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect ) )
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2SetRotateCenter,( iS32 CenterX, iS32 CenterY ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2RotateImage, ( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect, iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle ))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2ImageFillRect,( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect ) )
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2GetImagePixel,( iHANDLE ContainerHandle,iU32 ImageID, iU32 TransparentColor, iRECT * SrcRect, iCOLOR * ColorBuff, iU32 BuffLength) )

//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontIsExit, ( iU16* Path,  iU16 * FontType ))
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontInstall, ( iU16* SrcPath,  iU16 *SrcFontType, iU16* DesPath,  iU16 *DesFontType ) )
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontCreat, ( void * text, iU16* SrcPath,  iU16 *SrcFontType, iU16* DesPath,  iU16 *DesFontType ) )
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontSetUse, ( iFILE ReFile, iU32 TypeheadID )  )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontCachEnable, ( iFILE ReFile, iU32 TypeheadID  ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontCachDisable, ( iFILE ReFile, iU32 TypeheadID  ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontGetDemension, (iFILE ReFile, iU32 TypeheadID, iU32 WordsNum, iU32 RowWords, iU8 WordsDirection , iS16 RowGap, iS16 ColumnGap , iU32 * DrawWidth, iU32 * DrawHeight) )
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontSetGap, ( iU16 RowGap, iU16 ColumnGap ))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontDraw, (iFILE Handle, iU32 TypeheadID, void* text ,  iU16 WordsNum, iRECT* DrawRect , iRECT * SrcRect, iU16 DrawDirect, iS16 WordsRowGap,   iS16 WordsColumnGap,  iCOLOR fgcolor , iCOLOR bgcolor, iU16 Effect ))
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2FontDrawEx, ( iU8 * TypeheadData, iU32 TypeheadNum, iU32 TypeheadHeight,  iCOLOR Color, iU32 X, iU32 Y))

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPEnable,(void) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPDisable,(void) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iHANDLE,i51KitG2VPCreate,(iU16 TotalElement, iRECT * Rect))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPDestroy,(iHANDLE vpanel))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPSetCoord,( iHANDLE vpanel , iS32 X, iS32 Y))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPSetActivity,(iHANDLE vpanel))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iHANDLE,i51KitG2VPGetActivity,(void))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPClean,(iHANDLE vpanel))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPGetAtrribute,( iHANDLE vpanel , iRECT * Rect , iU16 * ResidueSpace  ))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPiSFull,(iHANDLE vpanel))
//i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2SetVPiSLoop,(iHANDLE vpanel, iBOOL Loop))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2VPRender,(iHANDLE vpanel, iRECT * DrawRect))

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2Update,(void))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2SetAlpha,(iU8 Alpha))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitG2ScreenZoom,( iRECT * DesRect, iRECT * SrcRect, iU32 ZoomRateX, iU32 ZoomRateY ))

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2UpdataContainerFromImage, ( iHANDLE ContainerHandle , void * ImageMess, iU8 MessType, iU32 DesImageId))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2UpdataContainerFromRe ,( iHANDLE ContainerHandle , void * ReMess, iU32 SrcImageId , iU8 MessType, iU32 DesImageId))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2ContainerImageIdExist ,( iHANDLE ContainerHandle, iU32 ImageId))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2GetContainerAttr ,(iHANDLE ContainerHandle, iU32 * TotalCapacity, iU32 * UsedCapacity, iU32 * ResidueCapacity, iU32 * UsableMinID ))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG2ContainerDeleteImage ,( iHANDLE ContainerHandle, iU32 ImageId))

// start add by otto 2012.6.11
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL, i51KitG2StopDecode, (iHANDLE))
// end add by otto 2012.6.11

#if  1
#ifdef RELEASE_i51KitG2

#define i51KitG2CleanScreen i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2CleanScreen)
#define i51KitG2DrawPoint i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2DrawPoint)
#define i51KitG2DrawLine i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2DrawLine)
#define i51KitG2DrawPolygon i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2DrawPolygon)
#define i51KitG2DrawArc i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2DrawArc)
#define i51KitG2DrawCircle i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2DrawCircle)

#define i51KitG2FillPolygon i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FillPolygon)
#define i51KitG2FillArc i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FillArc)
#define i51KitG2FillCircle i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FillCircle)

#define i51KitG2CreateImageContainer i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2CreateImageContainer)
#define i51KitG2CloseImageContainer i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2CloseImageContainer)
#define i51KitG2CheckImageContainer i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2CheckImageContainer)
#define i51KitG2OpenImageContainer i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2OpenImageContainer)
//#define i51KitG2UseImageContainer i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2UseImageContainer)
#define i51KitG2DestroyImageContainer i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2DestroyImageContainer)
//#define i51KitG2ImageContainerExist i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2ImageContainerExist)
#define i51KitG2SetContainerContentVersion i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2SetContainerContentVersion)
#define i51KitG2GetContainerContentVersion i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2GetContainerContentVersion)

#define i51KitG2SetImageMemoryCopy i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2SetImageMemoryCopy)
#define i51KitG2UnsetImageMemoryCopy i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2UnsetImageMemoryCopy)
#define i51KitG2GetImageAttr i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2GetImageAttr)
#define i51KitG2DrawImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2DrawImage)
#define i51KitG2MirrorImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2MirrorImage)
#define i51KitG2FlipImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FlipImage)
//#define i51KitG2SetRotateCenter i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2SetRotateCenter)
#define i51KitG2RotateImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2RotateImage)
#define i51KitG2ImageFillRect i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2ImageFillRect)
//#define i51KitG2GetImagePixel i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2GetImagePixel)

//#define i51KitG2FontIsExit  i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontIsExit )
//#define i51KitG2FontInstall  i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontInstall )
//#define i51KitG2FontCreat  i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontCreat )
//#define i51KitG2FontSetUse  i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontSetUse )
#define i51KitG2FontCachEnable  i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontCachEnable )
#define i51KitG2FontCachDisable i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontCachDisable)
#define i51KitG2FontGetDemension  i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontGetDemension )
//#define i51KitG2FontSetGap i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontSetGap)
#define i51KitG2FontDraw  i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontDraw )
//#define i51KitG2FontDrawEx i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2FontDrawEx)

#define i51KitG2VPEnable i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPEnable)
#define i51KitG2VPDisable i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPDisable)
#define i51KitG2VPCreate i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPCreate)
#define i51KitG2VPDestroy i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPDestroy)
#define i51KitG2VPSetCoord i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPSetCoord)
#define i51KitG2VPSetActivity i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPSetActivity)
#define i51KitG2VPGetActivity i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPGetActivity)
#define i51KitG2VPClean i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPClean)
#define i51KitG2VPGetAtrribute i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPGetAtrribute)
#define i51KitG2VPiSFull i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPiSFull)
#define i51KitG2VPRender i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2VPRender)

#define i51KitG2Update i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2Update)
#define i51KitG2SetAlpha i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2SetAlpha)
#define i51KitG2ScreenZoom i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2ScreenZoom)

#define i51KitG2UpdataContainerFromImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2UpdataContainerFromImage )
#define i51KitG2UpdataContainerFromRe i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2UpdataContainerFromRe )
#define i51KitG2ContainerImageIdExist i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2ContainerImageIdExist)
#define i51KitG2GetContainerAttr i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2GetContainerAttr)
#define i51KitG2ContainerDeleteImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2ContainerDeleteImage)

// start add by otto 2012.6.11
#define i51KitG2StopDecode i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG2,i51KitG2StopDecode)
// end add by otto 2012.6.11

#endif

i51KIT_DYNAMIC_KIT_DEC(i51KitG2);

#endif
#endif

