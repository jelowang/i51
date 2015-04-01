//#include "../marmota/marmota.h"
#include "inc/mm_superangrybird_resource.h"
#include "mm_superangrybird.h"

//extern mmCanvas* mmSABSceneCanva;
//extern mmCanvas* mmSABBaseCanva;
//extern mmCanvas* mmSABUIResCanva;

void
mmSABDrawUI(int x, int y, int id){
#ifdef zq
	return 
#endif
	/*mmCopyImageBuffer(mmSABUIResCanva, 
	mm_sab_ui_image[id].x, mm_sab_ui_image[id].y, mm_sab_ui_image[id].width, mm_sab_ui_image[id].height,  
	mmSABBaseCanva, x, y, mmTrue);*/
	pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_UI_RES_CANVA], x, y, mm_sab_ui_image[id].x, mm_sab_ui_image[id].y, mm_sab_ui_image[id].width, mm_sab_ui_image[id].height);
}

void
mm_sba_draw_image(int x, int y, int id, mmBool transparentSign){
#ifdef zq
	return 
#endif
	/*mmCopyImageBuffer(
		mmSABSceneCanva,
		mm_sab_raw_image[id].x,
		mm_sab_raw_image[id].y,
		mm_sab_raw_image[id].width,
		mm_sab_raw_image[id].height,  
		mmSABBaseCanva,
		x, y, transparentSign
	);*/
	pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_SCENE_CANVA], x, y, mm_sab_raw_image[id].x, mm_sab_raw_image[id].y, mm_sab_raw_image[id].width, mm_sab_raw_image[id].height);
}

