#pragma once

#include <string>

#include "FeatureDispatcher/Setting.h"
#include "Menu/Children/KeybindButton.h"
#include "Util/InputSystem.h"
#include "../Feature.h"

class KeybindSetting : public Setting<SDL_Scancode> {
public:

	Feature* parentFeature = nullptr;
	bool isDefaultKeybind = false;

	KeybindSetting(std::string name, SDL_Scancode defaultValue) : Setting(name, defaultValue) {}

	//Dont use this in a module bc its for the module keybind
	KeybindSetting(std::string name, SDL_Scancode, Feature* parentFeature) : Setting(name, defaultValue) {
		this->parentFeature = parentFeature;
		this->isDefaultKeybind = true;
	}

	std::unique_ptr<Component> CreateButton() override {
		auto btn = std::make_unique<KeybindButton>(this);
		btn->setting = this;
		return btn;
	}
};