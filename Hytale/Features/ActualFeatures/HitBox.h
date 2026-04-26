/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Features/Feature.h"
#include "Features/Settings/ToggleSetting.h"

#include <unordered_map>


class HitBox : public Feature {
public:
	HitBox();

	void OnFrame();

	void OnDeactivate() override;
	void Initialize() override;
	bool CanExecute() override {
		return Util::IsInGame();
	}
private:
	ToggleSetting* players;
	ToggleSetting* mobs;

	std::unordered_map<Entity*, BoundingBox> entityHotboxSave;
};