#include <wayland-server.h>
#include <wlr/types/wlr_input_device.h>
#include <xkbcommon/xkbcommon.h>

#include "bspwc/keyboard.h"
#include "bspwc/input.h"

struct keyboard* create_keyboard(struct input* input, struct wlr_input_device* device)
{
	struct keyboard* keyboard = calloc(1, sizeof(keyboard));
	if (keyboard == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create keyboard");
		return NULL;
	}
	device->data = keyboard;
	keyboard->device = device;
	keyboard->input = input;

	struct xkb_rule_names rules = {
		.rules = getenv("XKB_DEFAULT_RULES"),
		.model = getenv("XKB_DEFAULT_MODEL"),
		.layout = getenv("XKB_DEFAULT_LAYOUT"),
		.variant = getenv("XKB_DEFAULT_VARIANT"),
		.options = getenv("XKB_DEFAULT_OPTIONS"),
	};

	struct xkb_context* context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	if (context == NULL)
	{
		wlr_log(WLR_ERROR, "Cannot create XKB context");
		return NULL;
	}

	struct xkb_keymap* keymap = xkb_map_new_from_names(context, &rules,
		XKB_KEYMAP_COMPILE_NO_FLAGS);
	if (keymap == NULL) {
		xkb_context_unref(context);
		wlr_log(WLR_ERROR, "Cannot create XKB keymap");
		return NULL;
	}

	wlr_keyboard_set_keymap(device->keyboard, keymap);
	xkb_keymap_unref(keymap);
	xkb_context_unref(context);

	return keyboard;
}

void destroy_keyboard(struct keyboard* keyboard)
{
	free(keyboard);
}
