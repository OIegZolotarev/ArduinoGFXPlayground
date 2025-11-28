#include "net_wifi_serial_interface.h"
#include "timer.h"



WifiSerialInterface::WifiSerialInterface(const char* host, int port)
{
	serialImpl = new WiFiSerial(host, port);
}

WifiSerialInterface::~WifiSerialInterface()
{
	if (serialImpl)
		delete serialImpl;
}

bool WifiSerialInterface::hasData()
{
	return serialImpl->hasData();
}

uint16_t WifiSerialInterface::readUInt16()
{
	uint16_t r;
	serialImpl->readUint16(r);
	return r;
}

uint32_t WifiSerialInterface::readUInt32()
{
	uint32_t r;
	serialImpl->readUint32(r);
	return r;
}

void WifiSerialInterface::readStringNullTerminated(char* destBuffer, size_t bufferSize)
{
	serialImpl->readNullTerminatedString(destBuffer, bufferSize);
}

void WifiSerialInterface::queryMediaInfo()
{
	serialImpl->write({ 'I', 'N', 'F', 'O' });
}

void WifiSerialInterface::connect()
{
	if (millis() < nextReconnect)
		return;

	connectAttempts++;
	serialImpl->reconnect();

	nextReconnect = nextReconnect + 5000;
}

bool WifiSerialInterface::isConnected()
{
	bool r = serialImpl->isConnected();

	if (r == true && !wasConnectedPreviousFrame)
	{
		justConnectedNow = true;
		wasConnectedPreviousFrame = true;
	}
	else
	{
		justConnectedNow = false;
	}

	if (!r)
		wasConnectedPreviousFrame = false;

	return r;
}
