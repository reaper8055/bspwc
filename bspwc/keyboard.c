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

	return keyboard;
}
