
#pragma once 

#include "Arduino_Canvas.h"
#include "net_interface.h"

extern class UIController * g_uiController;
extern Arduino_GFX* gfx;

enum class PhysicalButtons
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FUNC1,
    FUNC2,
    FUNC3,
    FUNC4
};

class UIWidget
{
    UIWidget* previousWidgetInStack = nullptr;
public:
    UIWidget() = default;
    virtual ~UIWidget() = default;

    virtual void render() = 0;
    virtual bool handlePhysicalButton(PhysicalButtons btnId) = 0;

    virtual void onSelected() {};
    virtual void onUnselected() {};

    virtual void onNetworkPacketUpdate(responseData_t* packet) {};
    
    void setParent(UIWidget* topLevelWidget);

	UIWidget* getParent() const;

    virtual void setupTopLevelButtons(struct functionalButton_s* buttons);
};