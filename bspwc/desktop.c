#include "bspwc/desktop.h"

struct desktop* desktop_create(struct server* s, const char* name)
{
    wlr_log(L_DEBUG, "Creating desktop %s", name);

    struct desktop* d = calloc(1, sizeof(struct desktop));
    if (d == NULL)
    {
        wlr_log(L_ERROR, "Could not create desktop %s", name);
        return NULL;
    }

    strcpy(d->name, (name == NULL ? "default_monitor" : name));

    // Init links
    wl_list_init(&d->monitor_link);
    wl_list_init(&d->windows);
    
    d->server = s;



    return d;
}

void desktop_destroy(struct desktop* d)
{


}
