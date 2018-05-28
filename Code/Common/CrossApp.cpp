#include "CrossApp.h"
#include "CrossString.h"

#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
#	include <windows.h>
#endif

using namespace std;

namespace cross
{

	void CrossApp::MsgBox(const std::string& sTxt, const std::string& sTitle) {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//::MessageBox(0, sTxt.c_str(), sTitle.c_str(), 0);
#else
#endif
	}
	void CrossApp::Exec(const std::string& sExe, const std::string& sParam) {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		::ShellExecuteA(NULL, "open", sExe.c_str(), sParam.c_str(), NULL, SW_SHOWNORMAL);
#else
#endif
	}
	void CrossApp::ExecSubProcess(cpstr s) {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//::CreateProcess(0, s, 0, 0, FALSE, 0, 0, 0, 0, 0);
#else
#endif
	}
	void CrossApp::SystemBat(cpstr s) {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		system(s);
#else
#endif
	}
	ui32 CrossApp::GetLastError() {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		return ::GetLastError();
#else
		return 0;
#endif
	}

	bool CrossApp::Clipboard_Copy(const std::string& str) {
#if (defined(WIN32) || defined(_WIN32))
		HWND hwnd = ::GetForegroundWindow();
		if (!OpenClipboard((HWND)hwnd))
			return false;
		if (!::EmptyClipboard()) {
			::CloseClipboard();
			return false;
		}
		HGLOBAL HProg = GlobalAlloc(GHND, str.length()+1);
		if (HProg == 0) {
			::CloseClipboard();
			return false;
		}
		char* buf = (char*)::GlobalLock(HProg);
		::strcpy(buf, str.c_str());
		::GlobalUnlock(HProg);
		if (!::SetClipboardData(CF_TEXT, HProg))
			return false;
		::CloseClipboard();
		return true;
#else
		return false;
#endif
	}
	bool CrossApp::Clipboard_Copy(const std::wstring& wstr)
	{
#if (defined(WIN32) || defined(_WIN32))
		HWND hwnd = ::GetForegroundWindow();
		if (!OpenClipboard((HWND)hwnd))
			return false;
		if (!::EmptyClipboard()) {
			::CloseClipboard();
			return false;
		}
		HGLOBAL HProg = GlobalAlloc(GHND,(wstr.length()+1)*sizeof(wchar_t));
		if (HProg == 0) {
			::CloseClipboard();
			return false;
		}
		wchar_t* buf = (wchar_t*)::GlobalLock(HProg);
		::wcscpy(buf, wstr.c_str());
		::GlobalUnlock(HProg);
		if (!::SetClipboardData(CF_UNICODETEXT, HProg))
			return false;
		::CloseClipboard();
		return true;
#else
		return false;
#endif
	}
	bool CrossApp::Clipboard_Paste(string& str, ui32& n, pvd hWnd, bool bEmpty) {
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
		//HGLOBAL HClip;
		//HGLOBAL HProg;
		//char FAR * PClip;
		//char FAR * PProg;
		//if (!OpenClipboard((HWND)hWnd))
		//	return false;
		//HClip=::GetClipboardData(CF_TEXT);
		//if (HClip==0) {
		//	::CloseClipboard();
		//	return true;
		//}
		//HProg = GlobalAlloc(GMEM_MOVEABLE,::GlobalSize(HClip));
		//if (HProg==0) {
		//	::CloseClipboard();
		//	return false;
		//}
		//PClip=(char*)::GlobalLock(HClip);
		//PProg=(char*)::GlobalLock(HProg);
		//::lstrcpy(PProg,PClip);
		//::GlobalUnlock(HClip);
		//if (bEmpty)
		//	::EmptyClipboard();
		//::CloseClipboard();
		//string strName = PProg;
		//::GlobalUnlock(HProg);
		//::GlobalFree(HProg);
		//if (strName.empty())
		//	return false;
		//str = strName;
		return true;
#else
		return false;
#endif
	}

}
