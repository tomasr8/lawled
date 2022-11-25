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

  void fisher_yates_shuffle(int* arr, const int size) {
    for(int i = size - 1; i >= 1; i--) {
      const int j = random(i+1);
      const int tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
    }  
  }
}
