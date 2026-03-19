#include "../shared/platform.h"

class PlatformESP32S3 : public ApplicationPlatform
{

public:
	PlatformESP32S3();
	virtual ~PlatformESP32S3();

	NetworkInterface* networkInterfaceInstance() override;
	Arduino_GFX* gfxInstance() override;

	void setup() override;
	void loop() override;
private:

	bool handleEvents();
	bool running = true;
};
