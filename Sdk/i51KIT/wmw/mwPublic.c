#include "i51Public.h"
#include "mwLevelPublic.h"


#define MAX_CIRCLE_R	8
#define MAX_FILL_NUM	30

#define SPEED_WATER_MAX	350

iU8 COLOR_ALPHA = 150;
//static iCOLOR GraphFillColor[MAX_FILL_NUM] = {0};
// 游戏第一级状态机标记
mwGameState CurrGameState = GAME_STATE_IDLE;
// 鸭子是否减少的标记
iU8 DuckResidueFlag = iFALSE;
// 屏幕临时Buff需要更新的标记
iU8 TempScreenUpdataFlag = iFALSE;
// 每个网格中水滴数量
//iS8 GriddingWaterNum[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength]= {0};

// 当前游戏中水滴总数
iU32 CurrTotalWaterNum = 0;
// 释放允许龙头喷水的标记
iU32 WaterSprayFlag = iTRUE;

// 选择的音乐是否在播放的标记
iU8 SoundChoosePlay = iFALSE;
// 胜利的音乐是否在播放的标记
iU8 SoundWinPlay = iFALSE;

iU8 WaterDrawAll = iFALSE;

static iCOLOR i51KitG2GetNewColorEx (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
/**
 * @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
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
iBOOL mwFillCircle( iS32 X, iS32 Y, iU32 R, iCOLOR Color)
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
	
	int i = 0;

	iS32 WorldAttributeY = 0;
	iS32 WorldAttributeX = 0;
	iS32 RemainderY = 0;
	iS32 RemainderX = 0;
	iU8 WorldAttributeFlag = 0;

    if( X>(SCREEN_WIDTH-R)||(X<R) ||(Y>SCREEN_HEIGHT-R)||Y<(R)||R>15 )
    {
        return iFALSE;
    }

//	if(GraphFillColor[0]!=Color)
//	{
//		for(int i = 0; i<MAX_FILL_NUM; i++ )
//		{
//			GraphFillColor[i] = Color;
//		}
//	}

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
	if( CurrLineY>=0 && CurrLineY<SCREEN_HEIGHT&& CurrLineEndX>=0 && CurrLineStartX<SCREEN_WIDTH)
	{
		WorldAttributeY = CurrLineY/2;
		RemainderY = CurrLineY%2;
		
		if( CurrLineEndX >= SCREEN_WIDTH ) CurrLineEndX = SCREEN_WIDTH -1;
		if( CurrLineStartX < 0 ) CurrLineStartX = 0;
		CurrLineLength = CurrLineEndX - CurrLineStartX;
		ScreenPos = CurrLineY*SCREEN_WIDTH + CurrLineStartX;

		RemainderX = CurrLineStartX%2;
		for(i=0;i<CurrLineLength;i++)
		{
			WorldAttributeX = (CurrLineStartX+i)/2;

			if( RemainderX) WorldAttributeFlag = 0X20;
			else WorldAttributeFlag = 0X10;

			if( RemainderY ) WorldAttributeFlag = WorldAttributeFlag<<2;
				
			if( (((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&0XF)==0 || WaterDrawAll ))
			&& ((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&WorldAttributeFlag)==0) )
			{
				mwScreenBuffer[ScreenPos] = i51KitG2GetNewColorEx(mwScreenBuffer[ScreenPos], COLOR_WATER, COLOR_ALPHA);
				mwWorldAttribute[WorldAttributeY][WorldAttributeX] |= WorldAttributeFlag;
	//			mwScreenBuffer[ScreenPos] = COLOR_WATER;
			}

			if( RemainderX ) RemainderX = 0;
			else RemainderX = 1;
			ScreenPos++;
		}
//		i51AdeStdMemcpy16(&mwScreenBuffer[ScreenPos], COLOR_WATER, (CurrLineLength<<1));
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

        //绘制一三象限的图像
		CurrLineY = CurrentY;
		CurrLineStartX = CurrentX;
		CurrLineEndX = Center2X - CurrentX;
		if( CurrLineY>=0 && CurrLineY<SCREEN_HEIGHT && CurrLineEndX>=0 && CurrLineStartX<SCREEN_WIDTH )
		{
			WorldAttributeY = CurrLineY/2;
			RemainderY = CurrLineY%2;

			if( CurrLineEndX >= SCREEN_WIDTH ) CurrLineEndX = SCREEN_WIDTH -1;
			if( CurrLineStartX < 0 ) CurrLineStartX = 0;
			CurrLineLength = CurrLineEndX - CurrLineStartX;
			ScreenPos = CurrLineY*SCREEN_WIDTH + CurrLineStartX;

			RemainderX = CurrLineStartX%2;
			for( i=0;i<CurrLineLength;i++)
			{
				WorldAttributeX = (CurrLineStartX+i)/2;

				if( RemainderX) WorldAttributeFlag = 0X20;
				else WorldAttributeFlag = 0X10;

				if( RemainderY ) WorldAttributeFlag = WorldAttributeFlag<<2;
					
				if( (((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&0XF)==0 || WaterDrawAll ))
				&& ((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&WorldAttributeFlag)==0) )
				{
					mwScreenBuffer[ScreenPos] = i51KitG2GetNewColorEx(mwScreenBuffer[ScreenPos], COLOR_WATER, COLOR_ALPHA);
					mwWorldAttribute[WorldAttributeY][WorldAttributeX] |= WorldAttributeFlag;
		//			mwScreenBuffer[ScreenPos] = COLOR_WATER;
				}

				if( RemainderX ) RemainderX = 0;
				else RemainderX = 1;
				ScreenPos++;
//				mwScreenBuffer[ScreenPos] = i51KitG2GetNewColorEx(mwScreenBuffer[ScreenPos], COLOR_WATER, COLOR_ALPHA);
//				mwScreenBuffer[ScreenPos] = COLOR_WATER;
//				ScreenPos++;
			}

//			i51AdeStdMemcpy16(&mwScreenBuffer[ScreenPos], COLOR_WATER, (CurrLineLength<<1));
		}
//iLog("%d %d %d",CurrLineStartX,CurrLineY,CurrLineLength);

        //绘制二四象限的图像
		CurrLineY = Center2Y - CurrentY;
		CurrLineStartX = CurrentX;
		CurrLineEndX = Center2X - CurrentX;
		if( CurrLineY>=0 && CurrLineY<SCREEN_HEIGHT && CurrLineEndX>=0 && CurrLineStartX<SCREEN_WIDTH )
		{
			WorldAttributeY = CurrLineY/2;
			RemainderY = CurrLineY%2;

			if( CurrLineEndX >= SCREEN_WIDTH ) CurrLineEndX = SCREEN_WIDTH -1;
			if( CurrLineStartX < 0 ) CurrLineStartX = 0;
			CurrLineLength = CurrLineEndX - CurrLineStartX;
			ScreenPos = CurrLineY*SCREEN_WIDTH + CurrLineStartX;

			RemainderX = CurrLineStartX%2;
			for( i=0;i<CurrLineLength;i++)
			{
				WorldAttributeX = (CurrLineStartX+i)/2;

				if( RemainderX) WorldAttributeFlag = 0X20;
				else WorldAttributeFlag = 0X10;

				if( RemainderY ) WorldAttributeFlag = WorldAttributeFlag<<2;
					
				if( (((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&0XF)==0 || WaterDrawAll ))
				&& ((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&WorldAttributeFlag)==0) )
				{
					mwScreenBuffer[ScreenPos] = i51KitG2GetNewColorEx(mwScreenBuffer[ScreenPos], COLOR_WATER, COLOR_ALPHA);
					mwWorldAttribute[WorldAttributeY][WorldAttributeX] |= WorldAttributeFlag;
		//			mwScreenBuffer[ScreenPos] = COLOR_WATER;
				}

				if( RemainderX ) RemainderX = 0;
				else RemainderX = 1;
				ScreenPos++;
//				mwScreenBuffer[ScreenPos] = i51KitG2GetNewColorEx(mwScreenBuffer[ScreenPos], COLOR_WATER, COLOR_ALPHA);
//				mwScreenBuffer[ScreenPos] = COLOR_WATER;
//				ScreenPos++;
			}
		}
//iLog("%d %d %d",CurrLineStartX,CurrLineY,CurrLineLength);


    }

    #undef ABSOLUTE

    return iTRUE;

}

iBOOL DrawWaterDrop(iBody *body, iShapeCircle  *circle, iCOLOR Color)
{
	iS32 X = (iS32)(circle->CentrePoint.X);
	iS32 Y = (iS32)(circle->CentrePoint.Y);

	iS32 FactX = (iS32)(circle->CentrePoint.X)/COORDINATE_RATE;
	iS32 FactY = (iS32)(circle->CentrePoint.Y)/COORDINATE_RATE;

iShapeCircle * ShapeCircle = (iShapeCircle*)(body->Shape);

	int i =0;
	
	iS32 OutX = 0;
	iS32 OutY = 0;
	iS32 StartX = 0;
	iS32 StartY = 0;
	iS32 EndX = 0;
	iS32 EndY = 0;
	iS32 CurrX = 0;
	iS32 CurrY = 0;
	iS32 SpeedX = 0;
	iS32 SpeedY = 0;
	iS32 AddY = 0;
	iS32 AddX = 0;
	iS32 XMul[7][7] = {
			{-3,-2,-1,0,1,2,3},
			{-3,-2,-1,0,1,2,3},
			{-3,-2,-1,0,1,2,3},
			{-3,-2,-1,0,1,2,3},
			{-3,-2,-1,0,1,2,3},
			{-3,-2,-1,0,1,2,3},
			{-3,-2,-1,0,1,2,3}
		};
	iS32 YMul[7][7] = {
			{-3,-3,-3,-3,-3,-3,-3},
			{-2,-2,-2,-2,-2,-2,-2},
			{-1,-1,-1,-1,-1,-1,-1},
			{0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1},
			{2,2,2,2,2,2,2},
			{3,3,3,3,3,3,3}
		};
	iU8 Count = 0;
	int j;
	iRECTEX * SrcRect = iNULL;
	iS32 VX = 0;
	iS32 VY = 0;
	iU32 R = 0;

	iS32 V = 0;
	iS32 CentreX= 0;
	iS32 CentreY = 0;
	iS32 CurrR = 0;
	iS32 Point[2][2];

	DuckResidueFlag = iFALSE;
	if(FactY>SCREEN_HEIGHT+5 || FactX >  SCREEN_WIDTH+5 || FactX < -5 || FactY< -5 )
	{
		i51KitP2ReleaseBody(mwSpace, (iHANDLE)body);
		i51KitP2ReleaseShape(mwSpace,(iHANDLE)(circle));
		return iTRUE;
	}

	for( i = 0; i< Pipe_Manage.PipeNum; i++ )
	{
		if(	PipeMess[i].ValidityFlag 
		&&	FactX>PipeMess[i].RectWaterIn.X && FactX<PipeMess[i].RectWaterIn.X+PipeMess[i].RectWaterIn.Width
		&&	FactY>PipeMess[i].RectWaterIn.Y && FactY<PipeMess[i].RectWaterIn.Y+PipeMess[i].RectWaterIn.Height
		)
		{
			SpeedX = PipeMess[i].SpeedX*COORDINATE_RATE;
			SpeedY = PipeMess[i].SpeedY*COORDINATE_RATE;
			OutX = PipeMess[i].WaterOutX*COORDINATE_RATE;
			OutY = PipeMess[i].WaterOutY*COORDINATE_RATE;

			switch(PipeMess[i].SpeedX)
			{
				case PipeSpeedX_HOLD:
					if(body->V.X<0)
					{
						body->V.X = -body->V.X;
					}
					SpeedX = ((body->V.X));
					break;
				case PipeSpeedXFromY:
					SpeedX = (body->V.Y*PipeSpeedWeight);
					break;
				case PipeSpeedXZeor:
					SpeedX = 0;
					break;
				case PipeSpeedX_INYX:
					SpeedX = -((body->V.X*PipeSpeedWeight));
					break;
				case PipeSpeedX_INYY:
					SpeedX = -((body->V.Y*PipeSpeedWeight));
					break;
				default:
					SpeedX = PipeMess[i].SpeedX;
					break;
			}
			
			switch(PipeMess[i].SpeedY)
			{
				case PipeSpeedY_HOLD:
					SpeedY = ((body->V.Y));
					break;
				case PipeSpeedYFromX:
					SpeedY = (body->V.X*PipeSpeedWeight);
					break;
				case PipeSpeedYZeor:
					SpeedY = 0;
					break;
				case PipeSpeedY_INYX:
					SpeedY = -((body->V.X*PipeSpeedWeight));
					break;
				case PipeSpeedY_INYY:
					SpeedY = -((body->V.Y));
					break;
				default:
					SpeedY = PipeMess[i].SpeedY;
					break;
			}

			if(OutX == PipeOutX_HOLD*COORDINATE_RATE)	
			{
				OutX = X;
			}
			else
			{
				OutX = PipeMess[i].WaterOutX*COORDINATE_RATE;
			}
			
			if(OutY == PipeOutY_HOLD*COORDINATE_RATE)
			{
				OutY = Y;
			}
			else
			{
				OutY = PipeMess[i].WaterOutY*COORDINATE_RATE;
			}

			iVectInit2(&(body->Centroid), OutX, OutY);
			iVectInit2(&(body->V), SpeedX, SpeedY);
			if( i>0)
			{
				iVectInit2(&(body->MAX_V), 2500, 2500);
			}
			else
			{
				iVectInit2(&(body->MAX_V), 1000, 1000);
			}
//			else if( CurrLevelNum==MW_LEVEL_011 && i<5 )
			if( CurrLevelNum==MW_LEVEL_011 && i<5 )
			{
				iVectInit2(&(body->MAX_V), 1400, 1400);
			}
			else if( CurrLevelNum==MW_LEVEL_015 )
			{
				iVectInit2(&(body->MAX_V), 1400, 1400);
			}
			
			iVectInit2(&(circle->CentrePoint), OutX, OutY);

			iVectInit1( &(circle->LastPoint4), circle->CentrePoint);
			iVectInit1( &(circle->LastPoint3), circle->CentrePoint);
			iVectInit1( &(circle->LastPoint2), circle->CentrePoint);
			iVectInit1( &(circle->LastPoint1), circle->CentrePoint);
			iVectInit1( &(circle->LastPoint), circle->CentrePoint);

			X = (iS32)(circle->CentrePoint.X);
			Y = (iS32)(circle->CentrePoint.Y);

			if( i==0 )
			{
//				i51KitP2ReleaseBody(mwSpace, (iHANDLE)body);
//				i51KitP2ReleaseShape(mwSpace,(iHANDLE)(circle));
				body->Flag = iTRUE;
				WaterInBathtub++;
			}
			
			if( WaterInBathtub>=WIN_WATER )
			{
				CurrLevelState = MW_LEVEL_WIN;
			}
		}
	}	

	if( SpeedKeep_Manage.ActiveSpeedKeepNum>0 )
	{
		for( i = 0; i< SpeedKeep_Manage.TotalSpeedKeepNum; i++ )
		{
			if( SpeedKeepMess[i].KeepSpeedFlag )
			{
				StartX = SpeedKeepMess[i].Rect.X;
				StartY = SpeedKeepMess[i].Rect.Y;
				EndX = SpeedKeepMess[i].Rect.Width + StartX;
				EndY = SpeedKeepMess[i].Rect.Height + StartY;
				if( FactX>=StartX && FactX<=EndX && FactY>=StartY && FactY<=EndY )
				{
					CurrX = FactX + SpeedKeepMess[i].MoveX;
					CurrY = FactY+ SpeedKeepMess[i].MoveY;
					SpeedX =SpeedKeepMess[i].SpeedX;
					SpeedY = SpeedKeepMess[i].SpeedY;

					if( CurrLevelNum==MW_LEVEL_015 )
					{
						if( CurrX<StartX+40 ) CurrX = StartX+40;
						if( CurrX>285) CurrX = 285;

						if( CurrX>=270 && CurrX<=300 && CurrY>=305 && CurrY<= 460 )
						{
							iVectInit2(&(body->MAX_V), 1400, 3000);
							SpeedY = - 30;
							SpeedX = 0;
						}
					}

					CurrX = CurrX*COORDINATE_RATE + (X%COORDINATE_RATE);
					CurrY = CurrY*COORDINATE_RATE + (Y%COORDINATE_RATE);
					
					iVectInit2(&(body->V), SpeedX*COORDINATE_RATE, SpeedY*COORDINATE_RATE);
					iVectInit2(&(body->Centroid), CurrX, CurrY);
					iVectInit2(&(circle->CentrePoint), CurrX, CurrY);

					X = (iS32)(circle->CentrePoint.X);
					Y = (iS32)(circle->CentrePoint.Y);
				}
			}
		}
	}

	if( Duck_Manage.ResidueDuckNum>0)
	{
		for( i = 0; i< DUCK_NUM; i++ )
		{
			if( DuckMess[i].DuckExist && (i51AdeOsGetTick()-DuckMess[i].OldRushTime>=DUCK_RUSH_DELAY_TIME) )
			{
				if( FactX>DuckMess[i].Rect.X+10 && FactX<DuckMess[i].Rect.X+45
				&&	FactY>DuckMess[i].Rect.Y+10 && FactY<DuckMess[i].Rect.Y+45
				)
				{
					DuckMess[i].OldRushTime = i51AdeOsGetTick();
					DuckMess[i].WaterNum++;
					if(DuckMess[i].WaterNum>=DUCK_MAX_RUSH_NUM )
					{
						DuckMess[i].DuckExist = iFALSE;
						Duck_Manage.ResidueDuckNum --;
						DuckResidueFlag = iTRUE;
//						mwTempScreenBufUpdate(CurrLevelNum);
					}

					i51KitP2ReleaseBody(mwSpace, (iHANDLE)body);
					i51KitP2ReleaseShape(mwSpace, (iHANDLE)circle);

					return iTRUE;

				}
			}
		}
	}

	if(AlgaeNeed && (mwWorldAttribute[FactY/2][FactX/2]&WORD_ALGAE) == WORD_ALGAE
		&& (FactY>6) && (FactX>6) && (FactX<(SCREEN_WIDTH-6)) && (FactY<(SCREEN_HEIGHT-6))  )
	{
		AddY = FactY/2;
		AddX = FactX/2;

		X= circle->LastPoint.X/COORDINATE_RATE;
		Y= circle->LastPoint.Y/COORDINATE_RATE;

		if( mwWorldAttribute[AddY][AddX] >= WORD_ALGAE )
		{
			i51KitP2ReleaseBody(mwSpace, (iHANDLE)body);
			i51KitP2ReleaseShape(mwSpace, (iHANDLE)circle);
		}
		else
		{
			return iFALSE;
		}

		Count = 0;
		for( i=0; i<7; i++ )
		{
			for( j=0; j<7; j++ )
			{
				if( mwWorldAttribute[AddY+YMul[i][j]][AddX+XMul[i][j]] == WORD_SPACE)
				{
					Count++;
					mwWorldAttribute[AddY+YMul[i][j]][AddX+XMul[i][j]] = WORD_ALGAE;
				}
			}
		}

		if(Count<2)
		{
			if( (Y>6) && (X>6) && (X<(SCREEN_WIDTH-6)) && (Y<(SCREEN_HEIGHT-6))  )
			{
				if( Y>FactY )	AddY += 3;
				else if( Y<FactY )AddY -= 3;
				
				if( X>FactX )	AddX += 3;
				else if( X<FactX )AddX -= 3;

				Count = 0;
				for( i=0; i<7; i++ )
				{
					for( j=0; j<7; j++ )
					{
						if( mwWorldAttribute[AddY+YMul[i][j]][AddX+XMul[i][j]] == WORD_SPACE)
						{
							Count++;
							mwWorldAttribute[AddY+YMul[i][j]][AddX+XMul[i][j]] = WORD_ALGAE;
						}
					}
				}
			}
		}

		if( Count>5 )
		{
			mwWorldAttribute[AddY][AddX] = ((rand()%5+1)<<4) | WORD_ALGAE;
			TempScreenUpdataFlag = iTRUE;
		}
		
		return iTRUE;
		
	}

	if( Organ_Manage.ResidueOrganNum>0 )
	{
		for( i=0; i<Organ_Manage.TotalOrganNum; i++ )
		{
			if( OrganMess[i].OrgneState == ORGAN_STATE_SRC )
			{
				SrcRect = &(OrganMess[i].KeyRect);
				if( Y>=SrcRect->Y && Y < SrcRect->Y + SrcRect->Height 
				&&	X>=SrcRect->X && X < SrcRect->X + SrcRect->Width
				&& i51AdeOsGetTick() - OrganMess[i].OldTime >= ORGAN_TIME_DELAY
				)
				{
					i51KitP2ReleaseBody(mwSpace, (iHANDLE)body);
					i51KitP2ReleaseShape(mwSpace, (iHANDLE)circle);
					OrganMess[i].WaterNum ++;
					OrganMess[i].OldTime = i51AdeOsGetTick();
					if( OrganMess[i].WaterNum >= ORGAN_MAX_WATER )
					{
						OrganMess[i].OrgneState = ORGAN_STATE_MOVE;
					}

					return iTRUE;
				}
			}
		}
	}

#if 1

if( body->Flag )
{
	WaterDrawAll = iTRUE;
	COLOR_ALPHA = 200;
}
else
{
	COLOR_ALPHA = 130;
	WaterDrawAll = iFALSE;
}

if(  (AbsInt(ShapeCircle->CentrePoint.X-ShapeCircle->LastPoint4.X) + AbsInt(ShapeCircle->CentrePoint.Y-ShapeCircle->LastPoint4.Y)<1000)  )
{
	mwFillCircle( X/COORDINATE_RATE, Y/COORDINATE_RATE, MAX_CIRCLE_R, Color);
}
else
{
se_TrackPoint[5][0] = ShapeCircle->CentrePoint.X/COORDINATE_RATE;
se_TrackPoint[5][1] = ShapeCircle->CentrePoint.Y/COORDINATE_RATE;
se_TrackPoint[4][0] = ShapeCircle->LastPoint.X/COORDINATE_RATE;
se_TrackPoint[4][1] = ShapeCircle->LastPoint.Y/COORDINATE_RATE;
se_TrackPoint[3][0] = ShapeCircle->LastPoint1.X/COORDINATE_RATE;
se_TrackPoint[3][1] = ShapeCircle->LastPoint1.Y/COORDINATE_RATE;
se_TrackPoint[2][0] = ShapeCircle->LastPoint2.X/COORDINATE_RATE;
se_TrackPoint[2][1] = ShapeCircle->LastPoint2.Y/COORDINATE_RATE;
se_TrackPoint[1][0] = ShapeCircle->LastPoint3.X/COORDINATE_RATE;
se_TrackPoint[1][1] = ShapeCircle->LastPoint3.Y/COORDINATE_RATE;
se_TrackPoint[0][0] = ShapeCircle->LastPoint4.X/COORDINATE_RATE;
se_TrackPoint[0][1] = ShapeCircle->LastPoint4.Y/COORDINATE_RATE;
se_TrackPointCount = 6;
i51KitG1Draw();
}
#else	
	VX = (iS32)(body->V.X);
	VY = (iS32)(body->V.Y);
	R = (iU32)(MAX_CIRCLE_R);

	if( body->Flag )
	{
		WaterDrawAll = iTRUE;
		COLOR_ALPHA = 200;
	}
	else
	{
		COLOR_ALPHA = 130;
		WaterDrawAll = iFALSE;
	}
	
	if( (VX>4000||VX<-4000||VY>4000||VY<-4000))
	{
		V = SqrtInt(body->V.X*body->V.X+body->V.Y*body->V.Y);
		CentreX= circle->CentrePoint.X-body->V.X;
		CentreY = circle->CentrePoint.Y-body->V.Y;
		CurrR = R - V/2000;
		Point[2][2];

		Point[0][0] = (iS32)X;
		Point[0][1] = (iS32)Y;
		Point[1][0] = (iS32)CentreX;
		Point[1][1] = (iS32)CentreY;
		i51KitG2SetAlpha(COLOR_ALPHA);
		i51KitG2DrawLine(Point, Color);
		i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
		if(CurrR<3) CurrR = 3;
		mwFillCircle( CentreX/COORDINATE_RATE, CentreY/COORDINATE_RATE, CurrR, Color);

	}
	mwFillCircle( X/COORDINATE_RATE, Y/COORDINATE_RATE, R, Color);
#endif

	CurrTotalWaterNum ++;
	
	return iTRUE;
	
}

iBOOL mwPublicAnimation( AnimationStr * AnimationMess )
{
	iU32 CurrTime = i51AdeOsGetTick();
	iU32 OldTime = AnimationMess->OldPlayTime;
	iRECT SrcRect;
	
	if( CurrTime- OldTime >= AnimationMess->FrameTimeMul )
	{
		AnimationMess->CurrPlayFrame = (AnimationMess->CurrPlayFrame+1)%AnimationMess->ImageTotalFrame;
		AnimationMess->OldPlayTime = CurrTime;
	}

	i51KitG2DrawImage
		(
		AnimationMess->Container,
		AnimationMess->ImageId[AnimationMess->CurrPlayFrame],
		&(AnimationMess->DesRect),
		iNULL
		);

	return iTRUE;
}

iBOOL mwPublicAnimationEx( AnimationStr * AnimationMess )
{
	iU32 CurrTime = i51AdeOsGetTick();
	iU32 OldTime = AnimationMess->OldPlayTime;
	iRECT SrcRect;
	
	if( CurrTime- OldTime >= AnimationMess->FrameTimeMul )
	{
		AnimationMess->CurrPlayFrame = (AnimationMess->CurrPlayFrame+1)%AnimationMess->ImageTotalFrame;
		AnimationMess->OldPlayTime = CurrTime;
	}

	SrcRect.X = 0;
	SrcRect.Y = AnimationMess->CurrPlayFrame*AnimationMess->FrameHeight;
	SrcRect.Width = AnimationMess->ImageWidth;
	SrcRect.Height = AnimationMess->FrameHeight;

	i51KitG2DrawImage
		(
		AnimationMess->Container,
		AnimationMess->ImageId[0],
		&(AnimationMess->DesRect),
		&SrcRect
		);

	return iTRUE;
}

iBOOL mwPublicDrawPrompt( iU32 DisPlayTime, iU32 StartTime, iU32 DisplayID )
{
	// 从StartTime时间开始显示DisplayID号提示信息DisPlayTime毫秒
	// 显示时间到返回iTRUE,  否则返回iFALSE
	iRECT DesRect;
	iRECT SrcRect;
	
	if( i51AdeOsGetTick() - StartTime >= DisPlayTime )
	{
		return iTRUE;
	}
	
	DesRect.X = PromptFrame_X;
	DesRect.Y = PromptFrame_Y;
	DesRect.Width= PromptFrame_W;
	DesRect.Height= PromptFrame_H;
	i51KitG2DrawImage(mwContainer, PromptFrame, &DesRect, iNULL);
	DesRect.X = PromptWord_X;
	DesRect.Y = PromptWord_Y;
	DesRect.Width= PromptWord_W;
	DesRect.Height= PromptWord_H;
	SrcRect.X = 0;
	SrcRect.Y = PromptWord_H*DisplayID;
	SrcRect.Width= PromptWord_W;
	SrcRect.Height= PromptWord_H;
	i51KitG2DrawImage(mwContainer, PromptWord, &DesRect, &SrcRect);

	return iFALSE;
	
}

