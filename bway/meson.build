bway_src = files(
    'main.c',
    'config.c',
    'server.c',
)

bway_deps = [
    wayland_server,
    wlroots,
]

executable(
    'bway',
    bway_src,
    include_directories: [inc_folder],
    dependencies: bway_deps,
    install: true 
)
