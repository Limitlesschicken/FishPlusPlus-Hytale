#pragma once

#include "../FeatureDispatcher/FeatureDispatcher.h"

#include "FeatureDispatcher/Settings/SliderSetting.h"
#include "FeatureDispatcher/Settings/MultiSetting.h"

class Speed : public Feature {
public:
	Speed();
private:
	void PlayerMove(MoveCycleEvent& event) override;
	bool CanExecute() override;

	MultiSetting* mode;
	SliderSetting* speed;
};