//程序名称：Cross.h
//功能描述：跨平台中间件
//开发人：	龚晓剑
//开发时间：2012-11-9
//修改者：
//修改时间：
//修改说明：

#ifndef _cross_common_crosstime_h
#define _cross_common_crosstime_h

#include "Common.h"
#include <string>

namespace cross
{

//时间
class CrossTime
{
public:
	//时钟
	static TypeDate GetDate();
	//CPU时间
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