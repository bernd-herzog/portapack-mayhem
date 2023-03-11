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

#include <strings.h>
#include "ch.h"

#include "ui_lua.hpp"
#include "ui_alphanum.hpp"

#include "rtc_time.hpp"
#include "string_format.hpp"
#include "baseband_api.hpp"

#include "lua/lstate.h"

using namespace portapack;

namespace ui {

LuaView::~LuaView() {

	// save app settings

	//TODO: once all apps keep there own settin previous frequency logic can be removed
	//receiver_model.set_tuning_frequency(prevFreq);
	//rtc_time::signal_tick_second -= signal_token_tick_second;
	//receiver_model.disable();
	//
	baseband::shutdown();
}

LuaView::LuaView(NavigationView& nav) {

	//chDbgPanic("sadf");
    void (*bad_instruction)(void);
    bad_instruction = (void (*)())0xE0000000;
    bad_instruction(); // more bad

	

	//size_t m0_fragmented_free_space = 0;
	//chHeapStatus(NULL, &m0_fragmented_free_space);

	baseband::run_image(portapack::spi_flash::image_tag_test);

	add_children({
		&labels,
	});

	
	portapack::lua_binding->enable();
	//portapack::lua_binding->get_str();
	char *s = portapack::lua_binding->get_buf();

	labels.set_labels({{ { 0 * 8, 0 * 8 }, s, Color::light_grey() },});
	

}

} /* namespace ui */
