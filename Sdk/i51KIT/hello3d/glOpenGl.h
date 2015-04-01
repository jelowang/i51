
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

#ifndef OPENGL__
#define OPENGL__

#define glDEBUG

#include "GL.h"

#define glPI 3.1415927
// for 10000 matrices
#define glVIEWMATRIXSIZE 10000

enum {

	//	author : Jelo Wang
	//	since : 2013.1.10
	//	(C) TOK
	
	glCOLOR_RGB565 = 0 , 
	glCOLOR_RGB_F3 = 1 , 		

} glCOLOR ;

typedef struct {

	//	author : Jelo Wang
	//	since : 2013.1.9
	//	(C) TOK
	
	struct {
	
		GLfloat x ;
		GLfloat y ;
		GLfloat z ;
		
	} Orignal ;

	struct {
		
		GLfloat x ;
		GLfloat y ;
		GLfloat z ;
		
	} Current ;

} glVIEWMATRIXATOM ;

typedef struct {

	//	author : Jelo Wang
	//	since : 2013.1.9
	//	(C) TOK
	
	GLboolean glBegin ;
	GLboolean glEnd ;

	GLint glRgbFormat ;
	
	struct {

		void* buffer ;
		GLint walker ;
		GLint buffersize ;

		//	单位观察立方体坐标到屏幕坐标的变换矩阵
		GLfloat matrix[3][3] ;
		
	} glViewMatrix ;

	struct {

		//	perspective matrix

		GLdouble fovy ;
		GLdouble aspect ;
		GLdouble zNear ;
		GLdouble zFar ;

		struct {
			GLint top ;
			GLint bottom ;
			GLint left ;
			GLint right ;
		} ortho2d ;

		//	透视投影矩阵
		GLfloat matrix[4][4] ;

	} glPerspMatrix ;
	
	//	Current Primitive in the pipleline
	GLint glPrimitive ;

	struct {
		
		GLint x ;
		GLint y ;		
		GLint width ;
		GLint height ;

		struct {
			
			GLfloat red ;
			GLfloat green ;
			GLfloat blue ;		
			GLfloat alpha ;
			
		} background ;
		
		GLushort* panel ;

	} glViewPort ;

	struct {

		//	color format of opengl pipeline
		GLbyte pipeformat ;
		//	color format of panel
		GLbyte panelformat ;
			
		union {

			struct {

				GLfloat red ;
				GLfloat green ;
				GLfloat blue ;
				
			} f3 ;
			
		} format ;
		
	} glColor ;
	
} OpenGL ;

extern OpenGL opengl  ;

#endif

