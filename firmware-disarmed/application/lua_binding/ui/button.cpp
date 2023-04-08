
#include "lua_binding/lua_state.hpp"
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
    ButtonBinding::registerFunc<&lua_ui::Button::lua_OnClick>("OnClick");
    ButtonBinding::CreateLuaMetaClass(L, "CreateButton");
}

Button::Button() /* :button_click_handler(0)*/ {
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

int Button::lua_OnClick(lua_State *L) {
    bool isFunction = lua_isfunction(L, 2);

    if (isFunction == false) {
        lua_pushstring(L, "argumet - not a function.");
        lua_error(L);
    }

    // store somewhere
   	this->click_ref_id = luaL_ref(L, LUA_REGISTRYINDEX);
    
    //lua_setfield(L, 1, "on_click");

    // this->button_click_handler = lua_tocfunction(L, 2);
    // if (!this->button_click_handler) {
    //     chDbgPanic("no func");
    // }

    this->on_select = [](ui::Button& button) {
        lua_ui::Button *b = (lua_ui::Button *)(&button);
        
        lua::lua_state.execute_lua_function(b->GetClickRefID());



        // if (b->button_click_handler) {
        //     chDbgPanic("on_select");
            
        // }
    };

    //auto clickHandler = luaL_checkint(L, 2);

    return 0;
}


}