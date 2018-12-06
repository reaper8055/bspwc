#include "bspwc/window.h"

struct window* create_window()
{
	wlr_log(WLR_DEBUG, "Creating new window");
	struct window *window = calloc(1, sizeof(struct window));
	if (window == NULL)
	{
		return NULL;
	}

	window->title = NULL;
	window->x = 0;
	window->y = 0;
	window->width = 0;
	window->height = 0;
	window->alpha = 1.0f;

	window->desktop = NULL;

	window->type = NONE;

	window->wlr_surface = NULL;

	wl_signal_init(&window->event_unmap);
	wl_signal_init(&window->event_destroy);

	return window;
}

void destroy_window(struct window *window)
{
	wlr_log(WLR_DEBUG, "Destroying window");

	// TODO: Destroy signals

	free(window);
}

struct wlr_box get_window_box(const struct window *window)
{
	struct wlr_box box = {
		.x = window->x,
		.y = window->y,
		.width = window->width,
		.height = window->height,
	};

	return box;
}

void set_window_position(struct window *window, const double x, const double y)
{
	if (window == NULL)
	{
		wlr_log(WLR_ERROR, "Cannot resize null window");
		return;
	}

	wlr_log(WLR_INFO, "Window %s new position to %f, %f", window->title, x, y);

	window->x = x;
	window->y = y;
}

void resize_window(struct window *window, uint32_t width, uint32_t height)
{
	if (window == NULL)
	{
		wlr_log(WLR_ERROR, "Cannot resize null window");
		return;
	}

	wlr_log(WLR_INFO, "Window %s resize to %d, %d", window->title, width,
			height);

	window->width = width;
	window->height = height;

	switch (window->type)
	{
	case XDG_SHELL_V6:
		//resize_xdg_shell_v6(window, width, height);
		//break;
	case WL_SHELL:
	case XDG_SHELL:
	case XWAYLAND:
	case NONE:
	default:
		wlr_log(WLR_ERROR, "Unknown surface type");
	}
}

void render_window(const struct window *window)
{
	struct wlr_output *wlr_output = window->desktop->output->wlr_output;

	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);

	if (window->wlr_surface != NULL)
	{
		render_surface(wlr_output, window->wlr_surface, window->x, window->y);

		wlr_surface_send_frame_done(window->wlr_surface, &now);
	}
}

void focus_window(struct window *window)
{
	// Cannot focus empty window
	if (window == NULL)
	{
		return;
	}

	struct server *server = window->desktop->output->server;
	assert(server);
	struct input *input = server->input;
	assert(input);

	struct window *previous_focus = input->focus;
	if (previous_focus == window)
	{
		return;
	}

	wlr_log(WLR_DEBUG, "Focusing window %p", (void*)window);

	if (previous_focus)
	{
		activate_window(previous_focus, false);
	}

	activate_window(window, true);
	input->focus = window;

	// FIXME: move this logic to the input structure
	struct wlr_seat *seat = input->seat;
	struct wlr_keyboard *keyboard = wlr_seat_get_keyboard(seat);
	wlr_seat_keyboard_notify_enter(seat, window->wlr_surface,
			keyboard->keycodes, keyboard->num_keycodes, &keyboard->modifiers);
}

void activate_window(struct window *window, bool status)
{
	switch (window->type)
	{
	case XDG_SHELL:
		wlr_xdg_toplevel_set_activated(window->wlr_xdg_surface, status);
		break;
	case XDG_SHELL_V6:
	case WL_SHELL:
	case XWAYLAND:
	case NONE:
	default:
		wlr_log(WLR_ERROR, "Unknown surface type");
	}
}
