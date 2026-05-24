/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "sdk/BaseDataTypes/Object.h"
#include "Structs/IndependentStructs.h"

#pragma pack(push, 1)
struct MoveItemStack { // Doesent have a methodTable for some reason
	int fromSectionId;
	int fromSlotId;
	int quantity;
	int toSectionId;
	int toSlotId;
};
#pragma pack(pop)