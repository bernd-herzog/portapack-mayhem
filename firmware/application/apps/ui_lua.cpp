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

#include "ui_lua.hpp"
#include "ui_alphanum.hpp"

#include "rtc_time.hpp"
#include "string_format.hpp"
#include "baseband_api.hpp"

#include "lua/lstate.h"

//#include "portapack_persistent_memory.hpp"
extern char *debug_messages[2];
extern "C" { extern void test_heap(char *a); }

using namespace portapack;

namespace ui {

LuaView::~LuaView() {

	// save app settings

	//TODO: once all apps keep there own settin previous frequency logic can be removed
	//receiver_model.set_tuning_frequency(prevFreq);
	//rtc_time::signal_tick_second -= signal_token_tick_second;
	//receiver_model.disable();
	lua_binding.shutdown();
	baseband::shutdown();
}

LuaView::LuaView(NavigationView& nav) {


	//size_t m0_fragmented_free_space = 0;
	//chHeapStatus(NULL, &m0_fragmented_free_space);
  	//debug_messages[1] = (char *)std::to_string(m0_fragmented_free_space).c_str();

	baseband::run_image(portapack::spi_flash::image_tag_test);

	add_children({
		&labels,
	});

  	//debug_messages[0] = "LuaView f1";
	
	


	//debug_messages[0] = "LuaView f2";
	test_heap("lua_view1");
	lua_binding.init();
	//debug_messages[0] = "LuaView f3";
	//realloc(NULL, 222);
	//debug_messages[0] = "LuaView f3b";

	test_heap("lua_view2");
	//chHeapStatus(NULL, &m0_fragmented_free_space);

  	//debug_messages[1] = (char *)std::to_string(m0_fragmented_free_space).c_str();
	//debug_messages[0] = "LuaView f4";
	//global_State *g = nullptr;
	//try
	//{
	//	
	//	//labels.set_labels({{ { 0 * 8, 0 * 8 }, "Very long text 12234567890", Color::light_grey() },});
	//}
	//catch(...)
	//{
	//	labels.set_labels({{ { 0 * 8, 0 * 8 }, "Exception", Color::light_grey() },});
	//}
	
	//void *i = realloc(NULL, 2086);
	//char buf[10];
	//snprintf(buf, 10, "%X", i);
	//labels.set_labels({{ { 0 * 8, 0 * 8 }, buf, Color::light_grey() },});
	//

	//baseband::set_adsb();
	//
	//receiver_model.set_tuning_frequency(1090000000);
	//receiver_model.set_modulation(ReceiverModel::Mode::SpectrumAnalysis);
	//receiver_model.set_sampling_rate(2000000);
	//receiver_model.set_baseband_bandwidth(2500000);
	//receiver_model.enable();
}

} /* namespace ui */
