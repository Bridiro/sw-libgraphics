#include "graphics.h"

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

void _draw_text_box(struct TextBox *text_box)
{
    api->draw_rectangle(text_box->rect.x,
                        text_box->rect.y,
                        text_box->rect.w,
                        text_box->rect.h,
                        text_box->bg_color);

    _draw_text(text_box->rect.x + text_box->text_pos.x,
               text_box->rect.y + text_box->text_pos.y,
               text_box->font_aling,
               text_box->text,
               text_box->fg_color,
               text_box->font_size);
}

void init_graphics_api(struct GraphicsAPI *a)
{
    api = a;
}

void render_interface(struct TextBox *text_boxes, uint16_t num)
{
    api->clear_screen();
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

struct TextBox *get_text_box(struct TextBox *text_boxes, uint16_t num, uint16_t id)
{
    for (int i=0; i<num; i++) {
        if ((text_boxes + i)->id == id) {
            return (text_boxes + i);
        }
    }
    return NULL;
}
