
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

# ifndef __SCHAL
# define __SCHAL

# include <stdio.h>
# include <assert.h>

# define ASSERT assert

# define SEEK_HEAD SEEK_SET

extern void* SCNormalloc ( unsigned int length , char* , int ) ;

# define SCMalloc(length) SCNormalloc(length,__FILE__,__LINE__)

typedef struct MMN {

	//	author : Jelo Wang
	//	since : 20100417
	//	(C)TOK
	
	char* file ;
	int line ;
	int length ;
	int address ;
	
	struct MMN* head ;	
	struct MMN* next ;
	
} MEMORY_MONITOR ;

# endif

extern void* SCRealloc ( void* buffer , long int length ) ;
extern void* SCRemalloc ( void* buffer , long int bufferlen , long int length )  ;
extern int SCFree ( void* buffer ) ;
extern int SCFreeEx ( void** buffer ) ;
extern void SCHalMemoryOverflowed () ;
extern int SCHalMemoryLeaked () ;
extern void* SCHalFileOpen ( char* path , char* flag ) ;
extern int SCHalFileSeek( void* file , int offset , int direct ) ;
extern int SCHalFileRead ( void* file , void* buffer , int size , int counter ) ;
extern int SCHalFileWrite ( void* file , void* buffer , int size , int counter ) ;
extern short int SCHalFileGetc ( void* file ) ;
extern int SCHalFileEnd ( void* file ) ;
extern int SCHalFileLength( void* file ) ;
extern int SCHalFileClose ( void* file ) ;
extern char* SCHalGetFilePath ( char* name ) ;
extern void SCMemcpy ( void* target_memory , void* source_memory , int length ) ;
extern void SClog ( const	 char* SClog , ... ) ;
extern void SCTerminate () ;

