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
	
	//w25q80bv::reset_status();

	palTogglePad(2, 2);
	w25q80bv::remove_write_protection();
	w25q80bv::wait_not_busy();

	w25q80bv::erase_chip();
	w25q80bv::wait_not_busy();

	/*  -------------------  */
	//w25q80bv::remove_write_protection();
	//w25q80bv::wait_not_busy();


	// w25q80bv::power_down();

	// w25q80bv::remove_write_disable();
	// chThdSleepMilliseconds(100);

	// w25q80bv::disable_spifi();
	// w25q80bv::initialite_spi();
	// w25q80bv::setup();
	// w25q80bv::reset();
	// w25q80bv::wait_for_device();

	//w25q80bv::wait_not_busy();
	// w25q80bv::remove_write_disable();
	// w25q80bv::wait_not_busy();
	// w25q80bv::wait_for_device();
	// w25q80bv::wait_not_busy();
	// w25q80bv::remove_write_protection();
	// w25q80bv::wait_not_busy();
	
	// volatile uint8_t status = w25q80bv::get_status();
	// volatile uint8_t status2 = w25q80bv::get_status2();
	// HALT_UNTIL_DEBUGGING();

	/*  -------------------  */

	palTogglePad(2, 2);

	const TCHAR *filename = reinterpret_cast<const TCHAR *>(&shared_memory.bb_data.data[0]);

	sdcStart(&SDCD1, nullptr);

	bool_t connect = sdcConnect(&SDCD1);
	if (connect == CH_FAILED) {
		HALT_UNTIL_DEBUGGING();
	}

	FATFS fs;
	
	volatile FRESULT mount_result = f_mount(&fs, reinterpret_cast<const TCHAR*>(_T("")), 1);
	if (mount_result != FR_OK) {
		HALT_UNTIL_DEBUGGING();
	}

	FIL fil;
	FRESULT res = f_open(&fil, filename, FA_READ);
	uint8_t *data_buffer = &shared_memory.bb_data.data[0];
	size_t page_len = 256U;
	size_t num_pages = 4096U;

	if (res == FR_OK) {
		
		UINT bytes_read;
		palSetPad(2, 8);

		for (size_t page_index = 0; page_index < num_pages; page_index++){
			
			//hackrf::one::led_rx.toggle();
			if (page_index % 32 == 0)
				palTogglePad(2, 2);

			FRESULT read_status = f_read(&fil, data_buffer, page_len, &bytes_read);

			if (read_status != FR_OK) {
				HALT_UNTIL_DEBUGGING();
			}

			if (bytes_read == 0) {
				break;
			}

			if (bytes_read == page_len) {
				//chThdSleepMilliseconds(100);
				// w25q80bv::disable_spifi();
				// w25q80bv::initialite_spi();
				// w25q80bv::setup();

				// w25q80bv::wait_for_device();
				w25q80bv::wait_not_busy();
				//chThdSleepMilliseconds(100);

				w25q80bv::remove_write_protection();
				w25q80bv::wait_not_busy();
				w25q80bv::write(page_index, data_buffer, bytes_read);
				w25q80bv::wait_not_busy();

			} else if (bytes_read < page_len) {
				// w25q80bv::disable_spifi();
				// w25q80bv::initialite_spi();
				// w25q80bv::setup();

				// w25q80bv::wait_for_device();
				w25q80bv::wait_not_busy();
				w25q80bv::remove_write_protection();
				w25q80bv::wait_not_busy();
				w25q80bv::write(page_index, data_buffer, bytes_read);
				w25q80bv::wait_not_busy();
				break;
			}

		}

		palClearPad(2,2);
		palClearPad(2,8);
		HALT_UNTIL_DEBUGGING();

		FRESULT close_result = f_close(&fil);
		if (close_result != FR_OK) {
			HALT_UNTIL_DEBUGGING();
		}

	}
	else {
		HALT_UNTIL_DEBUGGING();
		f_close(&fil);	
	}


	//runtime_error(hackrf::one::led_rx);


	return 0;
}
