#pragma once

#include <string>

template <typename T>
class Setting {
public:

	Setting(std::string name, T defaultValue) {
		this->name = name;
		this->defaultValue = defaultValue;
	}

	void SetValue(T value) {
		this->value = value;
	}

	void GetValue() {
		return this->value;
	}

	T defaultValue;

private:
	T value;
	std::string name;
};