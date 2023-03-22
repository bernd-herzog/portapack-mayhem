#include "ch.h"
#include "hal.h"
#include "hackrf_gpio.hpp"

#include "w25q80bv.hpp"

// from hackrf/firmware/libopencm3/include/libopencm3/lpc43xx/rgu.h
#define MMIO32(addr)		(*(volatile uint32_t *)(addr))
#define RGU_BASE                        0x40053000
#define RESET_CTRL1 MMIO32(RGU_BASE + 0x104)
#define RESET_CTRL1_SPIFI_RST_SHIFT (21)
#define RESET_CTRL1_SPIFI_RST (1 << RESET_CTRL1_SPIFI_RST_SHIFT)



static constexpr uint32_t ssp0_cpsr = 2;
//constexpr ClockFrequency ssp0_pclk_f = base_m4_clk_f;
//constexpr ClockFrequency w25q80bv_spi_f = 20000000U;

static constexpr SPIConfig ssp_config_w25q80bv = {
	.end_cb = NULL,
	.ssport = hackrf::one::gpio_w25q80bv_select.port(),
	.sspad = hackrf::one::gpio_w25q80bv_select.pad(),
	.cr0 =
		  CR0_CLOCKRATE(ssp0_cpsr)
		| CR0_DSS8BIT
		,
	.cpsr = ssp0_cpsr,
};


void runtime_error(LED led) {
    led.off();

    while(true) {
        volatile size_t n = 1000000U;
        while(n--);
        led.toggle();
    }
}

#define HALT_IF_DEBUGGING()                              \
  do {                                                   \
    if ((*(volatile uint32_t *)0xE000EDF0) & (1 << 0)) { \
      __asm("bkpt 1");                                   \
    }                                                    \
} while (0)

int main() {

	// disable SPIFI
	RESET_CTRL1 = RESET_CTRL1_SPIFI_RST;

	hackrf::one::gpio_w25q80bv_select.set();
	hackrf::one::gpio_w25q80bv_select.output();

	//

	//while (((*(volatile uint32_t *)0xE000EDF0) & (1 << 0)) == 0) {	}

	//HALT_IF_DEBUGGING();

	spiObjectInit(&SPID1);
	//start SPI driver
	spiStart(&SPID1, &ssp_config_w25q80bv);

	runtime_error(hackrf::one::led_rx);

	uint8_t device_id = w25q80bv::get_device_id();

	w25q80bv::remove_write_protection();
	w25q80bv::erase_chip();
	return 0;
}
