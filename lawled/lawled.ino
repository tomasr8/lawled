/**
   The MIT License (MIT)

   Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
   Copyright (c) 2018 by Fabrice Weinberg

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

   ThingPulse invests considerable time and money to develop these open source libraries.
   Please support us by buying our products (and not the clones) from
   https://thingpulse.com

*/

#include <avr/pgmspace.h>
// #include <WString.h>
#include "SH1106Wire.h"
#include "glossary.h"

// Initialize the OLED display using Wire library
// SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
SH1106Wire display(0x3c, SDA, SCL);


#define WIDTH 128
#define HEIGHT 64

wchar_t characters[] = {
  L' ',
  L'!',
  L'"',
  L'#',
  L'$',
  L'%',
  L'&',
  L'\'',
  L'(',
  L')',
  L'*',
  L'+',
  L',',
  L'-',
  L'.',
  L'/',
  L'0',
  L'1',
  L'2',
  L'3',
  L'4',
  L'5',
  L'6',
  L'7',
  L'8',
  L'9',
  L':',
  L';',
  L'<',
  L'=',
  L'>',
  L'?',
  L'@',
  L'A',
  L'B',
  L'C',
  L'D',
  L'E',
  L'F',
  L'G',
  L'H',
  L'I',
  L'J',
  L'K',
  L'L', 
  L'M', 
  L'N', 
  L'O', 
  L'P', 
  L'Q', 
  L'R', 
  L'S', 
  L'T', 
  L'U', 
  L'V', 
  L'W', 
  L'X', 
  L'Y', 
  L'Z', 
  L'[', 
  L'\\',
  L']', 
  L'^', 
  L'_', 
  L'`',
  L'a', 
  L'b', 
  L'c', 
  L'd', 
  L'e', 
  L'f', 
  L'g', 
  L'h', 
  L'i', 
  L'j', 
  L'k', 
  L'l',
  L'm',
  L'n',
  L'o',
  L'p',
  L'q',
  L'r',
  L's',
  L't',
  L'u',
  L'v',
  L'w',
  L'x',
  L'y',
  L'z',
  L'{',
  L'|',
  L'}',
  L'~',
  L'ü',
  L'Ü',
  L'ä',
  L'Ä',
  L'ö',
  L'Ö',
  L'°',
  L'ß',
  // ....
  L'á',
  L'ě',
  L'é',
  L'í',
  L'ó',
  L'ú',
  L'ů',
  L'ý',
  // ....
  L'č',
  L'ď',
  L'ň',
  L'ř',
  L'š',
  L'ť',
  L'ž',
  // ....
  L'Á',
  L'Ě',
  L'É',
  L'Í',
  L'Ó',
  L'Ú',
  L'Ů',
  L'Ý',
  // ....
  L'Č',
  L'Ď',
  L'Ň',
  L'Ř',
  L'Š',
  L'Ť',
  L'Ž',
  // ....
  L'§',
};

const int N = std::size(characters);

uint8_t bitmaps[][6] = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space
  {0x00, 0x00, 0x00, 0x2f, 0x00, 0x00}, // !
  {0x00, 0x00, 0x07, 0x00, 0x07, 0x00}, // "
  {0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14}, // #
  {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12}, // $
  {0x00, 0x62, 0x64, 0x08, 0x13, 0x23}, // %
  {0x00, 0x36, 0x49, 0x55, 0x22, 0x50}, // &
  {0x00, 0x00, 0x05, 0x03, 0x00, 0x00}, // '
  {0x00, 0x00, 0x1c, 0x22, 0x41, 0x00}, // (
  {0x00, 0x00, 0x41, 0x22, 0x1c, 0x00}, // )
  {0x00, 0x14, 0x08, 0x3e, 0x08, 0x14}, // *
  {0x00, 0x08, 0x08, 0x3e, 0x08, 0x08}, // +
  {0x00, 0x00, 0x00, 0xa0, 0x60, 0x00}, // ,
  {0x00, 0x08, 0x08, 0x08, 0x08, 0x08}, // -
  {0x00, 0x00, 0x60, 0x60, 0x00, 0x00}, // .
  {0x00, 0x20, 0x10, 0x08, 0x04, 0x02}, // /
  {0x00, 0x3e, 0x51, 0x49, 0x45, 0x3e}, // 0
  {0x00, 0x00, 0x42, 0x7f, 0x40, 0x00}, // 1
  {0x00, 0x42, 0x61, 0x51, 0x49, 0x46}, // 2
  {0x00, 0x21, 0x41, 0x45, 0x4b, 0x31}, // 3
  {0x00, 0x18, 0x14, 0x12, 0x7f, 0x10}, // 4
  {0x00, 0x27, 0x45, 0x45, 0x45, 0x39}, // 5
  {0x00, 0x3c, 0x4a, 0x49, 0x49, 0x30}, // 6
  {0x00, 0x01, 0x71, 0x09, 0x05, 0x03}, // 7
  {0x00, 0x36, 0x49, 0x49, 0x49, 0x36}, // 8
  {0x00, 0x06, 0x49, 0x49, 0x29, 0x1e}, // 9
  {0x00, 0x00, 0x36, 0x36, 0x00, 0x00}, // :
  {0x00, 0x00, 0x56, 0x36, 0x00, 0x00}, // ;
  {0x00, 0x08, 0x14, 0x22, 0x41, 0x00}, // <
  {0x00, 0x14, 0x14, 0x14, 0x14, 0x14}, // =
  {0x00, 0x00, 0x41, 0x22, 0x14, 0x08}, // >
  {0x00, 0x02, 0x01, 0x51, 0x09, 0x06}, // ?
  {0x00, 0x32, 0x49, 0x59, 0x51, 0x3e}, // @
  {0x00, 0x7c, 0x12, 0x11, 0x12, 0x7c}, // A
  {0x00, 0x7f, 0x49, 0x49, 0x49, 0x36}, // B
  {0x00, 0x3e, 0x41, 0x41, 0x41, 0x22}, // C
  {0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c}, // D
  {0x00, 0x7f, 0x49, 0x49, 0x49, 0x41}, // E
  {0x00, 0x7f, 0x09, 0x09, 0x09, 0x01}, // F
  {0x00, 0x3e, 0x41, 0x49, 0x49, 0x7a}, // G
  {0x00, 0x7f, 0x08, 0x08, 0x08, 0x7f}, // H
  {0x00, 0x00, 0x41, 0x7f, 0x41, 0x00}, // I
  {0x00, 0x20, 0x40, 0x41, 0x3f, 0x01}, // J
  {0x00, 0x7f, 0x08, 0x14, 0x22, 0x41}, // K
  {0x00, 0x7f, 0x40, 0x40, 0x40, 0x40}, // L
  {0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f}, // M
  {0x00, 0x7f, 0x04, 0x08, 0x10, 0x7f}, // N
  {0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e}, // O
  {0x00, 0x7f, 0x09, 0x09, 0x09, 0x06}, // P
  {0x00, 0x3e, 0x41, 0x51, 0x21, 0x5e}, // Q
  {0x00, 0x7f, 0x09, 0x19, 0x29, 0x46}, // R
  {0x00, 0x46, 0x49, 0x49, 0x49, 0x31}, // S
  {0x00, 0x01, 0x01, 0x7f, 0x01, 0x01}, // T
  {0x00, 0x3f, 0x40, 0x40, 0x40, 0x3f}, // U
  {0x00, 0x1f, 0x20, 0x40, 0x20, 0x1f}, // V
  {0x00, 0x3f, 0x40, 0x38, 0x40, 0x3f}, // W
  {0x00, 0x63, 0x14, 0x08, 0x14, 0x63}, // X
  {0x00, 0x07, 0x08, 0x70, 0x08, 0x07}, // Y
  {0x00, 0x61, 0x51, 0x49, 0x45, 0x43}, // Z
  {0x00, 0x00, 0x7f, 0x41, 0x41, 0x00}, // [
  {0x02, 0x04, 0x08, 0x10, 0x20, 0x00}, // backslash
  {0x00, 0x00, 0x41, 0x41, 0x7f, 0x00}, // ]
  {0x00, 0x04, 0x02, 0x01, 0x02, 0x04}, // ^
  {0x00, 0x40, 0x40, 0x40, 0x40, 0x40}, // _
  {0x00, 0x00, 0x01, 0x02, 0x04, 0x00}, // `
  {0x00, 0x20, 0x54, 0x54, 0x54, 0x78}, // a
  {0x00, 0x7f, 0x48, 0x44, 0x44, 0x38}, // b
  {0x00, 0x38, 0x44, 0x44, 0x44, 0x20}, // c
  {0x00, 0x38, 0x44, 0x44, 0x48, 0x7f}, // d
  {0x00, 0x38, 0x54, 0x54, 0x54, 0x18}, // e
  {0x00, 0x08, 0x7e, 0x09, 0x01, 0x02}, // f
  {0x00, 0x18, 0xa4, 0xa4, 0xa4, 0x7c}, // g
  {0x00, 0x7f, 0x08, 0x04, 0x04, 0x78}, // h
  {0x00, 0x00, 0x44, 0x7d, 0x40, 0x00}, // i
  {0x00, 0x40, 0x80, 0x84, 0x7d, 0x00}, // j
  {0x00, 0x7f, 0x10, 0x28, 0x44, 0x00}, // k
  {0x00, 0x00, 0x41, 0x7f, 0x40, 0x00}, // l
  {0x00, 0x7c, 0x04, 0x18, 0x04, 0x78}, // m
  {0x00, 0x7c, 0x08, 0x04, 0x04, 0x78}, // n
  {0x00, 0x38, 0x44, 0x44, 0x44, 0x38}, // o
  {0x00, 0xfc, 0x24, 0x24, 0x24, 0x18}, // p
  {0x00, 0x18, 0x24, 0x24, 0x18, 0xfc}, // q
  {0x00, 0x7c, 0x08, 0x04, 0x04, 0x08}, // r
  {0x00, 0x48, 0x54, 0x54, 0x54, 0x20}, // s
  {0x00, 0x04, 0x3f, 0x44, 0x40, 0x20}, // t
  {0x00, 0x3c, 0x40, 0x40, 0x20, 0x7c}, // u
  {0x00, 0x1c, 0x20, 0x40, 0x20, 0x1c}, // v
  {0x00, 0x3c, 0x40, 0x30, 0x40, 0x3c}, // w
  {0x00, 0x44, 0x28, 0x10, 0x28, 0x44}, // x
  {0x00, 0x1c, 0xa0, 0xa0, 0xa0, 0x7c}, // y
  {0x00, 0x44, 0x64, 0x54, 0x4c, 0x44}, // z
  {0x00, 0x00, 0x08, 0x77, 0x41, 0x00}, // {
  {0x00, 0x00, 0x00, 0x63, 0x00, 0x00}, // ¦
  {0x00, 0x00, 0x41, 0x77, 0x08, 0x00}, // }
  {0x00, 0x08, 0x04, 0x08, 0x08, 0x04}, // ~
  {0x00, 0x3d, 0x40, 0x40, 0x20, 0x7d}, // ü
  {0x00, 0x3d, 0x40, 0x40, 0x40, 0x3d}, // Ü
  {0x00, 0x21, 0x54, 0x54, 0x54, 0x79}, // ä
  {0x00, 0x7d, 0x12, 0x11, 0x12, 0x7d}, // Ä
  {0x00, 0x39, 0x44, 0x44, 0x44, 0x39}, // ö
  {0x00, 0x3d, 0x42, 0x42, 0x42, 0x3d}, // Ö
  {0x00, 0x02, 0x05, 0x02, 0x00, 0x00}, // °
  {0x00, 0x7e, 0x01, 0x49, 0x55, 0x73}, // ß
  //..........................................
  {0x00, 0x20, 0x54, 0x56, 0x55, 0x78}, // á
  {0x00, 0x38, 0x55, 0x56, 0x55, 0x18}, // ě
  {0x00, 0x38, 0x54, 0x56, 0x55, 0x18}, // é
  {0x00, 0x00, 0x44, 0x7e, 0x41, 0x00}, // í
  {0x00, 0x38, 0x44, 0x46, 0x45, 0x38}, // ó
  {0x00, 0x3c, 0x40, 0x42, 0x21, 0x7c}, // ú
  {0x00, 0x3c, 0x42, 0x45, 0x22, 0x7c}, // ů
  {0x00, 0x1c, 0xa0, 0xa2, 0xa1, 0x7c}, // ý
  //..........................................
  {0x00, 0x38, 0x45, 0x46, 0x45, 0x20}, // č
  {0x38, 0x44, 0x44, 0x48, 0x7f, 0x03}, // ď
  {0x00, 0x7c, 0x09, 0x06, 0x05, 0x78}, // ň
  {0x00, 0x7c, 0x09, 0x06, 0x05, 0x08}, // ř
  {0x00, 0x48, 0x55, 0x56, 0x55, 0x20}, // š
  {0x00, 0x04, 0x3f, 0x44, 0x43, 0x20}, // ť
  {0x00, 0x44, 0x65, 0x56, 0x4d, 0x44}, // ž
  //..........................................
  {0x00, 0x20, 0x54, 0x56, 0x55, 0x78}, // á
  {0x00, 0x38, 0x55, 0x56, 0x55, 0x18}, // ě
  {0x00, 0x38, 0x54, 0x56, 0x55, 0x18}, // é
  {0x00, 0x00, 0x44, 0x7e, 0x41, 0x00}, // í
  {0x00, 0x38, 0x44, 0x46, 0x45, 0x38}, // ó
  {0x00, 0x3c, 0x40, 0x42, 0x21, 0x7c}, // ú
  {0x00, 0x3c, 0x42, 0x45, 0x22, 0x7c}, // ů
  {0x00, 0x1c, 0xa0, 0xa2, 0xa1, 0x7c}, // ý
  //..........................................
  {0x00, 0x38, 0x45, 0x46, 0x45, 0x20}, // č
  {0x38, 0x44, 0x44, 0x48, 0x7f, 0x03}, // ď
  {0x00, 0x7c, 0x09, 0x06, 0x05, 0x78}, // ň
  {0x00, 0x7c, 0x09, 0x06, 0x05, 0x08}, // ř
  {0x00, 0x48, 0x55, 0x56, 0x55, 0x20}, // š
  {0x00, 0x04, 0x3f, 0x44, 0x43, 0x20}, // ť
  {0x00, 0x44, 0x65, 0x56, 0x4d, 0x44}, // ž
  //..........................................
  {0x00, 0x9e, 0xa5, 0xa5, 0x79 ,0x00}, // §
};

static_assert(std::size(characters) == std::size(bitmaps));

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

int wstrlen(const wchar_t *wstr) {
  int i = 0;
  while(wstr[i] != 0) {
    i++;
  }
  return i;
}

int drawText(const wchar_t *text, int row, int skipRows, SH1106Wire *display) {
  int x = 0;
  int y = row;
  int counter = 0;
  for(int i = skipRows * 21; i < wstrlen(text); i++) {
    wchar_t c = text[i];
    bool found = false;
    for(int j = 0; j < N; j++) {
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

  // Serial.print(x);
  // Serial.print(", ");
  // Serial.println(y);

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

void fisher_yates_shuffle(int* arr, const int size) {
  for(int i = size - 1; i >= 1; i--) {
    const int j = random(i+1);
    const int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
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

// void drawJoke(int n, SH1106Wire *display) {
//   read_wstring_from_flash(jokes, n, joke);
//   drawText(current_term, 0, 0, &display);
// }

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(15));
  init_indices(indices, N_TERMS);
  fisher_yates_shuffle(indices, N_TERMS);

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
    fisher_yates_shuffle(indices, N_TERMS);
  }
}


















