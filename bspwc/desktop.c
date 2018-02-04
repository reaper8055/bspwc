#include "bspwc/desktop.h"

void output_add_notify(struct wl_listener *listener, void *data)
{}

void output_remove_notify(struct wl_listener *listener, void *data)
{}

void handle_xdg_shell_v6_surface(struct wl_listener* listener, void* data)
{}

void handle_wl_shell_surface(struct wl_listener* listener, void* data)
{}

void handle_xwayland_surface(struct wl_listener* listener, void* data)
{}

void handle_layout_change(struct wl_listener* listener, void* data)
{}

struct desktop* desktop_create(struct server* s)
{
    wlr_log(L_DEBUG, "Initializing desktop");

    struct desktop* d = calloc(1, sizeof(struct desktop));
    if (d == NULL)
    {
        return NULL;
    }

    wl_list_init(&d->views);
    wl_list_init(&d->outputs);

    d->output_add.notify = output_add_notify;
	wl_signal_add(&s->backend->events.output_add, &d->output_add);
	d->output_remove.notify = output_remove_notify;
	wl_signal_add(&s->backend->events.output_remove, &d->output_remove);

    d->layout = wlr_output_layout_create();
	d->layout_change.notify = handle_layout_change;
    wl_signal_add(&d->layout->events.change, &d->layout_change);

	d->wl_shell = wlr_wl_shell_create(s->display);
	wl_signal_add(&d->wl_shell->events.new_surface, &d->wl_shell_surface);
    d->wl_shell_surface.notify = handle_wl_shell_surface;

    d->xdg_shell_v6 = wlr_xdg_shell_v6_create(s->display);
	wl_signal_add(&d->xdg_shell_v6->events.new_surface,	&d->xdg_shell_v6_surface);
	d->xdg_shell_v6_surface.notify = handle_xdg_shell_v6_surface;

    wl_signal_add(&s->xwayland->events.new_surface, &d->xwayland_surface);
    d->xwayland_surface.notify = handle_xwayland_surface;

    return d;
}
