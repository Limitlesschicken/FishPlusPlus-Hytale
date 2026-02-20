/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Math/Vector3.h"

namespace Hooks {

	typedef __int64(__fastcall* DoMoveCycle)(__int64 thisptr, Vector3* offset);
	inline static DoMoveCycle oDoMoveCycle = nullptr;

	typedef __int64(__fastcall* SetUniformBuffers)(__int64 thisptr);
	inline static SetUniformBuffers oSetUniformBuffers = nullptr;

	typedef __int64(__fastcall* HandleScreenShotting)(__int64 a1);
	inline static HandleScreenShotting oHandleScreenShotting = nullptr;

	typedef __int64*(__fastcall* OnUserInput)(__int64 a1, int* a2);
	inline static OnUserInput oOnUserInput = nullptr;

	typedef __int64(__thiscall* SetCursorHidden)(__int64 thisptr, char hidden);
	inline static SetCursorHidden oSetCursorHidden = nullptr;

	typedef __int64(__thiscall* UpdateInputStates)(__int64 thisptr, char skipResetKeys);
	inline static UpdateInputStates oUpdateInputStates = nullptr;

	typedef void(__thiscall* WeatherUpdate)(__int64 thisptr, float deltaTime);
	inline static WeatherUpdate oWeatherUpdate = nullptr;

	typedef void(__thiscall* SetActiveHotbarSlot)(__int64 thisptr, unsigned int slot, bool triggerInteraction);
	inline static SetActiveHotbarSlot oSetActiveHotbarSlot = nullptr;

	bool CreateHooks();
}