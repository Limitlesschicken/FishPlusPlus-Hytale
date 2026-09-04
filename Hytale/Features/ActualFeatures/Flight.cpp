/*
 * Copyright (c) FishPlusPlus.
 */
#include "Flight.h"

#include "core.h"

Flight::Flight() : Feature("Flight") {
	this->mode = this->RegisterSetting<MultiSetting>("Mode", std::vector<std::string>{"Creative", "Velocity"}, 0);
	this->speed = this->RegisterSetting<SliderSetting>("Speed", 1.0f, 0.0f, 5.0f);
}

void Flight::OnMoveCycle(DefaultMovementController* dmc, Vector3& offset) {
	if (mode->GetValue() == 1) {
		dmc->clientMovementStates.IsFlying = false;
		float yawRad = Util::getLocalPlayer()->yawRad;
		float forwardX = -sin(yawRad);
		float forwardZ = -cos(yawRad);

		float strafeX = forwardZ;
		float strafeZ = -forwardX;

		dmc->Velocity = 0.0f;
		offset = 0.0f;
		Vector3 move = Vector3(0.0f, 0.0f, 0.0f);
		//dmc->clientMovementStates.IsSwimming = true; Bypass for Fringe anarchy

		if (!Util::ShouldInteractWithGame())
			return;

		if (InputSystem::IsKeyHeld(SDL_SCANCODE_W))
			move += Vector3(forwardX, 0.0f, forwardZ);
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_S)) 
			move += Vector3(-forwardX, 0.0f, -forwardZ);
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_A))
			move += Vector3(strafeX, 0.0f, strafeZ);
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_D))
			move += Vector3(-strafeX, 0.0f, -strafeZ);

		if (InputSystem::IsKeyHeld(SDL_SCANCODE_SPACE))
			offset.y = speed->GetValue();
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_LSHIFT))
			offset.y = -speed->GetValue();

		move = move.normalized();

		offset.x = move.x * speed->GetValue();
		offset.z = move.z * speed->GetValue();


	} else {
		dmc->clientMovementStates.IsFlying = true;
	}
}

void Flight::OnDeactivate() {
	DefaultMovementController* dmc = Util::GetMovementController();
	ValidPtrVoid(dmc);
	dmc->clientMovementStates.IsFlying = false;
}

void Flight::Initialize() {
	Util::log("Initialized Flight feature");
	RegisterEvent(this);
}