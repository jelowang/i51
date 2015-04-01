
#include "i51KitAde.h"
#include "i51KitStdDemo.h"
#include "i51KitStd.h"
#include "i51KitStdDemoRes.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);


static iS32 fun ()
{
	return -1 ;
}
iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
	int i ;
	static iBOOL flag = iTRUE ;
	switch ( message )
	{
	case i51_MSG_PAINT :
		{
			if (flag) {
			
						
#if 1
			iFILE file = 0 ;
			iFILE file2 = 0 ;
			iU32 size = 0 ;
			iU32 offset = 0 ;
			iS8 str[200] = {0} ;
			iS8 *p = NULL ;
			void *data = NULL ;


			file = i51KitStdResOpen ( L"fonts\\SmallFive.re" ) ;

			i51KitStdResClose ( file ) ;
			flag = iFALSE ;
			//i51AdeOsFileClose ( file2 ) ; 
#endif
			}

		}
		break;

		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
				//	加载i51KitG2动态库

			i51KIT_STATIC_KIT_ASSERT(Adapter);

			i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ) 
	         {
				i51AdeOsLog ( 0 , "LOAD FAIL") ;
	                 return 0 ;
	            }

	
		}
		break ;

		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
		break ;
		

	}

	

	return iTRUE ;

}
