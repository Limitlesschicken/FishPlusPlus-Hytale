#include "SliderButton.h"

#include <algorithm>
#include <Windows.h>
#include <format>

#include "Menu/Style.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FontRenderer/Fonts.h"
#include "FeatureDispatcher/Settings/SliderSetting.h"

void SliderButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;
	m_activePercent = std::clamp(m_activePercent, 0.0f, 1.0f);
	auto* s = static_cast<SliderSetting*>(this->setting);

	if (m_dragging)
		m_activePercent += (float)fastDeltaTime / 1.5f;
	else
		m_activePercent -= (float)fastDeltaTime / 1.5f;

	Color textColor = Color::Blend(Style::moduleTextActiveColor, Color::White(), m_activePercent);

	Renderer2D::colored->Square(Vector2(x, y), m_UIProgress, height, Color::Normalize(Style::sliderColor));
	Renderer2D::colored->Render();
	Fonts::Figtree->RenderTextShadow(setting->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor));
	std::string stringValue = std::format("{:.1f}", s->GetValue());
	Fonts::Figtree->RenderTextShadow(stringValue, x + width - Fonts::Figtree->getWidth(stringValue) - 6.0f, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor));

}
void SliderButton::Update(float mouseX, float mouseY) {
	auto* s = static_cast<SliderSetting*>(this->setting);
	m_UIProgress = s->GetValue() / s->maxValue * width;
	height = Style::featureHeight;
}

void SliderButton::MouseClicked(float mouseX, float mouseY, int vk) {
	if (!this->IsHovered(mouseX, mouseY))
		return;

	if (vk != VK_LBUTTON)
		return;

	CalculateSlider(mouseX);

	m_dragging = true;
}
void SliderButton::MouseReleased(float mouseX, float mouseY, int vk) {
	m_dragging = false;

}
void SliderButton::MouseDragged(float mouseX, float mouseY, int vk, float deltaX, float deltaY) {
	if (!m_dragging)
		return;

	CalculateSlider(mouseX);
}

void SliderButton::CalculateSlider(float mouseX) {
	m_UIProgress = mouseX - this->x;
	m_UIProgress = std::clamp(m_UIProgress, 0.0f, width);

	auto* s = static_cast<SliderSetting*>(this->setting);

	float normalizedProgress = m_UIProgress / width;
	float valueProgress = normalizedProgress * s->maxValue;

	s->SetValue(valueProgress);
}