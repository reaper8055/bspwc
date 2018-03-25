#include "bspwc/output.h"

void output_destroy_notify(struct wl_listener* listener, void* data)
{
    struct output* output = wl_container_of(listener, output, destroy);

    wl_list_remove(&output->link);
    wl_list_remove(&output->destroy.link);
    wl_list_remove(&output->frame.link);

    free(output);
}

void output_frame_notify(struct wl_listener* listener, void* data)
{
    struct output* output = wl_container_of(listener, output, frame);
    struct server* server = output->server;
    struct wlr_output* wlr_output = data;
    struct wlr_renderer* renderer = wlr_backend_get_renderer(wlr_output->backend);

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    wlr_output_make_current(wlr_output, NULL);
    wlr_renderer_begin(renderer, wlr_output->width, wlr_output->height);

        float color[4] = {0.23f, 0.26f, 0.32f, 1.0f};
        wlr_renderer_clear(renderer, color);

        struct wl_resource* wl_resource_surface;
        wl_resource_for_each(wl_resource_surface, &server->wlr_compositor->surfaces)
        {
            struct wlr_surface* surface = wlr_surface_from_resource(wl_resource_surface);
            if (!wlr_surface_has_buffer(surface))
            {
                continue;
            }

            struct wlr_box render_box = {
                .x = 20,
                .y = 20,
                .width = surface->current->width,
                .height = surface->current->height
            };

            float matrix[16];
            wlr_matrix_project_box(
                    matrix,
                    &render_box,
                    surface->current->transform,
                    0,
                    wlr_output->transform_matrix
                );

            wlr_render_texture_with_matrix(renderer, surface->texture, matrix, 1.0f);
            wlr_surface_send_frame_done(surface, &now);
        }

        wlr_output_swap_buffers(wlr_output, NULL, NULL);
    wlr_renderer_end(renderer);
}

void new_output_notify(struct wl_listener* listener, void* data)
{
    struct server* server = wl_container_of(listener, server, new_output);
    struct wlr_output* wlr_output = data;

    if (!wl_list_empty(&wlr_output->modes))
    {
        struct wlr_output_mode* mode = wl_container_of(wlr_output->modes.prev, mode, link);
        wlr_output_set_mode(wlr_output, mode);
    }

    struct output* output = calloc(1, sizeof(struct output));
    clock_gettime(CLOCK_MONOTONIC, &output->last_frame);
    output->server = server;
    output->wlr_output = wlr_output;
    wl_list_insert(&server->outputs, &output->link);

    output->destroy.notify = output_destroy_notify;
    wl_signal_add(&wlr_output->events.destroy, &output->destroy);

    output->frame.notify = output_frame_notify;
    wl_signal_add(&wlr_output->events.frame, &output->frame);

    wlr_output_create_global(wlr_output);
}
