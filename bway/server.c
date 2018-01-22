#include <wayland-server.h>

#include <wlr/backend.h>
#include <wlr/render.h>
#include <wlr/render/gles2.h>
#include <wlr/util/log.h>

#include "bway/server.h"

bool init_server(struct bway_server* s)
{
    wlr_log(L_INFO, "Initializing bway server");

    s->wl_display = wl_display_create();
   	s->wl_event_loop = wl_display_get_event_loop(s->wl_display);

	s->backend = wlr_backend_autocreate(s->wl_display);
    if (s->backend == NULL)
    {
        wlr_log(L_ERROR, "Could not create backend");
        return false;
    }
    s->renderer = wlr_gles2_renderer_create(s->backend);

    return true;
}

bool start_server(struct bway_server* s)
{
    wlr_log(L_INFO, "Starting bway server");
    
    if (!wlr_backend_start(s->backend))
    {
        wlr_log(L_ERROR, "Failed to start bway backend");
        return false;
    }

    return true;
}

bool terminate_server(struct bway_server* s)
{
    wlr_log(L_INFO, "Terminating bway server");

    wlr_backend_destroy(s->backend);
    wl_display_destroy(s->wl_display);

    return true;
}
