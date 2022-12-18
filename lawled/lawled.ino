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

int getLineCount(const wchar_t *text) {
  return std::ceil(lawled::wstrlen(text) / 21.0);
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
// last interaction
unsigned long lastUserActionTime = 0;
unsigned long lastAutomaticChangeTime = 0;

int getTermLineCount(int currentTerm) {
  const int n = indices[currentTerm];
  lawled::read_wstring_from_flash(terms, n, current_term);
  return getLineCount(current_term);
}

int getDefinitionLineCount(int currentTerm) {
  const int n = indices[currentTerm];
  lawled::read_wstring_from_flash(definitions, n, current_definition);
  return getLineCount(current_definition);
}

int getTotalLineCount(int currentTerm) {
  return 1 + getTermLineCount(currentTerm) + getDefinitionLineCount(currentTerm);
}

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

int drawTerm(int currentTerm, int skipRows, SH1106Wire &display) {
  int lines = 0;
  display.clear();
  const int n = indices[currentTerm];
  lawled::read_wstring_from_flash(terms, n, current_term);
  drawText(current_term, 0, 0, display);
  lines += getLineCount(current_term);
  lawled::drawHorizontalLine(lines*8 + 2, display);
  lawled::drawHorizontalLine(lines*8 + 3, display);
  lines++;
  lawled::read_wstring_from_flash(definitions, n, current_definition);
  const int emptySpace = (lines-1)*8 + 4;
  const int maxScrollHeight = HEIGHT - emptySpace;
  drawText(current_definition, lines, skipRows, display);
  lines += getLineCount(current_definition);

  if(lines <= 8) {
    display.display();
    return lines;
  }

  const int avail = 8 - (1 + getLineCount(current_term));
  const int steps = getLineCount(current_definition) - avail;

  int scrollSize = maxScrollHeight / (steps+0.5);
  int step = (maxScrollHeight - scrollSize)/(steps);

  lawled::drawVerticalScroll(scrollSize, skipRows*step + emptySpace, display);
  display.display();
  return lines;
}


#define LEFT_BTN 14
#define RIGHT_BTN 12

void setup() {
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);
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

  lastAutomaticChangeTime = millis();
}

int currentTerm = 0;
int currentScroll = 0;

void loop() {
  int leftBtn = digitalRead(LEFT_BTN);
  int rightBtn = digitalRead(RIGHT_BTN);

  if(leftBtn == LOW || rightBtn == LOW) {
    if((millis() - lastUserActionTime) < 50) {
      lastUserActionTime = millis();
      return;
    }
    lastUserActionTime = millis();
  }

  const bool manual = (lastUserActionTime != 0 && (millis() - lastUserActionTime) < 3e4);

  // Serial.print("Left: "); Serial.print(leftBtn); Serial.print(" Right: "); Serial.println(rightBtn);
  // Serial.print("Last interaction: "); Serial.println(lastUserActionTime);
  // Serial.print("Manual: "); Serial.println(manual);

  if(manual) {
    // handle stuff
    if(leftBtn == LOW && rightBtn == LOW) {
      // noop
    } else if(leftBtn == LOW) {
      if(currentScroll == 0) {
        currentTerm = max(0, currentTerm - 1);
      } else {
        currentScroll = max(0, currentScroll - 1);
      }

      drawTerm(currentTerm, currentScroll, display);
    } else if(rightBtn == LOW) {
      const int termCount = getTermLineCount(currentTerm);
      const int defCount = getDefinitionLineCount(currentTerm);
      if((1 + termCount + defCount) <= 8) {
        currentTerm = min(N_TERMS - 1, currentTerm + 1);
        currentScroll = 0;
      } else {
        const int avail = 8 - (1 + termCount);
        const int maxSkip = defCount - avail;
        if(currentScroll == maxSkip) {
          currentTerm = min(N_TERMS - 1, currentTerm + 1);
          currentScroll = 0;
        } else {
          currentScroll++;
        }
      }

      drawTerm(currentTerm, currentScroll, display);
    }
  } else {
    currentScroll = 0;
    drawTerm(currentTerm, 0, display);
    if((millis() - lastAutomaticChangeTime) > 4000) {
      currentTerm++; 
      lastAutomaticChangeTime = millis();
    }

    if(currentTerm >= N_TERMS) {
      lawled::fisher_yates_shuffle(indices, N_TERMS);
      currentTerm = 0;
    }
  }

  delay(50);
}


















