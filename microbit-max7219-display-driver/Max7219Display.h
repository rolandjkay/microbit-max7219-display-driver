#ifndef _MAX7291_DISPLAY_H
#define _MAX7291_DISPLAY_H
#ifdef TARGET_LIKE_OSX    // Substitute SDL implementation on OS X.
#  include "Max7219Display_SDL.h"
#else

#include <mbed.h>
#include <MicroBitPin.h>

class Max7219Display {
  const size_t* display_indices;
  size_t display_width;
  size_t display_height;
  size_t num_displays;
  SPI spi;
  uint8_t* buffer;
  DigitalOut cs;

  void set_register(uint8_t cmd, uint8_t data);
  void flush();
  void start_write();
  void end_write();

public:
  /*
   * For example, if the "ID displays" test card shows:
   *
   *   3210
   *   7654
   *
   * then display_indices=3,2,1,0,7,6,5,4 & display_width = 4.
   */
  Max7219Display(const size_t* display_indices,
                 size_t display_width,
                 size_t display_height
                );
  ~Max7219Display();

  void start_display();
  void set_brightness(uint8_t brightness);

  void set_led(size_t x, size_t y, bool value);
  void write_bitmap(const uint8_t* bitmap,
                    size_t width, size_t height,
                    size_t stride,
                    size_t x, size_t y);

  /*
   * Write an 8x8 bitmap to a single display.
   */
  void set_display(const uint8_t* image,
                   size_t image_stride,
                   size_t display_index);

  /*
   * Flush screen buffer to display.
   */
  void show();

};

#endif
#endif
