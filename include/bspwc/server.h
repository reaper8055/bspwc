#ifndef SERVER_H
#define SERVER_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <wlr/util/log.h>

#include "bspwc/backend.h"
#include "bspwc/common.h"
#include "bspwc/output.h"

struct server
{
    char* socket_name;
    int socket;

    struct backend* backend;
};

bool init_server(struct server* server);
bool start_server(struct server* server);
bool terminate_server(struct server* server);

#endif // SERVER_H
