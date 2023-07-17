#include <stdio.h>

internal void render_background(u32 color) {
    u32* pixel = (u32*) render_state.memory;
    for (int i = 0; i < render_state.width * render_state.height; i++) {
        *pixel++ = color;
    }
}

internal void draw_rect_in_pixels(int x, int y, int width, int height, u32 color) {
    x = clamp(0, render_state.width, x);
    y = clamp(0, render_state.height, y);
    width = clamp(0, render_state.width-x, width);
    height = clamp(0, render_state.height-y, height);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            *((u32*)render_state.memory + (x+i) + (y+j) * render_state.width) = color;
        }
    }
}
internal void draw_rect_outline_in_pixels(int x, int y, int width, int height, int thickness, u32 color) {
    x = clamp(0, render_state.width, x);
    y = clamp(0, render_state.height, y);
    width = clamp(0, render_state.width - x, width);
    height = clamp(0, render_state.height - y, height);

    draw_rect_in_pixels(x, y, width, thickness, color);
    draw_rect_in_pixels(x, y + height, width, thickness, color);
    draw_rect_in_pixels(x, y, thickness, height, color);
    draw_rect_in_pixels(x+width, y, thickness, height, color);
}


global_variable float render_scale = 0.01f;
internal void draw_rect(float x, float y, float half_width, float half_height, u32 color) {
    x *= render_state.width * render_scale;
    y *= render_state.width * render_scale;
    half_width *= render_state.width * render_scale;
    half_height *= render_state.width * render_scale;
    
    x += render_state.width / 2.f;
    y += render_state.height / 2.f;
    draw_rect_in_pixels(x - half_width, y - half_width, half_width, half_height, color);
}