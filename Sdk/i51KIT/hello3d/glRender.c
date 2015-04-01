
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
#include "glMatrix.h"
#include "glOpenGl.h"
#include "glRender.h"

static GLint glRenderColorTrans ( GLint ns , GLint fs ) {

	//	author : Jelo Wang
	//	since : 2013.1.10
	//	(C) TOK

	//	将fs 格式转换为ns格式，颜色值从opengl.glColor中取

	//	以浮点格式为例，三个分量取值区间[0,1]，RGB565 取值空间有R[0,32] G[64] , B[32]
	//	根据下式转换'p = p*d

	GLint color = 0 ;
	
	if ( glCOLOR_RGB565 == ns ) {

		GLbyte r5 = 31 ;
		GLbyte g6 = 63 ;
		GLbyte b5 = 31 ;

		GLbyte nr5 = 0 ;
		GLbyte ng6 = 0 ;
		GLbyte nb5 = 0 ;		
		
		if ( glCOLOR_RGB_F3 == fs ) {

			nr5 = opengl.glColor.format.f3.red * r5 ;
			ng6 = opengl.glColor.format.f3.green * g6 ;
			nb5 = opengl.glColor.format.f3.blue * b5 ;

			RENDERRGB565COLOR_EX(color,nr5,ng6,nb5) ;
			
		}
		
	}

	return color ;
	
}

void glRenderDrawPoint ( GLint x , GLint y ) {

	//	author : Jelo Wang
	//	since : 2013.1.10
	//	(C) TOK

	GLint color = 0 ;

	color = glRenderColorTrans ( opengl.glColor.panelformat , opengl.glColor.pipeformat ) ;
	
	if ( glCOLOR_RGB565 == opengl.glColor.panelformat ) {

		GLshort* buffer = (GLshort* ) opengl.glViewPort.panel ;
		buffer [y*opengl.glViewPort.width+x] = color ;
		
	}
}

void glRenderDrawLine ( GLint x0 , GLint y0 , GLint x1 , GLint y1 ) {
	
	//	author : Jelo Wang
	//	since : 2013.1.14
	//	(C)TOK
	
	int panel_width = opengl.glViewPort.width ;
	int panel_height = opengl.glViewPort.height ;
	short* panel16 = (GLshort* ) opengl.glViewPort.panel ;
	float dy,dx,x,y,m = 0 ;
	GLint color = 0 ;

	color = glRenderColorTrans ( opengl.glColor.panelformat , opengl.glColor.pipeformat ) ;
	
	if ( 0 > x0 || 0 > y0 ) return ;
	else if ( x1 >= panel_width || y1 >= panel_height ) return ;

	dx = x1-x0 ;
	dy = y1-y0 ;
	m = dy/dx ;

	if ( glCOLOR_RGB565 == opengl.glColor.panelformat ) {

		if ( x0 < x1 ) {
			
			if ( m <= 1 && m >= -1 ) {
				
				y = y0 ;
				
				for ( x = x0 ; x <= x1 ; x++ ) {
					panel16 [ ((int)(y+0.5))*panel_width + ((int)x) ] = color ;
					y +=m ;
				}
			}
			
		}


		if ( x0 > x1 ) {
			
			if ( m <=1 && m >= -1 ) {
				
				y = y0 ;
				
				for ( x = x0 ; x >= x1 ; x-- ) {
					panel16 [ ((int)(y+0.5))*panel_width + ((int)x) ] = color ;
					y -= m ;
				}
				
			}
			
		}
		
		if ( y0 < y1 ) {
			
			if ( m >= 1 || m <= -1 ) {
				
				m = 1 / m ;
				x = x0 ;
				
				for ( y = y0 ; y <= y1 ; y++ ) {
					panel16 [ ((int)(y+0.5))*panel_width + ((int)x) ] = color ;
					x+=m;
				}
				
			}
			
		}
		
		if ( y0 > y1 ) {
			
			if ( m <= -1 || m >= 1 ) {

				m = 1 / m ;
				x = x0 ;
				
				for ( y = y0 ; y >= y1 ; y-- ) {
					panel16 [ ((int)(y+0.5))*panel_width + ((int)x) ] = color ;
					x-=m;
				}
				
			}
			
		}

	}


}


void glRenderMatrixDraw ( void* buffer , GLint TotallMatrixs ) {

	GLint walker = 0 ;
	GLint totall = 0 ;

	glVIEWMATRIXATOM v1 = { 0 } ;
	glVIEWMATRIXATOM v2 = { 0 } ;
		
	glMatrixPerspectiveTransform () ;

	totall = opengl.glViewMatrix.walker ;	
	
	for ( walker = 0 ; walker < totall ; ) {

		switch ( opengl.glPrimitive ) {
			
			case GL_POINTS :
			break ;
			
			case GL_LINES :
			{
				memcpy ( &v1 , (void*)((int) opengl.glViewMatrix.buffer+walker ) , sizeof(glVIEWMATRIXATOM) ) ;
				walker = walker + sizeof(glVIEWMATRIXATOM) ;
				memcpy ( &v2 , (void*)((int) opengl.glViewMatrix.buffer+walker ) , sizeof(glVIEWMATRIXATOM) ) ;
				glRenderDrawLine ( v1.Current.x , v1.Current.y , v2.Current.x , v2.Current.y ) ;
				walker = walker + sizeof(glVIEWMATRIXATOM) ;
			}
			break ;
			
			case GL_POLYGON :
			break ;
			
			case GL_LINE_STRIP :			
			break ;
			
			case GL_LINE_LOOP :
			break ;
			
			case GL_QUAD_STRIP :
			break ;

			default :
				walker = walker + sizeof(glVIEWMATRIXATOM) ;

		}
		
		
	}



}

