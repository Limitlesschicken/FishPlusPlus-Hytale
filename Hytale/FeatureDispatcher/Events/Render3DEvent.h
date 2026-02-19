/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Event.h"

#include "Renderer/Renderer3D.h"

class Render3DEvent : public Event {
public:

	Render3DEvent(Renderer3D& renderer3D) : renderer3D(renderer3D) {}

	std::string_view GetName() const override {
		return "Render3DEvent";
	}

	Event::EventType GetEventType() const override {
		return Event::EventType::Render3DEvent;
	}

	Renderer3D& renderer3D;
};