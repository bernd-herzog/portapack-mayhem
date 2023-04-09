#pragma once

#include "button.hpp"
#include "ui_lua_system_view.hpp"
#include "lua_binding/lua_binding.hpp"

namespace lua_ui {
class Label : public ui::Labels {
    public:
        static void initialize_lua_binding(lua_State *L, ui::View *parent);

        Label();

        LUA_FUNCTION int lua_set_text_and_position(lua_State *L);
};
}
