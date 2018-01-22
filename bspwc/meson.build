bspwc_src = files(
    'main.c',
    'config.c',
    'desktop.c',
    'bspwc.c',
)

bspwc_deps = [
    wayland_server,
    wlroots,
]

executable(
    'bspwc',
    bspwc_src,
    include_directories: [inc_folder],
    dependencies: bspwc_deps,
    install: true 
)
