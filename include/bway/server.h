#ifndef SERVER_H
#define SERVER_H

#include <stdbool.h>

struct bway_server
{
	struct wl_display* wl_display;
	struct wl_event_loop* wl_event_loop;

	struct wlr_backend* backend;
    struct wlr_renderer* renderer;
};

bool init_server(struct bway_server* s);
bool start_server(struct bway_server* s);
bool terminate_server(struct bway_server* s);

#endif // SERVER_H
