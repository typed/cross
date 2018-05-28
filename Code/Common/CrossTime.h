//�������ƣ�Cross.h
//������������ƽ̨�м��
//�����ˣ�	������
//����ʱ�䣺2012-11-9
//�޸��ߣ�
//�޸�ʱ�䣺
//�޸�˵����

#ifndef crosstime_h
#define crosstime_h

#include "CommonDef.h"
#include <string>

namespace cm
{

//ʱ��
class CrossTime
{
public:
	//ʱ��
	static TypeDate GetDate();
	//CPUʱ��
	static TypeTime GetCPUTime();
	//
	static TypeDate MkTime(i32 year, i32 month, i32 day, i32 hour, i32 minute, i32 second);
	static bool LocalTime(TypeDate tm, i32& year, i32& month, i32& day, i32& wday, i32& hour, i32& minute, i32& second);
	static void FormatStringToTime(const std::string& stm, i32& year, i32& month, i32& day, i32& hour, i32& minute, i32& second);

	static TypeDate VariantTimeToSystemTime(f64);
	static f64 SystemTimeToVariantTime(TypeDate);

};

}
#endif