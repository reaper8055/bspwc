bspwc_src = files(
    'main.c',
    'config.c',
    'desktop.c',
    'server.c',
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
