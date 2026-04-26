/*
 * Copyright (c) FishPlusPlus.
 */
#include "NoFall.h"

#include "core.h"

void NoFall::OnMoveCycle(DefaultMovementController* dmc, Vector3& offset) {
	if (dmc->Velocity.y < -20.0f)
		dmc->Velocity.y = -20.0f;
}

void NoFall::Initialize() {
	Util::log("Initialized Nofall feature");
	RegisterEvent(this);
}