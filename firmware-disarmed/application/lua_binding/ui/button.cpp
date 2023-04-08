
#include "button.hpp"
#include "lua_binding/lua_binding.hpp"

namespace lua_ui {

/* static */ void Button::initialize_luabinding(lua_State *L, ui::LuaView *luaView) {
	typedef lua::LuaBinding<lua_ui::Button> ButtonBinding;

    ButtonBinding::Init("Button", [luaView](lua_ui::Button * s) {
        s->set_parent(luaView);
        luaView->add_children({s});
    });

    ButtonBinding::registerFunc<&lua_ui::Button::lua_SetText>("SetText");
    ButtonBinding::registerFunc<&lua_ui::Button::lua_SetRect>("SetRect");
    ButtonBinding::CreateLuaMetaClass(L, "CreateButton");
}

Button::Button() {
    this->set_parent_rect({{40, 40}, {60, 60}});
}

int Button::lua_SetText(lua_State *L) {
    auto text = luaL_checkstring(L, 2);
    this->set_text(text);

    return 0;
}

int Button::lua_SetRect(lua_State *L) {
    auto x = luaL_checkint(L, 2);
    auto y = luaL_checkint(L, 3);
    auto w = luaL_checkint(L, 4);
    auto h = luaL_checkint(L, 5);

    this->set_parent_rect({{x,y},{w,h}});

    return 0;
}


}