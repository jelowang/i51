#if !defined(_i51List_h_)
#define _i51List_h_
#include "i51.h"
#include "i51Public.h"


typedef struct tagList
{
  struct tagNode *pHead;
  struct tagNode *pTail;
  int sum;
}List;

typedef enum tagITEM_STATE
{
	ITEM_STATE_IDLE,
	ITEM_STATE_DOWN,
}ITEM_STATE;
typedef enum tagITEM_TYPE
{
  ITEM_TYPE_APP,
  ITEM_TYPE_TOOL,
  ITEM_TYPE_PACK,
}ITEM_TYPE;
typedef struct tagNode
{
	iWCHAR wszCaption[128];
	int index;
	struct tagNode *pPre;
	struct tagNode *pNext;
	iRECT rc;
	iCOLOR icIdleCompleten;
	iCOLOR icIdleUnCompleten;
	iCOLOR icDown;
	iCOLOR icon;
	iCOLOR txtCompleten;
	iCOLOR txtUnCompleten;
	ITEM_STATE preState;
	ITEM_STATE curState;
	iBOOL bCompleten;
	ITEM_TYPE type;
}Node;


extern int i51ListGetSum( List *pList);
extern iBOOL i51ListInit( List *pList);
extern void i51ListPrint( List *pList);
extern iBOOL i51ListInsertNode(List *pList,Node *pNode);
extern iBOOL i51ListDelNodeByNode(List *pList, List *pNode);
extern iBOOL i51ListDelNodeByCaption(List *pList, iWCHAR *pwszCap);
extern iBOOL i51ListDelNodeByIndex(List *pList, int Index);
extern iBOOL i51ListDestroy(List *pList);
extern void i51ListSort(List *pList);
extern iBOOL i51ListInsertNodeSort(List *pList,Node *pNode);
extern iBOOL i51ListInsertNodeBeforeOneNode(List *pList,Node *pAddNode,Node *pBeNode);
#endif