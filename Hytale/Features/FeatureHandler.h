/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Feature.h"
#include "HudFeature.h"

namespace FeatureHandler {
	inline std::vector<std::unique_ptr<Feature>> features;

	void Init();

	Feature* GetFeatureFromName(std::string name);
	bool FeaturesLoaded();
}
