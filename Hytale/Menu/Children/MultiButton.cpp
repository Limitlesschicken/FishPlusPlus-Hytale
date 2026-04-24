/*
 * Copyright (c) FishPlusPlus.
 */
#include "MultiButton.h"

#include <Windows.h>

#include "../Style.h"
#include "Features/Settings/MultiSetting.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FontRenderer/Fonts.h"

void MultiButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;
	auto* s = static_cast<MultiSetting*>(this->setting);

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	m_hoverAlpha = m_hoverAlpha > Style::featureHoverColor.a ? Style::featureHoverColor.a : (m_hoverAlpha < 0 ? 0 : m_hoverAlpha);

	Color topColor = Color(Style::featureHoverColor.r, Style::featureHoverColor.g, Style::featureHoverColor.b, m_hoverAlpha);
	Color bottomColor = Color(Style::featureHoverGradColor.r, Style::featureHoverGradColor.g, Style::featureHoverGradColor.b, m_hoverAlpha);

	Renderer2D::colored->SquareMultiColor(Vector2(x, y), width, height,
		Color::Normalize(topColor), Color::Normalize(topColor),
		Color::Normalize(bottomColor), Color::Normalize(bottomColor));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderText(s->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(Color::White()));
	Fonts::Figtree->RenderText(s->GetValueName(), x + width - Fonts::Figtree->getWidth(s->GetValueName()) - 6.0f, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(Color::White()));
}

void MultiButton::Update(float mouseX, float mouseY) {
	m_hovered = IsHovered(mouseX, mouseY);
	height = Style::featureHeight;
}

void MultiButton::MouseClicked(float mouseX, float mouseY, int vk) {
	if (!this->IsHovered(mouseX, mouseY))
		return;

	auto* s = static_cast<MultiSetting*>(this->setting);

	if (vk == VK_LBUTTON)
		s->Increment();

	if (vk == VK_RBUTTON)
		s->Decrement();
}