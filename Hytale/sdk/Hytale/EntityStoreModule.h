/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

struct EntityStoreModule {
	char pad_0000[0x60]; // 0x0000
	Array<Entity*>* entityArray; // 0x0060
	char pad_0060[144]; //0x0068
	int entityCount; // 0x00F8
};