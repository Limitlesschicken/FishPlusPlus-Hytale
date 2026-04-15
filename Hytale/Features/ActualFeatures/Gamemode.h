/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"
#include "sdk/GameInstance.h"

class Gamemode : public Feature {
public:
	Gamemode();

	void OnMoveCycle(DefaultMovementController* dmc, Vector3& offset);
	void OnActivate() override;
	void OnDeactivate() override;
	void Initialize() override;

	bool CanExecute() override;
};
