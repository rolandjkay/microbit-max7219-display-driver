#ifndef _MICROBIT_DAL_OSX_H
#define _MICROBIT_DAL_OSX_H
#ifdef TARGET_LIKE_OSX

void wait(float seconds);

struct MicroBitDisplay
{
  void scroll(const char* str);
  void clear() {}
};

struct MicroBitButtonA
{
  bool isPressed();
};

struct MicroBitButtonB
{
  bool isPressed();
};

struct MicroBitButtonAB
{
  bool isPressed();
};

struct MicroBit
{
  void init() {}

  MicroBitDisplay display;
  MicroBitButtonA buttonA;
  MicroBitButtonB buttonB;
  MicroBitButtonAB buttonAB;
};

static inline void release_fiber() {}

#endif
#endif
