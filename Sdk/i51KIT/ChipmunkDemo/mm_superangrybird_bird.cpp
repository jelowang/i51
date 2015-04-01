//#include "../marmota/marmota.h"
#include "inc/mm_superangrybird_resource.h"
#include "mm_superangrybird.h"
#include "marmota_math.h"

static const mmU32 birdAnimationFrames[9] = {1, 2, 2, 2, 1, 1, };
extern mmBool isReadyToShoot;
static const mmU32 birdAnimationSequences[4][9] = {
	{
		IMG_ANGRYBIRD_BIRD_RED,
		IMG_ANGRYBIRD_BIRD_RED, IMG_ANGRYBIRD_BIRD_RED + 3,
		IMG_ANGRYBIRD_BIRD_RED, IMG_ANGRYBIRD_BIRD_RED + 3,
		IMG_ANGRYBIRD_BIRD_RED, IMG_ANGRYBIRD_BIRD_RED + 1,
		IMG_ANGRYBIRD_BIRD_RED + 3,
		IMG_ANGRYBIRD_BIRD_RED + 2,
	},
	{
		IMG_ANGRYBIRD_BIRD_BLUE,
		IMG_ANGRYBIRD_BIRD_BLUE, IMG_ANGRYBIRD_BIRD_BLUE + 1,
		IMG_ANGRYBIRD_BIRD_BLUE, IMG_ANGRYBIRD_BIRD_BLUE + 1,
		IMG_ANGRYBIRD_BIRD_BLUE, IMG_ANGRYBIRD_BIRD_BLUE + 1,
		IMG_ANGRYBIRD_BIRD_BLUE,
		IMG_ANGRYBIRD_BIRD_BLUE,
	},
	{
		IMG_ANGRYBIRD_BIRD_YELLOW,
		IMG_ANGRYBIRD_BIRD_YELLOW, IMG_ANGRYBIRD_BIRD_YELLOW + 1,
		IMG_ANGRYBIRD_BIRD_YELLOW, IMG_ANGRYBIRD_BIRD_YELLOW + 1,
		IMG_ANGRYBIRD_BIRD_YELLOW, IMG_ANGRYBIRD_BIRD_YELLOW + 1,
		IMG_ANGRYBIRD_BIRD_YELLOW,
		IMG_ANGRYBIRD_BIRD_YELLOW,
	},
	{
		IMG_ANGRYBIRD_BIRD_BLACK,
		IMG_ANGRYBIRD_BIRD_BLACK, IMG_ANGRYBIRD_BIRD_BLACK + 1,
		IMG_ANGRYBIRD_BIRD_BLACK, IMG_ANGRYBIRD_BIRD_BLACK + 1,
		IMG_ANGRYBIRD_BIRD_BLACK, IMG_ANGRYBIRD_BIRD_BLACK + 1,
		IMG_ANGRYBIRD_BIRD_BLACK,
		IMG_ANGRYBIRD_BIRD_BLACK,
	},
};

mmSABBird* mmSABBirdListHead;
/*static*/ cpShape* mmSABBlueBirds[2];
extern mmU32 mmSABBlueNumOfPathPoint[2];
extern mmU32 mmSBANumberOfPathPoint;
static cpShape* mmSABBlackBombs[16];
mmS32 mmSABBomTimes;
extern void mm_sba_draw_image(int x, int y, int id, mmBool transparentSign);

static mmS32
getFirstFrame(mmSABBird* bird){
	mmU32 sp = 0, i = 0;
	for(; i < bird->animid; i++)
	{
		sp += birdAnimationFrames[i];
	}
	return sp;
}

static /*inline*/ mmU32 
getBirdImgid(mmSABBird* bird){
	mmU32 j =  getFirstFrame(bird) + bird->curframe;
	return birdAnimationSequences[bird->birdtype][j];
}

static /*inline*/ void
switchBirdState(mmSABBird* bird){
	switch(bird->aimstate)
	{
	case mmSABBirdState_JUMP:
		//bird->body->v.x = (BIRD_STARTPOSITION.x - bird->body->p.x) / 4;
		//bird->body->v.y = (BIRD_STARTPOSITION.y - bird->body->p.y) / 6;
		//bird->body->v.x = (120 - bird->body->p.x) / 4;
		//bird->body->v.y = (BIRD_STARTPOSITION.y - bird->body->p.y) / 15;
		bird->body->v.x = (120 - bird->body->p.x) / 4;
		bird->body->v.y = (BIRD_STARTPOSITION.y - bird->body->p.y) / 8;
	break;
	
	case mmSABBirdState_ONLINE:
		bird->body->p.x = BIRD_STARTPOSITION.x;
		bird->body->p.y = BIRD_STARTPOSITION.y;
	break;
	
	case mmSABBirdState_FLYING:
		bird->animid = mmSABBirdAnimation_FLYING;
		((mmSABBodyData *)bird->body->data)->img_id = getBirdImgid(bird);
	break;
	
	case mmSABBirdState_DIZZY://
		bird->animid = mmSABBirdAnimation_DIZZY;
		((mmSABBodyData *)bird->body->data)->img_id = getBirdImgid(bird);
	break;
	}
}

extern void addEffectObject(mmS32 x, mmS32 y, mmU32 img_id, mmU32 max_img_id, mmFloat a);
static mmS32 scoretime = 0;
void
mmSABBirdCalScore(){
	mmSABBird* cur = mmSABBirdListHead;
	mmSABBodyData *udata;
	while(cur){
		if(!cur->body)
		{
			cur = cur->next;
			continue;
		}
		udata = (mmSABBodyData *)cur->body->data;
		if( (cur->state < mmSABBirdState_FLYING) && udata->hp > 0){
			if(++scoretime == 5){//
					scoretime = 0;//
			mmSABAddScoreObject(cur->body->p.x, cur->body->p.y, cur->birdtype);
			cur->state = mmSABBirdState_DIZZY;
			mmSBAScoreOfStage += 10000;
			udata->hp = 0;
			}//
		}
		cur = cur->next;
	}
	
}

static /*inline*/ void 
mmSABDeleteBirdBody(mmSABBird* bird){
	if(null != bird->body->data)
	{
		mmFree(bird->body->data);
		bird->body->data = null;
	}
	cpSpaceRemoveBody(mmSABSpace, bird->body);
	cpSpaceRemoveShape(mmSABSpace, bird->shape);
	cpBodyFree(bird->body);
	cpShapeFree(bird->shape);
	
	bird->body = null;
	bird->shape = null;
}
extern mmFloat mmSABBirdRoundV;

//static int mmSABJumpOver = 0;
mmBool mmSabWaitBird;
static void
mmSABBirdRefresh(mmSABBird* bird){
	if(bird->animid == mmSABBirdAnimation_JUMP){
			bird->body->p.x = 20 +  (mm_sab_raw_image[((mmSABBodyData *)bird->body->data)->img_id].width >> 1);
	}
	
	if(bird->state != bird->aimstate)
	{
		switchBirdState(bird);
		bird->curframe = 0;
		bird->state = bird->aimstate;
	}	  
	
	if(bird->state == mmSABBirdState_JUMP){
		bird->body->p.x += bird->body->v.x;
		bird->body->p.y += bird->body->v.y;/**/
		bird->body->a += mmSABBirdRoundV;

		
		if(bird->body->p.x > 100){
			bird->body->v.x -= 10;
			bird->body->a = 0;
			bird->aimstate = mmSABBirdState_ONLINE;
		}
		
		if(bird->body->p.y > BIRD_STARTPOSITION.y - 5){
			bird->aimstate = mmSABBirdState_ONLINE;
			bird->body->a = 0;
		}
		
	}else if(bird->state == mmSABBirdState_ONLINE || bird->state == mmSABBirdState_IDLE){
		mmU32 sp = getFirstFrame(bird);
		int max = sp + birdAnimationFrames[bird->animid];
		mmSABBodyData *data = (mmSABBodyData *)bird->body->data;
		
		if(bird->curframe++ == birdAnimationFrames[bird->animid] - 1){
			mmU32 rannumber = rand()%20;
			bird->curframe = 0;
			if(rannumber <= 16)
			{
				bird->animid = mmSABBirdAnimation_IDLE;
			}
			else if(rannumber <= 17)
			{
				bird->animid = mmSABBirdAnimation_BLINK;
			}
			else if(rannumber <= 18)
			{
				if(bird->aimstate == mmSABBirdState_IDLE && bird->state != mmSABBirdState_ONLINE)
				{
					bird->animid = mmSABBirdAnimation_JUMP;
				}
				else
				{
					bird->animid = mmSABBirdAnimation_IDLE;
				}
			}
			else if(rannumber <= 19)
			{
				bird->animid = mmSABBirdAnimation_SING;
			}
		}
		data->img_id = getBirdImgid(bird);
		
		if(bird->animid == mmSABBirdAnimation_JUMP){
			bird->body->p.x = 40;
		}
		/**目的:处理小鸟跳上弹弓但却调到地面的情况**/
		if(mmSABBirdState_ONLINE == bird->state && mmFalse == isReadyToShoot)
		{
			bird->body->p.x = BIRD_STARTPOSITION.x;
			bird->body->p.y = BIRD_STARTPOSITION.y;
		}
		/******************************************/
	}
	else if(bird->state == mmSABBirdState_FLYING)
	{
		if(!bird || !bird->body )return;
		if(mmSABIsOccurSpecialTech && bird == mmSABLastFlying)
		{
			mmSABIsOccurSpecialTech = mmFalse;
			if(bird->body && bird->specialSkill)
			{
				bird->specialSkill(bird);
			}
		}
		else
		{
			if(bird != mmSABLastFlying || (bird->body->p.x < -40  || bird->body->p.y < -40 || bird->body->p.y > MMSAB_SCREEN_HEIGHT + 40) )
				{
					cpBodySleep(bird->body);
					if(mmSABBlueBirds[0])
						{
							cpBodySleep(mmSABBlueBirds[0]->body);
						}
					if(mmSABBlueBirds[1])
						{
							cpBodySleep(mmSABBlueBirds[1]->body);
						}
				}
		
			if(cpBodyIsSleeping(bird->body))
			{
		
				addEffectObject(bird->body->p.x, bird->body->p.y, IMG_ANGRYBIRD_EFFECT, IMG_ANGRYBIRD_EFFECT + 2, 0);
				
				mmSABDeleteBirdBody(bird);
				if(mmSABLastFlying->body == null && mmSABGameState != mmSABUserOperation)
				{
					mmSABGameState = mmSABShowBirdDel;
				}
				mmSABNumberOfBirds--;	
			}
		}
		
	}
}

extern cpBody* mmSABCreateGameObject(int typeId, mmFloat x, mmFloat y, mmFloat a, mmU32 *img_id, cpShape **shape);

static void
mmSABYellowBirdSpecial(void* bird){
	((mmSABBird *)bird)->body->v.y *= 2.4;
	((mmSABBird *)bird)->body->v.x *= 1.8;
}

static void
mmSABRedBirdSpecial(void* bird){
	
}

static void 
mmSABBlueBirdSpecial(void* b){
	int i = 0;
	cpBody *body;
	cpShape *shape;
	mmFloat x;
	mmFloat y;
	mmFloat vx;
	mmFloat vy;
	
	mmSABBird *bird = (mmSABBird *)b;
	
	mmU32 img_id = mmSABObjectsParam[ANGRYBIRD_BIRD_BLUE].img_id;

	if(mmSABBlueBirds[0] != null || mmSABBlueBirds[1] != null)return;
	x = bird->body->p.x;
	y = bird->body->p.y;
	vx = bird->body->v.x;
	vy = bird->body->v.y;
	
	img_id = mmSABObjectsParam[ANGRYBIRD_BIRD_BLUE].img_id;
	
	for(i = 0; i < 2; i++){
		mmSABBodyData *udata = (mmSABBodyData *)i51AdeOsMalloc(sizeof(mmSABBodyData), __FILE__, __LINE__);
		body = mmSABCreateGameObject(ANGRYBIRD_BIRD_BLUE, x, y, 0, &img_id, &shape);
		//shape->group = 1;
		shape->layers= 1;
		body->p.x = (i == 0 ? x - 10 : x + 10);
		body->p.y = y;
		
		if(body->p.x < 20 + (mm_sab_raw_image[ANGRYBIRD_BIRD_BLUE].width >> 1))
			body->p.x = 20 + (mm_sab_raw_image[ANGRYBIRD_BIRD_BLUE].width >> 1);
	/*	
		body->v.x = (i == 0) ? (vx - 100) : (vx + 100);
		body->v.y = (i == 0) ? vy * 0.8 : vy * 1.2;
	*/
		body->v.x = (i == 0) ? (vx - 100) : (vx + 100);
		body->v.y = vy;
	
		udata->img_id = img_id;
		udata->hp = mmSABObjectsParam[ANGRYBIRD_BIRD_BLUE].hp;
		udata->type_id = ANGRYBIRD_BIRD_BLUE;
		body->data = udata;
		
		shape->body = body;//
		mmSABBlueBirds[i] = shape;
	}
}

static void 
mmSABBlackBirdSpecial(void* b){
	cpBody *body;
	cpShape *shape;
	mmSABBird *bird = (mmSABBird *)b;
	mmFloat x = bird->body->p.x;
	mmFloat y = bird->body->p.y;
	int i;
	
	mmU32 img_id = mmSABObjectsParam[ANGRYBIRD_BIRD_YELLOW].img_id;

	addEffectObject(bird->body->p.x, bird->body->p.y, IMG_ANGRYBIRD_BOMB_EFFECT0, IMG_ANGRYBIRD_BOMB_EFFECT0 + 0, 0);
	
	for(i = 0; i < 16; i++){
		body = mmSABCreateGameObject(ANGRYBIRD_BIRD_YELLOW, x, y, 0, &img_id, &shape);
		shape->group = 1;//2;
		body->p.x = x; body->p.y = y;
		
		switch(i)
		{
			case 0:
				body->v.x = 2000; body->v.y = 0;
			break;
			
			case 1:
				body->v.x = 2000; body->v.y = 0;
			break;
			
			case 2:
				body->v.x = 0; body->v.y = -2000;
			break;
			
			case 3:
				body->v.x = 0; body->v.y = -2000;
			break;
			
			case 4:
				body->v.x = 0; body->v.y = 2000;
			break;
			
			case 5:
				body->v.x = 0; body->v.y = 2000;
			break;
			
			case 6:
				body->v.x = -2000; body->v.y = 0;
			break;
			
			case 7:
				body->v.x = -2000; body->v.y = 0;
			break;
			
			case 8:
				body->v.x = 2000; body->v.y = -2000;
			break;
			
			case 9:
				body->v.x = 2000; body->v.y = -2000;
			break;
			
			case 10:
				body->v.x = 2000; body->v.y = -2000;
			break;
			
			case 11:
				body->v.x = 2000; body->v.y = -2000;
			break;
			
			case 12:
				body->v.x = -2000; body->v.y = -2000;
			break;
			
			case 13:
				body->v.x = -2000; body->v.y = -2000;
			break;
			
			case 14:
				body->v.x = -2000; body->v.y = 2000;
			break;
			
			case 15:
				body->v.x = -2000; body->v.y = 2000;
			break;
		}
		
		mmSABBlackBombs[i] = shape;
	}
	
	mmSABBomTimes = 1;
	
	mmSABDeleteBirdBody(bird);
	if(mmSABLastFlying->body == null )
		mmSABGameState = mmSABShowBirdDel;
	//mmSABGameState = mmSABShowScene;
	mmSABNumberOfBirds--;
}

static const mmU32 mmSABStageBirdInfors[] = {
	0x00000031, 0x00000012, 0x00000031, 0x00000012,
	0x0000014, 0x00000233, 0x00000011, 0x00000033,
	0x00000233, 0x00000011, 0x00000014, 0x00001144,
	0x00000044, 0x00003311, 0x00000211, 0x00000331,
	0x00000313, 0x00000033, 0x00000011, 0x00000014,
	0x000000114,
	/*0x00002222, 0x00000222, 0x00000022, 0x00000002,
	0x00000000, 0x00002222, 0x00004444, 0x00004444,
	0x00000133, 0x00000011, 0x00000414, 0x00000141,
	0x00000433, 0x00000113, 0x00004112, 0x00000331,
	0x00000313, 0x00000031, 0x00000311, 0x00001314,
	0x00000141,*/
};

mmS32 mmSABNumberOfBirds;

void mmSABBirdListAdd(mmSABBird* bird){
	mmSABBird* cur = mmSABBirdListHead;
	
	if(mmSABBirdListHead == null){
		mmSABBirdListHead = bird;
	}
	else
	{
		while(null != cur->next){
			cur = cur->next;
		}
		cur->next = bird;
	}
}

void mmSABBirdListDelete(){
	mmSABBird* cur = mmSABBirdListHead;
	while(mmSABBirdListHead)
	{
		cur = mmSABBirdListHead;
		mmSABBirdListHead = mmSABBirdListHead->next;
		mmFree(cur);
	}
	cur = null;
	mmSABNumberOfBirds = 0;
}
extern mmBool mmSABBlueCollision[2];
void mmSABBirdListUpdate(){
	mmSABBird* cur = mmSABBirdListHead;
	int i;
	while(cur)
	{
		//KscDbgPrint("\r\n ffffffffffff");
		mmSABBirdRefresh(cur);
		//KscDbgPrint("\r\n kkkkkkkkkkkk");
		cur = cur->next;
	}
	
	for(i= 0; i < 2; i++){
		if(mmSABBlueBirds[i])
		{
			mmBool effect_sign = 0, remove_sign = 0;
			if(cpBodyIsSleeping(mmSABBlueBirds[i]->body))
			{
				 effect_sign = 1;
				 remove_sign = 1;
			}
			else if(mmSABBlueBirds[i]->body->p.x < -40  || mmSABBlueBirds[i]->body->p.y < -40|| mmSABBlueBirds[i]->body->p.y > MMSAB_SCREEN_HEIGHT + 40)
			{
				 remove_sign = 1;
			}
			
			if(effect_sign)
			{
				addEffectObject(mmSABBlueBirds[i]->body->p.x, mmSABBlueBirds[i]->body->p.y, IMG_ANGRYBIRD_EFFECT, IMG_ANGRYBIRD_EFFECT + 2, 0);
			}
			
			if(remove_sign)
			{
				cpSpaceRemoveBody(mmSABSpace, mmSABBlueBirds[i]->body);
				cpSpaceRemoveShape(mmSABSpace,mmSABBlueBirds[i]);
				mmFree(mmSABBlueBirds[i]->body->data);
				mmSABBlueBirds[i]->body->data = null;
				cpBodyFree(mmSABBlueBirds[i]->body);
				mmSABBlueBirds[i]->body = null;
				cpShapeFree(mmSABBlueBirds[i]);
				mmSABBlueBirds[i] = null;
			//	mmSABBlueNumOfPathPoint[i] = 0;
				mmSABBlueCollision[0] = mmFalse;
				mmSABBlueCollision[1] = mmFalse;
			}
		}
	}
	//KscDbgPrint("\r\n qqqqqqqqqqqqqqqq");
	if(mmSABBomTimes != 0)
	{
		//if(mmSABBomTimes++ > 10)
		if(mmSABBomTimes++ > 2)
		{
			for( i = 0; i < 16; i++){
				if(mmSABBlackBombs[i])
				{
					cpSpaceRemoveBody(mmSABSpace, mmSABBlackBombs[i]->body);
					cpSpaceRemoveShape(mmSABSpace, mmSABBlackBombs[i]);
					cpBodyFree(mmSABBlackBombs[i]->body);
					mmSABBlackBombs[i]->body = null;
					cpShapeFree(mmSABBlackBombs[i]);
					mmSABBlackBombs[i] = null;
				}
			}
			mmSABBomTimes = 0;
		}
	}
}
void
mmSABInitBirds()
{
	int i;
	
	mmU32 type_id;
	mmS32 x, y, r1, r2;
	mmU32 img_id;
	mmSABBird* bird = null;
	mmSABBodyData *data = null;
	for(i = 0; i < 8; i++, mmSABNumberOfBirds++)
	{
		
		type_id = (mmSABStageBirdInfors[mmSABGameStageId] >> (i * 4)) & 0xf;

		if(!type_id--)break;
		
		bird = (mmSABBird*)i51AdeOsMalloc(sizeof(mmSABBird), __FILE__, __LINE__);
		data = (mmSABBodyData*)i51AdeOsMalloc(sizeof(mmSABBodyData), __FILE__, __LINE__);
		
		img_id = mmSABObjectsParam[type_id ].img_id;
		r1 = mm_sab_raw_image[img_id].width;
		r2 = mm_sab_raw_image[img_id].height;
		x = 20 + (r1 >> 1);
		y = 95 - (25) * mmSABNumberOfBirds;

		bird->body = mmSABCreateGameObject(type_id, x, y, 0, &img_id, &bird->shape);
		cpBodySleep(bird->body);
		
	//	bird->shape->group = 1;
		bird->shape->layers= 0;
		mmMemSet(data, 0, sizeof(mmSABBodyData));			
		bird->birdtype	= type_id;
		bird->state		= mmSABBirdState_IDLE;
		bird->aimstate	= mmSABBirdState_IDLE;
	
		switch(bird->birdtype){
		case ANGRYBIRD_BIRD_RED:
			bird->specialSkill	= mmSABRedBirdSpecial;
			break;
		case ANGRYBIRD_BIRD_BLUE:
			bird->specialSkill	= mmSABBlueBirdSpecial;
			break;
		case ANGRYBIRD_BIRD_YELLOW:
			bird->specialSkill	= mmSABYellowBirdSpecial;
			break;
		case ANGRYBIRD_BIRD_BLACK:
			bird->specialSkill	= mmSABBlackBirdSpecial;
			break;
		default:
			bird->specialSkill	= null;
			break;
		}
		bird->animid		= mmSABBirdAnimation_IDLE;
		bird->next			= null;
		bird->curframe		= 0;		
		data->img_id = img_id;
		data->type_id = bird->birdtype;//
		data->hp = 1;
		bird->body->data = data;
		mmSABBirdListAdd(bird);
	}
	
	mmSABBlueBirds[0] = null;
	mmSABBlueBirds[1] = null;
	
	for(i=0; i<16; i++)
		mmSABBlackBombs[i] = null;
}

























