#ifndef _cross_math_line3d_h_
#define _cross_math_line3d_h_

#include "../CommonDef.h"
#include "MathBase.h"
#include "cVector3D.h"

namespace cross
{

template<class T>
class cLine3D
{
public:
	cLine3D() {};
	cLine3D(const cVector3D<T>& vfrom, const cVector3D<T>& vto) { from = vfrom; to = vto;}
	cLine3D(const cLine3D<T>& other) { *this = other; };
	cLine3D<T>& operator = (const cLine3D<T>& other) { from = other.from; to = other.to; return *this;}
	void Set0() { from.Set0(); to.Set0(); }
	bool operator == (const cLine3D<T>& other) const { return from == other.from && to == other.to;}
	bool operator != (const cLine3D<T>& other) const { return !operator == (other);}
	cLine3D<T> operator + (const cVector3D<T>& point) const { return cLine3D<T>(from + point, to + point); }
	cLine3D<T>& operator += (const cVector3D<T>& point) { from += point; to += point; return *this; }
	cLine3D<T> operator - (const cVector3D<T>& point) const { return cLine3D<T>(from - point, to - point); }
	cLine3D<T>& operator -= (const cVector3D<T>& point) { from -= point; to -= point; return *this; }

	void SetLine(const T& xa, const T& ya, const T& za, const T& xb, const T& yb, const T& zb) {from.Set(xa, ya, za); to.Set(xb, yb, zb);}
	void SetLine(const cVector3D<T>& nstart, const cVector3D<T>& nend) {from.Set(nstart); to.Set(nend);}
	void SetLine(const cLine3D<T>& line) {from.Set(line.from); to.Set(line.to);}
	void SetLine(const cVector3D<T>& start, const cVector3D<T>& vDir, T len) {from = start; to = start + vDir * len;}

	//距离
	T GetLength() const { cVector3D<T> v = to - from; return (T)v.GetLength();}
	//
	cVector3D<T> GetMiddle() const {return (from + to) * (T)0.5;}
	//
	cVector3D<T> GetVector() const {return to - from;}
	//
	bool IsPointBetweenStartAndEnd(const cVector3D<T>& point) const {return point.IsBetweenPoints(from, to);}
	//
	cVector3D<T> GetClosestPoint(const cVector3D<T>& point) const;
	//
	bool GetIntersectionWithSphere(cVector3D<T> sorigin, T sradius, double& outdistance) const;

	cVector3D<T>	from;
	cVector3D<T>	to;
};

template<class T>
inline cVector3D<T> cLine3D<T>::GetClosestPoint(const cVector3D<T>& point) const
{
	cVector3D<T> c = point - from;
	cVector3D<T> v = to - from;
	T d = (T)v.GetLength();
	v /= d;
	T t = v.dotProduct(c);

	if (t < (T)0)
		return from;
	if (t > d)
		return from;

	v *= t;
	return from + v;
}

template<class T>
inline bool cLine3D<T>::GetIntersectionWithSphere(cVector3D<T> sorigin, T sradius, double& outdistance) const
{
	const cVector3D<T> q = sorigin - from;
	T c = q.GetLength();
	T v = q.DotProduct(GetVector().Normalize());
	T d = sradius * sradius - (c*c - v*v);

	if (d < (T)0)
		return false;

	outdistance = v - sqrt ( d );
	return true;
}

//射线
template<class T>
class cRay3D
{
public:
	cRay3D() {};
	cRay3D(const cVector3D<T>& vStart, const cVector3D<T>& vDir) : m_vStart(vStart), m_vDir(vDir) {}
	cRay3D(const cRay3D<T>& other) { *this = other; };
	cRay3D<T>& operator = (const cRay3D<T>& other) { m_vStart = other.m_vStart; m_vDir = other.m_vDir; return *this;}
	void Set0() { m_vStart.Set0(); m_vDir.Set0(); }
	bool operator == (const cRay3D<T>& other) const { return m_vStart == other.m_vStart && m_vDir == other.m_vDir;}
	bool operator != (const cRay3D<T>& other) const { return !operator == (other);}
	const cVector3D<T>& GetStart() const {return m_vStart;}
	const cVector3D<T>& GetDir() const {return m_vDir;}
private:
	cVector3D<T>	m_vStart;
	cVector3D<T>	m_vDir;
};

typedef cLine3D<f32> cLine3Df;
typedef cLine3D<i32> cLine3Dl;

typedef cRay3D<f32> cRay3Df;
typedef cRay3D<i32> cRay3Dl;

}

typedef cm::cLine3Df		line3d;
typedef cm::cRay3Df		ray3d;

#endif
