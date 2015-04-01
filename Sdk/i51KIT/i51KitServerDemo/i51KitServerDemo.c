
#include "i51KitAde.h"
#include "i51KitServer.h"
#include "i51KitServerDemo.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_SYS_KIT_DEF(i51KitServer);
#if 1
unsigned char data_msg[] = {
	0x53, 0x00, 0x50, 0x00, 0x7B, 0x00, 0x31, 0x00, 0x33, 0x00, 0x34, 0x00, 0x38, 0x00, 0x30, 0x00, 
	0x31, 0x00, 0x38, 0x00, 0x36, 0x00, 0x30, 0x00, 0x31, 0x00, 0x36, 0x00, 0x7D, 0x00, 0x43, 0x00, 
	0x4F, 0x00, 0x4E, 0x00, 0x7B, 0x00, 0x2A, 0x00, 0xC6, 0x5B, 0x01, 0x78, 0x2A, 0x00, 0x6E, 0x78, 
	0xA4, 0x8B, 0xB9, 0x70, 0xAD, 0x64, 0x2A, 0x00, 0x7D, 0x00, 0x44, 0x00, 0x59, 0x00, 0x4E, 0x00, 
	0x7B, 0x00, 0x2C, 0x67, 0x21, 0x6B, 0xC6, 0x5B, 0x01, 0x78, 0x2A, 0x00, 0x30, 0x52, 0x7D, 0x00, 
	0x44, 0x00, 0x59, 0x00, 0x4E, 0x00, 0x7B, 0x00, 0x30, 0x52, 0xF7, 0x53, 0x01, 0x78, 0x2A, 0x00, 
	0x02, 0x30, 0x7D, 0x00, 0x41, 0x00, 0x43, 0x00, 0x54, 0x00, 0x7B, 0x00, 0x31, 0x00, 0x7D, 0x00, 
	0x54, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x45, 0x00, 0x7B, 0x00, 0x31, 0x00, 0x38, 0x00, 0x30, 0x00, 
	0x7D, 0x00, 0x00, 0x00
};
unsigned char data_msg1[] = {
	0x53, 0x00, 0x50, 0x00, 0x7B, 0x00, 0x31, 0x00, 0x33, 0x00, 0x34, 0x00, 0x38, 0x00, 0x30, 0x00, 
	0x31, 0x00, 0x38, 0x00, 0x36, 0x00, 0x30, 0x00, 0x31, 0x00, 0x36, 0x00, 0x7D, 0x00, 0x43, 0x00, 
	0x4F, 0x00, 0x4E, 0x00, 0x7B, 0x00, 0x2A, 0x00, 0xC6, 0x5B, 0x01, 0x78, 0x2A, 0x00, 0x6E, 0x78, 
	0xA4, 0x8B, 0xB9, 0x70, 0xAD, 0x64, 0x2A, 0x00, 0x7D, 0x00, 0x44, 0x00, 0x59, 0x00, 0x4E, 0x00, 
	0x7B, 0x00, 0x2C, 0x67, 0x21, 0x6B, 0xC6, 0x5B, 0x01, 0x78, 0x2A, 0x00, 0x30, 0x52, 0x7D, 0x00, 
	0x53, 0x00, 0x54, 0x00, 0x41, 0x00, 0x7B, 0x00, 0x4E, 0x00, 0x55, 0x00, 0x4C, 0x00, 0x4C, 0x00, 
	0x7D, 0x00, 0x41, 0x00, 0x43, 0x00, 0x54, 0x00, 0x7B, 0x00, 0x31, 0x00, 0x7D, 0x00, 0x54, 0x00, 
	0x49, 0x00, 0x4D, 0x00, 0x45, 0x00, 0x7B, 0x00, 0x31, 0x00, 0x39, 0x00, 0x30, 0x00, 0x7D, 0x00, 
	0x0D, 0x00, 0x53, 0x00, 0x50, 0x00, 0x7B, 0x00, 0x31, 0x00, 0x33, 0x00, 0x34, 0x00, 0x38, 0x00, 
	0x30, 0x00, 0x31, 0x00, 0x38, 0x00, 0x36, 0x00, 0x30, 0x00, 0x31, 0x00, 0x36, 0x00, 0x7D, 0x00, 
	0x43, 0x00, 0x4F, 0x00, 0x4E, 0x00, 0x7B, 0x00, 0x2A, 0x00, 0xC6, 0x5B, 0x01, 0x78, 0x2A, 0x00, 
	0x6E, 0x78, 0xA4, 0x8B, 0xB9, 0x70, 0xAD, 0x64, 0x2A, 0x00, 0x7D, 0x00, 0x53, 0x00, 0x54, 0x00, 
	0x41, 0x00, 0x7B, 0x00, 0x4E, 0x00, 0x55, 0x00, 0x4C, 0x00, 0x4C, 0x00, 0x7D, 0x00, 0x53, 0x00, 
	0x54, 0x00, 0x41, 0x00, 0x7B, 0x00, 0x4E, 0x00, 0x55, 0x00, 0x4C, 0x00, 0x4C, 0x00, 0x7D, 0x00, 
	0x41, 0x00, 0x43, 0x00, 0x54, 0x00, 0x7B, 0x00, 0x31, 0x00, 0x7D, 0x00, 0x54, 0x00, 0x49, 0x00, 
	0x4D, 0x00, 0x45, 0x00, 0x7B, 0x00, 0x32, 0x00, 0x30, 0x00, 0x30, 0x00, 0x7D, 0x00, 0x00, 0x00
};
#endif

void i51KitServerDemoSmsCallback ( iBOOL value ) {

	if ( iTRUE == value ) {
		i51AdeOsLog ( 0 , "短信发送成功" ) ;
	} else {
		i51AdeOsLog ( 0 , "短信发送失败" ) ;
	}
	
}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
	iS8* script1 = "4000 Check Idle>200&&calltime>20 do Server5";
	iS8* script2 = "5000 check idle>200&&calltime>20 do Server6";

	iU8 i = 50;
iU32 d = 0;
	switch ( message )
	{
 
		case i51_MSG_PAINT :
		{
			iU32* panel = i51AdeMmiGetPanel () ; 
			memset ( (void*)panel[1] , 0 , (panel[2]*panel[3])<<1 ) ;
			//i51AdeMmiDrawText ( (void*)panel[1] , "i51KitServer Demo Runing" , 0 , 30 , panel[2] , 0 ) ;
			//i51AdeMmiDrawText ( (iU8*)panel[1] , L"i51KitServer Demo Runing" , 0 , 30 , panel[2] , 0 ) ;
			                     
			i51AdeMmiUpdateScreen () ;
		}
		break ;
		case i51_MSG_INIT :
		{
	
			//i51SMS i51Sms = { 0 } ;

			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

			i51KIT_DYNAMIC_SYS_KIT_LOAD(i51KitServer,L"i51KitServer.kit");
	           	
			if ( iNULL == i51KIT_DYNAMIC_SYS_KIT_PROTOTYPE(i51KitServer) ) 
	        {
	           		i51AdeOsLog ( 0 , "i51KitServerDemo : Load i51KitServer.kit Failed" ) ;
	           	  	return 0 ;
			}

	        i51AdeOsLog ( 0 , "i51KitServerDemo : Load i51KitServer.kit Succeed" ) ;
			{
				i51KernelSendRequest ( aid , i51_REQ_EXIT , 0 , 0 ) ;
			}

				
#if 0
             i51KitServerSmsSetFlitScript((iU16 *)data_msg);
			 i51KitServerSmsSetFlitScript((iU16 *)data_msg1);
			//i51KitServerSmsUnsetFlitScript();
			 //i51KitServerT2rUnsetScript();
#endif

#if 1
			//i51KitServerT2rSetScript(script1);
			//i51KitServerT2rSetScript(script2);
			i51KitServerT2rUnsetScript();
#endif
#if 0
			 while(i>0)
			 {
				 i51KitServerSmsSend ( 1 , 1 , "13480186016" , L"hello" , 100 , i51KitServerDemoSmsCallback  ) ;
				 i51KitServerSmsCreateDevice();
				 i--;
			 }
#endif
		}
		break ;

		case i51_MSG_PAUSE :
			i51AdeOsLog ( 0 , "暂停" ) ;
		break ;

		case i51_MSG_EXIT :
			i51AdeOsLog ( 0 , "退出" ) ;
		break ;
		

	}


	return iTRUE ;

}
