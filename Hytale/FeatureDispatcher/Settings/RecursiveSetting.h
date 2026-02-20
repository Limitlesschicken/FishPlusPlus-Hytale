/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "core.h"
#include "../Setting.h"
#include "Menu/Children/RecursiveButton.h"

class RecursiveSetting : public Setting<bool> {
public:
	RecursiveSetting(std::string name, bool defaultValue) : Setting(name, defaultValue) {}

	void Toggle() {
		this->SetValue(!GetValue());
	}

	std::unique_ptr<Component> CreateButton() override {
		auto btn = std::make_unique<RecursiveButton>(this);
		btn->setting = this;
		return btn;
	}

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

private:

	std::vector<std::unique_ptr<ISetting>> m_settings;
};