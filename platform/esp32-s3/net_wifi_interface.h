
#include "net_interface.h"


class WifiSerialInterface : public NetworkInterface
{
	
public:
	WifiSerialInterface(const char* host, int port);
	~WifiSerialInterface();

	bool hasData() override;
	bool isConnected() override;

	void connect() override;
	void readData() override;

	void performServerCall(ServerCalls callid) override;

};