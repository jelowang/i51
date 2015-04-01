
#include "i51KitAde.h"
#include "i51KitAmr.h"

i51KIT_STATIC_KIT_DEF(Adapter);

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	ÏûÏ¢Ñ­»·

	switch ( message )
	{
		
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
	
		}
		break ;
		
		case i51_MSG_RUN :
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

	//	TODO : Initialize API Port for external layer

	extern iS32 i51KitAmrDecoder ( iU8* rbuff , iU32 rlength , iU8* wbuff , iU32  wlength ) ;
	extern iS32 i51KitAmrDecoderEx (const iU16 * rfilename,const iU16 * wfilename) ;
	
	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitAmr)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitAmrDecoder)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitAmrDecoderEx)
	i51KIT_DYNAMIC_KIT_MAPING_END 

	return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitAmr) ;
	

}
