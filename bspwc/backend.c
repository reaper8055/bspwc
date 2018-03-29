#include "bspwc/backend.h"

struct backend* create_backend(struct server* server)
{
    wlr_log(L_INFO, "Creating bspwc backend");

    struct backend* backend = calloc(1, sizeof(struct backend));

    backend->server = server;

    backend->wl_display = wl_display_create();
    assert(backend->wl_display);

    backend->wl_event_loop = wl_display_get_event_loop(backend->wl_display);
    assert(backend->wl_event_loop);

    backend->wlr_backend = wlr_backend_autocreate(backend->wl_display);
    assert(backend->wlr_backend);

    wl_list_init(&backend->outputs);
    backend->new_output.notify = new_output_notify;
    wl_signal_add(&backend->wlr_backend->events.new_output, &backend->new_output);

    wl_display_init_shm(backend->wl_display);
    wlr_gamma_control_manager_create(backend->wl_display);
    wlr_screenshooter_create(backend->wl_display);
    wlr_primary_selection_device_manager_create(backend->wl_display);
    wlr_idle_create(backend->wl_display);

    backend->wlr_compositor = wlr_compositor_create(
            backend->wl_display,
            wlr_backend_get_renderer(backend->wlr_backend)
        );

    wlr_xdg_shell_v6_create(backend->wl_display);

    return backend;
}

void destroy_backend(struct backend* backend)
{
    wlr_backend_destroy(backend->wlr_backend);
    wl_display_destroy(backend->wl_display);

    free(backend);
}
