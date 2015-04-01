#include "i51.h"

#ifdef __I51_CPU_ARM__
//__I51_LITTLE_ENDIAN__
//__I51_BIG_ENDIAN__
#ifdef __I51_BIG_ENDIAN__
#include "i51Sapi51ExplorerArmBE.c"
//#include "i51Sapi51InstallerArmBE.c"
//#include "i51SapBootMenuBE.c"
#include "i51SapAppBootArmBE.c"
#ifdef __I51_SIMPLE_INSTALL__
#include "i51SapAppMenuArmBE.c"
#else
#include "i51SapBootMenuArmBE.c"
#endif
#else
#include "i51Sapi51ExplorerArmLE.c"
//#include "i51Sapi51InstallerArmLE.c"
#include "i51SapAppBootArmLE.c"
#ifdef __I51_SIMPLE_INSTALL__
#include "i51SapAppMenuArmLE.c"
#else
#include "i51SapBootMenuArmLE.c"
#endif
#endif
#endif

#ifdef __I51_CPU_MIPS__
#ifdef __I51_BIG_ENDIAN__
//#include "i51Sapi51ExplorerBE.c"
//#include "i51Sapi51InstallerBE.c"
//#include "i51SapBootMenuBE.c"
#include "i51SapAppBootBE.c"
#ifdef __I51_SIMPLE_INSTALL__
#include "i51SapAppMenuBE.c"
#else
#include "i51SapBootMenuBE.c"
#endif
#else
#include "i51Sapi51ExplorerMipsLE.c"
//#include "i51Sapi51InstallerMipsLE.c"
#include "i51SapAppBootMipsLE.c"
#ifdef __I51_SIMPLE_INSTALL__
#include "i51SapAppMenuMipsLE.c"
#else
#include "i51SapBootMenuLE.c"
#endif
#endif
#endif


#ifndef __I51_SIMPLE_INSTALL__


#ifdef __I51_Appstore__
#include "i51AppstoreRes.c"
#endif


#ifdef __I51_Convoy__
#include "i51ConvoyRes.c"
#endif
#ifdef __I51_CutFruit__
#include "i51CutFruitRes.c"
#endif
#ifdef __I51_DDZ__
#include "i51DDZRes.c"
#endif
#ifdef __I51_DontRun__
#include "i51DontRunRes.c"
#endif
#ifdef __I51_fighter__
#include "i51fighterRes.c"
#endif
#ifdef __I51_FishingMaster__
#include "i51FishingMasterRes.c"
#endif
#ifdef __I51_FruitShot__
#include "i51FruitShotRes.c"
#endif
#ifdef __I51_HundredStorey__
#include "i51HundredStoreyRes.c"
#endif
#ifdef __I51_JigsawPuzzle__
#include "i51JigsawPuzzleRes.c"
#endif
#ifdef __I51_Mjump__
#include "i51MjumpRes.c"
#endif
#ifdef __I51_Portal__
#include "i51PortalRes.c"
#endif
#ifdef __I51_PortalKE__
#include "i51PortalKERes.c"
#endif
#ifdef __I51_RaceCar__
#include "i51RaceCarRes.c"
#endif
#ifdef __I51_SavageAdventure__
#include "i51SavageAdventureRes.c"
#endif
#ifdef __I51_VexedJewels__
#include "i51VexedJewelsRes.c"
#endif
#ifdef __I51_X_Space__
#include "i51X_SpaceRes.c"
#endif
#ifdef __I51_Snidget__
#include "i51SnidgetRes.c"
#endif
#ifdef __I51_AngryBird__
#include "i51AngryBirdRes.c"
#endif
#ifdef __I51_GlassDestroyer__
#include "i51GlassDestroyerRes.c"
#endif
#ifdef __I51_PullSkirt__
#include "i51PullSkirtRes.c"
#endif
#ifdef __I51_TOM__
#include "i51TOMRes.c"
#endif
#ifdef __I51_NinJump__
#include "i51NinJumpRes.c"
#endif
#ifdef __I51_PlantsVsZombies__
#include "i51PlantsVsZombiesRes.c"
#endif
#ifdef __I51_RipClothes__
#include "i51RipClothesRes.c"
#endif
#ifdef	__I51_VXin__
#include "i51VXinRes.c"
#endif
#ifdef	__I51_ZapBird__
#include "i51ZapBirdRes.c"
#endif

#endif

