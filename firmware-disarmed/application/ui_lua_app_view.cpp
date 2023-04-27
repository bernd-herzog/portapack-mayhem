#include "ui_lua_app_view.hpp"

#include "lua_binding/ui/button.hpp"
#include "lua_binding/ui/label.hpp"
#include "lua_binding/hw/firmware.hpp"

#include "baseband_api.hpp"
#include "core_control.hpp"
#include "string_format.hpp"

namespace ui {

LuaAppView::LuaAppView(NavigationView& nav) :
    nav_ (nav),
    app_name (nullptr)
{
	add_children({
		&button_run,
		&button_sd
	});

    button_run.on_select = [this](Button&) {
    };

    button_sd.on_select = [this](Button&) {
        this->ActivateSDMode();
    };
}

LuaAppView::LuaAppView(NavigationView& nav, const char *app_name) :
    nav_ (nav),
    app_name (app_name)
{
	add_children({
		&button_run,
		&button_sd
	});

    button_run.on_select = [this](Button&) {
    };

    button_sd.on_select = [this](Button&) {
        this->ActivateSDMode();
    };
}

LuaAppView::~LuaAppView(){
    for(auto child : this->lua_children) {
		delete child;
	}
}

void LuaAppView::focus() {
	button_run.focus();
}

void LuaAppView::paint(Painter& painter) {
    /* delayed execute */
    if (lua_initialized == false) {
        lua_initialized = true;

        this->LuaInit();
        this->set_dirty();
    }

    ui::View::paint(painter);
}

void LuaAppView::add_lua_child(ui::Widget * child){
    this->add_children({child});
    this->lua_children.push_back(child);
}

void LuaAppView::LuaInit() {
    lua::lua_state.init();
    auto lua_state = lua::lua_state.get_state();
    
    lua_ui::Button::initialize_lua_binding(lua_state, this);
    lua_ui::Label::initialize_lua_binding(lua_state, this);
    lua_hw::Firmware::initialize_lua_binding(lua_state, this->nav_);

    lua::lua_state.on_lua_error = [this](std::string luaError, int line) {
        constexpr int line_chars = 28;
        unsigned int i = 0;
        for (; i <= (luaError.length()-1)/line_chars; i++) {
            this->add_children({
                new ui::Labels({{{2, (int)i*20}, luaError.substr(i*line_chars, line_chars), Color::white()}}),
                    &button_run,
                    &button_sd
                });
        }

        this->add_children({
            new ui::Labels({{{2, (int)i*20}, to_string_dec_int(line), Color::white()}}),
                &button_run,
                &button_sd
            });
    };

    if (app_name) {
        static TCHAR buf[255];
        memset(buf, 255, sizeof(TCHAR));
        for (int i = 0; i < 255; i++) {
            buf[i] = app_name[i];
            if (app_name[i] == 0)
            break;
        }

        lua::lua_state.execute_lua_script(buf);
    }
    else {
        lua::lua_state.execute_lua_script(reinterpret_cast<const TCHAR*>(u"/APPS/main.lua"));
    }

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
