#include "bspwc/window.h"

struct window* create_window()
{
    struct window* window = calloc(1, sizeof(struct window));
    if (window == NULL)
    {
        return NULL;
    }

    window->alpha = 1.0f;

    return window;
}
