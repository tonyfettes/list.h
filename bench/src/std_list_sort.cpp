#include <list>
#include <cstdio>
#include <iostream>
#include <ctime>
#include "bench.h"

bool comp(const int &a, const int &b) {
  return a < b;
}

extern "C"
double bench() {
  clock_t start, stop;
  double sort_time;
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
    sort_time = (double)(stop - start) / CLOCKS_PER_SEC;
    printf("sort: %lf\n", sort_time);
    start = clock();
  }
  stop = clock();
  printf("dealloc: %lf\n", (double)(stop - start) / CLOCKS_PER_SEC);
  return sort_time;
}
