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


LuaBinding::LuaBinding(){
    this->enabled = 55;
}

LuaBinding::LuaBinding(const LuaBinding& other){
    this->luaState = other.luaState;
    this->enabled = other.enabled;
}

LuaBinding& LuaBinding::operator=(const LuaBinding& other){
    this->luaState = other.luaState;
    this->enabled = other.enabled;
    return *this;
}

LuaBinding::~LuaBinding(){
}



void LuaBinding::init() {
    lua_State *luaState = luaL_newstate();
    this->luaState = luaState;
	luaL_openlibs(luaState); // bad

    lua_pushcfunction(luaState, l_sin);
	lua_setglobal(luaState, "RegisterEvent");
}

void LuaBinding::enable() {
    this->enabled = 1337;
}

void LuaBinding::get_str() {

    //std::string s = std::to_string(this->enabled);


	if (this->enabled == 1337) {
        luaL_dostring(luaState, "RegisterEvent('Hello LUA');"); // bad
        //std::string s = std::to_string(this->enabled);
    }
    else {
    }

    //strncpy(this->buf, "2", 16);
}

char * LuaBinding::get_buf() {
    return s_str;
}

void LuaBinding::shutdown() {
    lua_close(this->luaState);
}
