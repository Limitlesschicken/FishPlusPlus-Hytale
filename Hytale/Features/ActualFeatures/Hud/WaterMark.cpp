/*
 * Copyright (c) FishPlusPlus.
 */

#include "WaterMark.h"

#include "Renderer/FontRenderer/Fonts.h"

void WaterMark::OnRender2D() {
	this->GetElement()->SetSize(Fonts::Figtree->getWidth("Fish++") + 6, Fonts::Figtree->getHeight() + 6);
	Fonts::Figtree->RenderText("Fish++",this->GetElement()->GetX() + 3, this->GetElement()->GetY() + 6, 1, Color::Normalize(Style::headerColor));
}

void WaterMark::Initialize() {
	Util::log("Initialized WaterMark feature");
	RegisterEvent(this);
}