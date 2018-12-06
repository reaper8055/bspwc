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

	//struct wl_listener surface_commit;
};

void create_xdg_surface(struct wl_listener *listener, void *data);
void destroy_xdg_surface(struct wl_listener *listener, void *data);
void new_popup_xdg_surface(struct wl_listener *listener, void *data);
void map_xdg_surface(struct wl_listener *listener, void *data);
void unmap_xdg_surface(struct wl_listener *listener, void *data);
void request_move_xdg_surface(struct wl_listener *listener, void *data);
void request_resize_xdg_surface(struct wl_listener *listener, void *data);
void request_maximize_xdg_surface(struct wl_listener *listener, void *data);
void request_fullscreen_xdg_surface(struct wl_listener *listener, void *data);

#endif // SHELL_XDG_SURFACE_H
