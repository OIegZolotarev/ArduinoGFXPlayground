#include "app/ui_controller.h"
#include "platform_esp32s3.h"
#include "arduino_gfx.h"
#include "esp_log.h"

#include "buttons.h"

static const char* TAG = "PlatformESP32S3";
#define GFX_BL 1


extern UIController* appInstance;

PlatformESP32S3::~PlatformESP32S3()
{
	delete appInstance;
}

NetworkInterface* PlatformESP32S3::networkInterfaceInstance()
{
	return nullptr;
}

Arduino_GFX* PlatformESP32S3::gfxInstance()
{
	return gfx;
}

void PlatformESP32S3::setup()
{
	appInstance = new UIController(this);
	
	// Init Display
	if (!gfx->begin())
	{
		ESP_LOGE(TAG, "gfx->begin() failed!");
		while (1);
	}
	else
	{
		ESP_LOGI(TAG, "gfx->begin() success");
	}

	// ¬ключить подсветку
	pinMode(GFX_BL, OUTPUT);
	digitalWrite(GFX_BL, 255);

	ESP_LOGI(TAG, "backlight enabled");

	initButtons();
}

void PlatformESP32S3::loop()
{
	handleEvents();
	appInstance->render();
	gfx->flush();
}

void PlatformESP32S3::submitTouchEvent(TouchEvents event, vec2i pt)
{

	switch (event)
	{
	case TouchEvents::Down:
		// ESP_LOGD("TOUCH", "submitTouchEvent(): Down at [%d, %d]\n", pt.x, pt.y);
		break;
	case TouchEvents::Drag:
		// ESP_LOGD("TOUCH", "submitTouchEvent(): Drag at [%d, %d]\n", pt.x, pt.y);
		break;
	case TouchEvents::Up:
		// ESP_LOGD("TOUCH", "submitTouchEvent(): Up at [%d, %d]\n", pt.x, pt.y);
		break;
	default:
		break;
		
	}
		
	appInstance->handleTouchEvent(event, pt);
}

bool PlatformESP32S3::handleEvents()
{
	pollButtons();
	return false;
}

PlatformESP32S3::PlatformESP32S3()
{

}
