#ifndef SSD1306_CONTROL_H
#define SSD1306_CONTROL_H

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDRESS 0x3C

#define WIDTH 128
#define HEIGHT 64

typedef enum
{
  SET_CONTRAST = 0x81,
  SET_ENTIRE_ON = 0xA4,
  SET_NORM_INV = 0xA6,
  SET_DISP = 0xAE,
  SET_MEM_ADDR = 0x20,
  SET_COL_ADDR = 0x21,
  SET_PAGE_ADDR = 0x22,
  SET_DISP_START_LINE = 0x40,
  SET_SEG_REMAP = 0xA0,
  SET_MUX_RATIO = 0xA8,
  SET_COM_OUT_DIR = 0xC0,
  SET_DISP_OFFSET = 0xD3,
  SET_COM_PIN_CFG = 0xDA,
  SET_DISP_CLK_DIV = 0xD5,
  SET_PRECHARGE = 0xD9,
  SET_VCOM_DESEL = 0xDB,
  SET_CHARGE_PUMP = 0x8D
} ssd1306_command_t;

typedef struct
{
  uint8_t width, height, pages, address;
  i2c_inst_t *i2c_port;
  bool external_vcc;
  uint8_t *ram_buffer;
  size_t bufsize;
  uint8_t port_buffer[2];
} ssd1306_t;

extern ssd1306_t ssd;

void ssd1306_init(uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
void ssd1306_config();
void ssd1306_command(uint8_t command);
void ssd1306_send_data();

void ssd1306_pixel(uint8_t x, uint8_t y, bool value);
void ssd1306_fill(bool value);
void ssd1306_rect(uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill);
void ssd1306_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value);
void ssd1306_hline(uint8_t x0, uint8_t x1, uint8_t y, bool value);
void ssd1306_vline(uint8_t x, uint8_t y0, uint8_t y1, bool value);
void ssd1306_diagonal(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value);
void draw_border(uint8_t n, bool value);

#endif // SSD1306_CONTROL_H