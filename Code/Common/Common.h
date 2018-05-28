#ifndef _cross_common_common_h
#define _cross_common_common_h

#include "Config.h"

namespace cross
{

void InitCommon(bool bConsole = false, int argc = 0, char** argv = 0, cpstr sExe = "", cpstr sCommand = "");
void ReleaseCommon();
void SetVer(cpstr);
cpstr GetVer();
cpstr GetInitAppDir();
void  Daemon();

};

#endif
