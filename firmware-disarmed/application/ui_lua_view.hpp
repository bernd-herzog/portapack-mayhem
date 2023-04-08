#pragma once

#include <setjmp.h>

#include "tiny_memory_elua/lua.hpp"
#include "lua_binding/lua_wrapper.hpp"

#include "ui.hpp"
#include "ui_widget.hpp"

namespace ui {
class LuaView : public View {
public:
	LuaView(
		ui::Context &context,
		const ui::Rect parent_rect);

	ui::Context &context() const override;

	void LuaInit(lua_State *L);
	void ActivateSDMode();

private:
	ui::Context &context_;

	std::string luaError;
	jmp_buf jumpBuffer;
	int lua_at_panic(lua_State *L);

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
