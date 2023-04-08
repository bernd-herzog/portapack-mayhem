#pragma once

#include "tiny_memory_elua/lua.hpp"
#include "lua_binding/lua_wrapper.hpp"

namespace lua {

class LuaState {
public:	
	LuaState();

	void init();
	void shutdown();
	void execute_lua_script(const TCHAR *path);
	char * get_buf();
	inline lua_State *get_state() {return luaState;}

private:
	lua_State *luaState = nullptr;
	char buf[16];

};

extern LuaState lua_state;

}