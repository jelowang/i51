#ifndef I51PUBLIC_H
#define I51PUBLIC_H

#include "i51.h"
#include "i51KitAde.h"
#include "LLPInput.h"
#include "i51KitG2Ex.h"



#define ARRAY_SUM(a) (sizeof(a)/sizeof(*(a)))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(x) ((x)>0?(1*(x)):(-1 * (x)))


#define COLOR_BLACK          (i51KitG2COLOR(0,0,0))
#define COLOR_GRAY          (i51KitG2COLOR(128,128,128))
#define COLOR_WHITE          (i51KitG2COLOR(255,255,255))

#define COLOR_RED         (i51KitG2COLOR(255,0,0))
#define COLOR_ORANGE         (i51KitG2COLOR(255,165,0))
#define COLOR_YELLOW         (i51KitG2COLOR(255,255,0))
#define COLOR_GREEN         (i51KitG2COLOR(0,255,0))
#define COLOR_DEEPGREEN         (i51KitG2COLOR(0,100,0))
#define COLOR_CHING         (i51KitG2COLOR(0,127,255))
#define COLOR_BLUE         (i51KitG2COLOR(0,0,255))
#define COLOR_PURPLE         (i51KitG2COLOR(139,0,255))


#define COLOR_MAGENTA         (i51KitG2COLOR(255,0,255))
#define COLOR_TEAL         (i51KitG2COLOR(0,255,255))
#define COLOR_LIME         (i51KitG2COLOR(127,255,0))
#define COLOR_BROWN         (i51KitG2COLOR(128,42,42))
#define COLOR_PINK         (i51KitG2COLOR(255,192,203))




#define COLOR_DONTKNOW        (i51KitG2COLOR(255,255,255))
#define COLOR_DEEPLY_BLUE        (i51KitG2COLOR(0,99,168))

#define COLOR_LOWBLUE          (i51KitG2COLOR(0,166,252))
#define COLOR_BIGBLUE          (i51KitG2COLOR(0,146,221))
typedef enum tagCOLOR_TYPE
{
	//COLOR_TYPE_RED = 0,
	COLOR_TYPE_ORANGE = 0,
	COLOR_TYPE_YELLOW,
	COLOR_TYPE_GREEN,
	COLOR_TYPE_CHING,
	COLOR_TYPE_BLUE,
	COLOR_TYPE_PURPLE,
	COLOR_TYPE_MAX,
}COLOR_TYPE;

typedef enum

{
	DRAW_STYLE_H_LEFT,
	DRAW_STYLE_H_MID,
	DRAW_STYLE_H_RIGHT,
	DRAW_STYLE_H_MAX,
}DRAW_STYLE_H;
typedef enum
{
	DRAW_STYLE_V_TOP,
	DRAW_STYLE_V_MID,
	DRAW_STYLE_V_BOTTOM,
	DRAW_STYLE_V_MAX,
}DRAW_STYLE_V;

typedef enum tagSTR_CMP
{
	STR_CMP_BIGER,
	STR_CMP_EQUAL,
	STR_CMP_SMALLER,
}STR_CMP;

#define _T(s) L ## s

#define APP_DIR       "i51Explorer"

//#define  SPECIAL_LOG
//#define  NORMAL_LOG
//////
#if defined(NORMAL_LOG)
#define LOG(...) iLog(APP_DIR": " __VA_ARGS__)
#else
#define LOG(...)
#endif

#if defined(SPECIAL_LOG)
#define SLOG(...) iLog(APP_DIR":S: " __VA_ARGS__)
#else
#define SLOG(...)
#endif

extern void i51FrameShiftXY(iRECT *prc,iS16 addX,iS16 addY);
extern iBOOL i51PublicCheckUSMatchTail(iWCHAR *src_Str,iWCHAR *tail_Str);
extern void i51FrameItemInitRect(iRECT *prc, iS32 x, iS32 y, iU32 w, iU32 h);
extern iBOOL i51FrameGetOverlapArea(iRECT *prc1, iRECT *prc2, iRECT *prcOverlap);
extern void i51FrameMovePanel(iRECT *prc, iS16 x, iS16 y);
extern void i51PublicDrawSysWStr(iWCHAR *pwStr,iRECT *prect,iCOLOR Color,DRAW_STYLE_H style_h,DRAW_STYLE_V style_v);
extern iBOOL i51PubicPtInRect(iS16 pt_x,iS16 pt_y,iRECT *dstRect);
extern STR_CMP i51PublicStrCmp(iCHAR *pStrA,iCHAR *pStrB);
//extern iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color);
#endif