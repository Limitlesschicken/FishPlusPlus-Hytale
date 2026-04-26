/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"

class NoFall : public Feature {
public:
	NoFall() : Feature("Nofall") {};

	//void PlayerMove(MoveCycleEvent& event) override;
	bool CanExecute() override {
		return Util::IsInGame();
	}
	void Initialize() override;
	void OnMoveCycle(DefaultMovementController* dmc, Vector3& offset);
};