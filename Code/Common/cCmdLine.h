#ifndef cross_common_ccmdline_h
#define cross_common_ccmdline_h

#include "iCmdLine.h"
#include <vector>
#include <string>

namespace cross
{

class cCmdLine :
	public iCmdLine
{
public:
	cCmdLine();
	~cCmdLine();
	void Build(i32 argc, i8** argv);
	cpstr GetCmd(i32) const;
	std::vector<std::string> m_aCmd;
};

}
#endif
