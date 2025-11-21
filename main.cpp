#include <SDL2/SDL.h>
#include "Arduino_Canvas.h"

#define TFT_W 480
#define TFT_H 272

Arduino_Canvas canvas(TFT_W, TFT_H, NULL); // драйвер не нужен, мы сами сливаем буфер

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "ArduinoCanvas SDL",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        TFT_W * 2, TFT_H * 2,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        TFT_W, TFT_H
    );

    // ArduinoCanvas init
    canvas.begin();

    bool running = true;
    SDL_Event event;

    int x = 0;
    int dx = 1;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // -----------------------------------------
        // --- РИСУЕМ ТАК ЖЕ, КАК НА МИКОКОНТРОЛЛЕРЕ ---
        // -----------------------------------------

        canvas.fillScreen(RGB565_BLACK);
        canvas.setCursor(10, 20);
        canvas.setTextSize(2);
        canvas.setTextColor(RGB565_WHITE);

        canvas.print("Hello World!\n");

        canvas.fillCircle(x, 150, 80, RGB565_RED);

        x += dx;
        if (x < 0 || x + 80 >= TFT_W)
            dx = -dx;

        // -----------------------------------------
        // --- ВЫВОД НА ЭКРАН SDL ---
        // -----------------------------------------

        uint16_t* fb = canvas.getFramebuffer();

        SDL_UpdateTexture(texture, nullptr, fb, TFT_W * sizeof(uint16_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 fps
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
