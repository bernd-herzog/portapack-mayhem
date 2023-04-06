#pragma once

#include "ui.hpp"
#include "ui_widget.hpp"

namespace ui {
    class LuaView : public View {
	public:
		LuaView(
			ui::Context &context,
			const ui::Rect parent_rect);

		ui::Context &context() const override;

	private:
		//SystemStatusView status_view{navigation_view};
		//InformationView info_view{navigation_view};
		//NavigationView navigation_view{};
		ui::Context &context_;
    };
}
