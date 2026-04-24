/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/HudFeature.h"

class WaterMark : public HudFeature {
public:
	WaterMark() : HudFeature("WaterMark") {};
	void OnRender2D();

	void Initialize() override;
};