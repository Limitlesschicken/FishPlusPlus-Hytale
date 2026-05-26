#pragma once

#include "Features/Feature.h"

class TestFeature : public Feature {
public:
	TestFeature() : Feature("TestFeature") {};

	void OnFrame();
	void OnActivate() override;

	void Initialize() override;
	bool CanExecute() override {
		return Util::IsInGame();
	}
};