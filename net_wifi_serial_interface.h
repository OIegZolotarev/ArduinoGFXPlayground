
#include "net_interface.h"
#include "wifi_serial.h"

class WifiSerialInterface: public NetworkInterface
{    
    WiFiSerial* serialImpl = nullptr;
public:
    WifiSerialInterface(const char* host, int port);
    ~WifiSerialInterface();

    bool hasData() override;
    bool isConnected() override;

    uint16_t readUInt16() override;
    uint32_t readUInt32() override;
    
    void readStringNullTerminated(char * destBuffer, size_t bufferSize) override;
    void queryMediaInfo() override;
       
    void connect() override;
};