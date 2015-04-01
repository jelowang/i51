#ifndef __PNG_H__
#define __PNG_H__

#include "i51.h"

//  author : yepan
//  since  : 2012-7-20
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			ImageHandle 数据源指针
//			HandleType	0==数据源是文件句柄,1==数据源是内存指针
//			ImageWidth	输出的图片宽度
//			ImageHeight 输出的图片高度
//
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			获取图片信息
iBOOL png_get_attribute(iHANDLE ImageHandle, iU8 HandleType,  iU32 * ImageWidth, iU32 * ImageHeight);

//  author : yepan
//  since  : 2012-7-20
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			type				0==解码， 1==解码并绘制
//			ImageHandle			数据源指针
//			ImageHandleType		0==数据源是文件句柄,1==数据源是内存指针
//			SrcRect				数据源区域
//			DesHandle			目的对象指针
//			DesHandleType		0==目的对象是文件句柄,1==目的对象是内存指针
//			DesRect				目标区域
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			解码png图片。
iBOOL png_decode(iS32 type, iU32 TransColor, iHANDLE ImageHandle, iU8 ImageHandleType, iU32 ImageFileSize, iRECT* SrcRect, iHANDLE DesHandle, iU8 DesHandleType, iRECT* DesRect);

#endif