#include "i51Public.h"
//#include "http_public.h"
#include "i51app.h"

iFILE pFont;

iBOOL find_aim_dir(char *dir)
{
	iU16 DirName[66];

	i51AdeStdWSprintf(DirName, 66, "%s", dir);
	if(i51AdeOsFileIsDirExist(DirName))
	{
		return iTRUE;
	}
	else
	{
		return i51AdeOsFileCreateDir(DirName);
	}
}

iBOOL check_file_is_exist(char *dir, char *file)
{
	char path[33];
	iU16 FileName[66];

	i51AdeStdWSprintf(FileName, 66, "%s\\%s", dir, file);
	return i51AdeOsFileIsExist(FileName);
}

void public_itoa(iS32 value, char *str)
{
//	sprintf( str, "%d", value);
}

void public_image_init(public_image_struct *image, iHANDLE pContainer, iU32 image_id)
{
	image->pContainer = pContainer;
	image->id         = image_id;
	i51KitG2GetImageAttr(pContainer, image->id, (iU32 *)&image->w, (iU32 *)&image->h, iNULL);
}

iRECT *public_build_rect(iRECT *rect, iS16 x, iS16 y, iS16 width, iS16 height)
{
	rect->X      = x;
	rect->Y      = y;
	rect->Width  = width;
	rect->Height = height;
	return rect;
}

iPoint *public_build_point(iPoint *point, iS32 x, iS32 y)
{
	point->x = x;
	point->y = y;
	return point;
}

void public_build_rect_vertex(iS32 point[][2], iS32 x, iS32 y, iS32 width, iS32 height)
{
	point[0][0] = x;
	point[0][1] = y;

	point[1][0] = x+width;
	point[1][1] = y;

	point[2][0] = x+width;
	point[2][1] = y+height;

	point[3][0] = x;
	point[3][1] = y+height;
}

iS32 public_draw_image(public_image_struct *image, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h)
{
	iRECT dstRect;
	iRECT srcRect;
	iS32 ret;

	if(src_w == SRC_IMAGE_WIDTH)
	{
		src_w = image->w;
	}
	if(src_h == SRC_IMAGE_HEIGHT)
	{
		src_h = image->h;
	}
	public_build_rect(&dstRect, dst_x, dst_y, PK_SCREEN_WIDTH, PK_SCREEN_HEIGHT);
	public_build_rect(&srcRect, src_x, src_y, src_w, src_h);

	ret = i51KitG2DrawImage(image->pContainer, image->id, &dstRect, &srcRect);
	//i51KitG2Update();
	return ret;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////字体部分/////////////////////////////////////////////////////////

iFILE public_font_init(iU16 *font_re)
{
	pFont = i51KitStdResOpen(font_re);
	i51AdeOsLog(0, "初始化字体文件句柄：%d", pFont);
	return pFont;
}

iBOOL public_font_close(void)
{
	iFILE temp;

	if(pFont == iNULL)
		return iFALSE;

	temp  = pFont;
	pFont = 0;
	return i51KitStdResClose(temp);
}

iBOOL public_draw_text(iCHAR *text, iS16 dst_x, iS16 dst_y, iU16 length, iCOLOR color)
{
	iRECT rect;

	public_build_rect(&rect, dst_x, dst_y, PK_SCREEN_WIDTH, PK_FONT_HEIGHT);
	return i51KitG2FontDraw (pFont, 0, (void *)((iU16 *)text), length, &rect, iNULL, i51KITG2_FONT_WORDS_ROTATE_0, 0, 0, color, 0, 0);
}

iBOOL public_draw_text_rotate_90(iCHAR *text, iS16 dst_x, iS16 dst_y, iU16 length, iCOLOR color)
{
	iRECT rect;

	public_build_rect(&rect, dst_x, dst_y, PK_FONT_HEIGHT, PK_SCREEN_HEIGHT);
	return i51KitG2FontDraw (pFont, 0, (void *)((iU16 *)text), length, &rect, iNULL, i51KITG2_FONT_WORDS_ROTATE_90, 0, 0, color, 0, 0);
}

iBOOL public_draw_string_text(iCHAR *text, iS16 dst_x, iS16 dst_y, iS16 width, iS16 height, iCOLOR color, iU16 spacing)
{
	iRECT rect;

	public_build_rect(&rect, dst_x, dst_y, width, height);
	return i51KitG2FontDraw (pFont, 0, (void *)((iU16 *)text), 0, &rect, iNULL, i51KITG2_FONT_WORDS_ROTATE_0, spacing, 0, color, 0, 0);
}

iBOOL public_draw_string_text_rotate_90(iCHAR *text, iS16 dst_x, iS16 dst_y, iS16 width, iS16 height, iCOLOR color, iU16 spacing)
{
	iRECT rect;

	public_build_rect(&rect, dst_x, dst_y, width, height);
	return i51KitG2FontDraw (pFont, 0, (void *)((iU16 *)text), 0, &rect, iNULL, i51KITG2_FONT_WORDS_ROTATE_90, spacing, 0, color, 0, 0);
}

iBOOL public_draw_text_x_middle(iCHAR *text, iS16 middle_x, iS16 dst_y, iCOLOR color)
{
	iS16 dst_x;
	iU16 len;

	len = strlen((char *)text);
	dst_x = middle_x - len*PK_FONT_HEIGHT/4;
	return public_draw_text(text, dst_x, dst_y, 0, color);
}

iBOOL public_draw_text_y_middle(char *text, iS16 dst_x, iS16 middle_y, iCOLOR color)
{
	iS16 dst_y;
	iU16 len;

	len = strlen(text);
	dst_y = middle_y - len*PK_FONT_HEIGHT/4;
	return public_draw_text(text, dst_x, dst_y, 0, color);
}

iBOOL public_draw_text_y_middle_rotate_90(char *text, iS16 dst_x, iS16 dst_y, iCOLOR color)
{
	iS16 i_dst_y = 0;
	iU16 length = 0;
	iU16 width  = PK_SCREEN_WIDTH;
	iU16 height = PK_SCREEN_HEIGHT;

	length = strlen(text) * PK_FONT_HEIGHT / 2;
	if(length >= height)
		return 0;

	i_dst_y = dst_y - length / 2;
	return public_draw_text_rotate_90(text, dst_x, i_dst_y, 0, color);
}

iBOOL public_draw_string_text_y_middle(char *text, iS16 dst_x, iS16 middle_y, iCOLOR color, iU16 spacing)
{
	iU16 width;
	iU16 height;
	iU16 RowWordLen = 0;
	iS16 dst_y;
	iU16 column = 0;
	iU16 screen_w = PK_SCREEN_WIDTH;

	width      = screen_w - 2 * dst_x;
	RowWordLen = width / PK_FONT_HEIGHT;
	column     = strlen(text) / 2 / RowWordLen + 1;
	height     = column * (spacing + PK_FONT_HEIGHT) / 2;
	dst_y      = middle_y - height;
	return public_draw_string_text(text, dst_x, dst_y, width, 2*height, color, spacing);
}

void public_draw_string_text_x_middle_rotate_90(char *text, iS16 middle_x, iS16 dst_y, iU32 color, iU32 spacing)
{
	iU16 width;
	iU16 height;
	iU16 RowWordLen = 0;
	iS16 dst_x;
	iU16 row = 0;
	iU16 screen_h = PK_SCREEN_HEIGHT;

	height     = screen_h - 2 * dst_y;
	RowWordLen = height / PK_FONT_HEIGHT;
	row        = strlen(text) / 2 / RowWordLen + 1;
	width      = row * (spacing + PK_FONT_HEIGHT) / 2;
	dst_x      = middle_x - width;
	public_draw_string_text_rotate_90(text, dst_x, dst_y, 2*width, height, color, spacing);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void public_fill_rect(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR color)
{
	iRECT rect;

	public_build_rect(&rect, x, y, width, height);
	//填充
	i51KitG2CleanScreen(color, &rect);
}

void public_draw_frame_rect(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR fill_color, iCOLOR frame_color)
{
	iS32 point[4][2];

	public_build_rect_vertex(point, x, y, width, height);

	//填充
	i51KitG2FillPolygon(4, point, fill_color);

	//画边框
	i51KitG2DrawPolygon(4, point, frame_color);
}

void public_draw_button(char *lsk_word, char *rsk_word, public_image_struct *image)
{
	if(lsk_word != NULL)
	{
#ifdef PKIG_SCREEN_VERTICAL /*PK_SCREEN_WIDTH == 320 && PK_SCREEN_HEIGHT == 240*/
		public_draw_image(image, 7, PK_SCREEN_HEIGHT-3-image->h, 0, 0, image->w, image->h);
		public_draw_text(lsk_word, (image->w-24)/2+7, PK_SCREEN_HEIGHT-image->h/2-9, 0, 0xffff);
#else
		public_draw_image(image, 3, 7, 0, 0, image->w, image->h);
		public_draw_text_rotate_90(lsk_word, (image->w-12)/2+5, (image->h-strlen(lsk_word)*6)/2+7, 0, 0xffff);
#endif
	}

	if(rsk_word != NULL)
	{
#ifdef PKIG_SCREEN_VERTICAL /*PK_SCREEN_WIDTH == 320 && PK_SCREEN_HEIGHT == 240*/
		public_draw_image(image, PK_SCREEN_WIDTH-7-image->w, PK_SCREEN_HEIGHT-3-image->h, 0, 0, image->w, image->h);
		public_draw_text(rsk_word, PK_SCREEN_WIDTH-image->w/2-19, PK_SCREEN_HEIGHT-image->h/2-9, 0, 0xffff);
#else
		public_draw_image(image, 3, PK_SCREEN_HEIGHT-7-image->h, 0, 0, image->w, image->h);
		public_draw_text_rotate_90(rsk_word, (image->w-12)/2+5, PK_SCREEN_HEIGHT-7-image->h+(image->h-24)/2, 0, 0xffff);
#endif
	}
}

void public_draw_score(iU32 score, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h, public_image_struct *image)
{
	char str[20];
	iS32 i;

	public_itoa(score, str);
	for(i=0;str[i] != '\0';i++)
	{
#ifdef PKIG_SCREEN_VERTICAL /*PK_SCREEN_WIDTH == 320 && PK_SCREEN_HEIGHT == 240*/
		public_draw_image(image, dst_x+i*src_w, dst_y, src_x, src_y+(str[i]-48)*src_h, src_w, src_h);
#else
		public_draw_image(image, dst_x, dst_y+i*src_h, src_x, src_y+(str[i]-48)*src_h, src_w, src_h);           
#endif
	}
}

iU8 public_pen_enabled_rect(iU16 pen_x, iU16 pen_y, iU16 min_x, iU16 max_x, iU16 min_y, iU16 max_y)
{
	if(pen_x >= min_x && pen_x <= max_x && pen_y >= min_y && pen_y <= max_y)
	{
		return 1;
	}
	return 0;
}

iS32 public_write_file(char *file_name, void *data, iU32 size)
{
	iU16 file[66];
	int  fd;
	iU32 Wsize = 0;

	i51AdeStdWSprintf(file, 66, "%s", file_name);
	fd = i51AdeOsFileOpen(file, i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE, __FILE__ , __LINE__);
	if(fd <= 0)
	{
		return fd;
	}
	else
	{
		i51AdeOsFileWrite(fd, data, size, &Wsize);
		i51AdeOsFileClose( fd );
		return iTRUE;
	}
}

iS32 public_read_file(char *file_name, void *data)
{
	iU16 file[66];
	int fd;
	iU32 Rsize;
	int fdLen;

	i51AdeStdWSprintf(file, 66, "%s", file_name);
	fd = i51AdeOsFileOpen(file, i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__, __LINE__);
	if(fd <= 0)
	{
		return fd;
	}
	else 
	{
		i51AdeOsFileGetSize(fd, &fdLen);
		i51AdeOsFileRead(fd, (void *)data, fdLen, &Rsize);
		i51AdeOsFileClose(fd);
		return iTRUE;
	}
}

iS32 public_delect_file(char *dir, char *filename)
{
	iU16 file[66];

	if(check_file_is_exist(dir, filename))
	{
		i51AdeStdWSprintf(file, 66, "%s\\%s", dir, filename);
		return i51AdeOsFileDelete(file);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////播放声音部分//////////////////////////
static audio_struct audio[AUDIO_PLAYER_NUM_MAX] = {0};

iBOOL public_audio_init(void (*cb)(iBOOL results))    
{
	iBOOL ret = iFALSE;

	ret = i51AdeMmiMediaPlayerCreate(cb);
	
	return iTRUE;
}

void public_audio_all_exit(void)
{
	iU32 channel;

	for(channel = 0; channel < AUDIO_PLAYER_NUM_MAX; channel++)
	{
		if(audio[channel].loop && audio[channel].playing)
		{
			audio[channel].resume = iTRUE;
		}
		audio[channel].playing = iFALSE;
		i51AdeMmiMediaPlayerStop(audio[channel].p);
	}

	i51AdeMmiMediaPlayerRelease();
}

void audio_resume(iBOOL results)
{
	iU32 channel;

	if(!results)
		return;

	for(channel = 0; channel < AUDIO_PLAYER_NUM_MAX; channel++)
	{
		if(audio[channel].resume)
		{
			public_audio_play(audio[channel].priority, audio[channel].p, audio[channel].leng, audio[channel].mem_or_file, channel, audio[channel].loop);
			audio[channel].resume = iFALSE;
		}
	}
}

void audio_play_cb(void* handle)
{
	iU32 channel;

	if(handle == iNULL)
		return;

	i51AdeOsLog(0, "播放声音完毕回调");
	for(channel = 0; channel < AUDIO_PLAYER_NUM_MAX; channel++)
	{
		if(handle == audio[channel].p)
		{
			memset(&audio[channel], 0, sizeof(audio_struct));
			break;
		}
	}
}

iBOOL public_audio_play(iU32 priority, void *audio_data, iU32 audio_len, i51MMIDEC mem_or_file, iU32 channel, iBOOL loop)
{
	i51METYPE audio_type;
	iBOOL ret = iFALSE;

	if(iNULL == audio_data)  //声音为空
		return iNULL;

	i51AdeMmiMediaPlayerStop(audio[channel].p);  //关闭当前通道正在播放的声音
	if(channel >= AUDIO_PLAYER_WAV_0)
	{
		audio_type = i51_MEDIA_TYPE_WAV;
	}
	else
	{
		audio_type = i51_MEDIA_TYPE_MIDI;
	}
	ret = i51AdeMmiMediaPlayerPlay( priority, audio_data, audio_len, audio_type, mem_or_file, loop, audio_play_cb);  //播放声音

	if(ret)
	{
		audio[channel].p           = audio_data;
		audio[channel].leng        = audio_len;
		audio[channel].loop        = loop;
		audio[channel].playing     = iTRUE;
		audio[channel].mem_or_file = mem_or_file;
		audio[channel].priority    = priority;

	}
	return ret;
}

iBOOL public_audio_stop(audio_player_type player_type) 
{
	audio[player_type].playing = iFALSE;
	return i51AdeMmiMediaPlayerStop(audio[player_type].p);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void draw_wait_link(void)
//{
//	static int link_frame = 0;
//	char text[4][33] = {"读取中，请稍候", "读取中，请稍候.", "读取中，请稍候..", "读取中，请稍候..."};
//
//	//UI_frame_init(TIPS_NULL, iNULL);
//	UI_frame_render();
//
//	public_draw_text(text[link_frame], PK_SCREEN_WIDTH/2-strlen(text[3])*PK_FONT_HEIGHT/4, PK_SCREEN_HEIGHT/2-15, 0, 0xffff);
//	if(link_frame < 3)
//	{
//		link_frame++;
//	}
//	else
//	{
//		link_frame = 0;
//	}
//}

iU8 save_screen(void)
{
	iU32 * LcdInfoArr;
	iFILE DesFile = iNULL;
	iBOOL FlagReturn = iFALSE;
	iU32 LengthPartDes = 0;
	iU32 LengFactWriteData = 0;
	iU16 ScreenWidth = 0;
	iU16 ScreenHeight = 0;
	iU16 filename[66];

	LcdInfoArr =  i51AdeMmiGetPanel();

	if( iNULL == LcdInfoArr )goto ERROR;

	LengthPartDes = LcdInfoArr[2]*LcdInfoArr[3]*2;

	i51AdeStdWSprintf(filename, 66, "%s\\%s", GAME_DIR, SCREEN_CONTENT_FILE);
	DesFile  = i51AdeOsFileOpen( filename, i51_OS_FL_WRITE |i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ );
	if (iNULL == DesFile)goto ERROR;
	FlagReturn = i51AdeOsFileSetSeek( DesFile, 0, i51_OS_FL_SEEK_HEAD) ;
	if( iFALSE == FlagReturn ) goto ERROR;
	FlagReturn = i51AdeOsFileWrite( DesFile, (void *)LcdInfoArr[1], LengthPartDes, &LengFactWriteData );
	if( iFALSE == FlagReturn ) goto ERROR;
	i51AdeOsFileClose(DesFile);
	return iTRUE;
ERROR:
	i51AdeOsLog(0,"WriteCurLcdBufferToFile  Error!");
	return iTRUE;
}
iU8 resume_screen(void)
{
	iU32 * LcdInfoArr;
	iFILE DesFile = iNULL;
	iBOOL FlagReturn = iFALSE;
	iU32 LengthPartDes = 0;
	iU32 LengFactReadData = 0;
	iU16 ScreenWidth = 0;
	iU16 ScreenHeight = 0;
	iU16 filename[66];

	LcdInfoArr =  i51AdeMmiGetPanel();

	if( iNULL == LcdInfoArr )goto ERROR;

	LengthPartDes = LcdInfoArr[2]*LcdInfoArr[3]*2;

	i51AdeStdWSprintf(filename, 66, "%s\\%s", GAME_DIR, SCREEN_CONTENT_FILE);
	DesFile  = i51AdeOsFileOpen(filename, i51_OS_FL_READ |i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
	if (iNULL == DesFile)goto ERROR;
	FlagReturn = i51AdeOsFileSetSeek( DesFile, 0, i51_OS_FL_SEEK_HEAD) ;
	if( iFALSE == FlagReturn ) goto ERROR;
	FlagReturn = i51AdeOsFileRead( DesFile, (void *)LcdInfoArr[1], LengthPartDes, &LengFactReadData );
	if( iFALSE == FlagReturn ) goto ERROR;
	i51AdeOsFileClose(DesFile);
	i51AdeMmiUpdateScreen();
	return iTRUE;
ERROR:
	i51AdeOsLog(0,"DrawScreenFromFile  Error!");
	return iTRUE;
}

iBOOL time_is_second(void)
{
	iU32 cur_time = 0;
	static iU32 last_time = 0;

	cur_time = i51AdeOsGetTick();
	if(cur_time - last_time >= 1000)
	{
		last_time = cur_time;
		return iTRUE;
	}

	return iFALSE;
}

////////////////////////////////////////////// 创建链路 //////////////////////////////////////////////////////


void kernel_send_request(iU32 cmd)
{
	char str[33];
	iU16 app_name[66];

	strcpy(str, GAME_DIR);
	i51AdeStdWSprintf(app_name, 66, "%s", str);
//	i51AdeOsLog(0, "退出参数 = %s", Narrows(str,app.app_param.srcapp));
//	if(app.app_param.srcapp[0])
	{
//		i51KernelSendRequest ( app.aid, app.app_param.srcapp, i51_REQ_LOADEX, &app.app_param, sizeof(app_param_struct));
	}
//	else
	{
//		i51KernelSendRequest ( app.aid, app_name, cmd, 0, 0);
	}
}

void public_pfunc_init(i51Public_struct *func)
{
	func->public_draw_text = public_draw_text;
	func->public_draw_string_text = public_draw_string_text;
	func->public_draw_text_x_middle = public_draw_text_x_middle;
	func->public_draw_text_y_middle = public_draw_text_y_middle;
	func->public_draw_string_text_y_middle = public_draw_string_text_y_middle;
	func->public_draw_text_rotate_90 = public_draw_text_rotate_90;
	func->public_draw_string_text_rotate_90 = public_draw_string_text_rotate_90;
	func->public_draw_string_text_x_middle_rotate_90 = public_draw_string_text_x_middle_rotate_90;
	func->public_draw_text_y_middle_rotate_90 = public_draw_text_y_middle_rotate_90;
	func->public_font_init = public_font_init;

	func->public_draw_frame_rect = public_draw_frame_rect;
	func->public_fill_rect       = public_fill_rect;

	func->public_image_init = public_image_init;
	func->public_draw_image = public_draw_image;
	func->public_draw_score = public_draw_score;
	func->public_draw_button = public_draw_button;

	func->public_read_file = public_read_file;
	func->public_write_file = public_write_file;
	func->find_aim_dir = find_aim_dir;
	func->check_file_is_exist = check_file_is_exist;
	func->public_delect_file = public_delect_file;

	func->public_build_point = public_build_point;
	func->public_build_rect  = public_build_rect;
	func->public_build_rect_vertex = public_build_rect_vertex;
	func->public_itoa = public_itoa;
	func->public_pen_enabled_rect = public_pen_enabled_rect;

	func->public_audio_play = public_audio_play;
	func->public_audio_stop = public_audio_stop;

	/*func->UI_image_init = UI_image_init;
	func->UI_frame_msg_init = UI_frame_msg_init;
	func->UI_frame_init = UI_frame_init;
	func->UI_frame_render = UI_frame_render;
	func->UI_frame_input_key = UI_frame_input_key;
	func->UI_frame_input_pen = UI_frame_input_pen;*/

	func->llp_get_input = llp_get_input;

	//func->load_fee_app = load_fee_app;

	/*func->hostname = hostname;
	func->probuf_pop_short = probuf_pop_short;
	func->probuf_pop_byte = probuf_pop_byte;
	func->probuf_pop_long = probuf_pop_long;
	func->probuf_pop_string = probuf_pop_string;
	func->probuf_init = probuf_init;
	func->probuf_push = probuf_push;
	func->probuf_push_short = probuf_push_short;
	func->probuf_push_long = probuf_push_long;
	func->probuf_push_string = probuf_push_string;
	func->pkig_httpc_postbuf = pkig_httpc_postbuf;*/


	//func->unload_kit_msg = unload_kit_msg;
	func->kernel_send_request = kernel_send_request;
	/*func->set_game_run_mode = set_game_run_mode;
	func->get_game_run_mode = get_game_run_mode;
	func->update_check = update_check;
	func->set_sound_state = set_sound_state;
	func->get_sound_state = get_sound_state;
	func->fee_pay = fee_pay;
	func->fee_node_init = fee_node_init;
	func->fee_node_save = fee_node_save;*/
}

//void public_pfunc_http_init(http_public_struct *func)
//{
//	func->create_link_layer = create_link_layer;
//	func->get_link_layer_status = get_link_layer_status;
//
//	func->httpc_error_string = httpc_error_string;
//	func->probuf_pop_short = probuf_pop_short;
//	func->probuf_pop_byte = probuf_pop_byte;
//	func->probuf_pop_long = probuf_pop_long;
//	func->probuf_pop_string = probuf_pop_string;
//	func->probuf_init = probuf_init;
//	func->probuf_push = probuf_push;
//	func->probuf_push_short = probuf_push_short;
//	func->probuf_push_long = probuf_push_long;
//	func->probuf_push_string = probuf_push_string;
//	func->pkig_httpc_postbuf = pkig_httpc_postbuf;
//}