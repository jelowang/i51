
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
#include "glOpenGl.h"

void glutInit(int *argcp, char **argv) {


}

void glutInitDisplayMode(unsigned int mode);
void glutInitWindowPosition(int x, int y);
void glutInitWindowSize(int width, int height);
void glutMainLoop(void);
void glutDisplayFunc(void (*func)(void));
void glutReshapeFunc(void (*func)(int width, int height));
void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
void glutMouseFunc(void (*func)(int button, int state, int x, int y));
void glutMotionFunc(void (*func)(int x, int y));

void glutSetPanel ( unsigned char*  panel ) {

	//	author : Jelo Wang
	//	since : 2013.1.18
	//	(C) TOK

	//	这是非标准接口，用来设置显存

	opengl.glViewPort.panel = panel ;
	
}

void gluOrtho2D ( GLdouble left , GLdouble right , GLdouble bottom , GLdouble top ) {

	//	author : Jelo Wang
	//	since : 2013.1.14
	//	(C) TOK

	opengl.glPerspMatrix.ortho2d.left = left ;
	opengl.glPerspMatrix.ortho2d.right = right ;
	opengl.glPerspMatrix.ortho2d.top = top ;
	opengl.glPerspMatrix.ortho2d.bottom = bottom ;	
		

}

void gluPerspective ( GLdouble fovy , GLdouble aspect ,  GLdouble zNear , GLdouble zFar ) {

	//	author : Jelo Wang
	//	since : 2013.1.10
	//	(C) TOK

	//	配置投影矩阵
	
	GLdouble N = abs(zNear) ;
	GLdouble F = abs(zFar) ;

	GLint left = (GLint ) opengl.glPerspMatrix.ortho2d.left ;
	GLint right = (GLint ) opengl.glPerspMatrix.ortho2d.right ;
	GLint top = (GLint ) opengl.glPerspMatrix.ortho2d.top ;
	GLint bottom = (GLint ) opengl.glPerspMatrix.ortho2d.bottom ;

	opengl.glPerspMatrix.fovy = fovy ;
	opengl.glPerspMatrix.aspect = aspect ;
	opengl.glPerspMatrix.zNear = zNear ;
	opengl.glPerspMatrix.zFar = zFar ;
	
	opengl.glPerspMatrix.matrix[0][0] = 2.0f*N/(right-left) ;
	opengl.glPerspMatrix.matrix[1][0] = 0 ;
	opengl.glPerspMatrix.matrix[2][0] = 0 ;
	opengl.glPerspMatrix.matrix[3][0] = 0 ;

	opengl.glPerspMatrix.matrix[0][1] = 0 ;
	opengl.glPerspMatrix.matrix[1][1] = 2.0f*N/(top-bottom) ;
	opengl.glPerspMatrix.matrix[2][1] = 0 ;
	opengl.glPerspMatrix.matrix[3][1] = 0 ;

	opengl.glPerspMatrix.matrix[0][2] = (right+left) / (right-left) ;
	opengl.glPerspMatrix.matrix[1][2] = (top+bottom) / (top-bottom) ;
	opengl.glPerspMatrix.matrix[2][2] = (F+N)/(F-N) ;
	opengl.glPerspMatrix.matrix[3][2] = -1 ;

	opengl.glPerspMatrix.matrix[0][3] = 0 ;
	opengl.glPerspMatrix.matrix[1][3] = 0 ;
	opengl.glPerspMatrix.matrix[2][3] = (2*F*N)/(F-N) ;
	opengl.glPerspMatrix.matrix[3][3] = 0 ;

	#ifdef glDEBUG
		printf("left %1.3f\n",((-1*opengl.glPerspMatrix.matrix[2][2]+opengl.glPerspMatrix.matrix[2][3]) /-1)) ;
		printf("right %1.3f\n",((-100*opengl.glPerspMatrix.matrix[2][2]+opengl.glPerspMatrix.matrix[2][3]) /-100)) ;
		printf("middle %1.3f\n",((-3*opengl.glPerspMatrix.matrix[2][2]+opengl.glPerspMatrix.matrix[2][3]) /-3)) ;
	#endif

	//	单位观察立方体坐标区间[-1,1]，长度2
	//	我们根据此配置投影平面坐标到屏幕坐标的变换矩阵
	opengl.glViewMatrix.matrix[0][0] = (GLfloat )opengl.glViewPort.width / 2 ;
	opengl.glViewMatrix.matrix[0][2] = (GLfloat )opengl.glViewPort.width  / 2 ;

	opengl.glViewMatrix.matrix[1][1] = (GLfloat )opengl.glViewPort.height / 2 ;
	opengl.glViewMatrix.matrix[1][2] = (GLfloat )opengl.glViewPort.height / 2 ;
	
	
}

void gluLookAt (
	
    GLdouble eyex, 
    GLdouble eyey, 
    GLdouble eyez, 
    GLdouble centerx, 
    GLdouble centery, 
    GLdouble centerz, 
    GLdouble upx, 
    GLdouble upy, 
    GLdouble upz);


