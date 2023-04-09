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
	static std::function<void (nestedClassType *)> object_creation_event;

public:
	template<int(nestedClassType::*SomeMember)(lua_State *L)>
	static void register_lua_function(std::string methodName);

	template <int (nestedClassType::*TMethod)(lua_State *L)>
	static int LUA_FUNCTION instance_finder(lua_State *L);

	static void initialize_object_creation(std::string className, std::function<void (nestedClassType *)> object_creation_event);
	static void regiser_object_creation_function(lua_State *L, std::string createFunctionName);
	static int LUA_FUNCTION index(lua_State *L);
	static int LUA_FUNCTION index_indirect(lua_State *L);
	static int LUA_FUNCTION CreateClass(lua_State *L);
};

template<class nestedClassType>
/* static */ std::vector<FunctionHelper *> LuaBinding<nestedClassType>::someVector;

template<class nestedClassType>
/* static */ std::string LuaBinding<nestedClassType>::className;

template<class nestedClassType>
std::function<void (nestedClassType *)> LuaBinding<nestedClassType>::object_creation_event;

template<class nestedClassType>
/* static */ void LuaBinding<nestedClassType>::initialize_object_creation(std::string className, std::function<void (nestedClassType *)> object_creation_event)
{
	LuaBinding<nestedClassType>::className = className;
	LuaBinding<nestedClassType>::object_creation_event = object_creation_event;
}

template<class nestedClassType>
/* static */ void LuaBinding<nestedClassType>::regiser_object_creation_function(lua_State *L, std::string createFunctionName)
{
	luaL_newmetatable(L, className.c_str());
	lua_pushcfunction(L, &LuaBinding<nestedClassType>::index);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);

	lua_pushcfunction(L, &LuaBinding<nestedClassType>::CreateClass);
	lua_setglobal(L, createFunctionName.c_str());
}

template<class nestedClassType>
template<int(nestedClassType::*SomeMember)(lua_State *L)>
/*static*/ void LuaBinding<nestedClassType>::register_lua_function(std::string methodName)
{
	FunctionHelper *a = new FunctionHelper();
	a->luaFuncName = methodName;
	a->staticClassMemberFunc = &instance_finder<SomeMember>;
	someVector.push_back(a);
}

template<class nestedClassType>
/*static*/ int LUA_FUNCTION LuaBinding<nestedClassType>::CreateClass(lua_State *L)
{
	nestedClassType *object_instance = new nestedClassType();
	nestedClassType **lua_user_data = static_cast<nestedClassType **>(lua_newuserdata(L, sizeof(nestedClassType *)));

	*lua_user_data = object_instance;

	luaL_getmetatable(L, className.c_str());
	lua_setmetatable(L, -2);
	lua_pushvalue(L, -1);
	object_creation_event(object_instance);

	return 1;
}

template<class nestedClassType>
template <int (nestedClassType::*TMethod)(lua_State *L)>
/*static*/ int LUA_FUNCTION LuaBinding<nestedClassType>::instance_finder(lua_State *L) // selector
{
	nestedClassType **obj = static_cast<nestedClassType **>(lua_touserdata(L, 1)); 
	return (*obj->*TMethod)(L);
}

template<class nestedClassType>
/*static*/ int LUA_FUNCTION LuaBinding<nestedClassType>::index(lua_State *L)
{
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
}

}