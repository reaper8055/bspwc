#include "bspwc/monitor.h"

void new_monitor_notify(struct wl_listener* listener, void* data)
{
    struct server* server = wl_container_of(listener, server, new_monitor);
    struct wlr_output* wlr_output = data;

    wlr_log(L_INFO, "New monitor %s", wlr_output->name);

    if (wl_list_length(&wlr_output->modes) > 0)
    {
        struct wlr_output_mode* mode = wl_container_of((&wlr_output->modes)->prev, mode, link);
        wlr_output_set_mode(wlr_output, mode);
    }

    struct monitor* m = calloc(1, sizeof(struct monitor));
    clock_gettime(CLOCK_MONOTONIC, &m->last_frame);
    m->server = server;
    m->wlr_output = wlr_output;
    m->color[0] = 1.0;
    m->color[1] = 1.0;
    m->color[2] = 1.0;
    m->color[3] = 1.0;
    wl_list_insert(&server->monitors, &m->server_link);

    m->destroy.notify = monitor_destroy_notify;
    wl_signal_add(&wlr_output->events.destroy, &m->destroy);
    m->frame.notify = monitor_frame_notify;
    wl_signal_add(&wlr_output->events.frame, &m->frame);
}

void monitor_destroy_notify(struct wl_listener* listener, void* data)
{
    struct monitor* monitor = wl_container_of(listener, monitor, destroy);

    wlr_log(L_DEBUG, "Removing monitor %s", monitor->wlr_output->name);

    wl_list_remove(&monitor->server_link);
    wl_list_remove(&monitor->destroy.link);
    wl_list_remove(&monitor->frame.link);
    free(monitor);
}

void monitor_frame_notify(struct wl_listener* listener, void* data)
{
    struct monitor* monitor = wl_container_of(listener, monitor, frame);
    struct wlr_output* wlr_output = data;
   
    struct wlr_renderer* renderer = wlr_backend_get_renderer(wlr_output->backend);

    wlr_output_make_current(wlr_output, NULL);
    wlr_renderer_begin(renderer, wlr_output);

    wlr_renderer_clear(renderer, &monitor->color);

    wlr_output_swap_buffers(wlr_output, NULL, NULL);
    wlr_renderer_end(renderer);
}
