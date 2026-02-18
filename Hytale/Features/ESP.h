#pragma once

#include "../FeatureDispatcher/Feature.h"

#include "FeatureDispatcher/Settings/ToggleSetting.h"

class ESP : public Feature {
public:
	ESP();
private:
	
	void OnRender3D(Render3DEvent& renderer3D);

	bool CanExecute() override;

	ToggleSetting* toggle;
};