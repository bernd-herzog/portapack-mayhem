#pragma once

#include "tiny_memory_elua/lua.hpp"
#include "lua_binding/lua_wrapper.hpp"

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"

namespace ui {
class LuaSystemView : public View {
public:
	LuaSystemView(
		ui::Context &context,
		const ui::Rect parent_rect);

	ui::Context &context() const override;
	void focus() override;

	void LuaInit(lua_State *L);
	void ActivateSDMode();

private:
	ui::Context &context_;
	ui::NavigationView navigation_view{};

	Button button_run {
		{ 1 * 8, 15 * 16, 12 * 8, 3 * 16 },
		"Run" 
	};
	Button button_sd {
		{ 14 * 8, 15 * 16, 12 * 8, 3 * 16 },
		"SD" 
	};

};
}
