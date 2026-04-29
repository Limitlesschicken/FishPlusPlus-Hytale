/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Feature.h"
#include "HudFeature.h"

namespace FeatureHandler {
	inline std::vector<std::unique_ptr<Feature>> features;

	void Init();

	template<typename TreatAs = Feature>
		requires std::derived_from<TreatAs,Feature>
	TreatAs* GetFeatureFromName(std::string_view name) {
		for (auto& feature : features) {
			if (feature->GetName()._Equal(name.data())) {
				if constexpr (std::is_same_v<TreatAs, Feature>) {
					return feature.get();
				}
				return reinterpret_cast<TreatAs*>(feature.get());
			}
		}
		return nullptr;
	}

	bool FeaturesLoaded();
}
