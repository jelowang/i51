 
/*

	Layer belong : 'Static Components'.
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/
 
#include "i51AdeOs.h"
#include "i51AdeStd.h"
#include "i51SlLoader.h"

#ifdef i51SIMULATOR
#include <windows.h>
#endif

static iS32 i51SlLoaderLastError = 0 ;
static i51SYMPOOL i51SymbolPool = { 0 } ;

#ifndef i51_LOADER_LOADFAKE

#ifdef i51_LOADER_DBG
void i51SlLoaderDump ( iU16* name , void* pool , iU32 length ) {

	//	author : Jelo 
	//	since : 2011.5.13
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	   
	#ifdef i51SIMULATOR
		FILE* file = fopen ( "c:\\i51DUMP.i51" , "wb+" ) ;
		fwrite ( pool , length , 1 , file ) ;
		fflush ( file ) ;
		fclose ( file ) ;
	#else

		iU32 written = 0 ;
		iFILE file = 0 ;

		iU16 newname[32] = { 0 } ;
		iU16* namep = 0 ;

		namep = i51AdeStdWStrcpy ( newname , name ) ; 
		namep = i51AdeStdWStrcat ( namep , L".a4rel.old" ) ;
		
		file = i51AdeOsFileOpen ( namep , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
		  
		if ( !file ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : Create DUMP F" ) ;		
			return ;
		}
    
		i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
		i51AdeOsFileWrite ( file , pool , length , &written ) ;
		i51AdeOsFileFlush ( file ) ;
		i51AdeOsFileClose ( file ) ;
		 
	#endif
 
}

void i51SlLoaderB4RelDump ( iU16* name , void* pool , iU32 length ) {

	//	author : Jelo 
	//	since : 2011.5.13
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	#ifdef i51SIMULATOR
		FILE* file = fopen ( "c:\\i51DUMP.i51" , "wb+" ) ;
		fwrite ( pool , length , 1 , file ) ;
		fflush ( file ) ;
		fclose ( file ) ;
	#else

		iU32 written = 0 ;
		iFILE file = 0 ;

		iU16 newname[32] = { 0 } ;
		iU16* namep = 0 ;

		namep = i51AdeStdWStrcpy ( newname , name ) ; 
		namep = i51AdeStdWStrcat ( namep , L".b4rel.old" ) ;
		
		file = i51AdeOsFileOpen ( namep , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
		  
		if ( !file ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : Create DUMP F" ) ;		
			return ;
		}
  
		i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
		i51AdeOsFileWrite ( file , pool , length , &written ) ;
		i51AdeOsFileFlush ( file ) ;
		i51AdeOsFileClose ( file ) ;
		 
	#endif
 
}

void i51SlLoaderDumpEx ( iU16* path , void* pool , iU32 length ) {

	//	author : Jelo 
	//	since : 2011.6.2
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	#ifdef i51SIMULATOR
		FILE* file = fopen ( path , "wb+" ) ;
		fwrite ( pool , length , 1 , file ) ;
		fflush ( file ) ;
		fclose ( file ) ;
	#else

		iU32 written = 0 ;
		iFILE file = i51AdeOsFileOpen ( path , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;
		
		if ( !file ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : C DUMP F" ) ;		
			return ;
		} 

		i51AdeOsFileSetSeek ( file , 0 , i51_OS_FL_SEEK_HEAD ) ;
		i51AdeOsFileWrite ( file , pool , length , &written ) ;
		i51AdeOsFileFlush ( file ) ;
		i51AdeOsFileClose ( file ) ;
		
	#endif

}

#endif

static void i51SlLoaderSetCacheScope () {

	//	author : Jelo 
	//	since : 2011.11.29
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	static iU32 times = 0 ;
	
	i51SYMPOOL* looper = 0 ;

	i51SYMPOOL* start_node = 0 ;
	i51SYMPOOL* end_node = 0 ;
	
	iU32 start_address = 0 ;
	iU32 end_address = 0 ;
	
	if ( 0 != times ) {  
		i51AdeOsUnsetCPUCache () ;
	}  
	
	if ( 0 != i51SymbolPool.head ) {
		if ( i51_LD_MEM_MODE_DYNAMIC == i51SymbolPool.head->Binary.type ) {
			start_node = i51SymbolPool.head ;
			end_node = i51SymbolPool.head ;
		}
	}

 	for ( looper = i51SymbolPool.head ; looper ; looper = looper->next ) {

		if ( i51_LD_MEM_MODE_DYNAMIC == looper->Binary.type ) {

			if ( 0 == start_node ) {
				start_node = looper ;
			} else { 
				end_node = looper ;
			}
			
		}		
		
 	}

	if ( 0 == start_node || 0 == end_node ) 
		return ;
	 
	start_address = (iU32 )start_node->Binary.address ;
	end_address = (iU32 )end_node->Binary.address + end_node->Binary.length-1 ;

	i51AdeOsLog ( i51_LOG_SC , "LOD : Cache Start %x , End %x , Times %d" , start_address , end_address , times ) ;
	
	i51AdeOsSetCPUCache ( start_address , end_address ) ;
			
	times ++ ;

}

static void i51SlLoaderLoadSnapshot ( iU16* path , iU8* memory , iU32 length )
{

	//	author : Jelo 
	//	since : 2011.9.16
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iFILE file = 0 ;
	iU16 snappath [ 256 ]  = { 0 } ;
	iU16* ppath = 0 ;
	iBOOL ret = iFALSE ;
	iU32 looper = 0 ;
	iU32 plen = 0 ;

	iASSERT( 256 > i51AdeStdWStrlen ( path ) ) ;	
		
	ppath = i51AdeStdWStrcpy ( snappath , path ) ;

	memcpy ( (iU16*)((iU32)ppath+(i51AdeStdWStrlen ( ppath )<<1)-8) , L".snap" ,i51AdeStdWStrlen(L".snap")<<1 ) ;	
	
	i51FILEOPEN(file,ppath,i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN) ;
	
	if ( 0 == file ) {
		return ;
	}

	i51AdeOsLog ( i51_LOG_SC , "LOD : Load Snapshot" ) ;
		
	i51AdeOsFileRead ( file , memory , length , 0 ) ;

	i51AdeOsFileClose ( file ) ;
	
	return ;
	
}

static void i51Relocate ( 

	iU8* pool , 
	i51HEADER i51header , 
	i51OBJ* Obj , 
	iU32 objlen , 
	i51REL* Rel , 
	iU32 rellen , 
	i51RWE* Rwe , 
	iU32 rwelen , 	
	i51LDRELOC reloc ,
	iU32 border
	
) {

	//	author : Jelo 
	//	since : 2011.5.13
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	iU32 looper = 0 ;
	iU32 based_address = (iU32 ) pool ; 
	
	if ( i51_LD_RELOC_RO == reloc ) {

		for ( looper = 0 ; looper < rellen ; looper ++ ) {
			iU32 append = 0 ;
			iU32 address = 0 ;
			iU32 ObjIndex = 0 ;
			iU32 ObjOffset = 0 ;
			iU32 RelOffset = 0 ; 

			ObjIndex = Rel[looper].objindex ;
			ObjOffset = Obj[ObjIndex].offset ;
			RelOffset = Rel[looper].offset + ObjOffset ;

			#ifdef i51_LOADER_DBG
				i51AdeOsLog ( i51_LOG_SC , "i51_LD_RELOC_RO S" ) ;
				i51AdeOsLog ( i51_LOG_SC , "O %d , R off %x , A off %x" , ObjIndex , Rel[looper].offset , RelOffset ) ;
			#endif
			
			memcpy ( &append , (void*)((iU32)pool+RelOffset) , sizeof(iU32) ) ;
			address = append + based_address ;

			#ifdef i51_LOADER_DBG
				i51AdeOsLog ( i51_LOG_SC , "Rel A %x , A %x" , append , address ) ;
			#endif

			#ifdef i51KERNEL_DEBUG
			if ( ((iU32)pool+RelOffset) >= border ) {
				i51AdeOsLog ( i51_LOG_SC , "!!!Ro Reloc Overflow at %x" , pool+RelOffset ) ;	
				continue ;
			}
			#endif
			  
			memcpy ( (void*)((iU32)pool+RelOffset) , &address , sizeof(iU32) ) ;	

			#ifdef i51_LOADER_DBG
				i51AdeOsLog ( i51_LOG_SC , "i51_LD_RELOC_RO E" ) ;			
			#endif
			
		}

	} else if ( i51_LD_RELOC_RW == reloc ) {
		
	
		for ( looper = 0 ; looper < rellen ; looper ++ ) {	
		
			iU32 append = 0 ;
			iU32 address = 0 ;
			iU32 ObjIndex = 0 ;		
			iU32 RelOffset = 0 ; 

			ObjIndex = Rel[looper].objindex ;
			RelOffset = Rel[looper].offset + i51header.RoSize ;	
			if ( 0 < rwelen ) RelOffset = RelOffset+Rwe[ObjIndex].offset ;  

			#ifdef i51_LOADER_DBG			  
				i51AdeOsLog ( i51_LOG_SC , "i51_LD_RELOC_RW S" ) ;
				i51AdeOsLog ( i51_LOG_SC , "O %d , R off %x , A off %x" , Rel[looper].objindex , Rel[looper].offset , RelOffset ) ;		  
			#endif
			
			memcpy ( &append , (void*)((iU32)pool+RelOffset) , sizeof(iU32) ) ;
			address = append + based_address ;   

			#ifdef i51_LOADER_DBG			
				i51AdeOsLog ( i51_LOG_SC , "Rel A %x , A %x" , append , address ) ;	
			#endif

			#ifdef i51KERNEL_DEBUG
			if ( ((iU32)pool+RelOffset) >= border ) {
				i51AdeOsLog ( i51_LOG_SC , "!!!Rw Reloc Overflow at %x" , pool+RelOffset ) ;	
				continue ;
			}
			#endif  
			
			memcpy ( (void*)((iU32)pool+RelOffset) , &address , sizeof(iU32) ) ;	

			#ifdef i51_LOADER_DBG			
				i51AdeOsLog ( i51_LOG_SC , "i51_LD_RELOC_RW E" ) ;	
 			#endif
				 
		}
  
	}


}

static iS32 i51SlLoaderLoadFile ( iFILE file , iU32 bulen , i51SYMPOOL* i51SymPool ) {

	//	author : Jelo 
	//	since : 2011.6.2
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	load i51 OBJ from file
	
	iU32 readcount = 0 ;
	iBOOL boolean = 0 ;
	iS32 ret = 0 ;
	
	i51REL* Rel = 0 ;
	i51REL* RwRel = 0 ;
	i51RWE* Rwe = 0 ;
	i51OBJ* Obj = 0 ;	

	i51SYMBOL* i51Symbols = 0 ;
	i51HEADER i51header = { 0 } ;

	//	将OBJ、REL、RWREL装入该内存区域，以便减少内存碎片
	iU8* AssistMemory = 0 ;

	boolean = i51AdeOsFileRead ( file , &i51header , sizeof(i51HEADER) , &readcount ) ;

	if ( iFALSE == boolean ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : File Read F(1)" ) ;
		return 0 ;
	}
	
	#ifdef i51_LOADER_DBG
		i51AdeOsLog ( i51_LOG_SC , "Name 	%d" ,i51header.Name)  ;

		i51AdeOsLog ( i51_LOG_SC , "Bitmap 	%d" ,i51header.Bitmap)  ;
		i51AdeOsLog ( i51_LOG_SC , "Version 	%d" ,i51header.Version)  ;
		i51AdeOsLog ( i51_LOG_SC , "Endian 	%d" ,i51header.Endian)  ;
		i51AdeOsLog ( i51_LOG_SC , "Set 		%d" , i51header.Set) ;

		i51AdeOsLog ( i51_LOG_SC , "Kernel 		%d" , i51header.Kernel) ; 
		i51AdeOsLog ( i51_LOG_SC , "Adapter 		%d" , i51header.Adapter) ;
		i51AdeOsLog ( i51_LOG_SC , "Weight 		%d" , i51header.Weight) ;

		i51AdeOsLog ( i51_LOG_SC , "BinOffset 	%d" ,i51header.BinOffset)  ;
		i51AdeOsLog ( i51_LOG_SC , "BinSize 	%d" , i51header.BinSize) ;
		
		i51AdeOsLog ( i51_LOG_SC , "RoSize 	%d" ,i51header.RoSize)  ;
		i51AdeOsLog ( i51_LOG_SC , "ZiSize	%d" , i51header.ZiSize) ;

		i51AdeOsLog ( i51_LOG_SC , "SymbolTotall 	%d" ,i51header.SymbolTotall)  ;
		i51AdeOsLog ( i51_LOG_SC , "SymbolOffset 	%d" , i51header.SymbolOffset) ;
		i51AdeOsLog ( i51_LOG_SC , "RelTotall 		%d" , i51header.RelTotall ) ;
		i51AdeOsLog ( i51_LOG_SC , "RelOffset 		%d" , i51header.RelOffset) ;
		i51AdeOsLog ( i51_LOG_SC , "RwRelTotall 	%d" , i51header.RwRelTotall ) ;
		i51AdeOsLog ( i51_LOG_SC , "RwRelOffset 	%d" , i51header.RwRelOffset) ;
		i51AdeOsLog ( i51_LOG_SC , "RwEntryMapTotall 	%d" , i51header.RwEntryMapTotall ) ;
		i51AdeOsLog ( i51_LOG_SC , "RwEntryMapOffset 	%d" , i51header.RwEntryMapOffset) ;		
		i51AdeOsLog ( i51_LOG_SC , "ObjTotall 	%d" , i51header.ObjTotall) ;
		i51AdeOsLog ( i51_LOG_SC , "ObjOffset 	%d" , i51header.ObjOffset) ;
		i51AdeOsLog ( i51_LOG_SC , "Refoffset 	%d" , i51header.Refoffset) ;	
		i51AdeOsLog ( i51_LOG_SC , "Reftotall 	%d" , i51header.Reftotall) ;	
	#endif 

	if ( 'i' != i51header.Magic[0] || '5' != i51header.Magic[1] || '1' != i51header.Magic[2] ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : i51 MC F" ) ;
		return 0 ;
	}  
	
	//	alloc symbol tabl
	i51Symbols = (i51SYMBOL* ) i51MALLOC ( sizeof(i51SYMBOL) * i51header.SymbolTotall ) ;  
	if ( 0 == i51Symbols ) {

		i51AdeOsLog ( i51_LOG_SC , "LOD : Symbol MEM NULL" ) ;
		i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
		return 0 ;
		
	}
	//	finished here  

	if ( 0 != i51header.RelTotall || 0 != i51header.RwRelTotall || 0 != i51header.ObjTotall ) {

		AssistMemory = (iU8* ) i51MALLOC ( 
			(i51header.RelTotall*sizeof(i51REL)) + 
			(i51header.RwRelTotall*sizeof(i51REL)) + (i51header.ObjTotall*sizeof(i51OBJ)) ) ;

		if ( 0 == AssistMemory ) {
			
			i51AdeOsLog ( i51_LOG_SC , "LOD : AssistMemory MEM NULL" ) ;
			i51FREE ( i51Symbols ) ;	
			i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
			return 0 ;
			
		}

	}
	
	//	Read Rel Table
	if ( 0 < i51header.RelTotall ) {
		
		memset ( AssistMemory , 0 , i51header.RelTotall*sizeof(i51REL) ) ;
	
		boolean = i51AdeOsFileRead ( file , AssistMemory , i51header.RelTotall*sizeof(i51REL) , 0 ) ;
		if ( iFALSE == boolean ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : File Read F(5)" ) ;
			i51FREE ( AssistMemory ) ;
			i51FREE ( i51Symbols ) ;
			i51SlLoaderLastError = i51_LD_ERROR_FILE ;		
			return 0 ;
		}		
		Rel = (i51REL* ) AssistMemory ;
		
	}
	//	Read Rel Table Finished Here
	
	//	Read RwRel Table
	if ( 0 < i51header.RwRelTotall ) {

		boolean = i51AdeOsFileRead ( 
			file , 
			(void*)((iU32)AssistMemory+i51header.RelTotall*sizeof(i51REL)) , 
			i51header.RwRelTotall*sizeof(i51REL) , 0 ) ;
		if ( iFALSE == boolean ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : File Read F(7)" ) ;
			i51FREE ( AssistMemory ) ;
			i51FREE ( i51Symbols ) ;
			i51SlLoaderLastError = i51_LD_ERROR_FILE ;		
			return 0 ;  
		}

		RwRel = (i51REL* ) ((iU32)AssistMemory+i51header.RelTotall*sizeof(i51REL)) ;

	}
	//	Read RwRel Table Finished Here	

	//	Read Rwe
	if ( 0 < i51header.RwEntryMapTotall ) {
		
		Rwe = (i51RWE* ) i51MALLOC ( i51header.RwEntryMapTotall*sizeof(i51RWE) ) ;
		
		if ( 0 == Rwe ) {
			
			i51AdeOsLog ( i51_LOG_SC , "LOD : RWE MEM NULL" ) ;
				
			i51FREE ( i51Symbols ) ;	
			i51FREE ( AssistMemory ) ;
			i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
			
			return 0 ;
			
		}

		boolean = i51AdeOsFileRead ( 
			file , 
			Rwe , 
			i51header.RwEntryMapTotall*sizeof(i51RWE) , 0 ) ;
		if ( iFALSE == boolean ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : File Read F(11)" ) ;
			i51FREE ( AssistMemory ) ;
			i51FREE ( i51Symbols ) ;
			i51SlLoaderLastError = i51_LD_ERROR_FILE ;		
			return 0 ;
		}
		
	}
	//	Read Rwe Finished Here			

	//	Read Obj Table
	{

		boolean = i51AdeOsFileRead ( 
			file , 
			(void*)((iU32)AssistMemory+i51header.RelTotall*sizeof(i51REL) + i51header.RwRelTotall*sizeof(i51REL)) , 
			i51header.ObjTotall*sizeof(i51OBJ) , 0 ) ;
		if ( iFALSE == boolean ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : File Read F(9)" ) ;
			i51FREE ( AssistMemory ) ;
			i51FREE ( i51Symbols ) ;
			i51SlLoaderLastError = i51_LD_ERROR_FILE ;		
			return 0 ;
		}    
			
		Obj = (i51OBJ* )((iU32)AssistMemory+i51header.RelTotall*sizeof(i51REL) + i51header.RwRelTotall*sizeof(i51REL)) ;
	}
	//	Read Obj Table Finished Here	

	//	Skip String Table
	
//	iCHAR* st = (iCHAR* ) i51AdeOsMallocEx ( i51header.Reftotall ) ;
//	i51AdeOsFileRead ( file , st , i51header.Reftotall , 0 ) ;

//	i51AdeOsLog ( 0 , "st:%s" , st ) ;

//	i51FREE(st) ;
	
	boolean = i51AdeOsFileSetSeek ( file , i51header.Reftotall*sizeof(i51SllDKITREF) , i51_OS_FL_SEEK_CURRENT ) ;
	if ( iFALSE == boolean ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : File Read F(3.0)" ) ;
		i51FREE ( Rwe ) ;
		i51FREE ( AssistMemory ) ;
		i51FREE ( i51Symbols ) ;
		i51SlLoaderLastError = i51_LD_ERROR_FILE ;		
		return 0 ;
	}	

	boolean = i51AdeOsFileRead ( file , i51SymPool->Binary.address , i51header.BinSize , 0 ) ;
	if ( iFALSE == boolean ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : File Read F(3)" ) ;
		i51FREE ( Rwe ) ;
		i51FREE ( AssistMemory ) ;
		i51FREE ( i51Symbols ) ;
		i51SlLoaderLastError = i51_LD_ERROR_FILE ;		
		return 0 ;
	}	

	//	Read Bin Image Finished Here

	//	Read Symbols
	{

		boolean = i51AdeOsFileRead ( 
			file , 
			i51Symbols , 
			i51header.SymbolTotall*sizeof(i51SYMBOL) , 0 ) ;
		if ( iFALSE == boolean ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : File Read F(11)" ) ;
			i51FREE ( Rwe ) ;
			i51FREE ( AssistMemory ) ;
			i51FREE ( i51Symbols ) ;
			i51SlLoaderLastError = i51_LD_ERROR_FILE ;		
			return 0 ;
		}
	

	}
	//	Read Symbols Finished Here	
  
	#ifdef i51_LOADER_DBG 
	{ 
		int looper = 0 ;
		
		for ( looper = 0 ; looper < i51header.RelTotall; looper ++ ) {
			
			//i51AdeOsLog ( i51_LOG_SC , "Rel[%d].objindex %d",looper,Rel[looper].objindex );
			//i51AdeOsLog ( i51_LOG_SC , "Rel[%d].offset %d",looper,Rel[looper].offset );
		}

		for ( looper = 0 ; looper < i51header.RwRelTotall; looper ++ ) {
			
			//i51AdeOsLog ( i51_LOG_SC , "RwRel[%d].objindex %d",looper,RwRel[looper].objindex );
			//i51AdeOsLog ( i51_LOG_SC , "RwRel[%d].offset %d",looper,RwRel[looper].offset );
		}
	}
	#endif  

	#ifdef i51_LOADER_DBG	 
	{
		int looper = 0 ;
		
		for ( looper = 0 ; looper < i51header.ObjTotall; looper ++ ) {
			
			//i51AdeOsLog ( i51_LOG_SC , "Obj[%d].offset %d" , looper,Obj[looper].offset) ;
			//i51AdeOsLog ( i51_LOG_SC , "Obj[%d].rwsize %d" ,looper,Obj[looper].rwsize ) ;
			
		}

		for ( looper = 0 ; looper < i51header.RwEntryMapTotall; looper ++ ) {
			
			//i51AdeOsLog ( i51_LOG_SC , "Rwe[%d].offset %d" , looper,Rwe[looper].offset) ;			
			
		}		
		 
	}
	#endif   

	//	Relocate RO	
	if ( 0 != Rel ) {

		i51Relocate ( 
			i51SymPool->Binary.address , 
			i51header , 
			Obj , 
			i51header.ObjTotall , 
			Rel , 
			i51header.RelTotall , 
			0 , 
			0 , 
			i51_LD_RELOC_RO ,
			(iU32)i51SymPool->Binary.address+i51header.BinSize+i51header.ZiSize
		) ; 

	}
	//	Relocate RW
	if ( 0 != RwRel ) {
		
		i51Relocate ( 
			i51SymPool->Binary.address , 
			i51header , 
			Obj , 
			i51header.ObjTotall , 
			RwRel , 
			i51header.RwRelTotall ,  
			Rwe , 
			i51header.RwEntryMapTotall , 
			i51_LD_RELOC_RW ,
			(iU32)i51SymPool->Binary.address+i51header.BinSize+i51header.ZiSize
		) ;
		
	}

	#ifdef i51_LOADER_DBG
		i51SlLoaderDump ( i51SymPool->name , i51SymPool->Binary.address , i51header.BinSize+i51header.ZiSize ) ;
	#endif  
	
	if ( 0 != Rwe ) i51FREE ( Rwe ) ;
	i51FREE ( AssistMemory ) ;
 
	//	Set Symbol Pool
	i51SymPool->handle = (iS32 )i51SymPool->Binary.address;
	i51SymPool->set = i51header.Set ;
	i51SymPool->SymbolTotall = i51header.SymbolTotall ;
	i51SymPool->symbols = i51Symbols ; 	
 
	i51AdeOsLog ( i51_LOG_SC , "LOD : Load i51 OBJ , Done , Handle %x" , i51SymPool ) ; 
	 
	return (iS32) i51SymPool ;	  
	
}


static iS32 i51SlLoaderLoadImage ( iU8* data , iU32 objlength , i51SYMPOOL* i51SymPool ) {

	//	author : Jelo 
	//	since : 2011.11.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	load i51 OBJ from Image

	iU32 readcount = 0 ;
	iBOOL boolean = 0 ;
	iS32 ret = 0 ;
	iU8* bufferwalker = data ;
	
	i51REL* Rel = 0 ;
	i51REL* RwRel = 0 ;
	i51RWE* Rwe = 0 ;
	i51OBJ* Obj = 0 ;	

	i51SYMBOL* i51Symbols = 0 ;
	i51HEADER i51header = { 0 } ;

	//	将OBJ、REL、RWREL装入该内存区域，以便减少内存碎片
	iU8* AssistMemory = 0 ;

	memcpy ( &i51header , data , sizeof(i51HEADER) ) ;
	bufferwalker = bufferwalker + sizeof(i51HEADER) ;
	
	#ifdef i51_LOADER_DBG
		i51AdeOsLog ( i51_LOG_SC , "Name 	%d" ,i51header.Name)  ;

		i51AdeOsLog ( i51_LOG_SC , "Bitmap 	%d" ,i51header.Bitmap)  ;
		i51AdeOsLog ( i51_LOG_SC , "Version 	%d" ,i51header.Version)  ;
		i51AdeOsLog ( i51_LOG_SC , "Endian 	%d" ,i51header.Endian)  ;
		i51AdeOsLog ( i51_LOG_SC , "Set 		%d" , i51header.Set) ;

		i51AdeOsLog ( i51_LOG_SC , "Kernel 		%d" , i51header.Kernel) ; 
		i51AdeOsLog ( i51_LOG_SC , "Adapter 		%d" , i51header.Adapter) ;
		i51AdeOsLog ( i51_LOG_SC , "Weight 		%d" , i51header.Weight) ;

		i51AdeOsLog ( i51_LOG_SC , "BinOffset 	%d" ,i51header.BinOffset)  ;
		i51AdeOsLog ( i51_LOG_SC , "BinSize 	%d" , i51header.BinSize) ;
		
		i51AdeOsLog ( i51_LOG_SC , "RoSize 	%d" ,i51header.RoSize)  ;
		i51AdeOsLog ( i51_LOG_SC , "ZiSize	%d" , i51header.ZiSize) ;

		i51AdeOsLog ( i51_LOG_SC , "SymbolTotall 	%d" ,i51header.SymbolTotall)  ;
		i51AdeOsLog ( i51_LOG_SC , "SymbolOffset 	%d" , i51header.SymbolOffset) ;
		i51AdeOsLog ( i51_LOG_SC , "RelTotall 		%d" , i51header.RelTotall ) ;
		i51AdeOsLog ( i51_LOG_SC , "RelOffset 		%d" , i51header.RelOffset) ;
		i51AdeOsLog ( i51_LOG_SC , "RwRelTotall 	%d" , i51header.RwRelTotall ) ;
		i51AdeOsLog ( i51_LOG_SC , "RwRelOffset 	%d" , i51header.RwRelOffset) ;
		i51AdeOsLog ( i51_LOG_SC , "RwEntryMapTotall 	%d" , i51header.RwEntryMapTotall ) ;
		i51AdeOsLog ( i51_LOG_SC , "RwEntryMapOffset 	%d" , i51header.RwEntryMapOffset) ;		
		i51AdeOsLog ( i51_LOG_SC , "ObjTotall 	%d" , i51header.ObjTotall) ;
		i51AdeOsLog ( i51_LOG_SC , "ObjOffset 	%d" , i51header.ObjOffset) ;
		i51AdeOsLog ( i51_LOG_SC , "Refoffset 	%d" , i51header.Refoffset) ;	
		i51AdeOsLog ( i51_LOG_SC , "Reftotall 	%d" , i51header.Reftotall) ;	
	#endif 

	if ( 'i' != i51header.Magic[0] || '5' != i51header.Magic[1] || '1' != i51header.Magic[2] ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : i51 MC F" ) ;
		return 0 ;
	}  
	
	//	alloc symbol tabl
	i51Symbols = (i51SYMBOL* ) i51MALLOC ( sizeof(i51SYMBOL) * i51header.SymbolTotall ) ;  
	if ( 0 == i51Symbols ) {

		i51AdeOsLog ( i51_LOG_SC , "LOD : Symbol MEM NULL" ) ;
		i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
		return 0 ;
		
	}
	//	finished here  

	if ( 0 != i51header.RelTotall || 0 != i51header.RwRelTotall || 0 != i51header.ObjTotall ) {

		AssistMemory = (iU8* ) i51MALLOC ( 
			(i51header.RelTotall*sizeof(i51REL)) + 
			(i51header.RwRelTotall*sizeof(i51REL)) + (i51header.ObjTotall*sizeof(i51OBJ)) ) ;

		if ( 0 == AssistMemory ) {
			
			i51AdeOsLog ( i51_LOG_SC , "LOD : AssistMemory MEM NULL" ) ;
			i51FREE ( i51Symbols ) ;	
			i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
			return 0 ;
			
		}

	}
	
	//	Read Rel Table
	if ( 0 < i51header.RelTotall ) {
		
		memset ( AssistMemory , 0 , i51header.RelTotall*sizeof(i51REL) ) ;
		memcpy ( AssistMemory , bufferwalker , i51header.RelTotall*sizeof(i51REL) ) ;
		Rel = (i51REL* ) AssistMemory ;
		//	next
		bufferwalker = bufferwalker + i51header.RelTotall*sizeof(i51REL) ; 
		
	}
	//	Read Rel Table Finished Here
	
	//	Read RwRel Table
	if ( 0 < i51header.RwRelTotall ) {

		memcpy ( 
			(void*)((iU32)AssistMemory+i51header.RelTotall*sizeof(i51REL)) , bufferwalker ,
			i51header.RwRelTotall*sizeof(i51REL) ) ;
		RwRel = (i51REL* ) ((iU32)AssistMemory+i51header.RelTotall*sizeof(i51REL)) ;
		//	next
		bufferwalker = bufferwalker + i51header.RwRelTotall*sizeof(i51REL) ; 

	}
	//	Read RwRel Table Finished Here	

	//	Read Rwe
	if ( 0 < i51header.RwEntryMapTotall ) {
		
		Rwe = (i51RWE* ) i51MALLOC ( i51header.RwEntryMapTotall*sizeof(i51RWE) ) ;
		
		if ( 0 == Rwe ) {
			
			i51AdeOsLog ( i51_LOG_SC , "LOD : RWE MEM NULL" ) ;
				
			i51FREE ( i51Symbols ) ;	
			i51FREE ( AssistMemory ) ;
			i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
			
			return 0 ;
			
		}

		memcpy ( 
			Rwe , 
			bufferwalker ,
			i51header.RwEntryMapTotall*sizeof(i51RWE) ) ;

		//	next
		bufferwalker = bufferwalker + i51header.RwEntryMapTotall*sizeof(i51RWE) ; 
		
	}
	//	Read Rwe Finished Here			

	//	Read Obj Table
	{

		memcpy ( 
			(void*)((iU32)AssistMemory+i51header.RelTotall*sizeof(i51REL) + i51header.RwRelTotall*sizeof(i51REL)) , 
			bufferwalker ,
			i51header.ObjTotall*sizeof(i51OBJ) ) ;
					
		Obj = (i51OBJ* )((iU32)AssistMemory+i51header.RelTotall*sizeof(i51REL) + i51header.RwRelTotall*sizeof(i51REL)) ;
		//	next
		bufferwalker = bufferwalker + i51header.ObjTotall*sizeof(i51OBJ) ;
	}
	//	Read Obj Table Finished Here	

	//	Skip String Table

	bufferwalker = bufferwalker + i51header.Reftotall*sizeof(i51SllDKITREF);
	memcpy ( i51SymPool->Binary.address , bufferwalker , i51header.BinSize ) ;
	bufferwalker = bufferwalker + i51header.BinSize ;
	//	Read Bin Image Finished Here

	//	Read Symbols
	memcpy ( i51Symbols , bufferwalker , i51header.SymbolTotall*sizeof(i51SYMBOL) ) ;	
	//	Read Symbols Finished Here	
  
	#ifdef i51_LOADER_DBG 
	{ 
		int looper = 0 ;
		
		for ( looper = 0 ; looper < i51header.RelTotall; looper ++ ) {
			
			//i51AdeOsLog ( i51_LOG_SC , "Rel[%d].objindex %d",looper,Rel[looper].objindex );
			//i51AdeOsLog ( i51_LOG_SC , "Rel[%d].offset %d",looper,Rel[looper].offset );
		}

		for ( looper = 0 ; looper < i51header.RwRelTotall; looper ++ ) {
			
			//i51AdeOsLog ( i51_LOG_SC , "RwRel[%d].objindex %d",looper,RwRel[looper].objindex );
			//i51AdeOsLog ( i51_LOG_SC , "RwRel[%d].offset %d",looper,RwRel[looper].offset );
		}
	}
	#endif  

	#ifdef i51_LOADER_DBG	 
	{
		int looper = 0 ;
		
		for ( looper = 0 ; looper < i51header.ObjTotall; looper ++ ) {
			
			//i51AdeOsLog ( i51_LOG_SC , "Obj[%d].offset %d" , looper,Obj[looper].offset) ;
			//i51AdeOsLog ( i51_LOG_SC , "Obj[%d].rwsize %d" ,looper,Obj[looper].rwsize ) ;
			
		}

		for ( looper = 0 ; looper < i51header.RwEntryMapTotall; looper ++ ) {
			
			//i51AdeOsLog ( i51_LOG_SC , "Rwe[%d].offset %d" , looper,Rwe[looper].offset) ;			
			
		}		
		 
	}
	#endif   

	#ifdef i51_LOADER_DBG
		i51SlLoaderB4RelDump ( i51SymPool->name , i51SymPool->Binary.address , i51header.BinSize+i51header.ZiSize ) ;
	#endif

	//	Relocate RO	
	if ( 0 != Rel ) {

		i51Relocate ( 
			i51SymPool->Binary.address , 
			i51header , 
			Obj , 
			i51header.ObjTotall , 
			Rel , 
			i51header.RelTotall , 
			0 , 
			0 , 
			i51_LD_RELOC_RO ,
			(iU32)i51SymPool->Binary.address+i51header.BinSize+i51header.ZiSize
		) ; 

	}
	//	Relocate RW
	if ( 0 != RwRel ) {
		
		i51Relocate ( 
			i51SymPool->Binary.address , 
			i51header , 
			Obj , 
			i51header.ObjTotall , 
			RwRel , 
			i51header.RwRelTotall ,  
			Rwe , 
			i51header.RwEntryMapTotall , 
			i51_LD_RELOC_RW ,
			(iU32)i51SymPool->Binary.address+i51header.BinSize+i51header.ZiSize
		) ;
		
	}

	#ifdef i51_LOADER_DBG
		i51SlLoaderDump ( i51SymPool->name , i51SymPool->Binary.address , i51header.BinSize+i51header.ZiSize ) ;
	#endif
	
	if ( 0 != Rwe ) i51FREE ( Rwe ) ;
	i51FREE ( AssistMemory ) ;
 
	//	Set Symbol Pool
	i51SymPool->handle = (iS32 )i51SymPool->Binary.address;
	i51SymPool->set = i51header.Set ;
	i51SymPool->SymbolTotall = i51header.SymbolTotall ;
	i51SymPool->symbols = i51Symbols ; 	
 
	i51AdeOsLog ( i51_LOG_SC , "LOD : Load i51 OBJ , Done , Handle %x" , i51SymPool ) ; 
	 
	return (iS32) i51SymPool ;	  
	

} 


static i51SYMPOOL* i51SlLoaderLoadFromFile ( 

	iU16* name ,
	iU16* path ,
	iU8* BinaryBuffer , 
	iU32 BinaryBufferlen 
	
) {


	//	author : Jelo 
	//	since : 2011.6.2
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	the length of orginal datas that without compress
	iU32 crc = 0 ;
	iS32 seek = 0 ;
	iU8* i51buffer = 0 ; 
	iU8* i51Binary = BinaryBuffer ;
	iU32 ret = iFALSE ;
	iFILE file = 0 ;
	iS32 filelength = 0 ;
	iU32 kelversion = 0 ;
	iU32 adaversion = 0 ;
	iU32 endian = 0 ;	
	
	i51HEADER i51Header = { 0 } ;
	i51SYMPOOL* i51SymPool = 0 ;
 
	i51SymPool = (i51SYMPOOL* ) i51MALLOC ( sizeof(i51SYMPOOL) ) ;

	if ( 0 == i51SymPool ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Mem F" ) ;
		return 0 ;		
	}

	memset ( i51SymPool , 0 , sizeof(i51SYMPOOL) ) ;
	
	i51AdeStdWStrcpy ( i51SymPool->name , name ) ;
	i51FILEOPEN(file,path,i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN) ;

	if ( 0 == file ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Kit Not Found" ) ;
		i51FREE(i51SymPool) ;
		return 0 ;
	}

	//i51AdeOsFileGetSeek ( file , &seek , i51_OS_FL_SEEK_HEAD ) ;
	ret = i51AdeOsFileGetSize ( file , &filelength ) ;
	
	if ( iFALSE == ret ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Kit Get Length F" ) ;
		i51AdeOsFileClose ( file ) ;
		i51FREE(i51SymPool) ;
		return 0 ;			
	}

	//	检测动态库版本是否支持
	if ( iFALSE == i51SlLoaderLoadable ( file ) ) {		
		i51AdeOsFileClose ( file ) ;
		i51FREE(i51SymPool) ;		
		return iFALSE ; 
	}

	//	 检查Kernel版本、Adapter版本、大小端
	i51SlLoaderGetAttr ( file , 0 , 0 , &kelversion , &adaversion , &endian ) ; 
	
	//	kernel 版本检测
	if ( kelversion > i51KernelGetVersion () ) {
		i51AdeOsFileClose ( file ) ;
		i51FREE(i51SymPool) ;		
		i51AdeOsLog ( i51_LOG_SC , "LOD : KEL %d UM" , kelversion ) ;
		return iFALSE ;
	}
 
	//	adapter 版本检测
	if ( adaversion > i51AdeCnfGetVersion () ) {		
		i51AdeOsFileClose ( file ) ;
		i51FREE(i51SymPool) ;		
		i51AdeOsLog ( i51_LOG_SC , "LOD : ADE %d UM" , adaversion ) ;
		return iFALSE ;
	}	

	//	大小端检测
	if ( endian != i51AdeCnfGetEndian () ) {
		i51AdeOsFileClose ( file ) ;
		i51FREE(i51SymPool) ;		
		i51AdeOsLog ( i51_LOG_SC , "LOD : Endian %d UM" , endian ) ;
		return iFALSE ;
	}	
			
	i51AdeOsFileSetSeek ( file , 60 , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileRead ( file , &i51Header.BinSize , sizeof(i51Header.BinSize) , 0 ) ;
	i51AdeOsFileRead ( file , &i51Header.RoSize , sizeof(i51Header.RoSize) , 0 ) ;
	i51AdeOsFileRead ( file , &i51Header.ZiSize , sizeof(i51Header.ZiSize) , 0 ) ;
	
	#ifdef i51_LOADER_DBG
		i51AdeOsLog ( i51_LOG_SC , "i51Header.BinSize %d" , i51Header.BinSize ) ;
		i51AdeOsLog ( i51_LOG_SC , "i51Header.RoSize %d" , i51Header.RoSize ) ;   
		i51AdeOsLog ( i51_LOG_SC , "i51Header.ZiSize %d" , i51Header.ZiSize ) ;
	#endif
	
	if ( 0 == i51Binary ) { 

		i51Binary = (iU8* ) i51MALLOC ( i51Header.BinSize + i51Header.ZiSize ) ; 
		//	for testing
		//i51Binary = (iU8* ) i51MALLOC ( i51Header.BinSize + i51Header.ZiSize+300*1024 ) ;   
		if ( 0 == i51Binary ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : MEM ALLOC F" ) ;
			i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
			i51AdeOsFileClose ( file ) ;
			i51FREE(i51SymPool) ;			
			return 0 ;
		}

		i51SymPool->Binary.type = i51_LD_MEM_MODE_DYNAMIC ;
		i51SymPool->Binary.address = i51Binary ;
		i51SymPool->Binary.length = i51Header.BinSize + i51Header.ZiSize ;

		
	} else { 

		i51SymPool->Binary.type = i51_LD_MEM_MODE_STATIC ;
		i51SymPool->Binary.address = BinaryBuffer ; 
		i51SymPool->Binary.length = BinaryBufferlen ;
		
	}

	memset ( i51SymPool->Binary.address , 0 , i51SymPool->Binary.length ) ;
	
	i51SymPool->Binary.rwoffset = (iU32)i51Header.RoSize ;
	i51SymPool->Binary.rosize = i51Header.RoSize ;	   
	
//	i51AdeOsSetCPUCache ( (iU32)i51SymPool->Binary.address , (iU32)i51SymPool->Binary.address+i51SymPool->Binary.length ) ;

	#ifdef i51_LOADER_DBG	
		i51AdeOsLog ( 0 , "i51SymPool->Binary.address %d" , i51SymPool->Binary.address ) ;
		i51AdeOsLog ( 0 , "i51Header.RoSize %d" , i51Header.RoSize ) ;
		i51AdeOsLog ( 0 , "i51SymPool->Binary.rosize %d" , i51SymPool->Binary.rosize ) ;
		i51AdeOsLog ( 0 , "i51SymPool->Binary.rwoffset %d" , i51SymPool->Binary.rwoffset ) ;
	#endif	
	
	i51buffer = (iU8* ) i51MALLOC ( filelength-sizeof(crc) ) ;
	if ( 0 == i51buffer ) {
		
		i51AdeOsLog ( i51_LOG_SC , "LOD : MEM ALLOC(2) F" ) ;

		if ( i51_LD_MEM_MODE_DYNAMIC  == i51SymPool->Binary.type ) {
			i51FREE ( i51SymPool->Binary.address ) ;
		}
		i51FREE ( i51SymPool ) ;
		i51AdeOsFileClose ( file ) ;	
		i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
		return 0 ;
	}
  
	i51AdeOsFileSetSeek ( file , seek , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileRead ( file , &crc , sizeof(crc) , 0 ) ;

	i51AdeOsFileRead ( file , i51buffer , filelength-sizeof(crc) , 0 ) ;

	//	Jelo Committed 2012-6-11
	#if 0
	if ( crc != i51AdeStdCrc32 ( i51buffer , filelength-sizeof(crc) , 0 ) ) {
		
		i51FREE ( i51buffer ) ;

		if ( i51_LD_MEM_MODE_DYNAMIC  == i51SymPool->Binary.type ) {
			i51FREE ( i51SymPool->Binary.address ) ;
		}
		i51FREE ( i51SymPool ) ;

		i51AdeOsFileClose ( file ) ;			
		
		i51AdeOsLog ( i51_LOG_SC , "LOD : FILE is Crashed , CRC F" ) ;
		i51SlLoaderLastError = i51_LD_ERROR_CRC ;		

		i51AdeOsFileAModeEnable () ; 
		i51AdeOsFileDelete ( path ) ;
		i51AdeOsFileAModeDisable () ; 
		
		return 0 ;
		
	}
	#endif
	//	Jelo Committed Finished

	i51FREE ( i51buffer ) ;

	ret = i51AdeOsFileSetSeek ( file , seek+sizeof(crc) , i51_OS_FL_SEEK_HEAD ) ;

	if ( iFALSE == ret ) {
		
		if ( i51_LD_MEM_MODE_DYNAMIC  == i51SymPool->Binary.type ) {
			i51FREE ( i51SymPool->Binary.address ) ;
		}
		i51FREE ( i51SymPool ) ;

		i51AdeOsFileClose ( file ) ;	
		
		i51SymPool = 0 ;
		
		return 0 ;
		
	}
	
	i51SlLoaderLoadSnapshot ( 
		path , 
		(iU8*)( (iU32)i51SymPool->Binary.address+i51Header.RoSize) 
		//	RW+ZI
		, i51Header.BinSize - i51Header.RoSize + i51Header.ZiSize ) ;

	ret = i51SlLoaderLoadFile ( file , filelength-sizeof(crc) , i51SymPool ) ;

	if ( iFALSE == ret ) {
		
		if ( i51_LD_MEM_MODE_DYNAMIC  == i51SymPool->Binary.type ) {
			i51FREE ( i51SymPool->Binary.address ) ;
		}
		i51FREE ( i51SymPool ) ;
		
		i51AdeOsFileClose ( file ) ;	
		
		i51SymPool = 0 ;
	}

	i51AdeOsFileClose ( file ) ;	
		
	return i51SymPool ;

}

static i51SYMPOOL* i51SlLoaderLoadFromImage ( 

	iU16* name ,
	iU8* data ,
	iU32 datasize ,
	iU8* BinaryBuffer , 
	iU32 BinaryBufferlen 
	
) {


	//	author : Jelo 
	//	since : 2011.11.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	the length of orginal datas that without compress
	//	modified from i51SlLoaderLoadFromFile
	
	iU32 crc = 0 ;
	iS32 seek = 0 ;
	iU8* i51buffer = 0 ; 
	iU8* i51Binary = BinaryBuffer ;
	iU32 ret = iFALSE ;
	iFILE file = 0 ;
	iS32 filelength = datasize ;	
	iU8* bufferwalker = data ;
	
	i51HEADER i51Header = { 0 } ;
	i51SYMPOOL* i51SymPool = 0 ;

	i51SymPool = (i51SYMPOOL* ) i51MALLOC ( sizeof(i51SYMPOOL) ) ;

	if ( 0 == i51SymPool ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Mem F" ) ;
		return 0 ;		
	}

	memset ( i51SymPool , 0 , sizeof(i51SYMPOOL) ) ;
	
	i51AdeStdWStrcpy ( i51SymPool->name , name ) ;
		
	bufferwalker = bufferwalker + 60 ;

	memcpy ( &i51Header.BinSize , bufferwalker , sizeof(i51Header.BinSize) ) ;
	bufferwalker = bufferwalker + sizeof(i51Header.BinSize) ;
	
	memcpy ( &i51Header.RoSize , bufferwalker , sizeof(i51Header.RoSize) ) ;
	bufferwalker = bufferwalker + sizeof(i51Header.RoSize) ;

	memcpy ( &i51Header.ZiSize , bufferwalker , sizeof(i51Header.ZiSize) ) ;
	bufferwalker = bufferwalker + sizeof(i51Header.ZiSize) ;

	#ifdef i51_LOADER_DBG
		i51AdeOsLog ( i51_LOG_SC , "i51Header.BinSize %d" , i51Header.BinSize ) ;
		i51AdeOsLog ( i51_LOG_SC , "i51Header.RoSize %d" , i51Header.RoSize ) ;   
		i51AdeOsLog ( i51_LOG_SC , "i51Header.ZiSize %d" , i51Header.ZiSize ) ;
	#endif
	
	if ( 0 == i51Binary ) { 

		i51Binary = (iU8* ) i51MALLOC ( i51Header.BinSize + i51Header.ZiSize ) ; 
		//	for testing
		//i51Binary = (iU8* ) i51MALLOC ( i51Header.BinSize + i51Header.ZiSize+300*1024 ) ;   		
		if ( 0 == i51Binary ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : MEM ALLOC F" ) ;
			i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
			i51FREE(i51SymPool) ;			
			return 0 ;
		}

		i51SymPool->Binary.type = i51_LD_MEM_MODE_DYNAMIC ;
		i51SymPool->Binary.address = i51Binary ;
		i51SymPool->Binary.length = i51Header.BinSize + i51Header.ZiSize ;

		
	} else { 

		i51SymPool->Binary.type = i51_LD_MEM_MODE_STATIC ;
		i51SymPool->Binary.address = BinaryBuffer ;
		i51SymPool->Binary.length = BinaryBufferlen ;
		
	}

	memset ( i51SymPool->Binary.address , 0 , i51SymPool->Binary.length ) ;
	
	i51SymPool->Binary.rwoffset = (iU32)i51Header.RoSize ;
	i51SymPool->Binary.rosize = i51Header.RoSize ;	   
	
//	i51AdeOsSetCPUCache ( (iU32)i51SymPool->Binary.address , (iU32)i51SymPool->Binary.address+i51SymPool->Binary.length ) ;

	#ifdef i51_LOADER_DBG	
		i51AdeOsLog ( 0 , "i51SymPool->Binary.address %d" , i51SymPool->Binary.address ) ;
		i51AdeOsLog ( 0 , "i51Header.RoSize %d" , i51Header.RoSize ) ;
		i51AdeOsLog ( 0 , "i51SymPool->Binary.rosize %d" , i51SymPool->Binary.rosize ) ;
		i51AdeOsLog ( 0 , "i51SymPool->Binary.rwoffset %d" , i51SymPool->Binary.rwoffset ) ;
	#endif	
	
	i51buffer = (iU8* ) i51MALLOC ( filelength-sizeof(crc) ) ;
	
	if ( 0 == i51buffer ) {
		
		i51AdeOsLog ( i51_LOG_SC , "LOD : MEM ALLOC(2) F" ) ;

		if ( i51_LD_MEM_MODE_DYNAMIC  == i51SymPool->Binary.type ) {
			i51FREE ( i51SymPool->Binary.address ) ;
		}
		i51FREE ( i51SymPool ) ;
		i51SlLoaderLastError = i51_LD_ERROR_MEMORY ;
		return 0 ;
	}

	memcpy ( &crc , data , sizeof(crc) ) ;
	memcpy ( i51buffer , (void*)(data+sizeof(crc)) , filelength-sizeof(crc) ) ;

	//	Jelo Committed 2012-6-11
	#if 0
	if ( crc != i51AdeStdCrc32 ( i51buffer , filelength-sizeof(crc) , 0 ) ) {
		
		i51FREE ( i51buffer ) ;

		if ( i51_LD_MEM_MODE_DYNAMIC  == i51SymPool->Binary.type ) {
			i51FREE ( i51SymPool->Binary.address ) ;
		}
		
		i51FREE ( i51SymPool ) ;		
		i51AdeOsLog ( i51_LOG_SC , "LOD : Image is Crashed , CRC F" ) ;
		i51SlLoaderLastError = i51_LD_ERROR_CRC ;		
		return 0 ;
		
	}
	#endif
	//	Jelo Committed Finished
	
	i51FREE ( i51buffer ) ;

	ret = i51SlLoaderLoadImage ( (iU8*)(data+sizeof(crc)) , filelength-sizeof(crc) , i51SymPool ) ;

	if ( iFALSE == ret ) {
		
		if ( i51_LD_MEM_MODE_DYNAMIC  == i51SymPool->Binary.type ) {
			i51FREE ( i51SymPool->Binary.address ) ;
		}
		i51FREE ( i51SymPool ) ;
				
		i51SymPool = 0 ;
	}
		
	return i51SymPool ;

}

#endif

iS32 i51SlLoaderLoad ( iU16* name , iU16* path , iU8* BinaryBuffer , iU32 BinaryBufferlen )
{

	//	author : Jelo 
	//	since : 2011.5.13
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes  : 成功返回> 0 ，失败返回0
	//	当BinaryBufferlen > 0 时，将动态库加载到BinaryBufferlen

	//	加载文件，BinaryBuffer = 0 时内部分配内存。

	i51SYMPOOL* i51SymPoolNew = 0 ;
	
#ifndef i51_LOADER_LOADFAKE

	iBOOL ret = 0 ;
	iFILE file = 0 ;
	iS32 filelength = 0 ;
	iS32 handle = 0 ;	
	
	if ( 0 == name || 0 == path ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Para Null" ) ;
		return 0 ;
	}

	#ifdef i51SIMULATOR
	//#if 0
	{	
			
		i51SymPoolNew = (i51SYMPOOL* ) i51MALLOC ( sizeof(i51SYMPOOL) ) ;

		if ( 0 == i51SymPoolNew ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : Mem F" ) ;
			return 0 ;		
		}

		i51AdeStdMemset16 ( i51SymPoolNew , 0 , sizeof(i51SYMPOOL) ) ; 
		i51AdeStdWStrcpy ( i51SymPoolNew->name , name ) ;

		if ( 0 == BinaryBuffer ) {
			i51SymPoolNew->Binary.type = i51_LD_MEM_MODE_DYNAMIC ;
		} else {
			i51SymPoolNew->Binary.type = i51_LD_MEM_MODE_STATIC ;
		}
		
		i51SymPoolNew->handle = LoadLibraryW ( path ) ;

		if ( 0 == i51SymPoolNew->handle ) {
			i51FREE ( i51SymPoolNew ) ;
			return 0 ;
		}
	
	}	
	#else
	{	

		i51SymPoolNew = i51SlLoaderLoadFromFile ( name , path , BinaryBuffer , BinaryBufferlen ) ;

		if ( 0 == i51SymPoolNew ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : Kit LD F" ) ;
			return 0 ;			 
		}			
	
	}
	#endif
 
	i51SymPoolNew->next = 0 ;
	i51SymPoolNew->head = i51SymbolPool.next ;
		
	if ( 0 == i51SymbolPool.head ) {

		i51SymbolPool.head = i51SymPoolNew ;
		i51SymbolPool.next = i51SymPoolNew ;		
		
	} else if ( 0 != i51SymbolPool.next ) {
	
		i51SymbolPool.next->next = i51SymPoolNew ;		
		i51SymbolPool.next = i51SymPoolNew ;		
		
	}

	//
	//	i51SlLoaderSetCacheScope () ;
	// 
   
	return i51SymPoolNew->handle ;


#elif i51_LOADER_LOADFAKE

	static iS32 handle = 0 ;

	i51SymPoolNew = (i51SYMPOOL* ) i51MALLOC ( sizeof(i51SYMPOOL) ) ;

	if ( 0 == i51SymPoolNew ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Mem F" ) ;
		return 0 ;		
	}

	handle ++ ;
	i51AdeStdMemset16 ( i51SymPoolNew , 0 , sizeof(i51SYMPOOL) ) ; 
	i51AdeStdWStrcpy ( i51SymPoolNew->name , name ) ;
	i51AdeStdWStrcpy ( i51SymPoolNew->path , path ) ;
	
	i51SymPoolNew->Binary.type = i51_LD_MEM_MODE_STATIC ;

	i51SymPoolNew->next = 0 ;
	i51SymPoolNew->head = i51SymbolPool.next ;
		
	if ( 0 == i51SymbolPool.head ) {

		i51SymbolPool.head = i51SymPoolNew ;
		i51SymbolPool.next = i51SymPoolNew ;		
		
	} else if ( 0 != i51SymbolPool.next ) {

		i51SymbolPool.next->next = i51SymPoolNew ;		
		i51SymbolPool.next = i51SymPoolNew ;		
		
	}

	i51SymPoolNew->handle = handle ;

	return i51SymPoolNew->handle ;
		
#endif

}

iS32 i51SlLoaderLoadEx ( iU16* name , iU8* data , iU32 datasize , iU8* BinaryBuffer , iU32 BinaryBufferlen ) {

	//	author : Jelo 
	//	since : 2011.11.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	加载内存，并将动态库加载到指定的BinaryBuffer 中。

	i51SYMPOOL* i51SymPoolNew = 0 ;
	
#ifndef i51_LOADER_LOADFAKE

	iBOOL ret = 0 ;
	iFILE file = 0 ;
	iS32 filelength = 0 ;
	iS32 handle = 0 ;	

	if ( 0 == data || 0 == datasize ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Para Null" ) ;
		return 0 ;
	}

	i51SymPoolNew = i51SlLoaderLoadFromImage ( name , data , datasize , BinaryBuffer , BinaryBufferlen ) ;
	
	if ( 0 == i51SymPoolNew ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Kit LD F" ) ;
		return 0 ;			 
	}			

	i51SymPoolNew->next = 0 ;
	i51SymPoolNew->head = i51SymbolPool.next ;
		
	if ( 0 == i51SymbolPool.head ) {

		i51SymbolPool.head = i51SymPoolNew ;
		i51SymbolPool.next = i51SymPoolNew ;		
		
	} else if ( 0 != i51SymbolPool.next ) {
	
		i51SymbolPool.next->next = i51SymPoolNew ;		
		i51SymbolPool.next = i51SymPoolNew ;		
		
	}

	//
	//	i51SlLoaderSetCacheScope () ;
	//

	return i51SymPoolNew->handle ;

#elif i51_LOADER_LOADFAKE

		static iS32 handle = 0 ;

		i51SymPoolNew = (i51SYMPOOL* ) i51MALLOC ( sizeof(i51SYMPOOL) ) ;

		if ( 0 == i51SymPoolNew ) {
			i51AdeOsLog ( i51_LOG_SC , "LOD : Mem F" ) ;
			return 0 ;		
		}

		handle ++ ;
		i51AdeStdMemset16 ( i51SymPoolNew , 0 , sizeof(i51SYMPOOL) ) ; 
		i51AdeStdWStrcpy ( i51SymPoolNew->name , name ) ;

		i51SymPoolNew->Binary.type = i51_LD_MEM_MODE_STATIC ;

		i51SymPoolNew->next = 0 ;
		i51SymPoolNew->head = i51SymbolPool.next ;
			
		if ( 0 == i51SymbolPool.head ) {

			i51SymbolPool.head = i51SymPoolNew ;
			i51SymbolPool.next = i51SymPoolNew ;		
			
		} else if ( 0 != i51SymbolPool.next ) {
		
			i51SymbolPool.next->next = i51SymPoolNew ;		
			i51SymbolPool.next = i51SymPoolNew ;		
			
		}
 
		i51SymPoolNew->handle = handle ;

		return i51SymPoolNew->handle ;
		
#endif
	
}

iBOOL i51SlLoaderUnload ( iS32 handle ) {

	//	author : Jelo 
	//	since : 2011.5.15
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes  : 销毁动态库
	
	i51SYMPOOL* looper = 0 ;
	i51SYMBOL* walker = 0 ; 

 	for ( looper = i51SymbolPool.head ; looper && looper->handle != handle ; ) {
		looper = looper->next ;
 	}

	if ( 0 == looper )  {
		
		i51AdeOsLog ( i51_LOG_SC , "LOD : Not Found Kit %x" , handle ) ;
		return iFALSE ;

	}
	
	if ( looper == i51SymbolPool.head ) {
		
		//	记录新的头结点
		i51SymbolPool.head = looper->next ;
		if ( 0 == i51SymbolPool.head ) {
			i51SymbolPool.next = 0 ;
		}
		
	} else if ( 0 == looper->next ) {
	
		//	记录尾结点
		i51SymbolPool.next = looper->head ;	
		  
	}

	#ifdef i51SIMULATOR
	{
		if ( i51_LD_MEM_MODE_DYNAMIC  == looper->Binary.type )
		{
			FreeLibrary ( looper->handle ) ;
		}
	}
	#else
	{
		if ( i51_LD_MEM_MODE_DYNAMIC  == looper->Binary.type )
		{
			i51FREE ( looper->Binary.address ) ;
		}
	}
	#endif

	if ( 0 != looper->symbols ) i51FREE ( looper->symbols ) ;
	if ( 0 != looper->head ) looper->head->next = looper->next ; 
	if ( 0 != looper->next ) looper->next->head = looper->head ;
	i51FREE ( looper ) ;
	
	return iTRUE ;
	
}

iS32 i51SlLoaderUnloadAll () {

	//	author : Jelo 
	//	since : 2011.5.15
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes  : 销毁动态库
	
	i51SYMPOOL* looper = 0 ;
	i51SYMBOL* walker = 0 ;    

 	for ( looper = i51SymbolPool.head ; looper ; ) {

		i51SymbolPool.next = looper->next ;
		if ( 0 != looper->symbols ) i51FREE ( looper->symbols ) ;

		#ifndef i51SIMULATOR
		{	
			if ( i51_LD_MEM_MODE_DYNAMIC  == looper->Binary.type )
			{
				i51FREE ( looper->Binary.address ) ;
			}
		}
		#endif
		
		i51FREE ( looper ) ;
		looper = i51SymbolPool.next ;

 	}

	i51SymbolPool.head = 0 ;
	i51SymbolPool.next = 0 ;

	return 1 ;
	
}

void* i51SlLoaderGetSymbol ( iS32 handle , iCHAR* name ) {

	//	author : Jelo 
	//	since : 2011.5.15
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes  : 查找符号入口地址，如果找到返回地址
	//	如果未找到则返回0

#ifndef i51_LOADER_LOADFAKE

	iS32 counter = 0 ;
	i51SYMPOOL* looper = 0 ;

 	for ( looper = i51SymbolPool.head ; looper && looper->handle != handle ; ) {
		looper = looper->next ;
 	}

	if ( 0 == looper ) {
		return 0 ;		
	}

	#ifdef i51SIMULATOR
	//#if 0
	{
		return (void* ) GetProcAddress ( looper->handle , name ) ;
	}
	#else
	{
		for ( counter = 0 ; counter < looper->SymbolTotall ; counter ++ ) 	{
			
			if ( 0 == strcmp ( looper->symbols [counter].name , name ) ) {

				if ( i51_SET_THUMB16 == looper->set ) {
					iS32 address = (iS32)looper->symbols[counter].address + (iS32)looper->handle + 1 ;
					return (void* ) address ;
				} else if ( i51_SET_ARM32 == looper->set ) {
					iS32 address = (iS32)looper->symbols[counter].address + (iS32)looper->handle ;
					return (void* ) address ;
				} else {	
					
					i51AdeOsLog ( i51_LOG_SC , "LOD : FATAL E Unkow i51SET" ) ;
					return 0 ;

				}
				 
			}
			
		}
	}
	#endif

	i51AdeOsLog ( i51_LOG_SC , "LOD : Not Found Symbol %s" , name ) ;
				
	return 0 ;

#elif i51_LOADER_LOADFAKE

	i51SYMPOOL* looper = 0 ;

	extern void* i51PortGetAppKitMain ( iU16* name ) ;
	extern void* i51PortGetAppKitPort ( iU16* name ) ;	

 	for ( looper = i51SymbolPool.head ; looper && looper->handle != handle ; ) {
		looper = looper->next ;
 	}

	if ( 0 == looper ) {
		return 0 ;		
	}

	if ( 0 == strcmp ( name , "i51KitMain" ) ) {
		return i51PortGetAppKitMain ( looper->path ) ;
	} else if ( 0 == strcmp ( name , "i51KitPort" ) ) {
		return i51PortGetAppKitPort ( looper->path ) ;	
	} else {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Undefined Symbol '%s'" , name ) ;
		return 0 ;
	}
	 
	return 0 ;


#endif
	
}

void* i51SlLoaderGetSymbolEx ( iU16* libname , iCHAR* name ) {

	//	author : Jelo 
	//	since : 2011.8.26
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

#ifndef i51_LOADER_LOADFAKE	

	iS32 counter = 0 ;
	i51SYMPOOL* looper = 0 ;
	
	for ( looper = i51SymbolPool.head ; looper ; looper = looper->next ) {
		
		if ( 0 == i51AdeStdWStrcmp ( libname , looper->name ) ) {
			break ;
		}
			
	}

	if ( 0 == looper ) {
		return 0 ;			
	}
		
	#ifdef i51SIMULATOR
	{
		return (void* ) GetProcAddress ( looper->handle , name ) ;
	}
	#else
	{
	
		for ( counter = 0 ; counter < looper->SymbolTotall ; counter ++ ) 	{
			
			if ( 0 == strcmp ( looper->symbols [counter].name , name ) ) {

				if ( i51_SET_THUMB16 == looper->set ) {
					iS32 address = (iS32)looper->symbols[counter].address + (iS32)looper->handle + 1 ;
					return (void* ) address ;
				} else if ( i51_SET_ARM32 == looper->set ) {
					iS32 address = (iS32)looper->symbols[counter].address + (iS32)looper->handle ;
					return (void* ) address ;
				} else {	
					
					i51AdeOsLog ( i51_LOG_SC , "LOD : FATAL E Unkow i51SET" ) ;
					return 0 ;

				}
				 
			}
			
		}
	}
	#endif

	i51AdeOsLog ( i51_LOG_SC , "LOD : Not Found Symbol %s" , name ) ;
				
	return 0 ;

#elif i51_LOADER_LOADFAKE

	i51SYMPOOL* looper = 0 ;

	extern void* i51PortGetAppKitMain ( iU16* name ) ;
	extern void* i51PortGetAppKitPort ( iU16* name ) ;	

	for ( looper = i51SymbolPool.head ; looper ; looper = looper->next ) {
		if ( 0 == i51AdeStdWStrcmp ( libname , looper->name ) ) {
			break ;
		}
	}

	if ( 0 == looper ) {
		return 0 ;			
	}

	if ( 0 == strcmp ( name , "i51KitMain" ) ) {
		return i51PortGetAppKitMain ( looper->path ) ;
	} else if ( 0 == strcmp ( name , "i51KitPort" ) ) {
		return i51PortGetAppKitPort ( looper->path ) ;	
	} else {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Undefined Symbol '%s'" , name ) ;
		return 0 ;
	}
	 
	return 0 ;

#endif
	
}

iS32 i51SlLoaderGetLastError () {

	//	author : Jelo 
	//	since : 2011.9.7
	// 	(C) 2011 , PKIG Tech. Co., Ltd.		

	return i51SlLoaderLastError ;
	
}

iBOOL i51SlLoaderGetAttr ( iFILE filehd , iCHAR* name , iU32* Version , iU32* Kernel , iU32* Adapter , iU32* Endian ) 
{

	//	author : Jelo 
	//	since : 2011.9.8
	// 	(C) 2011 , PKIG Tech. Co., Ltd.		

	iBOOL ret = iFALSE ;
	iS32 seek = 0 ;	
	i51HEADER i51header = { 0 } ;

	i51AdeOsFileGetSeek ( filehd , &seek , i51_OS_FL_SEEK_HEAD ) ;
	ret = i51AdeOsFileSetSeek ( filehd , 4 , i51_OS_FL_SEEK_HEAD ) ;

	if ( iFALSE == ret ) {		
		i51AdeOsLog ( i51_LOG_SC , "LOD : Big E With i51AdeOsFileSetSeek" ) ;
		ret = iFALSE ;
		goto ATTRERROR ;
	}

	ret = i51AdeOsFileRead ( filehd , &i51header , sizeof(i51HEADER) , 0 ) ;

	if ( iFALSE == ret ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Big E With i51AdeOsFileRead" ) ; 
		ret = iFALSE ;
		goto ATTRERROR ;
	}

	if ( 0 != name ) {
		strcpy ( name , i51header.Name ) ;	
	}

	if ( 0 != Version ) {
		//	Jelo Edited 20120302
		//	*(Version) = i51header.Weight & 0xffff ;
		//*(Version) = i51header.Weight ;
		//	Jelo Edited Finished
		
		//	Jelo Edited 20120628
		*(Version) = i51header.Weight & 0xffff ;
		//	Jelo Edited Finished

	}

	if ( 0 != Kernel ) {	
		*(Kernel) = i51header.Kernel ;
	}

	if ( 0 != Adapter ) {
		*(Adapter) = i51header.Adapter ;
	}

	if ( 0 != Endian ) {
		//	Jelo Edited 20120629
		//*(Endian) = i51header.Endian ;
		*(Endian) = i51header.Endian & 0x1 ;
		//	Jelo Edited Finished
	}

	i51AdeOsFileSetSeek ( filehd , seek , i51_OS_FL_SEEK_HEAD ) ;
	
	return iTRUE ;

ATTRERROR :	

	i51AdeOsFileSetSeek ( filehd , seek , i51_OS_FL_SEEK_HEAD ) ;

	return ret ;
	
}

iBOOL i51SlLoaderMakeSnapshot ( iU32 handle )
{

	//	author : Jelo 
	//	since : 2011.9.16
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	将RWZI Dump到外部文件

	iFILE file = 0 ;
	iU16 path [ 256 ]  = { 0 } ;
	iU16* ppath = 0 ;
	iBOOL ret = iFALSE ;
	i51SYMPOOL* looper = 0 ;

 	for ( looper = i51SymbolPool.head ; looper && looper->handle != handle ; ) {
		looper = looper->next ;
 	}

	if ( 0 == looper ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Snapshot id Not Found" ) ;
		return iFALSE ;
	}
	
	ppath = i51AdeStdWStrcat ( path , looper->name ) ;
	ppath = i51AdeStdWStrcat ( ppath , L"\\" ) ;
	ppath = i51AdeStdWStrcat ( ppath, looper->name ) ;
	ppath = i51AdeStdWStrcat ( ppath , L".snap" ) ;

	file = i51AdeOsFileOpen ( ppath , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE , __FILE__ , __LINE__ ) ;

	if ( 0 == file ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Create Snapshot File F" ) ;
		return iFALSE ;		
	}

	#ifdef i51_LOADER_DBG
		i51AdeOsLog ( 0 , "MAKE looper->Binary.rwoffset %d" , looper->Binary.rwoffset ) ;
		i51AdeOsLog ( 0 , "RW ZI Length %d" , looper->Binary.length-looper->Binary.rosize ) ;
	#endif
	
	ret = i51AdeOsFileWrite ( 
		file , 
		(void*)( (iU32)looper->Binary.address+looper->Binary.rwoffset) ,    
		looper->Binary.length-looper->Binary.rosize , 0 ) ; 
  
	if ( iFALSE == ret ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Write Snapshot File F" ) ;
		i51AdeOsFileClose ( file ) ; 
		return iFALSE ;				
	}

	i51AdeOsFileClose ( file ) ; 

	return iTRUE ;				

}

iBOOL i51SlLoaderDropSnapshot ( iU32 handle )
{

	//	author : Jelo 
	//	since : 2011.9.16
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iFILE file = 0 ;
	iU16 path [ 256 ]  = { 0 } ;
	iU16* ppath = 0 ;
	iBOOL ret = iFALSE ;
	i51SYMPOOL* looper = 0 ;

 	for ( looper = i51SymbolPool.head ; looper && looper->handle != handle ; ) {
		looper = looper->next ;
 	}

	if ( 0 == looper ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Snapshot id Not Found" ) ;
		return iFALSE ;
	}
	
	ppath = i51AdeStdWStrcat ( path , looper->name ) ;
	ppath = i51AdeStdWStrcat ( ppath , L"\\" ) ;
	ppath = i51AdeStdWStrcat ( ppath, looper->name ) ; 
	ppath = i51AdeStdWStrcat ( ppath , L".snap" ) ;

	ret = i51AdeOsFileDelete ( ppath ) ;

	if ( iFALSE == ret ) {
		i51AdeOsLog ( i51_LOG_SC , "LOD : Snapshot Drop File F" ) ;
		return iFALSE ;		
	}

	return iTRUE ;
	
}

void i51SlLoaderGetKitRef ( iFILE file , i51SllDKITREF* i51SllDKitRefBuf , iU32 MaxRef , iU32* i51SllDKitRefTotall ) {

	//	author : Jelo 
	//	since : 2011.10.27 
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 looper = 0 ;
	iU32 walker = 0 ;
	iU32 counter = 0 ;
	i51HEADER i51Header = { 0 } ;

	

	//	skip crc
	i51AdeOsFileSetSeek ( file , sizeof(iU32) , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileRead ( file , &i51Header , sizeof(i51HEADER) , 0 ) ;
	i51AdeOsFileSetSeek ( file , -8 , i51_OS_FL_SEEK_CURRENT ) ;
	
	i51AdeOsFileRead ( file , &i51Header.Refoffset , sizeof(i51Header.Refoffset) , 0 ) ;
	i51AdeOsFileRead ( file , &i51Header.Reftotall , sizeof(i51Header.Reftotall) , 0 ) ;

	if ( 0 == i51Header.Reftotall ) {
		*i51SllDKitRefTotall = 0 ;
		return ;
	}

	//	Wiilliam Since 2012.3.29
	i51AdeOsLog ( i51_LOG_SC , "LOD : Kit Ref , T %d , M %d" , i51Header.Reftotall , MaxRef ) ;
	//	William Edting Finished
	
	iASSERT( i51Header.Reftotall < MaxRef ) ;
	
	i51AdeOsFileSetSeek ( file , i51Header.Refoffset+sizeof(iU32) , i51_OS_FL_SEEK_HEAD ) ;

	for ( looper = 0 ; looper < i51Header.Reftotall ; looper ++ ) {
		i51AdeOsFileRead ( file , &i51SllDKitRefBuf[looper].version , sizeof(i51SllDKitRefBuf[looper].version) , 0 ) ;
		i51AdeOsFileRead ( file , i51SllDKitRefBuf[looper].path , sizeof(i51SllDKitRefBuf[looper].path) , 0 ) ;
	}

	*i51SllDKitRefTotall = i51Header.Reftotall ;
	return ;
	
}

void i51SlLoaderSetCache ( iU16* name ) {

	//	author : Jelo 
	//	since : 2011.11.22
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	i51SYMPOOL* looper = 0 ;

 	for ( looper = i51SymbolPool.head ; looper ; looper = looper->next ) {

		if ( 0 == i51AdeStdWStrcmp (looper->name , name ) ) {
			break ;
		}
		  
 	}

	if ( 0 != looper ) {
		i51AdeOsSetCPUCache ( (iU32)looper->Binary.address , (iU32)looper->Binary.address+looper->Binary.rosize ) ;
	}

	return ;
	
}

void i51SlLoaderUnsetCache ( iU16* name ) {

	//	author : Jelo 
	//	since : 2011.11.22
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	i51AdeOsUnsetCPUCache () ;
	
	return ;
	
}

#ifdef i51_LOADER_DBG 
void i51SlLoaderDumpAll () {

	//	author : Jelo 
	//	since : 2011.11.19
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	i51SYMPOOL* looper = 0 ;
	static iU32 times = 0 ;
	
	for ( looper = i51SymbolPool.head ; looper ; looper = looper->next ) {

		iU16* namep = 0 ; 
		iU16 buffer [64] = { 0 } ;
		iU8 buff8 [64] = { 0 } ; 

		namep = i51AdeStdWStrcpy ( buffer , looper->name ) ;
		i51AdeStdUnicode2Ascii ( namep , buff8 ) ;
		
		i51AdeStdWSprintf ( buffer , sizeof(buffer) , "%s.%d.new", buff8 , times ) ;
		i51SlLoaderDumpEx ( buffer , looper->Binary.address , looper->Binary.length ) ;
		
	}

	times ++ ;


}
#endif

iBOOL i51SlLoaderLoadable ( iFILE file ) {

	//	author : Jelo 
	//	since : 2011.11.28
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifndef i51_LOADER_LOADFAKE	

	iBOOL ret = iFALSE ;
	i51HEADER i51header = { 0 } ;	
	iS32 seek = 0 ;

	i51AdeOsFileGetSeek ( file , &seek , i51_OS_FL_SEEK_HEAD ) ;
	ret = i51AdeOsFileSetSeek ( file , 4 , i51_OS_FL_SEEK_HEAD ) ;

	if ( iFALSE == ret ) {
		i51AdeOsFileSetSeek ( file , seek , i51_OS_FL_SEEK_HEAD ) ;
		i51AdeOsLog ( i51_LOG_SC , "LOD : FE , SetSeek" ) ;
		return iFALSE ;
	}

	ret = i51AdeOsFileRead ( file , &i51header , sizeof(i51HEADER) , 0 ) ;

	
	if ( iFALSE == ret ) {
		i51AdeOsFileSetSeek ( file , seek , i51_OS_FL_SEEK_HEAD ) ;
		i51AdeOsLog ( i51_LOG_SC , "LOD : FE , SetSeek" ) ; 
		return iFALSE ;
	} 

	if ( i51_CPU_MIPS == i51AdeCnfGetCpu ()  ) {
		if ( 2 == i51header.Endian && i51_ADE_CNF_LITTEL_ENDIAN == i51AdeCnfGetEndian () ) {
			return iTRUE ;
		} else if ( 3 == i51header.Endian && i51_ADE_CNF_BIG_ENDIAN == i51AdeCnfGetEndian () ) {
			return iTRUE ;
		} else {
			return iFALSE ;		
		}
	} 

	if ( i51_CPU_ARM == i51AdeCnfGetCpu ()  ) {
		if ( 0 == i51header.Endian && i51_ADE_CNF_LITTEL_ENDIAN == i51AdeCnfGetEndian () ) {
			return iTRUE ;
		} else if ( 1 == i51header.Endian && i51_ADE_CNF_BIG_ENDIAN == i51AdeCnfGetEndian () ) {
			return iTRUE ;
		} else {
			return iFALSE ;		
		}
	} 

	//	Jelo Committed 2012.6.29
	//if ( i51_ADE_CNF_LITTEL_ENDIAN == i51header.Endian ) {
	//	if ( i51_ADE_CNF_LITTEL_ENDIAN == i51AdeCnfGetEndian () ) {
	//		return iTRUE ;
	//	} else { 
	//		return iFALSE ;
	//	}
	//}
	//	Jelo Committed Finished
	
	if ( i51_LOADER_OBJVERSION < i51header.Version ) {
		i51AdeOsFileSetSeek ( file , seek , i51_OS_FL_SEEK_HEAD ) ;
		i51AdeOsLog ( i51_LOG_SC , "LOD : Not Support i51-OBJ Version %x , LV %x " ,  i51header.Version , i51_LOADER_OBJVERSION ) ; 
		return iFALSE ;
	}

	return iTRUE ;

#elif i51_LOADER_LOADFAKE

	return iTRUE ;

#endif
		
}

