/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Core.h"

struct EntityData {
	Entity* entityPtr;
	Entity::EntityType entityType;
	std::string name;
	Vector3 position;
	bool isLocalPlayer;
};

namespace SDK {
	inline std::mutex global_mutex;
	inline std::vector<EntityData> entities;

	extern void Main();
}
