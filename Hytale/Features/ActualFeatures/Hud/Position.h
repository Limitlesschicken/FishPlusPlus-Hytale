/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/HudFeature.h"

class PositionHud : public HudFeature {
public:
	PositionHud() : HudFeature("Position") {};
	void OnRender2D();

	bool CanExecute() override {
		return Util::IsInGame();
	}
	void Initialize() override;
};