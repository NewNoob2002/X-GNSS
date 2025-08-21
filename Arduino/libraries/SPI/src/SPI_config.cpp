#include "SPI_config.h"

const spi_mclk_freq_div_type mclkpMap[] =
{
		SPI_MCLK_DIV_2,
		SPI_MCLK_DIV_2,
		SPI_MCLK_DIV_4,
		SPI_MCLK_DIV_8,
		SPI_MCLK_DIV_16,
		SPI_MCLK_DIV_32,
		SPI_MCLK_DIV_64,
		SPI_MCLK_DIV_128,
		SPI_MCLK_DIV_256,
		SPI_MCLK_DIV_512,
		SPI_MCLK_DIV_1024,
};
const uint8_t mapSize = sizeof(mclkpMap) / sizeof(mclkpMap[0]);
				
spi_config_t SPI3_config = {
	.peripheral = {
		.register_base = SPI3,
		.peripheral_clock_id = CRM_SPI3_PERIPH_CLOCK,
		.mosi_pin = PB5,
		.miso_pin = PIN_MAX,
		.clk_pin = PB3,
		.mux_sel = GPIO_MUX_6,
	}
};                                                                                                                      

spi_config_t SD_SPI_config = {
	.peripheral = {
		.register_base = SPI1,
		.peripheral_clock_id = CRM_SPI1_PERIPH_CLOCK,
		.mosi_pin = PA7,
		.miso_pin = PA6,
		.clk_pin = PA5,
		.mux_sel = GPIO_MUX_5,
	}
};