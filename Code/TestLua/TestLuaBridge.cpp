#include "TestLuaBridge.h"
#include "TestClass.h"
#include "Common.h"
#include "iLog.h"
#include "iReference.h"
#include "LuaHelper.h"
#include "LuaBridge.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cross;

void TestLuaBridge(int argc, char const *argv[])
{
	LInfo << "TestLuaBridge." << LEnd;
	GetRootNode()->SetName("Root");
	lua_State* lua_state = LuaInit();
	luabridge::getGlobalNamespace(lua_state)
		.beginClass<Node>("Node")
			//.addConstructor<void(*) (void)>()
			.addStaticFunction("New", &Node::New)
			.addFunction("SetName", &Node::SetName)
			.addFunction("GetName", &Node::GetName)
			.addFunction("SetTag", &Node::SetTag)
			.addFunction("GetTag", &Node::GetTag)
			.addFunction("AddChild", &Node::AddChild)
			.addFunction("RemoveChild", &Node::RemoveChild)
			.addFunction("RemoveAllChild", &Node::RemoveAllChild)
			.addFunction("RemoveFromParent", &Node::RemoveFromParent)
			.addFunction("Trace", &Node::Trace)
		.endClass()
		.deriveClass<Char, Node>("Char")
			//.addConstructor<void(*) (int)>()
			.addStaticFunction("New", &Char::New)
			.addFunction("SetAge", &Char::SetAge)
			.addFunction("GetAge", &Char::GetAge)
		.endClass()
		.deriveClass<Player, Char>("Player")
			//.addConstructor<void(*) (int, int)>()
			.addStaticFunction("New", &Player::New)
			.addFunction("SetControl", &Player::SetControl)
			.addFunction("GetControl", &Player::GetControl)
		.endClass()
		.addFunction("GetRootNode", GetRootNode)
	;
	DoFile(lua_state, "Lua/TestLuaBridge.lua");
	LuaRelease(lua_state);
}