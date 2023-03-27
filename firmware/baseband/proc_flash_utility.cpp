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

#include "ch.h"
#include "hal.h"
#include "ff.h"

#include "w25q80bv.hpp"
#include "debug.hpp"
#include "portapack_shared_memory.hpp"

int main() {
	w25q80bv::disable_spifi();
	w25q80bv::initialite_spi();
	w25q80bv::setup();

	w25q80bv::wait_for_device();
	w25q80bv::wait_not_busy();
	
	palTogglePad(LED_PORT, LEDRX_PAD);
	w25q80bv::remove_write_protection();
	w25q80bv::wait_not_busy();

	w25q80bv::erase_chip();
	w25q80bv::wait_not_busy();

	palTogglePad(LED_PORT, LEDRX_PAD);

	const TCHAR *filename = reinterpret_cast<const TCHAR *>(&shared_memory.bb_data.data[0]);

	sdcStart(&SDCD1, nullptr);

	bool_t connect = sdcConnect(&SDCD1);
	if (connect == CH_FAILED) {
		chDbgPanic("no sd card #1");
	}

	FATFS fs;
	
	volatile FRESULT mount_result = f_mount(&fs, reinterpret_cast<const TCHAR*>(_T("")), 1);
	if (mount_result != FR_OK) {
		chDbgPanic("no sd card #2");
	}

	FIL fil;
	FRESULT res = f_open(&fil, filename, FA_READ);
	if (res != FR_OK) {
		chDbgPanic("no file");
	}

	uint8_t *data_buffer = &shared_memory.bb_data.data[0];
	size_t page_len = 256U;
	size_t num_pages = 4096U;
	UINT bytes_read;
	palSetPad(LED_PORT, LEDTX_PAD);

	for (size_t page_index = 0; page_index < num_pages; page_index++){
		
		if (page_index % 32 == 0)
			palTogglePad(LED_PORT, LEDRX_PAD);

		FRESULT read_status = f_read(&fil, data_buffer, page_len, &bytes_read);

		if (read_status != FR_OK) {
			chDbgPanic("no data");
		}

		if (bytes_read == 0) {
			break;
		}

		if (bytes_read == page_len) {
			w25q80bv::wait_not_busy();
			w25q80bv::remove_write_protection();
			w25q80bv::wait_not_busy();
			w25q80bv::write(page_index, data_buffer, bytes_read);
			w25q80bv::wait_not_busy();

		} else if (bytes_read < page_len) {
			w25q80bv::wait_not_busy();
			w25q80bv::remove_write_protection();
			w25q80bv::wait_not_busy();
			w25q80bv::write(page_index, data_buffer, bytes_read);
			w25q80bv::wait_not_busy();
			break;
		}
	}

	palClearPad(LED_PORT, LEDRX_PAD);
	palClearPad(LED_PORT, LEDTX_PAD);

	f_close(&fil);

	while(1) {
		__WFE();
	}

	return 0;
}
