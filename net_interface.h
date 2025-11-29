#include <stdint.h>

#pragma once

#define PACKET_MAGIC 0x0000FEFF


typedef struct responseData_s
{
	uint32_t magic;
	uint32_t length;
	uint32_t checksum;
	uint32_t serverCall;
}responseData_t;



enum ServerCalls
{
	MediaInfo = 0,
	MediaState,
	MediaThumbnail,
	Play,
	Pause,
	PreviousTrack,
	NextTrack,
	VolumeUp,
	VolumeDown,
	Mute
};


class NetworkInterface
{
protected:
    int connectAttempts = 0;
    unsigned long nextReconnect = 0;
    
    bool wasConnectedPreviousFrame = false;
    bool justConnectedNow = false;

    uint8_t dataBuffer[2048];
    size_t bufferOffset = 0;
    
    responseData_t* responseData = (responseData_t*)dataBuffer;

public:

    virtual void readData() = 0;
    virtual void performServerCall(ServerCalls callid) = 0;

    virtual bool hasData() = 0;
    
    
    virtual bool isConnected() = 0;        
    virtual void connect() = 0;
    int  connectAttemps();

    bool justConnected();
    void resetConnectionAttempts();

};

class ResponseReader
{
    size_t offset = 0;
    responseData_t* packet = nullptr;
public:
    ResponseReader(responseData_t* r);

	virtual uint16_t readUInt16();
	virtual uint32_t readUInt32();
	virtual void readStringNullTerminated(char* destBuffer, size_t bufferSize);
};