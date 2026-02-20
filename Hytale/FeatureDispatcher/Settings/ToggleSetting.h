/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>

#include "FeatureDispatcher/Setting.h"
#include "Menu/Children/ToggleButton.h"

class ToggleSetting : public Setting<bool> {
public:
	ToggleSetting(std::string name, bool defaultValue) : Setting(name, defaultValue) {}

	void Toggle() {
		this->SetValue(!GetValue());
	}

	std::unique_ptr<Component> CreateButton() override {
		auto btn = std::make_unique<ToggleButton>(this);
		btn->setting = this;
		return btn;
	}

};