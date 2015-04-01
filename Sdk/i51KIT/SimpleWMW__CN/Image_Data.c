#include "i51Public.h"

#ifndef PKIG_SCREEN_HORIZONTAL

#if defined PKIG_SCREEN_320X480
    #include   "./dl_res/vertical/320x480/dl_Image_Hex.cpp"
#elif defined (PKIG_SCREEN_240X320)
    #include  "./dl_res/vertical/240x320/dl_Image_Hex.cpp"
#elif defined (PKIG_SCREEN_240X400)
    #include  "./dl_res/vertical/240x400/dl_Image_Hex.cpp"
#endif

#else

#if defined PKIG_SCREEN_320X480
#include   "./dl_res/horizontal/320x480/dl_Image_Hex.cpp"
#elif defined (PKIG_SCREEN_240X320)
#include  "./dl_res/horizontal/240x320/dl_Image_Hex.cpp"
#elif defined (PKIG_SCREEN_240X400)
#include  "./dl_res/horizontal/240x400/dl_Image_Hex.cpp"
#endif

#endif