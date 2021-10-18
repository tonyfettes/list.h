#include "list.h"
#include <stdio.h>
#include <time.h>
#include "sort_size.h"

int comp(const int *a, const int *b) {
  return *a - *b;
}

int main() {
  srand(1234);
  __auto_type list = list_init(int);
  for (int i = 0; i < BENCH_LIST_SIZE; i++) {
    list_push_back(list, rand());
  }
  clock_t start = clock();
  list_sort(list, comp);
  clock_t stop = clock();
  printf("%lf\n", (double)(stop - start) / CLOCKS_PER_SEC);
  list_trivial_destroy(list);
  return 0;
}
