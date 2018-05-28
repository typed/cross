#ifndef _cross_math_rect_h_
#define _cross_math_rect_h_

#include "Config.h"
#include "MathBase.h"
#include "cVector2D.h"
#include "cLine2D.h"

BEGIN_COMMON

enum eDirRect
{
	DR_TopLeft,
	DR_TopRight,
	DR_BottomLeft,
	DR_BottomRight,
	DR_Num
};

enum eDirLine
{
	DL_Left,
	DL_Top,
	DL_Right,
	DL_Bottom
};

template<typename T>
class cRectTemplate
{
public:
	
	cRectTemplate() { Set0(); };

	cRectTemplate(T l, T t, T r, T b) {
		SetData1(l, t, r, b);
	}

	cRectTemplate(T x, T y, const cSizeTemplate<T>& sz) {
		SetData2(x, y, sz);
	}

	cRectTemplate(const cPointTemplate<T>& pt, const cSizeTemplate<T>& sz) {
		SetData2(pt, sz);
	}

	cRectTemplate(T width, T height) {
		SetData2(0, 0, width, height);
	}

	cRectTemplate(const cSizeTemplate<T> sz) {
		Set0();
		SetSize(sz);
	}

	cRectTemplate(const cRectTemplate<T>& other) { *this = other; }

	void SetData1(T l, T t, T r, T b) {
		left = l; top = t; right = r; bottom = b;
	}

	void SetData2(T x, T y, T w, T h) {
		left = x; top = y; right = left + w; bottom = top + h;
	}

	void SetData2(T x, T y, const cSizeTemplate<T>& sz) {
		left = x; top = y; right = left + sz.x; bottom = top + sz.y;
	}

	void SetData2(const cPointTemplate<T>& pt, const cSizeTemplate<T>& sz) {
		left = pt.x; top = pt.y; right = left + sz.x; bottom = top + sz.y;
	}

	cRectTemplate<T>& operator = (const cRectTemplate<T>& other) {
		SetData1(other.left, other.top, other.right, other.bottom);
		return *this;
	}

	void Set0() { left = top = right = bottom = 0; }

	bool operator == (const cRectTemplate<T>& other) const {
		return ( Equals(left, other.left) && 
			Equals(top, other.top) && 
			Equals(right, other.right) &&
			Equals(bottom, other.bottom) );
	}

	bool operator != (const cRectTemplate<T>& other) const { return !operator == (other);}

	cRectTemplate<T> operator + (const cRectTemplate<T>& other) {
		cRectTemplate<T> temp;
		temp.left = left + other.left;
		temp.top = top + other.top;
		temp.right = right + other.right;
		temp.bottom = bottom + other.bottom;
		return temp;
	}

	cRectTemplate<T> operator - (const cRectTemplate<T>& other) {
		cRectTemplate<T> temp;
		temp.left = left - other.left;
		temp.top = top - other.top;
		temp.right = right - other.right;
		temp.bottom = bottom - other.bottom;
		return temp;
	}

	cRectTemplate<T> operator / (const cRectTemplate<T>& other) {
		cRectTemplate<T> temp;
		temp.left = left / other.left;
		temp.top = top / other.top;
		temp.right = right / other.right;
		temp.bottom = bottom / other.bottom;
		return temp;
	}

	cRectTemplate<T> operator * (const cRectTemplate<T>& other) {
		cRectTemplate<T> temp;
		temp.left = left * other.left;
		temp.top = top * other.top;
		temp.right = right * other.right;
		temp.bottom = bottom * other.bottom;
		return temp;
	}

	cRectTemplate<T>& operator += (const cRectTemplate<T>& other) {
		left += other.left;
		top += other.top;
		right += other.right;
		bottom += other.bottom;
		return *this;
	}

	cRectTemplate<T>& operator -= (const cRectTemplate<T>& other) {
		left -= other.left;
		top -= other.top;
		right -= other.right;
		bottom -= other.bottom;
		return *this;
	}

	cRectTemplate<T>& operator /= (const cRectTemplate<T>& other) {
		left /= other.left;
		top /= other.top;
		right /= other.right;
		bottom /= other.bottom;
		return *this;
	}

	cRectTemplate<T>& operator *= (const cRectTemplate<T>& other) {
		left *= other.left;
		top *= other.top;
		right *= other.right;
		bottom *= other.bottom;
		return *this;
	}

	cRectTemplate<T>& operator += (const T& num) {
		left += num;
		top += num;
		right += num;
		bottom += num;
		return *this;
	}

	cRectTemplate<T>& operator -= (const T& num) {
		left -= num;
		top -= num;
		right -= num;
		bottom -= num;
		return *this;
	}

	cRectTemplate<T>& operator /= (const T& num) {
		left /= num;
		top /= num;
		right /= num;
		bottom /= num;
		return *this;
	}

	cRectTemplate<T>& operator *= (const T& num) {
		left *= num;
		top *= num;
		right *= num;
		bottom *= num;
		return *this;
	}

	cRectTemplate<T> operator + (const T& num) {
		cRectTemplate<T> temp;
		temp.left = left + num;
		temp.top = top + num;
		temp.right = right + num;
		temp.bottom = bottom + num;
		return temp;
	}

	cRectTemplate<T> operator - (const T& num) {
		cRectTemplate<T> temp;
		temp.left = left - num;
		temp.top = top - num;
		temp.right = right - num;
		temp.bottom = bottom - num;
		return temp;
	}

	cRectTemplate<T> operator / (const T& num) {
		cRectTemplate<T> temp;
		temp.left = left / num;
		temp.top = top / num;
		temp.right = right / num;
		temp.bottom = bottom / num;
		return temp;
	}

	cRectTemplate<T> operator * (const T& num) {
		cRectTemplate<T> temp;
		temp.left = left * num;
		temp.top = top * num;
		temp.right = right * num;
		temp.bottom = bottom * num;
		return temp;
	}

	void SetLeft(T t) { left = t; }
	T GetLeft() const { return left; }
	
	void SetTop(T t) { top = t; }
	T GetTop() const { return top; }
	
	void SetRight(T t) { right = t; }
	T GetRight() const { return right; }
	
	void SetBottom(T t) { bottom = t; }
	T GetBottom() const { return bottom; }

	void SetWidth(T w) { right = left + w; }
	T GetWidth() const { return right - left; }

	void SetHeight(T h) { bottom = top + h; }
	T GetHeight() const { return bottom - top; }

	void SetPoint(const cPointTemplate<T>& pt) { MoveSelfTo(pt.x, pt.y); }
	cPointTemplate<T> GetPoint() const { return cPointTemplate<T>(left, top); }

	void SetSize(const cSizeTemplate<T>& sz) { right = left + sz.x; bottom = top + sz.y; }
	cSizeTemplate<T> GetSize() const { return cSizeTemplate<T>(GetWidth(), GetHeight()); }

	void MoveSelfTo(T x, T y) {
		right = x + GetWidth(); bottom = y + GetHeight(); left = x; top = y;
	}
	void MoveSelf(T offsetx, T offsety) { 
		MoveSelfTo(left + offsetx, top + offsety);
	}
	cRectTemplate<T> MoveTo(T x, T y) const {
		cRectTemplate<T> rc;
		rc.right = x + GetWidth();
		rc.bottom = y + GetHeight();
		rc.left = x;
		rc.top = y;
		return rc;
	}
	cRectTemplate<T> Move(T offsetx, T offsety) const {
		cRectTemplate<T> rc;
		rc = MoveTo( left + offsetx, top + offsety);
		return rc;
	}

	//取交集
	cRectTemplate<T> GetIntersection(const cRectTemplate<T>& rect) const {
		cRectTemplate<T> temp;
		if ((right > rect.left) && (left < rect.right) && 
			(bottom > rect.top) && (top < rect.bottom)) {
			temp.left = (left > rect.left) ? left : rect.left;
			temp.right = (right < rect.right) ? right : rect.right;
			temp.top = (top > rect.top) ? top : rect.top;
			temp.bottom = (bottom < rect.bottom) ? bottom : rect.bottom;
			return temp;
		}
		else
			return temp;
	}

	//取并集
	cRectTemplate<T> Union(const cRectTemplate<T>& rect) const {
		cRectTemplate<T> temp;
		if (IsEmpty())
			temp = rect;
		else {
			temp.left = ( left < rect.left ? left : rect.left );
			temp.top = ( top < rect.top ? top : rect.top );
			temp.right = ( right > rect.right ? right : rect.right );
			temp.bottom = ( bottom > rect.bottom ? bottom : rect.bottom );
		}
		return temp;
	}
	void UnionSelf(const cRectTemplate<T>& rect) {
		if (IsEmpty())
			*this = rect;
		else {
			left = ( left < rect.left ? left : rect.left );
			top = ( top < rect.top ? top : rect.top );
			right = ( right > rect.right ? right : rect.right );
			bottom = ( bottom > rect.bottom ? bottom : rect.bottom );
		}
	}
	void UnionPointSelf(const cPointTemplate<T>& pt, bool bFirst = false) {
		cRectTemplate<T> rect(pt.x, pt.y, pt.x, pt.y);
		if (bFirst)
			*this = rect;
		else {
			left = ( left < rect.left ? left : rect.left );
			top = ( top < rect.top ? top : rect.top );
			right = ( right > rect.right ? right : rect.right );
			bottom = ( bottom > rect.bottom ? bottom : rect.bottom );
		}
	}

	//效率不高
	bool IsIn(const cRectTemplate<T>& m) const {
		return GetIntersection(m) == m;
	}
	bool IsIn(T x, T y) const {
		return ( x >= left && x <= right && y >= top && y <= bottom );
	}
	bool IsIn(const cPointTemplate<T>& pt) const {
		return IsIn(pt.x, pt.y);
	}
	cPointTemplate<T> GetIn(const cPointTemplate<T>& pt) const {
		cPointTemplate<T> ptOut = pt;
		if (ptOut.x < left)
			ptOut.x = left;
		if (ptOut.x > right)
			ptOut.x = right;
		if (ptOut.y < top)
			ptOut.y = top;
		if (ptOut.y > bottom)
			ptOut.y = bottom;
		return ptOut;
	}

	bool IsEmpty() const {
		return right - left <= 0 || bottom - top <= 0;
	}

	//扩展
	void Inflate(const cSizeTemplate<T> sz) {
		Inflate(sz.x/2, sz.x/2, sz.y/2, sz.y/2);
	}
	void Inflate(T cx, T cy) {
		Inflate(cx/2, cx/2, cy/2, cy/2);
	}
	void Inflate(T l, T r, T t, T b) {
		left -= l; right += r; top -= t; bottom += b;
	}

	//是否相交
	bool IsIntersectant(const cRectTemplate<T>& m) const {
		return ( (right > m.left) && (left < m.right) && 
			(bottom > m.top) && (top < m.bottom) );
	}

	//设置中心点
	void SetCenter(const cVector2D<T>& vcenter, T sz) {
		SetCenter(vcenter, sz, sz, sz, sz);
	}
	void SetCenter(const cVector2D<T>& vcenter, const cVector2D<T>& sz) {
		SetCenter(vcenter, sz.x / 2, sz.x - sz.x / 2, sz.y / 2, sz.y - sz.y / 2);
	}
	void SetCenter(const cVector2D<T>& vcenter, T cx, T cy) {
		SetCenter(vcenter, cx, cx, cy, cy);
	}
	void SetCenter(const cVector2D<T>& vcenter, T cx1, T cx2, T cy1, T cy2) {
		left = vcenter.x - cx1;
		top = vcenter.y - cy1;
		right = vcenter.x + cx2;
		bottom = vcenter.y + cy2;
	}
	cPointTemplate<T> GetCenter() const {
		return cPointTemplate<T>( left + GetWidth() / 2, top + GetHeight() / 2 );
	}

	//返回等比内切矩形
	//aspect 宽高比
	cRectTemplate<T> GetInnerRect(float aspect) const {
		cRectTemplate<T> rc = *this;
		float myaspect = (float)GetWidth() / GetHeight();
		if (myaspect > aspect) {
			T w = (T) (GetHeight() * aspect);
			rc.SetWidth(w);
			rc.MoveSelf( ( GetWidth() - w ) / 2, 0);
		}
		else {
			T h = (T) (GetWidth() / aspect);
			rc.SetHeight(h);
			rc.MoveSelf( 0, ( GetHeight() - h ) / 2);
		}
		return rc;
	}

	cPointTemplate<T> GetPoint(eDirRect e) const {
		switch(e)
		{
		case DR_TopLeft:
			return cPointTemplate<T>(left, top);
		case DR_TopRight:
			return cPointTemplate<T>(right, top);
		case DR_BottomLeft:
			return cPointTemplate<T>(left, bottom);
		case DR_BottomRight:
			return cPointTemplate<T>(right, bottom);
		default:
			return cPointTemplate<T>();
		}
	}

	//特殊函数
	//得到两个rect相交部分，在this rect的坐标系下的rect
	cRectTemplate<T> GetInRect(const cRectTemplate<T>& rc) const {
		cRectTemplate<T> rcOut;
		cRectTemplate<T> rc1 = GetIntersection(rc);
		if (rc1.IsEmpty())
			return rcOut;
		cRectTemplate<T> rc2;
		rcOut.SetData1( rc1.left - left, rc1.top - top,
			GetWidth() - ( right - rc1.right ),
			GetHeight() - ( bottom - rc1.bottom ) );
		return rcOut;
	}

	
	cLine2D GetDirLine(eDirLine e) const {
		switch(e)
		{
		case DL_Top:
			return cLine2D(GetPoint(DR_TopLeft), GetPoint(DR_TopRight));
		case DL_Right:
			return cLine2D(GetPoint(DR_TopRight), GetPoint(DR_BottomRight));
		case DL_Bottom:
			return cLine2D(GetPoint(DR_BottomRight), GetPoint(DR_BottomLeft));
		case DL_Left:
			return cLine2D(GetPoint(DR_BottomLeft), GetPoint(DR_TopLeft));
		default:
			return cLine2D();
		}
	}

	T left;
	T top;
	T right;
	T bottom;

};

/** 一些帮助函数 */
/*
*	szSource	表示源区域大小
*	szBlock		表示一小块
*/
template<typename T>
inline int ClipRect(const cSizeTemplate<T>& szSource, const cSizeTemplate<T>& szBlock, cRectTemplate<T>* arcout = 0)
{
	cRectTemplate<T>* pRc = 0;
	int rcnum = 0;
	if (szSource.GetWidth() < szBlock.GetWidth() &&
		szSource.GetHeight() < szBlock.GetHeight())
	{
		if (arcout) {
			pRc = &arcout[rcnum];
			pRc->left = 0;
			pRc->right = szSource.GetWidth();
			pRc->top = 0;
			pRc->bottom = szSource.GetHeight();
		}
		rcnum++;
		return rcnum;
	}
	T h = 0, w = 0;
	do {
		w = 0;
		do {
			if (arcout) {
				pRc = &arcout[rcnum];
				pRc->left = w;
				if (w + szBlock.GetWidth() >= szSource.GetWidth())
					pRc->right = szSource.GetWidth();
				else
					pRc->right = ( w + szBlock.GetWidth() );
				pRc->top = h;
				if (h + szBlock.GetHeight() >= szSource.GetHeight())
					pRc->bottom = szSource.GetHeight();
				else
					pRc->bottom = ( h + szBlock.GetHeight() );
			}
			w += szBlock.GetWidth();
			rcnum++;
		} while(w < szSource.GetWidth());
		h += szBlock.GetHeight();
	} while(h < szSource.GetHeight());
	return rcnum;
}

/*
类似小地图的导航
*/
template<typename T>
inline bool MoveInRect(cRectTemplate<T>& rcView, cRectTemplate<T>& rcInMap, const cPointTemplate<T>& pt, 
	const cSizeTemplate<T>& szView, const cRectTemplate<T>& rcMap)
{
	if (!rcMap.IsIn(pt))
		return false;

	cRectTemplate<T> rcR, rc, rcT;
	T tx = 0, ty = 0;

	if (rcMap.GetWidth() >= szView.GetWidth() && 
		rcMap.GetHeight() >= szView.GetHeight())
	{
		//上下左右都可移动
		rc.SetCenter(pt, szView);
		rcView.SetSize(szView);
		if (rc.left < rcMap.left)
			tx = rcMap.left;
		else if (rc.right > rcMap.right)
			tx = rcMap.right - szView.GetWidth();
		else
			tx = rc.left;
		if (rc.top < rcMap.top)
			ty = rcMap.top;
		else if (rc.bottom > rcMap.bottom)
			ty = rcMap.bottom - szView.GetHeight();
		else
			ty = rc.top;
		rcView.MoveSelfTo(tx, ty);
	}
	else if (rcMap.GetWidth() > szView.GetWidth() && 
		rcMap.GetHeight() < szView.GetHeight())
	{
		//允许左右移动
		rc.SetCenter(pt, szView);
		rcView.SetSize(szView);
		ty = rcMap.top - ( szView.GetHeight() - rcMap.GetHeight() ) / 2;
		if (rc.left < rcMap.left)
			tx = rcMap.left;
		else if (rc.right > rcMap.right)
			tx = rcMap.right - szView.GetWidth();
		else
			tx = rc.left;
		rcView.MoveSelfTo(tx, ty);
	}
	else if (rcMap.GetHeight() > szView.GetHeight() &&
		rcMap.GetWidth() < szView.GetWidth())
	{
		//允许上下移动
		rc.SetCenter(pt, szView);
		rcView.SetSize(szView);
		tx = rcMap.left - ( szView.GetWidth() - rcMap.GetWidth() ) / 2;
		if (rc.top < rcMap.top)
			ty = rcMap.top;
		else if (rc.bottom > rcMap.bottom)
			ty = rcMap.bottom - szView.GetHeight();
		else
			ty = rc.top;
		rcView.MoveSelfTo(tx, ty);
	}
	else {
		//不能移动
		rc.SetCenter(pt, szView);
		rcView.SetSize(szView);
		tx = rcMap.left - ( szView.GetWidth() - rcMap.GetWidth() ) / 2;
		ty = rcMap.top - ( szView.GetHeight() - rcMap.GetHeight() ) / 2;
		rcView.MoveSelfTo(tx, ty);
	}

	rcT = rcMap.GetIntersection(rcView);
	rcInMap.left = rcT.left - rcMap.left;
	rcInMap.top = rcT.top - rcMap.top;
	rcInMap.right = rcInMap.left + rcT.GetWidth();
	rcInMap.bottom = rcInMap.top + rcT.GetHeight();
	return true;
}

/*
卷轴算法
为了效率暂时没有判断除0的情况
*/
template<typename T>
inline cRectTemplate<T> RealRect(const cSizeTemplate<T>& szBig,
					 const cRectTemplate<T>& rcView, const cSizeTemplate<T>& szReal, b8 bY = true)
{
	cSizeTemplate<T> sz;
	sz.x = szBig.x - rcView.GetWidth();
	sz.y = szBig.y - rcView.GetHeight();
	cSizeTemplate<T> sz1;
	sz1.x = szReal.x - rcView.GetWidth();
	sz1.y = szReal.y - rcView.GetHeight();
	cRectTemplate<T> rcReal(rcView);
	T x = 0, y = 0;
	if (sz.x > (T)0) x = (T)((f32)rcView.left / sz.x * sz1.x);
	if (bY && sz.y > (T)0) y = (T)((f32)rcView.top / sz.y * sz1.y);
	rcReal.MoveSelfTo(x, y);
	return rcReal;
}

typedef cRectTemplate<i32> cRect;
typedef cRectTemplate<f32> cRectf;
typedef cRectTemplate<ui16> cRectus;
typedef cRectTemplate<ui8> cRectuc;

END_COMMON

typedef cross::cRect		rect;
typedef cross::cRectf		rectf;
typedef cross::cRectuc		rectuc;

#endif

