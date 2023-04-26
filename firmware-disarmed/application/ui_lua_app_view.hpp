#pragma once

#include "tiny_memory_elua/lua.hpp"
#include "lua_binding/lua_wrapper.hpp"
#include "lua_binding/lua_state.hpp"

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"

namespace ui {
class LuaAppView : public ui::View {
public:
	LuaAppView(NavigationView& nav);
	LuaAppView(NavigationView& nav, const char *app_name);

	void focus() override;
	void paint(Painter& painter) override;

	void LuaInit();
	void ActivateSDMode();

private:
    bool lua_initialized = false;
	NavigationView& nav_;
	const char *app_name;

	Button button_run {
		{ 2 * 8, 14 * 16, 26 * 8, 2 * 16 },
		"Restart App"
	};

	Button button_sd {
		{ 2 * 8, 17 * 16, 26 * 8, 2 * 16 },
		"Start SD over USB"
	};
};
}
