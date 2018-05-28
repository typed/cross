//程序名称：Cross.h
//功能描述：跨平台中间件
//开发人：	龚晓剑
//开发时间：2012-11-9
//修改者：
//修改时间：
//修改说明：

#ifndef crossstring_h
#define crossstring_h

#include "CommonDef.h"
#include <vector>
#include <string>
#include <string.h>

namespace cm
{

//字符
class CrossString
{
public:
	//字符串比较
	static i32 StrCmp(cpstr, cpstr);
	//字符串拷贝
	static void StrCpy(pstr, cpstr);
	//字符串拷贝N个
	static void StrNCpy(pstr, cpstr, ui64);
	//字符串连接，Add by lkm,2013.3.22
	static void StrCat(pstr des, cpstr src);
	//求字符串长度，Add by lkm,2013.3.25
	static ui32 StrLen(cpstr s);
	//转换为小写
	static std::string ToLower(const std::string&);
	//转换为大写
	static std::string ToUpper(const std::string&);
	//字符串是否相等
	static bool Equal(cpstr, cpstr);
	//字符串是否相等忽略大小写
	static bool EqualIgnore(cpstr, cpstr);
	//字符串是否相等忽略大小写
	static bool EqualIgnore(const std::string&, const std::string&);
	//字符串 == "null"
	static bool EqualStrNull(cpstr);
	//格式化字符串,max 102400byte
	static std::string Format(cpstr src, ...);
	//sscanf,暂时没有实现
	static void SScanf(cpstr src, cpstr sText, ...);
	//UTF8转ASCII
	static std::string UTF8ToASCII(const std::string& utf8);
	//ASCII转UTF8
	static std::string ASCIIToUTF8(const std::string& ascii);
	//UTF8转WString
	static std::wstring UTF8ToWString(const std::string& utf8);
	//WString转UTF8
	static std::string WStringToUTF8(const std::wstring& wstr);
	//拆字符串
	static bool Split(const std::string& s, const std::string& sign, std::vector<std::string>& aSet);
	static std::string Split(const std::string& s, const std::string& sign, i32& offset);
	//add by yfw 在Split的基础上，增加过滤secSignStart和secSignEnd中出现的sign符号产生的段，即被secSignStart和secSignEnd围成的段，不会被切为两段
	static bool SplitEx(const std::string& s, const std::string& sign, const std::string& secSignStart, const std::string& secSignEnd, std::vector<std::string>& aSet);
	static bool SplitNumber(i64, std::vector<std::string>& aSet);
	//替换
	static std::string ReplaceAll(const std::string& str, const std::string& old_value, const std::string& new_value);
	static std::string Trim(const std::string& str);

	//关于<coord > .. <\coord>的格式化add by yfw
	//add by yfw 超级文本框 coord 的格式构建
	//<coord 地图名 NPC真实名字 任务名>NPC显示名字</coord> or <coord 地图名>显示文本</coord>
	static std::string StringFormatCoord(cpstr szContent, i32 nX, i32 nY, cpstr szMapName, cpstr szRealName = 0, cpstr szTaskName = 0);
	//<coord X Y 地图名 NPC真实名字 任务名>NPC显示名字</coord> or <coord X Y 地图名>显示文本</coord>
	static std::string StringFormatCoord(cpstr szContent, cpstr szMapName, cpstr szRealName = 0, cpstr szTaskName = 0);

	//得到sz 中 Coord格式中的nIndex段；但 bContent=true时，忽略nIndex，直接获得显示段。
	//限制，只能获得sz中第一个<coord 。。。>。。</coord>格式，之后的将被忽略
	static std::string GetParamCoord(cpstr sz, i32 nIndex);
	//取<coord 。。。>。。</coord>格式的前端字符，或后端字符；或显示字符
	static std::string GetContentCoord(cpstr sz, bool bBefor, bool bAfter);

	//字符 -> bool
	static bool ToBool(const std::string& s);
	//字符10进制 -> int
	static i32 _10ToInt(const std::string& s);
	//字符10进制 -> long Add by lkm,2013.3.26
	static l32 _10ToLong(const std::string& s);
	//字符10进制 -> float
	static f32 _10ToFloat(const std::string& s);
	//字符10进制 -> ui32
	static ui32 _10ToUInt(const std::string& s);
	//字符16进制 -> unsigned long
	static ui32 _16ToUInt(const std::string& s);

	static ui32 StrToHex(const std::string& s);
	//字符10进制 -> double
	static f64 _10ToDouble(const std::string& s);
	//int -> 字符10进制
	static std::string IntTo10(i32 num);
	//unsigned long -> 字符10进制
	static std::string UIntTo10(ui32 num);
	//float -> 字符10进制
	static std::string FloatTo10(f32 num);
	//int -> 字符16进制
	static std::string IntTo16(i32 num);
	//unsigned long -> 字符16进制
	static std::string UIntTo16(ui32 num);
	//unsigned long -> 字符16进制
	static std::string UIntTo016(ui32 num);
	//long long -> 字符10进制
	static std::string Int64To10(i64 num);

	static i8 Char16ToBinary(i8 c);
	//字符16进制转binary
	static std::string StringToBinary(std::string s);
	//buffer转字符16进制
	static std::string BinaryToString(cpstr aBuf, ui32 len);
	static std::string BinaryToString(const std::string& aBuf) {
		return BinaryToString(aBuf.c_str(), (ui32)aBuf.size());
	}
	//是否是整形支持在首位包含'+'和'-'符号的 add by yfw
	static bool IsInt(std::string s);
	//正则表达式
	static bool Match(const std::string& src, const std::string& regex);

	template<class T> 
	static inline i32 String2Int(const std::string& szString, T& t) {
		for (i32 i = 0; ;i++) {
			if (t[i][0] == 0)
				break;
			if (szString == t[i])
				return i;
		}
		return -1;
	}

	static std::string ToMD5(const std::string& sz);
	static std::string ToMD5Lower(const std::string& sz);

	static std::string EncryptAES(const std::string& sz, const std::string& key, const std::string& iv);
	static std::string DecryptAES(const std::string& sz, const std::string& key, const std::string& iv);

	static std::string MyEncrypt(std::string src);
	static std::string MyDecrypt(std::string src);
};

}

#endif