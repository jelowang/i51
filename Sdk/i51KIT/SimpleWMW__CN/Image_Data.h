#ifndef IMAGE_DATA_H
#define  IMAGE_DATA_H

#ifndef PKIG_SCREEN_HORIZONTAL

#if defined PKIG_SCREEN_320X480
//#include "./inc_mmSuperAngryBird/res_320x480/Image_Hex.h"
#include  "./dl_res/vertical/240x400/dl_Image_Hex.h"
#elif defined (PKIG_SCREEN_240X320)
//#include "./inc_mmSuperAngryBird/res_240x320/Image_Hex.h"
#include  "./dl_res/vertical/240x400/dl_Image_Hex.h"
#elif defined (PKIG_SCREEN_240X400)
//#include "./inc_mmSuperAngryBird/res_240x400/Image_Hex.h"
#include  "./dl_res/vertical/240x400/dl_Image_Hex.h"
#endif

#else

#if defined PKIG_SCREEN_320X480
//#include "./inc_mmSuperAngryBird/res_320x480/Image_Hex.h"
#include  "./dl_res/horizontal/320X480/dl_Image_Hex.h"
#elif defined (PKIG_SCREEN_240X320)
//#include "./inc_mmSuperAngryBird/res_240x320/Image_Hex.h"
#include  "./dl_res/horizontal/240x320/dl_Image_Hex.h"
#elif defined (PKIG_SCREEN_240X400)
//#include "./inc_mmSuperAngryBird/res_240x400/Image_Hex.h"
#include  "./dl_res/horizontal/240x400/dl_Image_Hex.h"
#endif

#endif

#endif