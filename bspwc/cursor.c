#include "bspwc/cursor.h"

void handle_cursor_motion(struct wl_listener* listener, void* data)
{
	struct cursor* cursor = wl_container_of(listener, cursor, motion);
	struct wlr_event_pointer_motion* event = data;

	wlr_log(L_DEBUG, "Cursor %d, %d", event->delta_x, event->delta_y);

	wlr_cursor_move(cursor->wlr_cursor, event->device, event->delta_x, event->delta_y);
}

void handle_cursor_motion_absolute(struct wl_listener* listener, void* data)
{}

void handle_cursor_button(struct wl_listener* listener, void* data)
{}

void handle_cursor_axis(struct wl_listener* listener, void* data)
{}

struct cursor* create_cursor(struct input* input)
{
	struct cursor* cursor = calloc(1, sizeof(struct cursor));
	if (cursor == NULL)
	{
		wlr_log(L_ERROR, "Failed to create cursor");
		return NULL;
	}

	cursor->input = input;

	struct wlr_output_layout* output_layout = input->server->output_layout;

	cursor->wlr_cursor = wlr_cursor_create();
	if (cursor->wlr_cursor == NULL)
	{
		wlr_log(L_ERROR, "Failed to create cursor's wlr_cursor");
		free(cursor);
	}
	
	cursor->wlr_xcursor_theme = wlr_xcursor_theme_load("default", 16);
	if (cursor->wlr_xcursor_theme == NULL)
	{
		wlr_log(L_ERROR, "Failed to load xcursor theme");
		free(cursor);
		return NULL;
	}

	cursor->wlr_xcursor = wlr_xcursor_theme_get_cursor(cursor->wlr_xcursor_theme, "left_ptr");
	if (cursor->wlr_cursor == NULL)
	{
		wlr_log(L_ERROR, "Failed to load left_ptr cursor");
		free(cursor);
		return NULL;
	}

	struct wlr_xcursor_image* image = cursor->wlr_xcursor->images[0];
	wlr_cursor_set_image(cursor->wlr_cursor, image->buffer, image->width * 4,
		image->width, image->height, image->hotspot_x, image->hotspot_y, 0);

	wlr_cursor_attach_output_layout(cursor->wlr_cursor, output_layout);

	struct wlr_box* layout_box = wlr_output_layout_get_box(output_layout, NULL);
	wlr_cursor_warp(cursor->wlr_cursor, NULL, layout_box->width / 2 , layout_box->height / 2);

	// Setup callbacks
	wl_signal_add(&cursor->wlr_cursor->events.motion, &cursor->motion);
	cursor->motion.notify = handle_cursor_motion;

	wl_signal_add(&cursor->wlr_cursor->events.motion_absolute, &cursor->motion_absolute);
	cursor->motion_absolute.notify = handle_cursor_motion_absolute;

	wl_signal_add(&cursor->wlr_cursor->events.button, &cursor->button);
	cursor->button.notify = handle_cursor_button;

	wl_signal_add(&cursor->wlr_cursor->events.axis, &cursor->axis);
	cursor->axis.notify = handle_cursor_axis;

	return cursor;
}

void destroy_cursor(struct cursor* cursor)
{
	wlr_cursor_destroy(cursor->wlr_cursor);
	wlr_xcursor_theme_destroy(cursor->wlr_xcursor_theme);
	free(cursor);
}
