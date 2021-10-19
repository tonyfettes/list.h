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

typedef struct list_iter_flip_t {
  struct list_iter_flip_t *next;
  struct list_iter_flip_t *prev;
} list_iter_flip_t;

#define list_t(type)                                                           \
  struct {                                                                     \
    list_iter_t iter;                                                          \
    type data;                                                                 \
  }

#define list_flip_t(type)                                                      \
  struct {                                                                     \
    list_iter_flip_t iter;                                                     \
    type data;                                                                 \
  }

typedef struct list_head_t {
  list_iter_t iter;
  size_t size;
} list_head_t;

#define list_iter(list) (&(list)->iter)
#define list_data(list, iter) (((typeof(list))(iter))->data)
#define list_type(list) typeof((list)->data)

#ifndef list_alloc
#define list_alloc malloc
#endif

#ifndef list_free
#define list_free free
#endif

#define list_reset(list)                                                       \
  do {                                                                         \
    __auto_type iter___ = list_iter(list);                                     \
    iter___->next = iter___;                                                   \
    iter___->prev = iter___;                                                   \
  } while (0)

#define list_init(type)                                                        \
  ({                                                                           \
    __auto_type list = (list_head_t *)list_alloc(sizeof(list_head_t));         \
    list_reset(list);                                                          \
    list->size = 0;                                                            \
    (list_t(type) *)list;                                                      \
  })

#define list_from_array(type, array, size)                                     \
  ({                                                                           \
    __auto_type list = list_init(type);                                        \
    for (size_t i = 0; i < size; i++) {                                        \
      list_push_back(list, (array)[i]);                                        \
    }                                                                          \
    list;                                                                      \
  })

#define list_begin(list) (list_iter(list)->next)
#define list_end(list) list_iter(list)
#define list_rbegin(list) (list_iter(list)->prev)
#define list_rend(list) list_iter(list)

#define list_empty(list)                                                       \
  ({                                                                           \
    __auto_type list___ = list;                                                \
    list_iter(list___)->next == list_iter(list___);                            \
  })

#define list_size(list)                                                        \
  ({                                                                           \
    const size_t size___ = ((list_head_t *)(list))->size;                      \
    size___;                                                                   \
  })

#define list_front(list) list_data(list, list_iter(list)->next)
#define list_back(list) list_data(list, list_iter(list)->prev)

#define list_iter_insert(prev_iter, next_iter, node_iter)                      \
  do {                                                                         \
    __auto_type prev___ = (prev_iter);                                         \
    __auto_type next___ = (next_iter);                                         \
    __auto_type node___ = (node_iter);                                         \
    next___->prev = node___;                                                   \
    node___->next = next___;                                                   \
    node___->prev = prev___;                                                   \
    prev___->next = node___;                                                   \
  } while (0)

#define list_push_front(list, value)                                           \
  ({                                                                           \
    __auto_type list___ = list;                                                \
    typeof(list___) new_node = (typeof(list___))list_alloc(sizeof(*list___));  \
    new_node->data = (value);                                                  \
    list_iter_insert(list_iter(list___), list_iter(list___)->next,             \
                     list_iter(new_node));                                     \
    ((list_head_t *)list___)->size++;                                          \
    new_node;                                                                  \
  })

#define list_push_back(list, value)                                            \
  ({                                                                           \
    __auto_type list___ = list;                                                \
    typeof(list___) new_node = (typeof(list___))list_alloc(sizeof(*list___));  \
    new_node->data = (value);                                                  \
    list_iter_insert(list_iter(list___)->prev, list_iter(list___),             \
                     list_iter(new_node));                                     \
    ((list_head_t *)list___)->size++;                                          \
    new_node;                                                                  \
  })

#define list_iter_concat(prev_iter, next_iter)                                 \
  do {                                                                         \
    __auto_type prev___ = (prev_iter);                                         \
    __auto_type next___ = (next_iter);                                         \
    prev___->next = next___;                                                   \
    next___->prev = prev___;                                                   \
  } while (0)

#define list_erase(list, iter)                                                 \
  ({                                                                           \
    __auto_type list____ = (list);                                             \
    __auto_type iter___ = (iter);                                              \
    list_iter_concat((iter___)->prev, (iter___)->next);                        \
    ((list_head_t *)list____)->size--;                                         \
    list_free(iter___);                                                        \
  })

#define list_pop_front(list)                                                   \
  ({                                                                           \
    __auto_type list___ = (list);                                              \
    __auto_type front = list_begin(list___);                                   \
    __auto_type ret = list_data(list___, front);                               \
    list_erase(list___, front);                                                \
    ret;                                                                       \
  })

#define list_pop_back(list)                                                    \
  ({                                                                           \
    __auto_type list___ = (list);                                              \
    __auto_type back = list_rbegin(list___);                                   \
    __auto_type ret = list_data(list___, back);                                \
    list_erase(list___, back);                                                 \
    ret;                                                                       \
  })

#define list_flip(list)                                                        \
  ({                                                                           \
    __auto_type list___ = (list);                                              \
    _Generic(list_iter(list___), list_iter_t *                                 \
             : (list_flip_t(list_type(list___)) *)(list___),                   \
               list_iter_flip_t *                                              \
             : (list_t(list_type(list___)) *)(list___));                       \
  })

#define list_iter_reverse(left_iter, right_iter)                               \
  do {                                                                         \
    __auto_type left___ = (left_iter);                                         \
    __auto_type right___ = (right_iter);                                       \
    left___->prev->next = right___->prev;                                      \
    right___->prev = left___;                                                  \
    for (__auto_type iter = left___; iter != right___;) {                      \
      __auto_type target = iter;                                               \
      iter = iter->next;                                                       \
      __auto_type temp = target->prev;                                         \
      target->prev = target->next;                                             \
      target->next = temp;                                                     \
    }                                                                          \
  } while (0)

#define list_equal(left, right, data_compare)                                  \
  ({                                                                           \
    __auto_type left___ = (left);                                              \
    __auto_type right___ = (right);                                            \
    __auto_type comp___ = (data_compare);                                      \
    __auto_type left_iter = list_begin(left___);                               \
    __auto_type right_iter = list_begin(right___);                             \
    int result = (list_size(left___) == list_size(right___));                  \
    if (result) {                                                              \
      while (left_iter != list_end(left___) &&                                 \
             right_iter != list_end(right___)) {                               \
        if (comp___(&list_data(left___, left_iter),                            \
                    &list_data(right___, right_iter)) != 0) {                  \
          result = 0;                                                          \
          break;                                                               \
        } else {                                                               \
          left_iter = left_iter->next;                                         \
          right_iter = right_iter->next;                                       \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    result;                                                                    \
  })

#define list_clone(list, data_copy)                                            \
  ({                                                                           \
    __auto_type list___ = (list);                                              \
    __auto_type copy___ = (data_copy);                                         \
    __auto_type new_list = list_init(list_type(list___));                      \
    for (__auto_type iter = list_begin(list___); iter != list_end(list___);    \
         iter = iter->next) {                                                  \
      list_push_back(new_list, copy___(&list_data(list___, iter)));            \
    }                                                                          \
    new_list;                                                                  \
  })

#define list_iter_swap(left_iter, right_iter)                                  \
  do {                                                                         \
    __auto_type left_iter___ = (left_iter);                                    \
    __auto_type right_iter___ = (right_iter);                                  \
    right_iter___->next->prev = left_iter___;                                  \
    right_iter___->prev->next = left_iter___;                                  \
    left_iter___->next->prev = right_iter___;                                  \
    left_iter___->prev->next = right_iter___;                                  \
    __auto_type temp = right_iter___->next;                                    \
    right_iter___->next = left_iter___->next;                                  \
    left_iter___->next = temp;                                                 \
    temp = right_iter___->prev;                                                \
    right_iter___->prev = left_iter___->prev;                                  \
    left_iter___->prev = temp;                                                 \
  } while (0)

#define list_swap(left, right)                                                 \
  do {                                                                         \
    __auto_type left___ = (left);                                              \
    __auto_type right___ = (right);                                            \
    list_iter_swap(list_iter(left___), list_iter(right___));                   \
  } while (0)

#define list_merge(left, right, data_compare)                                  \
  ({                                                                           \
    __auto_type left___ = (left);                                              \
    __auto_type right___ = (right);                                            \
    __auto_type comp___ = (data_compare);                                      \
    __auto_type new_list = list_init(list_type(left___));                      \
    __auto_type left_iter = list_begin(left___);                               \
    __auto_type right_iter = list_begin(right___);                             \
    __auto_type merge_iter = list_begin(new_list);                             \
    while (left_iter != list_end(left___) &&                                   \
           right_iter != list_end(right___)) {                                 \
      if (comp___(&list_data(left___, left_iter),                              \
                  &list_data(right___, right_iter)) < 0) {                     \
        list_iter_concat(merge_iter, right_iter);                              \
        right_iter = right_iter->next;                                         \
      } else {                                                                 \
        list_iter_concat(merge_iter, left_iter);                               \
        left_iter = left_iter->next;                                           \
      }                                                                        \
      merge_iter = merge_iter->next;                                           \
    }                                                                          \
    if (left_iter != list_end(left___)) {                                      \
      list_iter_concat(merge_iter, left_iter);                                 \
      list_iter_concat(list_rbegin(left___), list_begin(new_list));            \
    }                                                                          \
    if (right_iter != list_end(right___)) {                                    \
      list_iter_concat(merge_iter, right_iter);                                \
      list_iter_concat(list_rbegin(right___), list_begin(new_list));           \
    }                                                                          \
  })

#define list_sort(list, data_compare)                                          \
  do {                                                                         \
    __auto_type list___ = (list);                                              \
    __auto_type comp___ = (data_compare);                                      \
    __auto_type carry = list_init(list_type(list___));                         \
    typeof(list___) tmp[64];                                                   \
    for (int i = 0; i < 64; i++) {                                             \
      tmp[i] = list_init(list_type(list___));                                  \
    }                                                                          \
    typeof(list___) *fill = tmp;                                               \
    typeof(list___) *counter;                                                  \
    do {                                                                       \
      __auto_type node = list_begin(list);                                     \
      list_erase(list, node);                                                  \
      list_iter_insert(list_iter(carry), list_iter(carry)->next, node);        \
      for (counter = tmp; counter != fill && !list_empty(*counter);            \
           ++counter) {                                                        \
        *counter = list_merge(*counter, carry, comp___);                       \
        list_swap(*counter, carry);                                            \
      }                                                                        \
      list_swap(*counter, carry);                                              \
      if (counter == fill) {                                                   \
        fill++;                                                                \
      }                                                                        \
    } while (!list_empty(list___));                                            \
    for (counter = tmp + 1; counter != fill; ++counter) {                      \
      list_merge(*counter, *(counter - 1), comp___);                           \
    }                                                                          \
    list_swap(list___, *(fill - 1));                                           \
  } while (0)

#define list_access(list, func, ...)                                           \
  do {                                                                         \
    __auto_type begin___ = list_begin(list);                                   \
    __auto_type end___ = list_end(list);                                       \
    __auto_type func___ = (func);                                              \
    for (__auto_type iter = begin___; iter != end___; iter = iter->next) {     \
      func___(&list_data(list, iter), __VA_ARGS__);                            \
    }                                                                          \
  } while (0)

#define list_trivial_destroy(list)                                             \
  do {                                                                         \
    __auto_type list___ = (list);                                              \
    __auto_type begin = list_begin(list___);                                   \
    __auto_type end = list_end(list___);                                       \
    for (__auto_type iter = begin; iter != end;) {                             \
      __auto_type victim = iter;                                               \
      iter = iter->next;                                                       \
      list_free(victim);                                                       \
    }                                                                          \
    list_free(list);                                                           \
  } while (0)

#define list_destroy(list, data_destroy, ...)                                  \
  do {                                                                         \
    for (__auto_type iter = list_begin(list); iter != list_end(list);) {       \
      __auto_type victim = iter;                                               \
      iter = iter->next;                                                       \
      data_destroy(&list_data(list, iter), __VA_ARGS__);                       \
      list_free(victim);                                                       \
    }                                                                          \
    list_free(list);                                                           \
  } while (0)

#endif // LIST_H
