#include "net_wifi_serial_interface.h"
#include "timer.h"
#include <ui_controller.h>



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



void WifiSerialInterface::connect()
{
	if (millis() < nextReconnect)
		return;

	connectAttempts++;
	serialImpl->reconnect();

	nextReconnect = nextReconnect + 5000;
}

void WifiSerialInterface::readData()
{
	auto transferToBuffer = [&](std::vector<uint8_t> & received)
	{
			uint8_t* bufferPtr = dataBuffer + bufferOffset;
			memcpy(bufferPtr, received.data(), received.size());
			bufferOffset += received.size();
	};

	size_t leftToRead = 0;

	if (bufferOffset < sizeof(responseData_t))
	{
		leftToRead = sizeof(responseData_t) - bufferOffset;
		
		auto received = serialImpl->readBytes(leftToRead);
		transferToBuffer(received);
	}
	else
	{
		if (responseData->magic != PACKET_MAGIC)
		{
			bufferOffset = 0;
			return;
		}

		leftToRead = responseData->length - bufferOffset;

		auto received = serialImpl->readBytes(leftToRead);
		transferToBuffer(received);

		if (bufferOffset == responseData->length)
		{
			g_uiController->onPacketReceived(responseData);
			bufferOffset = 0;
		}
	}
	
}

void WifiSerialInterface::performServerCall(ServerCalls callId)
{
	if (!isConnected())
		return;

	serialImpl->write({ 0xFE, 0xFF, (uint8_t)callId });
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
