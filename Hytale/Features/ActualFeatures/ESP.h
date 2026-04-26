/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Features/Feature.h"

#include "Features/Settings/ToggleSetting.h"
#include "Features/Settings/ColorSetting.h"
#include "Features/Settings/RecursiveSetting.h"

class ESP : public Feature {
public:
	ESP();
	void OnRender3D(Renderer3D& renderer3D);
	bool CanExecute() override {
		return Util::IsInGame();
	}
	void Initialize() override;

	ToggleSetting* toggle;
	ColorSetting* insideColor;
	ColorSetting* outsideColor;

	RecursiveSetting* testRecursive;

	ColorSetting* colorUnderRecrusive;
};