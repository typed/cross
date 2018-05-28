#ifndef _cross_math_vector3d_h_
#define _cross_math_vector3d_h_

#include "Config.h"
#include "MathBase.h"

namespace cross
{

template<class T>
class cVector3D
{
public:
	cVector3D() { Set0(); };
	cVector3D(T px, T py, T pz) {x = px; y = py; z = pz;}
	cVector3D(const cVector3D<T>& other) { *this = other; };
	cVector3D<T>& operator = (const cVector3D<T>& other) {x = other.x; y = other.y; z = other.z; return *this;}
	void Set0() { x = y = z = 0; }
	cVector3D<T>& Set(const T nx, const T ny, const T nz) { x = nx; y = ny; z = nz; return *this;}
	cVector3D<T>& Set(const cVector3D<T>& p) { x = p.x; y = p.y; z = p.z; return *this;}
	bool operator == (const cVector3D<T>& other) const { return x == other.x && y == other.y && z == other.z; }
	bool operator != (const cVector3D<T>& other) const { return !operator == (other); }
	cVector3D<T> operator + (const cVector3D<T>& other) const { return cVector3D<T>(x + other.x, y + other.y, z + other.z); }
	cVector3D<T> operator - (const cVector3D<T>& other) const { return cVector3D<T>(x - other.x, y - other.y, z - other.z); }
	cVector3D<T> operator / (const cVector3D<T>& other) const { return cVector3D<T>(x / other.x, y / other.y, z / other.z); }
	cVector3D<T> operator * (const cVector3D<T>& other) const { return cVector3D<T>(x * other.x, y * other.y, z * other.z); }
	cVector3D<T>& operator += (const cVector3D<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
	cVector3D<T>& operator -= (const cVector3D<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	cVector3D<T>& operator /= (const cVector3D<T>& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
	cVector3D<T>& operator *= (const cVector3D<T>& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	cVector3D<T>& operator += (const T& num) { x += num; y += num; z += num; return *this; }
	cVector3D<T>& operator -= (const T& num) { x -= num; y -= num; z -= num; return *this; }
	cVector3D<T>& operator /= (const T& num) { x /= num; y /= num; z /= num; return *this; }
	cVector3D<T>& operator *= (const T& num) { x *= num; y *= num; z *= num; return *this; }
	cVector3D<T> operator + (const T& num) const {return cVector3D<T>(x + num, y + num, z + num); }
	cVector3D<T> operator - (const T& num) const {return cVector3D<T>(x - num, y - num, z - num); }
	cVector3D<T> operator / (const T& num) const {return cVector3D<T>(x / num, y / num, z / num); }
	cVector3D<T> operator * (const T& num) const {return cVector3D<T>(x * num, y * num, z * num); }
	bool operator <= (const cVector3D<T>& other) const { return x <= other.x && y <= other.y && z <= other.z;}
	bool operator >= (const cVector3D<T>& other) const { return x >= other.x && y >= other.y && z >= other.z;}
	bool operator < (const cVector3D<T>& other) const { return x < other.x && y < other.y && z < other.z;}
	bool operator > (const cVector3D<T>& other) const { return x > other.x && y > other.y && z > other.z;}
	T& operator [] (ui32 i) const { return ((T*)&x)[i];}

	//向量的大小
	T GetLength() const { return MathFunc::Sqrt(x * x + y * y + z * z); }
	T GetLengthSQ() const { return (x * x + y * y + z * z); }
	//规格化
	cVector3D<T>& Normalize();
	//叉积
	cVector3D<T> CrossProduct(const cVector3D<T>& v) const { return cVector3D<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	//点积
	T DotProduct(const cVector3D<T>& other) const { return x * other.x + y * other.y + z * other.z; }
	//距离
	T GetDistance(const cVector3D<T>& des) const { return (des - *this).GetLength(); }
	T GetDistanceSQ(const cVector3D<T>& des) const { return (des - *this).GetLengthSQ(); }

	//插值
	cVector3D<T> GetInterpolated(const cVector3D<T>& other, float d) const;

	void RotateXZBy(float degrees, const cVector3D<T>& center=cVector3D<T>());
	void RotateXYBy(float degrees, const cVector3D<T>& center=cVector3D<T>());
	void RotateYZBy(float degrees, const cVector3D<T>& center=cVector3D<T>());

	//绕任意轴旋转，轴通过原点
	void RotateRadianByAnyAxis(const cVector3D<T>& axis, float radian) {RotateRadianByAnyAxis(axis, radian, cVector3D<T>());}
	void RotateRadianByAnyAxis(const cVector3D<T>& axis, float radian, const cVector3D<T>& center);
	void RotateDegreeByAnyAxis(const cVector3D<T>& axis, float degree) {RotateDegreeByAnyAxis(axis, degree, cVector3D<T>());}
	void RotateDegreeByAnyAxis(const cVector3D<T>& axis, float degree, const cVector3D<T>& center);

	//根据公式 a.b = |a|*|b|*cos(theta); 推导出
	float GetTheta(const cVector3D<T>& vDes) const;
	//向任意平面投影，平面通过原点
	void ProjectByAnyPlane(const cVector3D<T>& directNor, const cVector3D<T>& center);

	void Reverse() { x = -x; y = -y; z = -z; }
	bool IsEmpty() const { return Equals(x, (T)0) && Equals(y, (T)0) && Equals(z, (T)0); }
	bool IsParallel(const cVector3D<T>& v) const { return CrossProduct(v).IsEmpty(); }

	bool IsBetweenPoints(const cVector3D<T>& begin, const cVector3D<T>& end) const;

	T x;
	T y;
	T z;
};

//add by yfw
template<class T>
class cSize3DTemplate :
	public cVector3D<T>
{
public:
	using cVector3D<T>::x;
	using cVector3D<T>::y;
	using cVector3D<T>::z;
	cSize3DTemplate() {}
	cSize3DTemplate(T cx, T cy, T cz) { x = cx; y = cy; z = cz;}
	cSize3DTemplate(const cVector3D<T>& a) { x = a.x; y = a.y; z = a.z; }
	void SetWidth(T t) { x = t; }			//
	T GetWidth() const { return x; }
	void SetHeight(T t) { y = t; }			//高度(垂直于水平)
	T GetHeight() const { return y; }
	void SetUpWidth(T t) { z = t; }			//z的宽度（在水平中，竖直的高度）
	T GetUpWidth() const { return z; }
	void SetSize(T cx, T cy, T cz) { x = cx; y = cy; z = cz;}
	//y特殊处理，在3d坐标中，只考虑xz一面时
	bool IsIn(T tx, T ty, T tz) const { return (tx >= 0 && tx < x && ((ty >= 0 && ty < y) || (y==0 && ty==y)) && tz >= 0 && tz < z); }
	bool IsEmpty() const {return x <= 0 || y <0 || z<=0;}
	//缩放，< 1.f 缩小，> 1.f 放大
	void SetScale(float xscale, float yscale, float zscale, float xfloat = 0.f, float yfloat = 0.f, float zfloat=0.0f) {
		x = x * xscale + xfloat;
		y = y * yscale + yfloat;
		z = z * zscale + zfloat;
	}
};

template<class T>
inline cVector3D<T>& cVector3D<T>::Normalize()
{
	T l = x * x + y * y + z * z;
	if (l == 0)
		return *this;
	l = (T) ( 1 / MathFunc::Sqrt(l) );
	x *= l; y *= l; z *= l;
	return *this;
}

template<class T>
inline cVector3D<T> cVector3D<T>::GetInterpolated(const cVector3D<T>& other, float d) const
{
	const float inv = 1.f - d;
	return cVector3D<T>((T)(other.x*inv + x*d), (T)(other.y*inv + y*d), (T)(other.z*inv + z*d));
}

template<class T>
inline void cVector3D<T>::RotateXZBy(float degrees, const cVector3D<T>& center)
{
	degrees *= c_fDegToRad;
	float cs = MathFunc::Cos(degrees);
	float sn = MathFunc::Sin(degrees);
	x -= center.x;
	z -= center.z;
	Set((T)(x*cs - z*sn), y, (T)(x*sn + z*cs));
	x += center.x;
	z += center.z;
}

template<class T>
inline void cVector3D<T>::RotateXYBy(float degrees, const cVector3D<T>& center)
{
	degrees *= c_fDegToRad;
	float cs = MathFunc::Cos(degrees);
	float sn = MathFunc::Sin(degrees);
	x -= center.x;
	y -= center.y;
	Set((T)(x*cs - y*sn), (T)(x*sn + y*cs), z);
	x += center.x;
	y += center.y;
}

template<class T>
inline void cVector3D<T>::RotateYZBy(float degrees, const cVector3D<T>& center)
{
	degrees *= c_fDegToRad;
	float cs = MathFunc::Cos(degrees);
	float sn = MathFunc::Sin(degrees);
	z -= center.z;
	y -= center.y;
	Set(x, (T)(y*cs - z*sn), (T)(y*sn + z*cs));
	z += center.z;
	y += center.y;
}

template<class T>
inline void cVector3D<T>::RotateRadianByAnyAxis(const cVector3D<T>& axis, float radian, const cVector3D<T>& center)
{
	float cs = MathFunc::Cos(radian);
	float sn = MathFunc::Sin(radian);
	float t  = 1 - cs;
	float mxyt = axis.x*axis.y*t;
	float mxzt = axis.x*axis.z*t;
	float myzt = axis.y*axis.z*t;
	x -= center.x;
	y -= center.y;
	z -= center.z;
	float xtemp = x*(axis.x*axis.x*t + cs) + y*(mxyt - axis.z*sn) + z*(mxzt + axis.y*sn);
	float ytemp = x*(mxyt + axis.z*sn) + y*(axis.y*axis.y*t + cs) + z*(myzt - axis.x*sn);
	float ztemp = x*(mxzt - axis.y*sn) + y*(myzt + axis.x*sn) + z*(axis.z*axis.z*t + cs);
	Set((T)xtemp, (T)ytemp, (T)ztemp);
	x += center.x;
	y += center.y;
	z += center.z;
}

template<class T>
inline void cVector3D<T>::RotateDegreeByAnyAxis(const cVector3D<T>& axis, float degree, const cVector3D<T>& center)
{
	RotateRadianByAnyAxis(axis, Deg2Rad(degree), center);
}

template<class T>
inline float cVector3D<T>::GetTheta(const cVector3D<T>& vDes) const
{
	cVector3D<T> v1 = *this;
	v1.Normalize();
	cVector3D<T> v2 = vDes;
	v2.Normalize();
	T n = v1.DotProduct(v2);
	if (n > (T)1)
		n = 1;
	else if (n < (T)-1)
		n = -1;
	return MathFunc::Acos(n);
}

template<class T>
inline void cVector3D<T>::ProjectByAnyPlane(const cVector3D<T>& direct, const cVector3D<T>& center)
{
	x -= center.x;
	y -= center.y;
	z -= center.z;
	float xtemp = x*(1 - direct.x*direct.x) + y*(-1*direct.x*direct.y) + z*(-1*direct.x*direct.z);
	float ytemp = x*(-1*direct.x*direct.y) + y*(1 - direct.y*direct.y) + z*(-1*direct.z*direct.y);
	float ztemp = x*(-1*direct.x*direct.z) + y*(-1*direct.z*direct.y) + z*(1 - direct.z*direct.z);
	Set((T)xtemp, (T)ytemp, (T)ztemp);
	x += center.x;
	y += center.y;
	z += center.z;
}

template<class T>
inline bool cVector3D<T>::IsBetweenPoints(const cVector3D<T>& begin, const cVector3D<T>& end) const
{
	const T f = (end - begin).GetLengthSQ();
	return GetDistanceSQ(begin) <= f && GetDistanceSQ(end) <= f;
}

//add by yfw
template<class T0, class T1>
inline cVector3D<T1> TransVector3DTo3D(cVector3D<T1>& vOut, const cVector3D<T0>& vIn)
{
	for (ui32 unIndex = 0; unIndex < 3; unIndex++)
		vOut[unIndex] = (T1)vIn[unIndex];
	return vOut;
}

typedef cVector3D<i32>			cVector3Dl;
typedef cVector3D<f32>			cVector3Df;
typedef cVector3D<ui16>			cVector3Dus;

typedef cSize3DTemplate<i32>	cSize3D;
typedef cSize3DTemplate<f32>	cSizef3D;

}

typedef cross::cVector3Df	vector3d;
typedef cross::cVector3Dus	face;

typedef cross::cSize3D		size3d;
typedef cross::cSizef3D		size3df;

#endif
