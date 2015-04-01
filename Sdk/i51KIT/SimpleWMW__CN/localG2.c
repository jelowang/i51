#include "localG2.h"
#include "i51KitAde.h"

iU8 LocalG2AlphaSys = 0XFF;

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
    iU32   SrcDataLength = iNULL;
    iU32 SrcPos = 0;
    iU32 DesPos = 0;	
    iU32 iTemp = 0;
    iU32 DesTempPos = 0;
    // 图片带调色板
    iU8 PaletteImage = iFALSE;
    // 调色板中颜色总数
    iU16 PaletteColorNum = 0;
    // 调色板颜色索引表
    iCOLOR * PaletteColorMem = iNULL;
    // 一段数据解码的临时存放处
    iU8  * TempDesData = iNULL;
    // 一个数据块的长度	:=6+连续点个数
    iS32 BlockLength = 0;
    // 当前数据块位置
    iS32 BlockPlace = 0;
    // 颜色临时变量	
    iCOLOR TempColor = 0;
    // 颜色指针地址
    iCOLOR * ColorPoint = iNULL;
    // 段数据头	
    iU8 TempHead[6];
    iS32 Temp = 0;		
    // 前面四个字节是保存的图片宽度和高度	
    iU8 * SrcDataPoint = iNULL;// = (iU8 * )(SrcData+4);
    iU16 ImageW = 0;
    iU16 ImageH = 0;	


    if( SrcData==iNULL ) return iFALSE;
	
    SrcDataPoint = (iU8 * )(SrcData);
    ImageW = 	SrcDataPoint[0]|(SrcDataPoint[1]<<8);
    ImageH = 	SrcDataPoint[2]|(SrcDataPoint[3]<<8);
	
    // 前面四个字节是保存的图片宽度和高度	
    SrcDataPoint = (iU8 * )(SrcData+4);
    DesDataLength = SrcDataPoint[4]|(SrcDataPoint[5]<<8)|(SrcDataPoint[6]<<16)|(SrcDataPoint[7]<<24);
    SrcDataLength = SrcDataPoint[8]|(SrcDataPoint[9]<<8)|(SrcDataPoint[10]<<16)|(SrcDataPoint[11]<<24);
    NumberParts = SrcDataPoint[16]|(SrcDataPoint[17]<<8);

    DesDataPoint = ( iU8 *)i51AdeOsMalloc( DesDataLength,  __FILE__ , __LINE__ );
    if( iNULL == DesDataPoint )
    {
        iLog("KIT - G2 : image create malloc des");
        goto ERROR;
    }

    if( NumberParts==1 && DesDataLength+514<32768 )
    {
        TempDesData = ( iU8 *)i51AdeOsMalloc( DesDataLength+514,  __FILE__ , __LINE__ );
    }
    else
    {
        TempDesData = ( iU8 *)i51AdeOsMalloc( 32768,  __FILE__ , __LINE__ );
    }
    if( iNULL == TempDesData )
    {
        iLog("KIT - G2 : image create malloc temp");
        goto ERROR;
    }

    SrcPos = 18;
    DesPos = 0;
	
    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        LengthPartSrc = SrcDataPoint[SrcPos]|(SrcDataPoint[SrcPos+1]<<8)|(SrcDataPoint[SrcPos+2]<<16)|(SrcDataPoint[SrcPos+3]<<24);
        SrcPos += 4;
		
        LengthPartDes = 32768;

        FlagReturn = i51AdeStdUncompress( TempDesData, &LengthPartDes, &SrcDataPoint[SrcPos], LengthPartSrc);
        if( iFALSE == FlagReturn ) goto ERROR;

        if( iTemp==0 )
        {
            if( TempDesData[4]>0X40)
            {
                PaletteImage = iTRUE;
                PaletteColorNum = (TempDesData[20]) | (TempDesData[21]<<8); 
                PaletteColorMem = i51AdeOsMalloc(PaletteColorNum*2, __FILE__ , __LINE__ );
                if ( iNULL==PaletteColorMem )
                {
                    iLog("KIT - G2 : mage create malloc PaletteColor" );
                    goto ERROR;
                }
                i51AdeStdMemcpy16(PaletteColorMem, &(TempDesData[22]), PaletteColorNum*2 );				
            }
        }
		
        SrcPos += LengthPartSrc;
        if( iFALSE==PaletteImage )
        {
//            DesTempPos = DesPos;
            i51AdeStdMemcpy16(&(DesDataPoint[DesPos]), TempDesData, LengthPartDes);				
            DesPos += LengthPartDes;
        }
        else
        {
            //带索引表的图片需要先把索引转为颜色
            if( iTemp==0 )
            {
                i51AdeStdMemcpy16(DesDataPoint,TempDesData, 20);				
                DesTempPos = 20 + PaletteColorNum*2 +2;
                DesPos = 20;
                BlockLength = 0;
                BlockPlace = 0;
            }
            else
            {
                DesTempPos = 0;
            }

            while(DesTempPos<LengthPartDes)
            {
                if( BlockPlace==0 )
                {
                    // 块重新开始
                    if( (LengthPartDes-DesTempPos)>=6 )
                    {
                        // 段剩余数据长度大于等于块头长度
                        BlockPlace = 6;
                        BlockLength = TempDesData[DesTempPos+4] | (TempDesData[DesTempPos+5]<<8);
                        i51AdeStdMemcpy16(&DesDataPoint[DesPos], &TempDesData[DesTempPos], 6);
                        i51AdeStdMemcpy16(TempHead, &TempDesData[DesTempPos], 6);
                        DesTempPos += 6;
                        DesPos += 6;
                    }
                    else
                    {
                        // 段剩余数据小于块头长度，把剩余数据保存
                        BlockPlace = LengthPartDes-DesTempPos;
                        for( Temp = 0; Temp < BlockPlace; Temp++ )
                        {
                            TempHead[Temp] = TempDesData[DesTempPos+Temp];
                        }
                        DesTempPos = 0;						
                        break; 						
                    }
                }
                else if( BlockPlace<6 )				
                {
                    for( Temp = BlockPlace; Temp < 6; Temp++ )
                    {
                        TempHead[Temp] = TempDesData[DesTempPos+Temp-BlockPlace];
                    }
                    //i51AdeStdMemcpy16(&TempDesData[DesPos], TempHead, 6);
                    i51AdeStdMemcpy16(&DesDataPoint[DesPos], TempHead, 6);
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    DesTempPos += 6-BlockPlace;
                    DesPos += 6;
                    BlockPlace = 6;
                }
                else
                {
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    BlockLength -= BlockPlace - 6;
                }

                ColorPoint = (iCOLOR*)(((iU32)DesDataPoint)+DesPos);

                if( DesTempPos+BlockLength > LengthPartDes)
                {
                    BlockLength = LengthPartDes - DesTempPos;
                    BlockPlace = 6 + BlockLength;
                }
                else				
                {
                    BlockPlace = 0;
                }
				
                for( Temp=0; Temp<BlockLength; Temp++ )
                {
                    ColorPoint[Temp] = PaletteColorMem[TempDesData[DesTempPos]];
                    DesTempPos++;
                    DesPos += 2;
                }
            	}
     }

		
    }

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
 	
    if(TempDesData)
    {
        i51AdeOsFree(TempDesData);
        TempDesData = iNULL;
    }
	
    if(i51AdeCnfGetEndian()==1)
    {
        DesTempPos = 0X14; 
        while(DesTempPos<DesPos)
        {
            DesDataPoint[DesTempPos] += DesDataPoint[DesTempPos+1];
            DesDataPoint[DesTempPos+1] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
            DesDataPoint[DesTempPos] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
            DesTempPos += 2;
        }
    }

    return (iU32)(DesDataPoint);

 ERROR:

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
 	
    if(TempDesData)
    {
        i51AdeOsFree(TempDesData);
        TempDesData = iNULL;
    }

    if(DesDataPoint)
    {
        i51AdeOsFree(DesDataPoint);
        DesDataPoint = iNULL;
    }

    iLog("KIT - G2 : create image ");
	
    return iFALSE;
	
}


iU32 i51KitG2GetImageAttrEx(iU32  SrcData, iU32 * ImageWidth, iU32 * ImageHeight)
{
	// 前面四个字节是保存的图片宽度和高度	
	iU8 * SrcDataPoint = (iU8 * )(SrcData);
	iU16 ImageW = 0;
	iU16 ImageH = 0;	

	if( SrcData==iNULL ) return iFALSE;

#if 0
	i51AdeStdMemcpy16(&ImageW, &SrcDataPoint[0], 2);
	i51AdeStdMemcpy16(&ImageH, &SrcDataPoint[2], 2);

#ifndef __SPR__
	*ImageWidth = ImageW;   //小端
	*ImageHeight = ImageH;
#else
	*ImageWidth = ((ImageW&0xff00)>>8) + ((ImageW&0x00ff)<<8) ;//大端展讯
	*ImageHeight = ((ImageH&0xff00)>>8) + ((ImageH&0x00ff)<<8) ;
#endif
#else
	*ImageWidth = (SrcDataPoint[0]) + (SrcDataPoint[1]<<8) ;
	*ImageHeight = (SrcDataPoint[2]) + (SrcDataPoint[3]<<8) ;
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
	iS32 Temp = 0;
	iU32 TempScreenBufferPos = 0;

	ImageDataPoint = (iU8 *)ImageData;
	if( iNULL==ImageDataPoint ) return iFALSE;

	if( ImageDataPoint[0X11]!=0X02 )
	{
		iLog("文件格式错误，请重新生成图片文件!");
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
	//iLog("ImageWidth=%d",ImageWidth);
	//iLog("ImageHeight=%d",ImageHeight);
	//iLog("LengthIamgeData=%d",LengthIamgeData);

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
	//iLog("SrcW=%d",SrcW);
	//iLog("SrcH=%d",SrcH);

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

	if(LocalG2AlphaSys==0XFF)
	{
		while(1)
		{
			TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
			CurrentImageX = TempReadDataBuff1[0];
			CurrentImageY = TempReadDataBuff1[1];
			CurrentImageWidth = TempReadDataBuff1[2];
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

				LcdBuffPos = (AlphaPosY-SrcStartY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
				ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx1(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
			}
		}	
	}
	else
	{
		while(1)
		{
			TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
			CurrentImageX = TempReadDataBuff1[0];
			CurrentImageY = TempReadDataBuff1[1];
			CurrentImageWidth = TempReadDataBuff1[2];
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
			TempScreenBufferPos = TempDrawY*ScreenWidth+TempDrawX;
			for( Temp = 0; Temp<LengthCopyColor; Temp++ )	
			{
				ScreenBuffer[TempScreenBufferPos] = i51KitG2GetNewColorEx1(ScreenBuffer[TempScreenBufferPos], TempReadDataBuff[Temp], LocalG2AlphaSys);
				TempScreenBufferPos++;
			}
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

				LcdBuffPos = (AlphaPosY-SrcStartY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
				ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx1(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber*LocalG2AlphaSys/255);
			}
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


iBOOL i51KitG2MirrorImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
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
		iLog("文件格式错误，请重新生成图片文件!");
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

	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

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
		DesStartX -= SrcStartX;
		SrcStartX= 0;		
	}

	if(SrcStartY<0)
	{
		SrcH += SrcStartY;
		DesStartY -= SrcStartY;
		SrcStartY= 0;		
	}

	if(DesStartX<0)
	{
		DesW += DesStartX;
		SrcStartX -= DesStartX;
		DesStartX= 0;		
	}

	if(DesStartY<0)
	{
		DesH += DesStartY;
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

	SrcStartX = ImageWidth - SrcStartX - DrawW;
	SrcEndX = SrcStartX + DrawW;

	while(1)
	{
		TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
		CurrentImageX = TempReadDataBuff1[0];
		CurrentImageY = TempReadDataBuff1[1];
		CurrentImageWidth = TempReadDataBuff1[2];

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

		CurrentImageX = ImageWidth - CurrentImageX - CurrentImageWidth;

		// 当前数据段长度超出图片绘制区域右边
		if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
		{
			LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
		}

		// 图片当前数据段X开始坐标在图片绘制区域左边            
		if( CurrentImageX<SrcStartX )
		{
			LengthCopyColor -= ((SrcStartX - CurrentImageX));
			//            CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
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
		LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX + LengthCopyColor-1;
		for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
		{
			ScreenBuffer[LcdBufferPostion--] = TempReadDataBuff[iTemp];
		}

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

			AlphaPosX = TempWidth - AlphaPosX ;

			if(AlphaPosY>=SrcEndY) break;
			if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

			LcdBuffPos = (AlphaPosY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
			ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}		
	return iTRUE;

}


iBOOL i51KitG2FlipImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
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
		iLog("文件格式错误，请重新生成图片文件!");
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

	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

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
		DesStartX -= SrcStartX;
		SrcStartX= 0;		
	}

	if(SrcStartY<0)
	{
		SrcH += SrcStartY;
		DesStartY -= SrcStartY;
		SrcStartY= 0;		
	}

	if(DesStartX<0)
	{
		DesW += DesStartX;
		SrcStartX -= DesStartX;
		DesStartX= 0;		
	}

	if(DesStartY<0)
	{
		DesH += DesStartY;
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

	SrcStartY = ImageHeight - SrcStartY - DrawH;
	SrcEndY = SrcStartY+ DrawH;

	while(1)
	{
		TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
		CurrentImageX = TempReadDataBuff1[0];
		CurrentImageY = TempReadDataBuff1[1];
		CurrentImageWidth = TempReadDataBuff1[2];

		ImageDataPoint += 6;
		CopyDataEntry = (iU32)ImageDataPoint;
		LengthCopyColor =  CurrentImageWidth;

		LengthTotalReadData += 6 + (CurrentImageWidth<<1);
		ImageDataPoint += ( CurrentImageWidth<<1 );

		CurrentImageY = ImageHeight -1 - CurrentImageY ;

		// 图片当前数据段X开始坐标在图片绘制区域右边或者Y坐标在图片绘制区域上方            
		if( (CurrentImageY < SrcStartY) || (CurrentImageY >= SrcEndY) || (CurrentImageX >= SrcEndX))
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

		if( TempDrawY > DesEndY ) break;
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
			AlphaPosY = TempHeight - AlphaPosY ;

			LcdBuffPos = (AlphaPosY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
			ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}		
	return iTRUE;

}

iS32 i51KitG2SinEx( iS32 Angle)
{
	//iS32 Angle: 需要计算的角度值
	// notes : 返回角度Angle的正玄放大16384倍(相当于左移14位)的值
	iS32 ActualAngle = 0;
	iS32 Ret = 0;

	ActualAngle = Angle%360;
	if( ActualAngle<0) ActualAngle = 360+ActualAngle;

	if(ActualAngle<=90) Ret = SinData[ActualAngle];
	else if ( ActualAngle<=180) Ret = SinData[180-ActualAngle];
	else if ( ActualAngle<=270) Ret = -SinData[ActualAngle-180];
	else Ret = -SinData[360-ActualAngle];

	return Ret;

}

iS32 i51KitG2CosEx( iS32 Angle)
{
	//iS32 Angle: 需要计算的角度值
	// notes : 返回角度Angle的正玄放大10000倍的值
	return i51KitG2SinEx(Angle+90);
}

iBOOL i51KitG2RotatePointEx(iS32 SrcX, iS32 SrcY, iS32 RotateCenterX,  iS32 RotateCenterY ,iS32 * DesX, iS32 * DesY, iS32 Angle)
{
	// A:原始点与原点之间相对X轴正方向的夹角
	// B:原始点和目的点与原点之间的夹角,即Angle
	// C:目的点与原点之间相对X轴正方向的夹角

	iS32 SinA = 0, SinB = 0, SinC = 0;
	iS32 CosA = 0, CosB = 0, CosC = 0;
	iS32 CenterX = RotateCenterX;
	iS32 CenterY = RotateCenterY;

	CosA = SrcX - CenterX;
	SinA = SrcY-CenterY;

	Angle = -Angle;
	SinB = i51KitG2SinEx(Angle);
	CosB = i51KitG2CosEx(Angle);

	SinC = SinA*CosB - CosA*SinB;
	CosC = CosA*CosB + SinA*SinB;

	*DesY = ((SinC+(CenterY<<14))>>14);
	*DesX = ((CosC+(CenterX<<14))>>14);

	//    *DesX = (((SrcX-CenterX)*CosB+(SrcY-CenterY)*SinB)>>14) + CenterX;
	//    *DesY = (((CenterX-SrcX)*SinB+(SrcY-CenterY)*CosB)>>14) + CenterY;
	return iTRUE;

}

iBOOL i51KitG2GetGirthPointsEx(iRECT SrcRect, iRECT DesRect, iS16 PointS[][2], iS32 SrcMoveX, iS32 SrcMoveY,
									  iS32 RotateCenterX,  iS32 RotateCenterY ,iU32 RotateType, iS32 Angle )
{
	// iRECT SrcRect : 需要变换的区域
	// iS16 PointS[][2] : 变换后周边的点新坐标
	// iS32 Angle :旋转角度
#define IMAGE_STRETCH 0
	// 旋转区域左上角坐标
	iS32 UpLeftX = 0, UpLeftY = 0;
	// 当前旋转点坐标	 
	iS32 SrcX= 0, SrcY = 0;
	iS32 SrcX1= 0, SrcY1 = 0;
	// 当前点旋转后坐标
	iS32 DesX = 0, DesY = 0;
	// 待旋转区域的长宽	
	iS32 SrcW, SrcH;
	iS32 Location = 0;
	iS32 EdgePoints[4][2] = {0};
	iS32 DesStartX = 0, DesStartY = 0;
	iS32 DesEndX = 0, DesEndY = 0;
#if IMAGE_STRETCH
	iS32 DesPoints[5][2] = {0};
	iS32 OldX = 0, OldY = 0;
	iS32 iTemp = 0, Min = 0, Num = 0 , Temp = 0;
#endif

	//    UpLeftX = SrcRect.X + SrcMoveX;
	//    UpLeftY = SrcRect.Y + SrcMoveY;
	UpLeftX = SrcMoveX;
	UpLeftY = SrcMoveY;
	SrcW = SrcRect.Width;
	SrcH = SrcRect.Height;

	DesStartX = DesRect.X;
	DesStartY = DesRect.Y;
	DesEndX = DesRect.Width + DesStartX;
	DesEndY = DesRect.Height + DesStartY;

	i51KitG2RotatePointEx(UpLeftX, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[0][0], &EdgePoints[0][1], Angle);
	i51KitG2RotatePointEx(UpLeftX+SrcW, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[1][0], &EdgePoints[1][1], Angle);
	i51KitG2RotatePointEx(UpLeftX, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[2][0], &EdgePoints[2][1], Angle);
	i51KitG2RotatePointEx(UpLeftX+SrcW, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[3][0], &EdgePoints[3][1], Angle);

	if( EdgePoints[0][0]<DesStartX&&EdgePoints[1][0]<DesStartX&&EdgePoints[2][0]<DesStartX&&EdgePoints[3][0]<DesStartX) return iFALSE;
	if( EdgePoints[0][1]<DesStartY&&EdgePoints[1][1]<DesStartY&&EdgePoints[2][1]<DesStartY&&EdgePoints[3][1]<DesStartY) return iFALSE;
	if( EdgePoints[0][0]>DesEndX&&EdgePoints[1][0]>DesEndX&&EdgePoints[2][0]>DesEndX&&EdgePoints[3][0]>DesEndX) return iFALSE;
	if( EdgePoints[0][1]>DesEndY&&EdgePoints[1][1]>DesEndY&&EdgePoints[2][1]>DesEndY&&EdgePoints[3][1]>DesEndY) return iFALSE;

	SrcX = UpLeftX;
	SrcY = UpLeftY;

	for ( Location = 0; Location <SrcW; Location++)
	{
		i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX, RotateCenterY, &DesX, &DesY, Angle);
		PointS[Location][0] = DesX-SrcX;
		PointS[Location][1] = DesY-SrcY;
		SrcX++;
	}	

	SrcH = SrcW + SrcH;
	SrcX = UpLeftX;
	SrcY = UpLeftY;
	i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX,  RotateCenterY , &DesX, &DesY, Angle);
	SrcX1 = DesX;
	SrcY1 = DesY;

	for ( Location = SrcW; Location <SrcH; Location++)
	{
		i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX, RotateCenterY,&DesX, &DesY, Angle);
		PointS[Location][0] = DesX-SrcX1;
		PointS[Location][1] = DesY-SrcY1 ;
		SrcY++;
		SrcY1++;		
	}	

	for ( Location = 0; Location <SrcW; Location++)
	{
		PointS[Location][0] += SrcMoveX;
		PointS[Location][1] += SrcMoveY;
	}	

	return iTRUE;

}


iBOOL RotateRamLinearImageEx  ( iU32 ImageData , iRECT* DesRect , iRECT* SrcRect , iS16 ChangeData[][2])
{
	// author : Otto
	// since : Aug 15 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iU32 ImageID : 绘制图像的ID号
	// iRECT* DesRect : 绘制图像到屏幕上的区域
	// iRECT* SrcRect : 绘制图像的区域
	// iS32 Angle : 旋转角度

	// notes : 将图像容器内ID号为ImageID的图片SrcRect区域
	// 按照Angle的角度旋转后绘制到屏幕的DesRect区域
	// 图片的数据在内存中有备份，需要的数据从内存中读取
	// 这张图片的编码方式为I51KITG2_DECODE_LINEAR     

	// 图片宽度和高度
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	// 图片数据文件开始地址
	iU32 DataEntry = 0;
	// 读取到的数据长度
	//    iU32  LengthReadData = 0;
	// 累计读取到的数据长度
	iU32  LengthTotalReadData = 0;
	// 需要读取的数据长度
	//    iU32  LengthNeedReadData = 0;
	// 一次需要拷贝到显存的数据长度  
	iS32 LengthCopyData = 0;
	// 图片文件总长度
	iU32 LengthIamgeData = 0;
	//    iU32 LcdBufferPostion = 0;

	//    iU16 iTempX = 0;
	//    iU16 iTempY = 0;
	// 图片绘制的左上角和右下角坐标  
	iS16 SrcStartX = 0;
	iS16 SrcStartY = 0;
	iS16 SrcEndX = 0;
	iS16 SrcEndY = 0;
	// 绘制到屏幕上的左上角和右下角坐标  
	iS16 DrawStartX = 0;
	iS16 DrawStartY = 0;
	iS16 DrawEndX = 0;
	iS16 DrawEndY = 0;
	iU16 iTemp = 0;

	// ImageX
	iS32 CurrentImageX = 0;
	iS32 CurrentImageY = 0;
	iS32 CurrentImageWidth = 0;

	// 当前点的原始坐标	
	iS32 CurrX = 0, CurrY = 0;
	// 当前点选择后的坐标
	iS32 RotateX = 0, RotateY = 0;
	// 颜色buff
	iU16 * ColorBuff = iNULL;
	iS32 DrawW = 0;
	iS32 DrawH = 0;
	iU16* TempReadDataBuff1 = iNULL;
	iS32 AddsDirectionY = 0;
	iS32 BuffPos = 0;
	iCOLOR TempColor = 0;
	// 带Alpha信息的数据点个数
	iU16 TotalAlphaNumber = 0;
	// 从文件中读取的数据长度	
	iU32 LengthNeedReadData = 0;
	// 开闭空间读取文件
	iU8  * ImageDataEntry = iNULL;
	// 从容器中读到的文件总长度
	iU32 LengthTotalRead = 0;
	// 文件操作函数返回值
	iU8   FlagReturn = iFALSE;
	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;
	iU16 ScreenWidth = 0;
	iU16 ScreenHeight= 0;

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
	DataEntry = ImageData;
	ImageDataEntry = (iU8*)DataEntry;
	ImageWidth = ImageDataEntry[9]|(ImageDataEntry[10]<<8);
	ImageHeight = ImageDataEntry[11]|(ImageDataEntry[12]<<8);
	LengthIamgeData = ImageDataEntry[5]|(ImageDataEntry[6]<<8)|(ImageDataEntry[7]<<16)|(ImageDataEntry[8]<<24);
	TotalAlphaNumber = ImageDataEntry[18]|(ImageDataEntry[19]<<8);

	DrawW = SrcRect->Width;
	DrawH = SrcRect->Height;
	SrcStartX = SrcRect->X;
	SrcStartY = SrcRect->Y;
	SrcEndX = SrcStartX +DrawW;
	SrcEndY = SrcStartY + DrawH;  

	DrawStartX = DesRect->X;
	DrawStartY = DesRect->Y;
	DrawEndX = DrawStartX + DesRect->Width - 1;
	DrawEndY = DrawStartY + DesRect->Height;


	DataEntry = (iU32)ImageDataEntry;
	LengthTotalReadData = 0X14;
	DataEntry += 0X14;

	while(1)
	{
		TempReadDataBuff1 = (iU16 *)(DataEntry);   
		CurrentImageX = TempReadDataBuff1[0];
		CurrentImageY = TempReadDataBuff1[1];
		CurrentImageWidth = TempReadDataBuff1[2];

		DataEntry += 6;
		LengthTotalReadData += 6 + (CurrentImageWidth<<1);

		if( (CurrentImageY < SrcStartY) || (CurrentImageX >= SrcEndX) )
		{
			DataEntry += ( CurrentImageWidth<<1 );
			continue;
		}

		if( CurrentImageY >= SrcEndY ) break;

		ColorBuff = (iU16*)DataEntry;
		DataEntry += ( CurrentImageWidth<<1 );
		LengthCopyData = CurrentImageWidth;

		if( CurrentImageX<SrcStartX )
		{
			LengthCopyData -= SrcStartX - CurrentImageX;
			CurrentImageX = SrcStartX;
			ColorBuff = &ColorBuff[SrcStartX - CurrentImageX];
			if( LengthCopyData<0 ) continue;
		}

		if( (CurrentImageX+LengthCopyData) > SrcEndX )
		{
			LengthCopyData = SrcEndX - CurrentImageX;
		}

		AddsDirectionY = CurrentImageY-SrcStartY+DrawW;
		CurrY = CurrentImageY - SrcStartY;
		CurrX = CurrentImageX - SrcStartX;
		for( iTemp=0; iTemp<LengthCopyData; iTemp++ )
		{
			RotateX = CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
			RotateY = CurrY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
			CurrX++;
			if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;
			BuffPos = RotateY*ScreenWidth+ RotateX;
			ScreenBuffer[BuffPos] = ColorBuff[iTemp];
			ScreenBuffer[BuffPos+1] = ColorBuff[iTemp];
		}

		if( (CurrentImageY == SrcEndY) && ((CurrentImageX+LengthCopyData) >= SrcEndX)) break;

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

		DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;

		for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
		{
			TempReadDataBuff1 = (iU16 *)(DataEntry);   
			AlphaPosX = TempReadDataBuff1[0];
			AlphaPosY = TempReadDataBuff1[1];
			AlphaNumber = (iALPHA)TempReadDataBuff1[2];
			AlphaColor = TempReadDataBuff1[3];
			DataEntry += 8;

			if(AlphaPosY>=SrcEndY) break;
			if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

			CurrX = AlphaPosX - SrcStartX;
			CurrY = AlphaPosY - SrcStartY;
			AddsDirectionY = AlphaPosY-SrcStartY+DrawW;
			RotateX =  CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
			RotateY = AlphaPosY - SrcStartY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
			if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;

			LcdBuffPos =RotateY*ScreenWidth+RotateX;
			ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}

	return iTRUE;

}

iBOOL i51KitG2RotateImageEx(iU32 ImageData, iRECT* DesRect , iRECT* SrcRect,iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle )
{
	// author : Otto
	// since : Aug 29 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	//ContainerHandle	待绘制图片所在容器的句柄
	//ImageID	图片ID
	//DesRect	图片绘制的目的区域
	//ImageHeight	图片绘制的裁剪区
	//SrcMoveX	图片旋转前平移至点（SrcMoveX,SrcMoveY）
	//SrcMoveY	图片旋转前平移至点（SrcMoveX,SrcMoveY）
	//RotateCenterX 图片旋转中心X坐标
	//RotateCenterY 图片旋转中心Y坐标
	//RotateType	旋转类型
	//Angle 旋转角度

	// notes : 按顺时针方向旋转一定角度后绘制ImageContainer容器内编号为ImageID的图像
	// SrcRect 为空时在点(DesRect->X, DesRect->Y)处开始绘制整个图片
	// DesRect 为空则绘制从屏幕左上角开始

#define ROTATE_MAX_GIRTH_HALF 600

	iU8 * ImageDataPoint = iNULL;
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	// 图片编码方式
	iU8   DecodeType = 0;
	iRECT ImageRect;
	iRECT DrawDesRect;
	iRECT DrawSrcRect;
	iU8 Ret = iFALSE;
	iS16 * GirthPoint = iNULL;
	iU16 TotalAlphaNumber = 0;
	iU32 LengthIamgeData = 0;
	iU16 ScreenWidth = 0;
	iU16 ScreenHeight= 0;

	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	ImageDataPoint = (iU8 *)ImageData;
	if( iNULL==ImageDataPoint ) return iFALSE;

	if( ImageDataPoint[0X11]!=0X02 )
	{
		iLog("文件格式错误，请重新生成图片文件!");
		return iFALSE;
	}

	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

	if( iNULL==SrcRect )
	{
		DrawSrcRect.X = 0;
		DrawSrcRect.Y = 0;
		DrawSrcRect.Width = ImageWidth;
		DrawSrcRect.Height = ImageHeight;
	}
	else
	{
		DrawSrcRect.X = SrcRect->X;
		DrawSrcRect.Y = SrcRect->Y;
		DrawSrcRect.Width = SrcRect->Width;
		DrawSrcRect.Height = SrcRect->Height;
	}

	if( iNULL==DesRect )
	{
		DrawDesRect.X = 0;
		DrawDesRect.Y = 0;
		DrawDesRect.Width = ScreenWidth;
		DrawDesRect.Height = ScreenHeight;
	}
	else
	{
		DrawDesRect.X = DesRect->X;
		DrawDesRect.Y = DesRect->Y;
		DrawDesRect.Width = DesRect->Width;
		DrawDesRect.Height = DesRect->Height;
	}


	ImageRect.X = 0;
	ImageRect.Y = 0;
	ImageRect.Width= ImageWidth;
	ImageRect.Height= ImageHeight;

	if(DrawSrcRect.X<0) { DrawSrcRect.Width+= DrawSrcRect.X; DrawSrcRect.X = 0;}
	//    else if( DrawSrcRect.X>=ScreenWidth|| DrawSrcRect.X>=ImageRect.Width) return iTRUE;
	else if( DrawSrcRect.X>=ImageRect.Width) return iTRUE;
	//    if( (DrawSrcRect.X+DrawSrcRect.Width) > ScreenWidth ) DrawSrcRect.Width = ScreenWidth -DrawSrcRect.X;
	if( (DrawSrcRect.X+DrawSrcRect.Width) > ImageRect.Width ) DrawSrcRect.Width = ImageRect.Width -DrawSrcRect.X;

	if(DrawSrcRect.Y<0) { DrawSrcRect.Height+= DrawSrcRect.Y; DrawSrcRect.Y = 0;}
	else if( DrawSrcRect.Y>=ImageRect.Height) return iTRUE;
	//    else if( DrawSrcRect.Y>=ScreenHeight|| DrawSrcRect.Y>=ImageRect.Height) return iTRUE;
	//    if( (DrawSrcRect.X+DrawSrcRect.Width) > ScreenWidth ) DrawSrcRect.Width = ScreenWidth -DrawSrcRect.X;
	if( (DrawSrcRect.Y+DrawSrcRect.Height) > ImageRect.Height) DrawSrcRect.Height= ImageRect.Height-DrawSrcRect.Y;

	if(DrawDesRect.X<0) { DrawDesRect.Width+= DrawDesRect.X; DrawDesRect.X = 0;}
	else if( DrawDesRect.X>=ScreenWidth) return iTRUE;
	if( (DrawDesRect.X+DrawDesRect.Width) > ScreenWidth ) DrawDesRect.Width = ScreenWidth -DrawDesRect.X;

	if(DrawDesRect.Y<0) { DrawDesRect.Height+= DrawDesRect.Y; DrawDesRect.Y = 0;}
	else if( DrawDesRect.Y>=ScreenHeight) return iTRUE;
	if( (DrawDesRect.Y+DrawDesRect.Height) > ScreenHeight) DrawDesRect.Height= ScreenHeight-DrawDesRect.Y;

	if( (DrawSrcRect.Height+DrawSrcRect.Width)>=ROTATE_MAX_GIRTH_HALF  )   return iFALSE;

	GirthPoint = (iS16*)(i51AdeOsMalloc((DrawSrcRect.Height+DrawSrcRect.Width)*4,__FILE__ , __LINE__ ));

	if(iNULL==GirthPoint) 
	{
		iLog("KIT - G2 : malloc rotate");
		return iFALSE;		
	}

	if( iNULL==i51KitG2GetGirthPointsEx(DrawSrcRect, DrawDesRect,(iS16(*)[2])GirthPoint, SrcMoveX, SrcMoveY, RotateCenterX, RotateCenterY,RotateType, Angle))
	{
		i51AdeOsFree(GirthPoint);
		GirthPoint = iNULL;
		return iTRUE;
	}	

	RotateRamLinearImageEx((iU32)ImageDataPoint, &(DrawDesRect),  &(DrawSrcRect), (iS16(*)[2])GirthPoint);
	i51AdeOsFree(GirthPoint);
	GirthPoint = iNULL;

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
	iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
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
			iLog( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
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
	iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

	return iTRUE;

}

iU8 AlphaNeed = iTRUE;
iU8 AlphaSrc = 128;
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
	iLog( "G2CleanScreen Start , %x, %x", Color, Rect) ;
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

	if(!AlphaNeed)
	{
		for( TempX=0; TempX<CleanW ; TempX++)
		{
			OneRowColorBuf[TempX] = Color;
		}

		CleanW = (CleanW<<1);	
		LcdBufPos = CleanY*ScreenWidth + CleanX;
		for( TempY=0; TempY<CleanH; TempY++)
		{
			i51AdeStdMemcpy16(&(ScreenBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
			LcdBufPos += ScreenWidth;
		}

		//if(OneRowColorBuf)
		//{
		//	i51AdeOsFree(OneRowColorBuf);
		//	OneRowColorBuf = iNULL;
		//}
	}
	else
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			LcdBufPos = (CleanY+TempY)*ScreenWidth + CleanX;
			for( TempX=0; TempX<CleanW ; TempX++)
			{
				ScreenBuffer[LcdBufPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBufPos], Color,AlphaSrc);
				LcdBufPos++;
			}
		}
	}

	if(OneRowColorBuf)
	{
		i51AdeOsFree(OneRowColorBuf);
		OneRowColorBuf = iNULL;
	}


#if i51KITG2_DEBUG
	iLog( "G2CleanScreen End , %x, %x", Color, Rect) ;
#endif
	return iTRUE;

}


//iBOOL i51KitG2CleanScreenEx(iCOLOR Color, iRECT *Rect)
//{
//	// author : Otto
//	// since : Sep 27 2011
//	// (C) 2011 , PKIG Tech. Co., Ltd.
//
//	// iCOLOR Color : 填充屏幕的颜色
//	//iRECT *Rect : 填充屏幕的区域
//
//	// Notes : 以指定的颜色清屏幕区域
//
//	iS32 CleanX, CleanY;
//	iS32 CleanW, CleanH;
//	iS32 LcdBufPos = 0;
//	iS32 TempX, TempY;
//	iU16 ScreenWidth;
//	iU16 ScreenHeight;
//	iU16 * ScreenBuffer = iNULL;
//	iU32 * LcdInfoArr = iNULL;
//	iU16 * OneRowColorBuf = iNULL;
//
//#if i51KITG2_DEBUG
//	iLog( "G2CleanScreen Start , %x, %x", Color, Rect) ;
//#endif
//
//	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
//	{
//		return iFALSE;
//	}
//
//	LcdInfoArr =  i51AdeMmiGetPanel();
//	if( iNULL == LcdInfoArr )
//	{
//		return iFALSE;
//	}
//
//	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
//
//	if( Rect)
//	{
//		CleanX = Rect->X;
//		CleanY = Rect->Y;
//		CleanW = Rect->Width;
//		CleanH = Rect->Height;
//	}
//	else
//	{
//		CleanX = 0;
//		CleanY = 0;
//		CleanW = ScreenWidth;
//		CleanH = ScreenHeight;
//	}
//
//	if(CleanX<0)
//	{
//		CleanW += CleanX;
//		CleanX = 0;
//	}
//
//	if(CleanY<0)
//	{
//		CleanH += CleanH;
//		CleanY = 0;
//	}
//
//	if( (CleanX+CleanW)>ScreenWidth) CleanW = ScreenWidth-CleanX;
//	if( (CleanY+CleanH)>ScreenHeight) CleanH = ScreenHeight-CleanY;
//
//	if( ScreenHeight>ScreenWidth)
//	{
//		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenHeight<<1),  __FILE__ , __LINE__ );
//	}
//	else
//	{
//		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenWidth<<1),  __FILE__ , __LINE__ );
//	}
//
//	for( TempX=0; TempX<CleanW ; TempX++)
//	{
//		OneRowColorBuf[TempX] = Color;
//	}
//
//	CleanW = (CleanW<<1);	
//	LcdBufPos = CleanY*ScreenWidth + CleanX;
//	for( TempY=0; TempY<CleanH; TempY++)
//	{
//		i51AdeStdMemcpy16(&(ScreenBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
//		LcdBufPos += ScreenWidth;
//	}
//
//	if(OneRowColorBuf)
//	{
//		i51AdeOsFree(OneRowColorBuf);
//		OneRowColorBuf = iNULL;
//	}
//
//#if i51KITG2_DEBUG
//	iLog( "G2CleanScreen End , %x, %x", Color, Rect) ;
//#endif
//	return iTRUE;
//
//}
