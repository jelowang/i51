 /*********************************************************************************/
/**
 * Copyright (C) 2010, PKIG Tech. Co., Ltd.
 *
 * @file	Pk_Rank.h
 * @brief	declare
 * 
 * @author	terrcy
 * @version	$Id: Pk_Rank.h 293 2010-12-24 15:00:56Z terrcy $
 * @date	$Date:: 2010-12-24 15:00:56#$
 *********************************************************************************/
#ifndef _PK_RANK_H_
#define _PK_RANK_H_

#include "i51.h"
#include "i51KitAde.h"
//#include "i51KitG2.h"
//#include "i51KitStd.h"

#define APP_NAME  "SimpleWMW"    //在此修改你的应用名字，存档保存在 \WIN32FS\DRIVE_E\i51\i51APP\APP_NAME\0_0
#define GAME_MAX_USERNAME_LEN 12 
#define MAX_RECORD_NUM	10

#define RevivedID "TTT_001" 

typedef struct gameInfo{
	int 	game_id;
	int 	game_mode;
	int 	game_ver;
	int		user_id;
	char	user_name[GAME_MAX_USERNAME_LEN];
	char    RunMode;
}UAPP_GAME_INFO;
typedef struct tagRankData
{
	int		mUserId;
	char	mName[GAME_MAX_USERNAME_LEN];	//玩家名字
	int		mScore;                         //分数
}RANK_DATA_TAG;
typedef struct local_data{
	int	 mGameId;
	int  mVersion;  
	int  mChp;      //最好成绩或关卡
	int  mUserId;
	char mName[GAME_MAX_USERNAME_LEN];	
	RANK_DATA_TAG mRankData[MAX_RECORD_NUM];   //排名是下标,0-9代表1-10名
	char crc[28];
}LOCAL_DATA_TAG;
  
extern void init_local_data(const UAPP_GAME_INFO *pGameInfo, LOCAL_DATA_TAG *pLocalData);   //初始化排行榜,并把数据保存在 pLocalData 里
extern void set_local_score_data(int score,LOCAL_DATA_TAG *pLocalData);                     //更新排行榜，保存成绩             
extern void update_local_chp_data(int chp,LOCAL_DATA_TAG *pLocalData);                      //这里切水果用来保存了最好成绩，若有关卡的，可用此变量保存关卡
extern int	get_local_chp_data(const LOCAL_DATA_TAG *pLocalData);                           //获取最好成绩或关卡
#endif