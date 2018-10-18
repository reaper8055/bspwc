#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <wayland-server.h>
#include <wlr/types/wlr_input_device.h>

#include "bspwc/input.h"

struct keyboard {
	struct input *input;
	struct wlr_input_device *device;

	struct wl_listener key;
	struct wl_listener modifiers;
};

struct keyboard *create_keyboard(struct input *input,
		struct wlr_input_device *device);
void destroy_keyboard(struct keyboard *keyboard);

bool handle_keybinding(uint32_t modifiers, const xkb_keysym_t *syms, int nsyms);
void handle_keyboard_key(struct wl_listener *listener, void *data);
void handle_keyboard_modifiers(struct keyboard *keyboard);

#endif
