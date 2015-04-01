#include "PixtelDataTypes.h"
typedef enum 
{//String Ids
	#ifdef  __I51__
	MENU_ID_I51_TEXT_INNVRAM = APP_I51_BASE + 1,
	#endif
	
	#ifdef  __I51_Portal__		//51PK游戏
		MENU_ID_I51_TEXT_1,
		MENU_ID_I51_TEXT_2,
		MENU_ID_I51_TEXT_3,
		MENU_ID_I51_TEXT_4,
		MENU_ID_I51_TEXT_5,
		MENU_ID_I51_TEXT_6,
		MENU_ID_I51_JAVA_TEXT_1,
		MENU_ID_I51_JAVA_TEXT_2,
		MENU_ID_I51_JAVA_TEXT_3,
		MENU_ID_I51_JAVA_TEXT_4,
		MENU_ID_I51_JAVA_TEXT_5,
		MENU_ID_I51_JAVA_TEXT_6,
	#endif
	
	#ifdef	__I51_TQ__		// 天气预报
		STR_ID_I51_TQ,
	#endif
	
	#ifdef	__I51_SavageAdventure__		// 野人逃生记
		STR_ID_I51_SA,
	#endif
	
	#ifdef	__I51_BM__		// 宝石迷情
		STR_ID_I51_BM,
	#endif
	
	#ifdef	__I51_DM__		// 金字塔历险记
		STR_ID_I51_DM,
	#endif
	
	#ifdef	__I51_ST__		// 街头速递
		STR_ID_I51_ST,
	#endif
	
	#ifdef	__I51_Loli__		// 营救小LOLI
		STR_ID_I51_SL,
	#endif
	
	#ifdef	__I51_Mjump__		// 跳跳堂
		STR_ID_I51_MJ,
	#endif
	
	#ifdef	__I51_X_Space__	// 完美真相
		STR_ID_I51_SPACE,
	#endif
	
	#ifdef	__I51_DontRun__		// 虫虫别跑
		STR_ID_I51_DR,
	#endif
	
	#ifdef	__I51_DS__		// 豆豆广场
		STR_ID_I51_DS,
	#endif
	
	#ifdef	__I51_Convoy__		// 保卫钓鱼岛
		STR_ID_I51_CY,
	#endif
	
	#ifdef	__I51_HundredStorey__		// 丛林飞将
		STR_ID_I51_HS,
	#endif
	
	#ifdef	__I51_fighter__		// 玫瑰机动队
		STR_ID_I51_FT,
	#endif
	
	#ifdef	__I51_RaceCar__		// 炮弹飞车
		STR_ID_I51_RC,
	#endif
	
	#ifdef	__I51_VexedJewels__		// 重力宝石
		STR_ID_I51_VJ,
	#endif
	
	#ifdef	__I51_CutFruit__		// 水果忍者
		STR_ID_I51_CF,
	#endif
	
	#ifdef	__I51_JigsawPuzzle__		// 魅力拼图
		STR_ID_I51_JP,
	#endif
	
	#ifdef	__I51_Snidget__	// 武林飞侠
		STR_ID_I51_NIN,
	#endif
	
	#ifdef	__I51_IP__		// 成语迷局
		STR_ID_I51_IP,
	#endif
	
	#ifdef	__I51_DDZ__	// 斗地主
		STR_ID_I51_DDZ,
	#endif
	
	#ifdef	__I51_FruitShot__		// 水果快点
		STR_ID_I51_FS,
	#endif
	
	#ifdef	__I51_FishingMaster__	// 捕鱼达人
		STR_ID_I51_CFM,
	#endif

	#ifdef	__I51_AngryBird__ 	// 愤怒的小鸟
		STR_ID_I51_ANGRYBIRD,
	#endif
	
	#ifdef	__I51_GlassDestroyer__	// 碎玻璃
		STR_ID_I51_GD,
	#endif
	
	#ifdef	__I51_PullSkirt__	// 吹裙子
		STR_ID_I51_PS,
	#endif

	#ifdef	__I51_TOM__	// TOM
		STR_ID_I51_TOM,
	#endif

	#ifdef	__I51_Ninjump__		// 忍者跳跃
		STR_ID_I51_NINJUMP,
	#endif

	#ifdef	__I51_ZapBird__		// 闪电鸟
		STR_ID_I51_ZAPBIRD,
	#endif

	#ifdef	__I51_PlantsVsZombies__		// 植物大战僵尸
		STR_ID_I51_PLANTSVSZOMBIES,
	#endif

    #ifdef	__I51_VXin__		//V信
		STR_ID_I51_VXin,
    #endif

    #ifdef	__I51_RipClothes__		//撕衣服
		STR_ID_I51_RipClothes,
    #endif
    
    #ifdef	__I51_AppStore__		 //应用商店
		STR_ID_I51_AppStore,
    #endif
    
    #ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
        STR_ID_I51_wheresMywater,
    #endif
    
    #ifdef	__I51_MusicJam__        //劲乐达人
        STR_ID_I51_MusicJam,
    #endif
    
    #ifdef	__I51_AuPlanet__        //啊呜星球
        STR_ID_I51_AuPlanet,
    #endif
    
	STR_I51_END
}STR_I51_MENU_LIST;
   
typedef enum 
{//Image Ids
	#ifdef  __I51__
		IMG_I51_MAIN__INNVRAM = APP_I51_BASE + 1,
	#endif
		
	#ifdef  __I51_Portal__		//51PK游戏
		IMG_I51_Portal_ICON,
	#endif
	
	#ifdef	__I51_TQ__		// 天气预报
		IMG_ID_I51_TQ,
	#endif
	
	#ifdef	__I51_SavageAdventure__		// 野人逃生记
		IMG_ID_I51_SA,
	#endif
	
	#ifdef	__I51_BM__		// 宝石迷情
		IMG_ID_I51_BM,
	#endif
	
	#ifdef	__I51_DM__		// 金字塔历险记
		IMG_ID_I51_DM,
	#endif
	
	#ifdef	__I51_ST__		// 街头速递
		IMG_ID_I51_ST,
	#endif
	
	#ifdef	__I51_Loli__		// 营救小LOLI
		IMG_ID_I51_SL,
	#endif
	
	#ifdef	__I51_Mjump__		// 跳跳堂
		IMG_ID_I51_MJ,
	#endif
	
	#ifdef	__I51_X_Space__	// 完美真相
		IMG_ID_I51_SPACE,
	#endif
	
	#ifdef	__I51_DontRun__		// 虫虫别跑
		IMG_ID_I51_DR,
	#endif
	
	#ifdef	__I51_DS__		// 豆豆广场
		IMG_ID_I51_DS,
	#endif
	
	#ifdef	__I51_Convoy__		// 保卫钓鱼岛
		IMG_ID_I51_CY,
	#endif
	
	#ifdef	__I51_HundredStorey__		// 丛林飞将
		IMG_ID_I51_HS,
	#endif
	
	#ifdef	__I51_fighter__		// 玫瑰机动队
		IMG_ID_I51_FT,
	#endif
	
	#ifdef	__I51_RaceCar__		// 炮弹飞车
		IMG_ID_I51_RC,
	#endif
	
	#ifdef	__I51_VexedJewels__		// 重力宝石
		IMG_ID_I51_VJ,
	#endif
	
	#ifdef	__I51_CutFruit__		// 水果忍者
		IMG_ID_I51_CF,
	#endif
	
	#ifdef	__I51_JigsawPuzzle__		// 魅力拼图
		IMG_ID_I51_JP,
	#endif
	
	#ifdef	__I51_Snidget__	// 武林飞侠
		IMG_ID_I51_NIN,
	#endif
	
	#ifdef	__I51_IP__		// 成语迷局
		IMG_ID_I51_IP,
	#endif
	
	#ifdef	__I51_DDZ__	// 斗地主
		IMG_ID_I51_DDZ,
	#endif
	
	#ifdef	__I51_FruitShot__		// 水果快点
		IMG_ID_I51_FS,
	#endif
	
	#ifdef	__I51_FishingMaster__	// 捕鱼达人
		IMG_ID_I51_CFM,
	#endif

	#ifdef	__I51_AngryBird__ 	// 愤怒的小鸟
		IMG_ID_I51_ANGRYBIRD,
	#endif
	
	#ifdef	__I51_GlassDestroyer__	// 碎玻璃
		IMG_ID_I51_GD,
	#endif
	
	#ifdef	__I51_PullSkirt__	// 吹裙子
		IMG_ID_I51_PS,
	#endif

	#ifdef	__I51_TOM__	// TOM
		IMG_ID_I51_TOM,
	#endif

	#ifdef	__I51_Ninjump__		// 忍者跳跃
		IMG_ID_I51_NINJUMP,
	#endif

	#ifdef	__I51_ZapBird__		// 闪电鸟
		IMG_ID_I51_ZAPBIRD,
	#endif

	#ifdef	__I51_PlantsVsZombies__		// 植物大战僵尸
		IMG_ID_I51_PLANTSVSZOMBIES,
	#endif
    
    #ifdef	__I51_VXin__		//V信
		IMG_ID_I51_VXin,
    #endif

    #ifdef	__I51_RipClothes__		//撕衣服
		IMG_ID_I51_RipClothes,
    #endif
    
    #ifdef	__I51_AppStore__		 //应用商店
		IMG_ID_I51_AppStore,
    #endif

    #ifdef	__I51_wheresMywater__		 //鳄鱼洗澡
        IMG_ID_I51_wheresMywater,
    #endif
    
    #ifdef	__I51_MusicJam__        //劲乐达人
        IMG_ID_I51_MusicJam,
    #endif
    
    #ifdef	__I51_AuPlanet__        //啊呜星球
        IMG_ID_I51_AuPlanet,
    #endif
    
	IMG_I51_END
}IMG_I51_MENU_LIST;

