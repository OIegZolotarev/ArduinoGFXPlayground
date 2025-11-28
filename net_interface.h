#include <stdint.h>

#pragma once

class NetworkInterface
{
protected:
    int connectAttempts = 0;
    unsigned long nextReconnect = 0;
    
    bool wasConnectedPreviousFrame = false;
    bool justConnectedNow = false;
public:
    virtual bool hasData() = 0;
    
    virtual uint16_t readUInt16() = 0;
    virtual uint32_t readUInt32() = 0;
    
    virtual void readStringNullTerminated(char * destBuffer, size_t bufferSize) = 0;
    virtual void queryMediaInfo() = 0;

    virtual bool isConnected() = 0;
        
    virtual void connect() = 0;
    int  connectAttemps() { return connectAttempts; }

    bool justConnected()
    {
        if (justConnectedNow)
        {
            justConnectedNow = false;
            return true;
        }

        return false;
        
    }

    void resetConnectionAttempts()
    {
        connectAttempts = 0;
    }


};

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

typedef struct responseData_s
{
    uint32_t magic;
    uint16_t length;
    uint32_t checksum;
    uint32_t serverCall;
}responseData_t;

