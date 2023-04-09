#pragma once

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

private:
	ui::Context &context_;
	ui::NavigationView navigation_view{};

};
}
