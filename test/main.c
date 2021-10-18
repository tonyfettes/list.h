#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#ifndef RAND_SEED
#define RAND_SEED 1024
#endif

int comp(int a, int b) {
  return a < b;
}

int main() {
  srand(RAND_SEED);
  { // Push back
    __auto_type list = list_init(int);
    assert(list_size(list) == 0);
    assert(list_empty(list));
    list_push_back(list, 1);
    assert(list_back(list) == 1);
    assert(list_front(list) == 1);
    assert(list_size(list) == 1);
    assert(list_empty(list) == 0);
    list_push_back(list, 2);
    assert(list_back(list) == 2);
    assert(list_front(list) == 1);
    assert(list_size(list) == 2);
    list_free(list);
  }
  { // Push front
    __auto_type list = list_init(int);
    assert(list_size(list) == 0);
    list_push_front(list, 1);
    assert(list_back(list) == 1);
    assert(list_front(list) == 1);
    assert(list_size(list) == 1);
    list_push_front(list, 2);
    assert(list_back(list) == 1);
    assert(list_front(list) == 2);
    assert(list_size(list) == 2);
    list_free(list);
  }
  { // init_from_array
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    for (int i = 0; i < array_size; i++) {
      array[i] = rand();
    }
    __auto_type list = list_from_array(int, array, array_size);
    assert(list_size(list) == array_size);
    for (int i = 0; i < array_size; i++) {
      assert(array[i] == list_pop_front(list));
    }
    assert(list_size(list) == 0);
    for (int i = 0; i < array_size; i++) {
      list_push_front(list, array[i]);
    }
    for (int i = 0; i < array_size; i++) {
      assert(array[i] == list_pop_back(list));
    }
  }
  { // list_erase
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    int even_count = 0;
    for (int i = 0; i < array_size; i++) {
      array[i] = rand() % 16;
      if (array[i] % 2 == 0) {
        even_count++;
      }
    }
    __auto_type list = list_from_array(int, array, array_size);
    assert(list_size(list) == array_size);
    for (__auto_type iter = list_begin(list); iter != list_end(list); ) {
      if (list_data(list, iter) % 2 == 0) {
        __auto_type victim = iter;
        iter = iter->next;
        list_erase(list, victim);
      } else {
        iter = iter->next;
      }
    }
    assert(list_size(list) == array_size - even_count);
  }
  {
  }
  return 0;
}
