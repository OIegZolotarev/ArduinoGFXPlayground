#pragma once

#include "ui_widget.h"
#include <ui_numeric_editor.h>

enum class SettingsItemKinds
{
	TextSimple,
	TextPassword,
	NumericLimited,
	ChoiceWifiNetwork,
	Unset
};

class SettingsItem
{
	const char* description;	

	SettingsItem* next = nullptr;
	SettingsItem* prev = nullptr;

public:
	SettingsItem(const char* descr);
	const char* getDescription();
	void link(SettingsItem* item);

	SettingsItem* nextInList();
	SettingsItem* prevInList();
	
	virtual SettingsItemKinds kind()
	{
		return SettingsItemKinds::Unset;
	}

	virtual void valueDescription(char* destBuffer, size_t bufferLength) { *destBuffer = 0; }

};

class NumericSetting : public SettingsItem
{
	float value = 0;
	float limitMin = 1;
	float limitMax = 2;

	int precision = 0;
	int stepping = 1;

public:

	NumericSetting(const char* descr);
	~NumericSetting();


	SettingsItemKinds kind() override;

	void setLimits(float _limitMin, float _limitMax);
	void setPrecision(int decimalPlaces);
	void setStepping(int value);

	float getValue();

	void setValue(float v)
	{
		value = v;
	}
	
	float getLimitMin();
	float getLimitMax();
	
	int getStepping();

	float constrain(float value);

	void valueDescription(char* destBuffer, size_t bufferLength) override
	{
		sprintf(destBuffer, "%f", value);
	}
};

class SettingsWidget: public UIWidget
{
	const char* title;

	SettingsItem* items = nullptr;
	SettingsItem* selectedItem = nullptr;

	SettingsItem* addSetting(SettingsItem* item)
	{
		if (!items)
		{
			items = item;
			selectedItem = item;
			return item;
		}

		item->link(items);
		items = item;

		return item;
	}

	NumericEditorWidget* numericalEditor;

	void editSelectedItem(SettingsItem* selectedItem);

public:
	SettingsWidget(const char* _title);
	~SettingsWidget();
	
	void render() override;
	bool handlePhysicalButton(PhysicalButtons btnId) override;



};