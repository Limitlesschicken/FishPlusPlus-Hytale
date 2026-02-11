#pragma once

#include "../Component.h"

class ISetting;

class SettingButton : public Component {
public:

	SettingButton(ISetting* setting) {
		this->setting = setting;
	}
	bool m_hovered = false;
	float m_hoverAlpha = 0.0f;
	float m_activePercent = 0.0f;
	float bodyHeight = 0.0f;
	ISetting* setting;
};