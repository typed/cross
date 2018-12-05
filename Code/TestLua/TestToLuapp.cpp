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

#define OBJ_BIND_MAPPING "obj_bind_mapping"

static void printLuaStack(lua_State* L, const char* pre = "")
{
    int stackTop = lua_gettop(L);
    int nIdx = 0;
    int nType;
    printf("%s  ", pre);
    for (nIdx = stackTop; nIdx > 0; --nIdx)
    {
        nType = lua_type(L, nIdx);
        const char* st = lua_typename(L, nType);
        const char* sv = luaL_tolstring(L, nIdx, NULL);
        lua_pop(L, 1);
        printf("%d:%s(%s)|", nIdx, st, sv);
    }
    stackTop = lua_gettop(L);
    printf("\n");
}

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
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self) {
		self->Trace();
		return 0;
	}
	LError << "'Trace' function of Node has wrong self == nullptr" << LEnd;
	return 0;
}

static int lua_Node_SetName(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	if (!tolua_iscppstring(L, 2, 0, &tolua_err)) {
		LError << "args 2 invalid" << LEnd;
		return 0;
	}
	const char* str = tolua_tocppstring(L, 2, "");
	self->SetName(str);
	return 0;
}

static int lua_Node_GetName(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	const string& str = self->GetName();
	tolua_pushcppstring(L, str);
	return 1;
}

static int lua_Node_AddChild(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	if (!tolua_isusertype(L, 2, "Node", 0, &tolua_err)) {
		LError << "args 2 invalid" << LEnd;
		return 0;
	}
	Node* nod = (Node*)tolua_tousertype(L, 2, 0);
	if (nod == nullptr) {
		LError << "args 2 is nil" << LEnd;
		return 0;
	}
	bool ret = self->AddChild(nod);
	tolua_pushboolean(L, ret);
	return 1;
}

static int lua_Node_RemoveChild(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	if (!tolua_isusertype(L, 2, "Node", 0, &tolua_err)) {
		LError << "args 2 invalid" << LEnd;
		return 0;
	}
	Node* nod = (Node*)tolua_tousertype(L, 2, 0);
	if (nod == nullptr) {
		LError << "args 2 is nil" << LEnd;
		return 0;
	}
	self->RemoveChild(nod);
	return 0;
}

static int lua_Node_RemoveAllChild(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	self->RemoveAllChild();
	return 0;
}

static int lua_Node_RemoveFromParent(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	self->RemoveFromParent();
	return 0;
}

static int lua_Node_FindChildByName(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	if (!tolua_iscppstring(L, 2, 0, &tolua_err)) {
		LError << "args 2 invalid" << LEnd;
		return 0;
	}
	const string& name = tolua_tocppstring(L, 2, "");
	Node* tolua_ret = self->FindChildByName(name);
	if (tolua_ret) {
		toluafix_pushusertype_ccobject(L, tolua_ret->_ID, &tolua_ret->_luaID, tolua_ret, "Node");
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

static int lua_Node_SetLuaUserData(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
    Node* self = (Node*)tolua_tousertype(L, 1, 0);
    if (tolua_istable(L, 2, 0, &tolua_err)) {
        int tbl = tolua_tovalue(L, 2, 0);
        toluafix_add_usertable_by_refid(L, self->_luaID, tbl);
		return 0;
    }
    if (lua_isnil(L, 2)) {
        toluafix_remove_usertable_by_refid(L, self->_luaID);
        return 0;
    }
    LError << "args 2 must table or nil." << LEnd;
    return 0;
}

static int lua_Node_GetLuaUserData(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
    Node* self = (Node*)tolua_tousertype(L, 1, 0);
    lua_pushstring(L, TOLUA_REFID_USERTABLE_MAPPING);
    lua_rawget(L, LUA_REGISTRYINDEX);
    lua_pushinteger(L, self->_luaID);
    printLuaStack(L, "");
    lua_rawget(L, -2);
    printLuaStack(L, "");
    lua_remove(L, -2);
    printLuaStack(L, "");
    //toluafix_get_usertable_by_refid(L, self->_luaID);
	return 1;
}

static int lua_Node_CallFuncLuaUserData(lua_State* L)
{
    tolua_Error tolua_err;
    if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
        LError << "args 1 invalid" << LEnd;
        return 0;
    }
    Node* self = (Node*)tolua_tousertype(L, 1, 0);
    const char* func = "test";
    lua_pushstring(L, TOLUA_REFID_USERTABLE_MAPPING);
    lua_rawget(L, LUA_REGISTRYINDEX);
    lua_pushinteger(L, self->_luaID);
    lua_rawget(L, -2);
    //printLuaStack(L, "");
    lua_remove(L, -2);
    //printLuaStack(L, "");
    if (!lua_istable(L, -1)) {
        return 0;
    }
    lua_pushstring(L, func);
    //printLuaStack(L, "");
    lua_rawget(L, -2);
    if (!lua_isfunction(L, -1)) {
        return 0;
    }
    //printLuaStack(L, "");
    lua_replace(L, -3);
    //printLuaStack(L, "");
    lua_call(L, 1, 0);
    //printLuaStack(L, "");
    return 0;
}

static int lua_Node_GetClassType(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Node", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Node* self = (Node*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	const char* str = self->GetClassType();
	tolua_pushstring(L, str);
	return 1;
}

static int lua_Char_New(lua_State* L)
{
	Char* tolua_ret = Char::New();
	if (tolua_ret) {
		toluafix_pushusertype_ccobject(L, tolua_ret->_ID, &tolua_ret->_luaID, tolua_ret, "Char");
		return 1;
	}
	LError << "'New' function of Char has wrong self == nullptr" << LEnd;
	lua_pushnil(L);
	return 1;
}

static int lua_Char_SetAge(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Char", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Char* self = (Char*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	if (!tolua_isnumber(L, 2, 0, &tolua_err)) {
		LError << "args 2 invalid" << LEnd;
		return 0;
	}
	int arg = tolua_tonumber(L, 2, 0);
	self->SetAge(arg);
	return 0;
}

static int lua_Char_GetAge(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Char", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Char* self = (Char*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	int age = self->GetAge();
	tolua_pushnumber(L, age);
	return 1;
}

static int lua_Player_New(lua_State* L)
{
	Player* tolua_ret = Player::New();
	if (tolua_ret) {
		toluafix_pushusertype_ccobject(L, tolua_ret->_ID, &tolua_ret->_luaID, tolua_ret, "Player");
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

static int lua_Player_SetControl(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Player", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Player* self = (Player*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	if (!tolua_isnumber(L, 2, 0, &tolua_err)) {
		LError << "args 2 invalid" << LEnd;
		return 0;
	}
	int arg = tolua_tonumber(L, 2, 0);
	self->SetControl(arg);
	return 0;
}

static int lua_Player_GetControl(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "Player", 0, &tolua_err)) {
		LError << "args 1 invalid" << LEnd;
		return 0;
	}
	Player* self = (Player*)tolua_tousertype(L, 1, 0);
	if (self == nullptr) {
		LError << "args 1 is nil" << LEnd;
		return 0;
	}
	int age = self->GetControl();
	tolua_pushnumber(L, age);
	return 1;
}

static int lua_GetRootNode(lua_State* L)
{
	Node* pRoot = GetRootNode();
	if (pRoot) {
		toluafix_pushusertype_ccobject(L, pRoot->_ID, &pRoot->_luaID, pRoot, "Node");
		return 1;
	}
	lua_pushnil(L);
	return 1;
}


void TestToLuapp(int argc, char const *argv[])
{
	LInfo << "TestToLua++." << LEnd;
	GetRootNode()->SetName("Root");
	lua_State* L = LuaInit();
	g_lua_State = L;
	tolua_open(L);
	toluafix_open(L);

	lua_pushstring(L, OBJ_BIND_MAPPING);
	lua_newtable(L);
	lua_rawset(L, LUA_REGISTRYINDEX);
	
	tolua_module(L, 0, 0);
	tolua_beginmodule(L, 0);

	tolua_usertype(L, "Node");
	tolua_cclass(L, "Node", "Node", "", NULL);
	tolua_beginmodule(L, "Node");
		tolua_function(L, "New", lua_Node_New);
		tolua_function(L, "Trace", lua_Node_Trace);
		tolua_function(L, "GetClassType", lua_Node_GetClassType);
		tolua_function(L, "SetName", lua_Node_SetName);
		tolua_function(L, "GetName", lua_Node_GetName);
		tolua_function(L, "AddChild", lua_Node_AddChild);
		tolua_function(L, "RemoveChild", lua_Node_RemoveChild);
		tolua_function(L, "RemoveAllChild", lua_Node_RemoveAllChild);
		tolua_function(L, "RemoveFromParent", lua_Node_RemoveFromParent);
		tolua_function(L, "FindChildByName", lua_Node_FindChildByName);
		tolua_function(L, "SetLuaUserData", lua_Node_SetLuaUserData);
		tolua_function(L, "GetLuaUserData", lua_Node_GetLuaUserData);
        tolua_function(L, "CallFuncLuaUserData", lua_Node_CallFuncLuaUserData);
	tolua_endmodule(L);

	tolua_usertype(L, "Char");
	tolua_cclass(L, "Char", "Char", "Node", NULL);
	tolua_beginmodule(L, "Char");
		tolua_function(L, "New", lua_Char_New);
		tolua_function(L, "SetAge", lua_Char_SetAge);
		tolua_function(L, "GetAge", lua_Char_GetAge);
	tolua_endmodule(L);

	tolua_usertype(L, "Player");
	tolua_cclass(L, "Player", "Player", "Char", NULL);
	tolua_beginmodule(L, "Player");
		tolua_function(L, "New", lua_Player_New);
		tolua_function(L, "SetControl", lua_Player_SetControl);
		tolua_function(L, "GetControl", lua_Player_GetControl);
	tolua_endmodule(L);

	tolua_function(L, "GetRootNode", lua_GetRootNode);

	tolua_endmodule(L);

	DoFile(L, "Lua/TestToLuapp.lua");
	LuaRelease(L);
	g_lua_State = nullptr;
}