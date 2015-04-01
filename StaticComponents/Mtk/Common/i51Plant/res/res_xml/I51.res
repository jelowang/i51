#include "mmi_features.h"
#include "custresdef.h"
#include "..\..\plutommi\mmi\i51\i51Adapter\C\i51Scripts.h"

<?xml version="1.0" encoding="UTF-8"?>
	
#ifdef __COSMOS_MMI_PACKAGE__

#ifdef __I51__
<APP id="APP_I51"

    type="venus" >

#include "../../../plutommi/mmi/i51/i51Plant/res/res_xml/I51_NVRam_Res.h"

</APP>
#endif

#ifdef __I51_Portal__
<APP id="APP_I51_PORTAL"
    package_name="native.mtk.I51.PORTAL"
    name="MENU_ID_I51_TEXT_1"
    img="IMG_I51_Portal_ICON"
    launch="I51_menu_entry"
    type="venus">
    <LAYOUT file="I51_PORTAL.xml"/> 

			//51PK游戏
		<STRING id="MENU_ID_I51_TEXT_1"/>
	    <STRING id="MENU_ID_I51_TEXT_2"/>
	    <STRING id="MENU_ID_I51_TEXT_3"/>
	    <STRING id="MENU_ID_I51_TEXT_4"/>
	    <STRING id="MENU_ID_I51_TEXT_5"/>
	    <STRING id="MENU_ID_I51_TEXT_6"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_1"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_2"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_3"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_4"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_5"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_6"/>

	    #ifdef __I51_Portal_JAVA__
	    	<IMAGE id="IMG_I51_MAIN_ICON">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\SubMenu\\\\NS_JV.png"</IMAGE>
	    	
		    <MENUITEM id="MENU_ID_FNG_I51_1" str="MENU_ID_I51_JAVA_TEXT_1" img="IMG_I51_MAIN_ICON"
		            highlight="I51_mmi_menu_highlight"/>
		    <MENUITEM id="MENU_ID_FNG_I51_2" str="MENU_ID_I51_JAVA_TEXT_2" img="IMG_I51_MAIN_ICON"
		            highlight="I51_mmi_menu_highlight"/>
	    #else
	    //	<IMAGE id="IMG_I51_Portal_ICON">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\SubMenu\\\\Portal.png"</IMAGE>
	    	<IMAGE id="IMG_I51_Portal_ICON">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Portal.png"</IMAGE>
	    	
		    <MENUITEM id="MENU_ID_FNG_I51_1" str="MENU_ID_I51_TEXT_1" img="IMG_I51_Portal_ICON"
		            highlight="I51_mmi_menu_highlight"/>
		    <MENUITEM id="MENU_ID_FNG_I51_2" str="MENU_ID_I51_TEXT_2" img="IMG_I51_Portal_ICON"
		            highlight="I51_mmi_menu_highlight"/>
	    #endif  
</APP>
#endif

#ifdef	__I51_TQ__		// 天气预报
  <APP id="APP_I51_TQ"
	package_name="native.mtk.I51.TQ"
    name="STR_ID_I51_TQ"
    img="IMG_ID_I51_TQ"
    launch="weather_I51_entry"
    type="venus">
    <LAYOUT file="I51_TQ.xml"/>

	<STRING id="STR_ID_I51_TQ"/>
//	<IMAGE id="IMG_ID_I51_TQ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_TQ.png"</IMAGE>
	<IMAGE id="IMG_ID_I51_TQ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_TQ.png"</IMAGE>
	<MENUITEM id="MENU_ID_I51_TQ" str="STR_ID_I51_TQ" img="IMG_ID_I51_TQ"
	    	highlight="weather_I51_entry_highlight"/>
    
  </APP>
#endif

#ifdef	__I51_SavageAdventure__		// 野人逃生记
  <APP id="APP_I51_SA"
	package_name="native.mtk.I51.SA"
    name="STR_ID_I51_SA"
    img="IMG_ID_I51_SA"
    launch="sa_game_entry_2"
    type="venus">
    <LAYOUT file="I51_SA.xml"/>

	<STRING id="STR_ID_I51_SA"/>
//	<IMAGE id="IMG_ID_I51_SA">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\SavageAdventure.png"</IMAGE>
	<IMAGE id="IMG_ID_I51_SA">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\SavageAdventure.png"</IMAGE>
	<MENUITEM id="MENU_ID_I51_SA" str="STR_ID_I51_SA" img="IMG_ID_I51_SA"
			highlight="sa_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_BM__		// 宝石迷情
  <APP id="APP_I51_BM"
	package_name="native.mtk.I51.BM"
    name="STR_ID_I51_BM"
    img="IMG_ID_I51_BM"
    launch="bm_game_entry_2"
    type="venus">
    <LAYOUT file="I51_BM.xml"/>

    <STRING id="STR_ID_I51_BM"/>
//  <IMAGE id="IMG_ID_I51_BM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_BM.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_BM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_BM.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_BM" str="STR_ID_I51_BM" img="IMG_ID_I51_BM"
            highlight="bm_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_DM__		// 金字塔历险记
  <APP id="APP_I51_DM"
	package_name="native.mtk.I51.DM"
    name="STR_ID_I51_DM"
    img="IMG_ID_I51_DM"
    launch="dm_game_entry_2"
    type="venus">
    <LAYOUT file="I51_DM.xml"/>

    <STRING id="STR_ID_I51_DM"/>
//  <IMAGE id="IMG_ID_I51_DM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_DM.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_DM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_DM.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_DM" str="STR_ID_I51_DM" img="IMG_ID_I51_DM"
            highlight="dm_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_ST__		// 街头速递
  <APP id="APP_I51_ST"
	package_name="native.mtk.I51.ST"
    name="STR_ID_I51_ST"
    img="IMG_ID_I51_ST"
    launch="streetboy_game_entry_2"
    type="venus">
    <LAYOUT file="I51_ST.xml"/>

    <STRING id="STR_ID_I51_ST"/>
//  <IMAGE id="IMG_ID_I51_ST">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_ST.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_ST">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_ST.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_ST" str="STR_ID_I51_ST" img="IMG_ID_I51_ST"
            highlight="streetboy_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Loli__		// 营救小LOLI
  <APP id="APP_I51_SL"
	package_name="native.mtk.I51.SL"
    name="STR_ID_I51_SL"
    img="IMG_ID_I51_SL"
    launch="sl_game_entry_2"
    type="venus">
    <LAYOUT file="I51_SL.xml"/>

    <STRING id="STR_ID_I51_SL"/>
//  <IMAGE id="IMG_ID_I51_SL">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Loli.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_SL">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Loli.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_SL" str="STR_ID_I51_SL" img="IMG_ID_I51_SL"
            highlight="sl_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Mjump__		// 跳跳堂
  <APP id="APP_I51_MJ"
	package_name="native.mtk.I51.MJ"
    name="STR_ID_I51_MJ"
    img="IMG_ID_I51_MJ"
    launch="Mjump_game_entry_2"
    type="venus">
    <LAYOUT file="I51_MJ.xml"/> 
	
    <STRING id="STR_ID_I51_MJ"/>
//  <IMAGE id="IMG_ID_I51_MJ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Mjump.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_MJ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Mjump.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_MJ" str="STR_ID_I51_MJ" img="IMG_ID_I51_MJ"
            highlight="Mjump_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_X_Space__	// 完美真相
  <APP id="APP_I51_SPACE"
	package_name="native.mtk.I51.SPACE"
    name="STR_ID_I51_SPACE"
    img="IMG_ID_I51_SPACE"
    launch="X_Space_game_entry_2"
    type="venus">
    <LAYOUT file="I51_SPACE.xml"/>

    <STRING id="STR_ID_I51_SPACE"/>
//  <IMAGE id="IMG_ID_I51_SPACE">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\X_Space.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_SPACE">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\X_Space.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_SPACE" str="STR_ID_I51_SPACE" img="IMG_ID_I51_SPACE"
            highlight="X_Space_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_DontRun__		// 虫虫别跑
  <APP id="APP_I51_DR"
	package_name="native.mtk.I51.DR"
    name="STR_ID_I51_DR"
    img="IMG_ID_I51_DR"
    launch="DR_game_entry_2"
    type="venus">
    <LAYOUT file="I51_DR.xml"/>

    <STRING id="STR_ID_I51_DR"/>
//  <IMAGE id="IMG_ID_I51_DR">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\DontRun.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_DR">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\DontRun.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_DR" str="STR_ID_I51_DR" img="IMG_ID_I51_DR"
            highlight="DR_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_DS__		// 豆豆广场
  <APP id="APP_I51_DS"
	package_name="native.mtk.I51.DS"
    name="STR_ID_I51_DS"
    img="IMG_ID_I51_DS"
    launch="Ds_game_entry_2"
    type="venus">
    <LAYOUT file="I51_DS.xml"/>

    <STRING id="STR_ID_I51_DS"/>
//  <IMAGE id="IMG_ID_I51_DS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_DS.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_DS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_DS.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_DS" str="STR_ID_I51_DS" img="IMG_ID_I51_DS"
            highlight="Ds_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Convoy__		// 保卫钓鱼岛
  <APP id="APP_I51_CY"
	package_name="native.mtk.I51.CY"
    name="STR_ID_I51_CY"
    img="IMG_ID_I51_CY"
    launch="convoy_game_entry_2"
    type="venus">
    <LAYOUT file="I51_CY.xml"/>

    <STRING id="STR_ID_I51_CY"/>
//  <IMAGE id="IMG_ID_I51_CY">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Convoy.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_CY">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Convoy.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_CY" str="STR_ID_I51_CY" img="IMG_ID_I51_CY"
            highlight="convoy_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_HundredStorey__		// 丛林飞将
  <APP id="APP_I51_HS"
	package_name="native.mtk.I51.HS"
    name="STR_ID_I51_HS"
    img="IMG_ID_I51_HS"
    launch="hs_game_entry_2"
    type="venus">
    <LAYOUT file="I51_HS.xml"/>

    <STRING id="STR_ID_I51_HS"/>
//  <IMAGE id="IMG_ID_I51_HS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\HundredStorey.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_HS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\HundredStorey.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_HS" str="STR_ID_I51_HS" img="IMG_ID_I51_HS"
            highlight="hs_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_fighter__		// 玫瑰机动队
  <APP id="APP_I51_FT"
	package_name="native.mtk.I51.FT"
    name="STR_ID_I51_FT"
    img="IMG_ID_I51_FT"
    launch="fighter_game_entry_2"
    type="venus">
    <LAYOUT file="I51_FT.xml"/>

    <STRING id="STR_ID_I51_FT"/>
//  <IMAGE id="IMG_ID_I51_FT">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\fighter.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_FT">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\fighter.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_FT" str="STR_ID_I51_FT" img="IMG_ID_I51_FT"
            highlight="fighter_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_RaceCar__		// 炮弹飞车
  <APP id="APP_I51_RC"
	package_name="native.mtk.I51.RC"
    name="STR_ID_I51_RC"
    img="IMG_ID_I51_RC"
    launch="rc_game_entry_2"
    type="venus">
    <LAYOUT file="I51_RC.xml"/>

    <STRING id="STR_ID_I51_RC"/>
//  <IMAGE id="IMG_ID_I51_RC">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\RaceCar.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_RC">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\RaceCar.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_RC" str="STR_ID_I51_RC" img="IMG_ID_I51_RC"
            highlight="rc_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_VexedJewels__		// 重力宝石
  <APP id="APP_I51_VJ"
	package_name="native.mtk.I51.VJ"
    name="STR_ID_I51_VJ"
    img="IMG_ID_I51_VJ"
    launch="VJ_game_entry_2"
    type="venus">
    <LAYOUT file="I51_VJ.xml"/>

    <STRING id="STR_ID_I51_VJ"/>
//  <IMAGE id="IMG_ID_I51_VJ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\VexedJewels.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_VJ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\VexedJewels.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_VJ" str="STR_ID_I51_VJ" img="IMG_ID_I51_VJ"
            highlight="VJ_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_CutFruit__		// 水果忍者
  <APP id="APP_I51_CF"
	package_name="native.mtk.I51.CF"
    name="STR_ID_I51_CF"
    img="IMG_ID_I51_CF"
    launch="cf_game_entry_2"
    type="venus">
    <LAYOUT file="I51_CF.xml"/>

    <STRING id="STR_ID_I51_CF"/>
//  <IMAGE id="IMG_ID_I51_CF">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\CutFruit.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_CF">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\CutFruit.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_CF" str="STR_ID_I51_CF" img="IMG_ID_I51_CF"
            highlight="cf_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_JigsawPuzzle__		// 魅力拼图
  <APP id="APP_I51_JP"
	package_name="native.mtk.I51.JP"
    name="STR_ID_I51_JP"
    img="IMG_ID_I51_JP"
    launch="JigsawPuzzle_game_entry_2"
    type="venus">
    <LAYOUT file="I51_JP.xml"/>

    <STRING id="STR_ID_I51_JP"/>
//  <IMAGE id="IMG_ID_I51_JP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\JigsawPuzzle.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_JP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\JigsawPuzzle.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_JP" str="STR_ID_I51_JP" img="IMG_ID_I51_JP"
            highlight="JigsawPuzzle_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Snidget__	// 武林飞侠
  <APP id="APP_I51_NIN"
	package_name="native.mtk.I51.NIN"
    name="STR_ID_I51_NIN"
    img="IMG_ID_I51_NIN"
    launch="nin_game_entry_2"
    type="venus">
    <LAYOUT file="I51_NIN.xml"/>

    <STRING id="STR_ID_I51_NIN"/>
//  <IMAGE id="IMG_ID_I51_NIN">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Snidget.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_NIN">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Snidget.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_NIN" str="STR_ID_I51_NIN" img="IMG_ID_I51_NIN"
            highlight="nin_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_IP__		// 成语迷局
  <APP id="APP_I51_IP"
	package_name="native.mtk.I51.IP"
    name="STR_ID_I51_IP"
    img="IMG_ID_I51_IP"
    launch="IdiomPuzzle_game_entry_2"
    type="venus">
    <LAYOUT file="I51_IP.xml"/>

    <STRING id="STR_ID_I51_IP"/>
//  <IMAGE id="IMG_ID_I51_IP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_IP.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_IP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_IP.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_IP" str="STR_ID_I51_IP" img="IMG_ID_I51_IP"
            highlight="IdiomPuzzle_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_DDZ__	// 斗地主
  <APP id="APP_I51_DDZ"
	package_name="native.mtk.I51.DDZ"
    name="STR_ID_I51_DDZ"
    img="IMG_ID_I51_DDZ"
    launch="DDZ_game_entry_2"
    type="venus">
    <LAYOUT file="I51_DDZ.xml"/>

    <STRING id="STR_ID_I51_DDZ"/>
//  <IMAGE id="IMG_ID_I51_DDZ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\DDZ.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_DDZ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\DDZ.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_DDZ" str="STR_ID_I51_DDZ" img="IMG_ID_I51_DDZ"
            highlight="DDZ_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_FruitShot__		// 水果快点
  <APP id="APP_I51_FS"
	package_name="native.mtk.I51.FS"
    name="STR_ID_I51_FS"
    img="IMG_ID_I51_FS"
    launch="FruitShot_game_entry_2"
    type="venus">
    <LAYOUT file="I51_FS.xml"/>

    <STRING id="STR_ID_I51_FS"/>
//  <IMAGE id="IMG_ID_I51_FS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\FruitShot.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_FS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\FruitShot.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_FS" str="STR_ID_I51_FS" img="IMG_ID_I51_FS"
            highlight="FruitShot_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_FishingMaster__	// 捕鱼达人
  <APP id="APP_I51_CFM"
	package_name="native.mtk.I51.CFM"
    name="STR_ID_I51_CFM"
    img="IMG_ID_I51_CFM"
    launch="CFM_game_entry_2"
    type="venus">
    <LAYOUT file="I51_CFM.xml"/>

    <STRING id="STR_ID_I51_CFM"/>
//  <IMAGE id="IMG_ID_I51_CFM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\FishingMaster.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_CFM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\FishingMaster.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_CFM" str="STR_ID_I51_CFM" img="IMG_ID_I51_CFM"
            highlight="CFM_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_AngryBird__	     //愤怒的小鸟
  <APP id="APP_I51_ANGRYBIRD"
	package_name="native.mtk.I51.ANGRYBIRD"
    name="STR_ID_I51_ANGRYBIRD"
    img="IMG_ID_I51_ANGRYBIRD"
    launch="ANGRYBIRD_game_entry_2"
    type="venus">
    <LAYOUT file="I51_ANGRYBIRD.xml"/>

    <STRING id="STR_ID_I51_ANGRYBIRD"/>
//  <IMAGE id="IMG_ID_I51_ANGRYBIRD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\AngryBird.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_ANGRYBIRD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\AngryBird.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_ANGRYBIRD" str="STR_ID_I51_ANGRYBIRD" img="IMG_ID_I51_ANGRYBIRD"
            highlight="ANGRYBIRD_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_GlassDestroyer__	//碎玻璃
  <APP id="APP_I51_GD"
	package_name="native.mtk.I51.GD"
    name="STR_ID_I51_GD"
    img="IMG_ID_I51_GD"
    launch="GD_game_entry_2"
    type="venus">
    <LAYOUT file="I51_GD.xml"/>

    <STRING id="STR_ID_I51_GD"/>
//  <IMAGE id="IMG_ID_I51_GD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\GlassDestroyer.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_GD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\GlassDestroyer.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_GD" str="STR_ID_I51_GD" img="IMG_ID_I51_GD"
            highlight="GD_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_PullSkirt__	// 吹裙子
  <APP id="APP_I51_PS"
	package_name="native.mtk.I51.PS"
    name="STR_ID_I51_PS"
    img="IMG_ID_I51_PS"
    launch="PS_game_entry_2"
    type="venus">
    <LAYOUT file="I51_PS.xml"/>

    <STRING id="STR_ID_I51_PS"/>
//  <IMAGE id="IMG_ID_I51_PS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PullSkirt.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_PS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PullSkirt.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_PS" str="STR_ID_I51_PS" img="IMG_ID_I51_PS"
            highlight="PS_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_TOM__		// tom猫
  <APP id="APP_I51_TOM"
	package_name="native.mtk.I51.TOM"
    name="STR_ID_I51_TOM"
    img="IMG_ID_I51_TOM"
    launch="TOM_game_entry_2"
    type="venus">
    <LAYOUT file="I51_TOM.xml"/>

    <STRING id="STR_ID_I51_TOM"/>
//  <IMAGE id="IMG_ID_I51_TOM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\TOM.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_TOM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\TOM.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_TOM" str="STR_ID_I51_TOM" img="IMG_ID_I51_TOM"
            highlight="TOM_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Ninjump__		// 忍者跳跃
  <APP id="APP_I51_NINJUMP"
	package_name="native.mtk.I51.NINJUMP"
    name="STR_ID_I51_NINJUMP"
    img="IMG_ID_I51_NINJUMP"
    launch="Ninjump_game_entry_2"
    type="venus">
    <LAYOUT file="I51_NINJUMP.xml"/>

    <STRING id="STR_ID_I51_NINJUMP"/>
//  <IMAGE id="IMG_ID_I51_NINJUMP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Ninjump.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_NINJUMP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Ninjump.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_NINJUMP" str="STR_ID_I51_NINJUMP" img="IMG_ID_I51_NINJUMP"
            highlight="Ninjump_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_ZapBird__		// 闪电鸟
  <APP id="APP_I51_ZAPBIRD"
	package_name="native.mtk.I51.ZAPBIRD"
    name="STR_ID_I51_ZAPBIRD"
    img="IMG_ID_I51_ZAPBIRD"
    launch="ZapBird_game_entry_2"
    type="venus">
    <LAYOUT file="I51_ZAPBIRD.xml"/>

    <STRING id="STR_ID_I51_ZAPBIRD"/>
//  <IMAGE id="IMG_ID_I51_ZAPBIRD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\ZapBird.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_ZAPBIRD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\ZapBird.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_ZAPBIRD" str="STR_ID_I51_ZAPBIRD" img="IMG_ID_I51_ZAPBIRD"
            highlight="ZapBird_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_PlantsVsZombies__		// 植物大战僵尸
  <APP id="APP_I51_PLANTSVSZOMBIES"
	package_name="native.mtk.I51.PLANTSVSZOMBIES"
    name="STR_ID_I51_PLANTSVSZOMBIES"
    img="IMG_ID_I51_PLANTSVSZOMBIES"
    launch="PlantsVsZombies_game_entry_2"
    type="venus">
    <LAYOUT file="I51_PLANTSVSZOMBIES.xml"/>

    <STRING id="STR_ID_I51_PLANTSVSZOMBIES"/>
//  <IMAGE id="IMG_ID_I51_PLANTSVSZOMBIES">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PlantsVsZombies.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_PLANTSVSZOMBIES">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PlantsVsZombies.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_PLANTSVSZOMBIES" str="STR_ID_I51_PLANTSVSZOMBIES" img="IMG_ID_I51_PLANTSVSZOMBIES"
            highlight="PlantsVsZombies_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_VXin__		//V信
  <APP id="APP_I51_VXin"
	package_name="native.mtk.I51.VXin"
    name="STR_ID_I51_VXin"
    img="IMG_ID_I51_VXin"
    launch="VXin_game_entry_2"
    type="venus">
    <LAYOUT file="I51_VXin.xml"/>

    <STRING id="STR_ID_I51_VXin"/>
//  <IMAGE id="IMG_ID_I51_VXin">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\VXin.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_VXin">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\VXin.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_VXin" str="STR_ID_I51_VXin" img="IMG_ID_I51_VXin"
            highlight="VXin_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_RipClothes__		//撕衣服
  <APP id="APP_I51_RipClothes"
	package_name="native.mtk.I51.RipClothes"
    name="STR_ID_I51_RipClothes"
    img="IMG_ID_I51_RipClothes"
    launch="RipClothes_game_entry_2"
    type="venus">
    <LAYOUT file="I51_RipClothes.xml"/>

    <STRING id="STR_ID_I51_RipClothes"/>
//  <IMAGE id="IMG_ID_I51_RipClothes">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\RipClothes.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_RipClothes">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\RipClothes.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_RipClothes" str="STR_ID_I51_RipClothes" img="IMG_ID_I51_RipClothes"
            highlight="RipClothes_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_AppStore__		//应用商店
  <APP id="APP_I51_AppStore"
	package_name="native.mtk.I51.AppStore"
    name="STR_ID_I51_AppStore"
    img="IMG_ID_I51_AppStore"
    launch="AppStore_game_entry_2"
    type="venus">
    <LAYOUT file="I51_AppStore.xml"/>

    <STRING id="STR_ID_I51_AppStore"/>
//  <IMAGE id="IMG_ID_I51_AppStore">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\AppStore.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_AppStore">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\AppStore.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_AppStore" str="STR_ID_I51_AppStore" img="IMG_ID_I51_AppStore"
            highlight="AppStore_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
  <APP id="APP_I51_wheresMywater"
	package_name="native.mtk.I51.wheresMywater"
    name="STR_ID_I51_wheresMywater"
    img="IMG_ID_I51_wheresMywater"
    launch="wheresMywater_game_entry_2"
    type="venus">
    <LAYOUT file="I51_wheresMywater.xml"/>

    <STRING id="STR_ID_I51_wheresMywater"/>
//  <IMAGE id="IMG_ID_I51_wheresMywater">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\wheresMywater.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_wheresMywater">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\wheresMywater.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_wheresMywater" str="STR_ID_I51_wheresMywater" img="IMG_ID_I51_wheresMywater"
            highlight="wheresMywater_game_entry_2_highlight"/>

  </APP>
#endif
    
#ifdef	__I51_MusicJam__        //劲乐达人
  <APP id="APP_I51_MusicJam"
	package_name="native.mtk.I51.MusicJam"
    name="STR_ID_I51_MusicJam"
    img="IMG_ID_I51_MusicJam"
    launch="MusicJam_game_entry_2"
    type="venus">
    <LAYOUT file="I51_MusicJam.xml"/>

    <STRING id="STR_ID_I51_MusicJam"/>
//  <IMAGE id="IMG_ID_I51_MusicJam">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\MusicJam.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_MusicJam">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\MusicJam.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_MusicJam" str="STR_ID_I51_MusicJam" img="IMG_ID_I51_MusicJam"
            highlight="MusicJam_game_entry_2_highlight"/>

  </APP>
#endif
    
#ifdef	__I51_AuPlanet__        //啊呜星球
  <APP id="APP_I51_AuPlanet"
	package_name="native.mtk.I51.AuPlanet"
    name="STR_ID_I51_AuPlanet"
    img="IMG_ID_I51_AuPlanet"
    launch="AuPlanet_game_entry_2"
    type="venus">
    <LAYOUT file="I51_AuPlanet.xml"/>

    <STRING id="STR_ID_I51_AuPlanet"/>
//  <IMAGE id="IMG_ID_I51_AuPlanet">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\AuPlanet.png"</IMAGE>
    <IMAGE id="IMG_ID_I51_AuPlanet">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\AuPlanet.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_AuPlanet" str="STR_ID_I51_AuPlanet" img="IMG_ID_I51_AuPlanet"
            highlight="AuPlanet_game_entry_2_highlight"/>

  </APP>
#endif

#else  // no define cosmos

#ifdef __I51_1224_MENUSTYLE__

#ifdef __I51__
<APP id="APP_I51"
 
 >
 
#include "../../../plutommi/mmi/i51/i51Plant/res/res_xml/I51_NVRam_Res.h"
//<INCLUDE file="I51_NVRam_Res.h"/>

</APP>
#endif

#ifdef __I51_Portal__
<APP id="APP_I51_PORTAL"
#if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.portal"
     name = "MENU_ID_I51_TEXT_1"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_I51_Portal_ICON"
#endif
     launch = "I51_menu_entry"
#else
	 name = "MENU_ID_I51_TEXT_1"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "MENU_ID_I51_TEXT_1"
#endif

>

	<STRING id="MENU_ID_I51_TEXT_1"/>
	<STRING id="MENU_ID_I51_TEXT_2"/>
	
	//<IMAGE id="IMG_I51_Portal_ICON">//"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Portal.png"</IMAGE>

	<MENUITEM id="MENU_ID_FNG_I51_1" str="MENU_ID_I51_TEXT_1"
		            highlight="I51_mmi_menu_highlight"/>
	<MENUITEM id="MENU_ID_FNG_I51_2" str="MENU_ID_I51_TEXT_2"
		            highlight="I51_mmi_menu_highlight"/>
</APP>
#endif

#ifdef	__I51_TQ__
  <APP id="APP_I51_TQ"
#if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.TQ"
     name = "STR_ID_I51_TQ"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_TQ"
#endif
     launch = "weather_I51_entry"
#else
	 name = "STR_ID_I51_TQ"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_TQ"
#endif

>

	<STRING id="STR_ID_I51_TQ"/>
	
	//<IMAGE id="IMG_ID_I51_TQ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_TQ.png"</IMAGE>
	
	<MENUITEM id="MENU_ID_I51_TQ" str="STR_ID_I51_TQ" 
	    	highlight="weather_I51_entry_highlight"/>

  </APP>
#endif

#ifdef	__I51_SavageAdventure__
  <APP id="APP_I51_SA"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.SA"
     name = "STR_ID_I51_SA"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_SA"
#endif
     launch = "sa_game_entry_2"
#else
	 name = "STR_ID_I51_SA"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_SA"
#endif

>

	<STRING id="STR_ID_I51_SA"/>
//	<IMAGE id="IMG_ID_I51_SA">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\SavageAdventure.png"</IMAGE>
	<MENUITEM id="MENU_ID_I51_SA" str="STR_ID_I51_SA"
			highlight="sa_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_BM__
  <APP id="APP_I51_BM"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.BM"
     name = "STR_ID_I51_BM"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_BM"
#endif
     launch = "bm_game_entry_2"
#else
	 name = "STR_ID_I51_BM"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_BM"
#endif

>

    <STRING id="STR_ID_I51_BM"/>
//    <IMAGE id="IMG_ID_I51_BM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_BM.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_BM" str="STR_ID_I51_BM"
            highlight="bm_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_DM__
  <APP id="APP_I51_DM"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.DM"
     name = "STR_ID_I51_DM"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_DM"
#endif
     launch = "dm_game_entry_2"
#else
	 name = "STR_ID_I51_DM"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_DM"
#endif

>

    <STRING id="STR_ID_I51_DM"/>
//    <IMAGE id="IMG_ID_I51_DM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_DM.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_DM" str="STR_ID_I51_DM"
            highlight="dm_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_ST__
  <APP id="APP_I51_ST"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.ST"
     name = "STR_ID_I51_ST"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_ST"
#endif
     launch = "streetboy_game_entry_2"
#else
	 name = "STR_ID_I51_ST"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_ST"
#endif

>

    <STRING id="STR_ID_I51_ST"/>
//    <IMAGE id="IMG_ID_I51_ST">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_ST.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_ST" str="STR_ID_I51_ST"
            highlight="streetboy_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Loli__
  <APP id="APP_I51_SL"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.SL"
     name = "STR_ID_I51_SL"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_SL"
#endif
     launch = "sl_game_entry_2"
#else
	 name = "STR_ID_I51_SL"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_SL"
#endif

>

    <STRING id="STR_ID_I51_SL"/>
//    <IMAGE id="IMG_ID_I51_SL">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Loli.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_SL" str="STR_ID_I51_SL"
            highlight="sl_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Mjump__
  <APP id="APP_I51_MJ"
#if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.MJ"
     name = "STR_ID_I51_MJ"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_MJ"
#endif
     launch = "Mjump_game_entry_2"
#else
	 name = "STR_ID_I51_MJ"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_MJ"
#endif

>

    <STRING id="STR_ID_I51_MJ"/>
//    <IMAGE id="IMG_ID_I51_MJ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Mjump.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_MJ" str="STR_ID_I51_MJ"
            highlight="Mjump_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_X_Space__
  <APP id="APP_I51_SPACE"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.SPACE"
     name = "STR_ID_I51_SPACE"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_SPACE"
#endif
     launch = "X_Space_game_entry_2"
#else
	 name = "STR_ID_I51_SPACE"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_SPACE"
#endif

>

    <STRING id="STR_ID_I51_SPACE"/>
//    <IMAGE id="IMG_ID_I51_SPACE">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\X_Space.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_SPACE" str="STR_ID_I51_SPACE"
            highlight="X_Space_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_DontRun__ 
  <APP id="APP_I51_DR"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.DR"
     name = "STR_ID_I51_DR"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_DR"
#endif
     launch = "DR_game_entry_2"
#else
	 name = "STR_ID_I51_DR"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_DR"
#endif

>

    <STRING id="STR_ID_I51_DR"/>
//    <IMAGE id="IMG_ID_I51_DR">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\DontRun.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_DR" str="STR_ID_I51_DR"
            highlight="DR_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_DS__ 
  <APP id="APP_I51_DS"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.DS"
     name = "STR_ID_I51_DS"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_DS"
#endif
     launch = "Ds_game_entry_2"
#else
	 name = "STR_ID_I51_DS"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_DS"
#endif

>

    <STRING id="STR_ID_I51_DS"/>
//    <IMAGE id="IMG_ID_I51_DS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_DS.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_DS" str="STR_ID_I51_DS"
            highlight="Ds_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Convoy__ 
  <APP id="APP_I51_CY"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.CY"
     name = "STR_ID_I51_CY"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_CY"
#endif
     launch = "convoy_game_entry_2"
#else
	 name = "STR_ID_I51_CY"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_CY"
#endif

>

    <STRING id="STR_ID_I51_CY"/>
//    <IMAGE id="IMG_ID_I51_CY">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Convoy.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_CY" str="STR_ID_I51_CY"
            highlight="convoy_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_HundredStorey__ 
  <APP id="APP_I51_HS"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.HS"
     name = "STR_ID_I51_HS"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_HS"
#endif
     launch = "hs_game_entry_2"
#else
	 name = "STR_ID_I51_HS"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_HS"
#endif

>

    <STRING id="STR_ID_I51_HS"/>
//    <IMAGE id="IMG_ID_I51_HS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\HundredStorey.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_HS" str="STR_ID_I51_HS"
            highlight="hs_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_fighter__ 
  <APP id="APP_I51_FT"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.FT"
     name = "STR_ID_I51_FT"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_FT"
#endif
     launch = "fighter_game_entry_2"
#else
	 name = "STR_ID_I51_FT"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_FT"
#endif

>

    <STRING id="STR_ID_I51_FT"/>
//    <IMAGE id="IMG_ID_I51_FT">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\fighter.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_FT" str="STR_ID_I51_FT"
            highlight="fighter_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_RaceCar__	
  <APP id="APP_I51_RC"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.RC"
     name = "STR_ID_I51_RC"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_RC"
#endif
     launch = "rc_game_entry_2"
#else
	 name = "STR_ID_I51_RC"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_RC"
#endif

>

    <STRING id="STR_ID_I51_RC"/>
//    <IMAGE id="IMG_ID_I51_RC">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\RaceCar.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_RC" str="STR_ID_I51_RC"
            highlight="rc_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_VexedJewels__	
  <APP id="APP_I51_VJ"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.VJ"
     name = "STR_ID_I51_VJ"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_VJ"
#endif
     launch = "VJ_game_entry_2"
#else
	 name = "STR_ID_I51_VJ"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_VJ"
#endif

>

    <STRING id="STR_ID_I51_VJ"/>
//    <IMAGE id="IMG_ID_I51_VJ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\VexedJewels.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_VJ" str="STR_ID_I51_VJ"
            highlight="VJ_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_CutFruit__ 
  <APP id="APP_I51_CF"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.CF"
     name = "STR_ID_I51_CF"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_CF"
#endif
     launch = "cf_game_entry_2"
#else
	 name = "STR_ID_I51_CF"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_CF"
#endif

>

    <STRING id="STR_ID_I51_CF"/>
//    <IMAGE id="IMG_ID_I51_CF">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\CutFruit.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_CF" str="STR_ID_I51_CF"
            highlight="cf_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_JigsawPuzzle__ 
  <APP id="APP_I51_JP"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.JP"
     name = "STR_ID_I51_JP"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_JP"
#endif
     launch = "JigsawPuzzle_game_entry_2"
#else
	 name = "STR_ID_I51_JP"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_JP"
#endif

>

    <STRING id="STR_ID_I51_JP"/>
//    <IMAGE id="IMG_ID_I51_JP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\JigsawPuzzle.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_JP" str="STR_ID_I51_JP"
            highlight="JigsawPuzzle_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Snidget__ 
  <APP id="APP_I51_NIN"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.NIN"
     name = "STR_ID_I51_NIN"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_NIN"
#endif
     launch = "nin_game_entry_2"
#else
	 name = "STR_ID_I51_NIN"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_NIN"
#endif

>

    <STRING id="STR_ID_I51_NIN"/>
//    <IMAGE id="IMG_ID_I51_NIN">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Snidget.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_NIN" str="STR_ID_I51_NIN"
            highlight="nin_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_IP__ 
  <APP id="APP_I51_IP"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.IP"
     name = "STR_ID_I51_IP"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_IP"
#endif
     launch = "IdiomPuzzle_game_entry_2"
#else
	 name = "STR_ID_I51_IP"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_IP"
#endif

>

    <STRING id="STR_ID_I51_IP"/>
//    <IMAGE id="IMG_ID_I51_IP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PK_IP.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_IP" str="STR_ID_I51_IP"
            highlight="IdiomPuzzle_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_DDZ__ 
  <APP id="APP_I51_DDZ"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.DDZ"
     name = "STR_ID_I51_DDZ"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_DDZ"
#endif
     launch = "DDZ_game_entry_2"
#else
	 name = "STR_ID_I51_DDZ"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_DDZ"
#endif

>

    <STRING id="STR_ID_I51_DDZ"/>
//    <IMAGE id="IMG_ID_I51_DDZ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\DDZ.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_DDZ" str="STR_ID_I51_DDZ"
            highlight="DDZ_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_FruitShot__ 
  <APP id="APP_I51_FS"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.FS"
     name = "STR_ID_I51_FS"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_FS"
#endif
     launch = "FruitShot_game_entry_2"
#else
	 name = "STR_ID_I51_FS"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_FS"
#endif

>

    <STRING id="STR_ID_I51_FS"/>
//    <IMAGE id="IMG_ID_I51_FS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\FruitShot.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_FS" str="STR_ID_I51_FS"
            highlight="FruitShot_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_FishingMaster__ 
  <APP id="APP_I51_CFM"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.CFM"
     name = "STR_ID_I51_CFM"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_CFM"
#endif
     launch = "CFM_game_entry_2"
#else
	 name = "STR_ID_I51_CFM"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_CFM"
#endif

>

    <STRING id="STR_ID_I51_CFM"/>
//    <IMAGE id="IMG_ID_I51_CFM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\FishingMaster.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_CFM" str="STR_ID_I51_CFM"
            highlight="CFM_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_AngryBird__ 
  <APP id="APP_I51_ANGRYBIRD"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.ANGRYBIRD"
     name = "STR_ID_I51_ANGRYBIRD"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_ANGRYBIRD"
#endif
     launch = "ANGRYBIRD_game_entry_2"
#else
	 name = "STR_ID_I51_ANGRYBIRD"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_ANGRYBIRD"
#endif

>

    <STRING id="STR_ID_I51_ANGRYBIRD"/>
//    <IMAGE id="IMG_ID_I51_ANGRYBIRD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\AngryBird.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_ANGRYBIRD" str="STR_ID_I51_ANGRYBIRD"
            highlight="ANGRYBIRD_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_GlassDestroyer__ 
  <APP id="APP_I51_GD"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.GD"
     name = "STR_ID_I51_GD"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_GD"
#endif
     launch = "GD_game_entry_2"
#else
	 name = "STR_ID_I51_GD"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_GD"
#endif

>

    <STRING id="STR_ID_I51_GD"/>
//    <IMAGE id="IMG_ID_I51_GD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\GlassDestroyer.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_GD" str="STR_ID_I51_GD"
            highlight="GD_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_PullSkirt__ 
  <APP id="APP_I51_PS"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.PS"
     name = "STR_ID_I51_PS"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_PS"
#endif
     launch = "PS_game_entry_2"
#else
	 name = "STR_ID_I51_PS"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_PS"
#endif

>

    <STRING id="STR_ID_I51_PS"/>
//    <IMAGE id="IMG_ID_I51_PS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PullSkirt.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_PS" str="STR_ID_I51_PS"
            highlight="PS_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_TOM__	
  <APP id="APP_I51_TOM"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.TOM"
     name = "STR_ID_I51_TOM"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_TOM"
#endif
     launch = "TOM_game_entry_2"
#else
	 name = "STR_ID_I51_TOM"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_TOM"
#endif

>

    <STRING id="STR_ID_I51_TOM"/>
//    <IMAGE id="IMG_ID_I51_TOM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\TOM.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_TOM" str="STR_ID_I51_TOM"
            highlight="TOM_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_Ninjump__	
  <APP id="APP_I51_NINJUMP"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.NINJUMP"
     name = "STR_ID_I51_NINJUMP"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_NINJUMP"
#endif
     launch = "Ninjump_game_entry_2"
#else
	 name = "STR_ID_I51_NINJUMP"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_NINJUMP"
#endif

>

    <STRING id="STR_ID_I51_NINJUMP"/>
//    <IMAGE id="IMG_ID_I51_NINJUMP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\Ninjump.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_NINJUMP" str="STR_ID_I51_NINJUMP"
            highlight="Ninjump_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_ZapBird__	
  <APP id="APP_I51_ZAPBIRD"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.ZAPBIRD"
     name = "STR_ID_I51_ZAPBIRD"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_ZAPBIRD"
#endif
     launch = "ZapBird_game_entry_2"
#else
	 name = "STR_ID_I51_ZAPBIRD"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_ZAPBIRD"
#endif

>

    <STRING id="STR_ID_I51_ZAPBIRD"/>
//    <IMAGE id="IMG_ID_I51_ZAPBIRD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\ZapBird.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_ZAPBIRD" str="STR_ID_I51_ZAPBIRD"
            highlight="ZapBird_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_PlantsVsZombies__	
  <APP id="APP_I51_PLANTSVSZOMBIES"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.PLANTSVSZOMBIES"
     name = "STR_ID_I51_PLANTSVSZOMBIES"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_PLANTSVSZOMBIES"
#endif
     launch = "PlantsVsZombies_game_entry_2"
#else
	 name = "STR_ID_I51_PLANTSVSZOMBIES"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_PLANTSVSZOMBIES"
#endif

>

    <STRING id="STR_ID_I51_PLANTSVSZOMBIES"/>
//    <IMAGE id="IMG_ID_I51_PLANTSVSZOMBIES">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\PlantsVsZombies.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_PLANTSVSZOMBIES" str="STR_ID_I51_PLANTSVSZOMBIES"
            highlight="PlantsVsZombies_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_VXin__ 
  <APP id="APP_I51_VXin"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.VXin"
     name = "STR_ID_I51_VXin"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_VXin"
#endif
     launch = "VXin_game_entry_2"
#else
	 name = "STR_ID_I51_VXin"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_VXin"
#endif

>

    <STRING id="STR_ID_I51_VXin"/>
//    <IMAGE id="IMG_ID_I51_VXin">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\VXin.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_VXin" str="STR_ID_I51_VXin"
            highlight="VXin_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_RipClothes__ 
  <APP id="APP_I51_RipClothes"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.RipClothes"
     name = "STR_ID_I51_RipClothes"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_RipClothes"
#endif
     launch = "RipClothes_game_entry_2"
#else
	 name = "STR_ID_I51_RipClothes"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_RipClothes"
#endif

>

    <STRING id="STR_ID_I51_RipClothes"/>
//    <IMAGE id="IMG_ID_I51_RipClothes">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\RipClothes.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_RipClothes" str="STR_ID_I51_RipClothes"
            highlight="RipClothes_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_AppStore__ 
  <APP id="APP_I51_AppStore"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.AppStore"
     name = "STR_ID_I51_AppStore"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_AppStore"
#endif
     launch = "AppStore_game_entry_2"
#else
	 name = "STR_ID_I51_AppStore"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_AppStore"
#endif

>

    <STRING id="STR_ID_I51_AppStore"/>
//    <IMAGE id="IMG_ID_I51_AppStore">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\AppStore.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_AppStore" str="STR_ID_I51_AppStore"
            highlight="AppStore_game_entry_2_highlight"/>

  </APP>
#endif

#ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
  <APP id="APP_I51_wheresMywater"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.wheresMywater"
     name = "STR_ID_I51_wheresMywater"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_wheresMywater"
#endif
     launch = "wheresMywater_game_entry_2"
#else
	 name = "STR_ID_I51_wheresMywater"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_wheresMywater"
#endif

>

    <STRING id="STR_ID_I51_wheresMywater"/>
//    <IMAGE id="IMG_ID_I51_wheresMywater">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\wheresMywater.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_wheresMywater" str="STR_ID_I51_wheresMywater"
            highlight="wheresMywater_game_entry_2_highlight"/>

  </APP>
#endif
    
#ifdef	__I51_MusicJam__        //劲乐达人
  <APP id="APP_I51_MusicJam"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.MusicJam"
     name = "STR_ID_I51_MusicJam"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_MusicJam"
#endif
     launch = "MusicJam_game_entry_2"
#else
	 name = "STR_ID_I51_MusicJam"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_MusicJam"
#endif

>

    <STRING id="STR_ID_I51_MusicJam"/>
//    <IMAGE id="IMG_ID_I51_MusicJam">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\MusicJam.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_MusicJam" str="STR_ID_I51_MusicJam"
            highlight="MusicJam_game_entry_2_highlight"/>

  </APP>
#endif
    
#ifdef	__I51_AuPlanet__        //啊呜星球
  <APP id="APP_I51_AuPlanet"
  #if !defined(__COSMOS_MMI_PACKAGE__)
#if defined(__MMI_APP_MANAGER_SUPPORT__)
     package_name = "native.i51.AuPlanet"
     name = "STR_ID_I51_AuPlanet"
#if defined(__MMI_VUI_LAUNCHER__) || defined(__MMI_MAINMENU_SPREAD_SUPPORT__)
     img = "IMG_ID_I51_AuPlanet"
#endif
     launch = "AuPlanet_game_entry_2"
#else
	 name = "STR_ID_I51_AuPlanet"
#endif /*__MMI_APP_MANAGER_SUPPORT__*/
#else
	 name = "STR_ID_I51_AuPlanet"
#endif

>

    <STRING id="STR_ID_I51_AuPlanet"/>
//    <IMAGE id="IMG_ID_I51_AuPlanet">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\50_50\\\\AuPlanet.png"</IMAGE>
    <MENUITEM id="MENU_ID_I51_AuPlanet" str="STR_ID_I51_AuPlanet"
            highlight="AuPlanet_game_entry_2_highlight"/>

  </APP>
#endif

#else

<APP id="APP_I51">

	#include "../../../plutommi/mmi/i51/i51Plant/res/res_xml/I51_NVRam_Res.h"

    /* String ID ,Image ID and MenuItem ID of you Application; we wiI51 get string from ref_list.txt for all languages */

	#ifdef __I51_Portal__			//51PK游戏
		<STRING id="MENU_ID_I51_TEXT_1"/>
	    <STRING id="MENU_ID_I51_TEXT_2"/>
	    <STRING id="MENU_ID_I51_TEXT_3"/>
	    <STRING id="MENU_ID_I51_TEXT_4"/>
	    <STRING id="MENU_ID_I51_TEXT_5"/>
	    <STRING id="MENU_ID_I51_TEXT_6"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_1"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_2"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_3"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_4"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_5"/>
	    <STRING id="MENU_ID_I51_JAVA_TEXT_6"/>

	    #ifdef __I51_Portal_JAVA__
	    	<IMAGE id="IMG_I51_MAIN_ICON">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\SubMenu\\\\NS_JV.png"</IMAGE>
	    	
		    <MENUITEM id="MENU_ID_FNG_I51_1" str="MENU_ID_I51_JAVA_TEXT_1" img="IMG_I51_MAIN_ICON"
		            highlight="I51_mmi_menu_highlight"/>
		    <MENUITEM id="MENU_ID_FNG_I51_2" str="MENU_ID_I51_JAVA_TEXT_2" img="IMG_I51_MAIN_ICON"
		            highlight="I51_mmi_menu_highlight"/>
	    #else
	    //	<IMAGE id="IMG_I51_Portal_ICON">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\SubMenu\\\\Portal.png"</IMAGE>
	    	<IMAGE id="IMG_I51_Portal_ICON">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Portal.png"</IMAGE>
	    	
		    <MENUITEM id="MENU_ID_FNG_I51_1" str="MENU_ID_I51_TEXT_1" img="IMG_I51_Portal_ICON"
		            highlight="I51_mmi_menu_highlight"/>
		    <MENUITEM id="MENU_ID_FNG_I51_2" str="MENU_ID_I51_TEXT_2" img="IMG_I51_Portal_ICON"
		            highlight="I51_mmi_menu_highlight"/>
	    #endif  

	#endif
    
	#ifdef	__I51_TQ__		// 天气预报
		<STRING id="STR_ID_I51_TQ"/>
		<IMAGE id="IMG_ID_I51_TQ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_TQ.png"</IMAGE>
		<MENUITEM id="MENU_ID_I51_TQ" str="STR_ID_I51_TQ" img="IMG_ID_I51_TQ"
		    	highlight="weather_I51_entry_highlight"/>

	#endif
	
	#ifdef	__I51_SavageAdventure__		// 野人逃生记
		<STRING id="STR_ID_I51_SA"/>
		<IMAGE id="IMG_ID_I51_SA">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\SavageAdventure.png"</IMAGE>
		<MENUITEM id="MENU_ID_I51_SA" str="STR_ID_I51_SA" img="IMG_ID_I51_SA"
				highlight="sa_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_BM__		// 宝石迷情
	    <STRING id="STR_ID_I51_BM"/>
	    <IMAGE id="IMG_ID_I51_BM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_BM.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_BM" str="STR_ID_I51_BM" img="IMG_ID_I51_BM"
	            highlight="bm_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_DM__		// 金字塔历险记
	    <STRING id="STR_ID_I51_DM"/>
	    <IMAGE id="IMG_ID_I51_DM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_DM.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_DM" str="STR_ID_I51_DM" img="IMG_ID_I51_DM"
	            highlight="dm_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_ST__		// 街头速递
	    <STRING id="STR_ID_I51_ST"/>
	    <IMAGE id="IMG_ID_I51_ST">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_ST.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_ST" str="STR_ID_I51_ST" img="IMG_ID_I51_ST"
	            highlight="streetboy_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_Loli__		// 营救小LOLI
	    <STRING id="STR_ID_I51_SL"/>
	    <IMAGE id="IMG_ID_I51_SL">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Loli.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_SL" str="STR_ID_I51_SL" img="IMG_ID_I51_SL"
	            highlight="sl_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_Mjump__		// 跳跳堂
	    <STRING id="STR_ID_I51_MJ"/>
	    <IMAGE id="IMG_ID_I51_MJ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Mjump.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_MJ" str="STR_ID_I51_MJ" img="IMG_ID_I51_MJ"
	            highlight="Mjump_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_X_Space__	// 完美真相
	    <STRING id="STR_ID_I51_SPACE"/>
	    <IMAGE id="IMG_ID_I51_SPACE">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\X_Space.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_SPACE" str="STR_ID_I51_SPACE" img="IMG_ID_I51_SPACE"
	            highlight="X_Space_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_DontRun__		// 虫虫别跑
	    <STRING id="STR_ID_I51_DR"/>
	    <IMAGE id="IMG_ID_I51_DR">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\DontRun.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_DR" str="STR_ID_I51_DR" img="IMG_ID_I51_DR"
	            highlight="DR_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_DS__		// 豆豆广场
	    <STRING id="STR_ID_I51_DS"/>
	    <IMAGE id="IMG_ID_I51_DS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_DS.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_DS" str="STR_ID_I51_DS" img="IMG_ID_I51_DS"
	            highlight="Ds_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_Convoy__		// 保卫钓鱼岛
	    <STRING id="STR_ID_I51_CY"/>
	    <IMAGE id="IMG_ID_I51_CY">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Convoy.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_CY" str="STR_ID_I51_CY" img="IMG_ID_I51_CY"
	            highlight="convoy_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_HundredStorey__		// 丛林飞将
	    <STRING id="STR_ID_I51_HS"/>
	    <IMAGE id="IMG_ID_I51_HS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\HundredStorey.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_HS" str="STR_ID_I51_HS" img="IMG_ID_I51_HS"
	            highlight="hs_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_fighter__		// 玫瑰机动队
	    <STRING id="STR_ID_I51_FT"/>
	    <IMAGE id="IMG_ID_I51_FT">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\fighter.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_FT" str="STR_ID_I51_FT" img="IMG_ID_I51_FT"
	            highlight="fighter_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_RaceCar__		// 炮弹飞车
	    <STRING id="STR_ID_I51_RC"/>
	    <IMAGE id="IMG_ID_I51_RC">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\RaceCar.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_RC" str="STR_ID_I51_RC" img="IMG_ID_I51_RC"
	            highlight="rc_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_VexedJewels__		// 重力宝石
	    <STRING id="STR_ID_I51_VJ"/>
	    <IMAGE id="IMG_ID_I51_VJ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\VexedJewels.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_VJ" str="STR_ID_I51_VJ" img="IMG_ID_I51_VJ"
	            highlight="VJ_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_CutFruit__		// 水果忍者
	    <STRING id="STR_ID_I51_CF"/>
	    <IMAGE id="IMG_ID_I51_CF">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\CutFruit.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_CF" str="STR_ID_I51_CF" img="IMG_ID_I51_CF"
	            highlight="cf_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_JigsawPuzzle__		// 魅力拼图
	    <STRING id="STR_ID_I51_JP"/>
	    <IMAGE id="IMG_ID_I51_JP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\JigsawPuzzle.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_JP" str="STR_ID_I51_JP" img="IMG_ID_I51_JP"
	            highlight="JigsawPuzzle_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_Snidget__	// 武林飞侠
	    <STRING id="STR_ID_I51_NIN"/>
	    <IMAGE id="IMG_ID_I51_NIN">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Snidget.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_NIN" str="STR_ID_I51_NIN" img="IMG_ID_I51_NIN"
	            highlight="nin_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_IP__		// 成语迷局
	    <STRING id="STR_ID_I51_IP"/>
	    <IMAGE id="IMG_ID_I51_IP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PK_IP.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_IP" str="STR_ID_I51_IP" img="IMG_ID_I51_IP"
	            highlight="IdiomPuzzle_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_DDZ__	// 斗地主
	    <STRING id="STR_ID_I51_DDZ"/>
	    <IMAGE id="IMG_ID_I51_DDZ">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\DDZ.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_DDZ" str="STR_ID_I51_DDZ" img="IMG_ID_I51_DDZ"
	            highlight="DDZ_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_FruitShot__		// 水果快点
	    <STRING id="STR_ID_I51_FS"/>
	    <IMAGE id="IMG_ID_I51_FS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\FruitShot.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_FS" str="STR_ID_I51_FS" img="IMG_ID_I51_FS"
	            highlight="FruitShot_game_entry_2_highlight"/>

	#endif
	
	#ifdef	__I51_FishingMaster__	// 捕鱼达人
	    <STRING id="STR_ID_I51_CFM"/>
	    <IMAGE id="IMG_ID_I51_CFM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\FishingMaster.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_CFM" str="STR_ID_I51_CFM" img="IMG_ID_I51_CFM"
	            highlight="CFM_game_entry_2_highlight"/>

	#endif

	#ifdef	__I51_AngryBird__	     //愤怒的小鸟
	    <STRING id="STR_ID_I51_ANGRYBIRD"/>
	    <IMAGE id="IMG_ID_I51_ANGRYBIRD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\AngryBird.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_ANGRYBIRD" str="STR_ID_I51_ANGRYBIRD" img="IMG_ID_I51_ANGRYBIRD"
	            highlight="ANGRYBIRD_game_entry_2_highlight"/>

	#endif

	#ifdef	__I51_GlassDestroyer__	//碎玻璃
	    <STRING id="STR_ID_I51_GD"/>
	    <IMAGE id="IMG_ID_I51_GD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\GlassDestroyer.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_GD" str="STR_ID_I51_GD" img="IMG_ID_I51_GD"
	            highlight="GD_game_entry_2_highlight"/>

	#endif

	#ifdef	__I51_PullSkirt__	// 吹裙子
	    <STRING id="STR_ID_I51_PS"/>
	    <IMAGE id="IMG_ID_I51_PS">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PullSkirt.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_PS" str="STR_ID_I51_PS" img="IMG_ID_I51_PS"
	            highlight="PS_game_entry_2_highlight"/>

	#endif

	#ifdef	__I51_TOM__		// tom猫
	    <STRING id="STR_ID_I51_TOM"/>
	    <IMAGE id="IMG_ID_I51_TOM">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\TOM.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_TOM" str="STR_ID_I51_TOM" img="IMG_ID_I51_TOM"
	            highlight="TOM_game_entry_2_highlight"/>

	#endif

	#ifdef	__I51_Ninjump__		// 忍者跳跃
	    <STRING id="STR_ID_I51_NINJUMP"/>
	    <IMAGE id="IMG_ID_I51_NINJUMP">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\Ninjump.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_NINJUMP" str="STR_ID_I51_NINJUMP" img="IMG_ID_I51_NINJUMP"
	            highlight="Ninjump_game_entry_2_highlight"/>
	#endif

	#ifdef	__I51_ZapBird__		// 闪电鸟
	    <STRING id="STR_ID_I51_ZAPBIRD"/>
	    <IMAGE id="IMG_ID_I51_ZAPBIRD">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\ZapBird.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_ZAPBIRD" str="STR_ID_I51_ZAPBIRD" img="IMG_ID_I51_ZAPBIRD"
	            highlight="ZapBird_game_entry_2_highlight"/>
	#endif

	#ifdef	__I51_PlantsVsZombies__		// 植物大战僵尸
	    <STRING id="STR_ID_I51_PLANTSVSZOMBIES"/>
	    <IMAGE id="IMG_ID_I51_PLANTSVSZOMBIES">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\PlantsVsZombies.png"</IMAGE>
	    <MENUITEM id="MENU_ID_I51_PLANTSVSZOMBIES" str="STR_ID_I51_PLANTSVSZOMBIES" img="IMG_ID_I51_PLANTSVSZOMBIES"
	            highlight="PlantsVsZombies_game_entry_2_highlight"/>
	#endif
	
	#ifdef	__I51_VXin__		//V信

    	<STRING id="STR_ID_I51_VXin"/>
	    <IMAGE id="IMG_ID_I51_VXin">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\VXin.png"</IMAGE>
    	<MENUITEM id="MENU_ID_I51_VXin" str="STR_ID_I51_VXin" img="IMG_ID_I51_VXin"
            	highlight="VXin_game_entry_2_highlight"/>
	#endif

	#ifdef	__I51_RipClothes__		//撕衣服

    	<STRING id="STR_ID_I51_RipClothes"/>
	    <IMAGE id="IMG_ID_I51_RipClothes">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\RipClothes.png"</IMAGE>
    	<MENUITEM id="MENU_ID_I51_RipClothes" str="STR_ID_I51_RipClothes" img="IMG_ID_I51_RipClothes"
            	highlight="RipClothes_game_entry_2_highlight"/>
	#endif

	#ifdef	__I51_AppStore__		//应用商店

    	<STRING id="STR_ID_I51_AppStore"/>
	    <IMAGE id="IMG_ID_I51_AppStore">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\AppStore.png"</IMAGE>
    	<MENUITEM id="MENU_ID_I51_AppStore" str="STR_ID_I51_AppStore" img="IMG_ID_I51_AppStore"
            	highlight="AppStore_game_entry_2_highlight"/>
	#endif

	#ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
    	<STRING id="STR_ID_I51_wheresMywater"/>
	    <IMAGE id="IMG_ID_I51_wheresMywater">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\wheresMywater.png"</IMAGE>
    	<MENUITEM id="MENU_ID_I51_wheresMywater" str="STR_ID_I51_wheresMywater" img="IMG_ID_I51_wheresMywater"
            	highlight="wheresMywater_game_entry_2_highlight"/>
    #endif
    
    #ifdef	__I51_MusicJam__        //劲乐达人
    	<STRING id="STR_ID_I51_MusicJam"/>
	    <IMAGE id="IMG_ID_I51_MusicJam">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\MusicJam.png"</IMAGE>
    	<MENUITEM id="MENU_ID_I51_MusicJam" str="STR_ID_I51_MusicJam" img="IMG_ID_I51_MusicJam"
            	highlight="MusicJam_game_entry_2_highlight"/>
    #endif
    
    #ifdef	__I51_AuPlanet__        //啊呜星球
    	<STRING id="STR_ID_I51_AuPlanet"/>
	    <IMAGE id="IMG_ID_I51_AuPlanet">"..\\\\..\\\\mmi\\\\I51\\\\i51Plant\\\\Res\\\\image\\\\MainMenu\\\\60_60\\\\AuPlanet.png"</IMAGE>
    	<MENUITEM id="MENU_ID_I51_AuPlanet" str="STR_ID_I51_AuPlanet" img="IMG_ID_I51_AuPlanet"
            	highlight="AuPlanet_game_entry_2_highlight"/>
    #endif
	
</APP>

#endif //__I51_1224_MENUSTYLE__

#endif //__COSMOS_MMI_PACKAGE__
