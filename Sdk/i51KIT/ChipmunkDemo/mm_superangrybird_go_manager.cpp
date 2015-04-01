//#include "../marmota/marmota.h"
#include "inc/mm_superangrybird_resource.h"
#include "mm_superangrybird.h"
#include "marmota_math.h"

#define MMSAB_SCORE_MOVE_SPEED 6 //8

mmS32 mmSABNumberOfPigs;
mmBool mmSABCollision = mmFalse;//用来记录小鸟第一次撞击
mmBool mmSABBlueCollision[2] = {mmFalse, mmFalse};
mmFloat mmSABBirdRoundV;

extern void mm_sba_draw_image(int x, int y, int id, mmBool transparentSign);
extern void mmSABBirdListDelete();
const mmSABObjectType mmSABObjectsParam[] = {
	{MM_CIRCLE_SHAPE,	500,	0.5f, 1.0f, 	(mmU32)IMG_ANGRYBIRD_BIRD_RED,		0, 0xfff00001},
	{MM_CIRCLE_SHAPE,	500,	0.5f, 1.0f, 	(mmU32)IMG_ANGRYBIRD_BIRD_BLUE,		0, 0xfff00001},
	{MM_CIRCLE_SHAPE,	500,	0.1f, 1.0f, 	(mmU32)IMG_ANGRYBIRD_BIRD_YELLOW,	0, 0xfff00001},
	{MM_CIRCLE_SHAPE,	1000,	0.5f, 1.0f, 	(mmU32)IMG_ANGRYBIRD_BIRD_BLACK,	0, 0xfff00001},
	
	{MM_BOX_SHAPE, 		1000,	0.5f, 1.0f, 	(mmU32)IMG_ANGRYBIRD_PIG_SML,		1800, 0xffff0005},
	{MM_BOX_SHAPE, 		1000,	0.5f, 1.0f, 	(mmU32)IMG_ANGRYBIRD_PIG_BIG,		1800, 0xffff0005},
	{MM_BOX_SHAPE,		1000, 	0.5f, 1.0f,		(mmU32)IMG_ANGRYBIRD_MONKEY,		1800, 0xffff0005},//
	
	{MM_BOX_SHAPE,		1500,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_WOOD_MAX,		800, 0xfffff001},
	{MM_BOX_SHAPE,		1000,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_WOOD_MID,		600, 0xffffff05},
	{MM_BOX_SHAPE,		1000,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_WOOD_LIT,		400, 0xffffff52},
	
	{MM_BOX_SHAPE,		1000,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_ICE_MAX,		800, 0xfffff001},
	{MM_BOX_SHAPE,		800,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_ICE_MID,		600, 0xffffff05},
	{MM_BOX_SHAPE,		600,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_ICE_LIT,		400, 0xffffff52},

	{MM_BOX_SHAPE,		3500,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_STONE_MAX,		2000, 0xfffff001},
	{MM_BOX_SHAPE,		3000,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_STONE_MID,		1800, 0xffffff05},
	{MM_BOX_SHAPE,		2500,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_STONE_LIT,		1500, 0xffffff52},
	{MM_BOX_SHAPE,		2000,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_STONE_TIN,		1500, 0xffffff01},

	{MM_BOX_SHAPE,		2000,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_BIG_WOOD,		1000, 0xfffff001},
	{MM_BOX_SHAPE,		4000,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_BIG_STONE,		3000, 0xfffff001},
	
	{MM_TRIANGLE_SHAPE,	1500,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_STONE_TRI,		500, 0xfffff001},
	{MM_BOX_SHAPE,		1500,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_ICE_BOX,		600, 0xfffff001},
	/**TODO:到时删掉**/
	{MM_STATIC_SHAPE,	400,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_STONE_MAX,		80000, 0xffffff52},
	{MM_STATIC_SHAPE,	400,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_STONE_MID,		80000, 0xffffff52},
	{MM_STATIC_SHAPE,	400,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_STONE_LIT,		80000, 0xffffff52},
	/*****************/
	{MM_BOX_SHAPE,		800, 	0.5f, 1.0f,		(mmU32)IMG_ANGRYBIRD_LANTERN,		1, 0xffff0003},//灯笼,自动往上找东西来绑
	{MM_BOX_SHAPE,		800, 	0.5f, 1.0f,		(mmU32)IMG_ANGRYBIRD_LANTERN1,		1, 0xffff0003},//灯笼1，不会自动往上找东西来绑
	{MM_BOX_SHAPE,		800, 	0.5f, 1.0f,		(mmU32)IMG_ANGRYBIRD_MOONCAKE,		10, 0xffff0003},//
	{MM_BOX_SHAPE,		800, 	0.5f, 1.0f,		(mmU32)IMG_ANGRYBIRD_CAP,			1500, 0xffff0003},
	
	{MM_STATIC_SHAPE,	400,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_BARRIER_TIN,		80000, 0xffffff52},
	{MM_STATIC_SHAPE,	400,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_BARRIER_LIT,		80000, 0xffffff52},
	{MM_STATIC_SHAPE,	800,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_BARRIER_MIN,		80000, 0xffffff52},
	{MM_STATIC_SHAPE,	1000,	0.0f, 1.0f,		(mmU32)IMG_ANGRYBIRD_BARRIER_MAX,		80000, 0xffffff52},
	
	{MM_JOINTER, 		1000, 	0.0f, 0.0f, 	0, 0, 0},
	
	
};

cpSpace *mmSABSpace;
mmSABBird* mmSABOnlineBird;
mmSABBird* mmSABLastFlying;
mmSABEffectObjectLNode *mmSABEffectObjectListHead;
mmSABScoreObjectNode *mmSABScoreListHead;

extern cpShape* mmSABBlueBirds[2];

static void 
removeConstraint(cpConstraint *ptr, cpBody *body)
{
	/*if(ptr->a == body || ptr->b == body)
	{
		cpSpaceRemoveConstraint(mmSABSpace, ptr);
	}*/

	if(ptr->a == body || ptr->b == body)
	{
		cpSpaceRemoveConstraint(mmSABSpace, ptr);
		cpConstraintFree(ptr);
		ptr = NULL;
	}
}


static /*inline*/ mmU32
mmSABGetScoreImageId(mmU32 type_id){
	switch(type_id){
		case ANGRYBIRD_BIRD_RED:		return IMG_ANGRYBIRD_UI_SCORENUMBER_BIRD;
		case ANGRYBIRD_BIRD_BLUE:		return IMG_ANGRYBIRD_UI_SCORENUMBER_BIRD + 2;
		case ANGRYBIRD_BIRD_YELLOW:		return IMG_ANGRYBIRD_UI_SCORENUMBER_BIRD + 4;
		case ANGRYBIRD_BIRD_BLACK:		return IMG_ANGRYBIRD_UI_SCORENUMBER_BIRD + 6;
		case ANGRYBIRD_PIG:
		case ANGRYBIRD_MONKEY://
		case ANGRYBIRD_PIG_BIG:			return IMG_ANGRYBIRD_UI_SCORENUMBER_PIG; 
		
		case ANGRYBIRD_LANTERN:
		case ANGRYBIRD_LANTERN1:
		case ANGRYBIRD_MOONCAKE:
		case ANGRYBIRD_CAP:				return IMG_ANGRYBIRD_UI_SCORENUMBER_LANTERN;
		
		default:						return IMG_ANGRYBIRD_UI_SCORENUMBER_0;		
	}
}

static /*inline*/ mmU32
mmSABGetScore(mmU32 type_id){
	mmU32 num = mmSABObjectsParam[type_id].score;
	mmU32 power = 1;
	mmU32 score = 0;
	int i;
	for( i= 7; i >= 0; i--)
	{
		mmU32 temp = (num >> (4 * i)) & 0xf;
		if(temp == 0xf)continue;
		score += temp * power;
		power *= 10;
	}
	
	return score;
}

static mmSABBird* mmSABGetABirdOnline(){
	mmSABBird* cur = mmSABBirdListHead;
	
	while(cur){
		if(cur->state == mmSABBirdState_IDLE)break;
		cur = cur->next;
	}
	
	return cur;
}

void 
addEffectObject(mmS32 x, mmS32 y, mmU32 img_id, mmU32 max_img_id, mmFloat a){
	mmSABEffectObjectLNode *node = (mmSABEffectObjectLNode *)i51AdeOsMalloc(sizeof(mmSABEffectObjectLNode), __FILE__, __LINE__);
	node->img_id = img_id;
	node->max_img_id = max_img_id;
	node->p.x = x;
	node->p.y = y;
	node->a = a;
	node->next = null;
	
	if(null == mmSABEffectObjectListHead){
		mmSABEffectObjectListHead = node;
	}
	else{
		mmSABEffectObjectLNode *temp = mmSABEffectObjectListHead;
		while(null != temp->next){
			temp = temp->next;
		}
		temp->next = node;
	}
}

mmU32 mm_sab_get_stageobject_startpoint(int stageId)
{
	mmU32 sp = 0, i = 0;
	for(; i < stageId; i++)
	{
		sp += mmNumberOfStageObjects[i];
	}
	
	return sp;
}

//static inline mmSABG
cpBody* mmSABCreateGameObject(int typeId, mmFloat x, mmFloat y, mmFloat a, mmU32 *img_id, cpShape **s){
	cpBody *body;
	cpShape *shape;
	mmFloat m, e, u, r1, r2;
	
	//typeId = mmSABStageObjectsParam[i].objectType;
	m = mmSABObjectsParam[typeId].m;
	e = mmSABObjectsParam[typeId].e;
	u = mmSABObjectsParam[typeId].u;
	*img_id = mmSABObjectsParam[typeId].img_id;
	r1 = mm_sab_raw_image[*img_id].width;
	r2 = mm_sab_raw_image[*img_id].height;
	mmSBAFullScoreOfStage += mmSABGetScore(typeId);
	
	if(mmSABObjectsParam[typeId].shapetype == MM_CIRCLE_SHAPE){
		body = cpSpaceAddBody(mmSABSpace, cpBodyNew(m, cpMomentForCircle(m, cpfmax(r1, r2) / 2, 0.0f, cpvzero)));
		body->p = cpv(x, y);
		shape = cpSpaceAddShape(mmSABSpace, cpCircleShapeNew(body, cpfmax(r1, r2) / 2, cpvzero));
	}else if(mmSABObjectsParam[typeId].shapetype == MM_BOX_SHAPE){
		body = cpSpaceAddBody(mmSABSpace, cpBodyNew(m, cpMomentForBox(m, r1, r2)));
		body->p = cpv(x, y);
		shape = cpSpaceAddShape(mmSABSpace, cpBoxShapeNew(body, r1, r2));
	}else if(mmSABObjectsParam[typeId].shapetype == MM_TRIANGLE_SHAPE){
		body = cpSpaceAddBody(mmSABSpace, cpBodyNew(m, cpMomentForBox(m, r1, r2)));
		body->p = cpv(x, y);
		shape = cpSpaceAddShape(mmSABSpace, cpBoxShapeNew(body, r1, r2));
	}else if(mmSABObjectsParam[typeId].shapetype == MM_STATIC_SHAPE){
		body = cpSpaceAddBody(mmSABSpace, cpBodyNewStatic());
		body->p = cpv(x, y);
		shape = cpSpaceAddShape(mmSABSpace, cpBoxShapeNew(body, r1, r2));
	}
	shape->e = e; shape->u = u;
	
	cpBodySetAngle(body, a);
	if(typeId == ANGRYBIRD_BIRD_RED){
		shape->collision_type = ANGRYBIRD_COLLISION_BIRD;
	}
	else if(typeId == ANGRYBIRD_PIG || typeId == ANGRYBIRD_PIG_BIG || typeId == ANGRYBIRD_MONKEY){
		shape->collision_type = ANGRYBIRD_COLLISION_PIG;
	}
	else if(typeId >= ANGRYBIRD_WOOD_MAX){
		shape->collision_type = ANGRYBIRD_COLLISION_BARRIER;
	}
	
	if(s != null)*s = shape;
	
	if(typeId == ANGRYBIRD_PIG || typeId == ANGRYBIRD_PIG_BIG || typeId == ANGRYBIRD_MONKEY)mmSABNumberOfPigs++;
	
	return body;
}

static cpBody*
mmSABCreateBarrierObject(int typeId, mmFloat x, mmFloat y, mmFloat a){
	mmU32 img_id;
	mmSABBodyData *udata;
	cpBody *body = mmSABCreateGameObject(typeId, x, y, a, &img_id, null);
	
	if(mmSABObjectsParam[typeId].shapetype != MM_STATIC_SHAPE)
	cpBodySleep(body);
	
	udata = (mmSABBodyData *)i51AdeOsMalloc(sizeof(mmSABBodyData), __FILE__, __LINE__);
	udata->img_id = img_id;
	udata->hp = mmSABObjectsParam[typeId].hp;
	udata->type_id = typeId;

	body->data = (void*)udata;

	body->a = a;
	return body;
}

extern mmS32 mmSABIsEffectPoint;
mmBool mmSABCanOccurSpecial;
void mm_sab_setangrybird_v(mmFloat x, mmFloat y){
	
	//cpBodyActivate(mmSABBirdListHead->body);
	cpBodyActivate(mmSABOnlineBird->body);
	mmSABOnlineBird->shape->layers = 1;
	mmSABOnlineBird->aimstate = mmSABBirdState_FLYING;
	mmSABOnlineBird->body->v.x = (BIRD_STARTPOSITION.x - x) * 16;
	mmSABOnlineBird->body->v.y = (BIRD_STARTPOSITION.y - y) * 16;
	mmSABLastFlying = mmSABOnlineBird;
	mmSABIsOccurSpecialTech = mmFalse;
	mmSABCanOccurSpecial = mmTrue;
	mmSABIsEffectPoint = 0;
	
	mmSABOnlineBird = mmSABGetABirdOnline();
	if(mmSABOnlineBird)
	{
		mmSABOnlineBird->aimstate = mmSABBirdState_JUMP;
//		mmSABBirdRoundV = MM_PI / (BIRD_STARTPOSITION.y - mmSABOnlineBird->body->p.y);
		mmSABBirdRoundV = (MM_PI / (60 - mmSABOnlineBird->body->p.y));
		mmSABBirdRoundV = mmSABBirdRoundV > 0 ? -mmSABBirdRoundV : mmSABBirdRoundV;
	}
}

void mm_sab_initgameobjects(int stageId)
{
	/*int sp = mm_sab_get_stageobject_startpoint(stageId);
	int max = sp + mmNumberOfStageObjects[stageId];
	int i = sp;
	
	for(; i < max; i++)
	{
		int typeId = mmSABStageObjectsParam[i].objectType;
		mmSABCreateBarrierObject(typeId,
			mmSABStageObjectsParam[i].pos.x,
			mmSABStageObjectsParam[i].pos.y,
			mmSABStageObjectsParam[i].a);
	}*/
	
	/*int sp = mm_sab_get_stageobject_startpoint(stageId);
	int max = sp + mmNumberOfStageObjects[stageId];
	int i = sp;
	int isNeedJoint = 0;
	cpBody *joint0, *joint1;
	
	for(; i < max; i++)
	{
		if(!isNeedJoint)
		{		
			if(mmSABStageObjectsParam[i].objectType == ANGRYBIRD_JOINT)
			{
				isNeedJoint = 1;
			}
			else
			{
				mmSABCreateBarrierObject(
					mmSABStageObjectsParam[i].objectType,
					mmSABStageObjectsParam[i].pos.x,
					mmSABStageObjectsParam[i].pos.y,
					mmSABStageObjectsParam[i].a
				);
			}
		}
		else
		{
			cpVect p0, p1;
			joint0 = mmSABCreateBarrierObject(
				mmSABStageObjectsParam[i].objectType,
				mmSABStageObjectsParam[i].pos.x,
				mmSABStageObjectsParam[i].pos.y,
				mmSABStageObjectsParam[i].a
			);
			i++;
			joint1 = mmSABCreateBarrierObject(
				mmSABStageObjectsParam[i].objectType,
				mmSABStageObjectsParam[i].pos.x,
				mmSABStageObjectsParam[i].pos.y,
				mmSABStageObjectsParam[i].a
			);
			
			p0.x = mmSABStageObjectsParam[i-2].pos.x;
			p0.y = mmSABStageObjectsParam[i-2].pos.y;
			p1.x = mmSABStageObjectsParam[i].pos.x;
			p1.y = mmSABStageObjectsParam[i].pos.y;
			cpSpaceAddConstraint(mmSABSpace, cpPinJointNew(joint0, joint1, p0, cpvzero));
			
			cpBodySleep(joint0);
			cpBodySleep(joint1);
			
			isNeedJoint = 0;

		}
	}*/

	int sp = mm_sab_get_stageobject_startpoint(stageId);
	int max = sp + mmNumberOfStageObjects[stageId];
	int i = sp;
	cpBody *lastBody = null;

	for(; i < max; i++)
	{
		//int typeId = mmSABStageObjectsParam[i].objectType;
		
		cpBody *currentBody;
		currentBody = mmSABCreateBarrierObject(
			mmSABStageObjectsParam[i].objectType,
			mmSABStageObjectsParam[i].pos.x,
			mmSABStageObjectsParam[i].pos.y,
			mmSABStageObjectsParam[i].a);
			
		if(mmSABStageObjectsParam[i].objectType == ANGRYBIRD_LANTERN)
		{
			if(lastBody)
			{
				cpSpaceAddConstraint(mmSABSpace, cpPinJointNew(lastBody, currentBody, cpvzero, cpvzero));
				
				cpBodySleep(lastBody); 
				cpBodySleep(currentBody);
			}
		}
		
		lastBody = currentBody;
	}
}

void
mmSABAddScoreObject(mmS32 x, mmS32 y, mmS32 type_id)
{
	mmSABScoreObjectNode *node = (mmSABScoreObjectNode *)i51AdeOsMalloc(sizeof(mmSABScoreObjectNode), __FILE__, __LINE__);
	
	node->type_id =  type_id;
	node->pos.y = y - 10;		node->pos.x = x;
	node->end_pos.y = node->pos.y;	node->end_pos.x = x + 32;
	node->next = null;
	
	if(mmSABScoreListHead == null){
		mmSABScoreListHead = node;
	}
	else{
		mmSABScoreObjectNode *temp = mmSABScoreListHead;
		while(temp->next){
			temp = temp->next;
		}
		temp->next = node;
	}
}

mmS32 mmSabSoundPlayTime = 0;
static void
postStepRemove(cpSpace *space, cpShape *shape, void *unused){
	cpBody* body;
	mmSABBodyData *data;
	mmU32 img_id = 0;
	mmU32 flag = 0;
	if(!shape)return;
	body = shape->body;
	data = 	(mmSABBodyData *)body->data;
	switch(data->type_id){
		case ANGRYBIRD_WOOD_MAX:
		case ANGRYBIRD_WOOD_MID:
		case ANGRYBIRD_WOOD_LIT:
			/*if(1 == mmSABSound)
			{
				marmota->audio->stopAllAudio();
				marmota->audio->playRawData((mmAudioData*)sab_wood_break, 0);
			}*/
			img_id = IMG_ANGRYBIRD_EFFECT + 3;
		break;
		
		case ANGRYBIRD_ICE_MAX:
		case ANGRYBIRD_ICE_MID:
		case ANGRYBIRD_ICE_LIT:
			/*if(1 == mmSABSound)
			{
				marmota->audio->stopAllAudio();
				marmota->audio->playRawData((mmAudioData*)sab_ice_break, 0);
			}*/
			img_id = IMG_ANGRYBIRD_EFFECT + 6;
		break;
		
		case ANGRYBIRD_STONE_MAX:
		case ANGRYBIRD_STONE_MID:
		case ANGRYBIRD_STONE_LIT:
			/*if(1 == mmSABSound)
			{
				marmota->audio->stopAllAudio();
				marmota->audio->playRawData((mmAudioData*)sab_stone_break, 0);
			}*/
			flag = 1;
			break;
		
		case ANGRYBIRD_PIG:
		case ANGRYBIRD_PIG_BIG:
		case ANGRYBIRD_MONKEY:
			/*if(1 == mmSABSound)
			{
				marmota->audio->stopAllAudio();			
				marmota->audio->playRawData((mmAudioData*)sab_pig_break, 0);
			}*/
			img_id = IMG_ANGRYBIRD_EFFECT + 0;
			mmSABNumberOfPigs--;
			data->hp = 0;
		break;
		default: flag = 1;
		break;
			
	}
	mmSBAScoreOfStage += mmSABGetScore(data->type_id);
	mmSABAddScoreObject((mmS32)body->p.x, (mmS32)body->p.y, data->type_id);
	
	if(0 == flag)
		addEffectObject((mmS32)body->p.x, (mmS32)body->p.y, img_id, img_id + 2, body->a);
		
	mmFree(body->data);
	body->data = null;
	
	cpArrayEach(mmSABSpace->constraints, (cpArrayIter)&removeConstraint, body);
	
	cpSpaceRemoveBody(mmSABSpace, body);
	cpSpaceRemoveShape(mmSABSpace, shape);
	cpBodyFree(body);
	cpShapeFree(shape);
	body = null;
	shape = null;
}


//鸟撞猪
static mmBool
mmCollisionHandlerBiPi(cpArbiter *arb, cpSpace *space, void *ignore)
{
	CP_ARBITER_GET_SHAPES(arb, a, b);
	CP_ARBITER_GET_BODIES(arb, c, d);
	
	/*mmSABBodyData *cdata;
	cdata = (mmSABBodyData *)c->data;
	if(cdata->type_id == ANGRYBIRD_RED_BIRD)
	{
		addEffectObject((mmS32)c->p.x, (mmS32)c->p.y, IMG_ANGRYBIRD_RED_BIRD_EFFECT, IMG_ANGRYBIRD_RED_BIRD_EFFECT + 3, c->a);
	}*/
	
	if(a == mmSABBlueBirds[0])
		mmSABBlueCollision[0] = mmTrue;
	if(a == mmSABBlueBirds[1]) 
		mmSABBlueCollision[1] = mmTrue;
	if(a == mmSABLastFlying->shape)
		mmSABCollision = mmTrue;
	cpSpaceAddPostStepCallback(mmSABSpace, (cpPostStepFunc)postStepRemove, b, NULL);
	return mmTrue;
}


//鸟撞地板
static mmBool
mmCollisionHandlerBiGr(cpArbiter *arb, cpSpace *space, void *ignore)
{
	cpShape *a, *b; 
	cpBody *c, *d;
	cpArbiterGetShapes(arb, &a, &b);
	cpArbiterGetBodies(arb, &c, &d);
	
	/*mmSABBodyData *cdata;
	cdata = (mmSABBodyData *)c->data;
	
	if(cdata->type_id == ANGRYBIRD_RED_BIRD)
	{
		addEffectObject((mmS32)c->p.x, (mmS32)c->p.y, IMG_ANGRYBIRD_RED_BIRD_EFFECT, IMG_ANGRYBIRD_RED_BIRD_EFFECT + 3, c->a);
	}*/
	
	
	//c->t /= 32;
	if(a == mmSABBlueBirds[0])
		mmSABBlueCollision[0] = mmTrue;
	if(a == mmSABBlueBirds[1]) 
		mmSABBlueCollision[1] = mmTrue;
	if(a == mmSABLastFlying->shape)
		mmSABCollision = mmTrue;
//	mmSBAIsWatching = mmFalse;
	return mmTrue;
}

static mmBool
mmCollisionHandlerBiGr2(cpArbiter *arb, cpSpace *space, void *ignore)
{
	cpShape *a, *b; 
	cpBody *c, *d;
	cpArbiterGetShapes(arb, &a, &b);
	cpArbiterGetBodies(arb, &c, &d);
	
	if(a == mmSABBlueBirds[0])
		mmSABBlueCollision[0] = mmTrue;
	if(a == mmSABBlueBirds[1]) 
		mmSABBlueCollision[1] = mmTrue;
	if(a == mmSABLastFlying->shape)
		mmSABCollision = mmTrue;
	c->v = cpvmult(c->v, 0.95f);

	return mmTrue;
}
//障碍物撞猪或鸟撞障碍物
static mmBool
mmCollisionHandlerBaPiOrBiBa(cpArbiter *arb, cpSpace *space, void *ignore)
{
	mmS32 momentum;
	mmU32 i = 0;
	mmU32 img_id = 0;
	mmSABBodyData *ddata, *cdata;
	cpShape *a, *b; 
	cpBody *c, *d;
	#ifdef zq
	CP_ARBITER_GET_SHAPES(arb, a, b);
	#else
	cpArbiterGetShapes(arb, &a, &b);
	#endif
	
	
	
	c = a->body;
	d = b->body;
	
	cdata = (mmSABBodyData *)c->data;
	if(ANGRYBIRD_BIRD_RED <= cdata->type_id && cdata->type_id <= ANGRYBIRD_BIRD_BLACK)
	{
		if(a == mmSABLastFlying->shape)
		mmSABCollision = mmTrue;
	}
		
	if(a == mmSABBlueBirds[0])
		mmSABBlueCollision[0] = mmTrue;
	if(a == mmSABBlueBirds[1]) 
		mmSABBlueCollision[1] = mmTrue;
	momentum = ((mmS32)(mmSqrtf((mmS32)(c->v.x) * (mmS32)c->v.x + (mmS32)c->v.y * (mmS32)c->v.y) * c->m)) >> 9;

	ddata = (mmSABBodyData *)d->data;
	
	if(mmSABObjectsParam[ddata->type_id].shapetype != MM_STATIC_SHAPE)
		ddata->hp -= momentum;
	if(ddata->hp > 0)
	{
		if(ddata->hp * 2 <= mmSABObjectsParam[ddata->type_id].hp)
			i = 1;
		if(ddata->type_id == ANGRYBIRD_LANTERN || ddata->type_id == ANGRYBIRD_LANTERN1 ||
			 ddata->type_id == ANGRYBIRD_MONKEY || ddata->type_id == ANGRYBIRD_MOONCAKE ||
			 ddata->type_id == ANGRYBIRD_CAP)
			i = 0; 
		ddata->img_id = mmSABObjectsParam[ddata->type_id].img_id  + i;
	}
	else
	{
		cpSpaceAddPostStepCallback(mmSABSpace, (cpPostStepFunc)postStepRemove, b, NULL);
	}
	
	//ddata->hp -= momentum;
	return mmTrue;
}

//猪撞地板或障碍物撞地板
static mmBool
mmCollisionHandlerPiGrOrBaGr(cpArbiter *arb, cpSpace *space, void *ignore){
	mmS32 momentum;//动量
	mmU32 i = 0;
	cpShape *a, *b;
	cpBody *c;
	mmSABBodyData  *cdata;
	
	cpArbiterGetShapes(arb, &a, &b);
//	cpArbiterGetBodies(arb, &c, &d);
	
	c = a->body;

	momentum = ((mmS32)(mmSqrtf((mmS32)(c->v.x) * (mmS32)c->v.x + (mmS32)c->v.y * (mmS32)c->v.y) * c->m)) >> 9;
	
	cdata = (mmSABBodyData *)c->data;

	if(mmSABObjectsParam[cdata->type_id].shapetype != MM_STATIC_SHAPE)
		cdata->hp -= momentum * 0.2;
	
	if(cdata->hp > 0)
	{
		if(cdata->hp * 2 <= mmSABObjectsParam[cdata->type_id].hp)
			i = 1;
		if(cdata->type_id == ANGRYBIRD_LANTERN || cdata->type_id == ANGRYBIRD_LANTERN1 ||
			cdata->type_id == ANGRYBIRD_MONKEY || cdata->type_id == ANGRYBIRD_MOONCAKE ||
			cdata->type_id == ANGRYBIRD_CAP)
			i = 0; 
		cdata->img_id = mmSABObjectsParam[cdata->type_id].img_id  + i;
	}
	else
	{	
		cpSpaceAddPostStepCallback(mmSABSpace, (cpPostStepFunc)postStepRemove, a, NULL);
	}
	
	return mmTrue;
}


/***********/

static mmBool
mmCollisionHandlerBegin(cpArbiter *arb, cpSpace *space, void *ignore)
{
	mmSABBodyData *cdata;
	mmSABBodyData *ddata;
#ifdef zq
	CP_ARBITER_GET_SHAPES(arb, a, b);
	CP_ARBITER_GET_BODIES(arb, c, d);
#else
	cpShape *a, *b; 
	cpBody *c, *d;
	cpArbiterGetShapes(arb, &a, &b);
	cpArbiterGetBodies(arb, &c, &d);

#endif
	
	cdata = 	(mmSABBodyData *)c->data;
	ddata = 	(mmSABBodyData *)d->data;
		
	return mmTrue;
}
extern void mmSABInitBirds();

static void
mmSBAInitGround(){
	cpShape *ground;
	int i;
	for( i = mmGroundPointStart; i < mmGroundPointStart + mmSABNumberOfGroundPoint[mmSABGameStageId] - 1; i++)
	{
		ground = cpSegmentShapeNew(&mmSABSpace->staticBody,
		 cpv(mmSABGroundPoints[i].x, mmSABGroundPoints[i].y), 
		 cpv(mmSABGroundPoints[i + 1].x, mmSABGroundPoints[i + 1].y), 0.0f);
		ground->e = 0.5f; ground->u = 1.0f;
		ground->collision_type = ANGRYBIRD_COLLISION_GROUND;
		cpSpaceAddShape(mmSABSpace, ground);
	}
}

void mm_sab_initspace(int stageId)
{
	mmSABEffectObjectListHead = null;
	mmSABScoreListHead = null;
	
	/// init physics engine
//	mmADMMemoryInit();
	cpInitChipmunk();

	cpResetShapeIdCounter();
	
	/// init space.
	mmSABSpace = cpSpaceNew();
	
	mmSABSpace->iterations = 1;
	//mmSABSpace->elasticIterations = 10;
	
	mmSABSpace->gravity = cpv(-1000, 0);
	
	mmSABSpace->sleepTimeThreshold = 0.2f;//0.2f;
	
	//cpSpaceResizeStaticHash(mmSABSpace, 30.0f, 1000);
	//cpSpaceResizeActiveHash(mmSABSpace, 30.0f, 1000);
	
	
	mmSBAInitGround();
	
	mm_sab_initgameobjects(stageId);

	mmSABInitBirds();


	mmSABOnlineBird = mmSABGetABirdOnline();
	
	if(mmSABOnlineBird != null)
		mmSABOnlineBird->aimstate = mmSABBirdState_JUMP;

	//cpSpaceAddCollisionHandler(mmSABSpace, ANGRYBIRD_COLLISION_BIRD,	ANGRYBIRD_COLLISION_PIG,		mmCollisionHandlerBegin, NULL, NULL, NULL, NULL);
	cpSpaceAddCollisionHandler(mmSABSpace, ANGRYBIRD_COLLISION_BIRD,	ANGRYBIRD_COLLISION_BARRIER,	mmCollisionHandlerBaPiOrBiBa, NULL, NULL, NULL, NULL);
	//cpSpaceAddCollisionHandler(mmSABSpace, ANGRYBIRD_COLLISION_BIRD,	ANGRYBIRD_COLLISION_GROUND,		mmCollisionHandlerBiGr, NULL, NULL, NULL, NULL);
	cpSpaceAddCollisionHandler(mmSABSpace, ANGRYBIRD_COLLISION_BIRD,	ANGRYBIRD_COLLISION_GROUND,		NULL, mmCollisionHandlerBiGr2, NULL, NULL, NULL);
	cpSpaceAddCollisionHandler(mmSABSpace, ANGRYBIRD_COLLISION_BIRD,	ANGRYBIRD_COLLISION_PIG,		mmCollisionHandlerBiPi, NULL, NULL, NULL, NULL);
	cpSpaceAddCollisionHandler(mmSABSpace, ANGRYBIRD_COLLISION_BARRIER,	ANGRYBIRD_COLLISION_PIG,		mmCollisionHandlerBaPiOrBiBa, NULL, NULL, NULL, NULL);
	cpSpaceAddCollisionHandler(mmSABSpace, ANGRYBIRD_COLLISION_PIG,		ANGRYBIRD_COLLISION_GROUND,		mmCollisionHandlerPiGrOrBaGr, NULL, NULL, NULL, NULL);
	cpSpaceAddCollisionHandler(mmSABSpace, ANGRYBIRD_COLLISION_BARRIER,	ANGRYBIRD_COLLISION_GROUND,		mmCollisionHandlerPiGrOrBaGr, NULL, NULL, NULL, NULL);
}

static void releaseEachBody(cpBody *body, void *unused)
{
	if(body->data != null)
	{
		mmFree(body->data);
		body->data = null;
	}
}

extern void mmSABBirdListDelete();
void mm_sab_destoryspace()
{
	mmSABBirdListDelete();

	if(mmSABEffectObjectListHead != null){
		mmSABEffectObjectLNode *node;
		while(mmSABEffectObjectListHead){
			node = mmSABEffectObjectListHead;
			mmSABEffectObjectListHead = mmSABEffectObjectListHead->next;
			mmFree(node);
		}
	}

	if(mmSABScoreListHead != null){

		mmSABScoreObjectNode *node = null;
		while(mmSABScoreListHead){
			node = mmSABScoreListHead;
			mmSABScoreListHead = mmSABScoreListHead->next;
			mmFree(node);
		}
	}
	cpSpaceEachBody(mmSABSpace, &releaseEachBody, NULL);
	cpSpaceFreeChildren(mmSABSpace);
	cpSpaceFree(mmSABSpace);
	extern void cpCollideFree(void);
	cpCollideFree();
}

static /*inline*/ void
mm_sba_rorate_image(int x, int y, int id, mmFloat a)
{
	if(id == IMG_ANGRYBIRD_BIRD_YELLOW  || id == IMG_ANGRYBIRD_BIRD_YELLOW+1)
		{
			mmS32 k;
			k = -1000*a;
			k = k % 6282;
			if(k<0)
				k+= 6282;
			
			if(k>(3141>>1)  &&  k<(3141>>1)+3141)
				{
					x -= 4;
				}
		}
		/*mmRotateImageBuffer(
		mmSABSceneCanva, 
		(int)mm_sab_raw_image[id].x + ((int)mm_sab_raw_image[id].width + 1) / 2, 
		(int)mm_sab_raw_image[id].y + ((int)mm_sab_raw_image[id].height + 1) / 2, 
		(int)mm_sab_raw_image[id].width,
		(int)mm_sab_raw_image[id].height,  
		mmSABBaseCanva, 
		x, y, -a
	);*/
	//添加旋转接口
	{
		iS32 angle;
		iS32 dst_x;
		iS32 dst_y;
		iRECT srcRect;

		angle = a*180/__MM_PI__;
		dst_x = x - ((iS32)mm_sab_raw_image[id].width + 1) / 2;
		dst_y = y - ((iS32)mm_sab_raw_image[id].height + 1) / 2;
		pfunc->public_build_rect(&srcRect, (iS16)mm_sab_raw_image[id].x, (iS16)mm_sab_raw_image[id].y, (iS16)mm_sab_raw_image[id].width, (iS16)mm_sab_raw_image[id].height);
		i51KitG2RotateImage(AngryBird.images[ANGRYBIRD_SCENE_CANVA].pContainer, AngryBird.images[ANGRYBIRD_SCENE_CANVA].id, iNULL, &srcRect, dst_x, dst_y, x, y, 0, angle);
	}
}

extern void mm_sba_draw_image(int x, int y, int id, mmBool transparentSign);
void mmDrawImageBuffer(int x, int y, int id, mmFloat a){
	if((mm_sab_raw_image[id].width < 16 && mm_sab_raw_image[id].height < 16))
	{
		mm_sba_draw_image(x -  ((int)mm_sab_raw_image[id].width + 1) / 2, y -  ((int)mm_sab_raw_image[id].height + 1) / 2, id , mmTrue);
	}
	else
	{
		mm_sba_rorate_image(x, y, id, a);
	}
}

static void
eachBody(cpBody *body, void *unused){
	int i = 0;
	if(body->data != null)
	{
		mmSABBodyData *udata = 	(mmSABBodyData *)body->data;
		//mmDrawImageBuffer((int)(body->p.x), (int)(body->p.y - cameraOffsetY), (int)(udata->img_id), body->a);
		
		//MM_DBG_PRINT("\r\n udata->type_id = %d",udata->type_id);
		
		if(udata->type_id == ANGRYBIRD_PIG && udata->hp == mmSABObjectsParam[ANGRYBIRD_PIG].hp)
		{
			mmU32 rannumber = rand()%20;
			if(rannumber >= 19)
				i = 1;
			else
				i = 0;
		}
		
		if(body->a == 0)
			mm_sba_draw_image((int)(body->p.x) -  ((int)mm_sab_raw_image[(int)(udata->img_id)].width + 1) / 2, (int)(body->p.y - cameraOffsetY) -  ((int)mm_sab_raw_image[(int)(udata->img_id)].height + 1) / 2, (int)(udata->img_id + i) , mmTrue);
		else
			mm_sba_rorate_image((int)(body->p.x), (int)(body->p.y - cameraOffsetY), (int)(udata->img_id + i), body->a);
		if((udata->type_id == ANGRYBIRD_PIG || ANGRYBIRD_PIG_BIG == udata->type_id || ANGRYBIRD_MONKEY == udata->type_id) 
		&& (body->p.x < 0 || body->p.y > MMSAB_SCREEN_HEIGHT) && (udata->hp > 0)){
			udata->hp = 0;
			mmSABNumberOfPigs--;
		}
		
	}
}

/****************************************************************
*Function:
*	updateEffectList
*Descript:
*	update effectlist
*Parameter:
*	void
*Return:
*	void
*****************************************************************/
static void
updateEffectList()
{
	mmSABEffectObjectLNode *cur = mmSABEffectObjectListHead, *temp;
	
	while(cur){
		temp = cur->next;
		//mmDrawImageBuffer((int)(cur->p.x), (int)(cur->p.y - cameraOffsetY), cur->img_id, 0);
		mm_sba_draw_image((int)(cur->p.x) -  ((int)mm_sab_raw_image[cur->img_id].width + 1) / 2, (int)(cur->p.y - cameraOffsetY) -  ((int)mm_sab_raw_image[cur->img_id].height + 1) / 2,  cur->img_id , mmTrue);
		if(++cur->img_id >= cur->max_img_id){
			if(cur == mmSABEffectObjectListHead){
				mmSABEffectObjectListHead = mmSABEffectObjectListHead->next;
				mmFree(cur);
			}
			
		}
		cur = temp;
	}
}

extern void mmSABBirdListUpdate();
extern void mmSABDrawUI(int x, int y, int id);
extern void mmSABDrawScoreNumber(int x, int y, mmU32 score, int img_id);
extern void mmSABDrawBirdOrPigNumber(int x, int y, mmU32 score, int img_id, mmS32 type_id);

void mmSABUpdateScoreList()
{
	int i;
	mmSABScoreObjectNode *node = mmSABScoreListHead;
	while(node){
		mmSABScoreObjectNode *temp = node->next;
		mmU32 score = mmSABObjectsParam[node->type_id].score;
		mmU32 score_number;
		mmU32 img_id = mmSABGetScoreImageId(node->type_id);
		mmU32 score_width = 16;
		for( i = 0; i < 8; i++){
			score_number = (score >> (i * 4)) & 0xf;
			if(score_number == 0xf)break;
		
			if( (node->type_id >= ANGRYBIRD_BIRD_RED && node->type_id <= ANGRYBIRD_MONKEY) ||
				(node->type_id >= ANGRYBIRD_LANTERN && node->type_id <= ANGRYBIRD_CAP) ){
				score_number = (score_number == 0) ? 0 : 1;
				score_width = 15;
			}
			mmSABDrawUI(node->pos.x, node->pos.y + i * score_width - cameraOffsetY, img_id + score_number);
		}
		node->pos.x += MMSAB_SCORE_MOVE_SPEED;

		if(node->pos.x >= node->end_pos.x){
			if(node == mmSABScoreListHead){
				mmSABScoreListHead = mmSABScoreListHead->next;
				mmFree(node);
			}
			
		}
		node = temp;
	}
}

void
mm_sab_runspace()
{
	for(int i = 0;i < 3; i++)
		cpSpaceStep(mmSABSpace, 0.0086f);//mtk35平台
	//	cpSpaceStep(mmSABSpace, 0.007f);//mtk36平台
	//cpSpaceStep(mmSABSpace, 0.05f);
	
	cpSpaceEachBody(mmSABSpace, &eachBody, NULL);
	
/*	if(mmSABScoreListHead != null)
	{
		mmSABUpdateScoreList();
	}
	*/
	if(mmSABEffectObjectListHead != null)
	{
		updateEffectList();
	}
	mmSABBirdListUpdate();
}
