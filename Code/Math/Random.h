#ifndef random_h
#define random_h
#include "../CommonDef.h"

namespace cm
{

COMMON_API void RandomSeed();

COMMON_API i32 RandomAInt();

COMMON_API ui32 RandomAUnsignedInt();

COMMON_API i32 RandomInt(i32 min, i32 max);

COMMON_API ui32 RandomUnsignedInt(ui32 min, ui32 max);

COMMON_API f32 RandomFloat(f32 min, f32 max);

COMMON_API f64 RandomDouble(f64 min, f64 max);

COMMON_API b8 RandomChance(f32 chance);

//随机排序
//...

//随机字符串
COMMON_API cpstr RandomChar(ui32 nChar);

//随机英文
COMMON_API cpstr RandomEnglish(ui32 nChar);

//随机索引不重复
COMMON_API void RandomList(i32* aList, i32 len);

//Added by lkm,2011.2.10,产生符合高斯分布的随机数
COMMON_API f64 RandomGaussDouble(f64 dExpection = 0.0, f64 dSigma = 1.0);
//Added by lkm,2011.2.10,产生符合高斯分布的随机数
COMMON_API i32 RandomGaussInt(i32 nExpection, i32 nSigma);

}
#endif
