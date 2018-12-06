#include "bspwc/shell/xdg_surface.h"

void create_xdg_surface(struct wl_listener *listener, void *data)
{
	struct backend *backend = wl_container_of(listener, backend,
			new_xdg_surface);
	assert(backend);

	struct wlr_xdg_surface *wlr_xdg_surface = data;
	assert(wlr_xdg_surface);

	// Only toplevel surfaces are hand
	if (wlr_xdg_surface->role != WLR_XDG_SURFACE_ROLE_TOPLEVEL)
	{
		return;
	}

	// FIXME: find a better way to solve the geometry problem
	wlr_xdg_toplevel_set_maximized(wlr_xdg_surface, true);

	char *title = wlr_xdg_surface->toplevel->title;
	wlr_log(WLR_INFO, "New xdg surface: title=%s, app_id=%s", title,
			wlr_xdg_surface->toplevel->app_id);

	//wlr_xdg_surface_ping(wlr_xdg_surface); // TODO: needed?

	struct xdg_surface *xdg_surface = malloc(sizeof(struct xdg_surface));
	if (xdg_surface == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create surface for %s", title);
		return;
	}

	// Wire up signal handlers
	xdg_surface->destroy.notify = destroy_xdg_surface;
	wl_signal_add(&wlr_xdg_surface->events.map, &xdg_surface->map);

	xdg_surface->new_popup.notify = new_popup_xdg_surface;
	wl_signal_add(&wlr_xdg_surface->events.new_popup, &xdg_surface->new_popup);

	xdg_surface->map.notify = map_xdg_surface;
	wl_signal_add(&wlr_xdg_surface->events.map, &xdg_surface->map);

	xdg_surface->unmap.notify = unmap_xdg_surface;
	wl_signal_add(&wlr_xdg_surface->events.unmap, &xdg_surface->unmap);

	xdg_surface->request_move.notify = request_move_xdg_surface;
	wl_signal_add(&wlr_xdg_surface->toplevel->events.request_move,
			&xdg_surface->request_move);

	xdg_surface->request_resize.notify = request_resize_xdg_surface;
	wl_signal_add(&wlr_xdg_surface->toplevel->events.request_resize,
			&xdg_surface->request_resize);

	xdg_surface->request_maximize.notify = request_maximize_xdg_surface;
	wl_signal_add(&wlr_xdg_surface->toplevel->events.request_maximize,
			&xdg_surface->request_maximize);

	xdg_surface->request_fullscreen.notify = request_fullscreen_xdg_surface;
	wl_signal_add(&wlr_xdg_surface->toplevel->events.request_fullscreen,
			&xdg_surface->request_fullscreen);

	// FIXME: do we need xdg_surface::surface_commit ?

	const struct server *server = backend->server;
	struct output *output = get_current_output(server);
	if (output == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to get current output");
		return;
	}

	struct window *window = create_window();
	if (xdg_surface->window == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create window for xdg surface %s", title);
		free(xdg_surface);
		return;
	}

	window->title = title;
	window->desktop = output->desktop;

	// Fill the window with xdg_shell related pointers
	window->type = XDG_SHELL;
	window->wlr_xdg_surface = wlr_xdg_surface;
	window->xdg_surface = xdg_surface;

	xdg_surface->window = window;

	struct node *node = create_node(output->desktop);
	node->window = window;

	if (insert_node(server, &output->desktop->root, node) == false)
	{
		wlr_log(WLR_ERROR, "Failed to insert window into desktop");
		destroy_window(window);
	}
}

void destroy_xdg_surface(struct wl_listener *listener, void *data)
{}

void new_popup_xdg_surface(struct wl_listener *listener, void *data)
{}

void map_xdg_surface(struct wl_listener *listener, void *data)
{}

void unmap_xdg_surface(struct wl_listener *listener, void *data)
{}

void request_move_xdg_surface(struct wl_listener *listener, void *data)
{}

void request_resize_xdg_surface(struct wl_listener *listener, void *data)
{}

void request_maximize_xdg_surface(struct wl_listener *listener, void *data)
{}

void request_fullscreen_xdg_surface(struct wl_listener *listener, void *data)
{}
