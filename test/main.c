#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#ifndef RAND_SEED
#define RAND_SEED 1024
#endif

int comp(const void *a, const void *b) {
  return *(const int *)(a) - *(const int *)b;
}

int int_copier(const int *data) {
  return *data;
}

void list_array_check(const int *list, const int *array, size_t index) {
  assert(*list == array[index]);
}

int main() {
  srand(RAND_SEED);
  { // Push back
    __auto_type list = list_init(int);
    assert(list_size(list) == 0);
    assert(list_empty(list));
    list_push_back(list, 1);
    assert(list_end(list) == &list->iter);
    assert(list_back(list) == 1);
    assert(list_front(list) == 1);
    assert(list_size(list) == 1);
    assert(list_empty(list) == 0);
    list_push_back(list, 2);
    assert(list_end(list) == &list->iter);
    assert(list_back(list) == 2);
    assert(list_front(list) == 1);
    assert(list_size(list) == 2);
    list_trivial_destroy(list);
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
    list_trivial_destroy(list);
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
    list_trivial_destroy(list);
  }
  { // list_erase
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    size_t even_count = 0;
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
    list_trivial_destroy(list);
  }
  { // equal
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    for (int i = 0; i < array_size; i++) {
      array[i] = rand();
    }
    __auto_type left = list_from_array(int , array, array_size);
    __auto_type right = list_from_array(int , array, array_size);
    assert(list_equal(left, right, comp));
    list_trivial_destroy(left);
    list_trivial_destroy(right);
  }
  { // copy
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    for (int i = 0; i < array_size; i++) {
      array[i] = rand();
    }
    __auto_type list = list_from_array(int , array, array_size);
    __auto_type copy = list_clone(list, int_copier);
    assert(list_equal(list, copy, comp));
    list_trivial_destroy(list);
    list_trivial_destroy(copy);
  }
  { // flip
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    for (int i = 0; i < array_size; i++) {
      array[i] = rand();
    }
    __auto_type list = list_from_array(int, array, array_size);
    __auto_type flip = list_flip(list_clone(list, int_copier));
    while (list_size(list) && list_size(flip)) {
      assert(list_pop_front(list) == list_pop_back(flip));
    }
    assert(list_empty(list));
    assert(list_empty(flip));
    list_trivial_destroy(list);
    list_trivial_destroy(flip);
  }
  { // reverse
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    for (int i = 0; i < array_size; i++) {
      array[i] = rand() % 16;
    }
    __auto_type list = list_from_array(int, array, array_size);
    for (__auto_type iter = list_begin(list); iter != list_end(list); iter = iter->next) {
      fprintf(stderr, "%d ", list_data(list, iter));
    }
    fputc('\n', stderr);
    __auto_type flip = list_flip(list_clone(list, int_copier));
    list_iter_reverse(list_begin(list), list_end(list));
    for (__auto_type iter = list_begin(list); iter != list_end(list); iter = iter->next) {
      fprintf(stderr, "%d ", list_data(list, iter));
    }
    fputc('\n', stderr);
    assert(list_equal(flip, list, comp));
    list_trivial_destroy(list);
    list_trivial_destroy(flip);
  }
  { // merge
    __auto_type left = list_init(int);
    list_push_back(left, 1);
    list_push_back(left, 3);
    list_push_back(left, 5);
    list_push_back(left, 8);
    __auto_type right = list_init(int);
    list_push_back(right, 2);
    list_push_back(right, 4);
    list_push_back(right, 7);
    list_push_back(right, 9);
    list_merge(left, right, comp);
    __auto_type correct = list_init(int);
    list_push_back(correct, 1);
    list_push_back(correct, 2);
    list_push_back(correct, 3);
    list_push_back(correct, 4);
    list_push_back(correct, 5);
    list_push_back(correct, 7);
    list_push_back(correct, 8);
    list_push_back(correct, 9);
    for (__auto_type iter = list_begin(left); iter != list_end(left);
         iter = iter->next) {
      printf("%d ", list_data(left, iter));
    }
    putchar('\n');
    assert(list_equal(left, correct, comp));
    fprintf(stderr, "merge test successful\n");
  }
  { // swap
    __auto_type left = list_init(int);
    list_push_back(left, 1);
    list_push_back(left, 3);
    list_push_back(left, 5);
    list_push_back(left, 8);
    __auto_type left_copy = list_clone(left, int_copier);
    __auto_type right = list_init(int);
    list_push_back(right, 2);
    list_push_back(right, 4);
    list_push_back(right, 7);
    list_push_back(right, 9);
    __auto_type right_copy = list_clone(right, int_copier);
    list_swap(left, right);
    assert(list_equal(left, right_copy, comp));
    assert(list_equal(right, left_copy, comp));
  }
  { // swap for sort
    fprintf(stderr, "swap for sort\n");
    __auto_type left = list_init(int);
    list_push_back(left, 4);
    __auto_type left_copy = list_clone(left, int_copier);
    __auto_type right = list_init(int);
    __auto_type right_copy = list_clone(right, int_copier);
    list_swap(left, right);
    assert(list_equal(left, right_copy, comp));
    assert(list_equal(right, left_copy, comp));
  }
  { // sort
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    for (int i = 0; i < array_size; i++) {
      array[i] = rand() % 16;
    }
    __auto_type list = list_from_array(int, array, array_size);
    assert(list_size(list) == array_size);
    list_sort(list, comp);
    fprintf(stderr, "list_size: %zu\n", list_size(list));
    assert(list_size(list) == array_size);
    qsort(array, array_size, sizeof(array[0]), comp);
    for (int i = 0; i < array_size; i++) {
      fprintf(stderr, "%d ", array[i]);
    }
    fputc('\n', stderr);
    for (__auto_type iter = list_begin(list); iter != list_end(list); iter = iter->next) {
      fprintf(stderr, "%d ", list_data(list, iter));
    }
    fputc('\n', stderr);
    for (int i = 0; i < array_size; i++) {
      int result = list_pop_front(list);
      fprintf(stderr, "[%d]: %d <=> %d\n", i, array[i], result);
      assert(array[i] == result);
    }
    assert(list_size(list) == 0);
    assert(list_empty(list));
    list_trivial_destroy(list);
  }
  { // access
    enum { array_size = 64 };
    int array[array_size] = { 0 };
    for (int i = 0; i < array_size; i++) {
      array[i] = rand();
    }
    __auto_type list = list_from_array(int, array, array_size);
    size_t index = 0;
    list_access(list, list_array_check, array, index++);
    list_trivial_destroy(list);
  }
  return 0;
}
