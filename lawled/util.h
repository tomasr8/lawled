namespace lawled {
  int wstrlen(const wchar_t *wstr);
  void read_wstring_from_flash(const wchar_t * const *wstrings, int n, wchar_t *buffer);
  void fisher_yates_shuffle(int* arr, const int size);
  void arange(int * indices, size_t size);
}