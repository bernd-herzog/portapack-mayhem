#pragma once
#include <string>

#include "tiny_memory_elua/lua.hpp"

/* this function can be called from a LUA script */
#define LUA_FUNCTION

namespace lua {
class FunctionHelper
{
public:
	std::string luaFuncName;
	lua_CFunction staticClassMemberFunc;
};
}
