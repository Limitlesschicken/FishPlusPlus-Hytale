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
	Bubble,
	Cells
};

const std::string MainMenuEffectNames[] = {
	"City",
	"Ghost",
	"Fractal",
	"Happy",
	"Sphere",
	"Hexagon",
	"Bubble",
	"Cells"
};

class MainMenuEffect : public Feature {
public:
	MainMenuEffect();

	void OnFrame();
	void Initialize() override;

	static inline MainMenuEffects currentEffect = Cells;
	static inline bool hasRefreshed = false;

	ToggleSetting* random;
	MultiSetting* effectSelect;
};