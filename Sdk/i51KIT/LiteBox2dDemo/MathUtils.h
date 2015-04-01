/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies.
* Erin Catto makes no representations about the suitability 
* of this software for any purpose.  
* It is provided "as is" without express or implied warranty.
*/

#ifndef MATHUTILS_H
#define MATHUTILS_H

// #include "MexLib.h"
// #include "MexLib_Std.h"
// #include "MexLib_Os.h"
#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitStd.h"
//#include "math.h"
//extern funcptr *MEX_C_LIB_FUNC_NAME_STD;
//extern funcptr *MEX_C_LIB_FUNC_NAME_OS;

//#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if 1

extern float sqrtf(float f);
extern float fabsf(float f);
extern float cosf(float f);
extern float sinf(float f);

#define RAND_MAX 0x7fff
#define FLT_MAX         3.402823466e+38F        /* max value */

#endif /* MEX_PLATFORM_MODIS MEX_PLATFORM_MTK */

//extern const float k_pi;
#define k_pi 3.14159265358979323846264f
#define triangle_accuracy  0.00001f

class  Vec2
{

public:
	Vec2();// {}
	Vec2(float x, float y) ;//: x(x), y(y) {}

	void Set(float x_, float y_);// { x = x_; y = y_; }

	Vec2 operator -();// { return Vec2(-x, -y); }
	
	void operator += (const Vec2& v);//
 	//{
 	//	x += v.x; y += v.y;
 	//}
	
	void operator -= (const Vec2& v);
	//{
	//	x -= v.x; y -= v.y;
	//}

	void operator *= (float a);
	//{
	//	x *= a; y *= a;
	//}

	float Length() const;
	//{
	//	return sqrtf(x * x + y * y);
	//}

	float x, y;
};

class Mat22
{
public:
	Mat22();// {}
//	Mat22(float angle);
	void Mat22_1(float angle);
	Mat22(const Vec2& col1, const Vec2& col2) ;

	Mat22 Transpose() const;

	Mat22 Invert() const;
	Vec2 col1, col2;
};

 extern float Dot(const Vec2& a, const Vec2& b);
extern  float Cross(const Vec2& a, const Vec2& b);
extern  Vec2 Cross(const Vec2& a, float s);
extern  Vec2 Cross(float s, const Vec2& a);
extern  Vec2 operator * (const Mat22& A, const Vec2& v);
extern  Vec2 operator + (const Vec2& a, const Vec2& b);
extern  Vec2 operator - (const Vec2& a, const Vec2& b);
 extern Vec2 operator * (float s, const Vec2& v);
 extern Mat22 operator + (const Mat22& A, const Mat22& B);
extern  Mat22 operator * (const Mat22& A, const Mat22& B);
extern  float Abs(float a);
extern  Vec2 Abs(const Vec2& a);
extern  Mat22 Abs(const Mat22& A);
extern  float Sign(float x);
extern  float Min(float a, float b);
extern float Max(float a, float b);
extern  float Clamp(float a, float low, float high);
template<typename T> extern  void Swap(T& a, T& b);
extern float Random();
extern float Random(float lo, float hi);

 #endif


