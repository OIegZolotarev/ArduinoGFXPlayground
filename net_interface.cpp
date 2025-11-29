#include "net_interface.h"



int NetworkInterface::connectAttemps()
{
	return connectAttempts;
}

bool NetworkInterface::justConnected()
{
	if (justConnectedNow)
	{
		justConnectedNow = false;
		return true;
	}

	return false;
}

void NetworkInterface::resetConnectionAttempts()
{
	connectAttempts = 0;
}

ResponseReader::ResponseReader(responseData_t* r)
{
	packet = r;
	offset = sizeof(responseData_t);
}

uint16_t ResponseReader::readUInt16()
{
	uint16_t r = *(uint16_t*)((uint8_t*)packet + offset);

	offset += sizeof(uint16_t);
	return r;
}

uint32_t ResponseReader::readUInt32()
{
	uint32_t r = *(uint32_t*)((uint8_t*)packet + offset);

	offset += sizeof(uint32_t);
	return r;
}

void ResponseReader::readStringNullTerminated(char* destBuffer, size_t bufferSize)
{
	uint8_t* p = (uint8_t*)packet + offset;
	uint8_t* d = (uint8_t*)destBuffer;

	size_t count = 0;

	while (*p && count < (bufferSize - 1))
	{
		*d++ = *p++;
		count++;
	}

	*d = 0;

	// Skip null byte
	offset += count + 1;
}
