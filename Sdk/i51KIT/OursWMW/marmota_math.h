#ifndef __MARMOTA_MATH_H__
#define __MARMOTA_MATH_H__

#include "chipmunk_types.h"

typedef union MSVC_EVIL_FLOAT_HACK
{
	unsigned char Bytes[4];
	float Value;
}MSVC_EVIL_FLOAT_HACK;
// 小端无穷大数据定义
extern union MSVC_EVIL_FLOAT_HACK INFINITY_HACK ;//= {{0x00, 0x00, 0x80, 0x7F}};
// 大端无穷大数据定义
//		static union MSVC_EVIL_FLOAT_HACK INFINITY_HACK = {{0x7F,0x80,0x00, 0x00}};
#define INFINITY (INFINITY_HACK.Value)

#define CP_FIXED_FRACBITS		14

#define CP_FIXED_SQRT_ERR 		8 /*(CP_FIXED_FRACBITS - 10) << 1*/
#define CP_FIXED_RESOLUTION		(1 << CP_FIXED_FRACBITS)
//#define CP_FIXED_INT_MASK		(0xffffffff << CP_FIXED_FRACBITS)
#define CP_FIXED_INT_MASK		(0xffffC000)

#define __integer_to_fixpoint__(i, fracbits)			((cpReal)((i) << (fracbits)))
#define __float_to_fixpoint__(f, fracbits)				((cpReal)((f) * (1 << fracbits)))
#define __fixpoint_to_integer__(r, fracbits)			((int)((r) >> (fracbits)))
#define __fixpoint_to_float__(r, fracbits)				((float)((r) / (float)(1 << fracbits)))

#define cpIntegerToReal(i)								__integer_to_fixpoint__(i, CP_FIXED_FRACBITS)
#define cpFloatToReal(f)								__float_to_fixpoint__(f, CP_FIXED_FRACBITS)
#define cpRealToInteger(r)								__fixpoint_to_integer__(r, CP_FIXED_FRACBITS)
#define cpRealToFloat(r)								__fixpoint_to_float__(r, CP_FIXED_FRACBITS)

#define __CP_PI__										3.1415926535897932384626433832795
#define __CP_2PI__										6.283185307179586476925286766559
#define __CP_PIO2__    									1.5707963267948966192313216916398
#define __CP_PIO4__										0.78539816339744830961566084581988

#define CP_PI											cpFloatToReal(__CP_PI__  )
#define CP_2PI											cpFloatToReal(__CP_2PI__ )
#define CP_PIO2											cpFloatToReal(__CP_PIO2__)
#define CP_PIO4											cpFloatToReal(__CP_PIO4__)

#define cpConvertFracbits(num, q1, q2)					(((q2)>(q1)) ? (num)<<((q2) - (q1)) : (num)>>((q1) - (q2)))

#define __cp_multiply__(x, y, xFb, yFb, resFb)			((cpReal)(((__int64)(x) * (__int64)(y)) >> ((xFb) + (yFb) - (resFb))))
#define __cp_divide__(x, y, xFb, yFb, resFb) 			((cpReal)(((__int64)(x) << ((xFb) + (yFb) - (resFb))) / (__int64)(y)))

#define cpRealAdd(a, b)									((a)+(b))
#define cpRealSub(a, b)									((a)-(b))
#define cpRealMultiply(x, y)							__cp_multiply__((x), (y),  CP_FIXED_FRACBITS,  CP_FIXED_FRACBITS,  CP_FIXED_FRACBITS)

#define cpRealDivide(x, y)								__cp_divide__((x), (y), CP_FIXED_FRACBITS, CP_FIXED_FRACBITS, CP_FIXED_FRACBITS)


#define cpRealMod(x, y)									((x) % (y))
#define cpRealAbs(x)									(((x)<0)?(-x):(x))

extern cpReal cpRealSqrt(cpReal p_Square);
extern float cpSqrtf(float x);
extern cpReal  __qsin__(cpReal  radian);
extern cpReal __fsin__(cpReal realRadian);
extern cpReal  cpQSin(cpReal  theta);
extern float __atan__(float a);
extern float _atan_(float arg);
extern float cpAsin(float arg);
extern float cpExp(float x);
extern cpReal __cpFloor__(cpReal x);
extern cpReal  __cpCeil__(cpReal x);
extern float cpSin(float theta);
extern float cpCos(float theta);
extern float  cpTan(float  theta);
extern float cpAtan(float arg);
extern float cpAtan2(float arg1, float arg2);
extern float cpAcos(float arg);
extern float cpMod(float a, float b);
extern float cpPow(float x ,float y);
extern float cpFloor(float x);
extern float cpCeil(float x);
extern cpFloat cpfmax(cpFloat a, cpFloat b);
extern cpFloat cpfmin(cpFloat a, cpFloat b);
extern cpFloat cpfabs(cpFloat n);
extern cpFloat cpfclamp(cpFloat f, cpFloat min, cpFloat max);
extern cpFloat cpflerp(cpFloat f1, cpFloat f2, cpFloat t);
extern cpFloat cpflerpconst(cpFloat f1, cpFloat f2, cpFloat d);

#endif
