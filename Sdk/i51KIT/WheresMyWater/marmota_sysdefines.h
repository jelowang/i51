#ifndef __MARMOTA_SYSDEFINES_H__
#define __MARMOTA_SYSDEFINES_H__

#include "marmota_backend_defines.h"
#include "i51.h"
#include "i51KitAde.h"

extern void * i51_malloc(unsigned int size);
extern void i51_free(void *ptr);
extern void *mmRealloc(void *src_ptr, int size, int resize);
extern void *mmCalloc(int num, int size);

#ifndef inline
	#define inline __inline
#endif
#ifndef null
#define null 0//((void *)0)
#endif

typedef enum 
{
	MM_SOUND_MIDI = 17,
	MM_SOUND_WAV  = 13,
	MM_SOUND_MP3  = 5,
}MM_SOUND_TYPE;

/// marmota memory system call define.

#define mmMalloc		i51_malloc//OslMalloc
#define mmFree			i51_free//OslMfree
//#define mmMedMalloc		med_alloc_ext_mem
//#define mmMedFree		med_free_ext_mem

//#define mmMemSet		memset//kal_mem_set
//#define mmMemCpy		memcpy//kal_mem_cpy
//#define mmRand          rand

extern void *
mmRealloc(void *src_ptr, int size, int resize);

extern void *
mmCalloc(int num, int size);

//inline void *
//mmRealloc(void *src_ptr, int size, int resize)
//{
//	void *temp_ptr = null;
//	if(!resize){
//		mmFree(src_ptr);
//	}
//	else
//	{
//		temp_ptr = (void*)i51AdeOsMalloc(resize, __FILE__, __LINE__);
//		if(temp_ptr)
//		{
//			if(src_ptr)mmMemCpy(temp_ptr, src_ptr, size < resize ? size : resize);
//			mmFree(src_ptr);
//		}
//	}
//
//	return temp_ptr;
//}
//
//inline void *
//mmCalloc(int num, int size)
//{
//	i51AdeOsLog(0, "mmCalloc.......ǰ");
//	 void *p = (void *)i51AdeOsMalloc(num * size, __FILE__, __LINE__);
//	 i51AdeOsLog(0, "mmCalloc.......");
//	 mmMemSet(p, 0, num * size);
//	 return p;
//}

#endif