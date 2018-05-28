//程序名称：Cross.h
//功能描述：跨平台中间件
//开发人：	龚晓剑
//开发时间：2012-11-9
//修改者：
//修改时间：
//修改说明：

#ifndef _cross_common_crossmemory_h
#define _cross_common_crossmemory_h

#include "Common.h"

namespace cross
{

//内存
class CrossMemory
{
public:
	static void MemCpy(pvd des, cpvd src, ui32 sz);
	static void MemSet(pvd des, i32 n, ui32 sz);
	static void WMemSet(pvd des, i32 n, ui32 sz);
	static void ZeroMem(pvd des, ui32);
	static void MemMove(pvd des, cpvd src, ui32 sz);
	static pvd Malloc(ui32);
	static void Free(pvd);
	static ui32 GetPhysMemory();
};

}

#define cmemcpy cross::CrossMemory::MemCpy
#define cmemset cross::CrossMemory::MemSet
#define cmemmove cross::CrossMemory::MemMove
#define czeromem cross::CrossMemory::ZeroMem

#endif