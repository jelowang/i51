
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwStateExit( void )
{
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
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
		static iU8 ExitFlag = iFALSE;
		iRECT DesRect;
		iU16 PenDownX = 0;
		iU16 PenDownY = 0;
		
		
		if(!Flag)
		{
			Flag = iTRUE;
			i51KitG2SetAlpha(160);
			i51KitG2CleanScreen(0X0, iNULL);
			i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
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
			&&PenDownY>=YesY&&PenDownY<(YesY+YesH) && ExitFlag==iFALSE )
			{
				DesRect.X = YesX;
				DesRect.Y = YesY;
				DesRect.Width= YesW;
				DesRect.Height= YesH;
				i51KitG2DrawImage(mwContainer, YesPress, &DesRect, iNULL);
				iLog("WMW i51_REQ_EXIT");
				pfunc->kernel_send_request(i51_REQ_EXIT);

//				i51KernelSendRequest ( AppID, iNULL, i51_REQ_EXIT, iNULL, iNULL) ;
//				CurrGameState = GAME_STATE_MAIN;
//				Flag = iFALSE;
				ExitFlag = iTRUE;
			}
			else if( PenDownX>=NoX&&PenDownX<(NoX+NoW)
			&&PenDownY>=NoY&&PenDownY<(NoY+NoH)  )
			{
				DesRect.X = NoX;
				DesRect.Y = NoY;
				DesRect.Width= NoW;
				DesRect.Height= NoH;
				i51KitG2DrawImage(mwContainer, NoPress, &DesRect, iNULL);
				CurrGameState = GAME_STATE_MAIN;
				Flag = iFALSE;
			}
	
		}
	return iTRUE;
}



