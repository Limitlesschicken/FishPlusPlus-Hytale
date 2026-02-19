/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Component.h"

#include "../../FeatureDispatcher/Feature.h"

class FeatureButton;

class SettingsBody : public Component {
public:
	SettingsBody(FeatureButton* featureButton, Feature* feature);

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int virtualKeyCode) override;

	void ToggleOpen() {
		m_open = !m_open;
	}

	bool IsOpened() {
		return m_open;
	}
private:

	FeatureButton* featureButton;
	bool m_open = false;

	float m_wantedHeight = 0.0f;
};

class FeatureButton : public Component {
public:
	FeatureButton(Feature* feature);

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int virtualKeyCode) override;

	float GetBodyHeight() {
		return m_bodyHeight;
	}
private:
	Feature* feature;
	SettingsBody* m_body;

	float m_hoverAlpha = 0;
	float m_textColorPercent = 0;
	float m_bodyHeight = 0;

	bool m_hovered = false;

	friend class SettingsBody;
};