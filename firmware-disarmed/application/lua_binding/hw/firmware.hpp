#pragma once

#include <functional>

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"

#include "lua_binding/lua_binding.hpp"

namespace lua_hw {
class Firmware {
    public:
        static void initialize_lua_binding(lua_State *L, ui::NavigationView& nav);

        Firmware();

        LUA_FUNCTION int lua_run_sd_over_usb(lua_State *L);
        LUA_FUNCTION int lua_run_flash_utility(lua_State *L);
        LUA_FUNCTION int lua_run_hack_rf_mode(lua_State *L);
        LUA_FUNCTION int lua_get_free_heap(lua_State *L);
        LUA_FUNCTION int lua_get_free_stack(lua_State *L);
        LUA_FUNCTION int lua_take_screenshot(lua_State *L);
        LUA_FUNCTION int lua_run_app(lua_State *L);

        std::function<void(void)> run_flash_utility = [](){};

    private:

};
}