
/*

The Implementing of 'i51Port' , Module : 'PORT'.	
Layer belong : 'Static Components'.
For more details about 'i51Port' you could read : "The_i51_SC_Port_Interface.xls"
For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"

The 51PK Platform was implementing based on 'The i51 Architecture'.
'The i51 Architecture' is designed by Jelo.

Copyright (C) 2011 , PKIG Tech. Co., Ltd.

*/
#include "i51.h"
#include "i51AdeOs.h"
#include "i51AdeMmi.h"
#include "i51AdeSms.h"
#include "i51AdeSoc.h"
#include "i51AdeStd.h"
#include "i51Kernel.h"
#include "i51Port.h"
#include "i51Script.h"

extern int gi51Game_id;
extern  iBOOL i51PortRunHintScreen (int type) ;
iS32 i51PortRunSapGameExCheck(iU16 *name,void *parameter)
{

    if(i51KernelCheckAppCompleten(name))
    {
        i51KernelRunDynamicAppEx(name,parameter);
        return 1;
    }
    else
    {
        i51PortRunHintScreen(1);
        return 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef __I51_APP_LANG_EN__
                 //portal 
extern void* i51SapPortalParameter();
                //切西瓜
extern void* i51SapCutFruitParameter();
extern void* i51SapfighterParameter();
extern void* i51SapSnidgetParameter();
extern void* i51SapConvoyParameter();
extern void* i51SapFruitShotParameter();

extern void* i51SapDDZParameter();
extern void* i51SapDontRunParameter();
extern void* i51SapFishingMasterParameter();
extern void* i51SapHundredStoreyParameter();
extern void* i51SapJigsawPuzzleParameter();

extern void* i51SapMjumpParameter();
extern void* i51SapRaceCarParameter();
extern void* i51SapSavageAdventureParameter();
extern void* i51SapVexedJewelsParameter();
extern void* i51SapX_SpaceParameter();
extern void* i51SapAppstoreParameter();

#ifdef  __I51_TINY__
extern iU32 i51KitStaticCoreAdapter[] ;		//akilae add 2012.7.20
extern const int i51SapGetAppmenuSize(void);
void i51SimpleLunchApp(iU16 *pAppName)
{
    typedef struct
	{
		unsigned short *appname;
		int logosize;
		int dispsize;
		char logotype;//0 文字 1图片
		char gprsprompt;
		void *dispname;//游戏名称字模
		void *logo;//游戏说明 图片buffer或文字字模
	}BooterParam;
	static BooterParam param = {0};
	i51KITMAIN i51KitMain = 0;
	
	param.appname = pAppName;
#ifdef __I51_APP_GPRS_PROMPT__
    param.gprsprompt = 1;
#else
    param.gprsprompt = 0;
#endif
    if( i51KernelCheckAppCompleten(pAppName) ){
        i51KernelRunDynamicApp( pAppName, NULL );
    	}
    else{
		i51KernelRunStaticApp ( L"Appmenu" , (unsigned char* )i51SapGetAppmenu () , (unsigned int )i51SapGetAppmenuSize () , &param, 0 , 1 ) ;
#if 1
		//akilae add since 2012.7.20	
		i51KitMain = (i51KITMAIN ) i51SlLoaderGetSymbolEx ( L"Appmenu" , "i51KitMain" ) ;
		if(NULL == i51KitMain){
		i51AdeOsLog(0, "KEL : Get i51KitMain F");
		return;
		}
		i51KitMain( 0, 0x00FFFFFF , (void*)i51KitStaticCoreAdapter );
#endif		
    	}
//akilae add end	
}

void Tinyi51PortRunSapGame(void)
{

    switch(gi51Game_id)
    {
#ifdef __I51_Portal__  
        case 0:         //51PK游戏中心
        i51SimpleLunchApp(L"Portal");
            break;
#endif
#ifdef __I51_CutFruit__  
        case 1:
           //切西瓜
        i51SimpleLunchApp(L"CutFruit");
            break;
#endif
#ifdef __I51_fighter__  
        case 2:
            //玫瑰机动队          
        i51SimpleLunchApp(L"fighter");
            break;
#endif
#ifdef __I51_Snidget__  
        case 3:
            //武林飞侠
        i51SimpleLunchApp(L"Snidget");
            break;
#endif
#ifdef __I51_Convoy__  
        case 4:
            //保卫钓鱼岛        
        i51SimpleLunchApp(L"Convoy");
            break;
#endif
#ifdef __I51_FruitShot__  
        case 5:
            //水果快点
        i51SimpleLunchApp(L"FruitShot");
            break;
#endif
#ifdef __I51_DDZ__  
        case 6:
            //斗地主
        i51SimpleLunchApp(L"DDZ");
            break;
#endif
#ifdef __I51_DontRun__  
        case 7:
            //虫虫别跑
        i51SimpleLunchApp(L"DontRun");
            break;
#endif
#ifdef __I51_FishingMaster__  
        case 8:
            //捕鱼达人
        i51SimpleLunchApp(L"FishingMaster");
            break;
#endif
#ifdef __I51_HundredStorey__  
        case 9:
            //丛林飞将
        i51SimpleLunchApp(L"HundredStorey");
            break;
#endif
#ifdef __I51_JigsawPuzzle__  
        case 10:
            //魅力拼图
        i51SimpleLunchApp(L"JigsawPuzzle");
            break;
#endif
#ifdef __I51_Mjump__  
        case 11:
            //跳跳堂
        i51SimpleLunchApp(L"Mjump");
            break;
#endif
#ifdef __I51_RaceCar__  
        case 12:
            //炮弹飞车
        i51SimpleLunchApp(L"RaceCar");
            break;
#endif
#ifdef __I51_SavageAdventure__  
        case 13:
            //野人逃生记
        i51SimpleLunchApp(L"SavageAdventure");
            break;
#endif
#ifdef __I51_VexedJewels__  
        case 14:
            //重力宝石
        i51SimpleLunchApp(L"VexedJewels");
            break;
#endif
#ifdef __I51_X_Space__  
        case 15:
            //完美真相
        i51SimpleLunchApp(L"X_Space");
            break;
#endif
        case 199: //工程模式
            i51KernelRunDynamicAppEx ( L"i51Debuger" , 0 ) ;
            break;
        default:                    //浏览器  
	    i51PortRunStaticApp ( L"i51Explorer" , (unsigned char*)i51SapGeti51Explorer(), (unsigned int)i51SapGeti51ExplorerSize() , 0 ,0 ,1) ;		
            break;
   
    }

}
#else
void i51PortRunSapGame(void)
{

    switch(gi51Game_id)
    {
#ifdef __I51_Portal__  
        case 0:         //51PK游戏中心
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapPortalParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_CutFruit__  
        case 1:
           //切西瓜
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapCutFruitParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_fighter__  
        case 2:
            //玫瑰机动队          
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapfighterParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_Snidget__  
        case 3:
            //武林飞侠
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapSnidgetParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_Convoy__  
        case 4:
            //保卫钓鱼岛        
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapConvoyParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_FruitShot__  
        case 5:
            //水果快点
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapFruitShotParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_DDZ__  
        case 6:
            //斗地主
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapDDZParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_DontRun__  
        case 7:
            //虫虫别跑
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapDontRunParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_FishingMaster__  
        case 8:
            //捕鱼达人
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapFishingMasterParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_HundredStorey__  
        case 9:
            //丛林飞将
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapHundredStoreyParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_JigsawPuzzle__  
        case 10:
            //魅力拼图
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapJigsawPuzzleParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_Mjump__  
        case 11:
            //跳跳堂
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapMjumpParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_RaceCar__  
        case 12:
            //炮弹飞车
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapRaceCarParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_SavageAdventure__  
        case 13:
            //野人逃生记
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapSavageAdventureParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_VexedJewels__  
        case 14:
            //重力宝石
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapVexedJewelsParameter() ,0 ,1) ;
            break;
#endif
#ifdef __I51_X_Space__  
        case 15:
            //完美真相
            i51PortRunStaticApp ( L"BootMenu" , (unsigned char*)i51SapGetBootMenu(), (unsigned int)i51SapGetBootMenuSize() , i51SapX_SpaceParameter() ,0 ,1) ;
            break;
#endif
        case 199: //工程模式
            i51KernelRunDynamicAppEx ( L"i51Debuger" , 0 ) ;
            break;
        default:                    //浏览器     
	    i51PortRunStaticApp ( L"i51Explorer" , (unsigned char*)i51SapGeti51Explorer(), (unsigned int)i51SapGeti51ExplorerSize() , 0 ,0 ,1) ;		
            break;
   
    }

}
#endif /*#ifdef  __I51_TINY__*/

#endif/*ifndef __I51_APP_LANG_EN__*/

void i51PortRunSapGameEx(void)
{

    switch(gi51Game_id)
    {
#ifdef __I51_Portal__  
        case 0:         //51PK游戏中心
            i51PortRunSapGameExCheck (L"Portal",0) ;
            break;
#endif
#ifdef __I51_CutFruit__  
        case 1:
           //切西瓜
            i51PortRunSapGameExCheck (L"CutFruit",0) ;
            break;
#endif
#ifdef __I51_fighter__  
        case 2:
            //玫瑰机动队          
            i51PortRunSapGameExCheck (L"fighter",0) ;
            break;
#endif
#ifdef __I51_Snidget__  
        case 3:
            //武林飞侠
            i51PortRunSapGameExCheck (L"Snidget",0) ;
            break;
#endif
#ifdef __I51_Convoy__  
        case 4:
            //保卫钓鱼岛        
            i51PortRunSapGameExCheck (L"Convoy",0) ;
            break;
#endif
#ifdef __I51_FruitShot__  
        case 5:
            //水果快点
            i51PortRunSapGameExCheck (L"FruitShot",0) ;
            break;
#endif
#ifdef __I51_DDZ__  
        case 6:
            //斗地主
            i51PortRunSapGameExCheck (L"DDZ",0) ;
            break;
#endif
#ifdef __I51_DontRun__  
        case 7:
            //虫虫别跑
            i51PortRunSapGameExCheck (L"DontRun",0) ;
            break;
#endif
#ifdef __I51_FishingMaster__  
        case 8:
            //捕鱼达人
            i51PortRunSapGameExCheck (L"FishingMaster",0) ;
            break;
#endif
#ifdef __I51_HundredStorey__  
        case 9:
            //丛林飞将
            i51PortRunSapGameExCheck (L"HundredStorey",0) ;
            break;
#endif
#ifdef __I51_JigsawPuzzle__  
        case 10:
            //魅力拼图
            i51PortRunSapGameExCheck (L"JigsawPuzzle",0) ;
            break;
#endif
#ifdef __I51_Mjump__  
        case 11:
            //跳跳堂
            i51PortRunSapGameExCheck (L"Mjump",0) ;
            break;
#endif
#ifdef __I51_RaceCar__  
        case 12:
            //炮弹飞车
            i51PortRunSapGameExCheck (L"RaceCar",0) ;
            break;
#endif
#ifdef __I51_SavageAdventure__  
        case 13:
            //野人逃生记
            i51PortRunSapGameExCheck (L"SavageAdventure",0) ;
            break;
#endif
#ifdef __I51_VexedJewels__  
        case 14:
            //重力宝石
            i51PortRunSapGameExCheck (L"VexedJewels",0) ;
            break;
#endif
#ifdef __I51_X_Space__  
        case 15:
            //完美真相
            i51PortRunSapGameExCheck (L"X_Space",0) ;
            break;
#endif

        case 199://工程模式
            i51KernelRunDynamicAppEx ( L"i51Debuger" , 0 ) ;
            break;
        default:                    //浏览器        
	    i51PortRunStaticApp ( L"i51Explorer" , (unsigned char*)i51SapGeti51Explorer(), (unsigned int)i51SapGeti51ExplorerSize() , 0 ,0 ,1) ;		
            break;

    }

}
void i51PortRunDynamicAPP(iU16 *name)
{
    i51PortRunSapGameExCheck (name,0) ;
}

