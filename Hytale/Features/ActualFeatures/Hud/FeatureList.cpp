/*
 * Copyright (c) FishPlusPlus.
 */

#include "FeatureList.h"

#include <algorithm>

#include "Renderer/FontRenderer/Fonts.h"
#include "../../FeatureHandler.h"

void FeatureList::OnRender2D() {

	std::vector<std::string> list;

	for (const auto& feature : FeatureHandler::features) {
		if (!feature->IsActive())
			continue;

		if (feature->GetCategory() == "Hud")
			continue;

		list.push_back(feature->GetName());
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


	for (size_t i = 0; i < list.size(); i++) {
		if (this->GetElement()->GetX() + this->GetElement()->GetWidth() / 2 < Util::app->Engine->Window->WindowWidth / 2)
			Fonts::Figtree->RenderText(list[i], this->GetElement()->GetX() + 3, this->GetElement()->GetY() + 6 + (i * Fonts::Figtree->getHeight()), 1, Color::Normalize(Style::headerColor));
		else
			Fonts::Figtree->RenderText(
				list[i], this->GetElement()->GetX() + this->GetElement()->GetWidth() - Fonts::Figtree->getWidth(list[i]) - 3,
				this->GetElement()->GetY() + 6 + (i * Fonts::Figtree->getHeight()),
				1, Color::Normalize(Style::headerColor));
	}

	this->GetElement()->SetSize(Fonts::Figtree->getWidth(isOnTop ? list[0] : list.back()) + 6, Fonts::Figtree->getHeight() * list.size() + 6);
}

void FeatureList::Initialize() {
	Util::log("Initialized FeatureList feature");
	RegisterEvent(this);
}