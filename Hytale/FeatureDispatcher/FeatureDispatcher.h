/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <vector>
#include <memory>

#include "Event.h"
#include "Feature.h"

namespace FeatureDispatcher {
extern std::vector<std::unique_ptr<Feature>> features;

void initFeatures();

void TickAll();

void DispatchEvent(Event& event);
}