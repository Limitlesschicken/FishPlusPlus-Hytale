/*
 * Copyright (c) FishPlusPlus.
 */
#include "RecursiveButton.h"
#include "Features/Settings/RecursiveSetting.h"

RecursiveButton::RecursiveButton(Setting<bool>* setting) : SettingButton(setting) {
	auto body = std::make_unique<RecursiveBody>(this);
	this->body = body.get();
	AddChild(std::move(body));
}

void RecursiveButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	m_hoverAlpha = m_hoverAlpha > Style::featureHoverColor.a ? Style::featureHoverColor.a : (m_hoverAlpha < 0 ? 0 : m_hoverAlpha);

	auto* s = static_cast<Setting<bool>*>(this->setting);

	m_activePercent = std::clamp(m_activePercent, 0.0f, 1.0f);

	if (s->GetValue())
		m_activePercent += (float)fastDeltaTime / 1.5f;
	else
		m_activePercent -= (float)fastDeltaTime / 1.5f;

	Color topColor = Color(Style::recursiveColor.r, Style::recursiveColor.g, Style::recursiveColor.b, m_hoverAlpha);
	Color bottomColor = Color(Style::featureHoverColor.r, Style::featureHoverColor.g, Style::featureHoverColor.b, m_hoverAlpha);

	Color hoverColor(Style::featureHoverColor.r, Style::featureHoverColor.g, Style::featureHoverColor.b, m_hoverAlpha);

	Color boxColor = Color::Blend(Style::recursiveColor, hoverColor, m_activePercent);
	Color boxColorBottom = Color::Blend(Style::recursiveGradColor, bottomColor, m_activePercent);

	Renderer2D::colored->SquareMultiColor(Vector2(x, y), width, height, 
		Color::Normalize(boxColor), Color::Normalize(boxColor),
		Color::Normalize(boxColorBottom), Color::Normalize(boxColorBottom));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderText(this->setting->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(Color::White()));
	Fonts::Figtree->RenderText("=", x + width - Fonts::Figtree->getWidth("=") - 6.0f, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(Color::White()));

	if (this->body->open)
		Component::Render(deltaTime);
}

void RecursiveButton::MouseClicked(float mouseX, float mouseY, int vk) {
	if (this->body->open)
		Component::MouseClicked(mouseX, mouseY, vk);
	if (!this->IsHovered(mouseX, mouseY))
		return;

	if (vk == VK_RBUTTON)
		this->body->open = !this->body->open;

	auto* s = static_cast<Setting<bool>*>(this->setting);
	if (vk == VK_LBUTTON)
		s->SetValue(!s->GetValue());
}

void RecursiveButton::MouseReleased(float mouseX, float mouseY, int vk) {
	if (this->body->open)
		Component::MouseReleased(mouseX, mouseY, vk);
}

void RecursiveButton::Update(float mouseX, float mouseY) {
	this->m_hovered = this->IsHovered(mouseX, mouseY);
	this->height = Style::featureHeight;
	this->bodyHeight = this->body->GetHeight();
	this->body->SetX(x);
	this->body->SetY(y + height);
	this->body->SetWidth(width);

	Component::Update(mouseX, mouseY);
}

RecursiveBody::RecursiveBody(RecursiveButton* parent) {
	this->parent = parent;

	auto* s = static_cast<RecursiveSetting*>(parent->setting);

	for (auto& setting : s->GetSettings()) {
		AddChild(setting->CreateButton());
	}
}


void RecursiveBody::Update(float mouseX, float mouseY) {
	float offsetY = 0.0f;

	

	for (auto& component : m_children) {
		SettingButton* setting = (SettingButton*)(component.get());
		setting->SetX(this->x);
		setting->SetY(offsetY + this->y);
		setting->SetWidth(this->width);
		setting->Update(mouseX, mouseY);
		offsetY += setting->GetHeight();
		offsetY += setting->bodyHeight;
	}
	this->height = open ? offsetY : 0.0f;
}