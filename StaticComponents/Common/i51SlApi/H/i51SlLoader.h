
/*

	Layer belong : 'Static Components'.
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef THE_i51LOADER_H
#define THE_i51LOADER_H

//	加载动态库文件
//#define i51_LOADER_LOADFILE

//	虚拟加载
//#define i51_LOADER_LOADFAKE

#define i51_SYMBOL_NAME_MAX 32  
//	object version that loadable with loader
#define i51_LOADER_OBJVERSION 1

#include "i51.h" 
 
typedef enum   
{   

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.  
 
	i51_LD_ERROR_MEMORY = -1 ,
	i51_LD_ERROR_CRC = -2 ,  
	i51_LD_ERROR_FILE = -3 ,

} i51LDERROR ;

typedef enum {

	//	author : Jelo
	//	since : 2011.5.15
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	静态内存
	i51_LD_MEM_MODE_STATIC , 
	//	动态内存
	i51_LD_MEM_MODE_DYNAMIC , 
	
} i51LDMEMMODE ;

typedef enum {
	
	//	author : Jelo
	//	since : 2011.5.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	i51_LD_RELOC_RO ,
	i51_LD_RELOC_RW ,

} i51LDRELOC ;

typedef enum {
	
	//	author : Jelo
	//	since : 2011.5.15
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	i51_SET_THUMB16  = 16 ,
	i51_SET_ARM32 = 32 ,

} i51SET ;

typedef enum {
	
	//	author : Jelo
	//	since : 2011.5.15
	// 	(C) 2011 , PKIG Tech. Co., Ltd.  
	
	i51_LITTEL_ENDIAN = 0 ,
	i51_BIG_ENDIAN ,

} i51ENDIAN ;

typedef struct {

	//	author : Jelo
	//	since : 2011.5.13
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	int offset ;
	//	for RW relocation
	int rwsize ;

} i51OBJ ;

typedef struct {

	//	author : Jelo
	//	since : 2011.5.13
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : i51 符号
	
	//	i51OBJ 索引
	int objindex ;
	int offset ;

} i51REL ;

typedef struct  {

	//	author : Jelo
	//	since : 2011.6.8
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : 各个OBJ的RW段在整个image中的入口
	//	RW Entry Map
	
	int offset ;
	
} i51RWE ;

typedef struct {
	
 	//	author : Jelo
	//	since : 2011.5.12
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	char Magic [ 4 ] ;		
	char Name [ 32 ] ;
	
	//	For user's custom data
	unsigned short Bitmap ; 	
	//	file structure version
	unsigned short Version ;
	unsigned short Endian ;
	unsigned short Set ;
	unsigned short Kernel ;
	unsigned short Adapter ;
	//	Weight 0xffff,ffff the high 16 bit saving permisons
	//	the last saving version of a kit
	unsigned int Weight ;
	unsigned int BinOffset ;
	unsigned int BinSize ;
	unsigned int RoSize ;
	unsigned int ZiSize ;
	unsigned int SymbolTotall ;
	unsigned int SymbolOffset ;
	unsigned int RelTotall ;
	unsigned int RelOffset ;
	unsigned int RwRelTotall ;
	unsigned int RwRelOffset ;
	unsigned int RwEntryMapTotall ;		
	unsigned int RwEntryMapOffset ;	
	unsigned int ObjTotall ;
	unsigned int ObjOffset ;	
	unsigned int Refoffset ;	
	unsigned int Reftotall ;

} i51HEADER ;

typedef struct i51SYMN {

 	//	author : Jelo
	//	since : 2011.5.15
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	iCHAR name [ i51_SYMBOL_NAME_MAX ] ;
	void* address ;

} i51SYMBOL  ;

typedef struct i51LD  {

 	//	author : Jelo
	//	since : 2011.5.15
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iS32 handle ;
	iU16 name [32] ;

	#ifdef i51_LOADER_LOADFAKE
	iU16 path [64] ;
	#endif
	
	//	指令集
	i51SET set ;

	//	符号数
	//	符号表
	iS32 SymbolTotall ;
	i51SYMBOL* symbols ;

	struct {

		//	二进制镜像

		i51LDMEMMODE type ;
		iU8* address ;
		iU32 rosize ;
		iU32 rwoffset ;
		//	ro rw zi
		iU32 length ;
		
	} Binary ;
	
	struct i51LD* head ; 
	struct i51LD* next; 

} i51SYMPOOL ;

#endif 

extern iS32 i51SlLoaderLoad ( iU16* name , iU16* path , iU8* BinaryBuffer , iU32 BinaryBufferlen ) ;
extern iS32 i51SlLoaderLoadEx ( iU16* name , iU8* data , iU32 datasize , iU8* BinaryBuffer , iU32 BinaryBufferlen ) ;
extern iBOOL i51SlLoaderUnload ( iS32 handle ) ;
extern iS32 i51SlLoaderUnloadAll () ;
extern void* i51SlLoaderGetSymbol ( iS32 handle , iCHAR* name ) ;
extern void* i51SlLoaderGetSymbolEx ( iU16* name , iCHAR* symbol ) ;
extern iS32 i51SlLoaderGetLastError () ;
extern iBOOL i51SlLoaderGetAttr ( iFILE filehd , iCHAR* name , iU32* Version , iU32* Kernel , iU32* Adapter , iU32* Endian )  ;
extern iBOOL i51SlLoaderMakeSnapshot ( iU32 handle ) ;
extern iBOOL i51SlLoaderDropSnapshot ( iU32 handle ) ;
extern void i51SlLoaderGetKitRef ( iFILE file , i51SllDKITREF* i51SllDKitRefBuf , iU32 MaxRef , iU32* i51SllDKitRefTotall ) ;

#ifdef i51_LOADER_DBG
extern void i51SlLoaderDumpAll () ;
#endif

extern void i51SlLoaderSetCache ( iU16* name ) ;
extern void i51SlLoaderUnsetCache ( iU16* name )  ;
extern iBOOL i51SlLoaderLoadable ( iFILE file ) ;

