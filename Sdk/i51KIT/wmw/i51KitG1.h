
#ifndef _I51KITG1_H_

#include "i51.h"

#define EFFECT_MAXBLADEWIDTH	4		//刀锋最大宽度(一半)
#define EFFECT_MAXKNIFEPOINT    12		//刀尖长
#define EFFECT_MAXTRACKPOINT	6		//轨迹点数组最大个数
#define EFFECT_MINDISTANCE		144		//轨迹点间最小距离的平方
#define EFFECT_MAXPOINTNUM		5

extern iS16 se_TrackPointCount;
extern iS16 se_TrackPoint[EFFECT_MAXTRACKPOINT][2];

extern iBOOL i51KitG1Draw(void);

#endif

