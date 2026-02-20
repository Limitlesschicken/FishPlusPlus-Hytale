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
	virtual void onEnable();
	virtual void onDisable();
	
	template<typename T, typename... Args>
	T* RegisterSetting(Args&&... args) {
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = ptr.get();
		m_settings.push_back(std::move(ptr));
		return raw;
	}

	[[nodiscard]] const std::string& getName() const { return m_name; }
	[[nodiscard]] const std::string& getCategory() const { return m_category; }
	[[nodiscard]] const std::vector<std::unique_ptr<ISetting>>& getSettings() const { return m_settings; }
	[[nodiscard]] SDL_Scancode getKeybind() const { return keybind; }
	[[nodiscard]] bool isEnabled() const { return enabled; }

	void setEnabled(bool state) {
		enabled = state;
		if (enabled) {
			onEnable();
		}
		else {
			onDisable();
		}
	}

	void setKeybind(SDL_Scancode newKey) { keybind = newKey; }
	void toggleState() {
		setEnabled(!enabled);
	}

	void setCategory(std::string category);
private:
	bool enabled;
	SDL_Scancode keybind = SDL_SCANCODE_UNKNOWN;
	std::string m_name;
	std::string m_category;

	std::vector<std::unique_ptr<ISetting>> m_settings;
};