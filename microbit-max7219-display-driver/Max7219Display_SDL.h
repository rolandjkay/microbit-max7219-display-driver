#ifndef _MAX7291_DISPLAY_SDL_H
#define _MAX7291_DISPLAY_SDL_H
#ifdef TARGET_LIKE_OSX    // Include only when not building on ARM.

#include <stddef.h>
#include <stdint.h>
#include <SDL2/SDL.h>

class Max7219Display
{
  struct Coordinate
  {
    Coordinate(uint8_t _x, uint8_t _y) : x(_x), y(_y) {}
    uint8_t x, y;
  };

  const size_t* display_indices;
  size_t display_width;
  size_t display_height;
  size_t num_displays;
  Coordinate* display_mapping;

  size_t sdl_window_width;
  size_t sdl_window_height;

  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Surface* screen;
  SDL_Texture * texture;
  uint8_t *pixels;

  // RGB value used to draw the LEDs
  uint8_t led_colour;

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
