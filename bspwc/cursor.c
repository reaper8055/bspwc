#include "bspwc/cursor.h"

struct cursor* create_cursor(struct input* input)
{
	struct cursor* cursor = calloc(1, sizeof(struct cursor));
	if (cursor == NULL)
	{
		wlr_log(L_ERROR, "Failed to create cursor");
		return NULL;
	}

	cursor->input = input;

	cursor->cursor = wlr_cursor_create();
	if (cursor->cursor == NULL)
	{
		wlr_log(L_ERROR, "Failed to create cursor's wlr_cursor");
		free(cursor);
	}


	return cursor;
}

void destroy_cursor(struct cursor* cursor)
{
	free(cursor);
}
