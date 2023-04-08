#pragma once
#include <string>

#include "tiny_memory_elua/lua.hpp"
#include "lua_binding/function_helper.hpp"

namespace lua {

template<class nestedClassType>
class LuaBinding
{
	static std::vector<FunctionHelper *> someVector;
	static std::string className;
	static std::function<void (nestedClassType *)> callback;

public:
	template<int(nestedClassType::*SomeMember)(lua_State *L)>
	static void registerFunc(std::string methodName);

	template <int (nestedClassType::*TMethod)(lua_State *L)>
	static int LUA_FUNCTION instance_finder(lua_State *L);

	static void Init(std::string className, std::function<void (nestedClassType *)> callback);
	static void CreateLuaMetaClass(lua_State *L, std::string createFunctionName);
	static int LUA_FUNCTION index(lua_State *L);
	static int LUA_FUNCTION index_indirect(lua_State *L);
	static int LUA_FUNCTION CreateClass(lua_State *L);
};

template<class nestedClassType>
/* static */ std::vector<FunctionHelper *> LuaBinding<nestedClassType>::someVector;

template<class nestedClassType>
/* static */ std::string LuaBinding<nestedClassType>::className;

template<class nestedClassType>
std::function<void (nestedClassType *)> LuaBinding<nestedClassType>::callback;

template<class nestedClassType>
/* static */ void LuaBinding<nestedClassType>::Init(std::string className, std::function<void (nestedClassType *)> callback) // should be called only once
{
	LuaBinding<nestedClassType>::className = className;
	LuaBinding<nestedClassType>::callback = callback;
}

template<class nestedClassType>
/* static */ void LuaBinding<nestedClassType>::CreateLuaMetaClass(lua_State *L, std::string createFunctionName) // for every lua context
{
    //new metatable
	luaL_newmetatable(L, className.c_str());
	lua_pushcfunction(L, &LuaBinding<nestedClassType>::index);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);

	//register create function
	lua_pushcfunction(L, &LuaBinding<nestedClassType>::CreateClass);
	lua_setglobal(L, createFunctionName.c_str());
}

template<class nestedClassType>
template<int(nestedClassType::*SomeMember)(lua_State *L)>
/*static*/ void LuaBinding<nestedClassType>::registerFunc(std::string methodName) // should be called only once
{
	FunctionHelper *a = new FunctionHelper();
	a->luaFuncName = methodName;
	a->staticClassMemberFunc = &instance_finder<SomeMember>;
	someVector.push_back(a);
}

template<class nestedClassType>
/*static*/ int LUA_FUNCTION LuaBinding<nestedClassType>::CreateClass(lua_State *L)
{
	nestedClassType *button = new nestedClassType();
	//m_uiElements.push_back(button);

	nestedClassType **b = static_cast<nestedClassType **>(lua_newuserdata(L, sizeof(nestedClassType *)));

	*b = button;

	luaL_getmetatable(L, className.c_str());
	lua_setmetatable(L, -2);

	lua_pushvalue(L, -1);
    // TODO: impelment for events on click...
	int refId = luaL_ref(L, LUA_REGISTRYINDEX);
	(void)refId;
	// button->SetRefID(refId); // add dictionary refID to object

	callback(button);

	return 1;
}

template<class nestedClassType>
template <int (nestedClassType::*TMethod)(lua_State *L)>
/*static*/ int LUA_FUNCTION LuaBinding<nestedClassType>::instance_finder(lua_State *L) // selector
{
//	nestedClassType *obj = static_cast<nestedClassType *>(luaL_checkudata(L, -2, className.c_str())); 
	//int n = lua_gettop(L);
	nestedClassType **obj = static_cast<nestedClassType **>(lua_touserdata(L, 1)); 
	return (*obj->*TMethod)(L);
}

template<class nestedClassType>
/*static*/ int LUA_FUNCTION LuaBinding<nestedClassType>::index(lua_State *L)
{
	// -1: string "SetPosition" // 2nd parameter
	// -2: userdata UITextField // first parameter

	//nestedClassType *obj = static_cast<nestedClassType *>(luaL_checkudata(L, -2, className.c_str()));
	const char *func = luaL_checkstring(L, -1);

	for(auto a : someVector)
	{
		if (strcmp(func, a->luaFuncName.c_str()) == 0)
		{
			lua_pushcfunction(L, a->staticClassMemberFunc);
			return 1;
		}
	}

	return 0;
	//return UIElement::lua_Index(L);

	//TODO: vererbung
	// return LuaBinding<nestedClassType /*::Base*/>::index_indirect(L);
}

// template<class nestedClassType>
// /*static*/ int LUA_FUNCTION LuaBinding<nestedClassType>::index_indirect(lua_State *L)
// {
// 	// -1: string "SetPosition" // 2nd parameter
// 	// -2: userdata UITextField // first parameter
// 	//nestedClassType *obj = static_cast<nestedClassType *>(lua_touserdata(L, -2));
// 	const char *func = luaL_checkstring(L, -1);

// 	for(auto a : someVector)
// 	{
// 		if (strcmp(func, a->luaFuncName.c_str()) == 0)
// 		{
// 			lua_pushcfunction(L, a->staticClassMemberFunc);
// 			return 1;
// 		}
// 	}

// 	//return UIElement::lua_Index(L);

// 	//TODO: vererbung
// 	return 0;
// }






}