#include "../../inc/math/Random.h"
#include "../../inc/CrossMath.h"
#include <vector>

using namespace std;

namespace cm
{

static ui32 g_seed = 0;
static ui64 g_seed64 = 0;
static const ui32 g_nNull = 0;
static const ui32 g_nOne = 1;
static const ui32 g_nPar1 = 214013;
static const ui32 g_nPar2 = 2531011;
static const ui32 g_nPar3 = 15;
static const ui32 g_nPar4 = 16;
static const f32 g_fPar5 = 1.0f / 65535.0f;
static const f64 g_dPar6 = 1.0 / 4294967295.0;
static const ui32 g_nPar7 = 0x7fffffff;

void RandomSeed()
{
	g_seed64 = CrossMath::RandomSeed();
	g_seed = (ui32)g_seed64;
}

ui32 RandomAUnsignedInt()
{
	g_seed = g_nPar1 * g_seed + g_nPar2;
	return ( g_seed ^ g_seed >> g_nPar3 );
}

ui32 RandomUnsignedInt(ui32 min, ui32 max)
{
	return min + RandomAUnsignedInt() % ( max - min + g_nOne );
}

i32 RandomAInt()
{
	return (i32)RandomUnsignedInt(g_nNull, g_nPar7);
}

i32 RandomInt(i32 min, i32 max)
{
	return min + (i32)(RandomAUnsignedInt() % ( max - min + g_nOne ));
}

f32 RandomFloat(f32 min, f32 max)
{
	g_seed = g_nPar1 * g_seed + g_nPar2;
	return min + ( g_seed >> g_nPar4 ) * g_fPar5 * ( max - min );
}
	
f64 RandomDouble(f64 min, f64 max)
{
	g_seed = g_nPar1 * g_seed + g_nPar2;
	return min + g_seed * g_dPar6 * ( max - min );
}

b8 RandomChance(f32 chance)
{
	if (chance >= 100.f)
		return true;
	if (chance <= 0.f)
		return false;
	return RandomFloat(0.f, 100.f) < chance;
}

//随机字符串
cpstr RandomChar(ui32 nChar)
{
	if (nChar == 0)
		return 0;
	static vector<i8> s_aStr;
	s_aStr.resize(nChar+1);
	for (ui32 i = 0; i < nChar; i++)
		s_aStr[i] = (i8)RandomUnsignedInt(1, 0xff);
	s_aStr[nChar] = 0;
	return &s_aStr.front();
}

//随机英文
cpstr RandomEnglish(ui32 nChar)
{
	if (nChar == 0)
		return 0;
	static vector<i8> s_aStr;
	s_aStr.resize(nChar+1);
	for (ui32 i = 0; i < nChar; i++) {
		if (RandomAInt()%2 == 0)
			s_aStr[i] = (i8)RandomUnsignedInt('A', 'Z');
		else
			s_aStr[i] = (i8)RandomUnsignedInt('a', 'z');
	}
	s_aStr[nChar] = 0;
	return &s_aStr.front();
}

void RandomList(i32* aList, i32 len)
{
	i32 iPos = 0;
	i32 i, j;
	i32 temp;
	for (i = 0; i < len; i++)
		aList[i] = -1;
	for (i = 0; i < len; i++) {
		temp = len - i;
		iPos = RandomAInt() % temp + 1;
		for (j = 0; j < len; j++) {
			if (aList[j] == -1)
				iPos--;
			if (iPos == 0)
				break;
		}
		aList[j] = i;
	}
}

//Added by lkm,2011.2.10,产生符合高斯分布的随机数
f64 RandomGaussDouble(f64 dExpection /*= 0.0*/, f64 dSigma /*= 1.0*/)
{
	static f64 dV1,dV2,dS;
	static bool bSet = false;
	f64 dRes;
	if(!bSet)
	{
		do
		{
			f64 dU1 = RandomDouble(0.0,1.0);
			f64 dU2 = RandomDouble(0.0,1.0);
			dV1 = 2.0 * dU1 - 1.0;
			dV2 = 2.0 * dU2 - 1.0;
			dS = dV1 * dV1 + dV2 * dV2;
		}
		while(dS >= 1.0 || dS == 0.0);
		dRes = dV1 * CrossMath::SqrtD(-2.0 * CrossMath::LogD(dS) / dS);
	}
	else
	{
		dRes = dV2 * CrossMath::SqrtD(-2.0 * CrossMath::LogD(dS) / dS);
	}
	bSet = !bSet;
	return dRes * dSigma + dExpection;
}

//? 已解决？
//? 取整后导致μ（期望）处随机数数量偏少，结果是鞍型的曲线
//Added by lkm,2011.2.10,产生符合高斯分布的随机数
i32 RandomGaussInt(i32 nExpection, i32 nSigma)
{
	f64 dRes = RandomGaussDouble(nExpection,nSigma);
	//return i32(dRes < 0.0 ? dRes - 0.5 : dRes + 0.5);//导致[μ-nσ,μ+nσ]区间内随机数数量过多,尤其是n=1时较明显
	//return i32(dRes + dRes - i32(dRes));
	//return i32(dRes < 0.0 ? dRes - 1 + 0.00001 : dRes + 1 - 0.00001);//导致鞍型曲线
	return i32((dRes < nExpection - nSigma || dRes > nExpection + nSigma ||
					dRes < nExpection - 2 * nSigma || dRes > nExpection + 2 * nSigma ||
					dRes < nExpection - 3 * nSigma || dRes > nExpection + 3 * nSigma) ? 
				(dRes < 0.0 ? dRes - 1 + 0.00001 : dRes + 1 - 0.00001) : 
				(dRes < 0.0 ? dRes - 0.5 : dRes + 0.5));//可行？
}

}