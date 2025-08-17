#include "Arduino.h"

#if defined(SPI1_ENABLE)
SPIClass SPI_SD(&SD_SPI_config);
#endif

#if defined(SPI3_ENABLE)
#define SPI_CLOCK (F_CPU / 2)
SPIClass SPI(&SPI3_config);
#endif

SPIClass::SPIClass(spi_config_t *config)
    : spi_config(config), clock_div(0), dataSize(0), bitOrder(0), dataMode(0), initialized(false)
{
    memset(&this->SPI_InitStructure, 0, sizeof(this->SPI_InitStructure));
}

SPIClass::~SPIClass()
{
}

void SPIClass::SPI_Settings(
    spi_master_slave_mode_type SPI_Mode_x,
    spi_frame_bit_num_type SPI_DataSize_x,
    SPI_MODE_TypeDef SPI_MODEx,
    spi_cs_mode_type SPI_NSS_x,
    spi_mclk_freq_div_type SPI_BaudRatePrescaler_x,
    spi_first_bit_type SPI_FirstBit_x)
{
    crm_periph_clock_enable(this->spi_config->peripheral.peripheral_clock_id, TRUE);
		gpio_pin_mux_config(digitalPinToPort(this->spi_config->peripheral.mosi_pin), 
											 (gpio_pins_source_type)digitalPinToBitPos(this->spi_config->peripheral.mosi_pin),
												this->spi_config->peripheral.mux_sel);
		gpio_pin_mux_config(digitalPinToPort(this->spi_config->peripheral.clk_pin), 
											 (gpio_pins_source_type)digitalPinToBitPos(this->spi_config->peripheral.clk_pin),
												this->spi_config->peripheral.mux_sel);
		
    spi_clock_polarity_type SPI_CPOL_x;
    spi_clock_phase_type SPI_CPHA_x;

    switch (SPI_MODEx)
    {
    case SPI_MODE0:
        SPI_CPOL_x = SPI_CLOCK_POLARITY_LOW;
        SPI_CPHA_x = SPI_CLOCK_PHASE_1EDGE;
        break;
    case SPI_MODE1:
        SPI_CPOL_x = SPI_CLOCK_POLARITY_LOW;
        SPI_CPHA_x = SPI_CLOCK_PHASE_2EDGE;
        break;
    case SPI_MODE2:
        SPI_CPOL_x = SPI_CLOCK_POLARITY_HIGH;
        SPI_CPHA_x = SPI_CLOCK_PHASE_1EDGE;
        break;
    case SPI_MODE3:
        SPI_CPOL_x = SPI_CLOCK_POLARITY_HIGH;
        SPI_CPHA_x = SPI_CLOCK_PHASE_2EDGE;
        break;
    default:
        return;
    }

    spi_default_para_init(&SPI_InitStructure);
    SPI_InitStructure.transmission_mode = SPI_TRANSMIT_HALF_DUPLEX_TX;
    SPI_InitStructure.master_slave_mode = SPI_Mode_x;
    SPI_InitStructure.mclk_freq_division = SPI_BaudRatePrescaler_x;
    SPI_InitStructure.first_bit_transmission = SPI_FirstBit_x;
    SPI_InitStructure.frame_bit_num = SPI_DataSize_x;
    SPI_InitStructure.clock_polarity = SPI_CPOL_x;
    SPI_InitStructure.clock_phase = SPI_CPHA_x;
    SPI_InitStructure.cs_mode_selection = SPI_NSS_x;
}

void SPIClass::begin()
{
    //	if(this->SPI_InitStructure.transmission_mode == SPI_TRANSMIT_HALF_DUPLEX_TX &&
    //		 this->SPI_InitStructure.master_slave_mode == SPI_MODE_MASTER)
    //	{
		if(this->spi_config->peripheral.miso_pin != PIN_MAX)
		{
			pinMode(this->spi_config->peripheral.miso_pin, OUTPUT_AF_PP);
		}
    pinMode(this->spi_config->peripheral.mosi_pin, OUTPUT_AF_PP);
    pinMode(this->spi_config->peripheral.clk_pin, OUTPUT_AF_PP);
    //	}
    //	else
    //	{
    //		pinMode(this->spi_config->peripheral.mosi_pin, OUTPUT_AF_PP);
    //		pinMode(this->spi_config->peripheral.miso_pin, OUTPUT_AF_PP);
    //		pinMode(this->spi_config->peripheral.clk_pin,  OUTPUT_AF_PP_DOWN);
    //	}
    SPI_Settings(SPI_MODE_MASTER,
                 SPI_FRAME_8BIT,
                 SPI_MODE0,
                 SPI_CS_SOFTWARE_MODE,
                 SPI_MCLK_DIV_4,
                 SPI_FIRST_BIT_MSB);
    spi_init(this->spi_config->peripheral.register_base, &this->SPI_InitStructure);

    spi_enable(this->spi_config->peripheral.register_base, TRUE);
    this->initialized = true;
}

void SPIClass::begin(uint32_t clock, uint16_t dataOrder, uint16_t dataMode)
{
    begin();
    spi_enable(this->spi_config->peripheral.register_base, FALSE);
    setClock(clock);
    setBitOrder(dataOrder);
    setDataMode(dataMode);
    spi_init(this->spi_config->peripheral.register_base, &this->SPI_InitStructure);

    spi_enable(this->spi_config->peripheral.register_base, TRUE);
}

void SPIClass::begin(SPISettings settings)
{
    begin();
    setClock(settings.clock);
    setBitOrder(settings.bitOrder);
    setDataMode(settings.dataMode);
    // SPI_Enable(SPIx, ENABLE);
}

void SPIClass::end(void)
{
    spi_enable(this->spi_config->peripheral.register_base, FALSE);
}

void SPIClass::setClock(uint32_t clock)
{
    if (clock == 0)
    {
        return;
    }

    static const spi_mclk_freq_div_type mclkpMap[] =
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
    uint32_t clockDiv = SPI_CLOCK / clock;
    uint8_t mapIndex = 0;

    while (clockDiv > 1)
    {
        clockDiv = clockDiv >> 1;
        mapIndex++;
    }

    if (mapIndex >= mapSize)
    {
        mapIndex = mapSize - 1;
    }

    spi_mclk_freq_div_type mclkp = mclkpMap[mapIndex];

    SPI_InitStructure.mclk_freq_division = mclkp;
}

void SPIClass::setClockDivider(uint32_t Div)
{
    if (Div == 0)
    {
        Div = 1;
    }
#if SPI_CLASS_AVR_COMPATIBILITY_MODE
    setClock(16000000 / Div); // AVR:16MHz
#else
    setClock(SPI_CLOCK / Div);
#endif
}

void SPIClass::setBitOrder(uint16_t bitOrder)
{
    SPI_InitStructure.first_bit_transmission = (bitOrder == MSBFIRST) ? SPI_FIRST_BIT_MSB : SPI_FIRST_BIT_LSB;
    //    SPI_Init(SPIx, &SPI_InitStructure);
    //    SPI_Enable(SPIx, ENABLE);
}

/*  Victor Perez. Added to test changing datasize from 8 to 16 bit modes on the fly.
 *   Input parameter should be SPI_CR1_DFF set to 0 or 1 on a 32bit word.
 *
 */
void SPIClass::setDataSize(spi_frame_bit_num_type datasize)
{

    SPI_InitStructure.frame_bit_num = datasize;
    // SPI_Init(SPIx, &SPI_InitStructure);
    // SPI_Enable(SPIx, ENABLE);
}

void SPIClass::setDataMode(uint8_t dataMode)
{
    /* Notes.  As far as I can tell, the AVR numbers for dataMode appear to match the numbers required by the STM32

    From the AVR doc http://www.atmel.com/images/doc2585.pdf section 2.4

    SPI Mode    CPOL    CPHA    Shift SCK-edge  Capture SCK-edge
    0           0       0       Falling         Rising
    1           0       1       Rising          Falling
    2           1       0       Rising          Falling
    3           1       1       Falling         Rising


    On the STM32 it appears to be

    bit 1 - CPOL : Clock polarity
        (This bit should not be changed when communication is ongoing)
        0 : CLK to 0 when idle
        1 : CLK to 1 when idle

    bit 0 - CPHA : Clock phase
        (This bit should not be changed when communication is ongoing)
        0 : The first clock transition is the first data capture edge
        1 : The second clock transition is the first data capture edge

    If someone finds this is not the case or sees a logic error with this let me know ;-)
     */
    spi_enable(this->spi_config->peripheral.register_base, FALSE);
    spi_clock_polarity_type SPI_CPOL_x;
    spi_clock_phase_type SPI_CPHA_x;

    switch (dataMode)
    {
    case SPI_MODE0:
        SPI_CPOL_x = SPI_CLOCK_POLARITY_LOW;
        SPI_CPHA_x = SPI_CLOCK_PHASE_1EDGE;
        break;
    case SPI_MODE1:
        SPI_CPOL_x = SPI_CLOCK_POLARITY_LOW;
        SPI_CPHA_x = SPI_CLOCK_PHASE_2EDGE;
        break;
    case SPI_MODE2:
        SPI_CPOL_x = SPI_CLOCK_POLARITY_HIGH;
        SPI_CPHA_x = SPI_CLOCK_PHASE_1EDGE;
        break;
    case SPI_MODE3:
        SPI_CPOL_x = SPI_CLOCK_POLARITY_HIGH;
        SPI_CPHA_x = SPI_CLOCK_PHASE_2EDGE;
        break;
    default:
        return;
    }

    SPI_InitStructure.clock_polarity = SPI_CPOL_x;
    SPI_InitStructure.clock_phase = SPI_CPHA_x;
    spi_init(this->spi_config->peripheral.register_base, &this->SPI_InitStructure);

    spi_enable(this->spi_config->peripheral.register_base, TRUE);
}

void SPIClass::beginTransaction(SPISettings settings)
{
    SPISettings(settings.clock, settings.bitOrder, settings.dataMode);

    setClock(settings.clock);
    setBitOrder(settings.bitOrder);
    setDataMode(settings.dataMode);
    setDataSize(settings.dataSize);

    spi_enable(this->spi_config->peripheral.register_base, TRUE);
}

// void SPIClass::beginTransactionSlave(void)
//{
//     beginSlave();
// }

void SPIClass::endTransaction(void)
{
    spi_enable(this->spi_config->peripheral.register_base, FALSE);
}

uint16_t SPIClass::read(void)
{
    SPI_I2S_WAIT_RX_FULL(this->spi_config->peripheral.register_base);
    return (uint16_t)(SPI_I2S_RXDATA(this->spi_config->peripheral.register_base));
}

void SPIClass::read(uint8_t *buf, uint32_t len)
{
    if (len == 0)
        return;

    SPI_I2S_RXDATA_VOLATILE(this->spi_config->peripheral.register_base);
    SPI_I2S_TXDATA(this->spi_config->peripheral.register_base, 0x00FF);

    while ((--len))
    {
        SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base);
        noInterrupts();
        SPI_I2S_TXDATA(this->spi_config->peripheral.register_base, 0x00FF);
        SPI_I2S_WAIT_RX_FULL(this->spi_config->peripheral.register_base);
        *buf++ = (uint8_t)SPI_I2S_RXDATA(this->spi_config->peripheral.register_base);
        interrupts();
    }
    SPI_I2S_WAIT_RX_FULL(this->spi_config->peripheral.register_base);
    *buf++ = (uint8_t)SPI_I2S_RXDATA(this->spi_config->peripheral.register_base);
}

void SPIClass::write(uint16_t data)
{
    SPI_I2S_TXDATA(this->spi_config->peripheral.register_base, data);
    SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base);
    SPI_I2S_WAIT_BUSY(this->spi_config->peripheral.register_base);
}

void SPIClass::write(uint16_t data, uint32_t n)
{
    while ((n--) > 0)
    {
        SPI_I2S_TXDATA(this->spi_config->peripheral.register_base, data);  // write the data to be transmitted into the SPI_DR register (this clears the TXE flag)
        SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base); // wait till Tx empty
    }

    SPI_I2S_WAIT_BUSY(this->spi_config->peripheral.register_base); // wait until BSY=0 before returning
}

void SPIClass::write(const uint8_t *data, uint32_t length)
{
    while (length--)
    {
        SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base);
        SPI_I2S_TXDATA(this->spi_config->peripheral.register_base, *data++);
    }
    SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base);
    SPI_I2S_WAIT_BUSY(this->spi_config->peripheral.register_base);
}

void SPIClass::write(const uint16_t *data, uint32_t length)
{
    while (length--)
    {
        SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base);
        SPI_I2S_TXDATA(this->spi_config->peripheral.register_base, *data++);
    }
    SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base);
    SPI_I2S_WAIT_BUSY(this->spi_config->peripheral.register_base);
}

uint8_t SPIClass::transfer(uint8_t wr_data) const
{
    SPI_I2S_RXDATA_VOLATILE(this->spi_config->peripheral.register_base);
    SPI_I2S_TXDATA(this->spi_config->peripheral.register_base, wr_data);
    SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base);
    SPI_I2S_WAIT_BUSY(this->spi_config->peripheral.register_base);
    return (uint8_t)SPI_I2S_RXDATA(this->spi_config->peripheral.register_base);
}

uint8_t SPIClass::transfer(uint8_t *buffer, size_t len) const
{
    return len;
}

uint16_t SPIClass::transfer16(uint16_t wr_data) const
{
    SPI_I2S_RXDATA_VOLATILE(this->spi_config->peripheral.register_base);
    SPI_I2S_TXDATA(this->spi_config->peripheral.register_base, wr_data);
    SPI_I2S_WAIT_TX_EMPTY(this->spi_config->peripheral.register_base);
    SPI_I2S_WAIT_BUSY(this->spi_config->peripheral.register_base);
    return (uint16_t)SPI_I2S_RXDATA(this->spi_config->peripheral.register_base);
}

uint8_t SPIClass::send(uint8_t data)
{
    this->write(data);
    return 1;
}

uint8_t SPIClass::send(uint8_t *buf, uint32_t len)
{
    this->write(buf, len);
    return len;
}

uint8_t SPIClass::recv(void)
{
    return this->read();
}