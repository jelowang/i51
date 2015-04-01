
#include "i51KitAde.h"
#include "i51KitP2.h"
#include "i51KitG2.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);

struct iShape;

// 二维世界向量
typedef struct iVECT
{
	iS32 X;
	iS32 Y;
}iVECT;

// 物体状态
typedef enum iBodyState
{
	IBODY_STATE_SLEEP = 0,
	IBODY_STATE_AWAKE,
	IBODY_STATE_MOVE
} iBodyState;

// 世界中的物体
typedef struct iBody
{
	// 物体关联的外形
	struct  iShape * Shape;
	// 质量
	iS32 Mass;
	// 重心位置
	iVECT Centroid;
	// 阻尼
	iS32 Damping;
	// 速度、最大速度、速度修正值，为了防止运动物体间穿透
	iVECT V, MAX_V, v_bias;
	// 物体状态
	iBodyState BodyState;
	// 合力
	iVECT ActingForce;
	// 外力
	iVECT ExternalForce;
}iBody;

// 物体外形类型
typedef enum iShapeType{
	I51_CIRCLE_SHAPE = 0,
	I51_SEGMENT_SHAPE,
	I51_POLY_SHAPE,
	I51_NUM_SHAPES
} iShapeType;

#define MAX_COE	10000
#define MAX_COF	10000
#define MAX_MASS	100000

// 世界中的物体外形
typedef struct iShape
{
	// 外形类型
	iShapeType ShapeType;
	// 外形对应的Body
	iBody * Body;
	// 摩擦系数,取值范围(0~10000)
	iS32 cof;
	// 弹性系数,取值范围(0~10000)
	iS32 coe;
	// 角度
	iS32 Angle;
	// 角速度
	iS32 AngularRate;
	// 扭矩
	iS32 Torsion;
	// 外形所在的组，同一组内的外形不发生碰撞，默认为0，与所有同一层的外形发生碰撞，一个字节对应一个组，一个外形可以在多个组
	iU32 Group;
	// 外形所在的层，同一层内的外形才会发生碰撞，每一个字节对应一个层，最多支持32个层，一个外形可以在多个层上
	iU32 Layer;
	// 外形是否能移动的标记，如果绑定的body是空间的staticbody，则StaticFlag为真，物体不能移动
	iS8 StaticFlag;  
}iShape;

typedef struct iShapeCircle
{
	// 外形
	iShape Shape;
	// 半径
	iS32 Radius;
	// 圆心坐标
	iVECT CentrePoint;
}iShapeCircle;

typedef struct iShapeSeg
{
	// 外形
	iShape Shape;
	// 端点坐标
	iVECT p1,p2;
	// 线段厚度
	iS32 Thickness;
	// 直线一般式的A,B,C
	// A*x+B*y+C=0;
	iS32 a,b,c;
}iShapeSeg;

typedef struct iShapePoly
{
	// 外形
	iShape Shape;
	// 端点数
	iS32 PointNum;
	// 端点坐标
	iVECT * Points;
}iShapePoly;

typedef struct iNode
{
	iHANDLE Handle;
	struct iNode * Next;
	struct iNode * Parent;
}iNode;

typedef struct iWorld
{
	// 世界中的物体速度是m/s(米/秒)
	// 世界中的物体质量是kg(千克)
	// 世界中的所有时间单位是秒(s)
	// 世界中每一个像素对应1米
	// 世界坐标系按屏幕坐标系

	// 世界区域的大小
	iRECT WorldRect;
	// 世界重力大小
	iVECT WorldGravity;
	// 世界Body
	iBody StaticBody;
	// 两次调用i51KitP2Step()的时间差，单位秒(s)，默认为1
	iS32 Stamp;
	// 每调用一次i51KitP2Step()，世界中的物体迭代计算次数，默认为1
	// 如果Steps为0则世界中所有物体静止
	iS32 Steps;
	// 世界中Body数量
	iS32 BodyNum;
	// 世界中所有Body的头指针
	iNode * AllBodyHead;
	// 世界中所有Body的尾指针
	iNode * AllBodyTail;
	// 世界中活动Body的头指针
	iNode * ActiveBodyHead;
	// 世界中活动Body的尾指针
	iNode * ActiveBodyTail;
	// 世界中禁止Body的头指针
	iNode * StillBodyHead;
	// 世界中禁止Body的尾指针
	iNode * StillBodyTail;
	// 世界中Shape数量
	iS32 ShapeNum;
	// 世界中所有Shape的头指针
	iNode * AllShapeHead;
	// 世界中所有Shape的尾指针
	iNode * AllShapeTail;
	// 世界中静态Shape的头指针
	iNode * StaticShapeHead;
	// 世界中静态Shape的尾指针
	iNode * StaticShapeTail;
	// 世界中动态Shape的头指针
	iNode * ActiveShapeHead;
	// 世界中动态Shape的尾指针
	iNode * ActiveShapeTail;	
	
}iWorld;

static iU32 SqrtInt( iU32 Num )
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

	i = (Start+Num)/2;
	
	if( Num==0 || Num==1 ) return Num;
	if( Num>10000000 ) return Num;
	
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

#define AbsInt(X) ((X)>0?((X)):(-(X)))

static iBOOL iVectInit1(iVECT *DesVect, iVECT v1 )
{
	DesVect->X = v1.X;
	DesVect->Y = v1.Y;
	return iTRUE;
}

static iBOOL iVectInit2(iVECT *DesVect, iS32 X, iS32 Y )
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

static iBOOL MoveCentrePoint(iVECT * Des,iVECT C1,iVECT C2, iS32 Inset)
{
	iS32 Mul = (iS32)Inset;
	
	if( C1.X>C2.X ) Des->X  += Mul;
	else if( C1.X<C2.X ) Des->X  -= Mul;

	if( C1.Y>C2.Y ) Des->Y  += Mul;
	else if( C1.Y<C2.Y )Des->Y  -= Mul;

	return iTRUE;
}

iKIT iBOOL i51KitP2ShapePosMove( iHANDLE Shape, iVECT *MovePos );

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

static iS32 iVectDistanceP2P(iVECT v1, iVECT v2)
{
	// 计算两点之间的距离
//	return 100;
	return SqrtInt(((v1.X-v2.X)*(v1.X-v2.X) + (v1.Y-v2.Y)*(v1.Y-v2.Y) ));
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
	iVECT p1,p2;

	Pedal->X = (b*b*v.X-a*b*v.Y-a*c)/(a*a+b*b);
	Pedal->Y = (a*a*v.Y-a*b*v.X-b*c)/(a*a+b*b);

	iVectInit1(&p1, Shape->p1);
	iVectInit1(&p2, Shape->p2);

	if( p1.X==p2.X )
	{
		if( (Pedal->Y-p1.Y) * (Pedal->Y-p2.Y) >0 )
		{
			return iFALSE;
		}
	}
	else if( (Pedal->X-(p1.X)) * (Pedal->X-(p2.X)) >0 )
	{
		return iFALSE;
	}

	return iTRUE;
	
}

static iS32 iVectDistanceP2S(iVECT C, iVECT p1, iVECT p2)
{
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
}

iKIT iHANDLE i51KitP2CreateWorld( iRECT * WorldRect, iVECT * Gravity)
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iRECT * WorldRect : 世界大小
	// iVECT * Gravity : 世界的重力

	// Notes : 初始化世界

	iWorld * WorldSpace = iNULL;

	WorldSpace = ( iWorld *)i51AdeOsMalloc(sizeof(iWorld),  __FILE__ , __LINE__ );
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

iKIT iBOOL i51KitP2ReleaseWorld( iHANDLE Space)
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE World : 待释放的世界指针

	if( Space )
	{
		i51AdeOsFree(Space);
	}
	else
	{
		iLog("KIT - P2 : release null");
		return iFALSE;
	}
	
	return iTRUE;

}

iKIT iBOOL i51KitP2ResetWorld( iHANDLE Space, iRECT * WorldRect, iVECT * Gravity)
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

iKIT iBOOL i51KitP2ResetWorldTimeStamp( iHANDLE Space,  iS32 Stamp , iS32 Steps )
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
	
	if( (Distance>=(R1+R2))) return iTRUE;

#if 0
	B1->V.X = (B1->V.X + B2->V.X)/2;
	B2->V.X = B1->V.X;

	B1->V.Y = (B1->V.Y + B2->V.Y)/2;
	B2->V.Y = B1->V.Y;

	// 计算两个圆的相嵌像素
	Inset = R1+R2 - Distance;
	
	MoveCentrePoint(&(B1->v_bias), C1, C2, 1);
	MoveCentrePoint(&(B2->v_bias), C2, C1, 1);

	return iTRUE;
#endif

	// 计算两个圆的相嵌像素
	Inset = R1+R2 - Distance;
	
	MoveCentrePoint(&(B1->v_bias), C1, C2, 1);
	MoveCentrePoint(&(B2->v_bias), C2, C1, 1);

	if(  Distance<=0 ) return iTRUE;
	
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
		// 两球碰撞线不平行于坐标轴
		// Vf = vx*sina + vy*cosa
		// Vl = vx*cosa + vy*sina
		// 其中a是两球圆心的连线与坐标轴X轴的夹角
		// sina = TempY/Distance;cosa = TempX/Distance;
		// 两球碰撞后连接线方向交换速度，法线方向速度不变
		if( (C1.X>C2.X && C1.Y<C2.Y) || (C1.X<C2.X && C1.Y>C2.Y) )
		{
			TempX = AbsInt((C1.X-C2.X))*100;
		}
		else
		{
			TempX = -(AbsInt((C1.X-C2.X)))*100;
		}
		TempY = AbsInt((C1.Y-C2.Y))*100;

		Sina = TempY/Distance;
		Cosa = TempX/Distance;
		
		V1f = B1->V.X*Sina + B1->V.Y*Cosa;
		V1l = B1->V.X*Cosa + B1->V.Y*Sina;

		V2f = B2->V.X*Sina + B2->V.Y*Cosa;
		V2l = B2->V.X*Cosa + B2->V.Y*Sina;

		B1->V.X = (V2l*Cosa + V1f*Sina )/12000;
		B1->V.Y = (V2l*Sina - V1f*Cosa)/12000;
		
		B2->V.X = (V1l*Cosa + V2f*Sina)/12000 ;
		B2->V.Y = (V1l*Sina - V2f*Cosa)/12000;

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
	iVECT TempF;
	iVECT TempV;
	iVECT C1, C2;
	iS32 R1, R2;
	iS32 Distance = 0;
	iS32 Inset = 0;
	iBody * B1, *B2;
	
	R1 = CircleShape->Radius;
	R2 = SegShape->Thickness;

	// 计算经过圆心且与直线垂直的直线与直线的交点
	iVectInit1(&C1, CircleShape->CentrePoint);
	if(iFALSE==iVectGetP2SegPedal(&C2, C1, SegShape))
	{
		// 如果交点不在线段内，则返回
		return iTRUE;
	}
		
	Distance = iVectDistanceP2P(C1,C2);
	
	if( Distance>=(R1+R2) ) return iTRUE;

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
	body->ActingForce.X += body->Mass*(Gravity.X);
	body->ActingForce.Y += body->Mass*(Gravity.Y);

	return iTRUE;

}

static iBOOL i51KitP2CalActingForce( iNode * CurrNode , iNode * NodeHead )
{
	// 计算运动物体之间的相互作用力
	iNode * TempNode = NodeHead;
	iShape * CalShape = (iShape*)(CurrNode->Handle);
	iShape * TempShape = iNULL;

	if( CalShape->StaticFlag ) return iTRUE;

	while( TempNode )
	{
		TempShape = (iShape*)(TempNode->Handle);
		i51KitP2CalInteractionalForce(CalShape,TempShape);
		TempNode = TempNode->Next;
	}

	return iFALSE;
}

static iBOOL i51KitP2UpdateForceCircle2Seg( iShapeCircle * CircleShape, iShapeSeg * SegShape )
{
	iVECT TempF;
	iVECT TempV;
	iVECT C1, C2,Next;
	iS32 R1, R2;
	iS32 Distance = 0;
	iS32 NextDistance = 0;
	iS32 Temp = 0;
	iS32 Inset = 0;
	iBody * B1, *B2;
	
	R1 = CircleShape->Radius;
	R2 = SegShape->Thickness;

	// 计算经过圆心且与直线垂直的直线与直线的交点
	iVectInit1(&C1, CircleShape->CentrePoint);
	if( iFALSE==iVectGetP2SegPedal(&C2, C1, SegShape) )
	{
		// 如果交点不在线段内，则返回
		return iTRUE;
	}
		
	Distance = iVectDistanceP2P(C1,C2);
	
	if( Distance>=(R1+R2) ) return iTRUE;

	// 计算两个圆的相嵌像素
	Inset = R1+R2 - Distance;

	MoveCentrePoint(&(CircleShape->CentrePoint), C1, C2, Inset);
	MoveCentrePoint(&(CircleShape->Shape.Body->Centroid), C1, C2, Inset);

	// 计算两个圆的相对速度	
	B1 = (iBody*)(((CircleShape->Shape).Body));
	B2 = (iBody*)(((SegShape->Shape).Body));
	iVectSub(&TempV, B1->V, B2->V);

	// 计算两个圆的相互作用力
	iVectMult(&TempF, TempV, Inset*SqrtInt( (CircleShape->Shape.coe)*(SegShape->Shape.coe))/MAX_COE);

	iBodyForce(&(B1->ActingForce), TempF, C1, C2);
	iBodyForce(&(B2->ActingForce), TempF, C2, C1);

	// 更新物体受到的合力，如果合力导致物体之间越来越嵌入，则需要重新计算合力
	if( B1->ActingForce.X>0 ) Temp = 2;
	else Temp = -2;
	Next.X = C1.X + Temp;
	if( B1->ActingForce.X!=0 )
	{
//		Next.Y = C1.Y * Next.X / B1->ActingForce.Y;
		Next.Y = C1.Y + Temp*B1->ActingForce.Y/ B1->ActingForce.X;
	}
	else
	{
		Next.X = C1.X;
		if( B1->ActingForce.Y>0 )
		{
			Next.Y = C1.Y+2;
		}
		else if( B1->ActingForce.Y<0 )
		{
			Next.Y = C1.Y-2;
		}
	}
	
	iVectGetP2SegPedal(&C2, Next, SegShape);	
		
	NextDistance = iVectDistanceP2P(Next,C2);

	if( NextDistance<Distance )
	{
		if( SegShape->p1.X == SegShape->p2.X ) B1->ActingForce.X = 0;
		else if( SegShape->p1.Y == SegShape->p2.Y ) B1->ActingForce.Y = 0;
		else if( B1->ActingForce.Y!=0)
		{
			B1->ActingForce.X = (SegShape->p1.X-SegShape->p2.X)*(B1->ActingForce.Y)/(SegShape->p1.Y-SegShape->p2.Y);
		}
		else
		{
			B1->ActingForce.X = 0;
		}
	}

	// 更新物体的速度，如果速度导致物体之间越来越嵌入，则需要重新计算速度
//	iVectAdd(&TempV, B1->V, B1->v_bias);
	iVectInit1(&TempV, B1->V);
	if(  TempV.X>0 ) Temp = 2;
	else Temp = -2;
	Next.X = C1.X + Temp;
	if( TempV.X!=0 )
	{
//		Next.Y = Temp*TempV.Y/ TempV.X-C1.Y;
		Next.Y = C1.Y + Temp*TempV.Y/ TempV.X;
	}
	else
	{
		Next.X = C1.X;
		if( TempV.Y>0 )
		{
			Next.Y = C1.Y+2;
		}
		else if( TempV.Y<0 )
		{
			Next.Y = C1.Y-2;
		}
	}
	
	
	iVectGetP2SegPedal(&C2, Next, SegShape);	
		
	NextDistance = iVectDistanceP2P(Next,C2);

	if( NextDistance<Distance )
	{
		if( SegShape->p1.X == SegShape->p2.X ) TempV.X = 0;
		else if( SegShape->p1.Y == SegShape->p2.Y ) TempV.Y = 0;
		else if( TempV.Y!=0)
		{
			TempV.X = (SegShape->p1.X-SegShape->p2.X)*(TempV.Y)/(SegShape->p1.Y-SegShape->p2.Y);
		}
		else
		{
			TempV.X = 0;
		}
	}

	iVectInit1(&(B1->V), TempV);

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

static iBOOL i51KitP2UpdateForce( iNode * CurrNode , iNode * NodeHead )
{
	// 修正物体的合力和速度
	iNode * TempNode = NodeHead;
	iShape * CalShape = (iShape*)(CurrNode->Handle);
	iShape * TempShape = iNULL;

	if( CalShape->StaticFlag ) return iTRUE;

	while( TempNode )
	{
		TempShape = (iShape*)(TempNode->Handle);
		i51KitP2UpdateOneShapeForce(CalShape,TempShape);
		TempNode = TempNode->Next;
	}

	return iFALSE;
}

static iBOOL i51KitP2CalNewPos( iNode * CurrNode ,  iS32 Stamp , iS32 Steps )
{
	iVECT MovePos;
	iVECT a;
	iVECT v;
	iBody * Body = iNULL;
	
	iShape * CurrShape = (iShape*)(CurrNode->Handle);

	if( CurrShape->StaticFlag ) return iTRUE;
	
	Body = CurrShape->Body;
	if( 0==Body->Mass ) 
	{
		iLog("bode mass 0");
		return iFALSE;
	}
	
	iVectInit2(&a, Body->ActingForce.X/Body->Mass,Body->ActingForce.Y/Body->Mass);

#if 1
	if( Body->v_bias.X > 5 ) Body->v_bias.X = 5;
	else if( Body->v_bias.X < -5 ) Body->v_bias.X = -5;
	
	if( Body->v_bias.Y > 5 ) Body->v_bias.Y = 5;
	else if( Body->v_bias.Y < -5 ) Body->v_bias.Y = -5;

//	iVectAdd(&v, Body->V,Body->v_bias);
#endif
#if 1
	if( Body->V.X > 8 )  Body->V.X = 8;
	else if( Body->V.X < -8 )  Body->V.X = -8;
	if( Body->V.Y > 8 )  Body->V.Y= 8;
	else if( Body->V.Y < -8 )  Body->V.Y = -8;
#endif

//	iVectInit1(&v, Body->V);
	iVectAdd(&v, Body->V,Body->v_bias);

	MovePos.X = v.X*Stamp/Steps + a.X*Stamp*Stamp/Steps/Steps/2;
	MovePos.Y = v.Y*Stamp/Steps + a.Y*Stamp*Stamp/Steps/Steps/2;

	iVectInit2(&(Body->V), Body->V.X+a.X*Stamp/Steps, Body->V.Y+a.Y*Stamp/Steps);

	i51KitP2ShapePosMove( (iHANDLE)(CurrShape), &MovePos);
	iVectInit2(&(Body->Centroid), Body->Centroid.X+MovePos.X,Body->Centroid.Y+MovePos.Y);

	iVectInit2( &(Body->ActingForce), 0, 0);
	iVectInit2( &(Body->v_bias), 0, 0);

	return iTRUE;
	
}

iKIT iBOOL i51KitP2Step( iHANDLE Space )
{
	iWorld * World = (iWorld*)Space;
	iS32 Steps = World->Steps;
	iS32 Stamp = World->Stamp;
	iNode * StaticNode = iNULL;
	iNode * ActiveNode = iNULL;
	iShape * ActiveShape = iNULL;
	iS32 Temp = 0;

	for( Temp = 0; Temp<Steps; Temp++ )
	{
		// 计算运动的物体之间的相互作用力
		ActiveNode = World->ActiveShapeHead;
		while( ActiveNode )
		{
			ActiveShape = (iShape*)(ActiveNode->Handle);
			i51KitP2CalGravity( (iBody *)(ActiveShape->Body), World->WorldGravity);
			i51KitP2CalActingForce(ActiveNode, ActiveNode->Next);
			ActiveNode = ActiveNode->Next;
		}

		// 计算运动的物体与静态物体之间的相互作用力
		// 调整物体的速度
		ActiveNode = World->ActiveShapeHead;
		StaticNode = World->StaticShapeHead;
		while( ActiveNode )
		{
			i51KitP2UpdateForce(ActiveNode, StaticNode);
			ActiveNode = ActiveNode->Next;
		}

		ActiveNode = World->ActiveShapeHead;
		while( ActiveNode )
		{
			i51KitP2CalNewPos(ActiveNode, Stamp, Steps);
			ActiveNode = ActiveNode->Next;
		}

	}

	return iTRUE;
	
}

static iBOOL i51KitP2AddNode( iNode * NodeAdd, iNode ** NodeHead, iNode ** NodeTail )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iNode * NodeAdd :待增加的节点指针
	// iNode ** NodeHead :指向节点头指针的指针
	// iNode ** NodeTail :指向节点尾指针的指针

	NodeAdd->Next = iNULL;
	
	if( iNULL==*NodeHead)
	{
		NodeAdd->Parent = iNULL;
		*NodeHead = NodeAdd;
		*NodeTail = NodeAdd;
	}
	else if( *NodeTail )
	{
		NodeAdd->Parent = *NodeTail;
		(*NodeTail)->Next = NodeAdd;
		if( (*NodeHead) == (*NodeTail) )
		{
			(*NodeHead)->Next = NodeAdd;
		}
		*NodeTail = NodeAdd;
	}
	else
	{
		iLog("KIT - P2 : add shape");
		return iFALSE;
	}

	return iTRUE;
	
}

static iBOOL i51KitP2DeleteNode( iNode * NodeDel, iNode ** NodeHead, iNode ** NodeTail )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iNode * NodeDel :待删除的节点指针
	// iNode ** NodeHead :指向节点头指针的指针
	// iNode ** NodeTail :指向节点尾指针的指针

	if( NodeDel==*NodeHead)
	{
		*NodeHead= NodeDel->Next;
	}
	else if( NodeDel==*NodeTail)
	{
		*NodeTail= NodeDel->Parent;
	}
	else
	{
		NodeDel->Parent->Next = NodeDel->Next;
		NodeDel->Next->Parent = NodeDel->Parent;
	}

	i51AdeOsFree(NodeDel);

	return iTRUE;
	
}

static iBOOL i51KitP2DeleteHandle( iHANDLE Handle, iNode ** NodeHead, iNode ** NodeTail )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Handle :待删除的节点指针的数据
	// iNode ** NodeHead :指向节点头指针的指针
	// iNode ** NodeTail :指向节点尾指针的指针

	iNode * NodeDel = *NodeHead;

	while(NodeDel)
	{
		if( NodeDel->Handle==Handle ) break;
		else NodeDel = NodeDel->Next;
	}

	if( NodeDel )
	{
		i51KitP2DeleteNode(NodeDel, NodeHead, NodeTail);
		return iTRUE;
	}
	else
	{
		return iFALSE;
	}	
}

iKIT iHANDLE i51KitP2CreateBody( iHANDLE Space, iVECT * Centroid, iS32 Mass)
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE World : 世界指针
	// iVECT Centroid : Body 重心
	// iS32 Mass : Body质量
	
	iBody * Body = iNULL;
	iWorld * World = (iWorld*)(Space);
	iNode * CurrNode = iNULL;
	iNode * StillNode = iNULL;
	
	if( iNULL==World )
	{
		iLog("KIT - P2 : world inull");
		return iFALSE;
	}

	Body = ( iBody*)i51AdeOsMalloc(sizeof(iBody),  __FILE__ , __LINE__ );
	CurrNode = ( iNode*)i51AdeOsMalloc(sizeof(iNode),  __FILE__ , __LINE__ );
	StillNode = ( iNode*)i51AdeOsMalloc(sizeof(iNode),  __FILE__ , __LINE__ );
	if( iNULL == Body || iNULL == CurrNode || iNULL == StillNode )
	{
		iLog("KIT - P2 : create seg shape malloc");
		if(Body) i51AdeOsFree(Body);
		if(CurrNode) i51AdeOsFree(CurrNode);
		if(StillNode) i51AdeOsFree(StillNode);
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
		i51AdeOsFree(CurrNode);
		i51AdeOsFree(StillNode);
		iLog("KIT - P2 : Centroid null");
		return iFALSE;
	}

	Body->Mass = Mass;

	CurrNode->Handle = (iHANDLE)(Body);
	StillNode->Handle = (iHANDLE)(Body);
	
	i51KitP2AddNode(CurrNode, &(World->AllBodyHead), &(World->AllBodyTail));
	i51KitP2AddNode(StillNode, &(World->StillBodyHead), &(World->StillBodyTail));

	World->BodyNum++;
	
	return (iHANDLE)(Body);

}

iKIT iBOOL i51KitP2ReleaseBody(iHANDLE Space, iHANDLE Body )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Space : 世界指针
	// iHANDLE Body : Body 指针

	iWorld * World = iNULL;
	iU8 Temp = 0;

	World = (iWorld*)(Space);
	
	if( iNULL==World )
	{
		iLog("KIT - P2 : release body space null");
		return iFALSE;
	}

	if( World->BodyNum<=0 )
	{
		iLog("KIT - P2 : no body:%d ",World->BodyNum);
	}
	
	if( iFALSE==i51KitP2DeleteHandle(Body, &(World->AllBodyHead), &(World->AllBodyTail)) )
	{
		iLog("KIT - P2 : release body no in all");
		return iFALSE;
	}
	World->BodyNum--;
	Temp = 1;

	if( i51KitP2DeleteHandle(Body, &(World->StillBodyHead), &(World->StillBodyTail))) Temp++;
	if( i51KitP2DeleteHandle(Body, &(World->ActiveBodyHead), &(World->ActiveBodyTail))) Temp++;

	if( Temp==1 ) iLog("KIT - P2 : release body no in other");
	else if( Temp>2 ) iLog("KIT - P2 : release body in other 2");

	i51AdeOsFree((void*)Body);
	
	return iTRUE;

}

iKIT iBOOL i51KitP2SetBodyMass( iHANDLE Body, iS32 Mass)
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

iKIT iBOOL i51KitP2SetBodyDamping( iHANDLE Body, iS32 Damping)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iS32 Damping : 重置的Body阻尼
	
	iBody * SetBody = (iBody*)Body;

	if( iNULL==SetBody )
	{
		iLog("KIT - P2 : set damping body  null");
		return iFALSE;
	}

	SetBody->Damping = Damping;

	return iTRUE;
	
}

iKIT iBOOL i51KitP2SetBodyState( iHANDLE Body, iBodyState State)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iBodyState State : 重置的Body状态
	
	iBody * SetBody = (iBody*)Body;

	if( iNULL==SetBody )
	{
		iLog("KIT - P2 : set state body  null");
		return iFALSE;
	}

	SetBody->BodyState = State;

	return iTRUE;
	
}

iKIT iBOOL i51KitP2SetBodyCentroid( iHANDLE Body, iVECT * Centroid)
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

iKIT iBOOL i51KitP2SetBodySpeed( iHANDLE Body, iVECT * V)
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

iKIT iBOOL i51KitP2SetBodyExternalForce( iHANDLE Body, iVECT * ExternalForce)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iVECT* ExternalForce : 重置的Body外力
	
	iBody * SetBody = (iBody*)Body;

	if( iNULL==SetBody || iNULL==ExternalForce )
	{
		iLog("KIT - P2 : set ExternalForce body  null");
		return iFALSE;
	}

	iVectInit1(&(SetBody->ExternalForce), *ExternalForce);

	return iTRUE;
	
}

iKIT iBOOL i51KitP2GetBodyMass( iHANDLE Body, iS32 * Mass)
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

iKIT iBOOL i51KitP2GetBodyDamping( iHANDLE Body, iS32 * Damping)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iS32 *Damping : Body阻尼
	
	iBody * GetBody = (iBody*)Body;

	if( iNULL==GetBody || iNULL == Damping)
	{
		iLog("KIT - P2 : get damping null");
		return iFALSE;
	}

	*Damping = GetBody->Damping; 

	return iTRUE;
	
}

iKIT iBOOL i51KitP2GetBodyState( iHANDLE Body, iBodyState * State)
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Body : Body 指针
	// iBodyState * State : Body状态
	
	iBody * GetBody = (iBody*)Body;

	if( iNULL==GetBody || iNULL== State )
	{
		iLog("KIT - P2 : get state body  null");
		return iFALSE;
	}

	*State = GetBody->BodyState;

	return iTRUE;
	
}

iKIT iBOOL i51KitP2GetBodyCentroid( iHANDLE Body, iVECT * Centroid)
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

iKIT iBOOL i51KitP2GetBodySpeed( iHANDLE Body, iVECT * V)
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

iKIT iHANDLE i51KitP2CreateShapeSeg( iHANDLE Space, iHANDLE Body, iS32 Thickness, iVECT * Points )
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
	iNode * CurrNode = iNULL;
	iNode * SegNode = iNULL;
	iHANDLE StaticBody = iNULL;
	
	if( iNULL==World || iNULL == Points)
	{
		iLog("KIT - P2 : create seg shape data null");
		return iFALSE;
	}

	if( iVectEqual(Points[0],Points[1]) )
	{
		iLog("KIT - P2 : create seg point equal");
		return iFALSE;
	}

	SegShape = ( iShapeSeg*)i51AdeOsMalloc(sizeof(iShapeSeg),  __FILE__ , __LINE__ );
	CurrNode = ( iNode*)i51AdeOsMalloc(sizeof(iNode),  __FILE__ , __LINE__ );
	SegNode = ( iNode*)i51AdeOsMalloc(sizeof(iNode),  __FILE__ , __LINE__ );
	if( iNULL == SegShape || iNULL == CurrNode || iNULL == SegNode )
	{
		iLog("KIT - P2 : create seg shape malloc");
		if(SegShape) i51AdeOsFree(SegShape);
		if(CurrNode) i51AdeOsFree(CurrNode);
		if(SegNode) i51AdeOsFree(SegNode);
		return iFALSE;
	}

	memset ( SegShape, 0 , sizeof(iShape)) ;

	SegShape->p1.X = Points[0].X;
	SegShape->p1.Y = Points[0].Y;
	SegShape->p2.X = Points[1].X;
	SegShape->p2.Y = Points[1].Y;
	SegShape->a = Points[1].Y - Points[0].Y;
	SegShape->b = Points[0].X - Points[1].X;
	SegShape->c = Points[0].Y*Points[1].X- Points[1].Y*Points[0].X;
	
	SegShape->Thickness = Thickness;
	SegShape->Shape.Body = Body;
	SegShape->Shape.ShapeType = I51_SEGMENT_SHAPE;

	CurrNode->Handle = (iHANDLE)(SegShape);
	SegNode->Handle = (iHANDLE)(SegShape);

	((iBody*)Body)->Shape = (iShape*)SegShape;
	
	i51KitP2AddNode(CurrNode, &(World->AllShapeHead), &(World->AllShapeTail));

	World->ShapeNum++;
	StaticBody = (iHANDLE)(&(World->StaticBody));
	if( StaticBody==Body )
	{
		SegShape->Shape.StaticFlag = iTRUE;
		i51KitP2AddNode(SegNode, &(World->StaticShapeHead), &(World->StaticShapeTail));
	}
	else
	{
		i51KitP2AddNode(SegNode, &(World->ActiveShapeHead), &(World->AllShapeTail));
	}
	
	return (iHANDLE)(SegShape);

}

iKIT iHANDLE i51KitP2CreateShapeCircle( iHANDLE Space, iHANDLE Body, iS32 Radius, iVECT * Centre )
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
	iNode * CurrNode = iNULL;
	iNode * CircleNode = iNULL;
	iHANDLE StaticBody = iNULL;
	
	if( iNULL==World || iNULL == Centre )
	{
		iLog("KIT - P2 : create circle shape data null");
		return iFALSE;
	}

	CircleShape = ( iShapeCircle*)i51AdeOsMalloc(sizeof(iShapeCircle),  __FILE__ , __LINE__ );
	CurrNode = ( iNode*)i51AdeOsMalloc(sizeof(iNode),  __FILE__ , __LINE__ );
	CircleNode = ( iNode*)i51AdeOsMalloc(sizeof(iNode),  __FILE__ , __LINE__ );
	if( iNULL == CircleShape || iNULL == CurrNode || iNULL == CircleNode )
	{
		iLog("KIT - P2 : create circle shape malloc");
		if(CircleShape) i51AdeOsFree(CircleShape);
		if(CurrNode) i51AdeOsFree(CurrNode);
		if(CircleNode) i51AdeOsFree(CircleNode);
		return iFALSE;
	}

	memset ( CircleShape, 0 , sizeof(iShape)) ;

	CircleShape->CentrePoint.X = Centre->X;
	CircleShape->CentrePoint.Y = Centre->Y;
	CircleShape->Radius = Radius;

	CircleShape->Shape.Body = Body;
	CircleShape->Shape.ShapeType = I51_CIRCLE_SHAPE;

	CurrNode->Handle = (iHANDLE)(CircleShape);
	CircleNode->Handle = (iHANDLE)(CircleShape);

	((iBody*)Body)->Shape = (iShape*)CircleShape;
	
	i51KitP2AddNode(CurrNode, &(World->AllShapeHead), &(World->AllShapeTail));

	World->ShapeNum++;

	StaticBody = (iHANDLE)(&(World->StaticBody));
	if( StaticBody==Body )
	{
		CircleShape->Shape.StaticFlag = iTRUE;
		i51KitP2AddNode(CircleNode, &(World->StaticShapeHead), &(World->StaticShapeTail));
	}
	else
	{
		i51KitP2AddNode(CircleNode, &(World->ActiveShapeHead), &(World->ActiveShapeTail));
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

iKIT iHANDLE i51KitP2CreateShapePoly( iHANDLE Space, iHANDLE Body, iS32 Num, iVECT * Points )
{
	// author : Otto
	// since : 2012.6.6
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Space : 世界指针
	// iHANDLE Body : 与外形关联的body
	// iS32 Radius : 圆半径
	// iVECT * Centre : 圆心坐标
	
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
	
	PolyShape = ( iShapePoly*)i51AdeOsMalloc(sizeof(iShapePoly),  __FILE__ , __LINE__ );
	CurrNode = ( iNode*)i51AdeOsMalloc(sizeof(iNode),  __FILE__ , __LINE__ );
	PolyNode = ( iNode*)i51AdeOsMalloc(sizeof(iNode),  __FILE__ , __LINE__ );
	if( PolyShape ) PolyShape->Points = ( iVECT*)i51AdeOsMalloc(sizeof(iVECT)*Num,  __FILE__ , __LINE__ );

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
		i51KitP2AddNode(PolyNode, &(World->ActiveShapeHead), &(World->AllShapeTail));
	}

	return (iHANDLE)(PolyShape);

}

iKIT iBOOL i51KitP2ReleaseShape( iHANDLE Space, iHANDLE Shape )
{
	iWorld * World = (iWorld*)Space;
	iShape * DelShape = (iShape*)Shape;
	iShapePoly * PolyShape = iNULL;
	iU8 Temp = 0;
	
	if( iNULL==World || iNULL==DelShape )
	{
		iLog("KIT - P2 : release shape null");
		return iFALSE;
	}
	
	if( DelShape->ShapeType == I51_POLY_SHAPE )
	{
		PolyShape = (iShapePoly*)Shape;
		i51AdeOsFree(PolyShape->Points);
	}

	if( iNULL == i51KitP2DeleteHandle(Shape, &(World->AllShapeHead), &(World->AllShapeTail)))
	{
		iLog("KIT - P2 : del shape no in all");
		return iFALSE;
	}
	Temp = 1;

	if( i51KitP2DeleteHandle(Shape, &(World->ActiveShapeHead), &(World->ActiveShapeTail))) Temp++;
	if( i51KitP2DeleteHandle(Shape, &(World->StaticShapeHead), &(World->StaticShapeTail))) Temp++;

	if( Temp==1 ) iLog("KIT - P2 : del shape only in all");
	else if( Temp>2 ) iLog("KIT - P2 : del shape >2:%d",Temp);

	i51AdeOsFree((void*)Shape);

	return iTRUE;

}

iKIT iBOOL i51KitP2SetShapeCOF( iHANDLE Shape, iS32 cof )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 外形 指针
	// iS32 COF : 重置的外形摩擦系数
	
	iShape * SetShape = (iShape*)Shape;

	if( iNULL==SetShape )
	{
		iLog("KIT - P2 : set cof shape  null");
		return iFALSE;
	}

	SetShape->cof = cof;

	return iTRUE;
	
}

iKIT iBOOL i51KitP2SetShapeCOE( iHANDLE Shape, iS32 coe )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 外形 指针
	// iS32 coe : 重置的外形弹性系数
	
	iShape * SetShape = (iShape*)Shape;

	if( iNULL==SetShape )
	{
		iLog("KIT - P2 : set coe shape  null");
		return iFALSE;
	}

	SetShape->coe = coe;

	return iTRUE;
	
}

iKIT iBOOL i51KitP2SetShapeAngle( iHANDLE Shape, iS32 Angle )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 外形 指针
	// iS32 Angle : 重置的外形角度
	
	iShape * SetShape = (iShape*)Shape;

	if( iNULL==SetShape )
	{
		iLog("KIT - P2 : set Angle shape  null");
		return iFALSE;
	}

	SetShape->Angle= Angle;

	return iTRUE;
	
}

iKIT iBOOL i51KitP2SetShapeAngularRate( iHANDLE Shape, iS32 AngularRate )
{
	// author : Otto
	// since : 2012.6.7
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iHANDLE Shape : 外形 指针
	// iS32 Angle : 重置的外形角度
	
	iShape * SetShape = (iShape*)Shape;

	if( iNULL==SetShape )
	{
		iLog("KIT - P2 : set AngularRate shape  null");
		return iFALSE;
	}

	SetShape->AngularRate = AngularRate;

	return iTRUE;
	
}

iKIT iBOOL i51KitP2ShapePosMove( iHANDLE Shape, iVECT *MovePos )
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

iKIT iBOOL i51KitP2ResetCircleShapeCentre( iHANDLE Shape, iVECT *CircleCentre )
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

iKIT iBOOL i51KitP2ResetCircleShapeRadius( iHANDLE Shape, iS32 radius )
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

iKIT iBOOL i51KitP2ResetSegShapePoints( iHANDLE Shape, iVECT Points[2]  )
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

iKIT iBOOL i51KitP2ResetSegShapeThickness( iHANDLE Shape, iS32 Thickness )
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

iKIT iBOOL i51KitP2ResetPolyShapePoints( iHANDLE Shape, iS32 Num, iVECT * Points )
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

	PolyPoints = ( iVECT*)i51AdeOsMalloc(sizeof(iVECT)*Num,  __FILE__ , __LINE__ );
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

iHANDLE Space;

iBOOL TestInit()
{
	iHANDLE Body;
	iRECT WordRect;
	iVECT Gravity;
	iVECT BodyC;
	iVECT Points[2];
	iWorld * World = iNULL;
	iS32 BodyM;
	iS32 Temp = 0;
	
	Gravity.X = 0;
	Gravity.Y = 3;

	Space = i51KitP2CreateWorld(iNULL, &Gravity);

	for( Temp=0; Temp<200; Temp++ )
	{
		BodyC.X = (Temp*7)%320+5;
		BodyC.Y = -(Temp*6)%100;
		BodyM = 10;
		Body = i51KitP2CreateBody( Space, &BodyC, BodyM);
		i51KitP2CreateShapeCircle( Space, Body, 3, &BodyC);
	}

	i51KitP2ResetWorldTimeStamp(Space, 1, 2);
	
	World = (iWorld*)Space;
	Points[0].X = 0;
	Points[0].Y = 20;
	Points[1].X = 150;
	Points[1].Y = 300;
//	Points[1].X = 320;
//	Points[1].Y = 400;
	i51KitP2CreateShapeSeg( Space, (iHANDLE)(&(World->StaticBody)), 4, Points);
	Points[0].X = 320;
	Points[0].Y = 20;
	Points[1].X = 170;
	Points[1].Y = 300;
	i51KitP2CreateShapeSeg( Space, (iHANDLE)(&(World->StaticBody)), 4, Points);
	Points[0].X = 0;
	Points[0].Y = 300;
	Points[1].X = 320;
	Points[1].Y = 300;
	i51KitP2CreateShapeSeg( Space, (iHANDLE)(&(World->StaticBody)), 4, Points);

	return iTRUE;
	
}

iBOOL TestRun(void )
{
	iWorld * World = (iWorld*)Space;
	iShape * Shape = iNULL;
	iNode * CurrNode = World->AllShapeHead;
	iShapeSeg  * SegShape = iNULL;
	iShapeCircle * CircleShape = iNULL;
	iVECT BodyNewPos;
	iVECT BodyNewV;
	iS32 Point[2][2] = {0};
	
	i51KitP2Step(Space);

	i51KitG2CleanScreen( 0, 0);
	while(CurrNode)
	{
		Shape = (iShape*)(CurrNode->Handle);
		CurrNode = CurrNode->Next;
		switch(Shape->ShapeType)
		{
			case I51_CIRCLE_SHAPE:
				CircleShape = (iShapeCircle*)Shape;
				i51KitG2DrawCircle(CircleShape->CentrePoint.X, CircleShape->CentrePoint.Y, CircleShape->Radius, 0XFF00);
//				i51KitG2DrawPoint(CircleShape->CentrePoint.X, CircleShape->CentrePoint.Y, 0XFF00);
//				if( CircleShape->CentrePoint.X<0 || CircleShape->CentrePoint.X > 320)
				if( 0 )
				{
					CircleShape->Shape.Body->V.X = 0;
					CircleShape->Shape.Body->V.X = 0;
					CircleShape->Shape.Body->Centroid.X = 10;
					CircleShape->CentrePoint.X = 10;
				}
				if( CircleShape->CentrePoint.Y<-200 || CircleShape->CentrePoint.Y > 480 
				|| CircleShape->CentrePoint.X<-10 || CircleShape->CentrePoint.X > 320)
				{
//					i51KitP2ReleaseBody(Space, CircleShape->Shape.Body);
//					i51KitP2ReleaseShape(Space, CircleShape);
					
					BodyNewV.X = 0;
					BodyNewV.Y = 0;
					i51KitP2SetBodySpeed(CircleShape->Shape.Body, &BodyNewV);
					BodyNewPos.X = rand()%100+10;
					BodyNewPos.Y = -rand()%100-5;
					i51KitP2SetBodyCentroid(CircleShape->Shape.Body, &BodyNewPos);
					i51KitP2ResetCircleShapeCentre((iHANDLE)(CircleShape), &BodyNewPos);
				}
				break;
			case I51_SEGMENT_SHAPE:
				SegShape = (iShapeSeg*)Shape;
				Point[0][0] = SegShape->p1.X;
				Point[0][1] = SegShape->p1.Y;
				Point[1][0] = SegShape->p2.X;
				Point[1][1] = SegShape->p2.Y;
				i51KitG2DrawLine( Point, 0XAA00);
				break;
			default:
				break;
		}
	}

	i51KitG2Update();
	return iTRUE;
}
iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{
		case i51_MSG_PAINT :
			TestRun();
		break ;
	
		case i51_MSG_RUN :
		break ;
		
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
	
			i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,(iU16 *)L"PKiGAPI\\i51KitG2.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE ( i51KitG2 ) )
			{
				i51AdeOsLog ( i51_LOG_DC , "加载i51KitG2动态库失败" ) ;
				return iFALSE;
			}

			i51KitG2CleanScreen(0, 0);
			i51AdeOsScreenAlwaysLight(1);
			TestInit();
			iLog("Load ok");	
		}
		break ;
		
		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
		break ;
		

	}

	

	return iTRUE ;

}

iKIT iU32* i51KitPort ()
{

	//	TODO : Initialize API Port for external layer
	
	//	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(DlDemo)
	//	i51KIT_DYNAMIC_KIT_MAPING_END 

	//	return i51KIT_DYNAMIC_KIT_GET_MAPING(DlDemo) ;

	return 0 ;

}
