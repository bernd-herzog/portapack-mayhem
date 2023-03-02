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

char s_str[16];


//extern "C" {
    // int _gettimeofday(struct timeval *tv, void *tzvp) { return gettimeofday(tv, tzvp); }
    // int _unlink(const char *pathname) { return unlink(pathname); }
    // pid_t _getpid() { return getpid(); }
    // int _link(const char *oldpath, const char *newpath)  { return link(oldpath, newpath); }
    // void _exit(int status) { }
    // clock_t _times(struct tms *buf) { return times(buf); }
    // int _open(const char *pathname, int flags) { return open(pathname, flags); }
    //int _open(const char *pathname, int flags, mode_t mode) { return open(pathname, flags, mode); }

    //extern void test_heap(char *a);

    int l_sin (lua_State *L) {
        char *text = const_cast<char *>(luaL_checkstring(L, 1));
        strncpy(s_str, text, 16);
        return 0; 
    }
//}

extern char debug_messages[3][16];

LuaBinding::LuaBinding(){
    this->enabled = 55;
    //std::string s = std::to_string((int) this);
    //strncpy(debug_messages[0], s.c_str(), 16);
}

LuaBinding::~LuaBinding(){
    //strncpy(debug_messages[2], "dtor", 16);
}



void LuaBinding::init() {
    lua_State *luaState = luaL_newstate();
    this->luaState = luaState;
	//luaL_openlibs(luaState); // bad

    lua_pushcfunction(luaState, l_sin);
	lua_setglobal(luaState, "RegisterEvent");
}

void LuaBinding::enable() {
    this->enabled = 1337;
    //strncpy(debug_messages[0], "enable", 16);
}

void LuaBinding::get_str() {

    std::string s = std::to_string(this->enabled);
    strncpy(debug_messages[1], s.c_str(), 16);


	if (this->enabled == 1337) {
        strncpy(debug_messages[2], "enabled", 16);
        //luaL_dostring(luaState, "RegisterEvent('Hello LUA');"); // bad
        std::string s = std::to_string(this->enabled);
        strncpy(debug_messages[1], s.c_str(), 16);
    }
    else {
        strncpy(debug_messages[2], "disabled", 16);
    }

    //strncpy(this->buf, "2", 16);
}

char * LuaBinding::get_buf() {
    return s_str;
}

void LuaBinding::shutdown() {
    lua_close(this->luaState);
}
