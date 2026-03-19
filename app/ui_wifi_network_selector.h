#pragma once

#include "ui_widget.h"


class SettingsItem;

class WifiNetworkSelector : public UIWidget
{
	SettingsItem* item = nullptr;
	float intermediateValue = 0;

public:
	WifiNetworkSelector() = default;
	~WifiNetworkSelector() = default;

	void setItem(SettingsItem* _item);
	void render(ApplicationPlatform* platformInstance) override;


	bool handlePhysicalButton(PhysicalButtons btnId) override;
	void setupTopLevelButtons(struct functionalButton_s* buttons) override;

};