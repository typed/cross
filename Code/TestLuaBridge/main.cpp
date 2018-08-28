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
		.addConstructor<void(*) (void)>()//�޲ι��캯����ע��
		.addData("m_sName", &Node::m_sName)//ע�������lua
		.addFunction("Display", &Node::Display)//ע��test��������lua��addStaticFunction��̬����ע��Ҳ���ƣ�
		//.addProperty("name", &test_lua::GetName, &test_lua::SetName)//���Է�����ע�ᣨaddStaticProperty��̬���Է���Ҳ���ƣ�
		//.addCFunction("cFunc", &test_lua::cFunc)//ע�᷵�ض��������lua�ķ���
		.endClass()
		.deriveClass<Char, Node>("Char")//�����ע��
		.addConstructor<void(*) (string)>()//�вι��캯����ע��
		.addData("m_iAge", &Char::m_iAge)//ע�������lua
		.endClass()
		.endNamespace();
	DoFile(lua_state, "Lua/test.lua");
	LuaRelease(lua_state);
	ReleaseCommon();
    return 0;
}