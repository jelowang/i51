#ifndef __I51_KITCOREDL_H__
#define __I51_KITCOREDL_H__

#include "i51.h"

extern iBOOL i51KitCoreSlDlStart ( 
	iHANDLE dl , 
	const iCHAR *uri , 
	iU32 urisize ,  
	const iCHAR* path , 
	const iCHAR* name , 
	iU32 versionId ,
	i51SlDlCOMMAND cmd , 
	i51SlDlCALLBACK callback 
);
extern iBOOL i51KitCoreSlDlStop ( iHANDLE dl );
extern iHANDLE i51KitCoreSlDlCreate (void (*callback) (iBOOL ret));
extern iBOOL i51KitCoreSlDlAbleCreate ();

#endif