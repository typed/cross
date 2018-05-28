//�������ƣ�Cross.h
//������������ƽ̨�м��
//�����ˣ�	������
//����ʱ�䣺2012-11-9
//�޸��ߣ�
//�޸�ʱ�䣺
//�޸�˵����

#ifndef crossstl_h
#define crossstl_h

#include "CommonDef.h"
#include <vector>
#include <algorithm>

namespace cm
{

//STL�򻯺���
template<typename T> inline bool VectorRemove(typename std::vector<T>& a, const T& v) {
	typename std::vector<T>::iterator it = std::find(a.begin(), a.end(), v);
	if (it == a.end())
		return false;
	a.erase(it);
	return true;
}

template<typename T> inline bool VectorIsIn(const typename std::vector<T>& a, const T& v) {
	return std::find(a.begin(), a.end(), v) != a.end();
}

template<typename T> inline bool VectorMoveBegin(typename std::vector<T>& a, T v) {
	typename std::vector<T>::iterator it = std::find(a.begin(), a.end(), v);
	if (it == a.end())
		return false;
	a.erase(it);
	a.insert(a.begin(), v);
	return true;
}

template<typename T> inline bool VectorMoveEnd(typename std::vector<T>& a, T v) {
	typename std::vector<T>::iterator it = std::find(a.begin(), a.end(), v);
	if (it == a.end())
		return false;
	a.erase(it);
	a.push_back(v);
	return true;
}

template<typename T> inline void VectorPopFront(typename std::vector<T>& a) {
	if (a.empty())
		return;
	a.erase(a.begin());
}

template<typename T> inline void VectorInsertOnly(typename std::vector<T>& a, const T& v) {
	if (VectorIsIn(a, v))
		return;
	a.push_back(v);
}

template<typename T> inline bool VectorInsertOnlyBool(typename std::vector<T>& a, const T& v) {
	if (VectorIsIn(a, v))
		return false;
	a.push_back(v);
	return true;
}

template<typename T> inline bool VectorNext(typename std::vector<T>& a, T& v) {
	if (a.empty())
		return false;
	typename std::vector<T>::iterator it = std::find(a.begin(), a.end(), v);
	if (it == a.end())
		return false;
	++it;
	if (it == a.end())
		v = *a.begin();
	else
		v = *it;
	return true;
}

}

#endif