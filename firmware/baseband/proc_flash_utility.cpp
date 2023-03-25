#include "ch.h"
#include "hal.h"
#include "ff.h"
#include "debug.hpp"
#include "w25q80bv.hpp"

#include "portapack_shared_memory.hpp"

int main() {
	w25q80bv::disable_spifi();
	w25q80bv::initialite_spi();
	w25q80bv::setup();
	w25q80bv::remove_write_protection();
	//w25q80bv::erase_chip();

	const TCHAR *filename = reinterpret_cast<const TCHAR *>(&shared_memory.bb_data.data[0]);

	sdcStart(&SDCD1, nullptr);

	bool_t connect = sdcConnect(&SDCD1);
	if (connect == CH_FAILED) {
		HALT_UNTIL_DEBUGGING();
	}

	// while (true) {
	// 	volatile bool_t connect = sdcConnect(&SDCD1);

	// 	if (connect == CH_FAILED){
	// 		sdcDisconnect(&SDCD1);
	// 		sdcStop(&SDCD1);
	// 		//HALT_IF_DEBUGGING();
	// 		sdcStart(&SDCD1, nullptr);
	// 		continue;
	// 	}
	// 	else {
	// 		break;
	// 	}
	// }

	FATFS fs;
	
	volatile FRESULT mount_result = f_mount(&fs, reinterpret_cast<const TCHAR*>(_T("")), 1);
	if (mount_result != FR_OK) {
		HALT_UNTIL_DEBUGGING();
	}

	FIL fil;
	FRESULT res = f_open(&fil, filename, FA_READ);
	void *data_buffer = &shared_memory.bb_data.data[0];

	if (res == FR_OK) {
		
		UINT bytes_read;
		FRESULT read_status = f_read(&fil, data_buffer, 16u, &bytes_read);

		if (read_status != FR_OK) {
			HALT_UNTIL_DEBUGGING();
		}

		FRESULT close_result = f_close(&fil);
		if (close_result != FR_OK) {
			HALT_UNTIL_DEBUGGING();
		}

		HALT_UNTIL_DEBUGGING();
	}
	else {
		HALT_UNTIL_DEBUGGING();
		f_close(&fil);	
	}


	//runtime_error(hackrf::one::led_rx);


	return 0;
}
