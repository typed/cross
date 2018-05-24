//�������ƣ�Cross.h
//������������ƽ̨�м��
//�����ˣ�	������
//����ʱ�䣺2012-11-9
//�޸��ߣ�
//�޸�ʱ�䣺
//�޸�˵����

#ifndef crossstring_h
#define crossstring_h

#include "CommonDef.h"
#include <vector>
#include <string>
#include <string.h>

namespace cm
{

//�ַ�
class CrossString
{
public:
	//�ַ����Ƚ�
	static i32 StrCmp(cpstr, cpstr);
	//�ַ�������
	static void StrCpy(pstr, cpstr);
	//�ַ�������N��
	static void StrNCpy(pstr, cpstr, ui64);
	//�ַ������ӣ�Add by lkm,2013.3.22
	static void StrCat(pstr des, cpstr src);
	//���ַ������ȣ�Add by lkm,2013.3.25
	static ui32 StrLen(cpstr s);
	//ת��ΪСд
	static std::string ToLower(const std::string&);
	//ת��Ϊ��д
	static std::string ToUpper(const std::string&);
	//�ַ����Ƿ����
	static bool Equal(cpstr, cpstr);
	//�ַ����Ƿ���Ⱥ��Դ�Сд
	static bool EqualIgnore(cpstr, cpstr);
	//�ַ����Ƿ���Ⱥ��Դ�Сд
	static bool EqualIgnore(const std::string&, const std::string&);
	//�ַ��� == "null"
	static bool EqualStrNull(cpstr);
	//��ʽ���ַ���,max 102400byte
	static std::string Format(cpstr src, ...);
	//sscanf,��ʱû��ʵ��
	static void SScanf(cpstr src, cpstr sText, ...);
	//UTF8תASCII
	static std::string UTF8ToASCII(const std::string& utf8);
	//ASCIIתUTF8
	static std::string ASCIIToUTF8(const std::string& ascii);
	//UTF8תWString
	static std::wstring UTF8ToWString(const std::string& utf8);
	//WStringתUTF8
	static std::string WStringToUTF8(const std::wstring& wstr);
	//���ַ���
	static bool Split(const std::string& s, const std::string& sign, std::vector<std::string>& aSet);
	static std::string Split(const std::string& s, const std::string& sign, i32& offset);
	//add by yfw ��Split�Ļ����ϣ����ӹ���secSignStart��secSignEnd�г��ֵ�sign���Ų����ĶΣ�����secSignStart��secSignEndΧ�ɵĶΣ����ᱻ��Ϊ����
	static bool SplitEx(const std::string& s, const std::string& sign, const std::string& secSignStart, const std::string& secSignEnd, std::vector<std::string>& aSet);
	static bool SplitNumber(i64, std::vector<std::string>& aSet);
	//�滻
	static std::string ReplaceAll(const std::string& str, const std::string& old_value, const std::string& new_value);
	static std::string Trim(const std::string& str);

	//����<coord > .. <\coord>�ĸ�ʽ��add by yfw
	//add by yfw �����ı��� coord �ĸ�ʽ����
	//<coord ��ͼ�� NPC��ʵ���� ������>NPC��ʾ����</coord> or <coord ��ͼ��>��ʾ�ı�</coord>
	static std::string StringFormatCoord(cpstr szContent, i32 nX, i32 nY, cpstr szMapName, cpstr szRealName = 0, cpstr szTaskName = 0);
	//<coord X Y ��ͼ�� NPC��ʵ���� ������>NPC��ʾ����</coord> or <coord X Y ��ͼ��>��ʾ�ı�</coord>
	static std::string StringFormatCoord(cpstr szContent, cpstr szMapName, cpstr szRealName = 0, cpstr szTaskName = 0);

	//�õ�sz �� Coord��ʽ�е�nIndex�Σ��� bContent=trueʱ������nIndex��ֱ�ӻ����ʾ�Ρ�
	//���ƣ�ֻ�ܻ��sz�е�һ��<coord ������>����</coord>��ʽ��֮��Ľ�������
	static std::string GetParamCoord(cpstr sz, i32 nIndex);
	//ȡ<coord ������>����</coord>��ʽ��ǰ���ַ��������ַ�������ʾ�ַ�
	static std::string GetContentCoord(cpstr sz, bool bBefor, bool bAfter);

	//�ַ� -> bool
	static bool ToBool(const std::string& s);
	//�ַ�10���� -> int
	static i32 _10ToInt(const std::string& s);
	//�ַ�10���� -> long Add by lkm,2013.3.26
	static l32 _10ToLong(const std::string& s);
	//�ַ�10���� -> float
	static f32 _10ToFloat(const std::string& s);
	//�ַ�10���� -> ui32
	static ui32 _10ToUInt(const std::string& s);
	//�ַ�16���� -> unsigned long
	static ui32 _16ToUInt(const std::string& s);

	static ui32 StrToHex(const std::string& s);
	//�ַ�10���� -> double
	static f64 _10ToDouble(const std::string& s);
	//int -> �ַ�10����
	static std::string IntTo10(i32 num);
	//unsigned long -> �ַ�10����
	static std::string UIntTo10(ui32 num);
	//float -> �ַ�10����
	static std::string FloatTo10(f32 num);
	//int -> �ַ�16����
	static std::string IntTo16(i32 num);
	//unsigned long -> �ַ�16����
	static std::string UIntTo16(ui32 num);
	//unsigned long -> �ַ�16����
	static std::string UIntTo016(ui32 num);
	//long long -> �ַ�10����
	static std::string Int64To10(i64 num);

	static i8 Char16ToBinary(i8 c);
	//�ַ�16����תbinary
	static std::string StringToBinary(std::string s);
	//bufferת�ַ�16����
	static std::string BinaryToString(cpstr aBuf, ui32 len);
	static std::string BinaryToString(const std::string& aBuf) {
		return BinaryToString(aBuf.c_str(), (ui32)aBuf.size());
	}
	//�Ƿ�������֧������λ����'+'��'-'���ŵ� add by yfw
	static bool IsInt(std::string s);
	//������ʽ
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