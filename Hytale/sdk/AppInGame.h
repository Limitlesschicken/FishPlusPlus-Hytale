#pragma once
#include "GameInstance.h"

struct AppInGame {
	char pad[0x10];
	GameInstance* gameInstance;
};
