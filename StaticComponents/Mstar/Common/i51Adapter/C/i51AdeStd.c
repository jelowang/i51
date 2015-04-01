#include "i51AdeStd.h"
#include "ven_std.h"
#include "ven_editor.h"
#include "ven_stc.h"
#include "ven_ts.h"
#include "ven_os.h"
#include "ven_kpd.h"
#include "i51AdeOs.h"

#include "i51AdeCnf.h"

#if !defined(__I51_ADD_ZLIB__) && defined(__I51_TINY__)
#include "i51KitCore.h"
#endif /*#ifdef __I51_TINY__*/

typedef void (*i51EditorCallBack ) ( void* pbuffer , iU32 textlen ) ;
static i51EditorCallBack i51EditorCB ;
static i51ADECALLBACK i51AdaCreateCB ;

extern void i51TsHandle(ven_ts_evt_t type, u32 x, u32 y);
extern void i51KpdHandle(ven_kpd_act_t keyAct, ven_kpd_key_code_t keyCode);


//应该传入 AP 的handle   
extern iU32 g_i51PkigHandle; 
static void *i51EditorContent = NULL ;

#if !defined(__I51_TINY__) || defined(__I51_ADD_ZLIB__)

#ifndef FAR
#define FAR
#endif

#if !defined(MACOS) && !defined(TARGET_OS_MAC)
typedef unsigned char  Byte;  /* 8 bits */
#endif

#ifdef SMALL_MEDIUM
/* Borland C/C++ and some old MSC versions ignore FAR inside typedef */
#define Bytef Byte FAR
#else
typedef Byte  FAR Bytef;
#endif

typedef unsigned long  uLong; /* 32 bits or more */
typedef uLong FAR uLongf;

typedef void* (*_alloc_func)(unsigned int);
typedef void (*_free_func)(void*);

#ifndef ZEXPORT
#define ZEXPORT
#endif
extern void ZEXPORT i51appzlib_register(_alloc_func alloc_hdlr, _free_func free_hdlr);
extern int ZEXPORT i51uncompress (Bytef *dest,uLongf *destLen,const Bytef *source,uLong sourceLen);

#endif /*#ifndef __I51_TINY__*/
extern iBOOL i51AdeOsFree ( void* address ) ;
extern void* i51AdeOsMallocEx ( iU32 length ) ;

typedef struct
{    
	iU16* pinbuf;    
	iS32 bufsize;
}LLPInputDlgUserData;

const iU32 i51AdeStdCrc32Table[ 256 ] =
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

static void i51AdeStdCreate_CB(iU32 timerid)
{
	if (NULL != i51AdaCreateCB)
	{
		i51AdaCreateCB(iTRUE);
	}

	i51AdeOsTimerStop (timerid);
}

static void edit_Callback(iU32 UserData, iU32 RspData)
{
	LLPInputDlgUserData *pData=(LLPInputDlgUserData *)UserData;
	ven_editor_edit_rsp_t *pRsp=(ven_editor_edit_rsp_t *)RspData;

	i51AdeOsLog(i51_LOG_SC, "STD : edit_Callback , %x,%s,%d,%d" , i51EditorCB,pRsp->ptContent, pRsp->contentLen,pRsp->rspType);
    
	ven_ts_subscribe(g_i51PkigHandle, i51TsHandle );
	ven_kpd_subscribe(g_i51PkigHandle, i51KpdHandle );

	i51KernelSendEvent( i51_MSG_RESUME, NULL);

	if( pRsp->rspType ==VEN_EDITOR_RESULT_OK )
	{   
		if ( i51EditorCB ) 
		{
			i51EditorCB (pRsp->ptContent, pRsp->contentLen) ;
		}
	}
	else if( pRsp->rspType ==VEN_EDITOR_RESULT_CANCEL )
	{
	    if(i51EditorContent)
        {
            i51EditorCB((iU16 *)i51EditorContent, i51AdeStdWStrlen((iU16 *)i51EditorContent));
        } 
        else
        {
            i51EditorCB(NULL, 0);
        }
	}

	if (pData)
	{
		i51AdeOsFree((void*)pData);
		pData = NULL;
	}
}

#if !defined(__I51_TINY__) || defined(__I51_ADD_ZLIB__)
static int uncompress_mstar( iU8* dest , iU64* destlen , const iU8* source , iU64 srclen , _alloc_func alloc_hdlr,  _free_func free_hdlr)
{
	iS32 result ;

	i51appzlib_register(alloc_hdlr, free_hdlr);
	result = i51uncompress(dest , destlen , source , srclen);

	return result;
}
#endif /*#ifndef __I51_TINY__*/

static iCHAR* i51AdeStd_UCS2ToAscii(iCHAR* pAscii, const iU16* pUnicode)
{
	iCHAR* pAscCode = pAscii;
	while (1)
	{
		//not a valid ascii char, so stop converting
		if (*pUnicode & 0xFF00)
		{
			*pAscii = 0;
			break;
		}

		if (*pUnicode != 0)
		{
			*pAscii = (iCHAR)(*pUnicode);
			pAscii++;
			pUnicode++;
		}
		else
		{
			*pAscii = 0;
			break;
		}
	}
	return pAscCode;
}

static iU16* i51AdeStd_AsciiToUCS2(iU16* pUnicode, const iCHAR* pAscii)
{
	iU16* pUni = pUnicode;

    if(NULL == pUnicode || NULL == pAscii)
    {
        return NULL;
    }
    
	while (*pAscii)
	{
		*pUnicode++ = *pAscii++;
	}
	*pUnicode = 0;

	return pUni;
}

#if !defined(__I51_TINY__) || defined(__I51_ADD_ZLIB__)
static void i51AdeZlibMalloc( unsigned int size)
{
    i51AdeOsMalloc(size, __FILE__ , __LINE__ );
}

static void i51AdeZlibFree(void *p)
{
    if(NULL != p)
    {
        i51AdeOsFree(p);
    }
}
#endif /*#ifndef __I51_TINY__*/

//ok
iU32 i51AdeStdCrc32( iU8* buf , iS32 len , iU32** table )
{
	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 nCRC = 0xFFFFFFFF;
	iS32 i;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdCrc32 , S , %x , %d , %x", buf, len, table);
#endif

	if ( 0 != buf )
	{

		for( i = 0 ; i < len ; i ++ ) 
		{

			nCRC = ( nCRC >> 8 ) ^ i51AdeStdCrc32Table[ ((buf[i]+0)&0xFF) ^ ( nCRC & 0x000000FF ) ];

		}

	}

	if ( 0 != table ) 
	{
		*table = (iU32*)i51AdeStdCrc32Table ;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdCrc32 , E");
#endif

	return (nCRC ^ 0xffffffff);
}

//ok
iBOOL i51AdeStdWSprintf ( iU16 *buffer , iU32 bytes , const iCHAR* format , ... )
{
	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iCHAR *bufferAsc = NULL ;
	va_list ap;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWSprintf , S , %x , %d , %s", buffer, bytes, format);
#endif

	bufferAsc = ( iCHAR * )i51AdeOsMalloc ( bytes , __FILE__ , __LINE__ ) ;
	if(!bufferAsc)
	{
        i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWSprintf F , no empty memory , %d" , bytes);
        return 0;
	}
	memset ( bufferAsc , 0 , bytes ) ;

	va_start ( ap , format ) ;
	vsnprintf ( bufferAsc ,bytes-1, format , ap ) ;
	va_end ( ap ) ;

	i51AdeStd_AsciiToUCS2 ( buffer , bufferAsc ) ;

	if ( iNULL != bufferAsc )
	{
		i51AdeOsFree(bufferAsc) ;
		bufferAsc = iNULL ;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWSprintf , E");
#endif

	return iTRUE ;
}

//ok
iBOOL i51AdeStdEntryEditor (  i51STDEDITOR* info )
{ 

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	bool state;
	ven_editor_type_t type=VEN_EDITOR_TYPE_ALL;  
	ven_editor_style_t style=VEN_EDITOR_STYLE_TEXT;
	ven_req_data_t ReqData={0};
	LLPInputDlgUserData *pUserData = (LLPInputDlgUserData*)i51AdeOsMallocEx(sizeof(LLPInputDlgUserData));

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdEntryEditor ,  S , %x", info);
#endif

	switch (info->type) 
	{       
	case 0:  //数字          			
		type = VEN_EDITOR_TYPE_DIGITAL;
		break;        
	case 1:  //数字+英文   
		type = VEN_EDITOR_TYPE_ALPHA;
		break;                  
	case 2:  //all          
		type = VEN_EDITOR_TYPE_ALL;
		break;    
	default:
		break;
	};		

	i51KernelSendEvent( i51_MSG_PAUSE, NULL);
		
	ven_ts_unsubscribe(g_i51PkigHandle);
	ven_kpd_unsubscribe(g_i51PkigHandle);

	i51EditorCB = info->callback;
	i51EditorContent = info->buffer;

	ReqData.Handle = g_i51PkigHandle;
	ReqData.bSync = FALSE;
	ReqData.pfnCb = edit_Callback;
	ReqData.userData = (iU32)pUserData;

	pUserData->pinbuf = info->buffer;
	pUserData->bufsize = info->length;	

	state = ven_editor_edit( NULL, 0, type, style, VEN_EDITOR_ENCODING_UCS2
		, info->length - 1, (iU16*)info->buffer
		,i51AdeStdWStrlen((iU16*)info->buffer), NULL, 0, NULL, NULL, &ReqData);

	if( 1 == state)
	{
#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdEntryEditor , E");
#endif
		return iTRUE;
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdEntryEditor ,  F");
		return iFALSE;
	}

}

//ok
iU16* i51AdeStdAscii2Unicode(const iCHAR* src, iU16* dst)
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdAscii2Unicode, S , %s , %x", src, dst);
#endif

    if(NULL == src || NULL == dst)
    {
    	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdAscii2Unicode ,  F");
        return NULL;
    }
    
	dst = i51AdeStd_AsciiToUCS2(dst, src);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdAscii2Unicode ,  E , %x", dst);
#endif

	return dst;
}

//ok
iCHAR* i51AdeStdUnicode2Ascii(const iU16* src, iCHAR* dst)
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdUnicode2Ascii ,  S , %x , %s", src, dst);
#endif

    if(NULL == src || NULL == dst)
    {
        i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdUnicode2Ascii ,  F");
        return NULL;
    }
    
	dst = i51AdeStd_UCS2ToAscii(dst, src);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdUnicode2Ascii E , dst = %s", dst);
#endif

	return dst;
}

//ok
void i51AdeStdMemset16(void* dest, iU16 value, iU32 length)
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdMemset16 , S , %x , %d , %d", dest, value, length);
#endif

    if(NULL == dest)
    {
        i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdMemset16 , F");    
        return;
    }
    
	ven_std_memset(dest, value, length);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdMemset16 , E");
#endif
}

//ok
void i51AdeStdMemcpy16(void* dest, void* src, iU32 length)
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdMemcpy16 , S , %x , %x , %d", dest, src, length);
#endif

    if(NULL == dest)
    {
        i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdMemcpy16, F");    
        return ;
    }

	ven_std_memcpy(dest, src, length);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdMemcpy16, E");
#endif
}

#if !defined(__I51_TINY__) || defined(__I51_ADD_ZLIB__)
extern void i51KernelUnloadKitCore (iU32 id) ;
extern iBOOL  i51KernelLoadKitCore (iU32 id) ;
#endif

//ok
iBOOL i51AdeStdUncompress ( iU8* dest , iU32* destlen , const iU8* source , iU32 srclen )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iS32 result ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdUncompress , S , %x , %x , %x , %d", dest, destlen, source, srclen);
#endif

#if !defined(__I51_TINY__) || defined(__I51_ADD_ZLIB__)
	result = uncompress_mstar( dest , destlen , source , srclen, i51AdeZlibMalloc, i51AdeZlibFree);
#else
	if (!i51KernelLoadKitCore ((iU32)i51AdeStdUncompress))
		return iFALSE ;
	if(i51KitCoreZipUncompress != NULL){
	result = i51KitCoreZipUncompress( dest , (iU64*)destlen , source , srclen );
	}
	else{
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdUncompress , F");
	}
	i51KernelUnloadKitCore ((iU32)i51AdeStdUncompress) ;
#endif /*#ifndef __I51_TINY__*/

	if ( result == 0 ) 
	{

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdUncompress , E");
#endif
		return iTRUE ;
	}

	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdUncompress , F");
	return iFALSE ;
}

//ok
iU32 i51AdeStdWStrlen ( iU16* S )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iU32 len;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrlen S , %x", S);
#endif

	len = ven_std_wstrlen(S);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrlen E , %d", len);
#endif

	return len;
}

//ok
iU16* i51AdeStdWStrcpy ( iU16* T , iU16* S )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrcpy S , %x , %x", T, S);
#endif

    if(NULL == T || NULL == S)
    {
    	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrcpy F");
        return NULL;
    }

	T = ven_std_wstrcpy(T, S);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrcpy E , %x", T);
#endif

	return T;
}

//ok
iS32 i51AdeStdWStrcmp ( iU16* T , iU16* S )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iS32 ret = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrcmp S , %x , %x", T, S);
#endif

	ret = ven_std_wstrcmp(T, S);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrcmp E , %d", ret);
#endif
	
	return ret;
}

//ok
iU16* i51AdeStdWStrcat ( iU16* T , iU16* S )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrcat S , %x , %x", T, S);
#endif

    if(NULL == T || NULL == S)
    {
        i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrcat F");
        return NULL;
    }

	T = ven_std_wstrcat(T, S);
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdWStrcat E , %x", T);
#endif

	return T;
}

//ok
iS32 i51AdeStdAtoi (const iCHAR* buffer) 
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	iS32 ret = 0;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdAtoi S , %s", buffer);
#endif

	ret = ven_std_atoi( buffer ) ;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdAtoi E , %d" , ret);
#endif

	return ret;
}

//ok
void i51AdeStdItoa (iCHAR **buf, iS32 integer , iS32 base )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	char *s;
#define LEN	20
	int rem;
	static char rev[LEN+1];

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdItoa S , %x,%d,%d" , buf ,  integer , base);
#endif

	rev[LEN] = 0;
	if (integer == 0)
	{
		(*buf)[0] = '0';
		++(*buf);
        
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdItoa , E , integer = 0");
#endif

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
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdItoa , E");
#endif
}

iBOOL i51AdeStdCreate ( i51ADECALLBACK callback )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdCreate , S , %x" , callback);
#endif

	if (NULL != callback)
	{
		i51AdaCreateCB = callback;

		i51AdeOsTimerStart(i51AdeOsTimerCreate(iFALSE, 0), i51AdeStdCreate_CB, 30);

#ifdef i51ADAPTER_DEBUG
		i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdCreate , E");
#endif

		return iTRUE;
	}

	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeSocCreate , F");

	return iFALSE;;
}

iBOOL i51AdeStdRelease ( void )
{

	//	author : jone
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdRelease , S");
#endif

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "STD : i51AdeStdRelease , E");
#endif

	return iTRUE;

}

