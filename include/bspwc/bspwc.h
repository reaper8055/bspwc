#ifndef BSPWC_H
#define BSPWC_H

#include <stdbool.h>

struct bspwc
{
	struct wl_display* wl_display;
	struct wl_event_loop* wl_event_loop;

	struct wlr_backend* backend;
    struct wlr_renderer* renderer;
};

bool init_bspwc(struct bspwc* s);
bool start_bspwc(struct bspwc* s);
bool terminate_bspwc(struct bspwc* s);

#endif // BSPWC_H
