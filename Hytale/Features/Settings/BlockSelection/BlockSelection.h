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

struct BlockResult {
	uint64_t hashedName;
	std::string category;

	BlockResult(uint64_t hashedName, std::string category) {
		this->hashedName = hashedName;
		this->category = category;
	}
};

