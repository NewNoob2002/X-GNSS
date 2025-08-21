#include "HAL.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "Fonts/FreeMono12pt7b.h"

#define DISP_DMA_CHANNEL DMA1_CHANNEL3
#define DISP_DMA_MAX_SIZE 65535

Adafruit_ST7789 screen(
    &CONFIG_SCREEN_SPI,
    CONFIG_SCREEN_CS_PIN,
    CONFIG_SCREEN_DC_PIN,
    CONFIG_SCREEN_RST_PIN,
    CONFIG_SCREEN_BLK_PIN);

static uint8_t *Disp_DMA_TragetPoint = nullptr;
static uint8_t *Disp_DMA_CurrentPoint = nullptr;
static HAL::Display_CallbackFunc_t Disp_Callback = nullptr;

static void Display_SPI_DMA_Send(const void *buf, uint32_t size)
{
    if (size > DISP_DMA_MAX_SIZE)
    {
        if (Disp_DMA_TragetPoint == NULL)
        {
            Disp_DMA_TragetPoint = (uint8_t *)buf + size;
        }
        Disp_DMA_CurrentPoint = (uint8_t *)buf + DISP_DMA_MAX_SIZE;
        size = DISP_DMA_MAX_SIZE;
    }
    else
    {
        Disp_DMA_CurrentPoint = NULL;
        Disp_DMA_TragetPoint = NULL;
    }

    dma_channel_enable(DISP_DMA_CHANNEL, FALSE);
    DISP_DMA_CHANNEL->maddr = (uint32_t)buf;
    DISP_DMA_CHANNEL->dtcnt_bit.cnt = size;
    dma_channel_enable(DISP_DMA_CHANNEL, TRUE);
}

extern "C" void DMA1_Channel3_IRQHandler(void)
{
    if (dma_flag_get(DMA1_FDT3_FLAG) != RESET)
    {
        dma_flag_clear(DMA1_FDT3_FLAG);
        if (Disp_DMA_CurrentPoint < Disp_DMA_TragetPoint)
        {
            Display_SPI_DMA_Send(Disp_DMA_CurrentPoint, Disp_DMA_TragetPoint - Disp_DMA_CurrentPoint);
        }
        else
        {
            digitalWrite(CONFIG_SCREEN_CS_PIN, HIGH);

            if (Disp_Callback)
            {
                Disp_Callback();
            }
        }
    }
}

static void Display_SPI_DMA_Init()
{
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

    dmamux_enable(DMA1, TRUE);

    dma_reset(DISP_DMA_CHANNEL);
    dma_init_type dma_init_struct;
    dma_default_para_init(&dma_init_struct);

    dma_init_struct.buffer_size = DISP_DMA_MAX_SIZE;
    dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_base_addr = (uint32_t)NULL;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_base_addr = (uint32_t)&SPI3->dt;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.loop_mode_enable = FALSE;

    dma_init(DISP_DMA_CHANNEL, &dma_init_struct);

    dmamux_init(DMA1MUX_CHANNEL3, DMAMUX_DMAREQ_ID_SPI3_TX);

    spi_i2s_dma_transmitter_enable(SPI3, TRUE);

    NVIC_EnableIRQ(DMA1_Channel3_IRQn);

    dma_interrupt_enable(DISP_DMA_CHANNEL, DMA_FDT_INT, TRUE);
}

void HAL::Display_Init()
{
    Serial.print("Display: init...");
    screen.init(CONFIG_SCREEN_WIDTH, CONFIG_SCREEN_HIEGHT);
    screen.setRotation(3);
    screen.displayOff();
    screen.fillScreen(YELLOW);
    delay(150);
    screen.displayOn();

    Display_SPI_DMA_Init();

#if (DISP_USE_FPS_TEST == 1)
    HAL::Backlight_ForceLit(true);
    Display_GetFPS(&screen, 100);
    while (1)
        ;
#endif
    // HAL::Backlight_SetGradual(1000, 1000);
    Serial.println("success");
}

void HAL::Display_SetAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    screen.setAddrWindow(x0, y0, x1, y1);
}

void HAL::Display_SendPixels(const uint16_t *pixels, uint32_t len)
{
    digitalWrite(CONFIG_SCREEN_CS_PIN, LOW);
    digitalWrite(CONFIG_SCREEN_DC_PIN, HIGH);

    Display_SPI_DMA_Send(pixels, len * sizeof(uint16_t));
}

void HAL::Display_SetSendFinishCallback(Display_CallbackFunc_t func)
{
    Disp_Callback = func;
}