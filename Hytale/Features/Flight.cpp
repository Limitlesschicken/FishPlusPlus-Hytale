#include "Flight.h"

#include <iostream>

#include "../Util/Util.h"

#include "Util/InputSystem.h"

Flight::Flight() : Feature("Flight") {
	this->mode = this->RegisterSetting<MultiSetting>("Mode", std::vector<std::string>{"Creative", "Velocity"}, 0);
	this->speed = this->RegisterSetting<SliderSetting>("Speed", 1.0f, 0.0f, 5.0f);
}

void Flight::PlayerMove(MoveCycleEvent& event) {
	if (mode->GetValue() == 1) {
		event.dmc.SpeedMultiplier = 1.0f;
		event.dmc.clientMovementStates.IsFlying = false;
		float yawRad = Util::getLocalPlayer()->yawRad;
		float forwardX = -sin(yawRad);
		float forwardZ = -cos(yawRad);

		float strafeX = forwardZ;
		float strafeZ = -forwardX;

		event.dmc.Velocity = 0.0f;
		event.wishDirection = 0.0f;
		
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_W))
			event.wishDirection += Vector3(forwardX * this->speed->GetValue(), event.wishDirection.y, forwardZ * this->speed->GetValue());
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_S)) 
			event.wishDirection += Vector3(-forwardX * this->speed->GetValue(), event.wishDirection.y, -forwardZ * this->speed->GetValue());
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_A))
			event.wishDirection += Vector3(strafeX * this->speed->GetValue(), event.wishDirection.y, strafeZ * this->speed->GetValue());
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_D))
			event.wishDirection += Vector3(-strafeX * this->speed->GetValue(), event.wishDirection.y, -strafeZ * this->speed->GetValue());
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_SPACE))
			event.wishDirection.y = speed->GetValue();
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_LSHIFT))
			event.wishDirection.y = -speed->GetValue();
	} else {
		event.dmc.SpeedMultiplier = this->speed->GetValue();
		event.dmc.clientMovementStates.IsFlying = true;
	}
}

void Flight::OnDeactivate() {
	Util::dmc->clientMovementStates.IsFlying = false;
}

bool Flight::CanExecute() {
	return Util::dmc != nullptr;
}