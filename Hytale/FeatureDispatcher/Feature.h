#pragma once

#include <string>

#include "../sdk/DefaultMovementController.h"
#include "../Renderer/Renderer3D.h"

#include "Events/MoveCycleEvent.h"
#include "Events/Render3DEvent.h"
#include "Events/Render2DEvent.h"

class Feature {
public:
	Feature(std::string name);

	virtual void OnTick();
	virtual void PlayerMove(MoveCycleEvent& event);
	virtual void OnRender3D(Render3DEvent& event);
	virtual void OnRender2D(Render2DEvent& event);

	virtual bool CanExecute();
	virtual void OnActivate();
	virtual void OnDeactivate();

	void OnToggle();
	void setActive(bool active);

	std::string getName();
	std::string getCategory();
	void setCategory(std::string category);
	bool active;

private:
	std::string m_name;
	std::string m_category;

};