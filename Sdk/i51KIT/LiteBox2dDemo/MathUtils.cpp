
#include "MathUtils.h"

typedef union
{
  float value;
//  uint32 word;
  iU32 word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)                                     \
do {                                                            \
  ieee_float_shape_type gf_u;                                   \
  gf_u.value = (d);                                             \
  (i) = gf_u.word;                                              \
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)                                     \
do {                                                            \
  ieee_float_shape_type sf_u;                                   \
  sf_u.word = (i);                                              \
  (d) = sf_u.value;                                             \
} while (0)

float fabsf(float x)
{
//	uint32 ix;
//	iU32 ix;
//    GET_FLOAT_WORD(ix,x);
//    SET_FLOAT_WORD(x,ix&0x7fffffff);
	if(x>0) return x;
	else return -x;
 //   return x;
}

	Vec2::Vec2() {}
	Vec2::Vec2(float x, float y) : x(x), y(y) {}

	void Vec2::Set(float x_, float y_) { x = x_; y = y_; }

	Vec2 Vec2::operator -() { return Vec2(-x, -y); }
	
	void Vec2::operator += (const Vec2& v)
 	{
 		x += v.x; y += v.y;
 	}
	
	void Vec2::operator -= (const Vec2& v)
	{
		x -= v.x; y -= v.y;
	}

	void Vec2::operator *= (float a)
	{
		x *= a; y *= a;
	}

	float Vec2::Length() const
	{
		return sqrtf(x * x + y * y);
	}

	Mat22::Mat22() {}
	void Mat22::Mat22_1(float angle)
	{
		float c = cosf(angle), s = sinf(angle);
		col1.x = c; col2.x = -s;
		col1.y = s; col2.y = c;
	}

	Mat22::Mat22(const Vec2& col1, const Vec2& col2) : col1(col1), col2(col2) {};

	Mat22 Mat22::Transpose() const
	{
		return Mat22(Vec2(col1.x, col2.x), Vec2(col1.y, col2.y));
	}

	Mat22 Mat22::Invert() const
	{
		float a = col1.x, b = col2.x, c = col1.y, d = col2.y;
		Mat22 B;
		float det = a * d - b * c;
//		assert(det != 0.0f);
		det = 1.0f / det;
		B.col1.x =  det * d;	B.col2.x = -det * b;
		B.col1.y = -det * c;	B.col2.y =  det * a;
		return B;
	}
	
/* |cos(x) - c(x)| < 2**-34.1 (~[-5.37e-11, 5.295e-11]). */
static const double
C0  = -0.499999997251031003120,  /* -0x1ffffffd0c5e81.0p-54 */
C1  =  0.0416666233237390631894,  /* 0x155553e1053a42.0p-57  */
C2  = -0.00138867637746099294692,  /* -0x16c087e80f1e27.0p-62 */
C3  =  0.0000243904487962774090654;  /* 0x199342e0ee5069.0p-68 */
static const float 
one =  1.0;

float cosf(float x)
{
	float DoublePi = k_pi+k_pi;
	float SquareX = x*x;
	float CurrResult = 1.0;
	float NextResult = SquareX/2;
	int Temp = 2;
	char SignFlag = 9;
// double r, w, z;
// double TempResult = 0.0;
// /* Try to optimize for parallel evaluation as in k_tanf.c. */
// z = x*x;
// w = z*z;
// r = C2+z*C3;
// TempResult= (float)(((one+z*C0) + w*C1) + (w*z)*r);	
//return (float)(TempResult);
	while(x>(DoublePi)) x -= DoublePi;
	while(x<(-DoublePi)) x += DoublePi;

	SquareX = x*x;
	NextResult = SquareX/2;

	while(NextResult>triangle_accuracy||NextResult<(-triangle_accuracy))
	{
//iLog("sinf:SignFlag=%d,CurrResult=%d,NextResult=%d",SignFlag,(int)(CurrResult*1000000),(int)(NextResult*1000000));
		if(SignFlag>10)
		{
			CurrResult += NextResult;
			SignFlag = 9;
		}
		else
		{
			CurrResult -= NextResult;
			SignFlag = 100;
		}
		Temp+=2;
		NextResult = NextResult*SquareX/((float)(Temp*(Temp-1)));
	}
//iLog("cosf:X=%d,%d,Temp=%d,NextResult=%d",(int)(x*1000000),(int)(CurrResult*1000000),Temp,(int)(NextResult*1000000));
        return (float)(CurrResult);
}

/* |sin(x)/x - s(x)| < 2**-37.5 (~[-4.89e-12, 4.824e-12]). */
static const double
S1 = -0.166666666416265235595,   /* -0x15555554cbac77.0p-55 */
S2 =  0.0083333293858894631756,   /* 0x111110896efbb2.0p-59 */
S3 = -0.000198393348360966317347,   /* -0x1a00f9e2cae774.0p-65 */
S4 =  0.0000027183114939898219064;   /* 0x16cd878c3b46a7.0p-71 */


float sinf(float x)
{
	float DoublePi = k_pi+k_pi;
	float SquareX = x*x;
	float CurrResult = x;
	float NextResult = SquareX*x/2/3;
	int Temp = 3;
	char SignFlag = 9;

// double r, s, w, z;
// double TempResult = 0.0;
// /* Try to optimize for parallel evaluation as in k_tanf.c. */
// z = x*x;
// w = z*z;
// r = S3+z*S4;
// s = z*x;
// TempResult=((x + s*(S1+z*S2)) + s*w*r);
// //return (float)(TempResult);
	while(x>(DoublePi)) x -= DoublePi;
	while(x<(-DoublePi)) x += DoublePi;
	SquareX = x*x;
	CurrResult = x;
	NextResult = SquareX*x/2/3;

	while(NextResult>triangle_accuracy||NextResult<(-triangle_accuracy))
	{
//iLog("sinf:SignFlag=%d,CurrResult=%d,NextResult=%d",SignFlag,(int)(CurrResult*1000000),(int)(NextResult*1000000));
		if(SignFlag>10)
		{
			CurrResult += NextResult;
			SignFlag = 9;
		}
		else
		{
			CurrResult -= NextResult;
			SignFlag = 100;
		}
		Temp+=2;
		NextResult = NextResult*SquareX/((float)(Temp*(Temp-1)));
	}
//iLog("sinf:X=%d,%d,Temp=%d,NextResult=%d",(int)(x*1000000),(int)(CurrResult*1000000),Temp,(int)(NextResult*1000000));
       return (float)(CurrResult);
}

static const double tiny=1.0e-30;

float sqrtf(float x)
{
        float z;
// 		int32 sign = (int)0x80000000;
// 		int32 ix,s,q,m,t,i;
// 		uint32 r;
		iS32 sign = (int)0x80000000;
		iS32 ix,s,q,m,t,i;
		iU32 r;

        GET_FLOAT_WORD(ix,x);

    /* take care of Inf and NaN */
        if((ix&0x7f800000)==0x7f800000) {
            return x*x+x;               /* sqrt(NaN)=NaN, sqrt(+inf)=+inf
                                           sqrt(-inf)=sNaN */
        }
    /* take care of zero */
        if(ix<=0) {
            if((ix&(~sign))==0) return x;/* sqrt(+-0) = +-0 */
            else if(ix<0)
                return (x-x)/(x-x);             /* sqrt(-ve) = sNaN */
        }
    /* normalize x */
        m = (ix>>23);
        if(m==0) {                              /* subnormal x */
            for(i=0;(ix&0x00800000)==0;i++) ix<<=1;
            m -= i-1;
        }
        m -= 127;       /* unbias exponent */
        ix = (ix&0x007fffff)|0x00800000;
        if(m&1) /* odd m, double x to make it even */
            ix += ix;
        m >>= 1;        /* m = [m/2] */

    /* generate sqrt(x) bit by bit */
        ix += ix;
        q = s = 0;              /* q = sqrt(x) */
        r = 0x01000000;         /* r = moving bit from right to left */

        while(r!=0) {
            t = s+r;
            if(t<=ix) {
                s    = t+r;
                ix  -= t;
                q   += r;
            }
            ix += ix;
            r>>=1;
        }

    /* use floating add to find out rounding direction */
        if(ix!=0) {
            z = one-(float)tiny; /* trigger inexact flag */
            if (z>=one) {
                z = one+(float)tiny;
                if (z>one)
                    q += 2;
                else
                    q += (q&1);
            }
        }
        ix = (q>>1)+0x3f000000;
        ix += (m <<23);
        SET_FLOAT_WORD(z,ix);
        return z;
}

  float Dot(const Vec2& a, const Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

  float Cross(const Vec2& a, const Vec2& b)
{
	return a.x * b.y - a.y * b.x;
}

  Vec2 Cross(const Vec2& a, float s)
{
	return Vec2(s * a.y, -s * a.x);
}

  Vec2 Cross(float s, const Vec2& a)
{
	return Vec2(-s * a.y, s * a.x);
}

  Vec2 operator * (const Mat22& A, const Vec2& v)
{
	return Vec2(A.col1.x * v.x + A.col2.x * v.y, A.col1.y * v.x + A.col2.y * v.y);
}

  Vec2 operator + (const Vec2& a, const Vec2& b)
{
	return Vec2(a.x + b.x, a.y + b.y);
}

  Vec2 operator - (const Vec2& a, const Vec2& b)
{
	return Vec2(a.x - b.x, a.y - b.y);
}

  Vec2 operator * (float s, const Vec2& v)
{
	return Vec2(s * v.x, s * v.y);
}

  Mat22 operator + (const Mat22& A, const Mat22& B)
{
	return Mat22(A.col1 + B.col1, A.col2 + B.col2);
}

  Mat22 operator * (const Mat22& A, const Mat22& B)
{
	return Mat22(A * B.col1, A * B.col2);
}

  float Abs(float a)
{
	return a > 0.0f ? a : -a;
}

  Vec2 Abs(const Vec2& a)
{
	return Vec2(fabsf(a.x), fabsf(a.y));
}

  Mat22 Abs(const Mat22& A)
{
	return Mat22(Abs(A.col1), Abs(A.col2));
}

  float Sign(float x)
{
	return x < 0.0f ? -1.0f : 1.0f;
}

  float Min(float a, float b)
{
	return a < b ? a : b;
}

  float Max(float a, float b)
{
	return a > b ? a : b;
}

  float Clamp(float a, float low, float high)
{
	return Max(low, Min(a, high));
}



// Random number in range [-1,1]
float Random()
{
	float r = (float)(rand());
	if(r>RAND_MAX)
	{
		r = (float)(((int)r)%RAND_MAX);
	}
	r = r/(float)(RAND_MAX);
	r = 2.0f * r - 1.0f;
	return r;
}

float Random(float lo, float hi)
{
	float r = (float)(rand());
	if(r>RAND_MAX)
	{
		r = (float)(((int)r)%RAND_MAX);
	}
	r = r/((float)(RAND_MAX));
	r = (hi - lo) * r + lo;
	return r;
}

