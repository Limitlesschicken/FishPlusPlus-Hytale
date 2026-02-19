/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>

#include "Menu/Children/SettingButton.h"

class ISetting {
public:
	virtual ~ISetting() = default;
	virtual std::string GetName() const = 0;
	virtual std::unique_ptr<Component> CreateButton() = 0;
};

template <typename T>
class Setting : public ISetting {
public:

	Setting(std::string name, T defaultValue) {
		this->name = name;
		this->defaultValue = defaultValue;
		this->value = defaultValue;
	}

	void SetValue(T value) {
		this->value = value;
	}

	T GetValue() {
		return this->value;
	}

	std::string GetName() const override {
		return name;
	}


	T defaultValue;

private:
	T value;
	std::string name;
};