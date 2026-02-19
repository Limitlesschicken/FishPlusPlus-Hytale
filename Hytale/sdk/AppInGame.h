/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "GameInstance.h"

struct AppInGame {
	char pad[0x10];
	GameInstance* gameInstance;
	char pad_0018[72]; //0x0018
	int32_t IsInPause; //0x0060
	char pad_0064[8]; //0x0064
	bool IsInOverlay; //0x006C
	bool UnknownMouseCheck; //0x006D
	char pad_006E[6]; //0x006E

};
