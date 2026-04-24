/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/HudFeature.h"

class FeatureList : public HudFeature {
public:
	FeatureList() : HudFeature("FeatureList") {};
	void OnRender2D();

	void Initialize() override;
};