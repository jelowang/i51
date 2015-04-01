#include "i51KitAde.h"
#include "i51KitStd.h"
#include "i51KitG2.h"
#include "i51KitG1.h"

#define  G1_TEST_DEMO		0
#define  G1_BLADE_DEMO		0
#define  G1_PARTICLE_DEMO	0

#if G1_PARTICLE_DEMO
#include "i51KitG1Res.h"
#endif

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);

/*
********************************************************************************
*                         刀锋 常量或结构定义
********************************************************************************
*/
#define G1_MAXBLADEWIDTH	4		//刀锋最大宽度(一半)
#define G1_MAXKNIFEPOINT    12		//刀尖长
#define G1_MAXTRACKPOINT	8		//轨迹点数组最大个数
#define G1_MINDISTANCE		100		//轨迹点间最小距离的平方
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

/*
********************************************************************************
*                         粒子系统 常量或结构定义
********************************************************************************
*/
////粒子特效类型
//typedef enum enumPARTICLE_TYPE
//{
//	NINJA_JUMP = 1, //忍者跳跃
//	FINISH_JOY = 2, //捕鱼达人
//	NIN_JUMP   = 3, //忍着跳跃
//}G1_PARTICLE_TYPE;

#define  G1_DEFAULT_RADIUS			40  //粒子特效默认半径 单位像素
#define  G1_DEFAULT_GRAVITYACCEL	2   //重力加速度
#define  G1_DEFAULT_SPEEDVARIANCE	4   //粒子速度浮动值 0.5像素/每帧
#define  G1_DEFAULT_SPEED			10  //粒子速度		 0.5像素/每帧
#define  G1_DEFAULT_SPEED_UNIT		2	//
#define  G1_DEFAULT_ANGLE			90	//角度
#define  G1_DEFAULT_ANGLEVARIANCE	15  //角度浮动范围
#define  G1_DEFAULT_LIFETIME		10  //生命周期
#define  G1_DEFAULT_DECAYTIME		1   //衰减速度
#define  G1_DEFAULT_SPEEDFACTOR		1024 //速度增大倍数
#define  G1_DEFAULT_TRIANGLEFACTOR	1024 //三角函数增大倍数

//粒子结构
typedef struct tagPARTICLE_NODE
{
	iS16 IsValid;		 //粒子是否有效 0==否，1==是
	iS16 SizeId;		 //尺寸
	iS16 life;			 //生命值
	iS16 Decay;			 //衰减速度
	iS32  PosX, PosY;	 //坐标
	iS32 SpeedX, SpeedY; //速度
	iS32 AccelX, AccelY; //加速度
}G1_PARTICLE_NODE;

//粒子系统
typedef struct tagPARTICLE_SYSTEM
{
	iS16 IsActive;		       //是否激活 0==没有激活 1==激活
	iS16 radius;			   //半径
	iS16 MaxLifeTime;		   //粒子最大的生命值
	iS16 decay;				   //衰减速度
	iS16 StartX, StartY;       //发射点坐标
	iS16 AccelX, AccelY;	   //加速度
	iS16 speed;				   //速度
	iS16 SpeedVariance;		   //速度浮动值
	iS16 angle;				   //角度
	iS16 AngleVariance;		   //角度浮动值
	iS16 MaxParticleCount;     //粒子最大个数
	iS16 CurParticleCount;	   //当前粒子个数
	iU32 DurationTime;		   //持续时间
	iU32 ElapseTime;		   //流逝时间
	iHANDLE ImageHander;	   //图片容器句柄
	iU32	ImageNum;		   //图片数
	iU32	ImageId;		   //图片id
	G1_PARTICLE_NODE *NodeHeader; //粒子链表头指针
	G1_PARTICLE_MODE mode;	   //模式
}G1_PARTICLE_SYSTEM;

/*
********************************************************************************
*                         刀锋模块局部变量
********************************************************************************
*/
static G1_EDGE_TABLE  G1_EdgeTable = {0};
static iU16 *G1_ScreenBuf = iNULL;
static iU16 G1_ScreenWidth=0, G1_ScreenHeight=0;
static iCOLOR G1_BorderWidth = 3;
static iS16 G1_BaseCount = 0;
static iS16 G1_TrackPointCount = 0;
static iS16 G1_TrackPoint[G1_MAXTRACKPOINT][2] = {0};
static iALPHA G1_alpha = 0xFF;
static iCOLOR G1_FillColor = 0xFFFF, G1_BorderColor = 0xDDDD;

static iS16 G1_RepeatCount = 0;
static iS16 G1_ReferPoint[2] = {0};

/*
********************************************************************************
*                         粒子系统模块局部变量
********************************************************************************
*/
//正弦值扩大1024倍
static iS16 G1_SIN[91]={
	0, 18, 36, 54, 71, 89, 107, 125, 143, 160,
	178, 195, 213, 230, 248, 265, 282, 299, 316, 333,
	350, 367, 384, 400, 416, 433, 449, 465, 481, 496,
	512, 527, 543, 558, 573, 587, 602, 616, 630, 644,
	658, 672, 685, 698, 711, 724, 737, 749, 761, 773,
	784, 796, 807, 818, 828, 839, 849, 859, 868, 878,
	887, 896, 904, 912, 920, 928, 935, 943, 949, 956,
	962, 968, 974, 979, 984, 989, 994, 998, 1002, 1005,
	1008, 1011, 1014, 1016, 1018, 1020, 1022, 1023, 1023, 1024,
	1024
};

static iS16 i51KitG1Sin(iS16 angle)
{
	while (angle < 0) angle += 360;
	while (angle >= 360) angle -= 360;
	
	if (angle > 90 && angle <= 180){
		return G1_SIN[180-angle];
	}
	else if (angle > 180 && angle <=270){
		return -G1_SIN[angle-180];
	}
	else if (angle > 270 && angle <360){
		return -G1_SIN[360-angle];
	}

	return G1_SIN[angle];
}

static iS16 i51KitG1Cos(iS16 angle)
{
	return i51KitG1Sin(90 + angle);
}

/*
********************************************************************************
*                         轨迹函数部分
********************************************************************************
*/
static iBOOL i51KitG1TrackQueueEmpty()
{
	G1_TrackPointCount = 0;
	i51AdeStdMemset16(G1_TrackPoint, 0, sizeof(G1_TrackPoint));
	return iTRUE;
}

static iBOOL i51KitG1InsertPoint()
{
	//不够G1_MAXPOINTNUM 就插值
	if ( 2 == G1_TrackPointCount){
		G1_TrackPoint[4][0] = G1_TrackPoint[1][0];
		G1_TrackPoint[4][1] = G1_TrackPoint[1][1];

		G1_TrackPoint[2][0] = (G1_TrackPoint[0][0]+G1_TrackPoint[4][0])/2;
		G1_TrackPoint[2][1] = (G1_TrackPoint[0][1] + G1_TrackPoint[4][1])/2;

		G1_TrackPoint[1][0] = (G1_TrackPoint[0][0]+G1_TrackPoint[2][0])/2;
		G1_TrackPoint[1][1] = (G1_TrackPoint[0][1] + G1_TrackPoint[2][1])/2;

		G1_TrackPoint[3][0] = (G1_TrackPoint[2][0]+G1_TrackPoint[4][0])/2;
		G1_TrackPoint[3][1] = (G1_TrackPoint[2][1] + G1_TrackPoint[4][1])/2;

		G1_TrackPointCount = G1_MAXPOINTNUM;
	}else if ( 3 == G1_TrackPointCount ){
		G1_TrackPoint[4][0] = G1_TrackPoint[2][0];
		G1_TrackPoint[4][1] = G1_TrackPoint[2][1];

		G1_TrackPoint[2][0] = G1_TrackPoint[1][0];
		G1_TrackPoint[2][1] = G1_TrackPoint[1][1];

		G1_TrackPoint[1][0] = (G1_TrackPoint[0][0]+G1_TrackPoint[2][0])/2;
		G1_TrackPoint[1][1] = (G1_TrackPoint[0][1] + G1_TrackPoint[2][1])/2;

		G1_TrackPoint[3][0] = (G1_TrackPoint[2][0]+G1_TrackPoint[4][0])/2;
		G1_TrackPoint[3][1] = (G1_TrackPoint[2][1] + G1_TrackPoint[4][1])/2;

		G1_TrackPointCount = G1_MAXPOINTNUM;
	}else if ( 4 == G1_TrackPointCount ){
		G1_TrackPoint[4][0] = G1_TrackPoint[3][0];
		G1_TrackPoint[4][1] = G1_TrackPoint[3][1];

		G1_TrackPoint[3][0] = G1_TrackPoint[2][0];
		G1_TrackPoint[3][1] = G1_TrackPoint[2][1];

		G1_TrackPoint[2][0] = G1_TrackPoint[1][0];
		G1_TrackPoint[2][1] = G1_TrackPoint[1][1];

		G1_TrackPoint[1][0] = (G1_TrackPoint[0][0]+G1_TrackPoint[2][0])/2;
		G1_TrackPoint[1][1] = (G1_TrackPoint[0][1] + G1_TrackPoint[2][1])/2;

		G1_TrackPointCount = G1_MAXPOINTNUM;
	}

	return iTRUE;
}

static iBOOL i51KitG1TrackQueueIn(iS16 x, iS16 y, iS32 type)
{
	iS16 (*linePtr)[2] = iNULL;
	iS32 dis = 0, dx =0, dy=0;
	iBOOL iFlag = iTRUE;

	dx = x - G1_TrackPoint[G1_TrackPointCount-1][0];
	dy = y - G1_TrackPoint[G1_TrackPointCount-1][1];
	dis = dx*dx + dy*dy;

	if ( i51_MSG_PADDOWN == type)
	{
		i51KitG1TrackQueueEmpty();
	}

	//插入点
	if ( i51_MSG_PADDOWN == type || i51_MSG_PADMOVE == type || i51_MSG_PADUP == type )
	{
		if (dis >= G1_MINDISTANCE)
		{
			if (G1_TrackPointCount >= G1_MAXTRACKPOINT)
			{
				linePtr = G1_TrackPoint + 1;
				i51AdeStdMemcpy16((void*)G1_TrackPoint, (void*)linePtr, sizeof(iS16)*2*(G1_MAXTRACKPOINT-1));
				G1_TrackPoint[G1_MAXTRACKPOINT-1][0] = x;
				G1_TrackPoint[G1_MAXTRACKPOINT-1][1] = y;
				G1_TrackPointCount = G1_MAXTRACKPOINT;
			}
			else
			{
				G1_TrackPoint[G1_TrackPointCount][0] = x;
				G1_TrackPoint[G1_TrackPointCount][1] = y;
				G1_TrackPointCount++;
			}
			

			G1_BaseCount  = G1_TrackPointCount+1;
			G1_BaseCount  = G1_BaseCount > G1_MAXPOINTNUM ? G1_MAXPOINTNUM : G1_BaseCount;

			//G1_TouchTime = i51AdeOsGetTick();

			iFlag = iTRUE;
		}else{
			iFlag = iFALSE;
		}
	}

	if ( i51_MSG_PADUP  == type)
	{	
		//插值
		i51KitG1InsertPoint();

		G1_BaseCount = G1_MAXPOINTNUM;
	}

	return iFlag;
}

static iBOOL i51KitG1TrackQueueOut()
{
	iS16 (*linePtr)[2] = iNULL;

	if (G1_TrackPointCount >= 2)
	{
		linePtr = G1_TrackPoint + 1;
		G1_TrackPointCount -= 1;
		i51AdeStdMemcpy16((void*)G1_TrackPoint, (void*)linePtr, sizeof(iS16)*2*G1_TrackPointCount);
	}
	else
	{
		i51KitG1TrackQueueEmpty();
	}

	return iTRUE;
}


/*
********************************************************************************
*                         刀锋特效函数部分 
********************************************************************************
*/
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

static iBOOL i51KitG1GetNormalPoints(iS16 NormalPoint[2][2], iS16 LinePoint[2][2], iS16 width, iS16 CentreX, iS16 CentreY)
{
	//  author : yepan
	//  since  : 2012-7-23
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			NormalPoint  输出的两点
	//			LinePoint    直线
	//			width		 距离
	//			CentreX,CentreY 对称中心点
	//  return :
	//			iTRUE==成功， iFALSE==失败
	//  note   :
	//			求到线段LinePoint上（CentreX, CentreY)点距离为Width的两点
	

	iS16 dx=0, dy=0, tmp_var=0;
	iS16 abs_dx=0, abs_dy=0;
	iS16 move_x=0, move_y=0;
	iS32 fun_var = 0;

	dx= LinePoint[1][0] -  LinePoint[0][0];
	dy= LinePoint[1][1] -  LinePoint[0][1];

	if ( 0 == width || (0==dx &&0==dy)){
		NormalPoint[0][0] = CentreX;
		NormalPoint[0][1] = CentreY;
		NormalPoint[1][0] = CentreX;
		NormalPoint[1][1] = CentreY;
		return iTRUE;
	}

	//垂直x
	if ( 0 == dx ){
		NormalPoint[0][0] = CentreX - width;
		NormalPoint[0][1] = CentreY;
		NormalPoint[1][0] = CentreX + width;
		NormalPoint[1][1] = CentreY;
		return iTRUE;
	}

	//垂直y
	if ( 0 == dy ){
		NormalPoint[0][0] = CentreX;
		NormalPoint[0][1] = CentreY - width;
		NormalPoint[1][0] = CentreX;
		NormalPoint[1][1] = CentreY + width;
		return iTRUE;
	}

	tmp_var = dx;
	dx = -dy;
	dy = tmp_var;
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;

	if ( abs_dx > abs_dy ){

		if (dx < 0){
			dx = -dx;
			dy = -dy;
		}

		move_x	= width;
		fun_var = dy*move_x;

		if (fun_var > 0){
			move_y = (fun_var%dx > dx/2) ? (fun_var/dx + 1) : (fun_var/dx);
		}else{
			move_y = (fun_var%dx < -dx/2) ? (fun_var/dx - 1) : (fun_var/dx);
		}

		NormalPoint[0][0] = CentreX + move_x;
		NormalPoint[0][1] = CentreY + move_y;
		NormalPoint[1][0] = CentreX - move_x;
		NormalPoint[1][1] = CentreY - move_y;

	}else{

		if (dy < 0){
			dy = -dy;
			dx = -dx;
		}

		move_y	= width;
		fun_var = dx*move_y;

		if (fun_var > 0){
			move_x = (fun_var%dy > dy/2) ? (fun_var/dy + 1) : (fun_var/dy);
		}else{
			move_x = (fun_var%dy < -dy/2) ? (fun_var/dy - 1) : (fun_var/dy);
		}

		NormalPoint[0][0] = CentreX + move_x;
		NormalPoint[0][1] = CentreY + move_y;
		NormalPoint[1][0] = CentreX - move_x;
		NormalPoint[1][1] = CentreY - move_y;
	}

	return iTRUE;
}

static iBOOL i51KitG1GetForwardPoints(iS16 ForwardPoint[2], iS16 LinePoint[2][2], iS16 length)
{
//  author : yepan
//  since  : 2012-7-23
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			ForwardPoint 输出的点
//			LinePoint	 线段
//			length		 长度
//  return :
//			iTRUE==成功， iFALSE==失败			
//  note   :
//			获取直线方向上距离终点length的点

	iS16 dx=0, dy=0;
	iS16 abs_dx=0, abs_dy=0;
	iS16 move_x=0, move_y=0;
	iS32 fun_var = 0;

	dx= LinePoint[1][0] -  LinePoint[0][0];
	dy= LinePoint[1][1] -  LinePoint[0][1];
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;

	if ( 0 == length || (0==dx &&0==dy)){
		ForwardPoint[0] = LinePoint[1][0];
		ForwardPoint[1] = LinePoint[1][1];
		return iTRUE;
	}

	//垂直x
	if ( 0 == dx ){
		ForwardPoint[0] = LinePoint[1][0];

		if (dy > 0){
			ForwardPoint[1] = LinePoint[1][1] + length;
		}else{
			ForwardPoint[1] = LinePoint[1][1] - length;
		}
		return iTRUE;
	}

	//垂直y
	if ( 0 == dy ){
		if (dx>0){
			ForwardPoint[0] = LinePoint[1][0] + length;
		}else{
			ForwardPoint[0] = LinePoint[1][0] - length;
		}

		ForwardPoint[1] = LinePoint[1][1];
		return iTRUE;
	}

	if ( abs_dx > abs_dy ){

		move_x = dx > 0 ? length : -length;
		fun_var = dy*move_x;
		
		fun_var = dx > 0 ? fun_var : -fun_var;

		if (fun_var > 0){
			move_y = (fun_var%abs_dx > abs_dx/2) ? (fun_var/abs_dx + 1) : (fun_var/abs_dx);
		}else{
			move_y = (fun_var%abs_dx < -abs_dx/2) ? (fun_var/abs_dx - 1) : (fun_var/abs_dx);
		}

	}else{

		move_y = dy > 0 ? length : -length;
		fun_var = dx*move_y;

		fun_var = dy > 0 ? fun_var : -fun_var;

		if (fun_var > 0){
			move_x = (fun_var%abs_dy > abs_dy/2) ? (fun_var/abs_dy + 1) : (fun_var/abs_dy);
		}else{
			move_x = (fun_var%abs_dy < -abs_dy/2) ? (fun_var/abs_dy - 1) : (fun_var/abs_dy);
		}
	}

	ForwardPoint[0] = LinePoint[1][0] + move_x;
	ForwardPoint[1] = LinePoint[1][1] + move_y;

	return iTRUE;
}

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
//			绘制多线宽反走样直线, 带整体alpha

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
		if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
		{
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
		}
		
		return iTRUE;
	}
	else if ( 0 == abs_dx ) //垂直于x轴
	{
		StartX = line[0][0] - width/2;
		StartY = line[0][1];
		unit_dy = dy/abs_dy;
		dWidth = unit_dy>0 ? G1_ScreenWidth : -G1_ScreenWidth;
		width--;
		for (i=0; i<width; i++)
		{
			TempX = StartX+i;
			TempY = StartY;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
				}

				TempY += unit_dy;
				DesPix +=  dWidth;

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
		width--;
		for (i=0; i<width; i++)
		{
			TempX = StartX;
			TempY = StartY + i;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
				}

				TempX += unit_dx;
				DesPix +=  dWidth;
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
			if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
			{
				DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
				*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
			}

			AlphaValue = alpha - AlphaValue;
			TempY = StartY + width-1;
			if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
			{
				DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
				*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
			}

			AlphaValue = alpha;
			TempY = StartY + 1;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
				}
				TempY++;
				DesPix += G1_ScreenWidth;
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
			if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
			{
				DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
				*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
			}

			AlphaValue = alpha - AlphaValue;
			TempX = StartX + width-1;
			if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
			{
				DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
				*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
			}

			AlphaValue = alpha;
			TempX = StartX+1;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
				}

				TempX++;
				DesPix += 1;
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
		if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
		{
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			*DesPix = color;
		}

		return iTRUE;
	}
	else if ( 0 == abs_dx ) //垂直于x轴
	{
		StartX = line[0][0] - width/2;
		StartY = line[0][1];
		unit_dy = dy/abs_dy;
		dWidth = unit_dy>0 ? G1_ScreenWidth : -G1_ScreenWidth;

		
		if (2 == type)width--; //左
		if (3 == type) i = 1; //右
		for (i; i<width; i++)
		{
			TempX = StartX+i;
			TempY = StartY;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					//*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
					*DesPix = color;
				}

				TempY += unit_dy;
				DesPix +=  dWidth;

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
		
		if (0 == type) width--;
		if (1 == type) i = 1;
		for (i; i<width; i++)
		{
			TempX = StartX;
			TempY = StartY + i;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					//*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
					*DesPix = color;
				}

				TempX += unit_dx;
				DesPix +=  dWidth;

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
			AlphaValue = 0xFF*dis/abs_dx;
			StartY = BaseY - width/2;

			TempX = BaseX;
			if ( 0 == type) //下
			{
				TempY = StartY;
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
					*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
				}
			}

			if (1 == type) //上
			{
				AlphaValue = 0xFF - AlphaValue;
				TempY = StartY + width-1;
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
					*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
				}
			}

			TempY = StartY + 1;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					*DesPix = color;
				}
				TempY++;
				DesPix += G1_ScreenWidth;
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
			AlphaValue = 0xFF*dis/abs_dy;
			StartX = BaseX - width/2;

			TempY = BaseY;
			if ( 2 == type)//左
			{
				TempX = StartX;
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
					*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
				}
			}

			if ( 3 == type ) //右
			{
				AlphaValue = 0xFF - AlphaValue;
				TempX = StartX + width-1;
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
					*DesPix = i51KitG1GetNewColor((*DesPix), color, AlphaValue);
				}
			}

			TempX = StartX+1;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					*DesPix = color;
				}

				TempX++;
				DesPix += 1;
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

			StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor;
			EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor + 1;

			TempX = FixCoord;
			TempY = StartCoord;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (i=StartCoord; i<=EndCoord; i++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					//*DesPix = i51KitG1GetNewColor((*DesPix), color, alpha);
					*DesPix = color;
				}
				TempY++;
				DesPix +=  G1_ScreenWidth;
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

			StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor;
			EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor + 1;

			TempY = FixCoord;
			TempX = StartCoord;
			DesPix = G1_ScreenBuf + TempY*G1_ScreenWidth + TempX;
			for (i=StartCoord; i<=EndCoord; i++)
			{
				if (TempX>=0 && TempX<G1_ScreenWidth && TempY>=0 && TempY<G1_ScreenHeight)
				{
					//*DesPix = i51KitG1GetNewColor((*DesPix), color, alpha);
					*DesPix = color;
				}
				TempX++;
				DesPix += 1;
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
	//iS16 LineTemp[2][2] = {0};

	if ( iNULL == Polygon || number <= 0 ||  number > G1_MAXTOPPOINT ) return iFALSE;

	if ( number <= 2)
	{
		//画点或线
		i51KitG1DrawLine(Polygon, 2, color, alpha);
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
	iS16 NormalPoint[2][2] = {0};
	iS16 linePoint[2][2] = {0};
	iS16 TopPoint[2] = {0};
	iS16 PolygonPoint[4][2] = {0};

	i51KitG1GetNormalPoints(NormalPoint, line, width, line[1][0], line[1][1]);

	i51KitG1GetForwardPoints(TopPoint, line, length);

	PolygonPoint[0][0] = TopPoint[0];
	PolygonPoint[0][1] = TopPoint[1];
	PolygonPoint[1][0] = NormalPoint[0][0];
	PolygonPoint[1][1] = NormalPoint[0][1];
	PolygonPoint[2][0] = NormalPoint[1][0];
	PolygonPoint[2][1] = NormalPoint[1][1];
	PolygonPoint[3][0] = TopPoint[0];
	PolygonPoint[3][1] = TopPoint[1];
	i51KitG1DrawPolygon(PolygonPoint, 4, 0, G1_FillColor,  alpha);

	linePoint[0][0] = TopPoint[0];
	linePoint[0][1] = TopPoint[1];
	linePoint[1][0] = NormalPoint[0][0];
	linePoint[1][1] = NormalPoint[0][1];
	i51KitG1DrawLine(linePoint, G1_BorderWidth, G1_BorderColor, alpha);

	linePoint[1][0] = NormalPoint[1][0];
	linePoint[1][1] = NormalPoint[1][1];
	i51KitG1DrawLine(linePoint, G1_BorderWidth, G1_BorderColor, alpha);

	return iTRUE;
}

//获取两直线交点的对称点
static iBOOL i51KitG1GetSymmetricalPoints(iS16 PointOut[2], iS16 PointIn[2], iS16 Line1[2][2], iS16 Line2[2][2], iS16 ReferPoint[2],  iS16 width)
{
	iS16 first_dx=0, first_dy=0;
	iS16 second_dx=0, second_dy=0;
	iS16 Third_dx = 0, Third_dy=0;
	iS16 ForwardPoint[2] = {0};
	iS16 NormalPoint[2][2]={0};
	iS16 line[2][2]={0};
	iS32 dValue = 0, flag1=0, flag2=0;
	iS16 pointTemp[2] = {0};

	first_dx = Line1[1][0] -  Line1[0][0];
	first_dy = Line1[1][1] -  Line1[0][1];
	second_dx = Line2[1][0] -  Line2[0][0];
	second_dy = Line2[1][1] -  Line2[0][1];

	flag1 = first_dx*first_dx + first_dy*first_dy;
	flag2 = second_dx*second_dx + second_dy*second_dy;

	if ( 0 == flag1)
	{
		i51KitG1GetNormalPoints(NormalPoint, Line2, width, Line2[0][0], Line2[0][1]);
	}else if ( 0 == flag2)
	{
		i51KitG1GetNormalPoints(NormalPoint, Line1, width, Line1[1][0], Line1[1][1]);
	}else{
		NormalPoint[0][0] = 0;
		NormalPoint[0][1] = 0;
		NormalPoint[1][0] = first_dx;
		NormalPoint[1][1] = first_dy;
		i51KitG1GetForwardPoints(ForwardPoint, NormalPoint, 50);
		Third_dx = ForwardPoint[0] - NormalPoint[1][0];
		Third_dy = ForwardPoint[1] - NormalPoint[1][1];

		NormalPoint[0][0] = 0;
		NormalPoint[0][1] = 0;
		NormalPoint[1][0] = second_dx;
		NormalPoint[1][1] = second_dy;
		i51KitG1GetForwardPoints(ForwardPoint, NormalPoint, 50);
		Third_dx += (ForwardPoint[0] - NormalPoint[1][0]);
		Third_dy += (ForwardPoint[1] - NormalPoint[1][1]);

		line[0][0] = 0;
		line[0][1] = 0;
		line[1][0] = Third_dx;
		line[1][1] = Third_dy;
		i51KitG1GetNormalPoints(NormalPoint, line, width, 0, 0);

		NormalPoint[0][0] = NormalPoint[0][0] + Line1[1][0];
		NormalPoint[0][1] = NormalPoint[0][1] + Line1[1][1];
		NormalPoint[1][0] = NormalPoint[1][0] + Line1[1][0];
		NormalPoint[1][1] = NormalPoint[1][1] + Line1[1][1];
	}

	PointOut[0] = NormalPoint[0][0];
	PointOut[1] = NormalPoint[0][1];
	PointIn[0] = NormalPoint[1][0];
	PointIn[1] = NormalPoint[1][1];

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

iBOOL i51KitG1BladeDraw(void)
{
	iS16 i=0, count=0;
	iS16 width = 0, type=0;
	iS16 abs_dx = 0, abs_dy=0;
	iS16 (*point)[2] = iNULL;
	iS16 (*line)[2] = iNULL;
	iS16 PolygonPoint[4][2] = {0};

	//G1_DrawTime = i51AdeOsGetTick();

	if (G1_TrackPointCount>=2)
	{
		count = G1_TrackPointCount-2;
		PolygonPoint[0][0] = G1_TrackPoint[0][0];
		PolygonPoint[0][1] = G1_TrackPoint[0][1];
		PolygonPoint[3][0] = G1_TrackPoint[0][0];
		PolygonPoint[3][1] = G1_TrackPoint[0][1];
		point = PolygonPoint;
		line = G1_TrackPoint;

		if (2==G1_TrackPointCount) width = G1_MAXBLADEWIDTH;

		for (i=0; i<count; i++)
		{
			width = width < G1_MAXBLADEWIDTH ? (width+2) : width;
			i51KitG1GetSymmetricalPoints(*(point+1), *(point+2), line+i, line+i+1, (*point),  width);

			abs_dx = G1_TrackPoint[i+1][0] - G1_TrackPoint[i][0];
			abs_dy = G1_TrackPoint[i+1][1] - G1_TrackPoint[i][1];
			abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
			abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
			type = abs_dy > abs_dx ? 0 : 1;
			i51KitG1DrawPolygon(PolygonPoint, 4, type, G1_FillColor, G1_alpha);

			//i51KitG1DrawLine(point, G1_BorderWidth, G1_BorderColor, G1_alpha);
			//i51KitG1DrawLine(point+2, G1_BorderWidth, G1_BorderColor, G1_alpha);

			i51KitG1GetAlphaFillType(point, point+2, &type);
			i51KitG1DrawLineEx(point,  type, G1_BorderWidth, G1_BorderColor);
			i51KitG1GetAlphaFillType(point+2, point, &type);
			i51KitG1DrawLineEx(point+2, type, G1_BorderWidth, G1_BorderColor);


			PolygonPoint[0][0] = PolygonPoint[1][0];
			PolygonPoint[0][1] = PolygonPoint[1][1];
			PolygonPoint[3][0] = PolygonPoint[2][0];
			PolygonPoint[3][1] = PolygonPoint[2][1];
		}

		i51KitG1GetSymmetricalPoints(*(point+1), *(point+2), line+count, line+count, (*point),  width);
		abs_dx = G1_TrackPoint[i+1][0] - G1_TrackPoint[i][0];
		abs_dy = G1_TrackPoint[i+1][1] - G1_TrackPoint[i][1];
		abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
		abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
		type = abs_dy > abs_dx ? 0 : 1;
		i51KitG1DrawPolygon(PolygonPoint, 4, type, G1_FillColor, G1_alpha);

		//i51KitG1DrawLine(point, G1_BorderWidth, G1_BorderColor, G1_alpha);
		//i51KitG1DrawLine(point+2, G1_BorderWidth, G1_BorderColor, G1_alpha);
		i51KitG1GetAlphaFillType(point, point+2, &type);
		i51KitG1DrawLineEx(point,  type, G1_BorderWidth, G1_BorderColor);
		i51KitG1GetAlphaFillType(point+2, point, &type);
		i51KitG1DrawLineEx(point+2, type, G1_BorderWidth, G1_BorderColor);

		i51KitG1DrawBladeHead(G1_TrackPoint+count, width, G1_MAXKNIFEPOINT, G1_alpha);
	}

	if ( G1_TrackPointCount > 1 ){

		if (G1_ReferPoint[0] == G1_TrackPoint[0][0] && 
			G1_ReferPoint[1] == G1_TrackPoint[0][1]){

			G1_RepeatCount++;

			if (G1_RepeatCount >= G1_MAXPOINTNUM){
				G1_BaseCount = G1_MAXPOINTNUM;
				i51KitG1InsertPoint();
			}
		}else{
			G1_RepeatCount = 0;
			G1_ReferPoint[0] = G1_TrackPoint[0][0];
			G1_ReferPoint[1] = G1_TrackPoint[0][1];
		}

		if (G1_TrackPointCount >= G1_BaseCount){
			G1_BaseCount--;
			i51KitG1TrackQueueOut();
		}
	}

	return iTRUE;
}

iBOOL i51KitG1BladeClearTrack(void)
{
	i51KitG1TrackQueueEmpty();
	return iTRUE;
}

iBOOL i51KitG1BladeSetTrack(iS16 x, iS16 y, iS32 type)
{
	return i51KitG1TrackQueueIn(x , y, type);
}

iBOOL i51KitG1BladeSetColor(iS16 FillColor, iS16 BorderColor)
{
	G1_FillColor = FillColor; 
	G1_BorderColor = BorderColor;

	return iTRUE;
}


/*
********************************************************************************
*                         粒子系统 函数
********************************************************************************
*/
iHANDLE i51KitG1ParticleCreateEffect(iS32 type, iHANDLE ImageHander, iU32 ImageId, iS16 ImageNum, iS16 MaxCount, iS16 LifeTime)
{
//  author : yepan
//  since  : 2012-7-10
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			type      粒子系统模式
//			ImageHander	图片容器句柄
//			ImageId		图片id
//			ImageNum    粒子数目
//			BlendMode 混合特效 0==不使用alpha 1==使用alpha			
//			MaxCount  粒子系统粒子最大数量
//			LifeTime  生存时间 0==表示一直都存在 非零==有生存时间
//  return :
//			粒子系统句柄 iNULL==失败，非iNULL==成功
//  note   :
//			初始化粒子系统

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;

	if ( iNULL == ImageHander ) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM*)i51AdeOsMalloc(sizeof(G1_PARTICLE_SYSTEM), __FILE__, __LINE__);
	if ( iNULL == ParticleSys){
		iLog("i51KitG1ParticleCreateEffect ParticleSys i51AdeOsMalloc fail");
		return iFALSE;
	}

	i51AdeStdMemset16(ParticleSys, 0, sizeof(G1_PARTICLE_SYSTEM));

	ParticleSys->IsActive = 1;
	ParticleSys->mode = type;
	ParticleSys->MaxParticleCount = MaxCount;
	ParticleSys->CurParticleCount = 0;
	ParticleSys->DurationTime = LifeTime;
	ParticleSys->ElapseTime = 0;
	ParticleSys->ImageHander = ImageHander;
	ParticleSys->ImageId = ImageId;
	ParticleSys->ImageNum = ImageNum;

	//分配粒子空间
	ParticleSys->NodeHeader = (G1_PARTICLE_NODE*)i51AdeOsMalloc(sizeof(G1_PARTICLE_NODE)*MaxCount, __FILE__, __LINE__);
	if ( iNULL == ParticleSys->NodeHeader ){
		iLog("i51KitG1ParticleCreateEffect NodeHeader i51AdeOsMalloc fail");
		return iFALSE;
	}



	switch (type)
	{
	case G1_PARTICLE_JET: //粒子喷射 初始形状随机
		{
			ParticleSys->speed = G1_DEFAULT_SPEED;
			ParticleSys->SpeedVariance = G1_DEFAULT_SPEEDVARIANCE;
			ParticleSys->angle = G1_DEFAULT_ANGLE;
			ParticleSys->AngleVariance = G1_DEFAULT_ANGLEVARIANCE;
			ParticleSys->MaxLifeTime = G1_DEFAULT_LIFETIME;
			ParticleSys->decay = G1_DEFAULT_DECAYTIME;
		}break;
	case G1_PARTICLE_EXPLOSION: //爆炸
		{
			ParticleSys->speed = G1_DEFAULT_SPEED;
			ParticleSys->SpeedVariance = G1_DEFAULT_SPEEDVARIANCE;
			ParticleSys->angle = 0;
			ParticleSys->AngleVariance = 180;
			ParticleSys->MaxLifeTime = G1_DEFAULT_LIFETIME;
			ParticleSys->decay = G1_DEFAULT_DECAYTIME;
		}break;
	case G1_PARTICLE_CYCLICDIFFUSE: //环状扩散
		{
			ParticleSys->speed = G1_DEFAULT_SPEED;
			ParticleSys->SpeedVariance = 0;
			ParticleSys->angle = 0;
			ParticleSys->AngleVariance = 180;
			ParticleSys->MaxLifeTime = G1_DEFAULT_LIFETIME;
			ParticleSys->decay = G1_DEFAULT_DECAYTIME;
		}break;
	case G1_PARTICLE_AREADIFFUSE: //区域扩散
		{
			ParticleSys->speed = G1_DEFAULT_SPEED/2;
			ParticleSys->SpeedVariance = 0;
			ParticleSys->angle = 0;
			ParticleSys->AngleVariance = 180;
			ParticleSys->MaxLifeTime = G1_DEFAULT_LIFETIME;
			ParticleSys->decay = G1_DEFAULT_DECAYTIME;
			ParticleSys->radius = G1_DEFAULT_RADIUS;
		}break;
	default :
		break;
	}

	return ParticleSys;
}

iBOOL i51KitG1ParticleStopEffect(iHANDLE ParticleHandle)
{
//  author : yepan
//  since  : 2012-7-16
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			 EffectHandle     粒子系统句柄
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			停止粒子特效

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;

	if ( iNULL == ParticleHandle) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM*)ParticleHandle;

	ParticleSys->IsActive = 0;

	return iTRUE;
}

iBOOL i51KitG1ParticleResumeEffect(iHANDLE ParticleHandle)
{
//  author : yepan
//  since  : 2012-7-16
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			 EffectHandle     粒子系统句柄
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			恢复粒子特效

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;

	if ( iNULL == ParticleHandle) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM*)ParticleHandle;

	ParticleSys->IsActive = 1;

	return iTRUE;
}

iBOOL i51KitG1ParticleDeleteEffect(iHANDLE ParticleHandle)
{
//  author : yepan
//  since  : 2012-7-10
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			 EffectHandle     粒子系统句柄
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			销毁一个粒子系统		
//	

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	G1_PARTICLE_NODE *Node = iNULL;

	if ( iNULL == ParticleHandle) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM*)ParticleHandle;
	Node = ParticleSys->NodeHeader;

	i51AdeOsFree(Node);
	i51AdeOsFree(ParticleSys);

	return iTRUE;
}

iBOOL i51KitG1ParticleSetSpeed(iHANDLE ParticleHandle, iS16 speed, iS16 SpeedVariance)
{
//  author : yepan
//  since  : 2012-7-12
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			EffectHandle     粒子系统句柄
//			speed			 速度
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			设置粒子初始速度

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	if ( iNULL == ParticleHandle ) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;

	ParticleSys->speed		   = speed;
	ParticleSys->SpeedVariance = SpeedVariance;

	return iTRUE;
}

iBOOL i51KitG1ParticleSetDiffuseAngle(iHANDLE ParticleHandle, iS16 angle, iS16 AngleVariance)
{
//  author : yepan
//  since  : 2012-7-12
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			EffectHandle     粒子系统句柄
//			angle			 角度
//			AngleVariance    角度浮动范围 例如：角度为5  AngleVariance为1 则时间角度在4~6之间
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			设置粒子发射角度范围

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	if ( iNULL == ParticleHandle ) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;

	ParticleSys->angle = angle;
	ParticleSys->AngleVariance = AngleVariance;

	return iTRUE;
}

iBOOL i51KitG1ParticleSetActionRadius(iHANDLE ParticleHandle, iS16 radius)
{
//  author : yepan
//  since  : 2012-7-12
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			EffectHandle     粒子系统句柄
//			radius  半径
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			设置粒子活动半径

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	if ( iNULL == ParticleHandle ) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;

	ParticleSys->radius = radius;

	return iTRUE;
}

iBOOL i51KitG1ParticleSetAccel(iHANDLE ParticleHandle, iS16 AccelX, iS16 AccelY)
{
//  author : yepan
//  since  : 2012-7-12
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			EffectHandle     粒子系统句柄
//			AccelX X方向加速度分量，AccelY沿着Y加速度
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			设置粒子加速度

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	if ( iNULL == ParticleHandle ) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;

	ParticleSys->AccelX = AccelX;
	ParticleSys->AccelY = AccelY;

	return iTRUE;
}

iBOOL i51KitG1ParticleSetImage(iHANDLE ParticleHandle, iHANDLE ImageHander, iS16 ImageId, iS16 ImageNum)
{
//  author : yepan
//  since  : 2012-7-12
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			EffectHandle     粒子系统句柄
//			ImageId			 图片id
//			ImageNum		 粒子数目
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			设置粒子加速度

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	if ( iNULL == ParticleHandle || iNULL == ImageHander) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;

	ParticleSys->ImageHander = ImageHander;
	ParticleSys->ImageId	 = ImageId;
	ParticleSys->ImageNum = ImageNum;

	return iTRUE;
}

iBOOL i51KitG1ParticleSetTime(iHANDLE ParticleHandle, iS16 LifeTime, iS16 DecayTime)
{
//  author : yepan
//  since  : 2012-7-12
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			EffectHandle     粒子系统句柄
//			ImageId			 图片id
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			设置粒子生命周期和衰减速度


	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	if ( iNULL == ParticleHandle ) return iFALSE;
	
	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;

	ParticleSys->MaxLifeTime = LifeTime;
	ParticleSys->decay = DecayTime;

	return iTRUE;
}

iBOOL i51KitG1ParticleAdd(iHANDLE ParticleHandle, iS16 count, iS16 x, iS16 y)
{
	//  author : yepan
	//  since  : 2012-7-10
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			EffectHandle     粒子系统句柄
	//			count  粒子个数
	//			x,y    发射位置坐标
	//  return :
	//			iTRUE==成功， iFALSE==失败
	//  note   :
	//			向系统中添加粒子

	iS16 i=0, AddCount=0;
	iS16 base_angle = 0, AngleScope=0, tmp_angle=0;
	iS16 base_speed = 0, SpeedScope=0, tmp_speed=0;
	iS32 TmpSpeed = 0, PosX=0, PosY=0;
	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	G1_PARTICLE_NODE   *ParticleNode = iNULL;
	G1_PARTICLE_NODE   *ParticleBase = iNULL;

	if ( iNULL == ParticleHandle ) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;

	//没有激活
	if ( 0 == ParticleSys->IsActive ) return iTRUE;

	AddCount = ParticleSys->MaxParticleCount - ParticleSys->CurParticleCount;
	AddCount = count<=AddCount ? count : AddCount;

	ParticleSys->StartX = x;
	ParticleSys->StartY = y;
	ParticleBase = ParticleSys->NodeHeader + ParticleSys->CurParticleCount;

	switch (ParticleSys->mode)
	{
	case G1_PARTICLE_JET: //粒子喷射
		{
			base_angle = ParticleSys->angle - ParticleSys->AngleVariance;
			AngleScope = ParticleSys->AngleVariance * 2;
			PosX = ParticleSys->StartX*G1_DEFAULT_SPEEDFACTOR;
			PosY = ParticleSys->StartY*G1_DEFAULT_SPEEDFACTOR;

			if (ParticleSys->SpeedVariance){
				base_speed = ParticleSys->speed - ParticleSys->SpeedVariance;
				SpeedScope = ParticleSys->SpeedVariance*2;
			}else{
				base_speed = ParticleSys->speed;
				SpeedScope = 0;
			}

			for (i=0; i<AddCount; i++){
				ParticleNode = ParticleBase + i;
				i51AdeStdMemset16(ParticleNode, 0, sizeof(G1_PARTICLE_NODE));
				ParticleNode->IsValid = 1;
				ParticleNode->PosX = PosX;
				ParticleNode->PosY = PosY;
				ParticleNode->Decay = ParticleSys->decay;
				ParticleNode->life = ParticleSys->MaxLifeTime;

				//发射角度
				if ( 0 !=AngleScope ){
					tmp_angle = rand()%AngleScope + base_angle;
				}else{
					tmp_angle = base_angle;
				}

				//速度放大G1_DEFAULT_SPEEDFACTOR倍
				if ( 0 != SpeedScope ){
					tmp_speed =  rand()%SpeedScope + base_speed;
					tmp_speed =  tmp_speed*G1_DEFAULT_SPEEDFACTOR;
				}else{
					tmp_speed = base_speed*G1_DEFAULT_SPEEDFACTOR;
				}

				TmpSpeed = tmp_speed * i51KitG1Cos(tmp_angle);
				ParticleNode->SpeedX = TmpSpeed/G1_DEFAULT_TRIANGLEFACTOR;
				TmpSpeed = tmp_speed * i51KitG1Sin(tmp_angle);
				ParticleNode->SpeedY = TmpSpeed/G1_DEFAULT_TRIANGLEFACTOR;

				ParticleSys->CurParticleCount++;
			}			
		}break;
	case G1_PARTICLE_EXPLOSION: //爆炸
		{
			base_angle = ParticleSys->angle - ParticleSys->AngleVariance;
			AngleScope = ParticleSys->AngleVariance * 2;
			PosX = ParticleSys->StartX*G1_DEFAULT_SPEEDFACTOR;
			PosY = ParticleSys->StartY*G1_DEFAULT_SPEEDFACTOR;

			if (ParticleSys->SpeedVariance){
				base_speed = ParticleSys->speed - ParticleSys->SpeedVariance;
				SpeedScope = ParticleSys->SpeedVariance*2;
			}else{
				base_speed = ParticleSys->speed;
				SpeedScope = 0;
			}

			for (i=0; i<AddCount; i++){
				ParticleNode = ParticleBase + i;
				i51AdeStdMemset16(ParticleNode, 0, sizeof(G1_PARTICLE_NODE));
				ParticleNode->IsValid = 1;
				ParticleNode->PosX = PosX;
				ParticleNode->PosY = PosY;
				ParticleNode->Decay = ParticleSys->decay;

				ParticleNode->life = rand()%(ParticleSys->MaxLifeTime);

				//速度放大G1_DEFAULT_SPEEDFACTOR倍
				if ( 0 != SpeedScope ){
					tmp_speed =  rand()%SpeedScope + base_speed;
					tmp_speed =  tmp_speed*G1_DEFAULT_SPEEDFACTOR;
				}else{
					tmp_speed = base_speed*G1_DEFAULT_SPEEDFACTOR;
				}

				if ( 0 !=AngleScope ){
					tmp_angle = rand()%AngleScope + base_angle;
				}else{
					tmp_angle = base_angle;
				}

				TmpSpeed = tmp_speed * i51KitG1Cos(tmp_angle);
				ParticleNode->SpeedX = TmpSpeed/G1_DEFAULT_TRIANGLEFACTOR;
				TmpSpeed = tmp_speed * i51KitG1Sin(tmp_angle);
				ParticleNode->SpeedY = TmpSpeed/G1_DEFAULT_TRIANGLEFACTOR;

				ParticleSys->CurParticleCount++;
			}
		}break;
	case G1_PARTICLE_CYCLICDIFFUSE://环状扩散
		{
			base_angle = rand()%360;
			//AngleScope = AddCount > 0 ? (360/AddCount) : 0;
			base_speed = ParticleSys->speed;
			PosX = ParticleSys->StartX*G1_DEFAULT_SPEEDFACTOR;
			PosY = ParticleSys->StartY*G1_DEFAULT_SPEEDFACTOR;

			for (i=0; i<AddCount; i++){
				ParticleNode = ParticleBase + i;
				i51AdeStdMemset16(ParticleNode, 0, sizeof(G1_PARTICLE_NODE));
				ParticleNode->IsValid = 1;
				ParticleNode->PosX = PosX;
				ParticleNode->PosY = PosY;
				ParticleNode->life = ParticleSys->MaxLifeTime;
				ParticleNode->Decay = ParticleSys->decay;

				//速度放大G1_DEFAULT_SPEEDFACTOR倍
				tmp_speed = base_speed*G1_DEFAULT_SPEEDFACTOR;

				tmp_angle = i*360/AddCount + base_angle;

				//tmp_angle = i*AngleScope + base_angle;

				TmpSpeed = tmp_speed * i51KitG1Cos(tmp_angle);
				ParticleNode->SpeedX = TmpSpeed/G1_DEFAULT_TRIANGLEFACTOR;
				TmpSpeed = tmp_speed * i51KitG1Sin(tmp_angle);
				ParticleNode->SpeedY = TmpSpeed/G1_DEFAULT_TRIANGLEFACTOR;

				ParticleSys->CurParticleCount++;
			}
		}break;
	case G1_PARTICLE_AREADIFFUSE: //区域扩散
		{
			base_angle = ParticleSys->angle - ParticleSys->AngleVariance;
			AngleScope = ParticleSys->AngleVariance * 2;
			PosX = ParticleSys->StartX*G1_DEFAULT_SPEEDFACTOR;
			PosY = ParticleSys->StartY*G1_DEFAULT_SPEEDFACTOR;

			if (ParticleSys->SpeedVariance){
				base_speed = ParticleSys->speed - ParticleSys->SpeedVariance;
				SpeedScope = ParticleSys->SpeedVariance*2;
			}else{
				base_speed = ParticleSys->speed;
				SpeedScope = 0;
			}

			for (i=0; i<AddCount; i++){
				ParticleNode = ParticleBase + i;
				i51AdeStdMemset16(ParticleNode, 0, sizeof(G1_PARTICLE_NODE));
				ParticleNode->IsValid = 1;
				ParticleNode->PosX = PosX;
				ParticleNode->PosY = PosY;
				ParticleNode->Decay = ParticleSys->decay;
				
				ParticleNode->life = rand()%(ParticleSys->MaxLifeTime);

				base_angle = ParticleSys->angle - ParticleSys->AngleVariance;
				AngleScope = ParticleSys->AngleVariance * 2;

				//位置
				if ( 0 !=AngleScope ){
					tmp_angle = rand()%AngleScope + base_angle;
				}
				else{
					tmp_angle = base_angle;
				}
				tmp_speed = rand()%ParticleSys->radius;
				ParticleNode->PosX += tmp_speed*i51KitG1Cos(tmp_angle);
				ParticleNode->PosY += tmp_speed*i51KitG1Sin(tmp_angle);

				//速度放大G1_DEFAULT_SPEEDFACTOR倍
				if ( 0 != SpeedScope ){
					tmp_speed =  rand()%SpeedScope + base_speed;
					tmp_speed =  tmp_speed*G1_DEFAULT_SPEEDFACTOR;
				}else{
					tmp_speed = base_speed*G1_DEFAULT_SPEEDFACTOR;
				}

				base_angle = tmp_angle - 90;
				AngleScope = 180;
				if ( 0 !=AngleScope ){
					tmp_angle = rand()%AngleScope + base_angle;
				}
				else{
					tmp_angle = base_angle;
				}

				TmpSpeed = tmp_speed * i51KitG1Cos(tmp_angle);
				ParticleNode->SpeedX = TmpSpeed/G1_DEFAULT_TRIANGLEFACTOR;
				TmpSpeed = tmp_speed * i51KitG1Sin(tmp_angle);
				ParticleNode->SpeedY = TmpSpeed/G1_DEFAULT_TRIANGLEFACTOR;

				ParticleSys->CurParticleCount++;
			}
		}break;
	default :
		break;
	}

	return iTRUE;
}

static iBOOL i51KitG1UpdateParticle(iHANDLE ParticleHandle)
{
	//  author : yepan
	//  since  : 2012-7-11
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			EffectHandle     粒子系统句柄
	//  return :
	//			iTRUE==成功， iFALSE==失败
	//  note   :
	//			更新粒子信息		
	//
	iS16 i=0, count=0;
	iS32 TmpX=0, TmpY=0;
	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	G1_PARTICLE_NODE   *ParticleCur = iNULL;
	G1_PARTICLE_NODE   *ParticleNode = iNULL;
	if ( iNULL == ParticleHandle ) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;
	if ( 0 == ParticleSys->IsActive || ParticleSys->CurParticleCount <= 0){
		return iTRUE;
	}

	count = ParticleSys->CurParticleCount;

	for (i=0; i<count; i++){
		ParticleCur = ParticleSys->NodeHeader + i;
		ParticleCur->life -=  ParticleCur->Decay;

		if (ParticleCur->life < 0){
			ParticleNode = ParticleSys->NodeHeader + count - 1;
			i51AdeStdMemcpy16(ParticleCur, ParticleNode, sizeof(G1_PARTICLE_NODE));
			i51AdeStdMemset16(ParticleNode, 0, sizeof(G1_PARTICLE_NODE));
			ParticleSys->CurParticleCount--;
			count--;
			i = i>0 ? i-1 : 0;
			continue;
		}
		else{
			//更新坐标
			TmpX = ParticleCur->SpeedX + ParticleCur->AccelX / 2;
			TmpY = ParticleCur->SpeedY + ParticleCur->AccelY / 2;
			ParticleCur->PosX += TmpX/G1_DEFAULT_SPEED_UNIT;
			ParticleCur->PosY += TmpY/G1_DEFAULT_SPEED_UNIT;
			ParticleCur->SpeedX += ParticleCur->AccelX;
			ParticleCur->SpeedY += ParticleCur->AccelY;
		}
	}

	return iTRUE;
}

iBOOL i51KitG1ParticleDraw(iHANDLE ParticleHandle)
{
//  author : yepan
//  since  : 2012-7-11
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			EffectHandle     粒子系统句柄
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			绘制粒子
//

	iS16 i=0, count=0, index = 0;
	iS16 TempX = 0, TempY=0;
	iU32 ImageId = 0, ImageNum=0;
	iU8 *PicBuf = iNULL;
	iU32 width=0, height=0;
	iRECT SrcRect, DesRect;
	G1_PARTICLE_NODE   *ParticleNode = iNULL;
	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;

	if ( iNULL == ParticleHandle )return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;

	//绘制该粒子系统粒子
	ImageId = ParticleSys->ImageId;
	ImageNum = ParticleSys->ImageNum;
	count = ParticleSys->CurParticleCount;

	ImageNum = 0==ImageNum?1:ImageNum;

	for (i=0; i<count; i++)
	{
		ParticleNode = ParticleSys->NodeHeader + i;
		index = ParticleNode->life*ImageNum/ParticleSys->MaxLifeTime;
		TempX = ParticleNode->PosX/G1_DEFAULT_SPEEDFACTOR;
		TempY = ParticleNode->PosY/G1_DEFAULT_SPEEDFACTOR;
		i51KitG2GetImageAttr(ParticleSys->ImageHander, ImageId, &width, &height, PicBuf);
		DesRect.X = TempX - width/2;
		DesRect.Y = TempY - height/(ImageNum*2);
		DesRect.Width = width;
		DesRect.Height = height/ImageNum;
		SrcRect.X = 0;
		//SrcRect.Y = height*(ImageNum-index)/ImageNum; todo_yepan
		SrcRect.Y = height*index/ImageNum;
		SrcRect.Width = width;
		SrcRect.Height = DesRect.Height;
		i51KitG2DrawImage(ParticleSys->ImageHander ,ImageId, &DesRect, &SrcRect);
	}

	//更新粒子信息
	i51KitG1UpdateParticle(ParticleHandle);

	return iTRUE;
}

iBOOL i51KitG1ParticleClear(iHANDLE ParticleHandle)
{
//  author : yepan
//  since  : 2012-7-11
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			EffectHandle     粒子系统句柄
//  return :
//			iTRUE==成功， iFALSE==失败
//  note   :
//			清空ID号为id的粒子系统所有粒子

	G1_PARTICLE_SYSTEM *ParticleSys = iNULL;
	G1_PARTICLE_NODE   *ParticleNode = iNULL;

	if ( iFALSE == ParticleHandle ) return iFALSE;

	ParticleSys = (G1_PARTICLE_SYSTEM *)ParticleHandle;
	ParticleNode = ParticleSys->NodeHeader;

	ParticleSys->CurParticleCount = 0;
	i51AdeStdMemset16(ParticleNode, 0, (ParticleSys->MaxParticleCount * sizeof(G1_PARTICLE_NODE)));

	return iTRUE;
}

#if G1_PARTICLE_DEMO
static iHANDLE G1_ParticleHandle = iNULL;
static iHANDLE G1_ImageContainer = iNULL;
static iHANDLE G1_PH[4] = {0};
static iU32 G1_ImageId[2] = {alpha_blue_star5, alpha_blue_star7};
static iS16 G1_Count = 3;

static iS16 ParticleIndex = 0;
static iS16 TouchFlag = 0;
static iRECT G1_Rect = {0};

void i51KitG1Process(iU32 Step , iU32 Totall , iHANDLE Handle)
{

}

void i51KitG1Done( iHANDLE Handle)
{
	if( i51AdeOsFileIsExist(L"i51KitG1\\PARTICLE") ){
		iLog("create imageContainer fail");
	}

	if (Handle){
		G1_ImageContainer = Handle;

		i51KitG2SetImageMemoryCopy(G1_ImageContainer,2,G1_ImageId);

		G1_PH[0] = i51KitG1ParticleCreateEffect(G1_PARTICLE_JET, G1_ImageContainer, G1_ImageId[0], 5, 25, 0);
		G1_PH[1] = i51KitG1ParticleCreateEffect(G1_PARTICLE_EXPLOSION, G1_ImageContainer, G1_ImageId[1], 7, 25, 0);
		G1_PH[2] = i51KitG1ParticleCreateEffect(G1_PARTICLE_CYCLICDIFFUSE, G1_ImageContainer, G1_ImageId[1], 7, 25, 0);
		G1_PH[3] = i51KitG1ParticleCreateEffect(G1_PARTICLE_AREADIFFUSE, G1_ImageContainer, G1_ImageId[1], 7, 25, 0);

		G1_ParticleHandle = G1_PH[0];
	}
}
#endif //G1_PARTICLE_DEMO

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{
	//	TODO :	消息循环

	switch ( message )
	{
	case i51_MSG_INIT :
		{
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

			i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ){
				return 0;
			}

			//	加载i51KitG2动态库
			i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ){
				return 0 ;
			}

			//初始化局部变量
			if ( iNULL == G1_ScreenBuf ){

				iU32 *ScreenContext=iNULL;
				if( i51AdeMmiGetScreenScale ( &G1_ScreenWidth , &G1_ScreenHeight ) == 0 ){
					return iFALSE;
				}

				ScreenContext =  i51AdeMmiGetPanel();
				if( iNULL == ScreenContext ) return iFALSE;
				G1_ScreenBuf = (iU16 *)ScreenContext[1];
			}

			//清空轨迹队列
			i51KitG1TrackQueueEmpty();

#if G1_PARTICLE_DEMO
			//debug——粒子特效
			{
				iU16 *ResPath = L"i51KitG1\\i51KitG1.re";
				iU16 *ContainerPath = L"i51KitG1\\PARTICLE";
				iFILE fileRes = iNULL;

				if ( iTRUE == i51AdeOsFileIsExist(ResPath) ){

					fileRes = i51KitStdResOpen(ResPath);
					if ( iNULL == fileRes ){
						iLog("i51KitStdResOpen open res fail");
						return iFALSE;
					}

					i51KitG2CreateImageContainer (fileRes , G1_ImageId , 2 , ContainerPath ,  i51KitG1Process , i51KitG1Done );
				}
			}
#endif //G1_PARTICLE_DEMO

		}break ;
	case i51_MSG_RUN :
		break ;
	case i51_MSG_PAUSE :
		break ;
	case i51_MSG_EXIT :
		break ;

#if G1_TEST_DEMO
	case i51_MSG_PADDOWN:
	case i51_MSG_PADUP:
	case i51_MSG_PADMOVE:
		{
#if G1_BLADE_DEMO
			{
				iS16 x = 0;
				iS16 y = 0;
				x = (((iU32)parameter)>>16)&0XFFFF;
				y = (((iU32)parameter))&0XFFFF;

				i51KitG1BladeSetTrack(x, y, message);
			}
#endif //G1_BLADE_DEMO

#if G1_PARTICLE_DEMO
			{
				iS16 x = 0;
				iS16 y = 0;
				x = (((iU32)parameter)>>16)&0XFFFF;
				y = (((iU32)parameter))&0XFFFF;


				G1_Rect.X  = G1_ScreenWidth - 31;
				G1_Rect.Y  = G1_ScreenHeight - 31;	
				G1_Rect.Width = 30;
				G1_Rect.Height = 30;	
				if ( i51_MSG_PADDOWN == message )
				{
					if ( x>=G1_Rect.X && x<G1_ScreenWidth && 
						y >= G1_Rect.Y && y<G1_ScreenHeight)
					{
						TouchFlag = 1;
					}
				}

				if ( i51_MSG_PADDOWN == message )
				{
					if ( x>=G1_Rect.X && x<G1_ScreenWidth && 
						y >= G1_Rect.Y && y<G1_ScreenHeight && 1 == TouchFlag)
					{
						ParticleIndex++;
						ParticleIndex = ParticleIndex%4;

						G1_ParticleHandle=G1_PH[ParticleIndex];
						if ( 0 == ParticleIndex )
						{
							G1_Count = 3;							
						}
						else if ( 2 == ParticleIndex )
						{
							G1_Count = 12;
						}
						else
						{
							G1_Count = 25;
						}
					}
				}

				TouchFlag = 0;
				i51KitG1ParticleAdd(G1_ParticleHandle, G1_Count, x, y);
			}
#endif //G1_PARTICLE_DEMO
		}break;
#endif //G1_TEST_DEMO

	case i51_MSG_PAINT :
		{

#if G1_TEST_DEMO
			i51AdeStdMemset16(G1_ScreenBuf, 0, sizeof(iU16)*G1_ScreenWidth*G1_ScreenHeight);

#if G1_BLADE_DEMO			
			//测试多边形绘制
			//{
			//	int i=0; 
			//	iS16 polygon[4][2] = {{10,200}, {250,120}, {300,360}, {10,200}};
			//	while ( polygon[1][1] < polygon[2][1] )
			//	{
			//		i51AdeStdMemset16(G1_ScreenBuf, 0, sizeof(iU16)*G1_ScreenWidth*G1_ScreenHeight);
			//		i51KitG1DrawPolygon(polygon, 4, 1, 0xFFFF, 0xFF);
			//		i51AdeMmiUpdateScreen();

			//		polygon[1][1] += 2;
			//		polygon[2][1] -= 2;

			//		i51AdeOsSleep(20);
			//	}


			//	//int i=0; 
			//	//iS16 polygon[4][2] = {{10,200}, {250,238}, {300,242}, {10,200}};
			//	//i51AdeStdMemset16(G1_ScreenBuf, 0, sizeof(iU16)*G1_ScreenWidth*G1_ScreenHeight);
			//	//i51KitG1DrawPolygon(polygon, 4, 1, 0xFFFF, 0xFF);
			//	//i51AdeMmiUpdateScreen();
			//}

			i51KitG1BladeDraw();
#endif

#if G1_PARTICLE_DEMO 
			//sTime = i51AdeOsGetTick();
			i51KitG1ParticleDraw(G1_ParticleHandle);
			//sTime = i51AdeOsGetTick() - sTime;
			//iLog("i51KitG1ParticleDraw use time is %d, Particle Id is %d", sTime, ParticleIndex);
#endif //G1_PARTICLE_DEMO
			i51AdeMmiUpdateScreen();
#endif //G1_TEST_DEMO
		}break;
	}

	return iTRUE ;
}

iKIT iU32* i51KitPort ()
{
	//	TODO : Initialize API Port for external layer
	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitG1)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1BladeDraw)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1BladeClearTrack)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1BladeSetTrack)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1BladeSetColor)

		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleCreateEffect)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleStopEffect)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleResumeEffect)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleDeleteEffect)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleSetSpeed)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleSetDiffuseAngle)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleSetActionRadius)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleSetAccel)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleSetImage)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleSetTime)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleAdd)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleDraw)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitG1ParticleClear)
	i51KIT_DYNAMIC_KIT_MAPING_END 

	return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitG1) ;
}
