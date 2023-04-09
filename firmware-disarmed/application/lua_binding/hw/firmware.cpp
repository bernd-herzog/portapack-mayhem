
#include "lua_binding/lua_state.hpp"
#include "firmware.hpp"

#include "baseband_api.hpp"
#include "core_control.hpp"
#include "bitmap.hpp"
#include "ui_flash_utility.hpp"

namespace lua_hw {

/* static */ void Firmware::initialize_lua_binding(lua_State *L, ui::NavigationView& nav) {
	typedef lua::LuaBinding<lua_hw::Firmware> FirmwareBinding;

    FirmwareBinding::initialize_object_creation("Firmware", [&nav](lua_hw::Firmware *created_firmware) { 
        created_firmware->run_flash_utility = [&nav](){nav.push<ui::FlashUtilityView>();};
    });

    FirmwareBinding::regiser_object_creation_function(L, "CreateFirmware");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_run_sd_over_usb>("RunSDOverUSB");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_run_flash_utility>("RunFlashUtility");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_run_hack_rf_mode>("RunHackRfMode");
}

Firmware::Firmware() {
}

LUA_FUNCTION int Firmware::lua_run_sd_over_usb(lua_State *L) {
    (void)L;

    ui::Painter painter;
    painter.fill_rectangle(
        { 0, 0, portapack::display.width(), portapack::display.height() },
        ui::Color::black()
    );

    painter.draw_bitmap(
        { portapack::display.width()/2-8, portapack::display.height()/2-8 },
        ui::bitmap_icon_hackrf,
        ui::Color::yellow(),
        ui::Color::black()
    );

    sdcDisconnect(&SDCD1);
    sdcStop(&SDCD1);

    portapack::shutdown(true);
    m4_init(portapack::spi_flash::image_tag_usb_sd, portapack::memory::map::m4_code, false);
    m0_halt();


    return 0;
}

LUA_FUNCTION int Firmware::lua_run_flash_utility(lua_State *L) {
    (void)L;

    this->run_flash_utility();

    return 0;
}

LUA_FUNCTION int Firmware::lua_run_hack_rf_mode(lua_State *L) {
    (void)L;

    sdcDisconnect(&SDCD1);
    sdcStop(&SDCD1);

    portapack::shutdown(true);
	m4_init(portapack::spi_flash::image_tag_hackrf, portapack::memory::map::m4_code_hackrf, true);
	m0_halt();
    
    return 0;
}

}