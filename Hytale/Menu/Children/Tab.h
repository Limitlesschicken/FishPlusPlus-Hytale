/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>
#include <memory>

#include "../Component.h"


class Tab : public Component {
public:
	Tab(std::string name, float x, float y);

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;
	void MouseClicked(float mouseX, float mouseY, int vk);
	void MouseReleased(float mouseX, float mouseY, int vk);
	void MouseDragged(float mouseX, float mouseY, int vk, float deltaX, float deltaY) override;
	void DrawHeader();
private:
	std::string name;

	bool m_dragging;

	friend class Body;
};

class Body : public Component {
public:
	Body(Tab* tab);
	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;
private:
	Tab* tab;
};