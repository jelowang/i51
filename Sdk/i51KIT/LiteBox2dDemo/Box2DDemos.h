



#ifndef __BOX2D_DEMOS_H__
#define __BOX2D_DEMOS_H__

// #include "MexLib.h"
// #include "MexLib_Std.h"
// #include "MexLib_Os.h"
// 
// extern funcptr *MEX_C_LIB_FUNC_NAME_STD;
// extern funcptr *MEX_C_LIB_FUNC_NAME_OS;

#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitStd.h"

#include "World.h"
#include "Body.h"
#include "Joint.h"

extern Body *bodies;
extern Joint *joints;

extern Body* bomb;

extern float timeStep;
extern int iterations;

extern int numBodies;
extern int numJoints;

extern int demoIndex;
extern World world;

extern char demoStrings[9][40];



void InitDemo(int index);
void LaunchBomb(void);








#endif /* __BOX2D_DEMOS_H__ */

