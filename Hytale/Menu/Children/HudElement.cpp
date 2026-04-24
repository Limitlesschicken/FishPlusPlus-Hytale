/*
 * Copyright (c) FishPlusPlus.
 */
#include "HudElement.h"

#include "../../Renderer/Renderer2D.h"
#include "Features/HudFeature.h"

void HudElement::Render(double deltaTime) {
	if (!this->parentFeature->IsActive())
		return;
	double fastDeltaTime = deltaTime * 20.0;
	hoverAlpha += (hovered ? 50.0f : -50.0f) * fastDeltaTime;
	hoverAlpha = hoverAlpha > 100.0f ? 100.0f : (hoverAlpha < 0 ? 0 : hoverAlpha);

	Renderer2D::colored->Square(Vector2(this->GetX(), this->GetY()), this->GetWidth(), this->GetHeight(), Color::Normalize(Color(255, 255, 255, hoverAlpha)));
	Renderer2D::colored->Render();
}
void HudElement::Update(float mouseX, float mouseY) {
	if (!this->parentFeature->IsActive())
		return;
	this->hovered = this->IsHovered(mouseX, mouseY);

	if (this->allowDynamicResize) {
		bool isTopSide = this->GetY() + this->GetHeight() / 2 < Util::app->Engine->Window->WindowHeight / 2;
		bool isRightSide = this->GetX() + this->GetWidth() / 2 > Util::app->Engine->Window->WindowWidth / 2;

		if (isRightSide)
			this->resizeLeft = true;
		else
			this->resizeLeft = false;

		if (isTopSide)
			this->resizeUp = false;
		else
			this->resizeUp = true;
	}

	if (this->GetX() < 0)
		this->SetX(0);
	if (this->GetY() < 0)
		this->SetY(0);
	if (this->GetX() + this->GetWidth() > Util::app->Engine->Window->WindowWidth)
		this->SetX(Util::app->Engine->Window->WindowWidth - this->GetWidth());
	if (this->GetY() + this->GetHeight() > Util::app->Engine->Window->WindowHeight)
		this->SetY(Util::app->Engine->Window->WindowHeight - this->GetHeight());
}
void HudElement::MouseClicked(float mouseX, float mouseY, int virtualKeyCode) {
	if (!this->parentFeature->IsActive())
		return;

	if (!hovered)
		return;

	if (virtualKeyCode == VK_LBUTTON) {
		dragging = true;
	}
}
void HudElement::MouseReleased(float mouseX, float mouseY, int virtualKeyCode) {
	if (!this->parentFeature->IsActive())
		return;

	dragging = false;
}
void HudElement::MouseDragged(float mouseX, float mouseY, int virtualKeyCode, float deltaX, float deltaY) {
	if (!this->parentFeature->IsActive())
		return;

	if (dragging && virtualKeyCode == VK_LBUTTON) {
		this->Move(deltaX, deltaY);
	}
}

void HudElement::LoadConfigPosition(float x, float y, float width, float height) {
	bool isTopSide = y + height / 2 < Util::app->Engine->Window->WindowHeight / 2;
	bool isRightSide = x + width / 2 > Util::app->Engine->Window->WindowWidth / 2;
	
	if (isRightSide)
		this->SetX(x - (this->GetWidth() - width));
	else
		this->SetX(x);
	if (!isTopSide)
		this->SetY(y - (this->GetHeight() - height));
	else
		this->SetY(y);
}