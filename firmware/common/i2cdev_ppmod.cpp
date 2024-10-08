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

#include "i2cdev_ppmod.hpp"
#include "portapack.hpp"

#include <optional>

namespace i2cdev {

bool I2cDev_PPmod::init(uint8_t addr_) {
    if (addr_ != I2CDEV_PPMOD_ADDR_1) return false;
    addr = addr_;
    model = I2CDECMDL_PPMOD;

    return true;
}

void I2cDev_PPmod::update() {
}

std::optional<I2cDev_PPmod::device_info> I2cDev_PPmod::readDeviceInfo() {
    uint16_t cmd = 0x18F0;
    I2cDev_PPmod::device_info info;

    bool success = i2c_read((uint8_t*)&cmd, 2, (uint8_t*)&info, sizeof(I2cDev_PPmod::device_info));
    if (success == false) {
        return std::nullopt;
    }

    return info;
}

std::optional<I2cDev_PPmod::standalone_app_info> I2cDev_PPmod::getStandaloneAppInfo(uint32_t index) {
    uint16_t cmd = 0xA90B;
    uint32_t data = cmd + (index << 16);
    I2cDev_PPmod::standalone_app_info info;

    bool success = i2c_read((uint8_t*)&data, 4, (uint8_t*)&info, sizeof(I2cDev_PPmod::standalone_app_info));
    if (success == false) {
        return std::nullopt;
    }

    return info;
}

std::vector<uint8_t> I2cDev_PPmod::downloadStandaloneApp(uint32_t index, size_t offset) {
    if (offset % 32 != 0) {
        return {};
    }

    uint16_t data[3] = {0x4183, index, offset / 32};

    std::vector<uint8_t> ret(32);
    bool success = i2c_read((uint8_t*)&data, sizeof(data), (uint8_t*)ret.data(), 32);
    if (success == false) {
        return {};
    }

    return ret;
}

}  // namespace i2cdev
