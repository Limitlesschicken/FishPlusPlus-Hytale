/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include <cstdint>

struct InventoryModule {
	char pad_0000[24]; //0x0000
	void* StorageInventory; //0x0018
	void* ArmorInventory; //0x0020
	void* HotbarInventory; //0x0028
	void* ConsumableInventory; //0x0030
	void* ToolsInventory; //0x0038
	char pad_0040[8]; //0x0040
	int32_t HotbarActiveSlot; //0x0048
	int32_t ConsumableActiveSlot; //0x004C
	int32_t ToolsActiveSlot; //0x0050
	float dropBindingHeldTick; //0x0054
	float inToolsMenuTimer; //0x0058
	bool hasDroppedStack; //0x005C
	char pad_005D[295]; //0x005D

};