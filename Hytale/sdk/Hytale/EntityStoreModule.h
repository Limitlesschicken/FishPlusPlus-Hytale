/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

struct EntityStoreModule {
	char pad_0000[0x68]; // 0x0000
	Array<Entity*>* entityArray; // 0x0068
	char pad_0068[144]; //0x0070
	int entityCount; // 0x0108
};