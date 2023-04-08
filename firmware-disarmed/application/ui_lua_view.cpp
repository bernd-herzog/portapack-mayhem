#include "ui_lua_view.hpp"

#include "lua_binding/lua_state.hpp"
#include "lua_binding/lua_binding.hpp"
#include "ff.h"

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
    /////////////////////
    //lua_pushcfunction(luaState, l_sin);
	//lua_setglobal(luaState, "RegisterEvent");

    //lua::lua_state.register_function("CreateButton", this->create_button);
}

Context& LuaView::context() const {
    return context_;
}

MakeWrapper(LuaErrorWrapper, (lua_State *L), (L), LuaView, int);

void LuaView::LuaInit(lua_State *L) {
	typedef lua::LuaBinding<ui::Button> ButtonBinding;
    int i = 2;

    this->add_children({new ui::Labels({{{2,i++*20},"LuaInit();", Color::white()}})});

	lua_atpanic(L, GetWrapper(LuaErrorWrapper, &LuaView::lua_at_panic));


    this->add_children({new ui::Labels({{{2,i++*20},"lua_atpanic();", Color::white()}})});


    ButtonBinding::Init("Button", [this](ui::Button * s) {

        this->add_children({new ui::Labels({{{2,9*20},"lua CreateButton();", Color::white()}})});

        s->set_text("TEST");
        s->set_parent(this);
        s->set_parent_rect({{40, 40}, {60, 60}});

        this->add_children({s});
    });

    this->add_children({new ui::Labels({{{2,i++*20},"ButtonBinding::Init();", Color::white()}})});
    //ButtonBinding::registerFunc<>("SetPosition"); ...

    ButtonBinding::CreateLuaMetaClass(L, "CreateButton");

    this->add_children({new ui::Labels({{{2,i++*20},"CreateLuaMetaClass();", Color::white()}})});

    int r = setjmp(jumpBuffer);
    if (r != 0)
    {
        this->add_children({new ui::Labels({{{2,i++*20},"setjmp();", Color::white()}})});
        return;
    }

    lua::lua_state.execute_lua_script(reinterpret_cast<const TCHAR*>(u"/APPS/main.lua"));

    this->add_children({new ui::Labels({{{2,i++*20},"execute_lua_script();", Color::white()}})});
}


int LuaView::lua_at_panic(lua_State *L) {
	luaError = lua_tostring(L, -1);
    
    this->add_children({new ui::Labels({{{2,8*20}, luaError, Color::white()}})});

	lua_pop(L, 1);
	longjmp(jumpBuffer, 1);
	return 1;
}

}
