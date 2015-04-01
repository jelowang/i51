#ifdef __I51__

#include "i51Scripts.h"
#include "MMI_include.h"

#include "GlobalMenuItems.h"

#include "GlobalConstants.h"

#include "wgui_softkeys.h"
#if( I51_MTK_VS !=VS_11A_1112 )
#include "HistoryGprot.h"
#endif
#include "IdleAppResDef.h"

#if( I51_MTK_VS>=VS_W10 )
#include "mmi_rp_all_defs.h"
#endif

#include "i51Kernel.h"

#define SP_EXIT_PROMPT  1

extern int  i51EntryDo(void);
extern iBOOL i51NeedWaitEntry(void (*EntyMenuFnPtr)(void));

extern const unsigned char* i51SapGetBootMenu (void) ;
extern const int i51SapGetBootMenuSize (void) ;

#ifdef __I51_SIMPLE_INSTALL__
extern void i51SimpleLunchApp( U16 *pAppName );
#endif

#ifdef	__I51_TQ__		// 天气预报
void weather_I51_entry(void)
{	

}

void weather_I51_entry_highlight(void)
{
    SetLeftSoftkeyFunction (weather_I51_entry,KEY_EVENT_UP); 
    SetKeyHandler (weather_I51_entry,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void weather_I51_entry_init(void)
{
    SetHiliteHandler (MENU_ID_I51_TQ, weather_I51_entry_highlight);
}
#endif

#ifdef	__I51_SavageAdventure__		// 野人逃生记
void sa_game_entry_2(void)
{
    extern void *i51SapSavageAdventureParameter(void);

    if( i51NeedWaitEntry(sa_game_entry_2) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"SavageAdventure");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapSavageAdventureParameter(), 0 , 1 ) ;	
#endif
}

void sa_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (sa_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (sa_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void sa_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_SA, sa_game_entry_2_highlight);
}
#endif


#ifdef	__I51_BM__		// 宝石迷情
void bm_game_entry_2(void)
{

    extern void *i51SapParameter(void);

    if( i51NeedWaitEntry( bm_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"BM");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapParameter(), 0 , 1 ) ;	
#endif

}

void bm_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (bm_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (bm_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void bm_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_BM, bm_game_entry_2_highlight);
}
#endif


#ifdef	__I51_DM__		// 金字塔历险记
void dm_game_entry_2(void)
{
    extern void *i51SapParameter(void);

    if( i51NeedWaitEntry( dm_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"DM");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapParameter(), 0 , 1 ) ;	
#endif

}

void dm_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (dm_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (dm_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void dm_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_DM, dm_game_entry_2_highlight);
}
#endif


#ifdef	__I51_ST__		// 街头速递
void streetboy_game_entry_2(void)
{
    extern void *i51SapParameter(void);

    if( i51NeedWaitEntry( streetboy_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"ST");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapParameter(), 0 , 1 ) ;	
#endif
}

void streetboy_game_entry_2_highlight (void)
{
    SetLeftSoftkeyFunction (streetboy_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (streetboy_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void streetboy_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_ST, streetboy_game_entry_2_highlight);
}
#endif


#ifdef	__I51_Loli__		// 营救小LOLI
void sl_game_entry_2(void)
{
    extern void *i51SapParameter(void);

    if( i51NeedWaitEntry( sl_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"Loli");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapParameter(), 0 , 1 ) ;	
#endif
}

void sl_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (sl_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (sl_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void sl_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_SL, sl_game_entry_2_highlight);
}
#endif


#ifdef	__I51_Mjump__		// 跳跳堂
void Mjump_game_entry_2(void)
{
    extern void *i51SapMjumpParameter(void);

    if( i51NeedWaitEntry( Mjump_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"Mjump");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapMjumpParameter(), 0 , 1 ) ;	
#endif
}

void Mjump_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (Mjump_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (Mjump_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void Mjump_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_MJ, Mjump_game_entry_2_highlight);
}
#endif


#ifdef	__I51_X_Space__	// 完美真相
void X_Space_game_entry_2(void)
{
    extern void *i51SapX_SpaceParameter(void);

    if( i51NeedWaitEntry( X_Space_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"X_Space");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapX_SpaceParameter(), 0 , 1 ) ;	
#endif
}

void X_Space_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (X_Space_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (X_Space_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void X_Space_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_SPACE, X_Space_game_entry_2_highlight);
}
#endif


#ifdef	__I51_DontRun__		// 虫虫别跑
void DR_game_entry_2(void)
{
    extern void *i51SapDontRunParameter(void);

    if( i51NeedWaitEntry( DR_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"DontRun");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapDontRunParameter(), 0 , 1 ) ;	
#endif
}

void DR_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (DR_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (DR_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void DR_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_DR, DR_game_entry_2_highlight);
}
#endif


#ifdef	__I51_DS__		// 豆豆广场
void Ds_game_entry_2(void)
{
    extern void *i51SapParameter(void);

    if( i51NeedWaitEntry( Ds_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"DS");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapParameter(), 0 , 1 ) ;	
#endif
}

void Ds_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (Ds_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (Ds_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void Ds_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_DS, Ds_game_entry_2_highlight);
}
#endif


#ifdef	__I51_Convoy__		// 保卫钓鱼岛
void convoy_game_entry_2(void)
{
    extern void *i51SapConvoyParameter(void);

    if( i51NeedWaitEntry( convoy_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"Convoy");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapConvoyParameter(), 0 , 1 ) ;	
#endif
}

void convoy_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (convoy_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (convoy_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void convoy_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_CY, convoy_game_entry_2_highlight);
}
#endif



#ifdef	__I51_HundredStorey__		// 丛林飞将
void hs_game_entry_2(void)
{
    extern void *i51SapHundredStoreyParameter(void);

    if( i51NeedWaitEntry( hs_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"HundredStorey");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapHundredStoreyParameter(), 0 , 1 ) ;	
#endif
}

void hs_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (hs_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (hs_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void hs_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_HS, hs_game_entry_2_highlight);
}
#endif


#ifdef	__I51_fighter__		// 玫瑰机动队
void fighter_game_entry_2(void)
{
    extern void *i51SapfighterParameter(void);//玫瑰机动队

    if( i51NeedWaitEntry( fighter_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"fighter");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapfighterParameter(), 0 , 1 ) ;	
#endif
}

void fighter_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (fighter_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (fighter_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void fighter_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_FT, fighter_game_entry_2_highlight);
}
#endif


#ifdef	__I51_RaceCar__		// 炮弹飞车
void rc_game_entry_2(void)
{
    extern void *i51SapRaceCarParameter(void);

    if( i51NeedWaitEntry( rc_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"RaceCar");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapRaceCarParameter(), 0 , 1 ) ;	
#endif
}

void rc_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (rc_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (rc_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void rc_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_RC, rc_game_entry_2_highlight);
}
#endif


#ifdef	__I51_VexedJewels__		// 重力宝石
void VJ_game_entry_2(void)
{
    extern void *i51SapVexedJewelsParameter(void);

    if( i51NeedWaitEntry( VJ_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"VexedJewels");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapVexedJewelsParameter(), 0 , 1 ) ;	
#endif
}

void VJ_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (VJ_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (VJ_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void VJ_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_VJ, VJ_game_entry_2_highlight);
}
#endif


#ifdef	__I51_CutFruit__		// 水果忍者
void cf_game_entry_2(void)
{
    extern void *i51SapCutFruitParameter(void); 

    if( i51NeedWaitEntry( cf_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"CutFruit");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapCutFruitParameter(), 0 , 1 ) ;	
#endif
} 

void cf_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (cf_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (cf_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void cf_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_CF, cf_game_entry_2_highlight);
}
#endif


#ifdef	__I51_JigsawPuzzle__		// 魅力拼图
void JigsawPuzzle_game_entry_2(void)
{
    extern void *i51SapJigsawPuzzleParameter(void);

    if( i51NeedWaitEntry( JigsawPuzzle_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"JigsawPuzzle");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapJigsawPuzzleParameter(), 0 , 1 ) ;	
#endif
}

void JigsawPuzzle_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (JigsawPuzzle_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (JigsawPuzzle_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void JigsawPuzzle_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_JP, JigsawPuzzle_game_entry_2_highlight);
}
#endif


#ifdef	__I51_Snidget__	// 武林飞侠
void nin_game_entry_2(void)
{
    extern void *i51SapSnidgetParameter(void);

    if( i51NeedWaitEntry( nin_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"Snidget");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapSnidgetParameter(), 0 , 1 ) ;	
#endif
}

void nin_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (nin_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (nin_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void nin_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_NIN, nin_game_entry_2_highlight);
}
#endif


#ifdef	__I51_IP__		// 成语迷局
void IdiomPuzzle_game_entry_2(void)
{
    extern void *i51SapParameter(void);

    if( i51NeedWaitEntry( IdiomPuzzle_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"IP");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapParameter(), 0 , 1 ) ;	
#endif
}

void IdiomPuzzle_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (IdiomPuzzle_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (IdiomPuzzle_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void IdiomPuzzle_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_IP, IdiomPuzzle_game_entry_2_highlight);
}
#endif


#ifdef	__I51_DDZ__	// 斗地主
void DDZ_game_entry_2(void)
{
    extern void *i51SapDDZParameter(void);

    if( i51NeedWaitEntry( DDZ_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"DDZ");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapDDZParameter(), 0 , 1 ) ;	
#endif
}

void DDZ_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (DDZ_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (DDZ_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void DDZ_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_DDZ, DDZ_game_entry_2_highlight);
}
#endif


#ifdef	__I51_FruitShot__		// 水果快点
void FruitShot_game_entry_2(void)
{
    extern void *i51SapFruitShotParameter(void);

    if( i51NeedWaitEntry( FruitShot_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"FruitShot");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapFruitShotParameter(), 0 , 1 ) ;	
#endif
}

void FruitShot_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (FruitShot_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (FruitShot_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void FruitShot_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_FS, FruitShot_game_entry_2_highlight);
}
#endif


#ifdef	__I51_FishingMaster__		// 捕鱼达人
void CFM_game_entry_2(void)
{
    extern void *i51SapFishingMasterParameter(void);

    if( i51NeedWaitEntry( CFM_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"FishingMaster");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapFishingMasterParameter(), 0 , 1 ) ;	
#endif
}

void CFM_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (CFM_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (CFM_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void CFM_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_CFM, CFM_game_entry_2_highlight);
}
#endif

#ifdef	__I51_AngryBird__ 	// 愤怒的小鸟
void ANGRYBIRD_game_entry_2(void)
{
    extern void *i51SapAngryBirdParameter(void);

    if( i51NeedWaitEntry( ANGRYBIRD_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"AngryBird");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapAngryBirdParameter(), 0 , 1 ) ;	
#endif
}

void ANGRYBIRD_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (ANGRYBIRD_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (ANGRYBIRD_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void ANGRYBIRD_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_ANGRYBIRD, ANGRYBIRD_game_entry_2_highlight);
}
#endif
	
#ifdef	__I51_GlassDestroyer__	// 碎玻璃
void GD_game_entry_2(void)
{
    extern void *i51SapGlassDestroyerParameter(void);

    if( i51NeedWaitEntry( GD_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"GlassDestroyer");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapGlassDestroyerParameter(), 0 , 1 ) ;	
#endif
}

void GD_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (GD_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (GD_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void GD_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_GD, GD_game_entry_2_highlight);
}
#endif
	
#ifdef	__I51_PullSkirt__	// 吹裙子
void PS_game_entry_2(void)
{
    extern void *i51SapPullSkirtParameter(void);

    if( i51NeedWaitEntry( PS_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"PullSkirt");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapPullSkirtParameter(), 0 , 1 ) ;	
#endif
}

void PS_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (PS_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (PS_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void PS_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_PS, PS_game_entry_2_highlight);
}
#endif

#ifdef	__I51_TOM__	// TOM
void TOM_game_entry_2(void)
{
    extern void *i51SapTOMParameter(void);

    if( i51NeedWaitEntry( TOM_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"TOM");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapTOMParameter(), 0 , 1 ) ;	
#endif
}

void TOM_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (TOM_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (TOM_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void TOM_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_TOM, TOM_game_entry_2_highlight);
}
#endif

#ifdef	__I51_Ninjump__		// 忍者跳跃

void Ninjump_game_entry_2(void)
{
    extern void *i51SapNinJumpParameter(void);

    if( i51NeedWaitEntry( Ninjump_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"NinJump");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapNinJumpParameter(), 0 , 1 ) ;	
#endif
}

void Ninjump_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (Ninjump_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (Ninjump_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void Ninjump_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_NINJUMP, Ninjump_game_entry_2_highlight);
}

#endif

#ifdef	__I51_ZapBird__		// 闪电鸟

void ZapBird_game_entry_2(void)
{
    extern void *i51SapZapBirdParameter(void);

    if( i51NeedWaitEntry( ZapBird_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"ZapBird");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapZapBirdParameter(), 0 , 1 ) ;	
#endif
}

void ZapBird_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (ZapBird_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (ZapBird_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void ZapBird_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_ZAPBIRD, ZapBird_game_entry_2_highlight);
}

#endif

#ifdef	__I51_PlantsVsZombies__		// 植物大战僵尸

void PlantsVsZombies_game_entry_2(void)
{
    extern void *i51SapPlantsVsZombiesParameter(void);

    if( i51NeedWaitEntry( PlantsVsZombies_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"PlantsVsZombies");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapPlantsVsZombiesParameter(), 0 , 1 ) ;	
#endif
}

void PlantsVsZombies_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (PlantsVsZombies_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (PlantsVsZombies_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void PlantsVsZombies_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_PLANTSVSZOMBIES, PlantsVsZombies_game_entry_2_highlight);
}

#endif

#ifdef	__I51_VXin__		//V信

void VXin_game_entry_2(void)
{
    extern void *i51SapVXinParameter(void);

    if( i51NeedWaitEntry( VXin_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"VXin");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapVXinParameter(), 0 , 1 ) ;	
#endif
}

void VXin_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (VXin_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (VXin_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void VXin_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_VXin, VXin_game_entry_2_highlight);
}

#endif

#ifdef	__I51_RipClothes__		//撕衣服

void RipClothes_game_entry_2(void)
{
    extern void *i51SapRipClothesParameter(void);

    if( i51NeedWaitEntry( RipClothes_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"RipClothes");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapRipClothesParameter(), 0 , 1 ) ;	
#endif
}

void RipClothes_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (RipClothes_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (RipClothes_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void RipClothes_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_RipClothes, RipClothes_game_entry_2_highlight);
}

#endif

#ifdef	__I51_AppStore__		//应用商店
void AppStore_game_entry_2(void)
{
    extern void *i51SapAppstoreParameter(void);

    if( i51NeedWaitEntry( AppStore_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"AppStore");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapAppstoreParameter(), 0 , 1 ) ;	
#endif
}

void AppStore_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (AppStore_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (AppStore_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void AppStore_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_AppStore, AppStore_game_entry_2_highlight);
}

#endif

#ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
void wheresMywater_game_entry_2(void)
{
    extern void *i51SapwheresMywaterParameter(void);

    if( i51NeedWaitEntry( wheresMywater_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"wheresMywater");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapwheresMywaterParameter(), 0 , 1 ) ;	
#endif
}

void wheresMywater_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (wheresMywater_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (wheresMywater_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void wheresMywater_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_wheresMywater, wheresMywater_game_entry_2_highlight);
}
#endif
    
#ifdef	__I51_MusicJam__        //劲乐达人
void MusicJam_game_entry_2(void)
{
    extern void *i51SapMusicJamParameter(void);

    if( i51NeedWaitEntry( MusicJam_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"MusicJam");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapMusicJamParameter(), 0 , 1 ) ;	
#endif
}

void MusicJam_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (MusicJam_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (MusicJam_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void MusicJam_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_MusicJam, MusicJam_game_entry_2_highlight);
}
#endif
    
#ifdef	__I51_AuPlanet__        //啊呜星球
void AuPlanet_game_entry_2(void)
{
    extern void *i51SapAuPlanetParameter(void);

    if( i51NeedWaitEntry( AuPlanet_game_entry_2 ) ) return;
    if( i51EntryDo()== SP_EXIT_PROMPT ) return;
#ifdef __I51_SIMPLE_INSTALL__
    i51SimpleLunchApp(L"AuPlanet");
#else
    i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapAuPlanetParameter(), 0 , 1 ) ;	
#endif
}

void AuPlanet_game_entry_2_highlight(void)
{
    SetLeftSoftkeyFunction (AuPlanet_game_entry_2,KEY_EVENT_UP); 
    SetKeyHandler (AuPlanet_game_entry_2,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void AuPlanet_game_entry_init_2(void)
{
    SetHiliteHandler (MENU_ID_I51_AuPlanet, AuPlanet_game_entry_2_highlight);
}
#endif


#endif
