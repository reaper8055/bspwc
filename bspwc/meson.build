bspwc_src = files(
    'main.c',
    'config.c',
    'server.c',
    'monitor.c',
    'desktop.c',
    'window.c',
    'handles.c',
)

bspwc_deps = [
    wayland_server,
    wlroots,
    pixman,
]

executable(
    'bspwc',
    bspwc_src,
    include_directories: [inc_folder],
    dependencies: bspwc_deps,
    install: true 
)
