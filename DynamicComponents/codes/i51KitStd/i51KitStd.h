
#ifndef THE_i51KitStd_H
#define THE_i51KitStd_H

#include "i51.h"

//#define RELEASE_i51KitStd

i51KIT_DYNAMIC_KIT_DEF_BEGIN 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResOpen) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResDelete) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResGetTextSize) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResGetTextIDString) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResGetText) 
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResClose)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResGetContentSize)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResGetContentIDString)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdResGetContentEntry)
	//	Jelo Added Start 2011.11.29
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdGetKitRef)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdGetKitAttr)		
	//	Jelo Added Finish
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitStdGetReVersion)	
i51KIT_DYNAMIC_KIT_DEF_END

i51KIT_DYNAMIC_KIT_DEF_TYPE(iFILE , i51KitStdResOpen , ( iU16* name ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL , i51KitStdResDelete , ( iU16* name  ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iU32 , i51KitStdResGetTextSize , (  iFILE  file , iU32 textid  ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iCHAR* , i51KitStdResGetTextIDString , (  iFILE  file , iU32 textid  ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( void , i51KitStdResGetText , (  iFILE  file , iU32 textid ,  void* buffer , iU32 bufsize ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iBOOL , i51KitStdResClose , ( iFILE file ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iU32 , i51KitStdResGetContentSize , ( iFILE  file , iU32 imageid ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iCHAR* , i51KitStdResGetContentIDString , (  iFILE  file ,iU32 imageid ) )
i51KIT_DYNAMIC_KIT_DEF_TYPE( iU32 , i51KitStdResGetContentEntry , ( iFILE  file , iU32 imageid ) )
//	Jelo Added Start 2011.11.29
i51KIT_DYNAMIC_KIT_DEF_TYPE(iS32,i51KitStdGetKitRef,(iU16* filename , i51SllDKITREF* i51SllDKitRefBuf , iU32* i51SllDKitRefTotall))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitStdGetKitAttr,(iU16* filename , iCHAR* name , iU32* Version , iU32* Kernel , iU32* Adapter , iU32* Endian))
//	Jelo Added Finish
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL,i51KitStdGetReVersion,( iU16* name , iU32 *version ))

#ifdef RELEASE_i51KitStd

#define i51KitStdResOpen i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResOpen)
#define i51KitStdResDelete i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResDelete)
#define i51KitStdResGetTextSize i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResGetTextSize)
#define i51KitStdResGetTextIDString i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResGetTextIDString)
#define i51KitStdResGetText i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResGetText)
#define i51KitStdResClose i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResClose)
#define i51KitStdResGetContentSize i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResGetContentSize)
#define i51KitStdResGetContentIDString i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResGetContentIDString)
#define i51KitStdResGetContentEntry i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdResGetContentEntry)
//	Jelo Added Start 2011.11.29
#define i51KitStdGetKitRef i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdGetKitRef)
#define i51KitStdGetKitAttr i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdGetKitAttr)
//	Jelo Added Finish
#define i51KitStdGetReVersion i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitStd,i51KitStdGetReVersion)
#endif

i51KIT_DYNAMIC_KIT_DEC(i51KitStd) ;

#endif
