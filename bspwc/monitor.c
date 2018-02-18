#include "bspwc/monitor.h"

struct monitor* monitor_create(struct server* s, const char* name)
{
    wlr_log(L_DEBUG, "Creating monitor %s", name);

    struct monitor* m = calloc(1, sizeof(struct monitor));
    if (m == NULL)
    {
        wlr_log(L_ERROR, "Could not create monitor %s", name);
        return NULL;
    }

    // Copy name
    strcpy(m->name, (name == NULL ? "default_monitor" : name));

    return m;
}
