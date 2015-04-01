#ifndef i51DOWNLOADER_H_
#define i51DOWNLOADER_H_

#include "i51.h"

typedef enum
{

	//	notes : 请求类型

	//	自动请求
	i51_SAP_DL_REQ_MODE_AUTO = 1 ,
	//	点击请求
	i51_SAP_DL_REQ_MODE_CLICK = 2 ,

} i51DLREQMODE; 


extern iBOOL i51SlDlAbleCreate () ;
extern iHANDLE i51SlDlCreate (void (*callback) (iBOOL ret)) ;
extern iBOOL i51SlDlStop ( iHANDLE dl ) ;
extern iBOOL i51SlDlStart ( 
	iHANDLE dl , 
	const iCHAR *uri , 
	iU32 urisize ,  
	const iCHAR* path , 
	const iCHAR* name , 
	iU32 versionId ,
	i51SlDlCOMMAND cmd , 
	i51SlDlCALLBACK callback 
) ;

extern iBOOL i51MiniSlDlStop ( iHANDLE dl ) ;
extern iBOOL i51MiniSlDlStart ( 
	iHANDLE dl , 
	const iCHAR *uri , 
	iU32 urisize ,  
	const iCHAR* path , 
	const iCHAR* name , 
	iU32 versionId ,
	i51SlDlCOMMAND cmd , 
	i51SlDlCALLBACK callback 
) ;
#endif
