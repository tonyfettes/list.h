/**
 * Generic header-only C language list library.
 *
 * vim: ft=c
 */
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list_iter_t {
  struct list_iter_t *prev;
  struct list_iter_t *next;
} list_iter_t;

#define list_t(type)                                                           \
  struct {                                                                     \
    list_iter_t iter;                                                          \
    type data;                                                                 \
  }

#define list_iter(list) (&(list)->iter)
#define list_data(list, iter) (((typeof(list) *)(iter))->data)

#define list_alloc(type) (list_t(type) *)malloc(sizeof(list_t(type)))

#define list_reset(list)                                                       \
  do {                                                                         \
    list_iter_t *iter = list_iter(list);                                       \
    iter->next = iter;                                                         \
    iter->prev = iter;                                                         \
  } while (0)

#define list_init(type)                                                        \
  ({                                                                           \
    __auto_type *list = list_alloc(type);                                      \
    list_reset(list);                                                          \
    list;                                                                      \
  })

#define list_init_literal(type, ...)                                           \
  ({                                                                           \
    __auto_type *list = list_init(type);                                       \
    const type content_array[] = __VA_ARGS__;                                  \
    for (size_t i = 0; i < sizeof(content_array) / sizeof(type); i++) {        \
      list_push_back(list, content_array[i]);                                  \
    }                                                                          \
    list;                                                                      \
  })

#define list_init_array(type, array, size)                                     \
  ({                                                                           \
    __auto_type *list = list_init(type);                                       \
    for (size_t i = 0; i < size; i++) {                                        \
      list_push_back(list, array[i]);                                          \
    }                                                                          \
    list;                                                                      \
  })

#define list_begin(list) (list_iter(list)->next)
#define list_end(list) (list_iter(list)->list)

#define list_empty(list) (list_iter(list)->next == list_iter(list))

#define list_front(list) list_data(list, list_begin(list))
#define list_back(list) list_data(list, list_end(list))

static inline void list_insert(list_iter_t *prev, list_iter_t *next, list_iter_t *node) {
  next->prev = node;
  node->next = next;
  node->prev = prev;
  prev->next = node;
}

#define list_push_back(list, value)                                            \
  do {                                                                         \
    typeof(list) node = (typeof(list))malloc(sizeof(typeof(list)));            \
    node->data = (value);                                                      \
    list_insert(list_iter(list)->prev, list_iter(list), list_iter(node));      \
  } while (0)

#define list_push_front(list, value)                                           \
  do {                                                                         \
    typeof(list) node = (typeof(list))malloc(sizeof(typeof(list)));            \
    node->data = value;                                                        \
    list_insert(list_iter(list), list_iter(list)->next, list_iter(node));      \
  } while (0)

static inline void list_concat(list_iter_t *prev, list_iter_t *next) {
  prev->next = next;
  next->prev = prev;
}

#define list_remove(list, iter)                                                \
  ({                                                                           \
    list_concat(iter->prev, iter->next);                                       \
    list_data(list, iter);                                                     \
  })

#define list_pop_front(list, iter) list_remove(list, list_begin(list))
#define list_pop_back(list, iter) list_remove(list, list_end(list))

#define list_apply(list, func)                                                 \
  do {                                                                         \
    for (list_iter_t *iter = list->next; iter != list;) {                      \
      func(list_data(list, iter), __VA_ARGS__);                                \
    }                                                                          \
  } while (0)

#define list_destroy(list)                                                     \
  do {                                                                         \
    for (list_iter_t *iter = list->next; iter != list;) {                      \
      list_iter_t *victim = iter;                                              \
      iter = iter->next;                                                       \
      free(iter);                                                              \
    }                                                                          \
    free(list);                                                                \
  } while (0)

#define list_destroy_with(list, destructor, ...)                               \
  do {                                                                         \
    for (list_iter_t *iter = list->next; iter != list;) {                      \
      list_iter_t *victim = iter;                                              \
      iter = iter->next;                                                       \
      destructor(list_data(list, iter), __VA_ARGS__);                          \
      free(iter);                                                              \
    }                                                                          \
    free(list);                                                                \
  } while (0)

#endif // LIST_H
