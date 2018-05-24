//�������ƣ�Cross.h
//������������ƽ̨�м��
//�����ˣ�	������
//����ʱ�䣺2012-11-9
//�޸��ߣ�
//�޸�ʱ�䣺
//�޸�˵����

#ifndef crossmath_h
#define crossmath_h

#include "CommonDef.h"

namespace cm
{

//��ѧ
class CrossMath
{
public:

	static ui64 RandomSeed();

	static f64 Abs(f64);
	static f32 Abs(f32);
	static i32 Abs(i32);
	
	static f32 Sqrt(f32);
	static f64 SqrtD(f64);
	
	static f32 Sin(f32);
	static f32 Cos(f32);
	static f32 Tan(f32);
	static f32 Asin(f32);
	static f32 Acos(f32);
	static f32 Atan(f32);
	static f32 Atan2(f32,f32);
	static f32 Log(f32);
	static f64 LogD(f64);
	static f32 Exp(f32);
	static f32 Floor(f32);
	static f32 Pow(f32,i32);
	static f64 Pow(f64,i32);
	static f32 Mod(f32,f32);
	static f32 Ceil(f32);
	

};

}
#endif