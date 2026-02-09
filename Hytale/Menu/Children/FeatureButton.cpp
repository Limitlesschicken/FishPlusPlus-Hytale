#include "FeatureButton.h"

#include <Windows.h>

#include "../../FeatureDispatcher/Feature.h"
#include "../Style.h"

#include "../../Renderer/Renderer2D.h"
#include "../../Renderer/FontRenderer/Fonts.h"

FeatureButton::FeatureButton(Feature* feature)  {
	this->feature = feature;
	this->height = Style::featureHeight;

}

void FeatureButton::Render(double deltaTime) {
	Renderer2D::colored->Square(Vector2(x, y), width, height, m_hovered ? Color::From255(Style::moduleHoverColor) : Color(0, 0, 0, 0));
	Renderer2D::colored->Render();

	Fonts::Figtree->RenderTextShadow(feature->getName(), x + Style::moduleNamePadding.x, y + Style::moduleNamePadding.y, 1.0f, feature->active ? Color::From255(Style::moduleTextActiveColor) : Color::White());
}
void FeatureButton::Update(float mouseX, float mouseY) {
	m_hovered = this->IsHovered(mouseX, mouseY);
}

void FeatureButton::MouseClicked(float mouseX, float mouseY, int virtualKeyCode) {
	if (!this->IsHovered(mouseX, mouseY))
		return;

	if (virtualKeyCode == VK_LBUTTON) {
		feature->setActive(!feature->active);
	}
}