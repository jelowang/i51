
#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51Public.h"
#include "mwPublic.h"
#include "mwLevelPublic.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

// 世界指针
cpSpace *mwSpace = iNULL;
// 屏幕指针
iU16* mwScreenBuffer = iNULL;
// 获得的按键数据
LLPInputStruct mwKeyData;
// 图片容器句柄
iHANDLE mwContainer = iNULL;


void Process(iU32 Step , iU32 Totall , iHANDLE Handle )
{
	static iS16 Count;
	static iU32 OldTime = 0;
	mwContainer= Handle;
	if(Step>=4)
	{
		iU32 ImageW = 0;
		iU32 ImageH = 0;
		iRECT DesRect;
		iU32 ImageId[4] = {0,1,2,3};

		i51KitG2SetImageMemoryCopy(Handle, 4, ImageId );

		i51KitG2GetImageAttr(Handle, 0, &ImageW, &ImageH, iNULL);

		for( iS16 Y = 0; Y<SCREEN_HEIGHT; Y+=ImageH )
		{
			for( iS16 X = 0; X<SCREEN_WIDTH; X+=ImageW )
			{
				DesRect.X = X;
				DesRect.Y = Y;
				DesRect.Width = ImageW;
				DesRect.Height = ImageH;
				i51KitG2DrawImage(Handle, 0, &DesRect, iNULL);
			}
		}

		i51KitG2GetImageAttr(Handle, 1, &ImageW, &ImageH, iNULL);
		DesRect.X = 95;
		DesRect.Y = 311;
		DesRect.Width = ImageW;
		DesRect.Height = ImageH;
		i51KitG2DrawImage(Handle, 1, &DesRect, iNULL);
		
		i51KitG2GetImageAttr(Handle, 2, &ImageW, &ImageH, iNULL);
		DesRect.X = 102;
		DesRect.Y = 318;
		DesRect.Width = ImageW*Step/Totall;
		DesRect.Height = ImageH;
		i51KitG2DrawImage(Handle, 2, &DesRect, iNULL);

		i51KitG2GetImageAttr(Handle, 3, &ImageW, &ImageH, iNULL);
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
		i51KitG2UnsetImageMemoryCopy(Handle, iNULL, iNULL);
//		iU32 ImageId[9] = { PauseShow,PausePress,RestartPress,RestartShow,DuckMud,DuckGet,BlackBK,WordLevel,LevelNum};
//		i51KitG2SetImageMemoryCopy(mwContainer, 9, ImageId );
	}
}

iBOOL MyWater_Init( void )
{

	/// init space.
	mwInitSpace(CurrLevelNum);

	iU32 * LcdInfoArr;
    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    mwScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	CurrLevelNum = MW_LEVEL_CHOOSE;

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
		iFILE File = i51AdeOsFileOpen((iU16 *)MW_DATA_PATH, i51_OS_FL_READ|i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
		i51AdeOsFileSetSeek(File, 10, i51_OS_FL_SEEK_HEAD);
		i51AdeOsFileReadInt32(File, &CurrTotalDuck );
		i51AdeOsFileReadInt32(File, &UnlockMaxLevel  );
		iU32 Read;
		i51AdeOsFileRead(File, LevelDuckNum, TOTAL_LEVEL, &Read);
		i51AdeOsFileClose(File);
		
		CurrGameState= GAME_STATE_MAIN;
	}
	else
	{
		iLog("容器文件不存在");
		return iFALSE;
	}
	
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
	i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL );
	i51KitG2CloseImageContainer(mwContainer);

//	pfunc->public_audio_all_exit();
	SoundChoosePlay = iFALSE;
	// 胜利的音乐是否在播放的标记
	SoundWinPlay = iFALSE;

}
	

void MyWater_Main( void )
{
//iLog("CurrGameState=%d",CurrGameState);
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
		case GAME_STATE_EXIT:
			mwStateExit();
			break;
		default:
			break;
	}
//	iU32 OldTime = i51AdeOsGetTick();
//iLog("Start i51KitG2Update");
	i51KitG2Update();
//  iCOLOR Color = i51KitG2COLOR(62, 255, 255);
// iLog("Color:%d,0X%X",Color,Color);
//iLog("End i51KitG2Update");
//    i51AdeMmiUpdateScreenEx(100,100,200,200);
//	iLog("Update time:%d",i51AdeOsGetTick()-OldTime);
}


i51Public_struct *pfunc = iNULL;
iKIT void game_pfunc_init(i51Public_struct *p)
{
	pfunc = p;
}

iKIT iBOOL game_main ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
//	AppID = aid;
	
	switch ( message )
	{
	case i51_MSG_PAINT :
		pfunc->llp_get_input(&mwKeyData);
		MyWater_Main();
		break ;
	default:
		break;
	}
	return iTRUE ;

}


//iS32 AppID = 0;
extern "C" iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter ) ;
iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
//	AppID = aid;
	
	switch ( message )
	{
	case i51_MSG_PAINT :
	case i51_MSG_RUN:
		break;
	case i51_MSG_INIT :
		i51KIT_STATIC_KIT_SET(Adapter,parameter);
		i51KIT_STATIC_KIT_ASSERT(Adapter);

		i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,(iU16 *)L"PKiGAPI\\i51KitG2.kit");
		if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE ( i51KitG2 ) )
		{
			i51AdeOsLog ( i51_LOG_DC , "加载i51KitG2动态库失败" ) ;
			return iFALSE;
		}

		i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,(iU16 *)L"PKiGAPI\\i51KitStd.kit");
		if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE ( i51KitStd ) )
		{
			i51AdeOsLog ( i51_LOG_DC , "加载i51KitStd动态库失败" ) ;
			return iFALSE;
		}

		i51AdeOsScreenAlwaysLight(1);
		if(iFALSE==MyWater_Init())
		{
			return iFALSE;
		}
		break ;

	case i51_MSG_KEYDOWN:
	case i51_MSG_KEYUP:
	case i51_MSG_PADUP:
	case i51_MSG_PADMOVE:
	case i51_MSG_PADDOWN:
		break;
	case i51_MSG_PAUSE:
		MyWater_Pause();
		break ;
	case i51_MSG_RESUME:
		MyWater_Resume();
		break ;
	case i51_MSG_EXIT:
		MyWater_Exit();
		break;
	default:
		break;
	}
	return iTRUE ;

}

extern "C" iKIT iU32* i51KitPort ();
iKIT iU32* i51KitPort ()
{
	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(game)
		i51KIT_DYNAMIC_KIT_MAPING(game_pfunc_init)
		i51KIT_DYNAMIC_KIT_MAPING(game_main)
		i51KIT_DYNAMIC_KIT_MAPING_END 

		return i51KIT_DYNAMIC_KIT_GET_MAPING(game) ;

	return 0 ;
}
