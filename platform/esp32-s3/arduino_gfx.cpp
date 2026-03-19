#include "arduino_gfx.h"

class ESP32QSPIWrapper : public Arduino_ESP32QSPI
{
public:
	ESP32QSPIWrapper(int cs, int sck, int d0, int d1, int d2, int d3) : Arduino_ESP32QSPI(cs, sck, d0, d1, d2, d3)
	{

	}

	virtual bool begin(int32_t speed, int8_t dataMode)
	{
		return Arduino_ESP32QSPI::begin(60000000, dataMode);
	}
};

Arduino_DataBus* bus = new ESP32QSPIWrapper(45 /* cs */, 47 /* sck */, 21 /* d0 */, 48 /* d1 */, 40 /* d2 */, 39 /* d3 */);



Arduino_NV3041A* g = new Arduino_NV3041A(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);
Arduino_GFX* gfx = new Arduino_Canvas(480 /* width */, 272 /* height */, g);