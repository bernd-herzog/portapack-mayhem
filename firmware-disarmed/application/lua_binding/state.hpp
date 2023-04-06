#pragma once

#include "tiny_memory_elua/lua.hpp"

namespace lua {

class LuaBinding {
public:	
	LuaBinding();

	void init();
	void shutdown();
	void get_str();
	char * get_buf();

private:
	lua_State *luaState = nullptr;
	char buf[16];
};

extern LuaBinding lua_binding;

}