#include <SDL2/SDL.h>
#include "Arduino_Canvas.h"
#include "ui_controller.h"
#include "timer.h"

extern Arduino_GFX * gfx;

UIState* g_uiState;

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

void drawMusicTrackName(const char* track, int x, int y)
{
    vec2i bounds = getTextBounds(track);

    
    int stringLen = strlen(track);

    bounds.x = stringLen * 30;
    int charsLeft = stringLen;
    int charSize = bounds.x / charsLeft;

    gfx->setCursor(x,y);

    if (x + bounds.x > TFT_W)
        charsLeft = (TFT_W - x) / charSize;

    int offset = (int)(millis() / 500) % ((stringLen - charsLeft) + 1);


    char* p = (char*)track + offset;
    
    

    while(charsLeft > 0)
    {
        gfx->write(*p);
        charsLeft--;
        p++;
    }
    
}

void UIState::render()
{
    gfx->fillScreen(RGB565_BLACK);
    gfx->setTextSize(2);

    drawFunctionalButtons(g_uiState->getFunctionalButtons(), 1);

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

void UIState::stateGauges()
{
    
}

void UIState::stateMedia()
{    
    gfx->setTextSize(2);
    gfx->setCursor(0,100);
    gfx->print("Now playing:");
    gfx->setTextSize(5);

    gfx->setTextColor(RGB565_AQUA);

    drawMusicTrackName(mediaState.trackName,0,120);

    int y = TFT_H - 24;


    gfx->setTextColor(RGB565_BLUEVIOLET);

    gfx->setTextSize(3);
    gfx->setCursor(0,y);
    gfx->print("0:00");

    gfx->fillRect(18*4, y, TFT_W - 18*8, 24, RGB565_CHARTREUSE);

    gfx->setCursor(TFT_W - 18*4,y);
    gfx->print("4:53");

}

void drawGauges()
{

}

UIState::UIState()
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

    updateTrackState("music06.mp3", 100, 300);
}

void UIState::selectFunction(int funcId)
{
    for(int i = 0 ; i < 4; i++)
    {
        buttons[i].flags &= ~FB_HIGHLIGHT;
    }

    buttons[funcId].flags |= FB_HIGHLIGHT;

    state = (UIFunctionsState)funcId;
}


void UIState::handlePhysicalButton(PhysicalButtons btnId)
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

void UIState::updateTrackState(const char* trackName, int pos, int length)
{
    strncpy(mediaState.trackName, trackName, sizeof(mediaState.trackName));

    mediaState.trackLength = length;
    mediaState.trackPosition = pos;
}