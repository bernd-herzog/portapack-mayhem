#pragma once

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"

namespace ui {
class LuaSystemView : public OverlayView {
public:
	LuaSystemView(
		ui::Context &context,
		const ui::Rect parent_rect);

	ui::Context &context() const override;
	void toggle_overlay();
	void paint_overlay();

private:
	ui::Context &context_;
	ui::NavigationView navigation_view{};
	ui::DfuMenu overlay{navigation_view};
};
}
