#ifndef cross_common_ilog_h
#define cross_common_ilog_h

#include "Common.h"
#include "CrossApp.h"
#include <string>

namespace cross
{

class iLog
{
public:

	enum eLog_Type
	{
		LT_Info,
		LT_Warning,
		LT_Error,
		LT_Ok
	};

	enum eLog_State
	{
		Log_Enter,
		Log_Flush,
	};

	enum eLog_CoutColor
	{
		LC_White,
		LC_Yellow,
		LC_Red,
		LC_Green,
		LC_Blue,
		LC_Cyan,
	};

	virtual ~iLog() {}

	virtual bool Create(cpstr name) = 0;

	virtual iLog& operator << (bool nValue) = 0;
	virtual iLog& operator << (i8 nValue) = 0;
	virtual iLog& operator << (ui8 nValue) = 0;
	virtual iLog& operator << (i16 nValue) = 0;
	virtual iLog& operator << (ui16 nValue) = 0;
	virtual iLog& operator << (i32 nValue) = 0;
	virtual iLog& operator << (ui32 nValue) = 0;
	virtual iLog& operator << (ul32 nValue) = 0;
	virtual iLog& operator << (l32 nValue) = 0;
	virtual iLog& operator << (i64 nValue) = 0;
	virtual iLog& operator << (ui64 nValue) = 0;
	virtual iLog& operator << (f32 nValue) = 0;
	virtual iLog& operator << (f64 nValue) = 0;
	virtual iLog& operator << (cpstr sValue) = 0;
	virtual iLog& operator << (const std::string& sValue) = 0;
	virtual iLog& operator << (eLog_CoutColor lc) = 0;
	virtual void operator << (eLog_State state) = 0;

	//是否需要输出到控制台，默认为false
	virtual void SetCout(bool bCout) = 0;
	virtual bool IsCout() const = 0;

	//是否需要写磁盘，默认为true
	virtual void SetOutputFile(bool bOF) = 0;
	virtual bool IsOutputFile() const = 0;

};

//创建与销毁单独日志
//iLog* CreateALog();
//void DestroyALog(iLog*);

//直接查找日志，如果没有就返回0
iLog* FindLog(cpstr sName);

//查找日志，如果没有就创建一个新的日志
iLog* GetLog(cpstr sName);

void InitLog(bool bCloseAllLog = false);

//详细日志
iLog& LogOut(iLog* log, iLog::eLog_Type type);

}

#define DLog "defaultlog"
#define LogEnter cross::iLog::Log_Enter
#define LogFlush cross::iLog::Log_Flush
#define Endl "\n"
#define LEnd LogEnter
#define LFlush LogFlush

#define LCWhite cross::iLog::LC_White
#define LCYellow cross::iLog::LC_Yellow
#define LCRed cross::iLog::LC_Red
#define LCGreen cross::iLog::LC_Green
#define LCBlue cross::iLog::LC_Blue
#define LCCyan cross::iLog::LC_Cyan

#define LogInfoPtr(plog) cross::LogOut(plog, cross::iLog::LT_Info) << cross::iLog::LC_White
#define LogWarningPtr(plog) cross::LogOut(plog, cross::iLog::LT_Warning) << cross::iLog::LC_Yellow
#define LogErrorPtr(plog) cross::LogOut(plog, cross::iLog::LT_Error) << cross::iLog::LC_Red
#define LogOkPtr(plog) cross::LogOut(plog, cross::iLog::LT_Ok) << cross::iLog::LC_Green

#define LogInfo(logName) cross::LogOut(cross::GetLog(logName), cross::iLog::LT_Info) << cross::iLog::LC_White
#define LogWarning(logName) cross::LogOut(cross::GetLog(logName), cross::iLog::LT_Warning) << cross::iLog::LC_Yellow
#define LogError(logName) cross::LogOut(cross::GetLog(logName), cross::iLog::LT_Error) << cross::iLog::LC_Red
#define LogOk(logName) cross::LogOut(cross::GetLog(logName), cross::iLog::LT_Ok) << cross::iLog::LC_Green

#define LInfo LogInfo(DLog)
#define LWarning LogWarning(DLog)
#define LError LogError(DLog)
#define LOk LogOk(DLog)
#define LCyan LogInfo(DLog) << LCCyan
#define LBlue LogInfo(DLog) << LCBlue

#define LEnter LInfo << LEnd

//Assert

#ifdef Assert
#undef Assert
#endif

#ifdef _DEBUG
//#define Assert(c) if(c) 0; else cross::CrossApp::Assert4(#c, __FILE__, __FUNCTION__, __LINE__)
#	include <assert.h>
#	define Assert(c) assert(c)
#else
#	define Assert(c) 0
#endif

#ifdef _DEBUG
#define ASSERTMSGF(error) if (!(error)) {LError << "[Assert] " << #error << " " << __FUNCTION__ << ":" << __LINE__ << LogEnter; Assert(error);}
#define ASSERTMSGFR(error,rt) if (!(error)) {LError << "[Assert] " << #error << " " << __FUNCTION__ << ":" << __LINE__ << LogEnter; Assert(error); rt;}
#define ASSERTMSG1(error,msg) if (!(error)) {LError << "[Assert] " << msg << LogEnter; Assert(error);}
#define ASSERTMSG2(error,msg1,msg2) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << LogEnter; Assert(error);}
#define ASSERTMSG3(error,msg1,msg2,msg3) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << " " << msg3 << LogEnter; Assert(error);}
#define ASSERTMSG4(error,msg1,msg2,msg3,msg4) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << LogEnter; Assert(error);}
#define ASSERTMSG5(error,msg1,msg2,msg3,msg4,msg5) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " " << msg5 << LogEnter; Assert(error);}
#define ASSERTMSG6(error,msg1,msg2,msg3,msg4,msg5,msg6) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " " << msg5 << " " << msg6 << LogEnter; Assert(error);}
#else
#define ASSERTMSGF(error) if (!(error)) {LError << "[Assert] " << #error << " " << __FUNCTION__ << ":" << __LINE__ << LogEnter; }
#define ASSERTMSGFR(error,rt) if (!(error)) {LError << "[Assert] " << #error << " " << __FUNCTION__ << ":" << __LINE__ << LogEnter; rt;}
#define ASSERTMSG1(error,msg) if (!(error)) {LError << "[Assert] " << msg << LogEnter;}
#define ASSERTMSG2(error,msg1,msg2) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << LogEnter; }
#define ASSERTMSG3(error,msg1,msg2,msg3) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << " " << msg3 << LogEnter; }
#define ASSERTMSG4(error,msg1,msg2,msg3,msg4) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << LogEnter; }
#define ASSERTMSG5(error,msg1,msg2,msg3,msg4,msg5) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " " << msg5 << LogEnter; }
#define ASSERTMSG6(error,msg1,msg2,msg3,msg4,msg5,msg6) if (!(error)) {LError << "[Assert] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " " << msg5 << " " << msg6 << LogEnter;}
#endif

#define ASSERTWARF(error) if (!(error)) {LWarning << "[Warnning] " << __FUNCTION__ << ":" << __LINE__ << LogEnter; }
#define ASSERTWAR1(error,msg) if (!(error)) {LWarning << "[Warnning] " << msg << LogEnter; }
#define ASSERTWAR2(error,msg1,msg2) if (!(error)) {LWarning << "[Warnning] "<< msg1 << " " << msg2 << LogEnter; }
#define ASSERTWAR3(error,msg1,msg2,msg3) if (!(error)) {LWarning << "[Warnning] "<< msg1 << " " << msg2 << " " << msg3 << LogEnter; }
#define ASSERTWAR4(error,msg1,msg2,msg3,msg4) if (!(error)) {LWarning << "[Warnning] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << LogEnter; }
#define ASSERTWAR5(error,msg1,msg2,msg3,msg4,msg5) if (!(error)) {LWarning << "[Warnning] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " " << msg5 << LogEnter; }
#define ASSERTWAR6(error,msg1,msg2,msg3,msg4,msg5,msg6) if (!(error)) {LWarning << "[Warnning] "<< msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " " << msg5 << " " << msg6 << LogEnter;}

#endif
