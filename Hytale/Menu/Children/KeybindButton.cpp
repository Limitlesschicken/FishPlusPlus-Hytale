/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "KeybindButton.h"

#include <algorithm>

#include "../Style.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FontRenderer/Fonts.h"
#include "FeatureDispatcher/Settings/KeybindSetting.h"
#include "Util/Util.h"

void KeybindButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;
	auto* s = static_cast<KeybindSetting*>(this->setting);

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	if (m_hoverAlpha > Style::moduleHoverColor.a)
		m_hoverAlpha = Style::moduleHoverColor.a;
	if (m_hoverAlpha < 0)
		m_hoverAlpha = 0;

	if (this->choosingBind)
		m_activePercent += (float)fastDeltaTime / 1.5f;
	else
		m_activePercent -= (float)fastDeltaTime / 1.5f;


	m_activePercent = std::clamp(m_activePercent, 0.0f, 1.0f);

	Color textColor = Color::Blend(Style::moduleTextActiveColor, Color::White(), m_activePercent);

	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(Style::moduleHoverColor.r, Style::moduleHoverColor.g, Style::moduleHoverColor.b, m_hoverAlpha));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderTextShadow(s->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor));
	std::string keyText = this->choosingBind ? "..." : Util::GetKeyName(s->GetValue());
	Fonts::Figtree->RenderTextShadow(keyText, x + width - Fonts::Figtree->getWidth(keyText) - 6.0f, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor));
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
	if (Util::GetKeyName(key) == "None") {
		if (s->isDefaultKeybind && s->parentFeature)
			s->parentFeature->keybind = SDL_SCANCODE_UNKNOWN;
		s->SetValue(SDL_SCANCODE_UNKNOWN);
	} else {
		if (s->isDefaultKeybind && s->parentFeature)
			s->parentFeature->keybind = key;
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
