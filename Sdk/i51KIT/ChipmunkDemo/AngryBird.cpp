
#include "i51KitAde.h"
#include "AngryBird.h"
#include "i51KitKey.h"
#include "i51KitG2.h"

AngryBird_struct AngryBird;
i51Public_struct *pfunc = iNULL;
i51Public_struct i51Public;

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);
i51KIT_DYNAMIC_KIT_DEF(i51KitKey);

#define CHAIN_COUNT	4
#define LINK_COUNT	30
static cpSpace *mmSABSpace = iNULL;
static int ticks = 0;
// 当前屏幕宽度
static iU16 ScreenWidth = 0;

// 当前屏幕高度
static iU16 ScreenHeight = 0;

// 显存
static iU16* ScreenBuffer = 0;

#define WORD_BLANK 0
#define WORD_MUD	1
static iU8 WordFlag[61][41] = {0};

#define BACKGROUND_BLANK	0
#define BACKGROUND_MUD		1
#define BACKGROUND_SEAWEED	2
#define BACKGROUND_ROCK		3

static iU8 BackgroundType[320][240] = {0};

iHANDLE KeyQueue = iNULL;
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT	320

static iBOOL WordInit(void)
{
	// 屏幕信息数组
	iU32 * LcdInfoArr;
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

	KeyQueue = i51KitKeyInputQueueCreat();
	if( iNULL==KeyQueue ) return iFALSE;	

	cpInitChipmunk();

	cpResetShapeIdCounter();
	
	/// init space.
	mmSABSpace = cpSpaceNew();
	
	mmSABSpace->iterations = 1;
	mmSABSpace->elasticIterations = 10;
	
	mmSABSpace->gravity = cpv(0, 0);
	
	mmSABSpace->sleepTimeThreshold = 0.2f;//0.2f;

	cpBody *body, *staticBody = &(mmSABSpace->staticBody);
	cpShape *shape;
	cpFloat radius = 2.0f;
	
	for(int i=1; i<60; i++ )
	{
		for( int j = 1; j < 40; j++ )
		{
			WordFlag[i][j] = 1;
		}
	}
	
	for(int i=0; i<320; i++ )
	{
		for( int j = 0; j < 240; j++ )
		{
			BackgroundType[i][j] = 1;
		}
	}
	// Create segments around the edge of the screen.
//i51AdeOsLog(0, "cpSpaceAddShape 1");
//	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(0,0), cpv(239,0), 0.0f));
//i51AdeOsLog(0, "cpShapeSetElasticity");
//	shape->e = 1.0f;
//	shape->u = 1.0f;
//	shape->layers= 0X7FFFFFFF;
//	cpShapeSetElasticity(shape, 1.0f);
//i51AdeOsLog(0, "cpShapeSetFriction");
//	mmSABSpace->fr = 1.0f;
//	cpShapeSetFriction(shape, 1.0f);
//i51AdeOsLog(0, "cpShapeSetLayers");
//	mmSABSpace->
//	cpShapeSetLayers(shape, NOT_GRABABLE_MASK);

//i51AdeOsLog(0, "cpSpaceAddShape 2");
	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(0,0), cpv(0,319), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;
//	cpShapeSetElasticity(shape, 1.0f);
//	cpShapeSetFriction(shape, 1.0f);
//	cpShapeSetLayers(shape, NOT_GRABABLE_MASK);

//i51AdeOsLog(0, "cpSpaceAddShape 3");
	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(239,0), cpv(239,319), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;

	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(0,319), cpv(239,319), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;

	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(0,30), cpv(110,140), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;
	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(110,200), cpv(110,140), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;
	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(110,200), cpv(90,319), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;
	
	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(239,30), cpv(135,140), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;
	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(135,200), cpv(135,140), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;
	shape = cpSpaceAddShape(mmSABSpace, cpSegmentShapeNew(staticBody, cpv(135,200), cpv(155,319), 0.0f));
	shape->e = 1.0f;
	shape->u = 1.0f;
	shape->layers= 0X7FFFFFFF;

	cpFloat mass = 1;
	cpFloat width = 10;
	cpFloat height = 20;
	
	cpFloat spacing = width*0.3;
	
	// Add lots of boxes.
	for(int i=0; i<CHAIN_COUNT; i++){
		cpBody *prev = NULL;
		
		for(int j=0; j<LINK_COUNT; j++){
//i51AdeOsLog(0, "i=%d,j=%d",i,j);
//			cpVect pos = cpv(rand()%228+5,rand()%350-400);
cpVect pos = cpv((j)*4+5,(i)*4+5);
			
//i51AdeOsLog(0, "cpSpaceAddBody");
//			body = cpSpaceAddBody(mmSABSpace, cpBodyNew(mass, cpMomentForBox(mass, width, height)));
//			body = cpSpaceAddBody(mmSABSpace, cpBodyNew(10.0f, cpMomentForCircle(30.0f, 0.0f, radius, cpvzero)));
			body = cpSpaceAddBody(mmSABSpace, cpBodyNew(60.0f, cpMomentForCircle(60.0f, 0.0f, radius, cpvzero)));
			cpBodySetPos(body, pos);
			cpBodySetID(body,i*LINK_COUNT+j);
			cpArray ** TempArr = (cpArray **)(mmSABSpace->bodies->arr+(i*LINK_COUNT+j));
			cpBody * TempPoint = (cpBody *)(*(mmSABSpace->bodies->arr+(i*LINK_COUNT+j)));
			
//i51AdeOsLog(0, "cpSpaceAddShape");
//			shape = cpSpaceAddShape(mmSABSpace, cpBoxShapeNew(body, width, height));
shape = cpSpaceAddShape(mmSABSpace, cpCircleShapeNew(body, radius, cpvzero));
shape->e = 0.2f; 
shape->u = 0.01f; 
cpPolyShape * PolyShape = (cpPolyShape *)shape;
//			cpShapeSetFriction(shape, 0.8f);
			
			cpFloat breakingForce = 80000;
			
			cpConstraint *constraint = NULL;
//i51AdeOsLog(0, "cpSpaceAddConstraint");
			if(prev == NULL){
//				constraint = cpSpaceAddConstraint(mmSABSpace, cpSlideJointNew(body, staticBody, cpv(0, -height/2), cpv(pos.x, 0), 0, spacing));
			} else {
//				constraint = cpSpaceAddConstraint(mmSABSpace, cpSlideJointNew(body, prev, cpv(0, -height/2), cpv(0, height/2), 0, spacing));
			}
//			constraint->maxForce = breakingForce;
//			constraint->
//			cpConstraintSetMaxForce(constraint, breakingForce);
//			cpConstraintSetPostSolveFunc(constraint, BreakableJointPostSolve);
			
			prev = body;
//i51AdeOsLog(0, "prev = body;");
		}
	}
	
//i51AdeOsLog(0, "cpSpaceAddBody");
//	body = cpSpaceAddBody(mmSABSpace, cpBodyNew(10.0f, cpMomentForCircle(30.0f, 0.0f, radius, cpvzero)));
//i51AdeOsLog(0, "cpBodySetPos");
//	cpBodySetPos(body, cpv(130-radius, 320 - radius*2));
//	body->v = cpv(0,-2000);
//cpBodySetPos(body, cpv(120-radius, 0));
//i51AdeOsLog(0, "cpBodySetVel");
//	cpBodySetVel(body, cpv(0, -300));

//i51AdeOsLog(0, "cpSpaceAddShape");
//	shape = cpSpaceAddShape(mmSABSpace, cpCircleShapeNew(body, radius, cpvzero));
//shape->e = 0.0f;
//shape->u = 0.9f;
//i51AdeOsLog(0, "cpShapeSetElasticity");
//	cpShapeSetElasticity(shape, 0.0f);
//i51AdeOsLog(0, "cpShapeSetFriction");
//	cpShapeSetFriction(shape, 0.9f);

//i51AdeOsLog(0, "return mmSABSpace");
/*
cpSpaceHash * activeShapes = mmSABSpace->activeShapes;
cpHashSetBin **table = activeShapes->handleSet->table;
int size = 	activeShapes->handleSet->size;
for(int i=0;i<size;i++)
{
	shape = (cpShape*)table[i];
	if(shape)
	{
		if(shape->klass->type==CP_POLY_SHAPE)
		{
			iS32 Point[4][2];
			cpPolyShape * PolyShape = (cpPolyShape *)shape;
			i51KitG2CleanScreen(0, 0);
			Point[0][0] = (iS32)PolyShape->verts[0].x;
			Point[0][1] = (iS32)PolyShape->verts[0].y;
			Point[1][0] = (iS32)PolyShape->verts[1].x;
			Point[1][1] = (iS32)PolyShape->verts[1].y;
			Point[2][0] = (iS32)PolyShape->verts[2].x;
			Point[2][1] = (iS32)PolyShape->verts[2].y;
			Point[3][0] = (iS32)PolyShape->verts[3].x;
			Point[3][1] = (iS32)PolyShape->verts[3].y;
			i51KitG2DrawPolygon(4,Point,0XFF00);
			i51KitG2Update();
		}
	}
}
*/

}

void * i51_malloc(unsigned int size)
{
	return (void *)i51AdeOsMalloc(size,__FILE__,__LINE__);
}

void i51_free(void *ptr)
{
	if(ptr == iNULL)
		return;

	i51AdeOsFree(ptr);
}

void *mmRealloc(void *src_ptr, int size, int resize)
{
	void *temp_ptr = iNULL;
	if(!resize){
		i51_free(src_ptr);
	}
	else
	{
		temp_ptr = (void*)i51AdeOsMalloc(resize, __FILE__, __LINE__);
		if(temp_ptr)
		{
			if(src_ptr)memcpy(temp_ptr, src_ptr, size < resize ? size : resize);
			i51_free(src_ptr);
		}
	}

	return temp_ptr;
}

void *mmCalloc(int num, int size)
{
	void *p = (void *)i51_malloc(num * size);
	memset(p, 0, num * size);
	return p;
}

static void update(int ticks)
{
	int steps = 1;
	cpFloat dt = 1.0f/60.0f/(cpFloat)steps;
	
	for(int i=0; i<steps; i++){
//i51AdeOsLog ( i51_LOG_DC , "i=%d",i ) ; 
		cpSpaceStep(mmSABSpace, dt);
//		cpSpaceEachBody(space, &eachBody, NULL);
	}
}




#define MAX_TRIANGLE_POINTS 512
//填充三角形边缘坐标集
iS16 IntersectionCoordinates[MAX_TRIANGLE_POINTS][2] = {0};
iCOLOR GraphFillColor[MAX_TRIANGLE_POINTS] = {0};
#define GraphColorNum  0XFF0

iKIT iBOOL i51KitG2FillTriangle(iS32 Point[3][2])
{ 	
    iS32 EndOldY = -1;
    iS32 EndOldX = -1;
    iS32 StartOldY = -1;
    iS32 StartOldX = -1;
    iS32 EndX = Point[1][0];
    iS32 EndY = Point[1][1];
    iS32 StartX = Point[0][0];
    iS32 StartY = Point[0][1];

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
    iU32 PonterPoints = 0;  
    iU32 CopyLength = 0; 
    iCOLOR * ColorBuf = iNULL;
    iS16 Temp1 = 0, Temp2 = 0;
	iS32 CurrentIntersection = 0;

    for( Temp1=0; Temp1<3; Temp1++ )
    {
        if( Point[Temp1][0]>500||Point[Temp1][0]<-100
         ||Point[Temp1][1]>1000||Point[Temp1][1]<-100		
        )
        {
            return iFALSE;
        }
    }
    //统计三角形边缘
    for( Temp1=0; Temp1<3; Temp1++ )
    {
            StartX = Point[Temp1][0];
            StartY = Point[Temp1][1];
            EndX = Point[(Temp1+1)%3][0];
            EndY = Point[(Temp1+1)%3][1];
			if(StartY==EndY) continue;

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
            if( CurrentIntersection>=MAX_TRIANGLE_POINTS-2 ) return iFALSE;
            //同一直线同一Y值只取一个X
            if( (StartOldY!=StartY&&EndOldY!=StartY)||Location==0)
            {
                StartOldY = StartY;
                StartOldX = StartX;
                IntersectionCoordinates[CurrentIntersection][0] = StartX;
                IntersectionCoordinates[CurrentIntersection][1] = StartY;
                CurrentIntersection++;				
            }

            //同一直线同一Y值只取一个X
            if( (EndOldY != EndY  && EndY!=StartY) || Location==0 )
            {
                EndOldY = EndY;
                EndOldX = EndX;
                IntersectionCoordinates[CurrentIntersection][0] = EndX;
                IntersectionCoordinates[CurrentIntersection][1] = EndY;
                CurrentIntersection++;				
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
    }

    //边缘坐标按Y升序排序
	iS16 i,j;
	iS16 MinData,MinPos;
	iS16 Temp;
	for(i=0;i<CurrentIntersection;i++)
	{
		MinData= IntersectionCoordinates[i][1];
		MinPos = i;
		for(j=i+1;j<CurrentIntersection;j++)
		{
			if(IntersectionCoordinates[j][1]<MinData)
			{
				MinData = IntersectionCoordinates[j][1];
				MinPos = j;
			}
		}
		
		if(MinPos==i) continue;
		
		Temp = IntersectionCoordinates[i][0];
		IntersectionCoordinates[i][0] = IntersectionCoordinates[MinPos][0];
		IntersectionCoordinates[MinPos][0] = Temp;
		
		Temp = IntersectionCoordinates[i][1];
		IntersectionCoordinates[i][1] = IntersectionCoordinates[MinPos][1];
		IntersectionCoordinates[MinPos][1] = Temp;
		
	}

	Temp1 = 0;
	iS32 BuffPos = 0;
	iS32 BuffLength = 0;
	while(Temp1<CurrentIntersection)
	{
		if(IntersectionCoordinates[Temp1][1]<0||IntersectionCoordinates[Temp1][1]>=ScreenHeight) 
		{
			Temp1 += 2;
			continue;
		}
	
		if(IntersectionCoordinates[Temp1][0]>IntersectionCoordinates[Temp1+1][0])
		{
			Temp2 = IntersectionCoordinates[Temp1][0];
			IntersectionCoordinates[Temp1][0] = IntersectionCoordinates[Temp1+1][0];
			IntersectionCoordinates[Temp1+1][0] = Temp2;
		}
		if(IntersectionCoordinates[Temp1][0]>=ScreenWidth||IntersectionCoordinates[Temp1+1][0]<= 0)
		{
			Temp1 += 2;
			continue;
		}
		if(IntersectionCoordinates[Temp1][0]<0) IntersectionCoordinates[Temp1][0] =0;
		if(IntersectionCoordinates[Temp1+1][0]>=ScreenWidth) IntersectionCoordinates[Temp1+1][0] =ScreenWidth-1;
		BuffPos = IntersectionCoordinates[Temp1][1]*ScreenWidth + IntersectionCoordinates[Temp1][0];
		BuffLength = IntersectionCoordinates[Temp1+1][0] -	IntersectionCoordinates[Temp1][0]+1;
        i51AdeStdMemcpy16(&ScreenBuffer[BuffPos], GraphFillColor, (BuffLength<<1));
		Temp1 += 2;	
	}
    return iTRUE;
    
}


iKIT iBOOL CP_FillQuadrilateral(iS32 Point[4][2])
{ 	
    iS32 EndOldY = 0XFFFFFF;
//    iS32 EndOldX = 0XFFFFFF;
    iS32 StartOldY = 0XFFFFFF;
//    iS32 StartOldX = 0XFFFFFF;
    iS32 EndX = Point[1][0];
    iS32 EndY = Point[1][1];
    iS32 StartX = Point[0][0];
    iS32 StartY = Point[0][1];

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
    iU32 PonterPoints = 0;  
    iU32 CopyLength = 0; 
    iCOLOR * ColorBuf = iNULL;
    iS16 Temp1 = 0, Temp2 = 0;
	iS32 CurrentIntersection = 0;

    for( Temp1=0; Temp1<4; Temp1++ )
    {
        if( Point[Temp1][0]>500||Point[Temp1][0]<-100
         ||Point[Temp1][1]>1000||Point[Temp1][1]<-100		
        )
        {
            return iFALSE;
        }
    }
    //统计四边形边缘
    for( Temp1=0; Temp1<4; Temp1++ )
    {
            StartX = Point[Temp1][0];
            StartY = Point[Temp1][1];
            EndX = Point[(Temp1+1)%4][0];
            EndY = Point[(Temp1+1)%4][1];
			if(StartY==EndY) continue;

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

		IntersectionCoordinates[CurrentIntersection][0] = StartX;
		IntersectionCoordinates[CurrentIntersection][1] = StartY;
		IntersectionCoordinates[CurrentIntersection+1][0] = EndX;
		IntersectionCoordinates[CurrentIntersection+1][1] = EndY;
		CurrentIntersection += 2;
		StartOldY = StartY;
		EndOldY = EndY;
		
//        for ( Location = 0; Location <= HalfLength; Location++)
		while ( StartX<EndX )
        {
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
            if( CurrentIntersection>=MAX_TRIANGLE_POINTS ) return iFALSE;
            //同一直线同一Y值只取一个X
            if( StartOldY!=StartY && StartY!=EndOldY )
            {
                StartOldY = StartY;
                IntersectionCoordinates[CurrentIntersection][0] = StartX;
                IntersectionCoordinates[CurrentIntersection][1] = StartY;
                CurrentIntersection++;				
            }

            //同一直线同一Y值只取一个X
            if( (EndOldY != EndY  && EndY!=StartY))
            {
                EndOldY = EndY;
                IntersectionCoordinates[CurrentIntersection][0] = EndX;
                IntersectionCoordinates[CurrentIntersection][1] = EndY;
                CurrentIntersection++;				
            }
        }
    }

    //边缘坐标按Y升序排序
	iS16 i,j;
	iS16 MinData,MinPos;
	iS16 Temp;
	for(i=0;i<CurrentIntersection;i++)
	{
		MinData= IntersectionCoordinates[i][1];
		MinPos = i;
		for(j=i+1;j<CurrentIntersection;j++)
		{
			if(IntersectionCoordinates[j][1]<MinData)
			{
				MinData = IntersectionCoordinates[j][1];
				MinPos = j;
			}
		}
		
		if(MinPos==i) continue;
		
		Temp = IntersectionCoordinates[i][0];
		IntersectionCoordinates[i][0] = IntersectionCoordinates[MinPos][0];
		IntersectionCoordinates[MinPos][0] = Temp;
		
		Temp = IntersectionCoordinates[i][1];
		IntersectionCoordinates[i][1] = IntersectionCoordinates[MinPos][1];
		IntersectionCoordinates[MinPos][1] = Temp;
		
	}

	Temp1 = 0;
	iS32 BuffPos = 0;
	iS32 BuffLength = 0;
	while(Temp1<CurrentIntersection)
	{
		if(IntersectionCoordinates[Temp1][1]<0||IntersectionCoordinates[Temp1][1]>=ScreenHeight) 
		{
			Temp1 += 2;
			continue;
		}
	
		if(IntersectionCoordinates[Temp1][0]>IntersectionCoordinates[Temp1+1][0])
		{
			Temp2 = IntersectionCoordinates[Temp1][0];
			IntersectionCoordinates[Temp1][0] = IntersectionCoordinates[Temp1+1][0];
			IntersectionCoordinates[Temp1+1][0] = Temp2;
		}
		if(IntersectionCoordinates[Temp1][0]>=ScreenWidth||IntersectionCoordinates[Temp1+1][0]<= 0)
		{
			Temp1 += 2;
			continue;
		}
		// 两条直线的交点，会有两个点XY都相同,需要偏移一组数据
		// 如果需要绘制的直线两点Y坐标不同，则说明异常需要偏移一位
		if((IntersectionCoordinates[Temp1][0]==IntersectionCoordinates[Temp1+1][0]
		&&IntersectionCoordinates[Temp1][1]==IntersectionCoordinates[Temp1+1][1])
		|| (IntersectionCoordinates[Temp1][1]!=IntersectionCoordinates[Temp1+1][1]) )
		{
			BuffPos = IntersectionCoordinates[Temp1][1]*ScreenWidth + IntersectionCoordinates[Temp1][0];
			ScreenBuffer[BuffPos]=GraphFillColor[0];
			Temp1 ++;
			continue;
		}
		if(IntersectionCoordinates[Temp1][0]<0) IntersectionCoordinates[Temp1][0] =0;
		if(IntersectionCoordinates[Temp1+1][0]>=ScreenWidth) IntersectionCoordinates[Temp1+1][0] =ScreenWidth-1;
		BuffPos = IntersectionCoordinates[Temp1][1]*ScreenWidth + IntersectionCoordinates[Temp1][0];
		BuffLength = IntersectionCoordinates[Temp1+1][0] -	IntersectionCoordinates[Temp1][0]+1;
        i51AdeStdMemcpy16(&ScreenBuffer[BuffPos], GraphFillColor, (BuffLength<<1));
		Temp1 += 2;	
	}
    return iTRUE;
    
}

iKIT iBOOL CP_FillCircle( iS32 X, iS32 Y, iU32 R)
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 X : 圆心X坐标
    // iS32 Y : 圆心Y坐标
    // iU32 R : 圆半径
    // iCOLOR Color : 填充圆的颜色

    // notes : 填充以(X, Y)为圆心,半径为R的圆
    // 根据圆的对称性，我们找到圆上一点可以确定与之X轴对称、Y轴对称已经中心对称的另外三点
    // 所以计算时只需要通过计算确认一个象限内圆上的点,其他三个象限的点可以根据对称性直接获得
    // 此算法采用的是空间优先算法

    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))

    iS32 SquareR = 0;
    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 EndX = 0;
    iS32 EndY = 0;
    iS32 CenterX = X;
    iS32 CenterY = Y;
    iS32 Center2X = X+X;
    iS32 Center2Y = Y+Y;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS32 SymmetryPoints[3][2] ;
    iS32 SquareDifferential[3] = {0};
    iS32 Point[2][2];

    iS32 DrawX = X, DrawY = Y;  
	
    iS16 Temp1 = 0;//, Temp2 = 0;

	iS32 CurrLineY = 0;
	iS32 CurrLineStartX = 0;
	iS32 CurrLineEndX = 0;
	iS32 CurrLineLength = 0;
	iS32 ScreenPos = 0;
	
    if( X>2000||X<-1500 ||Y>2000||Y<-1500||R>500 )
    {
        return iFALSE;
    }

    SquareR = R * R;

    CurrentX = DrawX - R;
    CurrentY = DrawY;

    EndX = DrawX;
    EndY = DrawY - R;

    CenterX = DrawX;
    CenterY = DrawY;
    Center2X = DrawX+DrawX;
    Center2Y = DrawY+DrawY;

    //绘制与X轴平行的直径
    CurrLineY = CurrentY;
	CurrLineStartX = CurrentX;
	CurrLineEndX = X + R;
	if( CurrLineY>=0 && CurrLineY<ScreenHeight&& CurrLineEndX>=0 && CurrLineStartX<ScreenWidth )
	{
		if( CurrLineEndX >= ScreenWidth ) CurrLineEndX = ScreenWidth -1;
		if( CurrLineStartX < 0 ) CurrLineStartX = 0;
		CurrLineLength = CurrLineEndX - CurrLineStartX;
		ScreenPos = CurrLineY*ScreenWidth + CurrLineStartX;
        i51AdeStdMemcpy16(&ScreenBuffer[ScreenPos], GraphFillColor, (CurrLineLength<<1));
	}

    while(1)
    {
        if( CurrentX==EndX && CurrentY==EndY )
        {
            break;
        }

        OldX = CurrentX;
        OldY = CurrentY;

        SquareDifferential[0] = ABSOLUTE( (CurrentX+1-CenterX)*(CurrentX+1-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY - 1 -CenterY)*(CurrentY - 1 -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY -1  -CenterY)*(CurrentY -1 -CenterY)\
                                                         +(CurrentX+1-CenterX)*(CurrentX+1-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
        {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX ++ ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY -- ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }

/*
        SymmetryPoints[0][0] = Center2X - CurrentX;
        SymmetryPoints[0][1] = CurrentY;
        SymmetryPoints[1][0] = SymmetryPoints[0][0];
        SymmetryPoints[1][1] = Center2Y - CurrentY;
        SymmetryPoints[2][0] = CurrentX;
        SymmetryPoints[2][1] = SymmetryPoints[1][1];
*/
        //绘制一三象限的图像
		CurrLineY = CurrentY;
		CurrLineStartX = CurrentX;
		CurrLineEndX = Center2X - CurrentX;
		if( CurrLineY>=0 && CurrLineY<ScreenHeight && CurrLineEndX>=0 && CurrLineStartX<ScreenWidth )
		{
			if( CurrLineEndX >= ScreenWidth ) CurrLineEndX = ScreenWidth -1;
			if( CurrLineStartX < 0 ) CurrLineStartX = 0;
			CurrLineLength = CurrLineEndX - CurrLineStartX;
			ScreenPos = CurrLineY*ScreenWidth + CurrLineStartX;
			i51AdeStdMemcpy16(&ScreenBuffer[ScreenPos], GraphFillColor, (CurrLineLength<<1));
		}


        //绘制二四象限的图像
		CurrLineY = Center2Y - CurrentY;
		CurrLineStartX = CurrentX;
		CurrLineEndX = Center2X - CurrentX;
		if( CurrLineY>=0 && CurrLineY<ScreenHeight && CurrLineEndX>=0 && CurrLineStartX<ScreenWidth )
		{
			if( CurrLineEndX >= ScreenWidth ) CurrLineEndX = ScreenWidth -1;
			if( CurrLineStartX < 0 ) CurrLineStartX = 0;
			CurrLineLength = CurrLineEndX - CurrLineStartX;
			ScreenPos = CurrLineY*ScreenWidth + CurrLineStartX;
			i51AdeStdMemcpy16(&ScreenBuffer[ScreenPos], GraphFillColor, (CurrLineLength<<1));
		}


    }

    #undef ABSOLUTE

    return iTRUE;

}

typedef struct drawSpaceOptions {
	int drawHash;
	int drawBBs;
	int drawShapes;
	float collisionPointSize;
	float bodyPointSize;
	float lineThickness;
} drawSpaceOptions;

drawSpaceOptions options = {
	0,
	0,
	1,
	4.0f,
	0.0f,
	1.5f,
};

#define MAX_CIRCLE_R	6
#define BIG_CIRCLE_R	5
#define SMALL_CIRCLE_R	3
#define MIN_CIRCLE_R	2

int CountNum = 0;

static void
drawCircleShape(cpBody *body, cpCircleShape *circle, cpSpace *space)
{
	if(circle->tc.y>325)
	{
//		body->p = cpv(rand()%228+5,rand()%50-50);
		cpBodySetPos(body, cpv(rand()%10+5,rand()%100-100));
		body->v = cpv(0,0);
	}
	if(circle->tc.x>238)
	{
		cpBodySetPos(body, cpv(238,circle->tc.y));
		//circle->tc.x=235-circle->r;
	}
	if(circle->tc.x<3)
	{
		cpBodySetPos(body, cpv(3,circle->tc.y));
		//circle->tc.x=235-circle->r;
	}
	
	iS32 X = (iS32)(circle->tc.x);
	iS32 Y = (iS32)(circle->tc.y);
	iS32 VX = (iS32)(body->v.x);
	iS32 VY = (iS32)(body->v.y);
	iU32 R = (iU32)(MAX_CIRCLE_R);

	if(GraphFillColor[0]!=GraphColorNum)
	{
		for(int i = 0; i<MAX_TRIANGLE_POINTS; i++ )
		{
			GraphFillColor[i] = GraphColorNum;
		}
	}

	if( (VX>50||VX<-50||VY>50||VY<-50))
	{
		float V = mmSqrtf(body->v.x*body->v.x+body->v.y*body->v.y);
		float CentreX= circle->tc.x-body->v.x/20;
		float CentreY = circle->tc.y-body->v.y/20;
		iS32 CurrR = R - V/50;
		iS32 Point[2][2];

		Point[0][0] = (iS32)X;
		Point[0][1] = (iS32)Y;
		Point[1][0] = (iS32)CentreX;
		Point[1][1] = (iS32)CentreY;
		i51KitG2DrawLine(Point, 0XFF00);
		if(CurrR<2) CurrR = 2;
		CP_FillCircle( (iS32)CentreX, (iS32)CentreY, CurrR);

	}
	CP_FillCircle( X, Y, R);
	CP_FillCircle( X, Y, 8);

/*
CountNum++;
if(CountNum>2000)
{
	CountNum = 0;
	cpSpaceRemoveShape(mmSABSpace, (cpShape *)circle);
}
*/

}

static void DrawBackground( void )
{
	iU8 * Back =(iU8 * ) BackgroundType;
	for( int i = 0; i < 76800; i++ )
	{
		if(Back[i])	ScreenBuffer[i] = 0XF00;
	}
}

static const float pillVAR[] = {
	 0.0000f,  1.0000f, 1.0f,
	 0.2588f,  0.9659f, 1.0f,
	 0.5000f,  0.8660f, 1.0f,
	 0.7071f,  0.7071f, 1.0f,
	 0.8660f,  0.5000f, 1.0f,
	 0.9659f,  0.2588f, 1.0f,
	 1.0000f,  0.0000f, 1.0f,
	 0.9659f, -0.2588f, 1.0f,
	 0.8660f, -0.5000f, 1.0f,
	 0.7071f, -0.7071f, 1.0f,
	 0.5000f, -0.8660f, 1.0f,
	 0.2588f, -0.9659f, 1.0f,
	 0.0000f, -1.0000f, 1.0f,

	 0.0000f, -1.0000f, 0.0f,
	-0.2588f, -0.9659f, 0.0f,
	-0.5000f, -0.8660f, 0.0f,
	-0.7071f, -0.7071f, 0.0f,
	-0.8660f, -0.5000f, 0.0f,
	-0.9659f, -0.2588f, 0.0f,
	-1.0000f, -0.0000f, 0.0f,
	-0.9659f,  0.2588f, 0.0f,
	-0.8660f,  0.5000f, 0.0f,
	-0.7071f,  0.7071f, 0.0f,
	-0.5000f,  0.8660f, 0.0f,
	-0.2588f,  0.9659f, 0.0f,
	 0.0000f,  1.0000f, 0.0f,
};
static const int pillVAR_count = sizeof(pillVAR)/sizeof(float)/3;

static void
drawSegmentShape(cpBody *body, cpSegmentShape *seg, cpSpace *space)
{
	cpVect a = seg->ta;
	cpVect b = seg->tb;
	iS32 Point[2][2] = {0};// = {(iS32)a.x,(iS32)a.y,(iS32)b.x,(iS32)b.y};
	 Point[0][0]= (iS32)(a.x);
	 Point[0][1] = (iS32)(a.y);
	 Point[1][0]= (iS32)(b.x);
	 Point[1][1]= (iS32)(b.y);
	i51KitG2DrawLine( Point, 0XFF00);
}

iS32 PolygonPoint[10][2]= {0};
static void
drawPolyShape(cpBody *body, cpPolyShape *poly, cpSpace *space)
{
	int count = poly->numVerts;
	int i = 0;
	cpVect * verts = poly->tVerts;
/*
	if(body->v.x<1&&body->v.y<1&&body->v.x>-1&&body->v.y>-1)
	{
		body->v = cpv((120-body->p.x)*15,-rand()%300-300);
		if(body->v.x>500)  body->v.x = 500;
	}
*/
	for( i = 0; i< count; i++ )
	{
		 PolygonPoint[i][0]= (iS32)(verts[i].x);
		 PolygonPoint[i][1] = (iS32)(verts[i].y);
	}
	
	i51KitG2DrawPolygon(count, PolygonPoint, 0XF0F);

}

static void
drawObject(cpShape *shape, cpSpace *space)
{
	cpBody *body = shape->body;
	
	switch(shape->klass->type){
		case CP_CIRCLE_SHAPE:
			drawCircleShape(body, (cpCircleShape *)shape, space);
			break;
		case CP_SEGMENT_SHAPE:
			drawSegmentShape(body, (cpSegmentShape *)shape, space);
			break;
		case CP_POLY_SHAPE:
			drawPolyShape(body, (cpPolyShape *)shape, space);
			break;
		default:
			break;
//			printf("Bad enumeration in drawObject().\n");
	}
}


void
drawSpace(cpSpace *space, drawSpaceOptions *options)
{
	if(options->drawShapes){
		cpSpaceHashEach(space->activeShapes, (cpSpaceHashIterator)drawObject, space);
		cpSpaceHashEach(space->staticShapes, (cpSpaceHashIterator)drawObject, space);
	}

}

static void CP_FillDiamond(int i, int j, iCOLOR Color )
{
	iS32 Point[4][2];
	iS32 X = i*8;
	iS32 Y = j*8;
	Point[0][0] = X-8;
	Point[0][1] = Y;
	Point[1][0] = X;
	Point[1][1] = Y-8;
	Point[2][0] = X+8;
	Point[2][1] = Y;
	Point[3][0] = X;
	Point[3][1] = Y+8;
	i51KitG2FillPolygon(4, Point,Color);
	
}

i51InputStruct OldInputData;
#define KEY_RANGE	8
static void KeyResponse( void )
{
	i51InputStruct InputData;
	i51KitKeyInputQueueOut(KeyQueue, &InputData);
	if(InputData.type==i51_PEN_INPUT)
	{
		iU16 X = InputData.u.pen.x;
		iU16 Y = InputData.u.pen.y;
		switch(InputData.u.pen.type)
		{
			case i51_PEN_DOWN:
				if( X>=KEY_RANGE&&X<(SCREEN_WIDTH-KEY_RANGE)
				&&Y>=KEY_RANGE&&Y<(SCREEN_HEIGHT-KEY_RANGE) )
				{
					memset(&(BackgroundType[Y][X-KEY_RANGE]),0,22);

					memset(&(BackgroundType[Y+1][X-KEY_RANGE]),0,16);
					memset(&(BackgroundType[Y-1][X-KEY_RANGE]),0,16);

					memset(&(BackgroundType[Y+2][X-KEY_RANGE]),0,16);
					memset(&(BackgroundType[Y-2][X-KEY_RANGE]),0,16);

					memset(&(BackgroundType[Y+3][X-KEY_RANGE+1]),0,14);
					memset(&(BackgroundType[Y-3][X-KEY_RANGE+1]),0,14);

					memset(&(BackgroundType[Y+4][X-KEY_RANGE+1]),0,14);
					memset(&(BackgroundType[Y-4][X-KEY_RANGE+1]),0,14);

					memset(&(BackgroundType[Y+5][X-KEY_RANGE+2]),0,12);
					memset(&(BackgroundType[Y-5][X-KEY_RANGE+2]),0,12);

					memset(&(BackgroundType[Y+6][X-KEY_RANGE+3]),0,10);
					memset(&(BackgroundType[Y-6][X-KEY_RANGE+3]),0,10);

					memset(&(BackgroundType[Y+2][X-KEY_RANGE+5]),0,6);
					memset(&(BackgroundType[Y-2][X-KEY_RANGE+5]),0,6);

				}
				break;
			case i51_PEN_UP:
				break;
			case i51_PEN_LONG_PRESS:
				break;
			case i51_PEN_REPEAT:
				break;
			case i51_PEN_MOVE:
				break;
		}
	}

}
	
static void RedrawWord( void )
{
	WordFlag[12][20] = 0;
	WordFlag[18][5] = 0;
	for(int i=1; i<60; i++ )
	{
		for( int j = 1; j < 40; j++ )
		{
			if(WordFlag[i][j])
			{
				CP_FillDiamond(i,j,0XAAA0);
			}
		}
	}
}

static void display(void)
{
//i51AdeOsLog ( i51_LOG_DC , "Start display" ) ; 
//i51AdeOsLog ( i51_LOG_DC , "update(ticks)" ) ; 
	iRECT DesRect;
	update(ticks);
//i51AdeOsLog ( i51_LOG_DC , "drawSpace(mmSABSpace, &options)" ) ; 
	i51KitG2CleanScreen(0, 0);
	drawSpace(mmSABSpace, &options);
//	ticks++;
	DesRect.X = 0;
	DesRect.Y = 320;
	DesRect.Width = 320;
	DesRect.Height = 30;
	i51KitG2CleanScreen(0, &DesRect);
	//CP_FillCircle( 150, 250, 6);
	//CP_FillCircle( 170, 250, 2);
	//iS32 Point[4][2] = {{140,285},{160,255},{190,300},{170,310}};
	//CP_FillQuadrilateral(Point);
//	RedrawWord();
	DrawBackground();

	i51KitG2Update();
//	i51AdeOsSleep(1000);
}

#define STATE_IDLE	0
#define STATE_INIT	1
#define STATE_DROP	2


iS32 GameState = STATE_IDLE;
void Game()
{
	int i =0, j = 0;
	cpArray *bodies = mmSABSpace->bodies;
	cpBody * TempPoint = iNULL;
	cpVect pos;
	switch(GameState)
	{
		case STATE_INIT:
			GameState = STATE_DROP;
			mmSABSpace->gravity = cpv(0, 0);
			bodies = mmSABSpace->bodies;
			for(i=0; i<bodies->num; i++){
					TempPoint = (cpBody *)(*(bodies->arr+(i)));
					pos = cpv((i%LINK_COUNT)*4+5,(i/LINK_COUNT)*4+5);
					cpBodySetPos(TempPoint, pos);
					TempPoint->v = cpv(0,0);
			}
			bodies = mmSABSpace->sleepingComponents;
			for(j=0; j<bodies->num; j++){
				TempPoint = (cpBody *)(*(bodies->arr+(j)));
				pos = cpv((i%LINK_COUNT)*4+5,(i/LINK_COUNT)*4+5);
				cpBodySetPos(TempPoint, pos);
				TempPoint->v = cpv(0,0);
				cpBodyActivate(TempPoint);
			}
			break;
		default:
			mmSABSpace->gravity = cpv(0, 1000);
			GameState = STATE_INIT;
			bodies = mmSABSpace->sleepingComponents;
			for(j=0; j<bodies->num; j++){
				TempPoint = (cpBody *)(*(bodies->arr+(j)));
				cpBodyActivate(TempPoint);
			}
			break;
	}
}

void ShapeManage( void )
{
	cpHashSet * StaticShape = mmSABSpace->staticShapes->handleSet;
	cpHashSet * AcitveShape = mmSABSpace->activeShapes->handleSet;
	for(int i=0; i<StaticShape->size; i++)
	{
		cpHashSetBin *bin = StaticShape->table[i];
		while(bin)
		{
			cpHashSetBin *next = bin->next;	
			cpHandle *hand = (cpHandle *)(bin->elt);
			cpShape *shape = (cpShape *)hand->obj;
//			func(bin->elt, data);		
			bin = next;		
		}
	}

	for(int i=0; i<AcitveShape->size; i++)
	{
		cpHashSetBin *bin = AcitveShape->table[i];
		while(bin)
		{
			cpHashSetBin *next = bin->next;	
			cpHandle *hand = (cpHandle *)bin->elt;
			cpShape *shape = (cpShape *)hand->obj;
//			func(bin->elt, data);		
			bin = next;		
		}
	}
}

extern "C" iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter ) ;
iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{
	case i51_MSG_PAINT :
		{
			i51KitKeyInputQueueIn(KeyQueue, message, parameter);
			KeyResponse();
			display();
			ShapeManage();
		}
		break ;
	case i51_MSG_INIT :
		{
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

			//	加载i51KitG2动态库
			i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,(iU16 *)L"PKiGAPI\\i51KitG2.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE ( i51KitG2 ) )
			{
				i51AdeOsLog ( i51_LOG_DC , "加载i51KitG2动态库失败" ) ;
				return iFALSE;//	TODO : 动态库加载失败，错误处理。
			}

			//	加载i51KitStd动态库
			i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,(iU16 *)L"PKiGAPI\\i51KitStd.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE ( i51KitStd ) )
			{
				i51AdeOsLog ( i51_LOG_DC , "加载i51KitStd动态库失败" ) ;
				return iFALSE;//	TODO : 动态库加载失败，错误处理。
			}

			//	加载i51KitStd动态库
			i51KIT_DYNAMIC_KIT_LOAD(i51KitKey,(iU16 *)L"PKiGAPI\\i51KitKey.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE ( i51KitKey ) )
			{
				i51AdeOsLog ( i51_LOG_DC , "加载i51KitKey动态库失败" ) ;
				return iFALSE;//	TODO : 动态库加载失败，错误处理。
			}

			i51AdeOsLog ( i51_LOG_DC , "----------加载动态库成功" ) ; 

			i51AdeOsScreenAlwaysLight(1);
			WordInit();
			i51AdeOsLog ( i51_LOG_DC , "WordInit OK" ) ; 
		}
		break ;

	case i51_MSG_RESUME:
		break ;

	case i51_MSG_KEYDOWN:
	case i51_MSG_KEYUP:
	case i51_MSG_PADUP:
	case i51_MSG_PADMOVE:
	case i51_MSG_PADDOWN:
		i51KitKeyInputQueueIn(KeyQueue, message, parameter);
			Game();
			break;
	case i51_MSG_EXIT:
		cpSpaceFreeChildren(mmSABSpace);
		cpSpaceFree(mmSABSpace);
		break;
	}
	return iTRUE ;

}

iKIT iU32* i51KitPort ()
{
	iU32 * Temp = iNULL;
	return Temp ;
}
