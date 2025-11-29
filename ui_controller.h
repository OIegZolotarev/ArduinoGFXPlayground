#pragma once 

#define TFT_W 480
#define TFT_H 272

#include "net_interface.h"

typedef struct
{
    int x,y;
} vec2i;

typedef struct functionalButton_s
{
    const char* description = nullptr;
    void (*handler)() = nullptr;

    float timer = 0;

    uint16_t text_color = 0;
    uint16_t outline_color = 0;

    uint8_t flags = 0;

}functionalButton_t;

#define FB_HIGHLIGHT (1<<0)

enum class TopLevelWidgets
{
    Gauges,
    Tune,
    Media,
    Settings    
};


#include "ui_widget.h"


class UIController
{
    TopLevelWidgets state = TopLevelWidgets::Gauges;
    functionalButton_t buttons[4];
    vec2i clientAreaStart;
    
    int textSize = 0;

    void selectTopLevelWidget(TopLevelWidgets widgetId);     
    void stateSettings();

    // Gauges
    struct 
    {
        uint8_t brakeLevel = 0;
        uint16_t rpm = 0;
    }gaugesState;
    
    
    int printTimeFormatted(int timeSeconds);
    void drawMusicTrackName(int x,int y);

    
    NetworkInterface * network = nullptr;

    UIWidget* gaugesWidget = nullptr;
    UIWidget* tuneWidget = nullptr;
    UIWidget* mediaPlayerWidget = nullptr;
    UIWidget* settingsWidget = nullptr;

    UIWidget* topLevelWidget = nullptr;

	void drawConnectingString();
	void drawFunctionalButtons(int y);
	int drawBigButton(const char* text, int x, int y, int w, uint16_t rectColor, uint16_t textColor, bool flash);



public:
    UIController();

    functionalButton_t* getFunctionalButtons() { return buttons; }
    TopLevelWidgets currentState() { return state; }

    void handlePhysicalButton(PhysicalButtons btnId);
    void render();
        
    void setBrakeLevel(int brakeLevel) {};
    void setRPM(int rpm) {};

    void setTextSize(int size);

    const int getTextSize() const;
    const int getTextWidth() const;
    const int getTextHeight() const;

    const vec2i getClientAreaStart() const
    {
        return clientAreaStart;
    }

    void pushWidget(UIWidget* pWidget);
    void popWidget();
    void setWidget(UIWidget* pWidget);

    void onPacketReceived(responseData_t* responseData);
    void requestMediaState();
};

extern UIController* g_uiController;

