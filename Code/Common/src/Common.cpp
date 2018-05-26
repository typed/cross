#include "../inc/CommonDef.h"
#include "../inc/iLog.h"
#include "../inc/math/Random.h"
#include "../inc/CrossConsole.h"
#include "../inc/CrossFileOP.h"
#include <string>
#if (defined(WIN32) || defined(_WIN32))
#	include <windows.h>
#	include <time.h>
#	include "dbghelp.h"
#elif (defined __linux__)
	#include <unistd.h>
#endif

using namespace std;

namespace cm
{

string s_sInitAppDir;

#if (defined(WIN32) || defined(_WIN32))
LONG WINAPI UnhandledExceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
	time_t tmt = time(0);
	tm* ptm = localtime(&tmt);
	if (ptm) {
		int year = 1900 + ptm->tm_year;
		int month = 1 + ptm->tm_mon;
		int day = ptm->tm_mday;
		int hour = ptm->tm_hour;
		int minute = ptm->tm_min;
		int second = ptm->tm_sec;
		::CreateDirectory(L"crash", NULL);
		WCHAR szFileName[_MAX_PATH];
		::GetModuleFileName(NULL, szFileName, _MAX_PATH);
		wstring sf = szFileName;
		wstring::size_type of = sf.find_last_of(L"\\");
		if (of != wstring::npos) {
			sf = sf.substr(of+1);
			sf = sf.substr(0, sf.length()-4);
		}
		WCHAR s[128];
		wsprintf(s, L"crash/%s_%04d_%02d_%02d-%02d.%02d.%02d.dmp", sf.c_str(), year, month, day, hour, minute, second);
		HANDLE hFile = ::CreateFile(
			s,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (INVALID_HANDLE_VALUE != hFile) {
			MINIDUMP_EXCEPTION_INFORMATION einfo;
			einfo.ThreadId          = ::GetCurrentThreadId();
			einfo.ExceptionPointers = ExceptionInfo;
			einfo.ClientPointers    = FALSE;
			::MiniDumpWriteDump(
				::GetCurrentProcess(),
				::GetCurrentProcessId(),
				hFile,
				MiniDumpWithFullMemory,
				&einfo,
				NULL,
				NULL);
			::CloseHandle(hFile);
		}
	}
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void InitCommon(bool bConsole, int argc, char** argv, cpstr sExe, cpstr sCommand)
{
	RandomSeed();
	CrossFileOP::AppSetCurDir();
	s_sInitAppDir = CrossFileOP::AppGetCurDir();
	if (bConsole)
		CrossConsole::Show();
	InitLog();
#if (defined(WIN32) || defined(_WIN32))
	::SetUnhandledExceptionFilter(UnhandledExceptionHandler);
#endif
}

void ReleaseCommon()
{
	CrossConsole::Hide();
}

string& _GetVer()
{
	static string s_sVersion;
	return s_sVersion;
}
void SetVer(cpstr s)
{
	_GetVer() = s;
}

cpstr GetVer()
{
	return _GetVer().c_str();
}

cpstr GetInitAppDir()
{
	return s_sInitAppDir.c_str();
}


#if (defined __linux__)

void Daemon()
{
	// �˳���ǰ���򣬽����̨����
	if(fork()>0)
		exit(0); // ��������ֹ

	// �ӽ��̼���ִ��
	// parent������Ϊ�Ự����ͷ���̣����exit����ִ�еĻ�����ô�ӽ��̻��Ϊ�¶����̣�����init������
	// ִ��setsid()֮��,child�����»��һ���µĻỰ(session)id����ΪΪ�µĻỰ����ͷ���̣������丸���̵ĻỰ��ͽ��������롣
	// ���ڻỰ�Կ����ն˵Ķ�ռ�ԣ�����ͬʱ������ն����롣
	// ��ʱparent�˳�֮��,������Ӱ�쵽child�ˡ�
	setsid(); 

	if(fork() >0 ) 
		exit(0);

	//	chdir("/");
	//	umask(0);
}

#endif

}
