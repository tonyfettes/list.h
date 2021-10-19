#include <list>
#include <cstdio>
#include <iostream>
#include <ctime>
#include "sort_size.h"

bool comp(const int &a, const int &b) {
  return a < b;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::list<int> l;
  for (int i = 0; i < BENCH_LIST_SIZE; i++) {
    l.push_back(rand());
  }
  clock_t start = clock();
  // l.merge(l);
  l.sort(comp);
  clock_t end = clock();
  printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
  return 0;
}
