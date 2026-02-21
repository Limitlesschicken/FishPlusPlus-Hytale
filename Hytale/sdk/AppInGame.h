/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "GameInstance.h"

#include "Memory/Memory.h"

struct AppInGame {
	char pad[0x10];
	GameInstance* gameInstance;
	char pad_0018[72]; //0x0018
	int32_t IsInPause; //0x0060
	char pad_0064[8]; //0x0064
	bool IsInOverlay; //0x006C
	bool UnknownMouseCheck; //0x006D
	char pad_006E[6]; //0x006E

	void UpdateInputStates(bool skipResetKeys) {
		typedef __int64(__thiscall* UpdateInputStates)(__int64 thisptr, char skipResetKeys);
		static auto oUpdateInputStates = reinterpret_cast<UpdateInputStates>(PatternScan("57 56 53 48 83 EC ? 48 8B D9 8B F2 48 8B 4B ? 48 85 C9 0F 84"));
		oUpdateInputStates((__int64)this, skipResetKeys);
	}
};
