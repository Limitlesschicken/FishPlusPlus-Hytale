/*
 * Copyright (c) FishPlusPlus.
 */

#include <random>

#include "MainMenuEffect.h"

MainMenuEffect::MainMenuEffect() : Feature("Main Menu") {
	this->random = RegisterSetting<ToggleSetting>("Random", true);
	this->effectSelect = RegisterSetting<MultiSetting>("Effect", std::vector<std::string>(MainMenuEffectNames, MainMenuEffectNames + 8), 0);
};

void MainMenuEffect::OnFrame() {
	if (Util::app->Stage == AppStage::MainMenu) {
		if (!hasRefreshed) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(0, 7);
			currentEffect = static_cast<MainMenuEffects>(dist(gen));

			hasRefreshed = true;
		}
	}
	else {
		hasRefreshed = false;
	}

	if (!this->random->GetValue()) {
		currentEffect = static_cast<MainMenuEffects>(this->effectSelect->GetValue());
	}
}

void MainMenuEffect::Initialize() {
	RegisterEvent(this);
}
