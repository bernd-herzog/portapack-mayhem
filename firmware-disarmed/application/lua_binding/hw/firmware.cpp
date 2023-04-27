
#include "lua_binding/lua_state.hpp"
#include "firmware.hpp"

#include "baseband_api.hpp"
#include "core_control.hpp"
#include "bitmap.hpp"
#include "png_writer.hpp"
#include "ui_flash_utility.hpp"
#include "ui_lua_app_view.hpp"

extern uint32_t __process_stack_base__;
extern uint32_t __process_stack_end__;
#define CRT0_STACKS_FILL_PATTERN 0x55555555

namespace lua_hw {

/* static */ void Firmware::initialize_lua_binding(lua_State *L, ui::NavigationView& nav) {
	typedef lua::LuaBinding<lua_hw::Firmware> FirmwareBinding;

    FirmwareBinding::initialize_object_creation("Firmware", [&nav](lua_hw::Firmware *created_firmware) { 
        created_firmware->run_flash_utility = [&nav](){nav.push<ui::FlashUtilityView>();};
        created_firmware->get_navigation_view = [&nav]()->ui::NavigationView&{return nav;};
    });

    FirmwareBinding::regiser_object_creation_function(L, "GetFirmware");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_run_sd_over_usb>("RunSDOverUSB");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_run_flash_utility>("RunFlashUtility");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_run_hack_rf_mode>("RunHackRfMode");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_get_free_heap>("GetFreeHeap");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_get_free_stack>("GetFreeStack");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_take_screenshot>("TakeScreenshot");
    FirmwareBinding::register_lua_function<&lua_hw::Firmware::lua_run_app>("RunApp");
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

LUA_FUNCTION int Firmware::lua_get_free_heap(lua_State *L) {
    lua_pop(L, 1);

    const auto m0_core_free = chCoreStatus();

	size_t m0_fragmented_free_space = 0;
	chHeapStatus(NULL, &m0_fragmented_free_space);

    lua_pushnumber(L, (double) (m0_core_free + m0_fragmented_free_space));
    //lua_pushinteger(L, m0_fragmented_free_space);

    return 1;
}

LUA_FUNCTION int Firmware::lua_get_free_stack(lua_State *L) {
    (void)L;
        
    uint32_t *p;
    for (p = &__process_stack_base__; *p == CRT0_STACKS_FILL_PATTERN && p < &__process_stack_end__; p++);
    auto stack_space_left = p - &__process_stack_base__;

    lua_pushinteger(L, stack_space_left);

    return 1;
}

LUA_FUNCTION int Firmware::lua_take_screenshot(lua_State *L) {
    (void)L;

	auto path = next_filename_stem_matching_pattern(u"SCR_????");
	if( path.empty() ) {
		return 0;
	}

	PNGWriter png;
	auto create_error = png.create(path.replace_extension(u".PNG"));
	if( create_error.is_valid() ) {
		return 0;
	}

	for(int i = 0; i < 320; i++) {
		std::array<ui::ColorRGB888, 240> row;
		portapack::display.read_pixels({ 0, i, 240, 1 }, row);
		png.write_scanline(row);
	}

    return 0;
}

LUA_FUNCTION int Firmware::lua_run_app(lua_State *L) {
    char *app_name = const_cast<char *>(luaL_checkstring(L, 2));
    
    //TODO: create new lua state for every app instance
    //TODO: gc after events
    auto& navigation_view = get_navigation_view();

    //TODO: cant pop last item
    navigation_view.pop();
    navigation_view.push<ui::LuaAppView>(app_name);

    return 0;
}

}