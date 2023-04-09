#pragma once

#include "button.hpp"
#include "ui_lua_view.hpp"
#include "lua_binding/lua_binding.hpp"

namespace lua_ui {
class Button : public ui::Button {
    public:
        static void initialize_lua_binding(lua_State *L, ui::LuaView *luaView);

        Button();

        LUA_FUNCTION int lua_set_text(lua_State *L);
        LUA_FUNCTION int lua_set_rect(lua_State *L);
        LUA_FUNCTION int lua_on_click(lua_State *L);

        int get_click_event_ref_id() { return click_event_ref_id; }

    private:
        int click_event_ref_id;
};
}