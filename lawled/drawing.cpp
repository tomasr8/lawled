#include <Arduino.h>
#include "drawing.h"

namespace lawled {
  void drawHorizontalLine(int y, SH1106Wire &display) {
    for (int i = 0; i < WIDTH; i++) {
      display.setPixel(i, y);
    }
  }

  void drawVerticalScroll(int size, int position, SH1106Wire &display) {
    for (int i = 0; i < size; i++) {
      display.setPixel(WIDTH-1, position+i);
    }
  }

  void drawUnknown(uint16_t x, uint16_t y, SH1106Wire &display) {
    const uint8_t unknown[6] = {0xff, 0x81, 0x81, 0x81, 0x81, 0xff};
    for(int column = 0; column < 6; column++) {
      for(int i = 0; i < 8; i++) {
        if(unknown[column] & (1 << i)) {
          display.setPixel(column+x, i+y);
        }
      }
    }
  }

  void drawLoading(int percentage, SH1106Wire &display) {
    // Draw the borders first
    display.setPixel(12, HEIGHT-3);
    display.setPixel(12, HEIGHT-4);
    display.setPixel(12, HEIGHT-5);
    display.setPixel(13, HEIGHT-2);
    display.setPixel(13, HEIGHT-6);
    display.setPixel(14, HEIGHT-2);
    display.setPixel(14, HEIGHT-6);

    display.setPixel(WIDTH-13, HEIGHT-3);
    display.setPixel(WIDTH-13, HEIGHT-4);
    display.setPixel(WIDTH-13, HEIGHT-5);
    display.setPixel(WIDTH-14, HEIGHT-2);
    display.setPixel(WIDTH-14, HEIGHT-6);
    display.setPixel(WIDTH-15, HEIGHT-2);
    display.setPixel(WIDTH-15, HEIGHT-6);

    for(int x = 15; x <= WIDTH-16; x++) {
      display.setPixel(x, HEIGHT-1);
      display.setPixel(x, HEIGHT-7);
    }

    // Draw the inner bar based on the percentage
    if(percentage >= 1) {
      display.setPixel(13, HEIGHT-3);
      display.setPixel(13, HEIGHT-4);
      display.setPixel(13, HEIGHT-5);
    }
    if(percentage >= 2) {
      display.setPixel(14, HEIGHT-3);
      display.setPixel(14, HEIGHT-4);
      display.setPixel(14, HEIGHT-5);
    }
    if(percentage >= 3) {
      for(int x = 0; x <= min(percentage, 98); x++) {
        display.setPixel(x + 15, HEIGHT-2);
        display.setPixel(x + 15, HEIGHT-3);
        display.setPixel(x + 15, HEIGHT-4);
        display.setPixel(x + 15, HEIGHT-5);
        display.setPixel(x + 15, HEIGHT-6);
      }
    }
    if(percentage >= 99) {
      display.setPixel(WIDTH-15, HEIGHT-3);
      display.setPixel(WIDTH-15, HEIGHT-4);
      display.setPixel(WIDTH-15, HEIGHT-5);
    }
    if(percentage >= 100) {
      display.setPixel(WIDTH-14, HEIGHT-3);
      display.setPixel(WIDTH-14, HEIGHT-4);
      display.setPixel(WIDTH-14, HEIGHT-5);
    }
  }
}





