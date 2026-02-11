#pragma once

#include "SettingButton.h"
#include "FeatureDispatcher/Setting.h"


class ToggleButton : public SettingButton {
public:
	ToggleButton(Setting<bool>* setting) : SettingButton(setting) {}

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int virtualKeyCode) override;

};