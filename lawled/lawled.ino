#include <avr/pgmspace.h>
// #include "SH1106Wire.h"
#include "font.h"
#include "glossary.h"
#include "util.h"
#include "drawing.h"

SH1106Wire display(0x3c, SDA, SCL);

void drawCharacter(uint16_t x, uint16_t y, int c, SH1106Wire &display) {
  for(int column = 0; column < 6; column++) {
    for(int i = 0; i < 8; i++) {
      if(bitmaps[c][column] & (1 << i)) {
        display.setPixel(column+x, i+y);
      }
    }
  }
}

int drawText(const wchar_t *text, int row, int skipRows, SH1106Wire &display) {
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
      lawled::drawUnknown(x*6, y*8, display);
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
void drawImage(const uint8_t (&image)[rows][cols], int x, int y, SH1106Wire &display) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if(image[i][j] == 0) {
        display.setPixel(x+j, y+i);
      }
    }
  }
}

// pre-initialized randomly shuffled indices to use for the terms.
// if we just used 'terms[random()]' we could get duplicated.
// This way we go through the whole array w/o repeats.
int indices[N_TERMS] = {0};
// Max term length is 46 w/o the null byte.
wchar_t current_term[50] = {0};
// Max definition length is 697 w/o the null byte.
wchar_t current_definition[700] = {0};
// Max joke length is 146 w/o the null byte.
wchar_t joke[150] = {0};

void drawInitialScreen(SH1106Wire &display) {
  display.clear();
  drawImage(paragraph_image, 5, 7, display);
  drawImage(paragraph_image, 36, 7, display);
  drawImage(paragraph_image, 67, 7, display);
  drawImage(paragraph_image, 98, 7, display);
  display.display();
}

void drawLoadingScreen(SH1106Wire &display) {
  const int n = random(N_JOKES);
  for(int i = 0; i <= 100; i++) {
    display.clear();
    lawled::read_wstring_from_flash(jokes, n, joke);
    drawText(joke, 0, 0, display);
    lawled::drawLoading(i, display);
    display.display();
    delay(100);
  }
}

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(15));
  lawled::arange(indices, N_TERMS);
  lawled::fisher_yates_shuffle(indices, N_TERMS);

  display.init();
  display.flipScreenVertically();
  display.setContrast(255);

  drawInitialScreen(display);
  delay(3000);

  drawLoadingScreen(display);
  delay(1000);
  display.clear();
}

void loop() {
  while(true) {
    for(int i = 0; i < N_TERMS; i++) {
      int lines = 0;
      display.clear();
      const int n = indices[i];
      lawled::read_wstring_from_flash(terms, n, current_term);
      lines += drawText(current_term, 0, 0, display);
      lawled::drawHorizontalLine(lines*8 + 2, display);
      lawled::drawHorizontalLine(lines*8 + 3, display);
      lines++;
      lawled::read_wstring_from_flash(definitions, n, current_definition);
      const int emptySpace = (lines-1)*8 + 4;
      const int maxScrollHeight = HEIGHT - emptySpace;
      lines += drawText(current_definition, lines, 0, display);
      int scrollSize = maxScrollHeight * (8.0/lines);
      if(scrollSize >= maxScrollHeight) {
        scrollSize = 0;
      }
      int step = (maxScrollHeight - scrollSize)/(lines-10);

      lawled::drawVerticalScroll(scrollSize, emptySpace, display);
      display.display();
      delay(1000);

      if(scrollSize != 0) {
        for(int k = 0; k < (lines-10); k++) {
          display.clear();
          int lns = drawText(current_term, 0, 0, display);
          lawled::drawHorizontalLine(lns*8 + 2, display);
          lawled::drawHorizontalLine(lns*8 + 3, display);
          lns++;
          drawText(current_definition, lns, k+1, display);
          lawled::drawVerticalScroll(scrollSize, (k+1)*step + emptySpace, display);
          display.display();
          delay(1000);
        }
      }
    }
    lawled::fisher_yates_shuffle(indices, N_TERMS);
  }
}


















