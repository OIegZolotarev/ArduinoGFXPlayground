#include "ui_widget.h"
#include "ui_controller.h"



void UIWidget::setParent(UIWidget* topLevelWidget)
{
	previousWidgetInStack = topLevelWidget;
}

UIWidget* UIWidget::getParent() const
{
	return previousWidgetInStack;
}

void UIWidget::setupTopLevelButtons(functionalButton_s* buttons)
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
}
