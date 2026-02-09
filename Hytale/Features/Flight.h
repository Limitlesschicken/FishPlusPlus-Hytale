#pragma once


#include "../FeatureDispatcher/Feature.h"
#include "../sdk/AppInGame.h"

class Flight : public Feature {
public:
	Flight() : Feature("Flight") {};
private:
	void PlayerMove(MoveCycleEvent& event) override;
	void OnDeactivate() override;
	bool CanExecute() override;
};