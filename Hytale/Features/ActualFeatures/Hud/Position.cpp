/*
 * Copyright (c) FishPlusPlus.
 */

#include "Position.h"

#include "Renderer/FontRenderer/Fonts.h"

void PositionHud::OnRender2D() {
	this->GetElement()->allowDynamicResize = true;

	Fonts::Figtree->RenderText("XYZ: ", this->GetElement()->GetX() + 3, this->GetElement()->GetY() + 6, 1, Color::Normalize(Style::headerColor));
	std::string positionText = "x, y, z";
	if (Util::app->Stage == AppStage::InGame)
		positionText = Util::string_format("%.1f, %.1f, %.1f", Util::getLocalPlayer()->Position.x, Util::getLocalPlayer()->Position.y, Util::getLocalPlayer()->Position.z);
	Fonts::Figtree->RenderText(positionText, this->GetElement()->GetX() + 3 + Fonts::Figtree->getWidth("XYZ: "), this->GetElement()->GetY() + 6, 1, Color::Normalize(Color::White()));

	this->GetElement()->SetSize(Fonts::Figtree->getWidth("XYZ: ") + Fonts::Figtree->getWidth(positionText) + 6, Fonts::Figtree->getHeight() + 6);
}

void PositionHud::Initialize() {
	Util::log("Initialized Position feature");
	RegisterEvent(this);
}