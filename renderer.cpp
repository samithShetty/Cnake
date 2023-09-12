#include <stdio.h>

internal void renderBackground(u32 color) {
    u32* pixel = (u32*) renderState.memory;
    for (int i = 0; i < renderState.width * renderState.height; i++) {
        *pixel++ = color;
    }
}

internal void drawRect(int x, int y, int width, int height, u32 color) {
    x = clamp(0, renderState.width, x);
    y = clamp(0, renderState.height, y);
    width = clamp(0, renderState.width-x, width);
    height = clamp(0, renderState.height-y, height);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            *((u32*)renderState.memory + (x+i) + (y+j) * renderState.width) = color;
        }
    }
}
internal void drawRectOutline(int x, int y, int width, int height, int thickness, u32 color) {
    x = clamp(0, renderState.width, x);
    y = clamp(0, renderState.height, y);
    width = clamp(0, renderState.width - x, width);
    height = clamp(0, renderState.height - y, height);

    drawRect(x, y, width, thickness, color);
    drawRect(x, y + height, width, thickness, color);
    drawRect(x, y, thickness, height, color);
    drawRect(x+width, y, thickness, height, color);
}
