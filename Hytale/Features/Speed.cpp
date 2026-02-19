/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#include "Speed.h"

#include <cmath>

#include "core.h"

Speed::Speed() : Feature("Speed") {
	this->speed = this->RegisterSetting<SliderSetting>("Speed", 1.0f, 0.0f, 5.0f);
}

void Speed::PlayerMove(MoveCycleEvent& event) {
	event.dmc.SpeedMultiplier = 1.0f;
	float yawRad = Util::getLocalPlayer()->yawRad;
	float forwardX = -sin(yawRad);
	float forwardZ = -cos(yawRad);

	float strafeX = forwardZ;
	float strafeZ = -forwardX;

	event.dmc.Velocity.x = 0.0f;
	event.dmc.Velocity.z = 0.0f;
	event.wishDirection.x = 0.0f;
	event.wishDirection.z = 0.0f;
	if (!Util::ShouldInteractWithGame())
		return;
	if (InputSystem::IsKeyHeld(SDL_SCANCODE_W))
		event.wishDirection += Vector3(forwardX * this->speed->GetValue(), event.wishDirection.y, forwardZ * this->speed->GetValue());
	if (InputSystem::IsKeyHeld(SDL_SCANCODE_S))
		event.wishDirection += Vector3(-forwardX * this->speed->GetValue(), event.wishDirection.y, -forwardZ * this->speed->GetValue());
	if (InputSystem::IsKeyHeld(SDL_SCANCODE_A))
		event.wishDirection += Vector3(strafeX * this->speed->GetValue(), event.wishDirection.y, strafeZ * this->speed->GetValue());
	if (InputSystem::IsKeyHeld(SDL_SCANCODE_D))
		event.wishDirection += Vector3(-strafeX * this->speed->GetValue(), event.wishDirection.y, -strafeZ * this->speed->GetValue());
}

bool Speed::CanExecute() {
	ValidPtrBool(Util::getLocalPlayer());
}