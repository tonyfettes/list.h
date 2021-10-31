self_impl = executable(
  'self_impl',
  [
    'src/bench.h',
    'src/bench_size.h',
    'src/list_h_sort.c',
    'src/main.c'],
  dependencies: [list],
  c_args: [
    '-O3',
    '-flto',
  ],
  link_args: [
    '-flto',
  ]
)

std_impl = executable(
  'std_list_sort',
  [
    'src/bench.h',
    'src/bench_size.h',
    'src/std_list_sort.cpp',
    'src/main.c'],
  dependencies: [list],
  cpp_args: [
    '-O3',
    '-flto',
  ],
  link_args: [
    '-flto',
  ],
)
