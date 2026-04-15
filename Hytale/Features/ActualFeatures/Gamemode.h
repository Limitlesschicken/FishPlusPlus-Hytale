#pragma once

#include "Features/Feature.h"
#include "sdk/Hytale/GameInstance.h"

class Gamemode : public Feature {
public:
	Gamemode();

	void OnMoveCycle(DefaultMovementController* dmc, Vector3& offset);
	void OnActivate() override;
	void OnDeactivate() override;
	void Initialize() override;

	bool CanExecute() override;
};
