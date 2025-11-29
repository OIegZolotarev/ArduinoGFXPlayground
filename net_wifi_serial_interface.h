
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
       
    void connect() override;
    void readData() override;

    void performServerCall(ServerCalls callid) override;

};