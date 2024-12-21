#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <mcufont.h>

/**
 * @brief Rectangle definition for the UI
 *
 * @details 
 *     - x is the position in pixel on the x axis
 *     - y is the position in pixel on the y axis
 *     - w is the width in pixel
 *     - h is the height in pixel
 */
struct Rect
{
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
};

/**
 * @brief Represents a set of coordinates
 *
 * @details
 *     - x is the position in pixel on the x axis
 *     - y is the position in pixel on the y axis
 */
struct Coords
{
    uint32_t x;
    uint32_t y;
};

/**
 * @brief Represents an abstraction over mf_align_t
 *
 * @details
 *     - LEFT means that the text rendering will start from Coords.x to the left
 *     - CENTER means that the text rendering will be centered to Coords.x
 *     - RIGHT means that the text rendering will end at Coords.x
 */
enum FontAlign
{
    LEFT,
    CENTER,
    RIGHT
};

/**
 * @brief Defines 3 graphics functions for the library to work
 *
 * @details
 *     - draw_pixel is a pointer to a function that draws a pixel
 *     - draw_rectangle is a pointer to a function that draws a rectangle (may use DMA2D)
 *     - clear_screen is a pointer to a function that sets all pixel to black or white (user's choice)
 */
struct GraphicsAPI
{
    void (*draw_pixel)(int x, int y, uint32_t color);
    void (*draw_rectangle)(int x, int y, int w, int h, uint32_t color);
    void (*clear_screen)();
};

/**
 * @brief Defines a range for whom the color may be applied
 *
 * @details
 *     - min is the min value for whom the color is applied
 *     - max is the max value for whom the color is applied
 *     - bg_color is the background color to apply
 *     - fg_color is the foreground color to apply
 */
struct ColorRange
{
    float min;
    float max;
    uint32_t bg_color;
    uint32_t fg_color;  
};

/**
 * @brief Defines one component of the UI
 *
 * @details
 *     - text is a pointer to the string to plot
 *     - pos is the position where to plot the text
 *     - font_size defines the size of the text
 *     - align defines the alignement of the text
 */
struct Label
{
    char *text;
    struct Coords pos;
    float font_size;
    enum FontAlign aling;
};

/**
 * @brief Defines one component of the UI
 *
 * @details
 *     - value is the value to display
 *     - is_float tells if the value is to be casted
 *     - pos is the position there to plot the value
 *     - font_size defines the size of the text
 *     - align defines the alignement of the text
 *     - colors contains all the color ranges
 *     - color_nums contains the number of ranges
 */
struct Value
{
    float value;
    bool is_float;
    struct Coords pos;
    float font_size;
    enum FontAlign align;
    struct ColorRange *colors;
    uint8_t colors_num;
};

/**
 * @brief Defines a composed component of the UI
 *
 * @details
 *     - id is used to identify the component (can be used with CAN ids)
 *     - rect is to define the boundings of the Box
 *     - default_bg_color is used if no ranges are present or in range
 *     - default_fg_color is used if no ranges are present or in range
 *     - label is a pointer to Label component
 *     - value is a pointer to Value component
 */
struct Box
{
    uint16_t id;
    struct Rect rect;
    uint32_t default_bg_color;
    uint32_t default_fg_color;
    struct Label *label;
    struct Value *value;
};

void init_graphics_api(struct GraphicsAPI *a);
void render_interface(struct Box *boxes, uint16_t num);
uint8_t get_alpha(uint32_t color);
uint8_t get_red(uint32_t color);
uint8_t get_green(uint32_t color);
uint8_t get_blue(uint32_t color);
struct Box *get_box(struct Box *boxes, uint16_t num, uint16_t id);
uint32_t color_modify_rgb(uint32_t color, int8_t delta);
struct Label *create_label(char *text, struct Coords pos, float font_size, enum FontAlign align);
struct Value *create_value(float val, bool is_float, struct Coords pos, float font_size, enum FontAlign align, struct ColorRange *colors, uint8_t colors_num);
void free_boxes(struct Box *boxes, uint16_t num);

#endif // GRAPHICS_H
