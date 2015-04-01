
// 引入头文件
// #include "MexLib.h"
// #include "MexLib_Std.h"
// #include "MexLib_Os.h"
// #include "MexLib_Gui.h"
// #include "MexLib_Phs.h"
// 
// 声明库函数
// funcptr *MEX_C_LIB_FUNC_NAME_STD = NULL;
// funcptr *MEX_C_LIB_FUNC_NAME_OS = NULL;
// funcptr *MEX_C_LIB_FUNC_NAME_GUI = NULL;
// funcptr *MEX_C_LIB_FUNC_NAME_PHS = NULL;
// 
// 标准程序开始函数
// MEX_EXPORT E_Mex_Startup_Result MexApp_Start(int32 iMyAppId, MexApp_LoadLib_Func fnLoadLib, MexApp_FreeLib_Func fnFreeLib);
// 
// E_Mex_Startup_Result MexApp_Start(int32 iMyAppId, MexApp_LoadLib_Func fnLoadLib, MexApp_FreeLib_Func fnFreeLib)
// {
// 	E_Mex_Startup_Result ret = E_MSR_NoError;
// 	extern void TestBox2DLite_Exit(int32 iAppId, MexApp_FreeLib_Func fnFreeLib);
// 	extern int32 TestBox2DLite_MsgHandler(int32 iSrcAppId, int32 iDstAppId, E_Mex_MessageType eMsgType, int32 iMsgData0, int32 iMsgData1, int32 iMsgData2, int32 iMsgData3);
// 
// 	MEX_C_LIB_FUNC_NAME_STD = fnLoadLib( L"Std", L"MexLib_Std.mel" );
// 	MEX_C_LIB_FUNC_NAME_OS = fnLoadLib( L"Os", L"MexLib_Os.mel" );
// 	MEX_C_LIB_FUNC_NAME_GUI = fnLoadLib( L"Gui", L"MexLib_Gui.mel" );
//     MEX_C_LIB_FUNC_NAME_PHS = fnLoadLib( L"Phs", L"MexLib_Phs.mel" );
// 
// 	if (MEX_C_LIB_FUNC_NAME_STD == NULL ||
//         MEX_C_LIB_FUNC_NAME_OS == NULL ||
//         MEX_C_LIB_FUNC_NAME_GUI == NULL ||
//         MEX_C_LIB_FUNC_NAME_PHS == NULL)
// 	{
// 		ret = E_MSR_NoLib;
// 		goto StartError;
// 	}
// 
// 	RegisterExitFunc( iMyAppId, TestBox2DLite_Exit, false );
// 	RegisterMsgHandler( iMyAppId, TestBox2DLite_MsgHandler );
// 
// 	return ret;
// 
// StartError:
// 	TestBox2DLite_Exit( iMyAppId, fnFreeLib );
// 	return ret;
// }


#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitStd.h"
#include "i51KitKey.h"
#include "world.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);
i51KIT_DYNAMIC_KIT_DEF(i51KitKey);

// void TestBox2DLite_Exit(int32 iMyAppId, MexApp_FreeLib_Func fnFreeLib)
// {
// 	if (MEX_C_LIB_FUNC_NAME_STD != NULL)
// 		fnFreeLib( MEX_C_LIB_FUNC_NAME_STD );
// 	if (MEX_C_LIB_FUNC_NAME_OS != NULL)
// 		fnFreeLib( MEX_C_LIB_FUNC_NAME_OS );
// 	if (MEX_C_LIB_FUNC_NAME_GUI != NULL)
// 		fnFreeLib( MEX_C_LIB_FUNC_NAME_GUI );
//     if (MEX_C_LIB_FUNC_NAME_PHS != NULL)
//         fnFreeLib( MEX_C_LIB_FUNC_NAME_PHS );
// }

////////////////////////////////////////////////////////////////////////////
// 程序代码区（各种函数声明和实现请写在这段注释下面）
////////////////////////////////////////////////////////////////////////////

static iFILE FontFile = iNULL;
static iHANDLE KeyQueue = iNULL;

void * operator new(size_t size)
{
//    return malloc(size);
//	iLog("operator new");
    return i51AdeOsMalloc(size, __FILE__, __LINE__);
}

void operator delete(void *p)
{
//	free(p);
//	iLog("operator delete");
	i51AdeOsFree(p);
}

#include "Box2DDemos.h"


void DrawText(int x, int y, const char *pStr)
{

// 	uint16 *pScrBuffer;
// 	uint16 uWidth, uHeight;
// 
// 	GetFullScreenBuffer( &pScrBuffer, &uWidth, &uHeight );
// 
// 	GUI_BEGIN(pScrBuffer, uWidth, uHeight);
// 
// 	DrawString(
// 		tstr2wcs(pStr),
// 		x,
// 		y,
// 		MEX_COLOR_GREEN,
// 		MEX_FONTATTR_NORMAL,
// 		MEX_FONTSIZE_MEDIUM);
// 
// 	GUI_END();
iU32 DrawWidth = 0;
iU32 DrawHeight = 0;
iRECT DrawRect;

i51KitG2FontGetDemension(FontFile, 0, strlen(pStr), strlen(pStr), 0, 0, 0, &DrawWidth, &DrawHeight);
DrawRect.X = x;
DrawRect.Y = y;
DrawRect.Width = DrawWidth;
DrawRect.Height = DrawHeight;
i51KitG2FontDraw(FontFile, 0, (void *)pStr, 0, &DrawRect, &DrawRect, 0, 0, 0, 0XAA00, 0X00AA, 0);

}

Mat22 R;//(body->rotation);
Vec2 x;// = body->position;
Vec2 h;// = 0.5f * body->width;

Vec2 offset;//((float)(uWidth / 2), (float)(uHeight / 5));

Vec2 v1; //= x + R * Vec2(-h.x, -h.y);
Vec2 v2; //= x + R * Vec2( h.x, -h.y);
Vec2 v3; //= x + R * Vec2( h.x,  h.y);
Vec2 v4; //= x + R * Vec2(-h.x,  h.y);

void DrawBody(Body* body)
{
// 	uint16 *pScrBuffer;
// 	uint16 uWidth, uHeight;
// 	GetFullScreenBuffer( &pScrBuffer, &uWidth, &uHeight );
	iU16 *pScrBuffer;
	iU16 uWidth=0, uHeight=0;
	iS32 Point[2][2];
	if( i51AdeMmiGetScreenScale ( &uWidth , &uHeight ) == 0 ) return;

//	GetFullScreenBuffer( &pScrBuffer, &uWidth, &uHeight );

//iLog("Start DrawBody ");
    R.Mat22_1(body->rotation);
    x = body->position;
    h = 0.5f * body->width;

    offset.Set((float)(uWidth / 2), (float)(uHeight / 5));

    v1 = x + R * Vec2(-h.x, -h.y);
    v2 = x + R * Vec2( h.x, -h.y);
    v3 = x + R * Vec2( h.x,  h.y);
    v4 = x + R * Vec2(-h.x,  h.y);
//#if 1
    float factor = 10.0f;
    switch ( demoIndex )
    {
    case 1:
        factor = 30.0f;
        break;
    case 2:
        factor = 12.0f;
        break;
    case 5:
        factor = 15.0f;
        break;
    case 6:
        factor = 15.0f;
        break;
    case 9:
        factor = 6.0f;
        break;
    default:
        factor = 10.0f;
        break;
    }

    v1 *= factor;
    v2 *= factor;
    v3 *= factor;
    v4 *= factor;

    v1 += offset;
    v2 += offset;
    v3 += offset;
    v4 += offset;

    v1.y = uHeight - v1.y;
    v2.y = uHeight - v2.y;
    v3.y = uHeight - v3.y;
    v4.y = uHeight - v4.y;

    //log(MEX_LOG_APP0, "(%d, %d), (%d, %d)", (int16)v1.x, (int16)v1.y, (int16)v2.x, (int16)v2.y);
    //log(MEX_LOG_APP0, "(%d, %d), (%d, %d)", (int16)v2.x, (int16)v2.y, (int16)v3.x, (int16)v3.y);
    //log(MEX_LOG_APP0, "(%d, %d), (%d, %d)", (int16)v3.x, (int16)v3.y, (int16)v4.x, (int16)v4.y);
    //log(MEX_LOG_APP0, "(%d, %d), (%d, %d)", (int16)v4.x, (int16)v4.y, (int16)v1.x, (int16)v1.y);
// 
// 	GUI_BEGIN(pScrBuffer, uWidth, uHeight);
// 	DrawLine((int16)v1.x, (int16)v1.y, (int16)v2.x, (int16)v2.y, MEX_COLOR_WHITE);
// 	DrawLine((int16)v2.x, (int16)v2.y, (int16)v3.x, (int16)v3.y, MEX_COLOR_WHITE);
// 	DrawLine((int16)v3.x, (int16)v3.y, (int16)v4.x, (int16)v4.y, MEX_COLOR_WHITE);
// 	DrawLine((int16)v4.x, (int16)v4.y, (int16)v1.x, (int16)v1.y, MEX_COLOR_WHITE);
// 	GUI_END();
//	GUI_BEGIN(pScrBuffer, uWidth, uHeight);
//#endif
	Point[0][0] = (iS32)v1.x;
	Point[0][1] = (iS32)v1.y;
	Point[1][0] = (iS32)v2.x;
	Point[1][1] = (iS32)v2.y;
	i51KitG2DrawLine(Point, 0XFF00);
	Point[0][0] = (iS32)v2.x;
	Point[0][1] = (iS32)v2.y;
	Point[1][0] = (iS32)v3.x;
	Point[1][1] = (iS32)v3.y;
	i51KitG2DrawLine(Point, 0XFF00);
	Point[0][0] = (iS32)v3.x;
	Point[0][1] = (iS32)v3.y;
	Point[1][0] = (iS32)v4.x;
	Point[1][1] = (iS32)v4.y;
	i51KitG2DrawLine(Point, 0XFF00);
	Point[0][0] = (iS32)v4.x;
	Point[0][1] = (iS32)v4.y;
	Point[1][0] = (iS32)v1.x;
	Point[1][1] = (iS32)v1.y;
	i51KitG2DrawLine(Point, 0XFF00);
//iLog("End DrawBody ");
}

Mat22 R1;//(b1->rotation);
Mat22 R2;//(b2->rotation);
Vec2 x1 ;//= b1->position;
static Vec2 p1 ;//= x1 + R1 * joint->localAnchor1;

Vec2 x2 ;//= b2->position;
static Vec2 p2 ;//= x2 + R2 * joint->localAnchor2;
//#if 1

//Vec2 offset;//((float)(uWidth / 2), (float)(uHeight / 5));
void DrawJoint(Joint* joint)
{
    float factor = 10.0f;
    iS32 Point[2][2];
//iLog("Start DrawJoint ");
    switch ( demoIndex )
    {
    case 2:
        factor = 12.0f;
        break;
    case 6:
        factor = 15.0f;
        break;
    default:
        return;
    }

    Body* b1 = joint->body1;
    Body* b2 = joint->body2;

    R1.Mat22_1(b1->rotation);
    R2.Mat22_1(b2->rotation);

    x1 = b1->position;
    p1 = x1 + R1 * joint->localAnchor1;

    x2 = b2->position;
    p2 = x2 + R2 * joint->localAnchor2;
//#if 1

    iU16 *pScrBuffer;
    iU16 uWidth=240, uHeight=320;
	if( i51AdeMmiGetScreenScale ( &uWidth , &uHeight ) == 0 ) return;
    //GetFullScreenBuffer( &pScrBuffer, &uWidth, &uHeight );

    offset.Set((float)(uWidth / 2), (float)(uHeight / 5));

    x1 *= factor;
    p1 *= factor;
    x2 *= factor;
    p2 *= factor;

    x1 += offset;
    p1 += offset;
    x2 += offset;
    p2 += offset;

    x1.y = uHeight - x1.y;
    p1.y = uHeight - p1.y;
    x2.y = uHeight - x2.y;
    p2.y = uHeight - p2.y;


// 	GUI_BEGIN(pScrBuffer, uWidth, uHeight);
// 	DrawLine((int16)x1.x, (int16)x1.y, (int16)p1.x, (int16)p1.y, MEX_COLOR_RED);
// 	DrawLine((int16)x2.x, (int16)x2.y, (int16)p2.x, (int16)p2.y, MEX_COLOR_RED);
// 	GUI_END();
//#endif
	Point[0][0] = (iS32)x1.x;
	Point[0][1] = (iS32)x1.y;
	Point[1][0] = (iS32)p1.x;
	Point[1][1] = (iS32)p1.y;
	i51KitG2DrawLine(Point, 0XFF00);
	Point[0][0] = (iS32)x2.x;
	Point[0][1] = (iS32)x2.y;
	Point[1][0] = (iS32)p2.x;
	Point[1][1] = (iS32)p2.y;
	i51KitG2DrawLine(Point, 0XFF00);
//iLog("End DrawJoint ");
}

//模式初始化函数
//int32 TestBox2DLite_Init(void)
iS32 TestBox2DLite_Init(void)
{
	iU16 *pScrBuf;
	iU16 uScrWidth, uScrHeight;
// 	//取得系统屏幕buf信息
// 	GetFullScreenBuffer( &pScrBuf, &uScrWidth, &uScrHeight );
// 	//注册应用buf信息
// 	RegisterScreenBuffer( pScrBuf, uScrWidth, uScrHeight );
// 
// 	SetAppAttribute(GetCurrentApp(), MEX_APP_ATTRIB_FULL_NO_NEED_LAST_SCREEN);
// 
//     memset16( pScrBuf, MEX_COLOR_BLACK, uScrWidth * uScrHeight );
//     Phs_Set_Backlight(E_MPHS_PERMANENT);
// 
//     log(MEX_LOG_APP0, "Test sqrtf(100.0f) = %d", (int32)sqrtf(100.0f));
//     log(MEX_LOG_APP0, "Test fabsf(-0.005) * 1000 = %d", (int32)(fabsf(-0.005f)*1000));

// 	bodies = (Body*)smalloc(sizeof(Body) * 80);
// 	joints = (Joint*)smalloc(sizeof(Joint) * 80);
	bodies = (Body*)i51AdeOsMalloc((sizeof(Body) * 80),__FILE__,__LINE__);
	joints = (Joint*)i51AdeOsMalloc((sizeof(Joint) * 80),__FILE__,__LINE__);
	memset(bodies,(sizeof(Body) * 80),0);
	memset(joints,(sizeof(Joint) * 80),0);
	iterations = 3;
//iLog("bodies=%d",bodies);
//iLog("joints=%d",joints);
    world.gravity.x = 0.0f;
    world.gravity.y = -10.0f;
    world.iterations = iterations;
// for(int i=0; i<361;i++ )
// {
// 	float result1 = 0.0;
// 	float result2 = 0.0;
// 	result1 = cosf((float)(i)/180*k_pi);
// 	result2 = sinf((float)(i)/180*k_pi);
// 	iLog("i=%d\t%f\t%f",i,result1,result2);
// }
// 	{
// 		bool bTest = LoadAppIntoSRam(GetCurrentApp());
// 		log(MEX_LOG_APP0, "bTest is %d", bTest);
// 	}
// 
// 	return E_MIMR_Ok;
	return 1;
}

//绘图函数，每帧系统要把程序往屏幕上贴之前会发送Paint消息
//参数 pScrBuffer ：注册的buf指针
//参数 uWidth	  ：buf宽
//参数 uHeight	  ：buf高
//int32 TestBox2DLite_Paint(uint16 *pScrBuffer, uint16 uWidth, uint16 uHeight)
iS32 TestBox2DLite_Paint(void)
{
// 	World vWorld;
// 	//MyPair<int, int> MyArbPair;
// 	typedef MyPair<int, int> MyArbPair;
// 	int nTesta=1;
// 	int nTestb=2;
//     vWorld.arbiters.insert(MyArbPair(nTesta, nTestb));
// 	//arbiters.insert(ArbPair(key, newArb));
// 	 return 1;
//	static int32 iLastTime = gettime() - 33;
	static iS32 iLastTime = i51AdeOsGetTick() - 33;
	timeStep = (i51AdeOsGetTick() - iLastTime) / 1000.0f;
	if (timeStep > 0.05f)
		timeStep = 0.05f;
	iLastTime = i51AdeOsGetTick();
//i51AdeOsLog(0,"start TestBox2DLite_Paint");
//    memset( pScrBuffer, 0, uWidth * uHeight * 2 );
i51KitG2CleanScreen(0XAA,iNULL);

////i51AdeOsLog(0,"demoIndex=%d",demoIndex);

 //   if (demoIndex > 0)
//        DrawText(5, 15, demoStrings[demoIndex-1]);
    DrawText(5, 45, "Keys: 1-9 Demos");
    if (demoIndex > 0)
        DrawText(5, 75, "OK to Launch the Bomb");
//iLog("DrawText");
//i51AdeOsLog(0,"Draw Text!");
    world.Step(timeStep);
//iLog("world.Step(timeStep)");
//i51AdeOsLog(0,"numBodies=%d,numJoints=%d",numBodies,numJoints);
//iLog("numJoints=%d",numJoints);
    for (int i = 0; i < numBodies; ++i)
    {
 //   	iLog("i=%d",i);
        DrawBody(bodies + i);
    }

    for (int i = 0; i < numJoints; ++i)
    {
  //  	iLog("i=%d",i);
        DrawJoint(joints + i);
    }
//i51AdeOsLog(0,"Draw End!");

i51KitG2Update();
//i51AdeOsLog(0,"End TestBox2DLite_Paint");
    return 1;
}

//屏触响应
//int32 TestBox2DLite_MouseEvent( E_Mex_MouseEvent eEvent, int16 iPosX, int16 iPosY )
iS32 TestBox2DLite_MouseEvent(i51InputStruct InputData)
{
    iU16 uWidth=240, uHeight=320;
	if( i51AdeMmiGetScreenScale ( &uWidth , &uHeight ) == 0 ) return iFALSE;
    if(InputData.type==i51_PEN_INPUT&&InputData.u.pen.type==i51_PEN_UP)
    {
        iU16 PenX = InputData.u.pen.x;
        iU16 PenY = InputData.u.pen.y;
	if(PenY<uHeight/4)
	{
		if(PenX<uWidth/3) InitDemo(1);
		else if( PenX<uWidth*2/3) InitDemo(2);
		else InitDemo(3);
	}
	else if(PenY<uHeight*2/4)
	{
		if(PenX<uWidth/3) InitDemo(4);
		else if( PenX<uWidth*2/3) InitDemo(5);
		else InitDemo(6);
	}
	else if(PenY<uHeight*3/4)
	{
		if(PenX<uWidth/3) InitDemo(7);
		else if( PenX<uWidth*2/3) InitDemo(8);
		else InitDemo(9);
	}
	else
	{
            LaunchBomb();
	}
    }
	return 1;
}

//按键响应
//int32 TestBox2DLite_KeyEvent(E_Mex_KeyEvent keyEvent, E_Mex_KeyCode keyCode)
iS32 TestBox2DLite_KeyEvent(void)
{
i51KEYS keyCode;
i51InputStruct InputData;
i51KitKeyInputQueueOut(KeyQueue, &InputData);
if(InputData.type==i51_KEY_INPUT&&InputData.u.key.type==i51_KEY_UP)
{
i51KEYS keyCode = InputData.u.key.value;
    switch ( keyCode )
    {
    case i51_KEY_NUM0:
        InitDemo(0);
        break;
    case i51_KEY_NUM1:
        InitDemo(1);
        break;
    case i51_KEY_NUM2:
        InitDemo(2);
        break;
    case i51_KEY_NUM3:
        InitDemo(3);
        break;
    case i51_KEY_NUM4:
        InitDemo(4);
        break;
    case i51_KEY_NUM5:
        InitDemo(5);
        break;
    case i51_KEY_NUM6:
        InitDemo(6);
        break;
    case i51_KEY_NUM7:
        InitDemo(7);
        break;
    case i51_KEY_NUM8:
        InitDemo(8);
        break;
    case i51_KEY_NUM9:
        InitDemo(9);
        break;
    case i51_KEY_ENTER:
        LaunchBomb();
        break;
    }
}

TestBox2DLite_MouseEvent(InputData);

    return 1;
}

//模式退出函数
//int32 TestBox2DLite_Destroy(void)
iS32 TestBox2DLite_Destroy(void)
{
// 	sfree(bodies);
// 	sfree(joints);

	i51AdeOsFree(bodies);
	i51AdeOsFree(joints);

// 	//删除buf注册
// 	UnRegisterScreenBuffer();
// 
//     Phs_Set_Backlight(E_MPHS_OFF);
// 
// 	FreeAppFromSRam(GetCurrentApp());
// 
	return 1;
}

//函数MyApp_MsgHandler为应用消息的监听者，在第一个函数MexApp_Start中被注册。
//参数iSrcAppId ：消息源应用ID，SYSTEM为0。
//参数iDstAppId ：消息目标应用ID（比为本应用ID）。
//参数eMsgType ：消息类型，参见enum E_Mex_MessageType。
//参数iMsgData0 ~ iMsgData3：消息数据0-3，根据消息类型有特定意义。
//返回 int32     ：处理结果，0-未处理，1-已处理
extern "C" iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter ) ;
iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{


	//	TODO :	消息循环
	switch ( message )
	{
		case i51_MSG_PAINT :
//i51AdeOsLog(0, "Start i51_MSG_PAINT!");
i51KitKeyInputQueueIn(KeyQueue, message, parameter);
TestBox2DLite_KeyEvent();
TestBox2DLite_Paint();
//i51AdeOsLog(0, "End i51_MSG_PAINT!");
			break;
		case i51_MSG_KEYUP :
		case i51_MSG_KEYDOWN :
//i51AdeOsLog(0, "Start i51KitKeyInputQueueIn!");
			i51KitKeyInputQueueIn(KeyQueue, message, parameter);
//i51AdeOsLog(0, "End i51KitKeyInputQueueIn!");
			break ;
		case i51_MSG_PADDOWN:
		case i51_MSG_PADUP:
//i51AdeOsLog(0, "Start i51KitKeyPADQueueIn!");
			i51KitKeyInputQueueIn(KeyQueue, message, parameter);
//i51AdeOsLog(0, "End i51KitKeyPADQueueIn!");
			break;			
		case i51_MSG_INIT :
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

////i51AdeOsLog(0,"Adapter");

	             //	加载i51KitStd动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,(iU16*)L"PKiGAPI\\i51KitStd.kit");
////i51AdeOsLog(0,"PKiGAPI\\i51KitStd.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ) 
	             {
	                 return 0 ;
	             }
////i51AdeOsLog(0,"i51KitStd");
			 
	             //	加载i51KitStd动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,(iU16*)L"PKiGAPI\\i51KitG2.kit");
////i51AdeOsLog(0,"PKiGAPI\\i51KitG2.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ) 
	             {
	                 return 0 ;
	             }
	             //	加载i51KitKey动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitKey,(iU16*)L"PKiGAPI\\i51KitKey.kit");
////i51AdeOsLog(0,"PKiGAPI\\i51KitG2.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitKey) ) 
	             {
	                 return 0 ;
	             }
//i51KernelSetCache(L"i51KitG2");

////i51AdeOsLog(0,"i51KitG2");
    i51KitG2CleanScreen(0,iNULL);
	i51KitG2Update();
////i51AdeOsLog(0, "Init OK");
FontFile = i51KitStdResOpen((iU16*)L"Fonts\\Typehead.re");
KeyQueue=i51KitKeyInputQueueCreat();
TestBox2DLite_Init();
             InitDemo(4);
			i51AdeOsScreenAlwaysLight(1);
			break ;
		case i51_MSG_RUN :			i51KernelSetFPS(120,0) ;			break ;
		case i51_MSG_PAUSE :
			break ;
		case i51_MSG_EXIT :
			i51AdeOsScreenAlwaysLight(0);

TestBox2DLite_Destroy();
			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitStd);
			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitG2);
			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitKey);
			break ;
	}

	return iTRUE ;

}

