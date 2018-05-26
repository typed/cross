#ifndef cgridheight_h
#define cgridheight_h

#include "../CommonDef.h"
#include "MathBase.h"
#include "cTriangle3D.h"

namespace cm
{

class cGridHeight
{
public:
	cGridHeight() : m_data(0), m_x(0), m_y(0) {}
	~cGridHeight() {if (m_data) delete[] m_data; }
	void Reset(i32 x, i32 y);
	i32 GetWidthCount() const {return m_x;}
	i32 GetHeightCount() const {return m_y;}
	//为了效率这里就不做有效性验证
	f32 GetData(i32 xidx, i32 yidx) const {return m_data[yidx * m_x + xidx];}
	f32* GetDataPtr() const {return m_data;}
	void SetData(i32 xidx, i32 yidx, f32 f) {m_data[yidx * m_x + xidx] = f;}
	f32 GetHeight(f32 x, f32 z) const;
	bool GetPickFromLine(i32 xidx, i32 yidx, const cVector3Df& linePoint, const cVector3Df& lineVect, cVector3Df& v) const;
	bool GetPickFromLine(i32 idx, const cVector3Df& linePoint, const cVector3Df& lineVect, cVector3Df& v) const;
private:
	f32* m_data;
	i32 m_x;
	i32 m_y;
};

inline void cGridHeight::Reset(i32 x, i32 y)
{
	m_x = x + 1;
	m_y = y + 1;
	if (m_data)
		delete[] m_data;
	m_data = new f32[m_x * m_y];
	CrossMemory::ZeroMem(m_data, sizeof(f32) * m_x * m_y);
}

inline f32 cGridHeight::GetHeight(f32 x, f32 z) const
{
	cVector3Df v;
	if (GetPickFromLine((i32)x, (i32)z, cVector3Df(x, -1.f, z), cVector3Df(0.f, 1.f, 0.f), v))
		return v.y;
	return 0.f;
	//f32 xf = x - (f32)xidx;
	//f32 yf = z - (f32)yidx;
	//T topx = nLeftTop + ( nRightTop - nLeftTop ) * xf;
	//T bottomx = nLeftBottom + ( nRightBottom - nLeftBottom ) * xf;
	//T lefty = nLeftTop + ( nLeftBottom - nLeftTop ) * yf;
	//T righty = nRightTop + ( nRightBottom - nRightTop ) * yf;
	//T v = topx + ( bottomx - topx ) * yf;
	//T h = lefty + ( righty - lefty ) * xf;
	//return h + ( v - h ) / 2.f;
}

inline bool cGridHeight::GetPickFromLine(i32 xidx, i32 yidx, const cVector3Df& linePoint, const cVector3Df& lineVect, cVector3Df& v) const
{
	if (xidx < 0 || xidx >= (m_x-1) || yidx < 0 || yidx >= (m_y-1))
		return false;
	f32 nLeftBottom = GetData(xidx, yidx);
	f32 nRightBottom = GetData(xidx+1, yidx);
	f32 nLeftTop = GetData(xidx, yidx+1);
	f32 nRightTop = GetData(xidx+1, yidx+1);
	cTriangle3Df tri1(
		cVector3Df((f32)xidx, nLeftBottom, (f32)yidx),
		cVector3Df((f32)xidx+1, nRightBottom, (f32)yidx),
		cVector3Df((f32)xidx, nLeftTop, (f32)yidx+1));
	//if (tri1.GetIntersectionWithLine(linePoint, lineVect, v))
	//	return true;
	cTriangle3Df tri2(
		cVector3Df((f32)xidx, nLeftTop, (f32)yidx+1),
		cVector3Df((f32)xidx+1, nRightTop, (f32)yidx+1),
		cVector3Df((f32)xidx+1, nRightBottom, (f32)yidx));
	//if (tri2.GetIntersectionWithLine(linePoint, lineVect, v))
	//	return true;
	cVector3Df v1, v2;
	if (!tri1.GetIntersectionOfPlaneWithLine(linePoint, lineVect, v1))
		return false;
	if (!tri2.GetIntersectionOfPlaneWithLine(linePoint, lineVect, v2))
		return false;
	v = linePoint.GetDistanceSQ(v1) < linePoint.GetDistanceSQ(v2) ? v1 : v2;
	return true;
}

inline bool cGridHeight::GetPickFromLine(i32 idx, const cVector3Df& linePoint, const cVector3Df& lineVect, cVector3Df& v) const
{
	return GetPickFromLine(idx / (m_x-1), idx % (m_x-1), linePoint, lineVect, v);
}

}
#endif
