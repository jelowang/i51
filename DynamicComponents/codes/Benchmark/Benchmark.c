
#include "i51KitAde.h"
#include "Benchmark.h"
#include "i51KitG2.h"
#include "i51KitStd.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);


#define 	BENCHMARK_IDLE	-1
#define 	BENCHMARK_CPU	1
#define 	BENCHMARK_IO		2
#define 	BENCHMARK_G2		3
#define	BENCHMARK_RESULT	4

#define 	BENCHMARK_INT		0
#define 	BENCHMARK_FLOAT	1
#define 	BENCHMARK_FILE	2
#define 	BENCHMARK_SEEK	3
#define 	BENCHMARK_WRITE	4
#define 	BENCHMARK_READ	5
#define 	BENCHMARK_RAM	6
#define 	BENCHMARK_GRAPHA	7
#define 	BENCHMARK_IMAGE	8
#define 	BENCHMARK_FONT	9

#define MAX_TEST_NUM_CPU	50
#define MAX_TEST_NUM_IO	10
#define MAX_TEST_NUM_G2	50
#define MAX_TEST_NUM_FONT	20

static iS32 BenchmarkState =  BENCHMARK_IDLE;
static iS32 CurrTestNum = 0;
static iU32 CurrSysTime = 0;
static iU32 OldSysTime = 0;
static iU32 AllTestTime[10] = {0};
static iU32 ARM7Time[10] = {12,260,600,6,100,16,20,92,253,240};
static iU32 ARM9Time[10] = {1,16,200,1,6,6,4,4,20,100};
	
static iFILE TypeheadHandle = iNULL;
static iU8 DecodeComplete = iFALSE;
static iU8 FLAG_FIRSTRUN = iTRUE;
static iHANDLE CurrentImageContainer = iNULL;
static iS32 SysAlpha = i51KITG2_ALPHA_NONEED, MulAlpha = -5;
static iU8 NeedAlpha = iFALSE;

static iU16 ScreenWidth = 0;
static iU16 ScreenHeight = 0;

static iU8 ResultFlag = iTRUE;

static iHANDLE VP_BK = iNULL;
static iHANDLE VP_Road = iNULL;
static iHANDLE VP_Snidget  = iNULL;
static iHANDLE VP_Bee = iNULL;

enum
{
BG,	
RUN_LEFT,	
RUN_RIGHT,	
WALL,	
BEE,	
ORANGE,	
PINEAPPLE,	
STRAWBERRY,	
WATERMELON,	
APPLE,	
FRUIT_BG1,	
RESULT
};

iU32 ImageId[12] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE,ORANGE,
	PINEAPPLE,STRAWBERRY,WATERMELON,APPLE,FRUIT_BG1,RESULT};

static iBOOL Num2Str( iU32 Number, iCHAR * Str, iU16 Base)
{
	iU32 Divisor = 1; 
	iU32 iTemp = 0;

	if(Number==0)
	{
		Str[0] = '0';
		Str[1] = '\0';
		return iTRUE;
	}
	
	while( 1 )
	{
		if(Number>=Divisor*10) Divisor *= 10;
		else break;
	}
	
	while( Number>0 )
	{
		Str[iTemp++] = '0' + Number/Divisor;
		Number %= Divisor;
		if(Number==0&&Divisor>1)
		{
			while(Divisor>1)
			{
				Str[iTemp++] = '0';
				Divisor /= 10;
			}
		}	
		if(Divisor>=10) Divisor /= 10;
	}

	Str[iTemp] = '\0';

	return iTRUE;
	
}
	
static iBOOL ShowNumber(iS32 Number, iS16 X, iS16 Y , iCOLOR Color)
{
    iCHAR Str[15] = {0};
    iRECT DrawRect;
//    i51AdeStdItoa(&Str, Number, 10);
//    itoa(Number, Str, 10);
    Num2Str(Number, (iCHAR*)Str, 10);
    DrawRect.X = X;
    DrawRect.Y = Y;
    DrawRect.Width = 100;
    DrawRect.Height = 100;
    i51KitG2FontDraw(TypeheadHandle, 2, Str, 0, &DrawRect,iNULL,0,0,0,Color,0,0);
    return iTRUE;
}

static iBOOL TestIDLE( void )
{
	iRECT StrRect;
	iU8 * Str1 = (iU8 *)"系统测试即将开始";
	iU8 * Str2 = (iU8 *)"按任意键继续……";
	iU32 iTemp = 0;
	i51KitG2CleanScreen(0XAAAA,iNULL);
	StrRect.X = 55;
	StrRect.Y = 100;
	StrRect.Width = 160;
	StrRect.Height = 20;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str1,0,&StrRect,iNULL,0,0,0,0XEEEE,0,0);
	StrRect.Y = 200;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str2,0,&StrRect,iNULL,0,0,0,0XEEEE,0,0);

	CurrTestNum = 0;
	for( iTemp = 0; iTemp<10; iTemp++)
	{
		AllTestTime[iTemp] = 0;
	}
	ResultFlag = iTRUE;
	
	return iTRUE;
	
}


static iBOOL DrawCPU( void )
{
	iU16 * Str1 = (iU16*)"等待测试IO模块";
	iU16 * Str2 = (iU16*)"正在测试CPU模块 ......";
	iU16 * Str3 = (iU16*)"等待测试G2模块";
	static iU32 Num = 0;
	iRECT StrRect;

	i51KitG2CleanScreen(0X0,iNULL);
	StrRect.X = 15;
	StrRect.Y = 120;
	StrRect.Width = 180;
	StrRect.Height = 20;
	Num ++;
	if(Num>35) Num = 0;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str1,0,&StrRect,iNULL,0,0,0,0XFF00,0,0);
	StrRect.Y = 40;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str2,9+(Num/5),&StrRect,iNULL,0,0,0,0X07E0,0,0);
	StrRect.Y = 200;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str3,0,&StrRect,iNULL,0,0,0,0XFF00,0,0);

	return iTRUE;
	
}

static iU32 TestCPU( void )
{
	static iU32 TestState = 0;
	int i = 0, j = 0, k = 0;
	float Temp = 0.0;

	DrawCPU();
	
	OldSysTime = CurrSysTime;

	switch( TestState)
	{
		case 0:
			OldSysTime = i51AdeOsGetTick();
			for( i=0;i<20;i++ )
			{
				for( j=100;j>0;j--)
				{
					k = i*j;
					k = k + 346;
					k = (k<<3);
					k = k - 567;
					k = k%10000;
					k = k/4;
					k = (k>>3);
				}
			}
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_INT] += CurrSysTime - OldSysTime;
			TestState = 1;
			break;
		case 1:	
			OldSysTime = i51AdeOsGetTick();
			for( i=0;i<20;i++ )
			{
				for( j=100;j>0;j--)
				{
					Temp = i*j*2.3;
					Temp = Temp + 346.78;
					Temp = Temp*16.0;
					Temp = Temp - 567.56;
					Temp = Temp/2.0;
					Temp = Temp/8.0;
					Temp = Temp/4.3;
				}
			}
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_FLOAT] += CurrSysTime - OldSysTime;
			TestState = 2;
			break;
 		case 2:	
			CurrTestNum++;
			if(CurrTestNum>MAX_TEST_NUM_CPU)
			{
				BenchmarkState = BENCHMARK_IO;
				CurrTestNum = 0;
			}
			else 
			{
				TestState = 0;
			}
		default : break;	
	}

	return ((iU32)(Temp+k));
		
}

static iBOOL DrawIO( void )
{
	iU16 * Str1 = (iU16*)"正在测试IO模块 ......";
	iU16 * Str2 = (iU16*)"CPU模块测试完成";
	iU16 * Str3 = (iU16*)"等待测试G2模块";
	static iU32 Num = 0;
	iRECT StrRect;

	i51KitG2CleanScreen(0X0,iNULL);
	StrRect.X = 15;
	StrRect.Y = 120;
	StrRect.Width = 180;
	StrRect.Height = 20;
	Num ++;
	if(Num>35) Num = 0;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str1,9+(Num/5),&StrRect,iNULL,0,0,0,0X07E0,0,0);
	StrRect.Y = 40;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str2,0,&StrRect,iNULL,0,0,0,0X07E0,0,0);
	StrRect.Y = 200;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str3,0,&StrRect,iNULL,0,0,0,0XFF00,0,0);
	
	return iTRUE;
	
}

static iBOOL TestIO( void )
{
	iU8 Ret = iFALSE;
	iU8 Data8 = 0XEE;
	iU16 Data16 = 0XAAAA;
	iU32 Data32 = 0XBBBBBBBB;
	iU8 DataArr[1024] = {0};
	iU32 WriteNum = 0;
	iU32 ReadNum = 0;
	iS32 FileLen = 0;
	iU8 iTemp = 0;
	iU8 * DataMalloc = iNULL;
	static iU32 TestState = 0;
	iFILE Fp = iNULL;
	
	DrawIO();

	OldSysTime = CurrSysTime;
	switch( TestState)
	{
		case 0:
			OldSysTime = i51AdeOsGetTick();
			i51AdeOsFileIsExist(L"Benchmark\\Dir\\Test");
			if(Ret==iFALSE) i51AdeOsFileCreateDir(L"Benchmark\\Dir");
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_FILE] += CurrSysTime - OldSysTime;
			TestState = 1;
			break;
		case 1:	
			Fp = i51AdeOsFileOpen( L"Benchmark\\Dir\\Test", i51_OS_FL_WRITE |i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ );
			OldSysTime = i51AdeOsGetTick();
			i51AdeOsFileWriteInt8( Fp, &Data8);
			i51AdeOsFileWriteInt16( Fp, &Data16);
			i51AdeOsFileWriteInt32( Fp, &Data32);
			for( iTemp=0;iTemp<10;iTemp++ )
			{
				i51AdeOsFileWrite(Fp, DataArr, 1024, &WriteNum);
			}
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_WRITE] += CurrSysTime - OldSysTime;
			i51AdeOsFileFlush(Fp);
			i51AdeOsFileClose(Fp);
			TestState = 2;
			break;
		case 2:
			OldSysTime = i51AdeOsGetTick();
			i51AdeOsFileCopy(L"Benchmark\\Dir\\Test",L"Benchmark\\Dir\\Test1");
			i51AdeOsFileDelete(L"Benchmark\\Dir\\Test");
			Fp = i51AdeOsFileOpen( L"Benchmark\\Dir\\Test", i51_OS_FL_WRITE |i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ );
			i51AdeOsFileFlush(Fp);
			i51AdeOsFileClose(Fp);
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_FILE] += CurrSysTime - OldSysTime;
			TestState = 3;
			break;
		case 3:	
			OldSysTime = i51AdeOsGetTick();
			Fp = i51AdeOsFileOpen( L"Benchmark\\Dir\\Test", i51_OS_FL_READ, __FILE__ , __LINE__ );
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_FILE] += CurrSysTime - OldSysTime;

			OldSysTime = i51AdeOsGetTick();
			i51AdeOsFileSetSeek(Fp, 7, i51_OS_FL_SEEK_HEAD);
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_SEEK] += CurrSysTime - OldSysTime;

			OldSysTime = i51AdeOsGetTick();
			DataMalloc = i51AdeOsMalloc(10240, __FILE__ , __LINE__ );
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_RAM] += CurrSysTime - OldSysTime;

			for( iTemp=0;iTemp<10;iTemp++ )
			{
				OldSysTime = i51AdeOsGetTick();
				i51AdeOsFileRead(Fp, DataArr, 1024, &ReadNum);
				CurrSysTime = i51AdeOsGetTick();
				AllTestTime[BENCHMARK_READ] += CurrSysTime - OldSysTime;

				OldSysTime = i51AdeOsGetTick();
				i51AdeStdMemcpy16(DataMalloc+1024*iTemp, DataArr, 1024);
				CurrSysTime = i51AdeOsGetTick();
				AllTestTime[BENCHMARK_RAM] += CurrSysTime - OldSysTime;
			}

			OldSysTime = i51AdeOsGetTick();
			i51AdeOsFree(DataMalloc);
			DataMalloc = i51AdeOsMalloc(102400, __FILE__ , __LINE__ );
			for( iTemp=0;iTemp<100;iTemp++ )
			{
				i51AdeStdMemcpy16(DataMalloc+1024*iTemp, DataArr, 1024);
			}
			for( iTemp=0;iTemp<100;iTemp++ )
			{
				i51AdeStdMemcpy16(DataMalloc+1024*iTemp, DataArr, 1024);
			}
			i51AdeOsFree(DataMalloc);
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_RAM] += CurrSysTime - OldSysTime;
			DataMalloc = iNULL;

			OldSysTime = i51AdeOsGetTick();
			i51AdeOsFileGetSize(Fp, &FileLen);
			i51AdeOsFileSetSeek(Fp, FileLen/2, i51_OS_FL_SEEK_HEAD);
			i51AdeOsFileSetSeek(Fp, -FileLen/4, i51_OS_FL_SEEK_CURRENT);
			i51AdeOsFileSetSeek(Fp, FileLen/2, i51_OS_FL_SEEK_CURRENT);
			i51AdeOsFileSetSeek(Fp, -FileLen, i51_OS_FL_SEEK_TAIL);
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_SEEK] += CurrSysTime - OldSysTime;

			OldSysTime = i51AdeOsGetTick();
			i51AdeOsFileReadInt8(Fp, &Data8);
			i51AdeOsFileReadInt16(Fp, &Data16);
			i51AdeOsFileReadInt32(Fp, &Data32);
			i51AdeOsFileClose(Fp);
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_READ] += CurrSysTime - OldSysTime;
			TestState = 4;
			break;
		case 4:	
			OldSysTime = i51AdeOsGetTick();
			i51AdeOsFileMove(L"Benchmark\\Dir\\Test",L"Benchmark\\Dir\\NewTest");
			i51AdeOsFileDeleteDir(L"Benchmark\\Dir");
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_FILE] += CurrSysTime - OldSysTime;
			TestState = 5;
			break;
		case 5:	
			CurrTestNum++;
			if(CurrTestNum>MAX_TEST_NUM_IO )
			{
				BenchmarkState = BENCHMARK_G2;
				CurrTestNum = 0;
			}
			else 
			{
				TestState = 0;
			}
		default : break;	
	}

	return iTRUE;
	
}

static void Process(iU32 Step , iU32 Totall , iHANDLE Handle)
{
	CurrentImageContainer = Handle;
	if(Step<Totall)
	{
		DecodeComplete = iFALSE;
		i51KitG2DrawImage(CurrentImageContainer, Step-1, iNULL, iNULL);
		i51KitG2Update();
	}
	else
		DecodeComplete = iTRUE;
}


static void Done( iHANDLE Handle)
{
	if(Handle)
	{
		DecodeComplete = iTRUE;
		CurrentImageContainer = Handle;
		i51KitG2SetImageMemoryCopy(CurrentImageContainer,5,ImageId);
	}
	else
		DecodeComplete = iFALSE;
}

static iBOOL TestG2DrawGraph( void )
{
	iU32 VerticeNumber =0;
	iS32 Point[10][2];
	iCOLOR Color;
	static iS32 X = 0, Y = 0;
	static iS32 AddX = 30;
	static iS32 AddY = 60;
//	iRECT DesRect;


	i51KitG2CleanScreen(0XAAAA,iNULL);


	VerticeNumber = 7;
	Point[0][0] = 120;
	Point[0][1] = 70;
	Point[1][0] = 220;
	Point[1][1] = 50;
	Point[2][0] = 220;
	Point[2][1] = 170;
	Point[3][0] = 120;
	Point[3][1] = 150;
	Point[4][0] = 200;
	Point[4][1] = 130;
	Point[5][0] = 120;
	Point[5][1] = 110;
	Point[6][0] = 200;
	Point[6][1] = 90;
	Color =  CurrTestNum*1000;
	i51KitG2FillPolygon( VerticeNumber, Point, Color);

	X += AddX;
	Y += AddY;

	if(X>=200) AddX = -30;
	if(X<20) AddX = 30; 

	if(Y>=290) AddY = -60;
	if(Y<30) AddY = 60; 
	
	i51KitG2FillCircle(X,Y,20,(Color^0XFFFF));

	X += AddX;
	Y += AddY;

	if(X>=200) AddX = -30;
	if(X<20) AddX = 30; 

	if(Y>=290) AddY = -60;
	if(Y<30) AddY = 60; 
	
	i51KitG2FillCircle(X,Y,20,(Color^0XFFFF));

	return iTRUE;
  
}

static iBOOL TestG2DrawImage( void)
{
	iRECT Rect, SrcRect, DesRect;
	static iS32 VP_X1 =0, VP_Y1 = 0;
	static iS32 VP_X2 =0, VP_Y2 = 0;
	static iS32 VP_X3 =70, VP_Y3 = 319;
	static iS16 SpikeX = 5, SpikeY = 5;
	static iS16 DrawImage = iFALSE;
	static iS16 SymbolNumber1= 0;
	static iS16 SymbolNumber2= 0;
	static iS16 BeeDir= 0;
	iU16 * Path = L"Benchmark\\i51IMIC";
	static iFILE  Test ;
//	iS32 iTemp =0;	
	
	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"Benchmark\\i51IMIC") )
		{
		       if(iFALSE==i51KitG2CheckImageContainer(L"Benchmark\\i51IMIC")) return iFALSE;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
			i51KitG2SetImageMemoryCopy(CurrentImageContainer,5,ImageId);
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"Benchmark\\Benchmark.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 12 , Path ,  Process , Done );
		}
	}

	if(DecodeComplete)
	{
		if( VP_BK)
		{
			VP_X1 =0;VP_Y1 = 0;
			VP_X2 =0;VP_Y2 = 0;
			VP_X3 =70;VP_Y3 = 319;
			SpikeX = 5;SpikeY = 5;
			DrawImage = iTRUE;
			SymbolNumber1= 0;
			SymbolNumber2= 0;
			BeeDir= 0;
			DecodeComplete = iFALSE;
			i51KitG2UnsetImageMemoryCopy(CurrentImageContainer,4,iNULL);
			i51KitG2SetImageMemoryCopy(CurrentImageContainer,5,ImageId);
			i51KitG2VPEnable();
			return iFALSE;
		}
		i51KitG2VPEnable();
		Rect.X = 0;
		Rect.Y = 0;
		Rect.Width = 240;
		Rect.Height = 640;
		VP_BK = i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_BK);
		i51KitG2DrawImage(CurrentImageContainer ,BG, iNULL, iNULL);
		DesRect.X = 0;
		DesRect.Y = 320;
		DesRect.Width = 240;
		DesRect.Height = 320;
		i51KitG2DrawImage(CurrentImageContainer ,BG, &DesRect, iNULL);

		Rect.X = 0;
		Rect.Y = 0;
		Rect.Width = 240;
		Rect.Height = 480;
		VP_Road = i51KitG2VPCreate(6, &Rect);
		i51KitG2VPSetActivity(VP_Road);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 24;
		SrcRect.Height = 160;
		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width = 24;
		DesRect.Height = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 320;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		SrcRect.X = 24;
		DesRect.X = 216;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 0;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);

		VP_Snidget = i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_Snidget);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 25;
		SrcRect.Height = 39;
		DesRect.X = 24;
		DesRect.Y = 250;
		DesRect.Width = 25;
		DesRect.Height = 39;
		i51KitG2DrawImage(CurrentImageContainer ,RUN_LEFT, &DesRect, &SrcRect);
		DesRect.X = 191;
		i51KitG2DrawImage(CurrentImageContainer ,RUN_RIGHT, &DesRect, &SrcRect);

		Rect.X = 70;
		Rect.Y = 319;
		Rect.Width = 111;
		Rect.Height = 71;
		VP_Y3 = 319;
		VP_X3 = 70;
		VP_Bee= i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_Bee);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 111;
		SrcRect.Height = 71;
		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width = 25;
		DesRect.Height = 39;
		i51KitG2DrawImage(CurrentImageContainer ,BEE, &DesRect, &SrcRect);

//		i51KitG2VPRender( VP_BK,iNULL);
		FLAG_FIRSTRUN = iFALSE;
		DecodeComplete = iFALSE;
		DrawImage = iTRUE;

		return iFALSE;
		
	}

	if(DrawImage)
	{
		i51KitG2VPEnable();
		DesRect.X = 0;
		DesRect.Y= 0;
		DesRect.Width= 240;
		DesRect.Height= 320;
		i51KitG2VPSetCoord(VP_BK, VP_X1, VP_Y1);
		i51KitG2VPRender( VP_BK,&DesRect);
if(NeedAlpha)	i51KitG2SetAlpha(SysAlpha);
		VP_Y1 += 2;
		if( (VP_Y1>=0) || VP_Y1<-320)
		{
			VP_Y1 = -320;
		}
		i51KitG2VPSetCoord(VP_Road, VP_X2, VP_Y2);
		i51KitG2VPRender( VP_Road,&DesRect);

		VP_Y2 += 10;
		if( (VP_Y2>=0) || VP_Y2<-160)
		{
			VP_Y2 = -160;
		}

		i51KitG2VPClean(VP_Snidget);
		i51KitG2VPSetActivity(VP_Snidget);
		SymbolNumber1 = (SymbolNumber1+1)%4;
		SrcRect.X = 0;
		SrcRect.Y = SymbolNumber1*39;
		SrcRect.Width = 25;
		SrcRect.Height = 39;
		DesRect.X = 24;
		DesRect.Y = 250;
		DesRect.Width = 25;
		DesRect.Height = 39;
		i51KitG2DrawImage(CurrentImageContainer ,RUN_LEFT, &DesRect, &SrcRect);
		DesRect.X = 191;
		i51KitG2DrawImage(CurrentImageContainer ,RUN_RIGHT, &DesRect, &SrcRect);
		i51KitG2VPRender( VP_Snidget,iNULL);

		DesRect.X = VP_X3;
		DesRect.Y = VP_Y3;
		DesRect.Width = 111;
		DesRect.Height = 71;
		i51KitG2VPClean(VP_Bee);
		i51KitG2VPSetActivity(VP_Bee);
		SymbolNumber2 = (SymbolNumber2+1)%2;
		SrcRect.X = 0;
		SrcRect.Y = SymbolNumber2*71;
		SrcRect.Width = 111;
		SrcRect.Height = 71;
		i51KitG2VPSetCoord(VP_Bee, VP_X3, VP_Y3);
		if(BeeDir)
		{
			i51KitG2FlipImage(CurrentImageContainer ,4, iNULL, &SrcRect);
			VP_Y3 += 8;
		}
		else
		{
			i51KitG2DrawImage(CurrentImageContainer ,4, iNULL, &SrcRect);
			VP_Y3 -= 8;
		}
		
		i51KitG2VPRender( VP_Bee,&DesRect);

		if( VP_Y3>=320 )
		{
			VP_Y3 -= 8;
			BeeDir = 0;
		}
		if(VP_Y3<= -71)
		{
			VP_Y3 += 8;
			BeeDir = 1;
		}
		return iTRUE;
	}
	else
	{
		return iFALSE;
	}
}

static iBOOL TestG2RotateImage ( void )
{

#define MAX_FRUIT 3
	iU16 * Path = L"i51KitG2Demo\\i51IMIC";
	static iS32 Angle[MAX_FRUIT] = {0};
	static iS32 SrcMoveX[MAX_FRUIT] = {0};
	static iS32 SrcMoveY[MAX_FRUIT] = {0};
	static iS32 Aadds[MAX_FRUIT] = {0};
	static iS32 CurrImage[MAX_FRUIT] = {0};
	iU32 ImageWidth[13] = {0};	
	iU32 ImageHeight[13] = {0};	

	static iFILE  Test = iNULL;
	iS32 iTemp =0;	
	i51KitG2VPDisable();
	
	if( FLAG_FIRSTRUN)
	{
		for(iTemp=0;iTemp<MAX_FRUIT;iTemp++)
		{
			Angle[iTemp] = 0;
			CurrImage[iTemp] =  iTemp%(APPLE-ORANGE)+ORANGE;		
			SrcMoveX[iTemp] = (iTemp*100) + 10;
			SrcMoveY[iTemp] = (iTemp*140) + 10;
			Aadds[iTemp] = iTemp*15-10;
			if(Aadds[iTemp]<3 ) Aadds[iTemp] =10;
		}
		FLAG_FIRSTRUN = iFALSE;
		if( CurrentImageContainer )
		{
			i51KitG2UnsetImageMemoryCopy(CurrentImageContainer,0,iNULL);
			i51KitG2SetImageMemoryCopy(CurrentImageContainer,6,&ImageId[5]);
			DecodeComplete = iTRUE;
			return iFALSE;
		}
		
		if( i51AdeOsFileIsExist(L"Benchmark\\i51IMIC") )
		{
		       if(iFALSE==i51KitG2CheckImageContainer(L"Benchmark\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"Benchmark\\Benchmark.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 12 , Path ,  Process , Done );
		}
		return iFALSE;
	}

	if(DecodeComplete)
	{
		if( iNULL== CurrentImageContainer) return iFALSE;
		
		for(iTemp=0;iTemp<13;iTemp++)
		{
			i51KitG2GetImageAttr(CurrentImageContainer,iTemp,&ImageWidth[iTemp],&ImageHeight[iTemp],0);
		}

		i51KitG2DrawImage(CurrentImageContainer,FRUIT_BG1,iNULL,iNULL);

if( NeedAlpha) i51KitG2SetAlpha(SysAlpha);
 
		for(iTemp=0;iTemp<MAX_FRUIT;iTemp++)
		{
			i51KitG2RotateImage( CurrentImageContainer, CurrImage[iTemp], iNULL, iNULL,	SrcMoveX[iTemp],SrcMoveY[iTemp],
			SrcMoveX[iTemp]+ImageWidth[CurrImage[iTemp]]/2,SrcMoveY[iTemp]+ImageHeight[CurrImage[iTemp]]/2,0,Angle[iTemp]);
			Angle[iTemp] += Aadds[iTemp];
			if( Angle[iTemp]>=360)
			{
			    Angle[iTemp] %= 360;
			}
		}
		
		return iTRUE;
			
	}
	else 
	{
		return iFALSE;
	}

	return iTRUE;
	
}


static iBOOL TestFontDraw(void)
{
	static iCOLOR Color=0;
	iU16 * Str = (iU16 *)\
"深圳（Shēnzhèn）位于珠江三角洲东岸，隶属广东省，\
副省级城市，与香港陆地相连。（英语：Shenzhen City 汉语\
拼音：Shēnzhèn Shì）。中文别称“鹏城”，英文简称\
Shenzhen，珠三角都市圈重要城市之一，2020年粤港澳都市\
圈世界城市圈重要之一，在国家政策支持下，经过近30\
年深圳从一个南疆边陲小镇发展成为现代国际化城市，\
创造了世界城市化、工业化和现代化的奇迹。深圳是中\
国口岸最多和唯一拥有海陆空口岸的城市，是中国与世\
界交往的主要门户之一，中国最大的对外贸易基地，外\
贸出口连续13年超过上海居全国第一，有着强劲的经济\
支撑与现代化的城市基础设施。深圳的城市综合竞争力\
位列内地城市前列。深圳已经建设成为中国高新技术产\
业重要基地、全国性金融中心、信息中心和华南商贸中\
心、运输中心及旅游胜地，现代化的国际性城市。深圳\
与上海、北京是国内公认的内地三大金融中心。 ";
	iRECT DrawRect;
	
	DrawRect.X = 0;
	DrawRect.Y = 0;
	DrawRect.Width = 240;
	DrawRect.Height= 320;
	i51KitG2VPDisable();
	i51KitG2CleanScreen(0X0,iNULL);
	Color = CurrTestNum*3000;
	i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect,iNULL, i51KITG2_FONT_WORDS_ROTATE_0,0,0, Color, 0XFF00, 0);

	return iTRUE;
}

static iBOOL DrawG2( void )
{
	iU16 * Str1 = (iU16*)"IO模块测试完成";
	iU16 * Str2 = (iU16*)"CPU模块测试完成";
	iU16 * Str3 = (iU16*)"开始测试G2模块 ......";
	static iU32 Num = 0;
	iRECT StrRect;

	i51KitG2CleanScreen(0X0,iNULL);
	StrRect.X = 15;
	StrRect.Y = 120;
	StrRect.Width = 180;
	StrRect.Height = 20;
	Num ++;
	if(Num>35) 
	{
		Num = 0;
		return iTRUE;
	}
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str1,0,&StrRect,iNULL,0,0,0,0X07E0,0,0);
	StrRect.Y = 40;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str2,0,&StrRect,iNULL,0,0,0,0X07E0,0,0);
	StrRect.Y = 200;
	i51KitG2FontDraw(TypeheadHandle,2,(void*)Str3,9+(Num/5),&StrRect,iNULL,0,0,0,0X07E0,0,0);

	return iFALSE;
	
}


static iBOOL TestG2( void )
{
	static iU32 TestState = 0;
//	int i = 0, j = 0, k = 0;
//	float Temp = 0.0;
	
	OldSysTime = CurrSysTime;

	NeedAlpha = iFALSE;
	i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

	switch( TestState)
	{
		case 0:
			if(DrawG2())
			{
				TestState = 1;
			}
			break;
		case 1:
			OldSysTime = i51AdeOsGetTick();
			TestG2DrawGraph();
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_GRAPHA] += CurrSysTime - OldSysTime;
			CurrTestNum++;
			if(CurrTestNum>MAX_TEST_NUM_G2)
			{
				CurrTestNum = 0;
				i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
				SysAlpha = i51KITG2_ALPHA_NONEED;
				MulAlpha = -5;
				TestState = 2;
			}
			break;
		case 2:	
			OldSysTime = i51AdeOsGetTick();
			CurrSysTime = OldSysTime;
			i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
			if(TestG2DrawImage())
			{
				CurrSysTime = i51AdeOsGetTick();
				AllTestTime[BENCHMARK_IMAGE] += CurrSysTime - OldSysTime;
				CurrTestNum++;
				if(CurrTestNum>MAX_TEST_NUM_G2)
				{
					CurrTestNum = 0;
					i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
					SysAlpha = i51KITG2_ALPHA_NONEED;
					MulAlpha = -5;
					TestState = 3;
				}
			}
			break;
		case 3:
			OldSysTime = i51AdeOsGetTick();
			CurrSysTime = OldSysTime;
			SysAlpha += MulAlpha;
			if(SysAlpha>255){ MulAlpha = -5; SysAlpha = 250;}
			if(SysAlpha<0){ MulAlpha = 5; SysAlpha = 5;}
			NeedAlpha = iTRUE;
			if(TestG2DrawImage())
			{
				CurrSysTime = i51AdeOsGetTick();
				AllTestTime[BENCHMARK_IMAGE] += CurrSysTime - OldSysTime;
				CurrTestNum++;
				if(CurrTestNum>MAX_TEST_NUM_G2)
				{
					CurrTestNum = 0;
					DecodeComplete = iTRUE;
					FLAG_FIRSTRUN = iTRUE;
					i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
					SysAlpha = i51KITG2_ALPHA_NONEED;
					MulAlpha = -5;
					TestState = 4;					
				}
			}
			break; 	
		case 4:	
			OldSysTime = i51AdeOsGetTick();
			CurrSysTime = OldSysTime;
			i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
			if(TestG2RotateImage())
			{
				CurrSysTime = i51AdeOsGetTick();
				AllTestTime[BENCHMARK_IMAGE] += CurrSysTime - OldSysTime;
				CurrTestNum++;
				if(CurrTestNum>MAX_TEST_NUM_G2)
				{
					CurrTestNum= 0;
					i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
					SysAlpha = i51KITG2_ALPHA_NONEED;
					MulAlpha = -5;
					TestState = 5;					
				}
			}
			break;
		case 5:
			OldSysTime = i51AdeOsGetTick();
			CurrSysTime = OldSysTime;
			SysAlpha += MulAlpha;
			if(SysAlpha>255){ MulAlpha = -5; SysAlpha = 250;}
			if(SysAlpha<0){ MulAlpha = 5; SysAlpha = 5;}
			NeedAlpha = iTRUE;
			if(TestG2RotateImage())
			{
				CurrSysTime = i51AdeOsGetTick();
				AllTestTime[BENCHMARK_IMAGE] += CurrSysTime - OldSysTime;
				CurrTestNum++;
				if(CurrTestNum>MAX_TEST_NUM_G2)
				{
					CurrTestNum= 0;
					i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
					SysAlpha = 0;
					MulAlpha = 5;
					TestState = 6;					
				}
			}
			break;
		case 6:
			OldSysTime = i51AdeOsGetTick();
			TestFontDraw();
			CurrSysTime = i51AdeOsGetTick();
			AllTestTime[BENCHMARK_FONT] += CurrSysTime - OldSysTime;
			CurrTestNum++;
			if(CurrTestNum>MAX_TEST_NUM_FONT)
			{
				CurrTestNum= 0;
				i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
				SysAlpha = 0;
				MulAlpha = 5;
				TestState = 7;					
			}
			break;
		case 7:
			OldSysTime = CurrSysTime;
			BenchmarkState = BENCHMARK_RESULT;
			DecodeComplete = iTRUE;
			FLAG_FIRSTRUN = iFALSE;
			i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
			TestState = 0;
			break;
		default : break;	
	}

	return iTRUE;
		
}

static iBOOL DrawResult( void )
{
	iS32 PointTime[10][2] = {0};
	iS32 iTemp = 0;

	PointTime[0][0] = 94;
	PointTime[0][1] = 78;
	for( iTemp=1; iTemp<10; iTemp++ )
	{
		PointTime[iTemp][0] = 94;
		PointTime[iTemp][1] = PointTime[iTemp-1][1] + 24;
		if(iTemp%2) PointTime[iTemp][1]--;
	}

	i51KitG2DrawImage(CurrentImageContainer, RESULT,iNULL,iNULL);

	if(ResultFlag)
	{
		ResultFlag = iFALSE;
		AllTestTime[BENCHMARK_INT] = (AllTestTime[BENCHMARK_INT]+MAX_TEST_NUM_CPU/2)/MAX_TEST_NUM_CPU;
		AllTestTime[BENCHMARK_FLOAT] = (AllTestTime[BENCHMARK_FLOAT]+MAX_TEST_NUM_CPU/2)/MAX_TEST_NUM_CPU;

		AllTestTime[BENCHMARK_RAM] = (AllTestTime[BENCHMARK_RAM]+MAX_TEST_NUM_IO/2)/MAX_TEST_NUM_IO;
		AllTestTime[BENCHMARK_FILE] = (AllTestTime[BENCHMARK_FILE]+MAX_TEST_NUM_IO/2)/MAX_TEST_NUM_IO;
		AllTestTime[BENCHMARK_SEEK] = (AllTestTime[BENCHMARK_SEEK]+MAX_TEST_NUM_IO/2)/MAX_TEST_NUM_IO;
		AllTestTime[BENCHMARK_READ] = (AllTestTime[BENCHMARK_READ]+MAX_TEST_NUM_IO/2)/MAX_TEST_NUM_IO;
		AllTestTime[BENCHMARK_WRITE] = (AllTestTime[BENCHMARK_WRITE]+MAX_TEST_NUM_IO/2)/MAX_TEST_NUM_IO;

		AllTestTime[BENCHMARK_GRAPHA] = (AllTestTime[BENCHMARK_GRAPHA]+MAX_TEST_NUM_G2/2)/MAX_TEST_NUM_G2;
		AllTestTime[BENCHMARK_IMAGE] = (AllTestTime[BENCHMARK_IMAGE]+MAX_TEST_NUM_G2/2)/MAX_TEST_NUM_G2;
		AllTestTime[BENCHMARK_FONT] = (AllTestTime[BENCHMARK_FONT]+MAX_TEST_NUM_FONT/2)/MAX_TEST_NUM_FONT;
	}
	for( iTemp=0; iTemp<10; iTemp++)
	{
		if(AllTestTime[iTemp]==0) AllTestTime[iTemp] = 1;
		if(AllTestTime[iTemp]>=ARM7Time[iTemp])
		{
			ShowNumber(AllTestTime[iTemp], PointTime[iTemp][0], PointTime[iTemp][1], 0XF800 );
		}
		else if(AllTestTime[iTemp]<=ARM9Time[iTemp])
		{
			ShowNumber(AllTestTime[iTemp], PointTime[iTemp][0], PointTime[iTemp][1], 0X0FF0 );
		}
		else
		{
			ShowNumber(AllTestTime[iTemp], PointTime[iTemp][0], PointTime[iTemp][1], 0XFF00 );
		}
		
		ShowNumber(ARM7Time[iTemp], PointTime[iTemp][0]+47, PointTime[iTemp][1], 0XF800 );
		ShowNumber(ARM9Time[iTemp], PointTime[iTemp][0]+94, PointTime[iTemp][1], 0X0FF0 );
	}

 	return iTRUE;
	
}

static iBOOL BenchmarkDraw( void )
{
	iU16 WidthScale = 100;
	iU16 HeightScale = 100;
	
	switch( BenchmarkState )
	{
		case BENCHMARK_IDLE:
			TestIDLE();
			break;
		case BENCHMARK_CPU:
			TestCPU();
			break;
		case BENCHMARK_IO:
			TestIO();
			break;
		case BENCHMARK_G2:
			TestG2();
			break;
		case BENCHMARK_RESULT:
			DrawResult();
			break;
		default :
			break;
	}

		if(ScreenWidth>0) WidthScale = ScreenWidth*100/240;
		if(ScreenHeight>0) HeightScale = ScreenHeight*100/320;
		i51KitG2ScreenZoom(iNULL,iNULL,WidthScale,HeightScale);
		i51KitG2Update();
		return iTRUE;
}

static iBOOL BenchmarkMain( void )
{
	BenchmarkDraw();
	return iTRUE;
}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{
		case i51_MSG_PAINT :
			BenchmarkMain();
			break ;
	
		case i51_MSG_RUN :
			i51AdeOsScreenAlwaysLight(1);
//i51KernelSetCache(L"i51KitG2");
			TypeheadHandle = i51KitStdResOpen(L"Benchmark\\Typehead.re");
			i51KitG2FontCachEnable(TypeheadHandle,0);
			OldSysTime = i51AdeOsGetTick();
			i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight );

			break ;
		
		case i51_MSG_KEYUP:
			CurrSysTime = i51AdeOsGetTick();
			if( (CurrSysTime-OldSysTime>500) && (BenchmarkState == BENCHMARK_IDLE) )
			{
				BenchmarkState = BENCHMARK_CPU;
			}
			break;
			
		case i51_MSG_PADDOWN:
			CurrSysTime = i51AdeOsGetTick();
			if( (CurrSysTime-OldSysTime>300)  )
			{
				if( BenchmarkState >= BENCHMARK_RESULT )
				{
					OldSysTime = CurrSysTime;
					BenchmarkState = BENCHMARK_IDLE;
				}
				else if( BenchmarkState == BENCHMARK_IDLE)
				{
					OldSysTime = CurrSysTime;
					BenchmarkState = BENCHMARK_CPU;
				}
			}
			break;
		
		case i51_MSG_INIT :
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

	             //	加载i51KitStd动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ) 
	             {
					 i51AdeOsLog ( 0 , "Load Std Failed" ) ;
					 return 0 ;
	             }
			 
	             //	加载i51KitStd动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ) 
	             {
					 i51AdeOsLog ( 0 , "Load G2 Failed" ) ;
	                 return 0 ;
	             }
			break ;
		
		case i51_MSG_PAUSE :
			break ;

		case i51_MSG_EXIT :
			i51AdeOsScreenAlwaysLight(0);
			i51KitG2FontCachDisable(TypeheadHandle,0);
			i51KitStdResClose(TypeheadHandle);
			TypeheadHandle = iNULL;

			i51KitG2VPDestroy(VP_Bee);
			i51KitG2VPDestroy(VP_BK);
			i51KitG2VPDestroy(VP_Road);
			i51KitG2VPDestroy(VP_Snidget);
			VP_Bee = iNULL;
			VP_BK = iNULL;
			VP_Road = iNULL;
			VP_Snidget = iNULL;
			
			i51KitG2CloseImageContainer(CurrentImageContainer);
			CurrentImageContainer = iNULL;

			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitStd);
			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitG2);
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

}

 
