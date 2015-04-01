//#include "../marmota/marmota.h"
#include "inc/mm_superangrybird_resource.h"
#include "mm_superangrybird.h"
#include "marmota_math.h"

//static mmCanvas *mmSABMainmenuBaseCanva = NULL;
//mmCanvas *mmSABMainmenuCanva = NULL;
//mmCanvas *mmSABMainmenuResCanva = NULL;
//mmCanvas *mmSABLogoCanva = NULL;

static mmS32 mmSabWaitTime = 0;
static mmS32 SABTimeUp = 0;
static mmU8 mmSABReadFile = 0;
mmU8 mmSABDrawOnce = 0;
static mmS32 mmSABLogoOffset = 0;
mmS32 mmSABMainMenu = -1;
mmU32 mmSABSound = 1;

mmU32 mmSABLevelScore[MMSAB_LEVEL_SUM+1]={0};

//#ifndef KKFUN_RES_OFFSET
	static char mmSABResFullPath[128] = {'\0'};
//#endif

#define SAB_LEVEL_ROW_NUM 7 //一行中关卡图标的数目
#ifdef MM_SUPERANGRYBIRD_RESOURCE_240X320
#define SAB_LEVEL_OFFSET_X 53//关卡图标间隔
#define SAB_LEVEL_OFFSET_Y 44
#define SAB_LOGO_OFFSET_SUM -222
#define SAB_LOGO_OFFSET_Y -3
#define SAB_LOGO_WAIT_TIME 30
#elif defined MM_SUPERANGRYBIRD_RESOURCE_240X400
#define SAB_LEVEL_OFFSET_X 53//关卡图标间隔
#define SAB_LEVEL_OFFSET_Y 53
#define SAB_LOGO_OFFSET_SUM -143
#define SAB_LOGO_OFFSET_Y -3
#define SAB_LOGO_WAIT_TIME 5
#else
/***TODO:320X480的宏定义***/
#define SAB_LEVEL_OFFSET_X 80//关卡图标间隔
#define SAB_LEVEL_OFFSET_Y 61
#define SAB_LOGO_OFFSET_SUM -246
#define SAB_LOGO_OFFSET_Y -4
#define SAB_LOGO_WAIT_TIME 5
#endif

extern void mmSABListInit();
extern void mmSABListUpdate(mmSABirdFly* obj);
extern void mmSABListDestory();

static void mm_sab_mainmenu_point_event(mmU32 *eventType, mmU16 *wParam, mmU16 mParam);

void mm_sab_mainmenu_draw_image(int x, int y, int img_id, mmBool transparentSign)
{
#ifdef zq
	return;
	
#endif
	/*mmCopyImageBuffer(mmSABMainmenuResCanva, mm_sab_mainmenu_image[img_id].x, mm_sab_mainmenu_image[img_id].y, mm_sab_mainmenu_image[img_id].width, mm_sab_mainmenu_image[img_id].height,  
		mmSABMainmenuBaseCanva, x, y, transparentSign);*/
	pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_MAINMENU_RES_CANVA], x, y, mm_sab_mainmenu_image[img_id].x, mm_sab_mainmenu_image[img_id].y, mm_sab_mainmenu_image[img_id].width, mm_sab_mainmenu_image[img_id].height);
}

static void mm_draw_star(int x, int y, int level)
{
	mmU8 i;
	for(i=0; i<(mmSABLevelScore[level] & 0xff); i++)
	{	
		#ifdef MM_SUPERANGRYBIRD_RESOURCE_320X480
			mm_sab_mainmenu_draw_image(x+2, y+6+10*i, (int)ANGRYBIRD_MAINMENU_LITTLE_STAR_IMG,mmTrue);
		#else
			mm_sab_mainmenu_draw_image(x+0, y+4+10*i, (int)ANGRYBIRD_MAINMENU_LITTLE_STAR_IMG,mmTrue);
		#endif
	}
}

static void mm_draw_number(int x, int y, int level)
{
	mmU8 i;
	#ifdef MM_SUPERANGRYBIRD_RESOURCE_320X480
	if(level < 10)
		mm_sab_mainmenu_draw_image(x+20, y+16, (int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + level),mmTrue);
	else if(level < 20){
		mm_sab_mainmenu_draw_image(x+20,y+8,(int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + 1),mmTrue);
		mm_sab_mainmenu_draw_image(x+20,y+19,(int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + level%10),mmTrue);
	}
	else{
		mm_sab_mainmenu_draw_image(x+20,y+10,(int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + 2),mmTrue);
		mm_sab_mainmenu_draw_image(x+20,y+21,(int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + level%20),mmTrue);
	}
	#else
	if(level < 10)
		mm_sab_mainmenu_draw_image(x+11, y+12, (int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + level),mmTrue);
	else if(level < 20){
		mm_sab_mainmenu_draw_image(x+11,y+7,(int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + 1),mmTrue);
		mm_sab_mainmenu_draw_image(x+11,y+16,(int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + level%10),mmTrue);
	}
	else{
		mm_sab_mainmenu_draw_image(x+11,y+7,(int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + 2),mmTrue);
		mm_sab_mainmenu_draw_image(x+11,y+18,(int)(ANGRYBIRD_MAINMENU_NUMBER0_IMG + level%20),mmTrue);
	}
	#endif
}

void mmSABZoomBird(mmS32 x , mmS32 y, float zoom, mmS32 id)
{
	/*mmPoint dst_pos;
	mm_sImg_info	img_info;
	dst_pos.x = x;
	dst_pos.y = y;
	img_info.pos.x = mm_sab_mainmenu_image[id].x;
	img_info.pos.y = mm_sab_mainmenu_image[id].y;
	img_info.size.x = mm_sab_mainmenu_image[id].width;
	img_info.size.y = mm_sab_mainmenu_image[id].height;
	mmTestZoomRun(mmSABMainmenuResCanva, img_info, mmSABMainmenuBaseCanva, dst_pos, zoom, zoom);*/
	pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_MAINMENU_RES_CANVA], x, y, mm_sab_mainmenu_image[id].x, mm_sab_mainmenu_image[id].y, 
		mm_sab_mainmenu_image[id].width, mm_sab_mainmenu_image[id].height);

}

static void mmSABGetFullPath()
{
	/*KscMemset(mmSABResFullPath, 0, sizeof(mmSABResFullPath));

	KscStrcat(mmSABResFullPath, (char*)KscFileGetRevableDisk());
	
	#ifdef MAMOTA_BACKEND_MSTAR
	KscStrcat(mmSABResFullPath, "/funboxctrl/FB_Extr1.res");
	#else
	KscStrcat(mmSABResFullPath, "\\funboxctrl\\FB_Extr1.res");
	#endif*/
//	KscDbgPrint("%s", mmSABResFullPath);
}

static mmSABirdFly *temp = NULL;
mmSABirdFly *goHead = NULL;


void mm_superangrybird_mainmenu_initial(void)
{

//	WordInit();
	
	//mdi_audio_set_volume(7,6);
	
	if(0 == mmSABReadFile){
		mmSABReadGameRestore();
		mmSABReadFile = 1;
	}
	
	/*mmSABMainmenuBaseCanva = NULL;
	mmSABMainmenuCanva = NULL;
	mmSABMainmenuResCanva = NULL;
	mmSABLogoCanva = NULL;*/

	SABTimeUp = 0;
	mmSABDrawOnce = 0;
	mmSABLogoOffset = 0;
	mmSabWaitTime = 0;
	
		
	#ifndef KKFUN_RES_OFFSET
		mmSABGetFullPath();
	#endif
	goHead = NULL;
	temp = NULL;
	
	//声音初始化
//	if(1 == mmSABSound && -1 != mmSABMainMenu)
		//marmota->audio->playRawData((mmAudioData*)sab_audio_resources, 1);
//		AngryBird_play_audio(AUDIO_ID_MAINMENU_BG, AUDIO_PLAYER_MIDI_0, iTRUE);
//	else
//		AngryBird_stop_all_audio();
	
}


void mm_superangrybird_mainmenu_run(void)
{
	/*if(NULL == mmSABMainmenuBaseCanva){
		mmSABMainmenuBaseCanva = marmota->g->getDoubleBufferCanvas();
	}*/
	
	if(-1 == mmSABMainMenu)
	{
		SABTimeUp++;
		if(0 == mmSABDrawOnce){
			//#ifdef KKFUN_RES_OFFSET
			//marmota->g->drawImageFromBin(0, 0, mmResList, ANGRYBIRD_MAINMENU_LOGO_CANVA, MMSAB_RES_PATH);//主菜单logo图
			//#else
			//marmota->g->drawImageFromBin(0, 0, mmResList, ANGRYBIRD_MAINMENU_LOGO_CANVA, mmSABResFullPath);//主菜单logo图
			//#endif
			pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_MAINMENU_LOGO_CANVA], 0, 0, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
			mmSABDrawOnce = 1;
		}
		if(20 <= SABTimeUp)
		{
			SABTimeUp = 0;
			mmSABMainMenu = 0;
			mmSABDrawOnce = 0;
//			if(0 == mmSABSound)
//				AngryBird_stop_all_audio();
//			else
//				AngryBird_play_audio(AUDIO_ID_MAINMENU_BG, AUDIO_PLAYER_MIDI_0, iTRUE);
		}
	}	
		
	/*if(!mmSABMainmenuResCanva){
		#ifdef KKFUN_RES_OFFSET
			mmSABMainmenuResCanva = marmota->g->newCanvasWithImageFromBin(mmResList, ANGRYBIRD_MAINMENU_RES_CANVA, MMSAB_RES_PATH);
		#else
			mmSABMainmenuResCanva = marmota->g->newCanvasWithImageFromBin(mmResList, ANGRYBIRD_MAINMENU_RES_CANVA, mmSABResFullPath);
		#endif
	}*/
	
	if(0 == mmSABMainMenu)//主菜单
	{	
		/*if(!mmSABMainmenuCanva){
			#ifdef KKFUN_RES_OFFSET
				mmSABMainmenuCanva =  marmota->g->newCanvasWithImageFromBin(mmResList, ANGRYBIRD_MAINMENU_CANVA, MMSAB_RES_PATH);
			#else
				mmSABMainmenuCanva =  marmota->g->newCanvasWithImageFromBin(mmResList, ANGRYBIRD_MAINMENU_CANVA, mmSABResFullPath);
			#endif
		}
			
		mmCopyImageBuffer(mmSABMainmenuCanva, 0, 0, UI_device_width, UI_device_height, mmSABMainmenuBaseCanva, 0, 0, mmFalse);*/
		pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_MAINMENU_CANVA], 0, 0, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);

		if(NULL == goHead)
			mmSABListInit();
		temp = goHead;
		while(temp){
			mmSABListUpdate(temp);
			temp = temp->next;
		}
		mm_sab_mainmenu_draw_image(mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_BEGIN].x, mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_BEGIN].y, (int)ANGRYBIRD_MAINMENU_START_IMG,mmTrue);
		mm_sab_mainmenu_draw_image(mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_EXIT].x, mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_EXIT].y, (int)ANGRYBIRD_MAINMENU_EXIT_IMG,mmTrue);
		mm_sab_mainmenu_draw_image(mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_HELP].x, mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_HELP].y, (int)ANGRYBIRD_MAINMENU_QUESTION_IMG,mmTrue);
		mm_sab_mainmenu_draw_image(mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_SOUND].x, mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_SOUND].y, (int)ANGRYBIRD_MAINMENU_SOUND_IMG,mmTrue);
		if(mmSABSound == 0)
		mm_sab_mainmenu_draw_image(mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_SOUND].x, mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_SOUND].y, (int)ANGRYBIRD_MAINMENU_SOUND_OFF_IMG,mmTrue);
	
	}
	
	if(0 == mmSABDrawOnce)
	{
		if(1 == mmSABMainMenu)//关卡选择
		{
			mmU8 i,j;
			mmS32 x,y;
			
			/*#ifdef KKFUN_RES_OFFSET
				marmota->g->drawImageFromBin(0, 0, mmResList, ANGRYBIRD_MAINMENU_LEVEL_CANVA, MMSAB_RES_PATH);
			#else
				marmota->g->drawImageFromBin(0, 0, mmResList, ANGRYBIRD_MAINMENU_LEVEL_CANVA, mmSABResFullPath);
			#endif*/
			pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_MAINMENU_LEVEL_CANVA], 0, 0, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
			
			
			for(i=0; i< MMSAB_LEVEL_SUM/SAB_LEVEL_ROW_NUM; i++)
			{
				x = mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_LEVEL_ICON].x - SAB_LEVEL_OFFSET_X*i;
				y = mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_LEVEL_ICON].y;
				for(j=0; j<MMSAB_LEVEL_SUM/(MMSAB_LEVEL_SUM/SAB_LEVEL_ROW_NUM); j++)
				{
					y = mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_LEVEL_ICON].y + SAB_LEVEL_OFFSET_Y*j;
					if( (SAB_LEVEL_ROW_NUM*i+j+1) <= (mmSABLevelScore[0]+1) )
					{
						mm_sab_mainmenu_draw_image(x, y, (int)ANGRYBIRD_MAINMENU_LEVEL_UNLOCK_IMG,mmTrue);
						mm_draw_star(x,y,(SAB_LEVEL_ROW_NUM*i+j+1));
						mm_draw_number(x,y,(SAB_LEVEL_ROW_NUM*i+j+1));
					
					}
					else
						mm_sab_mainmenu_draw_image(x, y, (int)ANGRYBIRD_MAINMENU_LEVEL_LOCK_IMG,mmTrue);
				}
			}
			
		}
		else if(2 == mmSABMainMenu)//帮助
		{
			#ifdef KKFUN_RES_OFFSET
				//marmota->g->drawImageFromBin(0, 0, mmResList, ANGRYBIRD_MAINMENU_LEVEL_CANVA, MMSAB_RES_PATH);
				pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_MAINMENU_LEVEL_CANVA], 0, 0, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
			#else
				//marmota->g->drawImageFromBin(0, 0, mmResList, ANGRYBIRD_MAINMENU_LEVEL_CANVA, mmSABResFullPath);
			    pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_MAINMENU_LEVEL_CANVA], 0, 0, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
			#endif
			mm_sab_mainmenu_draw_image(mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_HELP_DES].x, mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_HELP_DES].y, (int)ANGRYBIRD_MAINMENU_HELP_IMG,mmTrue);
			mm_sab_mainmenu_draw_image(mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_HELP_TITLE].x, mm_sab_mainmenu_rect[ANGRYBIRD_MAINMENU_HELP_TITLE].y, (int)ANGRYBIRD_MAINMENU_HELP_TITLE_IMG,mmTrue);
		}
		mmSABDrawOnce = 1;
	}
	
	if(3 == mmSABMainMenu){//第一关logo
		/*if(!mmSABLogoCanva){
		#ifdef KKFUN_RES_OFFSET
			mmSABLogoCanva = marmota->g->newCanvasWithImageFromBin(mmResList, ANGRYBIRD_LOGO_CANVA, MMSAB_RES_PATH);
		#else
			mmSABLogoCanva = marmota->g->newCanvasWithImageFromBin(mmResList, ANGRYBIRD_LOGO_CANVA, mmSABResFullPath);
		#endif
		}
		mmCopyImageBuffer(mmSABLogoCanva, 0, mmSABLogoOffset, UI_device_width, UI_device_height, mmSABMainmenuBaseCanva, 0, 0, mmFalse);
		if(mmSABLogoOffset < SAB_LOGO_OFFSET_SUM)
			mmSABLogoOffset = mmSABLogoOffset + SAB_LOGO_OFFSET_Y;
		else{
			if(mmSabWaitTime > SAB_LOGO_WAIT_TIME)
			{
				mmSABLogoOffset = 0;
				mmSabWaitTime = 0;
				mmSABMainMenu = 0;
//				mmEntryState(MM_SUPERANGRYBIRD_KERNEL);
				return;
			}
			else
				mmSabWaitTime++;
		}
	*/
		//marmota->g->drawImageFromBin(0, mmSABLogoOffset, mmResList, ANGRYBIRD_LOGO_CANVA, mmSABResFullPath);
		pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_LOGO_CANVA], 0, 0, 0, -mmSABLogoOffset, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
		if(mmSABLogoOffset > SAB_LOGO_OFFSET_SUM)
			mmSABLogoOffset = mmSABLogoOffset + SAB_LOGO_OFFSET_Y;
		else
		{
			if(mmSabWaitTime > SAB_LOGO_WAIT_TIME)
			{
				mmSABLogoOffset = 0;
				mmSabWaitTime = 0;
				mmSABMainMenu = 0;
//				mmEntryState(MM_SUPERANGRYBIRD_KERNEL);
				return;
			}
			else
				mmSabWaitTime++;
		}
	}
}

void mm_superangrybird_mainmenu_event(mmU32 eventType, mmU16 wParam, mmU16 mParam)
{
	if(LLP_PEN_UP == eventType)
	{
		if(3 == mmSABMainMenu){
			mmSABLogoOffset = 0;
			mmSABMainMenu = 0;
//			mmEntryState(MM_SUPERANGRYBIRD_KERNEL);  //进入游戏
			return;
		}
		else
			mm_sab_mainmenu_point_event(&eventType, &wParam, mParam);
	}
}

static  mmBool mm_sab_check_point_in_rect(mmPoint point, int point_id, int i)
{
	int j = i / SAB_LEVEL_ROW_NUM;
		
	int x = mm_sab_mainmenu_rect[point_id].x - SAB_LEVEL_OFFSET_X * (j) - 5;
	int y = mm_sab_mainmenu_rect[point_id].y + SAB_LEVEL_OFFSET_Y * (i % SAB_LEVEL_ROW_NUM) - 5;
	int w = mm_sab_mainmenu_rect[point_id].width + 5;
	int h = mm_sab_mainmenu_rect[point_id].height + 5;
	
	if((point.x > x && point.x < x + w ) && (point.y > y && point.y < y + h) )
		return mmTrue;
	
	return mmFalse;
}


static void mm_sab_mainmenu_point_event(mmU32 *eventType, mmU16 *wParam, mmU16 mParam)
{
	mmPoint point;
	point.x = *wParam;
	point.y = mParam;
	
	if(0 == mmSABMainMenu)
	{
		if(mmTrue == mm_sab_check_point_in_rect(point, (int)ANGRYBIRD_MAINMENU_BEGIN,0))
		{
			mmSABMainMenu = 1;
			mmSABDrawOnce = 0;
			return;
		}
		else if(mmTrue == mm_sab_check_point_in_rect(point, (int)ANGRYBIRD_MAINMENU_HELP,0))
		{
			mmSABMainMenu = 2;
			mmSABDrawOnce = 0;
			return;
		}
		else if(mmTrue == mm_sab_check_point_in_rect(point, (int)ANGRYBIRD_MAINMENU_SOUND,0))
		{
			mmSABSound = mmSABSound > 0 ? 0 : 1;
			mmSABDrawOnce = 0;	
			/*if(0 == mmSABSound)
				marmota->audio->stopAllAudio();
			else
				marmota->audio->playRawData((mmAudioData *)sab_audio_resources, 1);*/
			
			return;
		}
		else if(mmTrue == mm_sab_check_point_in_rect(point, (int)ANGRYBIRD_MAINMENU_EXIT,0))
		{
			///mmMarmotaExit();
			pfunc->kernel_send_request(i51_REQ_EXIT);
			return;
		}
	}
	
	else if(1 == mmSABMainMenu)
	{
		mmU8 i;
		
		if(mmSABLevelScore[0] > MMSAB_LEVEL_SUM - 1)
			mmSABLevelScore[0] = MMSAB_LEVEL_SUM-1;

		for(i=0; i<=mmSABLevelScore[0]; i++)
		{
			if(mmTrue == mm_sab_check_point_in_rect(point, (int)ANGRYBIRD_MAINMENU_LEVEL_ICON, i))
			{
				mmSABGameStageId = i;
				
				if(0 == mmSABGameStageId)//第一关
				{	
				
					/*if(mmSABMainmenuCanva){
						marmota->g->destoryCanvas(mmSABMainmenuCanva);
						mmSABMainmenuCanva = null;
					}
					if(mmSABMainmenuResCanva){
						marmota->g->destoryCanvas(mmSABMainmenuResCanva);
						mmSABMainmenuResCanva = null;
					}*/
					mmSABListDestory();
					
					mmSABDrawOnce = 1;
					mmSABMainMenu = 3;
					if(mmSABSound == 1){
						/*marmota->audio->stopAllAudio();
						marmota->audio->playRawData((mmAudioData *)sab_audio_logo_resources,0);*/
					}
				}
				else
//					mmEntryState(MM_SUPERANGRYBIRD_KERNEL);
				return;
			}
			
		}
		
		if(mmTrue == mm_sab_check_point_in_rect(point, (int)ANGRYBIRD_MAINMENU_LEAVE_LEVEL,0))
		{
			mmSABMainMenu = 0;
			mmSABDrawOnce = 0;
			return;
		}
	}
	
	else if(2 == mmSABMainMenu)
	{
		if(mmTrue == mm_sab_check_point_in_rect(point, (int)ANGRYBIRD_MAINMENU_LEAVE_HELP,0))
		{
			mmSABMainMenu = 0;
			mmSABDrawOnce = 0;
			return;
		}
	}
}

/*
static void mm_sab_mainmenu_key_event(mmU16 wParam)
{
	if(0 == mmSABMainMenu)
	{
		switch(wParam)
		{
		case MM_KEY_RSK:
		case MM_KEY_ENTER:
			mmSABMainMenu = 1;//关卡
			break;
		default:
			break;
		}
	}
}
*/

void mm_superangrybird_mainmenu_destory(void)
{	
	mmSABListDestory();

	/*marmota->audio->stopAllAudio();
	
	mmSABMainmenuBaseCanva = null;
	
	if(mmSABMainmenuCanva){
		marmota->g->destoryCanvas(mmSABMainmenuCanva);
		mmSABMainmenuCanva = null;
	}
	if(mmSABMainmenuResCanva){
		marmota->g->destoryCanvas(mmSABMainmenuResCanva);
		mmSABMainmenuResCanva = null;
	}
	
	if(mmSABLogoCanva){
		marmota->g->destoryCanvas(mmSABLogoCanva);
		mmSABLogoCanva = null;
	}*/

}
