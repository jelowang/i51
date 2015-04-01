//#include "../marmota/marmota.h"
#include "inc/mm_superangrybird_resource.h"
#include "mm_superangrybird.h"
#include "marmota_math.h"


extern mmSABirdFly* goHead;
extern void mmSABZoomBird(mmS32 x , mmS32 y, float zoom, mmS32 id);
void mmSABListInsert(mmSABirdFly* node){
		
	//	assert(NULL != node);
		
		mmSABirdFly *temp = goHead;
		mmSABirdFly *tempNext = NULL;
		
		if(NULL == goHead){
				goHead = node;
				return;
		}
		/*TODO:再此鸟身覆盖处理**/
		if( (goHead->pos.x + mm_sab_mainmenu_image[goHead->type_id].width > node->pos.y + mm_sab_mainmenu_image[node->type_id].width) ){
			node->next = goHead;
			goHead = node;
			return;
		}
		/************************/
		while(1){
			tempNext = temp->next;
			if(NULL == tempNext){
					temp->next = node;
					break;
			}	
			else{
					if( (goHead->pos.x + mm_sab_mainmenu_image[goHead->type_id].width >node->pos.y + mm_sab_mainmenu_image[node->type_id].width) ){
							node->next = tempNext;
							temp->next = node;
							break;
					}
			}
			temp = temp->next;
		}
	
	}
	
//const mmU32 mmSABirdFlyNum = 0x01020304;

void mmSABListInit(){
		
	int i,k = 0;
	mmSABirdFly *temp = NULL;
	goHead = NULL;
	
	#ifdef MAMOTA_BACKEND_MSTAR
		srand(0);
	#endif
	
	for(i=0; i<6; i++){
	//	mmU8 ch = (mmSABirdFlyNum >> (8 * i)) & 0xff;
	//	for(j=0; j<ch; j++){
				k = i;
				if(i > 3)
					k = rand() % 4;
				temp = (mmSABirdFly*)i51AdeOsMalloc(sizeof(mmSABirdFly), __FILE__, __LINE__);
				mmMemSet(temp, 0, sizeof(mmSABirdFly));
				if(0 == k){
					temp->type_id = 0;
				}
				else if(1 == k){
					temp->type_id = 1;	
				}
				else if(2 == k){
					temp->type_id = 2;	
				}
				else{
					temp->type_id = 3;	
				}
			
			temp->pos.y = rand()% 480;
			if(0 == temp->pos.y)
				temp->pos.x = rand()% 100;
			else
				temp->pos.x = 0;
		
			temp->v_x = rand() % 20 + 10;
			//temp->v_x = (0 == temp->v_x) ? 15 : temp->v_x;
			
			temp->v_y = rand() % 10 + 10;
			//temp->v_y = (0 == temp->v_y) ? 10 : temp->v_y;
			
		//	temp->g = 2;
			
			mmSABListInsert(temp);
//		}	
	}
	
}

#ifdef MM_SUPERANGRYBIRD_RESOURCE_320X480
void mmSABListUpdate(mmSABirdFly* obj){
		obj->pos.x += obj->v_x;
		obj->pos.y += obj->v_y;
		obj->v_x -= 2;//obj->g;
		
		if(obj->pos.x > 160){
			obj->v_x -= 5;
			if(obj->pos.x > 200)
				obj->v_x = -obj->v_x > 0 ? obj->v_x : -obj->v_x;
		}
		
		if(obj->pos.x < -200 || obj->pos.y > 840){
			obj->pos.y = rand()% 640;
			if(0 == obj->pos.y)
				obj->pos.x = rand()% 150;
			else if(480 < obj->pos.y){
				obj->pos.x = rand()% 150;
				obj->pos.y = 0;
			}
				
			else
				obj->pos.x = 0;
		
			obj->v_x = rand() % 20 + 10;
			
			obj->v_y = rand() % 10 + 10;
			
			obj->zoom = (rand()%8 + 3)*0.1;
			obj->type_id = rand() % 4;
			//obj->g = 2;
		}
		
		/*if(obj->type_id == ANGRYBIRD_MAINMENU_RED_BIRD)
			mm_sab_mainmenu_draw_image(obj->pos.x,obj->pos.y,obj->type_id,mmTrue);
		else*/
			mmSABZoomBird(obj->pos.x,obj->pos.y,obj->zoom,obj->type_id);
}
#else
void mmSABListUpdate(mmSABirdFly* obj){

		//mm_sab_mainmenu_draw_image(obj->pos.x,obj->pos.y,obj->type_id,mmTrue);
		
		obj->pos.x += obj->v_x;
		obj->pos.y += obj->v_y;
		obj->v_x -= 2;//obj->g;
		
		if(obj->pos.x > 100){
			obj->v_x -= 5;
			if(obj->pos.x > 150)
				obj->v_x = -obj->v_x > 0 ? obj->v_x : -obj->v_x;
		}
		
		if(obj->pos.x < -200 || obj->pos.y > 840){
			obj->pos.y = rand()% 320;
			if(0 == obj->pos.y)
				obj->pos.x = rand()% 100;
			else if(200 < obj->pos.y){
				obj->pos.x = rand()% 100;
				obj->pos.y = 0;
			}
				
			else
				obj->pos.x = 0;
		
			obj->v_x = rand() % 20 + 10;
			
			obj->v_y = rand() % 10 + 10;
			obj->zoom = (rand()%8 + 3)*0.1;
			obj->type_id = rand() % 4;
		}
		
		
			mmSABZoomBird(obj->pos.x,obj->pos.y,obj->zoom,obj->type_id);
}
#endif
void mmSABListDestory()
{
	if(NULL == goHead)
		return;
		
	mmSABirdFly *t;
	t = goHead;
	while(goHead)
	{
		goHead = goHead->next;
		mmFree(t);
		t = goHead;
	}
	goHead = NULL;
	t = NULL;
}