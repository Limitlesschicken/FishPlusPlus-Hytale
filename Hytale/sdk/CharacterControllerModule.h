/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "DefaultMovementController.h"

struct CharacterControllerModule {
	char pad_0000[24]; //0x0000
	DefaultMovementController* MovementController; //0x0018
};