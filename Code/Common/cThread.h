#ifndef cthread_h
#define cthread_h

#include "CommonDef.h"
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

#define MUTEX cm::cMutex
#define LOCK(mt) cm::cMutexBind lk(mt)
#define SLEEP(tm) cm::cThread::Sleep(tm)

#endif
