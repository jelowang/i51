
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
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+16*COORDINATE_RATE,(i)*8*COORDINATE_RATE+159*COORDINATE_RATE);
			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),0,0);
			iVectInit2(&(body->MAX_V),WATER_MAX_SPEED,WATER_MAX_SPEED);
			shape = i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);

			iVectInit2(&pos,(j)*6*COORDINATE_RATE+230*COORDINATE_RATE,(i)*8*COORDINATE_RATE+159*COORDINATE_RATE);
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
	i51KitG2DrawImage(mwContainer, MudLevel011, iNULL, iNULL);
	i51KitG2DrawImage(mwContainer, RockLevel011, iNULL, iNULL);

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
	i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);
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

	mwLevelWaterSpray(CurrLevelNum);

	i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
			(void *)(&TempScreenBuf[0][0]), 
			SCREEN_WIDTH*SCREEN_HEIGHT*2);

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
			CurrLevelState = MW_LEVEL_PAUSE;
			mwWordPause = iTRUE;
		}
	}

	return iTRUE;
	
}





