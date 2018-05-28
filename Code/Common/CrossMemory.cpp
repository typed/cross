#include "../inc/CrossMemory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#	include <windows.h>
// #	ifdef MEMCHECK
// #		include "../../third/vld/include/vld.h"
// #	endif
#else
#endif

namespace cm
{
	pvd CrossMemory::Malloc(ui32 sz) {
		return ::malloc(sz);
	}
	void CrossMemory::Free(pvd p) {
		::free(p);
	}
	void CrossMemory::MemCpy(pvd des, cpvd src, ui32 sz) {
		memcpy(des, src, sz);
	}
	void CrossMemory::MemSet(pvd p, i32 n, ui32 sz) {
		memset(p, n, sz);
	}
	void CrossMemory::WMemSet(pvd des, i32 n, ui32 sz) {
		ui16* pDes = (ui16*)des;
		for (ui32 i = 0; i < sz; i++)
			*pDes = n;
	}
	void CrossMemory::ZeroMem(pvd p, ui32 sz) {
		memset(p, 0, sz);
	}
	void CrossMemory::MemMove(pvd des, cpvd src, ui32 sz) {
		memmove(des, src, sz);
	}
	ui32 CrossMemory::GetPhysMemory() {
//#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
//		MEMORYSTATUS stat;
//		GlobalMemoryStatus(&stat);
//		return (ui32)stat.dwAvailPhys;
//#else
		return 0;
//#endif
	}
}
