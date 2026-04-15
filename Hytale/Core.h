/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "GLAD/glad.h"

#include <iostream>
#include <memory>
#include <Windows.h>
#include <algorithm>
#include <mutex>
#include <psapi.h>
#include <winnt.h>

#include "sdk/App.h"
#include "sdk/DefaultMovementController.h"
#include "sdk/OptionsHelper.h"
#include "sdk/Paths.h"

#include "Events/EventSystem.h"

#include "external/minhook/minhook.h"
#include "Util/InputSystem.h"

#include "Menu/Menu.h"
#include "Menu/Style.h"

#include "Util/Util.h"

#include "sdk/SDK.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shaders.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/FontRenderer/Fonts.h"
#include "Renderer/FramebufferRenderer/FramebufferRenderer.h"

#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"

// Helper macros for validating pointers
// These macros check if a pointer is valid before proceeding with the function. If the pointer is invalid, they return an appropriate value (nullptr, void, false, empty, or continue the loop). This helps prevent crashes due to null or invalid pointers.
#define ValidPtr(ptr) if (!Util::IsValidPtr(ptr)) return nullptr;
#define ValidPtrVoid(ptr) if (!Util::IsValidPtr(ptr)) return;
#define ValidPtrBool(ptr) if (!Util::IsValidPtr(ptr)) return false;
#define ValidPtrEmpty(ptr) if (!Util::IsValidPtr(ptr)) return {};
#define ValidPtrLoop(ptr) if (!Util::IsValidPtr(ptr)) continue;

inline uint64_t gameBase;
inline uint64_t dllBase;
inline uint64_t dllBaseEnd;
inline bool uninjecting = false;

namespace Globals {
	inline OptionsHelper* optionsHelper;
	inline Paths* paths;
}

namespace SM { // Signature Methods
	inline uint64_t SetCursorHiddenAddress = 0;
	inline uint64_t UpdateInputStatesAddress = 0;
	inline uint64_t GCToEEInterface_CreateThreadAddress = 0;
	inline uint64_t Object_ToStringAddress = 0;
	inline uint64_t ConcurrentDictionary_TryGetValueAddress = 0;

	inline uint64_t beginGLContextAddress = 0;
	inline uint64_t endGLContextAddress = 0;
	inline uint64_t renderQueueFlushAddress = 0;
	inline uint64_t submitDrawCommandsAddress = 0;
	inline uint64_t g_UniformManagerAddress = 0;
	inline uint64_t g_BufferManagerAddress = 0;
	inline uint64_t g_GlobalStateTableAddress = 0;
}