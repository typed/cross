//�������ƣ�Cross.h
//������������ƽ̨�м��
//�����ˣ�	������
//����ʱ�䣺2012-11-9
//�޸��ߣ�
//�޸�ʱ�䣺
//�޸�˵����

#ifndef crossmemory_h
#define crossmemory_h

#include "CommonDef.h"

namespace cm
{

//�ڴ�
class COMMON_API CrossMemory
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

#define cmemcpy cm::CrossMemory::MemCpy
#define cmemset cm::CrossMemory::MemSet
#define cmemmove cm::CrossMemory::MemMove
#define czeromem cm::CrossMemory::ZeroMem

#endif