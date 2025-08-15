#ifndef __SPI_H
#define __SPI_H

#include "SPI_config.h"

#define SPI_I2S_GET_FLAG(spi_x, spi_i2s_flag) (spi_x->sts & spi_i2s_flag)
#define SPI_I2S_RXDATA(spi_x)                 (spi_x->dt)
#define SPI_I2S_RXDATA_VOLATILE(spi_x)         volatile uint16_t vn = SPI_I2S_RXDATA(spi_x)
#define SPI_I2S_TXDATA(spi_x, tx_data)        (spi_x->dt = tx_data)

#define SPI_I2S_WAIT_RX_FULL(spi_x)             do{ while (!SPI_I2S_GET_FLAG(spi_x, SPI_I2S_RDBF_FLAG));} while(0)
#define SPI_I2S_WAIT_TX_EMPTY(spi_x)            do{ while (!SPI_I2S_GET_FLAG(spi_x, SPI_I2S_TDBE_FLAG));} while(0)
#define SPI_I2S_WAIT_BUSY(spi_x)              	do{ while (SPI_I2S_GET_FLAG(spi_x,  SPI_I2S_BF_FLAG));}   while(0)


#define SPI_REG_TO_X(reg) \
    reg == SPI1   ? 1     \
    : reg == SPI2 ? 2     \
    : reg == SPI3 ? 3     \
                    : 0
#define SPI_DEBUG_PRINTF(fmt, ...) \
    CORE_DEBUG_PRINTF("[SPI%d] " fmt, SPI_REG_TO_X(this->spi_config->peripheral.register_base), ##__VA_ARGS__)

typedef enum
{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3
} SPI_MODE_TypeDef;

typedef enum
{
    SPI_STATE_IDLE,
    SPI_STATE_READY,
    SPI_STATE_RECEIVE,
    SPI_STATE_TRANSMIT,
    SPI_STATE_TRANSFER
} spi_mode_t;

class SPISettings
{
public:
    SPISettings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode)
    {
        init_AlwaysInline(clock, bitOrder, dataMode, SPI_FRAME_8BIT);
    }
    SPISettings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode, spi_frame_bit_num_type dataSize)
    {
        init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
    }
    SPISettings(uint32_t clock)
    {
        init_AlwaysInline(clock, MSBFIRST, SPI_MODE0, SPI_FRAME_8BIT);
    }
    SPISettings()
    {
        init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0, SPI_FRAME_8BIT);
    }
private:
    void init_MightInline(uint32_t clock, uint16_t bitOrder, uint8_t dataMode, spi_frame_bit_num_type dataSize)
    {
        init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
    }
    void init_AlwaysInline(uint32_t clock, uint16_t bitOrder, uint8_t dataMode, spi_frame_bit_num_type dataSize) __attribute__((__always_inline__))
    {
        this->clock = clock;
        this->bitOrder = bitOrder;
        this->dataMode = dataMode;
        this->dataSize = dataSize;
    }
    uint32_t clock;
    spi_frame_bit_num_type dataSize;
    uint16_t bitOrder;
    uint8_t dataMode;

    friend class SPIClass;
};

class SPIClass
{
public:
    SPIClass(spi_config_t* config);
		~SPIClass();
    spi_type* getSPI()
    {
        return spi_config->peripheral.register_base;
    }

    void SPI_Settings(
    spi_master_slave_mode_type SPI_Mode_x,
    spi_frame_bit_num_type SPI_DataSize_x,
    SPI_MODE_TypeDef SPI_MODEx,
    spi_cs_mode_type SPI_NSS_x,
    spi_mclk_freq_div_type SPI_BaudRatePrescaler_x,
    spi_first_bit_type SPI_FirstBit_x
		);
    void begin(void);
    void begin(uint32_t clock, uint16_t dataOrder, uint16_t dataMode);
    void begin(SPISettings settings);
    void beginSlave(uint32_t bitOrder, uint32_t mode);
    void beginSlave(void);
    void beginTransactionSlave(void);
    void beginTransaction(SPISettings settings);

    void endTransaction(void);
    void end(void);

    void setClock(uint32_t clock);
    void setClockDivider(uint32_t Div);
    void setBitOrder(uint16_t bitOrder);
    void setDataMode(uint8_t dataMode);
    void setDataSize(spi_frame_bit_num_type datasize);

    uint16_t read(void);
    void read(uint8_t* buffer, uint32_t length);
    void write(uint16_t data);
    void write(uint16_t data, uint32_t n);
    void write(const uint8_t* data, uint32_t length);
    void write(const uint16_t* data, uint32_t length);
    uint8_t transfer(uint8_t data) const;
		uint8_t transfer(uint8_t *buffer, size_t len) const;
    uint16_t transfer16(uint16_t data) const;
    uint8_t send(uint8_t data);
    uint8_t send(uint8_t* data, uint32_t length);
    uint8_t recv(void);

private:
		spi_config_t *spi_config;
    spi_init_type SPI_InitStructure;

		uint32_t clock_div;
    uint32_t dataSize;
    uint16_t bitOrder;
    uint8_t dataMode;

		bool initialized;
};

#if defined(SPI3_ENABLE)
extern SPIClass SPI;
// #define USART1_IOMUX_ENABLE
#endif
#endif