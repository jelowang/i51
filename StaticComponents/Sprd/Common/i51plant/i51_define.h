
#ifndef __I51_DEFINE_H__
#define __I51_DEFINE_H__

#ifndef __I51_TINY__
//	如果是瘦身版将此处打开
//#define __I51_TINY__
#endif



//根据平台需求开宏
//#define SPRD_6800
//#define SPRD_6600
//#define SPRD_6620
#define SPRD_6530

//	如果MMIAPICC_MakeCall函数有6个参数，我们将I51_MAKECALL_PARA_6 的宏打开，
//以此类推，7个参数开 I51_MAKECALL_PARA_7）


//#define I51_MAKECALL_PARA_6 
//#define I51_MAKECALL_PARA_7
#define I51_MAKECALL_PARA_8

//应用浏览器
#define __I51_Explorer__

//i51游戏中心
#define __I51_Portal__

//水果忍着
//#define __I51_CutFruit__

//玫瑰机动队
//#define __I51_fighter__		
//保卫钓鱼岛
//#define __I51_Convoy__

//武林飞侠
//#define __I51_Snidget__

//水果快点
//#define __I51_FruitShot__

//野人逃生记
//#define __I51_SavageAdventure__

//重力宝石
//#define __I51_VexedJewels__

//完美真相
//#define __I51_X_Space__

//虫虫别跑
//#define __I51_DontRun__

//跳跳堂
//#define __I51_Mjump__

//斗地主
//#define __I51_DDZ__

//丛林飞将
//#define __I51_HundredStorey__

//炮弹飞车
//#define __I51_RaceCar__

//捕鱼达人
//#define __I51_FishingMaster__

//魅力拼图
//#define __I51_JigsawPuzzle__

//愤怒的小鸟
//#define __I51_AngryBird__

 //碎屏
//#define __I51_GlassDestroyer__

//忍者跳跃
//#define __I51_NinJump__

//植物大战僵尸
//#define __I51_PlantsVsZombies__

//微信
//#define __I51_VXin__

//撕衣服
//#define __I51_RipClothes__



//至此以上的宏需要根据实际的情况打开，下面的宏移植人员不需要管。 


#if defined (MAINLCD_DEV_SIZE_128X128)
#define __I51_APP_128X128__
#elif  defined (MAINLCD_DEV_SIZE_176X220)
#define __I51_APP_176X220__
#elif  defined (MAINLCD_DEV_SIZE_220X176)
#define __I51_APP_220X176__
#elif  defined (MAINLCD_DEV_SIZE_240X320)
#define __I51_APP_240X320__
#elif  defined (MAINLCD_DEV_SIZE_320X240)
#define __I51_APP_320X240__
#elif  defined (MAINLCD_DEV_SIZE_240X400)
#define __I51_APP_240X400__
#elif  defined (MAINLCD_DEV_SIZE_320X400)
#define __I51_APP_320X400__
#elif  defined (MAINLCD_DEV_SIZE_272X480)
#define __I51_APP_272X480__
#elif  defined (MAINLCD_DEV_SIZE_320X480)
#define __I51_APP_320X480__
#elif  defined (MAINLCD_DEV_SIZE_480X320)
#define __I51_APP_480X320__
#endif

#if defined (__I51_TINY__)
//	目前为止瘦身版都是用精简版的界面
#define __I51_SIMPLE_INSTALL__
#endif

#ifndef SPRD_6530
#define __I51_BIG_ENDIAN__
#endif


#define __I51_CPU_ARM__



#endif

