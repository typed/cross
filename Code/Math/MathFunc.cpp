#include "MathFunc.h"
#include <math.h>
#include <time.h>

namespace cross
{

	ui64 MathFunc::RandomSeed() {
		int num = 123;
		return time(0)+(ui64)(&num);
	}

	f32 MathFunc::Abs(f32 a) {
		return fabs(a);
	}
	f64 MathFunc::Abs(f64 a) {
		return fabs(a);
	}
	i32 MathFunc::Abs(i32 a) {
		return a < 0 ? -a : a;
	}

	f32 MathFunc::Sqrt(f32 a) {
		return sqrt(a);
	}
	f64 MathFunc::SqrtD(f64 a) {
		return sqrt(a);
	}

	f32 MathFunc::Sin(f32 a) {
		return sin(a);
	}
	f32 MathFunc::Cos(f32 a) {
		return cos(a);
	}
	f32 MathFunc::Tan(f32 a) {
		return tan(a);
	}
	f32 MathFunc::Asin(f32 a) {
		return asin(a);
	}
	f32 MathFunc::Acos(f32 a) {
		return acos(a);
	}
	f32 MathFunc::Atan(f32 a) {
		return atan(a);
	}
	f32 MathFunc::Atan2(f32 a, f32 b) {
		return atan2(a, b);
	}
	f32 MathFunc::Log(f32 a) {
		return log(a);
	}
	f64 MathFunc::LogD(f64 a) {
		return log(a);
	}
	f32 MathFunc::Exp(f32 a) {
		return exp(a);
	}
	f32 MathFunc::Floor(f32 a) {
		return floor(a);
	}
	f32 MathFunc::Pow(f32 v, i32 n) {
		return pow(v, n);
	}
	f64 MathFunc::Pow(f64 v, i32 n) {
		return pow(v, n);
	}
	f32 MathFunc::Mod(f32 x, f32 y) {
		return fmod(x, y);
	}
	f32 MathFunc::Ceil(f32 v) {
		return ceil(v);
	}
}
