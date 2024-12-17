#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string.h>
#include <stdint.h>
#include <mcufont.h>
#include <stddef.h>

struct Rect
{
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
};

struct Coords
{
    uint32_t x;
    uint32_t y;
};

enum FontAlign
{
    LEFT,
    CENTER,
    RIGHT
};

struct GraphicsAPI
{
    void (*draw_pixel)(int x, int y, uint32_t color);
    void (*draw_rectangle)(int x, int y, int w, int h, uint32_t color);
    void (*clear_screen)();
};

struct Widget
{
    struct Rect rect;
    uint32_t bg_color;
    uint32_t fg_color;
    float font_size;
    enum FontAlign font_aling;
    struct Coords text_pos;
    char *text;
};

void init_graphics_api(struct GraphicsAPI *a);
void render_interface(struct Widget *widgets, uint16_t num);
uint8_t get_alpha(uint32_t color);
uint8_t get_red(uint32_t color);
uint8_t get_green(uint32_t color);
uint8_t get_blue(uint32_t color);

#endif // GRAPHICS_H
