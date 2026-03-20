#include "../shared/platform.h"
#include <SDL2/SDL.h>

#include "net_wifi_serial_interface.h"


class PlatformPC : public ApplicationPlatform
{

public:
	PlatformPC();
	virtual ~PlatformPC();

	NetworkInterface* networkInterfaceInstance() override;
	Arduino_GFX* gfxInstance() override;


	void setup() override;
	void loop() override;

	void submitTouchEvent(TouchEvents event, vec2i pt) override;

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	bool handleEvents();
	bool running = true;
	bool mouseDown = false;

	WifiSerialInterface* networkInterface = nullptr;
};