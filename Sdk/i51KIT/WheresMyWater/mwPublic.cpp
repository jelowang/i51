#include "i51Public.h"
#include "Chipmunk.h"
#include "mwLevelPublic.h"


#define MAX_CIRCLE_R	6
#define MAX_FILL_NUM	30
#define COLOR_WATER		0X3FFF
#define COLOR_ALPHA		255

#define SPEED_WATER_MAX	400

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

// 每一关网格约束大小
//iS16 RectSegR[15] = {
//	RECT_SEG_R,RECT_SEG_R,RECT_SEG_R,RECT_SEG_R,5,
//	RECT_SEG_R,RECT_SEG_R,RECT_SEG_R,RECT_SEG_R,RECT_SEG_R,
//	RECT_SEG_R,RECT_SEG_R,RECT_SEG_R,RECT_SEG_R,RECT_SEG_R
//};
// 每一关石头约束大小
//iS16 RockSegR[15] = {
//	ROCK_SEG_R,ROCK_SEG_R,ROCK_SEG_R,ROCK_SEG_R,5,
//	ROCK_SEG_R,ROCK_SEG_R,ROCK_SEG_R,ROCK_SEG_R,ROCK_SEG_R,
//	ROCK_SEG_R,ROCK_SEG_R,ROCK_SEG_R,ROCK_SEG_R,ROCK_SEG_R
//};
// 每一关浴缸约束大小
//iS16 BathSegR[15] = {
//	BATH_SEG_R,BATH_SEG_R,BATH_SEG_R,BATH_SEG_R,5,
//	BATH_SEG_R,BATH_SEG_R,BATH_SEG_R,BATH_SEG_R,BATH_SEG_R,
//	BATH_SEG_R,BATH_SEG_R,BATH_SEG_R,BATH_SEG_R,BATH_SEG_R
//};

void * i51_malloc(unsigned int size)
{
	void * p=null;
	if(size<4) 
	{
		iLog("i51_malloc size<4");
		size = 4;
	}
	
	p = (void *)i51AdeOsMalloc(size,__FILE__,__LINE__);
	if(null==p)
	{
		iLog("i51_malloc err");
		return null;
	}

	return p;
	
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
	int CopySize = 0;
	if(!resize){
		i51_free(src_ptr);
	}
	else
	{
		temp_ptr = (void*)i51_malloc(resize);
		if(temp_ptr)
		{
			if(src_ptr)
			{
				CopySize = size < resize ? size : resize;
//				memcpy(temp_ptr, src_ptr, size < resize ? size : resize);
				i51AdeStdMemcpy16(temp_ptr, src_ptr, CopySize);
			}
			i51_free(src_ptr);
		}
		else
		{
			iLog("mmRealloc malloc err!");
		}
	}

	return temp_ptr;
}

void *mmCalloc(int num, int size)
{
	void *p = (void *)i51_malloc(num * size);
	if(p)
	{
		memset(p, 0, num * size);
	}
	else
	{
		iLog("mmCalloc malloc err!");
	}
	return p;
}


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
		if( CurrLineEndX >= SCREEN_WIDTH ) CurrLineEndX = SCREEN_WIDTH -1;
		if( CurrLineStartX < 0 ) CurrLineStartX = 0;
		CurrLineLength = CurrLineEndX - CurrLineStartX;
		ScreenPos = CurrLineY*SCREEN_WIDTH + CurrLineStartX;
		for(int i=0;i<CurrLineLength;i++)
		{
			mwScreenBuffer[ScreenPos] = i51KitG2GetNewColorEx(mwScreenBuffer[ScreenPos], COLOR_WATER, COLOR_ALPHA);
			ScreenPos++;
		}
//        i51AdeStdMemcpy16(&mwScreenBuffer[ScreenPos], GraphFillColor, (CurrLineLength<<1));
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
			if( CurrLineEndX >= SCREEN_WIDTH ) CurrLineEndX = SCREEN_WIDTH -1;
			if( CurrLineStartX < 0 ) CurrLineStartX = 0;
			CurrLineLength = CurrLineEndX - CurrLineStartX;
			ScreenPos = CurrLineY*SCREEN_WIDTH + CurrLineStartX;
			for(int i=0;i<CurrLineLength;i++)
			{
				mwScreenBuffer[ScreenPos] = i51KitG2GetNewColorEx(mwScreenBuffer[ScreenPos], COLOR_WATER, COLOR_ALPHA);
				ScreenPos++;
			}
//			i51AdeStdMemcpy16(&mwScreenBuffer[ScreenPos], GraphFillColor, (CurrLineLength<<1));
		}
//iLog("%d %d %d",CurrLineStartX,CurrLineY,CurrLineLength);

        //绘制二四象限的图像
		CurrLineY = Center2Y - CurrentY;
		CurrLineStartX = CurrentX;
		CurrLineEndX = Center2X - CurrentX;
		if( CurrLineY>=0 && CurrLineY<SCREEN_HEIGHT && CurrLineEndX>=0 && CurrLineStartX<SCREEN_WIDTH )
		{
			if( CurrLineEndX >= SCREEN_WIDTH ) CurrLineEndX = SCREEN_WIDTH -1;
			if( CurrLineStartX < 0 ) CurrLineStartX = 0;
			CurrLineLength = CurrLineEndX - CurrLineStartX;
			ScreenPos = CurrLineY*SCREEN_WIDTH + CurrLineStartX;
			for(int i=0;i<CurrLineLength;i++)
			{
				mwScreenBuffer[ScreenPos] = i51KitG2GetNewColorEx(mwScreenBuffer[ScreenPos], COLOR_WATER, COLOR_ALPHA);
				ScreenPos++;
			}
//			i51AdeStdMemcpy16(&mwScreenBuffer[ScreenPos], GraphFillColor, (CurrLineLength<<1));
		}
//iLog("%d %d %d",CurrLineStartX,CurrLineY,CurrLineLength);


    }

    #undef ABSOLUTE

    return iTRUE;

}

iBOOL DrawWaterDrop(cpBody *body, cpCircleShape *circle, iCOLOR Color)
{
	iS32 X = (iS32)(circle->tc.x);
	iS32 Y = (iS32)(circle->tc.y);

	DuckResidueFlag = iFALSE;
	if(Y>SCREEN_HEIGHT+5 || X >  SCREEN_WIDTH+5 || X < -5 || Y< -5 )
	{
		cpSpaceRemoveBody(mwSpace, body);
		cpSpaceRemoveShape(mwSpace,(cpShape*)(circle));
		cpBodyFree(body);
		cpShapeFree((cpShape*)(circle));
		return iTRUE;
	}

	for( int i = 0; i< Pipe_Manage.PipeNum; i++ )
	{
		if(	PipeMess[i].ValidityFlag 
		&&	X>PipeMess[i].RectWaterIn.X && X<PipeMess[i].RectWaterIn.X+PipeMess[i].RectWaterIn.Width
		&&	Y>PipeMess[i].RectWaterIn.Y && Y<PipeMess[i].RectWaterIn.Y+PipeMess[i].RectWaterIn.Height
		)
		{
			iS16 SpeedX = PipeMess[i].SpeedX;
			iS16 SpeedY = PipeMess[i].SpeedY;
			iS16 OutX = PipeMess[i].WaterOutX;
			iS16 OutY = PipeMess[i].WaterOutY;

			switch(PipeMess[i].SpeedX)
			{
				case PipeSpeedX_HOLD:
					if(body->v.x<0)
					{
						body->v.x = -body->v.x;
					}
					SpeedX = ((iS16)(body->v.x));
					break;
				case PipeSpeedXFromY:
					SpeedX = (iS16)(body->v.y*PipeSpeedWeight);
					break;
				case PipeSpeedXZeor:
					SpeedX = 0;
					break;
				case PipeSpeedX_INYX:
					SpeedX = -((iS16)(body->v.x*PipeSpeedWeight));
					break;
				case PipeSpeedX_INYY:
					SpeedX = -((iS16)(body->v.y*PipeSpeedWeight));
					break;
				default:
					SpeedX = PipeMess[i].SpeedX;
					break;
			}
			
			switch(PipeMess[i].SpeedY)
			{
				case PipeSpeedY_HOLD:
					SpeedY = ((iS16)(body->v.y));
					break;
				case PipeSpeedYFromX:
					SpeedY = (iS16)(body->v.x*PipeSpeedWeight);
					break;
				case PipeSpeedYZeor:
					SpeedY = 0;
					break;
				case PipeSpeedY_INYX:
					SpeedY = -((iS16)(body->v.x*PipeSpeedWeight));
					break;
				case PipeSpeedY_INYY:
					SpeedY = -((iS16)(body->v.y));
					break;
				default:
					SpeedY = PipeMess[i].SpeedY;
					break;
			}

			if(OutX == PipeOutX_HOLD )	
			{
				OutX = X;
			}
			else
			{
				OutX = PipeMess[i].WaterOutX;
			}
			
			if(OutY == PipeOutY_HOLD )
			{
				OutY = Y;
			}
			else
			{
				OutY = PipeMess[i].WaterOutY;
			}
				
			cpBodySetPos(body,cpv(OutX,OutY));
			body->v = cpv(SpeedX,SpeedY);
			cpSpaceRehashShape(mwSpace, (cpShape*)(circle));
			X = (iS32)(circle->tc.x);
			Y = (iS32)(circle->tc.y);

			if( i==0 )
			{
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
		for( int i = 0; i< SpeedKeep_Manage.TotalSpeedKeepNum; i++ )
		{
			if( SpeedKeepMess[i].KeepSpeedFlag )
			{
				iS16 StartX = SpeedKeepMess[i].Rect.X;
				iS16 StartY = SpeedKeepMess[i].Rect.Y;
				iS16 EndX = SpeedKeepMess[i].Rect.Width + StartX;
				iS16 EndY = SpeedKeepMess[i].Rect.Height + StartY;
				if( X>=StartX && X<=EndX && Y>=StartY && Y<=EndY )
				{
					iS16 CurrX = X + SpeedKeepMess[i].MoveX;
					iS16 CurrY = Y + SpeedKeepMess[i].MoveY;
					iS16 SpeedX =SpeedKeepMess[i].SpeedX;
					iS16 SpeedY = SpeedKeepMess[i].SpeedY;
				//	if( CurrX < StartX+32 )
					{
				//		CurrX = StartX +32;
					}
					cpBodySetPos(body,cpv(CurrX,CurrY));

					if( CurrLevelNum==MW_LEVEL_015 )
					{
						if( CurrX>=270 && CurrX<=300 && CurrY>=400 && CurrY<= 460 )
						{
							SpeedY = - 450;
							SpeedX = 30;
						}
					}
					body->v = cpv(SpeedX,SpeedY);
					cpSpaceRehashShape(mwSpace, (cpShape*)(circle));
					X = (iS32)(circle->tc.x);
					Y = (iS32)(circle->tc.y);
				}
			}
		}
	}

	if( Duck_Manage.ResidueDuckNum>0)
	{
		for( int i = 0; i< DUCK_NUM; i++ )
		{
			if( DuckMess[i].DuckExist && (i51AdeOsGetTick()-DuckMess[i].OldRushTime>=DUCK_RUSH_DELAY_TIME) )
			{
				if( X>DuckMess[i].Rect.X+15 && X<DuckMess[i].Rect.X+35
				&&	Y>DuckMess[i].Rect.Y+15 && Y<DuckMess[i].Rect.Y+35
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

					cpSpaceRemoveBody(mwSpace, body);
					cpSpaceRemoveShape(mwSpace,(cpShape*)(circle));
					cpBodyFree(body);
					cpShapeFree((cpShape*)(circle));
					return iTRUE;
				}
			}
		}
	}

	if(AlgaeNeed && (mwWorldAttribute[Y/2][X/2]&WORD_ALGAE) == WORD_ALGAE
		&& (Y>6) && (X>6) && (X<(SCREEN_WIDTH-6)) && (Y<(SCREEN_HEIGHT-6))  )
	{
		iS16 AddY = Y/2;
		iS16 AddX = X/2;
		iS16 XMul[7][7] = {
				{-3,-2,-1,0,1,2,3},
				{-3,-2,-1,0,1,2,3},
				{-3,-2,-1,0,1,2,3},
				{-3,-2,-1,0,1,2,3},
				{-3,-2,-1,0,1,2,3},
				{-3,-2,-1,0,1,2,3},
				{-3,-2,-1,0,1,2,3}
			};
		iS16 YMul[7][7] = {
				{-3,-3,-3,-3,-3,-3,-3},
				{-2,-2,-2,-2,-2,-2,-2},
				{-1,-1,-1,-1,-1,-1,-1},
				{0,0,0,0,0,0,0},
				{1,1,1,1,1,1,1},
				{2,2,2,2,2,2,2},
				{3,3,3,3,3,3,3}
			};


		if( mwWorldAttribute[AddY][AddX] >= WORD_ALGAE )
		{
			cpSpaceRemoveBody(mwSpace, body);
			cpSpaceRemoveShape(mwSpace,(cpShape*)(circle));
			cpBodyFree(body);
			cpShapeFree((cpShape*)(circle));
		}
		else
		{
			return iFALSE;
		}

		iU8 Count = 0;
		for( int i=0; i<7; i++ )
		{
			for( int j=0; j<7; j++ )
			{
				if( mwWorldAttribute[AddY+YMul[i][j]][AddX+XMul[i][j]] == WORD_SPACE)
				{
					Count++;
					mwWorldAttribute[AddY+YMul[i][j]][AddX+XMul[i][j]] = WORD_ALGAE;
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
		for( int i=0; i<Organ_Manage.TotalOrganNum; i++ )
		{
			if( OrganMess[i].OrgneState == ORGAN_STATE_SRC )
			{
				iRECT * SrcRect = &(OrganMess[i].KeyRect);
				if( Y>=SrcRect->Y && Y < SrcRect->Y + SrcRect->Height 
				&&	X>=SrcRect->X && X < SrcRect->X + SrcRect->Width
				&& i51AdeOsGetTick() - OrganMess[i].OldTime >= ORGAN_TIME_DELAY
				)
				{
					cpSpaceRemoveBody(mwSpace, body);
					cpSpaceRemoveShape(mwSpace,(cpShape*)(circle));
					cpBodyFree(body);
					cpShapeFree((cpShape*)(circle));
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

	
	iS32 VX = (iS32)(body->v.x);
	iS32 VY = (iS32)(body->v.y);
	iU32 R = (iU32)(MAX_CIRCLE_R);

	if( VX >=SPEED_WATER_MAX )
	{
		VX = SPEED_WATER_MAX;
	}
	else if( VX <= -SPEED_WATER_MAX )
	{
		VX = -SPEED_WATER_MAX;
	}

	if( VY>=SPEED_WATER_MAX )
	{
		 VY = SPEED_WATER_MAX;
	}
	else if( VY <= -SPEED_WATER_MAX )
	{
		VY = -SPEED_WATER_MAX;
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
		i51KitG2SetAlpha(COLOR_ALPHA);
		i51KitG2DrawLine(Point, Color);
		i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
		if(CurrR<3) CurrR = 3;
		mwFillCircle( (iS32)CentreX, (iS32)CentreY, CurrR, Color);

	}
	mwFillCircle( X, Y, R, Color);

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

