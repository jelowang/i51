
#include "i51KitAde.h"
#include "i51KitPng.h"
#include "jzip.h"
#include "png.h"

i51KIT_STATIC_KIT_DEF(Adapter);

#define PNGCODEC_DEMO 0

iBOOL i51KitPngGetAttribute(iHANDLE ImageHandle, iU8 ImageHandleType, iU32 * ImageWidth, iU32 * ImageHeight)
{
	//  author : yepan
	//  since  : 2012-7-20
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			ImageHandle		数据源指针
	//			ImageHandleType	0==数据源是文件句柄,1==数据源是内存指针
	//			ImageWidth		输出的图片宽度
	//			ImageHeight		输出的图片高度
	//
	//  return :
	//			iTRUE==成功， iFALSE==失败
	//  note   :
	//			获取图片信息

	return png_get_attribute(ImageHandle, ImageHandleType, ImageWidth, ImageHeight);
}

iBOOL i51KitPngDecodeImage(iHANDLE ImageHandle, iU8 ImageHandleType, iU32 ImageFileSize, iHANDLE DesHandle, iU8 DesHandleType )
{
	//  author : yepan
	//  since  : 2012-7-20
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			ImageHandle			数据源句柄
	//			ImageHandleType		0==数据源是文件句柄,1==数据源是内存指针
	//			ImageFileSize		大小, 如果数据源为文件句柄此项赋0
	//			DesHandle			目的对象指针
	//			DesHandleType		0==目的对象是文件句柄,1==目的对象是内存指针
	//  return :
	//			iTRUE==成功， iFALSE==失败
	//  note   :
	//			解码png图片

	iRECT SrcRect = {0};
	iRECT DesRect = {0};
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;

	if ( iNULL == ImageHandle ||  iNULL == DesHandle ){
		iLog("i51KitPngDecodeImage parameter err, ImageHandle is %x, DesHandle is %x", ImageHandle, DesHandle);
		return iFALSE;
	}

	if ( iFALSE == i51KitPngGetAttribute(ImageHandle, ImageHandleType, &ImageWidth, &ImageHeight) ){
		return iFALSE;
	}

	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = (iS16)ImageWidth;
	SrcRect.Height = (iS16)ImageHeight;

	DesRect.X = 0;
	DesRect.Y = 0;
	DesRect.Width = SrcRect.Width;
	DesRect.Height = SrcRect.Height;

	return png_decode(0, 0x000FFFFF, ImageHandle, ImageHandleType, ImageFileSize, &SrcRect, DesHandle, DesHandleType, &DesRect);
}

iBOOL i51KitPngDrawImage(iU32 TransColor, iRECT* DstRect, iRECT* SrcRect, iHANDLE ImageHandle, iU8 ImageHandleType, iU32 ImageFileSize)
{
	//  author : yepan
	//  since  : 2012-7-20
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			DesRect		目标绘制区域		
	//			SrcRect		数据源区域
	//			ImageHandle 数据源指针
	//			ImageHandleType	0==数据源是文件句柄,1==数据源是内存指针
	//			ImageFileSize	数据大小, 如果数据源为文件句柄此项赋0
	//  return :
	//			iTRUE==成功， iFALSE==失败
	//  note   :
	//		   画png图片

	iU32 *ScreenContext=iNULL;
	iU16 *ScreenBuffer=iNULL;
	iU16 ScreenWidth=0, ScreenHeight=0;
	iS16 TempX = 0, TempY=0;
	iRECT Dst = {0};

	ScreenContext =  i51AdeMmiGetPanel();

	if ( iNULL == ImageHandle || iNULL == ScreenContext ) return iFALSE;

	ScreenBuffer = (iU16 *)ScreenContext[1];
	if(  iFALSE == i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight )){
		return iFALSE;
	}

	//裁剪目的绘制区域
	Dst.X = DstRect->X < 0 ? 0 : DstRect->X;
	Dst.Y = DstRect->Y < 0 ? 0 : DstRect->Y;
	TempX = DstRect->X + DstRect->Width - 1;
	TempY = DstRect->Y + DstRect->Height - 1;
	TempX = TempX < ScreenWidth-1 ? TempX : ScreenWidth-1;
	TempY = TempY < ScreenHeight-1 ? TempY : ScreenHeight-1;
	Dst.Width = TempX - Dst.X + 1;
	Dst.Height = TempY - Dst.Y + 1;

	return png_decode(1, TransColor, ImageHandle, ImageHandleType, ImageFileSize, SrcRect, ScreenBuffer, 1, &Dst);
}

#if PNGCODEC_DEMO
static 	iFILE pc_ImgFile = iNULL;
static  iFILE pc_DstFile = iNULL;
static  iRECT pc_SrcRect = {0, 0, 160, 160};
static  iRECT pc_DstRect = {50, 50, 160, 160};
#endif

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{
	case i51_MSG_PAINT :
		{
#if PNGCODEC_DEMO
			iU32 *ScreenContext=iNULL;
			iU16 *ScreenBuffer=iNULL;
			iU16 ScreenWidth=0, ScreenHeight=0;

			ScreenContext =  i51AdeMmiGetPanel();
			if( iNULL == ScreenContext ) return iFALSE;
			ScreenBuffer = (iU16 *)ScreenContext[1];

			if(  iFALSE == i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight )){
				return iFALSE;
			}

			i51AdeStdMemset16(ScreenBuffer, 0xFF, sizeof(iU16)*ScreenWidth*ScreenHeight);

			//{
			//	pc_SrcRect.X -= 1;
			//	pc_SrcRect.Y -= 1;
			//	pc_SrcRect.Width -= 5;
			//	pc_SrcRect.Height -= 5;

			//	pc_DstRect.X += 5;
			//	pc_DstRect.Y += 5;
			//	pc_DstRect.Width += 2;
			//	pc_DstRect.Height += 2;

			//	//#define ALPHA_COLOR 0xF81F

			//	i51KitPngDrawImage(0xF81F, &pc_DstRect, &pc_SrcRect, (iHANDLE)pc_ImgFile, 0);
			//}


			{
				iU32 i=0;
				iU32 ImageWidth = 0;
				iU32 ImageHeight = 0;
				iU16 *DrawBuf = iNULL;
				iU32 ReadLength = 0;

				i51KitPngGetAttribute((iHANDLE)pc_ImgFile, 0, &ImageWidth, &ImageHeight);

				if ( iNULL != pc_DstFile ){
					
					if ( iFALSE == i51AdeOsFileSetSeek(pc_DstFile, 0, i51_OS_FL_SEEK_HEAD)){
						iLog("demo file setseek err");
						return iFALSE;
					}

					for (i=0; i<ImageHeight; i++){
						DrawBuf = ScreenBuffer + i*ScreenWidth;

						if ( iFALSE == i51AdeOsFileRead((iFILE)pc_DstFile, (void*)DrawBuf, sizeof(unsigned short)*ImageWidth, &ReadLength) ){
							iLog("demo read file err");
							return iFALSE;
						}
					}
				}
			}

			i51AdeMmiUpdateScreen();
#endif
		}break ;
	case i51_MSG_INIT :
		{
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

#if PNGCODEC_DEMO

			pc_ImgFile = i51AdeOsFileOpen(L"i51KitPng\\pic.png", i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ, __FILE__, __LINE__);
			if ( iNULL == pc_ImgFile ){
				iLog("open file fail");
				return iFALSE;
			}

			{
				pc_DstFile = i51AdeOsFileOpen(L"i51KitPng\\pic.data", i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_ALWAYS_OPEN|i51_OS_FL_WRITE, __FILE__, __LINE__);
				if ( iNULL == pc_DstFile ){
					iLog("open file fail");
					return iFALSE;
				}

				i51KitPngDecodeImage((iHANDLE)pc_ImgFile, 0, 0, (iHANDLE)pc_DstFile, 0);

				if ( iNULL != pc_DstFile)
				{
					i51AdeOsFileClose(pc_DstFile);
					pc_DstFile = iNULL;
				}

				pc_DstFile = i51AdeOsFileOpen(L"i51KitPng\\pic.data", i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ, __FILE__, __LINE__);
				if ( iNULL == pc_DstFile ){
					iLog("open file fail");
					return iFALSE;
				}
			}
#endif 

		}break ;
	case i51_MSG_RUN :
		break ;
	case i51_MSG_PAUSE :
		break ;

	case i51_MSG_EXIT :
		{
#if PNGCODEC_DEMO
			if ( iNULL  != pc_ImgFile ){
				i51AdeOsFileClose(pc_ImgFile);
				pc_ImgFile = 0;
			}

			if ( iNULL != pc_DstFile)
			{
				i51AdeOsFileClose(pc_DstFile);
				pc_DstFile = iNULL;
			}
#endif
		}break ;
	}

	return iTRUE ;
}

iKIT iU32* i51KitPort ()
{

	//	TODO : Initialize API Port for external layer

	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitPng)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitPngGetAttribute)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitPngDecodeImage)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitPngDrawImage)
	i51KIT_DYNAMIC_KIT_MAPING_END 

	return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitPng) ;
}

