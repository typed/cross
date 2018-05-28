#ifndef icmdline_h
#define icmdline_h
#include "CommonDef.h"
namespace cross
{

class COMMON_API iCmdLine
{
public:
	virtual void Build(i32 argc, i8** argv) = 0;
	virtual cpstr GetCmd(i32) const = 0;
};

COMMON_API iCmdLine* GetCmdLine();

}
#endif