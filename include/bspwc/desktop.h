#ifndef DESKTOP_H
#define DESKTOP_H

#include <wayland-server.h>

#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_wl_shell.h>
#include <wlr/types/wlr_xdg_shell_v6.h>

#include "bspwc/handles.h"
#include "bspwc/server.h"
#include "bspwc/window.h"

struct desktop
{
    char name[16];
    struct server* server;

    struct wl_list monitor_link;
    struct wl_list windows;
    
    struct wl_listener window_add;
    struct wl_listener window_remove;
};

struct desktop* desktop_create(struct server* s, const char* name);
void desktop_destroy(struct desktop* d);


#endif // DESKTOP_H
