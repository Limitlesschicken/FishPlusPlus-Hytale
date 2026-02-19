/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "../Core.h"

#include "../FeatureDispatcher/Feature.h"

#include "FeatureDispatcher/Settings/ToggleSetting.h"
#include <FeatureDispatcher/Settings/ColorSetting.h>

class ESP : public Feature {
public:
	ESP();
private:
	
	void OnRender3D(Render3DEvent& renderer3D);

	bool CanExecute() override;

	ToggleSetting* toggle;
	ColorSetting* insideColor;
	ColorSetting* outsideColor;
};