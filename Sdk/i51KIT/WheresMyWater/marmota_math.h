#ifndef __MARMOTA_MATH_H__
#define __MARMOTA_MATH_H__

#ifdef __cplusplus
extern "C" {
#endif
//#include "math.h"
//#include "stdio.h"
#ifndef INFINITY
	#ifdef _MSC_VER
		union MSVC_EVIL_FLOAT_HACK
		{
			unsigned __int8 Bytes[4];
			float Value;
		};
		static union MSVC_EVIL_FLOAT_HACK INFINITY_HACK = {{0x00, 0x00, 0x80, 0x7F}};
		#define INFINITY (INFINITY_HACK.Value)
	#endif
	
	#ifdef __GNUC__
		#define INFINITY (__builtin_inf())
	#endif
	
	#ifndef INFINITY
		#define INFINITY (1e1000)
	#endif
#endif

#ifndef inline
#define inline __inline
#endif

typedef int mmReal;
#define MM_FIXED_FRACBITS		14

#define MM_FIXED_SQRT_ERR 		8 /*(MM_FIXED_FRACBITS - 10) << 1*/
#define MM_FIXED_RESOLUTION		(1 << MM_FIXED_FRACBITS)
#define MM_FIXED_INT_MASK		(0xffffffff << MM_FIXED_FRACBITS)

#define __integer_to_fixpoint__(i, fracbits)			((mmReal)((i) << (fracbits)))
#define __float_to_fixpoint__(f, fracbits)				((mmReal)((f) * (1 << fracbits)))
#define __fixpoint_to_integer__(r, fracbits)			((int)((r) >> (fracbits)))
#define __fixpoint_to_float__(r, fracbits)				((float)((r) / (float)(1 << fracbits)))

#define mmIntegerToReal(i)								__integer_to_fixpoint__(i, MM_FIXED_FRACBITS)
#define mmFloatToReal(f)								__float_to_fixpoint__(f, MM_FIXED_FRACBITS)
#define mmRealToInteger(r)								__fixpoint_to_integer__(r, MM_FIXED_FRACBITS)
#define mmRealToFloat(r)								__fixpoint_to_float__(r, MM_FIXED_FRACBITS)

#define __MM_PI__										3.1415926535897932384626433832795
#define __MM_2PI__										6.283185307179586476925286766559
#define __MM_PIO2__    									1.5707963267948966192313216916398
#define __MM_PIO4__										0.78539816339744830961566084581988

#define MM_PI											mmFloatToReal(__MM_PI__  )
#define MM_2PI											mmFloatToReal(__MM_2PI__ )
#define MM_PIO2											mmFloatToReal(__MM_PIO2__)
#define MM_PIO4											mmFloatToReal(__MM_PIO4__)

#define mmConvertFracbits(num, q1, q2)					(((q2)>(q1)) ? (num)<<((q2) - (q1)) : (num)>>((q1) - (q2)))

#define __mm_multiply__(x, y, xFb, yFb, resFb)			((mmReal)(((__int64)(x) * (__int64)(y)) >> ((xFb) + (yFb) - (resFb))))
#define __mm_divide__(x, y, xFb, yFb, resFb) 			((mmReal)(((__int64)(x) << ((xFb) + (yFb) - (resFb))) / (__int64)(y)))

#define mmRealAdd(a, b)									((a)+(b))
#define mmRealSub(a, b)									((a)-(b))
#define mmRealMultiply(x, y)							__mm_multiply__((x), (y),  MM_FIXED_FRACBITS,  MM_FIXED_FRACBITS,  MM_FIXED_FRACBITS)

#define mmRealDivide(x, y)								__mm_divide__((x), (y), MM_FIXED_FRACBITS, MM_FIXED_FRACBITS, MM_FIXED_FRACBITS)


#define mmRealMod(x, y)									((x) % (y))
#define mmRealAbs(x)									(((x)<0)?(-x):(x))

extern mmReal __fsin__(mmReal realRadian);
extern float mmAsin(float arg);
extern float mmSqrtf(float x);
extern float mmAsin(float arg);
//extern mmReal mmRealSqrt(mmReal num);
extern float _atan_(float arg);
extern float mmExp(float x);
extern mmReal __mmFloor__(mmReal x);
///*inline*/ mmReal __mmFloor__(mmReal x)
//{
//	// masks off fraction bits
//	return x & MM_FIXED_INT_MASK;
//}
//
extern mmReal  __mmCeil__(mmReal x);
///*inline*/ mmReal  __mmCeil__(mmReal x)
//{
//	// masks off fraction bits and adds one if there were some fractional bits
//	mmReal f = x & MM_FIXED_INT_MASK;
//	return f != x ? (f + MM_FIXED_RESOLUTION) : x;
//}
//
extern float mmSin(float theta);
///*inline*/ float mmSin(float theta)
//{
//	return mmRealToFloat(__fsin__(mmFloatToReal(theta)));
//}
//
extern float mmCos(float theta);
///*inline*/ float mmCos(float theta)
//{
//	return mmRealToFloat(__fsin__(mmFloatToReal(theta) + MM_PIO2));
//}
//
//extern float  mmTan(float  theta);
///*inline*/ float  mmTan(float  theta)
//{
//	return mmRealToFloat(mmRealDivide(__fsin__(mmFloatToReal(theta)),  __fsin__(mmFloatToReal(theta) + MM_PIO2)));
//}
//
extern float mmAtan(float arg);
///*inline*/ float mmAtan(float arg)
//{
//   return arg > 0 ? _atan_(arg) : -_atan_(-arg);
//}
//
extern float mmAtan2(float arg1, float arg2);
//// implementation of atan2
///*inline*/ float mmAtan2(float arg1, float arg2)
//{
//	if(arg1 + arg2 == arg1)
//	{
//		return arg1 >= 0 ? __MM_PIO2__ : -__MM_PIO2__;
//	}
//
//	arg1 = mmAtan(arg1 / arg2);
//	if(arg2 < 0)
//	{
//		return arg1 < 0 ? arg1 + __MM_PI__ : arg1 - __MM_PI__;
//	}
//
//	return arg1;
//}
//
extern float mmAcos(float arg);
///*inline*/ float mmAcos(float arg)
//{
//   if(arg > 1 || arg < -1)
//	   return INFINITY;
//   return __MM_PIO2__ - mmAsin(arg);
//}
//#define mmArccos mmAcos
extern float mmMod(float a, float b);
//
///*inline*/ float mmMod(float a, float b)
//{
//	return mmRealToFloat(mmRealMod(mmFloatToReal(a), mmFloatToReal(b)));
//}
//
extern float mmPow(float x ,float y);
///*inline*/ float mmPow(float x ,float y)
//{
//	return 1.0;
//}
//
extern float mmFloor(float x);
///*inline*/ float mmFloor(float x)
//{
//	return mmRealToFloat(__mmFloor__(mmFloatToReal(x)));
//}
extern float mmCeil(float x);
//
///*inline*/ float mmCeil(float x)
//{
//	return mmRealToFloat(__mmCeil__(mmFloatToReal(x)));
//}

#ifdef __cplusplus
}
#endif
#endif