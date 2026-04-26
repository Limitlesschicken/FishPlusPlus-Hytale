/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"
#include "Features/Settings/ToggleSetting.h"
#include "Features/Settings/ColorSetting.h"

class Tracers : public Feature {
public:
	Tracers();

	void OnRender3D(Renderer3D& renderer3D);

	bool CanExecute() override {
		return Util::IsInGame();
	}

	void Initialize() override;

	ToggleSetting* players;
	ToggleSetting* mobs;
	ColorSetting* color;

private:
};