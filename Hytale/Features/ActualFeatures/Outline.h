/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"

#include "Features/Settings/ToggleSetting.h"
#include "Features/Settings/SliderSetting.h"
#include "Features/Settings/ColorSetting.h"
#include "Features/Settings/RecursiveSetting.h"

class Outline : public Feature {
public:
	Outline();

	bool CanExecute() override {
		return Util::IsInGame();
	}

	RecursiveSetting* entities;
	ToggleSetting* entitiesWave;
	ToggleSetting* entitiesGlow;
	SliderSetting* entitiesGlowSize;
	ColorSetting* entitiesColor;

	RecursiveSetting* items;
	ToggleSetting* itemsWave;
	ToggleSetting* itemsGlow;
	SliderSetting* itemsGlowSize;
	ColorSetting* itemsColor;
};