
#include "i51KitAde.h"
#include "i51KitCore.h"

#include "i51KitCoreZip.h"
#include "i51KitCoreDl.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_STATIC_KIT_DEF(CoreAdapter);

#define __align(X)

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

typedef struct
{
	//	author : william
	//	since : 2012.4.17
	// 	(C) PKIG Tech. Co., Ltd.	

	//	下载器已经被创建情况，用定时器通知上层
	//	用来异步通知启用的定时器id
	// Install 定时器
	iU32 Installtimerid ;
	//	Reinstall 定时器
	iU32 Reinstalltimerid ;
	//	获取sn
	iU32 GetSntimerid ;
	//	安装应用名称
	iCHAR InstallAppname[64] ;
	//	更新应用名称
	iCHAR ReinstallAppname[64] ;
	//	install 回调
	void (*IntallCallback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount  ) ;
	//	Reinstall回调
	void (*ReinstallCallback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount  ) ;
	//	回去sn回调
	void (*GetSncallback)( iBOOL results , void* buffer , iU32 buflen ) ;

}I51DOWNLOADERRUNING ;

//	Max References of Kit
#define i51_KERNEL_iCL_MAX_REF 16

typedef struct {

	//	需要下载的列表，为在文件上的目标，真正下载的时候需要转换成下载路径
	iCHAR i51KernelInstallNeedDlList[i51_KERNEL_iCL_MAX_REF][64]  ;
	//	需要下载的版本id
	iU32 i51KernelInstallNeedDlVerionID[i51_KERNEL_iCL_MAX_REF] ;
	//	不需要解压列表,第一个为不要解压的个数，后面不需要解压的下标
	iU32 i51KernelNoUmpressIndex[i51_KERNEL_iCL_MAX_REF+1]  ;
	//	下载后的目标路径
	//	iCHAR i51KernelInstallDestPath[i51_KERNEL_iCL_MAX_REF][64] ;
	//	应用名称
	iCHAR i51KernelAppName[64] ;
	//	完成安装需要需要文件的总大小
	iU32 i51KerneltotalSize ;
	//	已经下载的大小
	iU32 i51KerneldlSize ;
	//	需要下载的个数
	iU32 i51KernelInstallNeedDlCount ;
	//	正在下载现在下载列表中第几个1开始
	iU32 i51KernelDlIndex ;
	//	解压开始下标
	iU32 i51KernelStartUnpressIndex ;
	//	从服务器得到需要下载的个数
	iU32 i51KernelFromServerDlCount ;
	//	安装时的状态码
	iS32 state ;
	//	安装时模式 0 安装1 更新
	iS32 i51KernelInstallMode ;
	// 更新地址
	iCHAR i51KernelUri[32] ;

} i51INSTALLER ;

static i51INSTALLER i51KernelInstaller = { 0 } ;
static I51DOWNLOADERRUNING i51DownloaderRunning = {0} ;
//	外部下载传进回调
static void (*KernelExternDLCallback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount  ) ;

static void i51KernelDlStart ();

static iBOOL i51SlLoaderGetAttr ( iFILE filehd , iCHAR* name , iU32* Version , iU32* Kernel , iU32* Adapter , iU32* Endian ) 
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

static void i51SlLoaderGetKitRef ( iFILE file , i51SllDKITREF* i51SllDKitRefBuf , iU32 MaxRef , iU32* i51SllDKitRefTotall ) {

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

static iBOOL i51KernelGetKitVersionExt ( iU16* RePath  , iU32 VersionID )
{
	//	author : Willam
	//	since : 2011.10.31
	// 	(C) PKIG Tech. Co., Ltd.	

	//	获取re文件的资源版本号，绝对路径

	iU32 version = 0 ;
	iFILE file = 0 ;
	iBOOL ret = iFALSE ;

 
	file = i51AdeOsFileOpen( RePath , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN  , __FILE__ , __LINE__ ) ;
	
	if ( 0 == file ) {

		//	Jelo Edited Start 2011.12.1
		//	iCHAR filepath8bit[64] = {0} ;
		//	i51AdeStdUnicode2Ascii (RePath,filepath8bit) ;
		//	i51AdeOsLog ( i51_LOG_SC , "KEL : Kit Not Found , %s" , filepath8bit ) ;

		if ( i51AdeOsFileIsExist ( RePath  ) ) {

			//	文件已经存在，故打开失败，可认为版本匹配
			i51AdeOsLog ( i51_LOG_SC , "KEL : Re is Exist" ) ;
			return iTRUE ;
		}
		
		#ifdef i51KERNEL_DEBUG	
		{
			iCHAR buffer [ 64 ] = { 0 } ;
			i51AdeStdUnicode2Ascii ( RePath , (iCHAR* ) buffer ) ;
			i51AdeOsLog ( i51_LOG_SC , "KEL : Kit '%s' Not Found" , buffer ) ;
		}
		#else
		{
			i51AdeOsLog ( i51_LOG_SC , "KEL : Kit 0x%x Not Found" , RePath ) ;
		}
		#endif
		//	Jelo Edited Finish


		return iFALSE ;
	}
	
	ret = i51SlLoaderGetAttr ( file , 0 , &version , 0 , 0 , 0 ) ;
	
	i51AdeOsFileClose(file) ;

	if ( iFALSE == ret ) {
		return iFALSE ;
	}

	if ( version >= VersionID )
		return iTRUE ;
	
	return iFALSE ;

}

static iBOOL i51KernelGetReVersion ( iU16* RePath , iU32 VersionID )
{
	//	author : Willam
	//	since : 2011.10.31
	// 	(C) PKIG Tech. Co., Ltd.	

	//	获取re文件的资源版本号，绝对路径

	iU32 length = 0 ;
	iU32 version = 0 ;
	iFILE file = 0 ;

	iASSERT ( length < sizeof(RePath) ) ;	
			
	file = i51AdeOsFileOpen ( RePath , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN  , __FILE__ , __LINE__ ) ;
 
	if ( 0 == file  ) {

		if ( i51AdeOsFileIsExist ( RePath  ) ) {

			//	文件已经存在，故打开失败，可认为版本匹配
			i51AdeOsLog ( i51_LOG_SC , "KEL : Re is Exist" ) ;
			return iTRUE ;
		}
		i51AdeOsLog ( i51_LOG_SC , "KEL : Kit Not Found ,  %d " , file ) ;
		return iFALSE ;
	}

	// src 4, magic 2
	i51AdeOsFileSetSeek ( file , sizeof(iU32) + 2 , i51_OS_FL_SEEK_HEAD) ;
	i51AdeOsFileReadInt32 ( file , &version ) ;
	
	i51AdeOsFileClose(file) ;

	if ( version >= VersionID )
		return iTRUE ;
	
	return iFALSE ;
            	
}

static iBOOL i51KernelCheckVersionIsMatch ( iU16 *path , iU32 VersionId )
{

        //	author : Willam
        //	since : 2011.10.31
        // 	(C) PKIG Tech. Co., Ltd.	

        //  检测版本号是否匹配,绝对路径

        iCHAR AscPath[50] = {0} ;
        iCHAR *p = iNULL  ;
        iBOOL ret = iFALSE ;    
        i51AdeStdUnicode2Ascii ( path , AscPath) ;
        if ( (p = strstr ( AscPath , ".re")) ) {

            if ( i51KernelGetReVersion (path , VersionId) )
                ret = iTRUE ;

        } else if ( (p = strstr ( AscPath , ".i51")) || (p = strstr ( AscPath , ".kit")) ) {

                if ( i51KernelGetKitVersionExt (path , VersionId) ) {

                    ret = iTRUE ;
                }

        }else {

		   i51AdeOsLog ( i51_LOG_SC , "KEL : Ver N Macth " ) ;
                ret = iFALSE ;
    
        }

        return ret ;

}

static iBOOL i51KernelCheckEndianIsMatch ( iU16 *path )
{

        //	author : Willam
        //	since : 2011.10.31
        // 	(C) PKIG Tech. Co., Ltd.	

        //  检测大小端是否匹配,绝对路径

        iCHAR AscPath[50] = {0} ;
        iCHAR *p = iNULL  ;
        iBOOL ret = iTRUE ;    
	  iFILE file = 0;
	  iU32 Endian = 0 ;
        i51AdeStdUnicode2Ascii ( path , AscPath) ;
        if ( (p = strstr ( AscPath , ".re")) ) {

		return iTRUE ;

        } 

	file = i51AdeOsFileOpen( path , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN  , __FILE__ , __LINE__ ) ;

	if ( file == 0 )
		return iFALSE ;
	
	 if ( (p = strstr ( AscPath , ".i51")) || (p = strstr ( AscPath , ".kit")) ) {

               i51SlLoaderGetAttr ( file , 0 , 0 , 0 , 0 , &Endian ) ;
		  if ( Endian !=  i51AdeCnfGetEndian() ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : Endian N Macth" ) ;
                    ret = iFALSE ;
		  }
					
        }else {

		   i51AdeOsLog ( i51_LOG_SC , "KEL : Endian F" ) ;
                ret = iFALSE ;
    
        }

	i51AdeOsFileClose(file) ;
        return ret ;

}

static iBOOL i51KernelCheckPackage ( iFILE handle ) {

	//	author : william
	//	since : 20111023
	//	PKIG Tech. Co., Ltd.

	//	检测安装包的完整性

	//	crc最大的检验的时候分配的内存
	#define RES_MALLOC_SIZE	(1024*50)

	iS32 Filesize = 0 ;
	iS32 ReadSize = 0 ;
	iS32 size = 0 ;
	iU64 CRCValue = 0 ;
	iU32 nCRC = 0xFFFFFFFF ;
	iU8* data = NULL ;
	iBOOL ret = iTRUE ;
	iU32 *CRC32Table = iNULL ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelCheckPackage S" ) ;
	#endif
	
	i51AdeOsFileGetSize ( handle , &Filesize) ;
	Filesize = Filesize - sizeof (iU32) ;
	data = (iU8*)i51AdeOsMalloc ( RES_MALLOC_SIZE ,__FILE__ ,__LINE__) ;
	if ( data == iNULL )
		return iFALSE ;
	i51AdeOsFileReadInt32 ( handle , (iU32*)&CRCValue) ;

	i51AdeStdCrc32 ( 0 , 0 ,(iU32**) &CRC32Table ) ; 
	
	while ( ReadSize <  Filesize   ) {

		if ( Filesize - ReadSize >= RES_MALLOC_SIZE ) {

			size = RES_MALLOC_SIZE ;

		} else {

			size =  Filesize - ReadSize ;
		
		}
		
		i51AdeOsFileRead ( handle , data , size , 0 ) ;
		
		{
			iS32 i = 0 ;
			for( i = 0 ; i < size ; i ++ ) {
					
				nCRC = ( nCRC >> 8 ) ^ CRC32Table[ ((data[i]+0)&0xFF) ^ ( nCRC & 0x000000FF ) ];
				
			}			
		}

		ReadSize += size ;
	
	}
	nCRC = nCRC ^ 0xFFFFFFFF ;

	if ( nCRC != CRCValue ) {
		
		ret =  iFALSE ;

	}
	
	i51AdeOsFree ( data ) ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelCheckPackage E" ) ;
	#endif
	
	return ret ;


}


//	For i51KernelInstallApplication、i51KernelReinstallApplication
//
static iBOOL i51KernelParseInternalPackageCDT ( iFILE handle , iU16* AppName , void* CompressBuffer , void* UncompressBuffer , iU32 uncompressBufferLength ,iU32 compressBufferLength)
{
	//	author : william
	//	since : 20111024
	//	PKIG Tech. Co., Ltd.

	//	解析 CDT，相对于i51l路径

	//	原始文件名长度
	#define SOURCEFILE_NAME_LEN_MAX	32

	//	原文件名
	iCHAR FileName[SOURCEFILE_NAME_LEN_MAX+1] = {0} ;
	iU16 UcsFileName[SOURCEFILE_NAME_LEN_MAX+1] = {0} ;
	iU16 path[128] = {0} ;

	//	分段数
	iU32 FiledCount = 0 ;
	//	原始数据长度
	iU32 UnCompressLen = 0 ; 
	//	压缩后数据长度
	iU32 CompressLen = 0 ;
	iU32 UnCompressLen2 = uncompressBufferLength ;
	iU16 i ;
	iFILE file ;
	iBOOL UnCompressFlag ;


	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelParseInternalPackageCDT S" ) ;
	#endif
	
	i51AdeOsFileRead ( handle , FileName , SOURCEFILE_NAME_LEN_MAX , 0 ) ;
	i51AdeOsFileReadInt32 ( handle , &FiledCount ) ;

	i = FiledCount ;

	i51AdeStdAscii2Unicode ( FileName , UcsFileName ) ;
	i51AdeStdWStrcpy ( path , AppName  ) ;
	i51AdeStdWStrcat ( path , L"\\" ) ;
	i51AdeStdWStrcat ( path , UcsFileName  ) ;

	file = i51AdeOsFileOpen ( path , i51_OS_FL_WRITE | i51_OS_FL_ALWAYS_CREATE, __FILE__, __LINE__) ;

	if ( file <= 0 ) {

		//	Jelo Edited Start 2011.12.1
		//	iCHAR filepath8bit[64] = {0} ;
		//	i51AdeStdUnicode2Ascii (path,filepath8bit) ;
		//	i51AdeOsLog ( i51_LOG_SC , "i51KernelParsePackageCDT File Open F , %s" , filepath8bit) ;	
		i51AdeStdUnicode2Ascii (path,(iCHAR* )UcsFileName) ;
		i51AdeOsLog ( i51_LOG_SC , "KEL : Create '%s' , %d F" , UcsFileName , file) ;			
		//	Jelo Edited Finish
			
		return iFALSE ;
		
	}
	i51AdeStdUnicode2Ascii (path,(iCHAR* )UcsFileName) ;
	i51AdeOsLog ( i51_LOG_SC , "KEL : Pack-Unc Create '%s, i=%d' " , UcsFileName, i ) ;	
		
	while ( i-- ) {

		UnCompressLen2 = uncompressBufferLength ;
		memset ( CompressBuffer, 0 , compressBufferLength ) ;
		memset ( UncompressBuffer, 0 , uncompressBufferLength ) ;
		i51AdeOsFileReadInt32 ( handle ,&UnCompressLen ) ;	
		i51AdeOsFileReadInt32 ( handle ,&CompressLen ) ;
		i51AdeOsFileRead ( handle , CompressBuffer , CompressLen , 0 ) ;
		UnCompressFlag = i51AdeStdUncompress ( (iU8*)UncompressBuffer , &UnCompressLen2 , (iU8*)CompressBuffer , CompressLen ) ;
		
		//i51AdeOsLog(0, "### UnCompressLen = %d, UnCompressLen2=%d, UnCompressFlag=%d", UnCompressLen, UnCompressLen2, UnCompressFlag);
		if (  (UnCompressLen2 == UnCompressLen) && UnCompressFlag ) {
  			
			//	存入文件
			if ( !i51AdeOsFileWrite ( file , UncompressBuffer , UnCompressLen2 , 0 ) ) {

				i51AdeOsLog ( i51_LOG_SC , "KEL : Pack-Unc Write F") ;	
				return iFALSE ;
			}
			i51AdeOsFileFlush ( file ) ;		

		} else {

			//	解压数据有问题关闭文件，删除文件
			
			i51AdeOsFileClose ( file ) ;
			i51AdeOsFileDelete (path) ;
			i51AdeOsLog ( i51_LOG_SC , "KEL : Pack-Unc F\r\n") ;					
			return iFALSE ;
			
		}
	
	}

	//	关闭文件
	i51AdeOsFileClose ( file ) ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelParseInternalPackageCDT E" ) ;
	#endif
	
	return iTRUE ;

}

static iS32 i51KernelPlanDlRef ( iU16 *Path )
{
	//	author : Willam
	//	since : 2011.10.29
	// 	(C) PKIG Tech. Co., Ltd.	

	//	为开始下载依赖做准备工作
	//	return -1 为失败，0 需要下载依赖	1 为没有依赖可下载
	//	此处打开文件用绝对路径

	iFILE file ;
	i51SllDKITREF i51SllDKitRef [ i51_KERNEL_iCL_MAX_REF ] = { 0 } ;
	iS32 ret = 0 ;
	iU32 DefCount = 0 ;
	iU32 n = 0 , m = 0 ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Plan Dl Ref S") ;

	//i51AdeOsFileAModeEnable () ;
	//i51FILEOPEN (file ,Path ,i51_OS_FL_READ |i51_OS_FL_EXISTING_OPEN) ;
	i51AdeOsFileAModeEnable ();
	file = i51AdeOsFileOpen ( Path , i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
	i51AdeOsFileAModeDisable () ;
	if ( file <= 0 ) {

		iCHAR OpenFileName[128] = {0} ;
		i51AdeStdUnicode2Ascii ( Path , OpenFileName ) ;
		i51KernelInstaller.state = -i51_KERNEL_INSTALL_ERROR_FILE ;
		i51AdeOsLog ( i51_LOG_SC , "Open '%s' F\r\n" , OpenFileName ) ;
		return -1 ;

	}

	//	Jelo Edit 2011.12.31
	//	获取到依赖文件列表
	//if (i51SlLoaderGetKitRef( file, i51SllDKitRef , i51_KERNEL_iCL_MAX_REF ,&DefCount ) ) {
	i51SlLoaderGetKitRef( file, i51SllDKitRef , i51_KERNEL_iCL_MAX_REF ,&DefCount ) ;
	if ( 0 < DefCount ) {
	//	Jelo Edit Finished  
	  
		n = DefCount ;
		while ( n-- )  {

			//	检测任务是否已经加入下载列表中，防止两文件依赖同一个文件情况
			for (m = 0 ; m <  i51KernelInstaller.i51KernelInstallNeedDlCount ;m++) {

				if ( strcmp ( i51KernelInstaller.i51KernelInstallNeedDlList[m] ,  i51SllDKitRef[n].path ) == 0 )
					break ;

			}
			if ( m == i51KernelInstaller.i51KernelInstallNeedDlCount ) {
				
				strcpy ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] , i51SllDKitRef[n].path ) ;
				i51KernelInstaller.i51KernelInstallNeedDlVerionID[i51KernelInstaller.i51KernelInstallNeedDlCount] = i51SllDKitRef[n].version ;
				i51KernelInstaller.i51KernelInstallNeedDlCount ++ ;
			}
		}
		
	} else {

		ret = 1 ;
	}
	i51AdeOsFileClose (file) ;
	//i51AdeOsFileAModeDisable () ;
	i51AdeOsLog ( i51_LOG_SC , "KEL : Plan Dl Ref E") ;
	return ret ;
	

}

static iBOOL i51KernelInstallGetRefToDlList ()
{
	//	author : Willam
	//	since : 2011.11.3
	// 	(C) PKIG Tech. Co., Ltd.	


	//	获取依赖，并且添加到下载列表
#if 0	
	iFILE file ;
	iU16 PackagePath[128]  ;
	iU32 n = i51KernelInstaller.i51KernelStartUnpressIndex ;
	iBOOL ret = iTRUE ;
	iCHAR *p1 , *p2 ;
	iCHAR path[50] ;
	iCHAR filename8bit[50] ;
	iU16 filename16bit[50] ;
	//	解压之后的文件名
	iCHAR unpressFileName[50] = {0} ;
	

	while ( n < i51KernelInstaller.i51KernelDlIndex) {

	
		if (  i51KernelInstaller.i51KernelNoUmpressIndex[0] ) {

			//	判断是否有不需要解压的列表在内
			iU16 i = 0 ;
			for ( i = 0 ; i <  i51KernelInstaller.i51KernelNoUmpressIndex[0] ; i ++ ) {

				if ( n == i51KernelInstaller.i51KernelNoUmpressIndex[i+1] )
					break ;

			}
			if ( i != i51KernelInstaller.i51KernelNoUmpressIndex[0] ) {

				n ++ ;
				continue ;
			}

		}
		memset ( PackagePath , 0 , sizeof(PackagePath) ) ;
		memset ( path , 0 , sizeof(path) ) ;

		memset ( filename8bit , 0 , sizeof(filename8bit) ) ;
		memset ( filename16bit , 0 , sizeof(filename16bit) ) ;



		p1 = i51KernelInstaller.i51KernelInstallNeedDlList[n] ;
		while ( (p2 = strstr ( p1 , "\\" ) ) ) {

		//	跳过\\，并且赋值给p3继续检测
		p1 = ++p2 ;

		}
		strcpy ( unpressFileName , p1) ;
		//	如下载列表的路径为i51kit\\pkingapi\\i51kitstd.kit  path 为i51kit\\pkigapi
		strncpy ( path , i51KernelInstaller.i51KernelInstallNeedDlList[n] , p1-i51KernelInstaller.i51KernelInstallNeedDlList[n]-1) ;
		strcpy ( filename8bit , i51KernelInstaller.i51KernelAppName) ;
		strcat ( filename8bit , "\\") ;
		strcat ( filename8bit , p1 ) ;
		strcat ( filename8bit , ".package" ) ;
		i51AdeStdAscii2Unicode ( filename8bit , filename16bit ) ;

		file = i51AdeOsFileOpen ( filename16bit , i51_OS_FL_READ |i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
		if ( file <= 0 ) {
			
			iCHAR OpenFileName[128] = {0} ;
			i51AdeStdUnicode2Ascii ( filename16bit , OpenFileName ) ;
			i51AdeOsLog ( i51_LOG_SC , "i51KernelInstallUnpressAndAddDlList %s open fail\r\n" , OpenFileName) ;
			ret = iFALSE ;
			break ;
			
		} else {
			//	解压到指定位置
			if (i51KernelExtractInternalPackage ( file , path) ) {

				//	解压成功
				
				iCHAR filepath[50] = {0} ;
				iU16 path[128] = {0} ;
				iS32 GetRefRet = 0 ;
				
				sprintf ( filepath , "%s\\%s" , i51KernelInstaller.i51KernelAppName , i51KernelInstaller.i51KernelAppName ) ;
				if ( strcmp ( i51KernelInstaller.i51KernelAppName , unpressFileName ) == 0 ) {
					//解压成功后，如果是i51
						i51AdeStdWSprintf ( path ,
							sizeof(path)/sizeof(path[0]) ,
							"%si51\\%s.i51" ,
							i51AdeOsGetPathSymbolEx() ,
							i51KernelInstaller.i51KernelInstallNeedDlList[n]) ;
						
						GetRefRet = i51KernelPlanDlRef (path) ;

				} else {
					//解压成功后，如果是kit ,re
					if (!strstr (i51KernelInstaller.i51KernelInstallNeedDlList[n] , ".re")) {
						i51AdeStdWSprintf ( path ,
							sizeof(path)/sizeof(path[0]) ,
							"%si51\\%s" ,
							i51AdeOsGetPathSymbolEx() ,
							i51KernelInstaller.i51KernelInstallNeedDlList[n]) ;
						GetRefRet = i51KernelPlanDlRef (path) ;
					}

				}

				if ( GetRefRet == -1 ) {

					//	获取依赖失败
					i51AdeOsLog ( i51_LOG_SC , "i51KernelInstallUnpressAndAddDlList get ref fail\r\n") ;
					ret = iFALSE;

				}
			
			}else {

				//	解压失败
				i51AdeOsLog ( i51_LOG_SC , "i51KernelInstallUnpressAndAddDlList compress fail\r\n") ;
				ret = iFALSE ;
				
			}
		}	
		if ( file > 0 ) {
			i51AdeOsFileClose(file) ;
		}
//		if (!ret) {
			//	只有在解压错误的时候，才会去删除解压文件，否则最后统一删除
			i51AdeOsFileDelete(filename16bit) ;
//		}
		if (!ret)
			break ;
		n ++ ;	
	}

	if ( ret  )
		i51KernelInstaller.i51KernelStartUnpressIndex = i51KernelInstaller.i51KernelDlIndex;
	return ret ;

	#else
	iCHAR *p1 = NULL , *p2 = NULL ;
	iU16 path[64] = {0} ;
	iCHAR unpressFileName[64] = {0} ;
	iS32 GetRefRet = 0 ;
	iU32 n = i51KernelInstaller.i51KernelStartUnpressIndex ;
	iBOOL ret = iTRUE ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Get Ref List S") ;

	p1 = i51KernelInstaller.i51KernelInstallNeedDlList[n] ;
	while ( (p2 = strstr ( p1 , "\\" ) ) ) {

	//	跳过\\，并且赋值给p3继续检测
	p1 = ++p2 ;
	}
	strcpy ( unpressFileName , p1) ;
	while ( n < i51KernelInstaller.i51KernelDlIndex) {

		if ( strcmp ( i51KernelInstaller.i51KernelAppName , unpressFileName ) == 0 ) {
			//解压成功后，如果下载是应用，下载列表中是没有后缀
				i51AdeStdWSprintf ( path ,
					sizeof(path) ,
					"%s%s.i51" ,
					i51KernelGeti51Path () ,
					i51KernelInstaller.i51KernelInstallNeedDlList[n]) ;
				GetRefRet = i51KernelPlanDlRef (path) ;
		} else {
			//解压成功后，如果是kit ,i51
			if (strstr (i51KernelInstaller.i51KernelInstallNeedDlList[n] , ".i51") ||
				strstr (i51KernelInstaller.i51KernelInstallNeedDlList[n] , ".kit")) {
				//	re没有依赖，不参与依赖获取
				i51AdeStdWSprintf ( path ,
					sizeof(path) ,
					"%s%s" ,
					i51KernelGeti51Path() ,
					i51KernelInstaller.i51KernelInstallNeedDlList[n]) ;
				GetRefRet = i51KernelPlanDlRef (path) ;
			}
		}
		if ( GetRefRet == -1 ) {

			//	获取依赖失败
			i51AdeOsLog ( i51_LOG_SC , "KEL : Get Ref F\r\n") ;
			ret = iFALSE;
		}	
		if (!ret)
			break ;
		n ++ ;	
	}
	if ( ret  )
		i51KernelInstaller.i51KernelStartUnpressIndex = i51KernelInstaller.i51KernelDlIndex;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Get Ref List E") ;
	
	return ret ;

	#endif
}

static iBOOL i51KernelExtractInternalPackage ( iFILE handle , iCHAR *path , iU32 mode ) 
{

	//	author : william
	//	since : 20111024
	//	PKIG Tech. Co., Ltd.

	
	//	参数3 : 1为解压安装包,相对路径i51
	//	参数3 : 0为解压安装包,相对路径i51\\i51KIT

	//	每段压缩的原始数据最大为100K
	#define UncompressBufferLength	(1024*100)
	//
	#define compressBufferLength	(1024*100+128)
	iCHAR magic[7+1] = {0} ;
//	iU32 version ;
	iU32 CDTCount = 0 ;
	iU16 i = 0 ;
	iBOOL ret = iTRUE ;
	iU16 path16bit[128] = {0} ;

	void *UncompressBuffer = iNULL ;
	void *CompressBuffer = iNULL ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelExtractInternalPackage S" ) ;
	#endif

	i51AdeOsLog ( i51_LOG_SC , "KEL : Inter Ex Package , '%s' , %d" , path , mode ) ;

	//	Jelo Add 2011.12.31
	if ( 0 == handle ) {
		return iFALSE ;
	}
	//	Jelo Add Finished
	

	//	采用决定路径模式
	if ( mode )	
		i51AdeOsFileAModeEnable () ;
	do {

		//	Jelo Edit 2011.12.31
		//	iASSERT ( handle > 0 ) ;
		//	Jelo Edit Finished

		if ( mode )		
			i51AdeStdWSprintf ( path16bit ,
					sizeof(path16bit) ,
					"%s%s" ,
					i51KernelGeti51Path () ,
					path) ;
		else
			i51AdeStdAscii2Unicode (path , path16bit) ;

		if ( !i51AdeOsFileIsDirExist(path16bit)) {

			// 如果此应用的目录不存在，创建此目录
			i51AdeOsFileCreateDir (path16bit) ;

		}
		if ( !i51KernelCheckPackage (handle) ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : Pack is Broken\r\n") ;			
			ret =  iFALSE ;
			break ;

		}

		
		UncompressBuffer = i51AdeOsMalloc ( UncompressBufferLength ,__FILE__ ,__LINE__) ; 
		if ( UncompressBuffer == iNULL ) {
			
			i51AdeOsLog ( i51_LOG_SC , "KEL : Unc Mem F , %d\r\n" , UncompressBufferLength ) ;				
			ret =  iFALSE ;
			break ;  
		}

		CompressBuffer = i51AdeOsMalloc ( compressBufferLength ,__FILE__ ,__LINE__) ; 

		if ( CompressBuffer == iNULL ) {
			
			i51AdeOsLog ( i51_LOG_SC , "KEL : Com Mem F , %d\r\n" , compressBufferLength ) ;				
			ret =  iFALSE ;
			break ;  
		}  
		
		i51AdeOsFileSetSeek ( handle , 4 , i51_OS_FL_SEEK_HEAD ) ;
		i51AdeOsFileRead ( handle , magic , 7 , 0 ) ;
		
		if ( strcmp ( magic , "i51Pack" ) != 0  ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : Magic F\r\n") ;				
			ret =  iFALSE ;
			break ;

		}
		
//		i51AdeOsFileReadInt32 ( handle , &version ) ;
		//	跳过内容版本号，大小端，文件格式版本号
		i51AdeOsFileSetSeek (handle, 12, i51_OS_FL_SEEK_CURRENT) ;
		i51AdeOsFileReadInt32 ( handle , &CDTCount  ) ;
	

		i = CDTCount ;

		while ( i-- ) {

			if ( !i51KernelParseInternalPackageCDT ( handle , path16bit , CompressBuffer , UncompressBuffer , UncompressBufferLength , compressBufferLength ) ) {

				ret =  iFALSE ;
				break ;

			}
				
		}

	}while(0) ;
	if ( mode )	
		i51AdeOsFileAModeDisable() ;
	
	//	释放分配的内存
	if ( UncompressBuffer != iNULL ) {
		i51AdeOsFree ( UncompressBuffer ) ;
	}

	if ( CompressBuffer != iNULL ) {
		i51AdeOsFree ( CompressBuffer ) ;
	}

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelExtractInternalPackage E" ) ;
	#endif
	return ret ;
}
static void i51KernelExternDLCallbackDo ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount )
{

	//	安装时回调返回上层在此处

//	i51AdeOsTimerStop (id) ;


	i51AdeOsLog ( i51_LOG_SC , "KEL : Dl-Do , '%s' , %d , %d , %d , %d , %d" , currentstep , state , step , total , curdl , dlcount ) ;
	
	if (state == i51_KERNEL_INSTALL_FINISHED) {

		memset (&i51KernelInstaller , 0 , sizeof(i51KernelInstaller)) ;
		i51KitCoreSlDlStop(0) ;
	} else if ( state  < 0) {

		i51KitCoreSlDlStop(0) ;
	}

	KernelExternDLCallback (currentstep , 
						state,
						step,
						total ,
						curdl ,
						dlcount ) ;
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Dl-Do E") ;
	
}

static void i51KernelInternalDlCallback (  iS32 state , iU32 textlen , iU32 offset , iU32 totall , iU32 refcount )
{
	//	author : Willam
	//	since : 2011.10.29
	// 	(C) PKIG Tech. Co., Ltd.	

	//	内部调用下载依赖回调


	//	列表第一次讲需要下载个数保存(本次+ 依赖)

	iCHAR *p1 = NULL , *p2 = NULL ;
	iCHAR stepName[32] = {0} ;

	if ( state == i51_SL_DL_ERROR_AGAIN_DLSTART ) {

		//	重新发送dl start 请求
		i51KernelDlStart() ;
		return ;
	}
	p1 = i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelDlIndex] ;
	if ( i51KernelInstaller.i51KernelDlIndex == 0 )  {
		
		i51KernelInstaller.i51KernelFromServerDlCount = refcount + 1 ;
	}
	i51KernelInstaller.i51KerneltotalSize = totall ;
	i51KernelInstaller.i51KerneldlSize =  offset ;
			
	if ( i51_SL_DL_FINISHED == state ) {
		
		while ( p2 = strstr ( p1 , "\\" ) ) {

			p2 ++ ;
			p1 = p2 ;
		}
		strcpy ( stepName , p1) ;

		
/**********下载完后，先解压在进行下面操作***********************/
{
		iBOOL UnpressRet = iTRUE ;
		iCHAR filename8bit[64] = {0} ;
		iU16 filename16bit[64] = {0} ;
		iCHAR UnpressDestPath[64] = {0} ;
		iFILE file = NULL ;

		if (  i51KernelInstaller.i51KernelNoUmpressIndex[0] ) {

			//	判断是否有不需要解压的列表在内
			iU16 i = 0 ;
			for ( i = 0 ; i <  i51KernelInstaller.i51KernelNoUmpressIndex[0] ; i ++ ) {

				if ( i51KernelInstaller.i51KernelDlIndex == i51KernelInstaller.i51KernelNoUmpressIndex[i+1] )
					break ;

			}
			if ( i != i51KernelInstaller.i51KernelNoUmpressIndex[0] ) {

				
				goto BREAK_UMPRESS ;
			}

		}
		
		strcpy ( filename8bit , i51KernelInstaller.i51KernelAppName) ;
		strcat ( filename8bit , "\\") ;
		strcat ( filename8bit , stepName ) ;
		strcat ( filename8bit , ".package" ) ;
		i51AdeStdAscii2Unicode ( filename8bit , filename16bit ) ;

		file = i51AdeOsFileOpen ( filename16bit , i51_OS_FL_READ |i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;
		if ( file <= 0 ) {
			
			iCHAR OpenFileName[64] = {0} ;
			i51AdeStdUnicode2Ascii ( filename16bit , OpenFileName ) ;

			i51AdeOsLog ( i51_LOG_SC , "KEL : DL-Cb '%s' open F , %d " , OpenFileName , file) ;

			i51KernelExternDLCallbackDo ( stepName , 
			i51_KERNEL_INSTALL_ERROR_FILE , 
			i51KernelInstaller.i51KerneldlSize, 
			i51KernelInstaller.i51KerneltotalSize ,
			i51KernelInstaller.i51KernelDlIndex + 1 ,
			i51KernelInstaller.i51KernelFromServerDlCount) ;
			UnpressRet = iFALSE ;
			goto UNPRESS_END ;
		} 
			//	解压到指定位置
		strncpy ( UnpressDestPath ,
				i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelDlIndex] ,
				p1-i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelDlIndex] -1) ;
		if (!i51KernelExtractInternalPackage ( file , UnpressDestPath , 1) ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : DL-Cb Uncompress F" ) ;

			i51KernelExternDLCallbackDo ( stepName , 
			i51_KERNEL_INSTALL_ERROR_UNPRESS , 
			i51KernelInstaller.i51KerneldlSize, 
			i51KernelInstaller.i51KerneltotalSize ,
			i51KernelInstaller.i51KernelDlIndex + 1 ,
			i51KernelInstaller.i51KernelFromServerDlCount) ;
			UnpressRet = iFALSE ;
			goto UNPRESS_END ;
		}

		UNPRESS_END:
		if (file)
			i51AdeOsFileClose(file) ;
		i51AdeOsFileDelete (filename16bit) ;
		if ( !UnpressRet )
			return ;
}


BREAK_UMPRESS:
/***********************************END**************************************/	

		i51KernelInstaller.i51KernelDlIndex ++ ;
		if ( i51KernelInstaller.i51KernelInstallNeedDlCount == i51KernelInstaller.i51KernelDlIndex ) {

			//	下载完成后，获取依赖
			
			iU16 AppName16Bit[64] = {0} ;
			i51AdeStdAscii2Unicode ( i51KernelInstaller.i51KernelAppName ,AppName16Bit) ;
			if (!i51KernelInstallGetRefToDlList ()) {
				i51KernelExternDLCallbackDo ( stepName , 
				i51_KERNEL_INSTALL_ERROR_FILE , 
				i51KernelInstaller.i51KerneldlSize, 
				i51KernelInstaller.i51KerneltotalSize ,
				i51KernelInstaller.i51KernelDlIndex ,
				i51KernelInstaller.i51KernelFromServerDlCount) ;
				return ;
			}
			//	解压以及依赖检测做完之后，进行判断是否还学下载
			if ( i51KernelInstaller.i51KernelInstallNeedDlCount == i51KernelInstaller.i51KernelDlIndex )  {

				//	已经全部下载完成
				i51KernelExternDLCallbackDo ( stepName , 
					i51_KERNEL_INSTALL_FINISHED,
					i51KernelInstaller.i51KerneldlSize,
					i51KernelInstaller.i51KerneltotalSize ,
					i51KernelInstaller.i51KernelFromServerDlCount ,
					i51KernelInstaller.i51KernelFromServerDlCount
					) ;
			} else {
			
				i51KernelExternDLCallbackDo (  stepName , 
					i51_KERNEL_INSTALL_RUNING ,
					i51KernelInstaller.i51KerneldlSize, 
					i51KernelInstaller.i51KerneltotalSize ,
					i51KernelInstaller.i51KernelDlIndex ,
					i51KernelInstaller.i51KernelFromServerDlCount
					) ;
				i51KernelDlStart () ;
			}
			
		}else {

			//	依赖下载未完成，下载下一个依赖
			i51KernelExternDLCallbackDo ( stepName , 
			i51_KERNEL_INSTALL_RUNING ,
			i51KernelInstaller.i51KerneldlSize,
			i51KernelInstaller.i51KerneltotalSize ,
			i51KernelInstaller.i51KernelDlIndex ,
			i51KernelInstaller.i51KernelFromServerDlCount) ;
			i51KernelDlStart () ;

		}


	} else {
		while ( p2 = strstr ( p1 , "\\" ) ) {

			p2 ++ ;
			p1 = p2 ;
		}
		strcpy ( stepName , p1) ;

		i51KernelExternDLCallbackDo ( stepName ,
			state , i51KernelInstaller.i51KerneldlSize, 
			i51KernelInstaller.i51KerneltotalSize ,
			i51KernelInstaller.i51KernelDlIndex + 1 ,
			i51KernelInstaller.i51KernelFromServerDlCount 
			) ;

	}


		

}

static void i51KernelDlStart ( )
{
	//	author : Willam
	//	since : 2011.11.1
	// 	(C) PKIG Tech. Co., Ltd.	

	//	下载下载列表中的内容
//	i51AdeOsFileAModeEnable () ;
//	i51AdeOsLog ( i51_LOG_SC , "i51KernelDlStart %d , %d " , i51KernelInstaller.i51KernelInstallNeedDlCount , i51KernelInstaller.i51KernelDlIndex ) ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Dl S , Totall : %d" , i51KernelInstaller.i51KernelInstallNeedDlCount ) ;

	while( i51KernelInstaller.i51KernelInstallNeedDlCount > i51KernelInstaller.i51KernelDlIndex) {

		iCHAR DlUrl[64] = {0} ;
		iCHAR DestPath[64] = {0} ;
		iCHAR DestName[64] = {0} ;
		iU16 FilePath[64] = {0} ;
		
		iCHAR *p1 , *p2 ;
		iBOOL isDown = iFALSE ;


		i51AdeOsLog ( i51_LOG_SC , "KEL : Check This : '%s'" , i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelDlIndex] ) ;

		p1 = i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelDlIndex] ;
		while ( (p2 = strstr ( p1 , "\\" ) ) ) {

			//	跳过\\，并且赋值给p3继续检测
			p1 = ++p2 ;

		}
		strcpy ( DlUrl , p1) ;

		//	开始下载时候，通知上层正在下载
		i51KernelExternDLCallbackDo ( DlUrl ,
		i51_KERNEL_INSTALL_RUNING, 
		0,
		0 ,
		i51KernelInstaller.i51KernelDlIndex + 1 ,
		i51KernelInstaller.i51KernelFromServerDlCount) ;

			
		/*
		strncpy ( DestPath , 
					i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelDlIndex] ,
					p1 - i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelDlIndex]) ; */
//		strcat ( DestPath , "\\" ) ;
		strcpy ( DestPath , i51KernelInstaller.i51KernelAppName );
		strcat ( DestPath , "\\" ) ;
		strcpy ( DestName , DlUrl ) ;
		strcat ( DestName , ".package" ) ;

		i51AdeStdWSprintf ( FilePath ,
					sizeof(FilePath) ,
					"%s%s" ,
					i51KernelGeti51Path() ,
					i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelDlIndex]) ;

		i51AdeOsFileAModeEnable () ;
		if ( (i51KernelInstaller.i51KernelDlIndex== 0) ||\
			!i51AdeOsFileIsExist(FilePath) || \
			!i51KernelCheckVersionIsMatch (FilePath , i51KernelInstaller.i51KernelInstallNeedDlVerionID[i51KernelInstaller.i51KernelDlIndex]) || \
			!i51KernelCheckEndianIsMatch (FilePath)
			) {	
			//	如果版本号不正确或者文件不存在或者是下载列表第一个
			i51AdeOsLog ( i51_LOG_SC , "KEL : Need DL '%s'" , DestName ) ;
			isDown = iTRUE ;

		} else {

				iS32 size ;
				iFILE file ;				

				i51AdeOsLog ( i51_LOG_SC , "KEL : Ref File Exist" ) ;
				
				file = i51AdeOsFileOpen ( FilePath , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN  , __FILE__ , __LINE__ ) ;
				if ( file <= 0 ) {

					iCHAR OpenFileName[64] = {0} ;
					i51AdeStdUnicode2Ascii ( FilePath , OpenFileName ) ;
					i51AdeOsLog ( i51_LOG_SC , "KEL : '%s' Open F ,  %d , Break Check" , OpenFileName , file ) ;
					i51KernelExternDLCallbackDo (DlUrl ,
						i51_KERNEL_INSTALL_ERROR_FILE, 
						i51KernelInstaller.i51KerneldlSize,
						i51KernelInstaller.i51KerneltotalSize ,
						i51KernelInstaller.i51KernelDlIndex + 1 ,
						i51KernelInstaller.i51KernelFromServerDlCount) ;
					break ;

				}
				i51AdeOsFileGetSize(file, &size) ;
				i51AdeOsFileClose(file) ;	
				
				//	讲不需要解压的索引加入到列表
				i51KernelInstaller.i51KernelNoUmpressIndex[0] = i51KernelInstaller.i51KernelNoUmpressIndex[0] + 1 ;
				i51KernelInstaller.i51KernelNoUmpressIndex[i51KernelInstaller.i51KernelNoUmpressIndex[0]] = i51KernelInstaller.i51KernelDlIndex ;
				
//				i51KernelInstaller.i51KernelDlIndex ++ ;
				
				if  ( i51KernelInstaller.i51KernelInstallNeedDlCount == i51KernelInstaller.i51KernelDlIndex + 1) {

					//	相当于列表中的信息下载完成

					//	此处--为了符合调用函数用的判断
//					i51KernelInstaller.i51KernelDlIndex -- ;
					//	为了保证对应，在break 之前加上关闭
					
					i51AdeOsLog ( i51_LOG_SC , "KEL : Break Check") ;
					
					i51AdeOsFileAModeDisable () ;
					i51KernelInternalDlCallback (
					i51_KERNEL_INSTALL_FINISHED , 
					size,
					size ,
					size ,
					i51KernelInstaller.i51KernelFromServerDlCount) ;

					break ;
					
				} else {

					i51AdeOsLog ( i51_LOG_SC , "KEL : Continue Check") ;
					
					//	如果已经存在，通知上层继续进行下一个下载
					i51KernelExternDLCallbackDo ( DlUrl ,
					i51_KERNEL_INSTALL_RUNING, 
					size,
					size ,
					i51KernelInstaller.i51KernelDlIndex + 1 ,
					i51KernelInstaller.i51KernelFromServerDlCount) ;

					//	下表加1
					i51KernelInstaller.i51KernelDlIndex ++ ;
					continue ;
				}
			
		}

		i51AdeOsFileAModeDisable () ;
		if ( isDown) {
			

			iCHAR uri[32+1] = {0} ;
			iCHAR *p1 = NULL , *p2 = NULL ;
			p1 = DlUrl ;
			p2 = strstr(p1 , ".")  ;
	
			if (p2) {
				
				strcpy ( uri , "ref://" ) ;
				strncat ( uri , p1 , p2 - p1 ) ;
			} else {

				if ( i51KernelInstaller.i51KernelInstallMode == 0 ) {

					//	安装
					strcpy ( uri , "app://" ) ;
					strcat ( uri , p1 ) ;
				} else  {

					//	更新
					strcpy ( uri ,  i51KernelInstaller.i51KernelUri ) ;
					
				}
			}

			i51AdeOsLog ( i51_LOG_SC , "KEL : Dl Start For '%s'" , uri ) ;
			
			i51KitCoreSlDlStart( 0 ,
				uri  , 
				strlen( uri  ) , 
				DestPath , 
				DestName  , 
				i51KernelInstaller.i51KernelInstallNeedDlVerionID[i51KernelInstaller.i51KernelDlIndex],
				i51_SAP_DLCOM_BITSTREAM , 
				i51KernelInternalDlCallback ) ;	
			
		}
		break ;
	} 
//	i51AdeOsFileAModeDisable () ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Dl E") ;

}


static void  i51KernelIntallUseDlFailCB (iU32 id)
{
	//	author : william
	//	since : 2012.4.17
	// 	(C) PKIG Tech. Co., Ltd.	

	
	//	Install互斥异步回调
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Install-Exclusived " ) ;
	
	if ( i51DownloaderRunning.Installtimerid )
		i51AdeOsTimerStop (i51DownloaderRunning.Installtimerid ) ;
	i51DownloaderRunning.Installtimerid  = 0 ;

	if ( i51DownloaderRunning.IntallCallback ) {

			i51DownloaderRunning.IntallCallback (  i51DownloaderRunning.InstallAppname ,
														 i51_KERNEL_INSTALL_ERROR_DOWNLOAER_USING , 
														 0 , 
														 0 ,
														 0 , 
														 0
														 ) ;
	}
	
}


static void  i51KernelReintallUseDlFailCB (iU32 id)
{
	//	author : william
	//	since : 2012.4.17
	// 	(C) PKIG Tech. Co., Ltd.	

	//	Reinstall互斥异步回调
	
	i51AdeOsLog ( i51_LOG_SC , "KEL : Reinstall-Exclusived " ) ;
	
	if ( i51DownloaderRunning.Reinstalltimerid)
		i51AdeOsTimerStop (i51DownloaderRunning.Reinstalltimerid ) ;
	i51DownloaderRunning.Reinstalltimerid  = 0 ;

	if ( i51DownloaderRunning.ReinstallCallback) {

			i51DownloaderRunning.ReinstallCallback (  i51DownloaderRunning.ReinstallAppname,
														 i51_KERNEL_INSTALL_ERROR_DOWNLOAER_USING , 
														 0 , 
														 0 ,
														 0 , 
														 0
														 ) ;
	}
}

static void KernelInstallCreateDlCallback ( iBOOL ret ) 
{
	//	author : Willam
	//	since : 2011.11.14
	// 	(C) PKIG Tech. Co., Ltd.	

	i51AdeOsLog ( i51_LOG_SC , "KEL : Install Create Dl Cb %d " , ret ) ;
	
	if (ret) {	

	#if 0
		iU16	PackagePath[50] = {0} ;
		iU16	i51Path[50] = {0} ;
		iU16 AppName[50] = {0};
//		iCHAR DlUrl[50] = {0} ;
		iCHAR DlName[50] = {0} ;
		iCHAR DlStorePath[50] = {0} ;
		iCHAR DlStoreName[50] = {0} ;
		iCHAR AppName8Bit[50] = {0};
		iBOOL interalRet = 0 ;

		if ( i51KernelInstaller.i51KernelFromServerDlCount != 0 ) {

				
			//继续下载或继续更新
			i51KernelDlStart () ;
			return ;
		}

		i51AdeStdAscii2Unicode ( i51KernelInstaller.i51KernelAppName , AppName  ) ;
		//	下载路径
//		i51AdeStdUnicode2Ascii ( AppName , DlUrl ) ;
		//	下载后的存储位置
//		strcpy ( DlStorePath ,  DlUrl ) ;
		strcpy ( DlStorePath ,  i51KernelInstaller.i51KernelAppName ) ;
		strcat ( DlStorePath , "\\") ;
		
		
		//	下载后文件名称
//		strcpy ( DlStoreName ,  DlUrl ) ;
		strcpy ( DlStoreName ,  i51KernelInstaller.i51KernelAppName ) ;
		strcat ( DlStoreName , ".package") ;	

		
		i51AdeStdWStrcpy ( PackagePath , AppName ) ;
		i51AdeStdWStrcat ( PackagePath , L"\\" ) ;
		i51AdeStdWStrcat ( PackagePath , AppName ) ;
		//	安装包路径
		i51AdeStdWStrcat ( PackagePath , L".package" ) ;
		//	i51路径
		i51AdeStdWStrcpy ( i51Path , i51KernelGetApplicationPath() ) ;
		i51AdeStdWStrcat ( i51Path , AppName ) ;
		i51AdeStdWStrcat ( i51Path , L"\\" ) ;
		i51AdeStdWStrcat ( i51Path , AppName ) ;
		i51AdeStdWStrcat ( i51Path , L".i51" ) ;

		if ( i51AdeOsFileIsExist (PackagePath)) {

			//如果存在，进行解压 
			iFILE file ;

			file = i51AdeOsFileOpen ( PackagePath , i51_OS_FL_READ |i51_OS_FL_EXISTING_OPEN , __FILE__ , __LINE__ ) ;

			if ( file <= 0 ) {

				iCHAR OpenFileName[64] = {0} ;
				i51AdeStdUnicode2Ascii ( PackagePath , OpenFileName ) ;
				i51KernelInstaller.state = i51_KERNEL_INSTALL_ERROR_FILE ;
				i51AdeOsLog ( i51_LOG_SC , "i51KernelInstallApplication %s open fail , %d " ,OpenFileName , file) ;
				interalRet = iFALSE ;
				
			} else {

				if (i51KernelExtractPackage ( file , AppName) ) {

					iS32 flag ;

					flag = i51KernelPlanDlRef ( i51Path ) ; 
					//	下载i51依赖
					if ( flag == -1 ) {
						//准备下载依赖失败
						interalRet = iFALSE ;
					} else if ( flag == 0 ){

						//	有依赖需要下载
						i51KernelDlStart () ;
						interalRet = iTRUE ;
					} else {

						//	无依赖需要下载安装完成
							KernelExternDLCallback (  i51KernelInstaller.i51KernelAppName ,
							i51_KERNEL_INSTALL_FINISHED , 0 , 0  ,
						i51KernelInstaller.i51KernelDlIndex ,
						i51KernelInstaller.i51KernelFromServerDlCount ) ;
						interalRet = iTRUE ;

					}	

				} else {
				
					interalRet = iFALSE ;	
				}
				i51AdeOsFileClose (file) ;
				
			}
//			if (!interalRet) {
				//	只有在解压错误的时候，才会去删除解压文件，否则最后统一删除
				i51AdeOsFileDelete (PackagePath) ;
//			}
			
		} else {

			//	不存在进行下载,添加到下载列表
			strcpy ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] , "i51KIT") ;  
			strcat ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] , "\\") ;  
			strcat ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] ,  i51KernelInstaller.i51KernelAppName ) ;
			strcat ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] , "\\") ;  
			strcat ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] ,  i51KernelInstaller.i51KernelAppName ) ;  
			i51KernelInstaller.i51KernelInstallNeedDlCount ++ ;
			i51KernelDlStart() ;
			interalRet = iTRUE ;
		}
	

		if (!interalRet)
			//	获取依赖失败，为文件操作失败
			i51KernelExternDLCallbackDo (  i51KernelInstaller.i51KernelAppName ,
			i51_KERNEL_INSTALL_ERROR_FILE , 
			0 ,
			0 ,
			0 ,
			0
			) ;
		#endif

		if ( i51KernelInstaller.i51KernelFromServerDlCount != 0 ) {

				
			//继续下载或继续更新
			i51KernelDlStart () ;
			return ;
		}
		
		strcpy ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] , "i51KIT") ;  
		strcat ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] , "\\") ;  
		strcat ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] ,  i51KernelInstaller.i51KernelAppName ) ;
		strcat ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] , "\\") ;  
		strcat ( i51KernelInstaller.i51KernelInstallNeedDlList[i51KernelInstaller.i51KernelInstallNeedDlCount] ,  i51KernelInstaller.i51KernelAppName ) ;  
		i51KernelInstaller.i51KernelInstallNeedDlCount ++ ;
		i51KernelDlStart() ;

	}else {

			//	create dl失败就相当于创建链路失败
			i51KernelExternDLCallbackDo (  i51KernelInstaller.i51KernelAppName ,
			i51_KERNEL_INSTALL_ERROR_CREATELINK , 
			0 ,
			0 ,
			0 ,
			0
			) ;
	}
		

}


iBOOL i51KitCoreInstallApplication ( iU16 *AppName , void (*callback) ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total , iU32 curdl , iU32 dlcount ) ) 
{
	
	//	author : Willam
	//	since : 2011.10.28
	// 	(C) PKIG Tech. Co., Ltd.	

	//安装应用程序

	//	安装包的路径

	iCHAR appname8bit[64] = {0} ;
	
	#ifdef i51KERNEL_DEBUG	
		iCHAR buffer [ 64 ] = { 0 } ;
		i51AdeStdUnicode2Ascii ( AppName , (iCHAR* ) buffer ) ;
		i51AdeOsLog ( i51_LOG_SC , "i51KernelInstallApplication S , Name %s , 0x%x" , buffer , callback ) ;
	#endif

	i51AdeStdUnicode2Ascii ( AppName , appname8bit ) ;
	i51AdeOsLog ( i51_LOG_SC , "KEL : Install %s" , appname8bit ) ;

	iASSERT (callback&&AppName) ;

	if ( !i51KitCoreSlDlAbleCreate () ) {

		i51AdeOsLog ( i51_LOG_SC , "KEL : Install dl aready create" ) ;
		i51DownloaderRunning.IntallCallback = callback ;
		strcpy ( i51DownloaderRunning.InstallAppname , appname8bit) ;
		i51DownloaderRunning.Installtimerid = i51AdeOsTimerCreate ( 0 , 1 ) ; 
		if ( i51DownloaderRunning.Installtimerid == 0 ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : Install create timer F" ) ;	
			return iFALSE ;
		}
		if ( 0 == i51AdeOsTimerStart (  i51DownloaderRunning.Installtimerid  , i51KernelIntallUseDlFailCB , 30*1000 ) )
		{
			i51AdeOsLog ( i51_LOG_SC , "KEL : Install  timer start F" ) ;	
			return iFALSE ;
		}		
		
		return iFALSE ;
	}

	if ( i51KernelInstaller.i51KernelFromServerDlCount == 0 ||
		strcmp ( i51KernelInstaller.i51KernelAppName , appname8bit ) != 0 )  {

		//	开始安装时清空，继续安装不需要
		memset (&i51KernelInstaller , 0 , sizeof(i51KernelInstaller)) ;
	}

		
	KernelExternDLCallback = callback ;
	i51KernelInstaller.i51KernelInstallMode = 0 ;
	strcpy ( i51KernelInstaller.i51KernelAppName , appname8bit ) ;
	i51KitCoreSlDlCreate (KernelInstallCreateDlCallback) ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelInstallApplication E" ) ;
	#endif
	
	return iTRUE ;

	
}

iBOOL i51KitCoreInstallApplicationStop () {
	//	author : Willam
	//	since : 2011.10.28
	// 	(C) PKIG Tech. Co., Ltd.	

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelInstallApplicationStop S" ) ;
	#endif


	i51AdeOsLog ( i51_LOG_SC , "KEL : Install Stop" ) ;

	//	停止安装
	i51KitCoreSlDlStop(0) ;



	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelInstallApplicationStop E" ) ;
	#endif
	
	return iTRUE ;
	
}


iBOOL i51KitCoreUninstallApplication ( iU16* appname ) { 
	//	author : Willam
	//	since : 2011.11.17
	// 	(C) PKIG Tech. Co., Ltd.	

	//	删除应用，即删除整个应用文件

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelUninstallApplication S" ) ;
	#endif

	i51AdeOsLog ( i51_LOG_SC , "KEL : Uninstall" ) ;
	
	i51AdeOsFileDeleteDir (appname) ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelUninstallApplication S" ) ;
	#endif
	
	return iTRUE ;

}


iBOOL i51KitCoreReinstallApplication ( iU16* appname , iCHAR *uri , iU32 version , void (*callback)( iCHAR* currentstep , iS32 status , iU32 step , iU32 totall, iU32 curdl , iU32 dlcount ) ) { 
	//	author : Willam
	//	since : 2011.11.17
	// 	(C) PKIG Tech. Co., Ltd.	

	//	更新

	iCHAR appname8bit[64] = {0} ;
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelReinstallApplication S , 0x%x , %s , 0x%x , 0x%x" , appname , uri , version , callback ) ;
	#endif

	i51AdeStdUnicode2Ascii ( appname , appname8bit ) ;
	i51AdeOsLog ( i51_LOG_SC , "KEL : Reinstall %s , %s" , appname8bit , uri ) ;

	iASSERT (callback&&appname) ;

	if ( !i51KitCoreSlDlAbleCreate () ) {

		i51AdeOsLog ( i51_LOG_SC , "KEL : Reinstall dl aready create" ) ;
		i51DownloaderRunning.ReinstallCallback = callback ;
		strcpy ( i51DownloaderRunning.ReinstallAppname , appname8bit) ;
		i51DownloaderRunning.Reinstalltimerid = i51AdeOsTimerCreate ( 0 , 1 ) ; 
		if ( i51DownloaderRunning.Reinstalltimerid == 0 ) {

			i51AdeOsLog ( i51_LOG_SC , "KEL : Reinstall create timer F" ) ;	
			return iFALSE ;
		}
		if ( 0 == i51AdeOsTimerStart (  i51DownloaderRunning.Reinstalltimerid  , i51KernelReintallUseDlFailCB , 30*1000 ) )
		{
			i51AdeOsLog ( i51_LOG_SC , "KEL : Reinstall  timer start F" ) ;	
			return iFALSE ;
		}		
		
		return iFALSE ;
	}


	if ( i51KernelInstaller.i51KernelFromServerDlCount == 0  || 
	strcmp ( i51KernelInstaller.i51KernelAppName , appname8bit ) != 0	) {

	//	开始更新时清空，继续更新不需要
		memset (&i51KernelInstaller , 0 , sizeof(i51KernelInstaller)) ;
	}

	//	更新
//	memset (&i51KernelInstaller , 0 , sizeof(i51KernelInstaller)) ;
	KernelExternDLCallback = callback ;
	//	更新是将版本好保存
	i51KernelInstaller.i51KernelInstallNeedDlVerionID[0] = version ;
	i51KernelInstaller.i51KernelInstallMode = 1 ;
	strcpy ( i51KernelInstaller.i51KernelAppName , appname8bit ) ;
	strcpy ( i51KernelInstaller.i51KernelUri , uri ) ;
//	i51AdeStdUnicode2Ascii ( appname , i51KernelInstaller.i51KernelAppName ) ;
	i51KitCoreSlDlCreate (KernelInstallCreateDlCallback) ;	
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelReinstallApplication E" ) ;
	#endif
	
	return iTRUE ;

}


iBOOL i51KitCoreReinstallApplicationStop () {

	//	author : Willam
	//	since : 2011.11.19
	// 	(C) PKIG Tech. Co., Ltd.	
	
	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelReinstallApplicationStop S" ) ;
	#endif

	i51AdeOsLog ( i51_LOG_SC , "KEL : Reinstall Stop" ) ;
	//	停止更新
	i51KitCoreSlDlStop(0) ;

	#ifdef i51KERNEL_DEBUG	
		i51AdeOsLog ( i51_LOG_SC , "i51KernelReinstallApplicationStop E" ) ;
	#endif
	
	return iTRUE ;

}


iBOOL i51KitCoreExtractPackage ( iFILE handle , iU16 *AppName ) 
{

	//	author : william
	//	since : 20111024
	//	PKIG Tech. Co., Ltd.

	//	解压安装包

	iBOOL ret = iTRUE ;
	iCHAR appname8bit[64] = {0} ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelExtractPackage S" ) ;
	#endif
	//	Jelo Add 2011.12.31
	if ( 0 == handle ) {
		return iFALSE ;
	}
	//	Jelo Add Finished
	
	i51AdeStdUnicode2Ascii ( AppName , appname8bit ) ;

	i51AdeOsLog ( i51_LOG_SC , "KEL : Ex Pack '%s'" , appname8bit ) ;
	
	ret = i51KernelExtractInternalPackage ( handle , appname8bit , 0 ) ;

	#ifdef i51KERNEL_DEBUG	
	i51AdeOsLog ( i51_LOG_SC , "i51KernelExtractPackage E" ) ;
	#endif
	
	return ret ;

}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
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
		
		case i51_MSG_RUN :
		break ;
		//接收到该消息时传入i51KitStaticCoreAdater数组的地址
		case 0xFFFFFF:	
			i51KIT_STATIC_KIT_SET(CoreAdapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(CoreAdapter);
		break;

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
	
	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitCore)
	
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreSlDlAbleCreate)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreSlDlCreate)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreSlDlStop)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreSlDlStart)

		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreInstallApplication)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreInstallApplicationStop)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreReinstallApplication)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreReinstallApplicationStop)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreUninstallApplication)

		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreExtractPackage)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitCoreZipUncompress)

	i51KIT_DYNAMIC_KIT_MAPING_END 

	return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitCore) ;
}
