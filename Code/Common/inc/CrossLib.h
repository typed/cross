//�������ƣ�Cross.h
//������������ƽ̨�м��
//�����ˣ�	������
//����ʱ�䣺2012-11-9
//�޸��ߣ�
//�޸�ʱ�䣺
//�޸�˵����

#ifndef crosslib_h
#define crosslib_h

#include "CommonDef.h"

namespace nsCommon
{

//��̬��
class CrossLib
{
public:
	static pvd LoadLib(cpstr sLib);
	static pvd GetSym(pvd, cpstr funcname);
	static void UnloadLib(pvd);
};

}

#endif