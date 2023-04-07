#include "ui_lua_view.hpp"

#include "lua_binding/state.hpp"
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

        lua::lua_binding.execute_lua_script(reinterpret_cast<const TCHAR*>(u"/APPS/main.lua"));
    }

    Context& LuaView::context() const {
        return context_;
    }
}
