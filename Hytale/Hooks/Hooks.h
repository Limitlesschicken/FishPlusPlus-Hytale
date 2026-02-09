#pragma once

#include "Math/Vector3.h"

namespace Hooks {

	typedef __int64(__fastcall* DoMoveCycle)(__int64 thisptr, Vector3 offset);
	inline static DoMoveCycle oDoMoveCycle = nullptr;

	typedef __int64(__fastcall* SetUniformBuffers)(__int64 thisptr);
	inline static SetUniformBuffers oSetUniformBuffers = nullptr;

	typedef __int64(__fastcall* HandleScreenShotting)(__int64 a1);
	inline static HandleScreenShotting oHandleScreenShotting = nullptr;

	typedef __int64*(__fastcall* OnUserInput)(__int64 a1, int* a2);
	inline static OnUserInput oOnUserInput = nullptr;

	typedef __int64(__thiscall* SetCursorVisible)(void* thisptr, bool visible);
	inline static SetCursorVisible oSetCursorVisible = nullptr;

	typedef __int64(__thiscall* CalculateFrameTime)(__int64* thisptr, __int64 packet);
	inline static CalculateFrameTime oCalculateFrameTime = nullptr;

	bool CreateHooks();
}