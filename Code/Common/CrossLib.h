//程序名称：Cross.h
//功能描述：跨平台中间件
//开发人：	龚晓剑
//开发时间：2012-11-9
//修改者：
//修改时间：
//修改说明：

#ifndef crosslib_h
#define crosslib_h

#include "CommonDef.h"

namespace nsCommon
{

//动态库
class CrossLib
{
public:
	static pvd LoadLib(cpstr sLib);
	static pvd GetSym(pvd, cpstr funcname);
	static void UnloadLib(pvd);
};

}

#endif