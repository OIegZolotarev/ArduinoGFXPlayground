#include <SDL2/SDL.h>
#include "Arduino_Canvas.h"
#include "ui_controller.h"
#include <chrono>
#include "timer.h"

Arduino_Canvas canvas(TFT_W, TFT_H, NULL); // драйвер не нужен, мы сами сливаем буфер
Arduino_GFX * gfx = &canvas;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "ArduinoCanvas SDL",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        TFT_W * 2, TFT_H * 2,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        TFT_W, TFT_H
    );

    // ArduinoCanvas init
    canvas.begin();
    timerStart();

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        DrawUI();

        uint16_t* fb = canvas.getFramebuffer();

        SDL_UpdateTexture(texture, nullptr, fb, TFT_W * sizeof(uint16_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        timerUpdate();
        
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
