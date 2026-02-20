/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "SettingButton.h"
#include "FeatureDispatcher/Setting.h"
#include "Util/InputSystem.h"

class KeybindButton : public SettingButton {
public:
	KeybindButton(Setting<SDL_Scancode>* setting) : SettingButton(setting) {}

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);
private:

	bool choosingBind = false;
};