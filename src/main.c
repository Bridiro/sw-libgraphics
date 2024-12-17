#include <SDL.h>
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
    struct Widget widgets[] = {
        { { 5, 5, 260, 233 }, 0xffff0000, 0xff00ffff, 1.0, CENTER, { 130, 40 }, "a" },
        { { 270, 5, 260, 233 }, 0xff00ff00, 0xffff00ff, 1.0, CENTER, { 130, 40 }, "b" },
        { { 535, 5, 260, 233 }, 0xff0000ff, 0xffffff00, 1.0, CENTER, { 130, 40 }, "c" },
        { { 5, 243, 260, 232 }, 0xffff00ff, 0xff00ff00, 1.0, CENTER, { 130, 40 }, "d" },
        { { 270, 243, 260, 232 }, 0xffffff00, 0xff0000ff, 1.0, CENTER, { 130, 40 }, "e" },
        { { 535, 243, 260, 232 }, 0xff00ffff, 0xffff0000, 1.0, CENTER, { 130, 40 }, "f" }
    };

    // Ciclo principale
    int running = 1;
    SDL_Event event;
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
        render_interface(widgets, 6);

        // Mostra il risultato
        SDL_RenderPresent(sdl_ctx.renderer);

        SDL_Delay(16); // Limitazione a ~60 FPS
    }

    // Cleanup di SDL
    SDL_DestroyRenderer(sdl_ctx.renderer);
    SDL_DestroyWindow(sdl_ctx.window);
    SDL_Quit();

    return 0;
}
