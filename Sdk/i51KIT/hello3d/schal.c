
/*

+	Semo Compiler HAL 

+	'Semo Compiler' is a multi-objective compiler which is developing under the terms of the 
+	GNU general public license as published by the Free Software Foundation.
+	The project lunched by Jelo Wang since 2.Feb.2008 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	突壳开源Techniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		-Thanks to Our Committers and Friends
+		-Best Wish to all who Contributed and Inspired
+		-Techniques of Knowledge 
+		-致伙伴们最美好祝愿
+		-突壳开源社区

*/ 

# include <malloc.h>
# include <memory.h>
# include <stdarg.h> 
# include "schal.h"

# define MALLOC malloc
# define FREE //free

//# define MEMORY_MONITOR_ENABLE
# ifdef MEMORY_MONITOR_ENABLE
# define MMT_MASK_LENGTH 4
unsigned char* mmt_mask = 0 ;
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 } ;
int MMTInit () {

	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK
	
	mem_monitor.file = 0 ;
	mem_monitor.line = 0 ;
	mem_monitor.length = 0 ;
	mem_monitor.address = 0 ;	
	mem_monitor.head = 0 ;		
	mem_monitor.next = 0 ;

	return 1 ;
		
}

void MMTCreateMask ()
{

	//	author : Jelo Wang
	//	since : 20110705
	//	(C)TOK

	int looper = 0 ;
	
	mmt_mask = (unsigned char* ) malloc ( MMT_MASK_LENGTH ) ;

	for ( looper = 0 ; looper < MMT_MASK_LENGTH ; looper ++ )
	{
		mmt_mask [ looper ] = looper ;
	}

}

void MMTAdd ( char* file , int line , int length , int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* newmem = 0 ;
		
	newmem	= (MEMORY_MONITOR* ) malloc ( sizeof(MEMORY_MONITOR) ) ;

	if ( 0 == mmt_mask ) MMTCreateMask () ;
	
	memcpy ( (void*)((unsigned int)address+length) , mmt_mask , MMT_MASK_LENGTH ) ;

	if ( file ) {
		newmem->file = (char* ) malloc ( sc_strlen ( file ) + 1 ) ;
		sc_strcpy ( newmem->file , file ) ;
	}

	newmem->line = line ; 
	newmem->length = length ;
	newmem->address = address ;
	newmem->head = 0 ;
	newmem->next = 0 ;
 
	if ( 0 == mem_monitor.head ) {
		mem_monitor.head = newmem ;
		mem_monitor.next = newmem ;
	} else {
		mem_monitor.next->next = newmem ;
		mem_monitor.next = newmem ;
	}

}

static int overtimes = 0 ;
void MMTCheckOverflow () {

	//	author : Jelo
	//	since : 2011.4.10
	//	(C)TOK
	
	//	notes : 识别溢出内存块

	MEMORY_MONITOR* looper = 0 ;
	unsigned char* mask = 0 ;

	mask = (unsigned char* ) malloc ( MMT_MASK_LENGTH ) ;

	for ( looper = mem_monitor.head ; looper ; looper=looper->next ) 
	{

		int counter = 0 ;
		
		memcpy ( mask , (void*)((unsigned int )looper->address+looper->length) , MMT_MASK_LENGTH ) ;

		for ( counter = 0 ; counter < MMT_MASK_LENGTH ; counter ++ )
		{

			if ( mask [ counter ] != mmt_mask [ counter ] )
			{
				overtimes ++ ;
				SClog ( "!!!!! M : %x , In : '%s' , At line : '%d' - overflowed\n" , looper->address , looper->file , looper->line ) ;	
				free ( mask ) ;
				return ;
			}
		}		

	}

	free ( mask ) ;
	
}

void MMTFree ( int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor.head ;
	MEMORY_MONITOR* pre = &mem_monitor ;
	
	if ( !address ) return ;

	for ( ; walker ; ) {
		
		if ( address == walker->address ) {
			if ( walker == mem_monitor.head ) {
				mem_monitor.head = walker->next ;
				if ( walker->file ) FREE ( walker->file ) ;
				FREE ( walker ) ;
				return ;
			} else {
				pre->next = walker->next ;
				if ( walker == mem_monitor.next ) {
					mem_monitor.next = pre ;
				}
				if ( walker->file ) FREE ( walker->file ) ;
				FREE ( walker ) ;
				return ;
			}
			
		}

		pre = walker ;
		walker = walker->next ;
			
	}
	
}


void MMTDestroy () {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	for ( walker = mem_monitor.head ; walker ; ) {
		mem_monitor.next = walker->next ;
		if ( walker->file ) FREE ( walker->file ) ;
		FREE ( walker ) ;
		walker = mem_monitor.next ;
	}
	
}

void MMTTest () 
{	
	void* add = (void*)malloc ( 10+4 ) ;
	MMTAdd ( "1" , -1 , 10 , (int)add ) ;
	MMTFree ( add ) ;

	add = malloc ( 10+4 ) ;
	MMTAdd ( "2" , -1 , 10 , (int)add ) ;

	add = malloc ( 10+4 ) ;
	MMTAdd ( "3" , -1 , 10 , (int)add ) ;

	add = malloc ( 10+4 ) ;
	MMTAdd ( "4" , -1 , 10 , (int)add ) ;

	add = malloc ( 10+4 ) ;
	MMTAdd ( "5" , -1 , 10 , (int)add ) ;

	add = malloc ( 10+4 ) ;
	MMTAdd ( "6" , -1 , 10 , (int)add ) ;
	MMTFree ( add ) ;

	add = malloc ( 10+4 ) ;
	MMTAdd ( "7" , -1 , 10 , (int)add ) ;

	add = malloc ( 10+4 ) ;
	MMTAdd ( "8" , -1 , 10 , (int)add ) ;
}

# endif

void SClog ( const   char* SClog , ... ) {

	//	author : Jelo Wang
	//	notes : printer
	//	since : 20090809
	
	va_list ap ;   
	va_start ( ap , SClog ) ;   
	vprintf ( SClog , ap ) ;
	va_end ( ap ) ;

} 


void* SCNormalloc ( unsigned int length , char* file , int line ) {

	//	author : Jelo Wang
	//	notes : malloc
	//	since : 20090809

	//	set zero automatically
	
	void* buffer = 0 ;

	# ifdef MEMORY_MONITOR_ENABLE
		buffer = (void*) MALLOC ( length + MMT_MASK_LENGTH ) ;
		memset ( buffer , 0 , length + MMT_MASK_LENGTH ) ;
		MMTAdd ( file , line , length , (int)buffer ) ;
		SCHalMemoryOverflowed () ;
	# else
		buffer = (void*) MALLOC ( length ) ;
		if ( buffer ) memset ( buffer , 0 , length ) ;
	# endif
	
	return buffer ;

}

void* SCRealloc ( void* buffer , long int length ) {

	//	author : Jelo Wang
	//	notes : remalloc
	//	since : 20090809

	void* address = 0 ;
	
	# ifdef MEMORY_MONITOR_ENABLE
		MMTFree ( (int)buffer ) ;
		address = realloc ( buffer , length+MMT_MASK_LENGTH ) ;
		memset ( address , 0 , length+MMT_MASK_LENGTH ) ;
		MMTAdd ( "SCRealloc" , -1 , length , (int)address ) ;
		SCHalMemoryOverflowed () ;
	# else
		address = realloc ( buffer , length ) ;
		if ( address ) memset ( address , 0 , length ) ;	
	# endif
	
 	return address ;


}

void* SCRemalloc ( void* buffer , long int bufferlen , long int length ) {

	//	author : Jelo Wang
	//	notes : SCRemalloc
	//	since : 20090831
	
	unsigned char* memory = 0 ;

	# ifdef MEMORY_MONITOR_ENABLE
		MMTFree ( (int)buffer ) ;
		memory = (unsigned char* ) MALLOC ( length + MMT_MASK_LENGTH ) ; 
		memset ( memory , 0 , length + MMT_MASK_LENGTH ) ;
		MMTAdd ( "SCRemalloc" , -1 , length , (int)memory ) ;
		SCHalMemoryOverflowed () ;
	# else
		memory = (unsigned char* ) MALLOC ( length ) ; 
		if ( memory ) memset ( memory , 0 , length ) ;
	# endif

	if ( memory ) memcpy ( memory , buffer , bufferlen ) ;
	
 	FREE ( buffer ) ;

	return memory ;

	
}

int SCFree ( void* buffer ) {

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MMTCheckOverflow () ; 
		MMTFree ( (int)buffer ) ; 
	# endif

	FREE ( buffer ) ;

	return 1 ;

}

int SCFreeEx ( void** buffer ) {

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MMTCheckOverflow () ;
		MMTFree ( (int)*buffer ) ;
	# endif

 	FREE ( *buffer ) ;

	*buffer = 0 ;

	return 1 ;

}

void SCHaMOFTimes () {

	//	author : Jelo Wang
	//	since : 20110702
	//	(C)TOK
	
//	SClog ("%d\n" , overtimes ) ;

}

void SCHalMemoryOverflowed ()
{

	//	author : Jelo Wang
	//	since : 2011630
	//	(C)TOK

	# ifdef MEMORY_MONITOR_ENABLE
		MMTCheckOverflow ( &mem_monitor ) ;
	# endif

}

int SCHalMemoryLeaked () {

	//	author : Jelo Wang
	//	since : 20091129

	# ifdef MEMORY_MONITOR_ENABLE
		
		int totall = 0 ;
		int leakedtimes = 0 ;
		
		MEMORY_MONITOR* walker = mem_monitor.head ;
	
		for ( ;walker;walker=walker->next) {

			SClog ("!!! M : %x , S : %d , In '%s' , At line : '%d' - leaked\n",
				walker->address,\
				walker->length,\
				walker->file,\
				walker->line\
			) ;\
		
			totall = totall + walker->length ;

			leakedtimes ++ ;
			
		}

		MMTDestroy () ;

		SClog ( "Leaked Totall : %d Bytes\n" , totall ) ;
		SClog ( "Leaked Times : %d\n" , leakedtimes ) ;

	# endif

	
	return 0 ;
}

void* SCHalFileOpen ( char* path , char* flag ) {

	//	author : Jelo Wang
	//	notes : fopen
	//	since : 20090809

 	return (void* ) fopen ( path , flag ) ;

}

int SCHalFileSeek ( void* file , int offset , int direct ) {

	//	author : Jelo Wang
	//	notes : fseek
	//	since : 20090809

	return fseek ( (FILE*)file , offset , direct ) ;

}

int SCHalFileRead ( void* file , void* buffer , int size , int counter ) {

	//	author : Jelo Wang
	//	notes : fread
	//	since : 20090809

	return fread ( buffer , size , counter , (FILE*)file ) ;

}

int SCHalFileWrite ( void* file , void* buffer , int size , int counter ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return fwrite ( buffer , size , counter , (FILE*)file ) ;

}

short int SCHalFileGetc ( void* file ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return fgetc ( (FILE*)file ) ;

}

int SCHalFileEnd ( void* file ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return feof ( (FILE*)file ) ;

}


int SCHalFileLength ( void* file ) {

	//	author : Jelo Wang
	//	notes : fseek
	//	since : 20090809
	
	int length = 0;

	SCHalFileSeek ( file , 0 , SEEK_SET ) ;

	while( !SCHalFileEnd(file) ) {

		SCHalFileGetc ( file ) ;
		length ++ ;

	}

	SCHalFileSeek ( file , 0 , SEEK_SET ) ;
	
	return length ;

}



int SCHalFileClose ( void* file ) {
	
	//	author : Jelo Wang
	//	notes : fclose
	//	since : 20090809
	
	return fclose ( (FILE*)file ) ;

}

void SCMemcpy ( void* target_memory , void* source_memory , int length ) {
	
	//	author : Jelo Wang
	//	since : 20100107

	memcpy ( target_memory , source_memory , length ) ; 

}

