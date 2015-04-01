
#include "i51KitAde.h"
#include "i51KitG2.h"
#include "SnapShotDemo.h"

enum 
{
    SIZE_1KB = 1 * 1024,
    SIZE_4KB = 4 * 1024,
    SIZE_8KB = 8 * 1024,
    SIZE_16KB = 16 * 1024,
    SIZE_32KB = 32 * 1024,
    SIZE_64KB = 64 * 1024,
    SIZE_128KB = 128 * 1024,
    SIZE_256KB = 256 * 1024,
    SIZE_512KB = 512 * 1024,
    SIZE_1MB = 1 * 1024 * 1024,
    SIZE_2MB = 2 * 1024 * 1024,
    SIZE_4MB = 4 * 1024 * 1024,
    SIZE_8MB = 8 * 1024 * 1024,
    SIZE_16MB = 16 * 1024 * 1024
};

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);

iU32 SnapShotTest = 0 ;

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
	
	iS32 Point[10][2];
	iU16 Color = 0 ;

	switch ( message )
	{
		case i51_MSG_PAINT :

			Point[0][0] = 15;
		Point[0][1] = 12;
		Point[1][0] = 115;
		Point[1][1] = 112;
		Color = 0xff99;
		i51KitG2DrawLine( Point, Color);		
		
		i51AdeMmiUpdateScreen () ;



		break ;
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

			i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE ( i51KitG2 ) ) 
			{
				//	TODO : 动态库加载失败，错误处理。
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

