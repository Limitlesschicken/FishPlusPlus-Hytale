/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"

class MiddleClickFriend : public Feature {
public:
	MiddleClickFriend() : Feature("Middle Click Friend") {};

	void OnFrame();

	void Initialize() override;
	bool CanExecute() override {
		return Util::IsInGame();
	}
};
