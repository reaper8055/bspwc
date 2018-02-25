#include "bspwc/handles.h"

void handle_xdg_shell_v6_surface(struct wl_listener *listener, void *data)
{}

void handle_wl_shell_surface(struct wl_listener *listener, void *data)
{}

void handle_xwayland_surface(struct wl_listener *listener, void *data)
{
    //struct wlr_xwayland_surface* s = data;

    //wlr_log(L_DEBUG, "new xwayland surface: title=%s, class=%s, instance=%s", s->title, s->class, s->instance);

}

void handle_layout_change(struct wl_listener *listener, void *data)
{}
