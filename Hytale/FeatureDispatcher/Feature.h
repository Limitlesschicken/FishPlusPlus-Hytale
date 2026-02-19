/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>
#include <vector>

#include "Setting.h"

#include "Events/MoveCycleEvent.h"
#include "Events/Render3DEvent.h"
#include "Events/Render2DEvent.h"

#include "Util/InputSystem.h"

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

	void CreateKeybind();
	void OnToggle();
	void setActive(bool active);
	
	template<typename T, typename... Args>
	T* RegisterSetting(Args&&... args) {
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = ptr.get();
		m_settings.push_back(std::move(ptr));
		return raw;
	}
	
	std::vector<std::unique_ptr<ISetting>>& GetSettings() {
		return m_settings;
	}
	
	std::string getName();
	std::string getCategory();
	void setCategory(std::string category);
	bool active;
	SDL_Scancode keybind = SDL_SCANCODE_UNKNOWN;
private:
	std::string m_name;
	std::string m_category;

	std::vector<std::unique_ptr<ISetting>> m_settings;
};