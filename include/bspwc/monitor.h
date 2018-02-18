#ifndef MONITOR_H
#define MONITOR_H

#include <stdlib.h>
#include <time.h>

#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_list.h>
#include <wlr/util/log.h>

#include "bspwc/server.h"

struct monitor
{
    char name[64];
    struct wlr_output* wlr_output;
    struct wl_list desktops;
    struct wl_listener frame;
    struct timespec last_frame;
};

struct monitor* monitor_create(struct server* s, const char* name);

#endif // MONITOR_H
