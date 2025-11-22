#include "Arduino_Canvas.h"
#include "ui_controller.h"
#include "timer.h"

extern Arduino_GFX * gfx;

UIController* g_uiController;

vec2i getTextBounds(const char* text)
{
    int16_t x1,y1;
    uint16_t w,h;
    gfx->getTextBounds(text, 0,0, &x1,&y1,&w,&h);

    return vec2i{w,h};
}

int drawBigButton(const char* text, int x, int y, int w, uint16_t rectColor, uint16_t textColor, bool flash)
{
    int margin = 4;
    

    vec2i bounds = getTextBounds(text);

    if (w < bounds.x)
        w = bounds.x + margin * 2;

    // Выравнивание кнопки посередине
    
    uint16_t finalRectColor = rectColor;
    uint16_t finalTextColor = textColor;

    if (flash)
    {
        finalRectColor = textColor;
        finalTextColor = rectColor;
    }


    if (flash)    
        gfx->fillRect(x,y, w , bounds.y + margin * 2, finalRectColor);    
    else
        gfx->drawRect(x,y, w , bounds.y + margin * 2, finalRectColor);    
    
    
    gfx->setCursor(x + (w/2 - bounds.x / 2), y + margin );
    gfx->setTextColor(finalTextColor);
    gfx->print(text);
    
    return w;
}

void drawFunctionalButtons(functionalButton_t * items, int y)
{
    int x = 0;

    for(int i = 0 ; i < 4; i++)
    {
        int w = drawBigButton(items[i].description, 
        x,
        y,
        TFT_W / 4,
        items[i].outline_color,
        items[i].text_color, items[i].flags & FB_HIGHLIGHT);

        x += w;
    }
}

void UIController::drawMusicTrackName(int x, int y)
{
    char* p = (char*)mediaState.trackName;
       
    int trackLabelWidth = mediaState.trackNameLength * textSize * 6;
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

    // vec2i bounds = getTextBounds(track);

    
    // int stringLen = strlen(track);

    // bounds.x = stringLen * 30;
    // int charsLeft = stringLen;
    // int charSize = bounds.x / charsLeft;

    // 

    // if (x + bounds.x > TFT_W)
    //     charsLeft = (TFT_W - x) / charSize;

    // int offset = (int)(millis() / 500) % ((stringLen - charsLeft) + 1);


    // char* p = (char*)track + offset;
    
    

    // while(charsLeft > 0)
    // {
    //     gfx->write(*p);
    //     charsLeft--;
    //     p++;
    // }
    
}

void UIController::render()
{
    gfx->fillScreen(RGB565_BLACK);
    setTextSize(2);

    drawFunctionalButtons(buttons, 1);

    switch(state)    
    {
    case UIFunctionsState::Gauges:
        stateGauges();
        break;
    case UIFunctionsState::Media:
        stateMedia();
        break;
    case UIFunctionsState::Tune:
        break;
    case UIFunctionsState::Settings:
        break;
    }

}

void UIController::stateGauges()
{
    
}

void UIController::stateMedia()
{    
    setTextSize(2);
    gfx->setCursor(0,100);
    gfx->print("Now playing:");
    setTextSize(5);

    gfx->setTextColor(RGB565_AQUA);

    drawMusicTrackName(0,120);

    setTextSize(3);

    int y = TFT_H - (8 * textSize);


    gfx->setTextColor(RGB565_BLUEVIOLET);
    gfx->setCursor(0,y);
    
    int timeSymbolsCount = printTimeFormatted(mediaState.trackPosition);
    int timeLabelWidth = 6 * textSize * timeSymbolsCount;

    int barWidth =  TFT_W - (timeLabelWidth * 2);
    gfx->drawRect(timeLabelWidth, y, barWidth, 24, RGB565_CHARTREUSE);
    gfx->fillRect(timeLabelWidth, y, barWidth * ((float)mediaState.trackPosition / mediaState.trackLength), 24, RGB565_CHARTREUSE);

    gfx->setCursor(TFT_W - timeLabelWidth,y);
    
    printTimeFormatted(mediaState.trackLength);

    auto delta = millis() - mediaState.update_millis;
    
    if (delta > 1000)
    {
        mediaState.trackPosition += delta / 1000;
        mediaState.update_millis = millis();
    }

}

UIController::UIController()
{
    buttons[0].description = "Gauges";
    buttons[0].outline_color = RGB565_RED;
    buttons[0].text_color = RGB565_YELLOW;

    buttons[1].description = "Tune";
    buttons[1].outline_color = RGB565_GREEN;
    buttons[1].text_color = RGB565_YELLOW;

    buttons[2].description = "Media";
    buttons[2].outline_color = RGB565_BLUE;
    buttons[2].text_color = RGB565_YELLOW;

    buttons[3].description = "Settings";
    buttons[3].outline_color = RGB565_MAGENTA;
    buttons[3].text_color = RGB565_YELLOW;    
    
    selectFunction(2);

    updateTrackState("Dave Rodgers feat. Eurobeat Union - Gold Night", 100, 300);
}

void UIController::selectFunction(int funcId)
{
    for(int i = 0 ; i < 4; i++)
    {
        buttons[i].flags &= ~FB_HIGHLIGHT;
    }

    buttons[funcId].flags |= FB_HIGHLIGHT;

    state = (UIFunctionsState)funcId;
}


void UIController::handlePhysicalButton(PhysicalButtons btnId)
{
    switch(btnId)    
    {
    case PhysicalButtons::FUNC1:
        selectFunction(0);
        break;
    case PhysicalButtons::FUNC2:
        selectFunction(1);
        break;
    case PhysicalButtons::FUNC3:
        selectFunction(2);
        break;        
    case PhysicalButtons::FUNC4:
        selectFunction(3);
        break;
    }
}

void UIController::updateTrackState(const char* trackName, int pos, int length)
{
    strncpy(mediaState.trackName, trackName, sizeof(mediaState.trackName));

    mediaState.trackLength = length;
    mediaState.trackPosition = pos;
    mediaState.update_millis = millis();    
    mediaState.labelScrollPosition = 0;
    mediaState.labelState = TrackLabelStates::StayingAtStart;
    mediaState.labelNextUpdate = millis() + 2000;
    mediaState.trackNameLength = strlen(trackName);
}

void UIController::setTextSize(int size)
{
    textSize = size;
    gfx->setTextSize(size);
}

int UIController::printTimeFormatted(int interval)
{
    int fullMinutes = interval / 60;
    int seconds = interval % 60;

    char temp[8];
    
    if (fullMinutes > 99)
        fullMinutes = 99;

    sprintf(temp, "%0.2d:%0.2d", fullMinutes, seconds);

    gfx->print(temp);

    return strlen(temp);
}

TrackLabelStates UIController::nextTrackLabelState(TrackLabelStates current)
{
    switch(current)
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