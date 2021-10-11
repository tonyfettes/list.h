#include "list.h"

#define auto __auto_type

int main() {
  auto *int_list = list_init(int);
  list_push_back(int_list, 3);
  list_push_back(int_list, 4);
  return 0;
}
