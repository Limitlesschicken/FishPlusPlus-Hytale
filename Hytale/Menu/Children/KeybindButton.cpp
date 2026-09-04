/*
 * Copyright (c) FishPlusPlus.
 */
#include "KeybindButton.h"

#include <algorithm>

#include "../Style.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FontRenderer/Fonts.h"
#include "Features/Settings/KeybindSetting.h"
#include "Util/Util.h"

void KeybindButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;
	auto* s = static_cast<KeybindSetting*>(this->setting);

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	m_hoverAlpha = m_hoverAlpha > Style::featureHoverColor.a ? Style::featureHoverColor.a : (m_hoverAlpha < 0 ? 0 : m_hoverAlpha);

	if (this->choosingBind)
		m_activePercent += (float)fastDeltaTime / 1.5f;
	else
		m_activePercent -= (float)fastDeltaTime / 1.5f;


	m_activePercent = std::clamp(m_activePercent, 0.0f, 1.0f);

	Color textColor = Color::Blend(Style::featureTextActiveColor, Color::White(), m_activePercent);

	Color topColor = Color(Style::featureHoverColor.r, Style::featureHoverColor.g, Style::featureHoverColor.b, m_hoverAlpha);
	Color bottomColor = Color(Style::featureHoverGradColor.r, Style::featureHoverGradColor.g, Style::featureHoverGradColor.b, m_hoverAlpha);

	Renderer2D::colored->SquareMultiColor(Vector2(x, y), width, height,
		Color::Normalize(topColor), Color::Normalize(topColor),
		Color::Normalize(bottomColor), Color::Normalize(bottomColor));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderText(s->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor));
	std::string keyText = this->choosingBind ? "..." : Util::GetKeyName(s->GetValue());
	Fonts::Figtree->RenderText(keyText, x + width - Fonts::Figtree->getWidth(keyText) - 6.0f, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor));
}
void KeybindButton::Update(float mouseX, float mouseY) {
	m_hovered = this->IsHovered(mouseX, mouseY);
	this->height = Style::featureHeight;

	if (!this->choosingBind)
		return;

	if (InputSystem::keysPressed.empty())
		return;

	auto* s = static_cast<KeybindSetting*>(this->setting);
	SDL_Scancode key = *InputSystem::keysPressed.begin();
	if (Util::GetKeyName(key) == "None" || key == SDL_SCANCODE_BACKSPACE) {
		if (s->isDefaultKeybind && s->parentFeature)
			s->parentFeature->SetKeybind(SDL_SCANCODE_UNKNOWN);
		s->SetValue(SDL_SCANCODE_UNKNOWN);
	} else {
		if (s->isDefaultKeybind && s->parentFeature)
			s->parentFeature->SetKeybind(key);
		s->SetValue(key);
	}

	this->choosingBind = false;
}

void KeybindButton::MouseClicked(float mouseX, float mouseY, int vk) {
	if (this->IsHovered(mouseX, mouseY)) {
		if (vk == VK_LBUTTON)
			this->choosingBind = true;
	}
}
