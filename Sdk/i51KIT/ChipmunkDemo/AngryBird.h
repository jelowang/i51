
#ifndef THE_bird_H
#define THE_bird_H

#include "i51.h"
#include "i51Public.h"
#include "i51app.h"
#include "chipmunk.h"
#include "create_container.h"

#define UI_device_width  PK_SCREEN_WIDTH
#define UI_device_height PK_SCREEN_HEIGHT

extern i51Public_struct *pfunc;

//typedef enum{
//	ANGRYBIRD_MOON_CANVA = 0,
//	ANGRYBIRD_STOREY_CANVA = 1,
//	ANGRYBIRD_MAINMENU_LEVEL_CANVA = 2,
//	ANGRYBIRD_LOGO_CANVA = 3,
//	ANGRYBIRD_MAINMENU_CANVA = 4,
//	ANGRYBIRD_MAINMENU_LOGO_CANVA = 5,
//	ANGRYBIRD_MAINMENU_RES_CANVA = 6,
//	ANGRYBIRD_PAUSE_CANVA = 7,
//	ANGRYBIRD_SCENE_CANVA = 8,
//	ANGRYBIRD_UI_RES_CANVA = 9,
//	ANGRYBIRD_RESULT_CANVA = 10,
//	IMG_ID_MAX
//}image_id;
typedef enum{
	ANGRYBIRD_MAINMENU_LOGO_CANVA = 0,
	ANGRYBIRD_MAINMENU_CANVA = 1,
	ANGRYBIRD_MAINMENU_RES_CANVA = 2,
	ANGRYBIRD_MOON_CANVA = 3,
	ANGRYBIRD_STOREY_CANVA = 4,
	ANGRYBIRD_MAINMENU_LEVEL_CANVA = 5,
	ANGRYBIRD_LOGO_CANVA = 6,
	ANGRYBIRD_PAUSE_CANVA = 7,
	ANGRYBIRD_SCENE_CANVA = 8,
	ANGRYBIRD_UI_RES_CANVA = 9,
	ANGRYBIRD_RESULT_CANVA = 10,
	IMG_ID_MAX
}image_id;

enum mm_concubine_state{MM_SUPERANGRYBIRD_DECODE, MM_SUPERANGRYBIRD_MAINMENU, MM_SUPERANGRYBIRD_KERNEL, MM_SUPERANGRYBIRD_STATE_MAX};

typedef enum
{
	AUDIO_ID_MAINMENU_BG,
	AUDIO_ID_MAX
}audio_id;

typedef struct  
{
	FuncPtr init;
	FuncPtr destroy;
	FuncPtr run;
	iU8     state;
}entry_state_struct;

typedef struct  
{
	iU8 init;
	iU8 entry_game;
	iU16 *ground_data_buf; //数据buf
	entry_state_struct cur_state;
	entry_state_struct last_state;
	iHANDLE ImageContainHanle;  //打开容器的指针
	public_image_struct images[IMG_ID_MAX];
}AngryBird_struct;

extern AngryBird_struct AngryBird;
//extern void public_draw_line(iS32 x0, iS32 y0, iS32 x1, iS32 y1, iCOLOR color);
//extern void draw_ground_buf_to_screen(iS32 dst_x, iS32 dst_y, iU32 length, iU8 row);
//extern void AngryBird_play_audio(iU8 audio_id, iU8 channel, iBOOL loop);
//extern void AngryBird_stop_all_audio(void);
//extern void mmEntryState(iU8 state);

///////////////////////////////////////////////////////////////////////////////
extern void mm_superangrybird_mainmenu_initial(void);
extern void mm_superangrybird_mainmenu_run(void);
extern void mm_superangrybird_mainmenu_destory(void);
extern void mm_superangrybird_mainmenu_event(iU32 eventType, iU16 wParam, iU16 mParam);

extern void mm_superangrybird_kernel_initial(void);
extern void mm_superangrybird_kernel_run(void);
extern void mm_superangrybird_kernel_destory(void);
extern void mm_superangrybird_kernel_event(iU32 eventType, iU16 wParam, iU16 mParam);

#endif

