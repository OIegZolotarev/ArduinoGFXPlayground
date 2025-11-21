#include <SDL2/SDL.h>
#include "Arduino_Canvas.h"
#include "ui_controller.h"
#include "timer.h"

extern Arduino_GFX * gfx;

int dx = 1;

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

    bool flash = (millis()  % 1000) > 500;

    for(int i = 0 ; i < 4; i++)
    {
        int w = drawBigButton(items[i].description, 
        x,
        y,
        TFT_W / 4,
        items[i].outline_color,
        items[i].text_color, flash);

        x += w;
    }
}

void drawMusicTrackName(const char* track, int x, int y)
{
    vec2i bounds = getTextBounds(track);

    bounds.x = 32 * 30;
    int stringLen = strlen(track);
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

void DrawUI()
{

    functionalButton_t items[4];

    items[0].description = "Gauges";
    items[0].outline_color = RGB565_RED;
    items[0].text_color = RGB565_YELLOW;

    items[1].description = "Tune";
    items[1].outline_color = RGB565_GREEN;
    items[1].text_color = RGB565_YELLOW;


    items[2].description = "Media";
    items[2].outline_color = RGB565_BLUE;
    items[2].text_color = RGB565_YELLOW;

    items[3].description = "Settings";
    items[3].outline_color = RGB565_MAGENTA;
    items[3].text_color = RGB565_YELLOW;


    gfx->fillScreen(RGB565_BLACK);
    gfx->setTextSize(2);

    drawFunctionalButtons(items, 1);

    gfx->setTextSize(2);
    gfx->setCursor(0,100);
    gfx->print("Now playing:");

    gfx->setTextSize(5);    
    drawMusicTrackName("BREAK IN2 THE NITE (Ileus Remix)",0,120);
    // gfx->print("M.O.V.E");

}
