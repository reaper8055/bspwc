#define _POSIX_C_SOURCE 200112L

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <wayland-server.h>

#include <wlr/backend.h>
#include <wlr/util/log.h>

#include "bspwc/config.h"
#include "bspwc/bspwc.h"

#define BSPWC_VERSION 0.1 // TODO: Move that elsewhere

struct bspwc compositor;

int main(int argc, char *argv[])
{
    static int verbose = 0, debug = 0;

    char* config_file = NULL;

    const char* usage = 
        "Usage: bspwc [option]\n"
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
                printf("%f\n", BSPWC_VERSION);
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

    init_bspwc(&compositor);

    // config is loaded last

    // If no config_file is given, the default one is
    // $HOME/.config/bspwc/bspwcrc
    if (config_file == NULL)
    {
        char* home_dir = getenv("HOME");
        if(home_dir == NULL)
        {
            wlr_log(L_ERROR, "Failed to get HOME environment variable");
        }
        
        const char* cfg_file = ".config/bspwc/bspwcrc";
        
        config_file = malloc(strlen(home_dir) + strlen(cfg_file));
        config_file[0] = '\0';

        strcat(config_file, home_dir);
        strcat(config_file, cfg_file);
    }

    if (!load_config_file(config_file))
    {
		wlr_log(L_ERROR, "Failed to load config file");
        terminate_bspwc(&compositor);
        return 1;
    }

    free(config_file);
    
    const char* wl_socket = wl_display_add_socket_auto(compositor.wl_display);
	if (!wl_socket)
    {
		wlr_log(L_ERROR, "Unable to open wayland socket");
        terminate_bspwc(&compositor);
		return 1;
    }

    wlr_log(L_INFO, "Running bspwc on wayland display '%s'", wl_socket);
    setenv("WAYLAND_DISPLAY", wl_socket, true);

    if (!start_bspwc(&compositor))
    {
        wlr_log(L_ERROR, "Failed to start compositor");
        terminate_bspwc(&compositor);
        return 1;
    }

    wl_display_run(compositor.wl_display);

    terminate_bspwc(&compositor);

    return 0;
}
