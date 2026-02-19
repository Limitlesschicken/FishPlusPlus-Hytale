/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once


#include "../FeatureDispatcher/Feature.h"
#include "../sdk/AppInGame.h"

#include "FeatureDispatcher/Settings/SliderSetting.h"
#include "FeatureDispatcher/Settings/MultiSetting.h"
#include "FeatureDispatcher/Settings/KeybindSetting.h"

class Flight : public Feature {
public:
	Flight();
private:
	void PlayerMove(MoveCycleEvent& event) override;
	void OnDeactivate() override;
	bool CanExecute() override;

	MultiSetting* mode;
	SliderSetting* speed;
};