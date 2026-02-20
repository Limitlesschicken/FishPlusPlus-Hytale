/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>

#include "FeatureDispatcher/Setting.h"
#include "Menu/Children/SliderButton.h"

class SliderSetting : public Setting<float> {
public:
	SliderSetting(std::string name, float defaultValue, float minValue, float maxValue) : Setting(name, defaultValue) {
		this->minValue = minValue;
		this->maxValue = maxValue;
	}

	std::unique_ptr<Component> CreateButton() override {
		auto btn = std::make_unique<SliderButton>(this);
		btn->setting = this;
		return btn;
	}

	float minValue;
	float maxValue;
};