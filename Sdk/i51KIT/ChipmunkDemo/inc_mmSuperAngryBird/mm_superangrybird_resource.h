#ifndef __MM_SUPERANGRYBIRD_RESOURCE__
#define __MM_SUPERANGRYBIRD_RESOURCE__

//#define MM_SUPERANGRYBIRD_RESOURCE_176X220
//#define MM_SUPERANGRYBIRD_RESOURCE_240X320
//#define MM_SUPERANGRYBIRD_RESOURCE_240X400
#define MM_SUPERANGRYBIRD_RESOURCE_320X480

#if defined (MM_SUPERANGRYBIRD_RESOURCE_176X220)
	#include "mm_superangrybird_resource_176x220.h"
#elif defined (MM_SUPERANGRYBIRD_RESOURCE_240X320)
	#include "mm_superangrybird_resource_240x320.h"
#elif defined (MM_SUPERANGRYBIRD_RESOURCE_240X400)
	#include "mm_superangrybird_resource_240x400.h"
#elif defined (MM_SUPERANGRYBIRD_RESOURCE_320X480)
	#include "mm_superangrybird_resource_320x480.h"
#endif

#endif