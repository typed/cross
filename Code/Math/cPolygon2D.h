#ifndef _cross_math_cpolygon2d_h
#define _cross_math_cpolygon2d_h

#include "Config.h"
#include "MathBase.h"
#include "cVector2D.h"
#include "cvector.h"

namespace cross
{

typedef cvector< cPoint > aPoint;

class cPolygon2D
{
public:
	void AddPoint(const cPoint& pt);
	bool Invalidate() const;
	//只适合凸多边形
	bool IsIn_Convex(const cPoint& pt) const;
private:
	aPoint	m_aPoint;
};

inline void cPolygon2D::AddPoint(const cPoint& pt)
{
	m_aPoint.push_back(pt);
}

inline bool cPolygon2D::Invalidate() const
{
	return m_aPoint.size() < 3;
}

//只适合凸多边形
inline bool cPolygon2D::IsIn_Convex(const cPoint& pt) const
{
	if (Invalidate())
		return false;
	bool bIn = true;
	for (ui32 i = 1; i < m_aPoint.size(); i++) {
		bIn = bIn && (PointInLine(m_aPoint[i - 1], m_aPoint[i], pt) >= 0);
		if (!bIn)
			return false;
	}
	return bIn && (PointInLine(m_aPoint[i - 1], m_aPoint[0], pt) >= 0);
}

}
#endif
