#ifndef DESKTOP_H
#define DESKTOP_H

#include <sys/types.h>

#include <wlr/types/wlr_list.h>
#include <wlr/types/wlr_wl_shell.h>
#include <wlr/types/wlr_xdg_shell_v6.h>

#include "bspwc/server.h"

struct padding
{
	int top;
	int right;
	int bottom;
	int left;
};

struct desktop
{
    struct wl_list views;
    struct wl_list outputs;

    struct wlr_output_layout* layout;
    struct wlr_wl_shell *wl_shell;
    struct wlr_xdg_shell_v6 *xdg_shell_v6;

	struct wl_listener output_add;
	struct wl_listener output_remove;
	struct wl_listener layout_change;
	struct wl_listener xdg_shell_v6_surface;
	struct wl_listener wl_shell_surface;
	struct wl_listener decoration_new;
	struct wl_listener xwayland_surface;
    struct wl_listener xwayland_ready;

    // BSPWM stuff
    char name[32];
    struct desktop* prev;
    struct desktop* next;
    struct padding padding;
};

void output_add_notify(struct wl_listener *listener, void *data);
void output_remove_notify(struct wl_listener *listener, void *data);

void handle_xdg_shell_v6_surface(struct wl_listener* listener, void* data);
void handle_wl_shell_surface(struct wl_listener* listener, void* data);
void handle_xwayland_surface(struct wl_listener* listener, void* data);
void handle_layout_change(struct wl_listener* listener, void* data);

struct desktop* desktop_create(struct server* s);

#endif // DESKTOP_H
