#include "bspwc/xdg_surface_v6.h"

void handle_xdg_surface_v6_commit(struct wl_listener *listener, void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 commit");
}

void handle_xdg_surface_v6_destroy(struct wl_listener *listener, void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 destroy");
}

void handle_xdg_surface_v6_new_popup(struct wl_listener *listener, void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 new poopup");
}

void handle_xdg_surface_v6_map(struct wl_listener *listener, void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 map");
}

void handle_xdg_surface_v6_unmap(struct wl_listener *listener, void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 unmap");
}

void handle_xdg_surface_v6_request_move(struct wl_listener *listener,
		void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 request move");
}

void handle_xdg_surface_v6_request_resize(struct wl_listener *listener,
		void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 request resize");
}

void handle_xdg_surface_v6_request_maximize(struct wl_listener *listener,
		void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 request maximize");
}

void handle_xdg_surface_v6_request_fullscreen(struct wl_listener *listener,
		void *data)
{
	wlr_log(WLR_INFO, "Handle xdg surface v6 request fullscreen");
}

void handle_xdg_shell_v6_surface(struct wl_listener *listener, void *data);

void handle_xdg_shell_v6_surface(struct wl_listener *listener, void *data)
{
	struct wlr_xdg_surface_v6 *wlr_xdg_surface_v6 = data;
	assert(wlr_xdg_surface_v6->role != WLR_XDG_SURFACE_V6_ROLE_NONE);

	if (wlr_xdg_surface_v6->role == WLR_XDG_SURFACE_V6_ROLE_POPUP)
	{
		return;
	}

	char *title = wlr_xdg_surface_v6->toplevel->title;

	wlr_log(WLR_INFO, "New xdg_v6 toplevel: title=%s, app_id=%s", title,
			wlr_xdg_surface_v6->toplevel->app_id);
	wlr_xdg_surface_v6_ping(wlr_xdg_surface_v6);

	struct xdg_surface_v6 *xdg_surface_v6 =
		calloc(1, sizeof(struct xdg_surface_v6));
	if (xdg_surface_v6 == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create surface for %s", title);
		return;
	}

	// Wire up handlers and signals
	xdg_surface_v6->surface_commit.notify = handle_xdg_surface_v6_commit;
	wl_signal_add(&wlr_xdg_surface_v6->surface->events.commit,
			&xdg_surface_v6->surface_commit);

	xdg_surface_v6->destroy.notify = handle_xdg_surface_v6_destroy;
	wl_signal_add(&wlr_xdg_surface_v6->events.destroy,
			&xdg_surface_v6->destroy);

	xdg_surface_v6->new_popup.notify = handle_xdg_surface_v6_new_popup;
	wl_signal_add(&wlr_xdg_surface_v6->events.new_popup,
			&xdg_surface_v6->new_popup);

	xdg_surface_v6->map.notify = handle_xdg_surface_v6_map;
	wl_signal_add(&wlr_xdg_surface_v6->events.map, &xdg_surface_v6->map);

	xdg_surface_v6->unmap.notify = handle_xdg_surface_v6_unmap;
	wl_signal_add(&wlr_xdg_surface_v6->events.unmap, &xdg_surface_v6->unmap);

	xdg_surface_v6->request_move.notify = handle_xdg_surface_v6_request_move;
	wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_move,
			&xdg_surface_v6->request_move);

	xdg_surface_v6->request_resize.notify =
			handle_xdg_surface_v6_request_resize;
	wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_resize,
			&xdg_surface_v6->request_resize);

	xdg_surface_v6->request_maximize.notify =
			handle_xdg_surface_v6_request_maximize;
	wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_maximize,
			&xdg_surface_v6->request_maximize);

	xdg_surface_v6->request_fullscreen.notify =
			handle_xdg_surface_v6_request_fullscreen;
	wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_fullscreen,
			&xdg_surface_v6->request_fullscreen);

	struct backend* backend = wl_container_of(listener, backend,
			new_xdg_shell_v6);
	if (backend == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to retrieve backend");
		return;
	}

	const struct server *server = backend->server;
	struct output *output = get_current_output(server);
	if (output == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to get current output");
		return;
	}

	// Create xdg_v6 window
	struct window* window = create_window();
	if (window == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create window for %s", title);
		free(xdg_surface_v6);
		return;
	}

	window->title = title;
	window->desktop = output->desktop;

	// Fill window with xdg_v6 related stuff
	window->type = XDG_SHELL_V6;
	window->wlr_xdg_surface_v6 = wlr_xdg_surface_v6;
	window->xdg_surface_v6 = xdg_surface_v6;
	xdg_surface_v6->window = window;

	struct node *child = create_node();
	child->window = window;

	if (insert_node(server, &output->desktop->root, child) == false)
	{
		wlr_log(WLR_ERROR, "Failed to insert window into desktop");
		destroy_window(window);
	}
}
