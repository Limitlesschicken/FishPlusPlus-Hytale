/*
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

	void CreateForcedKeybind();
	
	template<typename T, typename... Args>
	T* RegisterSetting(Args&&... args) {
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = ptr.get();
		m_settings.push_back(std::move(ptr));
		return raw;
	}

	[[nodiscard]] const std::string& GetName() const { return m_name; }
	[[nodiscard]] const std::string& GetCategory() const { return m_category; }
	[[nodiscard]] const std::vector<std::unique_ptr<ISetting>>& GetSettings() const { return m_settings; }
	[[nodiscard]] SDL_Scancode GetKeybind() const { return keybind; }
	[[nodiscard]] bool IsActive() const { return active; }

	void SetKeybind(SDL_Scancode key) {this->keybind = key;}

	void setActive(bool active) {
		this->active = active;
		if (active) {
			OnActivate();
		}
		else {
			OnDeactivate();
		}
	}

	void ToggleState() {
		setActive(!active);
	}


	void setCategory(std::string category);
	
private:
	bool active;
	SDL_Scancode keybind = SDL_SCANCODE_UNKNOWN;
	std::string m_name;
	std::string m_category;

	std::vector<std::unique_ptr<ISetting>> m_settings;
};