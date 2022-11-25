#include <Arduino.h>
#include "util.h"

namespace lawled {
  int wstrlen(const wchar_t *wstr) {
    int i = 0;
    while(wstr[i] != 0) {
      i++;
    }
    return i;
  }

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

  void fisher_yates_shuffle(int* arr, const int size) {
    for(int i = size - 1; i >= 1; i--) {
      const int j = random(i+1);
      const int tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
    }  
  }

  // fill an array with numbers 1..N
  void arange(int * indices, size_t size) {
    for(size_t i = 0; i < size; i++) {
      indices[i] = i;
    }
  }
}
