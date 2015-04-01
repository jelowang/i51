
#ifndef THE_i51KitP2_H
#define THE_i51KitP2_H

#include "i51.h"

#define COORDINATE_RATE	100
#define VELOCITY_RATE	11
#define FORCE_RATE	11

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
	// 重力大小
	iVECT GravityForce;
	// 重心位置
	iVECT Centroid;
	// 阻尼
//	iS32 Damping;
	// 速度、最大速度、速度修正值，为了防止运动物体间穿透
	// v_static是运动物体与静止墙壁之间的调整速度
	// v_bias是运动物体之间的调整速度
	iVECT V, MAX_V, v_bias;//, v_static;
	// 物体状态
//	iBodyState BodyState;
	// 合力
	iVECT ActingForce;
	// 外力
//	iVECT ExternalForce;
	// 水滴是否在浴缸中的标记
	iU8 Flag;
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

typedef struct mwMapBlock 
{
	unsigned char BlockType;
	unsigned char ShapeType;
	signed short BlockX;
	signed short BlockY;
}mwMapBlock;

// 世界中的物体外形
typedef struct iShape
{
	// 外形类型
	iShapeType ShapeType;
	// 外形对应的Body
	iBody * Body;
	// 摩擦系数,取值范围(0~10000)
//	iS32 cof;
	// 弹性系数,取值范围(0~10000)
//	iS32 coe;
	// 角度
//	iS32 Angle;
	// 角速度
//	iS32 AngularRate;
	// 扭矩
//	iS32 Torsion;
	// 外形所在的组，同一组内的外形不发生碰撞，默认为0，与所有同一层的外形发生碰撞，一个字节对应一个组，一个外形可以在多个组
//	iU32 Group;
	// 外形所在的层，同一层内的外形才会发生碰撞，每一个字节对应一个层，最多支持32个层，一个外形可以在多个层上
//	iU32 Layer;
	// 外形是否能移动的标记，如果绑定的body是空间的staticbody，则StaticFlag为真，物体不能移动
	iS8 StaticFlag;  
	// 外形所在的组，主要用在机关处
	iU32 ShapeGroup;
	mwMapBlock block;

}iShape;

typedef struct iShapeCircle
{
	// 外形
	iShape Shape;
	// 半径
	iS32 Radius;
	// 当前的圆心坐标
	iVECT CentrePoint;
	// 上一个周期的圆心坐标
	iVECT LastPoint;
	iVECT LastPoint1;
	iVECT LastPoint2;
	iVECT LastPoint3;
	iVECT LastPoint4;
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

typedef struct Node
{
	// 是否使用的标记
	iU8 Flag;
	iHANDLE Handle;
}Node;

typedef struct NodeContainer
{
	iS32 MaxNode;
	iS32 UseNode;
	iS32 IdlyNode;
	Node * NodeHandle;
}NodeContainer;

#define DEFAULT_BODY_NUM	50
#define DEFAULT_SHAPE_NUM	500

typedef struct iWorld
{
	// 世界中的物体速度是m/s(米/秒)
	// 世界中的物体质量是kg(千克)
	// 世界中的所有时间单位是秒(s)
	// 世界中每一个像素对应1024米
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
	// 世界中所有Body的容器
	NodeContainer BodyContainer;
	// 世界中shape数量
	iS32 ShapeNum;
	// 世界中所有Shape的容器
	NodeContainer ShapeContainer;
}iWorld;

#define AbsInt(X) ((X)>0?((X)):(-(X)))


extern void * i51KitP2Malloc(iU32 length);
extern iBOOL iVectInit1(iVECT * DesVect, iVECT v1);
extern iBOOL iVectInit2(iVECT * DesVect, iS32 X, iS32 Y);
extern iU32 SqrtInt(iU32 Num);
extern iHANDLE i51KitP2CreateWorld( iRECT * WorldRect, iVECT * Gravity);
extern iBOOL i51KitP2ReleaseWorld(iHANDLE Space);
extern iHANDLE i51KitP2CreateBody( iHANDLE Space, iVECT * Centroid, iS32 Mass);
extern iHANDLE i51KitP2CreateShapeSeg( iHANDLE Space, iHANDLE Body, iS32 Thickness, iVECT P1, iVECT P2);
extern iHANDLE i51KitP2CreateShapeCircle(iHANDLE Space, iHANDLE Body, iS32 Radius, iVECT * Centre);
extern iBOOL i51KitP2ReleaseBody(iHANDLE Space, iHANDLE Body);
extern iBOOL i51KitP2ReleaseShape(iHANDLE Space, iHANDLE Shape);
extern iBOOL i51KitP2ResetWorldTimeStamp(iHANDLE Space, iS32 Stamp, iS32 Steps);
extern iBOOL i51KitP2Step(iHANDLE Space);

#endif

