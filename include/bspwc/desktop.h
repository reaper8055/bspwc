#ifndef DESKTOP_H
#define DESKTOP_H

#include <wlr/types/wlr_output.h>

struct padding
{
	int top;
	int right;
	int bottom;
	int left;
};

struct desktop
{
    char name[32];
    struct wlr_output* output;
    struct desktop* prev;
    struct desktop* next;
    struct padding padding;
};

struct desktop* desktop_create(struct desktop* d);

#endif // DESKTOP_H
