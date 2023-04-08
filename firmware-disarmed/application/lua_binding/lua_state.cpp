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
#include "ui_lua_view.hpp"
#include "portapack_shared_memory.hpp"

namespace lua {

LuaState lua_state;

int l_sin (lua_State *L) {
    char *text = const_cast<char *>(luaL_checkstring(L, 1));
    strncpy(lua_state.get_buf(), text, 16);
    return 0;
}

LuaState::LuaState() {
}

void LuaState::init() {
    this->luaState = luaL_newstate();
	luaL_openlibs(luaState);
}

void LuaState::execute_lua_script(const TCHAR *path) {
    FIL lua_file;
    if (f_open(&lua_file, reinterpret_cast<const TCHAR*>(path), FA_READ) != FR_OK) {
         chDbgPanic("file not found");
    }

    size_t bytes_read;
    uint8_t *data_buffer = &shared_memory.bb_data.data[0]; // TODO: dont use shared memory for everything

    if (f_read(&lua_file, data_buffer, 512, &bytes_read) != FR_OK) chDbgPanic("no data");
    data_buffer[bytes_read] = 0;

    f_close(&lua_file);

    luaL_loadstring(luaState, reinterpret_cast<const char*>(data_buffer));
    lua_call(luaState, 0, LUA_MULTRET);
}

char * LuaState::get_buf() {
    return this->buf;
}

void LuaState::shutdown() {
    lua_close(this->luaState);
}

}