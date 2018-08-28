extern "C" {
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
}
#include "LuaBridge\LuaBridge.h"
#include "Common\Common.h"
#include "Common\LuaHelper.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cross;

class Node
{
public:
	Node() {
		m_sName = "Node";
		m_iOrder = 0;
		m_iType = 0;
	}
	virtual void Display() {
		cout << "m_sName=" << m_sName << " m_iType=" << m_iType << " m_iOrder=" << m_iOrder << endl;
	}
	int m_iOrder;
	int m_iType;
	string m_sName;
};

class Char : public Node
{
public:
	Char(string name) {
		m_sName = name;
		m_iOrder = 0;
		m_iType = 1;
		m_iAge = 10;
	}
	int m_iAge;
};

int main(int argc, char const *argv[])
{
    cout << "TestLuaBridge." << endl;
	InitCommon();
	lua_State* lua_state = LuaInit();
	luabridge::getGlobalNamespace(lua_state)
		.beginNamespace("test")
		.beginClass<Node>("Node")
		.addConstructor<void(*) (void)>()//无参构造函数的注册
		.addData("m_sName", &Node::m_sName)//注册变量到lua
		.addFunction("Display", &Node::Display)//注册test、方法到lua（addStaticFunction静态函数注册也类似）
		//.addProperty("name", &test_lua::GetName, &test_lua::SetName)//属性方法的注册（addStaticProperty静态属性方法也类似）
		//.addCFunction("cFunc", &test_lua::cFunc)//注册返回多个参数给lua的方法
		.endClass()
		.deriveClass<Char, Node>("Char")//子类的注册
		.addConstructor<void(*) (string)>()//有参构造函数的注册
		.addData("m_iAge", &Char::m_iAge)//注册变量到lua
		.endClass()
		.endNamespace();
	DoFile(lua_state, "Lua/test.lua");
	LuaRelease(lua_state);
	ReleaseCommon();
    return 0;
}