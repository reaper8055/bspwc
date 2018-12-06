#ifndef SHELL_XDG_SURFACE_H
#define SHELL_XDG_SURFACE_H

#include <assert.h>
#include <wayland-server.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

#include "bspwc/window.h"

struct xdg_surface
{
	struct window *window;

	struct wl_listener destroy;
	struct wl_listener new_popup;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener request_move;
	struct wl_listener request_resize;
	struct wl_listener request_maximize;
	struct wl_listener request_fullscreen;

	struct wl_listener surface_commit;
};

#endif // SHELL_XDG_SURFACE_H
