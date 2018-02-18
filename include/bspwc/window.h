#ifndef WINDOW_H
#define WINDOW_H

#include <wayland-server.h> 

struct window
{
    double x, y;
};

void window_add_notify(struct wl_listener* listener, void *data);
void window_remove_notify(struct wl_listener* listener, void *data);

#endif // WINDOW_H
