#include "Common.h"
#include "iLog.h"
#include "Math/Random.h"
#include "CrossConsole.h"
#include "CrossFileOP.h"
#include <string>
#if (defined __linux__)
	#include <unistd.h>
#endif

using namespace std;

namespace cross
{

string s_sInitAppDir;

void InitCommon(bool bConsole, int argc, char** argv, cpstr sExe, cpstr sCommand)
{
	RandomSeed();
	CrossFileOP::AppSetCurDir();
	s_sInitAppDir = CrossFileOP::AppGetCurDir();
	if (bConsole)
		CrossConsole::Show();
	InitLog();
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
	// 退出当前程序，进入后台程序
	if(fork()>0)
		exit(0); // 父进程终止

	// 子进程继续执行
	// parent进程作为会话的领头进程，如果exit结束执行的话，那么子进程会成为孤儿进程，并被init收养。
	// 执行setsid()之后,child将重新获得一个新的会话(session)id，成为为新的会话的领头进程，并与其父进程的会话组和进程组脱离。
	// 由于会话对控制终端的独占性，进程同时与控制终端脱离。
	// 这时parent退出之后,将不会影响到child了。
	setsid(); 

	if(fork() >0 ) 
		exit(0);

	//	chdir("/");
	//	umask(0);
}

#endif

}
