
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





//SPI ssp0(&SPID1);

// static SPIConfig spicfg = {
//   NULL,
//   GPIO1,
//   GPIO1_SPI0SEL,
//   CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0),
//   48
// };



#define W25Q80BV_WRITE_ENABLE 0x06
#define W25Q80BV_CHIP_ERASE   0xC7
#define W25Q80BV_DEVICE_ID    0xAB

namespace w25q80bv {

void remove_write_protection() {
	//remove hardware write protection
	hackrf::one::gpio_w25q80bv_hold.set();
	hackrf::one::gpio_w25q80bv_wp.set();
	hackrf::one::gpio_w25q80bv_hold.output();
	hackrf::one::gpio_w25q80bv_wp.output();
}

uint8_t get_device_id(){
	uint8_t data[] = {W25Q80BV_DEVICE_ID};
    spiSelect(&SPID1);
    spiSend(&SPID1, 1, data);
	spiReceive(&SPID1, 1, data);
    spiUnselect(&SPID1);
	return data[0];
}

void erase_chip() {



 	static uint8_t write_enable_data[] = {W25Q80BV_WRITE_ENABLE};
	static uint8_t erase_data[] = {W25Q80BV_CHIP_ERASE};
	uint8_t data[] = {W25Q80BV_DEVICE_ID, 0xFF, 0xFF, 0xFF, 0xFF};

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

