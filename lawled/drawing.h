#include "SH1106Wire.h"

#define WIDTH 128
#define HEIGHT 64
#define LINE_HEIGHT 8
#define LINE_LENGTH 21

namespace lawled {
  void drawHorizontalLine(int y, SH1106Wire &display);
  void drawVerticalScroll(int size, int position, SH1106Wire &display);
  void drawUnknown(uint16_t x, uint16_t y, SH1106Wire &display);
  void drawLoading(int percentage, SH1106Wire &display);

  class Display {
    private:
      SH1106Wire &_display;

    public:
      Display(SH1106Wire &display): _display(display) {}

      void clear() {
        _display.clear();
      }

      void display() {
        _display.display();
      }
  };
}