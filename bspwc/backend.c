#include "bspwc/backend.h"

struct backend* create_backend(struct server* server)
{
    wlr_log(L_INFO, "Creating bspwc backend");

    struct backend* backend = calloc(1, sizeof(struct backend));

    backend->server = server;

    backend->wlr_backend = wlr_backend_autocreate(server->wl_display);
    assert(backend->wlr_backend);

    wl_list_init(&backend->outputs);
    backend->new_output.notify = new_output_notify;
    wl_signal_add(&backend->wlr_backend->events.new_output, &backend->new_output);

    wl_display_init_shm(server->wl_display);
    backend->wlr_gamma_control_manager = wlr_gamma_control_manager_create(server->wl_display);
    backend->wlr_screenshooter = wlr_screenshooter_create(server->wl_display);
    backend->wlr_primary_selection_device_manager = wlr_primary_selection_device_manager_create(server->wl_display);
    backend->wlr_idle = wlr_idle_create(server->wl_display);
    backend->wlr_idle_inhibit = wlr_idle_inhibit_v1_create(server->wl_display);

    struct wlr_egl* wlr_egl = wlr_backend_get_egl(backend->wlr_backend);
    backend->wlr_linux_dmabuf = wlr_linux_dmabuf_create(server->wl_display, wlr_egl);

    backend->wlr_compositor = wlr_compositor_create(
            server->wl_display,
            wlr_backend_get_renderer(backend->wlr_backend)
        );

    // Init shells
    backend->wlr_xdg_shell_v6 = wlr_xdg_shell_v6_create(server->wl_display);
    wl_signal_add(&backend->wlr_xdg_shell_v6->events.new_surface, &backend->xdg_shell_v6_surface);
    backend->xdg_shell_v6_surface.notify = handle_xdg_shell_v6_surface;

    return backend;
}

void destroy_backend(struct backend* backend)
{
    wlr_backend_destroy(backend->wlr_backend);

    free(backend);
}
