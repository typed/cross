//�������ƣ�Cross.h
//������������ƽ̨�м��
//�����ˣ�	������
//����ʱ�䣺2012-11-9
//�޸��ߣ�
//�޸�ʱ�䣺
//�޸�˵����

#ifndef crosswindow_h
#define crosswindow_h

#include "CommonDef.h"
#include "cListenable.h"
#include <vector>
#include <string>

namespace cm
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