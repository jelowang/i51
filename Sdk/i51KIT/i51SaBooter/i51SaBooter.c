
#include "i51KitAde.h"
#include "i51SaBooter.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	ÏûÏ¢Ñ­»·

	switch ( message )
	{
		case i51_MSG_PAINT :
			
			
		break ;

		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
			
			i51AdeOsLog ( 0 , "APP : Interal Booter Runing" ) ;
			
			i51KernelSendRequest ( aid , i51_REQ_LOADEX , (void*)L"Demo" , 4*2+2 ) ;
			

		}
		break ;

		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
		break ;
		

	}

	return iTRUE ;

}
	
iKIT iU32* i51KitPort ()
{
	return 0 ;
}