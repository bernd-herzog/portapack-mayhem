#pragma once

#include "lua/lua.hpp"

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


