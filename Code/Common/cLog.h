#ifndef clog_h
#define clog_h

#include "../inc/iLog.h"
#include "../inc/CrossConsole.h"
#include "../inc/cThread.h"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

namespace cm
{

class cLog :
	public iLog
{
public:
	cLog();
	~cLog();
	bool Create(cpstr name);
	void Close();

	iLog& operator << (bool nValue);
	iLog& operator << (i8 nValue);
	iLog& operator << (ui8 nValue);
	iLog& operator << (i16 nValue);
	iLog& operator << (ui16 nValue);
	iLog& operator << (i32 nValue);
	iLog& operator << (ui32 nValue);
	iLog& operator << (ul32 nValue);
	iLog& operator << (l32 nValue);
	iLog& operator << (i64 nValue);
	iLog& operator << (ui64 nValue);
	iLog& operator << (f32 nValue);
	iLog& operator << (f64 nValue);
	iLog& operator << (cpstr sValue);
	iLog& operator << (const std::string& sValue);
	iLog& operator << (eLog_CoutColor lc);
	void operator << (eLog_State state);
	
	void SetCout(bool bCout) {m_bCout = bCout;}
	bool IsCout() const {return m_bCout;}

	void SetOutputFile(bool bOF);
	bool IsOutputFile() const {return m_bOutputFile;}

	void Enter();
	void Flush();

	bool						m_bCout;
	bool						m_bOutputFile;
	bool						m_bOpen;
	std::ofstream				m_ofs;
	std::string					m_sName;
	std::string					m_strFile;
};

typedef std::map<std::string, iLog*> hmLog;
typedef hmLog::iterator itLog;

class cLogManager
{
public:
	cLogManager();
	~cLogManager();
	void Init();
	iLog* GetLog(cpstr sName);
	iLog* FindLog(cpstr sName);
	void CloseAllLog();
	hmLog						m_hmLog;
	std::string					m_sModelName;
	MUTEX						m_mutex;
	bool						m_bCloseAllLog;
};

cLogManager& GetLogManager();

}
#endif
