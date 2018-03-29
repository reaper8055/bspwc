#ifndef BACKEND_H
#define BACKEND_H

#include <wayland-server.h>

#include <wlr/backend.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_gamma_control.h>
#include <wlr/types/wlr_idle.h>
#include <wlr/types/wlr_primary_selection.h>
#include <wlr/types/wlr_screenshooter.h>
#include <wlr/types/wlr_xdg_shell_v6.h>

#include "bspwc/output.h"
#include "bspwc/server.h"

struct backend
{
    struct server* server;

    struct wl_display* wl_display;
    struct wl_event_loop* wl_event_loop;
    struct wl_event_source* wl_event_source;

    struct wlr_backend* wlr_backend;
    struct wlr_compositor* wlr_compositor;

    struct wl_listener new_output;
    struct wl_list outputs; // output::link
};

struct backend* create_backend(struct server* server);
void destroy_backend(struct backend* backend);

#endif // BACKEND_H
