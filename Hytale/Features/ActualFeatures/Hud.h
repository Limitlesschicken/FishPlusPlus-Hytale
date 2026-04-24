/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"

class Hud : public Feature {
public:
	Hud() : Feature("Hud") {};
	void OnActivate() override;
};