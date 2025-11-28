#pragma once

#include "ui_widget.h"

class NumericSetting;
class SettingsItem;

class NumericEditorWidget : public UIWidget
{
	NumericSetting* item = nullptr;
	float intermediateValue = 0;

public:
	NumericEditorWidget() = default;
	~NumericEditorWidget() = default;

	void setItem(SettingsItem* _item);
	void render() override;


	bool handlePhysicalButton(PhysicalButtons btnId) override;
	void setupTopLevelButtons(struct functionalButton_s* buttons) override;

};