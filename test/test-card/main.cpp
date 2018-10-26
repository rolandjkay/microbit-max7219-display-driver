#ifdef TARGET_LIKE_ARM
#  include "MicroBit.h"
#  include "../source/Max7219Display.h"
#else
#  include "../microbit-max7219-display-driver/Max7219Display_SDL.h"
#  include "../microbit-max7219-display-driver/microbit-dal-osx.h"

void release_fiber() {};
#endif

#include "./bitmaps.h"

static const size_t display_indices[] = {3,2,1,0,
                                         7,6,5,4};
static Max7219Display display(display_indices, 4, 2);

static void id_displays()
{
  for (int display_index = 0; display_index < 8; ++display_index)
  {
    display.set_display(&digits_8x8[display_index],
                       /*stride=*/10,
                       display_index);
  }
}

int main()
{
  display.start_display();
  display.set_brightness(7);

  while (1)
  {
    /*id_displays();
    display.show();
    wait(1.0); */
    id_displays();
    display.show();
    wait(1);
    display.write_bitmap(one_16x16, 16, 16, 2, 0, 0);
    display.write_bitmap(pingo_16x16, 16, 16, 2, 16, 0);
    display.show();
    wait(1);
    display.write_bitmap(heart_16x16, 16, 16, 2, 0, 0);
    display.write_bitmap(heart_16x16, 16, 16, 2, 16, 0);
    display.show();
    wait(1);
    display.write_bitmap(girl_16x16, 16, 16, 2, 0, 0);
    display.write_bitmap(boy_16x16, 16, 16, 2, 16, 0);
    display.show();
    wait(1); 
  }

  // If main exits, there may still be other fibers running or registered event handlers etc.
  // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
  // sit in the idle task forever, in a power efficient sleep.
  release_fiber();
}
