/*
 * Copyright (c) FishPlusPlus.
 */
#include "Component.h"

Component::Component(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
Component::Component() {
	this->x = 0;
	this->y = 0;
	this->width = 0;
	this->height = 0;
}

void Component::Render(double deltaTime) {
	for (auto& children : m_children) {
		children->Render(deltaTime);
	}
}
void Component::Update(float mouseX, float mouseY) {
	for (auto& children : m_children) {
		children->Update(mouseX, mouseY);
	}
}

bool Component::IsHovered(float mouseX, float mouseY) {
	for (auto& children : m_children) {
		children->IsHovered(mouseX, mouseY);
	}

	return mouseX >= x &&
		mouseX < x + width &&
		mouseY >= y &&
		mouseY < y + height;
}

void Component::MouseClicked(float mouseX, float mouseY, int virtualKeyCode) {
	for (auto& children : m_children) {
		children->MouseClicked(mouseX, mouseY, virtualKeyCode);
	}
}

void Component::MouseReleased(float mouseX, float mouseY, int virtualKeyCode) {
	for (auto& children : m_children) {
		children->MouseReleased(mouseX, mouseY, virtualKeyCode);
	}
}

void Component::MouseDragged(float mouseX, float mouseY, int virtualKeyCode, float deltaX, float deltaY) {
	for (auto& children : m_children) {
		children->MouseDragged(mouseX, mouseY, virtualKeyCode, deltaX, deltaY);
	}
}

void Component::MenuOpened() {
	for (auto& children : m_children) {
		children->MenuOpened();
	}
}
void Component::MenuClosed() {
	for (auto& children : m_children) {
		children->MenuClosed();
	}
}

void Component::Move(float xDelta, float yDelta) {
	x += xDelta;
	y += yDelta;
}

void Component::AddChild(std::unique_ptr<Component> component) {
	m_children.push_back(std::move(component));
}

void Component::SetX(float x) {
	this->x = x;
}
void Component::SetY(float y) {
	this->y = y;
}
void Component::SetWidth(float width) {
	this->width = width;
}
void Component::SetHeight(float height) {
	this->height = height;
}
float Component::GetX() {
	return x;
}
float Component::GetY() {
	return y;
}
float Component::GetWidth() {
	return width;
}
float Component::GetHeight() {
	return height;
}