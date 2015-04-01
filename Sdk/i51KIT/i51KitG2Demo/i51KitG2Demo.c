
#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitG2Demo.h"
#include "i51KitG2DemoRes.h"
#include "i51KitStd.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

iU8 FirstRun = iTRUE;
iU8 DecodeComplete = iFALSE;
iU8 FlagTestVP = iFALSE;
static iU8 FLAG_FIRSTRUN  = iTRUE;
static iS32 ScreenWidth = 240;
static iS32 ScreenHeight = 320;

iHANDLE CurrentImageContainer = iFALSE;

iU8    UseMemory = iTRUE;
iU32 ZoomRateX = 100;	
iU32 ZoomRateY = 100;	
iU32 Type = 0;

static iHANDLE VP_BK = iNULL;
static iHANDLE VP_Road = iNULL;
static iHANDLE VP_Snidget  = iNULL;
static iHANDLE VP_Bee = iNULL;

void Process(iU32 Step , iU32 Totall , iHANDLE Handle)
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


void Done( iHANDLE Handle)
{
	if( i51AdeOsFileIsExist(L"i51KitG2Demo\\i51IMIC") )
	{
	    i51AdeOsLog(0, "Mic file is exist!");
	}
	if(Handle)
	{
		DecodeComplete = iTRUE;
		CurrentImageContainer = Handle;
	}
	else
		DecodeComplete = iFALSE;
}

void TestVP( void)
{
	iRECT Rect, SrcRect, DesRect;
	static iS32 VP_X1 =0, VP_Y1 = 0;
	static iS32 VP_X2 =0, VP_Y2 = 0;
	static iS32 VP_X3 =0, VP_Y3 = 0;
	static iS16 SpikeX = 5, SpikeY = 5;
	static iS16 DrawImage = iFALSE;
	static iS16 SymbolNumber1= 0;
	static iS16 SymbolNumber2= 0;
	static iS16 BeeDir= 0;
	iU16 * Path = L"i51KitG2Demo\\i51IMIC";
	static iFILE  Test ;
    iU32 ImageId[14] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE,LEAF,
		ORANGE,PINEAPPLE,STRAWBERRY,WATERMELON,APPLE,	BOMB,FRUIT_BG1,FRUIT_BG2
};


	iS32 iTemp =0;	
	
	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"i51KitG2Demo\\i51IMIC") )
		{
		       if(iFALSE==i51KitG2CheckImageContainer(L"i51KitG2Demo\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
if(ScreenWidth>240) ImageId[12] = ImageId[13];
i51KitG2SetImageMemoryCopy(CurrentImageContainer,13,ImageId);
			
//i51KitG2SetRotateCenter(120,160);
//i51KitG2SetRotateCenter(120,-300);
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"i51KitG2Demo\\i51KitG2.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 14 , Path ,  Process , Done );
		}
		}
//    memset ( LcdBuffer , 0XFF , 240*320*2 ) ;

	if(DecodeComplete)
	{
//		i51KitG2SetAlpha(0);
		i51KitG2VPEnable();
		Rect.X = 0;
		Rect.Y = 0;
		Rect.Width = 240;
		Rect.Height = 640;
		VP_BK = i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_BK);
//		i51KitG2SetVPiSLoop(VP_BK, iTRUE);
		i51KitG2DrawImage(CurrentImageContainer ,BG, iNULL, iNULL);
		DesRect.X = 0;
		DesRect.Y = 320;
		DesRect.Width = 240;
		DesRect.Height = 320;
		i51KitG2DrawImage(CurrentImageContainer ,BG, &DesRect, iNULL);

//		i51KitG2SetAlpha(150);
		Rect.X = 0;
		Rect.Y = 0;
		Rect.Width = 240;
		Rect.Height = 480;
		VP_Road = i51KitG2VPCreate(6, &Rect);
		i51KitG2VPSetActivity(VP_Road);
//		i51KitG2SetVPiSLoop(VP_Road, iTRUE);
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

		i51KitG2VPRender( VP_BK,iNULL);
		FLAG_FIRSTRUN = iFALSE;
		DecodeComplete = iFALSE;
		DrawImage = iTRUE;
	}
//i51KitG2SetAlpha(150);

if(DrawImage)
{
	DesRect.X = 0;
	DesRect.Y= 0;
	DesRect.Width= 240;
	DesRect.Height= 320;
#if 1	
	i51KitG2VPSetCoord(VP_BK, VP_X1, VP_Y1);
	i51KitG2VPRender( VP_BK,&DesRect);
	VP_Y1 += 2;
	if( (VP_Y1>=0) || VP_Y1<-320)
	{
//		SpikeY= -SpikeY;
		VP_Y1 = -320;
	}
#endif
#if 1
	i51KitG2VPSetCoord(VP_Road, VP_X2, VP_Y2);
	i51KitG2VPRender( VP_Road,&DesRect);

	VP_Y2 += 10;
	if( (VP_Y2>=0) || VP_Y2<-160)
	{
//		SpikeY= -SpikeY;
		VP_Y2 = -160;
	}
	i51KitG2VPRender( VP_Snidget,&DesRect);
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

	DesRect.X = VP_X3;
	DesRect.Y = VP_Y3;
	DesRect.Width = 111;
	DesRect.Height = 71;
	i51KitG2VPRender( VP_Bee,&DesRect);
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
#endif
	i51KitG2ScreenZoom(iNULL,iNULL,ZoomRateX, ZoomRateY);
	i51KitG2Update();
}
}


void TestRotateImage ( void )
{

	//    iU16 * Path = L"E:\\i51\\i51APP\\i51KitG2Demo\\";
    #define MAX_FRUIT 10
    iU16 * Path = L"i51KitG2Demo\\i51IMIC";
//    iRECT SrcRect, SrcRect1, DesRect1;
    static iS32 Angle[MAX_FRUIT] = {0};
    static iS32 SrcMoveX[MAX_FRUIT] = {0,80,160,0,80,160,0,80,160,90};
    static iS32 SrcMoveY[MAX_FRUIT] = {10,10,10,90,90,90,170,170,170,250};
    static iS32 Xadds[MAX_FRUIT] = {0};
    static iS32 Yadds[MAX_FRUIT] = {0};
    static iS32 Aadds[MAX_FRUIT] = {15,-18,15,-15,10,-15,15,-10,18,-15};
    static iS32 MaxX[MAX_FRUIT] = {0};
    static iS32 CurrImage[MAX_FRUIT] = {0};
    iU32 ImageId[14] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE,LEAF,
		ORANGE,PINEAPPLE,STRAWBERRY,WATERMELON,APPLE,	BOMB,FRUIT_BG1,FRUIT_BG2};
    iU32 ImageWidth[14] = {0};	
    iU32 ImageHeight[14] = {0};	

    static iFILE  Test ;
	iS32 iTemp =0;	

	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"i51KitG2Demo\\i51IMIC") )
		{
		       if(iFALSE==i51KitG2CheckImageContainer(L"i51KitG2Demo\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
if(ScreenWidth>240) ImageId[12] = ImageId[13];
i51KitG2SetImageMemoryCopy(CurrentImageContainer,13,ImageId);
			
//i51KitG2SetRotateCenter(120,160);
//i51KitG2SetRotateCenter(120,-300);
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"i51KitG2Demo\\i51KitG2.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 14 , Path ,  Process , Done );
		}
for(iTemp=0;iTemp<MAX_FRUIT;iTemp++)
{
    CurrImage[iTemp] =  rand()%(APPLE-ORANGE)+ORANGE;		
    SrcMoveX[iTemp] = rand()%100-100;
    SrcMoveY[iTemp] = rand()%ScreenHeight;
    Xadds[iTemp] = rand()%5+2;
    Aadds[iTemp] = Xadds[iTemp]*4;
    Yadds[iTemp] = rand()%7-3;
    MaxX[iTemp] = rand()%100+(ScreenWidth-100);
}
	}

if(DecodeComplete)
{
    if( iNULL== CurrentImageContainer) return;
for(iTemp=0;iTemp<14;iTemp++)
{
i51KitG2GetImageAttr(CurrentImageContainer,iTemp,&ImageWidth[iTemp],&ImageHeight[iTemp],0);
}
//    ImageContainer = (i51ImageContainer *)CurrentImageContainer;
//    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
//i51KitG2RotateImage( CurrentImageContainer, WALL, iNULL, iNULL, 96, 80 , 120,160,i51KITG2_ROTATE_POINT, Angle[0]);
//Angle[0]++;
//if(Angle[0]>=360) Angle[0] = 0;
//i51AdeMmiUpdateScreen  () ;
//return;	

//DecodeComplete = iFALSE;
if(ScreenWidth==240) i51KitG2DrawImage(CurrentImageContainer,FRUIT_BG1,iNULL,iNULL);
else i51KitG2DrawImage(CurrentImageContainer,FRUIT_BG2,iNULL,iNULL);
//	memset ( LcdBuffer , 0XFFFF , ScreenWidth*ScreenHeight*2 ) ;
//   i51KitG2RotateImage( CurrentImageContainer, 4, 65, 89 , iNULL, iNULL, i51KITG2_ROTATE_XLINE, Angle);
//   i51KitG2RotateImage( CurrentImageContainer, 4, 65, 89 , iNULL, iNULL, i51KITG2_ROTATE_YLINE, Angle);
//   i51KitG2RotateImage( CurrentImageContainer, BEE, 65, 89 , iNULL, iNULL, i51KITG2_ROTATE_POINT, Angle);
//   i51KitG2RotateImage( CurrentImageContainer, LEAF, 65, 89 , iNULL, iNULL, i51KITG2_ROTATE_POINT, Angle);
//   i51KitG2RotateImage( CurrentImageContainer, BG, 0, 0 , iNULL, iNULL, i51KITG2_ROTATE_POINT, Angle);
//   i51KitG2RotateImage( CurrentImageContainer, WALL, 96, 80 , iNULL, iNULL, i51KITG2_ROTATE_POINT, Angle);
//   i51KitG2RotateImage( CurrentImageContainer, 4, 65, 89 , iNULL, iNULL, i51KITG2_ROTATE_POINT, 45);
// i51KitG2RotateImage( CurrentImageContainer, 4, iNULL, iNULL,  0);
// i51KitG2RotateImage( CurrentImageContainer, 4, iNULL, iNULL,  45);
// i51KitG2RotateImage( CurrentImageContainer, 4, iNULL, iNULL,  90);
// i51KitG2RotateImage( CurrentImageContainer, 4, iNULL, iNULL,  135);
// i51KitG2RotateImage( CurrentImageContainer, 4, iNULL, iNULL,  180);
// i51KitG2RotateImage( CurrentImageContainer, 4, iNULL, iNULL,  225);
// i51KitG2RotateImage( CurrentImageContainer, 4, iNULL, iNULL,  270);
for(iTemp=0;iTemp<MAX_FRUIT;iTemp++)
{
i51KitG2RotateImage( CurrentImageContainer, CurrImage[iTemp], iNULL, iNULL,	SrcMoveX[iTemp],SrcMoveY[iTemp],
	SrcMoveX[iTemp]+ImageWidth[CurrImage[iTemp]]/2,SrcMoveY[iTemp]+ImageHeight[CurrImage[iTemp]]/2,0,Angle[iTemp]);
SrcMoveX[iTemp] += Xadds[iTemp];
SrcMoveY[iTemp] += Yadds[iTemp];
if(SrcMoveX[iTemp]>=MaxX[iTemp])
{
    Xadds[iTemp] = -Xadds[iTemp];
} else if(SrcMoveX[iTemp]<-100)
{
    CurrImage[iTemp] =  rand()%(APPLE-ORANGE)+ORANGE;		
    SrcMoveX[iTemp] = rand()%100-100;
    SrcMoveY[iTemp] = rand()%ScreenHeight;
    Xadds[iTemp] = rand()%5+2;
    Aadds[iTemp] = Xadds[iTemp]*4;
    Yadds[iTemp] = rand()%7-3;
    MaxX[iTemp] = rand()%100+(ScreenWidth-100);
    Angle[iTemp] = 0;
}
Angle[iTemp] += Aadds[iTemp];
if( Angle[iTemp]>=360)
{
    Angle[iTemp] = 0;
}
}
}
    i51AdeMmiUpdateScreen  () ;

}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{


	//	TODO :	消息循环
	switch ( message )
	{
		case i51_MSG_PAINT :
			TestVP();
//			TestRotateImage();
			break;
		case i51_MSG_KEYUP :
			//ZoomRate = ((iU32)parameter)*25;
			if ( UseMemory)
			{
				UseMemory = iFALSE;
				i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
			}
			else
			{
				UseMemory = iTRUE;
				i51KitG2UnsetImageMemoryCopy(CurrentImageContainer,0,iNULL);
			}
			i51AdeOsLog(0, "i51_MSG_KEYUP");
			break ;
		case i51_MSG_PADDOWN:
			switch(Type)
			{
				case 0 :
					ZoomRateX = 100;
					ZoomRateY = 100;
					Type = 1;
					break;
				case 1:
					ZoomRateX = 133;
					ZoomRateY = 150;
					Type = 2;
					break;
				case 2:	
					ZoomRateX = 92;
					ZoomRateY = 55;
					Type = 0;
					break;
				default :	
					ZoomRateX = 100;
					ZoomRateY = 100;
					Type = 1;
					break;
			}

			break;			
		case i51_MSG_INIT :
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

i51AdeOsLog(0,"Adapter");

	             //	加载i51KitStd动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
i51AdeOsLog(0,"PKiGAPI\\i51KitStd.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ) 
	             {
	                 return 0 ;
	             }
i51AdeOsLog(0,"i51KitStd");
			 
	             //	加载i51KitStd动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");
i51AdeOsLog(0,"PKiGAPI\\i51KitG2.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ) 
	             {
	                 return 0 ;
	             }
//i51KernelSetCache(L"i51KitG2");

i51AdeOsLog(0,"i51KitG2");
    i51KitG2CleanScreen(0,iNULL);
	i51KitG2Update();
i51AdeOsLog(0, "Init OK");
			break ;
		case i51_MSG_PAUSE :
			break ;
		case i51_MSG_EXIT :

			i51KitG2CloseImageContainer(CurrentImageContainer);
			CurrentImageContainer = iNULL;
			
			i51KitG2VPDestroy(VP_Bee);
			i51KitG2VPDestroy(VP_BK);
			i51KitG2VPDestroy(VP_Road);
			i51KitG2VPDestroy(VP_Snidget);
			VP_Bee = iNULL;
			VP_BK = iNULL;
			VP_Road = iNULL;
			VP_Snidget = iNULL;

			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitStd);
			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitG2);
			break ;
	}

	return iTRUE ;

}
