
#include "lua_binding/lua_state.hpp"
#include "button.hpp"
#include "ui_lua_app_view.hpp"

namespace lua_ui {

/* static */ void Button::initialize_lua_binding(lua_State *L, ui::LuaAppView *parent) {
	typedef lua::LuaBinding<lua_ui::Button> ButtonBinding;

    ButtonBinding::initialize_object_creation("Button", [parent](lua_ui::Button *created_button) {
        parent->add_lua_child(created_button);
    });

    ButtonBinding::regiser_object_creation_function(L, "CreateButton");
    ButtonBinding::register_lua_function<&lua_ui::Button::lua_set_text>("SetText");
    ButtonBinding::register_lua_function<&lua_ui::Button::lua_set_rect>("SetRect");
    ButtonBinding::register_lua_function<&lua_ui::Button::lua_on_click>("OnClick");
}

Button::Button() : ui::Button { } {
    this->on_select = [](ui::Button& button) {
        lua_ui::Button *b = (lua_ui::Button *)(&button);
        int click_ref_id = b->get_click_event_ref_id();

        if (click_ref_id != 0)
            lua::lua_state.execute_lua_function(click_ref_id);
    };
}

LUA_FUNCTION int Button::lua_set_text(lua_State *L) {
    auto text = luaL_checkstring(L, 2);

    this->set_text(text);
    return 0;
}

LUA_FUNCTION int Button::lua_set_rect(lua_State *L) {
    auto x = luaL_checkint(L, 2);
    auto y = luaL_checkint(L, 3);
    auto w = luaL_checkint(L, 4);
    auto h = luaL_checkint(L, 5);

    this->set_parent_rect({{x,y},{w,h}});
    return 0;
}

LUA_FUNCTION int Button::lua_on_click(lua_State *L) {
    bool isFunction = lua_isfunction(L, 2);

    if (isFunction == false) {
        lua_pushstring(L, "argumet - not a function.");
        lua_error(L);
    }

   	this->click_event_ref_id = luaL_ref(L, LUA_REGISTRYINDEX);
    return 0;
}

}