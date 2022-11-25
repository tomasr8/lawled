#include <avr/pgmspace.h>
#include "SH1106Wire.h"
#include "font.h"
#include "glossary.h"
#include "util.h"

// Initialize the OLED display using Wire library
// SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
SH1106Wire display(0x3c, SDA, SCL);

#define WIDTH 128
#define HEIGHT 64


void drawCharacter(uint16_t x, uint16_t y, int c, SH1106Wire *display) {
  for(int column = 0; column < 6; column++) {
    for(int i = 0; i < 8; i++) {
      if(bitmaps[c][column] & (1 << i)) {
        display->setPixel(column+x, i+y);
      }
    }
  }
}

void drawUnknown(uint16_t x, uint16_t y, SH1106Wire *display) {
  uint8_t unknown[6] = {0xff, 0x81, 0x81, 0x81, 0x81, 0xff};
  for(int column = 0; column < 6; column++) {
    for(int i = 0; i < 8; i++) {
      if(unknown[column] & (1 << i)) {
        display->setPixel(column+x, i+y);
      }
    }
  }
}

int drawText(const wchar_t *text, int row, int skipRows, SH1106Wire *display) {
  int x = 0;
  int y = row;
  int counter = 0;
  for(int i = skipRows * 21; i < lawled::wstrlen(text); i++) {
    wchar_t c = text[i];
    bool found = false;
    for(int j = 0; j < N_CHARACTERS; j++) {
      if(c == characters[j]) {
        drawCharacter(x*6, y*8, j, display);
        found = true;
        break;
      }
    }

    if(!found) {
      drawUnknown(x*6, y*8, display);
    }

    counter++;
    x++;
    if(counter == 21) {
      counter = 0;
      x = 0;
      y++;
    }
  }

  if(x == 0 && y == 0) {
    return 0;
  }
  return x == 0 ? y : (y + 1);
}

template <size_t rows, size_t cols>
void drawImage(const uint8_t (&image)[rows][cols], int x, int y, SH1106Wire *display) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if(image[i][j] == 0) {
        display->setPixel(x+j, y+i);
      }
    }
  }
}

void drawHorizontalLine(int y, SH1106Wire *display) {
  for (int i = 0; i < WIDTH; i++) {
    display->setPixel(i, y);
  }
}

void drawVerticalScroll(int size, int position, SH1106Wire *display) {
  for (int i = 0; i < size; i++) {
    display->setPixel(WIDTH-1, position+i);
  }
}

int indices[N_TERMS] = {0};

void init_indices(int* arr, const int size) {
  for(int i = 0; i < size; i++) {
    arr[i] = i;
  }
}

// Max term length is 46 w/o the null byte.
wchar_t current_term[50] = {0};
// Max definition length is 697 w/o the null byte.
wchar_t current_definition[700] = {0};
// Max joke length is 146 w/o the null byte.
wchar_t joke[150] = {0};

// Assumes sizeof(wchar_t) is the same size as a word.
static_assert(sizeof(wchar_t) == sizeof(word));
void read_wstring_from_flash(const wchar_t * const *wstrings, int n, wchar_t *buffer) {
  wchar_t * waddr = (wchar_t*)pgm_read_dword(&(wstrings[n]));
  uint16_t p = 0;
  while(true) {
    buffer[p] = pgm_read_dword(waddr+p);
    if(buffer[p] == '\0') {
      break;
    }
    p++;
  }
}

void drawLoading(int percentage, SH1106Wire *display) {
  display->setPixel(12, HEIGHT-3);
  display->setPixel(12, HEIGHT-4);
  display->setPixel(12, HEIGHT-5);
  display->setPixel(13, HEIGHT-2);
  display->setPixel(13, HEIGHT-6);
  display->setPixel(14, HEIGHT-2);
  display->setPixel(14, HEIGHT-6);

  display->setPixel(WIDTH-13, HEIGHT-3);
  display->setPixel(WIDTH-13, HEIGHT-4);
  display->setPixel(WIDTH-13, HEIGHT-5);
  display->setPixel(WIDTH-14, HEIGHT-2);
  display->setPixel(WIDTH-14, HEIGHT-6);
  display->setPixel(WIDTH-15, HEIGHT-2);
  display->setPixel(WIDTH-15, HEIGHT-6);

  for(int x = 15; x <= WIDTH-16; x++) {
    display->setPixel(x, HEIGHT-1);
    display->setPixel(x, HEIGHT-7);
  }

  if(percentage >= 1) {
    display->setPixel(13, HEIGHT-3);
    display->setPixel(13, HEIGHT-4);
    display->setPixel(13, HEIGHT-5);
  }
  if(percentage >= 2) {
    display->setPixel(14, HEIGHT-3);
    display->setPixel(14, HEIGHT-4);
    display->setPixel(14, HEIGHT-5);
  }
  if(percentage >= 3) {
    for(int x = 0; x <= min(percentage, 98); x++) {
      display->setPixel(x + 15, HEIGHT-2);
      display->setPixel(x + 15, HEIGHT-3);
      display->setPixel(x + 15, HEIGHT-4);
      display->setPixel(x + 15, HEIGHT-5);
      display->setPixel(x + 15, HEIGHT-6);
    }
  }
  if(percentage >= 99) {
    display->setPixel(WIDTH-15, HEIGHT-3);
    display->setPixel(WIDTH-15, HEIGHT-4);
    display->setPixel(WIDTH-15, HEIGHT-5);
  }
  if(percentage >= 100) {
    display->setPixel(WIDTH-14, HEIGHT-3);
    display->setPixel(WIDTH-14, HEIGHT-4);
    display->setPixel(WIDTH-14, HEIGHT-5);
  }
}


void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(15));
  init_indices(indices, N_TERMS);
  lawled::fisher_yates_shuffle(indices, N_TERMS);

  display.init();
  display.flipScreenVertically();
  display.setContrast(255);

  drawImage(paragraph_image, 5, 7, &display);
  drawImage(paragraph_image, 36, 7, &display);
  drawImage(paragraph_image, 67, 7, &display);
  drawImage(paragraph_image, 98, 7, &display);
  display.display();
  delay(3000);

  const int n = random(N_JOKES);
  for(int i = 0; i <= 100; i++) {
    display.clear();
    read_wstring_from_flash(jokes, n, joke);
    drawText(joke, 0, 0, &display);
    drawLoading(i, &display);
    display.display();
    delay(100);
  }
  delay(1000);
  display.clear();

}

void loop() {
  // for (int i = 0; i < 6; i++) {
  //   char * addr = (char*)pgm_read_dword(&(string_table[i]));
  //   uint16_t k = 0;
  //   while(true) {
  //     buffer[k] = pgm_read_byte(addr+k);
  //     if(buffer[k] == '\0') {
  //       break;
  //     }
  //     k++;
  //   }
  //   Serial.println(buffer);
  //   delay(1000);


  //   wchar_t * waddr = (wchar_t*)pgm_read_dword(&(wstring_table[i]));
  //   uint16_t p = 0;
  //   while(true) {
  //     buffer[p] = pgm_read_dword(waddr+p);
  //     if(buffer[p] == '\0') {
  //       break;
  //     }
  //     p++;
  //   }
  //   Serial.println(buffer);
  //   delay(1000);
  // }

  while(true) {
    for(int i = 0; i < N_TERMS; i++) {
      int lines = 0;
      display.clear();
      const int n = indices[i];
      read_wstring_from_flash(terms, n, current_term);
      lines += drawText(current_term, 0, 0, &display);
      // Serial.println(lines);
      drawHorizontalLine(lines*8 + 2, &display);
      drawHorizontalLine(lines*8 + 3, &display);
      lines++;
      read_wstring_from_flash(definitions, n, current_definition);
      const int emptySpace = (lines-1)*8 + 4;
      const int maxScrollHeight = HEIGHT - emptySpace;
      lines += drawText(current_definition, lines, 0, &display);
      int scrollSize = maxScrollHeight * (8.0/lines);
      // Serial.println(scrollSize);
      if(scrollSize >= maxScrollHeight) {
        scrollSize = 0;
      }
      int step = (maxScrollHeight - scrollSize)/(lines-10);

      drawVerticalScroll(scrollSize, emptySpace, &display);
      display.display();
      delay(1000);

      // Serial.printf("lines: %d\n", lines);

      if(scrollSize != 0) {
        for(int k = 0; k < (lines-10); k++) {
          display.clear();
          int lns = drawText(current_term, 0, 0, &display);
          drawHorizontalLine(lns*8 + 2, &display);
          drawHorizontalLine(lns*8 + 3, &display);
          lns++;
          drawText(current_definition, lns, k+1, &display);
          drawVerticalScroll(scrollSize, (k+1)*step + emptySpace, &display);
          display.display();
          delay(1000);
        }
      }
    }
    lawled::fisher_yates_shuffle(indices, N_TERMS);
  }
}


















