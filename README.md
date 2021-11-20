**DEPRECATED**

**See
[tllist](https://codeberg.org/dnkl/tllist/src/branch/master)**

# `list.h`

Linked list library implemented in GNU C.

## Usage

```c
#include "list.h"

int main() {
  __auto_type list = list_init(int);
  list_push_back(list, 3);
  assert(list_back(list), 3);
  assert(list_size(list), 1);
  return 0;
}
```

## Features

- template with `__auto_type`
- access/push/pop back/front
- merge sort
- reverse: O(1) (by type-casting) and O(n)
- customizable `malloc`/`free` by defining `list_alloc`/`list_free`

## Performance

`list_sort` in this implementation is around 10% faster than `std::list::sort` in c++.
