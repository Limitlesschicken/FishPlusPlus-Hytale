/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "core.h"

class RecursiveBody;

class RecursiveButton : public SettingButton {
public:
	RecursiveButton(Setting<bool>* setting);

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);

private:
	RecursiveBody* body;
};

class RecursiveBody : public Component {
public:
	RecursiveBody(RecursiveButton* parent);
	

	void Update(float mouseX, float mouseY) override;

	bool open = false;
private:

	RecursiveButton* parent;
};