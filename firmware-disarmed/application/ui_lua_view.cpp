#include "ui_lua_view.hpp"

#include "lua_binding/lua_state.hpp"
#include "lua_binding/lua_binding.hpp"
#include "ff.h"
#include "baseband_api.hpp"
#include "core_control.hpp"

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

	add_children({
		&button_run,
        &button_sd
	});

    button_run.on_select = [this](Button&) {
        this->LuaInit(lua::lua_state.get_state());
    };

    button_sd.on_select = [this](Button&) {
        this->ActivateSDMode();
    };
}

Context& LuaView::context() const {
    return context_;
}


void LuaView::LuaInit(lua_State *L) {
    lua_ui::Button::initialize_lua_binding(L, this);

    lua::lua_state.on_error = [this](std::string luaError) {
        constexpr int line_chars = 28;
        for (unsigned int i = 0; i <= (luaError.length()-1)/line_chars; i++)
            this->add_children({new ui::Labels({{{2, (int)i*20}, luaError.substr(i*line_chars, line_chars), Color::white()}})});
    };

    lua::lua_state.execute_lua_script(reinterpret_cast<const TCHAR*>(u"/APPS/main.lua"));
}

void LuaView::ActivateSDMode() {
    sdcDisconnect(&SDCD1);
    sdcStop(&SDCD1);

    portapack::shutdown(true);
    m4_init(portapack::spi_flash::image_tag_usb_sd, portapack::memory::map::m4_code, false);
    m0_halt();
}

}
