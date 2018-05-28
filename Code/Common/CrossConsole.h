//程序名称：Cross.h
//功能描述：跨平台中间件
//开发人：	龚晓剑
//开发时间：2012-11-9
//修改者：
//修改时间：
//修改说明：

#ifndef crossconsole_h
#define crossconsole_h

#include "CommonDef.h"
#include <string>

namespace cross
{

//控制台
class CrossConsole
{
public:
	//控制台输入,阻塞
	static std::string GetString();
	//控制台输入,非阻塞
	static bool GetString(std::string&);
	//控制台暂停
	static void Pause();
	enum eColor {
		CC_White,
		CC_Red,
		CC_Yellow,
		CC_Green,
		CC_Blue,
		CC_Cyan,
	};
	//控制台输出字体颜色
	static void SetColor(eColor fgCol = CC_White);
	static bool IsShow();
	static void Show();
	static void Hide();
	static void Write(i8 ch);
};

}
#endif