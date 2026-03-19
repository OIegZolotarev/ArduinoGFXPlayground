#include "ui_controller.h"
#include "ui_wifi_network_selector.h"


void WifiNetworkSelector::setItem(SettingsItem* _item)
{

}

void WifiNetworkSelector::render(ApplicationPlatform* platformInstance)
{
	const char* title = "Connect to";

	auto gfx = platformInstance->gfxInstance();
	auto netIface = platformInstance->networkInterfaceInstance();

	int cy = appInstance->getClientAreaStart().y;

	appInstance->setTextSize(4);

	int x = TFT_W / 2 - (appInstance->getTextWidth() * strlen(title)) / 2;

	gfx->setCursor(x, cy + 20);

	gfx->setTextColor(RGB565_WHEAT);
	gfx->print(title);


	int y = cy + 70;

	appInstance->setTextSize(2);
	int h = appInstance->getTextHeight();

	int textStart = 10;
	int margin = 4;

	for(size_t t = 0; t < netIface->numDiscoveredNetworks(); t++ )
	{

		gfx->setCursor(10, y);
		gfx->print(netIface->getDiscoveredNetworkName(t));


		// Value
		char temp[32];
		sprintf(temp, "AAAA");

		int valWidth = appInstance->getTextWidth() * strlen(temp);
		gfx->setCursor(TFT_W - valWidth - margin - 4, y);
		gfx->print(temp);

		// if (p == selectedItem)
		{

			int xs = textStart - margin;
			int xe = TFT_W - xs * 2;

			int ys = y - margin;
			int ye = h + margin * 2;

			gfx->drawRect(xs, ys, xe, ye, RGB565_WHEAT);
		}

		y += h + 8;		
	}
}

bool WifiNetworkSelector::handlePhysicalButton(PhysicalButtons btnId)
{
	switch (btnId)
	{
	case PhysicalButtons::FUNC1:
		appInstance->popWidget();
		return true;

	case PhysicalButtons::FUNC4:

		return true;
	case PhysicalButtons::LEFT:

		return true;
	case PhysicalButtons::RIGHT:

		return true;
	default:
		break;

	}

	return false;
}

void WifiNetworkSelector::setupTopLevelButtons(struct functionalButton_s* buttons)
{
	UIWidget::setupTopLevelButtons(buttons);

	buttons[0].description = "Cancel";
	buttons[1].description = "";
	buttons[2].description = "";

	buttons[3].description = "Accept";
}
