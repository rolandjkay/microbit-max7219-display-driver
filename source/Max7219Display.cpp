#ifdef TARGET_LIKE_ARM    // Exclude when not building on ARM.

#include "microbit-max7219-display-driver/Max7219Display.h"
#include <mbed.h>

// MAX7219 Commands
#define NOOP 0
#define ROW0 1
#define ROW1 2
#define ROW2 3
#define ROW3 4
#define ROW4 5
#define ROW5 6
#define ROW6 7
#define ROW7 8
#define DECODEMODE 9
#define INTENSITY 10
#define SCANLIMIT 11
#define SHUTDOWN 12
#define DISPLAYTEST 15

void Max7219Display::set_register(uint8_t cmd, uint8_t data)
{
  spi.write(cmd);
  spi.write(data);
}

void Max7219Display::flush()
{
  for (size_t i = 0; i < this->num_displays; ++i)
  {
    set_register(NOOP, 0);
  }
}

void Max7219Display::start_write()
{
  cs = 0;
}

void Max7219Display::end_write()
{
  cs = 1;
}

/*
 * Public interface
 */

Max7219Display::Max7219Display(const size_t* display_indices,
                               size_t display_width,
                               size_t display_height)
: display_indices(display_indices),
  display_width(display_width),
  display_height(display_height),
  num_displays(display_width * display_height),
  spi(/*MOSI=*/ MOSI, /*MISO=*/ MISO, /*SCLK=*/ SCK),
  cs(MICROBIT_PIN_P12)
{
  cs = 1;
  buffer = (uint8_t*)malloc(num_displays * 8);
  memset(buffer, 0, num_displays * 8);
}

Max7219Display::~Max7219Display()
{
  free(buffer);
}


void Max7219Display::start_display()
{
  spi.frequency(1000000);
  spi.format(8,0);

  start_write();
  for (int i = 0; i < 8; ++i) {
    set_register(SHUTDOWN, 0);
  }
  end_write();

  start_write();
  for (int i = 0; i < 8; ++i) {
    set_register(DISPLAYTEST, 0);
  }
  end_write();

  start_write();
  for (int i = 0; i < 8; ++i) {
    set_register(SCANLIMIT, 7);
  }
  end_write();

  start_write();
  for (int i = 0; i < 8; ++i) {
    set_register(DECODEMODE, 0);
  }
  end_write();

  start_write();
  for (int i = 0; i < 8; ++i) {
    set_register(SHUTDOWN, 1);
  }
  end_write();
}

void Max7219Display::set_brightness(uint8_t brightness)
{
  if (brightness > 15)
  {
      return;
  }

  start_write();
  for (int display_index = 0; display_index < 8; ++display_index)
  {
    set_register(INTENSITY, brightness);
  }
  end_write();
}

void Max7219Display::show()
{
  for (int row_index = 0; row_index < 8; ++row_index)
  {
    start_write();
    for (int display_index = num_displays - 1; display_index >= 0; --display_index)
    {
      set_register(ROW0 + row_index, buffer[display_index * 8 + row_index]);
    }
    end_write();
  }

}

void Max7219Display::set_led(size_t x, size_t y, bool value)
{
  int bit_num = x % 8;

  if (value)
  {
    buffer[(x / 8 + (y / 8) * 4) * 8 + (y%8)] |= (1 << bit_num);
  }
  else
  {
    buffer[(x / 8 + (y / 8) * 4) * 8 + (y%8)] &= ~(1 << bit_num);
  }
}

void Max7219Display::write_bitmap(const uint8_t* bitmap,
                                  size_t width, size_t height,
                                  size_t stride,
                                  size_t x, size_t y)
{
  size_t start_display_x = x / 8;
  size_t start_display_y = y / 8;

  for (size_t display_offset_y = 0;
       display_offset_y < (height/8);
       ++display_offset_y )
  {
    size_t display_y = start_display_y + display_offset_y;

    for (size_t display_offset_x = 0;
         display_offset_x < (width/8);
         ++display_offset_x)
    {
      size_t display_x = start_display_x + display_offset_x;

      set_display(bitmap + display_offset_x + display_offset_y*stride*8,
                  stride,
                  display_x + display_width * display_y);
    }

  }
}

void Max7219Display::set_display(const uint8_t* image,
                                 size_t image_stride,
                                 size_t display_index)
{
  size_t physical_display_index = display_indices[display_index];

  for (int row_index = 0; row_index < 8; ++row_index)
  {
    buffer[physical_display_index * 8 + row_index] = image[row_index * image_stride];
  }
}

#endif
