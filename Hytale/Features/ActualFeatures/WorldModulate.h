/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"

#include "Features/Settings/SliderSetting.h"
#include "Features/Settings/ToggleSetting.h"
#include "Features/Settings/ColorSetting.h"
#include "Features/Settings/RecursiveSetting.h"

class WorldModulate : public Feature {
public:
	WorldModulate();
	bool CanExecute() override {
		return Util::IsInGame();
	}
	void Initialize() override;

	RecursiveSetting* fogChanger;
	SliderSetting* fogStart;
	SliderSetting* fogEnd;
	ToggleSetting* noFog;
	ColorSetting* fogColor;
	RecursiveSetting* timeChanger;
	SliderSetting* time;
};