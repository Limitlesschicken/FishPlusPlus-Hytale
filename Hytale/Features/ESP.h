#pragma once

#include "../FeatureDispatcher/Feature.h"

class ESP : public Feature {
public:
	ESP() : Feature("ESP") {};
private:
	
	void OnRender3D(Render3DEvent& renderer3D);

	bool CanExecute() override;
};