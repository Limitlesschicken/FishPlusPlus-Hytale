/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include <cstddef>
#include <sdk/BaseDataTypes/HytaleString.h>

struct BuildInfo {
private:
	std::byte pad_0x10[0x10];
public:
	HytaleString* getArchitecture;
	HytaleString* getConfiguration;
	HytaleString* getVersion;
	HytaleString* getRevisionId;
};