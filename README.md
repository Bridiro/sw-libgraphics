# Text Widget Library

This library provides a simple yet powerful way to create graphical widgets for the steering wheel user interface, specifically focusing on the Box widget, which supports displaying and styling labels and values.

## Features

- **Dynamic Widget Composition:**

  - Widgets can consist of a label, a value, or both.
  - Saves memory by omitting unused components.

- **Styling Options:**

  - Independent font sizes and positions for labels and values.
  - Configurable background and foreground colors.
  - Supports color ranges for values, allowing dynamic styling based on thresholds.

- **Value Formatting:**

  - Supports floating-point and integer values.

## File Structure

```
project/
├── src/
│   ├── graphics.c       # Implementation of graphical rendering
│   └── main.c           # Demo application to display capabilities
├── inc/
│   └── graphics.h       # Header file for graphics functions
├── libs/
│   └── mcufont/         # Font handling library
├── build/               # Compiled object files
└── app                  # Final compiled application
```

## Usage

1. **Include the Header:**
   Include `graphics.h` in your source file to access widget creation and rendering functions.

   ```c
   #include "graphics.h"
   ```

2. **Init graphics API:**
   ```c
   struct GraphicsAPI api = {
       .draw_pixel = draw_pixel_function,
       .draw_rectangle = draw_rectangle_function,
       .clear_screen = clear_screen_function
   };
   init_graphics_api(&api);
   ```

3. **Create and Configure Boxes:**
   Initialize and configure boxes with labels, values, or both. Example:

   ```c
   struct ColorRange ranges[] = {
       {0.0f, 50.0f, 0x00FF00, 0x000000},
       {50.1f, 100.0f, 0xFFFF00, 0x000000},
       {100.1f, 200.0f, 0xFF0000, 0xFFFFFF}
   };

   struct Box boxes[] = {
       { 0x1, { 2, 2, 397, 237 }, 0xff000000, 0xffffffff, create_label("XD", (struct Coords){310, 95}, 0.4, CENTER), create_value(51, false, (struct Coords){140, 80}, 0.6, CENTER, ranges, 3) },
       { 0x2, { 401, 2, 397, 237 }, 0xff000000, 0xffffffff, create_label("SI", (struct Coords){196, 80}, 0.7, CENTER), NULL },
       { 0x3, { 2, 241, 397, 237 }, 0xff000000, 0xffffffff, create_label("PROVA", (struct Coords){196, 80}, 0.5, CENTER), NULL },
       { 0x4, { 401, 241, 397, 237 }, 0xff000000, 0xffffffff, NULL, create_value(51.0, true, (struct Coords){ 196, 80 }, 0.6, CENTER, ranges, 3) },
   };
   ```

3. **Rendering:**
   Call the rendering function to draw boxes on the screen:

   ```c
   render_interface(boxes, 4);
   ```

4. **String Conversions:**
   Interface to values directly by getting pointers to boxes:

   ```c
   struct Box *box = get_box(boxes, 4, 0x1);
   box->value->value = some_number;
   ```

## Compilation

To compile the project, use the provided `Makefile`, so just run `make` in terminal
