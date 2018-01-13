project(
  'bway',  
  'c',
  license: 'MIT',
  default_options: [
    'c_std=c11',
    'warning_level=3',
    'werror=false',
  ],
)

cc = meson.get_compiler('c')

wlroots = dependency('wlroots')

bway_incl = include_directories('include')

subdir('bway')
