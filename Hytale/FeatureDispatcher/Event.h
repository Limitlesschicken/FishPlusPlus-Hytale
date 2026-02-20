/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string_view>

class Event {
public:

	enum EventType {
		None,
		MoveCycleEvent,
		Render3DEvent,
		Render2DEvent
	};

	virtual ~Event() {};
	virtual EventType GetEventType() const = 0;
	virtual std::string_view GetName() const = 0;
};