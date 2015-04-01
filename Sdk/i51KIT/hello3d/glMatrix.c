
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

#include "memory.h"
#include "GL.h"
#include "schal.h"
#include "sccl.h"
#include "glMatrix.h"
#include "glOpenGl.h"

void glMatrixClear () {

	//	author : Jelo Wang
	//	since : 2013.2.1
	//	(C) TOK

	//	非OpenGL标准模块

	//	销毁所有顶点

	SCFree ( opengl.glViewMatrix.buffer ) ;
	opengl.glViewMatrix.buffer = 0 ;
	opengl.glViewMatrix.walker = 0 ;
	opengl.glViewMatrix.buffersize = 0 ;
	
}

void glMatrixRotate ( GLfloat angle , GLfloat x , GLfloat y , GLfloat z ) {

	//	author : Jelo Wang
	//	since : 2013.2.1
	//	(C) TOK

	//	绕中心旋转

//	float radian = (degree * glPI) / 180 ;

	GLint walker = 0 ;
	GLint totall = 0 ;

	GLint centerx = 0 ;
	GLint centery = 0 ;
	GLint centerz = 0 ;	

	glVIEWMATRIXATOM atom = { 0 } ;
/*
	if ( GL_LINES == opengl.glPrimitive )  {

		memcpy ( &v1 , (void*)((int) opengl.glViewMatrix.buffer+walker ) , sizeof(glVIEWMATRIXATOM) ) ;
		walker = walker + sizeof(glVIEWMATRIXATOM) ;
		memcpy ( &v2 , (void*)((int) opengl.glViewMatrix.buffer+walker ) , sizeof(glVIEWMATRIXATOM) ) ;
				
	}

	for ( walker = 0 ; walker < opengl.glViewMatrix.walker ; walker += sizeof(glVIEWMATRIXATOM) ) {

		memcpy ( &atom , (void*)((int) opengl.glViewMatrix.buffer+walker ) , sizeof(glVIEWMATRIXATOM) ) ;

		if ( 0 != x ) {
			
			atom.Current.x = atom.x ;
			atom.Current.y = atom.y+((orignal.y-125.0f)*cos(radian))+((orignal.z))*sin(radian) ;
			atom.Current.z = atom.z+(orignal.z)*cos(radian)-(orignal.y-125.0f)*sin(radian) ;

		}
	
	}
*/

	
}

void glMatrixAdd ( GLint model ,  GLfloat x , GLfloat y , GLfloat z ) {

	//	author : Jelo Wang
	//	since : 2013.1.9
	//	(C) TOK

	//	非OpenGL标准模块

	//	将顶点存入矩阵，用一个一维数组实现就可以了，主要是够简单。	
	
	glVIEWMATRIXATOM atom = { 0 } ;

	if ( 0 == opengl.glViewMatrix.buffer ) {		

		opengl.glViewMatrix.buffer = (void* ) SCMalloc ( glVIEWMATRIXSIZE * sizeof(glVIEWMATRIXATOM) ) ;
		ASSERT(opengl.glViewMatrix.buffer) ; 
		opengl.glViewMatrix.walker = 0 ;
		opengl.glViewMatrix.buffersize = glVIEWMATRIXSIZE * sizeof(glVIEWMATRIXATOM) ;
		
	} else if ( opengl.glViewMatrix.walker >= opengl.glViewMatrix.buffersize ) {
		
		SCRealloc ( opengl.glViewMatrix.buffer , opengl.glViewMatrix.buffersize + glVIEWMATRIXSIZE ) ;
		opengl.glViewMatrix.buffersize = opengl.glViewMatrix.buffersize + glVIEWMATRIXSIZE ;

	}
	
	atom.Orignal.x= x ;
	atom.Orignal.y = y ;
	atom.Orignal.z = z ;

	atom.Current.x = x ;
	atom.Current.y = y ;
	atom.Current.z = z ;	

	memcpy ( (void*)((int) opengl.glViewMatrix.buffer+opengl.glViewMatrix.walker) , &atom , sizeof(glVIEWMATRIXATOM) ) ;
	opengl.glViewMatrix.walker = opengl.glViewMatrix.walker + sizeof(glVIEWMATRIXATOM) ;

}


void glMatrixPerspectiveTransform () {

	//	author : Jelo Wang
	//	since : 2013.1.9
	//	(C) TOK

	//	透视投影变换，透视投影矩阵我们已经在gluPerspective 里面配置好了
	//	这里的变换过程就很简单了，只需要将顶点向量左乘投影矩阵即可

	GLint walker = 0 ;
	GLint totall = 0 ;

	glVIEWMATRIXATOM atom = { 0 } ;

	for ( walker = 0 ; walker < opengl.glViewMatrix.walker ; walker += sizeof(glVIEWMATRIXATOM) ) {

		memcpy ( &atom , (void*)((int) opengl.glViewMatrix.buffer+walker ) , sizeof(glVIEWMATRIXATOM) ) ;

		//	x*(2N/(right-left)) + (right+left/(right-left))
		atom.Current.x = atom.Current.x*opengl.glPerspMatrix.matrix[0][0] + (atom.Current.z*opengl.glPerspMatrix.matrix[0][2]) ;
		//	y*(2N/(top-bottom)) + (top+bottom/(top-bottom))
		atom.Current.y = atom.Current.y*opengl.glPerspMatrix.matrix[1][1] + (atom.Current.z*opengl.glPerspMatrix.matrix[1][2]) ;

		if ( 0 != atom.Current.z ) {
			
			//	投影矩阵是一个齐次矩阵，需要将齐次矩阵变为一般矩阵，最终除公因子-Z。
			atom.Current.x = atom.Current.x / -atom.Current.z ;
			atom.Current.y = atom.Current.y / -atom.Current.z ;

			//	z*a+b / (z)
			atom.Current.z = (atom.Current.z*opengl.glPerspMatrix.matrix[2][2] + opengl.glPerspMatrix.matrix[2][3] ) / atom.Current.z ;
			
		}

		#ifdef glDEBUG
			printf("x %1.3f\n",atom.Current.x*opengl.glViewMatrix.matrix[0][0]+opengl.glViewMatrix.matrix[0][2]) ;
			printf("y %1.3f\n",atom.Current.y*opengl.glViewMatrix.matrix[1][1]+opengl.glViewMatrix.matrix[0][2]) ;
		#endif
		
		//	最终变换到屏幕坐标
		atom.Current.x = atom.Current.x*opengl.glViewMatrix.matrix[0][0]+opengl.glViewMatrix.matrix[0][2] ;
		atom.Current.y = atom.Current.y*opengl.glViewMatrix.matrix[1][1]+opengl.glViewMatrix.matrix[1][2] ;

		memcpy ( (void*)((int) opengl.glViewMatrix.buffer+walker ) , &atom , sizeof(glVIEWMATRIXATOM) ) ;
					
		
	}
	
}

