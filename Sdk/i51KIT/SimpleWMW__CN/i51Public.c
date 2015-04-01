#include "i51Public.h"
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
	sprintf( str, "%d", value);
}
void public_image_init(public_image_struct *image, iU32 image_id, iU32 image_data)
{
	if(image_data)
	{
		if(image->pContainer != iNULL) 
		{
			i51AdeOsLog(0, "图像初始化,id = %d", image_id);
			return;
		}
		image->pContainer = i51KitG2ImageCreate(image_data);
		image->id         = image_id;
		i51KitG2GetImageAttrEx(image_data, (iU32 *)&image->w, (iU32 *)&image->h);
	}
}

void public_image_destroy(public_image_struct *image)
{
	if(image->pContainer)
	{
		int ret;
		ret = i51KitG2ImageDestory(image->pContainer);
		if(!ret)
		iLog("释放内存失败,id=%d", image->id);
		image->pContainer = iNULL;
	}
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

iS32 public_draw_image(public_image_struct *image, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h)
{
	iRECT dstRect;
	iRECT srcRect;
	iS32 ret;

	if(!image->pContainer || !image)
		return -1;
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


	ret = i51KitG2DrawImageEx((iU32)image->pContainer, &srcRect, &dstRect);
	//i51AdeMmiUpdateScreen();
	return ret;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////字体部分/////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////

void public_draw_line(iS32 x0, iS32 y0, iS32 x1, iS32 y1, iCOLOR color)
{
	iS32 p[2][2];

	p[0][0] = x0;
	p[0][1] = y0;
	p[1][0] = x1;
	p[1][1] = y1;
	//i51KitG2DrawLineEx(p, color);
}

void public_fill_rect(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR color)
{
	iRECT rect;

	public_build_rect(&rect, x, y, width, height);
	//填充
	i51KitG2CleanScreenEx(color, &rect);
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

	i51AdeStdWSprintf(file, 66*2, "%s", file_name);
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
//static audio_struct audio[AUDIO_PLAYER_NUM_MAX] = {0};
audio_struct audio[AUDIO_PLAYER_NUM_MAX] = {0};


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
			public_audio_play_x(audio[channel].priority, audio[channel].p, audio[channel].leng, audio[channel].mem_or_file, channel, audio[channel].loop);
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

iBOOL public_audio_play_x(iU32 priority, void *audio_data, iU32 audio_len, i51MMIDEC mem_or_file, iU32 channel, iBOOL loop)
{
	i51METYPE audio_type;
	iBOOL ret = iFALSE;

	if(iNULL == audio_data)  //声音为空
		return iNULL;

	i51AdeMmiMediaPlayerStop(audio[channel].p);  //关闭当前通道正在播放的声音



	//if(channel >= AUDIO_PLAYER_WAV_0)
	//{
	//	audio_type = i51_MEDIA_TYPE_WAV;
	//}
	//else if (channel == AUDIO_PLAYER_MP3)
	//{
	//	audio_type = i51_MEDIA_TYPE_MP3;
	//}


	if(channel == AUDIO_PLAYER_MP3)
	{
		audio_type = i51_MEDIA_TYPE_MP3;
	}
	else if (channel >= AUDIO_PLAYER_WAV_0)
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


iS32 SWMW_i51KitG2DrawImage(public_image_struct *image, iRECT *dstRect, iRECT *srcRect)
{
	iRECT dst;
	iRECT src;
	iS32 ret;

	if(!image->pContainer || !image)
		return -1;

	if (srcRect == iNULL)
	{
		public_build_rect(&src, 0, 0, image->w, image->h);
	}
	else
	{
		public_build_rect(&src, srcRect->X, srcRect->Y, srcRect->Width, srcRect->Height);
	}
	if(dstRect == iNULL)
	{
		public_build_rect(&dst, 0, 0, PK_SCREEN_WIDTH, PK_SCREEN_HEIGHT);
	}
	else
	{
		public_build_rect(&dst, dstRect->X, dstRect->Y, dstRect->Width, dstRect->Height);
	}
	//if(src_w == SRC_IMAGE_WIDTH)
	//{
	//	src_w = image->w;
	//}
	//if(src_h == SRC_IMAGE_HEIGHT)
	//{
	//	src_h = image->h;
	//}


	ret = i51KitG2DrawImageEx((iU32)image->pContainer, &src, &dst);
	//i51AdeMmiUpdateScreen();
	return ret;

}
