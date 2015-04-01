#if !defined (_i51Widget_h_)
#define  _i51Widget_h_

#include "i51.h"
#include "i51KitAde.h"
#include "i51Public.h"
#include "i51KitG2Ex.h"
#include "i51List.h"
#define  OFFSET_EACH  50
#define  ICON_W  40

typedef void (*fnAction)(void*, void*);
typedef enum tagSCROLL_DIRT
{
	SCROLL_DIRT_VERTICAL_UP,
	SCROLL_DIRT_VERTICAL_DOWN,
	SCROLL_DIRT_HORIZONTAL_LEFT,
	SCROLL_DIRT_HORIZONTAL_RIGHT,
}SCROLL_DIRT;

typedef enum tagSCROLL_REDRAW_LIMIT
{
   SCROLL_REDRAW_LIMIT_NONE,
   SCROLL_REDRAW_LIMIT_TOP,
   SCROLL_REDRAW_LIMIT_BOTTOM,
   SCROLL_REDRAW_LIMIT_LEFT,
   SCROLL_REDRAW_LIMIT_RIGHT,
}SCROLL_REDRAW_LIMIT;

typedef enum tagSCROLL_SPEED_DEP_RANGE
{
	SCROLL_SPEED_DEP_RANGE_0 = 0,
	SCROLL_SPEED_DEP_RANGE_200 = 200,
	SCROLL_SPEED_DEP_RANGE_300 = 300,
	SCROLL_SPEED_DEP_RANGE_400 = 400,
	SCROLL_SPEED_DEP_RANGE_500 = 500,
	SCROLL_SPEED_DEP_RANGE_600 = 600,
	SCROLL_SPEED_DEP_RANGE_700 = 700,
	SCROLL_SPEED_DEP_RANGE_800 = 800,
	SCROLL_SPEED_DEP_RANGE_900 = 900,
	SCROLL_SPEED_DEP_RANGE_1000 = 1000,
}SCROLL_SPEED_DEP_RANGE;

typedef struct tagBaseAttr
{

	iS16 id;

	iS16 item;

	iU8 priority;

	iU8 nChild;

	iS16 absX;

	iS16 absY;

	iRECT rc;

	struct tagGeneral *pParent;

	struct tagGeneral **pChildren;
}BaseAttr;

typedef struct tagGeneral
{
	BaseAttr    atr;
}General;


typedef struct tagAction
{
	fnAction onPadDown;
	fnAction onPadUp;
	fnAction onPadMove;
	fnAction onPadClicked;
}Action;


//typedef iU32 Adress
typedef struct tagImage 
{
	BaseAttr atr;
//	Adress ads; 
	iS32  id;
    iRECT src;
}Image;


typedef struct tagText
{

	BaseAttr atr;
	iS32 id;
	iU8 *pContent;
}Text;
typedef struct tagButton
{
    BaseAttr atr;
	General *pChildren[3];
//	Image bg;
	Image icon;
	iS32 bgIdle;
	iCOLOR bg;
	iS32 bgDown;
	iS32 icIdle;
	iS32 icDown;
}Button;


typedef struct tagScroll
{

	BaseAttr Attr;
	General *pChildren[2];
	Image bg;
	Image bar;
	iU32 range;
    SCROLL_DIRT dirt;
}Scroll;

typedef struct tagLabel
{
	BaseAttr atr;
	General *pChildren[2];
	Image bg;

}Label;

typedef struct tagListBar
{

	iRECT rc;
	iCOLOR icIdle;
	iCOLOR icDown;
    
}ListBar;

typedef struct  tagListView
{
	void *pParent;
	iRECT rcCtr;
	iS16  xRel;
	iS16  yRel;
	iS16  wAbs;
	iS16  hAbs;
	struct tagListBar bar;
	List ls;
	SCROLL_DIRT dir;
	iBOOL bBarVisible;
	iBOOL bAutoScrolling;
	int scrollSpeed;
	int scrollRange;
	int scrollCurOffSet;
}ListView;

typedef enum tagBTN_ID
{
//   BTN_ID_APP,
//   BTN_ID_SYS,
//   BTN_ID_TOOL,
  BTN_ID_UNPACKAGE_UNPACK,
  BTN_ID_UNPACKAGE_BACK,
  BTN_ID_MAX,
}BTN_ID;
typedef struct tagImageButton
{
  BTN_ID id;
  iU32 imgAD;
  iRECT imgRc;
  iRECT touchRc;
  ITEM_STATE preState;
  ITEM_STATE curState;
  iCOLOR bgIcidle;
  iCOLOR bgIcDown;
  iBOOL bNeedFlushed;
  iWCHAR wszCaption[128];
}ImageButton;
extern void i51WidgetMoveScrollRender(ListView *lv,SCROLL_DIRT dir,iS16 offSetTotal);
extern void i51WidgetScrollDraw(ListView *lv,iS16 dstX,iS16 dstY,iRECT *prc,SCROLL_REDRAW_LIMIT limit);
extern void i51WidgetScrollDrawBar(ListView *lv);
extern void i51WidgetScrollSetItemActived(ListView *lv,iS16 x,iS16 y);
extern void i51WidgetScrollSetAllIdleState(ListView *lv);
extern Node* i51WidgetScrollGetNodeInfo(ListView *lv,iS16 x,iS16 y);
extern iBOOL i51WidgetCreateImageButton(ImageButton *pBtn,BTN_ID id,iWCHAR *pwszCaption,iU32 imgAD,iRECT *pimageRc,iRECT *ptouchRc,iCOLOR bgIcidle,iCOLOR bgIcDown);
#endif