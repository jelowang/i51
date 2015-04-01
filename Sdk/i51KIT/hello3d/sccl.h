
/*

+	Semo Compiler Common Library 

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

# ifndef __SCCL
# define __SCCL

# define SC_SCCL_TEST

//	the default length of SCCString
//	create a SCCString with SCClSLEN could accelerate it compare with realloc
# define SCClSLEN 1024

typedef enum {

	//	author : Jelo Wang
	//	since : 20100609
 
	//	notes : seek

	SCCLLISTSEEK_HEAD ,
	SCCLLISTSEEK_TAIL ,

} SCCLLISTSEEK ;

typedef struct {

	//	author : Jelo Wang
	//	notes : SCClString
	//	since : 20090816
	//	(C)TOK

	char* data ;
	//	length of data
	int length ;
	int last_walker ;
	//	actual length of contents about the data
	int add_walker ;
	int get_walker ;

} SCClString ;

typedef struct ILN {

	//	author : Jelo Wang
	//	notes : SLIST
	//	since : 20091123
	//	(C)TOK

	//	data type of element
	int eltype ;
	int totall ;

	void* element ;
	
	struct ILN* head ;
	struct ILN* front ;
	struct ILN* next ;
	
} SCClList ;

typedef struct ISTK {

	//	author : Jelo Wang
	//	since : 20090819
	//	(C)TOK
	
	void* element ;

	int deep ;
	int number ;
	
	struct ISTK* base ;		
	struct ISTK* next ;

} SCClStack ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	SCClList data ;
	SCClList* front ;
	SCClList* rear ;
	
} SCClQueue ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	char* name; 
	int id ;
	int degree ;
	int color ;

	//	handle to anothers 
	void* handle ;
	
	int x ; 
	int y ;	
	
	//	neighbor
	SCClList nei ;
	
	
} SCClGraphNode ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	//	none-direction graph

	//	nodes list
	SCClList nl ;
	//	totall nodes
	int totall ;
	
} SCClGraph ;

# endif

//	generic strings func
extern int sc_randex ( int bottom , int upper ) ;
extern int sc_is_blank ( unsigned char element ) ;
extern int sc_is_space ( unsigned char element ) ;
extern int sc_is_alpha ( char el ) ;
extern int sc_is_digit ( char el ) ;
extern int sc_is_symbol ( char el ) ;
extern int sc_strlen ( char* str ) ;
extern int sc_strcpy_ex ( char** T , char* S ) ;
extern int sc_strcpy ( char* T , char* S ) ;
extern int sc_strcpy_withlen ( char* T , char* S , int length ) ;
extern int sc_strcmp ( char* T , char* S )  ;
extern int sc_strcmplen ( char* T , char* S ) ;
extern int sc_substrcmp ( char* A , char* B , int start ) ;
int sc_substrcmp_ex ( char* A , char* B , int start , char border ) ;
extern int sc_strfuzcmp ( char* A , char* B ) ;
extern char* sc_strnew ( char* S ) ;
extern char* sc_strcat ( char* A , char* B ) ; 
extern void sc_strcat_ex ( char* A , char* B , char* C ) ;
extern char* sc_substr_with_pos ( char* A , char* B , int pos ) ;
extern char* sc_substr_with_atom ( char* A , char atom ) ;
extern char* sc_substr ( char* A , int start , int end ) ;
extern void sc_strtrim ( char* S ) ;
extern int sc_memset ( void* , int , int ) ;
extern void sc_substrcpy ( char* A , char* B , int start ) ;
extern void sc_submemset ( unsigned char* buffer , int data , int offset , int end ) ;
extern void sc_back_insert ( char* path , char* name , int pos ) ;
extern void sc_strinsert ( char* A , char* S , int start ) ;

//	SCClString
extern SCClString* SCClStringNew () ;
extern SCClString* SCClStringCreate ( char* el , int len ) ;
extern void SCClStringInit ( SCClString* string ) ;
extern void SCClStringInitEx ( SCClString* string , int len ) ;
extern void SCClStringRepStr ( SCClString* A , char* S , int start , int len_d ) ;
extern void SCClStringRepStrMulti ( SCClString* A , char* B , char* C ) ;
extern void SCClStringRepStrMultiEx ( SCClString* A , char* B , char* C , char border ) ;
extern void SCClStringReset ( SCClString* string ) ;
extern void SCClStringAdd ( SCClString* string , char el ) ;
extern int SCClStringAddStr ( SCClString* string , char* el ) ;
extern void SCClStringInsert ( SCClString* A , char* S , int start ) ;
extern void SCClStringReplaireAtom ( SCClString* A , char atom , int start , int end ) ;
extern void SCClStringDestroy ( SCClString* string ) ;
extern void SCClStringDestroyEx ( SCClString* string ) ;

//	SCClList
extern void SCClListInit ( SCClList* list ) ;
extern SCClList* SCClListNew () ;
extern void SCClListInsert ( SCClList* listwalker , void* el ) ;
extern void SCClListInsertEx ( SCClList* list , void* el , int eltype ) ;
extern int SCClListEmpty ( SCClList* list ) ;
extern void SCClListConect ( void* N1 , void* element , void* N3  ) ;
extern int SCClListSetIterator ( void* lt , int position ) ;
extern int SCClListIteratorPermit ( void* lt ) ;
extern void* SCClListSearch ( void* lt , void* element ) ;
extern void* SCClListIteratorGetElement ( void* lt ) ;
extern int SCClListListIteratorNext ( void* lt ) ;
extern void* SCClListSearchBigestElement ( void* lt ) ;
extern void SCClListDeleteBetweenTwo ( void* N1 , void* N3 ) ;
extern void SCClListDestroy ( SCClList* list ) ;
extern void SCClListDestroyEx ( SCClList* list ) ;
extern int SCClAtoi ( char* number ) ;
extern char* SCClItoa ( int number ) ;

//	SCClStack
extern void SCClStackInit ( SCClStack* stack ) ;
extern int SCClStackGet ( SCClStack* stack  ) ;
extern int SCClStackEqual ( SCClStack* stack  , void* element ) ;
extern int SCClStackEmpty ( SCClStack* stack ) ;
extern void SCClStackPush ( SCClStack* stack , void* element ) ;
extern void* SCClStackPop ( SCClStack* stack ) ;
extern int SCClStackLook ( SCClStack* stack , void* element ) ;
extern int SCClStackGetDeep ( SCClStack* stack ) ;
extern void SCClStackDestroy ( SCClStack* stack ) ;

//	SCClQueue
extern void SCClQueueInit ( SCClQueue* queue ) ;
extern void SCClQueueEnter ( SCClQueue* queue , void* element ) ;
extern int SCClQueueOut ( SCClQueue* queue ) ;
extern int SCClQueueEmpty ( SCClQueue* queue ) ;
extern void SCClQueueDestroy ( SCClQueue* queue ) ;

//	SCClGraph
extern void SCClGraphInit ( SCClGraph* graph ) ;
extern SCClGraph* SCClGraphCreate () ;
extern SCClGraphNode* SCClGraphAddNode ( SCClGraph* graph , int N , void* handle ) ;
extern SCClGraphNode* SCClGraphSearchNode ( SCClGraph* graph , int N ) ;
extern void SCClGraphAddEdge (  SCClGraph* graph , int N1 , int N2 ) ;
extern int SCClGraphHaveEdge ( SCClGraph* graph , int N1 , int N2 ) ; 
extern int SCClGraphGetDegree ( SCClGraph* graph , int N ) ; 
extern int SCClGraphColoring ( SCClGraph* graph , int totall_colors ) ; 
extern void SCClGraphDelete ( SCClGraph* graph , int N ) ; 
extern void SCClGraphDestroy ( SCClGraph* graph ) ; 

