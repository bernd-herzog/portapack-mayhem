#include "lua_binding.hpp"

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

int l_sin (lua_State *L) {
    char *text = const_cast<char *>(luaL_checkstring(L, 1));
    strncpy(portapack::lua_binding.get_buf(), text, 16);
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

void LuaBinding::get_str() {
    luaL_dostring(luaState, "RegisterEvent('Hello LUA');");
}

char * LuaBinding::get_buf() {
    return this->buf;
}

void LuaBinding::shutdown() {
    lua_close(this->luaState);
}
