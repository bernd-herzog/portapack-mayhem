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

    // void focus() override;

    std::string title() const override { return "Signal view"; };

   private:
    OptionsField options_modulation{
        {0 * 8, 0 * 16},
        4,
        {
            {"RAW ", 0},
            {" FM ", 1},
            {" AM ", 2},
        }
    };

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
