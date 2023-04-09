#include "lua_state.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <string.h>
#include <string>
#include "ch.h"

#include "portapack.hpp"
#include "ui_lua_system_view.hpp"
#include "portapack_shared_memory.hpp"

namespace lua {

LuaState lua_state;
MakeWrapper(LuaErrorWrapper, (lua_State *L), (L), LuaState, int);

LuaState::LuaState() :
    on_lua_error([](std::string, int){}),
    luaState(luaL_newstate()),
    luaError("") {
}

void LuaState::init() {
	luaL_openlibs(get_state());
}

void LuaState::execute_lua_script(const TCHAR *path) {
    // FIL lua_file;
    // if (f_open(&lua_file, reinterpret_cast<const TCHAR*>(path), FA_READ) != FR_OK) {
    //      chDbgPanic("file not found");
    // }

    // size_t bytes_read;
    // //TODO: fix file reading
    // uint8_t *data_buffer = &shared_memory.bb_data.data[0]; // TODO: dont use shared memory for everything

    // if (f_read(&lua_file, data_buffer, 512, &bytes_read) != FR_OK) chDbgPanic("no data");
    // data_buffer[bytes_read] = 0;

    // f_close(&lua_file);

 	lua_atpanic(get_state(), GetWrapper(LuaErrorWrapper, &LuaState::lua_at_panic));

    int r = setjmp(jumpBuffer);
    if (r != 0) {
        this->on_lua_error(luaError, luaErrorLine);
    }
    else {
        luaL_loadfile2(get_state(), reinterpret_cast<const TCHAR*>(path));
//        luaL_loadstring(get_state(), reinterpret_cast<const char*>(data_buffer));
        lua_call(get_state(), 0, LUA_MULTRET);
    }
}

void LuaState::execute_lua_function(int ref_id) {
    int r = setjmp(jumpBuffer);
    if (r != 0) {
        this->on_lua_error(luaError, luaErrorLine);
    }
    else {
        lua_rawgeti(get_state(), LUA_REGISTRYINDEX, ref_id);
        lua_call(get_state(), 0, 0);
   }
}

void LuaState::shutdown() {
    lua_close(get_state());
}

LUA_FUNCTION int LuaState::lua_at_panic(lua_State *L) {
	luaError = lua_tostring(L, -1);

    // lua_Debug ar;
    // lua_getstack(L, 1, &ar);
    // lua_getinfo(L, "nSl", &ar);
    luaErrorLine = 0; //ar.currentline;

	lua_pop(L, 1);
	longjmp(jumpBuffer, 1);
	return 1;
}

}