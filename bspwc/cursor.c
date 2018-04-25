#include "bspwc/cursor.h"

struct cursor* create_cursor()
{
	struct cursor* cursor = calloc(1, sizeof(struct cursor));
	if (cursor == NULL)
	{
		wlr_log(L_ERROR, "Failed to create cursor");
		return NULL;
	}

	return cursor;
}

void destroy_cursor(struct cursor* cursor)
{
	free(cursor);
}
