/*
 * Copyright (C) 2024 Bernd Herzog
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

#include "ui_external_module_view.hpp"
#include "portapack.hpp"
#include "external_module_api.hpp"

#include <optional>

namespace ui {

void ExternalModuleView::on_tick_second() {
    std::optional<ExternalModule::device_info> device_info = portapack::external_module.get_device_info();

    if (device_info.has_value() == false) {
        text_header.set("No module connected");
        text_name.set("");
        text_version.set("");
        return;
    }

    text_header.set("Module found");

    std::string btnText = (std::string) "Module: " + device_info->module_name;
    text_name.set(btnText);
    text_version.set("Version: " + std::to_string(device_info->module_version));
}
}  // namespace ui