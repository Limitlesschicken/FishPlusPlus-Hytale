/*
 * Copyright (c) FishPlusPlus.
 */
#include "Speed.h"
#include "core.h"

#include "Events/EventRegister.h"

Speed::Speed() : Feature("Speed") {
	this->speed = this->RegisterSetting<SliderSetting>("Speed", 1.0f, 0.0f, 5.0f);
}

void Speed::OnMoveCycle(DefaultMovementController* dmc, Vector3& offset) {
    dmc->SpeedMultiplier = 1.0f;
    float yawRad = Util::getLocalPlayer()->yawRad;
    float forwardX = -sin(yawRad);
    float forwardZ = -cos(yawRad);

    float strafeX = forwardZ;
    float strafeZ = -forwardX;

    dmc->Velocity.x = 0.0f;
    dmc->Velocity.z = 0.0f;
    offset.x = 0.0f;
    offset.z = 0.0f;

    Vector3 move = Vector3(0.0f, 0.0f, 0.0f);

    float currentSpeed = this->speed->GetValue();

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

    move = move.normalized();

    offset.x = move.x * speed->GetValue();
    offset.z = move.z * speed->GetValue();
}

void Speed::Initialize() {
	Util::log("Initialized Speed feature");
    RegisterEvent(this);
}	
