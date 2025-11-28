#include "Arduino_Canvas.h"
#include "ui_controller.h"
#include "timer.h"
#include "net_wifi_serial_interface.h"

#include "com_colors.h"
#include "ui_keyboard.h"
#include "ui_media_player_widget.h"
#include "ui_settings_widget.h"

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





void UIController::render()
{
    gfx->fillScreen(RGB565_BLACK);

    if (network->justConnected())
    {
        network->resetConnectionAttempts();
        selectTopLevelWidget(TopLevelWidgets::Media);
    }

    if (!network->isConnected())
    {
        drawConnectingString();

        network->connect();
        return;
    }

    setTextSize(2);
    drawFunctionalButtons(1);

    if (topLevelWidget)
        topLevelWidget->render();

    return;

    static unsigned long nextUpdate = 0;

    if (millis() > nextUpdate)
    {
        network->queryMediaInfo();
        nextUpdate = millis() + 500;
    }

    if (network->hasData())
    {
        //uint16_t dataLength = network->readUInt16();
        //network->readStringNullTerminated(mediaState.trackName, sizeof(mediaState.trackName));        
        //
        //char artist[64];
        //network->readStringNullTerminated(artist, sizeof(artist));        

        //mediaState.trackLength = network->readUInt32();
        //int newPosition = network->readUInt32();

        //// Костыль когда хост не обновляет позицию трека - обновляем только тогда когда она явно обновилась
        //// В остальных случая позицию обновляет наш таймер.
        //// TODO: состояние добавить передачу состояния паузы и описать протокол в network_interface.h
        //if (mediaState.trackPositionFromHost != newPosition)
        //{
        //    mediaState.trackPosition = newPosition;
        //    mediaState.trackPositionFromHost = newPosition;
        //}

        

    }

}

void UIController::stateSettings()
{
    
}

void UIController::drawConnectingString()
{
	char* text = "Connecting...";

	gfx->setTextColor(RGB565_LIGHTCORAL);
	setTextSize(5);

	char* p = text;
	int n = 0;
	int l = strlen(text);

	int w = l * 6 * textSize;
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

		gfx->setCursor(x + n * 30, 100 + sin(millis() / 200.f + 6.28f * n / l) * 20);
		gfx->write(*p);

		n++;
		p++;
	}

    char temp[128];
    sprintf(temp, "Attempt #%d", network->connectAttemps() + 1);
    gfx->setCursor(0, TFT_H - getTextHeight());
    gfx->print(temp);

}

void UIController::drawFunctionalButtons(int y)
{
	int x = 0;

    auto items = buttons;

	for (int i = 0; i < 4; i++)
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

int UIController::drawBigButton(const char* text, int x, int y, int w, uint16_t rectColor, uint16_t textColor, bool flash)
{
	if (!text)
		return 0;

	int margin = 4;

	vec2i bounds = getTextBounds(text);

	if (w < bounds.x)
		w = bounds.x + margin * 2;

    if (*text == 0)
        return w;

	// Выравнивание кнопки посередине

	uint16_t finalRectColor = rectColor;
	uint16_t finalTextColor = textColor;

	if (flash)
	{
		finalRectColor = textColor;
		finalTextColor = rectColor;
	}

	if (flash)
		gfx->fillRect(x, y, w, bounds.y + margin * 2, finalRectColor);
	else
		gfx->drawRect(x, y, w, bounds.y + margin * 2, finalRectColor);

    // TODO: сделать это получше
    clientAreaStart.x = 0;
    clientAreaStart.y = bounds.y + margin * 2;

	gfx->setCursor(x + (w / 2 - bounds.x / 2), y + margin);
	gfx->setTextColor(finalTextColor);
	gfx->print(text);

	return w;
}

UIController::UIController()
{
    static char buffer[32];
    size_t bufLen = 32;

    kb = new OnScreenKeyboard(gfx, buffer, bufLen);
    network = new WifiSerialInterface("127.0.0.1", 35000);

    mediaPlayerWidget = new MediaControllerWidget();
    settingsWidget = new SettingsWidget("System settings");

}

void UIController::selectTopLevelWidget(TopLevelWidgets id)
{
    for(int i = 0 ; i < 4; i++)
    {
        buttons[i].flags &= ~FB_HIGHLIGHT;
    }

    switch (id)
    {
    case TopLevelWidgets::Gauges:
        buttons[0].flags |= FB_HIGHLIGHT;
        topLevelWidget = gaugesWidget;
        break;
    case TopLevelWidgets::Tune:
        buttons[1].flags |= FB_HIGHLIGHT;
        topLevelWidget = tuneWidget;
        break;
    case TopLevelWidgets::Media:
        buttons[2].flags |= FB_HIGHLIGHT;
        topLevelWidget = mediaPlayerWidget;
        break;
    case TopLevelWidgets::Settings:
        buttons[3].flags |= FB_HIGHLIGHT;
        topLevelWidget = settingsWidget;
        break;
    default:
        break;

    }


    if (topLevelWidget)
        topLevelWidget->setupTopLevelButtons(buttons);
}


void UIController::handlePhysicalButton(PhysicalButtons btnId)
{
    if (topLevelWidget && topLevelWidget->handlePhysicalButton(btnId))
    {
        return;
    }

    switch(btnId)    
    {
    case PhysicalButtons::FUNC1:
        selectTopLevelWidget(TopLevelWidgets::Gauges);
        break;
    case PhysicalButtons::FUNC2:
        selectTopLevelWidget(TopLevelWidgets::Tune);
        break;
    case PhysicalButtons::FUNC3:
        selectTopLevelWidget(TopLevelWidgets::Media);
        break;        
    case PhysicalButtons::FUNC4:
        selectTopLevelWidget(TopLevelWidgets::Settings);
        break;
    }
}


void UIController::setTextSize(int size)
{
    textSize = size;
    gfx->setTextSize(size);
}

const int UIController::getTextSize() const
{
    return textSize;
}

const int UIController::getTextWidth() const
{
    return textSize * 6;
}

const int UIController::getTextHeight() const
{
    return textSize * 8;
}

void UIController::pushWidget(UIWidget* pWidget)
{
    pWidget->setParent(topLevelWidget);
    topLevelWidget = pWidget;
    pWidget->setupTopLevelButtons(buttons);
}

void UIController::popWidget()
{
    if (!topLevelWidget)
        return;

    auto parent = topLevelWidget->getParent();

    if (parent)
    {
        topLevelWidget = parent;
        parent->setupTopLevelButtons(buttons);
    }
}

void UIController::setWidget(UIWidget* pWidget)
{
    topLevelWidget = pWidget;
}

