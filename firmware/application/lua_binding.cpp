#include "lua_binding.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/stat.h>

extern "C" {
    // int _gettimeofday(struct timeval *tv, void *tzvp) { return gettimeofday(tv, tzvp); }
    // int _unlink(const char *pathname) { return unlink(pathname); }
    // pid_t _getpid() { return getpid(); }
    // int _link(const char *oldpath, const char *newpath)  { return link(oldpath, newpath); }
    // void _exit(int status) { }
    // clock_t _times(struct tms *buf) { return times(buf); }
    // int _open(const char *pathname, int flags) { return open(pathname, flags); }
    //int _open(const char *pathname, int flags, mode_t mode) { return open(pathname, flags, mode); }

    extern void test_heap(char *a);
}

//extern char *debug_messages[2];

void LuaBinding::init() {
    //debug_messages[0] = "init";
    //debug_messages[1] = "f1";
    lua_State *luaState = luaL_newstate();
    test_heap("init1");
    //debug_messages[1] = "f2";
    this->luaState = luaState;
    //debug_messages[1] = "f3";
	//luaL_openlibs(luaState);
    test_heap("init2");
    //debug_messages[1] = "f4";
}

void LuaBinding::shutdown() {
    lua_close(this->luaState);
}
