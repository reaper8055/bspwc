#define _POSIX_C_SOURCE 200112L

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <wayland-server.h>
#include <wlr/backend.h>
#include <wlr/util/log.h>

#include "bspwc/config.h"
#include "bspwc/bspwc.h"

struct bspwc_server server;

/*
static int read_events(int fd, uint32_t mask, void* data)
{
    wlr_log(L_INFO, "reading events from socket %f, %f, %p", fd, mask, data);
    return 0;
}
*/

int main(int argc, char *argv[])
{
    static int verbose = 0, debug = 0;

    char* config_file = NULL;

    const char* usage = 
        "Usage: bspwc [option]\n"
        "\n"
        " -h, --help\t\tShow this message\n"
        " -c, --config\t\tSpecify a configuration file\n"
        " -s, --socket\t\tSpecify a socket to use\n"
        " -v, --version\t\tShow the version number and exits\n"
        " -V, --verbose\t\tEnable verbose output\n"
        " -d, --debug\t\tEnable debug output\n"
        "\n";

    static struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"config", required_argument, NULL, 'c'},
        {"socket", optional_argument, NULL, 's'},
        {"version", no_argument, NULL, 'v'},
        {"verbose", no_argument, NULL, 'V'},
        {"debug", no_argument, NULL, 'd'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "hc:s:vVd", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
                fprintf(stdout, "%s", usage);
			    exit(EXIT_SUCCESS);
                break;
            case 'c':
                config_file = malloc(strlen(optarg));
                strcpy(config_file, optarg);
                break;
            case 's':
                server.bspc_socket = malloc(strlen(optarg));
                strcpy(server.bspc_socket, optarg);
                break;
            case 'v':
                printf("%s\n", BSPWC_VERSION);
                exit(EXIT_SUCCESS);
                break;
            case 'V':
                verbose = 1;
                break;
            case 'd':
                debug = 1;
                break;
        }
    }

    if (debug)
    {
        wlr_log_init(L_DEBUG, NULL);
    }
    else if (verbose)
    {
        wlr_log_init(L_INFO, NULL);
    }
    else
    {
        wlr_log_init(L_ERROR, NULL);
    }

    if (!init_server(&server))
    {
		wlr_log(L_ERROR, "Failed to init server");
        exit(EXIT_FAILURE);
    }
   
    // Wayland display socket
    const char* wl_socket = wl_display_add_socket_auto(server.display);
	if (!wl_socket)
    {
		wlr_log(L_ERROR, "Unable to open wayland socket");
        terminate_server(&server);
        exit(EXIT_FAILURE);
    }
    setenv("WAYLAND_DISPLAY", wl_socket, true);
    wlr_log(L_INFO, "Running bspwc on wayland display '%s'", getenv("WAYLAND_DISPLAY"));

    if (!setup_bspwc(&server))
    {
		wlr_log(L_ERROR, "Unable to setup bspwc");
        terminate_server(&server);
        exit(EXIT_FAILURE);
    }

/*
    // Open socket
    unlink(bspc_socket);
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1)
    {
        wlr_log(L_ERROR, "Error creating local socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un sock;
    memset(&sock, 0, sizeof(struct sockaddr_un));
    sock.sun_family = AF_UNIX;
    strncpy(sock.sun_path, bspc_socket, sizeof(sock.sun_path) - 1);

    int ret = bind(fd, (const struct sockaddr*) &sock, sizeof(struct sockaddr_un));
    if (ret == -1)
    {
        wlr_log(L_ERROR, "Error binding socket");
        exit(EXIT_FAILURE);
    }
    // Register fd to wl_event_loops
    struct wl_event_source* input_event;
    input_event = wl_event_loop_add_fd(server.event_loop, fd, WL_EVENT_READABLE, read_events, server.backend);
    if (!input_event)
    {
		wlr_log(L_ERROR, "Failed to create input event on event loop");
		exit(EXIT_FAILURE);
    }
*/

    // Start BSPWC
    if (!start_server(&server))
    {
        wlr_log(L_ERROR, "Failed to start server");
        terminate_server(&server);
        exit(EXIT_FAILURE);
    }

    // If no config_file is given, the default one is
    // $HOME/.config/bspwc/bspwcrc
    if (config_file == NULL)
    {
        char* home_dir = getenv("HOME");
        if(home_dir == NULL)
        {
            wlr_log(L_ERROR, "Failed to get HOME environment variable");
        }
        
        config_file = malloc(strlen(home_dir) + strlen(BSPWC_DEFAUT_CONFIG_FILE));
        config_file[0] = '\0';

        strcat(config_file, home_dir);
        strcat(config_file, BSPWC_DEFAUT_CONFIG_FILE);
    }

    if (!load_config_file(config_file))
    {
		wlr_log(L_ERROR, "Failed to load config file");
        terminate_server(&server);
        exit(EXIT_FAILURE);
    }
    
    wl_display_run(server.display);

    terminate_server(&server);
    
    free(config_file);

    return 0;
}
