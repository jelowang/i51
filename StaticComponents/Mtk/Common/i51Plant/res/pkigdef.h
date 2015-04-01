#include "PixtelDataTypes.h"
typedef enum 
{//String Ids
	#ifdef  __PKIG__		//51PK游戏
		MENU_ID_PKIG_TEXT_1 = LL_BASE + 1,
		MENU_ID_PKIG_TEXT_2,
		MENU_ID_PKIG_TEXT_3,
		MENU_ID_PKIG_TEXT_4,
		MENU_ID_PKIG_TEXT_5,
		MENU_ID_PKIG_TEXT_6,
		MENU_ID_PKIG_JAVA_TEXT_1,
		MENU_ID_PKIG_JAVA_TEXT_2,
		MENU_ID_PKIG_JAVA_TEXT_3,
		MENU_ID_PKIG_JAVA_TEXT_4,
		MENU_ID_PKIG_JAVA_TEXT_5,
		MENU_ID_PKIG_JAVA_TEXT_6,
	#endif
	
	#ifdef	__PKIG_TQ__		// 天气预报
		STR_ID_PK_TQ,
	#endif
	
	#ifdef	__PKIG_SA__		// 野人逃生记
		STR_ID_PK_SA,
	#endif
	
	#ifdef	__PKIG_BM__		// 宝石迷情
		STR_ID_PK_BM,
	#endif
	
	#ifdef	__PKIG_DM__		// 金字塔历险记
		STR_ID_PK_DM,
	#endif
	
	#ifdef	__PKIG_ST__		// 街头速递
		STR_ID_PK_ST,
	#endif
	
	#ifdef	__PKIG_SL__		// 营救小LOLI
		STR_ID_PK_SL,
	#endif
	
	#ifdef	__PKIG_MJ__		// 跳跳堂
		STR_ID_PK_MJ,
	#endif
	
	#ifdef	__PKIG_SPACE__	// 完美真相
		STR_ID_PK_SPACE,
	#endif
	
	#ifdef	__PKIG_DR__		// 虫虫别跑
		STR_ID_PK_DR,
	#endif
	
	#ifdef	__PKIG_DS__		// 豆豆广场
		STR_ID_PK_DS,
	#endif
	
	#ifdef	__PKIG_CY__		// 保卫钓鱼岛
		STR_ID_PK_CY,
	#endif
	
	#ifdef	__PKIG_HS__		// 丛林飞将
		STR_ID_PK_HS,
	#endif
	
	#ifdef	__PKIG_FT__		// 玫瑰机动队
		STR_ID_PK_FT,
	#endif
	
	#ifdef	__PKIG_RC__		// 炮弹飞车
		STR_ID_PK_RC,
	#endif
	
	#ifdef	__PKIG_VJ__		// 重力宝石
		STR_ID_PK_VJ,
	#endif
	
	#ifdef	__PKIG_CF__		// 水果忍者
		STR_ID_PK_CF,
	#endif
	
	#ifdef	__PKIG_JP__		// 魅力拼图
		STR_ID_PK_JP,
	#endif
	
	#ifdef	__PKIG_NIN__	// 武林飞侠
		STR_ID_PK_NIN,
	#endif
	
	#ifdef	__PKIG_IP__		// 成语迷局
		STR_ID_PK_IP,
	#endif
	
	#ifdef	__PKIG_DDZ__	// 斗地主
		STR_ID_PK_DDZ,
	#endif
	
	#ifdef	__PKIG_FS__		// 水果快点
		STR_ID_PK_FS,
	#endif
	
	#ifdef	__PKIG_CFM__	// 捕鱼达人
		STR_ID_PK_CFM,
	#endif
	
	STR_PKIG_END
}STR_PKIG_MENU_LIST;
   
typedef enum 
{//Image Ids
	#ifdef  __PKIG__		//51PK游戏
		IMG_PKIG_MAIN_ICON = LL_BASE + 1,
	#endif
	
	#ifdef	__PKIG_TQ__		// 天气预报
		IMG_ID_PK_TQ,
	#endif
	
	#ifdef	__PKIG_SA__		// 野人逃生记
		IMG_ID_PK_SA,
	#endif
	
	#ifdef	__PKIG_BM__		// 宝石迷情
		IMG_ID_PK_BM,
	#endif
	
	#ifdef	__PKIG_DM__		// 金字塔历险记
		IMG_ID_PK_DM,
	#endif
	
	#ifdef	__PKIG_ST__		// 街头速递
		IMG_ID_PK_ST,
	#endif
	
	#ifdef	__PKIG_SL__		// 营救小LOLI
		IMG_ID_PK_SL,
	#endif
	
	#ifdef	__PKIG_MJ__		// 跳跳堂
		IMG_ID_PK_MJ,
	#endif
	
	#ifdef	__PKIG_SPACE__	// 完美真相
		IMG_ID_PK_SPACE,
	#endif
	
	#ifdef	__PKIG_DR__		// 虫虫别跑
		IMG_ID_PK_DR,
	#endif
	
	#ifdef	__PKIG_DS__		// 豆豆广场
		IMG_ID_PK_DS,
	#endif
	
	#ifdef	__PKIG_CY__		// 保卫钓鱼岛
		IMG_ID_PK_CY,
	#endif
	
	#ifdef	__PKIG_HS__		// 丛林飞将
		IMG_ID_PK_HS,
	#endif
	
	#ifdef	__PKIG_FT__		// 玫瑰机动队
		IMG_ID_PK_FT,
	#endif
	
	#ifdef	__PKIG_RC__		// 炮弹飞车
		IMG_ID_PK_RC,
	#endif
	
	#ifdef	__PKIG_VJ__		// 重力宝石
		IMG_ID_PK_VJ,
	#endif
	
	#ifdef	__PKIG_CF__		// 水果忍者
		IMG_ID_PK_CF,
	#endif
	
	#ifdef	__PKIG_JP__		// 魅力拼图
		IMG_ID_PK_JP,
	#endif
	
	#ifdef	__PKIG_NIN__	// 武林飞侠
		IMG_ID_PK_NIN,
	#endif
	
	#ifdef	__PKIG_IP__		// 成语迷局
		IMG_ID_PK_IP,
	#endif
	
	#ifdef	__PKIG_DDZ__	// 斗地主
		IMG_ID_PK_DDZ,
	#endif
	
	#ifdef	__PKIG_FS__		// 水果快点
		IMG_ID_PK_FS,
	#endif
	
	#ifdef	__PKIG_CFM__	// 捕鱼达人
		IMG_ID_PK_CFM,
	#endif
	
	IMG_PKIG_END
}IMG_PKIG_MENU_LIST;

