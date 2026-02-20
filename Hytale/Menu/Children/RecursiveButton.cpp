/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "RecursiveButton.h"
#include "FeatureDispatcher/Settings/RecursiveSetting.h"

RecursiveButton::RecursiveButton(Setting<bool>* setting) : SettingButton(setting) {
	auto body = std::make_unique<RecursiveBody>(this);
	this->body = body.get();
	AddChild(std::move(body));
}

void RecursiveButton::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;

	m_hoverAlpha += (m_hovered ? 50.0f : -50.0f) * fastDeltaTime;
	if (m_hoverAlpha > Style::moduleHoverColor.a)
		m_hoverAlpha = Style::moduleHoverColor.a;
	if (m_hoverAlpha < 0)
		m_hoverAlpha = 0;

	auto* s = static_cast<Setting<bool>*>(this->setting);

	m_activePercent = std::clamp(m_activePercent, 0.0f, 1.0f);

	if (s->GetValue())
		m_activePercent += (float)fastDeltaTime / 1.5f;
	else
		m_activePercent -= (float)fastDeltaTime / 1.5f;

	Color hoverColor(Style::moduleHoverColor.r, Style::moduleHoverColor.g, Style::moduleHoverColor.b, m_hoverAlpha);
	Color boxColor = Color::Blend(Style::recursiveColor, hoverColor, m_activePercent);

	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(boxColor));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderTextShadow(this->setting->GetName(), x + Style::settingsNamePadding.x, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(Color::White()));
	Fonts::Figtree->RenderTextShadow("=", x + width - Fonts::Figtree->getWidth("=") - 6.0f, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(Color::White()));

	if (this->body->open)
		Component::Render(deltaTime);
}

void RecursiveButton::MouseClicked(float mouseX, float mouseY, int vk) {
	Component::MouseClicked(mouseX, mouseY, vk);
	if (!this->IsHovered(mouseX, mouseY))
		return;

	if (vk == VK_RBUTTON)
		this->body->open = !this->body->open;

	auto* s = static_cast<Setting<bool>*>(this->setting);
	if (vk == VK_LBUTTON)
		s->SetValue(!s->GetValue());
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