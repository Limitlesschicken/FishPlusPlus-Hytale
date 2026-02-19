/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "NoFall.h"

#include "core.h"


void NoFall::PlayerMove(MoveCycleEvent& event) {
	if (event.dmc.Velocity.y < -20.0f)
		event.dmc.Velocity.y = -20.0f;

	
}
bool NoFall::CanExecute() {
	ValidPtrBool(Util::getLocalPlayer());
}