
/*

+	'Hello3d' is an implementation of OpenGL

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but WITHOUT ANY WARRANTY.without even the implied warranty of merchantability 
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
	
#include "GL.h"

# define RENDERRGB565COLOR(color,red,green,blue)\
	color = color | ( (red >> 3) << 11 ) ;\
	color = color | ( (green >> 2) << 5 ) ;\
	color = color | (blue >> 3) ;\


# define RENDERRGB565COLOR_EX(color,red,green,blue)\
	color = color | ( red << 11 ) ;\
	color = color | ( green << 5 ) ;\
	color = color | (blue) ;\

void glRenderDrawMatrix ( void* buffer , GLint TotallMatrixs ) ;
void glRenderDrawPoint ( GLint x , GLint y ) ;
void glRenderDrawLine ( GLint x0 , GLint y0 , GLint x1 , GLint y1 ) ;

