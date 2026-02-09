#pragma once

#include "../Component.h"

#include "../../FeatureDispatcher/Feature.h"
#include "../../Renderer/Mesh.h"

class FeatureButton : public Component {
public:
	FeatureButton(Feature* feature);

	void Render() override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int virtualKeyCode) override;
private:
	Feature* feature;

	float m_hoverAlpha = 0;
	float m_textColorPercent = 0;

	bool m_hovered = false;
};