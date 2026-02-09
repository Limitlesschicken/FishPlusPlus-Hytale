#pragma once

#include "../FeatureDispatcher/FeatureDispatcher.h"

class Speed : public Feature {
public:
	Speed() : Feature("Speed") {};
private:
	void PlayerMove(MoveCycleEvent& event) override;
	bool CanExecute() override;
};