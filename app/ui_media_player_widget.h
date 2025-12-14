
#pragma once

#include "ui_controller.h"
#include "ui_widget.h"

enum class TrackLabelStates
{
    StayingAtStart = 0,
    GoingRight,
    StayingAtEnd,
    GoingLeft
};

#define THUMBNAIL_W 100 
#define THUMBNAIL_H 55

class MediaControllerWidget: public UIWidget
{
    // Media:
    struct 
    {
        bool isPlaying = false;
        char trackName[64] = {0};
        uint16_t trackLength = 0;
        uint16_t trackPosition = 0;
        uint16_t trackPositionFromHost = 0;
        unsigned long update_millis = 0;

        uint16_t trackNameLength = 0;

        TrackLabelStates labelState = TrackLabelStates::StayingAtStart;
        float labelScrollPosition = 0;        
        unsigned long labelNextUpdate = 0;
    }mediaState;

#pragma pack(push,1)
    uint16_t thumbnailData[THUMBNAIL_W * THUMBNAIL_H];
#pragma pack(pop)

    void drawMusicTrackName(ApplicationPlatform* p, int x, int y);
    
	TrackLabelStates nextTrackLabelState(TrackLabelStates current);
    
	int printTimeFormatted(ApplicationPlatform* p,int interval);

	void renderStuffHasTrackData(ApplicationPlatform* p);
    void renderNoMediaLoadedBanner(ApplicationPlatform* p);

public:
    MediaControllerWidget();
    ~MediaControllerWidget();
    
    void render(ApplicationPlatform* platformInstance) override;
    bool handlePhysicalButton(PhysicalButtons btnId) override;

    void onSelected() override;
    void onUnselected() override;

    void updateTrackState(const char* trackName, int trackPos, int trackLength);

    void onNetworkPacketUpdate(responseData_t* packet) override;




    

private:
    void readMediaInfo(responseData_t* packet);
    void readMediaThumbnail(responseData_t* packet);
};