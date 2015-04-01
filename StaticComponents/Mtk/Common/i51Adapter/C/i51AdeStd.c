 
 
/*

	The Implementing of 'Adapter' , Module : 'STD'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Api.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/
#include "i51Scripts.h"
#include "i51AdeCnf.h"
#include "i51AdeStd.h"
#include "i51AdeOs.h"
#if defined(__I51_TINY__)
#if defined(__I51_ADD_ZLIB__)
#include "i51SlZlib.h"
#endif
#include "i51KitCore.h"
#else
#include "i51SlZlib.h"
#endif
#include "mmi_include.h"
#include "kal_release.h"
#include "med_utility.h"
#if( I51_MTK_VS >= VS_11B_1212 )
#include "mmi_frm_scenario_gprot.h"
#include "mmi_frm_history_gprot.h"
#include "gs_srv_vfx_framework.h"
#endif

#if( I51_MTK_VS ==VS_09B )
//#include "..\\..\\plutommi\\Customer\\ResGenerator\\zlib\\zlib.h"
#elif( I51_MTK_VS ==VS_08B )
//#include "..\\..\\vendor\\vectorfont\\freetype\\freetype\\src\\gzip\\zlib.h"
#endif



static i51ADECALLBACK i51AdaCreateCB ;

//输入框回调


static void *i51EditorContent = iNULL ;

const iU32 i51Crc32table[ 256 ] =
{
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};

#if( I51_MTK_VS <=VS_0812 )

static void *i51AdeStdZlibMalloc( unsigned int size)
{
    return  med_alloc_ext_mem(size);       
}

static  void i51AdeStdZlibFree( void *p)
{
    med_free_ext_mem(&p);   
}

#else

static void *i51AdeStdZlibMalloc(void *opaque, unsigned int items, unsigned int size)
{


    iS32 total_bytes;
    void *buffer_ptr = iNULL;
    
    if (opaque)
    {
        items += size - size;   /// make compiler happy
    }
    total_bytes = items * size;


    buffer_ptr = i51AdeOsMallocEx (total_bytes) ;

    ASSERT(buffer_ptr != iNULL);

    return buffer_ptr;
    
    

}

static  void i51AdeStdZlibFree(void *opaque,void *p)
{
	i51AdeOsFreeEx(p);
}

#endif

static void  i51AdeStd_i51ADECALLBACK_TIMERCB ( iU32 id )
{

	i51AdeOsTimerStop(id) ;
	i51AdaCreateCB (iTRUE) ;
}
static iBOOL  i51AdeStd_ASYN_i51ADECALLBACK ()
{

	//	 回调，异步通知

	iU32 timerid = 0 ;
	timerid = i51AdeOsTimerCreate ( 0 , 1 ) ;
	if ( 0 == timerid )
	{
		i51AdeOsLog ( 0 , "STD : i51AdeStd_ASYN_i51ADECALLBACK , TC " ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart (timerid , i51AdeStd_i51ADECALLBACK_TIMERCB , 1 ) )
	{
		i51AdeOsLog ( 0 , "STD : i51AdeStd_ASYN_i51ADECALLBACK , TS F" ) ;		
		return iFALSE ;
	}		
}



iU32 i51AdeStdCrc32( iU8* buf , iS32 len , iU32** Table )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	iU32 nCRC = 0xFFFFFFFF;
	iS32 i = 0;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdCrc32 , S , %x , %x , %x" , buf , len , Table) ;
	#endif	
	
	if ( 0 != buf )
	{
	
		for( i = 0 ; i < len ; i ++ ) {
			
			nCRC = ( nCRC >> 8 ) ^ i51Crc32table[ ((buf[i]+0)&0xFF) ^ ( nCRC & 0x000000FF ) ];
		
		}

	}
	if ( 0 != Table ) 
	{
		*Table = (iU32*)i51Crc32table ;
	}

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdCrc32 , E , ret = %x" , nCRC ^ 0xffffffff) ;
	#endif	
	
	return (nCRC ^ 0xffffffff);

}
iU16* i51AdeStdAscii2Unicode ( const iCHAR* src , iU16* dst )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.


	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdAscii2Unicode , S , %x , %x" , src , dst ) ;
	#endif	
	
	mmi_asc_to_ucs2 ( ( iCHAR *)dst ,  ( iCHAR *) src ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdAscii2Unicode , E , ret = %x" ,dst) ;
	#endif	
	
	return dst ;

}

iCHAR* i51AdeStdUnicode2Ascii ( const iU16* src , iCHAR* dst ) 
{

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdUnicode2Ascii , S , %x , %x" , src , dst ) ;
	#endif	
	
	mmi_ucs2_to_asc (dst, ( iCHAR *) src) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdUnicode2Ascii , E , ret = %x" ,dst) ;
	#endif	

	return dst ;
	
}

void i51AdeStdMemset16 ( void* dest , iU16 value , iU32 length )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdMemset16 , S , %x , %x , %x" , dest , value , length ) ;
	#endif	
	
	memset ( dest , value , length ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdMemset16 , E" ) ;
	#endif	
 
}

void i51AdeStdMemcpy16 ( void* dest , void* src , iU32 length )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdMemcpy16 , S , %x , %x , %x" , dest , src , length ) ;
	#endif	

	memcpy ( dest , src , length ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdMemcpy16 , E" ) ;
	#endif	

}

iBOOL i51AdeStdWSprintf ( iU16 *buffer , iU32 size , const char* format , ... )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.


	iCHAR *bufferAsc = iNULL ;
	va_list ap = {0};

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWSprintf , S , %x , %x , %x" , buffer , size , format ) ;
	#endif	
	
    if(size<6) 
        return iFALSE;
	bufferAsc = ( iCHAR * )i51AdeOsMallocEx ( size/2 ) ;
	if ( NULL == bufferAsc ) {

		i51AdeOsLog ( 0 , "STD : i51AdeStdWSprintf , BE" ) ;
		return iFALSE ;
	}
	memset ( bufferAsc , 0 , size/2 ) ;
	
	va_start ( ap , format ) ;
	vsnprintf ( bufferAsc , size/2-1, format , ap ) ;
	va_end ( ap ) ;

	mmi_asc_to_ucs2 ( (iCHAR *) buffer , bufferAsc ) ;

	if ( iNULL != bufferAsc ) {

		i51AdeOsFreeEx(bufferAsc) ;
		bufferAsc = iNULL ;

	}

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWSprintf , E , ret = %x" , iTRUE ) ;
	#endif	

	return iTRUE ;
	
}

extern iU16 I51_SCREEN_ID;
#if( I51_MTK_VS >= VS_11B_1212 )
extern iU16 I51_GROUP_ID;
#endif
static i51STDEDITOR l_EditorUseInfo;
static void i51AdeStdEditorBack ( void )
{

	//	author : miki
	//	since : 20120302
	//	PKIG Tech. Co., Ltd.

	GoBackHistory () ;
	if ( l_EditorUseInfo.callback )  {
		iU16 *pwcs =(iU16*) l_EditorUseInfo.buffer;
        pwcs[l_EditorUseInfo.length/2-1] = 0;      
		l_EditorUseInfo.callback( l_EditorUseInfo.buffer, i51AdeStdWStrlen(pwcs) ); 
		l_EditorUseInfo.callback = NULL;
	}
}

void i51AdeStdEntryEditor_void(void)
{
	//	author : miki
	//	since : 20120302
	//	PKIG Tech. Co., Ltd.

	iU32 type = 0 ;
    U8 * guiBuffer=NULL; 
	
	#if( I51_MTK_VS >= VS_11B_1212 )
	MMI_ID parent_id;  //定义上级group id

    parent_id=mmi_frm_group_get_active_id();

	mmi_frm_group_create(parent_id, 

                          I51_GROUP_ID+1, 

                          NULL, // 相当于group的callback函数，

                          NULL);


    mmi_frm_group_enter(I51_GROUP_ID+1, MMI_FRM_NODE_SMART_CLOSE_FLAG);
	

    if (!mmi_frm_scrn_enter(

            I51_GROUP_ID+1,//group id

            I51_SCREEN_ID+1,//screnn id 

            NULL,

            i51AdeStdEntryEditor_void,//入口函数

            MMI_FRM_FULL_SCRN))//全屏显示

    {

        return;

    }
   	guiBuffer = mmi_frm_scrn_get_active_gui_buf();//得到屏幕buf
	#else
    EntryNewScreen(I51_SCREEN_ID+1, NULL, i51AdeStdEntryEditor_void, NULL);
    guiBuffer = GetCurrGuiBuffer(I51_SCREEN_ID+1);  
	#endif
	
	switch ( l_EditorUseInfo.type ) {
		
	case i51_EDITOR_NUMBER:
		
		#if( I51_MTK_VS <=VS_0812 )
			type = INPUT_TYPE_NUMERIC;
		#endif

		#if( I51_MTK_VS >=VS_08B )
			type = IMM_INPUT_TYPE_NUMERIC;
		#endif
		
	break;

	case i51_EDITOR_PASSWORD:
		
		#if( I51_MTK_VS <=VS_0812 )
			type = INPUT_TYPE_ALPHANUMERIC_PASSWORD ;
		#endif

		#if( I51_MTK_VS >=VS_08B )
			type = IMM_INPUT_TYPE_ASCII_PASSWORD;//IMM_INPUT_TYPE_EXT_PASSWORD ;
		#endif
		
	break;

	case i51_EDITOR_TEXT:  //all
	
	default:
		#if( I51_MTK_VS <=VS_0812 )
			type = INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_USE_ENCODING_BASED_LENGTH;
		#endif

		#if( I51_MTK_VS >=VS_08B )
			type = IMM_INPUT_TYPE_SENTENCE | INPUT_TYPE_USE_ENCODING_BASED_LENGTH;
		#endif
		
		
	break;
    };
    
	ShowCategory5Screen ( iNULL,
					(U16) iNULL,
					STR_GLOBAL_OK,
					(U16) iNULL,
					STR_GLOBAL_BACK,
					(U16) iNULL,
					type,
					l_EditorUseInfo.buffer ,
					l_EditorUseInfo.length/2 ,
					guiBuffer);

	SetLeftSoftkeyFunction(i51AdeStdEditorBack, KEY_EVENT_UP);
	SetKeyHandler(i51AdeStdEditorBack, KEY_END, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(i51AdeStdEditorBack, KEY_EVENT_UP);
   
}

iBOOL i51AdeStdEntryEditor (  i51STDEDITOR* info )
{
	//	author : miki
	//	since : 20120302
	//	PKIG Tech. Co., Ltd.
    l_EditorUseInfo = *info;
//    memset( l_EditorUseInfo.buffer, 0, l_EditorUseInfo.length );
    i51AdeStdEntryEditor_void();

    return iTRUE ; 
}

#ifdef __I51_TINY__
extern void i51KernelUnloadKitCore (iU32 id) ;
extern iBOOL  i51KernelLoadKitCore (iU32 id) ;

#endif

iBOOL i51AdeStdUncompress ( iU8* dest , iU32* destlen , const iU8* source , iU32 srclen )
{

	//	author : william
	//	since : 20110830
	//	PKIG Tech. Co., Ltd.

	iS32 result = 0 ;
	iU64 destlength = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdUncompress , S , %x , %x , %x , %x" , dest , destlen ,source , srclen ) ;
	#endif	

	destlength = *destlen ; 
#if !defined(__I51_TINY__)||defined(__I51_ADD_ZLIB__)
#if( I51_MTK_VS <=VS_0812 )
    appzlib_register( i51AdeStdZlibMalloc , i51AdeStdZlibFree );
	result = uncompress ( dest , &destlength , source , srclen );
#else
	result = uncompress_mtk ( dest , &destlength , source , srclen , i51AdeStdZlibMalloc , i51AdeStdZlibFree ) ;	
#endif

#else
	//akilae add since 2012.7.12
	
	if (!i51KernelLoadKitCore ((iU32)i51AdeStdUncompress))
	    return iFALSE ;

	if(i51KitCoreZipUncompress != NULL){
		result = i51KitCoreZipUncompress( dest , &destlength , source , srclen );
	}
	else{
		i51AdeOsLog(0, "STD : Uncompress F");
	}
    
    i51KernelUnloadKitCore ((iU32)i51AdeStdUncompress) ;
	//akilae add end
#endif

	*destlen = destlength ; 
	 
	if ( result == 0 ) {

		#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog ( 0 , "STD : i51AdeStdUncompress , E , ret = %x" , iTRUE ) ;
		#endif	
		return iTRUE ;
	}
	else {

		i51AdeOsLog ( 0 , "STD : i51AdeStdUncompress , BE" ) ;
		return iFALSE ;
	}
	
}

iU32 i51AdeStdWStrlen ( iU16* S )
{


	//	author : william
	//	since : 20110830
	//	PKIG Tech. Co., Ltd.

	iU32 ret = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWStrlen , S , %x" , S ) ;
	#endif	
	ret = mmi_ucs2strlen ( (S8*) S ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWStrlen , E , ret = %x" , ret ) ;
	#endif
	
	return ret ;


}

iU16* i51AdeStdWStrcpy (  iU16* T ,  iU16* S )
{

	//	author : william
	//	since : 20110830
	//	PKIG Tech. Co., Ltd.

	iU16 *pDest = NULL ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWStrcpy , S , %x , %x" , T , S ) ;
	#endif	

	pDest =  (iU16*)mmi_ucs2cpy ( (S8*) T , (S8*)S ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWStrcpy , E , ret = %x" , pDest) ;
	#endif	
	
	return pDest ;

}

iS32 i51AdeStdWStrcmp ( iU16* T ,  iU16* S )
{

	//	author : william
	//	since : 20110830
	//	PKIG Tech. Co., Ltd.

	iS32 ret = 0 ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWStrcmp , S , %x , %x" , T , S ) ;
	#endif	
	
	ret = mmi_ucs2cmp ( (S8*) T , (S8*)S ) ;
	
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWStrcmp , E , ret = %x" , ret) ;
	#endif		
	
	return ret ;

	

}

iU16* i51AdeStdWStrcat ( iU16* T ,  iU16* S )
{

	//	author : william
	//	since : 20110830
	//	PKIG Tech. Co., Ltd.

	iU16 *pDest = NULL ;
	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWStrcat , S , %x , %x" , T , S ) ;
	#endif	
	
	pDest =  (iU16*)mmi_ucs2cat ( (S8*) T , (S8*)S ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdWStrcat , E , ret = %x" , pDest) ;
	#endif		

	return pDest ;
	

}

iS32 i51AdeStdAtoi (const iCHAR* buffer) 
{
	//	author : william
	//	since : 20110928
	//	PKIG Tech. Co., Ltd.

	iS32 ret = 0 ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdAtoi , S , %x" , buffer ) ;
	#endif	
	
	ret = atoi( buffer ) ;

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdAtoi , E , ret = %x" , ret) ;
	#endif		

	return ret ;
	
}

void i51AdeStdItoa (iCHAR **buf, iS32 integer , iS32 base )
{

	char *s = NULL ;
#define LEN	20
	int rem = 0 ;
	static char rev[LEN+1];

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdAtoi , S , %x , %x , %x" , buf , integer , base) ;
	#endif	
	
	rev[LEN] = 0;
	if (integer == 0)
	{
		(*buf)[0] = '0';
		++(*buf);
		return;
	}
	s = &rev[LEN];
	while (integer)
	{
		rem = integer % base;
		if (rem < 10)
			*--s = rem + '0';
		else if (base == 16)
			*--s = "abcdef"[rem - 10];
		integer /= base;
	}
	while (*s)
	{
		(*buf)[0] = *s++;
		++(*buf);
	}

	#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog ( 0 , "STD : i51AdeStdAtoi , E ") ;
	#endif		


}

iBOOL i51AdeStdCreate ( i51ADECALLBACK callback )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.

	iASSERT ( callback ) ;
	i51AdaCreateCB = callback ;
	if ( i51AdeStd_ASYN_i51ADECALLBACK() )
		return iFALSE ;
	
	return iTRUE ;


}

iBOOL i51AdeStdRelease ( void )
{

	//	author : william
	//	since : 20110805
	//	PKIG Tech. Co., Ltd.
	return iTRUE ;


}

