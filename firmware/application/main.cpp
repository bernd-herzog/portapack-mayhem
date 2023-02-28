/*
 * Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
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

// Color bitmaps generated with:
// Gimp image > indexed colors (16), then "xxd -i *.bmp"

// Note about available RAM:
// Check what ends up in the BSS section by looking at the map files !
// Use constexpr where possible or make sure const are in .cpp files, not headers !

// Note about messages:
// There can only be one message handler for one kind of message at once
// If an attempt is made to register a second handler, there's a chDbgPanic "MsgDblReg"

// Note about matched filters: see proc_sonde.hpp

//TEST: Goertzel tone detect
//TEST: Menuview refresh, seems to blink a lot
//TEST: Check AFSK transmit end, skips last bits ?
//TEST: Imperial in whipcalc

//BUG: Console lock-up if first string to be printed starts with escape character ?
//BUG: (Workaround ok) CPLD-related rx ok, tx bad, see portapack.cpp lines 214+ to disable CPLD overlay
//BUG: SCANNER Lock on frequency, if frequency jump, still locked on first one
//BUG: SCANNER Multiple slices
//GLITCH: The about view scroller sometimes misses lines because of a race condition between the display scrolling and drawing the line
//GLITCH: Start of tx using ReplayThread plays a small bit of previous transmission (content of 1 buffer ?)
//	See fifo.reset_in() ?

//FIXED: Update button in signal gen doesn't work for shape change
//BUG: Signal gen noise shape doesn't work
//TODO: Continue acars receiver. See matched filter, probably doesn't shift the spectrum correctly
//TODO: Add larger description text field in frequency load, under menuview
//TODO: Allow apps to select a preferred FREQMAN file
//TODO: Make play button larger in Replay
//TODO: Add default headphones volume setting in Audio settings
//TODO: Put LNA and VGA controls in Soundboard
//TODO: Make CTCSS display only when squelch is opened
//TODO: DCS decoder
//TODO: Increase resolution of audio FFT view ? Currently 48k/(256/2) (375Hz) because of the use of real values (half of FFT output)
//TODO: Move Touchtunes remote to Custom remote
//TODO: Use escapes \x1B to set colors in text, it works !
//TODO: Open files in File Manager
//TODO: Ask for filename after record
//TODO: Super simple text file viewer
//TODO: Clean up ReplayThread
//TODO: Cap Wav viewer position
//TODO: Adapt wav viewer position step
//TODO: Use unit_auto_scale
//TODO: Remove make_bistream from encoders.cpp, too complex, stinks. bitstream_append should be enough.
//TODO: Continue work on proc_afskrx_corr, see python script (it works !)
//TODO: De bruijn sequence scanner for encoders
//TODO: FILEMAN Move files
//TODO: Use separate thread for scanning in EPAR TX
//TODO: Make freqman refresh simpler (use previous black rectangle method)
//TODO: Merge AFSK and TONES procs ?
//TODO: NFM RX mode: nav.pop on squelch
//TODO: MORSE use prosigns
//TODO: MORSE live keying mode
//TODO: Use to_string_short_freq wherever possible
//TODO: SCANNER Waveform widget as FFT view ?
//TODO: Optimize (and group ?) CTCSS tone gen code
/*
Continuous (Fox-oring)
12s transmit, 48s space (Sprint 1/5th) 
60s transmit, 240s space (Classic 1/5 min) 
60s transmit, 360s space (Classic 1/7 min) 
*/
//TODO: FreqMan: Remove and rename categories
//TODO: Mousejack ?
//TODO: Move frequencykeypad from ui_receiver to ui_widget (used everywhere)
//TODO: ADS-B draw trajectory + GPS coordinates + scale, and playback
//TODO: RDS multiple groups (sequence)
//TODO: Use ModalMessageView confirmation for TX ?
//TODO: Use msgpack for settings, lists... on sd card

// Multimon-style stuff:
//TODO: DMR detector
//TODO: GSM channel detector
//TODO: Playdead amnesia and login
//TODO: Setup: Play dead by default ? Enable/disable ?

// Old or low-priority stuff:
//TODO: Bodet :)
//TODO: Analog TV tx with camcorder font character generator
//TODO: Scan for OOK TX
//TODO: Check more OOK encoders
//BUG (fixed ?): No audio in about when shown second time
//TODO: Show MD5 mismatches for modules not found, etc...
//TODO: Module name/filename in modules.hpp to indicate requirement in case it's not found ui_loadmodule
//BUG: Description doesn't show up first time going to system>module info (UI drawn on top)
//TODO: Two players tic-tac-toe
//TODO: Analog TV pong game

#include "ch.h"

#include "portapack.hpp"
#include "portapack_shared_memory.hpp"

#include "message_queue.hpp"

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_painter.hpp"
#include "ui_navigation.hpp"

#include "irq_lcd_frame.hpp"
#include "irq_controls.hpp"
#include "irq_rtc.hpp"

#include "event_m0.hpp"

#include "core_control.hpp"
#include "spi_image.hpp"

#include "debug.hpp"
#include "led.hpp"

#include "gcc.hpp"
#include "ui.hpp"
#include "sd_card.hpp"

#include "chheap.h"

#include <string.h>

#include "rffc507x.hpp"      /* c/m, avoiding initial short ON Ant_DC_Bias pulse, from cold reset  */

rffc507x::RFFC507x first_if;

static void event_loop() {
	ui::Context context;
	ui::SystemView system_view {
		context,
		portapack::display.screen_rect()
	};

	EventDispatcher event_dispatcher { &system_view, context };
	MessageHandlerRegistration message_handler_display_sleep {
		Message::ID::DisplaySleep,
		[&event_dispatcher](const Message* const) {
			event_dispatcher.set_display_sleep(true);
		}
	};

	event_dispatcher.run();
}



extern "C" {

	extern char debug_messages[3][16];
	char debug_messages[3][16];
	
	void test_heap(char *a){
		strncpy(debug_messages[0], a, 16);
		strncpy(debug_messages[1], "th1", 16);
		void * p = realloc(NULL, 1122);
		free (p);
		strncpy(debug_messages[1], "th2", 16);
		size_t m0_fragmented_free_space = 0;
		chHeapStatus(NULL, &m0_fragmented_free_space);
		strncpy(debug_messages[1], "th3", 16);
		strncpy(debug_messages[2], std::to_string(m0_fragmented_free_space).c_str(), 16);
	}



	CH_IRQ_HANDLER(HardFaultVector) {
		CH_IRQ_PROLOGUE();
		//portapack::shutdown();
		//m4_init(portapack::spi_flash::image_tag_hackrf, portapack::memory::map::m4_code_hackrf, true);
		//m0_halt();
		ui::Rect screen_rect = portapack::display.screen_rect();

		portapack::display.fill_rectangle(screen_rect, ui::Color::dark_grey());
		portapack::display.draw_line(screen_rect.location(), screen_rect.center(), ui::Color::blue());

		//std::stringstream ss;
		//ss << "SCB CPUID" << SCB->CPUID << " end";

		//ss >> s;

		//char buf[42];
		//snprintf(buf, 41, "%lX", SCB->CPUID);

		//s.append(buf);

		// debug_messages[0] = "text 1";
		// debug_messages[1] = "text 2";

		ui::Style style_default {
			.font = ui::font::fixed_8x16,
			.background = ui::Color::black(),
			.foreground = ui::Color::white()
		};

		//portapack::display
		//std::string s(buf);

		const auto m0_core_free = chCoreStatus();
		ui::Painter().draw_string(ui::Point(5, 5), style_default, std::string("free core:"));
		ui::Painter().draw_string(ui::Point(120, 5), style_default, std::string(std::to_string(m0_core_free)));
		ui::Painter().draw_string(ui::Point(5, 25), style_default, std::string("free heap:"));

		ui::Painter().draw_string(ui::Point(5, 45), style_default, std::string(debug_messages[0]));
		ui::Painter().draw_string(ui::Point(5, 65), style_default, std::string(debug_messages[1]));
		ui::Painter().draw_string(ui::Point(5, 85), style_default, std::string(debug_messages[2]));



		//ui::Painter().draw_string(ui::Point(120, 25), style_default, std::string("1"));
		//union heap_header *qp = &get_default_heap()->h_free;
//
		//ui::Painter().draw_string(ui::Point(120, 25), style_default, std::string("2"));
//
		//int i = 55;
		//ui::Painter().draw_string(ui::Point(5, i), style_default, std::to_string(qp->h.size));
		//ui::Painter().draw_string(ui::Point(120, i), style_default, std::to_string((unsigned int)qp->h.u.next));
		//
		//while(qp = qp->h.u.next) {
		//	i+= 20;
		//	ui::Painter().draw_string(ui::Point(5, i), style_default, std::to_string(qp->h.size));
		//	ui::Painter().draw_string(ui::Point(120, i), style_default, std::to_string((unsigned int)qp->h.u.next));
		//}
		//
//
		//size_t m0_fragmented_free_space = 0;
		//const auto m0_fragments = chHeapStatus(NULL, &m0_fragmented_free_space);
		//ui::Painter().draw_string(ui::Point(120, 25), style_default, std::string("3"));
//
		//ui::Painter().draw_string(ui::Point(120, 25), style_default, std::to_string(m0_fragmented_free_space));

		
		//while (!debug_messages.empty()) {
		//	ui::Painter().draw_string(ui::Point(5, i), style_default, debug_messages.front());
        //	debug_messages.pop();
		//	i+= 20;
    	//}

		

		// if (SCB->CFSR & SCB_CFSR_MSTKERR_Msk) {
		// 	// fault_printf("Stacking error");
		// 	// fault_info.decoded_fault_registers.memfault.stacking_error = true;
		// }

		chSysHalt();

		CH_IRQ_EPILOGUE();
	}
}

int main(void) {
	first_if.init();    /* To avoid initial short Ant_DC_Bias pulse ,we need quick set up GP01_RFF507X =1 */
	if( portapack::init() ) {
		portapack::display.init();

		// sdcStart(&SDCD1, nullptr); // Commented out as now happens in portapack.cpp

		// controls_init(); // Commented out as now happens in portapack.cpp
		lcd_frame_sync_configure();
		rtc_interrupt_enable();

		event_loop();


		sdcDisconnect(&SDCD1);
		sdcStop(&SDCD1);

		portapack::shutdown();
	}

	m4_init(portapack::spi_flash::image_tag_hackrf, portapack::memory::map::m4_code_hackrf, true);
	m0_halt();

	return 0;
}
