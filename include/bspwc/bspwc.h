#ifndef BSPWC_H
#define BSPWC_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <wayland-server.h>
#include <wlr/backend.h>
#include <wlr/render.h>
#include <wlr/render/gles2.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/util/log.h>

#include "bspwc/common.h"

struct bspwc_server
{
    struct wlr_data_device_manager* data_device_manager;

	struct wl_display* display;
	struct wl_event_loop* event_loop;
    struct wl_event_source* input_event;

	struct wlr_backend* backend;
    struct wlr_renderer* renderer;

    int socket;
    char* socket_name;
};

bool init_server(struct bspwc_server* s);
bool setup_bspwc(struct bspwc_server* s);
bool start_server(struct bspwc_server* s);
bool terminate_server(struct bspwc_server* s);

#endif // BSPWC_H
