#pragma once

#include "button.hpp"
#include "ui_lua_view.hpp"

namespace lua_ui {
class Button : public ui::Button {
    public:
        static void initialize_luabinding(lua_State *L, ui::LuaView *luaView);

        Button();

        int lua_SetText(lua_State *L);
        int lua_SetRect(lua_State *L);
};
}