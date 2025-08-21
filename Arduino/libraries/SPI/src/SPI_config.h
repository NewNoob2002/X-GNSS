#ifndef __SPI_CONFIG_H
#define __SPI_CONFIG_H

#include "mcu_types.h"
#include "wk_system.h"
#include "gpio/gpio.h"
/**
 * @brief SPI peripheral configuration
 */
struct spi_peripheral_config_t
{
    /**
     * @brief SPI peripheral register base address
     */
    spi_type *register_base;

    /**
     * @brief SPI peripheral clock id
     */
    crm_periph_clock_type peripheral_clock_id;

    /**
     * @brief pin function for usart tx pin
     */
    gpio_pin_t mosi_pin;

    /**
     * @brief pin function for usart rx pin
     */
    gpio_pin_t miso_pin;
    /**
     * @brief pin function for usart clk pin
     */
    gpio_pin_t clk_pin;
		
		gpio_mux_sel_type mux_sel;
};



struct spi_config_t
{
    /**
     * @brief USART peripheral configuration
     */
    spi_peripheral_config_t peripheral;

#ifdef USART_RX_DMA_SUPPORT
    /**
     * @brief USART DMA configuration
     */
    usart_dma_config_t dma;
#endif
};


#define SPI1_ENABLE
#if defined(SPI1_ENABLE)
extern spi_config_t SD_SPI_config;
#endif

#define SPI3_ENABLE
#if defined(SPI3_ENABLE)
extern spi_config_t SPI3_config;
#endif

extern const spi_mclk_freq_div_type mclkpMap[];
extern const uint8_t mapSize;
#endif