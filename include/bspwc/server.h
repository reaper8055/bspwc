#ifndef SERVER_H
#define SERVER_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <wayland-server.h>
#include <wlr/backend.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_gamma_control.h>
#include <wlr/types/wlr_idle.h>
#include <wlr/types/wlr_primary_selection.h>
#include <wlr/types/wlr_screenshooter.h>
#include <wlr/types/wlr_xdg_shell_v6.h>
#include <wlr/util/log.h>

#include "bspwc/common.h"
#include "bspwc/output.h"

struct server
{
    struct wl_display* wl_display;
    struct wl_event_loop* wl_event_loop;
    struct wl_event_source* wl_event_source;

    struct wlr_backend* wlr_backend;
    struct wlr_compositor* wlr_compositor;

    struct wl_listener new_output;
    struct wl_list outputs; // output::link

    // BSPWM related stuff
    int socket;
    char* socket_name;
};

bool init_server(struct server* server);
bool setup_server(struct server* server);
bool start_server(struct server* server);
bool terminate_server(struct server* server);

#endif // SERVER_H
