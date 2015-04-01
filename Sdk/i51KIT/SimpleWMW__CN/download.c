#include "download.h"
#include "i51Public.h"

download_struct download;
percent_struct  percent;
//static iU8 IsCompleten = iTRUE;
iU8 IsCompleten = iTRUE;
iU32 word_frame = 0;
iU8 IsEntry = iFALSE;


iU8 public_pen_enabled_rect_EX(iU16 pen_x, iU16 pen_y, iU16 min_x, iU16 max_x, iU16 min_y, iU16 max_y)
{
	if(pen_x >= min_x && pen_x <= max_x && pen_y >= min_y && pen_y <= max_y)
	{
		return 1;
	}
	return 0;
}

iU32 progress_bar_percent(iU8 step)
{
	iU32 i;
	iU32 per = PROGRESS_START;

	for(i = 0; i < step; i++)
	{
		per += percent.each[i];
	}
	return per;
}

void progress_state_init(iU8 state)
{
	download.progress_state = state;
}

void download_app_callback(iCHAR* currentstep, iS32 status, iU32 step, iU32 totall, iU32 curdl, iU32 dlcount)
{
	if(totall != 0)
	{
		iU32 temp = 1;

		if(totall >= 1024)
			temp = 1024;
		i51AdeOsLog(0, "install app, percent = %d, step = %d, totall = %d, curdl = %d, dlcount = %d", percent.current, step, totall, curdl, dlcount);
		percent.current = progress_bar_percent(PROGRESS_DOWNLOAD) + 9500 * (curdl-1) / dlcount +  9500 / dlcount * (step/temp) / (totall/temp) ;
	}
	if(status == i51_KERNEL_INSTALL_FINISHED)
	{
		i51SlDlStop(0);
		progress_state_init(DL_STATE_FINISH);
		download.dl_create_ret = iFALSE;
	}
	else if(status == i51_KERNEL_INSTALL_RUNING)
	{

	}
	else
	{
		download.IsError = iTRUE;
		download.error_type = ERROR_LINK_FALSE;
		i51AdeOsLog(0, "下载应用失败，ret = %d", status);
	}
}

void download_exit(void)
{
	//container_exit();
	
	/* 下载接口释放 */
	//if(download.dl_create_ret)
	{
		download.dl_create_ret = iFALSE;
		i51SlDlStop(0);
	}

	memset(&download, 0, sizeof(download_struct));
	memset(&percent,  0, sizeof(percent_struct));
}

void download_render(void)
{
	/*if(download.IsError)
	{
		UI_frame_render();
	}
	else
	{*/
		if(download.FuncRender != NULL)
		{
			download.FuncRender(percent.current);
		}
	/*}*/
}

void download_init(iU16 *package_name, void (*FuncRender)(iU32), void (*callback)(iBOOL))
{
	iS16 i;
	iU16 per[PROGRESS_NUM] = PROGRESS_EACH_PERCENT;

	for(i=0; i<PROGRESS_NUM; i++)
	{
		percent.each[i] = per[i];     //各个过程的进度百分比
	}

	download.FuncRender = FuncRender;
	download.cb = callback;
	download.dl_create_ret = iTRUE;
	i51AdeStdWStrcpy(download.package_name, package_name);
	i51KernelInstallApplication(download.package_name, download_app_callback);
	percent.current  = PROGRESS_START;
	progress_state_init(DL_STATE_DOWNLOAD);
}

void download_resource(iU16 *package_name, void (*FuncRender)(iU32), void (*callback)(iBOOL))
{
	download_render();
	switch(download.progress_state)
	{
	case DL_STATE_INIT:
		download_init(package_name, FuncRender, callback);
		break;
	case DL_STATE_DOWNLOAD:
		break;
	case DL_STATE_FINISH:
		download.cb(iTRUE);
		download_exit();
		IsCompleten = iTRUE;
		break;
	default:
		break;
	}
}

void progress_input_lsk(void)
{
	if(!download.IsError)
		return;

	switch(download.error_type)
	{
	case ERROR_DECODE_FALSE:
	case ERROR_SYSTEM:
		download.cb(iFALSE);//退出
		download_exit();
		break;
	case ERROR_LINK_FALSE:
		download.IsError = iFALSE;
		if(download.dl_create_ret)
		{
			i51SlDlStop(0);
			download.dl_create_ret = iFALSE;
		}
		i51KernelInstallApplication(download.package_name, download_app_callback);   //重试
		break;
	}
}

void progress_input_rsk(void)
{
	switch(download.error_type)
	{
	case ERROR_DECODE_FALSE:
	case ERROR_SYSTEM:
	case ERROR_LINK_FALSE:
		download.cb(iFALSE);//退出
		download_exit();
		break;
	}
}

void download_key_handle(LLPPhoneKey *key)
{
	download.lsk_down = iFALSE;
	download.rsk_down = iFALSE;
	if(key->type == LLP_KEY_DOWN)
	{
		if(key->value == VKEY_LSK)
		{
			download.lsk_down = iTRUE;
		}
		else if(key->value == VKEY_RSK)
		{
			download.rsk_down = iTRUE;
		}
	}
	else if(key->type == LLP_KEY_UP)
	{
		if(key->value == VKEY_LSK)
		{
			progress_input_lsk();
		}
		else if(key->value == VKEY_RSK)
		{
			progress_input_rsk();
		}
	}
}

void download_pen_handle(LLPPhonePen *pen)
{
	download.lsk_down = iFALSE;
	download.rsk_down = iFALSE;

#ifndef PKIG_SCREEN_HORIZONTAL
	if(pen->type == LLP_PEN_DOWN || pen->type == LLP_PEN_MOVE)
	{
		if(public_pen_enabled_rect_EX(pen->x, pen->y, 0, 60, PK_SCREEN_HEIGHT-60, PK_SCREEN_HEIGHT))
		{
			download.lsk_down = iTRUE;
		}
		else if(public_pen_enabled_rect_EX(pen->x, pen->y, PK_SCREEN_WIDTH-60, PK_SCREEN_WIDTH, PK_SCREEN_HEIGHT-60, PK_SCREEN_HEIGHT))
		{
			download.rsk_down = iTRUE;
		}
	}
	else if(pen->type == LLP_PEN_UP)
	{
		if(public_pen_enabled_rect_EX(pen->x, pen->y, 0, 60, PK_SCREEN_HEIGHT-60, PK_SCREEN_HEIGHT))
		{
			progress_input_lsk();
		}
		else if(public_pen_enabled_rect_EX(pen->x, pen->y, PK_SCREEN_WIDTH-60, PK_SCREEN_WIDTH, PK_SCREEN_HEIGHT-60, PK_SCREEN_HEIGHT))
		{
			progress_input_rsk();
		}
	}
#else
	if(pen->type == LLP_PEN_DOWN || pen->type == LLP_PEN_MOVE)
	{
		if(public_pen_enabled_rect_EX(pen->x, pen->y, 0, 60, 0, 60))
		{
			download.lsk_down = iTRUE;
		}
		else if(public_pen_enabled_rect_EX(pen->x, pen->y, 0, 60, PK_SCREEN_HEIGHT-60, PK_SCREEN_HEIGHT))
		{
			download.rsk_down = iTRUE;
		}
	}
	else if(pen->type == LLP_PEN_UP)
	{
		if(public_pen_enabled_rect_EX(pen->x, pen->y, 0, 60, 0, 60))
		{
			progress_input_lsk();
		}
		else if(public_pen_enabled_rect_EX(pen->x, pen->y, 0, 60, PK_SCREEN_HEIGHT-60, PK_SCREEN_HEIGHT))
		{
			progress_input_rsk();
		}
	}
#endif
}

typedef enum
{
	IMG_ID_BG = 0,
	IMG_ID_LOADING_BAR0,
	IMG_ID_LOADING_BAR1,
	IMG_ID_LSK_BUTTON,
	IMG_ID_RSK_BUTTON,
	IMG_ID_WORD_TIPS0,
	IMG_ID_WORD_TIPS1,
	IMG_ID_NUM,
	IMG_ID_MAX
}image_id;

//#include "version.h"
#include "localG2.h"
#include "Image_Data.h"

iHANDLE gImageContainHanle = iNULL;
public_image_struct gImages[IMG_ID_MAX];
iU8 ginit = iFALSE;

void public_image_init_EX(public_image_struct *image, iU32 image_id, iU32 image_data)
{
	if(image_data)
	{
		image->srcData = image_data;
		//image->pContainer = i51KitG2ImageCreate(image_data);
		image->id         = image_id;
		i51KitG2GetImageAttrEx(image_data, (iU32 *)&image->w, (iU32 *)&image->h);
		//i51KitG2ImageDestory(image->pContainer);
	}
}

iRECT *public_build_rect_EX(iRECT *rect, iS16 x, iS16 y, iS16 width, iS16 height)
{
	rect->X      = x;
	rect->Y      = y;
	rect->Width  = width;
	rect->Height = height;
	return rect;
}

iS32 public_draw_image_EX(public_image_struct *image, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h)
{
	iRECT dstRect;
	iRECT srcRect;
	iS32 ret;
	iU32 pImage = 0;
	iU32 tick0;

	if(src_w == SRC_IMAGE_WIDTH)
	{
		src_w = image->w;
	}
	if(src_h == SRC_IMAGE_HEIGHT)
	{
		src_h = image->h;
	}
	public_build_rect_EX(&dstRect, dst_x, dst_y, PK_SCREEN_WIDTH, PK_SCREEN_HEIGHT);
	public_build_rect_EX(&srcRect, src_x, src_y, src_w, src_h);

	tick0 = i51AdeOsGetTick();
	image->pContainer = i51KitG2ImageCreate(image->srcData);
	i51AdeOsLog(0, "解码图片耗时：%d", i51AdeOsGetTick()-tick0);
	ret = i51KitG2DrawImageEx(image->pContainer, &srcRect, &dstRect);
	i51KitG2ImageDestory(image->pContainer);
	return ret;
}

void public_draw_score_EX(iU32 score, iS16 dst_x, iS16 dst_y, public_image_struct *image)
{
	char str[20];
	iS32 i;
	iS16 num_len;

	image->pContainer = i51KitG2ImageCreate((iU32)num_Hex);//(image->srcData));

	/* 字符":"ASCII码为58，代表数字10 */
	sprintf(str, "%d%s", score, ":");
	for(i=0;str[i] != '\0';i++)
	{
		iRECT dstRect;
		iRECT srcRect;

#ifndef PKIG_SCREEN_HORIZONTAL
		num_len = image->h/11;
		public_build_rect_EX(&dstRect, dst_x+i*image->w, dst_y, PK_SCREEN_WIDTH, PK_SCREEN_HEIGHT);
		public_build_rect_EX(&srcRect, 0, (str[i]-48)*num_len, image->w, num_len);
		i51KitG2DrawImageEx(image->pContainer, &srcRect, &dstRect);
#else
		num_len = image->w/11;
		public_build_rect_EX(&dstRect, dst_x, dst_y+i*image->h, PK_SCREEN_WIDTH, PK_SCREEN_HEIGHT);
		public_build_rect_EX(&srcRect, image->w - (str[i]-48+1)*num_len, 0, num_len, image->h);
		i51KitG2DrawImageEx(image->pContainer, &srcRect, &dstRect);
#endif
	}

	i51KitG2ImageDestory(image->pContainer);
}

void download_interface_render(iU32 length)
{
	iU32 bar_len;
	char str[15];

	i51KitG2CleanScreenEx(0, iNULL);
	public_draw_image_EX(&gImages[IMG_ID_BG], BG_X, BG_Y, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
	
	if(download.IsError)
	{
		public_draw_image_EX(&gImages[IMG_ID_WORD_TIPS1], WORD_TIPS_X, WORD_TIPS_Y, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
		public_draw_image_EX(&gImages[IMG_ID_LSK_BUTTON], LEFT_BUTTON_X, LEFT_BUTTON_Y, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
	}
	else
	{
		//static iU32 word_frame = 0;
		char statr_x = 0;

		public_draw_image_EX(&gImages[IMG_ID_LOADING_BAR0], LOADING_BAR_X, LOADING_BAR_Y, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
		bar_len = length * LOADING_BAR_LEN / 10000;
#ifndef PKIG_SCREEN_HORIZONTAL
		public_draw_image_EX(&gImages[IMG_ID_LOADING_BAR1], LOADING_BAR_X, LOADING_BAR_Y, 0, 0, bar_len, SRC_IMAGE_HEIGHT);
#else
		public_draw_image_EX(&gImages[IMG_ID_LOADING_BAR1], LOADING_BAR_X, LOADING_BAR_Y, 0, 0, SRC_IMAGE_WIDTH, bar_len);
#endif

		if(word_frame >= 3)
		{
			word_frame = 0;
		}
		else
		{
			word_frame++;
		}
#ifndef PKIG_SCREEN_HORIZONTAL
		public_draw_image_EX(&gImages[IMG_ID_WORD_TIPS0], WORD_TIPS_X, WORD_TIPS_Y, 0, 0, 104-19-(3-word_frame)*5, SRC_IMAGE_HEIGHT);
#else
		public_draw_image_EX(&gImages[IMG_ID_WORD_TIPS0], WORD_TIPS_X, WORD_TIPS_Y, 0, 0, SRC_IMAGE_WIDTH, 104-19-(3-word_frame)*5);
#endif
		public_draw_score_EX(length / 100, PROGRESS_NUM_X, PROGRESS_NUM_Y, &gImages[IMG_ID_NUM]);
	}

	public_draw_image_EX(&gImages[IMG_ID_RSK_BUTTON], RIGHT_BUTTON_X, RIGHT_BUTTON_Y, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
}

void portal_entry(iU32 aid, void (*cb)(iBOOL))
{
	//static iU8 IsEntry = iFALSE;

	if(IsEntry)
		return;


	if(IsCompleten && i51KernelCheckAppCompleten((iU16 *)L"Portal"))
	{
		i51AdeOsLog(0, "加载portal");
		i51KernelSendRequest (aid, (iU16 *)L"Portal", i51_REQ_LOADEX, 0, 0);
		cb(iTRUE);
		IsEntry = iTRUE;
	}
	else
	{
		IsCompleten = iFALSE;
		if(!download.init)
		{
			download.init = iTRUE;

			public_image_init_EX(&gImages[IMG_ID_BG], IMG_ID_BG, (iU32)bg_Hex);
			public_image_init_EX(&gImages[IMG_ID_LSK_BUTTON], IMG_ID_LSK_BUTTON, (iU32)lsk_button_Hex);
			public_image_init_EX(&gImages[IMG_ID_RSK_BUTTON], IMG_ID_RSK_BUTTON, (iU32)rsk_button_Hex);
			public_image_init_EX(&gImages[IMG_ID_LOADING_BAR0], IMG_ID_LOADING_BAR0, (iU32)progress_bar0_Hex);
			public_image_init_EX(&gImages[IMG_ID_LOADING_BAR1], IMG_ID_LOADING_BAR1, (iU32)progress_bar1_Hex);
			public_image_init_EX(&gImages[IMG_ID_NUM], IMG_ID_NUM, (iU32)num_Hex);
			public_image_init_EX(&gImages[IMG_ID_WORD_TIPS0], IMG_ID_WORD_TIPS0, (iU32)word_tips0_Hex);
			public_image_init_EX(&gImages[IMG_ID_WORD_TIPS1], IMG_ID_WORD_TIPS1, (iU32)word_tips1_Hex);
		}

		download_resource((iU16 *)L"Portal", download_interface_render, cb);
	}
}

void portal_exit(void)
{
	download_exit();
}
