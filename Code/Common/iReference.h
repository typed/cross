#ifndef ireference_h
#define ireference_h
#include "CommonDef.h"

#ifdef _DEBUG
#	define TRACE_REF
#endif

#ifdef TRACE_REF
#	define SafeAddRef(p) { if ( (p) != 0 ) { (p)->AddRef(__FILE__, __FUNCTION__, __LINE__); } }
#else
#	define SafeAddRef(p) { if ( (p) != 0 ) { (p)->AddRef(); } }
#endif
#define SafeRelease(p) { if ( (p) != 0 ) { (p)->Release(); (p) = 0; } }
#define SafeDelete(p) { if ( (p) != 0 ) { delete (p); (p) = 0; } }
#define SafeDeleteArray(p) { if ( (p) != 0 ) { delete[] (p); (p) = 0; } }
#define SafeFree(x)	{ if( (x)!=0 ) { free(x); (x)=0; } }
#define SafeD(d,x)	{ if( (x)!=0 ) { d(x); (x)=0; } }
#define SafeD2(d,x)	{ if( (x)!=0 ) { (x)->d; (x)=0; } }
#define Saferelease(p) { if ( (p) != 0 ) { (p)->release(); (p) = 0; } }

namespace cm
{

class iReference
{
public:

	iReference() : m_nRef(0) {
	}

#ifdef TRACE_REF
	virtual ~iReference();
#else
	virtual ~iReference() {
	}
#endif

	int GetRefCount() const {
		return m_nRef;
	}

#ifdef TRACE_REF
	void TraceError();
#else
	void TraceError() {}
#endif

	bool Release() {
		if (m_nRef <= 0) {
			//´íÎó£¡0ÒýÓÃÊÍ·Å£¡
			RefError();
			return false;
		}
		--m_nRef;
		if (m_nRef == 0) {
			OnDestroy();
			return true;
		}
		return false;
	}

#ifdef TRACE_REF
	void AddRef(cpstr file, cpstr func, ui32 line);
#else
	void AddRef() {
		++m_nRef;
	}
#endif

private:

	virtual void OnDestroy() {
		delete this;
	}

	void RefError() {
#ifdef _DEBUG
		//__asm
		//{
		//	int 3;
		//}
#endif
	}

	int m_nRef;

};

template<class T>
class cRefPtr
{
public:
	cRefPtr() : m_p(0) {}
	cRefPtr(T* t) : m_p(t) {
		SafeAddRef(m_p);
	}
	cRefPtr(const cRefPtr& r) : m_p(r.m_p) {
		SafeAddRef(m_p);
	}
	~cRefPtr() {
		SafeRelease(m_p);
	}

	cRefPtr& operator = (T* q) {
		if (m_p != q) {
			T* t = m_p;
			m_p = q;
			SafeAddRef(m_p);
			SafeRelease(t);
		}
		return *this;
	}

	cRefPtr& operator = (const cRefPtr& r) {
		if (m_p != r.m_p) {
			T* t = m_p;
			m_p = r.m_p;
			SafeAddRef(m_p);
			SafeRelease(t);
		}
		return *this;
	}

	bool operator == (const cRefPtr& r) const { return m_p == r.m_p; }
	bool operator != (const cRefPtr& r) const { return m_p != r.m_p; }
	bool operator < (const cRefPtr& r) const { return m_p <  r.m_p; }
	bool operator > (const cRefPtr& r) const { return m_p >  r.m_p; }

	bool operator == (const T* q) const { return m_p == q; }
	bool operator != (const T* q) const { return m_p != q; }
	bool operator < (const T* q) const { return m_p <  q; }
	bool operator > (const T* q) const { return m_p >  q; }


	T& operator*() { return *m_p; }
	const T& operator*() const	{ return *m_p; }

	T* operator->() { return m_p; }
	const T* operator->() const { return m_p; }

	T* Get() { return m_p; }
	const T* Get() const { return m_p; }

	bool operator!() const { return m_p == 0; }
	bool IsValid() const { return m_p != 0; }

	T* Release() {
		T* t = m_p;
		SafeRelease(m_p);
		return t;
	}

private:
	T* m_p;
};

}



#endif
