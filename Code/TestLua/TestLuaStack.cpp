#include "TestLuaStack.h"
#include "Common.h"
#include "iLog.h"
#include "LuaHelper.h"
#include "tolua++.h"
#include "tolua_fix.h"

using namespace std;
using namespace cross;

void TestLuaStack(int argc, char const *argv[])
{
	LInfo << "TestLuaStack." << LEnd;
	lua_State* L = LuaInit();
	tolua_open(L);
	toluafix_open(L);
	
	LInfo << "lua stack." << LEnd;
	LInfo << lua_gettop(L) << LEnd;
	lua_pushstring(L, "g_tb_reg");
	LInfo << lua_gettop(L) << LEnd;
	lua_newtable(L);
	LInfo << lua_gettop(L) << LEnd;
	lua_rawset(L, LUA_REGISTRYINDEX);
	LInfo << lua_gettop(L) << LEnd;

	lua_pushstring(L, "g_tb_reg");
	lua_rawget(L, LUA_REGISTRYINDEX);
	
	LuaRelease(L);
}