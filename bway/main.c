#define _POSIX_C_SOURCE 200112L

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <wayland-server.h>

#include <wlr/util/log.h>

#include "bway/config.h"
#include "bway/server.h"

#define BWAY_VERSION 0.1 // TODO: Move that elsewhere

struct bway_server server;

int main(int argc, char *argv[])
{
    static int verbose = 0, debug = 0;

    char* config_file = NULL;

    const char* usage = 
        "Usage: bway [option]\n"
        "\n"
        " -h, --help\t\tShow this message\n"
        " -c, --config\t\tSpecify a configuration file\n"
        " -v, --version\t\tShow the version number and exits\n"
        " -V, --verbose\t\tEnable verbose output\n"
        " -d, --debug\t\tEnable debug output\n"
        "\n";

    static struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"config", required_argument, NULL, 'c'},
        {"version", no_argument, NULL, 'v'},
        {"verbose", no_argument, NULL, 'V'},
        {"debug", no_argument, NULL, 'd'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "hc:vVd", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
                fprintf(stdout, "%s", usage);
			    exit(EXIT_SUCCESS);
                break;
            case 'c':
                config_file = malloc(strlen(optarg) + 1);
                strcpy(config_file, optarg);
                break;
            case 'v':
                printf("%f\n", BWAY_VERSION);
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

    init_server(&server);

    // config is loaded last

    // If no config_file is given, the default one is
    // $HOME/.config/bway/bwayrc
    if (config_file == NULL)
    {
        char* home_dir = getenv("HOME");
        if(home_dir == NULL)
        {
            wlr_log(L_ERROR, "Failed to get HOME environment variable");
        }
        
        const char* cfg_file = ".config/bway/bwayrc";
        
        config_file = malloc(strlen(home_dir) + strlen(cfg_file));
        config_file[0] = '\0';

        strcat(config_file, home_dir);
        strcat(config_file, cfg_file);
    }

    if(!load_config_file(config_file))
    {
        terminate_server(&server);
        exit(EXIT_FAILURE);
    }

    free(config_file);

    const char* socket = wl_display_add_socket_auto(server.wl_display);
	if (!socket)
    {
		wlr_log(L_ERROR, "Unable to open wayland socket");
        terminate_server(&server);
		return 1;
    }

    wlr_log(L_INFO, "Running bway on wayland display '%s'", socket);
    setenv("WAYLAND_DISPLAY", socket, true);

    wl_display_run(server.wl_display);

    terminate_server(&server);

    return 0;
}
