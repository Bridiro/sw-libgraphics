#include <SDL.h>
#include <stdint.h>
#include "SDL_timer.h"
#include "graphics.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480

// Struttura globale per il contesto SDL
typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} SDLContext;

void sdl_clear_screen()
{
}

// Funzione per disegnare un pixel utilizzando SDL
void sdl_draw_pixel(int x, int y, uint32_t color)
{
    SDL_SetRenderDrawColor(SDL_GetRenderer(SDL_GetWindowFromID(1)),
                           get_red(color), get_green(color), get_blue(color), get_alpha(color));
    SDL_RenderDrawPoint(SDL_GetRenderer(SDL_GetWindowFromID(1)), x, y);
}

// Funzione per disegnare un rettangolo utilizzando SDL
void sdl_draw_rectangle(int x, int y, int w, int h, uint32_t color)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(SDL_GetRenderer(SDL_GetWindowFromID(1)),
                           get_red(color), get_green(color), get_blue(color), get_alpha(color));
    SDL_RenderFillRect(SDL_GetRenderer(SDL_GetWindowFromID(1)), &rect);
}

int main()
{
    // Inizializzazione di SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDLContext sdl_ctx = {
        .window = SDL_CreateWindow("Graphics Demo",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   WINDOW_WIDTH,
                                   WINDOW_HEIGHT,
                                   SDL_WINDOW_SHOWN),
        .renderer = NULL};

    if (!sdl_ctx.window)
    {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    sdl_ctx.renderer = SDL_CreateRenderer(sdl_ctx.window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");

    if (!sdl_ctx.renderer)
    {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(sdl_ctx.window);
        SDL_Quit();
        return 1;
    }

    // Definizione delle API grafiche
    struct GraphicsAPI api = {
        .draw_pixel = sdl_draw_pixel,
        .draw_rectangle = sdl_draw_rectangle,
        .clear_screen = sdl_clear_screen};
    init_graphics_api(&api);

    // Definizione dei widget
    struct ColorRange ranges[] = {
        {0.0f, 50.0f, 0x00FF00, 0x000000},
        {50.1f, 100.0f, 0xFFFF00, 0x000000},
        {100.1f, 200.0f, 0xFF0000, 0xFFFFFF}
    };

    struct Box boxes[] = {
        { 1, 0x1, { 2, 2, 397, 237 }, 0xff000000, 0xffffffff, create_label("XD", (struct Coords){310, 95}, 0.4, CENTER), create_value(51, false, (struct Coords){140, 80}, 0.6, CENTER, ranges, 3) },
        { 1, 0x2, { 401, 2, 397, 237 }, 0xff000000, 0xffffffff, create_label("SI", (struct Coords){196, 80}, 0.7, CENTER), NULL },
        { 1, 0x3, { 2, 241, 397, 237 }, 0xff000000, 0xffffffff, create_label("PROVA", (struct Coords){196, 80}, 0.5, CENTER), NULL },
        { 1, 0x4, { 401, 241, 397, 237 }, 0xff000000, 0xffffffff, NULL, create_value(51.0, true, (struct Coords){ 196, 80 }, 0.6, CENTER, ranges, 3) },
    };

    // Ciclo principale
    int running = 1;
    SDL_Event event;
    uint32_t last_time = SDL_GetTicks();
    int dir = 1;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        // Pulisci lo schermo
        SDL_SetRenderDrawColor(sdl_ctx.renderer, 255, 255, 255, 255);
        SDL_RenderClear(sdl_ctx.renderer);

        // Renderizza i widget
        render_interface(boxes, 4);

        if (SDL_GetTicks() - last_time > 60)
        {
            struct Box *box = get_box(boxes, 4, 0x1);
            struct Box *box2 = get_box(boxes, 4, 0x4);
            box->value->value += dir;
            box2->value->value += dir;
            if (box->value->value > 199)
                dir = -1;
            else if (box->value->value < 2)
                dir = 1;
        }

        // Mostra il risultato
        SDL_RenderPresent(sdl_ctx.renderer);

        SDL_Delay(16); // Limitazione a ~60 FPS
    }

    free_boxes(boxes, 2);
    // Cleanup di SDL
    SDL_DestroyRenderer(sdl_ctx.renderer);
    SDL_DestroyWindow(sdl_ctx.window);
    SDL_Quit();

    return 0;
}
