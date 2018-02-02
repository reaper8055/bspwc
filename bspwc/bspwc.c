#include "bspwc/bspwc.h"

bool init_server(struct bspwc_server* s)
{
    wlr_log(L_INFO, "Initializing bspwc");

    s->display = wl_display_create();

    wl_display_init_shm(s->display);

   	s->event_loop = wl_display_get_event_loop(s->display);
    s->data_device_manager = wlr_data_device_manager_create(s->display);

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

	s->compositor = wlr_compositor_create(s->display, s->renderer);
    s->xwayland = wlr_xwayland_create(s->display, s->compositor);

	const char *cursor_default = "left_ptr";
	s->xcursor_manager = wlr_xcursor_manager_create(cursor_default, 24);
	if (s->xcursor_manager == NULL)
    {
		wlr_log(L_ERROR, "Cannot create XCursor manager for theme %s", cursor_default);
		return false;
    }


    return true;
}

bool setup_bspwc(struct bspwc_server* s)
{

    struct sockaddr_un sock;
    memset(&sock, 0, sizeof(struct sockaddr_un));

    sock.sun_family = AF_UNIX;
    strncpy(sock.sun_path, s->socket_name, sizeof(sock.sun_path) - 1);

    if ((s->socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        wlr_log(L_ERROR, "Failed to create socket %s", s->socket_name);
        return false;
    }

    unlink(s->socket_name);
    
    int ret = bind(s->socket, (const struct sockaddr *) &sock, sizeof(struct sockaddr_un));
    if (ret == -1)
    {
        wlr_log(L_ERROR, "Failed to bind to socket %s", s->socket_name);
        return false;
    }

    ret = listen(s->socket, SOMAXCONN);
    if (ret == -1)
    {
        wlr_log(L_ERROR, "Failed to listen on socket %s", s->socket_name);
        return false;
    }

    wlr_log(L_INFO, "BSPWM socket setup to %s", s->socket_name);

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

    wlr_xcursor_manager_destroy(s->xcursor_manager);

    wlr_backend_destroy(s->backend);
    wl_display_destroy(s->display);

    return true;
}
