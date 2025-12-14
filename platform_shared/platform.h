#pragma once
#include "net_interface.h"

class UIController;
class Arduino_GFX;

class ApplicationPlatform
{
public:
	ApplicationPlatform() = default;
	virtual ~ApplicationPlatform() = default;

	virtual NetworkInterface* networkInterfaceInstance() = 0;
	virtual Arduino_GFX* gfxInstance() = 0;
	
	virtual void setup() = 0;
	virtual void loop() = 0;
};

extern UIController* appInstance;
extern NetworkInterface* networkInterface;