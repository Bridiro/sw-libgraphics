#include "graphics.h"

struct GraphicsAPI *api = NULL;

extern const struct mf_rlefont_s mf_rlefont_KonexyFont72;

void _pixel_callback(int16_t x, int16_t y, uint8_t count, uint8_t alpha, void *state) {
    struct font_color_s *rstate = state;
    while (count--) {
        api->draw_pixel(x, y, rstate->color | alpha << 24);
        x++;
    }
}

uint8_t _char_callback(int16_t x0, int16_t y0, mf_char character, void *state) {
    float size = ((struct font_color_s *)state)->font->x_scale;
    struct mf_scaledfont_s scaled_font;
    mf_scale_font(&scaled_font, &mf_rlefont_KonexyFont72.font, size, size);
    return mf_render_character(&scaled_font.font, x0, y0, character, &_pixel_callback, state);
}

void _draw_text(int16_t x, int16_t y, enum mf_align_t align, const char* text, uint32_t color, float size) {
    struct mf_scaledfont_s scaled_font;
    mf_scale_font(&scaled_font, &mf_rlefont_KonexyFont72.font, size, size);
    struct font_color_s mf_data = (struct font_color_s){
        .font  = &scaled_font,
        .color = color,
    };

    mf_render_aligned(&scaled_font.font, x, y, align, text, strlen(text), &_char_callback, (void *)&mf_data);
}

void init_graphics_api(struct GraphicsAPI *a) {
    api = a;
}

void draw_text_box(struct Rect rect, uint32_t bg_color, uint32_t fg_color, const char *text) {
    api->draw_rectangle(rect.x, rect.y, rect.w, rect.h, bg_color);
    _draw_text(rect.x, rect.y, MF_ALIGN_CENTER, text, fg_color, 0.5);
}

void render_interface(const struct Widget *widgets) {
    printf("Eh dai");
}

