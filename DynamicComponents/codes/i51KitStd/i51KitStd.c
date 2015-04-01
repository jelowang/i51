
#include "i51KitAde.h"
#include "i51KitStd.h"

i51KIT_STATIC_KIT_DEF(Adapter);
//	能够打开资源文件的最大的个数
#define RES_FILE_MAX_COUNT	10
//	资源文件名
#define RES_FILE_NAME_MAX_LEN	50
//	src校验时，最大分配50K内存
#define RES_MALLOC_SIZE	(1024*50)
//crc,re,资源版本号，文件格式版本号，资源的个数 占用的字节数
#define CRC_HEAD_RESCOUNT_COUNT	(sizeof ( iU32 )+2+4+4+4+4)
//读取资源个数的文件偏移量  src re ,crc,re,资源版本号，文件格式版本号
#define RES_COUNT_SETOFF	(sizeof ( iU32 )+2+4+4)
//	id的字符串形式的最大长度
#define RES_ID_STR_LEN	32
typedef struct {

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	文件句柄
	iFILE OpenedFilehandle ;
	//	re文件名
	iU16 OpenedFileName[RES_FILE_NAME_MAX_LEN+1] ;
	//	一个re文件打开的次数
	iU16 OpenedCount ;
	
}I51_RES_OPENED_FILE_INFO ;


typedef struct 
{

	//	author : william
	//	since : 2011.8.24
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	//id的字符串
	iCHAR ID[RES_ID_STR_LEN] ;
	//偏移量
	iU32 SetOff ;  
	iU32 size ;
	
} I51_RES_BIN_INDEX_INFO ;


static I51_RES_OPENED_FILE_INFO i51ResOpendFileInfo[RES_FILE_MAX_COUNT] = { 0 } ;
static iU32 *CRC32Table = NULL ;
static iCHAR IDString[RES_ID_STR_LEN+1] ;

static void i51KitStdGetCRC32 (  iU8* buf , iS32 len  ,iU32 *nCRC  )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	//unsigned long nCRC = 0xFFFFFFFF;
	int i;

	if ( 0 != buf )
	{
	
		for( i = 0 ; i < len ; i ++ ) {
			
			*nCRC = ( *nCRC >> 8 ) ^ CRC32Table[ ((buf[i]+0)&0xFF) ^ ( *nCRC & 0x000000FF ) ];
		
		}

	}

}
/*
*问题：1、添加文件句柄
2、增加文件打开，关闭计数器，为0时真正关闭
3、代开文件的时候，路径不需要拼接直接写全路径
*/

iFILE i51KitStdResOpen ( iU16* name )
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//打开re 文件 
	//打开文件不要加后缀   应用名\\资源名  如tomcat\\tomcat
	
	iFILE file ;
	iS32 Filesize = 0 ;
	iS32 ReadSize = 0 ;
	iS32 size = 0 ;
	iU32 CRCValue = 0 ;
	iU32 nCRC = 0xFFFFFFFF ;
	iU8* data = NULL ;
	iU16 i = 0 ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResOpen S , %x" , name ) ;
	#endif	


	for ( i = 0 ; i <  RES_FILE_MAX_COUNT ; i ++ ) {

		if ( i51AdeStdWStrcmp ( i51ResOpendFileInfo[i].OpenedFileName , name ) == 0 ){

			i51ResOpendFileInfo[i].OpenedCount++ ;
			
			#ifdef i51ADAPTER_DEBUG
			i51AdeOsLog ( 0 , "i51KitStdResOpen E , ret = %x" , i51ResOpendFileInfo[i].OpenedFilehandle ) ;
			#endif	
			
			return i51ResOpendFileInfo[i].OpenedFilehandle ;
		}

	}

	file = i51AdeOsFileOpen ( name , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
	
	if ( file <= 0) {

		iCHAR filename8bit[64] = {0} ;
		i51AdeStdUnicode2Ascii (name , filename8bit) ;
		i51AdeOsLog ( i51_LOG_DC , "KIT - STD : i51KitStdResOpen BE1.1 F , %s" , filename8bit ) ;
		return 0 ;
	}
	
#if 1
	i51AdeOsFileGetSize ( file , &Filesize) ;
	Filesize = Filesize - sizeof (iU32) ;
	data = (iU8*)i51AdeOsMalloc ( RES_MALLOC_SIZE ,__FILE__ ,__LINE__) ;
	
	if ( data == NULL ) {

		//分配空间失败，关闭文件返回
		i51AdeOsLog ( i51_LOG_DC , "KIT - STD : i51KitStdResOpen malloc F" ) ;
		goto ERROR_END ;
	}
	i51AdeOsFileSetSeek ( file, 0 ,i51_OS_FL_SEEK_HEAD) ;
	i51AdeOsFileReadInt32 ( file , &CRCValue) ;
	i51AdeStdCrc32 ( 0 , 0 ,(iU32**) &CRC32Table ) ; 
	
	while ( ReadSize <  Filesize   ) {

		if ( Filesize - ReadSize >= RES_MALLOC_SIZE ) {

			size = RES_MALLOC_SIZE ;

		} else {

			size =  Filesize - ReadSize ;
		}
		memset ( data , 0 , RES_MALLOC_SIZE ) ;
		i51AdeOsFileRead ( file , data , size , 0 ) ;
		i51KitStdGetCRC32 ( data , size , &nCRC ) ;	
		ReadSize += size ;
	
	}
	if ( NULL != data ) {
		
		i51AdeOsFree ( data ) ;
		data = NULL ;
	}
	nCRC = nCRC ^ 0xFFFFFFFF ;
	if ( nCRC != CRCValue ) {

		i51AdeOsLog ( i51_LOG_DC , "KIT - STD : i51KitStdResOpen BE1.2" ) ;
		//	crc 校验出问题，删除文件
		{
			iCHAR name8bit[64] = {0} ;
			i51AdeStdUnicode2Ascii (name ,name8bit) ;
			i51AdeOsLog ( i51_LOG_DC , "KIT - STD : i51KitStdResOpen del %s" ,name) ;
		}
		i51AdeOsFileClose( file ) ;
		file = 0 ;
		i51AdeOsFileDelete (name) ;
		goto ERROR_END ;
	}
	
#endif	
		for ( i = 0 ; i <  RES_FILE_MAX_COUNT ; i ++ ) {

			if ( !i51ResOpendFileInfo[i].OpenedFilehandle )
				break ;
		}
		if ( i ==  RES_FILE_MAX_COUNT) {

			i51AdeOsLog ( i51_LOG_DC , "KIT - STD : i51KitStdResOpen BE1.3" ) ;
			goto ERROR_END ;
		}
		//	打开条件成立了，添入管理
		i51AdeStdWStrcpy ( i51ResOpendFileInfo[i].OpenedFileName , name ) ;
		i51ResOpendFileInfo[i].OpenedFilehandle = file ;
		i51ResOpendFileInfo[i].OpenedCount ++ ;

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "i51KitStdResOpen E , ret = %x" , file) ;
		#endif			
		
		return file ;
ERROR_END:
{
	if (file > 0)

		i51AdeOsFileClose( file ) ;
	if (NULL != data)
		i51AdeOsFree ( data ) ;
	file = 0 ;
}

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResOpen E , ret = %x" , file) ;
	#endif			

	return file ;
	
}

/*
iBOOL i51KitStdResSetContext ( iFILE file ) 
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//设置当前使用的资源的文件

	CurUseFile = file ;

	return iTRUE ;

}
*/
iBOOL i51KitStdResDelete ( iU16* name ) 
{
	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	删除资源文件

	iBOOL ret = 0 ;
	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResDelete S , %x" , name ) ;
	#endif	


	ret =  i51AdeOsFileDelete ( name ) ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResDelete E , ret = %x" , ret ) ;
	#endif	
	
	return ret ;

}

iU32 i51KitStdResGetTextSize ( iFILE  file , iU32 textid )
{
	//	author : william
	//	since : 2011.9.7
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取到指定字符的大小

	iU32 TextCount = 0 ;
	iU32 setoff = 0 ;
	iU32 size = 0 ;
//	I51_RES_BIN_INDEX_INFO i51ResTextIndexInfo = {0} ;
	
	
	
	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetTextSize S , %x , %x" , file ,textid ) ;
	#endif	
	
	i51AdeOsFileSetSeek ( file ,  RES_COUNT_SETOFF  , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&TextCount ) ;
	

	if ( textid + 1 > TextCount || textid + 1 <= 0  ) {
		
		i51AdeOsLog ( i51_LOG_DC , "KIT - STD : i51KitStdResGetTextSize BE" ) ;
		return 0 ;
	}

	i51AdeOsFileSetSeek ( file ,  CRC_HEAD_RESCOUNT_COUNT + sizeof(I51_RES_BIN_INDEX_INFO) * textid   , i51_OS_FL_SEEK_HEAD ) ;
//	i51AdeOsFileRead ( file , &i51ResTextIndexInfo , sizeof( i51ResTextIndexInfo ) , 0 ) ;
	memset ( IDString , 0 , sizeof ( IDString ) ) ;
	i51AdeOsFileRead ( file , IDString , RES_ID_STR_LEN , 0 ) ;
	i51AdeOsFileReadInt32 ( file , &setoff ) ;
	i51AdeOsFileReadInt32 ( file , &size ) ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetTextSize E" ) ;
	#endif	
	
	return size ;

}

char*  i51KitStdResGetTextIDString ( iFILE  file , iU32 textid ) 
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取指定字符的id字符串形式
	iU32 TextCount = 0 ;
	iU32 setoff = 0 ;
	iU32 size = 0;
//	I51_RES_BIN_INDEX_INFO i51ResTextIndexInfo = {0} ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetTextIDString S , %x , %x" , file ,textid ) ;
	#endif	
	
	i51AdeOsFileSetSeek ( file ,  RES_COUNT_SETOFF , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&TextCount ) ;
	
	if ( textid + 1 > TextCount || textid + 1 <= 0  ) {
		
		i51AdeOsLog ( i51_LOG_DC , "KIT - STD i51KitStdResGetTextIDString BE") ;	
		return 0 ;
	}

	i51AdeOsFileSetSeek ( file ,  CRC_HEAD_RESCOUNT_COUNT + sizeof(I51_RES_BIN_INDEX_INFO) * textid   , i51_OS_FL_SEEK_HEAD ) ;
	memset ( IDString , 0 , sizeof ( IDString ) ) ;
	i51AdeOsFileRead ( file , IDString , RES_ID_STR_LEN , 0 ) ;
	i51AdeOsFileReadInt32 ( file , &setoff ) ;
	i51AdeOsFileReadInt32 ( file , &size ) ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetTextIDString E , ret =  %x" , IDString ) ;
	#endif	
	
	return IDString ;
	
}


void i51KitStdResGetText ( iFILE  file ,iU32 textid ,  void* buffer , iU32 bufsize  ) 
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取指定字符串的内容

	iU32 TextCount = 0 ;
	iU32 setoff = 0 ;
	iU32 size = 0;
//	I51_RES_BIN_INDEX_INFO i51ResTextIndexInfo = {0} ;
	
	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetText S , %x , %x , %x , %x" , file ,textid , buffer , bufsize) ;
	#endif		
	
	i51AdeOsFileSetSeek ( file ,  RES_COUNT_SETOFF , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&TextCount ) ;
	

	if ( textid + 1 > TextCount || textid + 1 <= 0  ) {

		i51AdeOsLog ( i51_LOG_DC , "KIT - STD i51KitStdResGetText BE1.1") ;	
		return  ;
	}

	i51AdeOsFileSetSeek ( file ,  CRC_HEAD_RESCOUNT_COUNT + sizeof(I51_RES_BIN_INDEX_INFO) * textid   , i51_OS_FL_SEEK_HEAD ) ;
//	i51AdeOsFileRead ( file , &i51ResTextIndexInfo , sizeof( i51ResTextIndexInfo ) , 0 ) ;
	memset ( IDString , 0 , sizeof ( IDString ) ) ;
	i51AdeOsFileRead ( file , IDString , RES_ID_STR_LEN , 0 ) ;
	i51AdeOsFileReadInt32 ( file , &setoff ) ;
	i51AdeOsFileReadInt32 ( file , &size ) ;

	if ( bufsize - 2 < size  ) {

		i51AdeOsLog ( i51_LOG_DC , "KIT - STD i51KitStdResGetText BE1.2") ;	
		return ;
	}

	//讲文件指针定位到需要的读取处
	i51AdeOsFileSetSeek ( file , setoff , i51_OS_FL_SEEK_HEAD ) ; 
	//读取需要的字符串的信息
	i51AdeOsFileRead ( file , buffer , size , 0 ) ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetText E" ) ;
	#endif


}

iBOOL i51KitStdResClose ( iFILE file  )
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	关闭资源文件
	iU16 i = 0 ;
	iBOOL ret = iFALSE ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResClose S , %x " , file ) ;
	#endif		

	for ( i = 0 ; i <  RES_FILE_MAX_COUNT ; i ++ ) {

		if ( i51ResOpendFileInfo[i].OpenedFilehandle == file ){
			
			break ;
		}
	}
	if ( i ==  RES_FILE_MAX_COUNT) {

		i51AdeOsLog ( i51_LOG_DC , "KIT - STD i51KitStdResClose BE") ;	
		return iFALSE ;
	}
	
	 i51ResOpendFileInfo[i].OpenedCount -- ;
	 if ( i51ResOpendFileInfo[i].OpenedCount == 0 ) {
	 	
	 	ret =i51AdeOsFileClose( file ) ;
		memset (&i51ResOpendFileInfo[i]  , 0 , sizeof(I51_RES_OPENED_FILE_INFO));
	 }

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResClose E , ret = %x" , iTRUE ) ;
	#endif
	
	return iTRUE ;
	

}

iU32 i51KitStdResGetContentSize ( iFILE  file ,iU32 imageid ) 
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取指定其他资源的大小
	
	iU32 TextCount = 0 ;
	iU32 BinCount = 0 ;
	iU32 setoff = 0 ;
	iU32 size = 0;
	
//	I51_RES_BIN_INDEX_INFO i51ResBinIndexInfo = {0} ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetContentSize S , %x , %x" , file ,imageid) ;
	#endif		
	
	i51AdeOsFileSetSeek ( file ,RES_COUNT_SETOFF  , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&TextCount ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&BinCount ) ;

	if ( imageid + 1 > BinCount || imageid + 1 <= 0  ) {

		i51AdeOsLog ( i51_LOG_DC , "KIT - STD i51KitStdResGetContentSize BE") ;	
		return 0 ;
	}

	
	i51AdeOsFileSetSeek ( file ,  CRC_HEAD_RESCOUNT_COUNT + sizeof ( I51_RES_BIN_INDEX_INFO) * TextCount  + sizeof (I51_RES_BIN_INDEX_INFO) * imageid , i51_OS_FL_SEEK_HEAD ) ;
//	i51AdeOsFileRead ( file , &i51ResBinIndexInfo , sizeof( i51ResBinIndexInfo ) , 0 ) ;
	memset ( IDString , 0 , sizeof ( IDString ) ) ;
	i51AdeOsFileRead ( file , IDString , RES_ID_STR_LEN , 0 ) ;
	i51AdeOsFileReadInt32 ( file , &setoff ) ;
	i51AdeOsFileReadInt32 ( file , &size ) ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetContentSize E , ret = %x" , iTRUE ) ;
	#endif
	
	return size ;
	
}

char*  i51KitStdResGetContentIDString ( iFILE  file , iU32 imageid ) 
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取指定其他资源id字符串形式
	
	iU32 TextCount = 0 ;
	iU32 BinCount = 0 ;
	iU32 setoff = 0 ;
	iU32 size = 0;
	
//	I51_RES_BIN_INDEX_INFO i51ResBinIndexInfo = {0} ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetContentIDString S , %x , %x" , file ,imageid) ;
	#endif		

	i51AdeStdMemset16 ( IDString , 0 , sizeof(IDString)) ;
	i51AdeOsFileSetSeek ( file ,RES_COUNT_SETOFF , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&TextCount ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&BinCount ) ;

	if ( imageid + 1 > BinCount || imageid + 1 <= 0  ) {
		
		i51AdeOsLog ( i51_LOG_DC , "KIT - STD i51KitStdResGetContentIDString BE") ;	
		return 0 ;
	}

	
	i51AdeOsFileSetSeek ( file ,  CRC_HEAD_RESCOUNT_COUNT + sizeof ( I51_RES_BIN_INDEX_INFO) * TextCount  + sizeof (I51_RES_BIN_INDEX_INFO) * imageid , i51_OS_FL_SEEK_HEAD ) ;
//	i51AdeOsFileRead ( file , &i51ResBinIndexInfo , sizeof( i51ResBinIndexInfo ) , 0 ) ;

	memset ( IDString , 0 , sizeof ( IDString ) ) ;
	i51AdeOsFileRead ( file , IDString , RES_ID_STR_LEN , 0 ) ;
	i51AdeOsFileReadInt32 ( file , &setoff ) ;
	i51AdeOsFileReadInt32 ( file , &size ) ;
	
//	i51AdeStdMemcpy16 ( IDString , i51ResBinIndexInfo.ID , sizeof(IDString)) ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetContentIDString E , ret = %x" , IDString ) ;
	#endif
	
	return IDString ;
	
}



iU32 i51KitStdResGetContentEntry (iFILE  file , iU32 imageid ) 
{

	//	author : william
	//	since : 2011.9.5
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取指定其他资源的入口即文件的偏移量
	
	iU32 TextCount = 0 ;
	iU32 BinCount = 0 ;
	iU32 setoff = 0 ;
	iU32 size = 0;
	
//	I51_RES_BIN_INDEX_INFO i51ResBinIndexInfo = {0} ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetContentEntry S , %x , %x" , file ,imageid) ;
	#endif	
	
	i51AdeOsFileSetSeek ( file ,  RES_COUNT_SETOFF, i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&TextCount ) ;
	i51AdeOsFileReadInt32 ( file , ( iU32* )&BinCount ) ;

	if ( imageid + 1 > BinCount || imageid + 1 <= 0  ) {

		i51AdeOsLog ( i51_LOG_DC , "KIT - STD i51KitStdResGetContentEntry BE") ;	
		return 0 ;
	}
	
	i51AdeOsFileSetSeek ( file ,  CRC_HEAD_RESCOUNT_COUNT + sizeof ( I51_RES_BIN_INDEX_INFO) * TextCount  + sizeof (I51_RES_BIN_INDEX_INFO) * imageid , i51_OS_FL_SEEK_HEAD ) ;
//	i51AdeOsFileRead ( file , &i51ResBinIndexInfo , sizeof( i51ResBinIndexInfo ) , 0 ) ;
	memset ( IDString , 0 , sizeof ( IDString ) ) ;
	i51AdeOsFileRead ( file , IDString , RES_ID_STR_LEN , 0 ) ;
	i51AdeOsFileReadInt32 ( file , &setoff ) ;
	i51AdeOsFileReadInt32 ( file , &size ) ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdResGetContentIDString E" ) ;
	#endif
	
	return setoff ;
	

}

iS32 i51KitStdGetKitRef ( iU16* filename , i51SllDKITREF* i51SllDKitRefBuf , iU32* i51SllDKitRefTotall ) {

	//	author : Jelo 
	//	since : 2011.11.29 
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	获取动态库的依赖表。
	
	typedef struct {
	
	 	//	author : Jelo
		//	since : 2011.5.12
		// 	(C) 2011 , PKIG Tech. Co., Ltd.

		//	注意：该结构只有本接口使用，
		//	对应动态库文件结构1号版本。
		
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

	#define i51_LOADER_OBJVERSION 1  

	iU32 looper = 0 ;
	iU32 walker = 0 ;
	iU32 counter = 0 ;

	i51HEADER i51Header = { 0 } ;
	
	iCHAR buffer [ 1024 ] = { 0 } ;
	iCHAR* buffpt = 0 ;

	iFILE file = 0 ;

	file = i51AdeOsFileOpen ( filename , i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;

	if ( 0 == file ) return 0 ;
	
	//	skip crc
	i51AdeOsFileSetSeek ( file , sizeof(iU32) , i51_OS_FL_SEEK_HEAD ) ;
	i51AdeOsFileRead ( file , &i51Header , sizeof(i51HEADER) , 0 ) ;
	i51AdeOsFileSetSeek ( file , -8 , i51_OS_FL_SEEK_CURRENT ) ;
	
	if ( i51_LOADER_OBJVERSION < i51Header.Version ) {
		i51AdeOsFileClose ( file ) ;
		return 0 ;
	}

	//	Jelo Edit 2011.12.31
	//	文件中数据已经是大小端无关
	//	可直接读取流。
	//	i51AdeOsFileReadInt32 ( file , &i51Header.Refoffset ) ;
	//	i51AdeOsFileReadInt32 ( file , &i51Header.Reftotall ) ;
	i51AdeOsFileRead ( file , &i51Header.Refoffset , sizeof(i51Header.Refoffset) , 0 ) ;
	i51AdeOsFileRead ( file , &i51Header.Reftotall , sizeof(i51Header.Reftotall) , 0 ) ;	
	//	Jelo Edit Finished
	
	if ( 0 == i51Header.Reftotall ) {
		i51AdeOsFileClose ( file ) ;
		return 0 ;
	}

	i51AdeOsFileSetSeek ( file , i51Header.Refoffset+sizeof(iU32) , i51_OS_FL_SEEK_HEAD ) ;

	for ( looper = 0 ; looper < i51Header.Reftotall ; looper ++ ) {
		//	Jelo Edit 2011.12.31
		//	文件中数据已经是大小端无关
		//	可直接读取流。		
		//	i51AdeOsFileReadInt16 ( file , &i51SllDKitRefBuf[looper].version ) ;
		i51AdeOsFileRead ( file , &i51SllDKitRefBuf[looper].version , sizeof(i51SllDKitRefBuf[looper].version) , 0 ) ;
		//	Jelo Edit Finished
		i51AdeOsFileRead ( file , i51SllDKitRefBuf[looper].path , sizeof(i51SllDKitRefBuf[looper].path) , 0 ) ;
	}

	*i51SllDKitRefTotall = i51Header.Reftotall ;

	i51AdeOsFileClose ( file ) ;

	return 1 ;
	
}

iBOOL i51KitStdGetKitAttr ( iU16* filename , iCHAR* name , iU32* Version , iU32* Kernel , iU32* Adapter , iU32* Endian ) 
{

	//	author : Jelo 
	//	since : 2011.11.29 
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	获取动态库属性
	//	依次是名称、版本、依赖的kernel版本、依赖的adapter版本，大小端。

	//	除第一个参数外其余参数如果不需要获取，可以传0。
	
	typedef struct {
	
	 	//	author : Jelo
		//	since : 2011.5.12
		// 	(C) 2011 , PKIG Tech. Co., Ltd.

		//	注意：该结构只有本接口使用，
		//	对应动态库文件结构1号版本。
		
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

	#define i51_LOADER_OBJVERSION 1  

	iBOOL ret = iFALSE ;
	i51HEADER i51header = { 0 } ;
	iFILE filehd = 0 ;
	
	filehd = i51AdeOsFileOpen ( filename , i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;

	if ( 0 == filehd ) return 0 ;
	
	//	skip crc
	ret = i51AdeOsFileSetSeek ( filehd , 4 , i51_OS_FL_SEEK_HEAD ) ;

	if ( iFALSE == ret ) {
		i51AdeOsFileClose ( filehd ) ;
		i51AdeOsLog ( i51_LOG_SC , "LOADER : Big E With i51AdeOsFileSetSeek" ) ;
		return iFALSE ;
	}

	ret = i51AdeOsFileRead ( filehd , &i51header , sizeof(i51HEADER) , 0 ) ;

	if ( iFALSE == ret ) {
		i51AdeOsFileClose ( filehd ) ;
		i51AdeOsLog ( i51_LOG_SC , "LOADER : Big E With i51AdeOsFileRead" ) ; 
		return iFALSE ;
	}

	if ( 0 != name ) {
		strcpy ( name , i51header.Name ) ;	
	}

	if ( 0 != Version ) {
//	Jelo Edited 20120307
		*(Version) = i51header.Weight & 0xffff ;
//		*(Version) = i51header.Weight ;
//	Jelo Edited Finished
	}

	if ( 0 != Kernel ) {	
		*(Kernel) = i51header.Kernel ;
	}

	if ( 0 != Adapter ) {
		*(Adapter) = i51header.Adapter ;
	}

	if ( 0 != Endian ) {
		*(Endian) = i51header.Endian & 0x1 ;
	}
	
	i51AdeOsFileClose ( filehd ) ;

	return iTRUE ;
	
}

iBOOL i51KitStdGetReVersion ( iU16* name , iU32 *version )
{
	//	author : william
	//	since : 2011.12.7
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	获取re版本号
	//	parameter:(1)应用名(2)版本buffer

	iFILE file = 0 ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdGetReVersion S , %x , %x" , name ,version) ;
	#endif	
	
	iASSERT ( name && version ) ;
	file = i51AdeOsFileOpen ( name , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
	
	if ( file <= 0) {

		iCHAR filename8bit[64] = {0} ;
		i51AdeStdUnicode2Ascii (name , filename8bit) ;
		i51AdeOsLog ( i51_LOG_DC , "KIT - STD : i51KitStdGetReVersion F, %s" , filename8bit ) ;
		return iFALSE ;
	}	
	
	i51AdeOsFileSetSeek ( file , sizeof(iU32) + 2 , i51_OS_FL_SEEK_HEAD) ;
	i51AdeOsFileReadInt32 ( file , version ) ;
	i51AdeOsFileClose(file) ;

	#ifdef i51KITSTD_DEBUG
	i51AdeOsLog ( 0 , "i51KitStdGetReVersion E , ret = %x" , iTRUE ) ;
	#endif
	
	return iTRUE ;

}


iKIT iBOOL i51KitMain ( iS32 kitid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	
	switch ( message )
	{
		case i51_MSG_INIT :
		{

			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
	
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

	//	TODO : Initialize API Port for external layer

	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(Std)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResOpen)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResDelete)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResGetTextSize) 
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResGetTextIDString) 
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResGetText)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResClose)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResGetContentSize)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResGetContentIDString)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdResGetContentEntry)
		//	Jelo Added Start 2011.11.29
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdGetKitRef)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdGetKitAttr)		
		//	Jelo Added Finish
		i51KIT_DYNAMIC_KIT_MAPING(i51KitStdGetReVersion)		
	i51KIT_DYNAMIC_KIT_MAPING_END 

	return i51KIT_DYNAMIC_KIT_GET_MAPING(Std) ;

	return 0 ;

}


