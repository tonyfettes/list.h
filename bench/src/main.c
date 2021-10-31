#include "bench.h"
#include <stdio.h>

int main() {
  double total_time = 0;
  for (int i = 0; i < BENCH_REPEAT; i++) {
    total_time += bench();
  }
  printf("average: %lf\n", total_time / BENCH_REPEAT);
}
