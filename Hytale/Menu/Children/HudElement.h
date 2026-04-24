/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Component.h"

class HudFeature;

class HudElement : public Component {
public:
	HudElement(HudFeature* feature) : Component() {
		this->parentFeature = feature;
	}

	void SetSize(float width, float height) {
		if (resizeLeft)
			this->SetX(this->GetX() - (width - this->GetWidth()));
		if (resizeUp)
			this->SetY(this->GetY() - (height - this->GetHeight()));
		this->SetWidth(width);
		this->SetHeight(height);
	}

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;
	void MouseClicked(float mouseX, float mouseY, int virtualKeyCode) override;
	void MouseReleased(float mouseX, float mouseY, int virtualKeyCode) override;
	void MouseDragged(float mouseX, float mouseY, int virtualKeyCode, float deltaX, float deltaY) override;

	void LoadConfigPosition(float x, float y, float width, float height);

	bool allowDynamicResize = true;
private:
	HudFeature* parentFeature;

	float hoverAlpha = 0;
	bool hovered = false;
	bool dragging = false;

	bool resizeLeft = false;
	bool resizeUp = false;
};