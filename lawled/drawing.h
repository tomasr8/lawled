#include "SH1106Wire.h"

#define WIDTH 128
#define HEIGHT 64

namespace lawled {
  void drawHorizontalLine(int y, SH1106Wire &display);
  void drawVerticalScroll(int size, int position, SH1106Wire &display);
  void drawUnknown(uint16_t x, uint16_t y, SH1106Wire &display);
  void drawLoading(int percentage, SH1106Wire &display);
}