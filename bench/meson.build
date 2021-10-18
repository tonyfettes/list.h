c = executable(
  'list_h_sort',
  'list_h_sort.c',
  include_directories: incdir,
  c_args: [
    '-O3'
  ],
)

cpp = executable(
  'std_list_sort',
  'std_list_sort.cpp',
  include_directories: incdir,
  cpp_args: [
    '-O3',
    '-flto',
  ],
  link_args: [
    '-flto',
  ],
)
