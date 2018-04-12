#include "bspwc/input.h"

const char* device_type(enum wlr_input_device_type type)
{
    switch (type)
    {
        case WLR_INPUT_DEVICE_KEYBOARD:
            return "keyboard";
        case WLR_INPUT_DEVICE_POINTER:
            return "pointer";
        case WLR_INPUT_DEVICE_TOUCH:
            return "touch";
        case WLR_INPUT_DEVICE_TABLET_TOOL:
            return "tablet tool";
        case WLR_INPUT_DEVICE_TABLET_PAD:
            return "tablet pad";
        default:
            return "UNKNOWN_DEVICE";
    }
    return NULL;
}

void handle_new_input(struct wl_listener* listener, void* data)
{
    struct wlr_input_device* device = data;
    struct input* input = wl_container_of(listener, input, new_input);

    wlr_log(L_DEBUG, "New input device: %s (%d:%d) %s", device->name, device->vendor, device->product, device_type(device->type));
}

struct input* create_input(struct server* server)
{
    wlr_log(L_DEBUG, "Creating input");
    assert(server);

    struct input* input = calloc(1, sizeof(struct input));
    if (input == NULL)
    {
        return NULL;
    }

    input->server = server;

    wl_list_init(&input->seats);

    input->new_input.notify = handle_new_input;
    wl_signal_add(&server->backend->wlr_backend->events.new_input, &input->new_input);

    return input;
}

void destroy_input(struct input* input)
{}
