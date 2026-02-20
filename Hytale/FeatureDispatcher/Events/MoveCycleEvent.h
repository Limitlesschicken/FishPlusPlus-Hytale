/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string_view>

#include "sdk/DefaultMovementController.h"
#include "Math/Vector3.h"

#include "../Event.h"

class MoveCycleEvent : public Event {
public:

	MoveCycleEvent(DefaultMovementController& dmc, Vector3& wishDirection) : dmc(dmc), wishDirection(wishDirection) {}

	std::string_view GetName() const override {
		return "MoveCycleEvent";
	}

	Event::EventType GetEventType() const override {
		return Event::EventType::MoveCycleEvent;
	}
		

	DefaultMovementController& dmc;
	Vector3& wishDirection;
};