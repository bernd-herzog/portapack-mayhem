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

#include "ui_sigview.hpp"
#include "portapack.hpp"
#include "baseband_api.hpp"
#include "portapack.hpp"

#include <cstring>
#include <stdio.h>

using namespace portapack;

namespace ui {

SignalAnalyserView::SignalAnalyserView(
    NavigationView& nav) {
    baseband::run_image(portapack::spi_flash::image_tag_sigview);

    add_children({&options_modulation,
                  &field_frequency,
                  &field_lna,
                  &field_vga,
                  &rssi,
                  &channel,
                  &audio,
                  &field_volume,
                  &label_config,
                  &field_bw});

    options_modulation.on_show_options = [this]() {
        this->on_show_options_modulation(options_modulation.selected_index_value());
    };

    // Set on_change before initialising the field
    field_frequency.on_change = [this](rf::Frequency f) {
        // this->on_tuning_frequency_changed(f);
    };

    field_frequency.set_value(receiver_model.tuning_frequency());
    field_frequency.set_step(receiver_model.frequency_step());
    field_frequency.on_edit = [this, &nav]() {
        // TODO: Provide separate modal method/scheme?
        auto new_view = nav.push<FrequencyKeypadView>(receiver_model.tuning_frequency());
        new_view->on_changed = [this](rf::Frequency f) {
            // this->on_tuning_frequency_changed(f);
            this->field_frequency.set_value(f);
        };
    };
    field_frequency.on_show_options = [this]() {
        // this->on_show_options_frequency();
    };
}

SignalAnalyserView::~SignalAnalyserView() {
    baseband::shutdown();
}

void SignalAnalyserView::focus() {
    options_modulation.focus();
}

void SignalAnalyserView::on_show_options_modulation(size_t option) {
}

void SignalAnalyserView::on_show_options_frequency() {
}

}  // namespace ui
