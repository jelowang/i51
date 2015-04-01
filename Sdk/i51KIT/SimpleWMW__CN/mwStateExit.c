
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif



iBOOL mwStatePartol()
{
	iRECT desRect;
	//iRECT srcRect;
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;


	//SWMW_i51KitG2DrawImage(&iMageInfo[Main], iNULL, iNULL);

	if(iScreenFlag == iTRUE)
	{
		LocalG2AlphaSys = 128;
		//i51KitG2CleanScreen(0X0, iNULL);
		AlphaNeed = iTRUE;
		AlphaSrc = 128;

		i51KitG2CleanScreenEx(0X0, iNULL);
		//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

		LocalG2AlphaSys = i51KITG2_ALPHA_NONEED;

		iScreenFlag = iFALSE;


	}

#ifdef PKIG_SCREEN_320X480
	desRect.X = 32;
	desRect.Y = 112;
	desRect.Width = 256;
	desRect.Height = 256;

#elif defined PKIG_SCREEN_240X400

	desRect.X = 32;
	desRect.Y = 112;
	desRect.Width = 256;
	desRect.Height = 256;


#elif defined PKIG_SCREEN_240X320

	desRect.X = 20;
	desRect.Y = 60;
	desRect.Width = 199;
	desRect.Height = 200;

#endif

	SWMW_i51KitG2DrawImage(&iMageInfo[PromptFrame], &desRect, iNULL);


	if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		if (PenDownX < PK_SCREEN_WIDTH / 2)
		{
			//½øÈë partol
			//i51AdeOsScreenAlwaysLight(1);
			//portal_entry(game_aid, portal_cb);
			
			CurrGameState = GAME_PARTOL_ENTRY;
			wmw_image_destroy();
			public_audio_all_exit();

		}
		else if (PenDownX > PK_SCREEN_WIDTH / 2)
		{
			CurrGameState = GAME_STATE_MAIN;
			wmw_image_destroy();
			wmw_main_page();
		}

	}


	return iTRUE;
}

iBOOL mwStateExit( void )
{

	//iLog("WMW i51_REQ_EXIT");
	//pfunc->kernel_send_request(i51_REQ_EXIT);

	//i51KernelSendRequest (game_aid,0 ,i51_REQ_EXIT , 0, 0);

	return iTRUE;
#if 0

#if defined(PKIG_SCREEN_240X320)
	iS16 YesX = 41;
	iS16 YesY = 189;
	iS16 YesW = 73;
	iS16 YesH = 35;
	iS16 NoX = 129;
	iS16 NoY = 189;
	iS16 NoW = 73;
	iS16 NoH = 35;
	iS16 ExitX = 26;
	iS16 ExitY = 43;
	iS16 ExitW = 214;
	iS16 ExitH = 194;

#elif defined(PKIG_SCREEN_240X400)	
	iS16 YesX = 41;
	iS16 YesY = 248;
	iS16 YesW = 73;
	iS16 YesH = 35;
	iS16 NoX = 129;
	iS16 NoY = 248;
	iS16 NoW = 73;
	iS16 NoH = 35;
	iS16 ExitX = 26;
	iS16 ExitY = 100;
	iS16 ExitW = 214;
	iS16 ExitH = 194;


#elif defined(PKIG_SCREEN_320X480)
	iS16 YesX = 62;
	iS16 YesY = 327;
	iS16 YesW = 97;
	iS16 YesH = 41;
	iS16 NoX = 175;
	iS16 NoY = 327;
	iS16 NoW = 97;
	iS16 NoH = 41;
	iS16 ExitX = 41;
	iS16 ExitY = 133;
	iS16 ExitW = 279;
	iS16 ExitH = 252;

#endif
		static iU8 Flag = iFALSE;
		iRECT DesRect;
		iRECT SrcRect;
		iU16 PenDownX = 0;
		iU16 PenDownY = 0;
	
		
		if(!Flag)
		{
			Flag = iTRUE;
			//i51KitG2SetAlpha(160);
			i51KitG2CleanScreen(0X0, iNULL);
			//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
			DesRect.X = ExitX;
			DesRect.Y = ExitY;
			DesRect.Width= ExitW;
			DesRect.Height= ExitH;
			i51KitG2DrawImage(mwContainer, ExitInterface, &DesRect, iNULL);
		}
		
		if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
		{
			PenDownX = mwKeyData.u.pen.x;
			PenDownY = mwKeyData.u.pen.y;
	
			if( PenDownX>=YesX&&PenDownX<(YesX + YesW)
			&&PenDownY>=YesY&&PenDownY<(YesY+YesH))
			{
				DesRect.X = YesX;
				DesRect.Y = YesY;
				DesRect.Width= YesW;
				DesRect.Height= YesH;
				SrcRect.X = 0;
				SrcRect.Y = 0;
				SrcRect.Width = YesW;
				SrcRect.Height = YesH;


				//iLog("WMW i51_REQ_EXIT");
				pfunc->kernel_send_request(i51_REQ_EXIT);

//				i51KernelSendRequest ( AppID, iNULL, i51_REQ_EXIT, iNULL, iNULL) ;
//				CurrGameState = GAME_STATE_MAIN;
				Flag = iFALSE;
			}
			else if( PenDownX>=NoX&&PenDownX<(NoX+NoW)
			&&PenDownY>=NoY&&PenDownY<(NoY+NoH))
			{
				DesRect.X = NoX;
				DesRect.Y = NoY;
				DesRect.Width= NoW;
				DesRect.Height= NoH;

				SrcRect.X = 0;
				SrcRect.Y = NoH;
				SrcRect.Width = NoW;
				SrcRect.Height = NoH;


				CurrGameState = GAME_STATE_MAIN;
				Flag = iFALSE;
			}
	
		}
	return iTRUE;
#endif //xie 
}



