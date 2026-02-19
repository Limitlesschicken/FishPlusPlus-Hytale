/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "SettingButton.h"
#include "FeatureDispatcher/Setting.h"


class MultiButton : public SettingButton {
public:
	MultiButton(Setting<int>* setting) : SettingButton(setting) {}

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);
};