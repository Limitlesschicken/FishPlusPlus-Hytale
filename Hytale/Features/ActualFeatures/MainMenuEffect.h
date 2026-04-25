/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"

#include "Features/Settings/ToggleSetting.h"
#include "Features/Settings/MultiSetting.h"

enum MainMenuEffects {
	City,
	Ghost,
	Fractal,
	Happy,
	MobiusSphere,
	Hexagon,
	Bubble
};

const std::string MainMenuEffectNames[] = {
	"City",
	"Ghost",
	"Fractal",
	"Happy",
	"Sphere",
	"Hexagon",
	"Bubble"
};

class MainMenuEffect : public Feature {
public:
	MainMenuEffect();

	void OnFrame();
	void Initialize() override;

	static inline MainMenuEffects currentEffect = Bubble;
	static inline bool hasRefreshed = false;

	ToggleSetting* random;
	MultiSetting* effectSelect;
};