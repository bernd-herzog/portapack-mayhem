/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
 * Copyright (C) 2016 Furrtek
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __UI_FLSH_UITILTY_H__
#define __UI_FLSH_UITILTY_H__

#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "string_format.hpp"
#include "ff.h"
#include "baseband_api.hpp"
#include "core_control.hpp"

#include <cstdint>

namespace ui {

class FlashUtilityView : public View {
public:
	FlashUtilityView(NavigationView& nav);
	~FlashUtilityView();
	void focus() override;
	
	std::string title() const override { return "Flash Utility"; };	

private:
	NavigationView& nav_;
	
	bool confirmed = false;
	static Thread* thread;
	
	static msg_t static_fn(void* arg) {
		auto obj = static_cast<FlashUtilityView*>(arg);
		obj->run();
		return 0;
	}
	
	void run();

	Labels labels {
		{ { 0, 0 }, "Select image:", Color::white() }
	};

	MenuView menu_view {
		{ 0, 2 * 8, 240, 26 * 8 },
		true
	};

	// Text text_info {
	// 	{ 10 * 8, 16 * 8, 10 * 8, 16 },
	// 	"Working..."
	// };
	
	// ProgressBar progress {
	// 	{ 2 * 8, 19 * 8, 26 * 8, 24 }
	// };
	
	// Button dummy {
	// 	{ 240, 0, 0, 0 },
	// 	""
	// };
};

} /* namespace ui */

#endif/*__UI_FLSH_UITILTY_H__*/
