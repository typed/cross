#ifndef _cross_math_triangle3d_h_
#define _cross_math_triangle3d_h_

#include "Config.h"
#include "MathBase.h"
#include "cVector3D.h"
#include "cAabbox.h"

namespace cross
{

template<typename T>
class cTriangle3D
{
public:
	cTriangle3D() {}
	cTriangle3D(const cTriangle3D<T>& tri) {*this = tri;}
	cTriangle3D(const cVector3D<T>& v1, const cVector3D<T>& v2, const cVector3D<T>& v3) {Set(v1, v2, v3);}
	cTriangle3D<T>& operator = (const cTriangle3D<T>& tri) {Set(tri.m_v1, tri.m_v2, tri.m_v3); return *this;}
	bool operator == (const cTriangle3D<T>& tri) const {return m_v1 == tri.m_v1 && m_v2 == tri.m_v2 && m_v3 == tri.m_v3;}
	bool operator != (const cTriangle3D<T>& tri) const {return ! operator == (tri);}
	void Set0() {m_v1.Set0(); m_v2.Set0(); m_v3.Set0();}
	void Set(const cVector3D<T>& v1, const cVector3D<T>& v2, const cVector3D<T>& v3) {m_v1 = v1; m_v2 = v2; m_v3 = v3;}

	bool IsPointInside(const cVector3D<T>& p) const {return IsOnSameSide(p, m_v1, m_v2, m_v3) && IsOnSameSide(p, m_v2, m_v1, m_v3) && IsOnSameSide(p, m_v3, m_v1, m_v2); }
	bool IsPointInsideFast(const cVector3D<T>& p) const;
	bool IsTotalInsideBox(const cAabbox<T>& box) const {return (box.IsPointInside(m_v1) && box.IsPointInside(m_v2) && box.IsPointInside(m_v3));}
	cVector3D<T> ClosestPointOnTriangle(const cVector3D<T>& p) const;
	bool IsFrontFacing(const cVector3D<T>& lookDirection) const;

	bool GetIntersectionWithLimitedLine(const cLine3D<T>& line, cVector3D<T>& outIntersection) const;
	bool GetIntersectionWithLine(const cVector3D<T>& linePoint, const cVector3D<T>& lineVect, cVector3D<T>& outIntersection) const;
	bool GetIntersectionOfPlaneWithLine(const cVector3D<T>& linePoint, const cVector3D<T>& lineVect, cVector3D<T>& outIntersection) const;

	cVector3D<T> GetNormal() const {return (m_v2 - m_v1).CrossProduct(m_v3 - m_v1);}
	cPlane3D<T> GetPlane() const {return cPlane3D<T>(m_v1, m_v2, m_v3);}
	T GetArea() const {return (m_v2 - m_v1).CrossProduct(m_v3 - m_v1).GetLength() * 0.5;}

private:
	bool IsOnSameSide(const cVector3D<T>& p1, const cVector3D<T>& p2, const cVector3D<T>& a, const cVector3D<T>& b) const;
	cVector3D<T> m_v1;
	cVector3D<T> m_v2;
	cVector3D<T> m_v3;
};

template<typename T>
inline bool cTriangle3D<T>::IsOnSameSide(const cVector3D<T>& p1, const cVector3D<T>& p2, const cVector3D<T>& a, const cVector3D<T>& b) const
{
	cVector3D<T> bminusa = b - a;
	cVector3D<T> cp1 = bminusa.CrossProduct(p1 - a);
	cVector3D<T> cp2 = bminusa.CrossProduct(p2 - a);
	return (cp1.DotProduct(cp2) >= 0.0f);
}

template<typename T>
inline bool cTriangle3D<T>::IsPointInsideFast(const cVector3D<T>& p) const
{
	const cVector3D<T> f = m_v2 - m_v1;
	const cVector3D<T> g = m_v3 - m_v1;

	const f32 a = f.DotProduct(f);
	const f32 b = f.DotProduct(g);
	const f32 c = g.DotProduct(g);

	const cVector3D<T> vp = p - m_v1;
	const f32 d = vp.DotProduct(f);
	const f32 e = vp.DotProduct(g);

	f32 x = (d*c)-(e*b);
	f32 y = (e*a)-(d*b);
	const f32 ac_bb = (a*c)-(b*b);
	f32 z = x+y-ac_bb;

	// return sign(z) && !(sign(x)||sign(y))
	return (( ((ui32&)z) & ~(((ui32&)(x))|((ui32&)(y))) ) & 0x80000000) != 0;
}

template<typename T>
inline cVector3D<T> cTriangle3D<T>::ClosestPointOnTriangle(const cVector3D<T>& p) const
{
	const cVector3D<T> rab = cLine3D<T>(m_v1, m_v2).GetClosestPoint(p);
	const cVector3D<T> rbc = cLine3D<T>(m_v2, m_v3).GetClosestPoint(p);
	const cVector3D<T> rca = cLine3D<T>(m_v3, m_v1).GetClosestPoint(p);
	const T d1 = rab.GetDistance(p);
	const T d2 = rbc.GetDistance(p);
	const T d3 = rca.GetDistance(p);
	if (d1 < d2)
		return d1 < d3 ? rab : rca;
	return d2 < d3 ? rbc : rca;
}

template<typename T>
inline bool cTriangle3D<T>::IsFrontFacing(const cVector3D<T>& lookDirection) const
{
	const cVector3D<T> n = GetNormal().Normalize();
	const f32 d = (f32)n.DotProduct(lookDirection);
	return d <= 0.0f;
}

template<typename T>
inline bool cTriangle3D<T>::GetIntersectionWithLimitedLine(const cLine3D<T>& line, cVector3D<T>& outIntersection) const
{
	return GetIntersectionWithLine(line.start, line.GetVector(), outIntersection) && 
		outIntersection.IsBetweenPoints(line.start, line.end);
}

template<typename T>
inline bool cTriangle3D<T>::GetIntersectionWithLine(const cVector3D<T>& linePoint, const cVector3D<T>& lineVect, cVector3D<T>& outIntersection) const
{
	if (GetIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
		return IsPointInside(outIntersection);
	return false;
}

template<typename T>
inline bool cTriangle3D<T>::GetIntersectionOfPlaneWithLine(const cVector3D<T>& linePoint, const cVector3D<T>& lineVect, cVector3D<T>& outIntersection) const
{
	const cVector3D<T> normal = GetNormal().Normalize();
	T t2;
	if ( cross::Equals( t2 = normal.DotProduct(lineVect), (T)0 ) )
		return false;
	T d = m_v1.DotProduct(normal);
	T t = -(normal.DotProduct(linePoint) - d) / t2;
	outIntersection = linePoint + (lineVect * t);
	return true;
}


typedef cTriangle3D<f32> cTriangle3Df;

}

typedef cross::cTriangle3Df	tri;

#endif
