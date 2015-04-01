//#include "../../marmota.h"
#include "inc/mm_superangrybird_resource.h"

//#include "inc/res_240x320/mm_superangrybird_res_audio.cpp"

#if defined (MM_SUPERANGRYBIRD_RESOURCE_176X220)
	#include "./res_176x220/mm_superangrybird_res_audio.h"
	#include "./res_176x220/mm_superangrybird_res_img.h"
	#include "./res_176x220/mm_superangrybird_ui_layout.h"
	#include "./res_176x220/mm_superangrybird_mainmenu_img.h"
	#include "./res_176x220/mm_superangrybird_mainmenu_layout.h"
#elif defined (MM_SUPERANGRYBIRD_RESOURCE_240X320)
   #include "inc/res_240x320/mm_superangrybird_ui_layout.cpp"
   #include "inc/res_240x320/mm_superangrybird_mainmenu_layout.cpp"
   #include "inc/res_240x320/Res.cpp"
#elif defined (MM_SUPERANGRYBIRD_RESOURCE_240X400)
	//#include "inc/res_240x400/mm_superangrybird_res_audio.cpp"
	#include "inc/res_240x400/mm_superangrybird_ui_layout.cpp"
	#include "inc/res_240x400/mm_superangrybird_mainmenu_layout.cpp"
	#include "inc/res_240x400/Res.cpp"
#elif defined (MM_SUPERANGRYBIRD_RESOURCE_320X480)
	//#include "./res_320x480/mm_superangrybird_res_audio.cpp"
	#include "inc/res_320x480/mm_superangrybird_ui_layout.cpp"
	#include "inc/res_320x480/mm_superangrybird_mainmenu_layout.cpp"
	#include "inc/res_320x480/Res.cpp"
	
#endif

void hhtest(void)
{
	int a =0;

	a = 2*3;
}