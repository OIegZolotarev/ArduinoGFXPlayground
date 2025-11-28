#include "ui_settings_widget.h"
#include "ui_controller.h"



SettingsWidget::SettingsWidget(const char* t)
{
	auto s = addSetting(new SettingsItem("Theme"));

	NumericSetting* brightness = (NumericSetting*)addSetting(new NumericSetting("Screen brightness"));
	brightness->setLimits(0, 100);
	brightness->setPrecision(0);
	brightness->setStepping(1);

	addSetting(new SettingsItem("Wi-fi password"));
	addSetting(new SettingsItem("Wi-fi network"));

	title = t;
	selectedItem = items;

	numericalEditor = new NumericEditorWidget;
}

SettingsWidget::~SettingsWidget()
{

}

void SettingsWidget::render()
{
	SettingsItem* p = items;

	int cy = g_uiController->getClientAreaStart().y;

	g_uiController->setTextSize(4);

	int x = TFT_W / 2 - (g_uiController->getTextWidth() * strlen(title)) / 2;

	gfx->setCursor(x, cy + 20);

	gfx->setTextColor(RGB565_WHEAT);
	gfx->print(title);


	int y = cy + 70;

	g_uiController->setTextSize(2);
	int h = g_uiController->getTextHeight();

	int textStart = 10;
	int margin = 4;

	while (p)
	{

		gfx->setCursor(10, y);
		gfx->print(p->getDescription());
	

		// Value
		char temp[32];
		p->valueDescription(temp, 32);

		int valWidth = g_uiController->getTextWidth() * strlen(temp);
		gfx->setCursor(TFT_W - valWidth - margin - 4, y);
		gfx->print(temp);

		if (p == selectedItem)
		{

			int xs = textStart - margin;
			int xe = TFT_W - xs * 2;

			int ys = y - margin;
			int ye = h + margin * 2;

			gfx->drawRect(xs, ys, xe, ye, RGB565_WHEAT);
		}

		y += h + 8;
		p = p->nextInList();
	}
}

bool SettingsWidget::handlePhysicalButton(PhysicalButtons btnId)
{
	switch (btnId)
	{
		case PhysicalButtons::DOWN:

		{
			auto a = selectedItem->nextInList();
			if (a)
				selectedItem = a;

			return true;
		}
		break;
		case PhysicalButtons::UP:
		{
			auto a = selectedItem->prevInList();
			if (a)
				selectedItem = a;

			return true;
		}

		case PhysicalButtons::RIGHT:
		{
			editSelectedItem(selectedItem);
		}

		break;

		default:
			break;
	}

	return false;
}

void SettingsWidget::editSelectedItem(SettingsItem* selectedItem)
{
	
	switch (selectedItem->kind())
	{

	case SettingsItemKinds::TextSimple:
		break;
	case SettingsItemKinds::TextPassword:
		break;
	case SettingsItemKinds::NumericLimited:
		numericalEditor->setItem(selectedItem);
		g_uiController->pushWidget(numericalEditor);
		break;
	case SettingsItemKinds::ChoiceWifiNetwork:
		break;
	case SettingsItemKinds::Unset:
		break;
	default:
		break;
	}
	
}

SettingsItem::SettingsItem(const char* descr)
{
	description = descr;
}

const char* SettingsItem::getDescription()
{
	return description;
}

void SettingsItem::link(SettingsItem* item)
{
	next = item;
	item->prev = this;
}

SettingsItem* SettingsItem::nextInList()
{
	return next;
}

SettingsItem* SettingsItem::prevInList()
{
	return prev;
}

NumericSetting::NumericSetting(const char* descr) : SettingsItem(descr)
{

}

NumericSetting::~NumericSetting()
{

}

SettingsItemKinds NumericSetting::kind()
{
	return SettingsItemKinds::NumericLimited;
}

void NumericSetting::setLimits(float _limitMin, float _limitMax)
{
	limitMin = _limitMin;
	limitMax = _limitMax;
}

void NumericSetting::setPrecision(int decimalPlaces)
{
	precision = decimalPlaces;
}

void NumericSetting::setStepping(int value)
{
	stepping = value;
}

float NumericSetting::getValue()
{
	return value;
}

float NumericSetting::getLimitMin()
{
	return limitMin;
}

float NumericSetting::getLimitMax()
{
	return limitMax;
}

int NumericSetting::getStepping()
{
	return stepping;
}

float NumericSetting::constrain(float value)
{
	if (value < limitMin)
		return limitMin;

	if (value > limitMax)
		return limitMax;

	return value;
}
