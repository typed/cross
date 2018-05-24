#include "cLog.h"
#include "../inc/CrossString.h"
#include "../inc/CrossFileOP.h"
#include "../inc/cClock.h"

using namespace std;

namespace cm
{

cLog::cLog()
{
	m_bOutputFile = true;
	m_bCout = true;
	m_bOpen = false;
}

cLog::~cLog()
{
	Close();
}

void cLog::Close()
{
	m_ofs.close();
}

bool cLog::Create(cpstr name)
{
	m_sName = name;
	if (!m_bOutputFile)
		return true;
	string sTime;
	sTime = GetCurDateTimeString();
	string sGroup = GetLogManager().m_sModelName;
	string s = "log/";
	if (!sGroup.empty())
		s += sGroup + "/";
	s += m_sName + "/" + sTime + ".log";
	if (!CrossFileOP::CreateDirPathIgnoreFile(s))
		return false;
	m_ofs.close();
	m_ofs.open(s.c_str(), ios_base::out);
	m_bOpen = m_ofs.is_open();
	if (!m_bOpen)
		return false;
	m_strFile = s;
	return true;
}

void cLog::SetOutputFile(bool bOF)
{
	if (m_bOutputFile == bOF)
		return;
	LOCK(GetLogManager().m_mutex);
	if (m_bOutputFile)
		m_ofs.close();
	m_bOutputFile = bOF;
	if (m_bOutputFile) {
		Create(m_sName.c_str());
	}
}

iLog& cLog::operator << (f32 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}

iLog& cLog::operator << (f64 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}

iLog& cLog::operator << (bool nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}

iLog& cLog::operator << (i8 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}

iLog& cLog::operator << (ui8 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}

iLog& cLog::operator << (i16 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}

iLog& cLog::operator << (ui16 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return (*this);
}

iLog& cLog::operator << (i32 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}

iLog& cLog::operator << (ui32 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return (*this);
}

iLog& cLog::operator << (ul32 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}
iLog& cLog::operator << (l32 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return *this;
}

iLog& cLog::operator << (i64 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return (*this);
}

iLog& cLog::operator << (ui64 nValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << nValue;
		if (IsCout())
			cout << nValue;
	}
	return (*this);
}

iLog& cLog::operator << (cpstr sValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << sValue;
		if (IsCout())
			cout << sValue;
	}
	return *this;
}

iLog& cLog::operator << (const string& sValue)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (m_bOutputFile)
			m_ofs << sValue;
		if (IsCout())
			cout << sValue;
	}
	return *this;
}

inline void SetCoutCol(iLog::eLog_CoutColor lc)
{
	switch(lc) {
	case iLog::LC_White:
		CrossConsole::SetColor(CrossConsole::CC_White); break;
	case iLog::LC_Yellow:
		CrossConsole::SetColor(CrossConsole::CC_Yellow); break;
	case iLog::LC_Red:
		CrossConsole::SetColor(CrossConsole::CC_Red); break;
	case iLog::LC_Green:
		CrossConsole::SetColor(CrossConsole::CC_Green); break;
	case iLog::LC_Blue:
		CrossConsole::SetColor(CrossConsole::CC_Blue); break;
	case iLog::LC_Cyan:
		CrossConsole::SetColor(CrossConsole::CC_Cyan); break;
	default:
		CrossConsole::SetColor(CrossConsole::CC_White); break;
	}
}

iLog& cLog::operator << (eLog_CoutColor lc)
{
	if (IsCout()) {
		SetCoutCol(lc);
	}
	return *this;
}

void cLog::operator << (eLog_State state)
{
	if (IsCout() || m_bOutputFile) {
		LOCK(GetLogManager().m_mutex);
		if (state == Log_Enter)
			Enter();
		else if (state == Log_Flush)
			Flush();
	}
}

void cLog::Enter()
{
	if (m_bOutputFile)
		m_ofs << endl;
	if (IsCout())
		cout << endl;
	operator << (LC_White);
}

void cLog::Flush()
{
	if (m_bOutputFile)
		m_ofs.flush();
	if (IsCout())
		cout.flush();
}

cLogManager::cLogManager()
{
	m_bCloseAllLog = false;
}

cLogManager::~cLogManager()
{
	for (itLog i = m_hmLog.begin(); i != m_hmLog.end(); i++)
		delete i->second;
	m_hmLog.clear();
}

void cLogManager::Init()
{
	m_sModelName = CrossString::ToLower(CrossFileOP::GetModuleName());
	iLog* pLog = new cLog();
#if (defined(WIN32) || defined(_WIN32)) // || defined(__linux__) ) 
	pLog->SetCout(true);
	pLog->SetOutputFile(false);
#elif (defined(__linux__) )
	pLog->SetCout(true);
	pLog->SetOutputFile(false);
#else
	pLog->SetCout(false);
	pLog->SetOutputFile(false);
#endif
	pLog->Create(DLog);
	m_hmLog[DLog] = pLog;
}

iLog* cLogManager::GetLog(cpstr sName)
{
	hmLog::iterator pN = m_hmLog.find(sName);
	if (pN != m_hmLog.end())
		return pN->second;
	iLog* pLog = new cLog();
#if (defined(WIN32) || defined(_WIN32) ) //|| defined(__linux__))
	pLog->SetCout(true);
	pLog->SetOutputFile(true);
#elif (defined(__linux__) )
	pLog->SetCout(true);
	pLog->SetOutputFile(false);
#else
	pLog->SetCout(false);
	pLog->SetOutputFile(false);
#endif
	if (!pLog->Create(sName)) {
		delete pLog;
		return 0;
	}
	m_hmLog[sName] = pLog;
	return pLog;
}

iLog* cLogManager::FindLog(cpstr sName)
{
	hmLog::iterator pN = m_hmLog.find(sName);
	if (pN != m_hmLog.end())
		return pN->second;
	return 0;
}

void cLogManager::CloseAllLog()
{
	m_bCloseAllLog = true;
}

void InitLog(bool bCloseAllLog)
{
	if (bCloseAllLog)
		GetLogManager().CloseAllLog();
	GetLogManager().Init();
}

cLogManager& GetLogManager()
{
	static cLogManager s_lm;
	return s_lm;
}

iLog* FindLog(cpstr sName)
{
	return GetLogManager().FindLog(sName);
}

iLog* GetLog(cpstr sName)
{
	return GetLogManager().GetLog(sName);
}

iLog& LogOut(iLog* log, iLog::eLog_Type type)
{
	static cpstr c_sLogType[] = {
		"info",
		"warning",
		"error",
		"ok"
	};
	cLog* pLog = (cLog*)log;
	if (pLog->IsOutputFile())
		pLog->m_ofs << GetCurDateTimeString() << " [" << c_sLogType[type] << "] ";
	if(pLog->IsCout())
		cout << GetCurDateTimeString() << " [" << c_sLogType[type] << "] ";
	return (*log);
}

}
