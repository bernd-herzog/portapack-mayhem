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
) : OverlayView { parent_rect },
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

void LuaSystemView::toggle_overlay() {
	if (overlay_active){
		this->remove_child(&this->overlay);
		this->set_dirty();
		shared_memory.request_m4_performance_counter = 0;
	}
	else{
		this->add_child(&this->overlay);
		this->set_dirty();
		shared_memory.request_m4_performance_counter = 1;
		shared_memory.m4_cpu_usage = 0;
		shared_memory.m4_heap_usage = 0;
		shared_memory.m4_stack_usage = 0;
	}

	overlay_active = !overlay_active;
}

void LuaSystemView::paint_overlay() {
	static bool last_paint_state = false;
	if (overlay_active){
		// paint background only every other second
		if ((((chTimeNow()>>10) & 0x01) == 0x01) == last_paint_state)
			return;

		last_paint_state = !last_paint_state;
		this->overlay.set_dirty();
	}
}

}
