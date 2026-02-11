#include "Speed.h"

#include <Windows.h>
#include <cmath>
#include <iostream>

#include "../Util/Util.h"


#include "Util/InputSystem.h"

Speed::Speed() : Feature("Speed") {
	this->mode = this->RegisterSetting<MultiSetting>("Mode", std::vector<std::string>{"Multiplier", "Velocity"}, 0);
	this->speed = this->RegisterSetting<SliderSetting>("Speed", 1.0f, 0.0f, 5.0f);
}

void Speed::PlayerMove(MoveCycleEvent& event) {

	if (mode->GetValue() == 1) {
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

		if (InputSystem::IsKeyHeld(SDL_SCANCODE_W))
			event.wishDirection += Vector3(forwardX * this->speed->GetValue(), event.wishDirection.y, forwardZ * this->speed->GetValue());
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_S))
			event.wishDirection += Vector3(-forwardX * this->speed->GetValue(), event.wishDirection.y, -forwardZ * this->speed->GetValue());
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_A))
			event.wishDirection += Vector3(strafeX * this->speed->GetValue(), event.wishDirection.y, strafeZ * this->speed->GetValue());
		if (InputSystem::IsKeyHeld(SDL_SCANCODE_D))
			event.wishDirection += Vector3(-strafeX * this->speed->GetValue(), event.wishDirection.y, -strafeZ * this->speed->GetValue());
	}
	else {
		event.dmc.SpeedMultiplier = this->speed->GetValue();
	}
}

bool Speed::CanExecute() {
	return Util::getLocalPlayer() != nullptr;
}