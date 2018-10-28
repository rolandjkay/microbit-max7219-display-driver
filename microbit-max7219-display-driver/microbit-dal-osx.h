#ifndef _MICROBIT_DAL_OSX_H
#define _MICROBIT_DAL_OSX_H
#ifdef TARGET_LIKE_OSX

void wait(float seconds);

struct MicroBitDisplay
{
  void scroll(const char* str);
  void clear() {}
};

struct MicroBitButton
{
  bool _is_pressed;
  bool isPressed() { return _is_pressed; }
};

struct MicroBit;

struct MicroBitVirtualButton
{
  MicroBit& _ubit;
  MicroBitVirtualButton(MicroBit& ubit) :_ubit(ubit) {}

  bool isPressed();
};

struct MicroBit
{
  MicroBit() : buttonAB(*this){}

  void init() {}

  MicroBitDisplay display;
  MicroBitButton buttonA;
  MicroBitButton buttonB;
  MicroBitVirtualButton buttonAB;
};

#endif
#endif
