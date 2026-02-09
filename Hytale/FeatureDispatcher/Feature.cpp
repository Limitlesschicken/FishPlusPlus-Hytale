#include "Feature.h"

#include "../Renderer/Renderer3D.h"

Feature::Feature(std::string name) {
	m_name = name;
	m_category = "";
	active = false;
}

void Feature::OnTick() {}
void Feature::PlayerMove(MoveCycleEvent& event) {}
void Feature::OnRender3D(Render3DEvent& event) {};
void Feature::OnRender2D(Render2DEvent& event) {};

bool Feature::CanExecute() { return true; }
void Feature::OnActivate() {}
void Feature::OnDeactivate() {}

void Feature::OnToggle() {
	if (active && CanExecute()) {
		OnActivate();
	}
	else if (!active && CanExecute()) {
		OnDeactivate();
	}
}
void Feature::setActive(bool active) {
	if (this->active == active)
		return;

	this->active = active;
	OnToggle();
}

std::string Feature::getName() {
	return m_name;
}

std::string Feature::getCategory() {
	return m_category;
}
void Feature::setCategory(std::string category) {
	m_category = category;
}
