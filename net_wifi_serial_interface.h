
#include "net_interface.h"
#include "WifiSerial.h"

class WifiSerialInterface: public NetworkInterface
{
    
    WiFiSerial* serialImpl = nullptr;
public:
    WifiSerialInterface(const char* host, int port)
    {
        serialImpl = new WiFiSerial(host, port);
    }

    ~WifiSerialInterface()
    {
        if (serialImpl)
            delete serialImpl;
    }

    virtual bool hasData() 
    {
        return serialImpl->hasData();
    }
    
    virtual uint16_t readUInt16() 
    {
        uint16_t r;
        serialImpl->readUint16(r);
        return r;
    }


    virtual uint32_t readUInt32() 
    {
        uint32_t r;
        serialImpl->readUint32(r);
        return r;
    }
    
    virtual void readStringNullTerminated(char * destBuffer, size_t bufferSize)
    {
        serialImpl->readNullTerminatedString(destBuffer, bufferSize);
    }

    virtual void queryMediaInfo()
    {
        serialImpl->write({'I', 'N', 'F', 'O'});
    }
};