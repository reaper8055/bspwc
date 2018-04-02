#include "bspwc/backend.h"

static void handle_wl_shell_surface(struct wl_listener* listener, void* data)
{
    struct wlr_wl_shell_surface* surface = data;

    if (surface->state == WLR_WL_SHELL_SURFACE_STATE_POPUP)
    {
        wlr_log(L_DEBUG, "New wl shell popup");
        return;
    }

    wlr_log(L_DEBUG, "New wl shell surface: title : %s, class : %s", surface->title, surface->class);
    wlr_wl_shell_surface_ping(surface);
}

static void handle_xdg_shell_surface(struct wl_listener* listener, void* data)
{
    struct wlr_xdg_surface* surface = data;
    assert(surface->role != WLR_XDG_SURFACE_ROLE_NONE);

    if (surface->role == WLR_XDG_SURFACE_ROLE_POPUP)
    {
        wlr_log(L_DEBUG, "New xdg popup");
        return;
    }

    wlr_log(
            L_DEBUG,
            "New xdg surface: title : %s, app_id : %s",
            surface->toplevel->title,
            surface->toplevel->app_id
        );
    wlr_xdg_surface_ping(surface);
}

static void handle_xdg_shell_v6_surface(struct wl_listener* listener, void* data)
{
    struct wlr_xdg_surface_v6* surface = data;
    assert(surface->role != WLR_XDG_SURFACE_V6_ROLE_NONE);

    if (surface->role == WLR_XDG_SURFACE_V6_ROLE_POPUP)
    {
        wlr_log(L_DEBUG, "New xdg v6 popup");
        return;
    }

    wlr_log(
            L_DEBUG,
            "New xdg v6 surface: title : %s, app_id : %s",
            surface->toplevel->title,
            surface->toplevel->app_id
        );
    wlr_xdg_surface_v6_ping(surface);
}

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
    backend->wlr_gamma_control_manager = wlr_gamma_control_manager_create(backend->wl_display);
    backend->wlr_screenshooter = wlr_screenshooter_create(backend->wl_display);
    backend->wlr_primary_selection_device_manager = wlr_primary_selection_device_manager_create(backend->wl_display);
    backend->wlr_idle = wlr_idle_create(backend->wl_display);
    backend->wlr_idle_inhibit = wlr_idle_inhibit_v1_create(backend->wl_display);

    struct wlr_egl* wlr_egl = wlr_backend_get_egl(backend->wlr_backend);
    backend->wlr_linux_dmabuf = wlr_linux_dmabuf_create(backend->wl_display, wlr_egl);

    backend->wl_shell = wlr_wl_shell_create(backend->wl_display);
    wl_signal_add(&backend->wl_shell->events.new_surface, &backend->wl_shell_surface);
    backend->wl_shell_surface.notify = handle_wl_shell_surface;

    backend->xdg_shell = wlr_xdg_shell_create(backend->wl_display);
    wl_signal_add(&backend->xdg_shell->events.new_surface, &backend->xdg_shell_surface);
    backend->xdg_shell_surface.notify = handle_xdg_shell_surface;

    backend->xdg_shell_v6 = wlr_xdg_shell_v6_create(backend->wl_display);
    wl_signal_add(&backend->xdg_shell_v6->events.new_surface, &backend->xdg_shell_v6_surface);
    backend->xdg_shell_surface.notify = handle_xdg_shell_v6_surface;

    backend->wlr_compositor = wlr_compositor_create(
            backend->wl_display,
            wlr_backend_get_renderer(backend->wlr_backend)
        );

    return backend;
}

void destroy_backend(struct backend* backend)
{
    wlr_backend_destroy(backend->wlr_backend);
    wl_display_destroy(backend->wl_display);

    free(backend);
}
