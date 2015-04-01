//#include "..\marmota\marmota.h"
#include "mm_superangrybird.h"

//#define CF_FILE_PATHD "/CARD/angrybird.data"

void mmSABReadGameRestore()
{
//	int handle;
//	char filepath[128] = {0};
//	#ifdef MAMOTA_BACKEND_MTK
//	sprintf(filepath, "%c:\\%s",marmota->record->getCardDerive(),MMSAB_FILE_PATH);
//	#elif defined(MAMOTA_BACKEND_MSTAR)
//	sprintf(filepath, "/CARD/%s",MMSAB_FILE_PATH);
//	#else
//	sprintf(filepath, "E:\\%s",MMSAB_FILE_PATH);
//	#endif 
////#define MAMOTA_BACKEND_MSTAR
////#define MAMOTA_BACKEND_ST
//	
//	handle = marmota->record->openRecord(filepath);
//	
//	if(handle == 0){
//		mmSABWriteGameRestore();
//	}
//	else{
//		marmota->record->readRecord(filepath, (void*)mmSABLevelScore, 4*(MMSAB_LEVEL_SUM+1));
//	}

	iU16 file[66];
	int fd;
	iU32 Rsize;

	i51AdeStdWSprintf(file, 66, "%s\\%s", GAME_DIR, MMSAB_FILE_PATH);
	fd = i51AdeOsFileOpen(file, i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__, __LINE__);
	if(fd <= 0)
	{
		mmSABWriteGameRestore();
	}
	else 
	{
		//i51AdeOsFileGetSize(fd, &fdLen);
		i51AdeOsFileRead(fd, (void *)mmSABLevelScore, 4*(MMSAB_LEVEL_SUM+1), &Rsize);
		i51AdeOsFileClose(fd);
	}
}

void mmSABWriteGameRestore()
{
	//marmota->record->writeRecord((char*)MMSAB_FILE_PATH, (void*)mmSABLevelScore, 64);
	char filepath[128] = {0};
	/*#ifdef MAMOTA_BACKEND_MTK
	sprintf( filepath, "%c:\\%s", marmota->record->getCardDerive(),MMSAB_FILE_PATH );
	#elif defined(MAMOTA_BACKEND_MSTAR)
	sprintf(filepath, "/CARD/%s", MMSAB_FILE_PATH);
	#else
	sprintf(filepath, "E:\\%s", MMSAB_FILE_PATH);
	#endif
	marmota->record->writeRecord(filepath, (void*)mmSABLevelScore, 4*(MMSAB_LEVEL_SUM+1));*/
//	sprintf(filepath, "%s\\%s", GAME_DIR, MMSAB_FILE_PATH);
	pfunc->public_write_file(filepath, (void *)mmSABLevelScore, 4*(MMSAB_LEVEL_SUM+1));
}
