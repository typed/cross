#ifndef cross_math_mathhelper_h
#define cross_math_mathhelper_h

#include "Config.h"
#include "MathBase.h"
#include "cVector2D.h"
#include "cVector3D.h"
#include "cVector4D.h"
#include "cRect.h"
#include "cRectSet.h"
#include "cMatrix4X4.h"
#include "cQuaternion.h"
#include "cLine3D.h"
#include "cPlane3D.h"
#include "cAabbox.h"
#include "cViewFrustum.h"
#include "cTriangle3D.h"
#include "cColor.h"
#include "Random.h"
#include "cGridHeight.h"
#include "cPosRotScl.h"
#include <vector>

namespace cross
{

typedef std::vector<matrix> aMatrix;
typedef std::vector<quat> aQuat;
typedef std::vector<vector3d> aVector3D;

//两点间距离
template<class T> inline T GetDistance(const cVector2D<T>& v1, const cVector2D<T>& v2)
{
	return (T) MathFunc::Sqrt( (f32) (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) );
}

//求直线斜率
template<class T> inline f32 LineSlope(const cVector2D<T>& vFrom, const cVector2D<T>& vTo)
{
	return (f32)( vTo.y - vFrom.y ) / ( vTo.x - vFrom.x );
}

//倍数转换
template<class T> inline T GetDiploid(const T n, const T d)
{
	return n % d ? n / d + 1 : n / d;
}

//求一个2D向量的弧度
//template<class T> inline T GetDiploid(const cVector2D<T>& v, bool bDeasil = false)
//{
//	return atanf( v.y / v.x );
//	if (v.x == 0 0 && v.y > 0) {
//
//	}
//	return n % d ? n / d + 1 : n / d;
//}

inline bool fgreat(f32 a, f32 b)
{
	return (a - b) > -0.001f;
}

inline bool fless(f32 a, f32 b)
{
	return (a - b) < +0.001f;
}

//8方向转弧度
inline cVector3Df Dir8ToRadian(i32 dir)
{
	static const f32 c_fRadian[] = {
		180.f * c_fDegToRad,
		225.f * c_fDegToRad,
		270.f * c_fDegToRad,
		315.f * c_fDegToRad,
		0.f,
		45.f * c_fDegToRad,
		90.f * c_fDegToRad,
		135.f * c_fDegToRad
	};
	if (dir < 0 || dir >= 8)
		return cVector3Df(0.f, c_fRadian[4], 0.f);
	return cVector3Df(0.f, c_fRadian[dir], 0.f);
}
inline i32 RadianToDir8(cVector3Df v3dRadian)
{
	static const f32 c_fRadian[] = {
		180.f * c_fDegToRad,
		225.f * c_fDegToRad,
		270.f * c_fDegToRad,
		315.f * c_fDegToRad,
		0.f,
		45.f * c_fDegToRad,
		90.f * c_fDegToRad,
		135.f * c_fDegToRad
	};
	i32 nIndex = 4;
	for(i32 i = 0; i < 8; i++)
	{
		if(MathFunc::Abs(v3dRadian.y - c_fRadian[i]) <= 22.5f * c_fDegToRad)
		{
			nIndex=i;
		}
	}
	return nIndex;
}

inline ui32 Get2Multiples(ui32 w, ui32 maxw = 2048)
{
	if (w >= maxw)
		return maxw;
	w--;
	w |= w >> 1;
	w |= w >> 2;
	w |= w >> 4;
	w |= w >> 8;
	w |= w >> 16;
	return w + 1;
}

//简单的曲线，基于bezier curve
//P1223(t) = (1-t)^2P1 + 2t(t-1)P2 + t^2P3
inline void GetCurvePos(const cVector3Df& ptFrom, const cVector3Df& ptTo, const cVector3Df& ptControl, cVector3Df& ptR, f32 t)
{
	ptR.x = ( (1.f - t) * (1.f - t) * ptFrom.x + 2.f * t * (1.f - t) * ptControl.x + t * t * ptTo.x + 0.5f );
	ptR.y = ( (1.f - t) * (1.f - t) * ptFrom.y + 2.f * t * (1.f - t) * ptControl.y + t * t * ptTo.y + 0.5f );
	ptR.z = ( (1.f - t) * (1.f - t) * ptFrom.z + 2.f * t * (1.f - t) * ptControl.z + t * t * ptTo.z + 0.5f );
}

//author:	yfw
//Desc:		根据制定的边界值，调整最大最小值	
//Param:	nLow 最小边界值		nUp 最大边界值  nMin nMax 要调整的最大最小值 
inline bool MaxMinValue(i32 nLow, i32 nUp, i32 &nMin, i32 &nMax)
{
	if (nLow > nUp)
		return false;

	if (nMin < nLow)
		nMin = nLow;
	else if (nMin > nUp)
		nMin = nUp;

	if (nMax < nMin)
		nMax = nMin;
	else if (nMax > nUp)
		nMax = nUp;

	return true;
}

enum eAngularDisplacementXY
{
	AD_X = 1,
	AD_Y = 2,
	AD_ALL = 3
};

//返回值是矩阵形式的角位移，如果两个向量相等就返回false
//游戏里默认(0,0,-1)方向为3D对象的0度方向。顺势针递增
inline bool GetRotation(cVector3Df& vOut, const cVector3Df& vTo, const cVector3Df& vFrom, 
	eAngularDisplacementXY e = AD_Y, const cVector3Df& vDefault = cVector3Df(0.f, 0.f, -1.f))
{
	if (vTo == vFrom)
		return false;
	vOut.Set0();
	cVector3Df vDir = vTo - vFrom;
	cVector3Df vDirC = vDir;
	vDirC.Normalize();
	vDirC.y = 0.f;
	if (e & AD_Y) {
		vOut.y = vDefault.GetTheta(vDirC);
		if (vDir.x > 0)
			vOut.y = c_f2PI - vOut.y;
	}
	if (e & AD_X) {
		if (vDir.y > 0)
			vOut.x = vDirC.GetTheta(vDir);
		else
			vOut.x = - vDirC.GetTheta(vDir);
	}
	return true;
}

//游戏里默认(0,0,-1)方向为3D对象的0度方向。顺势针递增
inline bool GetYDir(cVector3Df& vOut, const cVector3Df& vDes, const cVector3Df& vSrc, const cVector3Df& vDefault = cVector3Df(0.f, 0.f, -1.f))
{
	return GetRotation(vOut, vDes, vSrc, AD_Y, vDefault);
}

inline cVector3Df GetYDirVector(const cVector3Df& vFrom, f32 fYDeg, f32 fDis, const cVector3Df& vDefault = cVector3Df(0.f, 0.f, -1.f))
{
	cVector3Df v(vDefault);
	v.RotateXZBy(fYDeg);
	return vFrom + v * fDis;
}


//Added by lkm,2011.11.21
//判断点是否在凸多边形内,要求VertexList中顶点按逆时钟方向排列
template<typename T>
inline bool IsPointInPolygon(const std::vector<cVector2D<T> >& VertexList, const cVector2D<T>& vPoint)
{
	if(VertexList.size() < 3)
	{
		Assert(VertexList.size() < 3);
		return false;
	}
	bool bIn = true;
	ui32 i = 1;
	for(; i < VertexList.size(); ++ i)
	{
		bIn = bIn && (PointInLine(VertexList[i - 1], VertexList[i], vPoint) <= 0);
		if(!bIn)
		{
			return false;
		}
	}
	return bIn && (PointInLine(VertexList[i - 1], VertexList[0], vPoint) <= 0);
}

//add by yfw 
//求得指定x，z两维数据，求得指定平面内，对应的另外一维数据。
template<typename T>
inline T GetHeightInPlane(const cVector3D<T>& pt0, const cVector3D<T>& pt1, const cVector3D<T>& pt2, T x, T z)
{
	cVector3D<T> vNormal = (pt1 - pt0).CrossProduct(pt2 - pt1);
	vNormal.Normalize();

	f32 fD = - pt0.DotProduct(vNormal);
	if (Equals(vNormal.y, 0.0f))
		return T(0);

	T height = T((-fD - (vNormal.x*x + vNormal.z*z)) / vNormal.y);

	return height;
}

inline bool isOnSameSide(const cVector3Df& p1, const cVector3Df& p2, const cVector3Df& a, const cVector3Df& b)
{
	cVector3Df bminusa = b - a;
	cVector3Df cp1 = bminusa.CrossProduct(p1 - a);
	cVector3Df cp2 = bminusa.CrossProduct(p2 - a);
	return (cp1.DotProduct(cp2) >= 0.0f);
}
//求点是否在3点合成的三角形内（前提，点必须在该3点合成的面中）
inline bool isPointInside(const cVector3Df& p, const cVector3Df& a, const cVector3Df& b, const cVector3Df& c)
{
	return (isOnSameSide(p, a, b, c) && isOnSameSide(p, b, a, c) && isOnSameSide(p, c, a, b));
}

// static const i32 c_nNumDir8 = 8;		//8个方向
// static const point c_aPointDir8[c_nNumDir8] = {
// 	point(0,-1),
// 	point(1,-1),
// 	point(1,0),
// 	point(1,1),
// 	point(0,1),
// 	point(-1,1),
// 	point(-1,0),
// 	point(-1,-1)
// };

inline bool FindVortexRectPoint(std::vector<point>& aRet, point pt, i32 rCount, i32 rot = 0)
{
	aRet.clear();
	if (rCount < 0 || rot < 0 || rot >= 4)
		return false;
	if (rCount == 0) {
		aRet.push_back(pt);
		return true;
	}
	const i32 d[] = {0, -1, 1, 0, 0, 1, -1, 0};
	i32 dCount = (rCount * 2 + 1) * (rCount * 2 + 1) -1, k = 0, n = 0;
	aRet.resize(dCount+1);
	aRet[0] = pt;
	bool bLoop = true;
	while (bLoop) {
		for (i32 i = 0; i <= n / 2; i++) {
			pt.x += d[rot*2];
			pt.y += d[rot*2+1];
			aRet[++k] = pt;
			if (k >= dCount) {
				bLoop = false;
				break;
			}
		}
		rot = (++rot % 4);
		n++;
	}
	return true;
}

}
#endif
