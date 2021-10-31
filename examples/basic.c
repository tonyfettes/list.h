#include "list.h"
#include <assert.h>

int comp(const int *a, const int *b) {
  return *a - *b;
}

int main() {
  __auto_type list = list_init(int);
  list_push_back(list, 3);
  assert(list_back(list) == 3);
  assert(list_front(list) == 3);
  assert(list_size(list) == 1);
  list_push_back(list, 5);
  list_push_back(list, 1);
  list_push_back(list, 2);
  list_push_back(list, 4);
  list_sort(list, comp);
  int sorted_array[] = { 1, 2, 3, 4, 5 };
  __auto_type sorted = list_from_array(int, sorted_array, 5);
  assert(list_equal(list, sorted, comp));
  return 0;
}
