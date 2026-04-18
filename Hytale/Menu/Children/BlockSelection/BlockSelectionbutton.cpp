/*
 * Copyright (c) FishPlusPlus.
 */

#include "BlockSelectionButton.h"

#include "../../Style.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FontRenderer/Fonts.h"
#include "Features/Settings/BlockSelection/BlockSetting.h"

BlockSelectionButton::BlockSelectionButton(Setting<std::vector<BlockResult>>* setting) : SettingButton(setting) {
	this->screen = std::make_unique<BlockSelectionScreen>(setting);
}

void BlockSelectionButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;
	auto* s = static_cast<BlockSetting*>(this->setting);

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	if (m_hoverAlpha > Style::featureHoverColor.a)
		m_hoverAlpha = Style::featureHoverColor.a;
	if (m_hoverAlpha < 0)
		m_hoverAlpha = 0;

	Color textColor = Color::Blend(Style::featureTextActiveColor, Color::White(), m_activePercent);

	Color topColor = Color(Style::featureHoverColor.r, Style::featureHoverColor.g, Style::featureHoverColor.b, m_hoverAlpha);
	Color bottomColor = Color(Style::featureHoverGradColor.r, Style::featureHoverGradColor.g, Style::featureHoverGradColor.b, m_hoverAlpha);

	Renderer2D::colored->SquareMultiColor(Vector2(x, y), width, height,
		Color::Normalize(topColor), Color::Normalize(topColor),
		Color::Normalize(bottomColor), Color::Normalize(bottomColor));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderText(s->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor));
	Fonts::Figtree->RenderText("!", x + width - Fonts::Figtree->getWidth("!") - 6.0f, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(textColor)); //just a random icon for testing not permanent
}

void BlockSelectionButton::Update(float mouseX, float mouseY) {
	m_hovered = this->IsHovered(mouseX, mouseY);
	this->height = Style::featureHeight;
}

void BlockSelectionButton::MouseClicked(float mouseX, float mouseY, int vk) {
	if (this->IsHovered(mouseX, mouseY)) {
		if (vk == VK_LBUTTON)
			Menu::SetMainComponent(screen.get());
	}
}