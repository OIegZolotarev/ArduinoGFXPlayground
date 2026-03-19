#include "ui_controller.h"
#include "ui_numeric_editor.h"
#include "ui_settings_widget.h"

void NumericEditorWidget::setItem(SettingsItem* _item)
{
	item = (NumericSetting*)_item;
	intermediateValue = item->getValue();
}

void NumericEditorWidget::render(ApplicationPlatform* platformInstance)
{
	auto gfx = platformInstance->gfxInstance();

	int cy = appInstance->getClientAreaStart().y;
	
	// Заголовок

	cy += 60;

	appInstance->setTextSize(4);

	int x = TFT_W / 2 - (appInstance->getTextWidth() * strlen(item->getDescription())) / 2;

	gfx->setCursor(x, cy + 20);

	gfx->setTextColor(RGB565_WHEAT);
	gfx->print(item->getDescription());

	// Полоска

	int margin = 4;

	int xs = margin;
	int xe = TFT_W - margin * 2;

	cy += 20;
	int barY = cy + appInstance->getTextHeight() + margin;

	gfx->drawRect(xs, barY, xe - xs, 40, RGB565_AQUA);
	
	float frac = intermediateValue / item->getLimitMax();
	gfx->fillRect(xs, barY, (xe - xs) * frac, 40, RGB565_AQUA);


	cy += 100;
	appInstance->setTextSize(2);
	gfx->setCursor(0, cy);

	char temp[32];
	sprintf(temp, "%f", intermediateValue);
	gfx->print(temp);


}

bool NumericEditorWidget::handlePhysicalButton(PhysicalButtons btnId)
{
	switch (btnId)
	{
	case PhysicalButtons::FUNC1:
		appInstance->popWidget();
		return true;
	
	case PhysicalButtons::FUNC4:
		item->setValue(intermediateValue);
		appInstance->popWidget();
		return true;
	case PhysicalButtons::LEFT:
		intermediateValue -= item->getStepping();
		intermediateValue = item->calculateConstrain(intermediateValue);
		return true;
	case PhysicalButtons::RIGHT:
		intermediateValue += item->getStepping();
		intermediateValue = item->calculateConstrain(intermediateValue);
		return true;
	default:
		break;

	}

	return false;
}

void NumericEditorWidget::setupTopLevelButtons(struct functionalButton_s* buttons)
{
	UIWidget::setupTopLevelButtons(buttons);

	buttons[0].description = "Cancel";
	buttons[1].description = "";
	buttons[2].description = "";
	
	buttons[3].description = "Accept";	
}
