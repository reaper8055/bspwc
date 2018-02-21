#ifndef MONITOR_H
#define MONITOR_H

// Defined in main.c, needed for time.h
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdlib.h>
#include <time.h>

#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_list.h>
#include <wlr/render.h>
#include <wlr/util/log.h>

#include "bspwc/server.h"

struct monitor
{
    struct server* server;
    struct wlr_output* wlr_output;
    struct wl_list desktops;
    float color[4];
    struct timespec last_frame;

    struct wl_listener destroy;
    struct wl_listener frame;

    struct wl_list server_link;
};

void new_monitor_notify(struct wl_listener* listener, void* data);
void monitor_destroy_notify(struct wl_listener* listener, void* data);
void monitor_frame_notify(struct wl_listener* listener, void* data);

#endif // MONITOR_H
