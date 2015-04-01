#include "i51Public.h"

void i51FrameShiftXY(iRECT *prc,iS16 addX,iS16 addY)
{
	if (iNULL == prc)
	{
		return;
	}
	prc->X += addX;
	prc->Y += addY;
}


void i51FrameItemInitRect(iRECT *prc, iS32 x, iS32 y, iU32 w, iU32 h)
{
	if (iNULL == prc)
	{
		return;
	}
	prc->X = (iS16)x;
	prc->Y = (iS16)y;
	prc->Width = (iS16)w;
	prc->Height = (iS16)h;
}
iBOOL i51FrameGetOverlapArea(iRECT *prc1, iRECT *prc2, iRECT *prcOverlap)
{
	// author : John
	// since : 2011.12.16
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// RECT *prc1 : 区域1
	// RECT *prc2 : 区域2
	// RECT *prcOverlap : 输出的重叠区域
	// return iBOOL : 是否存在重叠区域

	// notes : 获取两个区域的重叠区域

	iRECT rc;
	iRECT rc1, rc2;
	iS16 rbX;
	iS16 rbY;
	iU16 sScreenW, sScreenH;
	i51AdeMmiGetScreenScale(&sScreenW, &sScreenH);
	if (iNULL == prc1) {
		i51FrameItemInitRect(&rc1, 0, 0, sScreenW, sScreenH);        
	} else {
		rc1 = *prc1;
	}

	if (iNULL == prc2) {
		i51FrameItemInitRect(&rc2, 0, 0, sScreenW, sScreenH);        
	} else {
		rc2 = *prc2;
	}

	// 左上角x和y坐标最大的值
	rc.X = MAX(rc1.X, rc2.X);
	rc.Y = MAX(rc1.Y, rc2.Y);

	//右下角x和y坐标最小的值
	rbX = MIN(rc1.X + rc1.Width, rc2.X + rc2.Width);
	rbY = MIN(rc1.Y + rc1.Height, rc2.Y + rc2.Height);

	// 重叠区域
	rc.Width = rbX - rc.X;
	rc.Height = rbY - rc.Y;
	if (iNULL != prcOverlap) *prcOverlap = rc;
	// 是否重叠
	return (rc.X < rbX && rc.Y < rbY);
}

void i51FrameMovePanel(iRECT *prc, iS16 x, iS16 y)
{
	// notes : 把屏幕上某一块区域(prc)的图像移动到新的坐标[x, y]
	//         直接操作屏幕指针。
	iU32 *array = i51AdeMmiGetPanel();
	iU16 *hPanel = (iU16 *)array[1];
	iU16 nPanelW = array[2];
	iU16 nPanelH = array[3];
	iRECT rcScreen;
	iRECT rcTar;
	iBOOL bValid = iFALSE;

	if (iNULL == prc) return;
	i51FrameItemInitRect(&rcScreen, 0, 0, nPanelW, nPanelH);
	if (iTRUE == i51FrameGetOverlapArea(prc, &rcScreen, prc))
	{
		i51FrameItemInitRect(&rcTar, x, y, prc->Width, prc->Height);
		if (iTRUE == i51FrameGetOverlapArea(&rcTar, &rcScreen, &rcTar))
		{
			bValid = iTRUE;
		}
	}

	if (iFALSE == bValid) return;

	if (prc->Width == nPanelW && rcTar.X == 0)      // memmove
	{
		memmove(&hPanel[rcTar.Y * nPanelW], &hPanel[prc->Y * nPanelW + prc->X], sizeof(iU16) * rcTar.Width * rcTar.Height);
	}
	else if (prc->Y > rcTar.Y)                      // memcpy line by line
	{
		iS16 i, j;
		for (i = rcTar.Y, j = prc->Y; i < rcTar.Y + rcTar.Height; ++i, ++j)
		{
			memcpy(&hPanel[i * nPanelW + rcTar.X], &hPanel[j * nPanelW + prc->X], sizeof(iU16) * rcTar.Width);
		}
	}
	else if (prc->Y < rcTar.Y)
	{
		iS16 i, j;
		for (i = rcTar.Y + rcTar.Height - 1, j = prc->Y + rcTar.Height -1; i >= rcTar.Y; --i, --j)
		{
			memcpy(&hPanel[i * nPanelW + rcTar.X], &hPanel[j * nPanelW + prc->X], sizeof(iU16) * rcTar.Width);
		}
	}
	else
	{
		iS16 i;
		for (i = rcTar.Y; i < rcTar.Y + rcTar.Height; ++i)
		{
			memmove(&hPanel[i * nPanelW + rcTar.X], &hPanel[i * nPanelW + prc->X], sizeof(iU16) * rcTar.Width);
		}
	}
}

iBOOL i51PublicCheckUSMatchTail(iWCHAR *src_Str,iWCHAR *tail_Str)
{
	int srcStrLen,tailStrLen,i,j;
	iBOOL ret = iFALSE;
	srcStrLen = i51AdeStdWStrlen(src_Str);
	tailStrLen = i51AdeStdWStrlen(tail_Str);
	if (srcStrLen < tailStrLen)
	{
		return ret;
	}
	else
	{
		i=srcStrLen;
		j=tailStrLen;
		for (;j>0;j--)
		{
			if (*(src_Str+i--)!=*(tail_Str+j))
			{
				return ret;
			}
		}
		ret = iTRUE;
		return ret;
	}
}

void i51PublicDrawSysWStr(iWCHAR *pwStr,iRECT *prect,iCOLOR Color,DRAW_STYLE_H style_h,DRAW_STYLE_V style_v)
{
	int i;
	iS16 X,Y;
	iU16 StrWidth,StrHeight;
	iU32 *lcd_address;
//	static iU16 wszStr[256];
	lcd_address =  i51AdeMmiGetPanel();
	if (iNULL == lcd_address)
	{
		return;
	}
//	mr.pscreen = 
	i51AdeMmiGetTextAttr(pwStr,&StrWidth,&StrHeight);
	if (style_h == DRAW_STYLE_H_LEFT)
	{
		X = prect->X;
	}
	else if (style_h == DRAW_STYLE_H_MID)
	{
		X = prect->X+(prect->Width-(iS16)StrWidth)/2;
	}
	else if (style_h == DRAW_STYLE_H_RIGHT)
	{
		X = prect->X+prect->Width-(iS16)StrWidth;
	}

	if (style_v == DRAW_STYLE_V_TOP)
	{
		Y = prect->Y;
	}
	else if (style_v == DRAW_STYLE_V_MID)
	{
		Y = prect->Y+(prect->Height-(iS16)StrHeight)/2;;
	}
	else if (style_v == DRAW_STYLE_V_BOTTOM)
	{
		Y = prect->Y+prect->Height-(iS16)StrHeight;
	}
	i51AdeMmiDrawText((iU8 *)lcd_address,StrWidth,StrHeight,pwStr, X ,Y,Color);
}


iBOOL i51PubicPtInRect(iS16 pt_x,iS16 pt_y,iRECT *dstRect)
{
	return ( (pt_x>dstRect->X)&&(pt_x<(dstRect->X+dstRect->Width))&&(pt_y>dstRect->Y)&&(pt_y<(dstRect->Y+dstRect->Height)) );
}
/*0~9   48~57*/
/*A~Z   65~90*/
/*a~z   97~122*/

#define  BM_OFFSET 32
STR_CMP i51PublicStrCmp(iCHAR *pStrA,iCHAR *pStrB)
{
   STR_CMP ret = STR_CMP_EQUAL;

   while(1)
   {
	   if (*pStrA != *pStrB)
	   {  
		   if ( ( (*pStrA >= 'A' && *pStrA <= 'Z')&&(*pStrB >= 'a' && *pStrB <= 'z') ))
		   {   
			   if (*pStrA+BM_OFFSET >= *pStrB)
			   {
				    ret = STR_CMP_BIGER;
			   }
			   else
			   {
				    ret = STR_CMP_SMALLER;
			   }
			  
		   }
		   else if (( (*pStrB >= 'A' && *pStrB <= 'Z')&&(*pStrA >= 'a' && *pStrA <= 'z') ))
		   {
			   if (*pStrB+BM_OFFSET >= *pStrA)
			   {
				   ret = STR_CMP_SMALLER;
			   }
			   else
			   {
				   ret = STR_CMP_BIGER;
			   }
		   }
		   else
		   {
			   if (*pStrA-*pStrB>0)
			   {
				   ret = STR_CMP_BIGER;
			   }
			   else
			   {
				   ret = STR_CMP_SMALLER;
			   }
		   }
		   break;
	   }
	   pStrA++;
	   pStrB++;
   }
   return ret;
}