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

#pragma once

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "ui_transmitter.hpp"

#include "portapack.hpp"
#include "message.hpp"

namespace ui {

class SignalAnalyserView : public View {
   public:
    SignalAnalyserView(NavigationView& nav);
    ~SignalAnalyserView();

    void focus() override;

    std::string title() const override { return "Signal view"; };

   private:
    void on_show_options_modulation(size_t option);
    void on_show_options_frequency();

    OptionsField options_modulation{
        {0 * 8, 0 * 16},
        4,
        {
            // How to get to the signal where the bits can be read
            {"OOK ", 0},
            {"AM  ", 1},
            {"BPSK", 2},
            {"QPSK", 3},
            {"QAM ", 4},  // needs sync
            {"FSK ", 5},  // FM demod eg. HC-12
            {"AFSK", 6},  // double FM
            {"DSSS", 7},  // FSK with repeated sequence for a bit
        }};

    FrequencyField field_frequency{
        {5 * 8, 0 * 16},
    };

    LNAGainField field_lna{
        {15 * 8, 0 * 16}};

    VGAGainField field_vga{
        {18 * 8, 0 * 16}};

    // TODO: remove ?
    RSSI rssi{
        {21 * 8, 0, 6 * 8, 4},
    };
    Channel channel{
        {21 * 8, 5, 6 * 8, 4},
    };
    Audio audio{
        {21 * 8, 10, 6 * 8, 4},
    };

    NumberField field_volume{
        {28 * 8, 0 * 16},
        2,
        {0, 99},
        1,
        ' ',
    };

    // TODO: hide in options
    Text label_config{
        {0 * 8, 1 * 16, 2 * 8, 1 * 16},
        "BW",
    };
    OptionsField field_bw{
        // 1.2k inc by 100Hz
        // 8.5k inc by 500Hz
        // 15k, inc by 1kHz
        // TODO: onclick: allow any value
        {3 * 8, 1 * 16},
        4,
        {
            // calculate in code
            {"1.5k", 0},
            {"  2k", 1},
            {"2.5k", 2},
        }};

    // freq + step
    // gain + power view
    // audio volume + squelch

    // TODO: view:
    // time with trigger, options: length, trigger off/value/(quiet time?),
    // waterfall
    // constellation & eye(s)
    // image

    // TODO: follow doppler shift/ center
    // TODO:
};

}  // namespace ui
