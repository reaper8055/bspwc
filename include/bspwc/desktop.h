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
    char name[64];
    struct server* server;

    struct wl_list monitor;
    struct wl_list windows;
    
    struct wl_listener window_add;
    struct wl_listener window_remove;

    struct wlr_output_layout* layout;
    struct wl_listener layout_change;
    
    struct wlr_compositor* compositor;

    struct wlr_xdg_shell_v6* xdg_shell_v6;
    struct wl_listener xdg_shell_v6_surface;
    struct wlr_wl_shell* wl_shell;
    struct wl_listener wl_shell_surface;
};

struct desktop* desktop_create(struct server* s, const char* name);
bool desktop_destroy();


#endif // DESKTOP_H
