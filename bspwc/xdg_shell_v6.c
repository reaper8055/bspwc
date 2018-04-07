#include "bspwc/xdg_shell_v6.h"

void handle_xdg_shell_v6_surface(struct wl_listener* listener, void* data)
{
    struct wlr_xdg_surface_v6* surface = data;
    assert(surface->role != WLR_XDG_SURFACE_V6_ROLE_NONE);

    if (surface->role == WLR_XDG_SURFACE_V6_ROLE_POPUP)
    {
        wlr_log(L_DEBUG, "new xdg v6 popup");
        return;
    }

    struct server* server = wl_container_of(listener, server, xdg_shell_v6_surface);

    wlr_log(L_DEBUG, "new xdg v6 toplevel: title=%s, app_id=%s", surface->toplevel->title, surface->toplevel->app_id);
    wlr_xdg_surface_v6_ping(surface);
}
