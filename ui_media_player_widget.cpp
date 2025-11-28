#include <string.h>

#include "ui_widget.h"
#include "ui_media_player_widget.h"
#include "timer.h"
#include "com_colors.h"

MediaControllerWidget::MediaControllerWidget()
{

}

MediaControllerWidget::~MediaControllerWidget()
{

}

void  MediaControllerWidget::drawMusicTrackName(int x, int y)
{
    char* p = (char*)mediaState.trackName;
       
    int trackLabelWidth = mediaState.trackNameLength * g_uiController->getTextWidth();
    int textOffset = 0;

    int overShoot = trackLabelWidth - TFT_W;

    textOffset = overShoot * (float)mediaState.labelScrollPosition / 100;
    

    gfx->setCursor(x - textOffset,y);
    gfx->setTextWrap(false);

    gfx->print(mediaState.trackName);

    int scrollDelay = 10;
    int endDelay = 2000;

    if (millis() > mediaState.labelNextUpdate)
    {
        switch (mediaState.labelState)
        {
        case TrackLabelStates::StayingAtStart:
            mediaState.labelState = nextTrackLabelState(mediaState.labelState);
            mediaState.labelNextUpdate = millis() + 200;
            break;
        case TrackLabelStates::GoingRight:
            if (mediaState.labelScrollPosition < 100)
                {
                    mediaState.labelScrollPosition += 0.1;
                    mediaState.labelNextUpdate = millis() + scrollDelay;
                }
            else
                {
                    mediaState.labelState = nextTrackLabelState(mediaState.labelState);
                    mediaState.labelNextUpdate = millis() + endDelay;
                }
            
            break;
        case TrackLabelStates::GoingLeft:
            if (mediaState.labelScrollPosition > 0)
                {
                    mediaState.labelScrollPosition -= 0.1;
                    mediaState.labelNextUpdate = millis() + scrollDelay;
                }
            else
                {
                    mediaState.labelState = nextTrackLabelState(mediaState.labelState);
                    mediaState.labelNextUpdate = millis() + endDelay;
                }
            
            break;
        case TrackLabelStates::StayingAtEnd:
            mediaState.labelState = nextTrackLabelState(mediaState.labelState);                    
            mediaState.labelNextUpdate = millis() + 200;
            break;            
        
        default:
            break;
        }
    }

}

TrackLabelStates MediaControllerWidget::nextTrackLabelState(TrackLabelStates current)
{
	switch (current)
	{
	case TrackLabelStates::StayingAtStart:
		return TrackLabelStates::GoingRight;
	case TrackLabelStates::GoingLeft:
		return TrackLabelStates::StayingAtStart;
	case TrackLabelStates::GoingRight:
		return TrackLabelStates::StayingAtEnd;
	case TrackLabelStates::StayingAtEnd:
		return TrackLabelStates::GoingLeft;
	}

	return TrackLabelStates::StayingAtStart;
}

int MediaControllerWidget::printTimeFormatted(int interval)
{
	// Проверка на хуету
	if (interval < 0 || interval > 5999)
		return 0;

	int fullMinutes = interval / 60;
	int seconds = interval % 60;

	char temp[8];

	if (fullMinutes > 99)
		fullMinutes = 99;


	sprintf(temp, "%0.2d:%0.2d", fullMinutes, seconds);

	gfx->print(temp);

	return strlen(temp);
}

void MediaControllerWidget::renderStuffHasTrackData()
{
	g_uiController->setTextSize(2);
	gfx->setCursor(0, 100);

	if (mediaState.playing)
		gfx->print("Now playing:");
	else
		gfx->print("Paused:");

	g_uiController->setTextSize(5);

	gfx->setTextColor(RGB565_AQUA);

	drawMusicTrackName(0, 120);

	g_uiController->setTextSize(3);

	int y = TFT_H - g_uiController->getTextHeight();


	gfx->setTextColor(RGB565_BLUEVIOLET);
	gfx->setCursor(0, y);

	int timeSymbolsCount = printTimeFormatted(mediaState.trackPosition);
	int timeLabelWidth = g_uiController->getTextWidth() * timeSymbolsCount;

	int barWidth = TFT_W - (timeLabelWidth * 2);
	gfx->drawRect(timeLabelWidth, y, barWidth, 24, RGB565_CHARTREUSE);
	gfx->fillRect(timeLabelWidth, y, barWidth * ((float)mediaState.trackPosition / mediaState.trackLength), 24, RGB565_CHARTREUSE);

	gfx->setCursor(TFT_W - timeLabelWidth, y);

	printTimeFormatted(mediaState.trackLength);


	if (mediaState.playing)
	{
		auto delta = millis() - mediaState.update_millis;

		if (delta > 1000)
		{
			mediaState.trackPosition += delta / 1000;
			mediaState.update_millis = millis();
		}
	}
}

void MediaControllerWidget::render() 
{
	if (*mediaState.trackName != 0)
		renderStuffHasTrackData();

	else
	{
		char* text = "No media loaded";

		gfx->setTextColor(RGB565_LIGHTCORAL);
		g_uiController->setTextSize(5);

		char* p = text;
		int n = 0;
		int l = strlen(text);

		int w = l * g_uiController->getTextWidth();
		int x = TFT_W / 2 - w / 2;

		while (*p)
		{
			HSV c;


			c.h = (float)n / (float)l * 360.f;
			c.s = 1;
			c.v = 1;

			c.h += millis() / 10.f;
			c.h = (int)c.h % 360;

			RGB r = hsvToRgb(c);

			int ir = r.r * 255;
			int ib = r.b * 255;
			int ig = r.g * 255;

			gfx->setTextColor(RGB565(ir, ig, ib));

			gfx->setCursor(x + n * 30, 120 + sin(millis() / 200.f + 6.28f * n / l) * 20);
			gfx->write(*p);

			n++;
			p++;
		}
	}

}

bool MediaControllerWidget::handlePhysicalButton(PhysicalButtons btnId) 
{
    return false;
}

void MediaControllerWidget::onSelected() 
{

}

void MediaControllerWidget::onUnselected() 
{
    
}

void MediaControllerWidget::updateTrackState(const char* trackName, int trackPos, int trackLength)
{
	strncpy(mediaState.trackName, trackName, sizeof(mediaState.trackName));

	mediaState.trackLength = trackLength;
	mediaState.trackPosition = trackPos;
	mediaState.update_millis = millis();
	mediaState.labelScrollPosition = 0;
	mediaState.labelState = TrackLabelStates::StayingAtStart;
	mediaState.labelNextUpdate = millis() + 2000;
	mediaState.trackNameLength = strlen(trackName);
}

void MediaControllerWidget::onNetworkPacketUpdate(responseData_t* packet)
{
	
}
