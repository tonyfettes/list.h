# `list.h`

Linked list library implemented in GNU C.

## Usage

```c
#include "list.h"

int main() {
  __auto_type list = list_init(int);
  list_push_back(list, 3);
  return 0;
}
```
