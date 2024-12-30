#include "graphics.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct GraphicsAPI *api = NULL;

extern const struct mf_rlefont_s mf_rlefont_KonexyFont140;

void _pixel_callback(int16_t x, int16_t y, uint8_t count, uint8_t alpha, void *state)
{
    struct font_color_s *rstate = state;
    while (count--)
    {
        api->draw_pixel(x, y, rstate->color | alpha << 24);
        x++;
    }
}

uint8_t _char_callback(int16_t x0, int16_t y0, mf_char character, void *state)
{
    struct mf_scaledfont_s *font = ((struct font_color_s *) state)->font;
    return mf_render_character(&(font->font), x0, y0, character, &_pixel_callback, state);
}

void _draw_text(int16_t x, int16_t y, enum FontAlign align, char *text, uint32_t color, float size)
{
    struct mf_scaledfont_s scaled_font;
    mf_scale_font(&scaled_font, &mf_rlefont_KonexyFont140.font, size, size);
    struct font_color_s mf_data = (struct font_color_s) {
        .font = &scaled_font,
        .color = color,
    };
    mf_render_aligned(&scaled_font.font, x, y, (enum mf_align_t) align, text, strlen(text), &_char_callback, (void *)&mf_data);
}

void _draw_text_box(struct Box *box)
{
#if GRAPHICS_OPT
    if (!box->updated) return;
#endif
    uint32_t bg_color = box->default_bg_color;
    uint32_t fg_color = box->default_fg_color;

    if (box->value && box->value->colors)
    {
        for (int i=0; i<box->value->colors_num; i++)
        {
            if (box->value->colors[i].min < box->value->value && box->value->value < box->value->colors[i].max)
            {
                bg_color = box->value->colors[i].bg_color;
                fg_color = box->value->colors[i].fg_color;
            }
        }
    }

    api->draw_rectangle(box->rect.x, box->rect.y, box->rect.w, box->rect.h, bg_color);
    if (box->value)
    {
        char buf[15];
        if (box->value->is_float)
        {
            snprintf(buf, sizeof(buf), "%.2f", box->value->value);
        } else
        {
            snprintf(buf, sizeof(buf), "%d", (int) box->value->value);
        }
        _draw_text(box->rect.x + box->value->pos.x,
                   box->rect.y + box->value->pos.y,
                   box->value->align,
                   buf, fg_color,
                   box->value->font_size);
    }

    if (box->label)
    {
        _draw_text(box->rect.x + box->label->pos.x,
                   box->rect.y + box->label->pos.y,
                   box->label->align,
                   box->label->text,
                   fg_color,
                   box->label->font_size);
    }
}

void init_graphics_api(struct GraphicsAPI *a)
{
    api = a;
}

void render_interface(struct Box *text_boxes, uint16_t num)
{
#if GRAPHICS_OPT == 0
    api->clear_screen();
#endif
    for (int i = 0; i < num; i++)
    {
        _draw_text_box(text_boxes + i);
    }
}

uint8_t get_alpha(uint32_t color)
{
    return (color >> 24) & 0xff;
}

uint8_t get_red(uint32_t color)
{
    return (color >> 16) & 0xff;
}

uint8_t get_green(uint32_t color)
{
    return (color >> 8) & 0xff;
}

uint8_t get_blue(uint32_t color)
{
    return color & 0xff;
}

struct Box *get_box(struct Box *boxes, uint16_t num, uint16_t id)
{
    for (int i=0; i<num; i++) {
        if ((boxes + i)->id == id) {
            return (boxes + i);
        }
    }
    return NULL;
}

uint32_t color_modify_rgb(uint32_t color, int8_t delta)
{
    uint8_t red = get_red(color);
    uint8_t green = get_green(color);
    uint8_t blue = get_blue(color);

    if (delta > 0) {
        red   = (red + delta > 255) ? 255 : red + delta;
        green = (green + delta > 255) ? 255 : green + delta;
        blue  = (blue + delta > 255) ? 255 : blue + delta;        
    } else {
        red   = (red + delta < 0) ? 0 : red + delta;
        green = (green + delta < 0) ? 0 : green + delta;
        blue  = (blue + delta < 0) ? 0 : blue + delta;
    }

    return (get_alpha(color) << 24) | (red << 16) | (green << 8) | blue;
}

struct Label *create_label(char *text, struct Coords pos, float font_size, enum FontAlign align)
{
    struct Label *label = malloc(sizeof(struct Label));
    if (label)
    {
        label->text = text;
        label->pos = pos;
        label->font_size = font_size;
        label->align = align;
    }
    return label;
}

struct Value *create_value(float val, bool is_float, struct Coords pos, float font_size, enum FontAlign align, struct ColorRange *colors, uint8_t colors_num)
{
    struct Value *value = malloc(sizeof(struct Value));
    if (value)
    {
        value->value = val;
        value->is_float = is_float;
        value->pos = pos;
        value->font_size = font_size;
        value->align = align;
        value->colors = colors;
        value->colors_num = colors_num;
    }
    return value;
}

void free_boxes(struct Box *boxes, uint16_t num)
{
    for (int i=0; i<num; i++)
    {
        struct Box *box = boxes + i;
        if (box)
        {
            struct Label *label = box->label;
            if (label)
            {
                free(label);
            }
            struct Value *value = box->value;
            if (value)
            {
                free(value);
            }
        }
    }
}
