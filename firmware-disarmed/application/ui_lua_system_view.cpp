#include "ui_lua_system_view.hpp"
#include "ui_lua_app_view.hpp"

#include "lua_binding/lua_state.hpp"
#include "lua_binding/lua_binding.hpp"
#include "ff.h"
#include "bitmap.hpp"

namespace ui {
static constexpr ui::Style style_default {
	.font = ui::font::fixed_8x16,
	.background = ui::Color::black(),
	.foreground = ui::Color::white()
};

LuaSystemView::LuaSystemView(
    Context& context,
    const Rect parent_rect
) : View { parent_rect },
    context_(context)
{
    set_style(&style_default);

    FIL lua_file;
    if (f_open(&lua_file, reinterpret_cast<const TCHAR*>(u"/APPS/main.lua"), FA_READ) != FR_OK) {
        auto *error = new ui::Labels({{{2,2},"/APPS/main.lua is missing.", Color::white()}});
        this->add_children({
            error
        });
        return;
    }

    f_close(&lua_file);

	navigation_view.set_parent_rect(parent_rect);

	add_children({
        &navigation_view,
	});

    navigation_view.push<ui::LuaAppView>();
    navigation_view.push<ui::BMPView>();
}

Context& LuaSystemView::context() const {
    return context_;
}

}
