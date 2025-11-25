#include "Arduino_Canvas.h"
#include "ui_controller.h"
#include "timer.h"
#include "net_wifi_serial_interface.h"

#include "com_colors.h"
#include "ui_keyboard.h"

extern Arduino_GFX * gfx;

UIController* g_uiController;

OnScreenKeyboard * kb = nullptr;

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
        kb->draw();
        break;
    case UIFunctionsState::Settings:
        break;
    }

    static unsigned long nextUpdate = 0;

    if (millis() > nextUpdate)
    {
        network->queryMediaInfo();
        nextUpdate = millis() + 500;
    }

    if (network->hasData())
    {
        uint16_t dataLength = network->readUInt16();
        network->readStringNullTerminated(mediaState.trackName, sizeof(mediaState.trackName));        
        
        char artist[64];
        network->readStringNullTerminated(artist, sizeof(artist));        

        mediaState.trackLength = network->readUInt32();
        int newPosition = network->readUInt32();

        // Костыль когда хост не обновляет позицию трека - обновляем только тогда когда она явно обновилась
        // В остальных случая позицию обновляет наш таймер.
        // TODO: состояние добавить передачу состояния паузы и описать протокол в network_interface.h
        if (mediaState.trackPositionFromHost != newPosition)
        {
            mediaState.trackPosition = newPosition;
            mediaState.trackPositionFromHost = newPosition;
        }

        

    }

}



void UIController::stateGauges()
{
    
    char *text = "Connecting...";

    gfx->setTextColor(RGB565_LIGHTCORAL);
    setTextSize(5);

    char* p = text;
    int n = 0;
    int l = strlen(text);

    int w = l * 6 * textSize;
    int x = TFT_W / 2 - w/2;

    while(*p)
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

        gfx->setCursor(x + n * 30, 100 + sin(millis() / 200.f + 6.28f * n / l) * 20);
        gfx->write(*p);

        n++;
        p++;
    }
    

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

    static char buffer[32];
    size_t bufLen = 32;

    kb = new OnScreenKeyboard(gfx, buffer, bufLen);
    network = new WifiSerialInterface("127.0.0.1", 35000);
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
    selectFunction(1);
    kb->handlePhysicalButton(btnId);
    return;

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