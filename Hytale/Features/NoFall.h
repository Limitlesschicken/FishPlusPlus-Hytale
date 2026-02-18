#pragma once

#include "../FeatureDispatcher/Feature.h"

class NoFall : public Feature {
public:
	NoFall() : Feature("Nofall") {};

	void PlayerMove(MoveCycleEvent& event) override;
	bool CanExecute() override;
};