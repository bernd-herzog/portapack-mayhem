/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
 * Copyright (C) 2017 Furrtek
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

//#pragma once

#include "ui.hpp"
#include "portapack.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "event_m0.hpp"
#include "lua_binding.hpp"

//#include "ui_geomap.hpp"
//#include "ui_font_fixed_8x16.hpp"

//#include "file.hpp"
//#include "database.hpp"
//#include "recent_entries.hpp"
//#include "log_file.hpp"
//#include "adsb.hpp"
//#include "message.hpp"
//#include "app_settings.hpp"
//#include "crc.hpp"

namespace ui {
	
class LuaView : public View {
public:
	LuaView(NavigationView& nav);
	~LuaView();
	
	void focus() override;

	LuaView(const LuaView&) = delete;
	LuaView(LuaView&&) = delete;
	LuaView& operator=(const LuaView&) = delete;
	LuaView& operator=(LuaView&&) = delete;
	
	std::string title() const override { return "Lua"; };

private:
	Labels labels {
		{ { 0 * 8, 0 * 8 }, "LNA:   VGA:   AMP:", Color::light_grey() }
	};

	Button button_run {
		{ 9 * 8, 15 * 16, 12 * 8, 3 * 16 },
		"Run" 
	};

	MessageHandlerRegistration message_handler_frame {
		Message::ID::TestAppPacket,
		[this](Message* const p) {
			const auto message = static_cast<const TestAppPacketMessage*>(p);
			(void) message;
			//this->on_frame(message);
		}
	};
};

} /* namespace ui */
