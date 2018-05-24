#include "../inc/CrossString.h"
#include "pcre/pcre.h"
#include "md5.h"
#include "aes.h"
#include <map>

//包含文件版本区分
#if (defined(WIN32) || defined(_WIN32) || defined (WINDOWS) || defined (_WINDOWS))
#	include <windows.h>
#	if defined(_MSC_VER)
#		pragma comment(lib, "version")
#	endif
#endif
#	include <stdio.h>
#	include <stdlib.h>
#	include <stdarg.h>
#	include <string>
#   include <sstream>
#   include <fstream>

using namespace std;

namespace cm
{

	static const i32 c_iTempSize = 102400;
	#define DEFSTR char sTemp[c_iTempSize];
	#define DEFWSTR wchar_t wsTemp[c_iTempSize];
	static const i32 c_iTempSizeNumber = 128;
	#define DEFSTRNUM char sTemp[c_iTempSizeNumber];
	#define DEFWSTRNUM wchar_t wsTemp[c_iTempSizeNumber];

	i32 CrossString::StrCmp(cpstr src, cpstr des) {
		return strcmp(src, des);
	}
	void CrossString::StrCpy(pstr des, cpstr src) {
		strcpy(des, src);
	}
	void CrossString::StrNCpy(pstr des, cpstr src, ui64 n)	{
		strncpy(des, src, (size_t)n);
	}
	void CrossString::StrCat(pstr des, cpstr src) {
		strcat(des, src);
	}
	ui32 CrossString::StrLen(cpstr s) {
		return (ui32)strlen(s);
	}
	bool CrossString::Equal(cpstr src, cpstr des) {
		return strcmp(src, des) == 0;
	}
	bool CrossString::EqualStrNull(cpstr src) {
		return CrossString::Equal(src, "null");
	}
	std::string CrossString::ToLower(const std::string& sz)
	{
		if (sz.empty())
			return sz;
		std::string s;
		s.resize(sz.size());
		char* pCh = (char*)s.data();
		const char* pSrc = sz.c_str();
		while (*pSrc != '\0') {
			if (*pSrc <= 'Z' && *pSrc >= 'A')
				*pCh = *pSrc + 32;
			else
				*pCh = *pSrc;
			pCh++;
			pSrc++;
		}
		*pCh = '\0';
		return s;
	}
	std::string CrossString::ToUpper(const std::string& sz)
	{
		std::string s;
		s.resize(sz.size());
		char* pCh = (char*)s.data();
		const char* pSrc = sz.c_str();
		while (*pSrc != '\0') {
			if (*pSrc <= 'z' && *pSrc >= 'a')
				*pCh = *pSrc - 32;
			else
				*pCh = *pSrc;
			pCh++;
			pSrc++;
		}
		*pCh = '\0';
		return s;
	}
	bool CrossString::EqualIgnore(cpstr s1, cpstr s2) {
		return ToLower(s1) == ToLower(s2);
	}
	bool CrossString::EqualIgnore(const std::string& s1, const std::string& s2) {
		return CrossString::EqualIgnore(s1.c_str(), s2.c_str());
	}
	std::string CrossString::Format(cpstr src, ...) {
		i8 buf[c_iTempSize] = {0};
		va_list argList;
		va_start(argList, src);
		vsnprintf(buf, sizeof(buf)-1, src, argList);
		va_end(argList);
		return buf;
	}
	void CrossString::SScanf(cpstr src, cpstr sText, ...) {
	}
	std::string CrossString::UTF8ToASCII(const std::string& src) {
#if (defined(WIN32) || defined(_WIN32))
		// UTF8 => WCHAR
		i32 lengthUtf8 = (i32)src.length();
		if (lengthUtf8 == 0)
			return "";
		i32 lengthUnicode = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), lengthUtf8, NULL, 0);
		wchar_t* szUnicode = new wchar_t[lengthUnicode+1];
		MultiByteToWideChar(CP_UTF8, 0, src.c_str(), lengthUtf8, szUnicode, lengthUnicode);
		szUnicode[lengthUnicode] = 0;
		// WCHAR => ANSI
		lengthUtf8 = WideCharToMultiByte(CP_ACP, 0, szUnicode, lengthUnicode, NULL, 0, NULL, NULL);
		char* szUtf8String = new char[lengthUtf8+1];
		WideCharToMultiByte(CP_ACP, 0, szUnicode, lengthUnicode, szUtf8String, lengthUtf8, NULL, NULL);
		szUtf8String[lengthUtf8] = 0;
		std::string sr = szUtf8String;
		delete[] szUnicode;
		delete[] szUtf8String;
		return sr;
#else
		return src;
#endif
	}
	std::string CrossString::ASCIIToUTF8(const std::string& src) {
#if (defined(WIN32) || defined(_WIN32))
		// ANSI => WCHAR
		i32 lengthAnsi = (i32)src.length();
		if (lengthAnsi == 0)
			return "";
		i32 lengthUnicode = MultiByteToWideChar(CP_ACP, 0, src.c_str(), lengthAnsi, NULL, 0);
		wchar_t* szUnicode = new wchar_t[lengthUnicode+1];
		MultiByteToWideChar(CP_ACP, 0, src.c_str(), lengthAnsi, szUnicode, lengthUnicode);
		szUnicode[lengthUnicode] = 0;
		// WCHAR => UTF8
		i32 lengthUtf8 = WideCharToMultiByte(CP_UTF8, 0, szUnicode, lengthUnicode, NULL, 0, NULL, NULL);
		char* szUtf8String = new char[lengthUtf8+1];
		WideCharToMultiByte(CP_UTF8, 0, szUnicode, lengthUnicode, szUtf8String, lengthUtf8, NULL, NULL);
		szUtf8String[lengthUtf8] = 0;
		std::string sr = szUtf8String;
		delete[] szUnicode;
		delete[] szUtf8String;
		return sr;
#else
		return src;
#endif
	}
	//UTF8转WString
	std::wstring CrossString::UTF8ToWString(const std::string& utf8) {
#if (defined(WIN32) || defined(_WIN32))
		int len = utf8.length();
		if (len == 0)
			return std::wstring();
		int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
		wchar_t* pUnicode;
		pUnicode = new wchar_t[unicodeLen + 1];
		memset((void*)pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
		std::wstring wstrReturn(pUnicode);
		delete[] pUnicode;
		return wstrReturn;
#else
		std::wstring wstr; wstr.resize(utf8.length());
		std::copy(utf8.begin(), utf8.end(), wstr.begin());
		return wstr;
#endif
	}
	//WString转UTF8
	std::string CrossString::WStringToUTF8(const std::wstring& wstr) {
#if (defined(WIN32) || defined(_WIN32))
		int len = wstr.length();
		if (len == 0)
			return std::string();
		char* pElementText;
		int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wstr.c_str(), -1, NULL, 0, NULL, NULL);
		pElementText = new char[iTextLen + 1];
		memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
		::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wstr.c_str(), -1, pElementText, iTextLen, NULL, NULL);
		std::string strReturn(pElementText);
		delete[] pElementText;
		return strReturn;
#else
		std::string str; str.resize(wstr.length());
		std::copy(wstr.begin(), wstr.end(), str.begin());
		return str;
#endif
	}

	bool CrossString::ToBool(const std::string& s) {
		return EqualIgnore(s, "true");
	}
	i32 CrossString::_10ToInt(const std::string& s) {
		ui32 v = 0;
		sscanf(s.c_str(), "%u", &v);
		return (i32)v;
	}
	l32 CrossString::_10ToLong(const std::string& s) {
		ui32 v = 0;
		sscanf(s.c_str(), "%u", &v);
		return (l32)v;
	}
	f32 CrossString::_10ToFloat(const std::string& s) {
		return (f32)atof(s.c_str());
	}
	ui32 CrossString::_10ToUInt(const std::string& s) {
		ui32 v = 0;
		sscanf(s.c_str(), "%u", &v);
		return v;
	}
	ui32 CrossString::_16ToUInt(const std::string& s) {
		ui32 idec = 0;
		sscanf(s.c_str(), "%x", &idec);
		return idec;
	}
	ui32 CrossString::StrToHex(const std::string& s) {
		return _16ToUInt(s);
	}
	f64 CrossString::_10ToDouble(const std::string& s) {
		return atof(s.c_str());
	}
	std::string CrossString::IntTo10(i32 num) {
		DEFSTRNUM;
		sprintf(sTemp, "%d", num);
		return sTemp;
	}
	std::string CrossString::UIntTo10(ui32 num) {
		DEFSTRNUM;
		sprintf(sTemp, "%u", num);
		return sTemp;
	}
	std::string CrossString::FloatTo10(f32 num) {
		DEFSTRNUM;
		sprintf(sTemp, "%f", num);
		return sTemp;
	}
	std::string CrossString::IntTo16(i32 num) {
		DEFSTRNUM;
		sprintf(sTemp, "%x", num);
		return sTemp;
	}
	std::string CrossString::UIntTo16(ui32 num) {
		DEFSTRNUM;
		sprintf(sTemp, "%x", num);
		return sTemp;
	}
	std::string CrossString::UIntTo016(ui32 num) {
		DEFSTRNUM;
		sprintf(sTemp, "%08X", num);
		return sTemp;
	}
	std::string CrossString::Int64To10(i64 num) {
		DEFSTRNUM;
		sprintf(sTemp, "%lld", num);
		return sTemp;
	}
	i8 CrossString::Char16ToBinary(i8 c)
	{
		if ( c >= '0' && c <= '9' )
			return c - '0';
		else if ( c >= 'A' && c <= 'F' )
			return ( c - 'A' ) + 10;
		else if ( c >= 'a' && c <= 'f' )
			return ( c - 'a' ) + 10;
		else
			return 0;
	}
	//字符16进制转binary
	std::string CrossString::StringToBinary(std::string s) {
		std::string aBuf;
		if (s.length() < 2 || s.length() % 2 != 0)
			return std::string();
		std::string::size_type of = (ui32)s.find("0x");
		if (of != std::string::npos)
			s = s.substr(2);
		ui32 len = (ui32)s.length();
		ui32 lenBuf = len / 2;
		aBuf.resize(lenBuf);
		i8* p = (i8*)aBuf.data();
		const i8* pi8 = s.c_str();
		for (ui32 i = 0; i < len; i += 2)
			p[i/2] = ( Char16ToBinary(pi8[i]) << 4 ) | Char16ToBinary(pi8[i+1]);
		return aBuf;
	}

	std::string CrossString::BinaryToString(cpstr aBuf, ui32 len) {
		if (len == 0)
			return "";
		vector<i8> s;
		i8* p = 0;
		s.resize(len * 2 + 3);
		p = &s.front();
		p[0] = '0';
		p[1] = 'x';
		p += 2;
		static const i8 c_sMakLower[] = {"0123456789abcdef"};
		//static const i8 c_sMakUpper[] = {"0123456789ABCDEF"};
		const i8* c_sMak;
		c_sMak = c_sMakLower;
		ui8 c = 0;
		for (ui32 i = 0; i < len; i++) {
			c = aBuf[i];
			*p++ = c_sMak[c >> 4];
			*p++ = c_sMak[c & 0xf];
		}
		*p = 0;
		return &s.front();
	}
	//是否是整形 包括负数 add by yfw
	//describe:循环检测每个字符是否为数字，第一位还可以包括'+' or '-'；只要有一个不满足，则直接返回false
	bool CrossString::IsInt(std::string s)
	{
		if (s.length() == 0)	//如果当前字符串为空，暂时也判断为其不是数字
			return false;
		const ui32 cnIni = 0;
		const i8 csAdd = '+';
		const i8 csSub = '-';
		for (ui32 i = 0; i < s.length(); i++) {
			i8 c = s[i];
			//如果是非数字字符
			if ( c < '0' || c > '9' ) {
				//第一位字符需要判断是否为'+' or '-'字符；其它的只要不是数字，则直接返回false
				if ( cnIni != i || ( csAdd != c && csSub != c ) )
					return false;
			}
		}
		return true;
	}

	bool CrossString::Split(const std::string& s, const std::string& sign, vector<std::string>& aSet)
	{
		std::string sIn = s;
		aSet.clear();
		i32 off = 0;
		i32 offend = 0;
		while(true) {
			offend = (i32)sIn.find(sign, off);
			if (offend == -1)
				break;
			if (offend >= off) {
				std::string sD = sIn.substr(off, offend-off);
				aSet.push_back(sD);
			}
			off = offend + (i32)sign.length();
		}
		if (off < (i32)sIn.length()) {
			std::string sD = sIn.substr(off);
			aSet.push_back(sD);
		}
		return !aSet.empty();
	}
	std::string CrossString::Split(const std::string& s, const std::string& sign, i32& offset)
	{
		if (offset == -1)
			return "";
		std::string sR;
		std::string sIn = s;
		i32 off = 0;
		off = (i32)sIn.find(sign, offset);
		if (off == -1) {
			if (offset < (i32)sIn.length())
				sR = sIn.substr(offset);
			offset = -1;
			return sR;
		}
		sR = sIn.substr(offset, off - offset);
		offset = (i32)(off + (i32)sign.length());
		return sR;
	}
	bool CrossString::SplitEx(const std::string& s, const std::string& sign, const std::string& secSignStart, const std::string& secSignEnd, vector<std::string>& aSet)
	{
		aSet.clear();
		if (s.empty() || sign.empty())
			return false;
		std::string sIn = s;
		i32 off = 0;
		i32 offend = 0;
		while(true) {
			offend = (i32)sIn.find(sign, off);
			if (offend == -1)
				break;
			//如果当前找到的段，
			if (!secSignStart.empty() && !secSignEnd.empty()) {
				i32 offset = -1;
				i32 SecOffStart = (i32)sIn.find(secSignStart, off);
				i32 SecOffEnd = (i32)sIn.find(secSignEnd, off);
				if ((SecOffStart != -1 && SecOffStart<offend) && (SecOffEnd != -1 && SecOffEnd>offend))
					offset = SecOffEnd;
				if (offset > 0) {
					offend = (i32)sIn.find(sign, offset);
					if (offend == -1)
						break;
				}
			}
			if (offend >= off) {
				std::string sD = sIn.substr(off, offend-off);
				aSet.push_back(sD);
			}
			off = offend + (i32)sign.length();
		}
		if (off < (i32)sIn.length()) {
			std::string sD = sIn.substr(off);
			aSet.push_back(sD);
		}
		return !aSet.empty();
	}

	bool CrossString::SplitNumber(i64 num, std::vector<std::string>& aSet)
	{
		aSet.clear();
		std::string s = Int64To10(num);
		for (std::string::iterator it = s.begin(); it != s.end(); it++) {
			char tmp[2] = {0};
			tmp[0] = *it;
			aSet.push_back(tmp);
		}
		return true;
	}

	std::string CrossString::ReplaceAll(const std::string& str, const std::string& old_value, const std::string& new_value)
	{
		std::string s = str;
		while(true) {
			std::string::size_type pos = 0;
			if((pos = s.find(old_value)) != std::string::npos)
				s.replace(pos, old_value.length(), new_value);
			else
				break;
		}
		return s;
	}

	std::string CrossString::Trim(const std::string& str)
	{
		std::string::size_type pos1 = 0;
		std::string::size_type pos2 = 0;
		pos1 = str.find_first_not_of(' ',pos1);
		if (pos1 == std::string::npos)
			return str;
		pos2 = str.find_last_not_of(' ', std::string::npos);
		return str.substr(pos1, pos2+1-pos1);
	}

	//add by yfw 超级文本框 coord 的格式构建
	//<coord 地图名 NPC真实名字 任务名>NPC显示名字</coord> or <coord 地图名>显示文本</coord>
	std::string CrossString::StringFormatCoord(cpstr szContent, i32 nX, i32 nY, cpstr szMapName, cpstr szRealName, cpstr szTaskName)
	{
		if (szContent || !szMapName)
			return "";
		if (szMapName && szRealName && szTaskName)
		{
			return CrossString::Format("<coord %d %d %s %s %s>%s</coord>", nX, nY, szMapName, szRealName, szTaskName, szContent);
		}
		else if (szMapName && szRealName)
		{
			return CrossString::Format("<coord %d %d %s %s>%s</coord>", nX, nY, szMapName, szRealName, szContent);
		}
		else if (szMapName)
		{
			return CrossString::Format("<coord %d %d %s>%s</coord>", nX, nY, szMapName, szContent);
		}
		return "";
	}
	//<coord X Y 地图名 NPC真实名字 任务名>NPC显示名字</coord> or <coord X Y 地图名>显示文本</coord>
	std::string CrossString::StringFormatCoord(cpstr szContent, cpstr szMapName, cpstr szRealName, cpstr szTaskName)
	{
		if (!szContent || !szMapName)
			return "";

		if (szMapName && szRealName && szTaskName)
		{
			return CrossString::Format("<coord %s %s %s>%s</coord>", szMapName, szRealName, szTaskName, szContent);
		}
		else if (szMapName && szRealName)
		{
			return CrossString::Format("<coord %s %s>%s</coord>", szMapName, szRealName, szContent);
		}
		else if (szMapName)
		{
			return CrossString::Format("<coord %s>%s</coord>", szMapName, szContent);
		}
		return "";
	}

	//得到sz 中 Coord格式中的nIndex段；
	//限制，只能获得sz中第一个<coord 。。。>。。</coord>格式，之后的将被忽略
	std::string CrossString::GetParamCoord(cpstr sz, i32 nIndex)
	{
		std::string strResult;
		if (!sz)
			return strResult;
		std::string strTemp = sz;

		if (nIndex >= 0)
		{
			i32 nStart = (i32)strTemp.find("<coord");
			i32 nEnd = (i32)strTemp.find("</coord>");
			i32 npos1=0, npos2=0;
			if (nStart>=0 && nEnd>=nStart)
			{
				//得到第nIndex段的左分段符位置
				npos1 = nStart;
				i32 n = 0;
				while (n < nIndex+1)
				{
					npos1 = (i32)strTemp.find_first_of(" ",npos1+1);
					n++;

					if (npos1 >= nEnd || (i32)(std::string::npos) == npos1)		//越界了
						return strResult;
				}

				if (npos1 > nStart)
				{
					npos2 = (i32)strTemp.find_first_of(" ",npos1+1);
					if (npos2 >= nEnd || (i32)(std::string::npos) == npos2)
					{
						npos2 = (i32)strTemp.find_first_of(">",npos1+1);
					}

					if (npos2 > npos1 && npos2 <= nEnd)
					{
						strResult = strTemp.substr(npos1+1, npos2-npos1-1);
					}
				}
			}
		}

		return strResult;
	}

	//bBefor = true取得 <coord 。。。>。。</coord>格式 的前端字符；即在<coord前面的字符
	//bAfter = true取得 <coord 。。。>。。</coord>格式 的后端字符；即在</coord>后面的字符
	//bBefor = false 且bAfter= false 取得显示字符
	std::string CrossString::GetContentCoord(cpstr sz, bool bBefor, bool bAfter)
	{
		std::string strResult;
		if (!sz)
			return strResult;

		std::string strTemp = sz;

		//前端字符串
		if (bBefor)
		{
			i32 npos = (i32)strTemp.find("<coord");

			if (npos > 0)
			{
				strResult = strTemp.substr(0, npos);
			}
		}
		//后端字符串
		else if (bAfter)
		{
			i32 npos = (i32)strTemp.find("</coord>");

			if (npos > 0)
			{
				strResult = strTemp.substr(npos+8);
			}
		}
		//显示字符
		else
		{
			i32 npos = (i32)strTemp.find("<coord");
			i32 npos1 = 0, npos2 = 0;
			if (-1 != npos)
			{
				npos1 = (i32)strTemp.find_first_of(">",npos);
				npos2 = (i32)strTemp.find("</coord>");

				if (npos1>0 && npos2>npos1)
				{
					strResult = strTemp.substr(npos1+1, npos2-npos1-1);
				}
			}
		}

		return strResult;
	}

	struct stRegexValidator
	{
		stRegexValidator() : m_pPcre(0) {}
		~stRegexValidator() {
			if (m_pPcre)
				pcre_free(m_pPcre);
		}
		pcre* m_pPcre;
	};
	typedef map<std::string, stRegexValidator> hmRegexValidator;
	typedef hmRegexValidator::iterator itRegexValidator;
	hmRegexValidator& GetHmRegexValidator()
	{
		static hmRegexValidator s_hmRegexValidator;
		return s_hmRegexValidator;
	}
	#define g_hmRegexValidator GetHmRegexValidator()

	bool CrossString::Match(const std::string& src, const std::string& regex)
	{
		stRegexValidator* pRv = 0;
		hmRegexValidator::iterator it = g_hmRegexValidator.find(regex);
		if (it == g_hmRegexValidator.end()) {
			cpstr prce_error;
			i32 pcre_erroff;
			pcre* p = pcre_compile(regex.c_str(), 0, &prce_error, &pcre_erroff, 0);
			if (p == 0)
				return false;
			pRv = &g_hmRegexValidator[regex];
			pRv->m_pPcre = p;
		}
		else
			pRv = &it->second;
		if (pRv == 0)
			return false;
		i32	match[3];
		i32 len = (i32)src.length();
		i32 result = pcre_exec(pRv->m_pPcre, 0, src.c_str(), (i32)src.length(), 0, 0, match, 3);
		if (result >= 0)
			return (match[1] - match[0] == len);
		else
			return false;
	}

	std::string CrossString::ToMD5(const std::string& sz)
	{
		return GetMD5Str(sz);
	}

	std::string CrossString::ToMD5Lower(const std::string& sz)
	{
		return GetMD5StrLower(sz);
	}

	static std::string Base64_Encode(const unsigned char* Data, int DataByte)
	{
		//编码表
		const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		//返回值
		string strEncode;
		unsigned char Tmp[4]={0};
		int LineLength=0;
		for(int i=0;i<(int)(DataByte / 3);i++)
		{
			Tmp[1] = *Data++;
			Tmp[2] = *Data++;
			Tmp[3] = *Data++;
			strEncode+= EncodeTable[Tmp[1] >> 2];
			strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
			strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
			strEncode+= EncodeTable[Tmp[3] & 0x3F];
			if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
		}
		//对剩余数据进行编码
		int Mod=DataByte % 3;
		if(Mod==1)
		{
			Tmp[1] = *Data++;
			strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
			strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
			strEncode+= "==";
		}
		else if(Mod==2)
		{
			Tmp[1] = *Data++;
			Tmp[2] = *Data++;
			strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
			strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
			strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
			strEncode+= "=";
		}

		return strEncode;
	}

	static std::string Base64_Decode(const char* Data, int DataByte, int& OutByte)
	{
		//解码表
		const char DecodeTable[] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			62, // '+'
			0, 0, 0,
			63, // '/'
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
			0, 0, 0, 0, 0, 0, 0,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
			0, 0, 0, 0, 0, 0,
			26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
			39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
		};
		//返回值
		string strDecode;
		int nValue;
		int i= 0;
		while (i < DataByte)
		{
			if (*Data != '\r' && *Data!='\n')
			{
				nValue = DecodeTable[*Data++] << 18;
				nValue += DecodeTable[*Data++] << 12;
				strDecode+=(nValue & 0x00FF0000) >> 16;
				OutByte++;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++] << 6;
					strDecode+=(nValue & 0x0000FF00) >> 8;
					OutByte++;
					if (*Data != '=')
					{
						nValue += DecodeTable[*Data++];
						strDecode+=nValue & 0x000000FF;
						OutByte++;
					}
				}
				i += 4;
			}
			else// 回车换行,跳过
			{
				Data++;
				i++;
			}
		}
		return strDecode;
	}

	std::string CrossString::EncryptAES(const std::string& sz, const std::string& key, const std::string& iv)
	{
		AES_KEY aes;
		std::string ret;
		ret.resize(sz.size());
		int rs = AES_set_encrypt_key1((ui8*)key.data(), 128, &aes);
		if (rs < 0)
			return "";
		AES_cbc_encrypt1((ui8*)sz.data(), (ui8*)ret.data(), ret.size(), &aes, (ui8*)iv.data(), AES_ENCRYPT);
		return Base64_Encode((unsigned char*)ret.data(), ret.length());
	}

	std::string CrossString::DecryptAES(const std::string& sz, const std::string& key, const std::string& iv)
	{
		AES_KEY aes;
		int out_len = 0;
		std::string in = Base64_Decode(sz.data(), sz.size(), out_len);
		std::string ret;
		ret.resize(out_len);
		int rs = AES_set_decrypt_key1((ui8*)key.data(), 128, &aes);
		if (rs < 0)
			return "";
		AES_cbc_encrypt1((ui8*)in.data(), (ui8*)ret.data(), ret.size(), &aes, (ui8*)iv.data(), AES_DECRYPT);
		return ret;
	}

	std::string CrossString::MyEncrypt(std::string src)
	{
		std::string key = "66b178cd96685385e019e313be5d9bb8";
		std::string result = Base64_Encode((unsigned char*)src.c_str(), strlen(src.c_str()));
		return ToMD5(result + key) + result;
	}

	std::string CrossString::MyDecrypt(std::string src)
	{
		std::string key = "66b178cd96685385e019e313be5d9bb8";
		if(src.length() < 32)
			return "0";
		std::string str_md5 = src.substr(0,32);
		std::string str_data = src.substr(32,src.length() - 32);
		//printf("md5=%s",str_md5);
		if(str_md5 != ToMD5(str_data + key) )
			return "0";
		int out_len = 0;
		std::string in = Base64_Decode(str_data.data(), str_data.size(), out_len);
		return in;
	}

}
