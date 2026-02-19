/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "../Event.h"

class Render2DEvent : public Event {
public:

	Render2DEvent() {}

	std::string_view GetName() const override {
		return "Render2DEvent";
	}

	Event::EventType GetEventType() const override {
		return Event::EventType::Render2DEvent;
	}
};