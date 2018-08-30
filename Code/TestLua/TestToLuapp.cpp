#include "tolua++.h"
#include "TestToLuapp.h"
#include "TestClass.h"
#include "Common.h"
#include "iLog.h"
#include "iReference.h"
#include "LuaHelper.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cross;

void TestToLuapp(int argc, char const *argv[])
{
	LInfo << "TestToLuapp." << LEnd;
	GetRootNode()->SetName("Root");
	lua_State* lua_state = LuaInit();
	DoFile(lua_state, "Lua/TestLuaBridge.lua");
	LuaRelease(lua_state);
}