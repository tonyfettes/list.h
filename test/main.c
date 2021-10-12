#include "list.h"
#include <stdlib.h>

int main() {
  __auto_type l = list_init(int);
  list_push_back(l, 3);
  return 0;
}
