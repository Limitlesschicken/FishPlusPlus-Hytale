/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "../Core.h"

namespace EventRegister {
	inline Event<DefaultMovementController*, Vector3&> DoMoveCycleEvent;
	inline Event<Renderer3D&> Render3DEvent;
	inline Event<> Render2DEvent;
	inline Event<Object*, PacketIndex&, bool&> PacketRecieveEvent;
	inline Event<> FrameEvent;
	inline Event<Object*, int, bool&> PacketSendEvent;
}