#ifndef _cross_math_random_h
#define _cross_math_random_h

#include "Config.h"

namespace cross
{

void RandomSeed();

i32 RandomAInt();

ui32 RandomAUnsignedInt();

i32 RandomInt(i32 min, i32 max);

ui32 RandomUnsignedInt(ui32 min, ui32 max);

f32 RandomFloat(f32 min, f32 max);

f64 RandomDouble(f64 min, f64 max);

b8 RandomChance(f32 chance);

//随机排序
//...

//随机字符串
cpstr RandomChar(ui32 nChar);

//随机英文
cpstr RandomEnglish(ui32 nChar);

//随机索引不重复
void RandomList(i32* aList, i32 len);

//Added by lkm,2011.2.10,产生符合高斯分布的随机数
f64 RandomGaussDouble(f64 dExpection = 0.0, f64 dSigma = 1.0);
//Added by lkm,2011.2.10,产生符合高斯分布的随机数
i32 RandomGaussInt(i32 nExpection, i32 nSigma);

}
#endif
