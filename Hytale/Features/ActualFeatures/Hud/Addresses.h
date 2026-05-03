/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/HudFeature.h"


class Addresses : public HudFeature {
public:
	Addresses() : HudFeature("Addresses") {};
	void OnRender2D();

	void Initialize() override;
};