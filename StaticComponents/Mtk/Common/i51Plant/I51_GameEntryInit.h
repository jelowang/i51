//game entry init

#if !defined(MMI_BOOTUP_INIT_ADP)&&!defined(VAPP_BOOTUP_INIT_ADP)

    	{
		extern void i51Boot( void );	
		i51Boot() ;
		}
	#ifdef 	__I51_Portal__		//51PK游戏
		{
		extern void I51_mmi_menu_init (void);
		I51_mmi_menu_init();
		}
	#endif
		
	#ifdef	__I51_TQ__		// 天气预报
		{
		extern void weather_I51_entry_init (void);
		weather_I51_entry_init();
		}
	#endif
		
	#ifdef	__I51_SavageAdventure__		// 野人逃生记
		{
		extern void sa_game_entry_init_2 (void);
		sa_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_BM__		// 宝石迷情
		{extern void bm_game_entry_init_2 (void);
		bm_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_DM__		// 金字塔历险记
		{
		extern void dm_game_entry_init_2 (void);
		dm_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_ST__		// 街头速递
		{
		extern void streetboy_game_entry_init_2 (void);
		streetboy_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_Loli__		// 营救小LOLI
		{
		extern void sl_game_entry_init_2 (void);
		sl_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_Mjump__		// 跳跳堂
		{
		extern void Mjump_game_entry_init_2 (void);
		Mjump_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_X_Space__	// 完美真相
		{
		extern void X_Space_game_entry_init_2 (void);
		X_Space_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_DontRun__		// 虫虫别跑
		{
		extern void DR_game_entry_init_2 (void);
		DR_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_DS__		// 豆豆广场
		{
		extern void Ds_game_entry_init_2 (void);
		Ds_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_Convoy__		// 保卫钓鱼岛
		{
		extern void convoy_game_entry_init_2 (void);
		convoy_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_HundredStorey__		// 丛林飞将
		{
		extern void hs_game_entry_init_2 (void);
		hs_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_fighter__		// 玫瑰机动队
		{
		extern void fighter_game_entry_init_2 (void);
		fighter_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_RaceCar__		// 炮弹飞车
		{
		extern void rc_game_entry_init_2 (void);
		rc_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_VexedJewels__		// 重力宝石
		{
		extern void VJ_game_entry_init_2 (void);
		VJ_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_CutFruit__		// 水果忍者
		{
		extern void cf_game_entry_init_2 (void);
		cf_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_JigsawPuzzle__		// 魅力拼图
		{
		extern void JigsawPuzzle_game_entry_init_2 (void);
		JigsawPuzzle_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_Snidget__	// 武林飞侠
		{
		extern void nin_game_entry_init_2 (void);
		nin_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_IP__		// 成语迷局
		{
		extern void IdiomPuzzle_game_entry_init_2 (void);
		IdiomPuzzle_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_DDZ__	// 斗地主
		{
		extern void DDZ_game_entry_init_2 (void);
		DDZ_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_FruitShot__		// 水果快点
		{
		extern void FruitShot_game_entry_init_2 (void);
		FruitShot_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_FishingMaster__		// 捕鱼达人
		{
		extern void CFM_game_entry_init_2 (void);
		CFM_game_entry_init_2();
		}
	#endif
	
	#ifdef	__I51_AngryBird__ 	// 愤怒的小鸟
		{
		
		extern void ANGRYBIRD_game_entry_init_2 (void);
		ANGRYBIRD_game_entry_init_2();	
		
		}
	#endif
	
	#ifdef	__I51_GlassDestroyer__	// 碎玻璃
		{
		extern void GD_game_entry_init_2 (void);
		GD_game_entry_init_2();	
		}
	#endif
	
	#ifdef	__I51_PullSkirt__	// 吹裙子
		{
		extern void PS_game_entry_init_2 (void);
		PS_game_entry_init_2();	
		}
	#endif

	#ifdef	__I51_TOM__	// TOM
		{
		extern void TOM_game_entry_init_2 (void);
		TOM_game_entry_init_2();	
		}
	#endif

	#ifdef	__I51_Ninjump__		// 忍者跳跃
		{
		extern void Ninjump_game_entry_init_2 (void);
		Ninjump_game_entry_init_2();	
		}	
	#endif

	#ifdef	__I51_ZapBird__		// 闪电鸟
		extern void ZapBird_game_entry_init_2 (void);
		ZapBird_game_entry_init_2();	
	#endif

	#ifdef	__I51_PlantsVsZombies__		// 植物大战僵尸
		extern void PlantsVsZombies_game_entry_init_2 (void);
		PlantsVsZombies_game_entry_init_2();	
	#endif	

    #ifdef	__I51_VXin__		//V信
		extern void VXin_game_entry_init_2 (void);
		VXin_game_entry_init_2();	
    #endif

    #ifdef	__I51_RipClothes__		//撕衣服
		extern void RipClothes_game_entry_init_2 (void);
		RipClothes_game_entry_init_2();	
    #endif

    #ifdef	__I51_AppStore__		//应用商店
		extern void AppStore_game_entry_init_2 (void);
		AppStore_game_entry_init_2();	
    #endif
    
    #ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
		extern void wheresMywater_game_entry_init_2 (void);
		wheresMywater_game_entry_init_2();	
    #endif
    
    #ifdef	__I51_MusicJam__        //劲乐达人
		extern void MusicJam_game_entry_init_2 (void);
		MusicJam_game_entry_init_2();	
    #endif
    
    #ifdef	__I51_AuPlanet__        //啊呜星球
		extern void AuPlanet_game_entry_init_2 (void);
		AuPlanet_game_entry_init_2();	
    #endif

#elif defined(VAPP_BOOTUP_INIT_ADP)
	
	#ifdef __I51__
		VAPP_BOOTUP_INIT_ADP(i51Boot)
	#endif

#elif defined(MMI_BOOTUP_INIT_ADP)
	
	#ifdef __I51__
		MMI_BOOTUP_INIT_ADP(i51Boot)
	#endif

#endif

