
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

class MediaControllerWidget: public UIWidget
{
    // Media:
    struct 
    {
        bool playing = false;
        char trackName[64] = {0};
        int trackLength = 0;
        int trackPosition = 0;
        int trackPositionFromHost = 0;
        unsigned long update_millis = 0;

        int trackNameLength = 0;        

        TrackLabelStates labelState = TrackLabelStates::StayingAtStart;
        float labelScrollPosition = 0;        
        unsigned long labelNextUpdate = 0;
    }mediaState;

    void drawMusicTrackName(int x, int y);
    
	TrackLabelStates nextTrackLabelState(TrackLabelStates current);
    
	int printTimeFormatted(int interval);

	void renderStuffHasTrackData();


public:
    MediaControllerWidget();
    ~MediaControllerWidget();
    
    void render() override;
    bool handlePhysicalButton(PhysicalButtons btnId) override;

    void onSelected() override;
    void onUnselected() override;

    void updateTrackState(const char* trackName, int trackPos, int trackLength);

    void onNetworkPacketUpdate(responseData_t* packet) override;




    void renderNoMediaLoadedBanner();

private:
    void readMediaInfo(responseData_t* packet);
    void readMediaState(responseData_t* packet);
    void readMediaThumbnail();
};