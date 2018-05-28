#ifndef _common_CommonDef_h_
#define _common_CommonDef_h_

#include "Config.h"

namespace cm
{

void InitCommon(bool bConsole = false, int argc = 0, char** argv = 0, cpstr sExe = "", cpstr sCommand = "");
void ReleaseCommon();
void SetVer(cpstr);
cpstr GetVer();
cpstr GetInitAppDir();
void  Daemon();

};

#endif
