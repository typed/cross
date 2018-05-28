#ifndef _cross_math_mathbase_h_
#define _cross_math_mathbase_h_

#include "Config.h"

namespace cross
{

//3D关系类型
enum eIntersectionRelation3D
{
	IR_Front,
	IR_Back,
	IR_Planer,
	IR_Spanning,
	IR_Clipped
};

static const f32 c_fPI = 3.1415926f;
static const f32 c_f2PI = 2.f * 3.1415926f;
static const f32 c_fDegToRad = c_fPI / 180.f;
static const f32 c_fRadToDeg = 180.f / c_fPI;
static const f32 c_fM_1_PI = 0.318309886183790671538f;

static const i32 c_iRounding_Error = 1;
static const f32 c_fRounding_Error = 0.000001f;
static const f64 c_dRounding_Error = 0.00000001;

//static const f32 c_fMin = 1.175494351e-38F;
//static const f32 c_fMax = 3.402823466e+38F;
static const f32 c_fMin = -9999999.f;
static const f32 c_fMax = 9999999.f;

static const ui32 c_ui32Max = 0xffffffff;
static const ui32 c_ui32Min = 0;
static const i32 c_i32Max = 0x7fffffff;
static const i32 c_i32Min = 0x80000000;

inline f32 Deg2Rad(f32 deg) {
	return deg * c_fDegToRad;
}
inline f32 Rad2Deg(f32 rad) {
	return rad * c_fRadToDeg;
}

//平方根的倒数：1.f/MathFunc::Sqrt(num) 不精确
inline f32 Reciprocal_SquareRoot(f32 x)
{
	f32* pf = (f32*)&x;
	ui32 n = *((ui32*)pf);
	ui32 tmp = (ui32(0x3f800000 << 1) + 0x3f800000 - n) >> 1;
	ui32* pTmp = (ui32*)&tmp;
	f32 y = *((f32*)pTmp);
	return y * (1.47f - 0.47f * x * y * y);
}

//快速平方根,不精确
inline f32 Faster_Sqrt(f32 x)
{
	union {
		i32 intPart;
		f32 floatPart;
	} convertor;
	union{
		i32 intPart;
		f32 floatPart;
	} convertor2;
	convertor.floatPart = x;
	convertor2.floatPart = x;
	convertor.intPart = 0x1FBCF800 + ( convertor.intPart >> 1 );
	convertor2.intPart = 0x5f3759df - ( convertor2.intPart >> 1 );
	return 0.5f * ( convertor.floatPart + (x * convertor2.floatPart) );
}

//数值相等(处理浮点数)
inline bool Equals(const f32 a, const f32 b)
{
	static const f32 c_fPar = 0.000001f;
	return (a + c_fPar >= b) && (a - c_fPar <= b);
}

inline bool Equals(const f64 a, const f64 b)
{
	static const f64 c_fPar = 0.00000001;
	return (a + c_fPar >= b) && (a - c_fPar <= b);
}

inline bool Equals(const long a, const long b)
{
	return a == b;
}

inline bool Equals(const i32 a, const i32 b)
{
	return a == b;
}

//add by yfw 数值小于或等于(大于或等于) 例：a = 0.0000001 时，使用a<=0.0f判断，将导致 返回false，但使用Equals判断将又等于0
inline bool LessOrEquals(const f32 a, const f32 b)
{
	static const f32 c_fPar = 0.000001f;
	return (a - c_fPar <= b);
}

inline bool LessOrEquals(const f64 a, const f64 b)
{
	static const f64 c_fPar = 0.00000001;
	return (a - c_fPar <= b);
}
//
inline bool MoreOrEquals(const f32 a, const f32 b)
{
	static const f32 c_fPar = 0.000001f;
	return (a + c_fPar >= b);
}

inline bool MoreOrEquals(const f64 a, const f64 b)
{
	static const f64 c_fPar = 0.00000001;
	return (a + c_fPar >= b);
}

template<class T> inline T Min(T t1, T t2)
{
	return t1 < t2 ? t1 : t2 ;
}

template<class T> inline T Max(T t1, T t2)
{
	return t1 > t2 ? t1 : t2 ;
}

template<class T> inline T Differ(T n1, T n2)
{
	return n1 > n2 ? n1 - n2 : n2 - n1;
}

}

#endif
