#ifndef CURSOR_H
#define CURSOR_H

#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_xcursor_manager.h>

#include "bspwc/input.h"

struct cursor
{
	struct input* input;

	struct wlr_cursor* cursor;
	struct wlr_xcursor* xcursor;
};

struct cursor* create_cursor(struct input* input);
void destroy_cursor(struct cursor* cursor);

#endif // CURSOR_H
