/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>

struct BlockSelection {
	std::string name;
	std::string category;

	BlockSelection(std::string name, std::string category) {
		this->name = name;
		this->category = category;
	}
};
