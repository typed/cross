#ifndef _cross_math_plane3d_h_
#define _cross_math_plane3d_h_

#include "../CommonDef.h"
#include "MathBase.h"
#include "cVector3D.h"
#include "cLine3D.h"

namespace cross
{

/*
	平面方程：n.p + d = 0;其中n为法线向量，p为平面上的点，
	则d = - n.p;

	求射线和平面的交点，原理:
	平面方程：
	n.p + d = 0;
	射线方程：
	p = p0 + tu; 其中p0为起点，u为射线方向
	将射线方程代入平面方程得到: n.(p0 + tu) + d = 0;
	计算可以得到t = (-d - (n.p0))/(n.u);
	如果t > 0 则射线和平面相交，将t代入射线方程 得到交点
*/
template<class T>
class cPlane3D
{
public:
	cPlane3D() : m_vNormal(0, 1, 0) {RecalculateD(cVector3D<T>(0, 0, 0));}
	cPlane3D(const cVector3D<T>& MPoint, const cVector3D<T>& Normal) : m_vNormal(Normal) {RecalculateD(MPoint);}
	cPlane3D(T px, T py, T pz, T nx, T ny, T nz) : m_vNormal(nx, ny, nz) { RecalculateD(cVector3D<T>(px, py, pz)); }
	cPlane3D(const cVector3D<T>& point1, const cVector3D<T>& point2, const cVector3D<T>& point3) { SetPlane(point1, point2, point3); }
	cPlane3D(const cPlane3D<T>& pd) {*this = pd;}
	cPlane3D<T>& operator = (const cPlane3D<T>& pd) {m_vNormal = pd.m_vNormal; m_d = pd.m_d; return *this;}
	bool operator == (const cPlane3D<T>& pd) const {return m_vNormal == pd.m_vNormal && Equals(m_d, pd.m_d);}
	bool operator != (const cPlane3D<T>& pd) const {return !operator == (pd);}
	void SetPlane(const cVector3D<T>& nvect, T d) {m_vNormal = nvect; m_d = d;}
	void SetPlane(const cVector3D<T>& point, const cVector3D<T>& nvector) {m_vNormal = nvector; RecalculateD(point);}
	void SetPlane(const cVector3D<T>& point1, const cVector3D<T>& point2, const cVector3D<T>& point3);
	//根据法向量和平面上的一个点计算d
	void RecalculateD(const cVector3D<T>& MPoint) {m_d = - MPoint.DotProduct(m_vNormal);}
	//
	cVector3D<T> GetMemberPoint() const {return m_vNormal * ( - m_d );}

	//点与面的关系
	eIntersectionRelation3D ClassifyPointRelation(const cVector3D<T>& point) const;
	//线与面的关系
	bool GetIntersectionWithLine(const cVector3D<T>& linePoint, const cVector3D<T>& lineVect, cVector3D<T>& outIntersection) const;
	//
	float GetKnownIntersectionWithLine(const cVector3D<T>& linePoint1, const cVector3D<T>& linePoint2) const;
	//
	bool GetIntersectionWithLimitedLine(const cVector3D<T>& linePoint1, const cVector3D<T>& linePoint2, cVector3D<T>& outIntersection) const;
	
	//面与面是否相交
	bool ExistsIntersection(const cPlane3D<T>& other) const { cVector3D<T> cross = other.m_vNormal.CrossProduct(m_vNormal); return cross.GetLength() > c_fRounding_Error;}
	//面与面相交线
	bool GetIntersectionWithPlane(const cPlane3D<T>& other, cVector3D<T>& outLinePoint, cVector3D<T>& outLineVect) const;
	//3个面的交点
	bool GetIntersectionWithPlanes(const cPlane3D<T>& o1, const cPlane3D<T>& o2, cVector3D<T>& outPoint) const;
	//点到面的距离
	T GetDistanceTo(const cVector3D<T>& point) const {return point.DotProduct(m_vNormal) + m_d;}
	//看看是正面还是背面
	bool IsFrontFacing(const cVector3D<T>& lookDirection) const {const float d = m_vNormal.DotProduct(lookDirection); return d <= 0.f;}

	void GetNormalLine(cLine3D<T>& line, T len) const {line.SetLine(GetMemberPoint(), m_vNormal, len);}

	//获得另一个维度值nDimension=2值,只输入的是xz，需要获得第二维y的数值		add by yfw 
	T GetOtherDimenValue(const cVector2D<T>& vPoint, int nDimension=2);

	//add by yfw
	//按照法线方向移动
	bool MoveByNormal(float fDis);
	//add by yfw 
	bool IsEmpty() const { return m_vNormal.IsEmpty();}
	bool Set0(){m_vNormal.Set0(); return true;}

	cVector3D<T> m_vNormal;//平面法线
	T			 m_d;
};

template<class T>
inline void cPlane3D<T>::SetPlane(const cVector3D<T>& point1, const cVector3D<T>& point2, const cVector3D<T>& point3)
{
	m_vNormal = (point2 - point1).CrossProduct(point3 - point1);
	m_vNormal.Normalize();
	RecalculateD(point1);
}

template<class T>
inline eIntersectionRelation3D cPlane3D<T>::ClassifyPointRelation (const cVector3D<T>& point) const
{
	const T d = m_vNormal.DotProduct(point) + m_d;
	if (d < -c_fRounding_Error)
		return IR_Back;
	if (d > c_fRounding_Error)
		return IR_Front;
	return IR_Planer;
}

template<class T>
inline bool cPlane3D<T>::GetIntersectionWithLine(const cVector3D<T>& linePoint, const cVector3D<T>& lineVect, cVector3D<T>& outIntersection) const
{
	T t2 = m_vNormal.DotProduct(lineVect);
	if (t2 == 0)
		return false;
	T t = - (m_vNormal.DotProduct(linePoint) + m_d) / t2;
	outIntersection = linePoint + (lineVect * t);
	return true;
}

template<class T>
inline float cPlane3D<T>::GetKnownIntersectionWithLine(const cVector3D<T>& linePoint1, const cVector3D<T>& linePoint2) const
{
	cVector3D<T> vect = linePoint2 - linePoint1;
	T t2 = (float)m_vNormal.DotProduct(vect);
	return (float) - ( ( m_vNormal.DotProduct(linePoint1) + m_d ) / t2 );
}

template<class T>
inline bool cPlane3D<T>::GetIntersectionWithLimitedLine(const cVector3D<T>& linePoint1, const cVector3D<T>& linePoint2, cVector3D<T>& outIntersection) const
{
	return ( GetIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection) && outIntersection.IsBetweenPoints(linePoint1, linePoint2) );
}

template<class T>
inline bool cPlane3D<T>::GetIntersectionWithPlane(const cPlane3D<T>& other, cVector3D<T>& outLinePoint, cVector3D<T>& outLineVect) const
{
	const T fn00 = m_vNormal.GetLength();
	const T fn01 = m_vNormal.DotProduct(other.m_vNormal);
	const T fn11 = other.m_vNormal.GetLength();
	const double det = fn00 * fn11 - fn01 * fn01;

	if (CrossMath::Abs(det) < c_dRounding_Error )
		return false;

	const double invdet = 1.0 / det;
	const double fc0 = (fn11 * -m_d + fn01 * other.m_d) * invdet;
	const double fc1 = (fn00 * -other.m_d + fn01 * m_d) * invdet;

	outLineVect = m_vNormal.CrossProduct(other.m_vNormal);
	outLinePoint = m_vNormal * (T)fc0 + other.m_vNormal * (T)fc1;
	return true;
}

template<class T>
inline bool cPlane3D<T>::GetIntersectionWithPlanes(const cPlane3D<T>& o1, const cPlane3D<T>& o2, cVector3D<T>& outPoint) const
{
	cVector3D<T> linePoint, lineVect;
	if (GetIntersectionWithPlane(o1, linePoint, lineVect))
		return o2.GetIntersectionWithLine(linePoint, lineVect, outPoint);
	return false;
}

template<class T>
inline T cPlane3D<T>::GetOtherDimenValue(const cVector2D<T>& vPoint, int nDimension)
{
	switch(nDimension)
	{
	case 2:
		{
			if (Equals(m_vNormal[1], T(0)))
				return T(0);

			T therDimension = T((-m_d - (m_vNormal[0]*vPoint.x + m_vNormal[2]*vPoint.y)) / m_vNormal[1]);
			return therDimension;
		}
	default:
		{
			return T(0);
		}
	}
}

//按照法线方向移动
template<class T>
bool cPlane3D<T>::MoveByNormal(float fDis)
{
	if (Equals(0.0f, fDis))
		return true;

	m_d += fDis;			//m_d 记录的是和真正距离，对应的负数数值
	return true;
}

typedef cPlane3D<float>			cPlane3Df;
typedef cPlane3D<long>			cPlane3Dl;

}

typedef cm::cPlane3Df		plane3d;

#endif
