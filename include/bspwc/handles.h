#ifndef HANDLES_H
#define HANDLES_H

#include <wayland-server.h>

void handle_xdg_shell_v6_surface(struct wl_listener *listener, void *data);
void handle_wl_shell_surface(struct wl_listener *listener, void *data);
void handle_xwayland_surface(struct wl_listener *listener, void *data);
void handle_layout_change(struct wl_listener *listener, void *data);

#endif // HANDLES_H
