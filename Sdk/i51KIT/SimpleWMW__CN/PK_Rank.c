#ifndef _PK_RANK_C_
#define _PK_RANK_C_
#include "PK_Rank.h"

//unsigned int local_get_best_score(LOCAL_DATA_TAG *pLocalData)
//{
//	unsigned short loop;
//	for (loop = 0; loop < 10; loop++)
//	{
//		if (pLocalData->mUserId == pLocalData->mRankData[loop].mUserId)
//		{
//			return pLocalData->mRankData[loop].mScore;
//		}
//	}
//	return 0;
//}

void reset_local_data(const UAPP_GAME_INFO *pGameInfo, LOCAL_DATA_TAG *pLocalData)
{	
#ifdef LANGUAGE_CHINESE
	RANK_DATA_TAG temp[10] = {
		{0,"夜袭991",10630},{0,"夜袭992",9130},{0,"花思容",8570},{0,"康少爷",7110},{0,"西门吹雪",5190},
		{0,"简单生活",970},{0,"美美假期",710},{0,"农夫",450},{0,"小丫头",130},{0,"幸福&味道",100}
	};
#else
	RANK_DATA_TAG temp[10] = {
		{0,"ck",10630},{0,"stone",9130},{0,"shawn",8570},{0,"kevin",7110},{0,"wood",5190},
		{0,"victor",970},{0,"oscar",710},{0,"G.G",450},{0,"terrcy",130},{0,"K.K",100}
	};
#endif

	pLocalData->mGameId = pGameInfo->game_id;
	pLocalData->mVersion = 0;//reserve
	pLocalData->mChp = 0;	
	pLocalData->mUserId = pGameInfo->user_id;		
	sprintf(pLocalData->mName,"%s",pGameInfo->user_name);
	memcpy(pLocalData->mRankData,temp,sizeof(temp));
}

void update_local_user_data(LOCAL_DATA_TAG *pLocalData,const char *userName)
{
	int loop;

	for(loop = 0; loop < MAX_RECORD_NUM; loop++)
	{
		if(pLocalData->mRankData[loop].mUserId == pLocalData->mUserId)
		{
			sprintf(pLocalData->mRankData[loop].mName,"%s",userName);
		}
	}
}

void local_file_encrypt(LOCAL_DATA_TAG *pLocalData)
{
//#ifndef WIN32
//	unsigned char key[8];
//	unsigned char data[256];
//	int ret;
//
//    memset(key, 0, sizeof(key));
//	memset(data,0,sizeof(data));
//    memcpy (key, "\x01\x23\x45\x67\x89\xab\xcd\xef", 8);	
//	memcpy((void *)data,pLocalData,sizeof(LOCAL_DATA_TAG));
//	ret = ll_des_encrypt (key, data, data, sizeof(LOCAL_DATA_TAG));
//	
//	memcpy((void *)pLocalData,data,sizeof(LOCAL_DATA_TAG));
//#endif
}

void local_file_decrypt(LOCAL_DATA_TAG *pLocalData)
{
//#ifndef WIN32
//	unsigned char key[8];
//	unsigned char data[256];
//	int ret;
//
//    memset(key, 0, sizeof(key));
//	memset(data,0,sizeof(data));
//    memcpy (key, "\x01\x23\x45\x67\x89\xab\xcd\xef", 8);
//	memcpy((void *)data,pLocalData,sizeof(LOCAL_DATA_TAG));
//	
//	ret = ll_des_decrypt(key, data, data, sizeof(LOCAL_DATA_TAG));
//	memcpy((void *)pLocalData,data,sizeof(LOCAL_DATA_TAG));
//#endif
}

void find_create_local_dir(char *dirName)
{
	iU16 dir[256];
	int ret = -1;

	memset((void *)dir, 0, sizeof(dir));
	i51AdeStdWSprintf(dir, 256, "%s",dirName);

	ret = i51AdeOsFileIsDirExist(dir);
	//if(0 == ret)
	{
		ret = i51AdeOsFileCreateDir(dir);
	}
}

void write_local_file(LOCAL_DATA_TAG *pLocalRankData)
{
	int				fd = -1;
	unsigned int	Wsize = 0;
	int				ret = -1;	
	LOCAL_DATA_TAG *pLocalData = 0;
	char 			file[128];
	char            dir_file[256];
	iU16 			localFile[256];

	memset((void *)file, 0, sizeof(file));		
	memset((void *)localFile, 0, sizeof(localFile));
	pLocalData = (LOCAL_DATA_TAG *)pLocalRankData;

	sprintf(file, "%d_%d",pLocalData->mGameId,pLocalData->mUserId);	

	sprintf(dir_file, "%s\\%d_%d",APP_NAME,pLocalData->mGameId,pLocalData->mUserId);		
	find_create_local_dir(dir_file);
	i51AdeStdWSprintf(localFile, 256, "%s\\%s\\%s.dat",APP_NAME,file,file);	
	fd = i51AdeOsFileOpen((unsigned short*)localFile, i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE, __FILE__ , __LINE__);			
	if(fd < 0)
	{
		return;
	}
	else
	{
		local_file_encrypt(pLocalData);
		ret = i51AdeOsFileWrite( fd, (void* )pLocalData, sizeof(LOCAL_DATA_TAG), &Wsize);
		local_file_decrypt(pLocalData);
		i51AdeOsFileClose( fd );
	}
}

void read_local_file(const UAPP_GAME_INFO *pGameInfo,unsigned short *localFile,LOCAL_DATA_TAG *pLocalData)
{
	if(0 == i51AdeOsFileIsExist (localFile))// the file not exist
	{
		reset_local_data(pGameInfo, pLocalData);
		write_local_file(pLocalData);	
	}
	else
	{
		int fd;
		unsigned int Wsize;
		int fdLen;
		fd = i51AdeOsFileOpen(( unsigned short* )localFile, i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );		
		if(fd <= 0)
		{
			reset_local_data(pGameInfo, pLocalData);			
		}
		else
		{
			i51AdeOsFileGetSize(fd, &fdLen);
			if(sizeof(LOCAL_DATA_TAG) == fdLen)
			{
				i51AdeOsFileRead(fd, (void *)pLocalData, sizeof(LOCAL_DATA_TAG), &Wsize);
				local_file_decrypt(pLocalData);
				i51AdeOsFileClose(fd);
				update_local_user_data(pLocalData,pGameInfo->user_name);
			}
			else
			{
				i51AdeOsFileClose(fd);//close the file
				reset_local_data(pGameInfo, pLocalData);
				write_local_file(pLocalData);								
			}
		}
	}
}

void init_local_data(const UAPP_GAME_INFO *pGameInfo, LOCAL_DATA_TAG *pLocalData)
{
	char fileName[256];
	iU16 file[256];
	char dir[256];

	memset((void *)fileName, 0, sizeof(fileName));		
	memset((void *)file, 0, sizeof(file));	
	memset((void *)pLocalData, 0, sizeof(LOCAL_DATA_TAG));
	sprintf(fileName, "%d_%d",pGameInfo->game_id,pGameInfo->user_id);

	sprintf(dir, "%s\\%d_%d",(char*)APP_NAME,pGameInfo->game_id,pGameInfo->user_id);

	find_create_local_dir(dir);
	i51AdeStdWSprintf(file, 256, "%s\\%s\\%s.dat",APP_NAME,fileName,fileName);	
	read_local_file(pGameInfo, (unsigned short *)file,pLocalData); 	
}

void update_local_chp_data(int chp,LOCAL_DATA_TAG *pLocalData)
{
	if(pLocalData->mChp < chp)
	{
		pLocalData->mChp = chp;
	}	
	write_local_file(pLocalData);
}

int get_local_chp_data(const LOCAL_DATA_TAG *pLocalData)
{
	return pLocalData->mChp;
}

void set_local_score_data(int score,LOCAL_DATA_TAG *pLocalData)
{
	int loop;
	int order;	
	RANK_DATA_TAG swapTag;
	int newScore;

	newScore = score;	
	memset((void *)&swapTag,0,sizeof(RANK_DATA_TAG));
	for(loop = 0; loop < MAX_RECORD_NUM; loop++)
	{
		if(newScore <= 0)
		{
			for(order = loop+1; order < MAX_RECORD_NUM; order++)
			{
				if( pLocalData->mRankData[order].mScore > pLocalData->mRankData[order - 1].mScore)
				{
					memcpy(&swapTag,&pLocalData->mRankData[order - 1],sizeof(RANK_DATA_TAG));					
					pLocalData->mRankData[order - 1] = pLocalData->mRankData[order];					
					memcpy(&pLocalData->mRankData[order],&swapTag,sizeof(RANK_DATA_TAG));
				}
			}
		}
		else
		{
			if(newScore > pLocalData->mRankData[loop].mScore)
			{	
				RANK_DATA_TAG insertTag;
				insertTag.mUserId = pLocalData->mUserId;
				sprintf(insertTag.mName,"%s",pLocalData->mName);				
				insertTag.mScore = newScore;
				for(order = loop; order < MAX_RECORD_NUM; order++)
				{						
					memcpy(&swapTag,&pLocalData->mRankData[order],sizeof(RANK_DATA_TAG));	
					memcpy(&pLocalData->mRankData[order],&insertTag,sizeof(RANK_DATA_TAG));											
					memcpy(&insertTag,&swapTag,sizeof(RANK_DATA_TAG));						
				}
				break;
			}
		}
	}
	write_local_file(pLocalData);
}

int update_local_success_data(int data[], char success_id)
{
	if(data[success_id])
		return 0;

	data[success_id] = iTRUE;
	return 1;
}

#endif