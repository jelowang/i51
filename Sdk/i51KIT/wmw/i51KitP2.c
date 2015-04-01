
#include "i51KitAde.h"
#include "i51KitP2.h"
#include "i51KitG2.h"
#include "mwPublic.h"

// 水滴与直线碰撞后可能的运动方向
#define DIRECTION_NO		0X00
#define DIRECTION_TOP		0X01
#define DIRECTION_RT		0X02
#define DIRECTION_RIGHT	0X04
#define DIRECTION_RB		0X08
#define DIRECTION_BOTTOM	0X10
#define DIRECTION_LB		0X20
#define DIRECTION_LEFT		0X40
#define DIRECTION_LT		0X80
#define DIRECTION_ALL		0XFF

#define DIRECTION_RIGHT_VX	(DIRECTION_RT|DIRECTION_RB|DIRECTION_RIGHT)
#define DIRECTION_LEFT_VX	(DIRECTION_LT|DIRECTION_LB|DIRECTION_LEFT)
#define DIRECTION_ZERO_VX	(DIRECTION_TOP|DIRECTION_BOTTOM)
#define DIRECTION_TOP_VY	(DIRECTION_RT|DIRECTION_TOP|DIRECTION_LT)
#define DIRECTION_BOTTOM_VY	(DIRECTION_RB|DIRECTION_BOTTOM|DIRECTION_LB)
#define DIRECTION_ZERO_VY	(DIRECTION_RIGHT|DIRECTION_LEFT)

#define MAX_INTERSECTION	10
static iVECT INTERSECTION_P2S[MAX_INTERSECTION];
static iS32 IntersectionNum = 0;

static iU32 CurrDirection = DIRECTION_ALL;
static iS32 DirectionNum = 0;

void * i51KitP2Malloc(iU32 length)
{
	if(length<4) length = 4;
	return i51AdeOsMalloc(length, __FILE__, __LINE__ );
}

void *i51KitP2Realloc(void *src_ptr, int size, int resize)
{
	void *temp_ptr = iNULL;
	int CopySize = 0;
	if(!resize){
		i51AdeOsFree(src_ptr);
	}
	else
	{
		temp_ptr = (void *)i51KitP2Malloc(resize);
		memset(temp_ptr,0,resize);
		if(temp_ptr)
		{
			if(src_ptr)
			{
				CopySize = size < resize ? size : resize;
				i51AdeStdMemcpy16(temp_ptr, src_ptr, CopySize);
			}
			i51AdeOsFree(src_ptr);
		}
		else
		{
			iLog("cpRealloc malloc err!");
		}
	}

	return temp_ptr;
}	
iU32 SqrtInt( iU32 Num )
{
#if 0
	float x = (float)(Num);
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);        // 计算第一个近似根
	x = *(float*)&i;
	x = x*(1.5f - xhalf*x*x);       // 牛顿迭代法
	return (iU32)x;

#else
	iU32 i = 0;
	iU32 Start = 1;
	iU32 End = Num;
	iU32 Temp = 0;

	if( Num==0 || Num==1 ) return Num;
	if( Num>100000000 )
	{
		End = 65535;
		Start = 10000;
	}
	else if(Num>1000000)
	{
		End = 10000;
		Start = 1000;
			
	}
	else if( Num>10000 )
	{
		End = 1000;
		Start = 100;
	}
	else if( Num>100 )
	{
		End = 100;
		Start = 10;
	}
	
	i = (Start+End)/2;
	
	while(1)
	{
		Temp = i*i;
		if( Temp <Num )
		{
			Start = i;
		}
		else if( Temp>Num )
		{
			End = i;
		}
		else
		{
			break;
		}
		i = (Start+End)/2;
		if(i==Start) break;

	}
	
	return i;
#endif
}

iBOOL iVectInit1(iVECT *DesVect, iVECT v1 )
{
	DesVect->X = v1.X;
	DesVect->Y = v1.Y;
	return iTRUE;
}

iBOOL iVectInit2(iVECT *DesVect, iS32 X, iS32 Y )
{
	DesVect->X = X;
	DesVect->Y = Y;
	return iTRUE;
}

/**
	2D vector cross product analog.
	The cross product of 2D vectors results in a 3D vector with only a z component.
	This function returns the magnitude of the z value.
*/
iS32 iVectCross( iVECT v1,  iVECT v2)
{
	return v1.X*v2.Y - v1.Y*v2.X;
}

static iBOOL iVectSub(iVECT *DesVect, iVECT v1, iVECT v2)
{
	DesVect->X = v1.X - v2.X;
	DesVect->Y = v1.Y - v2.Y;
	return iTRUE;
}

static iBOOL iVectAdd(iVECT *DesVect, iVECT v1, iVECT v2)
{
	DesVect->X = v1.X + v2.X;
	DesVect->Y = v1.Y + v2.Y;
	return iTRUE;
}

/// Scalar multiplication.
static iBOOL iVectMult(iVECT *DesVect, iVECT  V, iS32 S )
{
	DesVect->X = V.X*S;
	DesVect->Y = V.Y*S;
//	DesVect->X = 20*S;
//	DesVect->Y = 20*S;
	return iTRUE;
}

static iBOOL iVectEqual(iVECT p1, iVECT  p2 )
{
	// 判断两个向量是否相等
	return (p1.X==p2.X && p1.Y==p2.Y);
}

static iS32 iVectDistanceP2P(iVECT v1, iVECT v2)
{
	// 计算两点之间的距离
//	return 100;
	return SqrtInt(((v1.X-v2.X)*(v1.X-v2.X) + (v1.Y-v2.Y)*(v1.Y-v2.Y) ));
}

static iBOOL MoveCentrePoint(iVECT * Des,iVECT C1,iVECT C2, iS32 Inset)
{
	iS32 Mul = (iS32)Inset;
//	iS32 Distance = iVectDistanceP2P(C1, C2);

#if 1
	if( C1.X>C2.X) Des->X  += Inset;
	else if( C1.X<C2.X) Des->X  -= Inset;

	if( C1.Y>C2.Y) Des->Y  += Inset;
	else if( C1.Y<C2.Y) Des->Y  -= Inset;
#else
	if( 0!=Distance )
	{
		Des->X  += Inset*(C1.X-C2.X)/Distance;
		Des->Y  += Inset*(C1.Y-C2.Y)/Distance;
	}
#endif

	return iTRUE;
}

iBOOL i51KitP2ShapePosMove( iHANDLE Shape, iVECT *MovePos );

/// force
static iBOOL iBodyForce(iVECT *DesForce, iVECT SrcForce, iVECT  SrcC, iVECT CollideC )
{
	// author : Otto
	// since : 2012.6.9
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iVECT * DesForce : 物体合力指针
	// iVECT SrcForce : 相互碰撞的物体产生的力
	// iVECT SrcC : 物体的质心位置
	// iVECT CollideC : 碰撞物体的位置

	if( SrcC.X < CollideC.X ) DesForce->X -= AbsInt(SrcForce.X);
	else if( SrcC.X > CollideC.X ) DesForce->X += AbsInt(SrcForce.X);
	
	if( SrcC.Y < CollideC.Y ) DesForce->Y -= AbsInt(SrcForce.Y);
	else if( SrcC.Y > CollideC.Y ) DesForce->Y += AbsInt(SrcForce.Y);

	return iTRUE;
}

static iBOOL iVectGetP2SegVerticalFellowship( iVECT v, iShapeSeg * Shape )
{
	// 判断点到直线的垂点是否在线段内
	
	iS32 X = v.X;
	iS32 Y = v.Y;
	iS32 X1 = Shape->p1.X;
	iS32 Y1 = Shape->p1.Y;
	iS32 X2 = Shape->p2.X;
	iS32 Y2 = Shape->p2.Y;
	iS32 MaxX = 0;
	iS32 MinX = 0;
	iS32 MaxY = 0;
	iS32 MinY = 0;
	iS32 MulX = 0;
	iS32 MulY = 0;
	iS32 Slope1 = 0;
	iS32 Slope2 = 0;

	MulX = X1-X2;
	MulY = Y1-Y2;
	
	if(X1>X2)
	{
		MaxX=X1;
		MinX = X2;
	}
	else
	{
		MaxX = X2;
		MinX = X1;
	}
	
	if(Y1>Y2)
	{
		MaxY=Y1;
		MinY = Y2;
	}
	else
	{
		MaxY = Y2;
		MinY = Y1;
	}
	
	if( MaxX==MinX)
	{
		// 线段是垂直线
		if( Y>=MinY && Y<=MaxY ) return iTRUE;
		else return iFALSE;
	}

	if( MinY==MaxY )
	{
		// 线段是水平线
		if( X>=MinX && X<=MaxX ) return iTRUE;
		else return iFALSE;
	}

	if( Y==MaxY)
	{
		if( MulX*MulY > 0 )
		{
			// 直线角度大于90°
			if( X<=MaxX ) return iTRUE;
			else return iFALSE;
		}
		else
		{
			// 直线角度小于90°
			if( X>=MinX ) return iTRUE;
			else return iFALSE;
		}
	}
	else if( Y==MinY)
	{
		if( MulX*MulY > 0 )
		{
			// 直线角度大于90°
			if( X>=MinX ) return iTRUE;
			else return iFALSE;
		}
		else
		{
			// 直线角度小于90°
			if( X<=MaxX ) return iTRUE;
			else return iFALSE;
		}
	}
	
	Slope1 = (X-X1)*(MulY)/MulX/(Y-Y1);
	Slope2 = (X-X2)*(MulY)/MulX/(Y-Y2);

	if( (Slope1<=-1&& Slope2>=-1) || (Slope1>=-1&& Slope2<=-1) ) return iTRUE;
	
	return iFALSE;
	
}

static iBOOL iVectGetP2SegPedal( iVECT * Pedal, iVECT v, iShapeSeg * Shape )
{
	// 计算点到直线的垂点坐标
	// 设直线方程为ax+by+c=0,点坐标为(m,n)   
	//  则垂足为((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
	// 如果垂足不在线段内，则返回false
	
	iS32 a = Shape->a;
	iS32 b = Shape->b;
	iS32 c = Shape->c;
	iS32 X = v.X;
	iS32 Y = v.Y;
	iVECT p1,p2;

#if 0
	iS32 X1,Y1,X2,Y2,X3,Y3 ;
	iS32 Mx,My;
	iS32 X0, Y0;
#endif	
/*
	if( v.X==Shape->p1.X )
	{
		Pedal->X = Shape->p1.X;
		Pedal->Y = Shape->p1.Y;
		return iTRUE;
	}
	else if( v.X==Shape->p2.X )
	{
		Pedal->X = Shape->p2.X;
		Pedal->Y = Shape->p2.Y;
		return iTRUE;
	}
*/	
// 如果abc过大则结果越界
//	Pedal->X = (b*b*v.X-a*b*v.Y-a*c)/(a*a+b*b);
//	Pedal->Y = (a*a*v.Y-a*b*v.X-b*c)/(a*a+b*b);

	Pedal->X = (b*b*X-a*b*Y-a*c)/(a*a+b*b);
	Pedal->Y = (a*a*Y-a*b*X-b*c)/(a*a+b*b);

//	Pedal->X *= COORDINATE_RATE;
//	Pedal->Y *= COORDINATE_RATE;
	
	iVectInit1(&p1, Shape->p1);
	iVectInit1(&p2, Shape->p2);
#if 0
	X1 = p1.X;
	X2 = p2.X;
	X3 = v.X;
	
	Y1 = p1.Y;
	Y2 = p2.Y;
	Y3 = v.Y;

	Mx = X1-X2;
	My = Y1 - Y2;

	X0 = (Mx*My*(Y1-Y3)+Mx*Mx*X3-My*My*X1)/(Mx*Mx-My*My);
	Y0 = My*(X0-X1)/Mx + Y1;

	Pedal->X = X0;
	Pedal->Y = Y0;
#endif	
#if 1
	if( (Pedal->Y-p1.Y) * (Pedal->Y-p2.Y) >0 )
	{
		return iFALSE;
	}
	else if( (Pedal->X-(p1.X)) * (Pedal->X-(p2.X)) >0 )
	{
		return iFALSE;
	}
#else		
	{
		if( p1.X>p2.X)
		{
			if( Pedal->X>p1.X+1 || Pedal->X < p2.X - 1 )
			{
				return iFALSE;
			}
		}
		else if( p1.X<p2.X)
		{
			if( Pedal->X>p2.X+1 || Pedal->X < p1.X - 1 )
			{
				return iFALSE;
			}
		}
		else
		{
			if( p1.Y>p2.Y)
			{
				if( Pedal->Y>p1.Y+1 || Pedal->Y < p2.Y - 1 )
				{
					return iFALSE;
				}
			}
			else if( p1.Y<p2.Y)
			{
				if( Pedal->Y>p2.Y+1 || Pedal->Y < p1.Y - 1 )
				{
					return iFALSE;
				}
			}
		}
	}
#endif
	return iTRUE;
	
}

static iBOOL iVectGetP2SegIntersectant( iVECT C1, iS32 r, iShapeSeg * Shape )
{
	// 判断当圆到直线的垂直不在线段内，圆与线段是否有交点,
	// 并更新圆的速度限制
	
	iVECT p1,p2;
	iU32 Direction = DIRECTION_ALL;
	iS32 Xmul , Ymul;
	iS32 MaxX = 0;
	iS32 MaxY = 0;
	iS32 MinX = 0;
	iS32 MinY = 0;
	iS32 Cx = C1.X;
	iS32 Cy = C1.Y;
	iS32 TempY = 0;
	iS32 TempX = 0;

	iVectInit1(&p1, Shape->p1);
	iVectInit1(&p2, Shape->p2);
	Xmul = p1.X - p2.X;
	Ymul = p1.Y - p2.Y;
	
//	if( AbsInt(Xmul)>=AbsInt(Ymul) )
	{
		//  圆心坐标需要在端点的X区域内或者Y区域内才计算
		if( ((Cx-p1.X)*(Cx-p2.X)>0) && ((Cy-p1.Y)*(Cy-p2.Y)>0) ) return iFALSE;
	}
//	else
	{
		//  圆心坐标需要在端点的Y区域内才计算
//		if( (Cy-p1.Y)*(Cy-p2.Y)>0 ) return iFALSE;
	}

	// 如果圆心到端点的距离小于圆半径则说明圆与线段有交点
	if( (p1.X-Cx)/COORDINATE_RATE*(p1.X-Cx)/COORDINATE_RATE+(p1.Y-Cy)/COORDINATE_RATE*(p1.Y-Cy)/COORDINATE_RATE < r/COORDINATE_RATE*r/COORDINATE_RATE 
	|| (p2.X-Cx)/COORDINATE_RATE*(p2.X-Cx)/COORDINATE_RATE+(p2.Y-Cy)/COORDINATE_RATE*(p2.Y-Cy)/COORDINATE_RATE < r/COORDINATE_RATE*r /COORDINATE_RATE
	)
	{
		if(Ymul==0)
		{
			TempY = p1.Y;
			TempX = Cx;
		}
		else if(Xmul==0) 
		{
			TempY = Cy;
			TempX = p1.X;
		}
		else
		{
			TempY = p1.Y - (Ymul)*(p1.X-Cx)/Xmul;
			TempX = p1.X - (Xmul)*(p1.Y-Cy)/Ymul;
		}
	
		DirectionNum++;
		if(  p1.Y==p2.Y || AbsInt((p1.X-p2.X)/(p1.Y-p2.Y))>5 )
		{				
			// 如果斜度很小，则认为是水平线，处理Y方向速度
			if( Cy<=TempY)
			{
				// 水滴在约束线上边
				Direction = DIRECTION_TOP | DIRECTION_RT | DIRECTION_RIGHT | DIRECTION_LEFT | DIRECTION_LT;
			}
			else
			{
				// 水滴在约束线下边
				Direction = DIRECTION_BOTTOM| DIRECTION_RB | DIRECTION_RIGHT | DIRECTION_LEFT | DIRECTION_LB;
			}
		}
		if(  p1.X==p2.X || AbsInt((p1.Y-p2.Y)/(p1.X-p2.X))>5 )
		{
			// 如果斜度很大，则认为是竖直线，处理X方向速度
			if( Cx>=TempX)
			{
				// 水滴在约束线右边
				Direction = DIRECTION_TOP | DIRECTION_RT| DIRECTION_RIGHT | DIRECTION_RB| DIRECTION_BOTTOM;
			}
			else
			{
				// 水滴在约束线左边
				Direction = DIRECTION_TOP | DIRECTION_LT| DIRECTION_LEFT| DIRECTION_LB| DIRECTION_BOTTOM;
			}
		}
		else 
		{
			if( p1.X>p2.X)
			{
				MaxX = p1.X;
				MinX = p2.X;
			}
			else
			{
				MaxX = p2.X;
				MinX = p1.X;
			}
			
			if( p1.Y>p2.Y)
			{
				MaxY = p1.Y;
				MinY = p2.Y;
			}
			else
			{
				MaxY = p2.Y;
				MinY = p1.Y;
			}

			if( Xmul*Ymul > 0 )
			{
				// 直线角度大于90°
//				if(Cx>C2.X)
//				if( Cx<MinX || (Cx>MinX&&Cx<MaxX) )
				if( Cx<TempX )
				{
					// 圆在直线左下方
					Direction = DIRECTION_BOTTOM| DIRECTION_RB| DIRECTION_RIGHT;
					if( AbsInt(Xmul)>AbsInt(Ymul) ) Direction |= DIRECTION_RB;
					else if( AbsInt(Xmul)<AbsInt(Ymul) ) Direction |= DIRECTION_LT;
					else Direction |= (DIRECTION_LT | DIRECTION_RB);
				}
//				else if( Cx>MinX || (Cx>MaxX) )
				else if( Cx>TempX)
				{
					// 圆在直线右上方
					Direction = DIRECTION_TOP| DIRECTION_RT| DIRECTION_RIGHT;
					if( AbsInt(Xmul)>AbsInt(Ymul) ) Direction |= DIRECTION_LT;
					else if( AbsInt(Xmul)<AbsInt(Ymul) ) Direction |= DIRECTION_RB;
					else Direction |= (DIRECTION_LT | DIRECTION_RB);
				}
				else
				{
					// 圆心与线段端点X坐标重合
					Direction = DIRECTION_ALL;
				}
			}
			else
			{
				// 直线角度小于90°
//				if(Cx<C2.X)
//				if( Cx<MinX || (Cx>MinX&&Cx<MaxX) )
				if( Cx<TempX )
				{
					// 圆在直线左上方
					Direction = DIRECTION_TOP| DIRECTION_LT| DIRECTION_LEFT;
					if( AbsInt(Xmul)>AbsInt(Ymul) ) Direction |= DIRECTION_RT;
					else if( AbsInt(Xmul)<AbsInt(Ymul) ) Direction |= DIRECTION_LB;
					else Direction |= (DIRECTION_RT | DIRECTION_LB);
				}
//				else if( Cx>MinX || (Cx>MaxX) )
				else if( Cx>TempX )
				{
					// 圆在直线右下方
					Direction = DIRECTION_BOTTOM| DIRECTION_RB| DIRECTION_RIGHT;
					if( AbsInt(Xmul)>AbsInt(Ymul) ) Direction |= DIRECTION_LB;
					else if( AbsInt(Xmul)<AbsInt(Ymul) ) Direction |= DIRECTION_RT;
					else Direction |= (DIRECTION_RT | DIRECTION_LB);
				}
				else
				{
					// 圆心与线段端点X坐标重合
					Direction = DIRECTION_ALL;
				} 
			}
		}
	}

	CurrDirection &= Direction;
	
	return iTRUE;
	
}

static iS32 iVectDistanceP2S(iVECT C, iShapeSeg *Shape )
{
#if 1
	// 计算点到直线的垂点坐标
	// 设直线方程为ax+by+c=0,点坐标为(m,n)   
	//  则点到直线的距离为:(a*m+b*n+c)/(a*a+b*b)
	// 这个“距离”有符号，表示点在直线的上方或者下方，取绝对值表示欧式距离。
	iS32 a = Shape->a;
	iS32 b = Shape->b;
	iS32 c = Shape->c;
	iS32 m = C.X;
	iS32 n = C.Y;

	return AbsInt((a*m+b*n+c)*10/SqrtInt((a*a+b*b)*100));
	
#else
	// 计算点到直线的距离
	// p1,p2,直线上的两个点
	iVECT D;
	iS32 Distance = 0;

	if( p1.Y == p2.Y ) return AbsInt(C.Y-p1.Y);
	if( p1.X == p2.X ) return AbsInt(C.X-p1.X);

	// 求过点C平行于X轴的直线与过p1p2两点的直线的交点
	D.Y = C.Y;
	D.X = (p1.X-p2.X)*(D.Y-p2.Y)/(p1.X-p2.X) + p2.X;

	Distance = (p2.Y-p1.Y)*(D.X-C.X)/iVectDistanceP2P(p1, p2);
	
	return SqrtInt(Distance);
#endif	
}

static iBOOL i51KitP2P2Sintersect( iVECT v, iShapeSeg * Shape )
{
	// 判断点到直线的垂直是否在线段内
	iS32 m = v.X/COORDINATE_RATE;
	iS32 n = v.Y/COORDINATE_RATE;
	iVECT p1,p2;

	p1.X = Shape->p1.X/COORDINATE_RATE;
	p1.Y = Shape->p1.Y/COORDINATE_RATE;
	p2.X = Shape->p2.X/COORDINATE_RATE;
	p2.Y = Shape->p2.Y/COORDINATE_RATE;

	if( (m-p1.X)*(m-p2.X)>0 && (n-p1.Y)*(n-p2.Y)>0 ) return iFALSE;

	return iTRUE;


}
static iS32 i51KitP2GetDistanceP2S( iVECT v, iShapeSeg * Shape )
{
	// 计算点到直线距离
	// 设直线方程为ax+by+c=0,点坐标为(m,n)   
	// 则点到直线的距离为:(a*m+b*n+c)/sqrt(a*a+b*b)
	// 如果点和线段没交点，则返回-1
	
	iS32 a = Shape->a;
	iS32 b = Shape->b;
	iS32 c = Shape->c;
	iS32 m = v.X;
	iS32 n = v.Y;
	iS32 d = -1;
	iS32 Temp = 0;
	iS32 Temp1 = 0;
	iVECT p1,p2;

	iVectInit1(&p1, Shape->p1);
	iVectInit1(&p2, Shape->p2);

//	Temp = a*m;
//	Temp += b*n;
//	Temp += c;
//	Temp1 = (iS32)SqrtInt(a*a+b*b);
	d = (a*m+b*n+c)*100/((iS32)SqrtInt((a*a+b*b)*10000));

	return AbsInt(d);
	
}

iHANDLE i51KitP2CreateWorld( iRECT * WorldRect, iVECT * Gravity)
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iRECT * WorldRect : 世界大小
	// iVECT * Gravity : 世界的重力

	// Notes : 初始化世界

	iWorld * WorldSpace = iNULL;
	iU32 Length = 0;

	WorldSpace = ( iWorld *)i51KitP2Malloc(sizeof(iWorld));
	if( iNULL == WorldSpace )
	{
		iLog("KIT - P2 : malloc create");
		return iFALSE;
	}

	memset ( WorldSpace, 0 , sizeof(iWorld)) ;

	if( WorldRect )
	{
		WorldSpace->WorldRect.X = WorldRect->X;
		WorldSpace->WorldRect.Y = WorldRect->Y;
		WorldSpace->WorldRect.Width = WorldRect->Width;
		WorldSpace->WorldRect.Height = WorldRect->Height;
	}
	else
	{
		WorldSpace->WorldRect.X = -1000;
		WorldSpace->WorldRect.Y = -1000;
		WorldSpace->WorldRect.Width = 2000;
		WorldSpace->WorldRect.Height = 2000;
	}

	WorldSpace->ShapeContainer.MaxNode = DEFAULT_SHAPE_NUM;
	WorldSpace->ShapeContainer.IdlyNode = DEFAULT_SHAPE_NUM;
	WorldSpace->ShapeContainer.UseNode = 0;
	Length = sizeof(Node)*DEFAULT_SHAPE_NUM;
	WorldSpace->ShapeContainer.NodeHandle = ( Node*)i51KitP2Malloc(Length);
	if( iNULL == WorldSpace->ShapeContainer.NodeHandle )
	{
		iLog("KIT - P2 : malloc shape");
		i51AdeOsFree(WorldSpace);
		return iFALSE;
	}
	memset ( WorldSpace->ShapeContainer.NodeHandle, 0 , sizeof(Node)*DEFAULT_SHAPE_NUM) ;
	
	WorldSpace->BodyContainer.MaxNode = DEFAULT_BODY_NUM;
	WorldSpace->BodyContainer.IdlyNode = DEFAULT_BODY_NUM;
	WorldSpace->BodyContainer.UseNode = 0;
	Length = sizeof(Node)*DEFAULT_BODY_NUM;
	WorldSpace->BodyContainer.NodeHandle = ( Node*)i51KitP2Malloc(Length);
	if( iNULL == WorldSpace->BodyContainer.NodeHandle )
	{
		iLog("KIT - P2 : malloc body");
		i51AdeOsFree(WorldSpace->BodyContainer.NodeHandle);
		i51AdeOsFree(WorldSpace);
		return iFALSE;
	}
	memset ( WorldSpace->BodyContainer.NodeHandle, 0 , sizeof(Node)*DEFAULT_BODY_NUM) ;
	
	if( Gravity )
	{
		WorldSpace->WorldGravity.X = Gravity->X;
		WorldSpace->WorldGravity.Y = Gravity->Y;
	}
	else
	{
		WorldSpace->WorldGravity.X = 0;
		WorldSpace->WorldGravity.Y = 0;
	}

	WorldSpace->Stamp = 1;
	WorldSpace->Steps = 1;

	WorldSpace->StaticBody.Mass = MAX_MASS;

	return (iHANDLE)(WorldSpace);
	
}

iBOOL i51KitP2ReleaseWorld( iHANDLE Space)
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE World : 待释放的世界指针

	iWorld * World = (iWorld*)Space;
	NodeContainer * Container = iNULL;
	Node * NodeHandle = iNULL;
	iS32 MaxNode = 0;
	iS32 Temp = 0;
	
	if( Space )
	{
		Container = &(World->BodyContainer);
		MaxNode = Container->MaxNode;
		NodeHandle = Container->NodeHandle;
		for(Temp=0; Temp<MaxNode; Temp++ )
		{
			if( NodeHandle[Temp].Flag ) i51AdeOsFree(NodeHandle[Temp].Handle);
		}
		i51AdeOsFree(World->BodyContainer.NodeHandle);

		Container = &(World->ShapeContainer);
		MaxNode = Container->MaxNode;
		NodeHandle = Container->NodeHandle;
		for(Temp=0; Temp<MaxNode; Temp++ )
		{
			if( NodeHandle[Temp].Flag ) i51AdeOsFree(NodeHandle[Temp].Handle);
		}		
		i51AdeOsFree(World->ShapeContainer.NodeHandle);

		i51AdeOsFree(Space);
		
	}
	else
	{
		iLog("KIT - P2 : release null");
		return iFALSE;
	}
	
	return iTRUE;

}

iBOOL i51KitP2ResetWorld( iHANDLE Space, iRECT * WorldRect, iVECT * Gravity)
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE World : 世界指针
	// iRECT * WorldRect : 世界大小
	// iVECT * Gravity : 世界的重力

	// Notes : 重置世界大小和重力系统

	iWorld * WorldSpace = (iWorld*)Space;

	if( iNULL == WorldSpace )
	{
		iLog("KIT - P2 : reset null");
		return iFALSE;
	}

	if( WorldRect )
	{
		WorldSpace->WorldRect.X = WorldRect->X;
		WorldSpace->WorldRect.Y = WorldRect->Y;
		WorldSpace->WorldRect.Width = WorldRect->Width;
		WorldSpace->WorldRect.Height = WorldRect->Height;
	}

	if( Gravity )
	{
		WorldSpace->WorldGravity.X = Gravity->X;
		WorldSpace->WorldGravity.Y = Gravity->Y;
	}

	return iTRUE;
	
}

iBOOL i51KitP2ResetWorldTimeStamp( iHANDLE Space,  iS32 Stamp , iS32 Steps )
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Space : 世界指针
	// iS32 Stamp : 更新世界的时间间隔
	// iS32 Steps : 每次更新世界，世界中的物体的迭代次数

	iWorld * WorldSpace = (iWorld*)Space;

	if( iNULL == WorldSpace )
	{
		iLog("KIT - P2 : reset null");
		return iFALSE;
	}


	WorldSpace->Stamp = Stamp;
	WorldSpace->Steps = Steps;

	return iTRUE;
	
}

static iBOOL i51KitP2CalForceCircle2Circle( iShapeCircle * Shape1 , iShapeCircle* Shape2 )
{
	iVECT TempF;
	iVECT TempV;
	iVECT C1, C2;
	iS32 R1, R2;
	iS32 Distance = 0;
	iS32 Inset = 0;
	iS32 HalfInset = 0;
	iBody * B1, *B2;
	// 碰撞的两个小球分别在圆心连线和碰撞法线上的速度
	iS32 V1f, V1l, V2f, V2l;
	iS32 Temp = 0;
	iS32 TempX = 0;
	iS32 TempY = 0;
	iS32 Sina = 0;
	iS32 Cosa = 0;

	B1 = (iBody*)(((Shape1->Shape).Body));
	B2 = (iBody*)(((Shape2->Shape).Body));


	R1 = Shape1->Radius;
	R2 = Shape2->Radius;

	// 计算两个圆的圆心距离
	iVectInit1(&C1, Shape1->CentrePoint);
	iVectInit1(&C2, Shape2->CentrePoint);
	Distance = iVectDistanceP2P(C1,C2);
	
	if( (Distance>=(R1+R2+2))) return iTRUE;

	// 计算两个圆的相嵌像素
	Inset = R1+R2 - Distance+2;

	if(  Distance<=0 ) return iTRUE;

	// 计算两球的调整速度
	B1->v_bias.X  += (C1.X - C2.X)*Inset/Distance;
	B2->v_bias.X  += (C2.X - C1.X)*Inset/Distance;

	B1->v_bias.Y  += (C1.Y - C2.Y)*Inset/Distance;
	B2->v_bias.Y  += (C2.Y - C1.Y)*Inset/Distance;
	
	// 两球速度相等，则返回
	if( iVectEqual( B1->V, B2->V ) )
	{
		return iTRUE;
	}

	if( B1->V.Y==B2->V.Y )
	{
		// 两球在Y方向速度相等，则交换X方向的速度
		Temp = B2->V.X;
		B2->V.X = B1->V.X;
		B1->V.X = Temp;
	}
	else if( B1->V.X==B2->V.X )
	{
		// 两球在X方向速度相等，则交换Y方向的速度
		Temp = B2->V.Y;
		B2->V.Y = B1->V.Y;
		B1->V.Y = Temp;
	}
	else
	{
		iVectInit2(&(B1->V), (B1->V.X+B2->V.X)/2, (B1->V.Y+B2->V.Y)/2);
		iVectInit1(&(B2->V), B1->V);
		return iTRUE;

		// 两球碰撞线不平行于坐标轴
		// Vf = vx*sina + vy*cosa
		// Vl = vx*cosa + vy*sina
		// 其中a是两球圆心的连线与坐标轴X轴的夹角
		// sina = TempY/Distance;cosa = TempX/Distance;
		// 两球碰撞后连接线方向交换速度，法线方向速度不变
		if( (C1.X>C2.X && C1.Y<C2.Y) || (C1.X<C2.X && C1.Y>C2.Y) )
		{
			TempX = AbsInt((C1.X-C2.X));
		}
		else
		{
			TempX = -(AbsInt((C1.X-C2.X)));
		}
		TempY = AbsInt((C1.Y-C2.Y));

		Sina = TempY/Distance;
		Cosa = TempX/Distance;
		
		V1f = B1->V.X*Sina + B1->V.Y*Cosa;
		V1l = B1->V.X*Cosa + B1->V.Y*Sina;

		V2f = B2->V.X*Sina + B2->V.Y*Cosa;
		V2l = B2->V.X*Cosa + B2->V.Y*Sina;

		B1->V.X = (V2l*Cosa + V1f*Sina );
		B1->V.Y = (V2l*Sina - V1f*Cosa);
		
		B2->V.X = (V1l*Cosa + V2f*Sina) ;
		B2->V.Y = (V1l*Sina - V2f*Cosa);

	}

#if 0	
	// 计算两个圆的相嵌像素
	Inset = R1+R2 - Distance;

	// 计算两个圆的相对速度	
	B1 = (iBody*)(((Shape1->Shape).Body));
	B2 = (iBody*)(((Shape2->Shape).Body));
	iVectSub(&TempV, B1->V, B2->V);

	// 计算两个圆的相互作用力
	iVectMult(&TempF, TempV, Inset );

	iBodyForce(&(B1->ActingForce), TempF, C1, C2);
	iBodyForce(&(B2->ActingForce), TempF, C2, C1);
#endif

	return iTRUE;
	
}

static iBOOL i51KitP2CalForceCircle2Seg( iShapeCircle * CircleShape , iShapeSeg* SegShape )
{
#if 1
	return iTRUE;
#else
	iVECT TempF;
	iVECT TempV;
	iVECT C1, C2;
	iS32 R1, R2;
	iS32 Distance = 0;
	iS32 Inset = 0;
	iBody * B1, *B2;
	
	R1 = CircleShape->Radius;
	R2 = SegShape->Thickness;

	// 计算圆心到直线的距离
	Distance = i51KitP2GetDistanceP2S(C1, SegShape);
	
	if( Distance<0 || Distance>=(R1+R2) ) return iTRUE;

	// 计算两个圆的相嵌像素
	Inset = R1+R2 - Distance;

	// 计算两个圆的相对速度	
	B1 = (iBody*)(((CircleShape->Shape).Body));
	B2 = (iBody*)(((SegShape->Shape).Body));
	iVectSub(&TempV, B1->V, B2->V);

	// 计算两个圆的相互作用力
	iVectMult(&TempF, TempV, Inset );

	iBodyForce(&(B1->ActingForce), TempF, C1, C2);
	iBodyForce(&(B2->ActingForce), TempF, C2, C1);

	return iFALSE;
#endif

}

static iBOOL i51KitP2CalForceSeg2Seg( iShapeSeg * Shape1 , iShapeSeg* Shape2 )
{
	return iTRUE;
}

static iBOOL i51KitP2CalInteractionalForce( iShape * Shape1 , iShape * Shape2 )
{
	iShapeType S1 = Shape1->ShapeType;
	iShapeType S2 = Shape2->ShapeType;

	if( S1==I51_CIRCLE_SHAPE && S2 == I51_CIRCLE_SHAPE ) 
	{
		i51KitP2CalForceCircle2Circle( (iShapeCircle *) Shape1, (iShapeCircle *) Shape2);
	}
	else if( S1==I51_CIRCLE_SHAPE && S2 == I51_SEGMENT_SHAPE )
	{
		i51KitP2CalForceCircle2Seg( (iShapeCircle *) Shape1, (iShapeSeg*) Shape2);
	}
	else if( S1==I51_SEGMENT_SHAPE && S2 == I51_CIRCLE_SHAPE )
	{
		i51KitP2CalForceCircle2Seg( (iShapeCircle *) Shape2, (iShapeSeg*) Shape1);
	}
	else if( S1==I51_SEGMENT_SHAPE && S2 == I51_SEGMENT_SHAPE)
	{
		i51KitP2CalForceSeg2Seg( (iShapeSeg *) Shape1, (iShapeSeg*) Shape2);
	}
	else
	{
		return iFALSE;
	}

	return iTRUE;

}

static iBOOL i51KitP2CalGravity( iBody * body, iVECT Gravity )
{
	//计算物体的重力
	body->GravityForce.X = body->Mass*(Gravity.X);
	body->GravityForce.Y = body->Mass*(Gravity.Y);
	body->ActingForce.X += body->Mass*(Gravity.X);
	body->ActingForce.Y += body->Mass*(Gravity.Y);

	return iTRUE;

}

static iBOOL i51KitP2CalActingForce( NodeContainer * BodyContainer  )
{
	// 计算运动物体之间的相互作用力
	iS32 MaxNode = 0;
	iS32 UseNode = 0;
	iS32 IdlyNode = 0;
	iS32 Temp = 0;
	iS32 Temp1 = 0;
	iS32 TempUse = 0;
	iShape * CalShape = iNULL;
	iShape * TempShape = iNULL;
	iBody * CalBody = iNULL;
	iBody * TempBody = iNULL;
	Node * NodeHandle = iNULL;

	MaxNode = BodyContainer->MaxNode;
	UseNode = BodyContainer->UseNode;
	IdlyNode = BodyContainer->IdlyNode;
	NodeHandle = BodyContainer->NodeHandle;
	TempUse = 0;
	
	for( Temp=0; Temp<MaxNode; Temp++)
	{
		if( NodeHandle[Temp].Flag )
		{
			TempUse++;
			CalBody = (iBody*)NodeHandle[Temp].Handle;
			CalShape = CalBody->Shape;
			i51KitP2CalGravity(CalBody, mwSpace->WorldGravity);
			for( Temp1=Temp+1; Temp1<MaxNode; Temp1++ )
			{
				if( NodeHandle[Temp1].Flag )
				{
					TempBody= (iBody*)(NodeHandle[Temp1].Handle);
					TempShape = TempBody->Shape;
					i51KitP2CalInteractionalForce(CalShape,TempShape);
				}
			}

			if( TempUse==UseNode )
			{
				break;
			}

		}
	}
	
	if( TempUse!=UseNode )
	{
		iLog("i51KitP2CalActingForce:TempUse:%d,UseNode:%d",TempUse,UseNode);
	}

	return iFALSE;
	
}

static iBOOL i51KitP2UpdateBiasV( iVECT CurrCenter, iVECT * BiasV, iVECT *V )
{	
	//CurrCenter :当前圆心坐标
	//BiasV : 调整速度指针
	//BiasV : 水滴运动速度指针
	// notes : 如果当前圆所在的位置已经不在空白区域，则使用调整速度，使得圆回到空白区域

	iS32 Cx,Cy;
	iS32 Xmul = 0;
	iS32 Ymul = 0;
//	iS32 Count = 0;
	iS32 i = 0, Temp = 0;
	// 左右上下、左上、右上、左下右下
	iS32 Xmultiplier[8] = {-1,1,0,0,-1,1,-1,1};
	iS32 Ymultiplier[8] = {0,0,-1,1,-1,-1,1,1};
	
	Cx =CurrCenter.X/2/COORDINATE_RATE;
	Cy = CurrCenter.Y/2/COORDINATE_RATE;

	if( Cx<0 || Cy<0 || Cx>=SCREEN_WIDTH_HALF || Cy>=SCREEN_HEIGHT_HALF ) return iTRUE;

	if( WORD_SPACE==mwWorldAttribute[Cy][Cx] )  return iFALSE;

	while(1)
	{
		Temp++;
//		Count = 0;

		for( i=0; i<8; i++ )
		{
			Xmul = Temp*Xmultiplier[i];
			Ymul = Temp*Ymultiplier[i];
			if( Cy+Ymul<0 || Cy+Ymul >= SCREEN_HEIGHT_HALF || Cx+Xmul<0 || Cx+Ymul >= SCREEN_WIDTH_HALF )
			{
				continue;
//				Count++;
			}
			else if( WORD_SPACE==mwWorldAttribute[Cy+Ymul][Cx+Xmul] ) 
			{
				break;
			}
		}

		if( i!=8 ) break;
		
	}

	if( Xmul*V->X>0 )
	{
		BiasV->X = Xmul*14*COORDINATE_RATE/3;
	}
	else if( Xmul!=0 )
	{
		if( (Cx-Xmul>=0) && (Cx-Xmul < SCREEN_WIDTH_HALF) && (WORD_SPACE==mwWorldAttribute[Cy+Ymul][Cx-Xmul]) )
		{
			BiasV->X = -Xmul*14*COORDINATE_RATE/3;
			V->X = -Xmul/AbsInt(Xmul)*300;
		}
		else
		{
			BiasV->X = Xmul*14*COORDINATE_RATE/3;
			V->X = Xmul/AbsInt(Xmul)*300;
		}
	}

	if( Ymul*V->Y>0 )
	{
		BiasV->Y = Ymul*14*COORDINATE_RATE/3;
	}
	else if( Ymul!=0 )
	{
		if( (Cy-Ymul>=0) && (Cy-Ymul < SCREEN_HEIGHT_HALF) && (WORD_SPACE==mwWorldAttribute[Cy-Ymul][Cx+Xmul]) )
		{
			BiasV->Y = -Ymul*14*COORDINATE_RATE/3;
			V->Y = -Ymul/AbsInt(Ymul)*300;
		}
		else
		{
			BiasV->Y = Ymul*14*COORDINATE_RATE/3;
			V->Y = Ymul/AbsInt(Ymul)*300;
		}
	}
		
	return iTRUE;
	
}

static iBOOL i51KitP2UpdateForceCircle2Seg( iShapeCircle * CircleShape, iShapeSeg * SegShape )
{
	// iShapeCircle * CircleShape : 运动的球体
	// iShapeSeg * SegShape: 静止的线段
	iVECT TempF;
	iVECT TempV;
	iVECT C1, C2,Next;
	iVECT Clast;
	// 线段的端点坐标
	iVECT p1,p2;
	iS32 R1, R2;
	iS32 Distance = 0;
	iS32 NextDistance = 0;
	iS32 Temp = 0;
	iS32 Inset = 0;
	iBody * B1 = iNULL;//, *B2;
	iS32 Xmul = 0;
	iS32 Ymul = 0;
	iU8	ShapeType = Gridding_TYPE_SPACE;

	iU32 Direction = DIRECTION_NO;
	iS32 TempY = 0;
	iS32 Cy = 0;
	iS32 TempX = 0;
	iS32 Cx = 0;
	
	R1 = CircleShape->Radius;
	R2 = SegShape->Thickness;

	iVectInit1(&C1, CircleShape->CentrePoint);
	
	// 计算圆心到直线的垂足
	if( iFALSE ==	iVectGetP2SegPedal(&C2, C1, SegShape) )
//	if( iFALSE ==	iVectGetP2SegVerticalFellowship( C1, SegShape) )
	{
		// 如果垂点不在线段内，则返回	
		iVectGetP2SegIntersectant(C1,R1+R2, SegShape);
		return iTRUE;
	}
		
	Distance = iVectDistanceP2P(C1, C2);
//	Distance = iVectDistanceP2S(C1, SegShape);

#if 0
	if( iFALSE== i51KitP2P2Sintersect(C1, SegShape) )
	{
		return iTRUE;
	}
	Distance = i51KitP2GetDistanceP2S(C1, SegShape);
#endif

	// 点P（x0 ,yo）在直线Ax+By+C=0 (A2+B210)的位置关系是：
	// 若点在直线上，则Ax0+By0+C=0；若点在直线外，
	// 则当点P在直线上方且B >0时，有Ax0+By0+C >0，
	// 当点P在直线下方且B >0时，有Ax0+By0+C < 0	
	if( Distance>=(R1+R2) ) return iTRUE;

	DirectionNum++;
	
	// 计算两个圆的相嵌像素
	Inset = R1+R2 - Distance;

	B1 = CircleShape->Shape.Body;

	iVectInit1(&p1, SegShape->p1);
	iVectInit1(&p2, SegShape->p2);
	iVectInit1(&Clast, CircleShape->LastPoint);

	Xmul =(p1.X -p2.X)/10;
	Ymul =(p1.Y -p2.Y)/10;

	Cx = C1.X;
	Cy = C1.Y;
	
	if(Ymul==0)
	{
		TempY = p1.Y;
		TempX = Cx;
	}
	else if(Xmul==0) 
	{
		TempY = Cy;
		TempX = p1.X;
	}
	else
	{
		TempY = p1.Y - (Ymul)*(p1.X-Cx)/Xmul;
		TempX = p1.X - (Xmul)*(p1.Y-Cy)/Ymul;
	}

	// 计算圆与静止直线碰撞后的调整速度

	if(  p1.Y==p2.Y || AbsInt(Xmul/Ymul)>5 )
	{				
		// 如果斜度很小，则认为是水平线，处理Y方向速度
		if( C1.Y<C2.Y )
		{
			// 水滴在约束线上边
			if(B1->V.Y>0) B1->V.Y = 0;
			if(DirectionNum==1)
			{
				B1->v_bias.Y = -Inset;
			}
			else
			{
				B1->v_bias.Y -= Inset;
			}
			B1->ActingForce.Y = 0;
			Direction = DIRECTION_TOP | DIRECTION_RT | DIRECTION_RIGHT | DIRECTION_LEFT | DIRECTION_LT;
		}
		else if( C1.Y>C2.Y )
		{
			// 水滴在约束线下边
			if(B1->V.Y<0) B1->V.Y = 0;
			if(DirectionNum==1)
			{
				B1->v_bias.Y = Inset;
			}
			else
			{
				B1->v_bias.Y += Inset;
			}
			
			Direction = DIRECTION_BOTTOM| DIRECTION_RB | DIRECTION_RIGHT | DIRECTION_LEFT | DIRECTION_LB;
		}
		else
		{
			// 两个圆心已经重合
			if( Clast.X>C1.X ) B1->v_bias.X += Inset;
			else B1->v_bias.X -= Inset;

			if( Clast.Y>=C1.Y ) B1->v_bias.Y += Inset;
			else B1->v_bias.Y -= Inset;

			B1->ActingForce.X = 0;
			B1->ActingForce.Y = 0;
			B1->V.X /= 2;
			B1->V.Y /= 2;

			Direction = DIRECTION_ALL;
		}
		
		if( AbsInt(B1->V.X)<200 && DirectionNum==1)
		{
			if( Xmul*Ymul< 0 ) B1->V.X = -200;
			else if( Xmul*Ymul> 0 ) B1->V.X = 200;
		}

		CurrDirection &= Direction;

	if( CurrDirection==DIRECTION_NO )
	{
		iLog(" direction is no");
		return iFALSE;
	}		
		return iTRUE;

	}
	else if(  p1.X==p2.X || AbsInt((p1.Y-p2.Y)/(p1.X-p2.X))>5 )
	{
		// 如果斜度很大，则认为是竖直线，处理X方向速度
		if( C1.X>C2.X )
		{
			// 水滴在约束线右边
			B1->V.X = AbsInt(B1->V.X)/2;

			if(DirectionNum==1)
			{
				B1->v_bias.X = Inset;
			}
			else
			{
				B1->v_bias.X += Inset;
			}
			Direction = DIRECTION_TOP | DIRECTION_RT| DIRECTION_RIGHT | DIRECTION_RB| DIRECTION_BOTTOM;
		}
		else if( C1.X<C2.X )
		{
			// 水滴在约束线左边
			B1->V.X = -(AbsInt(B1->V.X)/2);
			if(DirectionNum==1)
			{
				B1->v_bias.X = -Inset;
			}
			else
			{
				B1->v_bias.X -= Inset;
			}
			Direction = DIRECTION_TOP | DIRECTION_LT| DIRECTION_LEFT| DIRECTION_LB| DIRECTION_BOTTOM;
		}
		else
		{
			// 两个圆心已经重合
			if( Clast.X>C1.X ) B1->v_bias.X += Inset;
			else B1->v_bias.X -= Inset;

			if( Clast.Y>=C1.Y ) B1->v_bias.Y += Inset;
			else B1->v_bias.Y -= Inset;

			B1->ActingForce.X = 0;
			B1->ActingForce.Y = 0;
			B1->V.X /= 2;
			B1->V.Y /= 2;

			Direction = DIRECTION_ALL;
		}

		CurrDirection &= Direction;
		
		return iTRUE;

	}
	else if( Distance!=0)
	{
		if(DirectionNum==1)
		{
			if(C1.X>=C2.X )  B1->v_bias.X = Inset;
			else B1->v_bias.X = -Inset;

			if( C1.Y<=C2.Y ) B1->v_bias.Y = -Inset;
			else B1->v_bias.Y = Inset;
		}			
		else
		{
			if(C1.X>=C2.X )  B1->v_bias.X += Inset;
			else B1->v_bias.X -= Inset;

			if( C1.Y<=C2.Y ) B1->v_bias.Y -= Inset;
			else B1->v_bias.Y += Inset;
		}
//		B1->v_bias.X  = (C1.X - C2.X)*Inset/Distance;
//		B1->v_bias.Y  = (C1.Y - C2.Y)*Inset/Distance;

		if( Xmul*Ymul > 0 )
		{
		 // 直线角度大于90°
			if(C1.X>C2.X)
			{
				// 圆在直线右上方
				Direction = DIRECTION_TOP| DIRECTION_RT| DIRECTION_RIGHT;
				if( AbsInt(Xmul)>AbsInt(Ymul) ) Direction |= DIRECTION_LT;
				else if( AbsInt(Xmul)<AbsInt(Ymul) ) Direction |= DIRECTION_RB;
				else Direction |= (DIRECTION_LT | DIRECTION_RB);
			}
			else
			{
				// 圆在直线左下方
				Direction = DIRECTION_BOTTOM| DIRECTION_LB| DIRECTION_LEFT;
				if( AbsInt(Xmul)>AbsInt(Ymul) ) Direction |= DIRECTION_RB;
				else if( AbsInt(Xmul)<AbsInt(Ymul) ) Direction |= DIRECTION_LT;
				else Direction |= (DIRECTION_LT | DIRECTION_RB);
			}
		}
		else
		{
			if(C1.X<C2.X)
			{
				// 圆在直线左上方
				Direction = DIRECTION_TOP| DIRECTION_LT| DIRECTION_LEFT;
				if( AbsInt(Xmul)>AbsInt(Ymul) ) Direction |= DIRECTION_RT;
				else if( AbsInt(Xmul)<AbsInt(Ymul) ) Direction |= DIRECTION_LB;
				else Direction |= (DIRECTION_RT | DIRECTION_LB);
			}
			else
			{
				// 圆在直线右下方
				Direction = DIRECTION_BOTTOM| DIRECTION_RB| DIRECTION_RIGHT;
				if( AbsInt(Xmul)>AbsInt(Ymul) ) Direction |= DIRECTION_LB;
				else if( AbsInt(Xmul)<AbsInt(Ymul) ) Direction |= DIRECTION_RT;
				else Direction |= (DIRECTION_RT | DIRECTION_LB);
			}
		}

	}
	else
	{
		// 两个圆心已经重合
		if( Clast.X>C1.X ) B1->v_bias.X += Inset;
		else B1->v_bias.X -= Inset;

		if( Clast.Y>=C1.Y ) B1->v_bias.Y += Inset;
		else B1->v_bias.Y -= Inset;

		B1->ActingForce.X = 0;
		B1->ActingForce.Y = 0;
		B1->V.X /= 2;
		B1->V.Y /= 2;

		Direction = DIRECTION_ALL;

		return iTRUE;
	}

	CurrDirection &= Direction;

	// 更新物体的速度，如果速度导致物体之间越来越嵌入，则需要重新计算速度
	iVectInit1(&TempV, B1->V);
	if(  TempV.X>0 ) Temp = 10;
	else Temp = -10;
	Next.X = C1.X + Temp;
	if( TempV.X!=0 )
	{
		Next.Y = C1.Y + Temp*TempV.Y/ TempV.X;
	}
	else
	{
		Next.X = C1.X;
		if( TempV.Y>0 )
		{
			Next.Y = C1.Y+10;
		}
		else if( TempV.Y<0 )
		{
			Next.Y = C1.Y-10;
		}
	}
	
	// 计算下一点到直线的距离
	Distance = i51KitP2GetDistanceP2S(Next, SegShape);
//	iVectInit1(&TempV, B1->V);
//	if( (C2.X-C1.X)*TempV.X>=0 && (C2.Y-C1.Y)*TempV.Y>=0 )
	if( NextDistance<=Distance || NextDistance<=0 || Distance<=0 )
	{
		if( SegShape->p1.X == SegShape->p2.X ) TempV.X = -TempV.X/2;
		else if( SegShape->p1.Y == SegShape->p2.Y ) TempV.Y = 0;
		else if( AbsInt(TempV.Y)>= 500 )
		{
//			TempV.Y = TempV.Y*Ymul*Ymul/(Xmul*Xmul+Ymul*Ymul)*5/6;
//			TempV.Y = TempV.Y*Ymul*Ymul/(Xmul*Xmul+Ymul*Ymul);
			TempV.X = Xmul*(TempV.Y)/Ymul;
		}
		else
		{
			TempV.X = TempV.X*Xmul*Xmul/(Xmul*Xmul+Ymul*Ymul);
			TempV.Y = Ymul*(TempV.X)/Xmul;
		}
	}

	iVectInit1(&(B1->V), TempV);

	// 更新物体受到的合力，如果合力导致物体之间越来越嵌入，则需要重新计算合力
	iVectInit1(&TempV, B1->ActingForce);
	if(  TempV.X>0 ) Temp = 10;
	else Temp = -10;
	Next.X = C1.X + Temp;
	if( TempV.X!=0 )
	{
		Next.Y = C1.Y + Temp*TempV.Y/ TempV.X;
	}
	else
	{
		Next.X = C1.X;
		if( TempV.Y>0 )
		{
			Next.Y = C1.Y+10;
		}
		else if( TempV.Y<0 )
		{
			Next.Y = C1.Y-10;
		}
	}
	
	// 计算下一点到直线的距离
	Distance = i51KitP2GetDistanceP2S(Next, SegShape);
//	iVectInit1(&TempV, B1->V);
//	if( (C2.X-C1.X)*TempV.X>=0 && (C2.Y-C1.Y)*TempV.Y>=0 )
	if( NextDistance<=Distance || NextDistance<=0 || Distance<=0 )
//	if( (C2.X-C1.X)*B1->ActingForce.X>=0 && (C2.Y-C1.Y)*B1->ActingForce.Y>=0 )
	{
		if( p1.X == p2.X ) B1->ActingForce.X = 0;
		else if( p1.Y == p2.Y ) B1->ActingForce.Y = 0;
		else if( B1->ActingForce.Y!=0)
		{
			B1->ActingForce.Y = B1->ActingForce.Y*Ymul*Ymul/(Xmul*Xmul+Ymul*Ymul);
			B1->ActingForce.X = Xmul*(B1->ActingForce.Y)/Ymul;
		}
		else
		{
			B1->ActingForce.X = 0;
		}
	}

	if( p1.Y!=p2.Y && AbsInt((p1.X-p2.X)/(p1.Y-p2.Y))>3 && AbsInt(B1->V.X) <200 && DirectionNum==1 )
	{				
		// 如果斜度很小，则强制给予速度
		if( (p1.X-p2.X)*(p1.Y-p2.Y) < 0 ) B1->V.X = -200;
		else if( (p1.X-p2.X)*(p1.Y-p2.Y) > 0 ) B1->V.X = 200;
	}

	return iTRUE;
	
}

static iBOOL i51KitP2UpdateOneShapeForce(iShape *Shape1, iShape * Shape2 )
{
	iShapeType S1 = Shape1->ShapeType;
	iShapeType S2 = Shape2->ShapeType;

	if( S1==I51_CIRCLE_SHAPE && S2 == I51_SEGMENT_SHAPE )
	{
		i51KitP2UpdateForceCircle2Seg( (iShapeCircle *) Shape1, (iShapeSeg*) Shape2);
	}
	else
	{
		return iFALSE;
	}

	return iTRUE;

}

static iBOOL i51KitP2UpdateForce( NodeContainer *BodyContainer, NodeContainer * ShapeContainer )
{
	// 修正物体的合力和速度
	iBody * B = iNULL;
	iVECT v,v_bias;

	iS32 BodyMaxNode = 0;
	iS32 BodyUseNode = 0;
	iS32 BodyIdlyNode = 0;
	iS32 BodyTempUse = 0;
	Node * BodyNodeHandle = iNULL;
	iS32 Temp = 0;
	iS32 Temp1 = 0;
	iS32 ShapeMaxNode = 0;
	iS32 ShapeUseNode = 0;
	iS32 ShapeIdlyNode = 0;
	iS32 ShapeTempUse = 0;
	Node * ShapeNodeHandle = iNULL;
	iShape * CalShape = iNULL;
	iShape * TempShape = iNULL;
	iBody * CalBody = iNULL;
	iBody * TempBody = iNULL;
	iU32 DirectionV = DIRECTION_NO;
	iU32 DirectionV1 = DIRECTION_NO;
	iU32 DirectionV2 = DIRECTION_NO;
	iU32 DirectionH = DIRECTION_NO;

	iS32 Vx, Vy;
	iS32 Cx, Cy;

	BodyMaxNode = BodyContainer->MaxNode;
	BodyUseNode = BodyContainer->UseNode;
	BodyIdlyNode = BodyContainer->IdlyNode;
	BodyNodeHandle = BodyContainer->NodeHandle;
	BodyTempUse = 0;
	
	ShapeMaxNode = ShapeContainer->MaxNode;
	ShapeUseNode = ShapeContainer->UseNode;
	ShapeIdlyNode = ShapeContainer->IdlyNode;
	ShapeNodeHandle = ShapeContainer->NodeHandle;
	ShapeTempUse = 0;

//	while( BodyTempUse<BodyUseNode )
	for( Temp=0; Temp<BodyMaxNode; Temp++ )
	{
		if( BodyNodeHandle[Temp].Flag )
		{
			BodyTempUse++;
			CalBody = (iBody*)BodyNodeHandle[Temp].Handle;
			CalShape = CalBody->Shape;
			CurrDirection = DIRECTION_ALL;
			DirectionNum = 0;
			for( Temp1=0; Temp1<ShapeMaxNode; Temp1++ )
			{
				if( ShapeNodeHandle[Temp1].Flag )
				{
					TempShape = ShapeNodeHandle[Temp1].Handle;
					if( iFALSE==TempShape->StaticFlag ) continue;
					i51KitP2UpdateOneShapeForce(CalShape,TempShape);
				}
			}
#if 0
#else
			B = CalBody;

			// 水滴已经到达浴缸区域，则无需计算
			if( B->Flag ) continue;

			if( CurrDirection==DIRECTION_NO )
			{
//				iLog(" direction is no");
//				iVectInit2(&(B->V), 0,0);
				i51KitP2UpdateBiasV(B->Centroid, &(B->v_bias),&(B->V));
//				iVectInit2(&(B->v_bias), 0,0);
				continue;
			}
			else if( CurrDirection==DIRECTION_ALL )
			{
				i51KitP2UpdateBiasV(B->Centroid, &(B->v_bias), &(B->V));
				continue;
			}

			Vx = B->V.X;
			Vy = B->V.Y;
 			if( Vx==0 && Vy==0 )
			{
				DirectionV1 = CurrDirection;
				DirectionV2 = CurrDirection;
			}
			else if( Vx>0 )
			{
				if(Vy==0) 
				{
					DirectionV1=DIRECTION_RIGHT;
					DirectionV2=DIRECTION_RIGHT;
				}
				else if( Vy/Vx>1 )
				{
					DirectionV1 = DIRECTION_BOTTOM;
					DirectionV2 = DIRECTION_RB;
				}
				else if( Vy/Vx==1 )
				{
					DirectionV1= DIRECTION_RB;
					DirectionV2= DIRECTION_RB;
				}
				else if( Vy>0 )
				{
					DirectionV1 = DIRECTION_RIGHT;
					DirectionV2 = DIRECTION_RB	;
				}
				else if( Vy/Vx<-1 )
				{
					DirectionV1 = DIRECTION_TOP;
					DirectionV2 = DIRECTION_RT;
				}
				else if( Vy/Vx==-1 )
				{
					DirectionV1= DIRECTION_RT;
					DirectionV2= DIRECTION_RT;
				}
				else
				{
					DirectionV1= DIRECTION_RIGHT;
					DirectionV2= DIRECTION_RIGHT;
				}
			}
			else if( Vx<0 )
			{
				if(Vy==0)
				{
					DirectionV1 = DIRECTION_LEFT;
					DirectionV2 = DIRECTION_LEFT;
				}
				else if( Vy/Vx>1 ) 
				{
					DirectionV1 = DIRECTION_TOP;
					DirectionV2 = DIRECTION_LT;
				}
				else if( Vy/Vx==1 )
				{
					DirectionV1= DIRECTION_LB;
					DirectionV2= DIRECTION_LB;
				}
				else if( Vy<0 )
				{
					DirectionV1 = DIRECTION_LEFT;
					DirectionV2 = DIRECTION_LT;
				}
				else if( Vy/Vx<-1 )
				{
					DirectionV1 = DIRECTION_BOTTOM;
					DirectionV2 = DIRECTION_LB;
				}
				else if( Vy/Vx==-1 )
				{
					DirectionV1 = DIRECTION_LB;
					DirectionV2 = DIRECTION_LB;
				}
				else
				{
					DirectionV1 = DIRECTION_LEFT;
					DirectionV2 = DIRECTION_LB;
				}
			}
			else if( Vy>0 )
			{
				DirectionV1 = DIRECTION_BOTTOM;
				DirectionV2 = DIRECTION_BOTTOM;
			}
			else 
			{
				DirectionV1 = DIRECTION_TOP;
				DirectionV2 = DIRECTION_TOP;
			}

			DirectionV = DirectionV1|DirectionV2;

			DirectionH = CurrDirection&DirectionV;

			if( DirectionH==DirectionV )
			{
			}
			else if( DirectionH!=DIRECTION_NO )
			{
				switch(DirectionH)
				{
					case DIRECTION_TOP:
					{
						Vx = 0;
						break;
					}
					case DIRECTION_RT: 
					{
						Vx = -Vy; 
						break;
					}
					case DIRECTION_RIGHT: 
					{
						Vy = 0; 
						break;
					}
					case DIRECTION_RB:
					{
						Vx = Vy; 
						break;
					}
					case DIRECTION_BOTTOM: 
					{
						Vx = 0; 
						break;
					}
					case DIRECTION_LB:
					{
						Vx = -Vy; 
						break;
					}
					case DIRECTION_LEFT:
					{
						Vy = 0;
						break;
					}
					case DIRECTION_LT:
					{
						Vx = Vy;
						break;
					}
					default :
						iLog("DirectionH:%d err",DirectionH);
						break;
				}
			}
			else
			{
//				Vx = 0;
//				Vy = 0;
			}
			iVectInit2(&(B->V), Vx,Vy);

			if(i51KitP2UpdateBiasV(B->Centroid, &(B->v_bias), &(B->V))) continue;

			Vx = B->v_bias.X;
			Vy = B->v_bias.Y;
 			if( Vx==0 && Vy==0 )
			{
				DirectionV1 = CurrDirection;
				DirectionV2 = CurrDirection;
			}
			else if( Vx>0 )
			{
				if(Vy==0) 
				{
					DirectionV1=DIRECTION_RIGHT;
					DirectionV2=DIRECTION_RIGHT;
				}
				else if( Vy/Vx>1 )
				{
					DirectionV1 = DIRECTION_BOTTOM;
					DirectionV2 = DIRECTION_RB;
				}
				else if( Vy/Vx==1 )
				{
					DirectionV1= DIRECTION_RB;
					DirectionV2= DIRECTION_RB;
				}
				else if( Vy>0 )
				{
					DirectionV1 = DIRECTION_RIGHT;
					DirectionV2 = DIRECTION_RB	;
				}
				else if( Vy/Vx<-1 )
				{
					DirectionV1 = DIRECTION_TOP;
					DirectionV2 = DIRECTION_RT;
				}
				else if( Vy/Vx==-1 )
				{
					DirectionV1= DIRECTION_RT;
					DirectionV2= DIRECTION_RT;
				}
				else
				{
					DirectionV1= DIRECTION_RIGHT;
					DirectionV2= DIRECTION_RIGHT;
				}
			}
			else if( Vx<0 )
			{
				if(Vy==0)
				{
					DirectionV1 = DIRECTION_LEFT;
					DirectionV2 = DIRECTION_LEFT;
				}
				else if( Vy/Vx>1 ) 
				{
					DirectionV1 = DIRECTION_TOP;
					DirectionV2 = DIRECTION_LT;
				}
				else if( Vy/Vx==1 )
				{
					DirectionV1= DIRECTION_LB;
					DirectionV2= DIRECTION_LB;
				}
				else if( Vy<0 )
				{
					DirectionV1 = DIRECTION_LEFT;
					DirectionV2 = DIRECTION_LT;
				}
				else if( Vy/Vx<-1 )
				{
					DirectionV1 = DIRECTION_BOTTOM;
					DirectionV2 = DIRECTION_LB;
				}
				else if( Vy/Vx==-1 )
				{
					DirectionV1 = DIRECTION_LB;
					DirectionV2 = DIRECTION_LB;
				}
				else
				{
					DirectionV1 = DIRECTION_LEFT;
					DirectionV2 = DIRECTION_LB;
				}
			}
			else if( Vy>0 )
			{
				DirectionV1 = DIRECTION_BOTTOM;
				DirectionV2 = DIRECTION_BOTTOM;
			}
			else 
			{
				DirectionV1 = DIRECTION_TOP;
				DirectionV2 = DIRECTION_TOP;
			}

			DirectionV = DirectionV1|DirectionV2;

			DirectionH = CurrDirection&DirectionV;

			if( DirectionH==DirectionV )
			{
			}
			else if( DirectionH!=DIRECTION_NO )
			{
				switch(DirectionH)
				{
					case DIRECTION_TOP:
					{
						Vx = 0;
						break;
					}
					case DIRECTION_RT: 
					{
						Vx = -Vy; 
						break;
					}
					case DIRECTION_RIGHT: 
					{
						Vy = 0; 
						break;
					}
					case DIRECTION_RB:
					{
						Vx = Vy; 
						break;
					}
					case DIRECTION_BOTTOM: 
					{
						Vx = 0; 
						break;
					}
					case DIRECTION_LB:
					{
						Vx = -Vy; 
						break;
					}
					case DIRECTION_LEFT:
					{
						Vy = 0;
						break;
					}
					case DIRECTION_LT:
					{
						Vx = Vy;
						break;
					}
					default :
						iLog("DirectionH:%d err",DirectionH);
						break;
				}
			}
			else
			{
				Vy = 0;
				if(!( ((Vx<0)&&(CurrDirection&DIRECTION_LEFT))
				|| ((Vx>0)&&(CurrDirection&DIRECTION_RIGHT)))
				) 
				{
					Vx = 0;
				}
			}
			iVectInit2(&(B->v_bias), Vx,Vy);
			
#endif			
			if(BodyTempUse==BodyUseNode) break;

		}
	}

	if(BodyTempUse!=BodyUseNode)	 
	{
		iLog("Update force ,BodyTempUse:%d,BodyUseNode:%d ",BodyTempUse,BodyUseNode);
		return iFALSE;
	}
	
	return iTRUE;
	
}

static iBOOL i51KitP2CalNewPos( NodeContainer * BodyContainer,  iS32 Stamp , iS32 Steps )
{
	iVECT MovePos;
	iVECT a;
	iVECT v;
	iBody * Body = iNULL;
	
	iShape * CurrShape = iNULL;
	iShapeCircle * CircleShape = iNULL;

	iS32 SignX = 0;
	iS32 SignY = 0;

	iS32 MaxNode = BodyContainer->MaxNode;
	iS32 UseNode = BodyContainer->UseNode;
	iS32 IdlyNode = BodyContainer->IdlyNode;
	Node * NodeHandle = BodyContainer->NodeHandle;
	iS32 Temp = 0;
	iS32 TempUse = 0;

	for( Temp = 0; Temp< MaxNode; Temp++)
	{
		if( NodeHandle[Temp].Flag )  TempUse++;
		else continue;

		Body = (iBody*)NodeHandle[Temp].Handle;
		CurrShape = Body->Shape;
		
		if( 0==Body->Mass ) 
		{
			iLog("bode mass 0");
			return iFALSE;
		}
		
		iVectInit2(&a, Body->ActingForce.X/Body->Mass,Body->ActingForce.Y/Body->Mass);

#if 0
		if( Body->v_bias.X > 5 ) Body->v_bias.X = 5;
		else if( Body->v_bias.X < -5 ) Body->v_bias.X = -5;
		
		if( Body->v_bias.Y > 3 ) Body->v_bias.Y = 3;
		else if( Body->v_bias.Y < -6 ) Body->v_bias.Y = -6;

	//	iVectAdd(&v, Body->V,Body->v_bias);
#endif

#if 0
	 	if( Body->V.X > WATER_MAX_SPEED)  Body->V.X = WATER_MAX_SPEED;
		else if( Body->V.X < -WATER_MAX_SPEED )  Body->V.X = -WATER_MAX_SPEED;
		if( Body->V.Y > WATER_MAX_SPEED )  Body->V.Y= WATER_MAX_SPEED;
		else if( Body->V.Y < -WATER_MAX_SPEED)  Body->V.Y = -WATER_MAX_SPEED;
#endif

	//	iVectInit1(&v, Body->V);
		iVectAdd(&v, Body->V,Body->v_bias);

		if( v.X>Body->MAX_V.X) v.X=Body->MAX_V.X;
		else if( v.X<-Body->MAX_V.X ) v.X=-Body->MAX_V.X;

		if(v.Y>Body->MAX_V.Y) v.Y = Body->MAX_V.Y;
		if(v.Y<-Body->MAX_V.Y) v.Y = -Body->MAX_V.Y;

		if(v.X>0)	MovePos.X = (v.X*Stamp+Steps-1)/Steps;// + (a.X*Stamp*Stamp+Steps*Steps-1)/Steps/Steps/2;
		else MovePos.X = (v.X*Stamp-Steps+1)/Steps;// + (a.X*Stamp*Stamp-Steps*Steps+1)/Steps/Steps/2;
		if(v.Y>0)	MovePos.Y = (v.Y*Stamp+Steps-1)/Steps;// + (a.Y*Stamp*Stamp+Steps*Steps-1)/Steps/Steps/2;
		else MovePos.Y = (v.Y*Stamp-Steps+1)/Steps;// + (a.Y*Stamp*Stamp-Steps*Steps+1)/Steps/Steps/2;

		if(a.X>0) SignX = 1;
		else SignX = -1;

		if(a.Y>0) SignY=1;
		else SignY = -1;
		
		iVectInit2(&(Body->V), Body->V.X+(a.X*Stamp+(Steps-1)*SignX)/Steps, Body->V.Y+(a.Y*Stamp+(Steps-1)*SignY)/Steps);

		if( Body->V.X>Body->MAX_V.X) Body->V.X=Body->MAX_V.X;
		else if( Body->V.X<-Body->MAX_V.X ) Body->V.X=-Body->MAX_V.X;

		if(Body->V.Y>Body->MAX_V.Y) Body->V.Y = Body->MAX_V.Y;
		if(Body->V.Y<-Body->MAX_V.Y) Body->V.Y = -Body->MAX_V.Y;

		if( CurrShape->ShapeType==I51_CIRCLE_SHAPE && (TimeCount%2==0) )
		{
			CircleShape = (iShapeCircle*)(CurrShape);
			iVectInit1( &(CircleShape->LastPoint4), CircleShape->LastPoint3);
			iVectInit1( &(CircleShape->LastPoint3), CircleShape->LastPoint2);
			iVectInit1( &(CircleShape->LastPoint2), CircleShape->LastPoint1);
			iVectInit1( &(CircleShape->LastPoint1), CircleShape->LastPoint);
			iVectInit1( &(CircleShape->LastPoint), CircleShape->CentrePoint);
//			CircleShape->LastPoint.X = CircleShape->CentrePoint.X;
//			CircleShape->LastPoint.Y = CircleShape->CentrePoint.Y;
		}
		i51KitP2ShapePosMove( (iHANDLE)(CurrShape), &MovePos);
		
		iVectInit2(&(Body->Centroid), Body->Centroid.X+MovePos.X,Body->Centroid.Y+MovePos.Y);

		iVectInit2( &(Body->ActingForce), 0, 0);
		iVectInit2( &(Body->v_bias), 0, 0);

		if(TempUse == UseNode ) break;
	}

	if( TempUse!=UseNode )
	{
		iLog("new pos TempUse:%d,UseNode:%d ",TempUse,UseNode );
	}
	return iTRUE;
	
}

iBOOL i51KitP2Step( iHANDLE Space )
{
	iWorld * World = (iWorld*)Space;
	iS32 Steps = World->Steps;
	iS32 Stamp = World->Stamp;

	NodeContainer * BodyContainer = iNULL;
	NodeContainer * ShapeContainer = iNULL;
	
//	for( Temp = 0; Temp<Steps; Temp++ )
	{
		// 计算运动的物体之间的相互作用力
		BodyContainer = &(World->BodyContainer);
		i51KitP2CalActingForce(BodyContainer);

		// 计算运动的物体与静态物体之间的相互作用力
		// 调整物体的速度
		ShapeContainer = &(World->ShapeContainer);
		i51KitP2UpdateForce(BodyContainer, ShapeContainer);

		i51KitP2CalNewPos(BodyContainer, Stamp, Steps);

	}

	return iTRUE;
	
}

iHANDLE i51KitP2CreateBody( iHANDLE Space, iVECT *Centroid, iS32 Mass)
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE World : 世界指针
	// iVECT Centroid : Body 重心
	// iS32 Mass : Body质量
	
	iBody * Body = iNULL;
	iWorld * World = (iWorld*)(Space);
	NodeContainer * BodyContainer = &(World->BodyContainer);
	Node * NodeHandle = iNULL;
	iS32 MaxBodyNum = 0;
	iS32 Temp = 0;
	
	if( iNULL==World )
	{
		iLog("KIT - P2 : world inull");
		return iFALSE;
	}

	Body = ( iBody*)i51KitP2Malloc(sizeof(iBody));
	if( iNULL == Body)
	{
		iLog("KIT - P2 : create body malloc");
		return iFALSE;
	}

	memset ( Body, 0 , sizeof(iBody)) ;

	if( Centroid )
	{
		Body->Centroid.X = Centroid->X;
		Body->Centroid.Y = Centroid->Y;
	}
	else
	{
		i51AdeOsFree(Body);
		iLog("KIT - P2 : Centroid null");
		return iFALSE;
	}

	Body->Mass = Mass;
	Body->MAX_V.X = 100000;
	Body->MAX_V.Y = 100000;

	if( 0==BodyContainer->IdlyNode )
	{
		BodyContainer->MaxNode += 50;
		BodyContainer->IdlyNode = 50;
		BodyContainer->NodeHandle
				=i51KitP2Realloc(	BodyContainer->NodeHandle,
								BodyContainer->UseNode*(sizeof(Node)), 
								BodyContainer->MaxNode*(sizeof(Node))	);
	}

	NodeHandle = BodyContainer->NodeHandle;
	MaxBodyNum = BodyContainer->MaxNode;
	for( Temp=0; Temp<MaxBodyNum; Temp++ )
	{
		if( iFALSE==NodeHandle[Temp].Flag )
		{
			NodeHandle[Temp].Flag = iTRUE;
			NodeHandle[Temp].Handle = (iHANDLE)(Body);
			BodyContainer->UseNode++;
			BodyContainer->IdlyNode--;
			break;
		}
	}

	if(Temp==MaxBodyNum)
	{
		iLog("BodyContainer full");
		i51AdeOsFree(Body);
		return iFALSE;
	}
	
	World->BodyNum++;
	
	return (iHANDLE)(Body);

}

iBOOL i51KitP2ReleaseBody(iHANDLE Space, iHANDLE Body )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Space : 世界指针
	// iHANDLE Body : Body 指针

	iWorld * World = iNULL;
	NodeContainer * BodyContainer = iNULL;
//	NodeContainer * ShapeContainer = iNULL;
	Node * NodeHandle = iNULL;
//	iHANDLE Shape = iNULL;
//	iBody * B = iNULL;
	iS32 Temp = 0;
	iS32 Max = 0;

	World = (iWorld*)(Space);
	
	if( iNULL==World )
	{
		iLog("KIT - P2 : release body space null");
		return iFALSE;
	}

	if( World->BodyNum<=0 )
	{
		iLog("KIT - P2 : no body:%d ",World->BodyNum);
		return iFALSE;
	}

//	B = (iBody*)Body;
//	Shape = B->Shape;
	BodyContainer = &(World->BodyContainer);
//	ShapeContainer = &(World->ShapeContainer);

	Max = BodyContainer->MaxNode;
	NodeHandle = BodyContainer->NodeHandle;
	for( Temp=0;Temp<Max; Temp++ )
	{
		if( NodeHandle[Temp].Handle==Body )
		{
			NodeHandle[Temp].Flag = iFALSE;
			NodeHandle[Temp].Handle = 0;
			BodyContainer->IdlyNode ++;
			BodyContainer->UseNode --;
			World->BodyNum--;
			i51AdeOsFree((void*)Body);
			break;
		}
	}
	if( Temp==Max )
	{
		iLog("release body err ");
		return iFALSE;
	}

/*	
	Max = ShapeContainer->MaxNode;
	NodeHandle = ShapeContainer->NodeHandle;
	for( Temp=0;Temp<Max; Temp++ )
	{
		if( NodeHandle[Temp].Handle==Shape)
		{
			NodeHandle[Temp].Flag = iFALSE;
			NodeHandle[Temp].Handle = 0;
			ShapeContainer->IdlyNode ++;
			ShapeContainer->UseNode --;
			i51AdeOsFree((void*)Shape);
			break;
		}
	}
	if( Temp==Max )
	{
		iLog("release body shape");
		return iFALSE;
	}
*/
	return iTRUE;

}

iBOOL i51KitP2SetBodyMass( iHANDLE Body, iS32 Mass)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iS32 Mass : 重置的Body质量
	
	iBody * SetBody = (iBody*)Body;

	if( iNULL==SetBody )
	{
		iLog("KIT - P2 : set mass body  null");
		return iFALSE;
	}

	SetBody->Mass = Mass;

	return iTRUE;
	
}

iBOOL i51KitP2SetBodyCentroid( iHANDLE Body, iVECT * Centroid)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iVECT * Centroid : 重置的Body质心
	
	iBody * SetBody = (iBody*)Body;

	if( iNULL==SetBody )
	{
		iLog("KIT - P2 : set centroid body  null");
		return iFALSE;
	}

	if( Centroid)
	{
		SetBody->Centroid.X = Centroid->X;
		SetBody->Centroid.Y = Centroid->Y;
	}
	else
	{
		SetBody->Centroid.X = 0;
		SetBody->Centroid.Y = 0;
	}

	return iTRUE;
	
}

iBOOL i51KitP2SetBodySpeed( iHANDLE Body, iVECT * V)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iVECT * V : 重置的Body速度
	
	iBody * SetBody = (iBody*)Body;

	if( iNULL==SetBody )
	{
		iLog("KIT - P2 : set speed body  null");
		return iFALSE;
	}

	if( V)
	{
		SetBody->V.X = V->X;
		SetBody->V.Y = V->Y;
	}
	else
	{
		SetBody->V.X = 0;
		SetBody->V.Y = 0;
	}

	return iTRUE;
	
}

iBOOL i51KitP2GetBodyMass( iHANDLE Body, iS32 * Mass)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iS32 * Mass : Body质量
	
	iBody * GetBody = (iBody*)Body;

	if( iNULL==GetBody || iNULL==Mass )
	{
		iLog("KIT - P2 : get mass null");
		return iFALSE;
	}

	*Mass = GetBody->Mass;

	return iTRUE;
	
}

iBOOL i51KitP2GetBodyCentroid( iHANDLE Body, iVECT * Centroid)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iVECT * Centroid :Body质心
	
	iBody * GetBody = (iBody*)Body;

	if( iNULL==GetBody || iNULL==Centroid )
	{
		iLog("KIT - P2 : get centroid body  null");
		return iFALSE;
	}

	Centroid->X = GetBody->Centroid.X;
	Centroid->Y = GetBody->Centroid.Y;

	return iTRUE;
	
}

iBOOL i51KitP2GetBodySpeed( iHANDLE Body, iVECT * V)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iVECT * V : Body速度
	
	iBody * GetBody = (iBody*)Body;

	if( iNULL==GetBody || iNULL==V)
	{
		iLog("KIT - P2 : get speed body  null");
		return iFALSE;
	}

	V->X = GetBody->V.X;
	V->Y = GetBody->V.Y;

	return iTRUE;
	
}

iHANDLE i51KitP2CreateShapeSeg( iHANDLE Space, iHANDLE Body, iS32 Thickness, iVECT P1, iVECT P2)
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Space : 世界指针
	// iHANDLE Body : 与外形关联的body
	// iS32 Thickness : 线厚度
	// iVECT * Points : 线端点坐标
	
	iShapeSeg * SegShape = iNULL;
	iWorld * World = (iWorld*)(Space);
	iHANDLE StaticBody = iNULL;
	NodeContainer * ShapeContainer = &(World->ShapeContainer);
	Node * NodeHandle = iNULL;
	iS32 MaxShapeNum = 0;
	iS32 Temp = 0;
	iS32 a,b,c;
	
	if( iNULL==World )
	{
		iLog("KIT - P2 : create seg shape data null");
		return iFALSE;
	}

	if( iVectEqual(P1,P2) )
	{
		iLog("KIT - P2 : create seg point equal,X:%d,Y:%d",P1.X,P1.Y);
		return iFALSE;
	}

	SegShape = ( iShapeSeg*)i51KitP2Malloc(sizeof(iShapeSeg));
	if( iNULL == SegShape)
	{
		iLog("KIT - P2 : create seg shape malloc");
		if(SegShape) i51AdeOsFree(SegShape);
		return iFALSE;
	}

	memset ( SegShape, 0 , sizeof(iShape)) ;

	SegShape->p1.X = P1.X;
	SegShape->p1.Y = P1.Y;
	SegShape->p2.X = P2.X;
	SegShape->p2.Y = P2.Y;

	a = (P2.Y - P1.Y)/COORDINATE_RATE;
	b = (P1.X - P2.X)/COORDINATE_RATE;
	c = P1.Y/COORDINATE_RATE*P2.X- P2.Y/COORDINATE_RATE*P1.X;

	while(1)
	{
		if( (0==a%2) && (0==b%2) && (0==c%2) )
		{
			a /= 2;
			b /= 2;
			c /= 2;
		}
		else
		{
			break;
		}
	}
	
	while(1)
	{
		if( (0==a%3) && (0==b%3) && (0==c%3) )
		{
			a /= 3;
			b /= 3;
			c /= 3;
		}
		else
		{
			break;
		}
	}

	while(1)
	{
		if( (0==a%5) && (0==b%5) && (0==c%5) )
		{
			a /= 5;
			b /= 5;
			c /= 5;
		}
		else
		{
			break;
		}
	}

	SegShape->a = a;
	SegShape->b = b;
	SegShape->c = c;
	
	SegShape->Thickness = Thickness;
	SegShape->Shape.Body = Body;
	SegShape->Shape.ShapeType = I51_SEGMENT_SHAPE;

	World->ShapeNum++;

	ShapeContainer = &(World->ShapeContainer);
	
	if( 0==ShapeContainer->IdlyNode )
	{
		ShapeContainer->MaxNode += 100;
		ShapeContainer->IdlyNode = 100;
		ShapeContainer->NodeHandle
				=i51KitP2Realloc(	ShapeContainer->NodeHandle,
								ShapeContainer->UseNode*(sizeof(Node)), 
								ShapeContainer->MaxNode*(sizeof(Node))	);
	}
	
	NodeHandle = ShapeContainer->NodeHandle;
	MaxShapeNum = ShapeContainer->MaxNode;
	for( Temp=0; Temp<MaxShapeNum; Temp++ )
	{
		if( iFALSE==NodeHandle[Temp].Flag )
		{
			NodeHandle[Temp].Flag = iTRUE;
			NodeHandle[Temp].Handle = (iHANDLE)(SegShape);
			ShapeContainer->UseNode++;
			ShapeContainer->IdlyNode--;
			break;
		}
	}

	if(Temp==MaxShapeNum)
	{
		iLog("ShapeContainer full");
		i51AdeOsFree(SegShape);
		return iFALSE;
	}
	
	StaticBody = (iHANDLE)(&(World->StaticBody));
	if( StaticBody==Body )
	{
		SegShape->Shape.StaticFlag = iTRUE;
	}
	else
	{
		SegShape->Shape.StaticFlag = iFALSE;
		((iBody*)Body)->Shape = (iShape*)SegShape;
	}

	return (iHANDLE)(SegShape);

}

iHANDLE i51KitP2CreateShapeCircle( iHANDLE Space, iHANDLE Body, iS32 Radius, iVECT * Centre )
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Space : 世界指针
	// iHANDLE Body : 与外形关联的body
	// iS32 Radius : 圆半径
	// iVECT * Centre : 圆心坐标
	
	iShapeCircle * CircleShape = iNULL;
	iWorld * World = (iWorld*)(Space);
	NodeContainer * ShapeContainer = iNULL;
	iHANDLE StaticBody = iNULL;
	iS32 Temp=0;
	iS32 MaxShapeNum = 0;
	Node * NodeHandle = iNULL;
	
	if( iNULL==World || iNULL == Centre )
	{
		iLog("KIT - P2 : create circle shape data null");
		return iFALSE;
	}

	CircleShape = ( iShapeCircle*)i51KitP2Malloc(sizeof(iShapeCircle));
	if( iNULL == CircleShape )
	{
		iLog("KIT - P2 : create circle shape malloc");
		return iFALSE;
	}

	memset ( CircleShape, 0 , sizeof(iShapeCircle)) ;

	CircleShape->CentrePoint.X = Centre->X;
	CircleShape->CentrePoint.Y = Centre->Y;

	iVectInit1( &(CircleShape->LastPoint), (*Centre));
	iVectInit1( &(CircleShape->LastPoint1), (*Centre));
	iVectInit1( &(CircleShape->LastPoint2), (*Centre));
	iVectInit1( &(CircleShape->LastPoint3), (*Centre));
	iVectInit1( &(CircleShape->LastPoint4), (*Centre));
	
	CircleShape->Radius = Radius;

	CircleShape->Shape.Body = Body;
	CircleShape->Shape.ShapeType = I51_CIRCLE_SHAPE;

	((iBody*)Body)->Shape = (iShape*)CircleShape;
	
	World->ShapeNum++;
	ShapeContainer = &(World->ShapeContainer);
	
	if( 0==ShapeContainer->IdlyNode )
	{
		ShapeContainer->MaxNode += 100;
		ShapeContainer->IdlyNode = 100;
		ShapeContainer->NodeHandle
				=i51KitP2Realloc(	ShapeContainer->NodeHandle,
								ShapeContainer->UseNode*(sizeof(Node)), 
								ShapeContainer->MaxNode*(sizeof(Node))	);
	}

	MaxShapeNum = ShapeContainer->MaxNode;
	NodeHandle = ShapeContainer->NodeHandle;
	for( Temp=0; Temp<MaxShapeNum; Temp++ )
	{
		if( iFALSE==NodeHandle[Temp].Flag )
		{
			NodeHandle[Temp].Flag = iTRUE;
			NodeHandle[Temp].Handle = (iHANDLE)(CircleShape);
			ShapeContainer->UseNode++;
			ShapeContainer->IdlyNode--;
			break;
		}
	}

	if(Temp==MaxShapeNum)
	{
		iLog("ShapeContainer full");
		i51AdeOsFree(CircleShape);
		return iFALSE;
	}
	
	StaticBody = (iHANDLE)(&(World->StaticBody));
	if( StaticBody==Body )
	{
		CircleShape->Shape.StaticFlag = iTRUE;
	}
	else
	{
		CircleShape->Shape.StaticFlag = iFALSE;
		((iBody*)Body)->Shape = (iShape*)CircleShape;
	}

	return (iHANDLE)(CircleShape);

}

static iBOOL i51KitP2PolyIsIllegality( iS32 Num, iVECT * Points )
{
	// author : Otto
	// since : 2012.6.8
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iS32 Num : 多边形顶点个数
	// iVECT * Points : 多边形顶点向量

	// notes : 多边形必须为凸多边形，且多边形的顶点必须是按顺时针方向依次排列
	
	iVECT a ,b ,c;
	int i = 0;

	if( Num<3 ) return iTRUE;
	
	for( i=0; i<Num; i++)
	{
		iVectInit1(&a, Points[i]);
		iVectInit1(&b, Points[(i+1)%Num]);
		iVectInit1(&c, Points[(i+2)%Num]);

		iVectSub(&a,b, a);
		iVectSub(&b,c, b);
		if(iVectCross(a,b ) > 0)
		{
			return iTRUE;
		}
	}
	
	return iFALSE;
	
}

iHANDLE i51KitP2CreateShapePoly( iHANDLE Space, iHANDLE Body, iS32 Num, iVECT * Points )
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Space : 世界指针
	// iHANDLE Body : 与外形关联的body
	// iS32 Radius : 圆半径
	// iVECT * Centre : 圆心坐标

#if 1
	return iFALSE;
#else
	iShapePoly * PolyShape = iNULL;
	iWorld * World = (iWorld*)(Space);
	iNode * CurrNode = iNULL;
	iNode * PolyNode = iNULL;
	iHANDLE StaticBody = iNULL;
	int Temp = 0;
	
	if( iNULL==World || iNULL == Points )
	{
		iLog("KIT - P2 : create poly shape data null");
		return iFALSE;
	}

	if( i51KitP2PolyIsIllegality(Num,Points) )
	{
		iLog("KIT - P2 : create poly points illegality");
		return iFALSE;
	}
	
	PolyShape = ( iShapePoly*)i51KitP2Malloc(sizeof(iShapePoly));
	CurrNode = ( iNode*)i51KitP2Malloc(sizeof(iNode));
	PolyNode = ( iNode*)i51KitP2Malloc(sizeof(iNode));
	if( PolyShape ) PolyShape->Points = ( iVECT*)i51KitP2Malloc(sizeof(iVECT)*Num);

	if( iNULL == PolyShape || iNULL == CurrNode || iNULL == PolyNode || iNULL==PolyShape->Points )
	{
		iLog("KIT - P2 : create poly shape malloc");
		if( PolyShape->Points ) i51AdeOsFree(PolyShape->Points);
		if(PolyShape) i51AdeOsFree(PolyShape);
		if(CurrNode) i51AdeOsFree(CurrNode);
		if(PolyNode) i51AdeOsFree(PolyNode);
		return iFALSE;
	}

	memset ( PolyShape, 0 , sizeof(iShape)) ;

	PolyShape->PointNum = Num;
	for( Temp = 0; Temp<Num; Temp++ )
	{
		PolyShape->Points[Temp].X = Points[Temp].X;
		PolyShape->Points[Temp].Y = Points[Temp].Y;
	}


	PolyShape->Shape.Body = Body;
	PolyShape->Shape.ShapeType = I51_POLY_SHAPE;

	CurrNode->Handle = (iHANDLE)(PolyShape);
	PolyNode->Handle = (iHANDLE)(PolyShape);

	((iBody*)Body)->Shape = (iShape*)PolyShape;
	
	i51KitP2AddNode(CurrNode, &(World->AllShapeHead), &(World->AllShapeTail));

	World->ShapeNum++;

	StaticBody = (iHANDLE)(&(World->StaticBody));
	if( StaticBody==Body )
	{
		PolyShape->Shape.StaticFlag = iTRUE;
		i51KitP2AddNode(PolyNode, &(World->StaticShapeHead), &(World->StaticShapeTail));
	}
	else
	{
		PolyShape->Shape.StaticFlag = iFALSE;
		i51KitP2AddNode(PolyNode, &(World->ActiveShapeHead), &(World->ActiveShapeTail));
	}

	return (iHANDLE)(PolyShape);
#endif
}

iBOOL i51KitP2ReleaseShape( iHANDLE Space, iHANDLE Shape )
{
	iWorld * World = iNULL;
//	NodeContainer * BodyContainer = iNULL;
	NodeContainer * ShapeContainer = iNULL;
	Node * NodeHandle = iNULL;
//	iHANDLE B = iNULL;
//	iShape * Sh = iNULL;
	iS32 Temp = 0;
	iS32 Max = 0;

	World = (iWorld*)(Space);
	
	if( iNULL==World )
	{
		iLog("KIT - P2 : release body space null");
		return iFALSE;
	}

	if( World->ShapeNum<=0 )
	{
		iLog("KIT - P2 : no shape:%d ",World->ShapeNum);
		return iFALSE;
	}

/*	
	Sh = (iShape*)Shape;
	B = (iHANDLE*)(Sh->Body);

	BodyContainer = &(World->BodyContainer);
	ShapeContainer = &(World->ShapeContainer);

	Max = BodyContainer->MaxNode;
	NodeHandle = BodyContainer->NodeHandle;
	for( Temp=0;Temp<Max; Temp++ )
	{
		if( B==&(World->StaticBody) ) break;
		if( NodeHandle[Temp].Handle==B )
		{
			NodeHandle[Temp].Flag = iFALSE;
			NodeHandle[Temp].Handle = 0;
			BodyContainer->IdlyNode ++;
			BodyContainer->UseNode --;
			i51AdeOsFree((void*)B);
			break;
		}
	}
	if( Temp==Max )
	{
		iLog("release shape body ");
		return iFALSE;
	}
*/	
	ShapeContainer = &(World->ShapeContainer);
	Max = ShapeContainer->MaxNode;
	NodeHandle = ShapeContainer->NodeHandle;
	for( Temp=0;Temp<Max; Temp++ )
	{
		if( NodeHandle[Temp].Handle==Shape)
		{
			NodeHandle[Temp].Flag = iFALSE;
			NodeHandle[Temp].Handle = 0;
			ShapeContainer->IdlyNode ++;
			ShapeContainer->UseNode --;
			mwSpace->ShapeNum--;
			i51AdeOsFree((void*)Shape);
			break;
		}
	}

	if( Temp==Max )
	{
		iLog("release shape err ");
		return iFALSE;
	}

	return iTRUE;
	
}

iBOOL i51KitP2ShapePosMove( iHANDLE Shape, iVECT *MovePos )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 外形 指针
	// iVECT * MovePos : 外形移动的向量
	
	iShape * SetShape = (iShape*)Shape;
	iShapeSeg * SegShape = iNULL;
	iShapeCircle * CircleShape = iNULL;
	iShapePoly * PolyShape = iNULL;
	int Num = 0;
	
	if( iNULL==SetShape || iNULL == MovePos )
	{
		iLog("KIT - P2 : move pos shape  null");
		return iFALSE;
	}

	switch(SetShape->ShapeType)
	{
		case I51_SEGMENT_SHAPE:
			SegShape = (iShapeSeg*)Shape;
			iVectAdd(&(SegShape->p1), SegShape->p1, *MovePos);
			iVectAdd(&(SegShape->p2), SegShape->p2, *MovePos);
			SegShape->c = SegShape->p1.Y/COORDINATE_RATE*SegShape->p2.X
				- SegShape->p2.Y/COORDINATE_RATE*SegShape->p1.X;
			if(SegShape->p1.X!=SegShape->p2.X) 
			{
				SegShape->c = SegShape->c/((SegShape->p1.X-SegShape->p2.X)/COORDINATE_RATE)*SegShape->b;
			}
			else if(SegShape->p1.Y!=SegShape->p2.Y) 
			{
				SegShape->c = SegShape->c/((SegShape->p2.Y-SegShape->p1.Y)/COORDINATE_RATE)*SegShape->a;
			}
			break;
		case I51_CIRCLE_SHAPE:
			CircleShape = ( iShapeCircle*)Shape;
			iVectAdd(&(CircleShape->CentrePoint), CircleShape->CentrePoint, *MovePos);
			break;
		case I51_POLY_SHAPE:
			PolyShape = (iShapePoly*)Shape;
			for( Num=0; Num<PolyShape->PointNum; Num++ )
			{
				iVectAdd( &(PolyShape->Points[Num]), PolyShape->Points[Num], *MovePos);
			}
			break;
		default:
			iLog("KIT - P2 : move pos shape  err");
			break;
	}

	return iTRUE;
	
}

iBOOL i51KitP2ResetCircleShapeCentre( iHANDLE Shape, iVECT *CircleCentre )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 圆外形 指针
	// iVECT * CircleCentre : 圆心坐标
	
	iShapeCircle * CircleShape = iNULL;
	int Num = 0;
	
	if( iNULL==Shape || iNULL == CircleCentre )
	{
		iLog("KIT - P2 : set circle centre null");
		return iFALSE;
	}

	CircleShape = ( iShapeCircle*)Shape;
	if( CircleShape->Shape.ShapeType != I51_CIRCLE_SHAPE )
	{
		iLog("KIT - P2 : set circle centre no circle ");
		return iFALSE;
	}
	
	iVectInit1(&(CircleShape->CentrePoint), *CircleCentre);

	return iTRUE;
	
}

iBOOL i51KitP2ResetCircleShapeRadius( iHANDLE Shape, iS32 radius )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 圆外形 指针
	// iS32 radius : 圆半径
	
	iShapeCircle * CircleShape = iNULL;
	int Num = 0;
	
	if( iNULL==Shape )
	{
		iLog("KIT - P2 : set circle radius null");
		return iFALSE;
	}

	CircleShape = ( iShapeCircle*)Shape;
	if( CircleShape->Shape.ShapeType != I51_CIRCLE_SHAPE )
	{
		iLog("KIT - P2 : set radius no circle ");
		return iFALSE;
	}
	
	CircleShape->Radius = radius;
	
	return iTRUE;
	
}

iBOOL i51KitP2ResetSegShapePoints( iHANDLE Shape, iVECT Points[2]  )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 线段外形 指针
	// iVECT Points[2]  : 线段端点向量
	
	iShapeSeg * SegShape = iNULL;
	
	if( iNULL==Shape )
	{
		iLog("KIT - P2 : set seg shape null");
		return iFALSE;
	}

	SegShape = ( iShapeSeg*)Shape;
	if( SegShape->Shape.ShapeType != I51_SEGMENT_SHAPE)
	{
		iLog("KIT - P2 : set points no seg ");
		return iFALSE;
	}
	
	iVectInit1( &SegShape->p1, Points[0]);
	iVectInit1( &SegShape->p2, Points[1]);
	SegShape->a = Points[1].Y - Points[0].Y;
	SegShape->b = Points[0].X - Points[1].X;
	SegShape->c = Points[0].Y*Points[1].X- Points[1].Y*Points[0].X;
	
	return iTRUE;
	
}

iBOOL i51KitP2ResetSegShapeThickness( iHANDLE Shape, iS32 Thickness )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 线段外形 指针
	// iS32  : 线段厚度
	
	iShapeSeg * SegShape = iNULL;
	
	if( iNULL==Shape )
	{
		iLog("KIT - P2 : set seg shape null");
		return iFALSE;
	}

	SegShape = ( iShapeSeg*)Shape;
	if( SegShape->Shape.ShapeType != I51_SEGMENT_SHAPE)
	{
		iLog("KIT - P2 : set points no seg ");
		return iFALSE;
	}
	
	SegShape->Thickness = Thickness;
	
	return iTRUE;
	
}

iBOOL i51KitP2ResetPolyShapePoints( iHANDLE Shape, iS32 Num, iVECT * Points )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 多边形外形 指针
	// iVECT * Points  : 线段厚度
	
	iShapePoly* PolyShape = iNULL;
	iVECT * PolyPoints = iNULL;
	iS32 Temp = 0;
	
	if( iNULL==Shape )
	{
		iLog("KIT - P2 : set poly shape null");
		return iFALSE;
	}

	PolyShape = ( iShapePoly*)Shape;
	if( PolyShape->Shape.ShapeType != I51_POLY_SHAPE )
	{
		iLog("KIT - P2 : set points no poly ");
		return iFALSE;
	}

	if( i51KitP2PolyIsIllegality( Num,  Points) )
	{
		iLog("KIT - P2 : set points poly  illegality");
	}

	PolyPoints = ( iVECT*)i51KitP2Malloc(sizeof(iVECT)*Num);
	if( iNULL==PolyPoints)
	{
		iLog("KIT - P2 : set points poly  malloc");
		return iFALSE;
	}

	for( Temp=0; Temp<Num; Temp++ )
	{
		iVectInit1(&PolyPoints[Temp], Points[Temp]);
	}
	
	i51AdeOsFree(PolyShape->Points);
	PolyShape->Points = PolyPoints;
	PolyShape->PointNum = Num;
	
	return iTRUE;
	
}

