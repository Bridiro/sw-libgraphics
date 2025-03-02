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
sw-lib-graphics/
├── src/
│   └── libgraphics.c    # Implementation of graphical rendering
├── inc/
│   └── libgraphics.h    # Header file for graphics functions
├── test/
│   └── main.c           # Demo to display capabilities
├── libs/
│   └── sw-libfont/      # Font handling library
├── build/               # Compiled object files
├── library.json         # Json file used by PlatformIO to identify libraries
└── app                  # Final compiled application
```

## Usage

1. **Include the Header:**
   Include `libgraphics.h` in your source file to access widget creation and rendering functions.

   ```c
   #include "libgraphics.h"
   ```

2. **Create and Configure Boxes:**
   Initialize and configure boxes with labels, values, or both. Example:

   ```c
   struct Threshold ranges[] = {
      {0.0f, 50.0f, 0x00FF00, 0x000000},
      {50.1f, 100.0f, 0xFFFF00, 0x000000},
      {100.1f, 200.0f, 0xFF0000, 0xFFFFFF}
   };

   struct Thresholds thresholds[] = {
      {ranges, 3}
   };

   struct Label l1;
   create_label(&l1, "XD", (struct Coords){310, 95}, KONEXY_110 0.4, FONT_ALIGN_CENTER);
   struct Value v1;
   create_value(&v1, 51, false, (struct Coords){140, 80}, KONEXY_110 0.7, FONT_ALIGN_CENTER, (union Colors){ .thresholds = thresholds}, THRESHOLDS);

   struct Value v2;
   create_value(&v2, 51, true, (struct Coords){ 196, 80 }, KONEXY_110 0.7, FONT_ALIGN_CENTER, (union Colors){ .slider = (struct Slider){0xff00ff00, ANCHOR_BOTTOM, 0, 200, 3}}, SLIDER);

   struct Label l2;
   create_label(&l2, "PROVA", (struct Coords){196, 80}, KONEXY_110 0.7, FONT_ALIGN_CENTER);

   struct Value v3;
   create_value(&v3, 51.0, true, (struct Coords){ 196, 80 }, KONEXY_110 0.7, FONT_ALIGN_CENTER, (union Colors){ .interpolation = (struct LinearInterpolation){0xff000000, 0xff00ff00, 0.0, 200.0}}, INTERPOLATION);

   struct Box boxes[] = {
       { 1, 0x1, { 2, 2, 397, 237 }, 0xff000000, 0xffffffff, &l1, &v1 },
       { 1, 0x2, { 401, 2, 397, 237 }, 0xff000000, 0xffffffff, NULL, &v2 },
       { 1, 0x3, { 2, 241, 397, 237 }, 0xff000000, 0xffffffff, &l2, NULL },
       { 1, 0x4, { 401, 241, 397, 237 }, 0xff000000, 0xffffffff, NULL, &v3 }
   };
   ```

3. **Rendering:**
   Call the rendering function to draw boxes on the screen:

   ```c
   render_interface(boxes, 4, draw_line_callback, draw_rectangle_callback, clear_screen_callback);
   ```

4. **String Conversions:**
   Interface to values directly by getting pointers to boxes:

   ```c
   struct Box *box = get_box(boxes, 4, 0x1);
   box->value->value = some_number;
   ```

## Compilation

To compile the project, use the provided `Makefile`, so just run `make` in terminal
