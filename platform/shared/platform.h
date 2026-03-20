#pragma once
#include "net_interface.h"

class UIController;
class Arduino_GFX;


enum class TouchEvents
{
	Down,
	Drag,
	Up
};

typedef struct
{
	uint16_t x, y;
} vec2i;

class ApplicationPlatform
{
public:
	ApplicationPlatform() = default;
	virtual ~ApplicationPlatform() = default;

	virtual NetworkInterface* networkInterfaceInstance() = 0;
	virtual Arduino_GFX* gfxInstance() = 0;
	
	virtual void setup() = 0;
	virtual void loop() = 0;

	virtual void submitTouchEvent(TouchEvents event, vec2i pt) = 0;
};

extern UIController* appInstance;
extern NetworkInterface* networkInterface;