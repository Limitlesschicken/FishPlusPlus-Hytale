/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Feature.h"

#include "Menu/Children/HudElement.h"


class HudFeature : public Feature {
public:
	HudFeature(std::string name) : Feature(name) {
		std::unique_ptr<HudElement> element = std::make_unique<HudElement>(this);
		this->element = element.get();
		element->SetSize(100, 100);
		Menu::hudComponent->AddChild(std::move(element));
	}

	HudElement* GetElement() {
		return element;
	}

private:
	HudElement* element;
};