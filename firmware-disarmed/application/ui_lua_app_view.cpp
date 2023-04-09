#include "ui_lua_app_view.hpp"

#include "lua_binding/ui/button.hpp"
#include "lua_binding/ui/label.hpp"

#include "baseband_api.hpp"
#include "core_control.hpp"

namespace ui {

LuaAppView::LuaAppView(NavigationView& nav) : nav_ (nav) {
	add_children({
		&button_run,
		&button_sd
	});

    button_run.on_select = [this](Button&) {
        this->remove_children(children());
        this->set_dirty();
        this->LuaInit(lua::lua_state.get_state());
    };

    button_sd.on_select = [this](Button&) {
        this->ActivateSDMode();
    };
}

void LuaAppView::focus() {
	button_run.focus();
}


void LuaAppView::LuaInit(lua_State *L) {
    lua_ui::Button::initialize_lua_binding(L, this);
    lua_ui::Label::initialize_lua_binding(L, this);

    lua::lua_state.on_error = [this](std::string luaError) {
        constexpr int line_chars = 28;
        for (unsigned int i = 0; i <= (luaError.length()-1)/line_chars; i++)
            this->add_children({new ui::Labels({{{2, (int)i*20}, luaError.substr(i*line_chars, line_chars), Color::white()}})});
    };

    lua::lua_state.execute_lua_script(reinterpret_cast<const TCHAR*>(u"/APPS/main.lua"));
}

void LuaAppView::ActivateSDMode() {
    ui::Painter painter;
        painter.fill_rectangle(
        { 0, 0, portapack::display.width(), portapack::display.height() },
        ui::Color::black()
    );

    painter.draw_bitmap(
        { portapack::display.width()/2-8, portapack::display.height()/2-8 },
        bitmap_icon_hackrf,
        ui::Color::yellow(),
        ui::Color::black()
    );

    sdcDisconnect(&SDCD1);
    sdcStop(&SDCD1);

    portapack::shutdown(true);
    m4_init(portapack::spi_flash::image_tag_usb_sd, portapack::memory::map::m4_code, false);
    m0_halt();
}

}
