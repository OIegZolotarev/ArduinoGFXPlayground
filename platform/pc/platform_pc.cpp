#include "platform_pc.h"
#include <app/ui_controller.h>
#include <SDL.h>
#include <timer.h>

Arduino_Canvas canvas(TFT_W, TFT_H, NULL); // драйвер не нужен, мы сами сливаем буфер
Arduino_GFX* gfx = &canvas;

UIController* appInstance;

PlatformPC::PlatformPC()
{
	appInstance = new UIController(this);

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
		"ArduinoCanvas SDL",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		TFT_W * 2, TFT_H * 2,
		SDL_WINDOW_SHOWN
	);

	renderer = SDL_CreateRenderer(window, -1, SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC);

	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGB565,
		SDL_TEXTUREACCESS_STREAMING,
		TFT_W, TFT_H
	);

}

PlatformPC::~PlatformPC()
{
	delete appInstance;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

NetworkInterface* PlatformPC::networkInterfaceInstance()
{
	return nullptr;
}

Arduino_GFX* PlatformPC::gfxInstance()
{
	return ::gfx;
}


void PlatformPC::setup()
{
	timerStart();
	canvas.begin();
}

void PlatformPC::loop()
{
	while (running)
	{
		if (!handleEvents())
		{
			running = false;
			break;
		}

		appInstance->render();

		uint16_t* fb = canvas.getFramebuffer();

		SDL_UpdateTexture(texture, nullptr, fb, TFT_W * sizeof(uint16_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);

		timerUpdate();
	}

}

bool PlatformPC::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0) {
		// Process individual events
		switch (event.type) {
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			// Handle keyboard key down event
			// Example: if (e.key.keysym.sym == SDLK_ESCAPE) quit = true;

			if (event.key.keysym.sym == SDLK_1)
				appInstance->handlePhysicalButton(PhysicalButtons::FUNC1);
			else if (event.key.keysym.sym == SDLK_2)
				appInstance->handlePhysicalButton(PhysicalButtons::FUNC2);
			else if (event.key.keysym.sym == SDLK_3)
				appInstance->handlePhysicalButton(PhysicalButtons::FUNC3);
			else if (event.key.keysym.sym == SDLK_4)
				appInstance->handlePhysicalButton(PhysicalButtons::FUNC4);
			else if (event.key.keysym.sym == SDLK_LEFT)
				appInstance->handlePhysicalButton(PhysicalButtons::LEFT);
			else if (event.key.keysym.sym == SDLK_RIGHT)
				appInstance->handlePhysicalButton(PhysicalButtons::RIGHT);
			else if (event.key.keysym.sym == SDLK_UP)
				appInstance->handlePhysicalButton(PhysicalButtons::UP);
			else if (event.key.keysym.sym == SDLK_DOWN)
				appInstance->handlePhysicalButton(PhysicalButtons::DOWN);

			break;
		}
	}

	return true;
}

