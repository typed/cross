#ifndef _cross_math_vector4d_h_
#define _cross_math_vector4d_h_

#include "Config.h"
#include "MathBase.h"
#include "cVector3D.h"

namespace cross
{

template<class T>
class cVector4D
{
public:
	cVector4D() { Set0(); }
	cVector4D(T px, T py, T pz, T pw) {x = px; y = py; z = pz; w = pw;}
	cVector4D(const cVector4D<T>& other) { *this = other; }
	cVector4D<T>& operator = (const cVector4D<T>& other) {x = other.x; y = other.y; z = other.z; w = other.w; return *this;}
	void Set0() { x = y = z = (T)0; w = (T)1;}
	cVector4D<T>& Set(const T nx, const T ny, const T nz, const T nw) { x = nx; y = ny; z = nz; w = nw; return *this;}
	cVector4D<T>& Set(const cVector4D<T>& p) { x = p.x; y = p.y; z = p.z; w = p.w; return *this;}
	cVector4D<T>& Set(const cVector3D<T>& p) { x = p.x; y = p.y; z = p.z; w = 1.f; return *this;}
	bool operator == (const cVector4D<T>& other) const { return ( Equals(x, other.x) && Equals(y, other.y) && Equals(z, other.z) && Equals(w, other.w) ); }
	bool operator != (const cVector4D<T>& other) const { return !operator == (other); }
	cVector4D<T> operator + (const cVector4D<T>& other) const { return cVector4D<T>(x + other.x, y + other.y, z + other.z, w + other.w); }
	cVector4D<T> operator - (const cVector4D<T>& other) const { return cVector4D<T>(x - other.x, y - other.y, z - other.z, w - other.w); }
	cVector4D<T> operator / (const cVector4D<T>& other) const { return cVector4D<T>(x / other.x, y / other.y, z / other.z, w / other.w); }
	cVector4D<T> operator * (const cVector4D<T>& other) const { return cVector4D<T>(x * other.x, y * other.y, z * other.z, w * other.w); }
	cVector4D<T>& operator += (const cVector4D<T>& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
	cVector4D<T>& operator -= (const cVector4D<T>& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
	cVector4D<T>& operator /= (const cVector4D<T>& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
	cVector4D<T>& operator *= (const cVector4D<T>& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
	cVector4D<T>& operator += (const T& num) { x += num; y += num; z += num; w += num; return *this; }
	cVector4D<T>& operator -= (const T& num) { x -= num; y -= num; z -= num; w -= num; return *this; }
	cVector4D<T>& operator /= (const T& num) { x /= num; y /= num; z /= num; w /= num; return *this; }
	cVector4D<T>& operator *= (const T& num) { x *= num; y *= num; z *= num; w *= num; return *this; }
	cVector4D<T> operator + (const T& num) const {return cVector4D<T>(x + num, y + num, z + num, w + num); }
	cVector4D<T> operator - (const T& num) const {return cVector4D<T>(x - num, y - num, z - num, w - num); }
	cVector4D<T> operator / (const T& num) const {return cVector4D<T>(x / num, y / num, z / num, w / num); }
	cVector4D<T> operator * (const T& num) const {return cVector4D<T>(x * num, y * num, z * num, w * num); }
	bool operator <= (const cVector4D<T>& other) const { return x <= other.x && y <= other.y && z <= other.z && w <= other.w;}
	bool operator >= (const cVector4D<T>& other) const { return x >= other.x && y >= other.y && z >= other.z && w >= other.w;}
	bool operator < (const cVector4D<T>& other) const { return x < other.x && y < other.y && z < other.z && w < other.w;}
	bool operator > (const cVector4D<T>& other) const { return x > other.x && y > other.y && z > other.z && w > other.w;}
	T& operator [] (ui32 i) const { return ((T*)&x)[i];}

	T x;
	T y;
	T z;
	T w;
};

typedef cVector4D<i32>			cVector4Dl;
typedef cVector4D<f32>			cVector4Df;
typedef cVector4D<ui16>			cVector4Dus;

}

typedef cross::cVector4Df	vector4d;
typedef cross::cVector4Dus	vector4dus;

#endif
