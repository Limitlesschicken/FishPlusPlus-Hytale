/*
 * Copyright (c) FishPlusPlus.
 */
#include "FeatureButton.h"

#include <Windows.h>
#include <algorithm>
#include <memory>
#include <iostream>

#include "../../Features/Feature.h"
#include "../Style.h"

#include "../../Renderer/Renderer2D.h"
#include "../../Renderer/FontRenderer/Fonts.h"


FeatureButton::FeatureButton(Feature* feature)  {
	this->feature = feature;
	this->height = Style::featureHeight;
	auto body = std::make_unique<SettingsBody>(this, feature);
	m_body = body.get();
	AddChild(std::move(body));
}

void FeatureButton::Render(double deltaTime) {

	double fastDeltaTime = deltaTime * 20.0;

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	if (m_hoverAlpha > Style::featureHoverColor.a)
		m_hoverAlpha = Style::featureHoverColor.a;
	if (m_hoverAlpha < 0)
		m_hoverAlpha = 0;
	
	if (this->feature->IsActive())
		m_textColorPercent += (float)fastDeltaTime / 1.5f;
	else
		m_textColorPercent -= (float)fastDeltaTime / 1.5f;

	m_textColorPercent = std::clamp(m_textColorPercent, 0.0f, 1.0f);


	Color textColor = Color::Blend(Style::featureTextActiveColor, Color::White(), m_textColorPercent);

	Color topColor = Color(Style::featureHoverColor.r, Style::featureHoverColor.g, Style::featureHoverColor.b, m_hoverAlpha);
	Color bottomColor = Color(Style::featureHoverGradColor.r, Style::featureHoverGradColor.g, Style::featureHoverGradColor.b, m_hoverAlpha);

	Renderer2D::colored->BeginScissor(x, y, width, height + m_bodyHeight);
	Renderer2D::colored->SquareMultiColor(Vector2(x, y ), width, height,
		Color::Normalize(topColor), Color::Normalize(topColor),
		Color::Normalize(bottomColor), Color::Normalize(bottomColor));
	Renderer2D::colored->Render();

	std::string openName = m_body->IsOpened() ? "-" : "+";

	Fonts::Figtree->RenderText(feature->GetName(), x + Style::featureNamePadding.x, y + Style::featureNamePadding.y, 1.0f, Color::Normalize(textColor));
	Fonts::Figtree->RenderText(openName, x + width - Fonts::Figtree->getWidth(openName) - 6.0f, y + Style::featureNamePadding.y, 1.0f, Color::Normalize(textColor));
	
	Component::Render(deltaTime);
	Renderer2D::colored->EndScissor();
}

void FeatureButton::Update(float mouseX, float mouseY) {
	m_hovered = this->IsHovered(mouseX, mouseY);
	this->m_bodyHeight = this->m_body->GetHeight();

	Component::Update(mouseX, mouseY);
}

void FeatureButton::MouseClicked(float mouseX, float mouseY, int virtualKeyCode) {
	Component::MouseClicked(mouseX, mouseY, virtualKeyCode);
	if (!this->IsHovered(mouseX, mouseY))
		return;

	if (virtualKeyCode == VK_LBUTTON) {
		feature->ToggleState();
	}

	if (virtualKeyCode == VK_RBUTTON) {
		m_body->ToggleOpen();
	}
}

SettingsBody::SettingsBody(FeatureButton* featureButton, Feature* feature) {
	this->width = 300.f;
	this->featureButton = featureButton;
	for (auto& setting : feature->GetSettings()) {
		AddChild(setting->CreateButton());
	}
}

void SettingsBody::Update(float mouseX, float mouseY) {
	this->x = featureButton->GetX();
	this->y = featureButton->GetY() + featureButton->GetHeight();
	float offsetY = 0.0f;
	m_wantedHeight = 0.0f;

	for (auto& component : m_children) {
		SettingButton* setting = (SettingButton*)(component.get());
		setting->SetX(this->x);
		setting->SetY(offsetY + this->y);
		setting->SetWidth(this->width);
		if (m_open)
			setting->Update(mouseX, mouseY);
		offsetY += setting->GetHeight();
		offsetY += setting->bodyHeight;

		m_wantedHeight = offsetY;
	}
	m_wantedHeight = m_open ? offsetY : 0.0f;
}

void SettingsBody::Render(double deltaTime) {
	height += (m_wantedHeight - height) * (float)deltaTime * 20.0f;

	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(Color(0, 0, 0, 70)));

	if (std::abs(height - m_wantedHeight) < 0.1f)
		height = m_wantedHeight;

	if (height > 1.0f)
		Component::Render(deltaTime);
}

void SettingsBody::MouseClicked(float mouseX, float mouseY, int virtualKeyCode) {
	if (m_open)
		Component::MouseClicked(mouseX, mouseY, virtualKeyCode);
}
