#include "marmota_math.h"

mmReal mmRealSqrt(mmReal p_Square)
{
	int i = 0;
	mmReal res, delta, maxError;

	if (p_Square <= 0) return 0;

	/* start at half */
	res = (p_Square >> 1);

	/* determine allowable error */
	maxError =  mmRealMultiply(p_Square, MM_FIXED_SQRT_ERR);
	
	do
	{
		delta =  (mmRealMultiply( res, res ) - p_Square);
		res -=  mmRealDivide(delta, ( res << 1 ));
	}while ( delta > maxError || delta < -maxError );
	
	return res;
}

float mmSqrtf(float x){
	union{
		int intPart;
		float floatPart;
	} convertor;
	
	union{
		int intPart;
		float floatPart;
	} convertor2;
	
	convertor.floatPart = x;
	convertor2.floatPart = x;
	convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
	convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
	return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
}
/*
mmReal mmFloor(mmReal x)
{
	// masks off fraction bits
	return x & MM_FIXED_INT_MASK;
}

mmReal  mmCeil(mmReal x)
{
	// masks off fraction bits and adds one if there were some fractional bits
	mmReal  f = x & MM_FIXED_INT_MASK;
	if (f != x) return f + MM_FIXED_RESOLUTION;
	return x;
}*/

/**
 * __qsin__, internal sine function
 * Taylor's Formula
 * http://en.wikipedia.org/wiki/Taylor_series
 * sin(x)=x-x^3/3!+x^5/5!-....(-1)^(m-1)*x^(2m-1)/(2m-1)! 
 */
mmReal  __qsin__(mmReal  radian)
{
	mmReal  numerator = radian, res = 0;
	int denominate = 1, signbit = 1, i;

	for (i = 1; i < 7; i += 2)
	{
		res += signbit * (numerator / denominate);
		numerator = mmRealMultiply(radian, mmRealMultiply(radian, numerator));
		denominate *= (i + 1)*(i + 2);
		signbit = (signbit == -1) ? 1 : -1;
	}

	return res;
}
/**
 * __fsin__: fast sin arithmateric
 *
**/
mmReal __fsin__(mmReal realRadian)
{
	/**Q14*/
	const mmReal __SIN_PARAM_1__ = 0x517c;	/// mmFloatToReal(1.27323954)
	const mmReal __SIN_PARAM_2__ = 0xe66;	/// mmFloatToReal(0.225)
	const mmReal __SIN_PARAM_3__ = 0x19f0;	/// mmFloatToReal(0.405284735)

	mmReal r1, r2;
	
	realRadian = mmRealMod(realRadian, MM_2PI);
	if (realRadian < -MM_PI)
		realRadian += MM_2PI;
	else if (realRadian > MM_PI)
		realRadian -= MM_2PI;

	r1 = mmRealMultiply(__SIN_PARAM_3__, realRadian);
	r1 = realRadian < 0 ? __SIN_PARAM_1__ + r1 : __SIN_PARAM_1__ - r1;
	r1 = mmRealMultiply(r1, realRadian);
	r2 = r1 < 0 ? mmRealMultiply(r1, -r1) : mmRealMultiply(r1, r1);
	r2 = mmRealMultiply(__SIN_PARAM_2__, (r2 - r1));
	r2 += r1;

	return r2;
}

/**
 * sine
 */
mmReal  mmQSin(mmReal  theta)
{
	mmReal  res, realRadian;
	int signBit = 1;

	if (realRadian = mmRealMod(theta, MM_2PI), realRadian < 0 )
	{
		realRadian += MM_2PI;
	}

	if (realRadian > MM_PI)
	{
		signBit = -1;
		realRadian -= MM_PI;
	}

	if (realRadian <= MM_PIO2 )
		res = __qsin__(realRadian);
	else
		res = __qsin__(MM_PI - realRadian);

	return signBit == -1 ? -res : res;
}

/*
value = ((((p4*argsq + p3)*argsq + p2)*argsq + p1)*argsq + p0);
value = value/(((((argsq + q4)*argsq + q3)*argsq + q2)*argsq + q1)*argsq + q0);
*/
float __atan__(float a)
{

	mmReal arg = mmFloatToReal(a);
	mmReal argsq, fraction, denominator, res;
	
	const mmReal p4 = mmFloatToReal(0.161536412982230228262e2);
	const mmReal p3 = mmFloatToReal(0.26842548195503973794141e3);
	const mmReal p2 = mmFloatToReal(0.11530293515404850115428136e4);
	const mmReal p1 = mmFloatToReal(0.178040631643319697105464587e4);
	const mmReal p0 = mmFloatToReal(0.89678597403663861959987488e3);

	const mmReal q4 = mmFloatToReal(0.5895697050844462222791e2);
	const mmReal q3 = mmFloatToReal(0.536265374031215315104235e3);
	const mmReal q2 = mmFloatToReal(0.16667838148816337184521798e4);
	const mmReal q1 = mmFloatToReal(0.207933497444540981287275926e4);
	const mmReal q0 = mmFloatToReal(0.89678597403663861962481162e3);

	argsq = mmRealMultiply(arg, arg);
	
	fraction = mmRealMultiply(p4, argsq)		+ p3;
	fraction = mmRealMultiply(fraction, argsq)	+ p2;
	fraction = mmRealMultiply(fraction, argsq)	+ p1;
	fraction = mmRealMultiply(fraction, argsq)	+ p0;
	
	denominator = mmRealMultiply((argsq + q4), argsq);
	denominator = mmRealMultiply((denominator + q3), argsq);
	denominator = mmRealMultiply((denominator + q2), argsq);
	denominator = mmRealMultiply((denominator + q1), argsq);
	denominator = denominator + q0;
	
	res = mmRealDivide(fraction, denominator);
	res = mmRealMultiply(res, arg);
	return mmRealToFloat(res);
}

// reduce
float _atan_(float arg)
{
	const float sq2p1	= 2.414213562373095048802e0;
	const float sq2m1	= 0.414213562373095048802e0;

   if(arg < sq2m1)
	   return __atan__(arg);

   if(arg > sq2p1)
	   return __MM_PIO2__ - __atan__(1 / arg);

	return __MM_PIO4__ + __atan__((arg-1)/(arg+1));
}


float mmAsin(float arg)
{
	float temp;
	int sign = 0;
   
	if(arg > 1)return INFINITY;
   
	if(arg < 0)
	{
		arg = -arg;
		sign++;
	}

	temp = mmSqrtf(1 - arg * arg);
	temp = arg > 0.7 ? __MM_PIO2__ - mmAtan(temp/arg) : mmAtan(arg/temp);
	return sign > 0 ? -temp : temp;
}

float mmExp(float x)
{
	int i,k,m = 1,t = 10;
	int xm=(int)x;
	float sum = 1, e= 1.0, ef=1.0, z= 1;
	float sub = x - xm;

	if (xm < 0) {
		xm=(-xm);
		for(k=0; k < xm; k++)
			ef *= 2.718281;
		e /= ef;
	}
	else
	{
		for(k=0; k<xm; k++)
			e *= 2.718281;
	}

	for(i=1; i < t; i++){
		m *= i;
		z *= sub;
		sum+=z/m;
	}
	return sum * e;
}


/*inline*/ mmReal __mmFloor__(mmReal x)
{
	// masks off fraction bits
	return x & MM_FIXED_INT_MASK;
}

/*inline*/ mmReal  __mmCeil__(mmReal x)
{
	// masks off fraction bits and adds one if there were some fractional bits
	mmReal f = x & MM_FIXED_INT_MASK;
	return f != x ? (f + MM_FIXED_RESOLUTION) : x;
}

/*inline*/ float mmSin(float theta)
{
	return mmRealToFloat(__fsin__(mmFloatToReal(theta)));
}

/*inline*/ float mmCos(float theta)
{
	return mmRealToFloat(__fsin__(mmFloatToReal(theta) + MM_PIO2));
}

/*inline*/ float  mmTan(float  theta)
{
	return mmRealToFloat(mmRealDivide(__fsin__(mmFloatToReal(theta)),  __fsin__(mmFloatToReal(theta) + MM_PIO2)));
}

/*inline*/ float mmAtan(float arg)
{
	return arg > 0 ? _atan_(arg) : -_atan_(-arg);
}

// implementation of atan2
/*inline*/ float mmAtan2(float arg1, float arg2)
{
	if(arg1 + arg2 == arg1)
	{
		return arg1 >= 0 ? __MM_PIO2__ : -__MM_PIO2__;
	}

	arg1 = mmAtan(arg1 / arg2);
	if(arg2 < 0)
	{
		return arg1 < 0 ? arg1 + __MM_PI__ : arg1 - __MM_PI__;
	}

	return arg1;
}

/*inline*/ float mmAcos(float arg)
{
	if(arg > 1 || arg < -1)
		return INFINITY;
	return __MM_PIO2__ - mmAsin(arg);
}
#define mmArccos mmAcos

/*inline*/ float mmMod(float a, float b)
{
	return mmRealToFloat(mmRealMod(mmFloatToReal(a), mmFloatToReal(b)));
}

/*inline*/ float mmPow(float x ,float y)
{
	return 1.0;
}

/*inline*/ float mmFloor(float x)
{
	return mmRealToFloat(__mmFloor__(mmFloatToReal(x)));
}

/*inline*/ float mmCeil(float x)
{
	return mmRealToFloat(__mmCeil__(mmFloatToReal(x)));
}