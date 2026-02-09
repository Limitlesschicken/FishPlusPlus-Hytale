#pragma once

#include <vector>
#include <memory>

#include "../Renderer/Mesh.h"


class Component {
public:
	Component(float x, float y, float width, float height);
	Component();

	virtual void Render();
	virtual void Update(float mouseX, float mouseY);

	bool IsHovered(float mouseX, float mouseY);
	virtual void MouseClicked(float mouseX, float mouseY, int virtualKeyCode);
	virtual void MouseReleased(float mouseX, float mouseY, int virtualKeyCode);
	virtual void MouseDragged(float mouseX, float mouseY, int virtualKeyCode, float deltaX, float deltaY);

	void MenuOpened();
	void MenuClosed();

	void Move(float xDelta, float yDelta);

	void AddChild(std::unique_ptr<Component> component);

	void setX(float x);
	void setY(float y);
	void setWidth(float width);
	void setHeight(float height);
	float getX();
	float getY();
	float getWidth();
	float getHeight();
private:

protected:
	std::vector<std::unique_ptr<Component>> m_children;

	float x;
	float y;
	float width;
	float height;
};