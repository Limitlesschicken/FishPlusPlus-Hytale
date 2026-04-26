/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "sdk/Hytale/DefaultMovementController.h"
#include "Features/Feature.h"

#include "Features/Settings/SliderSetting.h"
#include "Features/Settings/MultiSetting.h"

class Speed : public Feature {
public:
	Speed();
	float GetSpeed() { return this->speed->GetValue(); }
	void OnMoveCycle(DefaultMovementController* dmc, Vector3& offset);
	bool CanExecute() override {
		return Util::IsInGame();
	}
	
	void Initialize() override;
	
	SliderSetting* speed;

};