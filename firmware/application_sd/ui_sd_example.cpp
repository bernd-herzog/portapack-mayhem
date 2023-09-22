/*
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

#include "ui_sd_example.hpp"
#include "portapack_shared_memory.hpp"
#include "ff.h"

namespace ui {

SdExampleView::SdExampleView(NavigationView& nav)
    : nav_(nav) {
    // add_children({
    //     &labels /*,
    //      &button_run*/
    // });

    // button_run.on_select = [this](Button&) {
    //     ui::Painter painter;

    // for (size_t i = 0; i < 10; i++) {
    //     painter.fill_rectangle(
    //         {0, 0, portapack::display.width(), portapack::display.height()},
    //         ui::Color::blue());

    // chThdSleep(500);

    // painter.fill_rectangle(
    //     {0, 0, portapack::display.width(), portapack::display.height()},
    //     ui::Color::black());

    // chThdSleep(500);
    // }

    // // FIL firmware_file;
    // // if (f_open(&firmware_file, reinterpret_cast<const TCHAR*>(u"/sd.ext"), FA_READ) != FR_OK) chDbgPanic("no file");

    // // uint8_t* data_buffer = (uint8_t*)ADDRESS_MEMORY_SPACE;

    // // for (size_t page_index = 0; page_index < 64 * 512; page_index += 512) {
    // //     size_t bytes_read;
    // //     if (f_read(&firmware_file, data_buffer + page_index, 512, &bytes_read) != FR_OK) chDbgPanic("no data");

    // // if (bytes_read < 512)
    // //     break;
    // // }

    // // void (*f)(void) = (void (*)())(ADDRESS_MEMORY_SPACE | 0x01 /*Thumb mode*/);
    // // f();

    // this->set_dirty();

    // // ui::Painter painter;
    // // painter.fill_rectangle(
    // //     {0, 0, portapack::display.width(), portapack::display.height()},
    // //     ui::Color::black());

    // // painter.draw_bitmap(
    // //     {portapack::display.width() / 2 - 8, portapack::display.height() / 2 - 8},
    // //     bitmap_icon_hackrf,
    // //     ui::Color::yellow(),
    // //     ui::Color::black());
    // };
}

void SdExampleView::focus() {
    // button_run.focus();
}

} /* namespace ui */
