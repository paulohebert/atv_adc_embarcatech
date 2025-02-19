#include "ssd1306.h"

ssd1306_t ssd; // Inicializa a estrutura do display

void ssd1306_init(uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c)
{
  ssd.width = width;
  ssd.height = height;
  ssd.pages = height / 8U;
  ssd.address = address;
  ssd.i2c_port = i2c;
  ssd.bufsize = ssd.pages * ssd.width + 1;
  ssd.ram_buffer = calloc(ssd.bufsize, sizeof(uint8_t));
  ssd.ram_buffer[0] = 0x40;
  ssd.port_buffer[0] = 0x80;
}

void ssd1306_config()
{
  ssd1306_command(SET_DISP | 0x00);
  ssd1306_command(SET_MEM_ADDR);
  ssd1306_command(0x01);
  ssd1306_command(SET_DISP_START_LINE | 0x00);
  ssd1306_command(SET_SEG_REMAP | 0x01);
  ssd1306_command(SET_MUX_RATIO);
  ssd1306_command(HEIGHT - 1);
  ssd1306_command(SET_COM_OUT_DIR | 0x08);
  ssd1306_command(SET_DISP_OFFSET);
  ssd1306_command(0x00);
  ssd1306_command(SET_COM_PIN_CFG);
  ssd1306_command(0x12);
  ssd1306_command(SET_DISP_CLK_DIV);
  ssd1306_command(0x80);
  ssd1306_command(SET_PRECHARGE);
  ssd1306_command(0xF1);
  ssd1306_command(SET_VCOM_DESEL);
  ssd1306_command(0x30);
  ssd1306_command(SET_CONTRAST);
  ssd1306_command(0xFF);
  ssd1306_command(SET_ENTIRE_ON);
  ssd1306_command(SET_NORM_INV);
  ssd1306_command(SET_CHARGE_PUMP);
  ssd1306_command(0x14);
  ssd1306_command(SET_DISP | 0x01);
}

void ssd1306_command(uint8_t command)
{
  ssd.port_buffer[1] = command;
  i2c_write_blocking(
      ssd.i2c_port,
      ssd.address,
      ssd.port_buffer,
      2,
      false);
}

void ssd1306_send_data()
{
  ssd1306_command(SET_COL_ADDR);
  ssd1306_command(0);
  ssd1306_command(ssd.width - 1);
  ssd1306_command(SET_PAGE_ADDR);
  ssd1306_command(0);
  ssd1306_command(ssd.pages - 1);
  i2c_write_blocking(
      ssd.i2c_port,
      ssd.address,
      ssd.ram_buffer,
      ssd.bufsize,
      false);
}

void ssd1306_pixel(uint8_t x, uint8_t y, bool value)
{
  uint16_t index = (y >> 3) + (x << 3) + 1;
  uint8_t pixel = (y & 0b111);
  if (value)
    ssd.ram_buffer[index] |= (1 << pixel);
  else
    ssd.ram_buffer[index] &= ~(1 << pixel);
}

void ssd1306_fill(bool value)
{
  // Itera por todas as posições do display
  for (uint8_t y = 0; y < ssd.height; ++y)
  {
    for (uint8_t x = 0; x < ssd.width; ++x)
    {
      ssd1306_pixel(x, y, value);
    }
  }
}

void ssd1306_rect(uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill)
{
  for (uint8_t x = left; x < left + width; ++x)
  {
    ssd1306_pixel(x, top, value);
    ssd1306_pixel(x, top + height - 1, value);
  }
  for (uint8_t y = top; y < top + height; ++y)
  {
    ssd1306_pixel(left, y, value);
    ssd1306_pixel(left + width - 1, y, value);
  }

  if (fill)
  {
    for (uint8_t x = left + 1; x < left + width - 1; ++x)
    {
      for (uint8_t y = top + 1; y < top + height - 1; ++y)
      {
        ssd1306_pixel(x, y, value);
      }
    }
  }
}

void ssd1306_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value)
{
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);

  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;

  int err = dx - dy;

  while (true)
  {
    ssd1306_pixel(x0, y0, value); // Desenha o pixel atual

    if (x0 == x1 && y0 == y1)
      break; // Termina quando alcança o ponto final

    int e2 = err * 2;

    if (e2 > -dy)
    {
      err -= dy;
      x0 += sx;
    }

    if (e2 < dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}

void ssd1306_hline(uint8_t x0, uint8_t x1, uint8_t y, bool value)
{
  for (uint8_t x = x0; x <= x1; ++x)
    ssd1306_pixel(x, y, value);
}

void ssd1306_vline(uint8_t x, uint8_t y0, uint8_t y1, bool value)
{
  for (uint8_t y = y0; y <= y1; ++y)
    ssd1306_pixel(x, y, value);
}

void ssd1306_diagonal(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value)
{
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = (dx > dy ? dx : -dy) / 2, e2;

  while (1)
  {
    ssd1306_pixel(x0, y0, value);
    if (x0 == x1 && y0 == y1)
      break;
    e2 = err;
    if (e2 > -dx)
    {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy)
    {
      err += dx;
      y0 += sy;
    }
  }
}

void draw_border(uint8_t n, bool value)
{
    switch (n)
    {
    case 0:
        /* Sem Borda */
        if (value)
            draw_border(3, false);
        break;
    case 1:
        ssd1306_rect(0, 0, 8 * 5, 2, value, false);
        ssd1306_rect(0, 88, 8 * 5, 2, value, false);

        ssd1306_rect(62, 0, 8 * 5, 2, value, false);
        ssd1306_rect(62, 88, 8 * 5, 2, value, false);

        ssd1306_rect(8 * 2, 0, 2, 8 * 4, value, false);

        ssd1306_rect(8 * 2, 126, 2, 8 * 4, value, false);
        break;
    case 2:
        ssd1306_rect(0, 0, 128, 64, value, false);
        ssd1306_rect(3, 3, 122, 58, value, false);
        break;
    case 3:
        ssd1306_diagonal(0, 8, 8, 0, value);
        ssd1306_diagonal(128, 9, 119, 0, value);
        ssd1306_diagonal(0, 55, 8, 63, value);
        ssd1306_diagonal(128 - 8 - 1, 64 - 1, 128 - 1, 64 - 8 - 1, value);
        ssd1306_vline(0, 8, 64 - 9, value);
        ssd1306_vline(127, 8, 64 - 9, value);
        ssd1306_hline(8, 128 - 9, 0, value);
        ssd1306_hline(8, 128 - 9, 64 - 1, value);
        break;
    }
}
