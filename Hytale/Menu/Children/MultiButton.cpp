#include "MultiButton.h"

#include <Windows.h>

#include "../Style.h"
#include "FeatureDispatcher/Settings/MultiSetting.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FontRenderer/Fonts.h"

void MultiButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;
	auto* s = static_cast<MultiSetting*>(this->setting);

	if (m_hovered) {
		m_hoverAlpha += 50.0f * (float)fastDeltaTime;
		if (m_hoverAlpha > Style::moduleHoverColor.a)
			m_hoverAlpha = Style::moduleHoverColor.a;
	}
	else {
		m_hoverAlpha -= 50.0f * (float)fastDeltaTime;
		if (m_hoverAlpha < 0)
			m_hoverAlpha = 0;
	}

	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(Style::moduleHoverColor.r, Style::moduleHoverColor.g, Style::moduleHoverColor.b, m_hoverAlpha));
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