#ifndef _cross_common_luahelper_h_
#define _cross_common_luahelper_h_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "Common.h"
#include "iLog.h"
#include "iStream.h"
#include "CrossMemory.h"
#include <string>
#include <string.h>

namespace cross
{

static cpstr c_sLSE = "script_error";

#define LuaTry \
	try {

#define LuaCatch \
	} catch(...) {\
		LogError(c_sLSE) << "unknow error c++ crash." << LEnd;\
		throw;\
	}\
	return true;

inline void Push(lua_State* L, bool value) {
	lua_pushboolean(L, value);
}
inline void Push(lua_State* L, i8 value) {
	lua_pushnumber(L, value);
}
inline void Push(lua_State* L, ui8 value) {
	lua_pushnumber(L, value);
}
inline void Push(lua_State* L, i16 value) {
	lua_pushnumber(L, value);
}
inline void Push(lua_State* L, ui16 value) {
	lua_pushnumber(L, value);
}
inline void Push(lua_State* L, i32 value) {
	lua_pushnumber(L, value);
}
inline void Push(lua_State* L, ui32 value) {
	lua_pushnumber(L, value);
}
inline void Push(lua_State* L, l32 value) {
	lua_pushnumber(L, value);
}
inline void Push(lua_State* L, ul32 value) {
	lua_pushnumber(L, value);
}
inline void Push(lua_State* L, f64 value) {
	lua_pushnumber(L, (lua_Number)value);
}
inline void Push(lua_State* L, f32 value) {
	lua_pushnumber(L, (lua_Number)value);
}
inline void Push(lua_State* L, cpi8 value) {
	lua_pushstring(L, value);
}
inline void Push(lua_State* L, std::string value) {
	lua_pushlstring(L, value.c_str(), value.size());
}
inline void Push(lua_State* L, cIStream value) {
	lua_pushlstring(L, value.GetData(), value.GetDataSize());
}

template<class T> struct TypeWrapper {};

inline bool	Match(TypeWrapper<bool>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TBOOLEAN;
}
inline bool	Match(TypeWrapper<i8>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<i8>, lua_State* L, ui32 idx) {
	return lua_type(L, idx) == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<i16>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<ui16>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<i32>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<ui32>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<l32>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<ul32>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<f32>, lua_State* L, i32 idx) {
	int type = lua_type(L, idx);
	return type == LUA_TNUMBER || type == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<f64>, lua_State* L, i32 idx) {
	int type = lua_type(L, idx);
	return type == LUA_TNUMBER || type == LUA_TNUMBER;
}
inline bool	Match(TypeWrapper<cpi8>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TSTRING || lua_type(L, idx) == LUA_TNIL;
}
inline bool	Match(TypeWrapper<std::string>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TSTRING;
}
inline bool	Match(TypeWrapper<cIStream>, lua_State* L, i32 idx) {
	return lua_type(L, idx) == LUA_TSTRING;
}

inline cpstr GetTypeLua(lua_State* L, i32 idx) {
	switch(lua_type(L, idx)) {
	case LUA_TBOOLEAN:
		return "boolean";
	case LUA_TNUMBER:
		return "number";
	case LUA_TSTRING:
		return "std::string";
	case LUA_TNIL:
		return "nil";
	default:
		return "unknow";
	}
}
inline cpstr GetTypeCpp(TypeWrapper<bool>) {
	return "boolean";
}
inline cpstr GetTypeCpp(TypeWrapper<i8>) {
	return "number";
}
inline cpstr GetTypeCpp(TypeWrapper<ui8>) {
	return "number";
}
inline cpstr GetTypeCpp(TypeWrapper<i16>) {
	return "number";
}
inline cpstr GetTypeCpp(TypeWrapper<ui16>) {
	return "number";
}
inline cpstr GetTypeCpp(TypeWrapper<i32>) {
	return "number";
}
inline cpstr GetTypeCpp(TypeWrapper<ui32>) {
	return "number";
}
inline cpstr GetTypeCpp(TypeWrapper<l32>) {
	return "number";
}
inline cpstr GetTypeCpp(TypeWrapper<ul32>) {
	return "number";
}
inline cpstr GetTypeCpp(TypeWrapper<f32>) {
	return "float";
}
inline cpstr GetTypeCpp(TypeWrapper<f64>) {
	return "float";
}
inline cpstr GetTypeCpp(TypeWrapper<cpi8>) {
	return "std::string";
}
inline cpstr GetTypeCpp(TypeWrapper<std::string>) {
	return "std::string";
}
inline cpstr GetTypeCpp(TypeWrapper<cIStream>) {
	return "std::string";
}

#define luaL_argassert_begin \
	i32	argid = 0;

#define luaL_argassert(arg, _index_) \
	if (!Match(TypeWrapper<P##arg>(), L, _index_)) {\
		argid = arg;\
		goto err;\
	}

#define luaL_argassert_end1(arg1) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

#define luaL_argassert_end2(arg1,arg2) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "," + GetTypeLua(L, arg2) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "," + GetTypeCpp(TypeWrapper<P##arg2>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

#define luaL_argassert_end3(arg1,arg2,arg3) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "," + GetTypeLua(L, arg2) + "," + GetTypeLua(L, arg3) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "," + GetTypeCpp(TypeWrapper<P##arg2>()) + "," + GetTypeCpp(TypeWrapper<P##arg3>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

#define luaL_argassert_end4(arg1,arg2,arg3,arg4) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "," + GetTypeLua(L, arg2) + "," + GetTypeLua(L, arg3) + "," + GetTypeLua(L, arg4) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "," + GetTypeCpp(TypeWrapper<P##arg2>()) + "," + GetTypeCpp(TypeWrapper<P##arg3>()) + "," + GetTypeCpp(TypeWrapper<P##arg4>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

#define luaL_argassert_end5(arg1,arg2,arg3,arg4,arg5) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "," + GetTypeLua(L, arg2) + "," + GetTypeLua(L, arg3) + "," + GetTypeLua(L, arg4) + "," + GetTypeLua(L, arg5) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "," + GetTypeCpp(TypeWrapper<P##arg2>()) + "," + GetTypeCpp(TypeWrapper<P##arg3>()) + "," + GetTypeCpp(TypeWrapper<P##arg4>()) + "," + GetTypeCpp(TypeWrapper<P##arg5>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

#define luaL_argassert_end6(arg1,arg2,arg3,arg4,arg5,arg6) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "," + GetTypeLua(L, arg2) + "," + GetTypeLua(L, arg3) + "," + GetTypeLua(L, arg4) + "," + GetTypeLua(L, arg5) + "," + GetTypeLua(L, arg6) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "," + GetTypeCpp(TypeWrapper<P##arg2>()) + "," + GetTypeCpp(TypeWrapper<P##arg3>()) + "," + GetTypeCpp(TypeWrapper<P##arg4>()) + "," + GetTypeCpp(TypeWrapper<P##arg5>()) + "," + GetTypeCpp(TypeWrapper<P##arg6>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

#define luaL_argassert_end7(arg1,arg2,arg3,arg4,arg5,arg6,arg7) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "," + GetTypeLua(L, arg2) + "," + GetTypeLua(L, arg3) + "," + GetTypeLua(L, arg4) + "," + GetTypeLua(L, arg5) + "," + GetTypeLua(L, arg6) + "," + GetTypeLua(L, arg7) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "," + GetTypeCpp(TypeWrapper<P##arg2>()) + "," + GetTypeCpp(TypeWrapper<P##arg3>()) + "," + GetTypeCpp(TypeWrapper<P##arg4>()) + "," + GetTypeCpp(TypeWrapper<P##arg5>()) + "," + GetTypeCpp(TypeWrapper<P##arg6>()) + "," + GetTypeCpp(TypeWrapper<P##arg7>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

#define luaL_argassert_end8(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "," + GetTypeLua(L, arg2) + "," + GetTypeLua(L, arg3) + "," + GetTypeLua(L, arg4) + "," + GetTypeLua(L, arg5) + "," + GetTypeLua(L, arg6) + "," + GetTypeLua(L, arg7) + "," + GetTypeLua(L, arg8) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "," + GetTypeCpp(TypeWrapper<P##arg2>()) + "," + GetTypeCpp(TypeWrapper<P##arg3>()) + "," + GetTypeCpp(TypeWrapper<P##arg4>()) + "," + GetTypeCpp(TypeWrapper<P##arg5>()) + "," + GetTypeCpp(TypeWrapper<P##arg6>()) + "," + GetTypeCpp(TypeWrapper<P##arg7>()) + "," + GetTypeCpp(TypeWrapper<P##arg8>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

#define luaL_argassert_end9(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9) \
err:\
	if (argid) {\
		std::string serr = (std::string)"err[" + GetTypeLua(L, arg1) + "," + GetTypeLua(L, arg2) + "," + GetTypeLua(L, arg3) + "," + GetTypeLua(L, arg4) + "," + GetTypeLua(L, arg5) + "," + GetTypeLua(L, arg6) + "," + GetTypeLua(L, arg7) + "," + GetTypeLua(L, arg8) + "," + GetTypeLua(L, arg9) + "] ";\
		serr = serr + "ok[" + GetTypeCpp(TypeWrapper<P##arg1>()) + "," + GetTypeCpp(TypeWrapper<P##arg2>()) + "," + GetTypeCpp(TypeWrapper<P##arg3>()) + "," + GetTypeCpp(TypeWrapper<P##arg4>()) + "," + GetTypeCpp(TypeWrapper<P##arg5>()) + "," + GetTypeCpp(TypeWrapper<P##arg6>()) + "," + GetTypeCpp(TypeWrapper<P##arg7>()) + "," + GetTypeCpp(TypeWrapper<P##arg8>()) + "," + GetTypeCpp(TypeWrapper<P##arg9>()) + "]";\
		luaL_argerror(L, argid, serr.c_str());\
	}

inline void Get(TypeWrapper<void>, lua_State*, i32) {
}
inline bool Get(TypeWrapper<bool>, lua_State* L, i32 idx) {
	return lua_toboolean(L, idx) != 0;
}
inline i8 Get(TypeWrapper<i8>, lua_State* L, i32 idx) {
	return static_cast<i8>(lua_tonumber(L, idx));
}
inline ui8 Get(TypeWrapper<ui8>, lua_State* L, i32 idx) {
	return static_cast<ui8>(lua_tonumber(L, idx));
}
inline i16 Get(TypeWrapper<i16>, lua_State* L, i32 idx) {
	return static_cast<i16>(lua_tonumber(L, idx));
}
inline ui16 Get(TypeWrapper<ui16>, lua_State* L, i32 idx) {
	return static_cast<ui16>(lua_tonumber(L, idx));
}
inline i32 Get(TypeWrapper<i32>, lua_State* L, i32 idx) {
	return static_cast<i32>(lua_tonumber(L, idx));
}
inline ui32 Get(TypeWrapper<ui32>, lua_State* L, i32 idx) {
	return static_cast<ui32>(lua_tonumber(L, idx));
}
inline l32 Get(TypeWrapper<l32>, lua_State* L, i32 idx) {
	return static_cast<l32>(lua_tonumber(L, idx));
}
inline ul32 Get(TypeWrapper<ul32>, lua_State* L, i32 idx) {
	return static_cast<ul32>(lua_tonumber(L, idx));
}
inline f32 Get(TypeWrapper<f32>, lua_State* L, i32 idx) {
	return static_cast<f32>(lua_tonumber(L, idx));
}
inline f64 Get(TypeWrapper<f64>, lua_State* L, i32 idx) {
	return static_cast<f64>(lua_tonumber(L, idx));
}
inline cpi8 Get(TypeWrapper<cpi8>, lua_State* L, i32 idx) {
	return static_cast<cpi8>(lua_tostring(L, idx));
}
inline std::string Get(TypeWrapper<std::string>, lua_State* L, i32 idx) {
	size_t len = 0;
	cpi8 s = static_cast<cpi8>(lua_tolstring(L, idx, &len));
	if (s && len)
		return std::string(s, len);
	return "";
}

template<class RT>
struct ReturnSpecialization
{
	static inline i32 Call(RT (*func)(), lua_State* L, i32 /*index*/)
	{
		RT ret = func();
		Push(L, ret);
		return 1;
	}
	template <typename P1>
	static inline i32 Call(RT (*func)(P1), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert_end1(1);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0)
		);
		Push(L, ret);
		return 1;
	}
	template <typename P1, typename P2>
	static inline i32 Call(RT (*func)(P1, P2), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert_end2(1,2);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1)
		);
		Push(L, ret);
		return 1;
	}
	template <typename P1, typename P2, typename P3>
	static inline i32 Call(RT (*func)(P1, P2, P3), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert_end3(1,2,3);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2)
		);
		Push(L, ret);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4>
	static inline i32 Call(RT (*func)(P1, P2, P3, P4), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert_end4(1,2,3,4);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3)
		);
		Push(L, ret);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static inline i32 Call(RT (*func)(P1, P2, P3, P4, P5), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert_end5(1,2,3,4,5);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4)
		);
		Push(L, ret);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static inline i32 Call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert(6, index + 5);
		luaL_argassert_end6(1,2,3,4,5,6);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4),
			Get(TypeWrapper<P6>(), L, index + 5)
		);
		Push(L, ret);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static inline i32 Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert(6, index + 5);
		luaL_argassert(7, index + 6);
		luaL_argassert_end7(1,2,3,4,5,6,7);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4),
			Get(TypeWrapper<P6>(), L, index + 5),
			Get(TypeWrapper<P7>(), L, index + 6)
		);
		Push(L, ret);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static inline i32 Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert(6, index + 5);
		luaL_argassert(7, index + 6);
		luaL_argassert(8, index + 7);
		luaL_argassert_end8(1,2,3,4,5,6,7,8);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4),
			Get(TypeWrapper<P6>(), L, index + 5),
			Get(TypeWrapper<P7>(), L, index + 6),
			Get(TypeWrapper<P8>(), L, index + 7)
		);
		Push(L, ret);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	static inline i32 Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert(6, index + 5);
		luaL_argassert(7, index + 6);
		luaL_argassert(8, index + 7);
		luaL_argassert(9, index + 8);
		luaL_argassert_end9(1,2,3,4,5,6,7,8,9);
		RT ret = func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4),
			Get(TypeWrapper<P6>(), L, index + 5),
			Get(TypeWrapper<P7>(), L, index + 6),
			Get(TypeWrapper<P8>(), L, index + 7),
			Get(TypeWrapper<P9>(), L, index + 8)
		);
		Push(L, ret);
		return 1;
	}
};

template<>
struct ReturnSpecialization<void>
{
	static inline i32 Call(void (*func)(), lua_State* L, i32 /*index*/)
	{
		func();
		return 1;
	}
	template <typename P1>
	static inline i32 Call(void (*func)(P1), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert_end1(1);
		func(
			Get(TypeWrapper<P1>(), L, index + 0)
		);
		return 1;
	}
	template <typename P1, typename P2>
	static inline i32 Call(void (*func)(P1, P2), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert_end2(1,2);
		func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1)
		);
		return 1;
	}
	template <typename P1, typename P2, typename P3>
	static inline i32 Call(void (*func)(P1, P2, P3), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert_end3(1,2,3);
		func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2)
		);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4>
	static inline i32 Call(void (*func)(P1, P2, P3, P4), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert_end4(1,2,3,4);
		func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3)
		);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static inline i32 Call(void (*func)(P1, P2, P3, P4, P5), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert_end5(1,2,3,4,5);
		func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4)
		);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static inline i32 Call(void (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert(6, index + 5);
		luaL_argassert_end6(1,2,3,4,5,6);
		func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4),
			Get(TypeWrapper<P6>(), L, index + 5)
		);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static inline i32 Call(void (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert(6, index + 5);
		luaL_argassert(7, index + 6);
		luaL_argassert_end7(1,2,3,4,5,6,7);
		func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4),
			Get(TypeWrapper<P6>(), L, index + 5),
			Get(TypeWrapper<P7>(), L, index + 6)
		);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static inline i32 Call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert(6, index + 5);
		luaL_argassert(7, index + 6);
		luaL_argassert(8, index + 7);
		luaL_argassert_end8(1,2,3,4,5,6,7,8);
		func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4),
			Get(TypeWrapper<P6>(), L, index + 5),
			Get(TypeWrapper<P7>(), L, index + 6),
			Get(TypeWrapper<P8>(), L, index + 7)
		);
		return 1;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	static inline i32 Call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, i32 index)
	{
		luaL_argassert_begin;
		luaL_argassert(1, index + 0);
		luaL_argassert(2, index + 1);
		luaL_argassert(3, index + 2);
		luaL_argassert(4, index + 3);
		luaL_argassert(5, index + 4);
		luaL_argassert(6, index + 5);
		luaL_argassert(7, index + 6);
		luaL_argassert(8, index + 7);
		luaL_argassert(9, index + 8);
		luaL_argassert_end9(1,2,3,4,5,6,7,8,9);
		func(
			Get(TypeWrapper<P1>(), L, index + 0),
			Get(TypeWrapper<P2>(), L, index + 1),
			Get(TypeWrapper<P3>(), L, index + 2),
			Get(TypeWrapper<P4>(), L, index + 3),
			Get(TypeWrapper<P5>(), L, index + 4),
			Get(TypeWrapper<P6>(), L, index + 5),
			Get(TypeWrapper<P7>(), L, index + 6),
			Get(TypeWrapper<P8>(), L, index + 7),
			Get(TypeWrapper<P9>(), L, index + 8)
		);
		return 1;
	}
};

template <typename RT>
inline i32 Call(RT (*func)(), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1>
inline i32 Call(RT (*func)(P1), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1, typename P2>
inline i32 Call(RT (*func)(P1, P2), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1, typename P2, typename P3>
inline i32 Call(RT (*func)(P1, P2, P3), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1, typename P2, typename P3, typename P4>
inline i32 Call(RT (*func)(P1, P2, P3, P4), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5>
inline i32 Call(RT (*func)(P1, P2, P3, P4, P5), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline i32 Call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline i32 Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline i32 Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}
template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
inline i32 Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, i32 index) {
	return ReturnSpecialization<RT>::Call(func, L, index);
}

template<typename Func>
class cLuaDispatch
{
public:
	static inline i32 Dispatch(lua_State* L) {
		pvd buffer = lua_touserdata(L, lua_upvalueindex(1));
		return Call(*(Func*)(buffer), L, 1);
	}
};

template<typename Func>
inline void RegAFunc(lua_State* ls, cpi8 sFuncName, Func pFunc) {
	pvd buffer = lua_newuserdata(ls, sizeof(pFunc));
	cmemcpy(buffer, &pFunc, sizeof(pFunc));
	lua_pushcclosure(ls, cLuaDispatch<Func>::Dispatch, 1);
	lua_setglobal(ls, sFuncName);
}

inline bool HaveFunc(lua_State* ls, cpi8 sFuncName) {
	if (sFuncName == 0)
		return false;
	lua_getglobal(ls, sFuncName);
	bool b = lua_isfunction(ls, -1);
	lua_pop(ls, 1);
	return b;
}
inline bool GetFunc(lua_State* ls, cpi8 sFuncName) {
	if (sFuncName == 0)
		return false;
	lua_getglobal(ls, sFuncName);
	return true;
}
inline bool CallReturn(lua_State* ls, i32 error, cpi8 sinfo, cpi8 sdo) {
	if (error) {
		std::string msg = sdo;
		msg += sinfo ? sinfo : "";
		msg += "===>";
		msg += lua_tostring(ls, -1);
		lua_pop(ls, 1);
		LogError(c_sLSE) << msg << LEnd;
		return false;
	}
	return true;
}
inline bool PCallRTVoid(lua_State* ls, cpi8 sFuncName, i32 nArgs) {
	return CallReturn(ls, lua_pcall(ls, nArgs, 0, 0), sFuncName, "error call function:");
}
template<typename RT>
inline bool PCall(lua_State* ls, cpi8 sFuncName, RT& rt, i32 nArgs) {
	if (CallReturn(ls, lua_pcall(ls, nArgs, 1, 0), sFuncName, "error call function:")) {
		rt = Get(TypeWrapper<RT>(), ls, -1);
		lua_pop(ls, 1);
		return true;
	}
	return false;
}
template<typename RT>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	return PCall(ls, sFuncName, rt, 0);
	LuaCatch
}
template<typename RT, typename P1>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	return PCall(ls, sFuncName, rt, 1);
	LuaCatch
}
template<typename RT, typename P1, typename P2>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1, P2 p2) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	return PCall(ls, sFuncName, rt, 2);
	LuaCatch
}
template<typename RT, typename P1, typename P2, typename P3>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1, P2 p2, P3 p3) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	return PCall(ls, sFuncName, rt, 3);
	LuaCatch
}
template<typename RT, typename P1, typename P2, typename P3, typename P4>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1, P2 p2, P3 p3, P4 p4) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	return PCall(ls, sFuncName, rt, 4);
	LuaCatch
}
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	return PCall(ls, sFuncName, rt, 5);
	LuaCatch
}
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	Push(ls, p6);
	return PCall(ls, sFuncName, rt, 6);
	LuaCatch
}
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	Push(ls, p6);
	Push(ls, p7);
	return PCall(ls, sFuncName, rt, 7);
	LuaCatch
}
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	Push(ls, p6);
	Push(ls, p7);
	Push(ls, p8);
	return PCall(ls, sFuncName, rt, 8);
	LuaCatch
}
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
inline bool CallFunc(lua_State* ls, cpi8 sFuncName, RT& rt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	Push(ls, p6);
	Push(ls, p7);
	Push(ls, p8);
	Push(ls, p9);
	return PCall(ls, sFuncName, rt, 9);
	LuaCatch
}
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	return PCallRTVoid(ls, sFuncName, 0);
	LuaCatch
}
template<typename P1>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	PCallRTVoid(ls, sFuncName, 1);
	LuaCatch
}
template<typename P1, typename P2>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1, P2 p2) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	return PCallRTVoid(ls, sFuncName, 2);
	LuaCatch
}
template<typename P1, typename P2, typename P3>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1, P2 p2, P3 p3) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	return PCallRTVoid(ls, sFuncName, 3);
	LuaCatch
}
template<typename P1, typename P2, typename P3, typename P4>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1, P2 p2, P3 p3, P4 p4) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	return PCallRTVoid(ls, sFuncName, 4);
	LuaCatch
}
template<typename P1, typename P2, typename P3, typename P4, typename P5>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	return PCallRTVoid(ls, sFuncName, 5);
	LuaCatch
}
template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	Push(ls, p6);
	return PCallRTVoid(ls, sFuncName, 6);
	LuaCatch
}
template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	Push(ls, p6);
	Push(ls, p7);
	return PCallRTVoid(ls, sFuncName, 7);
	LuaCatch
}
template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	Push(ls, p6);
	Push(ls, p7);
	Push(ls, p8);
	return PCallRTVoid(ls, sFuncName, 8);
	LuaCatch
}
template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
inline bool CallFuncRTVoid(lua_State* ls, cpi8 sFuncName, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) {
	LuaTry
	if (!GetFunc(ls, sFuncName))
		return false;
	Push(ls, p1);
	Push(ls, p2);
	Push(ls, p3);
	Push(ls, p4);
	Push(ls, p5);
	Push(ls, p6);
	Push(ls, p7);
	Push(ls, p8);
	Push(ls, p9);
	return PCallRTVoid(ls, sFuncName, 9);
	LuaCatch
}
inline bool DoString(lua_State* ls, cpi8 s) {
	LuaTry
	if (s == 0)
		return false;
	return CallReturn(ls, luaL_dostring(ls, s), "", "error do std::string:");
	LuaCatch
}
inline bool DoFile(lua_State* ls, cpstr sFile) {
	LuaTry
	return CallReturn(ls, luaL_loadfile(ls, sFile) || lua_pcall(ls, 0, 0, 0), sFile, "error do file:");
	LuaCatch
}
inline bool DoFile(lua_State* ls, cpvd pBuf, ui32 bufSize) {
	LuaTry
	return CallReturn(ls, luaL_loadbuffer(ls, (cpstr)pBuf, bufSize, 0) || lua_pcall(ls, 0, 0, 0), "", "error do file from memory:");
	LuaCatch
}

inline lua_State* LuaInit() {
	lua_State* ls = luaL_newstate();
	luaL_openlibs(ls);
	return ls;
}

inline void LuaRelease(lua_State* ls) {
	lua_close(ls);
}

inline void LuaAddLoader(lua_State* ls, lua_CFunction func)
{
	// stack content after the invoking of the function
	// get loader table
	lua_getglobal(ls, "package");                                  /* L: package */
	lua_getfield(ls, -1, "loaders");                               /* L: package, loaders */

	// insert loader into index 2
	lua_pushcfunction(ls, func);                                   /* L: package, loaders, func */
	for (int i = (int)(lua_objlen(ls, -2) + 1); i > 2; --i)
	{
		lua_rawgeti(ls, -2, i - 1);                                /* L: package, loaders, func, function */
		// we call lua_rawgeti, so the loader table now is at -3
		lua_rawseti(ls, -3, i);                                    /* L: package, loaders, func */
	}
	lua_rawseti(ls, -2, 2);                                        /* L: package, loaders */

	// set loaders into package
	lua_setfield(ls, -2, "loaders");                               /* L: package */

	lua_pop(ls, 1);
}

static cpstr c_strLuaRefidFunction = "LuaHelperRefidFunctionMapping";

inline void LuaInitParamFunction(lua_State* L)
{
	lua_pushstring(L, c_strLuaRefidFunction);
	lua_newtable(L);
	lua_rawset(L, LUA_REGISTRYINDEX);
}

inline i32 LuaRefidFunction(lua_State* L, i32 lo)
{
	// function at lo
	if (!lua_isfunction(L, lo))
		return 0;

	static i32 s_function_ref_id = 0;
	s_function_ref_id++;

	lua_pushstring(L, c_strLuaRefidFunction);
	lua_rawget(L, LUA_REGISTRYINDEX);                           /* stack: fun ... refid_fun */
	lua_pushinteger(L, s_function_ref_id);                      /* stack: fun ... refid_fun refid */
	lua_pushvalue(L, lo);                                       /* stack: fun ... refid_fun refid fun */

	lua_rawset(L, -3);                  /* refid_fun[refid] = fun, stack: fun ... refid_ptr */
	lua_pop(L, 1);                                              /* stack: fun ... */

	return s_function_ref_id;
}

inline void LuaGetFunctionByRefid(lua_State* L, i32 refid)
{
	lua_pushstring(L, c_strLuaRefidFunction);
	lua_rawget(L, LUA_REGISTRYINDEX);                           /* stack: ... refid_fun */
	lua_pushinteger(L, refid);                                  /* stack: ... refid_fun refid */
	lua_rawget(L, -2);                                          /* stack: ... refid_fun fun */
	lua_remove(L, -2);                                          /* stack: ... fun */
}

inline void LuaRemoveFunctionByRefid(lua_State* L, i32 refid)
{
	lua_pushstring(L, c_strLuaRefidFunction);
	lua_rawget(L, LUA_REGISTRYINDEX);                           /* stack: ... refid_fun */
	lua_pushinteger(L, refid);                                  /* stack: ... refid_fun refid */
	lua_pushnil(L);                                             /* stack: ... refid_fun refid nil */
	lua_rawset(L, -3);                  /* refid_fun[refid] = fun, stack: ... refid_ptr */
	lua_pop(L, 1);                                              /* stack: ... */
}

inline void ExecFunctionByRefid(lua_State* L, i32 refid, i32 numArgs = 0, i32 numResults = 0)
{
	LuaGetFunctionByRefid(L, refid);
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	if (numArgs > 0)
		lua_insert(L, -(numArgs + 1));
	i32 functionIndex = -(numArgs + 1);
	if (!lua_isfunction(L, functionIndex)) {
		lua_pop(L, numArgs + 1);
		return;
	}
	i32 error = lua_pcall(L, numArgs, numResults, 0);
	if (error) {
		LogError(c_sLSE) << "lua error " << lua_tostring(L, -1) << LEnd;
		lua_pop(L, 1);
		return;
	}
}

// check lua value is funciton
inline b8 LuaIsFunction(lua_State* L, i32 lo) {
	return lua_gettop(L) >= lo && lua_isfunction(L, lo);
}

inline std::string LuaToCppString(lua_State* L, i32 lo)
{
	size_t len = 0;
	std::string ret;
	cpstr s1 = ((cpstr)lua_tolstring(L, lo, &len));
	if (s1 && len) {
		ret.resize(len);
		memcpy((i8*)ret.data(), s1, len);
	}
	return ret;
}

inline void LuaPushCppString(lua_State* L, const std::string& s)
{
	lua_pushlstring(L, s.c_str(), s.size());
}
inline void LuaModule(lua_State* L, cpstr name)
{
	if (name) {
		lua_pushstring(L,name);
		lua_rawget(L,-2);
		if (!lua_istable(L,-1)) {
			lua_pop(L,1);
			lua_newtable(L);
			lua_pushstring(L,name);
			lua_pushvalue(L,-2);
			lua_rawset(L,-4);
		}
	} else {
		lua_pushvalue(L,LUA_GLOBALSINDEX);
	}
	lua_pop(L,1);
}
inline void LuaBeginModule(lua_State* L, cpstr name)
{
	if (name) {
		lua_pushstring(L,name);
		lua_rawget(L,-2);
	} else
		lua_pushvalue(L,LUA_GLOBALSINDEX);
}
inline void LuaEndModule(lua_State* L)
{
    lua_pop(L,1);
}
inline void LuaConstant(lua_State* L, cpstr name, i32 v)
{
	lua_pushstring(L, name);
	lua_pushnumber(L, v);
	lua_rawset(L, -3);
}

}

#define REGFUNC(s,f) cross::RegAFunc(L,s,f);
#define REGFUNC0(f) cross::RegAFunc(L,#f,f);
#define REGCFUNC(s,f) lua_register(L, s, f);
#define REGCFUNC0(f) lua_register(L, #f, f);

#define PushStructParamBool(cobj, name) \
	lua_pushstring(L, #name);\
	lua_pushboolean(L, cobj.name);\
	lua_rawset(L, -3);

#define PushStructParamNum(cobj, name) \
	lua_pushstring(L, #name);\
	lua_pushnumber(L, (lua_Number)cobj.name);\
	lua_rawset(L, -3);

#define PushStructParamStdString(cobj, name) \
	lua_pushstring(L, #name);\
	lua_pushstring(L, cobj.name.c_str());\
	lua_rawset(L, -3);

#define GetStructParamBool(cobj, name) \
	lua_pushstring(L, #name);\
	lua_gettable(L, lo);\
	cobj.name = lua_isnil(L, -1) ? false : lua_toboolean(L, -1);\
	lua_pop(L, 1);

#define GetStructParamNum(cobj, name) \
	lua_pushstring(L, #name);\
	lua_gettable(L, lo);\
	cobj.name = lua_isnil(L, -1) ? false : lua_tonumber(L, -1);\
	lua_pop(L, 1);

#define GetStructParamStdString(cobj, name) \
	lua_pushstring(L, #name);\
	lua_gettable(L, lo);\
	cpstr s = lua_isnil(L, -1) ? "" : lua_tostring(L, -1);\
	cobj.name = s == nullptr ? "" : s;\
	lua_pop(L, 1);

#endif
