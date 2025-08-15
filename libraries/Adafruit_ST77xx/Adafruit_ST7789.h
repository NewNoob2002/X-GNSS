#ifndef _ADAFRUIT_ST7789H_
#define _ADAFRUIT_ST7789H_

#include "Adafruit_ST77xx.h"

#define rgb565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

#define RED    rgb565(255,   0,   0) // 0xf800
#define GREEN  rgb565(  0, 255,   0) // 0x07e0
#define BLUE   rgb565(  0,   0, 255) // 0x001f
#define BLACK  rgb565(  0,   0,   0) // 0x0000
#define WHITE  rgb565(255, 255, 255) // 0xffff
#define GRAY   rgb565(128, 128, 128) // 0x8410
#define YELLOW rgb565(255, 255,   0) // 0xFFE0
#define CYAN   rgb565(  0, 156, 209) // 0x04FA
#define PURPLE rgb565(128,   0, 128) // 0x8010
/// Subclass of ST77XX type display for ST7789 TFT Driver
class Adafruit_ST7789 : public Adafruit_ST77xx {
public:
  Adafruit_ST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk,
                  int8_t rst = -1);
  Adafruit_ST7789(int8_t cs, int8_t dc, int8_t rst);
#if !defined(ESP8266)
  Adafruit_ST7789(SPIClass *spiClass, int8_t cs, int8_t dc, int8_t rst);
#endif // end !ESP8266

  void setRotation(uint8_t m);
  void init(uint16_t width, uint16_t height, uint8_t spiMode = SPI_MODE0);

protected:
  uint8_t _colstart2 = 0, ///< Offset from the right
      _rowstart2 = 0;     ///< Offset from the bottom

private:
  uint16_t windowWidth;
  uint16_t windowHeight;
};

#endif // _ADAFRUIT_ST7789H_
