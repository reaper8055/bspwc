#include "bspwc/xdg_surface_v6.h"

void handle_surface_commit(struct wl_listener* listener, void* data)
{}

void handle_destroy(struct wl_listener* listener, void* data)
{}

void handle_new_popup(struct wl_listener* listener, void* data)
{}

void handle_map(struct wl_listener* listener, void* data)
{}

void handle_unmap(struct wl_listener* listener, void* data)
{}

void handle_request_move(struct wl_listener* listener, void* data)
{}

void handle_request_resize(struct wl_listener* listener, void* data)
{}

void handle_request_maximize(struct wl_listener* listener, void* data)
{}

void handle_request_fullscreen(struct wl_listener* listener, void* data)
{}

void handle_xdg_shell_v6_surface(struct wl_listener* listener, void* data)
{
    struct wlr_xdg_surface_v6* wlr_xdg_surface_v6 = data;
    assert(wlr_xdg_surface_v6->role != WLR_XDG_SURFACE_V6_ROLE_NONE);

    if (wlr_xdg_surface_v6->role == WLR_XDG_SURFACE_V6_ROLE_POPUP)
    {
        wlr_log(L_DEBUG, "new xdg v6 popup");
        return;
    }

    //struct backend* backend = wl_container_of(listener, backend, xdg_shell_v6_surface);

    wlr_log(L_DEBUG, "new xdg v6 toplevel: title=%s, app_id=%s",
            wlr_xdg_surface_v6->toplevel->title, wlr_xdg_surface_v6->toplevel->app_id
        );
    wlr_xdg_surface_v6_ping(wlr_xdg_surface_v6);

    struct xdg_surface_v6* surface = calloc(1, sizeof(struct xdg_surface_v6));

    surface->surface_commit.notify = handle_surface_commit;
    wl_signal_add(&wlr_xdg_surface_v6->surface->events.commit, &surface->surface_commit);

    surface->destroy.notify = handle_destroy;
    wl_signal_add(&wlr_xdg_surface_v6->events.destroy, &surface->destroy);

    surface->new_popup.notify = handle_new_popup;
    wl_signal_add(&wlr_xdg_surface_v6->events.new_popup, &surface->new_popup);

    surface->map.notify = handle_map;
    wl_signal_add(&wlr_xdg_surface_v6->events.map, &surface->map);

    surface->unmap.notify = handle_unmap;
    wl_signal_add(&wlr_xdg_surface_v6->events.unmap, &surface->unmap);

    surface->request_move.notify = handle_request_move;
    wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_move, &surface->request_move);

    surface->request_resize.notify = handle_request_resize;
    wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_resize, &surface->request_resize);

    surface->request_maximize.notify = handle_request_maximize;
    wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_maximize, &surface->request_maximize);

    surface->request_fullscreen.notify = handle_request_fullscreen;
    wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_fullscreen, &surface->request_fullscreen);

    struct window* window = create_window();
    if (window == NULL)
    {
        free(surface);
        return;
    }

    window->type = XDG_SHELL_V6;

    window->wlr_xdg_surface_v6 = wlr_xdg_surface_v6;
    window->xdg_surface_v6 = surface;
}
