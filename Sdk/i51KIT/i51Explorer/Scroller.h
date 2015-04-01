
#ifndef _Scroller_h_
#define _Scroller_h_

#include "i51.h"
#include "i51Widget.h"
#include "i51Public.h"
#include "i51Timer.h"
//#include "i51List.h"
//#define  SYS_TOOL  "SystemInfo"
#define  W_SYS_TOOL  L"SystemInfo"

#define  W_UNPACK_TOOL  L"UnPackage"

#define W_PACKAGE_TYPE L".package"
#define  START_Y  0
#define  MAX_INPUT_SUM 256
#define  MAX_LEN_OF_PATH_STR   256
typedef enum tagSYS_INFO_DIR{
	SYS_INFO_DIR_51PKPAY = 0,
	SYS_INFO_DIR_PKIGAPI,
	SYS_INFO_DIR_I51SYS,
	SYS_INFO_DIR_ID_MAX,
}SYS_INFO_DIR;
typedef enum
{
	uz_type_only_i51_package,
	uz_type_every_package,
}unzip_type;
typedef enum tagSTATE
{
  STATE_APP,
  STATE_UNPACKAGE,
  STATE_UNPACAGING,
}STATE;


#define  PEN_MOVE_FINAL_FIRST_INDEX 0
#define  PEN_MOVE_FINAL_LAST_INDEX 1
#define  MOVE_STAND  10

typedef struct tagScrollContent
{
	iCHAR  strSysInfoDir[SYS_INFO_DIR_ID_MAX][MAX_LEN_OF_PATH_STR];
	//List lv.ls;
	ListView lv;
	List packList;
	iU16 screenW;
	iU16 screenH;
	iU8  *pScreen;
	iBOOL bScrollStarted;
	iBOOL bBarVisible;
	iBOOL bSendSEQ;
	iU32 aid;
	
	LLPInputStruct inputArr[MAX_INPUT_SUM];
	LLPPhonePen penArr[MAX_INPUT_SUM];
	LLPPhonePen penDown;
	short offset;
	int count;
	iBOOL bPenDownActived;
	iBOOL bNewInputMove;
    ImageButton btn[BTN_ID_MAX];
	STATE state;
	iBOOL bNeedFlushBg;
	int unPackIndex;
}ScrollContent;
#endif