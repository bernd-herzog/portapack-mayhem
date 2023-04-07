#include "state.hpp"

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


namespace lua {

LuaBinding lua_binding;

int l_sin (lua_State *L) {
    char *text = const_cast<char *>(luaL_checkstring(L, 1));
    strncpy(lua_binding.get_buf(), text, 16);
    return 0;
}

LuaBinding::LuaBinding() {
}

void LuaBinding::init() {
    this->luaState = luaL_newstate();
	luaL_openlibs(luaState);

    lua_pushcfunction(luaState, l_sin);
	lua_setglobal(luaState, "RegisterEvent");
}

void LuaBinding::execute_lua_script(const TCHAR *path) {
    luaL_dostring(luaState, "RegisterEvent('Hello LUA');");
    luaL_dofile2(luaState, path);
}

char * LuaBinding::get_buf() {
    return this->buf;
}

void LuaBinding::shutdown() {
    lua_close(this->luaState);
}

}