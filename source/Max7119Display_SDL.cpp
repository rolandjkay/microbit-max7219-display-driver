#ifdef TARGET_LIKE_OSX    // Include only when not building on ARM.
#include "microbit-max7219-display-driver/Max7219Display_SDL.h"


#define PIXELS_PER_LED 16

static uint16_t led_bitmap[] = {
  0b0000111111100000,
  0b0001111111110000,
  0b0011111111111000,
  0b0111111111111100,
  0b1111111111111110,
  0b1111111111111110,
  0b1111111111111110,
  0b1111111111111110,
  0b1111111111111110,
  0b1111111111111110,
  0b1111111111111110,
  0b0111111111111100,
  0b0011111111111000,
  0b0001111111110000,
  0b0000111111100000,
  0b0000000000000000,
};

Max7219Display::Max7219Display(const size_t* display_indices,
                               size_t display_width, // in number of modules
                               size_t display_height
                              )
  : window(NULL), renderer(NULL), screen(NULL), texture(NULL),
    sdl_window_width(PIXELS_PER_LED * display_width * 8),
    sdl_window_height(PIXELS_PER_LED * display_height * 8),
    num_displays(display_width * display_height),
    pixels((uint8_t*)malloc(sdl_window_width * sdl_window_height)),
    display_mapping((Coordinate*)malloc(sizeof(Coordinate) * num_displays))
{
  /*
   * Create a mapping of display indecies to positions within the bitmap.
   * For example, if the display is
   *
   *      +-----+-----+-----+-----+
   *      |  3  |  2  |  1  |  0  |
   *      +-----+-----+-----+-----+
   *      |  7  |  6  |  5  |  4  +
   *      +-----+-----+-----+-----+
   *
   * Then we need a mapping:
   *
   *      0 -> 3,0    [fourth display accress, first down]
   *      1 -> 2,0
   *      2 -> 1,0
   *      3 -> 0,0
   *      4 -> 3,1
   *      5 -> 2,1
   *      6 -> 1,1
   *      7 -> 0,1
   *
   */
   for (int i = 0; i < num_displays; ++i)
   {
     int display_index = display_indices[i];
     display_mapping[i] = Coordinate(display_index % display_width,
                                     display_index / display_width);
   }
}

Max7219Display::~Max7219Display()
{
  free(pixels);
}

void Max7219Display::start_display()
{
  if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
  {
    fprintf(stderr, "Failed to initialize SDL\n");
    return;
  }

  //memset(self->_pixels, 0, sizeof(self->_pixels));
  //memset(self->_buffer, 0, sizeof(self->_buffer));

  SDL_CreateWindowAndRenderer(sdl_window_width,
                              sdl_window_height,
                              SDL_WINDOW_SHOWN,
                              &window, &renderer);

  if (!window || !renderer)
  {
    fprintf(stderr, "Failed to create SDL window & renderer\n");
    return;
  }

  texture = SDL_CreateTexture(renderer,
                              SDL_PIXELFORMAT_RGB332,
                              SDL_TEXTUREACCESS_STATIC,
                              sdl_window_width/* SDL_SCREEN_WIDTH */,
                              sdl_window_height/*SDL_SCREEN_HEIGHT*/);
  if (!texture)
  {
    fprintf(stderr, "Failed to create SDL texture\n");
    return;
  }
}

void Max7219Display::set_brightness(uint8_t brightness)
{
  //led_colour = (0b111 & (brightness >> 1)) << 5;
  led_colour = (0b111 & (13 >> 1)) << 5;
}

void Max7219Display::set_led(size_t x, size_t y, bool value)
{
  uint8_t colour = value ? led_colour : 0b01001001;
  int start_x = x * PIXELS_PER_LED;
  int start_y = y * PIXELS_PER_LED;

  for (int offset_x = 0 ; offset_x < 16 ; ++offset_x)
  {
    for (int offset_y = 0 ; offset_y < 16 ; ++offset_y)
    {
      pixels[(start_y + offset_y) * sdl_window_width + (start_x + offset_x)] =
        (led_bitmap[offset_y] & (1 << offset_x)) ? colour : 0x00;
    }
  }
}

void Max7219Display::set_display(const uint8_t* image,
                                 size_t image_stride,
                                 size_t display_index)
{
  // Lookup position of display in our image
  int display_offset_x = display_mapping[display_index].x * 8;
  int display_offset_y = display_mapping[display_index].y * 8;

  for (int y = 0; y < 8; ++y, image += image_stride)
  {
    for (int x = 0; x < 8; ++x)
    {
      set_led(display_offset_x + x, display_offset_y + y, *image & (1<<(7-x)));
    }
  }
}

void Max7219Display::write_bitmap(const uint8_t* bitmap,
                                  size_t width, size_t height,
                                  size_t stride,
                                  size_t x, size_t y)
{
  for (int offset_y = 0; offset_y < height; ++offset_y, bitmap += stride)
  {
    for (int offset_x = 0; offset_x < width; ++offset_x)
    {
      set_led(x + offset_x, y + offset_y, bitmap[offset_x/8] & (1<<(7-(offset_x%8))));
    }
  }
}

void Max7219Display::show()
{
  SDL_UpdateTexture(texture, NULL, pixels, sdl_window_width);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}



#endif
