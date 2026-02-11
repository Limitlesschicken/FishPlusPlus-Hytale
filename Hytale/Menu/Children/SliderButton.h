#pragma once

#include "SettingButton.h"
#include "FeatureDispatcher/Setting.h"

class SliderButton : public SettingButton {
public:
	SliderButton(Setting<float>* setting) : SettingButton(setting) {}

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);
	void MouseReleased(float mouseX, float mouseY, int vk);
	void MouseDragged(float mouseX, float mouseY, int vk, float deltaX, float deltaY) override;
private:

	bool m_dragging = false;
	float m_UIProgress = 0;

	void CalculateSlider(float mouseX);
};