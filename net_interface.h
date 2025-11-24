#include <stdint.h>

#pragma once

class NetworkInterface
{
public:
    virtual bool hasData() = 0;
    
    virtual uint16_t readUInt16() = 0;
    virtual uint32_t readUInt32() = 0;
    
    virtual void readStringNullTerminated(char * destBuffer, size_t bufferSize) = 0;

    virtual void queryMediaInfo() = 0;
};