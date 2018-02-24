#include "bspwc/server.h"

bool init_server(struct server* s)
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
    
    wl_display_init_shm(s->display);
    wlr_gamma_control_manager_create(s->display);
    wlr_screenshooter_create(s->display);
    wlr_primary_selection_device_manager_create(s->display);
    wlr_idle_create(s->display);
	
    wlr_data_device_manager_create(s->display);
    s->compositor = wlr_compositor_create(s->display, s->renderer);

    struct wlr_server_decoration_manager* sdm = wlr_server_decoration_manager_create(s->display);
    wlr_server_decoration_manager_set_default_mode(sdm, WLR_SERVER_DECORATION_MANAGER_MODE_CLIENT);

    wl_list_init(&s->monitors);
    s->new_monitor.notify = new_monitor_notify;
    wl_signal_add(&s->backend->events.new_output, &s->new_monitor);

    // Maybe make that configurable? Is everybody OK with the default pointer?
    s->xcursor_theme = wlr_xcursor_theme_load("default", 16);
    if (!s->xcursor_theme)
    {
		wlr_log(L_ERROR, "Failed to load cursor theme");
		return 1;
    }

	const char* cursor_default = "left_ptr";
	s->xcursor_manager = wlr_xcursor_manager_create(cursor_default, 24);
	if (s->xcursor_manager == NULL)
    {
		wlr_log(L_ERROR, "Cannot create XCursor manager for theme %s", cursor_default);
		return false;
    }

    struct wlr_xcursor* xcursor = wlr_xcursor_theme_get_cursor(s->xcursor_theme, cursor_default);
    if (xcursor == NULL)
    {
        wlr_log(L_ERROR, "Cannot get xcursor from manager");
        return false;
    }

    s->xwayland = wlr_xwayland_create(s->display, s->compositor);
    wl_signal_add(&s->xwayland->events.new_surface, &s->xwayland_surface);
    s->xwayland_surface.notify = handle_xwayland_surface;

    struct wlr_xcursor_image* image = xcursor->images[0];
    wlr_xwayland_set_cursor(
            s->xwayland,
            image->buffer,
            image->width,
            image->width,
            image->height,
            image->hotspot_x, 
            image->hotspot_y
        );

    s->cursor = wlr_cursor_create();
    wlr_cursor_set_image(
            s->cursor,
            image->buffer,
            image->width,
            image->width,
            image->height,
            image->hotspot_x,
            image->hotspot_y,
            0
        );


    return true;
}

bool setup_bspwc(struct server* s)
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

bool start_server(struct server* s)
{
    wlr_log(L_INFO, "Starting bspwc");
    
    // Wayland display socket
    const char* wl_socket = wl_display_add_socket_auto(s->display);
	if (!wl_socket)
    {
		wlr_log(L_ERROR, "Unable to open wayland socket");
        terminate_server(s);
        exit(EXIT_FAILURE);
    }
    
    if (!wlr_backend_start(s->backend))
    {
        wlr_log(L_ERROR, "Failed to start bspwc backend");
        return false;
    }
    
    setenv("WAYLAND_DISPLAY", wl_socket, true);
    wlr_log(L_INFO, "Running bspwc on wayland display '%s'", getenv("WAYLAND_DISPLAY"));

    return true;
}

bool terminate_server(struct server* s)
{
    wlr_log(L_INFO, "Terminating bspwc");

    wlr_xcursor_manager_destroy(s->xcursor_manager);
    wlr_xcursor_theme_destroy(s->xcursor_theme);
    wlr_cursor_destroy(s->cursor);

    wlr_backend_destroy(s->backend);
    wl_display_destroy(s->display);

    return true;
}
