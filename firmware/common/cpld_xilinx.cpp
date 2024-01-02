/*
 * Copyright (C) 2016 Jared Boone, ShareBrained Technology, Inc.
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

#include "ch.h"
#include "cpld_xilinx.hpp"

namespace cpld {
namespace xilinx {

void XC2C64A::write_sram(const verify_blocks_t& blocks) {
    tap.set_repeat(0);
    tap.set_end_ir(state_t::run_test_idle);
    tap.set_end_dr(state_t::run_test_idle);

    reset();
    enable();

    shift_ir(instruction_t::ISC_WRITE);
    for (const auto& block : blocks) {
        tap.state(state_t::shift_dr);
        tap.shift({block.data.data(), block_length}, false);
        tap.shift({&block.id, block_id_length}, true);
        tap.state(state_t::run_test_idle);
    }

    disable();
    bypass();

    tap.state(state_t::test_logic_reset);
}

bool XC2C64A::verify_sram(const verify_blocks_t& blocks) {
    prepare_read_sram();

    const jtag::tap::bits_t empty_row{block_length};

    auto error = false;
    for (const auto& block : blocks) {
        tap.shift({&block.id, block_id_length}, true);
        tap.state(state_t::run_test_idle);

        tap.state(state_t::shift_dr);
        error |= tap.shift(empty_row, {block.data.data(), block_length}, {block.mask.data(), block_length}, false, nullptr);
    }

    finalize_read_sram(blocks[0].id);

    return !error;
}

std::array<bool, 192UL> XC2C64A::gpio_test(bool control, bool in, bool out) {
    tap.set_repeat(0);
    tap.set_end_ir(state_t::run_test_idle);
    tap.set_end_dr(state_t::run_test_idle);

    reset();

    uint8_t buf[24] = {0};

    for (size_t i = 0; i < 8; i++) {
        /* code */

        uint8_t val = 0;
        if (in)
            val |= 0x1;  // INPUT
        if (out)
            val |= 0x4;  // OUTPUT3
        if (control)
            val |= 0x2;  // CONTROL

        uint32_t val2 = 0;

        val2 |= val;
        val2 |= val << 3;
        val2 |= val << 6;
        val2 |= val << 9;
        val2 |= val << 12;
        val2 |= val << 15;
        val2 |= val << 18;
        val2 |= val << 21;

        buf[i * 3 + 0] = (val2 >> 16) & 0xff;
        buf[i * 3 + 1] = (val2 >> 8) & 0xff;
        buf[i * 3 + 2] = val2 & 0xff;
    }

    // if (in)
    //     buf[0] |= 0x20;  // INPUT
    // if (out)
    //     buf[0] |= 0x80;  // OUTPUT3
    // if (control)
    //     buf[0] |= 0x40;  // CONTROL

    // buf[0] = 0x80 /*INPUT*/ | 0x40 /*OUTPUT3*/ | 0x20 /*CONTROL*/;

    const jtag::tap::bits_t empty_row1{&buf[0], 192};
    // const jtag::tap::bits_t empty_row2{192, true};
    std::vector<bool> from_device;
    // std::vector<bool> from_device2;

    shift_ir(instruction_t::SAMPLE);
    tap.state(state_t::capture_dr);
    tap.state(state_t::shift_dr);
    tap.shift(empty_row1, {}, {}, false, &from_device);
    // tap.shift(empty_row2, {}, {}, false, &from_device2);
    tap.state(state_t::update_dr);
    tap.state(state_t::run_test_idle);

    shift_ir(instruction_t::EXTEST);
    tap.state(state_t::capture_dr);
    // chThdSleepMilliseconds(1000);
    // tap.state(state_t::shift_dr);
    // chThdSleepMilliseconds(1000);
    // // std::vector<bool> from_device2;
    // // tap.shift(empty_row1, {}, {}, false, &from_device2);
    // tap.state(state_t::update_dr);
    // chThdSleepMilliseconds(1000);

    chThdSleepMilliseconds(1000);
    tap.state(state_t::run_test_idle);
    shift_ir(instruction_t::BYPASS);

    std::array<bool, 192UL> ret;
    std::copy_n(std::make_move_iterator(from_device.begin()), 192UL, ret.begin());
    return ret;
}

void XC2C64A::prepare_read_sram() {
    tap.set_repeat(0);
    tap.set_end_ir(state_t::run_test_idle);
    tap.set_end_dr(state_t::run_test_idle);

    reset();
    enable();

    shift_ir(instruction_t::ISC_SRAM_READ);

    // Prime the operation with a read of an empty row.
    const jtag::tap::bits_t empty_row{block_length};

    tap.state(state_t::shift_dr);
    tap.shift(empty_row, false);
}

std::array<bool, 274> XC2C64A::read_block_sram(verify_block_t block) {
    tap.shift({&block.id, block_id_length}, true);
    tap.state(state_t::run_test_idle);

    tap.state(state_t::shift_dr);
    const jtag::tap::bits_t empty_row{block_length};
    std::vector<bool> from_device;

    tap.shift(empty_row, {block.data.data(), block_length}, {block.mask.data(), block_length}, false, &from_device);

    std::array<bool, block_length> ret;
    std::copy_n(std::make_move_iterator(from_device.begin()), block_length, ret.begin());
    return ret;
}

void XC2C64A::finalize_read_sram(block_id_t id) {
    tap.shift({&id, block_id_length}, true);
    tap.state(state_t::run_test_idle);
    tap.set_end_dr(state_t::run_test_idle);

    disable();
    bypass();

    tap.state(state_t::test_logic_reset);
}

bool XC2C64A::verify_eeprom(const verify_blocks_t& blocks) {
    tap.set_repeat(0);
    tap.set_end_ir(state_t::run_test_idle);
    tap.set_end_dr(state_t::run_test_idle);

    reset();
    bypass();
    enable();

    shift_ir(instruction_t::ISC_READ);

    const jtag::tap::bits_t empty_row{block_length};

    auto error = false;
    for (const auto& block : blocks) {
        tap.set_end_dr(state_t::pause_dr);
        tap.shift_dr({&block.id, block_id_length});
        tap.set_end_ir(state_t::run_test_idle);
        tap.wait(state_t::pause_dr, state_t::pause_dr, 20);
        tap.set_end_ir(state_t::run_test_idle);
        tap.wait(state_t::run_test_idle, state_t::run_test_idle, 100);
        error |= tap.shift_dr(empty_row, {block.data.data(), block_length}, {block.mask.data(), block_length});
        tap.wait(state_t::run_test_idle, state_t::run_test_idle, 100);
    }

    disable();
    bypass();

    tap.state(state_t::test_logic_reset);

    return !error;
}

void XC2C64A::init_from_eeprom() {
    tap.set_repeat(0);
    tap.set_end_ir(state_t::run_test_idle);
    tap.set_end_dr(state_t::run_test_idle);

    reset();
    enable();

    discharge();
    init();

    disable();
    bypass();

    tap.state(state_t::test_logic_reset);
}

void XC2C64A::prepare_read_eeprom() {
    tap.set_repeat(0);
    tap.set_end_ir(state_t::run_test_idle);
    tap.set_end_dr(state_t::run_test_idle);

    reset();
    bypass();
    enable();

    shift_ir(instruction_t::ISC_READ);
}

std::array<bool, 274> XC2C64A::read_block_eeprom(block_id_t id) {
    const jtag::tap::bits_t empty_row{block_length};

    tap.set_end_dr(state_t::pause_dr);
    tap.shift_dr({&id, block_id_length});
    tap.set_end_ir(state_t::run_test_idle);
    tap.wait(state_t::pause_dr, state_t::pause_dr, 20);
    tap.set_end_ir(state_t::run_test_idle);
    tap.wait(state_t::run_test_idle, state_t::run_test_idle, 100);

    std::vector<bool> from_device = tap.shift_dr_read(empty_row);

    tap.wait(state_t::run_test_idle, state_t::run_test_idle, 100);

    std::array<bool, block_length> ret;
    std::copy_n(std::make_move_iterator(from_device.begin()), block_length, ret.begin());
    return ret;
}

void XC2C64A::finalize_read_eeprom() {
    disable();
    bypass();

    tap.state(state_t::test_logic_reset);
}

bool XC2C64A::shift_ir(const instruction_t instruction) {
    const ir_t ir_buffer = toUType(instruction);
    const jtag::tap::bits_t bits{&ir_buffer, ir_length};
    return tap.shift_ir(bits);
}

void XC2C64A::reset() {
    tap.state(state_t::test_logic_reset);
    tap.state(state_t::run_test_idle);
}

void XC2C64A::enable() {
    shift_ir(instruction_t::ISC_ENABLE);
    tap.wait(state_t::run_test_idle, state_t::run_test_idle, 800);
}

void XC2C64A::enable_otf() {
    shift_ir(instruction_t::ISC_ENABLE_OTF);
}

void XC2C64A::discharge() {
    shift_ir(instruction_t::ISC_INIT);
    tap.wait(state_t::run_test_idle, state_t::run_test_idle, 20);
}

void XC2C64A::init() {
    tap.set_end_ir(state_t::update_ir);
    shift_ir(instruction_t::ISC_INIT);
    tap.set_end_ir(state_t::run_test_idle);
    tap.state(state_t::capture_dr);
    tap.wait(state_t::run_test_idle, state_t::run_test_idle, 800);
}

void XC2C64A::disable() {
    shift_ir(instruction_t::ISC_DISABLE);
    tap.wait(state_t::run_test_idle, state_t::run_test_idle, 100);
}

bool XC2C64A::bypass() {
    return shift_ir(instruction_t::BYPASS);
}

} /* namespace xilinx */
} /* namespace cpld */
