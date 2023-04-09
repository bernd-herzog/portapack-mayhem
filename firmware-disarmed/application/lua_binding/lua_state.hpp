#pragma once

#include <setjmp.h>
#include <functional>
#include <memory>

#include "tiny_memory_elua/lua.hpp"
#include "lua_binding/lua_wrapper.hpp"
#include "lua_binding/function_helper.hpp"

namespace lua {

class LuaState {
public:	
	LuaState();
	~LuaState()=default;
    LuaState(const LuaState &L)=default;             // copy constructor
    LuaState & operator=(const LuaState &L)=default; // assignment

	void init();
	void shutdown();
	void execute_lua_script(const TCHAR *path);
	void execute_lua_function(int ref_id);
	inline lua_State *get_state() {return luaState;}

	std::function<void (std::string, int)> on_lua_error;

private:
	lua_State *luaState;

	std::string luaError;
	int luaErrorLine = 0;

	jmp_buf jumpBuffer;
	LUA_FUNCTION int lua_at_panic(lua_State *L);
};

extern LuaState lua_state;

}