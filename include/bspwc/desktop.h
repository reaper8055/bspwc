#ifndef DESKTOP_H
#define DESKTOP_H

#include <wlr/types/wlr_output.h>

struct desktop
{
    struct wlr_output* output;
};

struct desktop* desktop_create(struct desktop* d);

#endif // DESKTOP_H
