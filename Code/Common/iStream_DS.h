#ifndef cross_common_istream_array_h
#define cross_common_istream_array_h

#include "Common.h"
#include "iStream.h"
#include <vector>
#include <map>
#include <set>

namespace cross
{

static const ui32 c_nIStream_DS_MaxLen = 1000 * 1024;

template <typename T>
inline cOStream& operator << (cOStream& os, T* p)
{
	if (p == NULL)
		return os << ((i8)0);
	os << ((i8)1);
	return os << (*p);
}

template <typename T>
inline cIStream& operator >> (cIStream& is, T*& p) 
{
	i8 flag;
	is >> flag;
	if (flag == 1) {
		p = new T;
		return is >> (*p);
	}
	else
		p = NULL;
	return is;
}

//array
template <typename T>
inline cOStream& operator << (cOStream& os, const std::vector<T>& c)
{
	ui32 csz = (ui32)c.size();
	if (csz > c_nIStream_DS_MaxLen)
		throw (csz);
	os << csz;
	for (ui32 i = 0; i < csz; i++)
		os << c[i];
	return os;
}
template <typename T>
inline cIStream& operator >> (cIStream& is, std::vector<T>& c)
{
	ui32 size = c_nIStream_DS_MaxLen + 1;
	is >> size;
	if (size > c_nIStream_DS_MaxLen)
		throw (size);
	c.clear();
	c.resize(size);
	for (ui32 i = 0; i < c.size(); ++i)
		is >> c[i];
	return is;
}
template <typename T>
inline cOStream& array_to_stream(cOStream& os, const std::vector<T>& c)
{
	ui32 csz = (ui32)c.size();
	os << csz;
	if (csz)
		os.Write(&c.front(), csz * sizeof(T));
	return os;
}
template <typename T>
inline cIStream& array_from_stream(cIStream& is, std::vector<T>& c)
{
	ui32 csz = 0;
	is >> csz;
	if (csz) {
		c.resize(csz);
		is.Read(&c.front(), csz * sizeof(T));
	}
	else
		c.clear();
	return is;
}

#define vector_to_stream array_to_stream
#define vector_from_stream array_from_stream

//set
template <typename V>
inline cOStream& operator << (cOStream& os, const std::set<V>& c)
{
	ui32 csz = (ui32)c.size();
	if (csz > c_nIStream_DS_MaxLen)
		throw (csz);
	os << csz;
	typename std::set<V>::const_iterator i = c.begin();
	for (; i != c.end(); i++)
		os << (*i);
	return os;
}
template <typename V>
inline cIStream& operator >> (cIStream& is, std::set<V>& c)
{
	ui32 size = c_nIStream_DS_MaxLen + 1;
	is >> size;
	if (size > c_nIStream_DS_MaxLen)
		throw (size);
	c.clear();
	for (ui32 i = 0; i < size; ++i) {
		V v;
		is >> v;
		c.insert(v);
	}
	return is;
}

//multiset
template <typename V>
inline cOStream& operator << (cOStream& os, const std::multiset<V>& c)
{
	ui32 csz = (ui32)c.size();
	if (csz > c_nIStream_DS_MaxLen)
		throw (csz);
	os << csz;
	typename std::multiset<V>::const_iterator i = c.begin();
	for (;i != c.end(); i++)
		os << (*i);
	return os;
}
template <typename V>
inline cIStream& operator >> (cIStream& is, std::multiset<V>& c)
{
	ui32 size = c_nIStream_DS_MaxLen + 1;
	is >> size;
	if (size > c_nIStream_DS_MaxLen)
		throw (size);
	c.clear();
	for (ui32 i = 0; i < size; ++i) {
		V v;
		is >> v;
		c.insert(v);
	}
	return is;
}

//map
template <typename K, typename V>
inline cOStream& operator << (cOStream& os, const std::map<K,V>& c)
{
	ui32 csz = (ui32)c.size();
	if (csz > c_nIStream_DS_MaxLen)
		throw (csz);
	os << csz;
	typename std::map<K,V>::const_iterator i = c.begin();
	for (;i != c.end(); i++) {
		os << i->first;
		os << i->second;
	}
	return os;
}
template <typename K, typename V>
inline cIStream& operator >> (cIStream& is, std::map<K,V>& c)
{
	ui32 size = c_nIStream_DS_MaxLen + 1;
	is >> size;
	if (size > c_nIStream_DS_MaxLen)
		throw (size);
	c.clear();
	for (ui32 i = 0; i < size; ++i) {
		K k; V v;
		is >> k;
		is >> v;
		c.insert(std::make_pair(k, v));
	}
	return is;
}

//multimap
template <typename K, typename V>
inline cOStream& operator << (cOStream& os, const std::multimap<K,V>& c)
{
	ui32 csz = (ui32)c.size();
	if (csz > c_nIStream_DS_MaxLen)
		throw (csz);
	os << csz;
	typename std::multimap<K,V>::const_iterator i = c.begin();
	for (;i != c.end(); i++) {
		os << i->first;
		os << i->second;
	}
	return os;
}
template <typename K, typename V>
inline cIStream& operator >> (cIStream& is, std::multimap<K,V>& c)
{
	ui32 size = c_nIStream_DS_MaxLen + 1;
	is >> size;
	if (size > c_nIStream_DS_MaxLen)
		throw (size);
	c.clear();
	for (ui32 i = 0; i < size; ++i) {
		K k; V v;
		is >> k;
		is >> v;
		c.insert(std::make_pair(k, v));
	}
	return is;
}

}
#endif
