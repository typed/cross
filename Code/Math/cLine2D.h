#ifndef _cross_math_cline2d_h
#define _cross_math_cline2d_h

#include "Config.h"
#include "MathBase.h"
#include "cVector2D.h"

namespace cross
{

class cLine2D
{
public:
	cLine2D() {}
	cLine2D(const cPoint& ptFrom, const cPoint& ptTo) { from = ptFrom; to = ptTo; }
	cLine2D(const cLine2D& other) { *this = other; }
	cLine2D& operator = (const cLine2D& other) { from = other.from; to = other.to; return *this; }
	void Set0() { from.Set0(); to.Set0(); }
	bool operator == (const cLine2D& other) const { return from == other.from && to == other.to; }
	bool operator != (const cLine2D& other) const { return !operator == (other); }
	long GetLength() const { return (to - from).GetLength(); }
	cPoint GetCenter() const { return (from + to) / 2; }
	bool IsEmpty() const { return from == to; }
	bool GetSlope(float& fSlope) const { if (from.x == to.x) { return false; } fSlope = (float)( from.y - to.y ) / ( from.x - to.x ); return true; }
	enum ePointInLine {
		PIL_Left,
		PIL_Right,
		PIL_In,
		PIL_InExNearFrom,
		PIL_InExNearTo
	};
	ePointInLine GetPointInLine(const cPoint& v) const;
	//判断两 线段 位置关系
	enum ePositionRelation {
		Cross,		//相交
		Coincide,	//重合
		NotCross,	//不相交
		Parallel,	//平行
	};
	ePositionRelation GetPositionRelation(const cLine2D& ln, cPoint& ptCross) const;
	cPoint	from;
	cPoint	to;
};

inline cLine2D::ePointInLine cLine2D::GetPointInLine(const cPoint& v) const
{
	long r = (from.x - to.x) * (v.y - to.y) - (from.y - to.y) * (v.x - to.x);
	if ( r > 0 )
		return PIL_Right;
	else if (r == 0) {
		if (from.x > to.x) {
			if (v.x < to.x)
				return PIL_InExNearTo;
			else if (v.x > from.x)
				return PIL_InExNearFrom;
			else
				return PIL_In;
		}
		else if (from.x < to.x) {
			if (v.x < from.x)
				return PIL_InExNearFrom;
			else if (v.x > to.x)
				return PIL_InExNearTo;
			else
				return PIL_In;
		}
		else {
			if (from.y > to.y) {
				if (v.y < to.y)
					return PIL_InExNearTo;
				else if (v.y > from.y)
					return PIL_InExNearFrom;
				else
					return PIL_In;
			}
			else if (from.y < to.y) {
				if (v.y < from.y)
					return PIL_InExNearFrom;
				else if (v.y > to.y)
					return PIL_InExNearTo;
				else
					return PIL_In;
			}
			else
				return PIL_In;
		}
	}
	else
		return PIL_Left;
}

inline cLine2D::ePositionRelation cLine2D::GetPositionRelation(const cLine2D& ln, cPoint& ptCross) const
{
	ePointInLine eFrom1 = GetPointInLine(ln.from);
	ePointInLine eTo1 = GetPointInLine(ln.to);
	ePointInLine eFrom2 = ln.GetPointInLine(from);
	ePointInLine eTo2 = ln.GetPointInLine(to);
	if ( ( eFrom1 == PIL_Left && eTo1 == PIL_Left ) || 
		( eFrom1 == PIL_Right && eTo1 == PIL_Right ) || 
		( eFrom2 == PIL_Left && eTo2 == PIL_Left ) || 
		( eFrom2 == PIL_Right && eTo2 == PIL_Right ) )
	{
		float fSlope1, fSlope2;
		bool bX1 = GetSlope(fSlope1);
		bool bX2 = GetSlope(fSlope2);
		if ( !bX1 && !bX2 )
			return Parallel;
		if ( bX1 == bX2 && Equals(fSlope1, fSlope2) )
			return Parallel;
		else
			return NotCross;
	}
	else if ( ( eFrom1 == PIL_InExNearFrom && eTo1 == PIL_InExNearFrom ) || 
		( eFrom1 == PIL_InExNearTo && eTo1 == PIL_InExNearTo ) || 
		( eFrom2 == PIL_InExNearFrom && eTo2 == PIL_InExNearFrom ) || 
		( eFrom2 == PIL_InExNearTo && eTo2 == PIL_InExNearTo ) )
	{
		return NotCross;
	}
	else if (eFrom1 == PIL_In || eTo1 == PIL_In || eFrom2 == PIL_In || eTo2 == PIL_In)
	{
		return Coincide;
	}
	else
	{
		//求x坐标
		i32 tempLeft = (ln.to.x - ln.from.x) * (from.y - to.y) - (to.x - from.x) * (ln.from.y - ln.to.y);
		i32 tempRight = (from.y - ln.from.y) * (to.x - from.x) * (ln.to.x - ln.from.x) + ln.from.x * (ln.to.y - ln.from.y) * (to.x - from.x) - from.x * (to.y - from.y) * (ln.to.x - ln.from.x);
		ptCross.x = (i32)( (f32)tempRight / (f32)tempLeft );
		//求y坐标
		tempLeft = (from.x - to.x) * (ln.to.y - ln.from.y) - (to.y - from.y) * (ln.from.x - ln.to.x);
		tempRight = to.y * (from.x - to.x) * (ln.to.y - ln.from.y) + (ln.to.x - to.x) * (ln.to.y - ln.from.y) * (from.y - to.y) - ln.to.y * (ln.from.x - ln.to.x) * (to.y - from.y);
		ptCross.y = (i32)( (f32)tempRight / (f32)tempLeft );
		return Cross;
	}
}

}
#endif
