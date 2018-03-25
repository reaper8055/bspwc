#include "bspwc/server.h"

bool init_server(struct server* server)
{
    wlr_log(L_INFO, "Initializing bspwc server");

    server->wl_display = wl_display_create();
    assert(server->wl_display);

    server->wl_event_loop = wl_display_get_event_loop(server->wl_display);
    assert(server->wl_event_loop);

    server->wlr_backend = wlr_backend_autocreate(server->wl_display);
    assert(server->wlr_backend);

    wl_list_init(&server->outputs);
    server->new_output.notify = new_output_notify;
    wl_signal_add(&server->wlr_backend->events.new_output, &server->new_output);

    return true;
}

bool setup_server(struct server* server)
{
    struct sockaddr_un sock;
    memset(&sock, 0, sizeof(struct sockaddr_un));

    sock.sun_family = AF_UNIX;
    strncpy(sock.sun_path, server->socket_name, sizeof(sock.sun_path) - 1);

    if ((server->socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        wlr_log(L_ERROR, "Failed to create socket %s", server->socket_name);
        return false;
    }

    unlink(server->socket_name);

    int ret = bind(server->socket, (const struct sockaddr *) &sock, sizeof(struct sockaddr_un));
    if (ret == -1)
    {
        wlr_log(L_ERROR, "Failed to bind to socket %s", server->socket_name);
        return false;
    }

    ret = listen(server->socket, SOMAXCONN);
    if (ret == -1)
    {
        wlr_log(L_ERROR, "Failed to listen on socket %s", server->socket_name);
        return false;
    }

    wlr_log(L_INFO, "BSPWM socket setup to %s", server->socket_name);

    return true;
}

bool start_server(struct server* server)
{
    wlr_log(L_INFO, "Starting bspwc");

    // Wayland display socket
    const char* wl_socket = wl_display_add_socket_auto(server->wl_display);
    if (!wl_socket)
    {
        wlr_log(L_ERROR, "Unable to open wayland socket");
        return false;
    }

    if (!wlr_backend_start(server->wlr_backend))
    {
        wlr_log(L_ERROR, "Failed to start bspwc backend");
        return false;
    }

    setenv("WAYLAND_DISPLAY", wl_socket, true);
    wlr_log(L_INFO, "Running bspwc on wayland display '%s'", getenv("WAYLAND_DISPLAY"));

    // TODO: maybe find a better place for those
    wl_display_init_shm(server->wl_display);
    wlr_gamma_control_manager_create(server->wl_display);
    wlr_screenshooter_create(server->wl_display);
    wlr_primary_selection_device_manager_create(server->wl_display);
    wlr_idle_create(server->wl_display);

    server->wlr_compositor = wlr_compositor_create(
            server->wl_display,
            wlr_backend_get_renderer(server->wlr_backend)
        );

    wlr_xdg_shell_v6_create(server->wl_display);

    return true;
}

bool terminate_server(struct server* server)
{
    wlr_log(L_INFO, "Terminating bspwc");

    //wlr_xcursor_manager_destroy(server->xcursor_manager);
    //wlr_xcursor_theme_destroy(server->xcursor_theme);
    //wlr_cursor_destroy(server->cursor);

    wlr_backend_destroy(server->wlr_backend);
    wl_display_destroy(server->wl_display);

    return true;
}
