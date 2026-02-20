/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "../Core.h"
#include "../FeatureDispatcher/Feature.h"

class Nametags : public Feature {
public:
	Nametags() : Feature("Nametags") {};
private:

	void OnRender3D(Render3DEvent& renderer3D);

	bool CanExecute() override;
};