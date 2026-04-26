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

	bool CanExecute() override {
		return Util::IsInGame();
	}

	void Initialize() override;

	ToggleSetting* distance;
};