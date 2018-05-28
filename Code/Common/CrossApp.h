//程序名称：Cross.h
//功能描述：跨平台中间件
//开发人：	龚晓剑
//开发时间：2012-11-9
//修改者：
//修改时间：
//修改说明：

#ifndef _cross_common_crossapp_h
#define _cross_common_crossapp_h

#include "Common.h"
#include "cListenable.h"
#include <vector>
#include <string>

namespace cross
{

class CrossApp
{
public:
	static void MsgBox(const std::string& sTxt, const std::string& sTitle = "");
	static bool Clipboard_Copy(const std::string& str);
	static bool Clipboard_Copy(const std::wstring& wstr);
	static bool Clipboard_Paste(std::string& str, ui32& n, pvd hWnd, bool bEmpty = false);
	static void Exec(const std::string& sExe, const std::string& sParam = "");
	static void ExecSubProcess(cpstr);
	static void SystemBat(cpstr);
	static ui32 GetLastError();
};

}

#endif