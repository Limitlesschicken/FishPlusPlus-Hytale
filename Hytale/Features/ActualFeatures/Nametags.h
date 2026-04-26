/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Features/Feature.h"

#include "Features/Settings/ToggleSetting.h"

class Nametags : public Feature {
public:
	Nametags();
	void OnRender3D(Renderer3D& renderer3D);
	bool CanExecute() override {
		return Util::IsInGame();
	}
	void Initialize() override;

	ToggleSetting* NPCTags;
	
};