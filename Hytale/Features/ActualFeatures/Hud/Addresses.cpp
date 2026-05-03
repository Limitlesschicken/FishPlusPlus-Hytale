/*
 * Copyright (c) FishPlusPlus.
 */
#include "Addresses.h"

#include "Renderer/FontRenderer/Fonts.h"

void Addresses::OnRender2D() {
	this->GetElement()->SetSize(Fonts::Figtree->getWidth(Util::string_format("GameInstance: 0x%llX", Util::getGameInstance())) + 6, Fonts::Figtree->getHeight() + 6);
	Fonts::Figtree->RenderText(Util::string_format("GameInstance: 0x%llX", Util::getGameInstance()), this->GetElement()->GetX() + 3, this->GetElement()->GetY() + 6, 1, Color::Normalize(Style::headerColor));
}

void Addresses::Initialize() {
	Util::log("Initialized Addresses feature");
	RegisterEvent(this);
}