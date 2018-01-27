#include "bspwc/bspwc.h"

bool init_server(struct bspwc_server* s)
{
    wlr_log(L_INFO, "Initializing bspwc");

    s->display = wl_display_create();
   	s->event_loop = wl_display_get_event_loop(s->display);

	s->backend = wlr_backend_autocreate(s->display);
    if (s->backend == NULL)
    {
        wlr_log(L_ERROR, "Could not create backend");
        return false;
    }

    s->renderer = wlr_gles2_renderer_create(s->backend);
    if (s->renderer == NULL)
    {
        wlr_log(L_ERROR, "Could not create gles2 renderer");
        return false;
    }

    s->data_device_manager = wlr_data_device_manager_create(s->display);

    wl_display_init_shm(s->display);

    return true;
}

bool setup_bspwc(struct bspwc_server* s)
{
    // Establish connection with bspc socket
    if (s->bspc_socket == NULL)
    {
        s->bspc_socket = malloc(strlen(BSPWC_DEFAULT_SOCKET));
        strcpy(s->bspc_socket, BSPWC_DEFAULT_SOCKET);
    }
    setenv("BSPWM_SOCKET", s->bspc_socket, true);
    wlr_log(L_INFO, "Using BSPWM_SOCKET=%s", getenv("BSPWM_SOCKET"));

    return true;
}

bool start_server(struct bspwc_server* s)
{
    wlr_log(L_INFO, "Starting bspwc");
    
    if (!wlr_backend_start(s->backend))
    {
        wlr_log(L_ERROR, "Failed to start bspwc backend");
        return false;
    }

    return true;
}

bool terminate_server(struct bspwc_server* s)
{
    wlr_log(L_INFO, "Terminating bspwc");

    wlr_backend_destroy(s->backend);
    wl_display_destroy(s->display);

    return true;
}
