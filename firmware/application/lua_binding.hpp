#pragma once

#include "lua/lua.hpp"

class LuaBinding {
public:	
	void init();
	void shutdown();

private:
	lua_State *luaState = nullptr;
};