/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <vector>

#include "../../../Features/Feature.h"
#include "Menu/Children/BlockSelection/BlockSelectionButton.h"
#include "BlockSelection.h"

class BlockSetting : public Setting<std::vector<BlockResult>> {
public:
	BlockSetting(std::string name, std::string defaultCategory, std::vector<std::string> categories) : Setting(name, {}), defaultCategory(defaultCategory), categories(categories) {}

	std::vector<std::string> categories;
	std::string defaultCategory;

	std::unique_ptr<Component> CreateButton() override {
		auto btn = std::make_unique<BlockSelectionButton>(this);
		btn->setting = this;
		return btn;
	}
};