//Menu ID add to 'Res_MainMenu.c' as Submenus of MAIN_MENU_FUNANDGAMES_MENUID

	#ifdef  __I51_Portal__		//51PK游戏
		MENU_ID_FNG_I51_1,
	#endif
	
	#ifdef	__I51_TQ__		// 天气预报
		MENU_ID_I51_TQ,
	#endif
	
	#ifdef	__I51_SavageAdventure__		// 野人逃生记
		MENU_ID_I51_SA,
	#endif
	
	#ifdef	__I51_BM__		// 宝石迷情
		MENU_ID_I51_BM,
	#endif
	
	#ifdef	__I51_DM__		// 金字塔历险记
		MENU_ID_I51_DM,
	#endif
	
	#ifdef	__I51_ST__		// 街头速递
		MENU_ID_I51_ST,
	#endif
	
	#ifdef	__I51_Loli__		// 营救小LOLI
		MENU_ID_I51_SL,
	#endif
	
	#ifdef	__I51_Mjump__		// 跳跳堂
		MENU_ID_I51_MJ,
	#endif
	
	#ifdef	__I51_X_Space__	// 完美真相
		MENU_ID_I51_SPACE,
	#endif
	
	#ifdef	__I51_DontRun__		// 虫虫别跑
		MENU_ID_I51_DR,
	#endif
	
	#ifdef	__I51_DS__		// 豆豆广场
		MENU_ID_I51_DS,
	#endif
	
	#ifdef	__I51_Convoy__		// 保卫钓鱼岛
		MENU_ID_I51_CY,
	#endif
	
	#ifdef	__I51_HundredStorey__		// 丛林飞将
		MENU_ID_I51_HS,
	#endif
	
	#ifdef	__I51_fighter__		// 玫瑰机动队
		MENU_ID_I51_FT,
	#endif
	
	#ifdef	__I51_RaceCar__		// 炮弹飞车
		MENU_ID_I51_RC,
	#endif
	
	#ifdef	__I51_VexedJewels__		// 重力宝石
		MENU_ID_I51_VJ,
	#endif
	
	#ifdef	__I51_CutFruit__		// 水果忍者
		MENU_ID_I51_CF,
	#endif
	
	#ifdef	__I51_JigsawPuzzle__		// 魅力拼图
		MENU_ID_I51_JP,
	#endif
	
	#ifdef	__I51_Snidget__	// 武林飞侠
		MENU_ID_I51_NIN,
	#endif
	
	#ifdef	__I51_IP__		// 成语迷局
		MENU_ID_I51_IP,
	#endif
	
	#ifdef	__I51_DDZ__	// 斗地主
		MENU_ID_I51_DDZ,
	#endif
	
	#ifdef	__I51_FruitShot__		// 水果快点
		MENU_ID_I51_FS,
	#endif
	
	#ifdef	__I51_FishingMaster__	// 捕鱼达人
		MENU_ID_I51_CFM,
	#endif

	#ifdef	__I51_AngryBird__ 	// 愤怒的小鸟
		MENU_ID_I51_ANGRYBIRD,
	#endif
	
	#ifdef	__I51_GlassDestroyer__	// 碎玻璃
		MENU_ID_I51_GD,
	#endif
	
	#ifdef	__I51_PullSkirt__	// 吹裙子
		MENU_ID_I51_PS,
	#endif

	#ifdef	__I51_TOM__	// TOM
		MENU_ID_I51_TOM,
	#endif

	#ifdef	__I51_Ninjump__		// 忍者跳跃
		MENU_ID_I51_NINJUMP,
	#endif

	#ifdef	__I51_ZapBird__		// 闪电鸟
		MENU_ID_I51_ZAPBIRD,
	#endif

	#ifdef	__I51_PlantsVsZombies__		// 植物大战僵尸
		MENU_ID_I51_PLANTSVSZOMBIES,
	#endif
    
	#ifdef	__I51_VXin__		//V信
		MENU_ID_I51_VXin,
    #endif

    #ifdef	__I51_RipClothes__		//撕衣服
		MENU_ID_I51_RipClothes,
    #endif

    #ifdef	__I51_AppStore__		//应用商店
		MENU_ID_I51_AppStore,
    #endif

    #ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
        MENU_ID_I51_wheresMywater,
    #endif
    
    #ifdef	__I51_MusicJam__        //劲乐达人
        MENU_ID_I51_MusicJam,
    #endif
    
    #ifdef	__I51_AuPlanet__        //啊呜星球
        MENU_ID_I51_AuPlanet,
    #endif
    
//#else

	/*#ifdef  __I51_Portal__		//51PK游戏
		<MENUITEM_ID>MENU_ID_FNG_I51_1</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_TQ__		// 天气预报
		<MENUITEM_ID>MENU_ID_I51_TQ</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_SavageAdventure__		// 野人逃生记
		<MENUITEM_ID>MENU_ID_I51_SA</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_BM__		// 宝石迷情
		<MENUITEM_ID>MENU_ID_I51_BM</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_DM__		// 金字塔历险记
		<MENUITEM_ID>MENU_ID_I51_DM</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_ST__		// 街头速递
		<MENUITEM_ID>MENU_ID_I51_ST</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_Loli__		// 营救小LOLI
		<MENUITEM_ID>MENU_ID_I51_SL</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_Mjump__		// 跳跳堂
		<MENUITEM_ID>MENU_ID_I51_MJ</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_X_Space__	// 完美真相
		<MENUITEM_ID>MENU_ID_I51_SPACE</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_DontRun__		// 虫虫别跑
		<MENUITEM_ID>MENU_ID_I51_DR</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_DS__		// 豆豆广场
		<MENUITEM_ID>MENU_ID_I51_DS</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_Convoy__		// 保卫钓鱼岛
		<MENUITEM_ID>MENU_ID_I51_CY</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_HundredStorey__		// 丛林飞将
		<MENUITEM_ID>MENU_ID_I51_HS</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_fighter__		// 玫瑰机动队
		<MENUITEM_ID>MENU_ID_I51_FT</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_RaceCar__		// 炮弹飞车
		<MENUITEM_ID>MENU_ID_I51_RC</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_VexedJewels__		// 重力宝石
		<MENUITEM_ID>MENU_ID_I51_VJ</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_CutFruit__		// 水果忍者
		<MENUITEM_ID>MENU_ID_I51_CF</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_JigsawPuzzle__		// 魅力拼图
		<MENUITEM_ID>MENU_ID_I51_JP</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_Snidget__	// 武林飞侠
		<MENUITEM_ID>MENU_ID_I51_NIN</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_IP__		// 成语迷局
		<MENUITEM_ID>MENU_ID_I51_IP</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_DDZ__	// 斗地主
		<MENUITEM_ID>MENU_ID_I51_DDZ</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_FruitShot__		// 水果快点
		<MENUITEM_ID>MENU_ID_I51_FS</MENUITEM_ID>
	#endif
	
	#ifdef	__I51_FishingMaster__	// 捕鱼达人
		<MENUITEM_ID>MENU_ID_I51_CFM</MENUITEM_ID>
	#endif

	#ifdef	__I51_TOM_CAT__	//TOM猫
		<MENUITEM_ID>MENU_ID_I51_TOM_CAT</MENUITEM_ID>
	#endif

	#ifdef	__I51_ZapBird__		// 闪电鸟
		<MENUITEM_ID>MENU_ID_I51_ZAPBIRD</MENUITEM_ID>
	#endif

	#ifdef	__I51_PlantsVsZombies__		// 植物大战僵尸
		<MENUITEM_ID>MENU_ID_I51_PLANTSVSZOMBIES</MENUITEM_ID>
	#endif
	
    #ifdef	__I51_VXin__		//V信
		<MENUITEM_ID>MENU_ID_I51_VXin</MENUITEM_ID>
    #endif

    #ifdef	__I51_RipClothes__		//撕衣服
		<MENUITEM_ID>MENU_ID_I51_RipClothes</MENUITEM_ID>
    #endif

    #ifdef	__I51_AppStore__		
		<MENUITEM_ID>MENU_ID_I51_AppStore</MENUITEM_ID>
    #endif

    #ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
    
    #endif
    
    #ifdef	__I51_MusicJam__        //劲乐达人
    
    #endif
    
    #ifdef	__I51_AuPlanet__        //啊呜星球

    #endif
*/

