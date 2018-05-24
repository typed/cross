#ifndef _common_cvector2d_h_
#define _common_cvector2d_h_

#include "../CommonDef.h"
#include "MathBase.h"

namespace cm
{

template<class T>
class cVector2D
{
public:
	cVector2D() { Set0(); };

	cVector2D(T a,T b){x = a; y = b; }
	cVector2D(const cVector2D<T>& other) { *this = other; };
	cVector2D<T>& operator = (const cVector2D<T>& other) {
		x = other.x; y = other.y;
		return *this;
	}
	void Set0() { x = y = 0; }
	bool operator == (const cVector2D<T>& other) const {
		return ( Equals(x, other.x) && Equals(y, other.y) );
	}
	bool operator != (const cVector2D<T>& other) const { return !operator == (other);}

	cVector2D<T> operator + (const cVector2D<T>& other) const {
		cVector2D<T> temp;
		temp.x = x + other.x;
		temp.y = y + other.y;
		return temp;
	}

	cVector2D<T> operator - (const cVector2D<T>& other) const {
		cVector2D<T> temp;
		temp.x = x - other.x;
		temp.y = y - other.y;
		return temp;
	}

	cVector2D<T> operator / (const cVector2D<T>& other) const {
		cVector2D<T> temp;
		temp.x = x / other.x;
		temp.y = y / other.y;
		return temp;
	}

	cVector2D<T> operator * (const cVector2D<T>& other) const {
		cVector2D<T> temp;
		temp.x = x * other.x;
		temp.y = y * other.y;
		return temp;
	}

	cVector2D<T>& operator += (const cVector2D<T>& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	cVector2D<T>& operator -= (const cVector2D<T>& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	cVector2D<T>& operator /= (const cVector2D<T>& other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	cVector2D<T>& operator *= (const cVector2D<T>& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	cVector2D<T>& operator += (const T& num) {
		x += num;
		y += num;
		return *this;
	}

	cVector2D<T>& operator -= (const T& num) {
		x -= num;
		y -= num;
		return *this;
	}

	cVector2D<T>& operator /= (const T& num) {
		x /= num;
		y /= num;
		return *this;
	}

	cVector2D<T>& operator *= (const T& num) {
		x *= num;
		y *= num;
		return *this;
	}

	cVector2D<T> operator + (const T& num) {
		cVector2D<T> temp;
		temp.x = x + num;
		temp.y = y + num;
		return temp;
	}

	cVector2D<T> operator - (const T& num) {
		cVector2D<T> temp;
		temp.x = x - num;
		temp.y = y - num;
		return temp;
	}

	cVector2D<T> operator / (const T& num) {
		cVector2D<T> temp;
		temp.x = x / num;
		temp.y = y / num;
		return temp;
	}

	cVector2D<T> operator * (const T& num) {
		cVector2D<T> temp;
		temp.x = x * num;
		temp.y = y * num;
		return temp;
	}

	cVector2D<T> GetInterpolated(const cVector2D<T>& other, float d) const
	{
		float inv = 1.0f - d;
		return cVector2D<T>((T)(other.x*inv + x*d), (T)(other.y*inv + y*d));
	}

	cVector2D<T>& Normalize()
	{
		T l = x * x + y * y;
		if (l == 0)
			return *this;
		l = (T) ( 1 / CrossMath::Sqrt(l) );
		x *= l; y *= l;
		return *this;
	}

	//�����Ĵ�С
	T GetLength() const {
		return (T)CrossMath::Sqrt((f32)GetLengthSQ());
	}
	T GetDistance(const cVector2D<T>& des) const {
		return (T)CrossMath::Sqrt((f32)GetDistanceSQ(des));
	}

	//add by yfw 2012.08.30
	T GetLengthSQ() const {
		return (x * x + y * y);
	}
	T GetDistanceSQ(const cVector2D<T>& des) const {
		cVector2D<T> v = des - *this;
		return v.GetLengthSQ();
	}
	//��ǰ���Ƿ���ָ�� ����֮��
	bool IsBetweenPoints(const cVector2D<T>& begin, const cVector2D<T>& end) const
	{
		const T f = (end - begin).GetLengthSQ();
		return GetDistanceSQ(begin) <= f && GetDistanceSQ(end) <= f;
	}
	//��ǰ���Ƿ���ָ�� ����֮��  ����ҿ��ṹ
	bool IsBetweenPointsRightOpen(const cVector2D<T>& begin, const cVector2D<T>& end) const
	{
		const T f = (end - begin).GetLengthSQ();
		return GetDistanceSQ(begin) < f && GetDistanceSQ(end) <= f;
	}
	//��ǰ���Ƿ���ָ�� ����֮��  ����ҿ��ṹ
	bool LimitBetweenPointsRightOpen(const cVector2D<T>& begin, const cVector2D<T>& end)
	{
		cVector2D<T>& temp = *this;
		for (int i=0; i<2; i++)
		{
			if (temp[i] < begin[i])
				temp[i] = begin[i];
			else if (temp[i] >= end[i])
				temp[i] = end[i]-T(1);
		}
		return true;
	}
	//��ǰ���Ƿ���ָ�� ����֮��  ����ҿ��ṹ
	bool LimitBetweenPoints(const cVector2D<T>& begin, const cVector2D<T>& end)
	{
		cVector2D<T>& temp = *this;
		for (int i=0; i<2; i++)
		{
			if (temp[i] < begin[i])
				temp[i] = begin[i];
			else if (temp[i] >= end[i])
				temp[i] = end[i];
		}
		return true;
	}


	T& operator [] ( int i ) const		//add by yfw
	{
		return  ((T *)&x)[ i ];
	}

	T x;
	T y;
};

typedef cVector2D<f32>		cVector2Df;

template<class T>
class cSizeTemplate :
	public cVector2D<T>
{
public:
	using cVector2D<T>::x;
	using cVector2D<T>::y;

	cSizeTemplate():cVector2D<T>() {}

	cSizeTemplate(T cx, T cy) { x = cx; y = cy; }
	cSizeTemplate(cVector2D<T> a) { x = a.x; y = a.y; }

	void SetWidth(T t) { x = t; }
	T GetWidth() const { return x; }
	void SetHeight(T t) { y = t; }
	T GetHeight() const { return y; }
	void SetSize(T cx, T cy) { x = cx; y = cy;}

	bool IsIn(T tx, T ty) const { return (tx >= 0 && tx < x && ty >= 0 && ty < y); }

	bool IsEmpty() const {return x <= 0 || y <= 0;}

	//���ţ�< 1.f ��С��> 1.f �Ŵ�
	void SetScale(float xscale, float yscale, float xfloat = 0.f, float yfloat = 0.f) {
		x = x * xscale + xfloat;
		y = y * yscale + yfloat;
	}
};

typedef cSizeTemplate<i32>	cSize;
typedef cSizeTemplate<f32>	cSizef;

template<class T>
class cPointTemplate :
	public cVector2D<T>
{
public:
	using cVector2D<T>::x;
	using cVector2D<T>::y;

	cPointTemplate() : cVector2D<T>() {}

	cPointTemplate(T xp, T yp) { x = xp; y = yp; }

	cPointTemplate(const cVector2D<T>& other) {
		x = other.x;
		y = other.y;
	}

	void SetX(T t) { x = t; }

	T GetX() const { return x; }

	void SetY(T t) { y = t; }

	T GetY() const { return y; }

	void SetXY(T xp, T yp) { x = xp; y = yp;}

	void MoveSelf(T xoffset, T yoffset) { x += xoffset; y += yoffset;}

	cPointTemplate<T> Move(T xoffset, T yoffset) {
		cPointTemplate<T> pt = *this;
		pt.MoveSelf(xoffset, yoffset);
		return pt;
	}
};

typedef cPointTemplate<i32>	cPoint;
typedef cPointTemplate<f32>	cPointf;

inline int GetDir(const cPoint& pForm, const cPoint& pTo)
{
	if (pForm.x == pTo.x && pForm.y >= pTo.y)
		return 0;
	else if (pForm.x == pTo.x && pForm.y < pTo.y)
		return 4;
	else if (pForm.x < pTo.x && pForm.y == pTo.y)
		return 2;
	else if (pForm.x < pTo.x && pForm.y > pTo.y)
		return 1;
	else if (pForm.x < pTo.x && pForm.y < pTo.y)
		return 3;
	else if (pForm.x > pTo.x && pForm.y < pTo.y)
		return 5;
	else if (pForm.x > pTo.x && pForm.y == pTo.y)
		return 6;
	else
		return 7;
}


/*
7   0   1
6       2
5   4   3        ���У�pForm��0λ�ã�   ��������pTo��λ��   ���н�8��Ϊ0
*/

inline float GetDis(const cPoint& ptDes, const cPoint& ptSrc)
{
	cPoint pt = ptDes - ptSrc;
	return CrossMath::Sqrt(pt.x * pt.x * 1.f + pt.y * pt.y * 1.f);
}


inline float GetDis2(const cPoint& ptDes, const cPoint& ptSrc ,float fScale = 1.0) //add by cgw ����ƽ�� fScale
{
	cPoint pt = ptDes - ptSrc;
	return pt.x * pt.x * fScale * fScale + pt.y * pt.y * 1.f; //fScaleΪ����ĵ�λ������ԭ�򣬸��Ӵ�С�������ܲ�ͬ����64*32
}


//Ϊ����ȷ��һ��Ŀ�����Ա�һ��Ŀ������Ұ��ϵ����GetAngleFromDir��GetAngleByTwoPoint�еĽǶȶ�ת����0��360��

//angle�����ݷ���ȷ���÷��������Ӧ��ʵ�ʽǶ�  ע�����GetDir�Ķ�����ˣ�����Ҫ��Ӧ�ı�
inline int GetAngleFromDir(int & nDir)
{
	return (450-45*nDir)%360;
}

//pForm��pTo����ĽǶȣ���0��360��
inline int GetAngleByTwoPoint(const cPoint& pForm, const cPoint& pTo)
{
	float fDis = GetDis(pForm, pTo);
	if(fDis == 0)
	{
		return 0;
	}
	float fAngle = 180.f * c_fM_1_PI * CrossMath::Asin((pTo.y - pForm.y)/fDis);
	if((pTo.x - pForm.x) < 0)
	{
		fAngle = 180.f - fAngle;
	}
	return (360 + (int)fAngle)%360;
}

//pForm��pTo����ĽǶ����pForm����ļн�,������Ұ�ж�
inline int GetBetweenAngle(const cPoint& pForm, const cPoint& pTo,int & nDir)
{
	return CrossMath::Abs(GetAngleByTwoPoint(pForm, pTo)-GetAngleFromDir(nDir));
}


//add by cgw ��ȡpForm��pTo����Ĳ�������ȡ��Ŀ�������ĵ�,���ڽ������Ŀ����غ�
inline cPoint GetBestDirPoint(const cPoint& pForm, const cPoint& pTo/*, cPoint& pToNear*/)
{
	cPoint pToNear(0,0);
	if(pForm.x > pTo.x){  //��ֹ�ص�
		pToNear.x++;
	}
	else if(pForm.x < pTo.x){
		pToNear.x--;
	}

	if(pForm.y > pTo.y){
		pToNear.y++;
	}
	else if(pForm.y < pTo.y){
		pToNear.y--;
	}
	return  pToNear;
}


inline float Get2DTo3D(const cPoint& pt, float vdis)
{
	if (pt.x >= 0 && pt.y >= 0)
		return c_fPI * 1.5f + CrossMath::Asin((float)pt.y / vdis);
	else if (pt.x >= 0 && pt.y < 0)
		return c_fPI + CrossMath::Asin((float)pt.x / vdis);
	else if (pt.x < 0 && pt.y < 0)
		return c_fPI * 0.5f + CrossMath::Asin((float)CrossMath::Abs(pt.y) / vdis);
	else
		return CrossMath::Asin((float)CrossMath::Abs(pt.x) / vdis);
}

inline float Get2DTo3D_2(const cPoint&/* pt*/, float/* vdis*/)
{
	//if (pt.x >= 0 && pt.y >= 0)
	//	return c_fPI * 1.5f + CrossMath::Asin(pt.y / vdis);
	//else if (pt.x >= 0 && pt.y < 0)
	//	return c_fPI + CrossMath::Asin(pt.x / vdis);
	//else if (pt.x < 0 && pt.y < 0)
	//	return c_fPI * 0.5f + CrossMath::Asin(abs(pt.y) / vdis);
	//else
	//	return CrossMath::Asin(abs(pt.x) / vdis);
	return 0.f;
}

//�жϵ����߶ε���ߺ��ұ�
//��-1,��1,���ӳ�����0
template<class T> inline int PointInLine(const cVector2D<T>& vFrom, const cVector2D<T>& vTo, const cVector2D<T>& vPoint)
{
	T r = (vFrom.x - vTo.x) * (vPoint.y - vTo.y) - (vFrom.y - vTo.y) * (vPoint.x - vTo.x);
	if ( r > 0 ) {
		//��Cλ�������߶�AB�����
		return -1;
	}
	else if (r == 0) {
		//��Cλ�������߶�AB�ϣ�Ҳ�п������ӳ����ϣ�
		return 0;
	}
	else {
		//��Cλ�������߶�AB���Ҳ�
		return 1;
	}
}

//add by yfw
template<class T0, class T1>
cVector2D<T1> TransVector2DTo2D(cVector2D<T1>& vOut, const cVector2D<T0>& vIn)
{
	for (unsigned int unIndex=0; unIndex<2; unIndex++)
	{
		vOut[unIndex] = (T1)vIn[unIndex];
	}
	return vOut;
}

}

typedef cm::cVector2Df	vector2d;
typedef cm::cPoint		point;
typedef cm::cPointf		pointf;
typedef cm::cSize			size;
typedef cm::cSizef		sizef;

#endif
