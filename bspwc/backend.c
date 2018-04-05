#include "bspwc/backend.h"

struct backend* create_backend(struct server* server)
{
    wlr_log(L_INFO, "Creating bspwc backend");

    struct backend* backend = calloc(1, sizeof(struct backend));

    backend->server = server;

    backend->wlr_backend = wlr_backend_autocreate(server->wl_display);
    assert(backend->wlr_backend);

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

    wl_signal_add(&backend->wlr_backend->events.new_output, &server->new_output);

    return backend;
}

void destroy_backend(struct backend* backend)
{
    wlr_backend_destroy(backend->wlr_backend);

    free(backend);
}

void render_surface(struct wlr_output* wlr_output, struct wlr_surface* surface, const int x, const int y)
{
    if (!wlr_surface_has_buffer(surface))
    {
        return;
    }

    struct wlr_renderer* renderer = wlr_backend_get_renderer(wlr_output->backend);

    struct wlr_box render_box = {
        .x = x,
        .y = y,
        .width = surface->current->width,
        .height = surface->current->height
    };

    float matrix[16];
    wlr_matrix_project_box(
            matrix,
            &render_box,
            surface->current->transform,
            0,
            wlr_output->transform_matrix
        );

    wlr_render_texture_with_matrix(renderer, surface->texture, matrix, 1.0f);

    struct wlr_subsurface* subsurface;
    wl_list_for_each(subsurface, &surface->subsurface_list, parent_link)
    {
        struct wlr_surface_state *state = subsurface->surface->current;
        int sx = state->subsurface_position.x;
        int sy = state->subsurface_position.y;

        render_surface(wlr_output, subsurface->surface, x + sx, y + sy);
    }

}
