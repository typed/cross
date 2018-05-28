//�������ƣ�Cross.h
//������������ƽ̨�м��
//�����ˣ�	������
//����ʱ�䣺2012-11-9
//�޸��ߣ�
//�޸�ʱ�䣺
//�޸�˵����

#ifndef crossconsole_h
#define crossconsole_h

#include "CommonDef.h"
#include <string>

namespace cm
{

//����̨
class CrossConsole
{
public:
	//����̨����,����
	static std::string GetString();
	//����̨����,������
	static bool GetString(std::string&);
	//����̨��ͣ
	static void Pause();
	enum eColor {
		CC_White,
		CC_Red,
		CC_Yellow,
		CC_Green,
		CC_Blue,
		CC_Cyan,
	};
	//����̨���������ɫ
	static void SetColor(eColor fgCol = CC_White);
	static bool IsShow();
	static void Show();
	static void Hide();
	static void Write(i8 ch);
};

}
#endif