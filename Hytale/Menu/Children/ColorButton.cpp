/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "ColorButton.h"

#define NOMINMAX

#include "core.h"
#include <algorithm>



#include "../Style.h"
#include "../../Renderer/Renderer2D.h"
#include "../../Renderer/FontRenderer/Fonts.h"
#include "../../Renderer/SpecialRenderers/HueRenderer.h"
#include "FeatureDispatcher/Settings/ColorSetting.h"

ColorButton::ColorButton(Setting<Color>* setting) : SettingButton(setting) {
	auto body = std::make_unique<ColorSettingControl>(this);
	this->body = body.get();
	AddChild(std::move(body));
}

void ColorButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;
	auto* s = static_cast<ColorSetting*>(this->setting);

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	if (m_hoverAlpha > Style::moduleHoverColor.a)
		m_hoverAlpha = Style::moduleHoverColor.a;
	if (m_hoverAlpha < 0)
		m_hoverAlpha = 0;

	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(Style::moduleHoverColor.r, Style::moduleHoverColor.g, Style::moduleHoverColor.b, m_hoverAlpha));
	Renderer2D::colored->Render();

	Renderer2D::alphaChecker->Square(Vector2(x + width - this->previewSize - 6.0f, (y + height / 2.0f) - previewSize / 2.0f), previewSize, previewSize, Color::White());
	Renderer2D::alphaChecker->Render();
	
	Renderer2D::colored->Square(Vector2(x + width - this->previewSize - 6.0f, (y + height / 2.0f) - previewSize / 2.0f), previewSize, previewSize, Color::Normalize(s->GetValue()));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderTextShadow(s->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(Color::White()));

	if (this->body->open)
		Component::Render(deltaTime);
}

void ColorButton::Update(float mouseX, float mouseY) {
	m_hovered = this->IsHovered(mouseX, mouseY);
	this->height = Style::featureHeight;
	this->bodyHeight = body->GetHeight();
	this->body->SetX(x);
	this->body->SetY(y + height);
	this->body->SetWidth(width);
	Component::Update(mouseX, mouseY);
}

void ColorButton::MouseClicked(float mouseX, float mouseY, int vk) {
	if (this->body->open)
		Component::MouseClicked(mouseX, mouseY, vk);
	if (!this->IsHovered(mouseX, mouseY))
		return;

	if (vk == VK_RBUTTON)
		this->body->open = !this->body->open;
}

ColorSettingControl::ColorSettingControl(ColorButton* parent) {
	this->parent = parent;
	auto* s = static_cast<ColorSetting*>(this->parent->setting);
	colorPickerPos = Vector2(x + 4.0f, y + 4.0f);
	colorPickerSize = Vector2(170.0f, 170.0f);
	hueSliderPos = Vector2(x + 4.0f + colorPickerSize.x + 6.0f, y + 4.0f);
	hueSliderSize = Vector2(10.0f, 170.0f);
	alphaSliderPos = Vector2(x + 4.0f, y + colorPickerSize.y + 8.0f);
	alphaSliderSize = Vector2(192.0f, 20.0f);
	alphaSliderProgress = s->GetValue().a / 255 * alphaSliderSize.x;

	float r = s->GetValue().r / 255.0f;
	float g = s->GetValue().g / 255.0f;
	float b = s->GetValue().b / 255.0f;

	float max = std::max(r, std::max(g, b));
	float min = std::min(r, std::min(g, b));
	float delta = max - min;

	if (delta == 0) hue = 0;
	else if (max == r) hue = 60.0f * fmod(((g - b) / delta), 6.0f);
	else if (max == g) hue = 60.0f * (((b - r) / delta) + 2.0f);
	else hue = 60.0f * (((r - g) / delta) + 4.0f);

	if (hue < 0) hue += 360.0f;

	saturation = (max == 0) ? 0 : (delta / max);
	value = max;
	alpha = s->GetValue().a;

	hueSliderProgress = hue / 360.0f * hueSliderSize.y;
	colorPickerProgress.x = saturation * colorPickerSize.x;
	colorPickerProgress.y = (1.0f - value) * colorPickerSize.y;
}

void ColorSettingControl::Render(double deltaTime) {
	RenderColorPicker();
	RenderHueSlider();
	RenderAlphaSlider();
}

void ColorSettingControl::Update(float mouseX, float mouseY) {
	this->height = open ? 200.0f : 0.0f;

	colorPickerPos = Vector2(x + 4.0f, y + 4.0f);
	hueSliderPos = Vector2(x + 4.0f + colorPickerSize.x + 6.0f, y + 4.0f);
	alphaSliderPos = Vector2(x + 4.0f, y + colorPickerSize.y + 8.0f);
}

void ColorSettingControl::RenderColorPicker() {
	auto* s = static_cast<ColorSetting*>(this->parent->setting);
	Renderer2D::colored->SquareMultiColor(colorPickerPos, colorPickerSize.x, colorPickerSize.y,
		Color::Normalize(Color::White()),
		Color::Normalize(this->HSVtoRGB(hue, 1.0f, 1.0f)),
		Color::Normalize(Color::White()),
		Color::Normalize(this->HSVtoRGB(hue, 1.0f, 1.0f)));
	Renderer2D::colored->SquareMultiColor(Vector2(x + 4.0f, y + 4.0f), colorPickerSize.x, colorPickerSize.y,
		Color(0, 0, 0, 0),
		Color(0, 0, 0, 0),
		Color::Normalize(Color::Black()),
		Color::Normalize(Color::Black()));
	Renderer2D::colored->Square(Vector2(colorPickerPos.x + colorPickerProgress.x - 4.0f, colorPickerPos.y + colorPickerProgress.y - 4.0f), 8.0f, 8.0f, Color::Normalize(Color::White()));
	Renderer2D::colored->Render();
}
void ColorSettingControl::RenderHueSlider() {
	auto* hue = static_cast<HueRenderer*>(Renderer2D::hue.get());
	hue->Square(hueSliderPos, hueSliderSize.x, hueSliderSize.y, Color::White());
	hue->SetupUniformInfo(hueSliderPos.y, hueSliderSize.y);
	hue->Render();

	Renderer2D::colored->Square(Vector2(hueSliderPos.x - 2.0f, hueSliderPos.y + hueSliderProgress), hueSliderSize.x + 4.0f, 2.0f, Color::Normalize(Color::White()));
	Renderer2D::colored->Render();
}

void ColorSettingControl::RenderAlphaSlider() {
	auto* s = static_cast<ColorSetting*>(this->parent->setting);
	Renderer2D::colored->SquareMultiColor(alphaSliderPos, alphaSliderSize.x, alphaSliderSize.y,
		Color::Normalize(Color::Black()),
		Color::Normalize(Color(s->GetValue().r, s->GetValue().g, s->GetValue().b, 255)),
		Color::Normalize(Color::Black()),
		Color::Normalize(Color(s->GetValue().r, s->GetValue().g, s->GetValue().b, 255)));

	Renderer2D::colored->Square(Vector2(alphaSliderPos.x + alphaSliderProgress, alphaSliderPos.y - 2.0f), 2.0f, alphaSliderSize.y + 4.0f, Color::Normalize(Color::White()));
	Renderer2D::colored->Render();
}

void ColorSettingControl::MouseClicked(float mouseX, float mouseY, int vk) {
	if (vk != VK_LBUTTON)
		return;

	if (this->IsMouseHovered(mouseX, mouseY, colorPickerPos, colorPickerSize))
		colorPickerDragging = true;
	if (this->IsMouseHovered(mouseX, mouseY, alphaSliderPos, alphaSliderSize))
		alphaSliderDragging = true;
	if (this->IsMouseHovered(mouseX, mouseY, hueSliderPos, hueSliderSize))
		hueSliderDragging = true;

	CalculateSliders(mouseX, mouseY);
}

void ColorSettingControl::MouseReleased(float mouseX, float mouseY, int vk) {
	if (vk != VK_LBUTTON)
		return;

	colorPickerDragging = false;
	hueSliderDragging = false;
	alphaSliderDragging = false;
}
void ColorSettingControl::MouseDragged(float mouseX, float mouseY, int vk, float deltaX, float deltaY) {
	CalculateSliders(mouseX, mouseY);
}

void ColorSettingControl::CalculateSliders(float mouseX, float mouseY) {
	auto* s = static_cast<ColorSetting*>(this->parent->setting);
	if (alphaSliderDragging) {
		alphaSliderProgress = mouseX - alphaSliderPos.x;
		alphaSliderProgress = std::clamp(alphaSliderProgress, 0.0f, alphaSliderSize.x);
		alpha = alphaSliderProgress / alphaSliderSize.x * 255.0f;
	}
	if (hueSliderDragging) {
		hueSliderProgress = mouseY - hueSliderPos.y;
		hueSliderProgress = std::clamp(hueSliderProgress, 0.0f, hueSliderSize.y);
		hue = hueSliderProgress / hueSliderSize.y * 360.0f;
	}
	if (colorPickerDragging) {
		colorPickerProgress.x = mouseX - colorPickerPos.x;
		colorPickerProgress.x = std::clamp(colorPickerProgress.x, 0.0f, colorPickerSize.x);
		saturation = colorPickerProgress.x / colorPickerSize.x;

		colorPickerProgress.y = mouseY - colorPickerPos.y;
		colorPickerProgress.y = std::clamp(colorPickerProgress.y, 0.0f, colorPickerSize.y);
		value = 1.0f - (colorPickerProgress.y / colorPickerSize.y);
	}

	Color result = this->HSVtoRGB(hue, saturation, value, alpha);
	s->SetValue(result);
}