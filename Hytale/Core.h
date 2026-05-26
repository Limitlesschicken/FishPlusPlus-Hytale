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

inline uint64_t gameBase;
inline uint64_t dllBase;
inline uint64_t dllBaseEnd;
inline bool uninjecting = false;

#include "Util/SigManager.h"
#include "Util/NativeAOTAPI.h"
#include "Util/Hash.h"
#include "sdk/Hytale/App.h"
#include "sdk/Hytale/Input.h"
#include "sdk/Hytale/DefaultMovementController.h"
#include "sdk/Hytale/OptionsHelper.h"
#include "sdk/Hytale/Paths.h"
#include "sdk/Hytale/BuildInfo.h"

#include "Events/EventSystem.h"

#include "external/minhook/minhook.h"
#include "Util/InputSystem.h"

#include "Menu/Menu.h"
#include "Menu/Style.h"

#include "Util/Util.h"
#include "sdk/Packets/PacketRegistry.h"
#include "Util/Packet.h"
#include "sdk/SDK.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shaders.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/FontRenderer/Fonts.h"
#include "Renderer/FramebufferRenderer/FramebufferRenderer.h"

#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"

namespace Globals {
	inline OptionsHelper* optionsHelper;
	inline Paths* paths;
	inline BuildInfo* buildInfo;
}

// Helper macros for validating pointers
// These macros check if a pointer is valid before proceeding with the function. If the pointer is invalid, they return an appropriate value (nullptr, void, false, empty, or continue the loop). This helps prevent crashes due to null or invalid pointers.
#define ValidPtr(ptr) if (!Util::IsValidPtr(ptr)) return nullptr;
#define ValidPtrVoid(ptr) if (!Util::IsValidPtr(ptr)) return;
#define ValidPtrBool(ptr) if (!Util::IsValidPtr(ptr)) return false;
#define ValidPtrEmpty(ptr) if (!Util::IsValidPtr(ptr)) return {};
#define ValidPtrLoop(ptr) if (!Util::IsValidPtr(ptr)) continue;


void ClientInitialize(); //called from WglSwapBuffers to make sure glad is available
