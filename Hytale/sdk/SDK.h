/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Core.h"

struct EntityData {
	Entity* entityPtr;
	int networkID;
	bool player;
	Entity::EntityType entityType;
	std::string name;
	Vector3 position;
	bool isLocalPlayer;
};

struct FilteredBlockResult {
	Vector3 position;
	int blockId;
	std::string displayName;
	Color color;
};

namespace SDK {
	inline std::mutex global_mutex;
	inline std::vector<EntityData> entities;
	inline std::mutex filteredBlockMutex;
	inline bool filterInitialized = false;
	inline std::vector<FilteredBlockResult> filteredBlocks;

	void ScanForBlocks();

	extern void Reset();
	extern void Main();
}
