
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

# include "schal.h"
# include "sccl.h"

int sc_randex ( int bottom , int upper ) {

	//	author : Jelo Wang
	//	since : 20110120
	//	(C)TOK

	int value = rand () % upper ;
 
	while ( value < bottom ) {
		value = rand () % upper ;
	}

	return value ;
	
}

int sc_is_blank ( unsigned char element ) {

	//	author : Jelo Wang
	//	notes : is_blank_token
	//	since : 20090810
	//	(C)TOK

	if ( ' ' == element || '\t'== element ||'\r'== element || '\n'== element )
		return 1;
		return 0;

}

int sc_is_space ( unsigned char element ) {

	//	author : Jelo Wang
	//	notes : is_blank_token
	//	since : 20090828
	//	(C)TOK

	if ( ' ' == element || '\t'== element )
		return 1;
		return 0;

}

int sc_is_alpha ( char el ) {

	//	author : Jelo Wang
	//	since : 20090812
	//	(C)TOK

	return ( ('a' <= el && el <= 'z' ) || ('A' <= el && el <= 'Z' ) ) ? 1 : 0 ;

}

int sc_is_digit ( char el ) {

	//	author : Jelo Wang
	//	since : 20090812
	//	(C)TOK

	return ( ('0' <= el && el <= '9' ) ) ? 1 : 0 ;

}

int sc_is_symbol ( char el ) {

	//	author : Jelo Wang
	//	since : 20090828
	//	(C)TOK

	return ( ( 0x21 <= el && el <= 0x7e ) ) ? 1 : 0 ;

}

int sc_strlen ( char* str ) {

	//	author : Jelo Wang
	//	since : 20091120
	//	(C)TOK
	
	int length = 0 ;

	if ( !str ) return length ;

	for ( ; *str++ ; length ++ ) ;

	return length  ;
	

}

int sc_strcpy ( char* T , char* S ) {

	//	author : Jelo Wang
	//	since : 20091122
	//	(C)TOK

	int walker = 0 ;
	
	if ( !T || !S ) return 0 ;
	
	while ( *S ) *T++ = *S++;

	*T = *S ;
	
	return 1 ;
	

}

int sc_strcpy_ex ( char** T , char* S ) {

	//	author : Jelo Wang
	//	since : 20091122
	//	(C)TOK

	int walker = 0 ;
	char* R = 0 ;
	
	if ( !*T ) *T = (char* ) SCMalloc ( sc_strlen (S)+1 ) ;
	if ( !*T ) return 0 ;
	
	R = *T ;

	while ( *S ) *R++ = *S++;

	*R = *S ;
	
	return 1 ;
	

}

int sc_strcpy_withlen ( char* T , char* S , int length ) {

	//	author : Jelo Wang
	//	since : 20091122
	//	(C)TOK

	int walker = 0 ;

	for ( ; *S && walker < length ; ) {
		*T  ++ = *S ++ ; 
		walker ++ ;
	}

	*T = *S ;
		
	return 1 ;
	

}

int sc_strcmp ( char* T , char* S ) {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK

	int tlen = sc_strlen (T) ;
	int slen = sc_strlen (S) ;

	if ( tlen != slen ) 
		return 1 ;

	while ( *T && *S ) 
		if ( *T ++ != *S ++ ) 
			return 1 ;
	
	return 0 ;
	

}

int sc_strcmplen ( char* T , char* S ) {

	//	author : Jelo Wang
	//	since : 20100421
	//	(C)TOK

	//	return 0 if T == S
	//	return 1 if len(T) > len(S)
	//	return -1 if len(T) < len(S)

	int tlen = sc_strlen (T) ;
	int slen = sc_strlen (S) ;
	
	if ( tlen < slen ) return -1 ;
	else if ( tlen == slen ) return 0 ;
	else if ( tlen > slen ) return 1 ;
	
	return 1 ;

}

int sc_memset ( void* memory , int value , int length ) {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK
	
	unsigned char* buffer = (unsigned char*) memory ; 

	int walker = 0 ;

	for ( walker = 0 ; walker < length ; walker ++ ) {
		buffer [ walker ] = value ;
	}

	return 1 ;
	
}

char* sc_strnew ( char* S ) {


	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK

	char* T = 0 ;

	if ( !S ) return 0 ;

	T = (char* ) SCMalloc ( sc_strlen (S)+1 ) ;

	if ( !T ) return 0 ;
	
	sc_strcpy ( T , S ) ;

	return T ;
	
}

char* sc_substr_with_pos ( char* A , char* B , int pos ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	//	notes : substr from A at pos , save the results in B

	int len = sc_strlen (A) ;
	int walker = 0 ;
	
	if ( pos >= len )
		return 0 ;

	B = (char* ) SCMalloc ( len ) ; 

	if ( !B ) return 0 ;

	for ( walker = 0  ; pos < len && ( sc_is_alpha(A [ pos ]) || sc_is_digit(A [ pos ])||'_'==A [ pos ]) ; pos ++ , walker ++ ) {

		B [ walker ] = A [ pos ]  ;

	}

	B [ walker ] =  '\0' ;

	return B ;


}
	
char* sc_substr_with_atom ( char* A , char atom ) {

	//	author : Jelo Wang
	//	since : 20110406
	//	(C)TOK
	
	int len = sc_strlen (A) ;
	int walker = 0 ;
	
	char* B = (char* ) SCMalloc ( len ) ; 

	if ( !B ) return 0 ;

	for ( walker = 0  ; '\0' != A [ walker ] && atom != A [ walker ] ; walker ++ ) {
		B [ walker ] = A [ walker ]  ;
	}

	B [ walker ] =  '\0' ;

	return B ;

}

char* sc_substr ( char* A , int start , int end ) {

	//	author : Jelo Wang
	//	since : 20090824
	//	(C)TOK

	int walker = start;
	int counter = 0 ;

	char* results = (char* ) SCMalloc ( end ) ;

	if ( !results ) return 0 ;

	for ( ; walker < sc_strlen(A) && walker < start + end ; walker ++ , counter ++ )
		results [ counter ] = A[walker];
	
	results [counter] = 0;
	return results ;


}

char* sc_strcat ( char* A , char* B ) {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK
	
	char* C = 0 ;
	int walker = 0 ;
	int length_of_A = sc_strlen (A) ;
	int length_of_B = sc_strlen (B) ;

	if ( !A || !B ) return 0 ;

	C = (char* ) SCMalloc ( length_of_A + length_of_B + 1 ) ;

	for ( walker = 0 ; walker < length_of_A ; walker ++ ) 
		C [ walker ] = A [ walker ] ;

	for ( ; walker < length_of_A + length_of_B ; walker ++ ) 
		C [ walker ] = B [ walker-length_of_A ] ;
	
	C [ walker ] = '\0' ;

	return C ;
	
}

void sc_strcat_ex ( char* A , char* B , char* C ) {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK


	int walker = 0 ;
	int length_of_A = sc_strlen (A) ;
	int length_of_B = sc_strlen (B) ;

	if ( !A || !B ) return ;

	for ( walker = 0 ; walker < length_of_A ; walker ++ ) 
		C [ walker ] = A [ walker ] ;

	for ( ; walker < length_of_A + length_of_B ; walker ++ ) 
		C [ walker ] = B [ walker-length_of_A ] ;
	
	C [ walker ] = '\0' ;

	

}


void sc_strtrim ( char* S ) {

	// author : Jelo Wang
	// since : 20100311
	// (C)TOK
 
	int len_s = sc_strlen (S) ;
	int s_walker = 0 ;
	int a_walker = 0 ;
	char* A = (char* ) SCMalloc ( len_s + 1 ) ;

	for ( s_walker = 0 ; s_walker < len_s ; s_walker ++ ) {
  
 		if ( 0x20 != S [ s_walker ] && '\t' != S [ s_walker ] ) {
			A [ a_walker ] = S [ s_walker ] ;
			a_walker ++ ;
		}
 
 
	} 
 
	A [ a_walker ] = '\0' ;

	sc_strcpy ( S , A ) ;

}

void sc_substrcpy ( char* A , char* B , int start ) {

	//	author : Jelo Wang
	//	since : 20090812
	// (C)TOK
	
	int walker = start;
	int counter = 0 ;

	for ( ; counter < sc_strlen(B); walker ++ , counter ++ )
		A [ walker ] = B [ counter ] ;


}

int sc_substrcmp ( char* A , char* B , int start ) {

	//	author : Jelo Wang
	//	since : 20090816
	// (C)TOK
	
	///	notes : whole words macthing

	int walker = start;
	int counter = 0 ;
	int length_of_B = sc_strlen (B) ;
	int lenth_of_A = sc_strlen (A) ;

	for ( ; counter < length_of_B; walker ++ , counter ++ ) {
		if ( A [ walker ] != B [ counter ] ) 
			return 1;
	}

	if ( lenth_of_A == length_of_B )
		return 0 ;

	if ( counter < length_of_B )
		return 1 ;
	
	if ( counter >= length_of_B )
		return 0 ;

# if 0
	if ( walker < lenth_of_A ) {
		if ( sc_is_alpha(A [ walker ]) )
			return 1 ;
		if ( sc_is_alpha(A[walker-length_of_B-1]) )
			return 1 ;
	} else if ( walker == lenth_of_A ) {
		
		if ( sc_is_alpha(A[walker-length_of_B-1]) )
			return 1 ;

	}
# endif

	return 0;


}

int sc_substrcmp_ex ( char* A , char* B , int start , char border ) {

	//	author : Jelo Wang
	//	since : 20090816
	// (C)TOK
	
	///	notes : whole words macthing

	int walker = start ;
	int length_of_matched = 0 ;
	int counter = 0 ;
	int length_of_B = sc_strlen (B) ;
	int lenth_of_A = sc_strlen (A) ;

	if ( lenth_of_A == length_of_B )
		return strcmp ( A , B ) ;

	if ( 0x20 != A [ walker ] && '\t' != A [ walker ] && '\r' != A [ walker ] && '\n' != A [ walker ] && '\\' != A [ walker ] && border != A [ walker ] ) {
		if ( 0 == sc_is_alpha ( A[walker] ) )
			return 1 ;
	}

	//	skip 
	while ( 0x20 == A [ walker ] || '\t' == A [ walker ] || '\r' == A [ walker ] || '\n' == A [ walker ] || '\\' == A [ walker ] 
		|| border == A [ walker ] ) {
		walker ++ ;
	}
	
	for ( ; counter < length_of_B; walker ++ , counter ++ , length_of_matched ++ ) {
		if ( A [ walker ] != B [ counter ] ) 
			return 1;
	}

	if ( length_of_matched < lenth_of_A ) {
		if ( 0 != sc_is_alpha ( A[walker] ) || 0 != sc_is_digit ( A[walker] ) || '_' == A[walker] )
			return 1 ;
	}
	
	return 0;


}

int sc_strfuzcmp ( char* A , char* B ) {

	//	author : Jelo Wang
	//	since : 20100505
	// 	(C)TOK
	
	///	notes : fuzzy compare

	int len_b = sc_strlen (B) ;
	int len_a = sc_strlen (B) ;
	int walker = 0 ;

	if ( len_b > len_a ) return 0 ;

	for ( walker = 0 ; walker < len_b ; ) {
		if ( A[walker] == B[walker] ) walker ++ ;
		else break ;
	}

	if ( walker >= len_b ) return 1 ;
	
	return 0 ;


}

int sc_substr_search ( char* A , char* B ) {

	// AUTHOR : Jelo Wang
 	// since : 20100323
	// (C)TOK
	
 	// search substring B in A

 	int walker_A = 0 ;
 	int walker_B = 0 ; 
 	int len_of_A = sc_strlen (A) ;
 	int len_of_B = sc_strlen (B) ;

 	for ( walker_A = 0 ; walker_A < len_of_A && walker_B < len_of_B ; ) {
		if ( A [ walker_A ] == B [ walker_B ] ) {
   			walker_A ++ ;
   			walker_B ++ ;
  		} else {
   			walker_A ++ ;
   			walker_B = 0 ;
  		}
 	}

 	if ( walker_B == len_of_B ) return 1 ;
 	return 0 ;

}

void sc_submemset ( unsigned char* buffer , int data , int offset , int end ) {

	//	author : Jelo Wang
	//	since : 20090818
	// (C)TOK
	
	//	set buffer with data from offset to end

	int walker ;

	for( walker = offset ; walker < offset + end ; walker ++ )
		buffer [ walker ] = data ;

}

void sc_strinsert ( char* A , char* S , int start ) {

	//	author : Jelo Wang
	//	since : 20110119
	//	(C)TOK

	//	insert S into A from start to sc_strlen(S)
	
	int walker = 0 ;
	int counter = 0 ;
	int move_step = 0 ;
	int move_border = 0 ;

	int len_s = sc_strlen (S) ;
	int A_len = sc_strlen (A) ;

	move_step = len_s ;
	move_border = start ;

	for ( walker = A_len - 1 ; walker >= move_border ; walker -- ) {
		A [ move_step + walker ] = A [ walker ] ;
	}

	for ( counter = start ; counter < start + len_s ; counter ++ ) {
		A [ counter ] = S [ counter - start ] ;
	}

	A [ A_len+len_s ] = '\0' ;

}

void sc_back_insert ( char* path , char* name , int pos ) {

	//	author : Jelo Wang
	//	notes : sc_back_insert
	//	since : 20090831
	//	(C)TOK

	int walker = 0 ;

	for ( walker = pos ; '\0' != name [ walker - pos ]  ; walker ++ ) {
		path [ walker ] = name [ walker - pos ] ;
	}

	path [ walker ] = '\0' ;

}

SCClString* SCClStringNew () {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	SCClString* string = SCMalloc ( sizeof(SCClString ) ) ;

	SCClStringInit ( string ) ;

	return string ;
	
}

SCClString* SCClStringCreate ( char* el , int len ) {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK

 	SCClString* string = (SCClString* ) SCMalloc ( sizeof(SCClString) ) ;

	int walker = 0 ;
	
	if ( !string )
		return 0 ;
 
	string->data = (char* ) SCMalloc ( len + 1 ) ; 
	//	length of the data buffer
	string->length = len + 1 ;

	if ( !string->data )
		return 0 ;

	sc_strcpy ( string->data , el ) ;	
	string->add_walker = len ;
	string->get_walker = 0 ;
	string->last_walker = 0 ;

	return string ;


}

void SCClStringInit ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK
	
	if ( 0 < string->data ) SCFree ( string->data ) ;

	string->add_walker = 0 ;
	string->get_walker = 0 ;
	string->last_walker = 0 ;
	
	string->data = (char* ) SCMalloc ( SCClSLEN + 1 ) ; 
	string->length = SCClSLEN + 1 ;

}

void SCClStringInitEx ( SCClString* string , int len ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK
	
	if ( 0 < string->data ) SCFree ( string->data ) ;
	
	string->data = (char* ) SCMalloc ( len + 1 ) ; 
	string->add_walker = 0 ;
	string->get_walker = 0 ;
	string->last_walker = 0 ;
	string->length = len + 1 ;

}

void SCClStringReset ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK
	
	string->add_walker = 0 ;
	string->get_walker = 0 ;
	string->last_walker = 0 ;
	
}

void SCClStringAdd ( SCClString* string , char el ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	//	SCClStringAdd would add a '\0' at the end of SCClString
	if ( 0 < string->add_walker && '\0' == string->data [string->add_walker-1] ) {
		
		string->add_walker = string->add_walker - 1 ;

	} 

	if ( string->add_walker + sizeof(char) >= string->length ) {

		string->data = (char* ) SCRemalloc ( string->data , string->length , string->length + sizeof(char) + 1 ) ;
		//string->data = (char* ) SCRealloc ( string->data , string->length + sizeof(char) + 1 ) ;
		string->length = string->length + sizeof(char) + 1 ;		

	}

	string->data [ string->add_walker ] = el ;
	string->add_walker ++ ;

}

int SCClStringAddStr ( SCClString* string , char* el ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	int counter = 0 ;
	int ellen = 0 ;
	
	if ( !el ) return 0 ;
	
	if ( '\0' == el[0] ) ellen = 1 ;
	else ellen = sc_strlen (el) ;

	//	the first time of adding
	if ( 0 == string->length && ellen ) {

		string->data = (char* ) SCMalloc ( ellen + 1 ) ;
		string->length = ellen + 1 ;
		string->add_walker = ellen ;
		sc_strcpy ( string->data , el ) ;

		return 1 ;

	} 

	//	SCClStringAdd would add a '\0' at the end of SCClString
	if ( 0 < string->add_walker && '\0' == string->data [string->add_walker-1] )
	{
		string->add_walker = string->add_walker - 1 ;
	}
	
	if ( string->add_walker + ellen >= string->length ) {

		string->data = (char* ) SCRemalloc ( string->data , string->length , string->length + ellen + 1 ) ;
		//string->data = (char* ) SCRealloc ( string->data , string->length + ellen + 1 ) ;
		string->length += ellen + 1 ;

	}

	for ( counter = 0 ; counter < ellen ; counter ++ ) {
	 
		string->data [ string->add_walker ] = el [ counter ] ;
		string->add_walker ++ ;

	}

	return 1 ;

}

void SCClStringInsert ( SCClString* A , char* S , int start ) {

	//	author : Jelo Wang
	//	since : 20090830
	//	(C)TOK

	int walker = 0 ;
	int counter = 0 ;
	int move_step = 0 ;
	int move_border = 0 ;

	int len_s = sc_strlen (S) ;	

	//	Jelo Edited 20120314
	//	SCClStringAdd would add a '\0' at the end of SCClString
	//while ( 0 < A->add_walker && '\0' == A->data [A->add_walker-1] || 
	//	'\0' == A->data [A->add_walker] ) {
	//	A->add_walker = A->add_walker - 1 ;
	//}
	//	Jelo Edited Finished

	if ( (len_s + A->add_walker) >= A->length ) {

		A->data = (char* ) SCRemalloc ( A->data , A->length , A->length + len_s + 1 ) ;
		//A->data = (char* ) SCRealloc ( A->data , A->length + len_s + 1 ) ;
		A->length = A->length + len_s + 1 ;
		
	}
	
	move_step = len_s ;
	move_border = start ;

	for ( walker = A->add_walker ; walker >= move_border ; walker -- ) {
		A->data [ move_step + walker ] = A->data [ walker ] ;
	}

	for ( counter = start ; counter < start + len_s ; counter ++ ) {
		A->data [ counter ] = S [ counter - start ] ;
	}

	A->add_walker = A->add_walker + move_step ;
	
	if ( A->add_walker == A->length ) {
		A->data [ A->add_walker ] = '\0' ;
	}

}

void SCClStringReplaireAtom ( SCClString* A , char atom , int start , int end ) {

	//	author : Jelo Wang
	//	since : 20090921
	//	(C)TOK

	int walker = 0 ;
	int counter = 0 ;

	for ( counter = start ; counter < start + end && counter < A->length ; counter ++ ) {
		A->data [ counter ] = atom ;
	}

}
	
void SCClStringRepStr ( SCClString* A , char* S , int start , int len_b) {

	//	author : Jelo Wang
	//	since : 20090816
	//	updated : 20090827
	//	(C)TOK

	//	notes : replaire a substr of B from A with C
	//	notes : if start > sc_strlen(A) the length of A would be realloc 

	int walker = 0 ;
	int counter = 0 ;
	int move_step = 0 ;
	int move_border = 0 ;

	int len_s = sc_strlen ( S ) ;

	if ( (A->length + (len_s - len_b)) >= A->length ) {
		
		A->length = A->length + (len_s - len_b) + 1 ;
		A->data = (char* ) SCRemalloc ( A->data , A->length , A->length + 1 ) ; 
		//A->data = SCRealloc ( A->data , A->length + 1 ) ;
		
	}

	if ( len_s == len_b ) {
	//	len_s == len_b
	//	step = 1 , border = A -> length (no need to move anything)
		move_step = 0 ;
		move_border = A->length ;

		for ( counter = start ; counter < start + len_s ; counter ++ ) {
			A->data [ counter ] = S [ counter - start ] ;
		}
	
	} else if ( len_s > len_b ) {

	//	len_s > len_b
	//	step = len_s - len_b , border = start + len_b
		move_step = len_s - len_b ;
		move_border = start + len_b ;	
		
		//	the last length of data = A->length-(len_s - len_b)-1
		//for ( walker = A->length - (len_s - len_b)-1; walker >= move_border ; walker -- ) {
		for ( walker = A->add_walker ; walker >= move_border ; walker -- ) {
			A->data [ move_step + walker ] = A->data [ walker ] ;
		}

		for ( counter = start ; counter < start + len_s ; counter ++ ) {
			A->data [ counter ] = S [ counter - start ] ;
		}

		A->add_walker = A->length - 1 ;
		A->data [ A->add_walker ] = '\0' ;
			
	} else if ( len_b > len_s ) {
	//	len_b > len_s
	//	step = 1 , border = start + len_b	
		move_step = 0 ;
		move_border = start + len_b ;	

		for ( counter = start ; counter < start + len_s ; counter ++ ) {
			A->data [ counter ] = S [ counter - start ] ;
		}	

		for ( ; counter < start + len_b ; counter ++ ) {
			A->data [ counter ] = 0x20 ;
		}	

	} 
	
}

void SCClStringRepStrMulti ( SCClString* A , char* B , char* C ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	//	notes : lookup all substr B in A , then replaire B with C
	//	input : A = "asdf b sadf asdf bbb sdf b"
	//	input : B = b
	//	input : C = B

	//	output: A = "asdf B sadf asdf bbb sdf B"

	int walker = 0 ;
	int counter = 0 ;

	int len_b = sc_strlen(B) ;

	if ( !B || !C ) return ;

	for ( walker = 0 ; walker < A->length ; walker ++ ) {
		
		if ( !sc_substrcmp ( A->data , B , walker ) ) {

			SCClStringRepStr ( A , C , walker , len_b ) ;

		}

	}


}


void SCClStringRepStrMultiEx ( SCClString* A , char* B , char* C , char border ) {

	//	author : Jelo Wang
	//	since : 20120314
	//	(C)TOK

	//	notes : lookup all substr B in A , then replaire B with C
	//	input : A = "asdf b sadf asdf bbb sdf b"
	//	input : B = b
	//	input : C = B

	//	output: A = "asdf B sadf asdf bbb sdf B"

	int walker = 0 ;
	int counter = 0 ;

	int len_b = sc_strlen(B) ;

	if ( !B || !C ) return ;

	for ( walker = 0 ; walker < A->length ; walker ++ ) {
		
		if ( !sc_substrcmp_ex ( A->data , B , walker , border ) ) {
			
			//	skip 
			while ( 0x20 == A->data [ walker ] || '\t' == A->data [ walker ] 
			|| '\r' == A->data [ walker ] || '\n' == A->data [ walker ] 
			|| '\\' == A->data [ walker ] || border == A->data [ walker ] ) {
				walker ++ ;
			}

			SCClStringRepStr ( A , C , walker , len_b ) ;

		}

	}


}


void SCClStringDestroy ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	if ( !string ) return ;
	
  	if ( string->data ) SCFree ( string->data ) ;

	SCFree ( string ) ;

	
}

void SCClStringDestroyEx ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20110701
	//	(C)TOK

	if ( !string ) return ;
	
  	if ( string->data ) 
		SCFree ( string->data ) ;

	memset ( string , 0 , sizeof(SCClString) ) ;
	
}

void SCClListInit ( SCClList* list ) {

	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	list->element = 0 ;
	list->totall = 0 ;
	list->front = 0 ;
	list->head = 0 ;
	list->next = 0 ;

}

SCClList* SCClListNew () {

	//	author : Jelo Wang
	//	since : 20102806
	//	(C)TOK

	SCClList* list = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if ( !list ) return 0 ;
	
	list->element = 0 ;
	list->totall = 0 ;
	list->front = 0 ;
	list->head = 0 ;
	list->next = 0 ;

	return list ;

}

void SCClListInsert ( SCClList* list , void* el ) {

	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	SCClList* listnew = 0 ;

	if ( !list ) return ;

	listnew = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if( !listnew ) return ;
	
	listnew->element = el ;
	listnew->next = 0 ;
	
	if ( 0 == list->head ) {
		list->head = listnew ;
		listnew->front = list ;
		list->next = listnew ;		
		list->totall = 1 ;
	} else {
		list->next->next = listnew ;
		listnew->front = list->next ;
		list->next = listnew ;
		list->totall ++ ;
 	}
	
}

void SCClListInsertEx ( SCClList* list , void* el , int eltype ) {

	//	author : Jelo Wang
	//	since : 20100718
	//	(C)TOK

	SCClList* listnew = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if ( !list ) return ;
	else if( !listnew ) return ;

	listnew->eltype = eltype ;	
	listnew->element = el ;
	listnew->next = 0 ;
	
	if ( 0 == list->head ) {
		list->head = listnew ;
		listnew->front = list ; 
		list->next = listnew ;		
		list->totall = 1 ;		
	} else {
		list->next->next = listnew ;
		listnew->front = list->next ;
		list->next = listnew ;
		list->totall ++ ;		
	}
	
}

int SCClListEmpty ( SCClList* list ) {

	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK

	if ( !list ) return 1 ;

	if ( !list->head ) return 1 ;

	return 0 ;
	
}

int SCClListSetIterator ( void* lt , int position ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : set an iterator

	SCClList* list = (SCClList* ) lt ;
	SCClList* walker = 0 ;

	if ( 0 == list ) return 0 ;
	
	if ( SCCLLISTSEEK_HEAD == position ) {
		//	将迭代器指向头节点
		list->next = list->head ; 
	} else if ( SCCLLISTSEEK_TAIL == position ) {
		//	将迭代器指向尾节点
		for ( walker = list->head ; walker && walker->next ; walker = walker->next ) ;
		if ( walker ) list->next = walker ;
	}

	return 1 ; 

}

int SCClListIteratorPermit ( void* lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : 判断一个迭代器是否可以继续运行

	SCClList* list = (SCClList* ) lt ;
	SCClList* walker = 0 ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	return 1 ; 

}

void* SCClListIteratorGetElement ( void* lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : get an element

	SCClList* list = (SCClList* ) lt ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	return list->next->element ;

}

int SCClListListIteratorNext ( void* lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : iterator next

	SCClList* list = (SCClList* ) lt ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	list->next = list->next->next ; 

	return 1 ;

}

void* SCClListSearch ( void* lt , void* element ) {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK

	
	SCClList* looper = 0 ;
	
	for ( looper = ((SCClList* )lt)->head ; looper ; looper = looper->next ) {
		if ( (int ) element == (int ) looper->element ) {
			return (int ) looper ;
		}
	}

	return 0 ;

}

void* SCClListSearchBigestElement ( void* lt ) {

	//	author : Jelo Wang
	//	since : 20100805
	//	(C)TOK

	void* cv = 0 ;
	void* lv = 0 ;
	
	SCClList* looper = 0 ;
	
	for ( looper = ((SCClList* )lt)->head ; looper ; looper = looper->next ) {
		cv = looper->element ;
		if ( cv > lv ) lv = cv ;
	}

	return lv ;

}

void SCClListDeleteBetweenTwo ( void* N1 , void* N3 ) {

	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK

	//	notes : delete all of the nodes between N1 to N3
	
	SCClList* N1A = (SCClList* )N1 ;
	SCClList* N3A = (SCClList* )N3 ;

	SCClList* walker = 0 ;
	
	if ( !N1A ) return ;

	for ( walker = N1A->next ; walker && (int)walker != (int)N3A ; ) {	
		N1A->next = walker->next ;
		SCFree ( walker ) ;
		walker = N1A->next ;
	}

	N1A->next = N3A ;
	if ( N3A ) N3A->front = N1A ;	
	
}

void SCClListConect ( void* N1 , void* element , void* N3  ) {

	//	author : Jelo Wang
	//	since : 20100806
	//	(C)TOK

	//	notes : create N2 width element , contect N1 with N2 an N3 like : N1->N2->N3
	//	delete all of the nodes between N1 to N3
	
	SCClList* N1A = (SCClList* )N1 ;
	SCClList* N2A = 0 ;
	SCClList* N3A = (SCClList* )N3 ;

	if ( !N1A ) return ;

	N2A = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;
	
	if ( !N2A ) return ;
	
	N2A->element = element ;
	N2A->next = N3A ;
	N2A->front = N1A ;

	N1A->next = N2A ;

	if ( N3A ) N3A->front = N2A ;
	
	
}

void SCClListDestroy ( SCClList* list ) {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	SCClList* walker = list->head ;

	while ( walker ) {
		list->next = walker->next ;
		SCFree ( walker ) ;
		walker = list->next ;
	}
	
}

void SCClListDestroyEx ( SCClList* list ) {

	//	author : Jelo Wang
	//	since : 20110705
	//	(C)TOK

	SCClList* walker = list->head ;

	while ( walker ) {
		list->next = walker->next ;
		SCFree ( walker ) ;
		walker = list->next ;
	}

	SCFree ( list ) ;
	
}


char* SCClItoa ( int number ) {

	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK
	
	static char buffer [ 32 ] ;

	itoa ( number , buffer , 10 ) ;

	return buffer ; 
	
}

int SCClAtoi ( char* number ) {

	//	author : Jelo Wang
	//	since : 20100726
	//	(C)TOK
	
	return atoi ( number ) ; 
	
}

void SCClStackInit ( SCClStack* istack ) {

	//	author : Jelo Wang
	//	since : 20090819

	istack->element = 0 ;
	istack->next = 0 ;
	istack->base = 0 ;
	istack->deep = 0 ;
	istack->number = 0 ;
 
}

int SCClStackGet ( SCClStack* istack  ) {

	//	author : Jelo Wang
	//	since : 20090819
	
	if ( istack->next )
		return istack->next->element ;
		return 0 ;
	
}

int SCClStackEqual ( SCClStack* istack  , void* element ) {

	//	author : Jelo Wang
	//	since : 20091123
	
	return (istack->next) ? (istack->next->element == element ) : 0 ;
	
}

long int SCClStackEmpty ( SCClStack* istack ) {

	//	author : Jelo Wang
	//	since : 20091123
	
	return (istack->next) ?  0  : 1 ;
	
}

void SCClStackPush ( SCClStack* istack , void* eelement ) {

	//	author	: Jelo Wang
	//	since	: 20090819
	
	SCClStack* nnode = 0 ;

	nnode = (SCClStack* ) SCMalloc ( sizeof(SCClStack)  ) ;
	nnode->element = eelement ;
	nnode->next = istack->next  ;
	istack->next = nnode ;
	istack->deep ++ ;
	nnode->number = istack->deep ;

	if ( 0 == istack->base )
		istack->base = nnode ;	
	
	
}

void* SCClStackPop ( SCClStack* istack ) {

	//	author	: Jelo Wang
	//	since	: 20090819
	
	long int el = 0 ;
	SCClStack* deln = 0 ;
	
	if ( !istack->next )
		return 0 ;

	el = istack->next->element ;

	deln = istack->next ;

	istack->next = deln->next ;

	SCFree ( deln ) ;

	istack->deep -- ;

	return el ;
	
	
}

int SCClStackLook ( SCClStack* istack , void* eelement ) {

	//	author	: Jelo Wang
	//	since	: 20090819
	
	SCClStack* nnode = istack->next ; 

	for ( ; nnode && nnode->element != eelement ; nnode = nnode->next ) ;

	return nnode ? 1 : 0 ;	

}

int SCClStackGetDeep ( SCClStack* istack ) {

	//	author	: Jelo Wang
	//	since	: 20100425

	return istack->deep ;
	
}

void SCClStackDestroy ( SCClStack* istack ) {

	//	author	: Jelo Wang
	//	since	: 20100425

	while ( !SCClStackEmpty (istack) )
		SCClStackPop ( istack ) ;

	istack->base = 0 ;

}

void SCClQueueInit ( SCClQueue* queue ) {

	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK

	SCClListInit ( &queue->data ) ;

	queue->front = queue->data.head ;
	queue->rear = queue->data.next ;
	
}

int SCClQueueEmpty ( SCClQueue* queue ) {

	//	author : Jelo Wang
	//	since : 20110119
	//	(C)TOK

	if ( !queue ) return 1 ;
	else if ( !queue->front ) return 1 ;
	else return 0 ;
	
}

void SCClQueueEnter ( SCClQueue* queue , void* element ) {
	
	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK

	SCClListInsert ( &queue->data , element ) ;

	queue->front = queue->data.head ;
	queue->rear = queue->data.next ;
	
}

int SCClQueueOut ( SCClQueue* queue ) {
	
	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK
	
	int node = queue->data.head ;
	int element = 0 ;
	
	if ( !queue->front ) return 0 ;

	element = queue->front->element ;

	queue->data.head = queue->data.head->next ;
	queue->front = queue->data.head ; 	

	SCFree ( (void* )node ) ;
  
	return element ;

}

void SCClQueueDestroy ( SCClQueue* queue ) {
	
	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK

	SCClListDestroy ( &queue->data ) ;
	
}

void SCClGraphInit ( SCClGraph* graph ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	if ( !graph ) return ;

	SCClListInit ( &graph->nl ) ;

}

SCClGraph* SCClGraphCreate () {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK
	
	return (SCClGraph* ) SCMalloc ( sizeof( SCClGraph ) ) ; 
	
}

SCClGraphNode* SCClGraphAddNode ( SCClGraph* graph , int N , void* handle ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK
	
	SCClGraphNode* node = 0 ;

	if ( !graph ) return 0 ;

	node = SCClGraphSearchNode ( graph , N ) ;

	//	this node is already exits
	if ( node ) return node ;
		
	node = (SCClGraphNode* ) SCMalloc ( sizeof(SCClGraphNode) ) ;
	
	if ( !node ) return 0 ;

	SCClListInit ( &node->nei ) ;

	node->degree = 0 ;
	node->id = N ;
	node->color = -1 ;
	node->handle = handle ;

	SCClListInsert ( &graph->nl , (void* )node  ) ;
	graph->totall ++ ;

	return node ;

}

SCClGraphNode* SCClGraphSearchNode ( SCClGraph* graph , int N ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	SCClList* llooper = 0 ;
	
	if ( !graph ) return 0 ;

	for ( llooper = graph->nl.head ; llooper ; llooper = llooper->next ) {

		SCClGraphNode* node = (SCClGraphNode* ) llooper->element ;

		if ( !node ) continue ;
		if ( N == node->id ) return node ;
		
	}

	return 0 ;
	
}

void SCClGraphAddEdge (  SCClGraph* graph , int N1 , int N2 ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	SCClGraphNode* node1 = 0 ;
	SCClGraphNode* node2 = 0 ;
	
	if ( !graph ) return ;

	node1 = SCClGraphSearchNode ( graph , N1 ) ;

	if ( !node1 ) return ;
	
	node2 = SCClGraphSearchNode ( graph , N2 ) ;

	if ( !node2 ) return ;

	SCClListInsert ( &node1->nei , (void* )node2 ) ;
	SCClListInsert ( &node2->nei , (void* )node1 ) ;

	node1->degree ++ ;
	node2->degree ++ ;
	
}

int SCClGraphHaveEdge ( SCClGraph* graph , int N1 , int N2 ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK
	
	SCClGraphNode* node1 = 0 ;
	SCClGraphNode* node2 = 0 ;
	
	if ( !graph ) return 0 ;

	node1 = SCClGraphSearchNode ( graph , N1 ) ;

	if ( !node1 ) return 0 ;
	
	node2 = SCClGraphSearchNode ( graph , N2 ) ;

	if ( !node2 ) return 0 ;
	
	SCClListSetIterator ( &node1->nei , SCCLLISTSEEK_HEAD ) ;

	for ( ; SCClListIteratorPermit ( &node1->nei ) ; SCClListListIteratorNext ( &node1->nei ) ) {
		
		SCClGraphNode* node = (SCClGraphNode* )SCClListIteratorGetElement ( &node1->nei ) ;
			
		if ( !node ) continue ;
		if (  N2 == node->id ) return 1 ;
		
	}	

	return 0 ;

}

int SCClGraphGetDegree ( SCClGraph* graph , int N ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	SCClGraphNode* node = 0 ;
	
	if ( !graph ) return -1 ;

	node = SCClGraphSearchNode ( graph , N ) ;	

	if ( !node ) return -1 ;

	return node->degree ;
	
}

void SCClGraphDelete ( SCClGraph* graph , int N ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK	

	SCClList* llooper = 0 ;
	
	SCClGraphNode* node = 0 ;
	
	if ( !graph ) return  ;
	
	for ( llooper = graph->nl.head ; llooper ; llooper = llooper->next ) {

		node = (SCClGraphNode* ) llooper->element ;

		if ( !node ) continue ;
		if ( N == node->id ) continue ;
		
	}

	if ( !llooper ) return ;

	//
	//	delete node on graph list
	llooper->front->next = llooper->next ; 
	llooper->next->front = llooper->front ;
	SCFree ( llooper ) ;
	//
	
	SCClListDestroy ( &node->nei ) ;

	SCFree ( node ) ;
	
	
}

static int SCClGraphGetColor ( SCClGraphNode* node , int totall_colors ) {

	//	author : Jelo Wang
	//	since : 20110118
	//	(C)TOK	

#if 1

	int colorwalker = 0 ;
	SCClList* looper = 0 ;
		
	for ( looper = node->nei.head , colorwalker = 0 ; looper && colorwalker < totall_colors ; looper = looper->next , colorwalker ++ ) {

		node = (SCClGraphNode* ) looper->element ;

		if ( -1 == node->color ) {
			//	Get a free color 
			//	Found a free color , return it
			return colorwalker ;
		} 
		
	}
	
	//	have a free color here
	if ( colorwalker < totall_colors ) {
		return colorwalker ;
	}

	return -1 ;
	
#elif

	SCClList* looper = 0 ;

	//	value of colors bettwen [0,totall_colors)
	int* colors = (int* ) SCMalloc ( sizeof(int)*totall_colors ) ; 

	for ( looper = node->nei.head ; looper ; looper = looper->next ) {

		node = (SCClGraphNode* ) looper->element ;

		//	记录邻接点的颜色
		if ( -1 != node->color ) {	
			//	邻接点颜色已被占用
			colors [ node->color ] = 1 ;
		}
		
	}

	{	
		int looper = 0 ;
		for ( looper = 0 ; looper < totall_colors ; looper ++ ) {
			//	顺序取一个没有占用的颜色
			if ( 0 == colors [looper] ) {	
				SCFree ( colors ) ;
				//	return color
				return looper ;
			}
		}
	}

	SCFree ( colors ) ;

	return -1 ;
#endif
	
}

int SCClGraphColoring ( SCClGraph* graph , int totall_colors ) {

	//	author : Jelo Wang
	//	since : 20110118
	//	(C)TOK	

	//	when processing of coloring graph is failed return LAIR handle
	//	else return -1

	SCClList* llooper = 0 ;
	SCClList* inlooper = 0 ;
	SCClGraphNode* node = 0 ;
	
	ASSERT(graph) ;
	
	for ( llooper = graph->nl.head ; llooper ; llooper = llooper->next ) {

		node = (SCClGraphNode* ) llooper->element ;

		//	node->handle is a handle of LAIR
		if ( node->degree >= totall_colors ) return (int )node->handle ;

		if ( -1 == node->color ) node->color = SCClGraphGetColor ( node , totall_colors ) ;

		//	neighbor
		for ( inlooper = node->nei.head ; inlooper ;  inlooper = inlooper->next ) {

			SCClGraphNode* innode = (SCClGraphNode* ) inlooper->element ;

			//	node->handle is a handle of LAIR
			if ( innode->degree >= totall_colors ) 
				return (int )innode->handle ;
		
			if ( -1 == innode->color ) innode->color = SCClGraphGetColor ( innode , totall_colors ) ;
			
		}
	
						
	}	
	
	return -1 ;
	
}

void SCClGraphDestroy ( SCClGraph* graph ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK	

	if ( !graph ) return ;

	SCClListSetIterator ( &graph->nl , SCCLLISTSEEK_HEAD ) ;

	for ( ; SCClListIteratorPermit ( &graph->nl ) ; SCClListListIteratorNext ( &graph->nl ) ) {
		
		SCClGraphNode* node = (SCClGraphNode* ) SCClListIteratorGetElement ( &graph->nl ) ;
		
		if ( !node ) continue ;

  		SCClListDestroy ( &node->nei ) ;
		SCFree ( node ) ;
		
	}
	
	SCClListDestroy ( &graph->nl ) ;
	
}

# ifdef SC_SCCL_TEST

void SCClStringTest ()
{

	//	author : Jelo Wang
	//	since : 20110701
	//	(C)TOK	
	
	//	测试SCClString 接口

	SCClString S = {0} ;

	SCClStringAddStr ( &S , "GTKINGS" ) ;
	SCClStringAdd ( &S , '\0' ) ;
	SCClStringAddStr ( &S , "TOK" ) ;
	SCClStringAdd ( &S , '\0' ) ;
	SClog ( "%s\n" , S.data ) ;
	SCClStringDestroyEx ( &S ) ;

	SCClStringAddStr ( &S , "GTKINGS" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringAddStr ( &S , "TOK" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SClog ( "%s\n" , S.data ) ;
	SCClStringDestroyEx ( &S ) ;

	SCClStringAddStr ( &S , "GTKINGS" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringAddStr ( &S , "TOK" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SClog ( "%s\n" , S.data ) ;
	SCClStringDestroyEx ( &S ) ;
	
	SCClStringAddStr ( &S , "GTKINGS" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringAddStr ( &S , "TOK" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringRepStrMulti ( &S , "SEMO" , "semo" ) ;
	SClog ( "%s\n" , S.data ) ;
	SCClStringDestroyEx ( &S ) ;

	SCClStringAddStr ( &S , "GTKINGS" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringAddStr ( &S , "TOK" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringRepStrMulti ( &S , "SEMO" , "s" ) ;
	SClog ( "%s\n" , S.data ) ;
	SCClStringDestroyEx ( &S ) ;

	SCClStringAddStr ( &S , "GTKINGS" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringAddStr ( &S , "TOK" ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringInsert ( &S , "SEMO" , 7 ) ;
	SCClStringAddStr ( &S , "\0" ) ;
	SCClStringRepStrMulti ( &S , "SEMO" , "Semo C()mpiler" ) ;
	SClog ( "%s\n" , S.data ) ;
	SCClStringDestroyEx ( &S ) ;	

	SCClStringAddStr ( &S , "sc" ) ;
	SCClStringAdd ( &S , 0x20 ) ;
	SCClStringAddStr ( &S , "ca.txt" ) ;
	SCClStringAdd ( &S , 0x20 ) ;
	SCClStringAddStr ( &S , "-cr" ) ;
	SCClStringAdd ( &S , 0x20 ) ;
	SCClStringAddStr ( &S , "-lair" ) ;
	SCClStringAdd ( &S , 0x20 ) ;	
	SCClStringAdd ( &S , '\0' ) ;
	SClog ( "%s\n" , S.data ) ;

}

# endif

