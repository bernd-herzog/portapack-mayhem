#pragma once

#include "lua/lua.hpp"

class LuaBinding {
public:	
	LuaBinding();
	~LuaBinding();

	LuaBinding(const LuaBinding&);
	LuaBinding& operator=(const LuaBinding&);

	void init();
	void shutdown();
	void get_str();
	char * get_buf();
	void enable();

private:
	lua_State *luaState = nullptr;
	char buf[16];
	int enabled;
};


