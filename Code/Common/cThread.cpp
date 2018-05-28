#include "cThread.h"

#if (defined(WIN32) || defined(_WIN32))
#	ifndef intmax_t
	typedef signed long long intmax_t;
#	endif
#	ifndef uintmax_t
	typedef unsigned long long uintmax_t;
#	endif
#endif
#include <thread>
#include <mutex>
#include <sstream>

namespace cross
{

cMutex::cMutex()
{
	m_mt = new std::mutex();
}

cMutex::~cMutex()
{
	delete ((std::mutex*)m_mt);
}

void cMutex::Lock()
{
	((std::mutex*)m_mt)->lock();
}

void cMutex::Unlock()
{
	((std::mutex*)m_mt)->unlock();
}

cThread::cThread(std::function<void(cThread*)> func)
{
	m_bNeedEnd = false;
	m_pThread = new std::thread(func, this);
}

cThread::~cThread()
{
	End();
}

void cThread::End(b8 bWait)
{
	m_bNeedEnd = true;
	if (m_pThread) {
		std::thread* p = (std::thread*)m_pThread;
		if (bWait)
			p->join();
		else
			p->detach();
		delete p;
		m_pThread = nullptr;
	}
}

bool cThread::IsNeedEnd() const
{
	return m_bNeedEnd;
}

std::string cThread::GetThisThreadID()
{
	std::stringstream sid;
	sid << std::this_thread::get_id();
	return sid.str();
}

void cThread::Sleep(ui32 tm)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(tm));
}

}
