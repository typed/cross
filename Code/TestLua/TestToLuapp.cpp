#include "TestToLuapp.h"
#include "TestClass.h"
#include "Common.h"
#include "iLog.h"
#include "iReference.h"
#include "LuaHelper.h"
#include "tolua_fix.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cross;

static int lua_Node_New(lua_State* L)
{
	Node* tolua_ret = Node::New();
	if (tolua_ret) {
		toluafix_pushusertype_ccobject(L, tolua_ret->_ID, &tolua_ret->_luaID, tolua_ret, "Node");
		return 1;
	}
	LError << "'New' function of Node has wrong self == nullptr" << LEnd;
	lua_pushnil(L);
	return 1;
}

static int lua_Node_Trace(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, nullptr);
	if (self) {
		self->Trace();
		return 0;
	}
	LError << "'Trace' function of Node has wrong self == nullptr" << LEnd;
	return 0;
}

void TestToLuapp(int argc, char const *argv[])
{
	LInfo << "TestToLua++." << LEnd;
	GetRootNode()->SetName("Root");
	lua_State* L = LuaInit();
	g_lua_State = L;
	tolua_open(L);
	toluafix_open(L);
	
	tolua_module(L, 0, 0);
	tolua_beginmodule(L, 0);

	tolua_usertype(L, "Node");
	tolua_cclass(L, "Node", "Node", "", NULL);
	tolua_beginmodule(L, "Node");
		tolua_function(L, "New", lua_Node_New);
		tolua_function(L, "Trace", lua_Node_Trace);
	tolua_endmodule(L);

	tolua_endmodule(L);

	DoFile(L, "Lua/TestToLuapp.lua");
	LuaRelease(L);
	g_lua_State = nullptr;
}