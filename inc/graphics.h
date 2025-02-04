/**
 * @file graphics.h
 * @date 2024-12-13
 * @author Alessandro Bridi [ale.bridi15@gmail.com]
 *
 * @brief Graphics handling functions
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GRAPHICS_OPT 1

#include <stdbool.h>
#include <stdint.h>
#include "text.h"

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

#ifndef TEXT_H
/**
 * @brief Callback to draw a single pixel
 * 
 * @param x Position in pixel on x axis
 * @param y Position in pixel on y axis
 * @param color Color in ARGB format
 */
 typedef void (*draw_pixel_callback_t)(int x, int y, uint32_t color);
#endif

 /**
  * @brief Callback to draw a rectangle (may use DMA2D)
  *
  * @param x Position in piexel on x axis
  * @param y Position in piexel on y axis
  * @param w Width in pixel of rectangle
  * @param h Height in pixel of rectangle
  * @param color Color in ARGB format
  */
 typedef void (*draw_rectangle_callback_t)(int x, int y, int w, int h, uint32_t color);

 /**
  * @brief Function used to clear screen (user decides the color)
  */
 typedef void (*clear_screen_callback_t)(void);

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
    enum FontAlign align;
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

struct Thresholds {
    struct ColorRange *colors;
    uint8_t colors_num;
};

struct LinearInterpolation {
    uint32_t color_min;
    uint32_t color_max;
    float min;
    float max;
};

enum AnchorSlider {
    ANCHOR_TOP,
    ANCHOR_BOTTOM,
    ANCHOR_LEFT,
    ANCHOR_RIGHT,
};

struct Slider {
    uint32_t bg_color;
    uint32_t slider_color;
    enum AnchorSlider anchor;
    float max;
    float min;
    uint32_t margin;
};

enum ColorType {
    THRESHOLDS,
    INTERPOLATION,
    SLIDER,
};

union Colors {
    struct Thresholds *colors;
    struct LinearInterpolation interpolation;
    struct Slider slider;
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
    union Colors colors;
    enum ColorType color_type;
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
#if GRAPHICS_OPT
    bool updated;
#endif
    uint16_t id;
    struct Rect rect;
    uint32_t default_bg_color;
    uint32_t default_fg_color;
    struct Label *label;
    struct Value *value;
};

/**
 * @brief Renders the whole interface
 *
 * @param boxes Pointer to the defined interface
 * @param num Number of Box in the interface
 * @param draw_pixel Draw pixel callback
 * @param draw_rectangle Draw rectangle callback
 * @param clear_screen Clear screen callback
 */
void render_interface(struct Box *boxes, uint16_t num,
                      draw_pixel_callback_t draw_pixel,
                      draw_rectangle_callback_t draw_rectangle
#if GRAPHICS_OPT == 0
                      ,clear_screen_callback_t clear_screen
#endif
                      );

/**
 * @brief Utility to extract 8 bit alpha value from ARGB8888 format
 *
 * @param color Color value
 * @return alpha value
 */
uint8_t get_alpha(uint32_t color);

/**
 * @brief Utility to extract 8 bit red value from ARGB8888 format
 *
 * @param color Color value
 * @return red value
 */
uint8_t get_red(uint32_t color);

/**
 * @brief Utility to extract 8 bit green value from ARGB8888 format
 *
 * @param color Color value
 * @return green value
 */
uint8_t get_green(uint32_t color);

/**
 * @brief Utility to extract 8 bit blue value from ARGB8888 format
 *
 * @param color Color value
 * @return blue value
 */
uint8_t get_blue(uint32_t color);

/**
 * @brief Utility to get a Box based on id value
 *
 * @param boxes Pointer to the defined interface
 * @param num Nunber of Box in the inteface
 * @param id ID of the box to search for
 * @return struct Box*
 *     - Box pointer if found
 *     - NULL if not found
 */
struct Box *get_box(struct Box *boxes, uint16_t num, uint16_t id);

/**
 * @brief Utility to populate struct Label
 *
 * @param label The label struct to populate
 * @param text Text in the label
 * @param pos Position of the text
 * @param font_size Text size
 * @param align Alignment of font
 */
void create_label(struct Label *label, char *text, struct Coords pos, float font_size, enum FontAlign align);

/**
 * @brief Utility to populate struct Value
 *
 * @param value The value struct to populate
 * @param val Value
 * @param is_float Says is the value is to cast to int
 * @param pos Position of the value
 * @param font_size Value size
 * @param align ALignment of font
 * @param colors Pointer to color ranges
 * @param colors_num Number of color ranges
 */
 void create_value(struct Value *value, float val, bool is_float, struct Coords pos, float font_size, enum FontAlign align, union Colors colors, enum ColorType color_type);


#endif // GRAPHICS_H
