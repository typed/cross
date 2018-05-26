#include "../inc/CrossMath.h"
#include <math.h>
#include <time.h>

namespace cm
{

	ui64 CrossMath::RandomSeed() {
		int num = 123;
		return time(0)+(ui64)(&num);
	}

	f32 CrossMath::Abs(f32 a) {
		return fabs(a);
	}
	f64 CrossMath::Abs(f64 a) {
		return fabs(a);
	}
	i32 CrossMath::Abs(i32 a) {
		return a < 0 ? -a : a;
	}

	f32 CrossMath::Sqrt(f32 a) {
		return sqrt(a);
	}
	f64 CrossMath::SqrtD(f64 a) {
		return sqrt(a);
	}

	f32 CrossMath::Sin(f32 a) {
		return sin(a);
	}
	f32 CrossMath::Cos(f32 a) {
		return cos(a);
	}
	f32 CrossMath::Tan(f32 a) {
		return tan(a);
	}
	f32 CrossMath::Asin(f32 a) {
		return asin(a);
	}
	f32 CrossMath::Acos(f32 a) {
		return acos(a);
	}
	f32 CrossMath::Atan(f32 a) {
		return atan(a);
	}
	f32 CrossMath::Atan2(f32 a, f32 b) {
		return atan2(a, b);
	}
	f32 CrossMath::Log(f32 a) {
		return log(a);
	}
	f64 CrossMath::LogD(f64 a) {
		return log(a);
	}
	f32 CrossMath::Exp(f32 a) {
		return exp(a);
	}
	f32 CrossMath::Floor(f32 a) {
		return floor(a);
	}
	f32 CrossMath::Pow(f32 v, i32 n) {
		return pow(v, n);
	}
	f64 CrossMath::Pow(f64 v, i32 n) {
		return pow(v, n);
	}
	f32 CrossMath::Mod(f32 x, f32 y) {
		return fmod(x, y);
	}
	f32 CrossMath::Ceil(f32 v) {
		return ceil(v);
	}
}
