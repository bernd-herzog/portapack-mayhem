#include "ui_lua_view.hpp"

#include "lua_binding/lua_state.hpp"
#include "lua_binding/lua_binding.hpp"
#include "ff.h"

#include "lua_binding/ui/button.hpp"

namespace ui {
static constexpr ui::Style style_default {
	.font = ui::font::fixed_8x16,
	.background = ui::Color::black(),
	.foreground = ui::Color::white()
};

LuaView::LuaView(
    Context& context,
    const Rect parent_rect
) : View { parent_rect },
    context_(context),
    luaError("")
{
    set_style(&style_default);
    this->add_children({new ui::Labels({{{2,8},"ctor();", Color::white()}})});

    FIL lua_file;
    if (f_open(&lua_file, reinterpret_cast<const TCHAR*>(u"/APPS/main.lua"), FA_READ) != FR_OK) {
        auto *error = new ui::Labels({{{2,2},"/APPS/main.lua is missing.", Color::white()}});
        this->add_children({
            error
        });
        return;
    }

    f_close(&lua_file);

	add_children({
		&button_run
	});

    button_run.on_select = [this](Button&) {
        this->LuaInit(lua::lua_state.get_state());
    };
}

Context& LuaView::context() const {
    return context_;
}

MakeWrapper(LuaErrorWrapper, (lua_State *L), (L), LuaView, int);

void LuaView::LuaInit(lua_State *L) {
    lua_ui::Button::initialize_luabinding(L, this);

 	lua_atpanic(L, GetWrapper(LuaErrorWrapper, &LuaView::lua_at_panic));
    
    int r = setjmp(jumpBuffer);
    if (r != 0)
    {
        this->add_children({new ui::Labels({{{2,2*20},"setjmp();", Color::white()}})});
        return;
    }

    lua::lua_state.execute_lua_script(reinterpret_cast<const TCHAR*>(u"/APPS/main.lua"));

    this->add_children({new ui::Labels({{{2,3*20},"execute_lua_script();", Color::white()}})});
}

int LuaView::lua_at_panic(lua_State *L) {
	luaError = lua_tostring(L, -1);
    
    this->add_children({new ui::Labels({{{2,8*20}, luaError, Color::white()}})});

	lua_pop(L, 1);
	longjmp(jumpBuffer, 1);
	return 1;
}

}
