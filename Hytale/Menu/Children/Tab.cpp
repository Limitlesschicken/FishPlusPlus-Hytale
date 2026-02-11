#include "Tab.h"

#include <Windows.h>
#include <memory>
#include <iostream>

#include "FeatureButton.h"

#include "../Style.h"

#include "../../Renderer/Renderer2D.h"
#include "../../Renderer/FontRenderer/Fonts.h"

#include "../../FeatureDispatcher/FeatureDispatcher.h"

Tab::Tab(std::string name, float x, float y) : Component(x, y, 200, Style::headerHeight) {
	this->name = name;
	this->m_dragging = false;

	this->AddChild(std::make_unique<Body>(this));
}

void Tab::Render(double deltaTime) {
	DrawHeader();
	Component::Render(deltaTime);
}

void Tab::DrawHeader() {
	Renderer2D::colored->BeginScissor(x, y, width, height);
	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(Style::headerColor));
	Renderer2D::colored->Render();
	Fonts::Figtree->RenderTextShadow(name, x + Style::headerPadding.x, y + Style::headerPadding.y, 1, Color::White());
	Renderer2D::colored->EndScissor();
}

void Tab::Update(float mouseX, float mouseY) {
	Component::Update(mouseX, mouseY);
}

void Tab::MouseClicked(float mouseX, float mouseY, int vk) {
	Component::MouseClicked(mouseX, mouseY, vk);
	if (!IsHovered(mouseX, mouseY))
		return;

	if (vk == VK_LBUTTON)
		m_dragging = true;
}
void Tab::MouseReleased(float mouseX, float mouseY, int vk) {
	Component::MouseReleased(mouseX, mouseY, vk);

	if (vk == VK_LBUTTON)
		m_dragging = false;
}

void Tab::MouseDragged(float mouseX, float mouseY, int vk, float deltaX, float deltaY) {
	Component::MouseDragged(mouseX, mouseY, vk, deltaX, deltaY);

	if (m_dragging) {
		Move(deltaX, deltaY);
		Update(mouseX, mouseY);
	}
}

Body::Body(Tab* tab) {
	this->tab = tab;
	for (auto& feature : FeatureDispatcher::features) {
		if (feature->getCategory() != tab->name)
			continue;
		this->AddChild(std::make_unique<FeatureButton>(feature.get()));
	}
	this->width = 200;
	this->height = this->m_children.size() * Style::featureHeight;
}

void Body::Render(double deltaTime) {
	Renderer2D::colored->SquareOutline(Vector2(x, y), width, height, Color::Normalize(Style::tabBgColor), Color::Normalize(Style::tabOutlineColor));
	Renderer2D::colored->Render();

	Component::Render(deltaTime);
}

void Body::Update(float mouseX, float mouseY) {
	this->x = tab->x;
	this->y = tab->y + tab->height;

	float offsetY = 0;
	for (auto& feature : this->m_children) {
		feature->SetX(tab->x + 1);
		feature->SetY(offsetY + y);
		feature->SetWidth(width - 2);
		feature->Update(mouseX, mouseY);
		offsetY += feature->GetHeight();
		if (auto* fb = dynamic_cast<FeatureButton*>(feature.get())) {
			offsetY += fb->GetBodyHeight();
		}
	}
	height = offsetY;
}