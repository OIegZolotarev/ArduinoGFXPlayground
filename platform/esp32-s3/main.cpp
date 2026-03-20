#include "Arduino.h"
#include "app/ui_controller.h"
#include "platform_esp32s3.h"

//uncomment for most capacitive touchscreen
#define TOUCH_MODULES_GT911 // GT911 / CST_SELF / CST_MUTUAL / ZTW622 / L58 / FT3267 / FT5x06
#define TOUCH_MODULE_ADDR GT911_SLAVE_ADDRESS1 // CTS328_SLAVE_ADDRESS / L58_SLAVE_ADDRESS / CTS826_SLAVE_ADDRESS / CTS820_SLAVE_ADDRESS / CTS816S_SLAVE_ADDRESS / FT3267_SLAVE_ADDRESS / FT5x06_ADDR / GT911_SLAVE_ADDRESS1 / GT911_SLAVE_ADDRESS2 / ZTW622_SLAVE1_ADDRESS / ZTW622_SLAVE2_ADDRESS
#define TOUCH_SCL 4
#define TOUCH_SDA 8
#define TOUCH_RES 38
#define TOUCH_INT 3

#include <Wire.h>
#include <TouchLib.h>

TouchLib touch(Wire, TOUCH_SDA, TOUCH_SCL, TOUCH_MODULE_ADDR);
static uint16_t touchLastX = 0, touchLastY = 0;
static bool touchDown = false;
static uint8_t touchSettled = 10;

UIController* appInstance;
PlatformESP32S3* g_pPlatform;

void setup() 
{

	g_pPlatform = new PlatformESP32S3;
	g_pPlatform->setup();

	// Reset touchscreen
#if (TOUCH_RES > 0)
	pinMode(TOUCH_RES, OUTPUT);
	digitalWrite(TOUCH_RES, 0);
	delay(200);
	digitalWrite(TOUCH_RES, 1);
	delay(200);
#endif
	Wire.begin(TOUCH_SDA, TOUCH_SCL);
	touch.init();
}

bool touch_has_signal()
{
	return true;
}


bool touch_touched()
{
	if (touch.read())
	{
		TP_Point t = touch.getPoint(0);

		touchLastX = t.x;
		touchLastY = t.y;

		return true;
	}

	return false;
}

void loop()
{
	g_pPlatform->loop();

	if (touch_has_signal() && touchSettled == 0)
	{
		if (touch_touched())
		{
			if (touchDown)
				g_pPlatform->submitTouchEvent(TouchEvents::Drag, { touchLastX, touchLastY });
			else
			{
				g_pPlatform->submitTouchEvent(TouchEvents::Down, { touchLastX, touchLastY });
				touchDown = true;
			}
		}
		else 
		{
			if (touchDown)
				g_pPlatform->submitTouchEvent(TouchEvents::Up, { touchLastX, touchLastY });

			touchDown = false;
		}
	}
	else
	{
		// g_pPlatform->submitTouchEvent(TouchEvents::Up, { touchLastX, touchLastY });
		touchSettled--;
	}

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