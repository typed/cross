#ifndef _common_aabbox_h_
#define _common_aabbox_h_

#include "../CommonDef.h"
#include "MathBase.h"
#include "cVector3D.h"
#include "cLine3D.h"
#include "cPlane3D.h"

namespace cm
{

template<class T>
class cAabbox
{
public:
	cAabbox() {Reset();}
	cAabbox(const cVector3D<T>& init) : m_min(init), m_max(init) {}
	cAabbox(const cVector3D<T>& mi, const cVector3D<T>& ma) : m_min(mi), m_max(ma) {}
	cAabbox(const cAabbox& aabb) {*this = aabb;}
	cAabbox(T mi_x, T mi_y, T mi_z, T ma_x, T ma_y, T ma_z) : m_min(mi_x, mi_y, mi_z), m_max(ma_x, ma_y, ma_z) {}
	cAabbox& operator = (const cAabbox& aabb) {Reset(aabb); return *this;}
	bool operator == (const cAabbox& aabb) const {return m_min == aabb.m_min && m_max == aabb.m_max;}
	bool operator != (const cAabbox& aabb) const {return !operator == (aabb);}
	const cVector3D<T>& GetMin() const {return m_min;}
	const cVector3D<T>& GetMax() const {return m_max;}
	void Reset(T x, T y, T z) {Reset(cVector3D<T>(x, y, z));}
	void Reset(const cVector3D<T>& init) {m_min = m_max = init;}
	void Reset(const cVector3D<T>& mi, const cVector3D<T>& ma) {m_min = mi; m_max = ma;}
	void Reset(const cAabbox& aabb) {m_min = aabb.m_min; m_max = aabb.m_max;}
	void Reset() { m_min.Set(999999.9f, 999999.9f, 999999.9f); m_max.Set(-999999.9f, -999999.9f, -999999.9f); }
	bool IsEmpty() const {return m_min >= m_max;}
	void Repair();

	void AddInternalPoint(T x, T y, T z);
	void AddInternalPoint(const cVector3D<T>& pt) {AddInternalPoint(pt.x, pt.y, pt.z);}
	void AddInternalBox(const cAabbox<T>& box) {AddInternalPoint(box.m_min); AddInternalPoint(box.m_max);}
	
	bool IsPointInside(const cVector3D<T>& p) const {return (p.x >= m_min.x && p.x <= m_max.x && p.y >= m_min.y && p.y <= m_max.y && p.z >= m_min.z && p.z <= m_max.z);}
	bool IsPointTotalInside(const cVector3D<T>& p) const {return (p.x > m_min.x && p.x < m_max.x && p.y > m_min.y && p.y < m_max.y && p.Z > m_min.z && p.z < m_max.z);}
	bool IsFullInside(const cAabbox<T>& other) {return m_min >= other.m_min && m_max <= other.m_max;}
	//检测与线段相交
	bool IntersectsWithLine(const cLine3D<T>& line) const {return IntersectsWithLine(line.GetMiddle(), line.GetVector().Normalize(), (T)(line.GetLength() * 0.5));}
	bool IntersectsWithLine(const cVector3D<T>& linemiddle, const cVector3D<T>& linevect, T halflength) const;

	//add by yfw
	bool IntersectsWithRay(const cVector3D<T>& vIni, const cVector3D<T>& vDir, T fMaxHeight=(T)1000) const;

	//检测与射线相交
	f32 IntersectsWithRay2(const cVector3Df& rayOrg, const cVector3Df& rayDelta, cVector3D<T>* returnNormal) const;
	bool Intersects(const cVector3Df& origin, const cVector3Df& direction, f32& dist) const;
	//与面的关系
	eIntersectionRelation3D ClassifyPlaneRelation(const cPlane3D<T>& plane) const;
	//检测与box相交
	bool IntersectsWithBox(const cAabbox<T>& other) const {return (m_min <= other.m_max && m_max >= other.m_min);}
	bool GetIntersectsWithBox(cAabbox<T>& boxOut, const cAabbox<T>& boxIn) const;
	// 插值，Calculates a new interpolated bounding box.
	cAabbox<T> GetInterpolated(const cAabbox<T>& other, f32 d) const;
	//获取8个顶点
	void GetEdges(cVector3D<T>* edges) const;
	void GetEdgesIndex(ui16* edgesindex) const;
	//获取12条线
	void GetLines(cLine3D<T>* lines) const;

	T GetVolume() const {const cVector3D<T> e = GetExtent(); return e.x * e.y * e.z;}
	T GetArea() const {const cVector3D<T> e = GetExtent(); return 2 * ( e.x * e.y + e.x * e.z + e.y * e.z );}
	cVector3D<T> GetCenter() const {return (m_min + m_max) / 2;}
	cVector3D<T> GetExtent() const {return m_max - m_min;}

	cVector3D<T> m_min;
	cVector3D<T> m_max;
};

template<class T>
inline void cAabbox<T>::Repair()
{
	T t;
	if (m_min.x > m_max.x) {
		t = m_min.x;
		m_min.x = m_max.x;
		m_max.x = t;
	}
	if (m_min.y > m_max.y) {
		t = m_min.y;
		m_min.y = m_max.y;
		m_max.y = t;
	}
	if (m_min.z > m_max.z) {
		t = m_min.z;
		m_min.z = m_max.z;
		m_max.z = t;
	}
}

template<class T>
inline void cAabbox<T>::AddInternalPoint(T x, T y, T z)
{
	if ( x > m_max.x)
		m_max.x = x;
	if ( y > m_max.y)
		m_max.y = y;
	if ( z > m_max.z)
		m_max.z = z;
	if ( x < m_min.x)
		m_min.x = x;
	if ( y < m_min.y)
		m_min.y = y;
	if ( z < m_min.z)
		m_min.z = z;
}

template<class T>
inline bool cAabbox<T>::IntersectsWithLine(const cVector3D<T>& linemiddle, const cVector3D<T>& linevect, T halflength) const
{
	const cVector3D<T> e = GetExtent() * (T)0.5;
	const cVector3D<T> t = GetCenter() - linemiddle;

	if ((CrossMath::Abs(t.x) > e.x + halflength * CrossMath::Abs(linevect.x)) ||
		(CrossMath::Abs(t.y) > e.y + halflength * CrossMath::Abs(linevect.y)) ||
		(CrossMath::Abs(t.z) > e.z + halflength * CrossMath::Abs(linevect.z)) )
		return false;

	T r = e.y * (T)CrossMath::Abs(linevect.z) + e.z * (T)CrossMath::Abs(linevect.y);
	if (CrossMath::Abs(t.y * linevect.z - t.z *linevect.y) > r )
		return false;

	r = e.x * (T)CrossMath::Abs(linevect.z) + e.z * (T)CrossMath::Abs(linevect.x);
	if (CrossMath::Abs(t.z * linevect.x - t.x * linevect.z) > r )
		return false;

	r = e.x * (T)CrossMath::Abs(linevect.y) + e.y * (T)CrossMath::Abs(linevect.x);
	if (CrossMath::Abs(t.x * linevect.y - t.y * linevect.x) > r)
		return false;

	return true;
}

template<class T>
inline eIntersectionRelation3D cAabbox<T>::ClassifyPlaneRelation(const cPlane3D<T>& plane) const
{
	cVector3D<T> nearPoint(m_max);
	cVector3D<T> farPoint(m_min);

	if (plane.m_vNormal.x > (T)0)
	{
		nearPoint.x = m_min.x;
		farPoint.x = m_max.x;
	}

	if (plane.m_vNormal.y > (T)0)
	{
		nearPoint.y = m_min.y;
		farPoint.y = m_max.y;
	}

	if (plane.m_vNormal.z > (T)0)
	{
		nearPoint.z = m_min.z;
		farPoint.z = m_max.z;
	}

	if (plane.m_vNormal.DotProduct(nearPoint) + plane.m_d > (T)0)
		return IR_Front;

	if (plane.m_vNormal.DotProduct(farPoint) + plane.m_d > (T)0)
		return IR_Clipped;

	return IR_Back;
}

template<class T>
inline bool cAabbox<T>::GetIntersectsWithBox(cAabbox<T>& boxOut, const cAabbox<T>& boxIn) const
{
	//不相交
	if (!IntersectsWithBox(boxIn))
		return false;
	boxOut.m_min.x = Max(m_min.x, boxIn.m_min.x);
	boxOut.m_max.x = Min(m_max.x, boxIn.m_max.x);
	boxOut.m_min.y = Max(m_min.y, boxIn.m_min.y);
	boxOut.m_max.y = Min(m_max.y, boxIn.m_max.y);
	boxOut.m_min.z = Max(m_min.z, boxIn.m_min.z);
	boxOut.m_max.z = Min(m_max.z, boxIn.m_max.z);
	return true;
}

template<class T>
inline cAabbox<T> cAabbox<T>::GetInterpolated(const cAabbox<T>& other, f32 d) const
{
	f32 inv = 1.0f - d;
	return cAabbox<T>( ( other.m_min * inv ) + ( m_min * d ), ( other.m_max * inv ) + ( m_max * d ) );
}

template<class T>
inline void cAabbox<T>::GetEdges(cVector3D<T>* edges) const
{
	const cVector3D<T> middle = GetCenter();
	const cVector3D<T> diag = middle - m_max;
	/*
	Edges are stored in this way:
	Hey, am I an ascii artist, or what? :) niko.
             /3--------/7
            /  |      / |
           /   |     /  |
           1---------5  |
           |   2- - -| -6
           |  /      |  /
           |/        | /
           0---------4/
	*/
	edges[0].Set(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
	edges[1].Set(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
	edges[2].Set(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
	edges[3].Set(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
	edges[4].Set(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
	edges[5].Set(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
	edges[6].Set(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
	edges[7].Set(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
}

template<class T>
inline void cAabbox<T>::GetEdgesIndex(ui16* edgesindex) const
{
	ui16 indexes[] = {
		2, 3, 1, 2, 1, 0,
		0, 1, 5, 0, 5, 4,
		4, 5, 7, 4, 7, 6,
		6, 7, 3, 6, 3, 2,
		1, 3, 7, 1, 7, 5,
		2, 0, 4, 2, 4, 6
	};
	i32 num = sizeof(indexes)/sizeof(ui16);
	for (i32 i = 0; i < num; i++) 
		edgesindex[i] = indexes[i];
}

template<class T>
inline void cAabbox<T>::GetLines(cLine3D<T>* lines) const
{
	cVector3D<T> vs[8];
	GetEdges(vs);
	lines[0].from = vs[0];
	lines[0].to = vs[1];
	lines[1].from = vs[1];
	lines[1].to = vs[5];
	lines[2].from = vs[5];
	lines[2].to = vs[4];
	lines[3].from = vs[4];
	lines[3].to = vs[0];

	lines[4].from = vs[1];
	lines[4].to = vs[3];
	lines[5].from = vs[0];
	lines[5].to = vs[2];
	lines[6].from = vs[4];
	lines[6].to = vs[6];
	lines[7].from = vs[5];
	lines[7].to = vs[7];

	lines[8].from = vs[2];
	lines[8].to = vs[3];
	lines[9].from = vs[3];
	lines[9].to = vs[7];
	lines[10].from = vs[7];
	lines[10].to = vs[6];
	lines[11].from = vs[6];
	lines[11].to = vs[2];
}
template<class T>
inline bool cAabbox<T>::IntersectsWithRay(const cVector3D<T>& vIni, const cVector3D<T>& vDir, T fMaxHeight) const
{
	if (vDir.IsEmpty())
		return false;
	cVector3D<T> vTemp = vDir;
	vTemp.Normalize();
	T fHalfLenght = fMaxHeight / (T)2;
	cVector3D<T> vMiddle = vIni + vTemp*fHalfLenght;
	return IntersectsWithLine(vMiddle, vTemp, fHalfLenght);
}

template<class T>
inline bool cAabbox<T>::Intersects(const cVector3Df& origin, const cVector3Df& direction, f32& dist) const
{
	f32 lowt = 0.0f;
	f32 t;
	bool hit = false;
	cVector3Df hitpoint;

	// 如果射线出发点在包围盒的内部，则必定相交

	//现在认为在包围盒内部,无法pick到
	if (( origin.x > m_min.x && origin.x < m_max.x )
		&& ( origin.y > m_min.y && origin.y < m_max.y )
		&&( origin.z > m_min.z && origin.z < m_max.z ))
	{
		return false;
	}

	//分别判断与包围盒各平面的相交情况
	//1. 与x = m_min.x面的相交的可能性:射线起点在该面左侧，而且direction.x > 0,如下图所示：
		
	/*
		
							m_max
					/3--------/7
					/  |      / |
(direction)			/   |     /  |
		。		1---------5  |
		。		|   2- - -| -6
		。			|  /      |  /
(origin)。			|/        | /
				0---------4/ 
				m_min
	*/


	if (origin.x <= m_min.x && direction.x > 0)
	{
		t = (m_min.x - origin.x) / direction.x;
		if (t >= 0)
		{
			//计算出相交点，判断交点是否在包围盒上
			hitpoint = origin + direction * t;
			if (hitpoint.y >= m_min.y && hitpoint.y <= m_max.y &&
				hitpoint.z >= m_min.z && hitpoint.z <= m_max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	//2.与x = m_max.x面的相交的可能性:射线起点在该面右侧，而且direction.x < 0
	if (origin.x >= m_max.x && direction.x < 0)
	{
		t = (m_max.x - origin.x) / direction.x;
		if (t >= 0)
		{
			hitpoint = origin + direction * t;
			if (hitpoint.y >= m_min.y && hitpoint.y <= m_max.y &&
				hitpoint.z >= m_min.z && hitpoint.z <= m_max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	//3.与y = m_min.y面的相交的可能性:射线起点在该面左侧，而且direction.y > 0
	if (origin.y <= m_min.y && direction.y > 0)
	{
		t = (m_min.y - origin.y) / direction.y;
		if (t >= 0)
		{
			hitpoint = origin + direction * t;
			if (hitpoint.x >= m_min.x && hitpoint.x <= m_max.x &&
				hitpoint.z >= m_min.z && hitpoint.z <= m_max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	//4.与y = m_max.y面的相交的可能性:射线起点在该面右侧，而且direction.y <0
	if (origin.y >= m_max.y && direction.y < 0)
	{
		t = (m_max.y - origin.y) / direction.y;
		if (t >= 0)
		{
			hitpoint = origin + direction * t;
			if (hitpoint.x >= m_min.x && hitpoint.x <= m_max.x &&
				hitpoint.z >= m_min.z && hitpoint.z <= m_max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	//5.与z = m_min.z面的相交的可能性:射线起点在该面左侧，而且direction.z > 0
	if (origin.z <= m_min.z && direction.z > 0)
	{
		t = (m_min.z - origin.z) / direction.z;
		if (t >= 0)
		{
			hitpoint = origin + direction * t;
			if (hitpoint.x >= m_min.x && hitpoint.x <= m_max.x &&
				hitpoint.y >= m_min.y && hitpoint.y <= m_max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	//6.与z = m_max.z面的相交的可能性:射线起点在该面右侧，而且direction.z < 0
	if (origin.z >= m_max.z && direction.z < 0)
	{
		t = (m_max.z - origin.z) / direction.z;
		if (t >= 0)
		{
			hitpoint = origin + direction * t;
			if (hitpoint.x >= m_min.x && hitpoint.x <= m_max.x &&
				hitpoint.y >= m_min.y && hitpoint.y <= m_max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	dist = lowt;
	return hit;
}

template<class T>
inline f32 cAabbox<T>::IntersectsWithRay2(const cVector3Df& rayOrg, const cVector3Df& rayDelta, cVector3D<T>* returnNormal) const
{

	// We'll return this huge number if no intersection
	const f32 kNoIntersection = 1e30f;

	// Check for point inside box, trivial reject, and determine parametric distance to each front face.
	bool inside = true;

	f32 xt, xn;

	if (rayOrg.x < m_min.x) 
	{
		xt = m_min.x - rayOrg.x;
		if (xt > rayDelta.x) 
			return kNoIntersection;

		xt /= rayDelta.x;
		inside = false;
		xn = -1.0f;
	} 
	else if (rayOrg.x > m_max.x) 
	{
		xt = m_max.x - rayOrg.x;
		if (xt < rayDelta.x) 
			return kNoIntersection;

		xt /= rayDelta.x;
		inside = false;
		xn = 1.0f;
	} 
	else
		xt = -1.0f;    

	f32 yt, yn;

	if (rayOrg.y < m_min.y) 
	{
		yt = m_min.y - rayOrg.y;
		if (yt > rayDelta.y) 
			return kNoIntersection;

		yt /= rayDelta.y;
		inside = false;
		yn = -1.0f;
	} 
	else if (rayOrg.y > m_max.y) 
	{
		yt = m_max.y - rayOrg.y;
		if (yt < rayDelta.y) 
			return kNoIntersection;

		yt /= rayDelta.y;
		inside = false;
		yn = 1.0f;
	} 
	else 
		yt = -1.0f;    

	f32 zt, zn;

	if (rayOrg.z < m_min.z) 
	{
		zt = m_min.z - rayOrg.z;
		if (zt > rayDelta.z) 
			return kNoIntersection;

		zt /= rayDelta.z;
		inside = false;
		zn = -1.0f;
	} 
	else if (rayOrg.z > m_max.z) 
	{
		zt = m_max.z - rayOrg.z;
		if (zt < rayDelta.z) 
			return kNoIntersection;

		zt /= rayDelta.z;
		inside = false;
		zn = 1.0f;
	} 
	else 
		zt = -1.0f;    

	// Inside box?

	if (inside) 
	{
		//if (returnNormal != 0) 
		//{
		//	*returnNormal = -rayDelta;
		//	returnNormal->Normalize();
		//}

		return 0.0f;
	}

	// Select farthest plane - this is
	// the plane of intersection.

	int which = 0;
	f32 t = xt;

	if (yt > t) 
	{
		which = 1;
		t = yt;
	}

	if (zt > t) 
	{
		which = 2;
		t = zt;
	}

	switch (which) 
	{
	case 0: // intersect with yz plane
		{
			f32 y = rayOrg.y + rayDelta.y * t;

			if (y < m_min.y || y > m_max.y) 
				return kNoIntersection;

			f32 z = rayOrg.z + rayDelta.z * t;
			if (z < m_min.z || z > m_max.z) 
				return kNoIntersection;

			if (returnNormal != 0) 
			{
				returnNormal->x = xn;
				returnNormal->y = 0.0f;
				returnNormal->z = 0.0f;
			}
		} 
		break;

	case 1: // intersect with xz plane
		{
			f32 x = rayOrg.x + rayDelta.x * t;
			if (x < m_min.x || x > m_max.x) 
				return kNoIntersection;

			f32 z = rayOrg.z + rayDelta.z * t;
			if (z < m_min.z || z > m_max.z) 
				return kNoIntersection;

			if (returnNormal != 0) 
			{
				returnNormal->x = 0.0f;
				returnNormal->y = yn;
				returnNormal->z = 0.0f;
			}

		} 
		break;

	case 2: // intersect with xy plane
		{
			f32 x = rayOrg.x + rayDelta.x * t;
			if (x < m_min.x || x > m_max.x) 
				return kNoIntersection;

			f32 y = rayOrg.y + rayDelta.y * t;
			if (y < m_min.y || y > m_max.y) 
				return kNoIntersection;

			if (returnNormal != 0) 
			{
				returnNormal->x = 0.0f;                                
				returnNormal->y = 0.0f;
				returnNormal->z = zn;
			}
		} 
		break;
	}

	// Return parametric point of intersection
	return t;
}

typedef cAabbox<f32>		cAabboxf;
typedef cAabbox<long>		cAabboxl;

}

typedef cm::cAabboxf	aabb;

#endif
