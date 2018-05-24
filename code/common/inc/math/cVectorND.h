#ifndef _common_vectornd_h_
#define _common_vectornd_h_

#include "MathBase.h"
#include "../CrossMemory.h"

namespace cm
{
	//const ui32 g_cunMaxDimemsion = 4;		//最大维数

	//enum eVecDimemsion
	//{
	//	eVecDimemsion_1 = 1,
	//	eVecDimemsion_2 = 2,
	//	eVecDimemsion_3 = 3,
	//	eVecDimemsion_4 = 4,
	//};
	//template<class T, eVecDimemsion DIMEMSION>
	//class cVectorND
	//{
	//public:
	//	cVectorND()
	//	{
	//		//ini
	//		Init();
	//	}
	//	~cVectorND()
	//	{
	//	}
	//	//cVectorND(T a,T b)
	//	//{
	//	//	x = a; y = b;
	//	//}
	//	cVectorND(const cVectorND<T, DIMEMSION>& other)
	//	{
	//		*this = other;
	//	}
	//	cVectorND(ui32 unDimemsion, const T* pData)
	//	{
	//		//ini
	//		Init();

	//		if (!pData || DIMEMSION != unDimemsion)
	//			return;

	//		//alloc
	//		memcpy(m_Data, pData, sizeof(T)*DIMEMSION);
	//	}

	//	bool IsEmpty() const
	//	{
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			if (!Equals(m_Data[unIndex], (T)0))
	//				return false;
	//		}
	//		return true;
	//	}
	//	bool Init()
	//	{
	//		memset(m_Data, 0, sizeof(T)*DIMEMSION);

	//		return true;
	//	}
	//	float& operator [] ( int i ) const
	//	{
	//		return *(float *)&m_Data[i];
	//	}

	//	//float& operator [] ( int i ) const
	//	//{
	//	//	return  ((float *)&x)[ i ];
	//	//}

	//	T* GetData() const
	//	{
	//		return (float*)&m_Data[0];
	//	}

	//	//复值 如果维数不相等，则需要先重新分配
	//	cVectorND<T, DIMEMSION>& operator = (const cVectorND<T, DIMEMSION>& other)
	//	{
	//		//当前维数不相等时，需要重新分配
	//		memcpy(m_Data, other.GetData(), sizeof(T)*DIMEMSION);

	//		return *this;
	//	}

	//	bool operator == (const cVectorND<T, DIMEMSION>& other) const
	//	{
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			if ( !Equals(m_Data[unIndex], other[unIndex]))
	//				return false;
	//		}
	//		return true;
	//	}

	//	cVectorND<T, DIMEMSION> operator + (const cVectorND<T, DIMEMSION>& other) const
	//	{

	//		cVectorND<T, DIMEMSION> temp;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			temp[unIndex] = m_Data[unIndex] + other[unIndex];
	//		}
	//		return temp;
	//	}

	//	cVectorND<T, DIMEMSION> operator - (const cVectorND<T, DIMEMSION>& other) const
	//	{
	//		cVectorND<T, DIMEMSION> temp;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			temp[unIndex] = m_Data[unIndex] - other[unIndex];
	//		}
	//		return temp;
	//	}

	//	cVectorND<T, DIMEMSION> operator / (const cVectorND<T, DIMEMSION>& other) const
	//	{
	//		cVectorND<T, DIMEMSION> temp;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			temp[unIndex] = m_Data[unIndex] / other[unIndex];
	//		}
	//		return temp;
	//	}

	//	cVectorND<T, DIMEMSION> operator * (const cVectorND<T, DIMEMSION>& other) const
	//	{
	//		cVectorND<T, DIMEMSION> temp;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			temp[unIndex] = m_Data[unIndex] * other[unIndex];
	//		}
	//		return temp;
	//	}

	//	cVectorND<T, DIMEMSION>& operator += (const cVectorND<T, DIMEMSION>& other)
	//	{
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] += other[unIndex];
	//		}

	//		return *this;
	//	}

	//	cVectorND<T, DIMEMSION>& operator -= (const cVectorND<T, DIMEMSION>& other)
	//	{
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] -= other[unIndex];
	//		}

	//		return *this;
	//	}

	//	cVectorND<T, DIMEMSION>& operator /= (const cVectorND<T, DIMEMSION>& other)
	//	{
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] /= other[unIndex];
	//		}

	//		return *this;
	//	}

	//	cVectorND<T, DIMEMSION>& operator *= (const cVectorND<T, DIMEMSION>& other)
	//	{
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] *= other[unIndex];
	//		}

	//		return *this;
	//	}

	//	cVectorND<T, DIMEMSION>& operator += (const T& num)
	//	{
	//		if (Equals(num, T(0)))
	//			return *this;

	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] += num;
	//		}

	//		return *this;
	//	}

	//	cVectorND<T, DIMEMSION>& operator -= (const T& num)
	//	{
	//		if (Equals(num, (T)0))
	//			return *this;

	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] -= num;
	//		}

	//		return *this;
	//	}

	//	cVectorND<T, DIMEMSION>& operator /= (const T& num)
	//	{
	//		if (Equals(num, (T)0))
	//			return *this;

	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] /= num;
	//		}

	//		return *this;
	//	}

	//	cVectorND<T, DIMEMSION>& operator *= (const T& num)
	//	{
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] *= num;
	//		}

	//		return *this;
	//	}

	//	cVectorND<T, DIMEMSION> operator + (const T& num)
	//	{
	//		if (Equals(num, (T)0))
	//			return *this;

	//		cVectorND<T, DIMEMSION> temp;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			temp[unIndex] = m_Data[unIndex] + num;
	//		}
	//		return temp;
	//	}

	//	cVectorND<T, DIMEMSION> operator - (const T& num)
	//	{
	//		if (Equals(num, (T)0))
	//			return *this;

	//		cVectorND<T, DIMEMSION> temp;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			temp[unIndex] = m_Data[unIndex] - num;
	//		}
	//		return temp;
	//	}

	//	cVectorND<T, DIMEMSION> operator / (const T& num)
	//	{
	//		if (Equals(num, (T)0))
	//			return *this;

	//		cVectorND<T, DIMEMSION> temp;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			temp[unIndex] = m_Data[unIndex] / num;
	//		}
	//		return temp;
	//	}

	//	cVectorND<T, DIMEMSION> operator * (const T& num)
	//	{

	//		cVectorND<T, DIMEMSION> temp;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			//temp[unIndex] = num;
	//			//*temp[unIndex] = m_Data[unIndex] + num
	//			temp[unIndex] = m_Data[unIndex] * num;
	//		}
	//		return temp;
	//	}

	//	//向量的大小
	//	T GetLength() const
	//	{
	//		T l = (T)0;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			l += m_Data[unIndex] * m_Data[unIndex];
	//		}
	//		return (T)Faster_Sqrt((float)(l));
	//	}

	//	//规格化
	//	cVectorND<T, DIMEMSION>& Normalize()
	//	{
	//		T l = (T)0;
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			l += m_Data[unIndex] * m_Data[unIndex];
	//		}
	//		if (l == 0)
	//			return *this;

	//		l = (T) Reciprocal_SquareRoot ( l );
	//		for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//		{
	//			m_Data[unIndex] *= l;
	//		}
	//		return *this;
	//	}

	//	////将vBorder作为边界 调整	bUpper = true是上边界；否则为下边界
	//	//cVectorND<T, DIMEMSION>& AdjustData(const cVectorND<T, DIMEMSION>& vBorder, bool bUpper = true)
	//	//{
	//	//	for (ui32 unIndex=0; unIndex<DIMEMSION; unIndex++)
	//	//	{
	//	//		if (bUpper)
	//	//			m_Data[unIndex] = m_Data[unIndex]>vBorder[unIndex] ? vBorder[unIndex] : m_Data[unIndex];
	//	//		else
	//	//			m_Data[unIndex] = m_Data[unIndex]<vBorder[unIndex] ? vBorder[unIndex] : m_Data[unIndex];
	//	//	}

	//	//	return *this;
	//	//}


	//private:
	//	T m_Data[DIMEMSION];
	//};

	//typedef cVectorND<float, eVecDimemsion_2> cVector2NDf;
	//typedef cVectorND<float, eVecDimemsion_3> cVector3NDf;


	//Ver1
	//通用转换函数cVectorND<T, eVecDimemsion_2 和 cVector2D<T>之间的互相转换；cVectorND<T, eVecDimemsion_3 和 cVector3D<T>之间的互相转换
	//template<class T>
	//cVector2D<T> TransVector2NDTo2D(const cVectorND<T, eVecDimemsion_2>& vDis)
	//{
	//	cVector2D<T> temp;
	//	for (ui32 unIndex=0; unIndex<eVecDimemsion_2; unIndex++)
	//	{
	//		temp[unIndex] = vDis[unIndex];
	//	}
	//	return temp;
	//}
	//template<class T>
	//	cVectorND<T, eVecDimemsion_2> TransVector2DTo2ND(const cVector2D<T>& vDis)
	//{
	//	cVectorND<T, eVecDimemsion_2> temp;
	//	for (ui32 unIndex=0; unIndex<eVecDimemsion_2; unIndex++)
	//	{
	//		temp[unIndex] = vDis[unIndex];
	//	}
	//	return temp;
	//}

	//template<class T>
	//	cVector3D<T> TransVector3NDTo3D(const cVectorND<T, eVecDimemsion_3>& vDis)
	//{
	//	cVector3D<T> temp;
	//	for (ui32 unIndex=0; unIndex<eVecDimemsion_3; unIndex++)
	//	{
	//		temp[unIndex] = vDis[unIndex];
	//	}
	//	return temp;
	//}
	//template<class T>
	//	cVectorND<T, eVecDimemsion_3> TransVector3DTo3ND(const cVector3D<T>& vDis)
	//{
	//	cVectorND<T, eVecDimemsion_3> temp;
	//	for (ui32 unIndex=0; unIndex<eVecDimemsion_3; unIndex++)
	//	{
	//		temp[unIndex] = vDis[unIndex];
	//	}
	//	return temp;
	//}

	//ver2
	//template<class T, class TN>
	//bool TransVector2NDTo2D(cVector2D<T>& vOut, const cVectorND<TN, eVecDimemsion_2>& vIn)
	//{
	//	for (ui32 unIndex=0; unIndex<eVecDimemsion_2; unIndex++)
	//	{
	//		vOut[unIndex] = (T)vIn[unIndex];
	//	}
	//	return true;
	//}
	//template<class T, class TN>
	//bool TransVector2DTo2ND(cVectorND<TN, eVecDimemsion_2>& vOut, const cVector2D<T>& vIn)
	//{
	//	for (ui32 unIndex=0; unIndex<eVecDimemsion_2; unIndex++)
	//	{
	//		vOut[unIndex] = (TN)vIn[unIndex];
	//	}
	//	return true;
	//}

	//template<class T, class TN>
	//bool TransVector3NDTo3D(cVector3D<T>& vOut, const cVectorND<TN, eVecDimemsion_3>& vIn)
	//{
	//	for (ui32 unIndex=0; unIndex<eVecDimemsion_3; unIndex++)
	//	{
	//		vOut[unIndex] = (T)vIn[unIndex];
	//	}
	//	return true;
	//}
	//template<class T, class TN>
	//bool TransVector3DTo3ND(cVectorND<TN, eVecDimemsion_3>& vOut, const cVector3D<T>& vIn)
	//{
	//	for (ui32 unIndex=0; unIndex<eVecDimemsion_3; unIndex++)
	//	{
	//		vOut[unIndex] = (TN)vIn[unIndex];
	//	}
	//	return true;
	//}

const ui32 g_cunMaxDimemsion = 4;		//最大维数
enum eVecDimemsion
{
	eVecDimemsion_1 = 1,
	eVecDimemsion_2 = 2,
	eVecDimemsion_3 = 3,
	eVecDimemsion_4 = 4,
};

template<class T>
class cVectorND
{
//protected:
//	cVectorND()
//	{
//		m_pData = 0;
//		m_unDimemsion = 0;
//	}
public:
	cVectorND(ui32 unDimemsion=eVecDimemsion_2)
	{
		//ini
		m_pData = 0;
		m_unDimemsion = 0;

		//check
		if (0==unDimemsion || unDimemsion>g_cunMaxDimemsion)
			return;

		//alloc
		m_unDimemsion = unDimemsion;

		m_pData = new T[m_unDimemsion];
		CrossMemory::ZeroMem(m_pData, sizeof(T)*m_unDimemsion);
	}
	~cVectorND()
	{
		Clear();
	}
	//cVectorND(ui32 unNum, ...)
	//{
	//	x = a; y = b;
	//}
	cVectorND(ui32 unDimemsion, const T* pData)
	{
		//ini
		m_pData = 0;
		m_unDimemsion = 0;

		if (0==unDimemsion || unDimemsion>g_cunMaxDimemsion || !pData)
			return;

		//alloc
		m_unDimemsion = unDimemsion;

		m_pData = new T[m_unDimemsion];
		CrossMemory::MemCpy(m_pData, pData, sizeof(T)*m_unDimemsion);
	}

	cVectorND(const cVectorND<T>& other)
	{
		if (!other.IsValid())
			return;

		m_unDimemsion = 0;
		m_pData = 0;

		*this = other;
	};

	ui32 GetDimension() const
	{
		return m_unDimemsion;
	}
	bool IsValid() const
	{
		return m_pData && 0!=m_unDimemsion && m_unDimemsion<=g_cunMaxDimemsion;
	}

	bool IsEmpty() const
	{
		if (!IsValid())		//当前数据无效，也算是 空！
			return true;

		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			if (!Equals(m_pData[unIndex], (T)0))
				return false;
		}
		return true;
	}
	bool Set0()
	{
		if (!IsValid())		//当前数据无效，也算是 空！
			return false;

		CrossMemory::ZeroMem(m_pData, sizeof(T)*m_unDimemsion);
		return true;
	}
	bool Clear()
	{
		m_unDimemsion = 0;

		if (m_pData)
			delete [] m_pData;
		m_pData = 0;

		return true;
	}
	//重新分配（先将原来数据清空）再重新分配，初始化
	bool ReAllot(ui32 unDimemsion)
	{
		if (0==unDimemsion || unDimemsion>g_cunMaxDimemsion)
			return false;

		//如果当前的数据时一致的，则不需要重新分配，单纯初始化一下就ok
		if (unDimemsion == m_unDimemsion)
		{
			if (Set0())
				return true;
		}
		Clear();

		m_unDimemsion = unDimemsion;

		m_pData = new T[m_unDimemsion];
		CrossMemory::ZeroMem(m_pData, sizeof(T)*m_unDimemsion);

		return true;
	}
	//T* operator [] ( ui32 i ) const
	//{
	//	if (!IsValid())
	//		return 0;

	//	if (i >= m_unDimemsion)
	//		return 0;

	//	return &m_pData[i];
	//}
	T operator [] ( ui32 i ) const
	{
		if (!IsValid())
			return (T)0;

		if (i >= m_unDimemsion)
			return (T)0;

		return m_pData[i];
	}
	bool SetData(ui32 i, T num)
	{
		if (!IsValid())
			return false;

		if (i >= m_unDimemsion)
			return false;

		m_pData[i] = num;
		return true;
	}
	bool SetData(T num)		//暂时加一个这个函数
	{
		if (!IsValid())
			return false;

		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			m_pData[unIndex] = num;
		}
		return true;
	}

 //	T* operator [] ( ui32 i ) const
	//{
	//	if (!IsValid())
	//		return 0;

	//	if (i >= m_unDimemsion)
	//		return 0;

	//	return &m_pData[i];
	//}
	const T* GetData() const
	{
		return m_pData;
	}

	//复值 如果维数不相等，则需要先重新分配
	cVectorND<T>& operator = (const cVectorND<T>& other)
	{
		if (!other.IsValid())
			return *this;
		//当前维数不相等时，需要重新分配
		if (m_unDimemsion != other.m_unDimemsion)
			ReAllot(other.m_unDimemsion);
		CrossMemory::MemCpy(m_pData, other.GetData(), sizeof(T)*m_unDimemsion);
		return *this;
	}

	bool operator == (const cVectorND<T>& other) const
	{
		if (IsValid() != other.IsValid())
			return false;

		if (!IsValid() && !other.IsValid())		//这里暂时返回true
			return true;

		if (m_unDimemsion != other.m_unDimemsion)
			return false;

		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
			if (!Equals(m_pData[unIndex], other[unIndex]))
				return false;
		return true;
	}

	//cVectorND<T> operator + (const cVectorND<T>& other) const
	//{
	//	if (!IsValid())
	//		return other;
	//	if (!other.IsValid())
	//		return *this;


	//	if (m_unDimemsion != other.m_unDimemsion)
	//	{
	//		cVectorND<T> temp(0);
	//		return temp;
	//	}
	//	else
	//	{
	//		cVectorND<T> temp(m_unDimemsion);
	//		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
	//		{
	//			*temp[unIndex] = m_pData[unIndex] + *other[unIndex];
	//		}
	//		return temp;
	//	}

	//	//这里不写返回值，可以么？
	//}
	//cVectorND<T> operator - (const cVectorND<T>& other) const
	//{
	//	if (!IsValid())
	//		return other;
	//	if (!other.IsValid())
	//		return *this;


	//	if (m_unDimemsion != other.m_unDimemsion)
	//	{
	//		cVectorND<T> temp(0);
	//		return temp;
	//	}
	//	else
	//	{
	//		cVectorND<T> temp(m_unDimemsion);
	//		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
	//		{
	//			*temp[unIndex] = m_pData[unIndex] - other[unIndex];
	//		}
	//		return temp;
	//	}

	//	//这里不写返回值，可以么？
	//}
	//cVectorND<T> operator / (const cVectorND<T>& other) const
	//{
	//	if (!IsValid())
	//		return other;
	//	if (!other.IsValid())
	//		return *this;


	//	if (m_unDimemsion != other.m_unDimemsion)
	//	{
	//		cVectorND<T> temp(0);
	//		return temp;
	//	}
	//	else
	//	{
	//		cVectorND<T> temp(m_unDimemsion);
	//		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
	//		{
	//			*temp[unIndex] = m_pData[unIndex] / other[unIndex];
	//		}
	//		return temp;
	//	}

	//	//这里不写返回值，可以么？
	//	return temp;
	//}

	//cVectorND<T> operator * (const cVectorND<T>& other) const
	//{
	//	if (!IsValid())
	//		return other;
	//	if (!other.IsValid())
	//		return *this;


	//	if (m_unDimemsion != other.m_unDimemsion)
	//	{
	//		cVectorND<T> temp(0);
	//		return temp;
	//	}
	//	else
	//	{
	//		cVectorND<T> temp(m_unDimemsion);
	//		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
	//		{
	//			*temp[unIndex] = m_pData[unIndex] * other[unIndex];
	//		}
	//		return temp;
	//	}

	//	//这里不写返回值，可以么？
	//}
	//计算时，两数值不能进行运算，则直接返回当前对象
	cVectorND<T> operator + (const cVectorND<T>& other) const
	{
		if (!IsValid())
			return *this;
		if (!other.IsValid())
			return *this;

		if (m_unDimemsion == other.m_unDimemsion)
		{
			cVectorND<T> temp(m_unDimemsion);
			for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
			{
				//*temp[unIndex] = m_pData[unIndex] + *other[unIndex];
				temp.SetData(unIndex, m_pData[unIndex]+other[unIndex]);
			}
			return temp;
		}

		return *this;
	}
	cVectorND<T> operator - (const cVectorND<T>& other) const
	{
		if (!IsValid() || !other.IsValid())
			return *this;

		if (m_unDimemsion == other.m_unDimemsion)
		{
			cVectorND<T> temp(m_unDimemsion);
			for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
			{
				//*temp[unIndex] = m_pData[unIndex] - *other[unIndex];
				temp.SetData(unIndex, m_pData[unIndex]-other[unIndex]);
			}
			return temp;
		}

		return *this;

		//这里不写返回值，可以么？
	}

	cVectorND<T> operator / (const cVectorND<T>& other) const
	{
		if (!IsValid() || !other.IsValid())
			return *this;

		if (m_unDimemsion == other.m_unDimemsion)
		{
			cVectorND<T> temp(m_unDimemsion);
			for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
			{
				//T num = *other[unIndex];
				//if (Equals(f, T(0)))
				//	return *this;
				//*temp[unIndex] = m_pData[unIndex] / *other[unIndex];
				if (!Equals(other[unIndex], T(0)))
					return *this;
				temp.SetData(unIndex, m_pData[unIndex] / other[unIndex]);
			}
			return temp;
		}

		return *this;
	}

	cVectorND<T> operator * (const cVectorND<T>& other) const
	{
		if (!IsValid() || !other.IsValid())
			return *this;

		if (m_unDimemsion == other.m_unDimemsion)
		{
			cVectorND<T> temp(m_unDimemsion);
			for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
			{
				//*temp[unIndex] = m_pData[unIndex] * *other[unIndex];
				temp.SetData(unIndex, m_pData[unIndex] * other[unIndex]);
			}
			return temp;
		}

		return *this;
	}

	cVectorND<T>& operator += (const cVectorND<T>& other)
	{
		if (!IsValid() || other.IsValid() || m_unDimemsion!=other.m_unDimemsion)
			return *this;

		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			//m_pData[unIndex] += *other[unIndex];
			m_pData[unIndex] += other[unIndex];
		}

		return *this;
	}

	cVectorND<T>& operator -= (const cVectorND<T>& other)
	{
		if (!IsValid() || other.IsValid() || m_unDimemsion!=other.m_unDimemsion)
			return *this;


		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			//m_pData[unIndex] -= *other[unIndex];
			m_pData[unIndex] -= other[unIndex];
		}

		return *this;
	}

	cVectorND<T>& operator /= (const cVectorND<T>& other)
	{
		if (!IsValid() || other.IsValid() || m_unDimemsion!=other.m_unDimemsion)
			return *this;


		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			//T num = *other[unIndex];
			//if (Equals(f, T(0)))
			//	return *this;

			//m_pData[unIndex] /= *other[unIndex];

			if (Equals(other[unIndex], T(0)))
				return *this;

			m_pData[unIndex] /= other[unIndex];
		}

		return *this;
	}

	cVectorND<T>& operator *= (const cVectorND<T>& other)
	{
		if (!IsValid() || other.IsValid() || m_unDimemsion!=other.m_unDimemsion)
			return *this;


		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			//m_pData[unIndex] *= *other[unIndex];
			m_pData[unIndex] *= other[unIndex];
		}

		return *this;
	}

	cVectorND<T>& operator += (const T& num)
	{
		if ( !IsValid() )
			return *this;

		if (Equals(num, (T)0))
			return *this;

		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			m_pData[unIndex] += num;
		}

		return *this;
	}

	cVectorND<T>& operator -= (const T& num)
	{
		if ( !IsValid() )
			return *this;

		if (Equals(num, (T)0))
			return *this;

		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			m_pData[unIndex] -= num;
		}

		return *this;
	}

	cVectorND<T>& operator /= (const T& num)
	{
		if ( !IsValid() )
			return *this;

		if (Equals(num, (T)0))
			return *this;

		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			m_pData[unIndex] /= num;
		}

		return *this;
	}

	cVectorND<T>& operator *= (const T& num)
	{
		if ( !IsValid() )
			return *this;

		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			m_pData[unIndex] *= num;
		}

		return *this;
	}

	cVectorND<T> operator + (const T& num)
	{
		if ( !IsValid() )
			return *this;

		if (Equals(num, (T)0))
			return *this;

		cVectorND<T> temp(m_unDimemsion);
		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			//*temp[unIndex] = m_pData[unIndex] + num;
			temp.SetData(unIndex, m_pData[unIndex] + num);
		}
		return temp;
	}

	cVectorND<T> operator - (const T& num)
	{
		if ( !IsValid() )
			return *this;

		if (Equals(num, (T)0))
			return *this;

		cVectorND<T> temp(m_unDimemsion);
		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			//*temp[unIndex] = m_pData[unIndex] - num;
			temp.SetData(unIndex, m_pData[unIndex] - num);
		}
		return temp;
	}

	cVectorND<T> operator / (const T& num)
	{
		if ( !IsValid() )
			return *this;

		if (Equals(num, (T)0))
			return *this;

		cVectorND<T> temp(m_unDimemsion);
		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			//*temp[unIndex] = m_pData[unIndex] / num;
			temp.SetData(unIndex, m_pData[unIndex] / num);
		}
		return temp;
	}

	cVectorND<T> operator * (const T& num)
	{
		if ( !IsValid() )
			return *this;

		cVectorND<T> temp(m_unDimemsion);
		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			//*temp[unIndex] = m_pData[unIndex] * num;
			temp.SetData(unIndex, m_pData[unIndex] * num);
		}
		return temp;
	}

	//向量的大小
	T GetLength() const
	{
		if (!IsValid())
			return (T)0;

		T l = (T)0;
		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			l += m_pData[unIndex] * m_pData[unIndex];
		}
		return (T)CrossMath::Sqrt((float)(l));
	}

	//规格化
	cVectorND<T>& Normalize()
	{
		if (!IsValid())
			return *this;

		T l = (T)0;
		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			l += m_pData[unIndex] * m_pData[unIndex];
		}
		if (l == 0)
			return *this;

		l = (T) ( 1 / CrossMath::Sqrt(l) );
		for (ui32 unIndex=0; unIndex<m_unDimemsion; unIndex++)
		{
			m_pData[unIndex] *= l;
		}
		return *this;
	}


private:
	ui32 m_unDimemsion;

	T* m_pData;
};

	//ver3

	//static const int eVecDimemsion_1 = 1;
	////static const int eVecDimemsion_2 = 2;
	//static const int eVecDimemsion_3 = 3;
	template<class T, class TN>
	bool TransVector2NDTo2D(cVector2D<T>& vOut, const cVectorND<TN>& vIn)
	{
		if (eVecDimemsion_2 != vIn.GetDimension())
			return false;

		for (ui32 unIndex=0; unIndex<eVecDimemsion_2; unIndex++)
		{
			//vOut[unIndex] = (T)(*vIn[unIndex]);
			vOut[unIndex] = (T)vIn[unIndex];
		}
		return true;
	}
	template<class T, class TN>
	bool TransVector2DTo2ND(cVectorND<TN>& vOut, const cVector2D<T>& vIn)
	{
		if (eVecDimemsion_2 != vOut.GetDimension())
			return false;

		for (ui32 unIndex=0; unIndex<eVecDimemsion_2; unIndex++)
		{
			//*vOut[unIndex] = (TN)vIn[unIndex];
			vOut.SetData(unIndex, (TN)vIn[unIndex]);
		}
		return true;
	}

	template<class T, class TN>
	bool TransVector3NDTo3D(cVector3D<T>& vOut, const cVectorND<TN>& vIn)
	{
		if (eVecDimemsion_3 != vIn.GetDimension())
			return false;

		for (ui32 unIndex=0; unIndex<eVecDimemsion_3; unIndex++)
		{
			//vOut[unIndex] = (T)(*vIn[unIndex]);
			vOut[unIndex] = (T)vIn[unIndex];
		}
		return true;
	}
	template<class T, class TN>
	bool TransVector3DTo3ND(cVectorND<TN>& vOut, const cVector3D<T>& vIn)
	{
		if (eVecDimemsion_3 != vOut.GetDimension())
			return false;

		for (ui32 unIndex=0; unIndex<eVecDimemsion_3; unIndex++)
		{
			//*vOut[unIndex] = (TN)vIn[unIndex];
			vOut.SetData(unIndex, (TN)vIn[unIndex]);
		}
		return true;
	}

typedef cVectorND<float>		cVectorNDf;

}

typedef cm::cVectorNDf	vectorNd;

#endif

