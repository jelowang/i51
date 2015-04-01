#include "i51KitAde.h"
//#include "i51KitG2.h"
#include "i51Public.h"
#include "mwPublic.h"
#include "mwLevelPublic.h"
//#include "Image_Hex.h"
#include "PK_Rank.h"


i51KIT_STATIC_KIT_DEF(Adapter);
//i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
//i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

// 世界指针
iWorld *mwSpace = iNULL;
// 屏幕指针
iU16* mwScreenBuffer = iNULL;
// 获得的按键数据
LLPInputStruct mwKeyData;
// 图片容器句柄
iHANDLE mwContainer = iNULL;

//存放图片的结构体
public_image_struct iMageInfo[MAX_ID_WMW];

iBOOL iScreenFlag = iFALSE;
static iS32 gFlag = 1;
iU32 game_aid =0;
iBOOL MyWater_Init( void );

void  wmw_audio_init();



void portal_cb(iBOOL data)
{

	if (data ==iFALSE)
	{
		//失败回主界面
		wmw_image_destroy();
		SoundChoosePlay = iFALSE;
		MusicCloseFlag = iFALSE;
		wmw_audio_init();


		MyWater_Init();

	}
	i51AdeOsLog(0, "下载portal，回调， result = %d", data);
}


void wmw_portal_ui()
{
	iBOOL portalret;
	portalret =i51KernelCheckAppCompleten((iU16 *)L"Portal");
	if (portalret)
	{    //卸载自己游戏，加载portal

		public_audio_all_exit();

		wmw_image_destroy();

		i51KernelSendRequest (game_aid, (iU16 *)L"Portal", i51_REQ_LOADEX, 0, 0);
	} 
	else
	{    //加载图片
#ifdef LANGUAGE_CHINESE
		wmw_image_destroy();
		wmw_partol_page();
		CurrGameState = GAME_STATE_PARTOL;
		iScreenFlag = iTRUE;
#endif
	}
}



void wmw_main_page()
{
	public_image_init(&iMageInfo[Main], Main, (iU32)(Main_Hex));
	public_image_init(&iMageInfo[MainLeadWait00], MainLeadWait00, (iU32)(MainLeadWait00_Hex));
	public_image_init(&iMageInfo[MainLeadWait01], MainLeadWait01, (iU32)(MainLeadWait01_Hex));
	//public_image_init(&iMageInfo[MainLeadWait02], MainLeadWait02, (iU32)(MainLeadWait02_Hex));
	//public_image_init(&iMageInfo[MainLeadWait03], MainLeadWait03, (iU32)(MainLeadWait03_Hex));
	//public_image_init(&iMageInfo[MainLeadWait04], MainLeadWait04, (iU32)(MainLeadWait04_Hex));
	//public_image_init(&iMageInfo[MainLeadWait05], MainLeadWait05, (iU32)(MainLeadWait05_Hex));
	//public_image_init(&iMageInfo[MainLeadWait06], MainLeadWait06, (iU32)(MainLeadWait06_Hex));
	//public_image_init(&iMageInfo[MainLeadWait07], MainLeadWait07, (iU32)(MainLeadWait07_Hex));
	//public_image_init(&iMageInfo[MainLeadWait08], MainLeadWait08, (iU32)(MainLeadWait08_Hex));
	//public_image_init(&iMageInfo[MainLeadWait09], MainLeadWait09, (iU32)(MainLeadWait09_Hex));
	//public_image_init(&iMageInfo[MainLeadWait10], MainLeadWait10, (iU32)(MainLeadWait10_Hex));
	//public_image_init(&iMageInfo[MainLeadWait11], MainLeadWait11, (iU32)(MainLeadWait11_Hex));

}

void wmw_choose_page()
{
	public_image_init(&iMageInfo[ChsLvBath], ChsLvBath, (iU32)(ChsLvBath_Hex));
	public_image_init(&iMageInfo[ChooseBKHead], ChooseBKHead, (iU32)(ChooseBKHead_Hex));
	public_image_init(&iMageInfo[ChooseBKBlock], ChooseBKBlock, (iU32)(ChooseBKBlock_Hex));
	public_image_init(&iMageInfo[ChooseWater], ChooseWater, (iU32)(ChooseWater_Hex));
	public_image_init(&iMageInfo[DuckGet], DuckGet, (iU32)(DuckGet_Hex));
	public_image_init(&iMageInfo[ChsLv], ChsLv, (iU32)(ChsLv_Hex));
	public_image_init(&iMageInfo[Return], Return, (iU32)(Return_Hex));
	public_image_init(&iMageInfo[NumBig], NumBig, (iU32)(NumBig_Hex));
	public_image_init(&iMageInfo[NumFrame], NumFrame, (iU32)(NumFrame_Hex));
	public_image_init(&iMageInfo[NumSmall], NumSmall, (iU32)(NumSmall_Hex));

}

void wmw_partol_page()
{
	//public_image_init(&iMageInfo[Main], Main, (iU32)(Main_Hex));
	public_image_init(&iMageInfo[PromptFrame], PromptFrame, (iU32)(PromptFrame_Hex));
}

void wmw_game_page()
{

	public_image_init(&iMageInfo[LeadWait00], LeadWait00, (iU32)(LeadWait00_Hex));
	public_image_init(&iMageInfo[LeadWait01], LeadWait01, (iU32)(LeadWait01_Hex));
	//public_image_init(&iMageInfo[LeadWait02], LeadWait02, (iU32)(LeadWait02_Hex));
	//public_image_init(&iMageInfo[LeadWait03], LeadWait03, (iU32)(LeadWait03_Hex));
	//public_image_init(&iMageInfo[LeadWait04], LeadWait04, (iU32)(LeadWait04_Hex));
	//public_image_init(&iMageInfo[LeadWait05], LeadWait05, (iU32)(LeadWait05_Hex));
	//public_image_init(&iMageInfo[LeadWait06], LeadWait06, (iU32)(LeadWait06_Hex));
	//public_image_init(&iMageInfo[LeadWait07], LeadWait07, (iU32)(LeadWait07_Hex));
	//public_image_init(&iMageInfo[LeadWait08], LeadWait08, (iU32)(LeadWait08_Hex));
	//public_image_init(&iMageInfo[LeadWait09], LeadWait09, (iU32)(LeadWait09_Hex));
	//public_image_init(&iMageInfo[LeadWait10], LeadWait10, (iU32)(LeadWait10_Hex));
	public_image_init(&iMageInfo[LevelNum], LevelNum, (iU32)(LevelNum_Hex));
	public_image_init(&iMageInfo[BKwall], BKwall, (iU32)(BKwall_Hex));
	public_image_init(&iMageInfo[DuckGet], DuckGet, (iU32)(DuckGet_Hex));
	public_image_init(&iMageInfo[WaterDuck], WaterDuck, (iU32)(WaterDuck_Hex));
	public_image_init(&iMageInfo[WordLevel], WordLevel, (iU32)(WordLevel_Hex));
	public_image_init(&iMageInfo[BlackBK], BlackBK, (iU32)(BlackBK_Hex));
	public_image_init(&iMageInfo[Pause], Pause, (iU32)(Pause_Hex));
	public_image_init(&iMageInfo[Restart], Restart, (iU32)(Restart_Hex));


	switch( CurrLevelNum )
	{
	case MW_LEVEL_001:

		public_image_init(&iMageInfo[RockLevel001], RockLevel001, (iU32)(RockLevel001_Hex));
		public_image_init(&iMageInfo[MudLevel001], MudLevel001, (iU32)(MudLevel001_Hex));

		break;

	case MW_LEVEL_002:
		public_image_init(&iMageInfo[RockLevel002], RockLevel002, (iU32)(RockLevel002_Hex));
		public_image_init(&iMageInfo[MudLevel002], MudLevel002, (iU32)(MudLevel002_Hex));

		break;

	case MW_LEVEL_003:
		public_image_init(&iMageInfo[RockLevel003], RockLevel003, (iU32)(RockLevel003_Hex));
		public_image_init(&iMageInfo[MudLevel003], MudLevel003, (iU32)(MudLevel003_Hex));

		break;

	case MW_LEVEL_004:
		public_image_init(&iMageInfo[RockLevel004], RockLevel004, (iU32)(RockLevel004_Hex));
		public_image_init(&iMageInfo[MudLevel004], MudLevel004, (iU32)(MudLevel004_Hex));
		public_image_init(&iMageInfo[Algae], Algae, (iU32)(Algae_Hex));
		break;

	case MW_LEVEL_005:
		public_image_init(&iMageInfo[RockLevel006], RockLevel006, (iU32)(RockLevel006_Hex));
		public_image_init(&iMageInfo[MudLevel006], MudLevel006, (iU32)(MudLevel006_Hex));

		break;

	case MW_LEVEL_006:
		public_image_init(&iMageInfo[RockLevel009], RockLevel009, (iU32)(RockLevel009_Hex));
		public_image_init(&iMageInfo[MudLevel009], MudLevel009, (iU32)(MudLevel009_Hex));
		public_image_init(&iMageInfo[Organ], Organ, (iU32)(Organ_Hex));
		

		break;

	case MW_LEVEL_007:
		public_image_init(&iMageInfo[RockLevel011], RockLevel011, (iU32)(RockLevel011_Hex));
		public_image_init(&iMageInfo[MudLevel011], MudLevel011, (iU32)(MudLevel011_Hex));

		break;

	case MW_LEVEL_008:
		public_image_init(&iMageInfo[RockLevel013], RockLevel013, (iU32)(RockLevel013_Hex));
		public_image_init(&iMageInfo[MudLevel013], MudLevel013, (iU32)(MudLevel013_Hex));
		public_image_init(&iMageInfo[Algae], Algae, (iU32)(Algae_Hex));

		break;

	//case MW_LEVEL_009:
	//	break;

	//case MW_LEVEL_010:
	//	break;

	//case MW_LEVEL_011:
	//	break;

	//case MW_LEVEL_012:
	//	break;

	//case MW_LEVEL_013:
	//	break;

	//case MW_LEVEL_014:
	//	break;


	//case MW_LEVEL_015:
	//	break;
	}

	public_image_init(&iMageInfo[LeadBathe00], LeadBathe00, (iU32)(LeadBathe00_Hex));
	public_image_init(&iMageInfo[LeadBathe01], LeadBathe01, (iU32)(LeadBathe01_Hex));
	public_image_init(&iMageInfo[LeadBathe02], LeadBathe02, (iU32)(LeadBathe02_Hex));
	public_image_init(&iMageInfo[PsMain], PsMain, (iU32)(PsMain_Hex));

}

void wmw_pause_page()
{
//	public_image_init(&iMageInfo[PsMain], PsMain, (iU32)(PsMain_Hex));

}

void wmw_win_page()
{
	//public_image_init(&iMageInfo[LeadBathe00], LeadBathe00, (iU32)(LeadBathe00_Hex));
	//public_image_init(&iMageInfo[LeadBathe01], LeadBathe01, (iU32)(LeadBathe01_Hex));
	//public_image_init(&iMageInfo[LeadBathe02], LeadBathe02, (iU32)(LeadBathe02_Hex));
}

void wmw_destroy_win_page()
{
	public_image_destroy(&iMageInfo[LeadBathe00]);
	public_image_destroy(&iMageInfo[LeadBathe01]);
	public_image_destroy(&iMageInfo[LeadBathe02]);
}


void wmw_result_page()
{
	public_image_init(&iMageInfo[WinWord], WinWord, (iU32)(WinWord_Hex));
	public_image_init(&iMageInfo[WinAgain], WinAgain, (iU32)(WinAgain_Hex));
	public_image_init(&iMageInfo[PsMain], PsMain, (iU32)(PsMain_Hex));
	public_image_init(&iMageInfo[WinCrocodile], WinCrocodile, (iU32)(WinCrocodile_Hex));
	public_image_init(&iMageInfo[WinDuck], WinDuck, (iU32)(WinDuck_Hex));
	public_image_init(&iMageInfo[NumBig], NumBig, (iU32)(NumBig_Hex));
	public_image_init(&iMageInfo[NumSmall], NumSmall, (iU32)(NumSmall_Hex));


}
void wmw_destroy_result_page()
{
	public_image_destroy(&iMageInfo[WinWord]);
	public_image_destroy(&iMageInfo[WinAgain]);
	public_image_destroy(&iMageInfo[PsMain]);
	public_image_destroy(&iMageInfo[WinCrocodile]);
	public_image_destroy(&iMageInfo[WinDuck]);
	public_image_destroy(&iMageInfo[NumBig]);
	public_image_destroy(&iMageInfo[NumSmall]);


}

#if 0 //xie
void Process(iU32 Step , iU32 Totall , iHANDLE Handle )
{
	static iS16 Count;
	static iU32 OldTime = 0;
	
	iU32 ImageW = 0;
	iU32 ImageH = 0;
	iRECT DesRect;
	iU32 ImageId[4] = {0,1,2,3};
	iS16 X = 0, Y = 0;
	
	mwContainer= Handle;
	if(Step>=4)
	{

		//i51KitG2SetImageMemoryCopy(Handle, 4, ImageId );

		//i51KitG2GetImageAttr(Handle, 0, &ImageW, &ImageH, iNULL);

		for( Y = 0; Y<SCREEN_HEIGHT; Y+=ImageH )
		{
			for( X = 0; X<SCREEN_WIDTH; X+=ImageW )
			{
				DesRect.X = X;
				DesRect.Y = Y;
				DesRect.Width = ImageW;
				DesRect.Height = ImageH;
				i51KitG2DrawImage(Handle, 0, &DesRect, iNULL);
			}
		}

		//i51KitG2GetImageAttr(Handle, 1, &ImageW, &ImageH, iNULL);
		DesRect.X = 95;
		DesRect.Y = 311;
		DesRect.Width = ImageW;
		DesRect.Height = ImageH;
		i51KitG2DrawImage(Handle, 1, &DesRect, iNULL);
		
		//i51KitG2GetImageAttr(Handle, 2, &ImageW, &ImageH, iNULL);
		DesRect.X = 102;
		DesRect.Y = 318;
		DesRect.Width = ImageW*Step/Totall;
		DesRect.Height = ImageH;
		i51KitG2DrawImage(Handle, 2, &DesRect, iNULL);

		//i51KitG2GetImageAttr(Handle, 3, &ImageW, &ImageH, iNULL);
		DesRect.X = 108;
		DesRect.Y = 366;
		DesRect.Width = 73+(Count%4)*5;
		DesRect.Height = ImageH;
		i51KitG2DrawImage(Handle, 3, &DesRect, iNULL);

		if( i51AdeOsGetTick() - OldTime > 300 )
		{
			Count++;
			OldTime = i51AdeOsGetTick();
		}
		i51KitG2Update();
	}
	
}

void Done( iHANDLE Handle)
{
	if(Handle)
	{
		mwContainer = Handle;
		CurrGameState= GAME_STATE_MAIN;
		//i51KitG2UnsetImageMemoryCopy(Handle, iNULL, iNULL);
//		iU32 ImageId[9] = { PauseShow,PausePress,RestartPress,RestartShow,DuckMud,DuckGet,BlackBK,WordLevel,LevelNum};
//		i51KitG2SetImageMemoryCopy(mwContainer, 9, ImageId );
	}
}


#endif //xie end

void read_local_data(unsigned short *localFile)
{
	int				fd = -1;
	int				fdLen = 0;
	unsigned int	Wsize = 0;

	if(0 == i51AdeOsFileIsExist (localFile))// the file not exist
	{

		CurrTotalDuck = 0;
		UnlockMaxLevel = 1;
		memset(LevelDuckNum, 0x00,TOTAL_LEVEL * sizeof(iU8));


		fd = i51AdeOsFileOpen((unsigned short*)localFile, i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE, __FILE__ , __LINE__);			
		if(fd < 0)
		{
			return;
		}
		else
		{

			//i51AdeOsFileReadInt32(File, &CurrTotalDuck );
			//i51AdeOsFileReadInt32(File, &UnlockMaxLevel  );
			//i51AdeOsFileRead(File, LevelDuckNum, TOTAL_LEVEL, &Read);

			i51AdeOsFileWriteInt32(fd, &CurrTotalDuck );
			i51AdeOsFileWriteInt32(fd, &UnlockMaxLevel  );
			i51AdeOsFileWrite(fd, LevelDuckNum, TOTAL_LEVEL, &Wsize);

			i51AdeOsFileClose( fd );
		}

	}
	else
	{
		fd = i51AdeOsFileOpen(( unsigned short* )localFile, i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );		
		if(fd <= 0)
		{
			CurrTotalDuck = 0;
			UnlockMaxLevel = 1;
			memset(LevelDuckNum, 0x00,TOTAL_LEVEL * sizeof(iU8));
		}
		else
		{
			i51AdeOsFileGetSize(fd, &fdLen);
			if(2*sizeof(iU32) + TOTAL_LEVEL * sizeof(iU8) == fdLen)
			{
				i51AdeOsFileReadInt32(fd, &CurrTotalDuck );
				i51AdeOsFileReadInt32(fd, &UnlockMaxLevel  );
				i51AdeOsFileRead(fd, LevelDuckNum, TOTAL_LEVEL, &Wsize);
				i51AdeOsFileClose(fd);
			}
			else
			{
				i51AdeOsFileClose( fd );

				CurrTotalDuck = 0;
				UnlockMaxLevel = 1;
				memset(LevelDuckNum, 0x00,TOTAL_LEVEL * sizeof(iU8));

				fd = i51AdeOsFileOpen((unsigned short*)localFile, i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE, __FILE__ , __LINE__);			
				if(fd < 0)
				{
					return;
				}
				else
				{

					i51AdeOsFileWriteInt32(fd, &CurrTotalDuck );
					i51AdeOsFileWriteInt32(fd, &UnlockMaxLevel  );
					i51AdeOsFileWrite(fd, LevelDuckNum, TOTAL_LEVEL, &Wsize);

					i51AdeOsFileClose( fd );
				}

			}
		}
	}

}

void check_local_dir(char *dirName)
{
	iU16 dir[256];
	int ret = -1;

	memset((void *)dir, 0, sizeof(dir));
	i51AdeStdWSprintf(dir, 256, "%s",dirName);

	ret = i51AdeOsFileIsDirExist(dir);
	if(0 == ret)
	{
		ret = i51AdeOsFileCreateDir(dir);
	}

}
void MyWater_data_init()
{	
	char fileName[256] = "GameData";
	iU16 file[256];
	char dir[256];

	memset((void *)file, 0, sizeof(file));	
	sprintf(dir, "%s",(char*)APP_NAME);

	check_local_dir(dir);
	i51AdeStdWSprintf(file, 256, "%s\\%s.dat",APP_NAME,fileName);	
	read_local_data((unsigned short *)file); 	

		
}

iBOOL MyWater_Init( void )
{

	iU32 * LcdInfoArr;
	iFILE File;
	iU32 Read;
	
	/// init space.
	mwInitSpace(CurrLevelNum);

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    mwScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	CurrLevelNum = MW_LEVEL_CHOOSE;

	//XIE
	wmw_main_page();
	CurrGameState = GAME_STATE_MAIN;

#if SOUND_NEED
	if(iFALSE==MusicCloseFlag && SoundChoosePlay == iFALSE)

		//if(iFALSE==MusicCloseFlag && SoundWinPlay == iFALSE)
	{
		//SoundWinPlay = iTRUE;

		SoundChoosePlay = iTRUE;

		//pfunc->public_audio_play(
		//	0, 
		//	MW_SOUND_NAME_CHOOSE, 
		//	i51AdeStdWStrlen((iU16*)MW_SOUND_NAME_CHOOSE), 
		//	i51_MMI_DEC_FILE_BASED,
		//	AUDIO_PLAYER_WAV_0, 
		//	iTRUE
		//	);
		//pfunc->public_audio_play(0, SD_CHOOSE, AUDIO_PLAYER_MP3, iTRUE);

		public_audio_play_x(0, wmw_SoundChoose_mp3, wmw_SoundChoose_mp3_len,i51_MMI_DEC_MEM_BASED, AUDIO_PLAYER_MP3, iTRUE);
	}
#endif



	//CurrTotalDuck = 0;
	//UnlockMaxLevel = 1;
	//memset(LevelDuckNum, 0x00,TOTAL_LEVEL * sizeof(iU8));



	MyWater_data_init();



#if 0 //xie
	if( i51AdeOsFileIsExist((iU16 *)(MW_CONTAIN_PATH)) )
	{
		if(iFALSE==i51KitG2CheckImageContainer((iU16 *)MW_CONTAIN_PATH))
		{
			iLog("容器文件损坏");
			return iFALSE;
		}
		mwContainer = i51KitG2OpenImageContainer((iU16 *)MW_CONTAIN_PATH);

		if(iFALSE==i51AdeOsFileIsExist((iU16 *)MW_DATA_PATH))
		{
			iLog("数据文件不存在");
			return iFALSE;
		}
		File = i51AdeOsFileOpen((iU16 *)MW_DATA_PATH, i51_OS_FL_READ|i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
		i51AdeOsFileSetSeek(File, 10, i51_OS_FL_SEEK_HEAD);
		i51AdeOsFileReadInt32(File, &CurrTotalDuck );
		i51AdeOsFileReadInt32(File, &UnlockMaxLevel  );
		i51AdeOsFileRead(File, LevelDuckNum, TOTAL_LEVEL, &Read);
		i51AdeOsFileClose(File);
//		UnlockMaxLevel = 15;
		
		CurrGameState= GAME_STATE_MAIN;
	}
	else
	{
		iLog("容器文件不存在");
		return iFALSE;
	}

#endif

	return iTRUE;

}

void MyWater_Resume( void )
{
	mwLevelResume(CurrLevelNum);
}

void MyWater_Pause( void )
{
//	mwLevelPause(CurrLevelNum);
}

void MyWater_Exit( void )
{
	mwLevelExit(CurrLevelNum);
	//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL );
	//i51KitG2CloseImageContainer(mwContainer);

//	pfunc->public_audio_all_exit();

	//SoundWinPlay =iFALSE;
	SoundChoosePlay = iFALSE;//xie
	// 胜利的音乐是否在播放的标记
	SoundWinPlay = iFALSE;
	//i51KIT_DYNAMIC_KIT_UNLOAD(i51KitStd);
	//i51KIT_DYNAMIC_KIT_UNLOAD(i51KitG2);

}
	

void MyWater_Main( void )   // xie_main
{
	switch(CurrGameState)
	{
		case GAME_STATE_START:
			break;
		case GAME_STATE_MAIN:
			mwStateMain();
			break;
		case GAME_STATE_MODE:
			mwStateMode();
			break;
		case GAME_STATE_PLAY:
			mwStateGame();
//			mwPlayGame(CurrLevelNum);
			break;
		case GAME_STATE_SET:
			mwStateSet();
			break;

		case GAME_STATE_PARTOL:
			mwStatePartol();
			break;

		case GAME_PARTOL_ENTRY:
#ifdef LANGUAGE_CHINESE
			download_pen_handle(&mwKeyData.u.pen);
			i51AdeOsScreenAlwaysLight(1);
			portal_entry(game_aid, portal_cb);
#endif
			break;

		case GAME_STATE_EXIT:
			//mwStateExit();
			CurrGameState = GAME_STATE_IDLE;
			i51KernelSendRequest (game_aid,0 ,i51_REQ_EXIT , 0, 0);

			break;
		default:
			break;
	}
//	iU32 OldTime = i51AdeOsGetTick();
//iLog("Start i51KitG2Update");
//i51KitG1Draw();
	TimeCount++;


	//i51KitG2Update();//xie  ???


	i51AdeMmiUpdateScreen();

//  iCOLOR Color = i51KitG2COLOR(62, 255, 255);
// iLog("Color:%d,0X%X",Color,Color);
//iLog("End i51KitG2Update");
//    i51AdeMmiUpdateScreenEx(100,100,200,200);
//	iLog("Update time:%d",i51AdeOsGetTick()-OldTime);
}


//i51Public_struct *pfunc = iNULL;
//iKIT void game_pfunc_init(i51Public_struct *p)
//{
//	pfunc = p;
//}

//iKIT iBOOL game_main ( iS32 aid , iU32 message , void* parameter )
//{
//
//	//	TODO :	消息循环
////	AppID = aid;
//	
//	switch ( message )
//	{
//	case i51_MSG_PAINT :
//		pfunc->llp_get_input(&mwKeyData);
//		MyWater_Main();
//		break ;
//	default:
//		break;
//	}
//	return iTRUE ;
//
//}

void wmw_image_destroy()
{
	int i;

	for (i = 0; i < MAX_ID_WMW; i++)
	{	
		public_image_destroy(&iMageInfo[i]);
	}
}


void wmw_resume()
{
	iRECT DesRect;
	iRECT SrcRect;

#ifdef PKIG_SCREEN_320X480

#define CONTINUE_X	93
#define CONTINUE_Y	115
#define CONTINUE_W	128
#define CONTINUE_H	41
#define CHOOSE_X	93
#define CHOOSE_Y	170
#define CHOOSE_W	128
#define CHOOSE_H	41
#define MAIN_X		93
#define MAIN_Y		227
#define MAIN_W		128
#define MAIN_H		41

#elif defined PKIG_SCREEN_240X400

#define CONTINUE_X	72
#define CONTINUE_Y	123
#define CONTINUE_W	97
#define CONTINUE_H	27
#define CHOOSE_X	72
#define CHOOSE_Y	170
#define CHOOSE_W	97
#define CHOOSE_H	27
#define MAIN_X		72
#define MAIN_Y		218
#define MAIN_W		97
#define MAIN_H		27

#elif defined PKIG_SCREEN_240X320

#define CONTINUE_X	72
#define CONTINUE_Y	103
#define CONTINUE_W	97
#define CONTINUE_H	27
#define CHOOSE_X	72
#define CHOOSE_Y	140
#define CHOOSE_W	97
#define CHOOSE_H	27
#define MAIN_X		72
#define MAIN_Y		178
#define MAIN_W		97
#define MAIN_H		27

#endif


	public_audio_init(audio_resume); 
	i51KernelSetFPS(game_aid, 15); //控制帧率
	i51AdeOsScreenAlwaysLight(1);

	if (CurrLevelState == MW_LEVEL_PAUSE && CurrGameState == GAME_STATE_PLAY)
	{
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
			(void *)(TempScreenBuf), 
			SCREEN_WIDTH*SCREEN_HEIGHT*2);

		mwLevelDrawPause();
		mwLevelDrawRestart();



#if 1		
		//i51KitG2SetAlpha(160);
		LocalG2AlphaSys = 160;
		//i51KitG2CleanScreen(0X0, iNULL);
		AlphaNeed = iTRUE;
		AlphaSrc = 160;

		i51KitG2CleanScreenEx(0X0, iNULL);
		//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
		LocalG2AlphaSys = i51KITG2_ALPHA_NONEED;
#endif
		DesRect.X = CONTINUE_X;
		DesRect.Y = CONTINUE_Y;
		DesRect.Width= CONTINUE_W;
		DesRect.Height= CONTINUE_H;
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = CONTINUE_W;
		SrcRect.Height = CONTINUE_H;

		SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);
		DesRect.X = CHOOSE_X;
		DesRect.Y = CHOOSE_Y;
		DesRect.Width= CHOOSE_W;
		DesRect.Height= CHOOSE_H;

		SrcRect.X = 0;
		SrcRect.Y = CHOOSE_H * 2;
		SrcRect.Width = CHOOSE_W;
		SrcRect.Height = CHOOSE_H;

		//SWMW_i51KitG2DrawImage(mwContainer, PsMain, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);

		DesRect.X = MAIN_X;
		DesRect.Y = MAIN_Y;
		DesRect.Width= MAIN_W;
		DesRect.Height= MAIN_H;

		SrcRect.X = 0;
		SrcRect.Y = MAIN_H * 4;
		SrcRect.Width = MAIN_W;
		SrcRect.Height = MAIN_H;

		//SWMW_i51KitG2DrawImage(mwContainer, PsMain, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);
	}
	else if (CurrGameState == GAME_STATE_PARTOL)
	{
		public_image_init(&iMageInfo[Main], Main, (iU32)Main_Hex);
		SWMW_i51KitG2DrawImage(&iMageInfo[Main], iNULL, iNULL);
		public_image_destroy(&iMageInfo[Main]);

	}

	iScreenFlag = iTRUE;

}

void wmw_destroy_buffer(void)
{   
	iU8 ret;
	int i = 0;
	if (TempScreenBuf != iNULL)
	{
		ret = i51AdeOsFree((void*)TempScreenBuf);
		TempScreenBuf = iNULL;
	}
	//iU8 mwWorldAttribute[SCREEN_HEIGHT_HALF][SCREEN_WIDTH_HALF] = {0};


	if (mwWorldAttribute != iNULL)
	{
		for(i =0;i<SCREEN_HEIGHT_HALF;i++)
		{
			ret = i51AdeOsFree((iU8*)(mwWorldAttribute[i]));
			mwWorldAttribute[i] = iNULL;
		}
		ret = i51AdeOsFree((iU8**)(mwWorldAttribute));
		mwWorldAttribute =iNULL;
	}
	return;
}


void wmw_exit()
{
	public_audio_all_exit();

	wmw_image_destroy();

	wmw_destroy_buffer();

	mwLevelExit(CurrLevelNum);

	i51AdeOsLog(0, "退出鳄鱼内置版");

}
void wmw_input()
{
	llp_get_input(&mwKeyData);
}

void audio_cb(iBOOL results)
{
	if (!results)
	{
		i51AdeOsLog( 0, "声音初始化失败！");
	}
}


void  wmw_audio_init()
{
	public_audio_init(audio_cb);
}


void wmw_alldata_init()
{

	// 世界指针
	mwSpace = iNULL;
	// 屏幕指针
	mwScreenBuffer = iNULL;
	// 获得的按键数据

	memset(&mwKeyData, 0x00, sizeof(LLPInputStruct));

	//存放图片的结构体
	memset(iMageInfo, 0x00, MAX_ID_WMW * sizeof(public_image_struct));

	iScreenFlag = iFALSE;

	game_aid =0;

	gFlag = 1;

	COLOR_ALPHA = 150;
	//static iCOLOR GraphFillColor[MAX_FILL_NUM] = {0};
	// 游戏第一级状态机标记
	CurrGameState = GAME_STATE_IDLE;
	// 鸭子是否减少的标记
	DuckResidueFlag = iFALSE;
	// 屏幕临时Buff需要更新的标记
	TempScreenUpdataFlag = iFALSE;

	// 当前游戏中水滴总数
	CurrTotalWaterNum = 0;
	// 释放允许龙头喷水的标记
	WaterSprayFlag = iTRUE;

	// 选择的音乐是否在播放的标记
	SoundChoosePlay = iFALSE;
	// 胜利的音乐是否在播放的标记
	SoundWinPlay = iFALSE;

	WaterDrawAll = iFALSE;

	////恒速区域 详情
	//SpeedKeep SpeedKeepMess[SpeedKeepNum];

	memset(SpeedKeepMess, 0x00, SpeedKeepNum*sizeof(SpeedKeep));

	//// 恒速区域管理器
	//SpeedKeepManage SpeedKeep_Manage;

	memset(&SpeedKeep_Manage, 0x00, sizeof(SpeedKeepManage));

	//// 鸭子详情
	//DuckData DuckMess[DUCK_NUM];

	memset(DuckMess, 0x00, DUCK_NUM*sizeof(DuckData));

	//// 鸭子管理器
	//DuckManage Duck_Manage;

	memset(&Duck_Manage, 0x00, sizeof(DuckManage));

	//// 机关具体信息
	//OrganData OrganMess[MAX_ORGAN];

	memset(OrganMess, 0x00, MAX_ORGAN*sizeof(OrganData));


	//// 机关管理器
	//OrganManage Organ_Manage;

	memset(&Organ_Manage, 0x00, sizeof(OrganManage));

	//// 水管具体信息
	//PipeData PipeMess[MAX_PIPE];

	memset(PipeMess, 0x00, MAX_PIPE * sizeof(PipeData));

	//// 水管管理器
	//PipeManage Pipe_Manage;

	memset(&Pipe_Manage, 0x00, sizeof(PipeManage));


	// 浴缸中已有水滴数量
	WaterInBathtub = 0;
	// 获得时间奖励的分数
	CurrTimeWin = 0;
	// 获得鸭子奖励的分数
	CurrDuckWin = 0;
	// 获得溢水奖励的分数
	CurrWaterWin = 0;
	// 关卡获得的总分
	CurrTotalWin = 0;

	// 当前是第几关的标记
	CurrLevelNum = MW_LEVEL_CHOOSE;
	// 当前关卡内状态的标记
	CurrLevelState = MW_LEVEL_INIT;

	//// 当前世界的标记，记录每一个像素点的属性
	//iU8 mwWorldAttribute[SCREEN_HEIGHT_HALF][SCREEN_WIDTH_HALF] = {0};
	mwWorldAttribute = iNULL;
	//memset(mwWorldAttribute, 0x00, SCREEN_HEIGHT_HALF * SCREEN_WIDTH_HALF * sizeof(iU8));
	// 关卡暂停的标记
	mwWordPause = iFALSE;
	// 音效开关的标记
	SoundCloseFlag = iFALSE;
	// 音乐开关的标记
	MusicCloseFlag = iFALSE;
	// 关卡开始的时间
	TimeLevelStart = 0;
	// 关卡当前的时间
	TimeLevelCurr = 0;

	////// 屏幕buff备份
	////iU16 TempScreenBuf[SCREEN_HEIGHT][SCREEN_WIDTH] = {0}; 
	//memset(TempScreenBuf, 0x00, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(iU16));

	TempScreenBuf = iNULL;

	//// 屏幕网格管理器
	//GriddingMud[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};

	memset(GriddingMud, 0x00, (SCREEN_HEIGHT/SideLength) * (SCREEN_WIDTH/SideLength) * sizeof(iU8));

	//// 屏幕网格状态
	//GriddingType[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};

	memset(GriddingType, 0x00, (SCREEN_HEIGHT/SideLength) * (SCREEN_WIDTH/SideLength) * sizeof(iU8));


	//// 屏幕网格是否需要更新的标记
	//GriddingUpdataFlag[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};

	memset(GriddingUpdataFlag, 0x00, (SCREEN_HEIGHT/SideLength) * (SCREEN_WIDTH/SideLength) * sizeof(iU8));

	AlgaeNeed = iFALSE;

	//// 鳄鱼等待水来动画结构
	//AnimationStr CrocodileWait;

	memset(&CrocodileWait, 0x00, sizeof(AnimationStr));
	//// 鳄鱼洗澡动画结构
	//AnimationStr CrocodileBathe;

	memset(&CrocodileBathe, 0x00, sizeof(AnimationStr));

	TimeCount = 0;

	InitFlag= iFALSE;

	memset(INTERSECTION_P2S, 0x00, 10*sizeof(iVECT));

	IntersectionNum = 0;

	CurrDirection = 0XFF;

	DirectionNum = 0;


	se_TrackPointCount = 0;

	//se_TrackPoint[EFFECT_MAXTRACKPOINT][2] = {0};

	memset(se_TrackPoint, 0x00, EFFECT_MAXTRACKPOINT*2*sizeof(iS16));


	IsCompleten = iTRUE;

	word_frame = 0;

	IsEntry = iFALSE;

	////memset(&G1_EdgeTable, 0x00, sizeof(G1_EDGE_TABLE));

	memset(audio, 0X00, AUDIO_PLAYER_NUM_MAX*sizeof(audio_struct));

	ChooseErrFlag = iFALSE;

	ChooseErrTime = 0;

	mwLevelWinFlag = iFALSE;

	mwLevelResultFlag = iFALSE;

	LevelErrStartTime = 0;

	LevelErrFlag = iFALSE;

	DrawBlankAlpha = iFALSE;

	OldGridding_X = 0;

	OldGridding_Y = 0;

	CurrGridding_X = 0;

	CurrGridding_Y = 0;


}

void wmw_malloc_buffer()
{
	iU32 * LcdInfoArr;
	iU32 LengthPartDes = 0;
	int i = 0;

	LcdInfoArr =  i51AdeMmiGetPanel();

	if( iNULL == LcdInfoArr )goto ERROR;

	LengthPartDes = LcdInfoArr[2]*LcdInfoArr[3]*2;

	if (iNULL == TempScreenBuf)
	{
		TempScreenBuf  = (iU16 *)i51AdeOsMalloc(LengthPartDes,__FILE__,__LINE__);
	}
	if (iNULL == TempScreenBuf)
	{
		i51AdeOsLog(0,"wmw_malloc_buffer  Error!");
	}

	if(iNULL == mwWorldAttribute)
	{
		mwWorldAttribute = (iU8 **)i51AdeOsMalloc(sizeof(iU8 *)*SCREEN_HEIGHT_HALF ,__FILE__,__LINE__);
		for(i = 0; i<SCREEN_HEIGHT_HALF; i++)
		{
			mwWorldAttribute[i] = (iU8 *)i51AdeOsMalloc(sizeof(iU8)*SCREEN_WIDTH_HALF,__FILE__,__LINE__);
		}
	}

	return ;
ERROR:
	i51AdeOsLog(0,"wmw_malloc_buffer  Error!");
	return ;

}




iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
//	AppID = aid;
	
	switch ( message )
	{
	case i51_MSG_PAINT :
		{
			if(gFlag)
			{
				i51AdeOsLog(0, "进入鳄鱼内置版 初始化");

				gFlag = 0;
				wmw_audio_init();
				wmw_malloc_buffer();
				MyWater_Init();
				game_aid = aid;
				i51KernelSetFPS(aid, 15); //控制帧率
				i51AdeOsScreenAlwaysLight(1);
				i51AdeOsLog(0, "进入鳄鱼内置版  结束");

			}
			wmw_input();
			MyWater_Main();
		}
		break ;

	case i51_MSG_RUN :
		break ;

	case i51_MSG_INIT :
		{

			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
			wmw_alldata_init();

		}
		break ;

	case i51_MSG_PAUSE :
		{
			public_audio_all_exit();
			i51AdeOsScreenAlwaysLight(0);
		}
		break;
	case i51_MSG_EXIT:
		wmw_exit();

#ifdef LANGUAGE_CHINESE
		portal_exit();
#endif
		break ;	
	case i51_MSG_KEYDOWN:
	case i51_MSG_KEYUP:
		llp_input_key(message, (int)parameter);
		break;
	case i51_MSG_PADDOWN:
	case i51_MSG_PADUP:
	case i51_MSG_PADMOVE:
		llp_input_pen(message, (int)parameter);
		break;	
	case i51_MSG_RESUME:
		if (CurrGameState != GAME_PARTOL_ENTRY)
		{
			wmw_resume();
			i51AdeOsLog(0, "恢复游戏");
		}
		break ;


	}

	return iTRUE ;

}


iKIT iBOOL i51SimpleWMWKitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
	//	AppID = aid;


	switch ( message )
	{
	case i51_MSG_PAINT :
		{
			if(gFlag)
			{
				i51AdeOsLog(0, "....鳄鱼....");

				gFlag = 0;
				wmw_audio_init();
				wmw_malloc_buffer();
				MyWater_Init();
				game_aid = aid;
				i51KernelSetFPS(aid, 15); //控制帧率
				i51AdeOsScreenAlwaysLight(1);
				i51AdeOsLog(0, "进入鳄鱼内置版");

			}
			wmw_input();
			MyWater_Main();
		}
		break ;

	case i51_MSG_RUN :
		break ;

	case i51_MSG_INIT :
		{

			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
			wmw_alldata_init();

		}
		break ;

	case i51_MSG_PAUSE :
		{
			public_audio_all_exit();
			i51AdeOsScreenAlwaysLight(0);
		}
		break;
	case i51_MSG_EXIT:
		wmw_exit();

#ifdef LANGUAGE_CHINESE
		portal_exit();
#endif
		break ;	
	case i51_MSG_KEYDOWN:
	case i51_MSG_KEYUP:
		llp_input_key(message, (int)parameter);
		break;
	case i51_MSG_PADDOWN:
	case i51_MSG_PADUP:
	case i51_MSG_PADMOVE:
		llp_input_pen(message, (int)parameter);
		break;	
	case i51_MSG_RESUME:
		if (CurrGameState != GAME_PARTOL_ENTRY)
		{
			wmw_resume();
			i51AdeOsLog(0, "恢复游戏");
		}
		break ;


	}

	return iTRUE ;

}


iKIT iU32* i51SimpleWMWKitPort ()
{

	//	TODO : Initialize API Port for external layer

	//	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(DlDemo)
	//	i51KIT_DYNAMIC_KIT_MAPING_END 

	//	return i51KIT_DYNAMIC_KIT_GET_MAPING(DlDemo) ;

	return (iU32*)1 ;

}