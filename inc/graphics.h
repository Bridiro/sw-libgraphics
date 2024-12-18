#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <mcufont.h>

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

struct TextBox
{
    uint16_t id;
    struct Rect rect;
    uint32_t bg_color;
    uint32_t fg_color;
    float font_size;
    enum FontAlign font_aling;
    struct Coords text_pos;
    char *text;
};

void init_graphics_api(struct GraphicsAPI *a);
void render_interface(struct TextBox *text_boxes, uint16_t num);
uint8_t get_alpha(uint32_t color);
uint8_t get_red(uint32_t color);
uint8_t get_green(uint32_t color);
uint8_t get_blue(uint32_t color);
struct TextBox *get_text_box(struct TextBox *text_boxes, uint16_t num, uint16_t id);
uint32_t color_modify_rgb(uint32_t color, int8_t delta);

#endif // GRAPHICS_H
