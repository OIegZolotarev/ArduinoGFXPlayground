#include "Arduino.h"

#include "app/ui_controller.h"

#include "platform_esp32s3.h"


UIController* appInstance;
PlatformESP32S3* g_pPlatform;

void setup() 
{

	g_pPlatform = new PlatformESP32S3;
	g_pPlatform->setup();
}

void loop()
{
	g_pPlatform->loop();
}

extern "C" void app_main() 
{
	initArduino();
	setup();

	while (true) 
	{
		loop();
		vTaskDelay(1);
	}
}