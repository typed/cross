#include "cCmdLine.h"

namespace cm
{

cCmdLine::cCmdLine()
{

}

cCmdLine::~cCmdLine()
{
}

void cCmdLine::Build(i32 argc, i8** argv)
{
	if (argc == 0 || argv == 0)
		return;
	for (i32 i = 0; i < argc; i++)
		m_aCmd.push_back(argv[i]);
}

cpstr cCmdLine::GetCmd(i32 idx) const
{
	if (idx < 0 || idx >= (i32)m_aCmd.size())
		return nullptr;
	return m_aCmd[idx].c_str();
}

iCmdLine* GetCmdLine()
{
	static cCmdLine s_cl;
	return &s_cl;
}

}
