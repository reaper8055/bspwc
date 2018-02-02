project(
	'bspwc',
	'c',
	version: '0.0.1',
	license: 'MIT',
	meson_version: '>=0.43.0',
	default_options: [
		'c_std=c11',
		'warning_level=3',
		'werror=true',
	],
)

add_project_arguments('-Wno-unused-parameter', language: 'c')

if (get_option('buildtype') == 'release')
  add_project_arguments('-D_FORTIFY_SOURCE=2', language: 'c')
endif

inc_folder = include_directories('include')

cc = meson.get_compiler('c')

wlroots = dependency('wlroots')
wayland_server = dependency('wayland-server')
wayland_client = dependency('wayland-client')
pixman = dependency('pixman-1')

subdir('bspwc')
