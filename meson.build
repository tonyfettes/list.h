project(
  'list.h',
  ['c', 'cpp'],
  default_options: [
    'c_std=gnu2x',
    'warning_level=3',
    'b_ndebug=if-release',
  ]
)

list_incdir = include_directories('src')
install_headers('src/list.h')
list = declare_dependency(include_directories: list_incdir)

subdir('bench')
subdir('test')
subdir('examples')
