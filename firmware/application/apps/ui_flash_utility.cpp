/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
 * Copyright (C) 2023 Bernd Herzog
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

#include "ui_flash_utility.hpp"
#include "portapack_shared_memory.hpp"

namespace ui {

Thread* FlashUtilityView::thread { nullptr };

FlashUtilityView::FlashUtilityView(NavigationView& nav) : nav_ (nav) {
	add_children({
		&menu_view
	});

	menu_view.set_parent_rect({ 0, 3 * 8, 240, 29 * 8 });

	for (const auto& entry : std::filesystem::directory_iterator(u"FIRMWARE", u"*.bin")) {
		auto filename = entry.path().filename();
		auto path = entry.path().native();

		menu_view.add_item({
			filename.string(),
			ui::Color::red(),
			&bitmap_icon_temperature,
			[this, path]() {
				nav_.push<ModalMessageView>(
					"Warning!", 
					"This will replace your\ncurrent firmware.\n\nIf things go wrong you are\nrequired to flash manually\nwith dfu.",
					YESCANCEL, 
					[this, path](bool choice) {
						if (choice) {
							std::u16string full_path = std::u16string(u"FIRMWARE/") + path;
							std::memcpy(&shared_memory.bb_data.data[0], full_path.c_str(), (full_path.length() + 1) * 2);

							portapack::shutdown();

							m4_init(portapack::spi_flash::image_tag_flash_utility, portapack::memory::map::m4_code, false);
							m0_halt();
						}
					}
				);
			}
		});

	}

}

FlashUtilityView::~FlashUtilityView() {
	// if (thread)
	// 	chThdTerminate(thread);
}

 void FlashUtilityView::focus() {
	menu_view.focus();
	
// 	// if (!confirmed) {
// 	// 	nav_.push<ModalMessageView>("Warning !", "Wipe ROM?", YESCANCEL, [this](bool choice) {
// 	// 			if (choice)
// 	// 				confirmed = true;
// 	// 		}
// 	// 	);
// 	// } else {
// 	// 	thread = chThdCreateFromHeap(NULL, 2048, NORMALPRIO, FlashUtilityView::static_fn, this);
// 	// }
 }

void FlashUtilityView::run(){
		// const int count = 16;
		// progress.set_max(count);

		// for (uint32_t i = 0; i < count; i++) {
		// 	progress.set_value(i);
		// 	chThdSleepMilliseconds(20);
		// }

		baseband::run_image(portapack::spi_flash::image_tag_flash_utility);
		m0_halt();
		/* m0_halt never returns */
	}

} /* namespace ui */
