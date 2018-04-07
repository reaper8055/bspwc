#ifndef XDG_SHELL_V6_H
#define XDG_SHELL_V6_H

#include <assert.h>
#include <wayland-server.h>
#include <wlr/types/wlr_xdg_shell_v6.h>
#include <wlr/util/log.h>

#include "bspwc/server.h"

void handle_xdg_shell_v6_surface(struct wl_listener* listener, void* data);

#endif // XDG_SHELL_V6_H
