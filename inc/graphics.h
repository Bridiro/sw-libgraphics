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
 *     - x Is the position in pixel on the x axis
 *     - y Is the position in pixel on the y axis
 *     - w Is the width in pixel
 *     - h Is the height in pixel
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
 *     - x Is the position in pixel on the x axis
 *     - y Is the position in pixel on the y axis
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
 *     - text Is a pointer to the string to plot
 *     - pos Is the position where to plot the text
 *     - font_size Defines the size of the text
 *     - align Defines the alignement of the text
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
 *     - min Is the min value for whom the color is applied
 *     - max Is the max value for whom the color is applied
 *     - bg_color Is the background color to apply
 *     - fg_color Is the foreground color to apply
 */
struct Threshold
{
    float min;
    float max;
    uint32_t bg_color;
    uint32_t fg_color;  
};

/**
 * @brief Includes all the thresholds in one struct
 *
 * @details
 *     - threshold Pointer to Threshold struct
 *     - thresholds_num Number of thresholds in the array
 */
struct Thresholds {
    struct Threshold *threshold;
    uint8_t thresholds_num;
};

/**
 * @brief Defines an interpolation between 2 colors based on 2 values
 *
 * @details
 *     - color_min The color applied if the value is lower or equal to min
 *     - color_max The color applied if the value is higher ot equal to max
 *     - min The minimum value
 *     - max The maximum value
 */
struct LinearInterpolation {
    uint32_t color_min;
    uint32_t color_max;
    float min;
    float max;
};

/**
 * @brief Defines how the slider should behave (where to attach in the box)
 *
 * @details
 *     - ANCHOR_<self descriptive> Attaches in the direction described in the name
 */
enum AnchorSlider {
    ANCHOR_TOP,
    ANCHOR_BOTTOM,
    ANCHOR_LEFT,
    ANCHOR_RIGHT,
};

/**
 * @brief Describes all the useful parameters of the slider
 *
 * @details
 *     - color The color of the slider (background used is the default one)
 *     - anchor Defines on which side of the box to attach the slider
 *     - max Max value of the slider
 *     - min Min value of the slider
 *     - margin Space to leave in between the box borders and the slider
 */
struct Slider {
    uint32_t color;
    enum AnchorSlider anchor;
    float max;
    float min;
    uint32_t margin;
};

/**
 * @brief Defines which type of box is this
 */
enum ColorType {
    THRESHOLDS,
    INTERPOLATION,
    SLIDER,
};

/**
 * @brief Contains the struct that defines one coloring method. ColorType should match the one placed in here.
 */
union Colors {
    struct Thresholds *thresholds;
    struct LinearInterpolation interpolation;
    struct Slider slider;
};

/**
 * @brief Defines one component of the UI
 *
 * @details
 *     - value Is the value to display
 *     - is_float Tells if the value is to be casted
 *     - pos Is the position there to plot the value
 *     - font_size Defines the size of the text
 *     - align Defines the alignement of the text
 *     - colors Contains one possible coloring method
 *     - color_type Describes which coloring method to use with the box based on the value
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
 *     - updated Is used to optimize the interface drawing (usable only is GRAPHICS_OPT == 1)
 *     - id Is used to identify the component
 *     - rect Defines the boundings of the Box
 *     - default_bg_color Is used if no ranges are present or in range
 *     - default_fg_color Is used if no ranges are present or in range
 *     - label Is a pointer to Label component
 *     - value Is a pointer to Value component
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
