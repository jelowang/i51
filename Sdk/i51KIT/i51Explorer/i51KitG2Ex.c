#include "i51KitAde.h"
#include "i51KitG2Ex.h"
//系统当前的Alpha值
static iALPHA AlphaSys = i51KITG2_ALPHA_NONEED;
// 原图的Alpha
static iALPHA AlphaSrc = i51KITG2_ALPHA_NONEED;
// 目的地的Alpha
static iALPHA AlphaDes = 0;
iCOLOR i51KitG2GetNewColor (iCOLOR dpixel, iCOLOR spixel);
static iS32 SinData[91] = 
{
	0, 285,  571,  857, 1142, 1427, 1712, 1996, 2280, 2563, 2845,
	3126, 3406, 3685, 3963, 4240, 4516, 4790, 5062, 5334, 5603,
	5871, 6137, 6401, 6663, 6924, 7182, 7438, 7691, 7943, 8191,
	8438, 8682, 8923, 9161, 9397, 9630, 9860, 10086, 10310, 10531,
	10748, 10963, 11173, 11381, 11585, 11785, 11982, 12175, 12365, 12550,
	12732, 12910, 13084, 13254, 13420, 13582, 13740, 13894, 14043, 14188,
	14329, 14466, 14598, 14725, 14848, 14967, 15081, 15190, 15295, 15395,
	15491, 15582, 15668, 15749, 15825, 15897, 15964, 16025, 16082, 16135,
	16182, 16224, 16261, 16294, 16321, 16344, 16361, 16374, 16381, 16384,
};

iCOLOR i51KitG2GetNewColorEx1 (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
	/**
	* @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
	* @brief	计算alpha值。alpha算法：dest <- (dest*(255 - alpha) + src * alpha) / 255
	* 
	* @param	dpixel	目的像素值
	* @param	spixel	源像素值
	* @param	alpha	alpha值
	* 
	* @return	返回alpha运算结果
	*/
	iU32 rd, gd, bd;
	iU32 rs, gs, bs;
	iALPHA DesAlpha = 0XFF - SrcAlpha;

	rs = (spixel) & 0XF800;
	gs = (spixel) & 0X07E0;
	bs = spixel & 0X1F;

	rd = (dpixel) & 0XF800;
	gd = (dpixel) & 0X07E0;
	bd = dpixel & 0X1F;

	rd = ((rd*DesAlpha+ rs * SrcAlpha)>>8)&0XF800;
	gd = ((gd*DesAlpha+ gs * SrcAlpha)>>8)&0X7E0;
	bd = ((bd*DesAlpha+ bs * SrcAlpha)>>8)&0X1F;
	return (bd +(gd)+ (rd ));

}
iU32 i51KitG2ImageCreate( iU32  SrcData)
{
	// author : Otto
	// since : 10.20 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iU32 SrcData : 需要解码的数据存放指针

	// notes : 解压一张图片到内内存中
	// SrcFile文件中存放的图片数据是分段用Zlib压缩的
	// 具体文件格式请参考文档"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

	// 段数量
	iU16 NumberParts = 0;
	// 当前段解压前数据长度
	iU32 LengthPartSrc = 0;
	// 当前段解压后数据长度
	iU32 LengthPartDes = 0;
	// 函数返回值
	iBOOL FlagReturn = iFALSE;
	// 解压后数据buffer
	iU8  * DesDataPoint = iNULL;
	iU32   DesDataLength = iNULL;
	iU32 SrcPos = 0;
	iU32 DesPos = 0;	
	iU32 iTemp = 0;
	iU32 DesTempPos = 0;

	// 前面四个字节是保存的图片宽度和高度	
	iU8 * SrcDataPoint = (iU8 * )(SrcData+4);

	if( SrcData==iNULL ) return iFALSE;

	//    i51AdeStdMemcpy16(&DesDataLength, &SrcDataPoint[4], 4);
	//    i51AdeStdMemcpy16(&NumberParts, &SrcDataPoint[16], 2);
	DesDataLength = SrcDataPoint[4]|(SrcDataPoint[5]<<8)|(SrcDataPoint[6]<<16)|(SrcDataPoint[7]<<24);
	NumberParts = SrcDataPoint[16]|(SrcDataPoint[17]<<8);
	////LOG("DesDataLength=%d",DesDataLength);
	////LOG("NumberParts=%d",NumberParts);
	DesDataPoint = ( iU8 *)i51AdeOsMalloc( DesDataLength,  __FILE__ , __LINE__ );
	if( iNULL == DesDataPoint ) return iFALSE;

	SrcPos = 18;
	DesPos = 0;
	for( iTemp=0; iTemp<NumberParts; iTemp++ )
	{
		//        i51AdeStdMemcpy16(&LengthPartSrc, &SrcDataPoint[SrcPos], 4);
		LengthPartSrc = SrcDataPoint[SrcPos+0]|(SrcDataPoint[SrcPos+1]<<8)|(SrcDataPoint[SrcPos+2]<<16)|(SrcDataPoint[SrcPos+3]<<24);
		////LOG("LengthPartSrc=%d",LengthPartSrc);
		SrcPos += 4;

		LengthPartDes = 32768;
		FlagReturn = i51AdeStdUncompress( &DesDataPoint[DesPos], &LengthPartDes, &SrcDataPoint[SrcPos], LengthPartSrc);
		if( iFALSE == FlagReturn ) goto ERROR;
		////LOG("LengthPartDes=%d",LengthPartDes);

		DesTempPos = DesPos;
		SrcPos += LengthPartSrc;
		DesPos += LengthPartDes;


		if(i51AdeCnfGetEndian()==1)
		{
			if(iTemp==0) DesTempPos = 0X14; 
			while(DesTempPos<DesPos)
			{
				DesDataPoint[DesTempPos] += DesDataPoint[DesTempPos+1];
				DesDataPoint[DesTempPos+1] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
				DesDataPoint[DesTempPos] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
				DesTempPos += 2;
			}
		}

	}

	return (iU32)(DesDataPoint);

ERROR:
	i51AdeOsFree(DesDataPoint);
	DesDataPoint = iNULL;
	return iFALSE;

}

iU32 i51KitG2GetImageAttrEx(iU32  SrcData, iU32 * ImageWidth, iU32 * ImageHeight)
{
	// 前面四个字节是保存的图片宽度和高度	
	iU8 * SrcDataPoint = (iU8 * )(SrcData);
	iU16 ImageW = 0;
	iU16 ImageH = 0;	

	if( SrcData==iNULL ) return iFALSE;

	i51AdeStdMemcpy16(&ImageW, &SrcDataPoint[0], 2);
	i51AdeStdMemcpy16(&ImageH, &SrcDataPoint[2], 2);

#ifndef __SPR__
	*ImageWidth = ImageW;   //小端
	*ImageHeight = ImageH;
#else
	*ImageWidth = ((ImageW&0xff00)>>8) + ((ImageW&0x00ff)<<8) ;//大端展讯
	*ImageHeight = ((ImageH&0xff00)>>8) + ((ImageH&0x00ff)<<8) ;
#endif
	return iTRUE;

}

iCOLOR i51KitG2GetNewColorEx (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
	/**
	* @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
	* @brief	计算alpha值。alpha算法：dest <- (dest*(255 - alpha) + src * alpha) / 255
	* 
	* @param	dpixel	目的像素值
	* @param	spixel	源像素值
	* @param	alpha	alpha值
	* 
	* @return	返回alpha运算结果
	*/
	iU32 rd, gd, bd;
	iU32 rs, gs, bs;
	iALPHA DesAlpha = 0XFF - SrcAlpha;

	rs = (spixel) & 0XF800;
	gs = (spixel) & 0X07E0;
	bs = spixel & 0X1F;

	rd = (dpixel) & 0XF800;
	gd = (dpixel) & 0X07E0;
	bd = dpixel & 0X1F;

	rd = ((rd*DesAlpha+ rs * SrcAlpha)>>8)&0XF800;
	gd = ((gd*DesAlpha+ gs * SrcAlpha)>>8)&0X7E0;
	bd = ((bd*DesAlpha+ bs * SrcAlpha)>>8)&0X1F;
	return (bd +(gd)+ (rd ));

}


iBOOL i51KitG2DrawImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
{
	iU8 * ImageDataPoint = iNULL;
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	iU16 * TempReadDataBuff = iNULL;
	iU32 CopyDataEntry = 0;
	// 累计读取到的数据长度
	iU32  LengthTotalReadData = 0;
	// 一次需要拷贝到缓冲区的颜色数量
	iS32 LengthCopyColor = 0;
	// 图片文件总长度
	iU32 LengthIamgeData = 0;

	// 当前数据段在屏幕上的开始绘制位置
	iS16 TempDrawX = 0;
	iS16 TempDrawY = 0;
	// 图片绘制的左上角和右下角坐标  
	iS16 SrcStartX = 0;
	iS16 SrcStartY = 0;
	iS16 SrcEndX = 0;
	iS16 SrcEndY = 0;
	// 屏幕绘制的左上角和右下角坐标  
	iS16 DesStartX = 0;
	iS16 DesStartY = 0;
	iS16 DesEndX = 0;
	iS16 DesEndY = 0;
	iS32 iTemp = 0;

	// ImageX
	iS32 CurrentImageX = 0;
	iS32 CurrentImageY = 0;
	iS32 CurrentImageWidth = 0;


	iU32 LcdBufferPostion = 0;

	// 读到的一段数据首地址
	iU16 * TempReadDataBuff1 = iNULL;

	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;
	iS16 SrcW, SrcH, DesW, DesH;
	iS16 DrawW, DrawH;
	iU16 ScreenWidth = 0;
	iU16 ScreenHeight= 0;
	iU16 CurrColor = 0;
	iU16 TotalAlphaNumber = 0;

	ImageDataPoint = (iU8 *)ImageData;
	if( iNULL==ImageDataPoint ) return iFALSE;

	if( ImageDataPoint[0X11]!=0X02 )
	{
		////LOG("文件格式错误，请重新生成图片文件!");
		return iFALSE;
	}


	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	LcdInfoArr =  i51AdeMmiGetPanel();
	if( iNULL == LcdInfoArr )
	{
		return iFALSE;
	}

	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	//    i51AdeStdMemcpy16(&ImageWidth, &ImageDataPoint[0X09], 2);
	//    i51AdeStdMemcpy16(&ImageHeight, &ImageDataPoint[0X0B], 2);
	//    i51AdeStdMemcpy16(&LengthIamgeData, &ImageDataPoint[0X05], 4);
	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);
	////LOG("ImageWidth=%d",ImageWidth);
	////LOG("ImageHeight=%d",ImageHeight);
	////LOG("LengthIamgeData=%d",LengthIamgeData);

	if(SrcRect)
	{
		SrcStartX = SrcRect->X;
		SrcStartY = SrcRect->Y;
		SrcW = SrcRect->Width;
		SrcH = SrcRect->Height;
	}
	else
	{
		SrcStartX = 0;
		SrcStartY = 0;
		SrcW = ImageWidth;
		SrcH = ImageHeight;
	}
	////LOG("SrcW=%d",SrcW);
	////LOG("SrcH=%d",SrcH);

	if( DesRect )
	{
		DesStartX= DesRect->X;
		DesStartY= DesRect->Y;
		DesW = DesRect->Width;
		DesH = DesRect->Height;
	}
	else
	{
		DesStartX= 0;
		DesStartY= 0;
		DesW = ScreenWidth;
		DesH = ScreenHeight;
	}

	if(SrcStartX<0)
	{
		SrcW += SrcStartX;
		DesW += SrcStartX;
		DesStartX -= SrcStartX;
		SrcStartX= 0;		
	}

	if(SrcStartY<0)
	{
		SrcH += SrcStartY;
		DesH += SrcStartY;
		DesStartY -= SrcStartY;
		SrcStartY= 0;		
	}

	if(DesStartX<0)
	{
		DesW += DesStartX;
		SrcW += DesStartX;
		SrcStartX -= DesStartX;
		DesStartX= 0;		
	}

	if(DesStartY<0)
	{
		DesH += DesStartY;
		SrcH += DesStartY;
		SrcStartY -= DesStartY;
		DesStartY= 0;		
	}

	if( SrcStartX+SrcW > ImageWidth) SrcW = ImageWidth - SrcStartX;
	if( SrcStartY+SrcH > ImageHeight) SrcH= ImageHeight- SrcStartY;

	if( DesStartX+DesW > ScreenWidth) DesW = ScreenWidth - DesStartX;
	if( DesStartY+DesH > ScreenHeight) DesH= ScreenHeight- DesStartY;

	if( SrcW<DesW ) DrawW = SrcW;
	else DrawW = DesW;

	if( SrcH<DesH ) DrawH = SrcH;
	else DrawH = DesH;

	ImageDataPoint += 0X14;
	LengthTotalReadData = 0X14;	
	SrcEndX = SrcStartX + DrawW;
	SrcEndY = SrcStartY + DrawH;
	DesEndX = DesStartX + DrawW;
	DesEndY = DesStartY + DrawH;

	if(SrcRect)
	{
		if( SrcStartX>=SrcRect->X + SrcRect->Width || SrcEndX<SrcRect->X )
		{
			return iTRUE;
		}
		if( SrcStartY>=SrcRect->Y + SrcRect->Height|| SrcEndY<SrcRect->Y )
		{
			return iTRUE;
		}
	}
	else
	{
		if( SrcStartX>=ImageWidth || SrcEndX<0 )
		{
			return iTRUE;
		}
		if( SrcStartY>=ImageHeight || SrcEndY<0 )
		{
			return iTRUE;
		}
	}

	////LOG("DesStartX=%d",DesStartX);
	////LOG("DesStartY=%d",DesStartY);
	////LOG("DesEndX=%d",DesEndX);
	////LOG("DesEndY=%d",DesEndY);

	while(1)
	{
		TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
		CurrentImageX = TempReadDataBuff1[0];
		CurrentImageY = TempReadDataBuff1[1];
		CurrentImageWidth = TempReadDataBuff1[2];
		//CurrentImageX = ImageDataPoint[1]|(ImageDataPoint[0]<<8);
		//CurrentImageY = ImageDataPoint[3]|(ImageDataPoint[2]<<8);
		//CurrentImageWidth = ImageDataPoint[5]|(ImageDataPoint[4]<<8);
		////LOG("CurrentImageX=%d",CurrentImageX);
		////LOG("CurrentImageY=%d",CurrentImageY);
		////LOG("CurrentImageWidth=%d",CurrentImageWidth);
		ImageDataPoint += 6;
		CopyDataEntry = (iU32)ImageDataPoint;
		LengthCopyColor =  CurrentImageWidth;

		LengthTotalReadData += 6 + (CurrentImageWidth<<1);
		ImageDataPoint += ( CurrentImageWidth<<1 );

		if( CurrentImageY>= SrcEndY) break;

		// 图片当前数据段X开始坐标在图片绘制区域右边或者Y坐标在图片绘制区域上方            
		if( (CurrentImageY < SrcStartY) || (CurrentImageX >= SrcEndX))
		{
			if( LengthTotalReadData> (LengthIamgeData-3) ) break;
			continue;
		}

		// 当前数据段长度超出图片绘制区域右边
		if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
		{
			LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
		}

		// 图片当前数据段X开始坐标在图片绘制区域左边            
		if( CurrentImageX<SrcStartX )
		{
			LengthCopyColor -= ((SrcStartX - CurrentImageX));
			CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
			if( LengthCopyColor<=0 ) 
			{
				if( LengthTotalReadData> (LengthIamgeData-3) ) break;
				continue;
			}
		}

		// 计算当前数据段在屏幕上开始显示的位置
		TempDrawX = DesStartX;
		TempDrawY = DesStartY + CurrentImageY - SrcStartY;

		// 图片当前数据段X开始坐标在图片开始绘制绘制区域右边
		// 需要移动屏幕开始绘制区域
		if( CurrentImageX>SrcStartX )
		{
			TempDrawX += ((CurrentImageX-SrcStartX));
		}

		if( TempDrawX >= DesEndX)
		{
			if( LengthTotalReadData> (LengthIamgeData-3) ) break;
			continue;
		}

		TempReadDataBuff = (iU16*)CopyDataEntry;
		i51AdeStdMemcpy16( &ScreenBuffer[ TempDrawY*ScreenWidth+TempDrawX], TempReadDataBuff, (LengthCopyColor<<1));
		if( LengthTotalReadData> (LengthIamgeData-3) ) break;

	}

	if(TotalAlphaNumber>0)
	{
		iS32 CurrAlphaNum = 0;
		iS32 CurrBuffPos = LengthIamgeData;		
		iS16 AlphaPosX = 0;
		iS16 AlphaPosY = 0;
		iALPHA AlphaNumber = 0;
		iS16 AlphaColor = 0;
		iS32 LcdBuffPos = 0;
		iU32 TempHeight = ImageHeight -1;
		iU32 TempWidth = ImageWidth-1;

		ImageDataPoint = (iU8*)((iU32)ImageData+CurrBuffPos + 12);

		for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
		{
			TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
			AlphaPosX = TempReadDataBuff1[0];
			AlphaPosY = TempReadDataBuff1[1];
			AlphaNumber = (iALPHA)TempReadDataBuff1[2];
			AlphaColor = TempReadDataBuff1[3];
			ImageDataPoint += 8;

			if(AlphaPosY>=SrcEndY) break;
			if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

			LcdBuffPos = (AlphaPosY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
			ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx1(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}		
	return iTRUE;

}





iBOOL i51KitG2ImageDestory( iU32  SrcData)
{
	// author : Otto
	// since : 10.20 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iU32 SrcData : 需要销毁的图片数据地址

	// notes : 销毁由i51KitG2ImageCreate函数创建的图片

	if( iNULL == SrcData ) return iFALSE;

	i51AdeOsFree((void *)SrcData);
	SrcData = 0;
	return iTRUE;

}

iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color)
{

	// author : Otto
	// since : Aug 10 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iU32 Point[][2] :
	//    Point[0][0]是需要绘制线段的起点X坐标
	//    Point[0][1]是需要绘制线段的起点Y坐标
	//    Point[1][0]是需要绘制线段的终点X坐标
	//    Point[1][1]是需要绘制线段的终点Y坐标
	// iCOLOR Color : 绘制线段的颜色

	// notes : 以颜色Color绘制一条从点(Point[0][0], Point[0][1])到点(Point[1][0], Point[1][1])的线段
	// 绘制算法使用的是对称直线生成算法，对称直线生成算法基于这样一个事实：直线以中点为界，其两边是对称的,
	// 因而可以利用这个对称性，对Bresenham算法进行改进，使得每进行一次判断就可以生成相对于直线中点的两个
	// 对称点。如此以来，直线就由两端向中间生成

	iS32 StartX = Point[0][0];
	iS32 StartY = Point[0][1];
	iS32 EndX = Point[1][0];
	iS32 EndY = Point[1][1];

	iS32 TempX1, TempY1;	
	iS32 TempX2, TempY2;	

	//起点与终点X和Y的差值
	iS32 dx = 0;
	iS32 dy = 0;
	iS32 dx2 = 0;
	iS32 dy2 = 0;

	// 决策量
	iS32 DecisionTag = 0;

	// 需要绘制次数
	iU32 HalfLength = 0;

	//当前绘制位置
	iU32 Location = 0;

	//校准Y值
	iS16 RegulationY = 1;

	// 当前绘制的元素在VP上的ID
	iU16 CurrDrawID;

	// 创建的VP实际大小	
	iRECT * VPActualRect = iNULL;
	// 创建的VP在屏幕上的左上和右下坐标
	iS32 VP_TOP_LEFT_X = 0;
	iS32 VP_TOP_LEFT_Y = 0;
	iS32 VP_LOVER_RIGHT_X = 0;
	iS32 VP_LOVER_RIGHT_Y = 0;
	// 需要绘制的VP区域	
	iRECT * VPDrawRect = iNULL;
	// 绘制区域	
	iS16 DrawRectStartX = 0;
	iS16 DrawRectStartY = 0;
	iU16 ScreenWidth;
	iU16 ScreenHeight;
	iS16 DrawRectEndX;
	iS16 DrawRectEndY;
	iU32 BuffPos = 0;
	iS32 Temp1 = 0;
	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;

#if i51KITG2_DEBUG
	//LOG( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	DrawRectEndX = ScreenWidth;
	DrawRectEndY = ScreenHeight;

	LcdInfoArr =  i51AdeMmiGetPanel();
	if( iNULL == LcdInfoArr )
	{
		return iFALSE;
	}

	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	for( Temp1=0; Temp1<2; Temp1++ )
	{
		if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
			||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
			)
		{
#if i51KITG2_DEBUG
			//LOG( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
			return iFALSE;
		}
	}     	

	DrawRectStartX = 0;
	DrawRectStartY = 0;
	DrawRectEndX = ScreenWidth;
	DrawRectEndY = ScreenHeight;
	StartX = Point[0][0];
	StartY = Point[0][1];
	EndX = Point[1][0];
	EndY = Point[1][1];

	//  如果需要绘制的线段的两端点都在屏幕外，且在屏幕同侧
	//  则绘制的线段是屏幕没有交集
	if( (StartX<0&&EndX<0)
		||(StartX>=ScreenWidth&&EndX>=ScreenWidth)
		||(StartY<0&&EndY<0)
		||(StartY>=ScreenHeight&&EndY>=ScreenHeight)
		)
	{
		return iTRUE;
	}

	//如果终点在起点左边则交换他们的位置
	if ( EndX < StartX )
	{
		EndX += StartX;
		StartX = EndX - StartX;
		EndX = EndX - StartX;

		EndY += StartY;
		StartY = EndY - StartY;
		EndY = EndY - StartY;
	}

	//计算起点与终点X和Y的差值
	dx = EndX - StartX;
	if( EndY > StartY )
	{
		dy = EndY - StartY;
		RegulationY = 1;
	}
	else
	{
		dy = StartY - EndY;
		RegulationY = -1;
	}

	//起点与终点X和Y的差值放大一倍
	dx2 = dx << 1;
	dy2 = dy << 1;

	//决策量赋初值
	if( dx>=dy){
		DecisionTag = dy2 - dx;
		HalfLength = (dx+1) >> 1;
	} else{
		DecisionTag = dy - dx2;
		HalfLength = (dy+1) >> 1;
	}

	for ( Location = 0; Location <= HalfLength; Location++)
	{
		TempX1 = StartX;
		TempY1 = StartY;
		TempX2 = EndX;
		TempY2 = EndY;

		if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
		{
#if LCDBUFF_DEBUG
			i51KitG2CheckLcdbuffer(ScreenWidth*TempY1 + TempX1,1);
#endif
			ScreenBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
		}

		if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
		{
#if LCDBUFF_DEBUG
			i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
#endif
			ScreenBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
		}

		if( dx>= dy ){
			// 当DecisionTag>0时，始端y向加RegulationY，末端y向减RegulationY
			if (DecisionTag > 0)
			{
				StartY += RegulationY;
				EndY -= RegulationY;    
				DecisionTag -= dx2;
			}

			//始端x向加1，末端x向减1
			StartX++;    
			EndX--;
			DecisionTag += dy2;
		}
		else
		{
			// 当DecisionTag<0时，始端X向加1，末端X向减1
			if (DecisionTag < 0)
			{
				StartX++;    
				EndX--;
				DecisionTag += dy2;
			}

			//始端y向加RegulationY，末端y向减RegulationY
			StartY += RegulationY;
			EndY -= RegulationY;          
			DecisionTag -= dx2;

		}
	}

#if i51KITG2_DEBUG
	//LOG( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

	return iTRUE;

}

iCOLOR i51KitG2GetNewColor (iCOLOR dpixel, iCOLOR spixel)
{
/**
 * @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
 * @brief	计算alpha值。alpha算法：dest <- (dest*(255 - alpha) + src * alpha) / 255
 * 
 * @param	dpixel	目的像素值
 * @param	spixel	源像素值
 * @param	alpha	alpha值
 * 
 * @return	返回alpha运算结果
 */
    iU32 rd, gd, bd;
    iU32 rs, gs, bs;
	
    rs = (spixel) & 0XF800;
    gs = (spixel) & 0X07E0;
    bs = spixel & 0X1F;
                                
    rd = (dpixel) & 0XF800;
    gd = (dpixel) & 0X07E0;
    bd = dpixel & 0X1F;
                                
    rd = ((rd*AlphaDes+ rs * AlphaSrc)>>8)&0XF800;
    gd = ((gd*AlphaDes+ gs * AlphaSrc)>>8)&0X7E0;
    bd = ((bd*AlphaDes+ bs * AlphaSrc)>>8)&0X1F;
    return (bd +(gd)+ (rd ));

}

iBOOL i51KitG2CleanScreenEx(iCOLOR Color, iRECT *Rect)
{
	// author : Otto
	// since : Sep 27 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iCOLOR Color : 填充屏幕的颜色
	//iRECT *Rect : 填充屏幕的区域

	// Notes : 以指定的颜色清屏幕区域

	iS32 CleanX, CleanY;
	iS32 CleanW, CleanH;
	iS32 LcdBufPos = 0;
	iS32 TempX, TempY;
	iU16 ScreenWidth;
	iU16 ScreenHeight;
	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;
	iU16 * OneRowColorBuf = iNULL;

#if i51KITG2_DEBUG
	//LOG( "G2CleanScreen Start , %x, %x", Color, Rect) ;
#endif

	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	LcdInfoArr =  i51AdeMmiGetPanel();
	if( iNULL == LcdInfoArr )
	{
		return iFALSE;
	}

	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	if( Rect)
	{
		CleanX = Rect->X;
		CleanY = Rect->Y;
		CleanW = Rect->Width;
		CleanH = Rect->Height;
	}
	else
	{
		CleanX = 0;
		CleanY = 0;
		CleanW = ScreenWidth;
		CleanH = ScreenHeight;
	}

	if(CleanX<0)
	{
		CleanW += CleanX;
		CleanX = 0;
	}

	if(CleanY<0)
	{
		CleanH += CleanH;
		CleanY = 0;
	}

	if( (CleanX+CleanW)>ScreenWidth) CleanW = ScreenWidth-CleanX;
	if( (CleanY+CleanH)>ScreenHeight) CleanH = ScreenHeight-CleanY;

	if( ScreenHeight>ScreenWidth)
	{
		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenHeight<<1),  __FILE__ , __LINE__ );
	}
	else
	{
		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenWidth<<1),  __FILE__ , __LINE__ );
	}

	for( TempX=0; TempX<CleanW ; TempX++)
	{
		OneRowColorBuf[TempX] = Color;
	}

	CleanW = (CleanW<<1);	
	LcdBufPos = CleanY*ScreenWidth + CleanX;
//	if(AlphaSys==i51KITG2_ALPHA_NONEED)
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			i51AdeStdMemcpy16(&(ScreenBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
			LcdBufPos += ScreenWidth;
		}
	}
#if 0
	else
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			LcdBufPos = (CleanY+TempY)*ScreenWidth + CleanX;
			for( TempX=0; TempX<CleanW ; TempX++)
			{
				ScreenBuffer[LcdBufPos] = i51KitG2GetNewColor(ScreenBuffer[LcdBufPos], Color);
				LcdBufPos++;
			}
		}
	}
#endif
	if(OneRowColorBuf)
	{
		i51AdeOsFree(OneRowColorBuf);
		OneRowColorBuf = iNULL;
	}

#if i51KITG2_DEBUG
	//LOG( "G2CleanScreen End , %x, %x", Color, Rect) ;
#endif
	return iTRUE;

}
iCOLOR NewColorFunc (iCOLOR spixel, iALPHA SrcAlpha)
{
	/**
	* @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
	* @brief	计算alpha值。alpha算法：dest <- (dest*(255 - alpha) + src * alpha) / 255
	* 
	* @param	dpixel	目的像素值
	* @param	spixel	源像素值
	* @param	alpha	alpha值
	* 
	* @return	返回alpha运算结果
	*/
	iU32 rd, gd, bd;
	iU32 rs, gs, bs;
	iALPHA DesAlpha = 0XFF - SrcAlpha;

	rs = (spixel) & 0XF800;
	gs = (spixel) & 0X07E0;
	bs = spixel & 0X1F;

	rd = ((rs * SrcAlpha)>>8)&0XF800;
	gd = ((gs * SrcAlpha)>>8)&0X7E0;
	bd = ((bs * SrcAlpha)>>8)&0X1F;


	return (bd +(gd)+ (rd ));

}
iBOOL i51KitG2CleanScreenBaseColorEx(iCOLOR Color, iRECT *Rect,iALPHA alpha)
{
	// author : Otto
	// since : Sep 27 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iCOLOR Color : 填充屏幕的颜色
	//iRECT *Rect : 填充屏幕的区域

	// Notes : 以指定的颜色清屏幕区域

	iS32 CleanX, CleanY;
	iS32 CleanW, CleanH;
	iS32 LcdBufPos = 0;
	iS32 TempX, TempY;
	iU16 ScreenWidth;
	iU16 ScreenHeight;
	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;
	iU16 * OneRowColorBuf = iNULL;
	iCOLOR TransedColor;
     
	TransedColor = NewColorFunc (Color, alpha);
#if i51KITG2_DEBUG
	//LOG( "G2CleanScreen Start , %x, %x", Color, Rect) ;
#endif

	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	LcdInfoArr =  i51AdeMmiGetPanel();
	if( iNULL == LcdInfoArr )
	{
		return iFALSE;
	}

	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	if( Rect)
	{
		CleanX = Rect->X;
		CleanY = Rect->Y;
		CleanW = Rect->Width;
		CleanH = Rect->Height;
	}
	else
	{
		CleanX = 0;
		CleanY = 0;
		CleanW = ScreenWidth;
		CleanH = ScreenHeight;
	}

	if(CleanX<0)
	{
		CleanW += CleanX;
		CleanX = 0;
	}

	if(CleanY<0)
	{
		CleanH += CleanH;
		CleanY = 0;
	}

	if( (CleanX+CleanW)>ScreenWidth) CleanW = ScreenWidth-CleanX;
	if( (CleanY+CleanH)>ScreenHeight) CleanH = ScreenHeight-CleanY;

	if( ScreenHeight>ScreenWidth)
	{
		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenHeight<<1),  __FILE__ , __LINE__ );
	}
	else
	{
		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenWidth<<1),  __FILE__ , __LINE__ );
	}

	for( TempX=0; TempX<CleanW ; TempX++)
	{
		//OneRowColorBuf[TempX] = Color;
		OneRowColorBuf[TempX] = TransedColor;
		//NewColorFunc
	}
///////////////

	//////////////
	CleanW = (CleanW<<1);	
	LcdBufPos = CleanY*ScreenWidth + CleanX;
	//	if(AlphaSys==i51KITG2_ALPHA_NONEED)
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			i51AdeStdMemcpy16(&(ScreenBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
			LcdBufPos += ScreenWidth;
		}
	}
#if 0
	else
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			LcdBufPos = (CleanY+TempY)*ScreenWidth + CleanX;
			for( TempX=0; TempX<CleanW ; TempX++)
			{
				ScreenBuffer[LcdBufPos] = i51KitG2GetNewColor(ScreenBuffer[LcdBufPos], Color);
				LcdBufPos++;
			}
		}
	}
#endif
	if(OneRowColorBuf)
	{
		i51AdeOsFree(OneRowColorBuf);
		OneRowColorBuf = iNULL;
	}

#if i51KITG2_DEBUG
	//LOG( "G2CleanScreen End , %x, %x", Color, Rect) ;
#endif
	return iTRUE;

}

//iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color)
//{
//
//	// author : Otto
//	// since : Aug 10 2011
//	// (C) 2011 , PKIG Tech. Co., Ltd.
//
//	// iU32 Point[][2] :
//	//    Point[0][0]是需要绘制线段的起点X坐标
//	//    Point[0][1]是需要绘制线段的起点Y坐标
//	//    Point[1][0]是需要绘制线段的终点X坐标
//	//    Point[1][1]是需要绘制线段的终点Y坐标
//	// iCOLOR Color : 绘制线段的颜色
//
//	// notes : 以颜色Color绘制一条从点(Point[0][0], Point[0][1])到点(Point[1][0], Point[1][1])的线段
//	// 绘制算法使用的是对称直线生成算法，对称直线生成算法基于这样一个事实：直线以中点为界，其两边是对称的,
//	// 因而可以利用这个对称性，对Bresenham算法进行改进，使得每进行一次判断就可以生成相对于直线中点的两个
//	// 对称点。如此以来，直线就由两端向中间生成
//
//	iS32 StartX = Point[0][0];
//	iS32 StartY = Point[0][1];
//	iS32 EndX = Point[1][0];
//	iS32 EndY = Point[1][1];
//
//	iS32 TempX1, TempY1;	
//	iS32 TempX2, TempY2;	
//
//	//起点与终点X和Y的差值
//	iS32 dx = 0;
//	iS32 dy = 0;
//	iS32 dx2 = 0;
//	iS32 dy2 = 0;
//
//	// 决策量
//	iS32 DecisionTag = 0;
//
//	// 需要绘制次数
//	iU32 HalfLength = 0;
//
//	//当前绘制位置
//	iU32 Location = 0;
//
//	//校准Y值
//	iS16 RegulationY = 1;
//
//	// 当前绘制的元素在VP上的ID
//	iU16 CurrDrawID;
//
//	// 创建的VP实际大小	
//	iRECT * VPActualRect = iNULL;
//	// 创建的VP在屏幕上的左上和右下坐标
//	iS32 VP_TOP_LEFT_X = 0;
//	iS32 VP_TOP_LEFT_Y = 0;
//	iS32 VP_LOVER_RIGHT_X = 0;
//	iS32 VP_LOVER_RIGHT_Y = 0;
//	// 需要绘制的VP区域	
//	iRECT * VPDrawRect = iNULL;
//	// 绘制区域	
//	iS16 DrawRectStartX = 0;
//	iS16 DrawRectStartY = 0;
//	iU16 ScreenWidth;
//	iU16 ScreenHeight;
//	iS16 DrawRectEndX;
//	iS16 DrawRectEndY;
//	iU32 BuffPos = 0;
//	iS32 Temp1 = 0;
//	iU16 * ScreenBuffer = iNULL;
//	iU32 * LcdInfoArr = iNULL;
//
//#if i51KITG2_DEBUG
//	//LOG( "G2DrawLine Start , %x, %x", Point, Color) ;
//#endif
//
//	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
//	{
//		return iFALSE;
//	}
//
//	DrawRectEndX = ScreenWidth;
//	DrawRectEndY = ScreenHeight;
//
//	LcdInfoArr =  i51AdeMmiGetPanel();
//	if( iNULL == LcdInfoArr )
//	{
//		return iFALSE;
//	}
//
//	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
//
//	for( Temp1=0; Temp1<2; Temp1++ )
//	{
//		if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
//			||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
//			)
//		{
//#if i51KITG2_DEBUG
//			//LOG( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
//#endif
//			return iFALSE;
//		}
//	}     	
//
//	DrawRectStartX = 0;
//	DrawRectStartY = 0;
//	DrawRectEndX = ScreenWidth;
//	DrawRectEndY = ScreenHeight;
//	StartX = Point[0][0];
//	StartY = Point[0][1];
//	EndX = Point[1][0];
//	EndY = Point[1][1];
//
//	//  如果需要绘制的线段的两端点都在屏幕外，且在屏幕同侧
//	//  则绘制的线段是屏幕没有交集
//	if( (StartX<0&&EndX<0)
//		||(StartX>=ScreenWidth&&EndX>=ScreenWidth)
//		||(StartY<0&&EndY<0)
//		||(StartY>=ScreenHeight&&EndY>=ScreenHeight)
//		)
//	{
//		return iTRUE;
//	}
//
//	//如果终点在起点左边则交换他们的位置
//	if ( EndX < StartX )
//	{
//		EndX += StartX;
//		StartX = EndX - StartX;
//		EndX = EndX - StartX;
//
//		EndY += StartY;
//		StartY = EndY - StartY;
//		EndY = EndY - StartY;
//	}
//
//	//计算起点与终点X和Y的差值
//	dx = EndX - StartX;
//	if( EndY > StartY )
//	{
//		dy = EndY - StartY;
//		RegulationY = 1;
//	}
//	else
//	{
//		dy = StartY - EndY;
//		RegulationY = -1;
//	}
//
//	//起点与终点X和Y的差值放大一倍
//	dx2 = dx << 1;
//	dy2 = dy << 1;
//
//	//决策量赋初值
//	if( dx>=dy){
//		DecisionTag = dy2 - dx;
//		HalfLength = (dx+1) >> 1;
//	} else{
//		DecisionTag = dy - dx2;
//		HalfLength = (dy+1) >> 1;
//	}
//
//	for ( Location = 0; Location <= HalfLength; Location++)
//	{
//		TempX1 = StartX;
//		TempY1 = StartY;
//		TempX2 = EndX;
//		TempY2 = EndY;
//
//		if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
//		{
//#if LCDBUFF_DEBUG
//			i51KitG2CheckLcdbuffer(ScreenWidth*TempY1 + TempX1,1);
//#endif
//			ScreenBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
//		}
//
//		if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
//		{
//#if LCDBUFF_DEBUG
//			i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
//#endif
//			ScreenBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
//		}
//
//		if( dx>= dy ){
//			// 当DecisionTag>0时，始端y向加RegulationY，末端y向减RegulationY
//			if (DecisionTag > 0)
//			{
//				StartY += RegulationY;
//				EndY -= RegulationY;    
//				DecisionTag -= dx2;
//			}
//
//			//始端x向加1，末端x向减1
//			StartX++;    
//			EndX--;
//			DecisionTag += dy2;
//		}
//		else
//		{
//			// 当DecisionTag<0时，始端X向加1，末端X向减1
//			if (DecisionTag < 0)
//			{
//				StartX++;    
//				EndX--;
//				DecisionTag += dy2;
//			}
//
//			//始端y向加RegulationY，末端y向减RegulationY
//			StartY += RegulationY;
//			EndY -= RegulationY;          
//			DecisionTag -= dx2;
//
//		}
//	}
//
//#if i51KITG2_DEBUG
//	//LOG( "G2DrawLine Start , %x, %x", Point, Color) ;
//#endif
//
//	return iTRUE;
//
//}
//iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color)
//{
//
//	// author : Otto
//	// since : Aug 10 2011
//	// (C) 2011 , PKIG Tech. Co., Ltd.
//
//	// iU32 Point[][2] :
//	//    Point[0][0]是需要绘制线段的起点X坐标
//	//    Point[0][1]是需要绘制线段的起点Y坐标
//	//    Point[1][0]是需要绘制线段的终点X坐标
//	//    Point[1][1]是需要绘制线段的终点Y坐标
//	// iCOLOR Color : 绘制线段的颜色
//
//	// notes : 以颜色Color绘制一条从点(Point[0][0], Point[0][1])到点(Point[1][0], Point[1][1])的线段
//	// 绘制算法使用的是对称直线生成算法，对称直线生成算法基于这样一个事实：直线以中点为界，其两边是对称的,
//	// 因而可以利用这个对称性，对Bresenham算法进行改进，使得每进行一次判断就可以生成相对于直线中点的两个
//	// 对称点。如此以来，直线就由两端向中间生成
//
//	iS32 StartX = Point[0][0];
//	iS32 StartY = Point[0][1];
//	iS32 EndX = Point[1][0];
//	iS32 EndY = Point[1][1];
//
//	iS32 TempX1, TempY1;	
//	iS32 TempX2, TempY2;	
//
//	//起点与终点X和Y的差值
//	iS32 dx = 0;
//	iS32 dy = 0;
//	iS32 dx2 = 0;
//	iS32 dy2 = 0;
//
//	// 决策量
//	iS32 DecisionTag = 0;
//
//	// 需要绘制次数
//	iU32 HalfLength = 0;
//
//	//当前绘制位置
//	iU32 Location = 0;
//
//	//校准Y值
//	iS16 RegulationY = 1;
//
//	// 当前绘制的元素在VP上的ID
//	iU16 CurrDrawID;
//
//	// 创建的VP实际大小	
//	iRECT * VPActualRect = iNULL;
//	// 创建的VP在屏幕上的左上和右下坐标
//	iS32 VP_TOP_LEFT_X = 0;
//	iS32 VP_TOP_LEFT_Y = 0;
//	iS32 VP_LOVER_RIGHT_X = 0;
//	iS32 VP_LOVER_RIGHT_Y = 0;
//	// 需要绘制的VP区域	
//	iRECT * VPDrawRect = iNULL;
//	// 绘制区域	
//	iS16 DrawRectStartX = 0;
//	iS16 DrawRectStartY = 0;
//	iU16 ScreenWidth;
//	iU16 ScreenHeight;
//	iS16 DrawRectEndX;
//	iS16 DrawRectEndY;
//	iU32 BuffPos = 0;
//	iS32 Temp1 = 0;
//	iU16 * ScreenBuffer = iNULL;
//	iU32 * LcdInfoArr = iNULL;
//
//#if i51KITG2_DEBUG
//	iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
//#endif
//
//	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
//	{
//		return iFALSE;
//	}
//
//	DrawRectEndX = ScreenWidth;
//	DrawRectEndY = ScreenHeight;
//
//	LcdInfoArr =  i51AdeMmiGetPanel();
//	if( iNULL == LcdInfoArr )
//	{
//		return iFALSE;
//	}
//
//	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
//
//	for( Temp1=0; Temp1<2; Temp1++ )
//	{
//		if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
//			||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
//			)
//		{
//#if i51KITG2_DEBUG
//			iLog( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
//#endif
//			return iFALSE;
//		}
//	}     	
//
//	DrawRectStartX = 0;
//	DrawRectStartY = 0;
//	DrawRectEndX = ScreenWidth;
//	DrawRectEndY = ScreenHeight;
//	StartX = Point[0][0];
//	StartY = Point[0][1];
//	EndX = Point[1][0];
//	EndY = Point[1][1];
//
//	//  如果需要绘制的线段的两端点都在屏幕外，且在屏幕同侧
//	//  则绘制的线段是屏幕没有交集
//	if( (StartX<0&&EndX<0)
//		||(StartX>=ScreenWidth&&EndX>=ScreenWidth)
//		||(StartY<0&&EndY<0)
//		||(StartY>=ScreenHeight&&EndY>=ScreenHeight)
//		)
//	{
//		return iTRUE;
//	}
//
//	//如果终点在起点左边则交换他们的位置
//	if ( EndX < StartX )
//	{
//		EndX += StartX;
//		StartX = EndX - StartX;
//		EndX = EndX - StartX;
//
//		EndY += StartY;
//		StartY = EndY - StartY;
//		EndY = EndY - StartY;
//	}
//
//	//计算起点与终点X和Y的差值
//	dx = EndX - StartX;
//	if( EndY > StartY )
//	{
//		dy = EndY - StartY;
//		RegulationY = 1;
//	}
//	else
//	{
//		dy = StartY - EndY;
//		RegulationY = -1;
//	}
//
//	//起点与终点X和Y的差值放大一倍
//	dx2 = dx << 1;
//	dy2 = dy << 1;
//
//	//决策量赋初值
//	if( dx>=dy){
//		DecisionTag = dy2 - dx;
//		HalfLength = (dx+1) >> 1;
//	} else{
//		DecisionTag = dy - dx2;
//		HalfLength = (dy+1) >> 1;
//	}
//
//	for ( Location = 0; Location <= HalfLength; Location++)
//	{
//		TempX1 = StartX;
//		TempY1 = StartY;
//		TempX2 = EndX;
//		TempY2 = EndY;
//
//		if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
//		{
//#if LCDBUFF_DEBUG
//			i51KitG2CheckLcdbuffer(ScreenWidth*TempY1 + TempX1,1);
//#endif
//			ScreenBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
//		}
//
//		if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
//		{
//#if LCDBUFF_DEBUG
//			i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
//#endif
//			ScreenBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
//		}
//
//		if( dx>= dy ){
//			// 当DecisionTag>0时，始端y向加RegulationY，末端y向减RegulationY
//			if (DecisionTag > 0)
//			{
//				StartY += RegulationY;
//				EndY -= RegulationY;    
//				DecisionTag -= dx2;
//			}
//
//			//始端x向加1，末端x向减1
//			StartX++;    
//			EndX--;
//			DecisionTag += dy2;
//		}
//		else
//		{
//			// 当DecisionTag<0时，始端X向加1，末端X向减1
//			if (DecisionTag < 0)
//			{
//				StartX++;    
//				EndX--;
//				DecisionTag += dy2;
//			}
//
//			//始端y向加RegulationY，末端y向减RegulationY
//			StartY += RegulationY;
//			EndY -= RegulationY;          
//			DecisionTag -= dx2;
//
//		}
//	}
//
//#if i51KITG2_DEBUG
//	iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
//#endif
//
//	return iTRUE;
//
//}