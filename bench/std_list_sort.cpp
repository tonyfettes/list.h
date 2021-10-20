#include <list>
#include <cstdio>
#include <iostream>
#include <ctime>
#include "sort_size.h"

bool comp(const int &a, const int &b) {
  return a < b;
}

int main() {
  srand(1234);
  std::ios_base::sync_with_stdio(false);
  clock_t start, stop;
  {
    start = clock();
    std::list<int> l;
    for (int i = 0; i < BENCH_LIST_SIZE; i++) {
      l.push_back(rand());
    }
    stop = clock();
    printf("setup: %lf\n", (double)(stop - start) / CLOCKS_PER_SEC);
    start = clock();
    // l.merge(l);
    l.sort(comp);
    stop = clock();
    printf("sort: %lf\n", (double)(stop - start) / CLOCKS_PER_SEC);
    start = clock();
  }
  stop = clock();
  printf("dealloc: %lf\n", (double)(stop - start) / CLOCKS_PER_SEC);
  return 0;
}
