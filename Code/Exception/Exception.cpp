#include "Exception.h"
#include <string>
#if (defined(WIN32) || defined(_WIN32))
#	include <windows.h>
#	include <time.h>
#	include "dbghelp.h"
#	pragma comment(lib, "dbghelp.lib")
#endif

using namespace std;

namespace cross
{

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

void SetupException()
{
#if (defined(WIN32) || defined(_WIN32))
	::SetUnhandledExceptionFilter(UnhandledExceptionHandler);
#endif
}

}
