
#include "lua_binding/lua_state.hpp"
#include "label.hpp"

namespace lua_ui {

/* static */ void Label::initialize_lua_binding(lua_State *L, ui::View *parent) {
	typedef lua::LuaBinding<lua_ui::Label> LabelBinding;

    LabelBinding::initialize_object_creation("Label", [parent](lua_ui::Label *created_label) {
        created_label->set_parent(parent);
        parent->add_children({created_label});
    });

    LabelBinding::regiser_object_creation_function(L, "CreateLabel");
    LabelBinding::register_lua_function<&lua_ui::Label::lua_set_text_and_position>("SetTextAndPosition");
}

Label::Label() : ui::Labels({}) {
}

LUA_FUNCTION int Label::lua_set_text_and_position(lua_State *L) {
    auto text = luaL_checkstring(L, 2);
    auto x = luaL_checkint(L, 3);
    auto y = luaL_checkint(L, 4);

    this->set_labels({{{x,y}, text, ui::Color::white()}});
    return 0;
}

}
