/*
 * Copyright (c) FishPlusPlus.
 */

#include "Outline.h"

Outline::Outline() : Feature("Outline") {
	this->entities = this->RegisterSetting<RecursiveSetting>("Entities", true);
	this->entitiesWave = entities->RegisterSetting<ToggleSetting>("Wave", false);
	this->entitiesGlow = entities->RegisterSetting<ToggleSetting>("Glow", true);
	this->entitiesGlowSize = entities->RegisterSetting<SliderSetting>("Glow Size", 5.0f, 0.0f, 10.0f);
	this->entitiesColor = entities->RegisterSetting<ColorSetting>("Color", Color(50, 50, 255, 255));

	this->items = this->RegisterSetting<RecursiveSetting>("Items", true);
	this->itemsWave = items->RegisterSetting<ToggleSetting>("Wave", false);
	this->itemsGlow = items->RegisterSetting<ToggleSetting>("Glow", true);
	this->itemsGlowSize = items->RegisterSetting<SliderSetting>("Glow Size", 5.0f, 0.0f, 10.0f);
	this->itemsColor = items->RegisterSetting<ColorSetting>("Color", Color(50, 50, 255, 255));
}

bool Outline::CanExecute() {
	return Util::isFullyInitialized();
}
