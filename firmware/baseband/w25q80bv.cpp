
#include "w25q80bv.hpp"
//#include "portapack.hpp"

//#include "rf_path.hpp"
//
//#include "rffc507x.hpp"
//#include "max2837.hpp"
//#include "max2839.hpp"
//#include "max5864.hpp"
//#include "baseband_cpld.hpp"
//
//#include "tuning.hpp"
//
//#include "spi_arbiter.hpp"
//
//#include "hackrf_hal.hpp"
//#include "hackrf_gpio.hpp"
//using namespace hackrf::one;
//
//#include "cpld_update.hpp"
//


// from hackrf/firmware/libopencm3/include/libopencm3/lpc43xx/rgu.h
#define MMIO32(addr)		(*(volatile uint32_t *)(addr))
#define RGU_BASE                        0x40053000
#define RESET_CTRL1                     MMIO32(RGU_BASE + 0x104)
#define RESET_CTRL1_SPIFI_RST_SHIFT (21)
#define RESET_CTRL1_SPIFI_RST (1 << RESET_CTRL1_SPIFI_RST_SHIFT)



//SPI ssp0(&SPID1);

// static SPIConfig spicfg = {
//   NULL,
//   GPIO1,
//   GPIO1_SPI0SEL,
//   CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0),
//   48
// };

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


#define W25Q80BV_WRITE_ENABLE 0x06
#define W25Q80BV_CHIP_ERASE   0xC7

namespace w25q80bv {
void erase() {

	// disable SPIFI
	RESET_CTRL1 = RESET_CTRL1_SPIFI_RST;

	//start SPI driver
	spiStart(&SPID1, &ssp_config_w25q80bv);

	//remove hardware write protection
	hackrf::one::gpio_w25q80bv_hold.set();
	hackrf::one::gpio_w25q80bv_wp.set();
	hackrf::one::gpio_w25q80bv_hold.output();
	hackrf::one::gpio_w25q80bv_wp.output();

 	static uint8_t write_enable_data[] = {W25Q80BV_WRITE_ENABLE};
	static uint8_t erase_data[] = {W25Q80BV_CHIP_ERASE};
    spiSelect(&SPID1);
    spiSend(&SPID1, 1, write_enable_data);
    spiSend(&SPID1, 1, erase_data);
    spiUnselect(&SPID1);
}
}



// static constexpr uint32_t ssp_scr(
// 	const float pclk_f,
// 	const uint32_t cpsr,
// 	const float spi_f
// ) {
// 	return static_cast<uint8_t>(pclk_f / cpsr / spi_f - 1);
// }



// static spi::arbiter::Arbiter ssp0_arbiter(ssp0);
// static spi::arbiter::Target ssp0_target_w25q80bv {
// 	ssp0_arbiter,
// 	ssp_config_w25q80bv
// };

