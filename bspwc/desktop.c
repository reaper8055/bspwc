#include "bspwc/desktop.h"

struct desktop* desktop_create(struct server* s, const char* name)
{
    wlr_log(L_DEBUG, "Creating desktop %s", name);

    struct desktop* d = calloc(1, sizeof(struct desktop));
    if (d == NULL)
    {
        wlr_log(L_ERROR, "Could not create desktop %s", name);
        return NULL;
    }

    d->server = s;

    strcpy(d->name, (name == NULL ? "default_monitor" : name));

    // Init links
    wl_list_init(&d->monitor);
    wl_list_init(&d->windows);

    // Setup window callbacks
    d->window_add.notify = window_add_notify;
    d->window_remove.notify = window_remove_notify;

    // Layout arranges monitors relative to each other in physical space
    // using global coordinates systems to do operations on boxes
    d->layout = wlr_output_layout_create();
    d->layout_change.notify = handle_layout_change;
    wl_signal_add(&d->layout->events.change, &d->layout_change);
   
    d->compositor = wlr_compositor_create(s->display, s->renderer);

    d->xdg_shell_v6 = wlr_xdg_shell_v6_create(s->display);
    wl_signal_add(&d->xdg_shell_v6->events.new_surface, &d->xdg_shell_v6_surface);
    d->xdg_shell_v6_surface.notify = handle_xdg_shell_v6_surface;
    
    d->wl_shell = wlr_wl_shell_create(s->display);
    wl_signal_add(&d->wl_shell->events.new_surface, &d->wl_shell_surface);
    d->wl_shell_surface.notify = handle_wl_shell_surface;


    return d;
}
