/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <vector>
#include <memory>

class Component {
public:
	Component(float x, float y, float width, float height);
	Component();

	virtual void Render(double deltaTime);
	virtual void Update(float mouseX, float mouseY);

	bool IsHovered(float mouseX, float mouseY);
	virtual void MouseClicked(float mouseX, float mouseY, int virtualKeyCode);
	virtual void MouseReleased(float mouseX, float mouseY, int virtualKeyCode);
	virtual void MouseDragged(float mouseX, float mouseY, int virtualKeyCode, float deltaX, float deltaY);

	void MenuOpened();
	void MenuClosed();

	void Move(float xDelta, float yDelta);

	void AddChild(std::unique_ptr<Component> component);

	void SetX(float x);
	void SetY(float y);
	void SetWidth(float width);
	void SetHeight(float height);
	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();
private:

protected:
	std::vector<std::unique_ptr<Component>> m_children;

	float x;
	float y;
	float width;
	float height;
};