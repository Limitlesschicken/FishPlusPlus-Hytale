/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "FeatureDispatcher/Setting.h"
#include "Menu/Children/ColorButton.h"
#include "Math/Color.h"

class ColorSetting : public Setting<Color> {
public:
	ColorSetting(std::string name, Color defaultColor) : Setting(name, defaultColor) {}

	std::unique_ptr<Component> CreateButton() override {
		auto btn = std::make_unique<ColorButton>(this);
		btn->setting = this;
		return btn;
	}
};