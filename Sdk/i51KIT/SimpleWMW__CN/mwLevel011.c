
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwInitLevel011( void )
{
	iBody*body;//, *staticBody = &(mwSpace->staticBody);
	iShape *shape;
	iS32 radius = WATER_RADIUS;
	iS32 i=0,j=0;
	iVECT pos;
	
	mwInitSpace(CurrLevelNum);	
	iVectInit2( &(mwSpace->WorldGravity), 0, 150);

	for( i=0; i<1; i++){
		for( j=0; j<10; j++){
#if defined(PKIG_SCREEN_240X320)
			iVectInit2(&pos,(j)*4*COORDINATE_RATE+20*COORDINATE_RATE,(i)*4*COORDINATE_RATE+110*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_240X400)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+16*COORDINATE_RATE,(i)*8*COORDINATE_RATE+140*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_320X480)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+16*COORDINATE_RATE,(i)*8*COORDINATE_RATE+159*COORDINATE_RATE);

#endif

			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),0,0);
			iVectInit2(&(body->MAX_V),WATER_MAX_SPEED,WATER_MAX_SPEED);
			shape = i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);

#if defined(PKIG_SCREEN_240X320)
			iVectInit2(&pos,(j)*4*COORDINATE_RATE+180*COORDINATE_RATE,(i)*4*COORDINATE_RATE+110*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_240X400)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+170*COORDINATE_RATE,(i)*8*COORDINATE_RATE+140*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_320X480)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+230*COORDINATE_RATE,(i)*8*COORDINATE_RATE+159*COORDINATE_RATE);

#endif

			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),0,0);
			iVectInit2(&(body->MAX_V),WATER_MAX_SPEED,WATER_MAX_SPEED);
			shape = i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
	}
	
	mwLevelWordInit(CurrLevelNum);
	mwLevelDuckInit(CurrLevelNum);
	mwLevelPipeInit(CurrLevelNum);
	mwLevelWaitInit(CurrLevelNum);

	mwLevelDrawBKwall();
	SWMW_i51KitG2DrawImage(&iMageInfo[MudLevel011], iNULL, iNULL);
	SWMW_i51KitG2DrawImage(&iMageInfo[RockLevel011], iNULL, iNULL);

	mwLevelDrawPause();
	mwLevelDrawRestart();

	mwLevelOrganInit(CurrLevelNum);

/*
	for( i =0;i<Organ_Manage.TotalOrganNum;i++ )

	{
		cpv(&v1,OrganMess[i].OrganStartRect.X,OrganMess[i].OrganStartRect.Y);
		cpv(&v2,OrganMess[i].OrganStartRect.X + OrganMess[i].OrganStartRect.Width,OrganMess[i].OrganStartRect.Y);
		shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, v1,v2,RECT_SEG_R));
	 	shape->e = SHAPE_E; 
		shape->u = SHAPE_U; 
		shape->block.ShapeType = OrganMess[i].OrganType;
	}
*/
#ifdef __MODIS_DEBUG__
	for( i=0;i<SCREEN_HEIGHT; i++)
	{
		i51AdeStdMemcpy16( (void *)(TempScreenBuf + i*SCREEN_WIDTH),
			(void *)(mwScreenBuffer+i*320),			 
			SCREEN_WIDTH*2);
	}
#else
	i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);
#endif
	mwLevelSpeedKeepInit(CurrLevelNum);
	mwTempScreenBufUpdate(CurrLevelNum);

	mwLevelDrawDuck();
//	mwLevelDrawOrgan();
	TimeLevelStart = i51AdeOsGetTick();
	return iTRUE;
}

iBOOL mwRunLevel011( void )
{
	
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;
	int i = 0;
	mwLevelWaterSpray(CurrLevelNum);

#ifdef __MODIS_DEBUG__
	for( i=0;i<SCREEN_HEIGHT; i++)
	{
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+i*320),
			(void *)(TempScreenBuf + i*SCREEN_WIDTH), 
			SCREEN_WIDTH*2);
	}
#else
	i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
		(void *)(TempScreenBuf), 
		SCREEN_WIDTH*SCREEN_HEIGHT*2);

#endif

 	mwPublicAnimation(&CrocodileWait);
	mwLevelDrawDuck();
	
	cpWordDraw();
	if( TempScreenUpdataFlag )
	{
		TempScreenUpdataFlag = iFALSE;
		mwTempScreenBufUpdate(CurrLevelNum);
		cpWordDraw();
	}

	if(	OrganMess[0].OrgneState != ORGAN_STATE_DES)
	{
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
	}
	if(	OrganMess[1].OrgneState != ORGAN_STATE_DES)
	{
		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
	}
//	mwLevelDrawOrgan();
	
	TimeLevelCurr= i51AdeOsGetTick();
	if( TimeLevelCurr- TimeLevelStart < PromptShowTime )
	{
		mwLevelDrawPrompt(CurrLevelNum);
	}

	if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		if(mwLevelPressRestart(PenDownX, PenDownY))
		{
			CurrLevelState = MW_LEVEL_INIT;
		}

		else if( mwLevelPressPause(PenDownX, PenDownY))
		{
			wmw_pause_page();
			CurrLevelState = MW_LEVEL_PAUSE;
			mwWordPause = iTRUE;
		}
	}

	return iTRUE;
	
}





