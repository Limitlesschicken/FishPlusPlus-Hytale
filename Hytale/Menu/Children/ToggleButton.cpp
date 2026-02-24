/*
 * Copyright (c) FishPlusPlus.
 */
#include "ToggleButton.h"

#include <algorithm>
#include <Windows.h>

#include "../Style.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FontRenderer/Fonts.h"

void ToggleButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	if (m_hoverAlpha > Style::moduleHoverColor.a)
		m_hoverAlpha = Style::moduleHoverColor.a;
	if (m_hoverAlpha < 0)
		m_hoverAlpha = 0;

	auto* s = static_cast<Setting<bool>*>(this->setting);

	if (s->GetValue())
		m_activePercent += (float)fastDeltaTime / 1.5f;
	else
		m_activePercent -= (float)fastDeltaTime / 1.5f;


	m_activePercent = std::clamp(m_activePercent, 0.0f, 1.0f);

	Color textColor = Color::Blend(Style::moduleTextActiveColor, Color::White(), m_activePercent);

	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(Style::moduleHoverColor.r, Style::moduleHoverColor.g, Style::moduleHoverColor.b, m_hoverAlpha));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderText(s->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor));

}
void ToggleButton::Update(float mouseX, float mouseY) {
	m_hovered = this->IsHovered(mouseX, mouseY);
	this->height = Style::featureHeight;
}

void ToggleButton::MouseClicked(float mouseX, float mouseY, int virtualKeyCode) {
	if (this->IsHovered(mouseX, mouseY)) {
		auto* s = static_cast<Setting<bool>*>(this->setting);

		if (virtualKeyCode == VK_LBUTTON)
			s->SetValue(!s->GetValue());

	}
	
}