/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/HudFeature.h"

#include "../../Settings/ToggleSetting.h"

class PlayerList : public HudFeature {
public:
	PlayerList();
	void OnRender2D();

	void Initialize() override;

	ToggleSetting* distance;
};