#ifndef cross_common_cthread_h
#define cross_common_cthread_h

#include "Common.h"
#include <string>
#include <functional>

namespace cross
{

class cMutex
{
public:
	cMutex();
	~cMutex();
	void Lock();
	void Unlock();
private:
	pvd m_mt;
};

class cMutexBind
{
public:
	cMutexBind(cMutex& m) : m_mt(m) {
		m_mt.Lock();
	}
	~cMutexBind() {
		m_mt.Unlock();
	}
private:
	cMutex& m_mt;
};

class cThread
{
public:
	cThread(std::function<void(cThread*)> func);
	~cThread();
	void End(b8 bWait = true);
	bool IsNeedEnd() const;
	static std::string GetThisThreadID();
	static void Sleep(ui32 tm);
private:
	pvd	m_pThread;
	b8	m_bNeedEnd;
};

}

#define MUTEX cross::cMutex
#define LOCK(mt) cross::cMutexBind lk(mt)
#define SLEEP(tm) cross::cThread::Sleep(tm)

#endif
