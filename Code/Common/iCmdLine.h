#ifndef cross_common_icmdline_h
#define cross_common_icmdline_h

#include "Common.h"

namespace cross
{

class iCmdLine
{
public:
	virtual void Build(i32 argc, i8** argv) = 0;
	virtual cpstr GetCmd(i32) const = 0;
};

iCmdLine* GetCmdLine();

}
#endif