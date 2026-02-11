#pragma once

#include <string>
#include <vector>

#include "FeatureDispatcher/Setting.h"
#include "Menu/Children/MultiButton.h"

class MultiSetting : public Setting<int> {
public:
	MultiSetting(std::string name, std::vector<std::string> options, int defaultValue) : Setting(name, defaultValue) {
		this->options = options;
	}

	void Increment() {
		this->SetValue((this->GetValue() + 1) % (int)options.size());
	}

	void Decrement() {
		this->SetValue((this->GetValue() - 1 + options.size()) % (int)options.size());
	}

	std::string GetValueName() {
		return options.at(this->GetValue());
	}

	std::unique_ptr<Component> CreateButton() override {
		auto btn = std::make_unique<MultiButton>(this);
		btn->setting = this;
		return btn;
	}

private:
	std::vector<std::string> options;
};