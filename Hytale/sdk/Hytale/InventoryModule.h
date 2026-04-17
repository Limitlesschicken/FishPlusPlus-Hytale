/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include <cstdint>

struct InventoryModule {
	char pad_0000[0x10];			//0x0000
	void* gameInstance;				//0x0010
	void* _storageInventory;		//0x0018
	void* _armorInventory;			//0x0020
	void* HotbarInventory;			//0x0028
	void* UtilityInventory;			//0x0030
	void* ConsumableInventory;		//0x0038
	void* ToolsInventory;			//0x0040
	int HotbarActiveSlot;			//0x0048
	int ConsumableActiveSlot;		//0x004C
	int ToolsActiveSlot;			//0x0050
	float dropBindingHeldTick;		//0x0054
	float inToolsMenuTimer;			//0x0058
	bool hasDroppedStack;			//0x005C
};