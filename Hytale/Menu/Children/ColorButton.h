#pragma once

#include "SettingButton.h"
#include "FeatureDispatcher/Setting.h"
#include "Math/Color.h"
#include "Math/Vector2.h"
#include <Menu/Component.h>

class ColorSettingControl;

class ColorButton : public SettingButton {
public:
	ColorButton(Setting<Color>* setting);

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);

private:
	const float previewSize = 14.0f;

	ColorSettingControl* body;

	friend class ColorSettingControl;
};


class ColorSettingControl : public Component {
public:
	ColorSettingControl(ColorButton* parent);
	
	void Update(float mouseX, float mouseY) override;
	void Render(double deltaTime) override;
	bool open = false;
private:
	void RenderColorPicker();
	void RenderHueSlider();
	void RenderAlphaSlider();

	void CalculateSliders(float mouseX, float mouseY);
	void MouseClicked(float mouseX, float mouseY, int vk);
	void MouseReleased(float mouseX, float mouseY, int vk);
	void MouseDragged(float mouseX, float mouseY, int vk, float deltaX, float deltaY);

	bool IsMouseHovered(float mouseX, float mouseY, Vector2 min, Vector2 size) {
		return mouseX >= min.x &&
			mouseX < min.x + size.x &&
			mouseY >= min.y &&
			mouseY < min.y + size.y;
	}

	Color HSVtoRGB(float h, float s, float v, float alpha = 255.0f) {
		float c = v * s;
		float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
		float m = v - c;

		float r1, g1, b1;

		if (h < 60) { r1 = c; g1 = x; b1 = 0; }
		else if (h < 120) { r1 = x; g1 = c; b1 = 0; }
		else if (h < 180) { r1 = 0; g1 = c; b1 = x; }
		else if (h < 240) { r1 = 0; g1 = x; b1 = c; }
		else if (h < 300) { r1 = x; g1 = 0; b1 = c; }
		else { r1 = c; g1 = 0; b1 = x; }

		return Color(
			(r1 + m) * 255,
			(g1 + m) * 255,
			(b1 + m) * 255,
			alpha
		);
	}

	float hue = 0.0f;
	float saturation = 0.0f;
	float value = 0.0f;
	float alpha = 255.0f;

	Vector2 colorPickerPos;
	Vector2 colorPickerSize;
	Vector2 colorPickerProgress;
	bool colorPickerDragging = false;

	Vector2 hueSliderPos;
	Vector2 hueSliderSize;
	float hueSliderProgress;
	bool hueSliderDragging = false;

	Vector2 alphaSliderPos;
	Vector2 alphaSliderSize;
	float alphaSliderProgress;
	bool alphaSliderDragging = false;

	ColorButton* parent;

	friend class ColorButton;
protected:
	
};