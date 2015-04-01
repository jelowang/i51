
#include "i51KitAde.h"
#include "i51KitG1.h"
#include "i51KitG2.h"
#include "mwPublic.h"

//static iU16 *se_ScreenBuf = iNULL;
static iU16 se_ScreenWidth=SCREEN_WIDTH, se_ScreenHeight=SCREEN_HEIGHT;
static iCOLOR se_BorderWidth = 3;
//static iCOLOR se_FillColor = 0xFFFF, se_BorderColor = 0xDDDD;
static iCOLOR se_FillColor = COLOR_WATER, se_BorderColor = COLOR_WATER;
static iS16 se_BaseCount = 0;
iS16 se_TrackPointCount = 0;
iS16 se_TrackPoint[EFFECT_MAXTRACKPOINT][2] = {0};
static iS16 se_endDrawFlag = 0;
static iALPHA se_alpha = 0xFF;

#define G1_MAXBLADEWIDTH	4		//刀锋最大宽度(一半)
#define G1_MAXKNIFEPOINT    12		//刀尖长
#define G1_MAXTRACKPOINT	8		//轨迹点数组最大个数
#define G1_MINDISTANCE		144		//轨迹点间最小距离的平方
#define G1_MAXPOINTNUM		5

#define G1_MAXTOPPOINT      8		//多边线顶点最大值
//活性边表结构
typedef struct tagACTIVE_EDGE
{
	iS16 IsValid;		 //是否有效的标志 0==无效 1==有效
	iS16 MaxCoord;		 //扫描线移动的方向上最大值
	iS16 factor;		 //乘积因子
	iS32 slope;			 //斜率（扩大factor倍）
	iS32 IntersectCoord; //与扫描线交点坐标（扩大factor倍）

	struct tagACTIVE_EDGE *next; //下一条活性边
}G1_ACTIVE_EDGE;


//边分类表结构
typedef struct tagCLASS_EDGE
{
	iS16 coord;					  //扫描线坐标
	iS16 count;					  //本分类的边条数
	iS16 id[G1_MAXTOPPOINT];	  //边id集合
}G1_CLASS_EDGE;

typedef struct tagEDGE_TABLE
{
	iS16	type;							//扫描方式 0==X轴方向， 1==Y轴方向
	iS16	coord;							//当前扫描线坐标

	iS32    EdgeCount;						//边条数
	iS16	line[G1_MAXTOPPOINT][4];		//边 [2] x, y

	iS32	ClassCount;							//边分类个数
	G1_CLASS_EDGE	ClassTable[G1_MAXTOPPOINT]; //分类边表

	iS32    ActiveCount;						//活性边条数
	G1_ACTIVE_EDGE *ActiveHead;						//链表头
	G1_ACTIVE_EDGE ActiveTable[G1_MAXTOPPOINT]; //边表
}G1_EDGE_TABLE;

static G1_EDGE_TABLE  G1_EdgeTable = {0};

static iCOLOR i51KitG1GetNewColor(iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
	/**
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

static iBOOL i51KitG1DrawPoint( iS32 X, iS32 Y, iCOLOR color ,iALPHA Alpha)
{
	if( Y>=0 && Y<SCREEN_HEIGHT&& X>=0 && X<SCREEN_WIDTH)
	{
		iS32 WorldAttributeY = Y/2;
		iS32 WorldAttributeX = X/2;
		iS32 RemainderY = Y%2;
		iS32 RemainderX = X%2;
		iS32 ScreenPos = 0;
		iU8 WorldAttributeFlag = 0;
	
		
		if( RemainderX) WorldAttributeFlag = 0X20;
		else WorldAttributeFlag = 0X10;

		if( RemainderY ) WorldAttributeFlag = WorldAttributeFlag<<2;

		if( (((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&0XF)==0  || WaterDrawAll))
		&& ((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&WorldAttributeFlag)==0) )
		{
			ScreenPos = Y*SCREEN_WIDTH + X;
			mwScreenBuffer[ScreenPos] = i51KitG1GetNewColor(mwScreenBuffer[ScreenPos], color, Alpha);
			mwWorldAttribute[WorldAttributeY][WorldAttributeX] |= WorldAttributeFlag;
		}

		return iTRUE;
	}

	return iFALSE;
}
/************************************************************************/
/*          以下是绘图函数部分                                          */
/************************************************************************/

static float i51KitG1Sqrt(float x)
{
	union{
		int intPart;
		float floatPart;
	} convertor;

	union{
		int intPart;
		float floatPart;
	} convertor2;

	convertor.floatPart = x;
	convertor2.floatPart = x;
	convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
	convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);

	return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
}

static iBOOL i51KitG1GetNormalPoints(iS16 NormalPoint[2][2], iS16 LinePoint[2][2], iS16 Width, iS16 CentreX, iS16 CentreY)
{
	iS16 dx=0, dy=0;
	iS16 abs_dx=0, abs_dy=0;
	iS32 flag = 0, dis=0, dvalue=0;
	iS16 MoveX=0, MoveY=0;
	float fvalue = 0;

	if (Width <0) return iFALSE;

	dx= LinePoint[1][0] -  LinePoint[0][0];
	dy= LinePoint[1][1] -  LinePoint[0][1];
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;
	flag   = dx * dy;

	if ( 0 == Width || (0==dx &&0==dy))
	{
		NormalPoint[0][0] = CentreX;
		NormalPoint[0][1] = CentreY;
		NormalPoint[1][0] = CentreX;
		NormalPoint[1][1] = CentreY;

		return iTRUE;
	}

	//垂直x
	if ( 0 == dx )
	{
		NormalPoint[0][0] = CentreX - Width;
		NormalPoint[0][1] = CentreY;
		NormalPoint[1][0] = CentreX + Width;
		NormalPoint[1][1] = CentreY;
		return iTRUE;
	}

	//垂直y
	if ( 0 == dy )
	{
		NormalPoint[0][0] = CentreX;
		NormalPoint[0][1] = CentreY - Width;
		NormalPoint[1][0] = CentreX;
		NormalPoint[1][1] = CentreY + Width;

		return iTRUE;
	}

	if(flag > 0)
	{
		dis    = abs_dx*abs_dx + abs_dy*abs_dy;
		dvalue = Width*Width*abs_dy*abs_dy;
		fvalue =  (float)dvalue / (float)dis;
		fvalue = i51KitG1Sqrt(fvalue);
		MoveX = (iS16)(fvalue + 0.5);

		dvalue = Width*Width*abs_dx*abs_dx;
		fvalue =  (float)dvalue / (float)dis;
		fvalue = i51KitG1Sqrt(fvalue);
		MoveY = (iS16)(fvalue + 0.5);

		NormalPoint[0][0] = -MoveX + CentreX;
		NormalPoint[0][1] = MoveY + CentreY;

		NormalPoint[1][0] = MoveX + CentreX;
		NormalPoint[1][1] = -MoveY + CentreY;
	}
	else
	{
		dis    = abs_dx*abs_dx + abs_dy*abs_dy;
		dvalue = Width*Width*abs_dy*abs_dy;
		fvalue =  (float)dvalue / (float)dis;
		fvalue = i51KitG1Sqrt(fvalue);
		MoveX = (iS16)(fvalue + 0.5);

		dvalue = Width*Width*abs_dx*abs_dx;
		fvalue =  (float)dvalue / (float)dis;
		fvalue = i51KitG1Sqrt(fvalue);
		MoveY = (iS16)(fvalue + 0.5);

		NormalPoint[0][0] = -MoveX + CentreX;
		NormalPoint[0][1] = -MoveY + CentreY;

		NormalPoint[1][0] = MoveX + CentreX;
		NormalPoint[1][1] = MoveY + CentreY;
	}

	return iTRUE;
}

/*
//////////////////////////////////////////////////////////////////////////
//以下是新绘制函数
static iBOOL i51KitG1DrawLine(iS16 line[2][2], iS16 width, iCOLOR color, iALPHA alpha)
{
//  author : yepan
//  since  : 2012-7-5
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			 line  直线
//			 width 直线宽度
//			 color 颜色
//			 alpha 透明度
//  return :
//  note   :
//			绘制多线宽反走样直线

	iS16 i=0, j=0, count = 0;
	iS16 dx=0, dy=0;
	iS16 abs_dx=0, abs_dy=0;
	iS16 unit_dx=0, unit_dy=0;
	iS16 TempX = 0, TempY=0;
	iS16 StartX = 0, StartY=0;
	iS16 BaseX = 0, BaseY=0;
	iALPHA AlphaValue = 0xFF;
	iU16 *DesPix = iNULL;
	iS16 d0=0, di=0, dWidth=0;
	iS32 BaseValue=0, factor=0, dis=0;

	if (width <= 0) return iFALSE;

	width =  width<2 ? 2 : width;

	dx = line[1][0] - line[0][0];
	dy = line[1][1] - line[0][1];
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;

	count = abs_dx > abs_dy ? abs_dx : abs_dy;
	AlphaValue = alpha;

	//直线两点重合
	if ( 0 == abs_dx &&  0 == abs_dy )
	{
		TempX = line[0][0];
		TempY = line[0][1];
		if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
		{
			i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
		}
		
		return iTRUE;
	}
	else if ( 0 == abs_dx ) //垂直于x轴
	{
		StartX = line[0][0] - width/2;
		StartY = line[0][1];
		unit_dy = dy/abs_dy;
		dWidth = unit_dy>0 ? se_ScreenWidth : -se_ScreenWidth;
		for (i=0; i<width; i++)
		{
			TempX = StartX+i;
			TempY = StartY;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}
				TempY += unit_dy;
			}
		}

		//对称算法 略
		return iTRUE;	
	}
	else if (0 == abs_dy) //垂直于y轴
	{
		StartX = line[0][0];
		StartY = line[0][1] - width/2;
		unit_dx = dx/abs_dx;
		dWidth = unit_dx>0 ? 1 : -1;
		for (i=0; i<width; i++)
		{
			TempX = StartX;
			TempY = StartY + i;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}

				TempX += unit_dx;

			}
		}

		return iTRUE;
	}


	//既不垂直x轴也不垂直y轴
	//bresenham直线画法
	unit_dx = dx/abs_dx;
	unit_dy = dy/abs_dy;
	BaseX = line[0][0];
	BaseY = line[0][1];
	//斜率绝对值小于1
	if ( abs_dx > abs_dy )
	{
		d0 = 2*abs_dy - abs_dx;
		di = d0;

		factor = (dx*line[0][1] - dy*line[0][0]);
		for (i=0; i<=count; i++)
		{	
			//画一条扫描线
			BaseValue = dy*BaseX + factor;
			dis = dx*BaseY - BaseValue;
			dis = dis*unit_dx + abs_dx/2;
			AlphaValue = alpha*dis/abs_dx;
			StartY = BaseY - width/2;
			
			TempX = BaseX;
			TempY = StartY;
			if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
			{
				i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
			}

			AlphaValue = alpha - AlphaValue;
			TempY = StartY + width-1;
			if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
			{
				i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
			}

			AlphaValue = alpha;
			TempY = StartY + 1;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}
				TempY++;
			}

			if (di < 0)
			{
				di += 2*abs_dy;
			}
			else
			{
				di += 2*(abs_dy - abs_dx);
				BaseY += unit_dy;
			}
			BaseX += unit_dx;
		}
	}
	else
	{
		d0 = 2 * abs_dx - abs_dy;
		di = d0;
		factor = (dx*line[0][1] - dy*line[0][0]);
		for (i=0; i<=count; i++)
		{
			//画一条扫描线
			BaseValue = dx*BaseY - factor;
			dis = dy*BaseX - BaseValue;
			dis = dis*unit_dy + abs_dy/2;
			AlphaValue = alpha*dis/abs_dy;
			StartX = BaseX - width/2;

			TempX = StartX;
			TempY = BaseY;
			if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
			{
				i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
			}

			AlphaValue = alpha - AlphaValue;
			TempX = StartX + width-1;
			if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
			{
				i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
			}

			AlphaValue = alpha;
			TempX = StartX+1;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}

				TempX++;
			}

			if (di < 0)
			{
				di += 2*abs_dx;
			}
			else
			{
				di += 2*(abs_dx - abs_dy);
				BaseX += unit_dx;
			}
			BaseY += unit_dy;
		}
	}

	return iTRUE;
}

*/
/*
static iBOOL i51KitG1DrawLineEx(iS16 line[2][2], iS16 type, iS16 width, iCOLOR color)
{
	//  author : yepan
	//  since  : 2012-7-5
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			 line  直线
	//			 type  alpha方向 0==下 1==上 2==左 3==右
	//			 width 直线宽度
	//			 color 颜色
	//  return :
	//  note   :
	//			绘制多线宽反走样直线

	iS16 i=0, j=0, count = 0;
	iS16 dx=0, dy=0;
	iS16 abs_dx=0, abs_dy=0;
	iS16 unit_dx=0, unit_dy=0;
	iS16 TempX = 0, TempY=0;
	iS16 StartX = 0, StartY=0;
	iS16 BaseX = 0, BaseY=0;
	iALPHA AlphaValue = 0xFF;
	iU16 *DesPix = iNULL;
	iS16 d0=0, di=0, dWidth=0;
	iS32 BaseValue=0, factor=0, dis=0;

	return iTRUE;
	
	if (width <= 0) return iFALSE;

	width =  width<2 ? 2 : width;

	dx = line[1][0] - line[0][0];
	dy = line[1][1] - line[0][1];
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;

	count = abs_dx > abs_dy ? abs_dx : abs_dy;
	AlphaValue = 0xFF;

	//直线两点重合
	if ( 0 == abs_dx &&  0 == abs_dy )
	{
		TempX = line[0][0];
		TempY = line[0][1];
		if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
		{
			i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
		}

		return iTRUE;
	}
	else if ( 0 == abs_dx ) //垂直于x轴
	{
		StartX = line[0][0] - width/2;
		StartY = line[0][1];
		unit_dy = dy/abs_dy;
		dWidth = unit_dy>0 ? se_ScreenWidth : -se_ScreenWidth;

		if ( 2 == type )//左
		{
			i = 1;
		}
		else if (3 == type)//右
		{
			width--;			
		}

		for (i; i<width; i++)
		{
			TempX = StartX+i;
			TempY = StartY;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}

				TempY += unit_dy;
			}
		}

		return iTRUE;	
	}
	else if (0 == abs_dy) //垂直于y轴
	{
		StartX = line[0][0];
		StartY = line[0][1] - width/2;
		unit_dx = dx/abs_dx;
		dWidth = unit_dx>0 ? 1 : -1;

		if ( 0 == type )//下
		{
			i = 1;
		}
		else if (1 == type)//上
		{
			width--;			
		}

		for (i=0; i<width; i++)
		{
			TempX = StartX;
			TempY = StartY + i;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}

				TempX += unit_dx;

			}
		}

		return iTRUE;
	}


	//既不垂直x轴也不垂直y轴
	//bresenham直线画法
	unit_dx = dx/abs_dx;
	unit_dy = dy/abs_dy;
	BaseX = line[0][0];
	BaseY = line[0][1];
	//斜率绝对值小于1
	if ( abs_dx > abs_dy )
	{
		d0 = 2*abs_dy - abs_dx;
		di = d0;

		factor = (dx*line[0][1] - dy*line[0][0]);
		for (i=0; i<=count; i++)
		{	
			//画一条扫描线
			BaseValue = dy*BaseX + factor;
			dis = dx*BaseY - BaseValue;
			dis = dis*unit_dx + abs_dx/2;
			AlphaValue = COLOR_ALPHA*dis/abs_dx;
			StartY = BaseY - width/2;

			TempX = BaseX;
			if ( 0 == type) //下
			{
				TempY = StartY;
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
			}

			if (1 == type) //上
			{
				AlphaValue = COLOR_ALPHA - AlphaValue;
				TempY = StartY + width-1;
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
			}

			TempY = StartY + 1;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
				TempY++;
			}

			if (di < 0)
			{
				di += 2*abs_dy;
			}
			else
			{
				di += 2*(abs_dy - abs_dx);
				BaseY += unit_dy;
			}
			BaseX += unit_dx;
		}
	}
	else
	{
		d0 = 2 * abs_dx - abs_dy;
		di = d0;
		factor = (dx*line[0][1] - dy*line[0][0]);
		for (i=0; i<=count; i++)
		{
			//画一条扫描线
			BaseValue = dx*BaseY - factor;
			dis = dy*BaseX - BaseValue;
			dis = dis*unit_dy + abs_dy/2;
			AlphaValue = COLOR_ALPHA*dis/abs_dy;
			StartX = BaseX - width/2;

			TempY = BaseY;
			if ( 2 == type)//左
			{
				TempX = StartX;
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
			}

			if ( 3 == type ) //右
			{
				AlphaValue = COLOR_ALPHA - AlphaValue;
				TempX = StartX + width-1;
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
			}

			TempX = StartX+1;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}

				TempX++;
			}

			if (di < 0)
			{
				di += 2*abs_dx;
			}
			else
			{
				di += 2*(abs_dx - abs_dy);
				BaseX += unit_dx;
			}
			BaseY += unit_dy;
		}
	}

	return iTRUE;
}
*/

static iBOOL i51KitG1GreateClassET(iS16 (*Polygon)[2])
{
	//  author : yepan
	//  since  : 2012-7-2
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			无
	//  return :
	//			iTRUE==成功， iFALSE==失败
	//  note   :
	//			创建边分类表, 下闭上开，左闭右开
	
	iS16 i=0, j=0, k=0;
	iS16 EdgeCount=0, CoordCount = 0;
	iS16 coord[G1_MAXTOPPOINT] = {0}; //坐标
	iS16 CoordValue = 0;
	iS16 flag = 1;					//是否需要插入数组的标志 1==是, 0==否
	G1_CLASS_EDGE *pNode = iNULL;

	if (G1_EdgeTable.EdgeCount < 3) return iFALSE;

	G1_EdgeTable.ClassCount = 0;
	i51AdeStdMemset16(G1_EdgeTable.ClassTable, 0, sizeof(G1_EdgeTable.ClassTable));

	//分类并排序扫描线方向的坐标值
	EdgeCount = G1_EdgeTable.EdgeCount;
	for ( i=0; i<EdgeCount; i++ )
	{
		flag = 1;
		if ( 0 == G1_EdgeTable.type )
		{
			CoordValue = Polygon[i][0];
		}
		else
		{
			CoordValue = Polygon[i][1];	
		}

		//定位要插入的位置
		for( j=0; j<CoordCount; j++)
		{
			if (CoordValue <= coord[j])
			{
				flag = (CoordValue == coord[j]) ? 0 : 1;
				break;
			}
		}

		if ( flag )
		{
			for (k=CoordCount-1; k>=j; k--)
			{
				coord[k+1] = coord[k];
			}

			coord[j] = CoordValue;
			CoordCount++;
		}
	}

	G1_EdgeTable.ClassCount = CoordCount;
	//构建边分类表
	if ( 0 == G1_EdgeTable.type )
	{
		for (i=0; i<CoordCount; i++)
		{
			pNode = &(G1_EdgeTable.ClassTable[i]);
			pNode->coord = coord[i];

			for (j=0; j<EdgeCount; j++)
			{
				if ( coord[i] >= G1_EdgeTable.line[j][0] && coord[i] < G1_EdgeTable.line[j][2])
				{
					pNode->id[pNode->count] = j;
					(pNode->count)++;
				}
			}
		}
	}
	else
	{
		for (i=0; i<CoordCount; i++)
		{
			pNode = &(G1_EdgeTable.ClassTable[i]);
			pNode->coord = coord[i];

			for (j=0; j<EdgeCount; j++)
			{
				if ( coord[i] >= G1_EdgeTable.line[j][1] && coord[i] < G1_EdgeTable.line[j][3])
				{
					pNode->id[pNode->count] = j;
					(pNode->count)++;
				}
			}
		}
	}

	return iTRUE;
}

static iBOOL i51KitG1UpdateActiveET(iS16 ScanCoord)
{
//  author : yepan
//  since  : 2012-7-5
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			ScanCoord 扫描线坐标
//  return :
//  note   :
//			更新活性边表

	iS16 i=0, count=0;
	iS32 ValueTmp=0, ValueCur=0;
	iS16 (*line)[4] = 0;
	G1_ACTIVE_EDGE *pNodePre = iNULL;
	G1_ACTIVE_EDGE *pNodeCur = iNULL;
	G1_ACTIVE_EDGE *ActiveEdge = iNULL;
	G1_CLASS_EDGE  *pClassNode = iNULL;

	if ( iNULL == G1_EdgeTable.ActiveHead )
	{
		G1_EdgeTable.ActiveCount = 0;
		G1_EdgeTable.ActiveHead = G1_EdgeTable.ActiveTable;
	}

	//更新活性边里面的数据并且删除无效边
	if ( G1_EdgeTable.ActiveCount > 0)
	{
		//更新活性边里面的数据
		pNodeCur = G1_EdgeTable.ActiveHead;
		while (pNodeCur)
		{
			pNodeCur->IntersectCoord += pNodeCur->slope;
			pNodeCur = pNodeCur->next;
		}

		//删除无效边
		pNodeCur = G1_EdgeTable.ActiveHead;
		pNodePre = pNodeCur;
		while (pNodeCur)
		{
			if (pNodeCur->MaxCoord <= ScanCoord)
			{
				if ( G1_EdgeTable.ActiveHead == pNodeCur )
				{
					G1_EdgeTable.ActiveHead = pNodeCur->next;
					i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
					pNodeCur = G1_EdgeTable.ActiveHead;
					pNodePre = pNodeCur;
				}
				else
				{
					pNodePre->next = pNodeCur->next;
					i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
					pNodeCur = pNodePre->next;
				}

				G1_EdgeTable.ActiveCount--;
			}
			else
			{
				pNodePre = pNodeCur;
				pNodeCur = pNodeCur->next;
			}
		}
	}
	
	//分类表中是否有该坐标
	count = G1_EdgeTable.ClassCount;
	for (i=0; i<count; i++)
	{
		pClassNode = G1_EdgeTable.ClassTable + i;
		//该分类有边相交
		if ( ScanCoord == pClassNode->coord && pClassNode->count > 0) break;
	}

	if ( count  == i ) return iTRUE;


	//更新活性边
	pClassNode = G1_EdgeTable.ClassTable + i;
	count = pClassNode->count;
	//x方向扫描
	if ( 0 == G1_EdgeTable.type)
	{
		for (i=0; i<count; i++)
		{
			line = G1_EdgeTable.line + pClassNode->id[i];
			
			//获取空闲的节点
			ActiveEdge = G1_EdgeTable.ActiveTable;
			while ( 1 == ActiveEdge->IsValid ) ActiveEdge++;

			ActiveEdge->MaxCoord =  (*line)[2];
			ActiveEdge->factor = (*line)[2] - (*line)[0];
			ActiveEdge->slope = (*line)[3] - (*line)[1];
			ActiveEdge->IntersectCoord = ActiveEdge->slope * ScanCoord
				+ ( (ActiveEdge->factor)*((*line)[1]) - (ActiveEdge->slope)*((*line)[0]) );

			//活性边中无节点插入活性边
			if ( 0 == G1_EdgeTable.ActiveCount )
			{
				ActiveEdge->IsValid = 1;
				ActiveEdge->next = iNULL;
				G1_EdgeTable.ActiveCount = 1;
				G1_EdgeTable.ActiveHead = ActiveEdge;
				continue;
			}

			pNodeCur = G1_EdgeTable.ActiveHead;
			pNodePre = pNodeCur;
			while (pNodeCur)
			{
				ValueCur = (ActiveEdge->IntersectCoord) * (pNodeCur->factor);
				ValueTmp = (pNodeCur->IntersectCoord) * (ActiveEdge->factor);

				if (ValueCur < ValueTmp)
				{
					break;
				}
				else if (ValueCur == ValueTmp)
				{
					ValueTmp = ActiveEdge->slope * pNodeCur->factor;
					ValueCur = pNodeCur->slope * ActiveEdge->factor;

					if( ValueTmp > ValueCur )
					{
						pNodePre = pNodeCur;
						pNodeCur = pNodeCur->next;
					}
					else if ( ValueTmp == ValueCur )
					{
						if ( pNodePre != pNodeCur )
						{
							pNodePre->next = pNodeCur->next;
							i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
							G1_EdgeTable.ActiveCount--;
							pNodeCur = pNodePre->next;	
						}
						else
						{
							pNodePre = pNodeCur->next;
							i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
							G1_EdgeTable.ActiveCount--;
							pNodeCur = pNodePre;
						}
					}
					break;
				}
				else
				{
					pNodePre = pNodeCur;
					pNodeCur = pNodeCur->next;
				}
			}

			if ( pNodePre == pNodeCur )
			{
				ActiveEdge->IsValid = 1;
				ActiveEdge->next = pNodeCur;
				G1_EdgeTable.ActiveHead = ActiveEdge;
			}
			else
			{
				ActiveEdge->IsValid = 1;
				pNodePre->next = ActiveEdge;
				ActiveEdge->next = pNodeCur;
			}

			G1_EdgeTable.ActiveCount++;
		}
	}
	else //y方向扫描
	{
		for (i=0; i<count; i++)
		{
			line = G1_EdgeTable.line + pClassNode->id[i];

			//获取空闲的节点
			ActiveEdge = G1_EdgeTable.ActiveTable;
			while ( 1 == ActiveEdge->IsValid ) ActiveEdge++;

			ActiveEdge->MaxCoord =  (*line)[3];
			ActiveEdge->factor = (*line)[3] - (*line)[1];
			ActiveEdge->slope = (*line)[2] - (*line)[0];
			ActiveEdge->IntersectCoord = ActiveEdge->slope * ScanCoord
				- ( (ActiveEdge->slope)*((*line)[1]) - (ActiveEdge->factor)*((*line)[0]));

			//活性边中无节点插入活性边
			if ( 0 == G1_EdgeTable.ActiveCount )
			{
				ActiveEdge->IsValid = 1;
				ActiveEdge->next = iNULL;
				G1_EdgeTable.ActiveCount = 1;
				G1_EdgeTable.ActiveHead = ActiveEdge;
				continue;
			}

			pNodeCur = G1_EdgeTable.ActiveHead;
			pNodePre = pNodeCur;
			while (pNodeCur)
			{
				ValueCur = (ActiveEdge->IntersectCoord) * (pNodeCur->factor);
				ValueTmp = (pNodeCur->IntersectCoord) * (ActiveEdge->factor);

				if (ValueCur < ValueTmp)
				{
					break;
				}
				else if (ValueCur == ValueTmp)
				{
					ValueTmp = ActiveEdge->slope * pNodeCur->factor;
					ValueCur = pNodeCur->slope * ActiveEdge->factor;
					if( ValueTmp > ValueCur )
					{
						pNodePre = pNodeCur;
						pNodeCur = pNodeCur->next;
					}
					else if (ValueTmp == ValueCur )
					{
						if ( pNodePre != pNodeCur )
						{
							pNodePre->next = pNodeCur->next;
							i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
							G1_EdgeTable.ActiveCount--;
							pNodeCur = pNodePre->next;	
						}
						else
						{
							pNodePre = pNodeCur->next;
							i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
							G1_EdgeTable.ActiveCount--;
							pNodeCur = pNodePre;
						}
					}
					break;
				}
				else
				{
					pNodePre = pNodeCur;
					pNodeCur = pNodeCur->next;
				}
			}

			if ( pNodePre == pNodeCur )
			{
				ActiveEdge->IsValid = 1;
				ActiveEdge->next = pNodeCur;
				G1_EdgeTable.ActiveHead = ActiveEdge;
			}
			else
			{
				ActiveEdge->IsValid = 1;
				pNodePre->next = ActiveEdge;
				ActiveEdge->next = pNodeCur;
			}

			G1_EdgeTable.ActiveCount++;
		}
	}

	return iTRUE;
}

static iBOOL i51KitG1DrawActiveET(iCOLOR color, iALPHA alpha)
{
	iS16 i=0;
	iS16 StartCoord=0, EndCoord=0, FixCoord=0;
	iS16 TempX=0, TempY=0;
	G1_ACTIVE_EDGE *EdgeFirst=iNULL;
	G1_ACTIVE_EDGE *EdgeSecond=iNULL;
	iU16 *DesPix = iNULL;

	if (  0 != G1_EdgeTable.ActiveCount%2 ||  0 == G1_EdgeTable.ActiveCount)
	{
		return iFALSE;
	}

	FixCoord = G1_EdgeTable.coord;
	if ( 0 == G1_EdgeTable.type)
	{
		EdgeFirst = G1_EdgeTable.ActiveHead;
		while (EdgeFirst)
		{
			EdgeSecond = EdgeFirst->next;

			//StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor+1;
			//EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor;

			StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor;
			EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor + 1;

			TempX = FixCoord;
			TempY = StartCoord;
			for (i=StartCoord; i<=EndCoord; i++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}
				TempY++;
			}

			EdgeFirst = EdgeSecond->next;
		}
	}
	else
	{
		EdgeFirst = G1_EdgeTable.ActiveHead;
		while (EdgeFirst)
		{
			EdgeSecond = EdgeFirst->next;
			//StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor+1;
			//EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor;
			StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor;
			EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor + 1;

			TempY = FixCoord;
			TempX = StartCoord;
			for (i=StartCoord; i<=EndCoord; i++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}
				TempX++;
			}

			EdgeFirst = EdgeSecond->next;
		}
	}

	return iTRUE;
}

static iBOOL i51KitG1DrawPolygon(iS16 (*Polygon)[2], iS16 number, iS16 type, iCOLOR color, iALPHA alpha)
{
//  author : yepan
//  since  : 2012-7-5
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			Polygon 多边形顶点集合
//			number 边条数
//			color 颜色
//			alpha 透明度
//  return :
//  note   :
//		   多边形绘制	
//			

	int i=0, count = 0;
	iS16 StartIndex = 0, EndIndex=0;
	iS16 min_coord=0, max_coord=0;
	iS16 (*line)[4] = 0;
	iS16 LineTemp[2][2] = {0};

	if ( iNULL == Polygon || number <= 0 ||  number > G1_MAXTOPPOINT ) return iFALSE;

	if ( number <= 2)
	{
		//画点或线
//		i51KitG1DrawLine(Polygon, 2, color, alpha);
		return iTRUE;
	}

	//初始化边表
	i51AdeStdMemset16(&G1_EdgeTable, 0, sizeof(G1_EdgeTable));

	G1_EdgeTable.type = type;
	G1_EdgeTable.EdgeCount = number;
	if ( 0 == G1_EdgeTable.type )
	{
		for (i=0; i<number; i++)
		{
			StartIndex = i;
			EndIndex   = (i+1)%number;
			line = G1_EdgeTable.line + i;
			if ( Polygon[StartIndex][0] > Polygon[EndIndex][0] )
			{
				EndIndex   ^= StartIndex;
				StartIndex ^=  EndIndex;
				EndIndex   ^= StartIndex;
			}

			(*line)[0] = Polygon[StartIndex][0];
			(*line)[1] = Polygon[StartIndex][1];
			(*line)[2] = Polygon[EndIndex][0];
			(*line)[3] = Polygon[EndIndex][1];
		}
	}
	else
	{
		for (i=0; i<number; i++)
		{
			StartIndex = i;
			EndIndex   = (i+1)%number;
			line = G1_EdgeTable.line + i;
			if ( Polygon[StartIndex][1] > Polygon[EndIndex][1] )
			{
				EndIndex   ^= StartIndex;
				StartIndex ^=  EndIndex;
				EndIndex   ^= StartIndex;
			}

			(*line)[0] = Polygon[StartIndex][0];
			(*line)[1] = Polygon[StartIndex][1];
			(*line)[2] = Polygon[EndIndex][0];
			(*line)[3] = Polygon[EndIndex][1];
		}
	}

	if ( iFALSE == i51KitG1GreateClassET(Polygon) ) return iFALSE;
	
	count = G1_EdgeTable.ClassCount - 1;
	min_coord = G1_EdgeTable.ClassTable[0].coord;
	max_coord = G1_EdgeTable.ClassTable[count].coord;

	//更新活性边，绘制图形
	for (i=min_coord; i<=max_coord; i++)
	{
		G1_EdgeTable.coord = i;

		i51KitG1UpdateActiveET(i);

		i51KitG1DrawActiveET(color, alpha);
	}

	////多边形反走样边
	//count = number - 1;
	//for (i=0; i<count; i++)
	//{
	//	i51KitG1DrawLine(Polygon+i, 3, color, alpha);
	//}

	//i51AdeStdMemcpy16(LineTemp, Polygon+count, sizeof(LineTemp)/2);
	//i51AdeStdMemcpy16(LineTemp+1, Polygon, sizeof(LineTemp)/2);
	//i51KitG1DrawLine(LineTemp, 3, color, alpha);

	return iTRUE;
}

static iBOOL i51KitG1DrawBladeHead(iS16 line[2][2], iS16 width, iS16 length, iALPHA alpha)
{
	mwFillCircle(line[1][0], line[1][1], width, se_FillColor);

	return iTRUE;
}

//iBOOL i51KitG1Draw(void)
//{
//	iS16 i = 0;
//	iS16 count = 0, width = 0;
//	iS16 abs_dx = 0, abs_dy=0;
//	iS16 (*linePtr)[2] = iNULL;
//	iS16 BeginPoint[2][2] = {0};
//	iS16 MidPoint[2][2] = {0};
//	iS16 EndPoint[2][2] = {0};
//	iS16 TmpPoint[2][2] = {0};
//	iS16 lineFirst[2][2] = {0};
//	iS16 lineSecond[2][2] = {0};
//	iS16 PolygonPoint[4][2] = {0};
//	iS32 flag1 = 0, flag2=0, type=0;
//
//	if (se_TrackPointCount <= 1)
//	{
//		return iTRUE;
//	}
//	else if ( 2 == se_TrackPointCount )
//	{
//		i51KitG1GetNormalPoints(BeginPoint, se_TrackPoint, G1_MAXBLADEWIDTH, se_TrackPoint[1][0], se_TrackPoint[1][1]);
//		PolygonPoint[0][0] = se_TrackPoint[0][0];
//		PolygonPoint[0][1] = se_TrackPoint[0][1];
//		PolygonPoint[1][0] = BeginPoint[0][0];
//		PolygonPoint[1][1] = BeginPoint[0][1];
//		PolygonPoint[2][0] = BeginPoint[1][0];
//		PolygonPoint[2][1] = BeginPoint[1][1];
//		PolygonPoint[3][0] = se_TrackPoint[0][0];
//		PolygonPoint[3][1] = se_TrackPoint[0][1];
//
//		abs_dx = se_TrackPoint[1][0] - se_TrackPoint[0][0];
//		abs_dy = se_TrackPoint[1][1] - se_TrackPoint[0][1];
//		abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
//		abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
//		type = abs_dy > abs_dx ? 0 : 1;
//		i51KitG1DrawPolygon(PolygonPoint, 4, type, se_FillColor, se_alpha);
//
//		lineFirst[0][0] = se_TrackPoint[0][0];
//		lineFirst[0][1] = se_TrackPoint[0][1];
//		lineFirst[1][0] = BeginPoint[0][0];
//		lineFirst[1][1] = BeginPoint[0][1];
//		i51KitG1DrawLine(lineFirst, se_BorderWidth, se_BorderColor, se_alpha);
//
//		lineSecond[0][0] = se_TrackPoint[0][0];
//		lineSecond[0][1] = se_TrackPoint[0][1];
//		lineSecond[1][0] = BeginPoint[1][0];
//		lineSecond[1][1] = BeginPoint[1][1];
//		i51KitG1DrawLine(lineSecond, se_BorderWidth, se_BorderColor, se_alpha);
//
//		i51KitG1DrawBladeHead(se_TrackPoint, G1_MAXBLADEWIDTH, G1_MAXKNIFEPOINT, se_alpha);
//	}
//	else
//	{
//		width = 0;
//		count = se_TrackPointCount - 2;
//		i51KitG1GetNormalPoints(BeginPoint, se_TrackPoint, width, se_TrackPoint[0][0], se_TrackPoint[0][1]);
//		for (i=0; i<count; i++)
//		{
//			if (width<G1_MAXBLADEWIDTH) width += 2;
//
//			linePtr  = se_TrackPoint + i;
//			i51AdeStdMemcpy16(lineFirst, linePtr, sizeof(lineFirst));
//
//			i51KitG1GetNormalPoints(MidPoint, lineFirst, width, lineFirst[1][0], lineFirst[1][1]);
//			linePtr  = linePtr + 1;
//			i51AdeStdMemcpy16(lineSecond, linePtr, sizeof(lineSecond));
//			i51KitG1GetNormalPoints(EndPoint, lineSecond, width, lineSecond[0][0], lineSecond[0][1]);
//
//			flag1 = (lineFirst[1][0] - lineFirst[0][0])*(lineFirst[1][1] - lineFirst[0][1]);
//			flag2 = (lineSecond[1][0] - lineSecond[0][0])*(lineSecond[1][1] - lineSecond[0][1]);
//
//			if (flag1 >0 && flag2>0)
//			{
//				TmpPoint[0][0] = (MidPoint[0][0] + EndPoint[0][0])/2;
//				TmpPoint[0][1] = (MidPoint[0][1] + EndPoint[0][1])/2;
//				TmpPoint[1][0] = (MidPoint[1][0] + EndPoint[1][0])/2;
//				TmpPoint[1][1] = (MidPoint[1][1] + EndPoint[1][1])/2;
//			}
//			else if (flag1 >0 && flag2<=0)
//			{
//				TmpPoint[0][0] = (MidPoint[0][0] + EndPoint[1][0])/2;
//				TmpPoint[0][1] = (MidPoint[0][1] + EndPoint[1][1])/2;
//				TmpPoint[1][0] = (MidPoint[1][0] + EndPoint[0][0])/2;
//				TmpPoint[1][1] = (MidPoint[1][1] + EndPoint[0][1])/2;
//			}
//			else if (flag1<=0 && flag2>0)
//			{
//				TmpPoint[0][0] = (MidPoint[0][0] + EndPoint[1][0])/2;
//				TmpPoint[0][1] = (MidPoint[0][1] + EndPoint[1][1])/2;
//				TmpPoint[1][0] = (MidPoint[1][0] + EndPoint[0][0])/2;
//				TmpPoint[1][1] = (MidPoint[1][1] + EndPoint[0][1])/2;
//			}
//			else
//			{
//				TmpPoint[0][0] = (MidPoint[0][0] + EndPoint[0][0])/2;
//				TmpPoint[0][1] = (MidPoint[0][1] + EndPoint[0][1])/2;
//				TmpPoint[1][0] = (MidPoint[1][0] + EndPoint[1][0])/2;
//				TmpPoint[1][1] = (MidPoint[1][1] + EndPoint[1][1])/2;
//			}
//
//			PolygonPoint[0][0] = BeginPoint[0][0];
//			PolygonPoint[0][1] = BeginPoint[0][1];
//			PolygonPoint[1][0] = TmpPoint[0][0];
//			PolygonPoint[1][1] = TmpPoint[0][1];
//			PolygonPoint[2][0] = TmpPoint[1][0];
//			PolygonPoint[2][1] = TmpPoint[1][1];
//			PolygonPoint[3][0] = BeginPoint[1][0];
//			PolygonPoint[3][1] = BeginPoint[1][1];
//
//			abs_dx = lineFirst[1][0] - lineFirst[0][0];
//			abs_dy = lineFirst[1][1] - lineFirst[0][1];
//			abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
//			abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
//			type = abs_dy > abs_dx ? 0 : 1;
//			i51KitG1DrawPolygon(PolygonPoint, 4, type, se_FillColor, se_alpha);
//
//			//边线
//			lineFirst[0][0] = BeginPoint[0][0];
//			lineFirst[0][1] = BeginPoint[0][1];
//			lineFirst[1][0] = TmpPoint[0][0];
//			lineFirst[1][1] = TmpPoint[0][1];
//			i51KitG1DrawLine(lineFirst, se_BorderWidth, se_BorderColor, se_alpha);
//
//			lineSecond[0][0] = BeginPoint[1][0];
//			lineSecond[0][1] = BeginPoint[1][1];
//			lineSecond[1][0] = TmpPoint[1][0];
//			lineSecond[1][1] = TmpPoint[1][1];
//			i51KitG1DrawLine(lineSecond, se_BorderWidth, se_BorderColor, se_alpha);
//
//			if (flag1 >0 && flag2>0)
//			{
//				i51AdeStdMemcpy16(BeginPoint, TmpPoint, sizeof(TmpPoint));
//			}
//			else if (flag1 >0 && flag2<=0)
//			{
//				BeginPoint[0][0] = TmpPoint[1][0];
//				BeginPoint[0][1] = TmpPoint[1][1];
//				BeginPoint[1][0] = TmpPoint[0][0];
//				BeginPoint[1][1] = TmpPoint[0][1];
//			}
//			else if (flag1<=0 && flag2>0)
//			{
//				BeginPoint[0][0] = TmpPoint[1][0];
//				BeginPoint[0][1] = TmpPoint[1][1];
//				BeginPoint[1][0] = TmpPoint[0][0];
//				BeginPoint[1][1] = TmpPoint[0][1];
//			}
//			else
//			{
//				i51AdeStdMemcpy16(BeginPoint, TmpPoint, sizeof(TmpPoint));
//			}
//		}
//
//		linePtr  = se_TrackPoint + count;
//		i51AdeStdMemcpy16(lineFirst, linePtr, sizeof(lineFirst));
//		i51KitG1GetNormalPoints(EndPoint, lineFirst, width, lineFirst[1][0], lineFirst[1][1]);
//		PolygonPoint[0][0] = BeginPoint[0][0];
//		PolygonPoint[0][1] = BeginPoint[0][1];
//		PolygonPoint[1][0] = EndPoint[0][0];
//		PolygonPoint[1][1] = EndPoint[0][1];
//		PolygonPoint[2][0] = EndPoint[1][0];
//		PolygonPoint[2][1] = EndPoint[1][1];
//		PolygonPoint[3][0] = BeginPoint[1][0];
//		PolygonPoint[3][1] = BeginPoint[1][1];
//
//		abs_dx = lineFirst[1][0] - lineFirst[0][0];
//		abs_dy = lineFirst[1][1] - lineFirst[0][1];
//		abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
//		abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
//		type = abs_dy > abs_dx ? 0 : 1;
//		i51KitG1DrawPolygon(PolygonPoint, 4, type, se_FillColor, se_alpha);
//
//
//		//边线
//		lineFirst[0][0] = BeginPoint[0][0];
//		lineFirst[0][1] = BeginPoint[0][1];
//		lineFirst[1][0] = EndPoint[0][0];
//		lineFirst[1][1] = EndPoint[0][1];
//		i51KitG1DrawLine(lineFirst, se_BorderWidth, se_BorderColor, se_alpha);
//
//		lineSecond[0][0] = BeginPoint[1][0];
//		lineSecond[0][1] = BeginPoint[1][1];
//		lineSecond[1][0] = EndPoint[1][0];
//		lineSecond[1][1] = EndPoint[1][1];
//		i51KitG1DrawLine(lineSecond, se_BorderWidth, se_BorderColor, se_alpha);
//
//		linePtr  = se_TrackPoint + se_TrackPointCount - 2;
//		i51AdeStdMemcpy16(lineFirst, linePtr, sizeof(lineFirst));
//		i51KitG1DrawBladeHead(lineFirst, width, G1_MAXKNIFEPOINT, se_alpha);
//	}
//
//	if (se_TrackPointCount >= se_BaseCount)
//	{
//		se_BaseCount--;
//		i51KitG1TrackQueueOut();
//	}
//
//	return iTRUE;
//}

//获取两直线交点的对称点
static iBOOL i51KitG1GetSymmetricalPoints(iS16 PointOut[2], iS16 PointIn[2], iS16 Line1[2][2], iS16 Line2[2][2], iS16 ReferPoint[2],  iS16 width)
{
	iS16 first_dx=0, first_dy=0;
	iS16 second_dx=0, second_dy=0;
	iS16 Third_dx = 0, Third_dy=0;
	iS16 dx=0, dy=0; //两直线对角线方向向量
	iS16 move_dx=0, move_dy=0; //两直线对角线方向向量
	iS32 dis = 0, dValue=0;
	iS32 flag1 = 0, flag2=0;
	float fValue = 0;
	iS16 pointTemp[2]={0};

	first_dx = Line1[1][0] -  Line1[0][0];
	first_dy = Line1[1][1] -  Line1[0][1];
	second_dx = Line2[1][0] -  Line2[0][0];
	second_dy = Line2[1][1] -  Line2[0][1];

	flag1 = first_dx*first_dx + first_dy*first_dy;
	flag2 = second_dx*second_dx + second_dy*second_dy;
	if (flag1 > flag2)
	{
		fValue = (float)flag1/(float)flag2;
		fValue =  i51KitG1Sqrt(fValue);
		second_dx = (iS16)(second_dx*fValue + 0.5);
		second_dy = (iS16)(second_dy*fValue + 0.5);
	}
	else
	{
		fValue = (float)flag2/(float)flag1;
		fValue =  i51KitG1Sqrt(fValue);
		first_dx = (iS16)(first_dx*fValue + 0.5);
		first_dy = (iS16)(first_dy*fValue + 0.5);
	}

	Third_dx = first_dx + second_dx;
	Third_dy = first_dy + second_dy;
	dx = -Third_dy;
	dy = Third_dx;

	if (width < 0) return iFALSE;

	if ( 0 == width)
	{
		PointOut[0] = Line1[1][0];
		PointOut[1] = Line1[1][1];
		PointIn[0] = Line1[1][0];
		PointIn[1] = Line1[1][1];
		return iTRUE;
	}

	if ( 0 == dx && 0 == dy)
	{
		dx = -first_dy;
		dy = first_dx;
	}

	dis    = dx*dx + dy*dy;
	dValue = dx*dx*width*width;
	fValue = (float)dValue/(float)dis;
	fValue =  i51KitG1Sqrt(fValue);
	move_dx = (iS16)(fValue + 0.5);
	dValue = dy*dy*width*width;
	fValue = (float)dValue/(float)dis;
	fValue =  i51KitG1Sqrt(fValue);
	move_dy = (iS16)(fValue + 0.5);

	move_dx = dx>0 ? move_dx : -move_dx;
	move_dy = dy>0 ? move_dy : -move_dy;

	PointOut[0] = Line1[1][0] + move_dx;
	PointOut[1] = Line1[1][1] + move_dy;
	PointIn[0] = Line1[1][0] - move_dx;
	PointIn[1] = Line1[1][1] - move_dy;

	//保证同一侧的点
	//F(x) = dx*y-dy*x - (dx*y0 - dy*x0)
	dValue = first_dx*Line1[0][1] - first_dy*Line1[0][0];
	flag1 = first_dx*ReferPoint[1] - first_dy*ReferPoint[0] - dValue;
	flag2 = first_dx*PointOut[1] - first_dy*PointOut[0] - dValue;
	if ( flag1*flag2 < 0 )
	{
		//交换点
		pointTemp[0] = PointIn[0];
		pointTemp[1] = PointIn[1];
		PointIn[0] = PointOut[0];
		PointIn[1] = PointOut[1];
		PointOut[0] = pointTemp[0];
		PointOut[1] = pointTemp[1];		
	}

	return iTRUE;
}

static  iBOOL i51KitG1GetAlphaFillType(iS16 line1[2][2], iS16 line2[2][2], iS16 *out_type)
{
	iS16 CenterX = 0, CenterY=0;
	iS16 dx = 0, dy=0;
	iS16 abs_dx = 0, abs_dy=0;
	iS32 flag = 0, factor=0, slope=0;

	CenterX = (line1[0][0] + line1[1][0] + line2[0][0] + line2[1][0])/4;
	CenterY = (line1[0][1] + line1[1][1] + line2[0][1] + line2[1][1])/4;

	
	dx = line1[1][0] - line1[0][0];
	dy = line1[1][1] - line1[0][1];
	abs_dx = dx>0 ? dx : -dx;
	abs_dy = dy>0 ? dy : -dy;
	slope = dx*dy;

	//F(x) = dx*y-dy*x - (dx*y0 - dy*x0)
	factor = dx*line1[0][1] - dy*line1[0][0];
	flag = dx*CenterY - dy*CenterX - factor;

	if ( 0 == dx )
	{
		*out_type = CenterX > line1[0][0] ? 2 : 3;
		return iTRUE;
	}

	if ( 0 == dy )
	{
		*out_type = CenterY > line1[0][1] ? 0 : 1;
		return iTRUE;
	}

	flag = dx < 0 ? -flag : flag;
	
	if (abs_dx>abs_dy)
	{
		*out_type = flag > 0 ? 0 : 1;
	}
	else
	{
		if (slope > 0)
		{
			*out_type = flag > 0 ? 3 : 2;
		}
		else
		{
			*out_type = flag > 0 ? 2 : 3;
		}
	}

	return iTRUE;
}

iBOOL i51KitG1Draw()
{

	iS16 i=0, count=0;
	iS16 width = 0, type=0;
	iS16 abs_dx = 0, abs_dy=0;
	iS16 (*point)[2] = iNULL;
	iS16 (*line)[2] = iNULL;
	iS16 PolygonPoint[4][2] = {0};

	//debug
	//{
	//	se_TrackPointCount = 5;
	//	se_TrackPoint[0][0] = 10;
	//	se_TrackPoint[0][1] = 10;
	//	se_TrackPoint[1][0] = 230;
	//	se_TrackPoint[1][1] = 100;
	//	se_TrackPoint[2][0] = 50;
	//	se_TrackPoint[2][1] = 180;
	//	se_TrackPoint[3][0] = 200;
	//	se_TrackPoint[3][1] = 250;
	//	se_TrackPoint[4][0] = 60;
	//	se_TrackPoint[4][1] = 300;
	//}

	if (se_TrackPointCount>=2)
	{
		count = se_TrackPointCount-2;
		PolygonPoint[0][0] = se_TrackPoint[0][0];
		PolygonPoint[0][1] = se_TrackPoint[0][1];
		PolygonPoint[3][0] = se_TrackPoint[0][0];
		PolygonPoint[3][1] = se_TrackPoint[0][1];
		point = PolygonPoint;
		line = se_TrackPoint;

		if (2==se_TrackPointCount) width = G1_MAXBLADEWIDTH;

		for (i=0; i<count; i++)
		{
			width = width < G1_MAXBLADEWIDTH ? (width+2) : width;
			i51KitG1GetSymmetricalPoints(*(point+1), *(point+2), line+i, line+i+1, (*point),  width);

			abs_dx = se_TrackPoint[i+1][0] - se_TrackPoint[i][0];
			abs_dy = se_TrackPoint[i+1][1] - se_TrackPoint[i][1];
			abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
			abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
			type = abs_dy > abs_dx ? 0 : 1;
			i51KitG1DrawPolygon(PolygonPoint, 4, type, se_FillColor, se_alpha);

			//i51KitG1DrawLine(point, se_BorderWidth, se_BorderColor, se_alpha);
			//i51KitG1DrawLine(point+2, se_BorderWidth, se_BorderColor, se_alpha);

			i51KitG1GetAlphaFillType(point, point+2, &type);
//			i51KitG1DrawLineEx(point,  type, se_BorderWidth, se_BorderColor);
			i51KitG1GetAlphaFillType(point+2, point, &type);
//			i51KitG1DrawLineEx(point+2, type, se_BorderWidth, se_BorderColor);


			PolygonPoint[0][0] = PolygonPoint[1][0];
			PolygonPoint[0][1] = PolygonPoint[1][1];
			PolygonPoint[3][0] = PolygonPoint[2][0];
			PolygonPoint[3][1] = PolygonPoint[2][1];
		}

		i51KitG1GetSymmetricalPoints(*(point+1), *(point+2), line+count, line+count, (*point),  width);
		abs_dx = se_TrackPoint[i+1][0] - se_TrackPoint[i][0];
		abs_dy = se_TrackPoint[i+1][1] - se_TrackPoint[i][1];
		abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
		abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
		type = abs_dy > abs_dx ? 0 : 1;
		i51KitG1DrawPolygon(PolygonPoint, 4, type, se_FillColor, se_alpha);

		//i51KitG1DrawLine(point, se_BorderWidth, se_BorderColor, se_alpha);
		//i51KitG1DrawLine(point+2, se_BorderWidth, se_BorderColor, se_alpha);
		i51KitG1GetAlphaFillType(point, point+2, &type);
//		i51KitG1DrawLineEx(point,  type, se_BorderWidth, se_BorderColor);
		i51KitG1GetAlphaFillType(point+2, point, &type);
//		i51KitG1DrawLineEx(point+2, type, se_BorderWidth, se_BorderColor);

		i51KitG1DrawBladeHead(se_TrackPoint+count, width, G1_MAXKNIFEPOINT, se_alpha);
	}


	return iTRUE;

}
//以上是新绘制函数
//////////////////////////////////////////////////////////////////////////


