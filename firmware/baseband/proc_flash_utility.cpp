#include "ch.h"
#include "hal.h"
// typedef uint8_t ioportid_t;
// typedef uint8_t iopadid_t;

#include "w25q80bv.hpp"



int main() {
	w25q80bv::disable_spifi();
	w25q80bv::initialite_spi();
	w25q80bv::setup();
	w25q80bv::remove_write_protection();
	w25q80bv::erase_chip();

	//while (((*(volatile uint32_t *)0xE000EDF0) & (1 << 0)) == 0) {	}

	//HALT_IF_DEBUGGING();

	//runtime_error(hackrf::one::led_rx);


	return 0;
}
