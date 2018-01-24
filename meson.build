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

inc_folder = include_directories('include')

cc = meson.get_compiler('c')

wlroots = dependency('wlroots')
wayland_server = dependency('wayland-server')
wayland_client = dependency('wayland-client')
pixman = dependency('pixman-1')

subdir('bspwc')
