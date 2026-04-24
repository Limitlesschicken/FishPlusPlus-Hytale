/*
 * Copyright (c) FishPlusPlus.
 */

#include "PlayerList.h"

#include "Renderer/FontRenderer/Fonts.h"

PlayerList::PlayerList() : HudFeature("PlayerList") {
	this->distance = RegisterSetting<ToggleSetting>("Distance", false);
}

void PlayerList::OnRender2D() {
	std::vector<std::string> list;

	if (Util::app->Stage != AppStage::InGame) {
		this->GetElement()->SetSize(Fonts::Figtree->getWidth("PlayerList") + 6, Fonts::Figtree->getHeight() + 6);
		Fonts::Figtree->RenderText("PlayerList", this->GetElement()->GetX() + 3, this->GetElement()->GetY() + 6, 1, Color::Normalize(Style::headerColor));
		return;
	}


	
	for (EntityData entity : SDK::entities) {
		if (!entity.player)
			continue;

		if (entity.isLocalPlayer)
			continue;

		if (this->distance->GetValue()) {
			float dist = (Util::getLocalPlayer()->Position - entity.position).length();
			list.push_back(entity.name + " [" + std::to_string((int)dist) + "m]");
		}
		else
			list.push_back(entity.name);
	}

	if (list.empty())
		return;

	bool isOnTop = this->GetElement()->GetY() + this->GetElement()->GetHeight() / 2 < Util::app->Engine->Window->WindowHeight / 2;

	this->GetElement()->allowDynamicResize = true;
	std::sort(list.begin(), list.end(),
		[isOnTop](const std::string& a, const std::string& b) {
			if (isOnTop)
				return Fonts::Figtree->getWidth(a) > Fonts::Figtree->getWidth(b);
			else
				return Fonts::Figtree->getWidth(a) < Fonts::Figtree->getWidth(b);
		}
	);

	this->GetElement()->SetSize(Fonts::Figtree->getWidth(isOnTop ? list[0] : list.back()) + 6, Fonts::Figtree->getHeight() * list.size() + 6);


	for (size_t i = 0; i < list.size(); i++) {
		if (this->GetElement()->GetX() + this->GetElement()->GetWidth() / 2 < Util::app->Engine->Window->WindowWidth / 2)
			Fonts::Figtree->RenderText(list[i], this->GetElement()->GetX() + 3, this->GetElement()->GetY() + 6 + (i * Fonts::Figtree->getHeight()), 1, Color::Normalize(Style::headerColor));
		else
			Fonts::Figtree->RenderText(
				list[i], this->GetElement()->GetX() + this->GetElement()->GetWidth() - Fonts::Figtree->getWidth(list[i]) - 3,
				this->GetElement()->GetY() + 6 + (i * Fonts::Figtree->getHeight()),
				1, Color::Normalize(Style::headerColor));
	}
}

void PlayerList::Initialize() {
	Util::log("Initialized PlayerList feature");
	RegisterEvent(this);
}